<?php
/**
 * The declaration of MCL_TEMPLATE_DELETE_FEATURE_BIMS class.
 *
 */
class MCL_TEMPLATE_DELETE_FEATURE_BIMS extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_UPLOAD::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'DELETE';
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
    $desc = 'The description for delete feature sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'feature_name'  => array('req' => TRUE,  'width' => 10, 'desc' => "name of the feature to be deleted."),
      'type'          => array('req' => TRUE,  'width' => 10, 'desc' => "type of the feature (Marker, QTL, MTL and Haplotype_Block)."),
      'genus'         => array('req' => FALSE, 'width' => 10, 'desc' => "genus of the marker to be deleted."),
      'species'       => array('req' => FALSE, 'width' => 10, 'desc' => "species of the marker to be deleted."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['sequence']['genetic_marker']              = -1;
    $cvterms['sequence']['qtl']                         = -1;
    $cvterms['sequence']['heritable_phenotypic_marker'] = -1;
    $cvterms['sequence']['haplotype_block']             = -1;
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

    // Gets BIMS_CHADO tables.
    $bc_feature = new BIMS_CHADO_FEATURE($program_id);

    // Checks the type.
    $type_name = $this->_getTypeName($line['type']);
    if (!$type_name) {
      $this->updateMsg('E', 'Invalid feature type : ' . $line['type']);
    }

    // Checks feature.
    $bc_feature->checkFeature($this, $prefix . $line['feature_name'], $line['genus'], $line['species'], $this->cvterms['sequence']['haplotype_block']);

    // Checks organism.
    if ($line['genus'] && $line['species']) {
      MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);
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
    $organism_id = $organism->getOrganismID();

    // Gets type ID of the feature.
    $type_name = $this->_getTypeName($line['type']);
    $type_id = $this->cvterms['sequence'][$type_name];

    // Gets the feature.
    $keys = array(
      'uniquename'  => $prefix . $line['feature_name'],
      'organism_id' => $organism_id,
      'type_id'     => $type_id,
    );
    $feature = $bc_feature->byTKey('feature', $keys);

    // Deletes the feature.
    if ($feature) {
      $bc_feature->setFeatureID($feature->feature_id);
      $name = $feature->name;
      if ($bc_feature->delete()) {
        $this->updateMsg('L', "The feature has been deleted ($name)");
      }
      else {
        $this->updateMsg('E', "Failed to delete the feature ($name)");
      }
    }
  }

  /**
   * Returns cvterm ID of the type.
   */
  private function _getTypeName($type) {
    $type_lc = strtolower($type);
    if ($type_lc == 'marker') {
      return 'genetic_marker';
    }
    else if ($type_lc == 'qtl') {
      return 'qtl';
    }
    else if ($type_lc == 'mtl') {
      return 'heritable_phenotypic_marker';
    }
    else if ($type_lc == 'haplotype_block') {
      return 'haplotype_block';
    }
    return '';
  }
}