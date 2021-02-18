<?php
/**
 * The declaration of MCL_TEMPLATE_DATASET_BIMS class.
 *
 */
class MCL_TEMPLATE_DATASET_BIMS extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'DATASET';
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
    $desc = 'The description for dataset sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'dataset_name'    => array('req' => TRUE,  'width' => 20, 'desc' => "Names of the datasets."),
      'type'            => array('req' => TRUE,  'width' => 15, 'desc' => "Type of the datasset (eg. cross, phenotype or genotype)."),
      'sub_type'        => array('req' => FALSE, 'width' => 15, 'desc' => "Sub-Type of the dataset (eg. SNP, SSR or breeding). It is requried for phenotype and genotype projects"),
      'description'     => array('req' => FALSE, 'width' => 90, 'desc' => "General description of the dataset (purpose, target, how many crosses, etc)."),
      'pi'              => array('req' => FALSE, 'width' => 12, 'desc' => "Principal Investigator (eg. breeder, corresponding author of the QTL paper) of the Dataset.   It should match a 'contact_name' column in 'Contact' sheet."),
      '##property_name' => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns (##) : followed by name (cvterm name) of the property."),
    );

    // Adds labels.
    $headers['pi']['label'] = 'PI';
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['project_type']  = -1;
    $cvterms['SITE_CV']['sub_type']      = -1;
    $cvterms['SITE_CV']['description']   = -1;
    $cvterms['SITE_CV']['crop']          = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks PI.
    MCL_CHADO_CONTACT::checkContact($this, $line['pi'], "[;]");

    // Checks project type.
    if (!preg_match("/^(phenotype|genotype|cross)$/i", $line['type'])) {
      $this->updateMsg('E', "The type must be either 'phenotype', 'genotype' or 'cross.");
    }

    // Checks project sub type.
    if (preg_match("/^(phenotype|genotype)$/i", $line['type'])) {
      if (!$line['sub_type']) {
        $this->updateMsg('E', "'sub type' of a project must be provided for the 'phenotype' and 'genotype' project.");
      }
    }
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the job paremters.
    $program_id = $this->getJob()->getParamByKey('program_id');
    $crop       = $this->getJob()->getParamByKey('crop');
    $pi         = $this->getJob()->getParamByKey('PI');

    // Gets the prefix.
    $prefix = BIMS_PROGRAM::getPrefixByID($program_id);

    // Gets BIMS_CHADO tables.
    $bc_project = new BIMS_CHADO_PROJECT($program_id);

    // Adds a project.
    $details = array(
      'name'        => $prefix . $line['dataset_name'],
      'type'        => $line['type'],
      'description' => $line['description'],
    );

    $project_id = $bc_project->addProject($this, $details);
    if ($project_id) {
      $bc_project->setProjectID($project_id);

      // Adds the properties.
      $bc_project->addProp($this, 'SITE_CV', 'crop', $crop);
      $bc_project->addProp($this, 'SITE_CV', 'project_type', $line['type']);
      $bc_project->addProp($this, 'SITE_CV', 'sub_type', $line['sub_type']);
      $bc_project->addProp($this, 'SITE_CV', 'description', $line['description']);

      // Adds PI.
      $bc_project->addContact($this, $line['pi']);

      // Adds the user-specific properties.
      $this->addSpecialColumns('##', $bc_project, $line);
    }
  }

  /**
   * @see MCL_TEMPLATE::postProcessECD()
   */
  public function postProcessECD() {

    // Lists the job paremters.
    $cv_arr = $this->getJob()->getParamByKey('cv_arr');

    // Saves the user-specific properties.
    if (!$this->saveSpecialColumns('##', $cv_arr['dataset']['cv_id'], $this->getHeaders())) {
      $this->updateMsg('E', "Failed to save the special columns");
      return FALSE;
    }
    return TRUE;
  }
}