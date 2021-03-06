<?php
/**
 * The declaration of MCL_TEMPLATE_CROSS class.
 *
 */
class MCL_TEMPLATE_CROSS extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_UPLOAD::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'CROSS';
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
    $desc = 'The description for cross sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'dataset_name'    => array('req' => TRUE,  'width' => 17, 'desc' => "Name of the dataset (super-dataset or sub-category) to which the cross belongs. The details of the dataset should be given in the 'Dataset' sheet."),
      'site_name'       => array('req' => FALSE, 'width' => 10, 'desc' => "Site information where the stock for the phenotyping is planted. It should match 'site_name' in the 'Site' sheet."),
      'cross_number'    => array('req' => TRUE,  'width' => 17, 'desc' => "ID number or name for this specific cross."),
      'cross_group'     => array('req' => FALSE, 'width' => 13, 'desc' => "Name of group. The name must be cross_number."),
      'mother'          => array('req' => FALSE, 'width' => 13, 'desc' => "ID name or number for the seed parent for the cross.  This ID must also be an entry in the Stock sheet."),
      'mother_location' => array('req' => FALSE, 'width' => 17, 'desc' => "Location details for the mother of this cross (eg. Plant location, individual stock name, etc)."),
      'father'          => array('req' => FALSE, 'width' => 13, 'desc' => "ID name or number for the pollen parent for the cross.  This ID must also be an entry in the Stock sheet."),
      'father_location' => array('req' => FALSE, 'width' => 17, 'desc' => "Location details for the father of this cross (eg. Plant location, individual stock name, etc)."),
      'cross_date'      => array('req' => FALSE, 'width' => 15, 'desc' => "Date for the cross."),
      '##property_name' => array('req' => FALSE, 'width' => 10, 'desc' => "Followed by property (cvterm) name. The data will be stored in property table."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['cross_experiment'] = -1;
    $cvterms['SITE_CV']['cross_date']       = -1;
    $cvterms['SITE_CV']['cross_number']     = -1;
    $cvterms['SITE_CV']['cross_group']      = -1;
    $cvterms['SITE_CV']['mother_location']  = -1;
    $cvterms['SITE_CV']['father_location']  = -1;
    $cvterms['SITE_CV']['maternal_parent']  = -1;
    $cvterms['SITE_CV']['paternal_parent']  = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks dataset.
    MCL_CHADO_DATASET::checkDataset($this, $line['dataset_name']);

    // Checks site.
    MCL_CHADO_LOCATION::checkSite($this, $line['site_name']);

    // Checks the parents only if 'progeny' sheet exists.
    $flag_progeny = $this->hasTemplate('progeny');

    // Checks maternal parent.
    if ($line['mother'] && $line['mother'] != 'N/A' && !$flag_progeny) {
      MCL_CHADO_STOCK::checkStock($this, $line['mother']);
    }

    // Checks paternal parent.
    if ($line['father'] && $line['father'] != 'N/A' && !$flag_progeny) {
      MCL_CHADO_STOCK::checkStock($this, $line['father']);
    }
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets nd_geolocation_id.
    $nd_geolocation_id = MCL_CHADO_LOCATION::getLocationIDBySite($line['site_name']);

    // Gets dataset.
    $dataset = MCL_CHADO_DATASET::byName($line['dataset_name']);

    // Adds a cross.
    $dup_key = $dataset->getProjectID() . '_' . $line['cross_number'];
    $cross = MCL_CHADO_ND_EXPERIMENT::addNdExperiment($this, $this->cvterms['SITE_CV']['cross_experiment'], $nd_geolocation_id, $this->cvterms['SITE_CV']['cross_experiment'], $dup_key);
    if ($cross) {

      // Adds properties.
      $cross->addProp($this, 'SITE_CV', 'cross_number', $line['cross_number']);
      $cross->addProp($this, 'SITE_CV', 'cross_date', $line['cross_date']);
      $cross->addProp($this, 'SITE_CV', 'mother_location', $line['mother_location']);
      $cross->addProp($this, 'SITE_CV', 'father_location', $line['father_location']);

      // Adds the user-specific properties.
      $this->addSpecialColumns('##', $cross, $line);

      // Adds a dataset.
      $cross->addDataset($this, $line['dataset_name']);

      // Keeps the cross group.
      if ($line['cross_group']) {
        $GLOBALS['cross_groups'][$cross->getNdExperimentID()] = array(
          'project_id'  => $dataset->getProjectID(),
          'type_id'     => $this->cvterms['SITE_CV']['cross_experiment'],
          'cross_group' => $line['cross_group'],
        );
      }

      // Adds a maternal parent.
      if ($line['mother'] && $line['mother'] != 'N/A') {
        $maternal_parent = MCL_CHADO_STOCK::getStock($line['mother']);
        $cross->addStock($this, $maternal_parent, $this->cvterms['SITE_CV']['maternal_parent']);
      }

      // Adds a paternal parent.
      if ($line['father'] && $line['father'] != 'N/A') {
        $paternal_parent = MCL_CHADO_STOCK::getStock($line['father']);
        $cross->addStock($this, $paternal_parent, $this->cvterms['SITE_CV']['paternal_parent']);;
      }
    }
  }

  /**
   * @see MCL_TEMPLATE::postProcessECD()
   */
  public function postProcessECD() {

    // Lists the job paremters.
    $cv_name = $this->getJob()->getParamByKey('cv');

    // Gets the cv.
    $cv = MCL_CHADO_CV::getCV($cv_name, 'SITE_CV');
    if (!$cv) {
      $this->updateMsg('E', "cv does not exist.");
      return FALSE;
    }

    // Saves the user-specific properties.
    if (!$this->saveSpecialColumns('##', $cv->getCvID(), $this->getHeaders())) {
      $this->updateMsg('E', "Failed to save the special columns");
      return FALSE;
    }

    // Initializes $GLOBALS.
    $GLOBALS['cross_groups'] = array();
    return TRUE;
  }

  /**
   * @see MCL_TEMPLATE::postProcessDU()
   */
  public function postProcessDU() {

    // Adds cross group.
    $no_error = TRUE;
    foreach ((array)$GLOBALS['cross_groups'] as $nd_experiment_id => $info) {

      // Gets the parent cross_number.
      $cross_parent = MCL_CHADO_ND_EXPERIMENT::byCrossNumber($info['project_id'], $info['cross_group'], $info['type_id'], $info['type_id']);
      if (!$cross_parent) {
        $this->updateMsg('E', $info['cross_group'] . " does not exist");
        $no_error = FALSE;
        continue;
      }

      // Gets the chidl cross_number.
      $cross_child  = MCL_CHADO_ND_EXPERIMENT::byID($nd_experiment_id);
      if (!$cross_child) {
        $this->updateMsg('E', "The nd_experiment_id [$nd_experiment_id] does not exist");
        $no_error = FALSE;
        continue;
      }

      // Adds the nd_experiment_id as property (cross group).
      $cross_child->addProp($this, 'SITE_CV', 'cross_group', $cross_parent->getNdExperimentID());
    }
    return $no_error;
  }
}