<?php
/**
 * The declaration of MCL_TEMPLATE_ENVIRONMENT_BIMS class.
 *
 */
class MCL_TEMPLATE_ENVIRONMENT_BIMS extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'ENVIRONMENT';
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
    $desc = 'The description for environment sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'name'        => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the environment."),
      'label'       => array('req' => TRUE,  'width' => 12, 'desc' => "Label of the environment."),
      'description' => array('req' => FALSE, 'width' => 15, 'desc' => "Description of the environment."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {}

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the job paremters.
    $program_id = $this->getJob()->getParamByKey('program_id');

    // Adds an environment.
    $details = array(
      'name'        => $line['name'],
      'program_id'  => $program_id,
      'label'       => $line['label'],
      'description' => $line['description'],
    );
    $bims_environment = BIMS_ENVIRONMENT::addEnvironment($details);
  }
}