<?php
/**
 * The declaration of MCL_TEMPLATE_MARKER_BIMS class.
 *
 */
class MCL_TEMPLATE_MARKER_BIMS extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'MARKER';
    parent::__construct($details);
  }

  /**
   * @see MCL_TEMPLATE::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
      return new self($parent->getMemberArr());
    }
  }

  /**
   * @see MCL_TEMPLATE::defineDescription()
   */
  public function defineDescription() {
    $desc = 'The description for marker sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'dataset_name'      => array('req' => FALSE, 'width' => 15, 'desc' => "Name of the phenotyping dataset (project). It should match a 'dataset_name' column entry in the 'Dataset' sheet."),
      'marker_name'       => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the marker."),
      'marker_type'       => array('req' => TRUE,  'width' => 10, 'desc' => "Marker type such as SSR, RFLP, RAPD, STS, SCAR, overgo probe and Isozyme. Refer to the available marker_type page for each database."),
      'genus'             => array('req' => TRUE,  'width' => 10, 'desc' => "The genus name of the organism from which the marker was developed."),
      'species'           => array('req' => TRUE,  'width' => 10, 'desc' => "The species name of the organism from which the marker was developed. Enter 'sp.' to represent one unknown species, 'spp.' to represent multiple unknown species."),
      'alias'             => array('req' => FALSE, 'width' => 10, 'desc' => "Other names used for the marker. Multiple aliases, separated by a semicolon (;), can be added."),
      'ss_id'             => array('req' => FALSE, 'width' => 10, 'desc' => "The submitted SNPs (SS) number."),
      'rs_id'             => array('req' => FALSE, 'width' => 10, 'desc' => "The refSNP (RS) number."),
      'comments'          => array('req' => FALSE, 'width' => 10, 'desc' => "Any comments about the marker."),
      '##property_name'   => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns (##) : followed by name (cvterm name) of the property."),
      '###property_name'  => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns (###) : followed by name (cvterm name) of the property. The propery is associated with the project."),
    );

    // Adds labels.
    $headers['ss_id']['label'] = 'SS_ID';
    $headers['rs_id']['label'] = 'RS_ID';
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['alias']            = -1;
    $cvterms['SITE_CV']['ss_id']            = -1;
    $cvterms['SITE_CV']['rs_id']            = -1;
    $cvterms['SITE_CV']['marker_type']      = -1;
    $cvterms['SITE_CV']['comments']         = -1;
    $cvterms['sequence']['genetic_marker']  = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Gets the job paremters.
    $program_id = $this->getJob()->getParamByKey('program_id');

    // Gets the prefix.
    $prefix = BIMS_PROGRAM::getPrefixByID($program_id);

    // Checks organism.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);

    // Checks dataset.
    if ($line['dataset_name']) {
      $bc_project = new BIMS_CHADO_PROJECT($program_id);
      $bc_project->checkProject($this, $prefix . $line['dataset_name']);
    }
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the job paremters.
    $program_id = $this->getJob()->getParamByKey('program_id');

    // Gets the prefix.
    $prefix = BIMS_PROGRAM::getPrefixByID($program_id);

    // Gets BIMS_CHADO tables.
    $bc_feature = new BIMS_CHADO_FEATURE($program_id);

    // Gets the organism.
    $organism = MCL_CHADO_ORGANISM::getOrganism($line['genus'], $line['species']);

    // Adds a marker.
    $details = array(
      'uniquename'  => $prefix . $line['marker_name'],
      'name'        => $line['marker_name'],
      'type_id'     => $this->cvterms['sequence']['genetic_marker'],
      'organism_id' => $organism->getOrganismID(),
    );
    $feature_id = $bc_feature->addFeature($this, $details);
    if ($feature_id) {
      $bc_feature->setFeatureID($feature_id);

      // Adds the properties.
      $bc_feature->addProp($this, 'SITE_CV', 'marker_type', $line['marker_type']);
      $bc_feature->addProp($this, 'SITE_CV', 'comments', $line['comments']);

      // Adds SS ID and RS ID.
      $bc_feature->addSynonyms($this, 'feature', 'SITE_CV', 'ss_id', $line['ss_id']);
      $bc_feature->addSynonyms($this, 'feature', 'SITE_CV', 'rs_id', $line['rs_id']);

      // Adds aliases.
      $bc_feature->addAlias($this, $line['alias'], "[,;]");

      // Adds the user-specific properties.
      $this->addSpecialColumns('##', $bc_feature, $line);

      // Adds the user-specific properties with the rank.
      if ($line['dataset_name']) {
        $bc_project = new BIMS_CHADO_PROJECT($program_id);
        $dataset = $bc_project->byTKey('project', array('name' => $prefix . $line['dataset_name']));
        $this->addSpecialColumnsRank('###', $bc_feature, $line, $dataset->project_id);
      }
    }
  }

  /**
   * @see MCL_TEMPLATE::postProcessECD()
   */
  public function postProcessECD() {

    // Lists the job paremters.
    $cv_arr = $this->getJob()->getParamByKey('cv_arr');

    // Saves the user-specific properties ('##').
    if (!$this->saveSpecialColumns('##', $cv_arr['marker']['cv_id'], $this->getHeaders())) {
      $this->updateMsg('E', "Failed to save the special columns");
      return FALSE;
    }

    // Saves the user-specific properties ('###').
    if (!$this->saveSpecialColumns('###', $cv_arr['marker']['cv_id'], $this->getHeaders())) {
      $this->updateMsg('E', "Failed to save the special columns ('###')");
      return FALSE;
    }
    return TRUE;
  }
}
