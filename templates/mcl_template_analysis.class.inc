<?php
/**
 * The declaration of MCL_TEMPLATE_ANALYSIS class.
 *
 */
class MCL_TEMPLATE_ANALYSIS extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'ANALYSIS';
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
    $desc = 'The description for analysis sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'name'            => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the analysis (Maxium 255 charactrers)."),
      'program'         => array('req' => TRUE,  'width' => 10, 'desc' => "Program name, e.g. blastx, blastp, sim4, genscan (Maxium 255 charactrers)."),
      'program_version' => array('req' => TRUE,  'width' => 10, 'desc' => "Version description, e.g. TBLASTX 2.0MP-WashU [09-Nov-2000] (Maxium 255 charactrers)."),
      'algorithm'       => array('req' => FALSE, 'width' => 10, 'desc' => "Algorithm name, e.g. blast (Maxium 255 charactrers)."),
      'source_name'     => array('req' => FALSE, 'width' => 10, 'desc' => "Source name, e.g. cDNA, SwissProt (Maxium 255 charactrers)."),
      'source_version'  => array('req' => FALSE, 'width' => 10, 'desc' => "Source version (Maxium 255 charactrers)."),
      'description'     => array('req' => FALSE, 'width' => 10, 'desc' => "The description of the analysis."),
      'timeexecuted'    => array('req' => FALSE, 'width' => 10, 'desc' => "Time of analysis execution. Use the format yyyy-mm-dd hh:mm:ss e.g. \"2001-02-03 04:05:06\". Leave blank to use the time when the data are uploaded."),
      '##property_name' => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns (##) : followed by name (cvterm name) of the property, or cv::name."),
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
  public function runErrorCheckDataLine($line) {

    // Checks for the length of the data.
    $this->checkLength(255, 'name', $line['name']);
    $this->checkLength(255, 'program', $line['program']);
    $this->checkLength(255, 'program_version', $line['program_version']);
    $this->checkLength(255, 'algorithm', $line['algorithm']);
    $this->checkLength(255, 'source_name', $line['source_name']);
    $this->checkLength(255, 'source_version', $line['source_version']);
    $this->checkLength(32,  'timeexecuted', $line['timeexecuted']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Adds an analysis.
    $details = array(
      'name'            => $line['name'],
      'program'         => $line['program'],
      'programversion'  => $line['program_version'],
      'algorithm'       => $line['algorithm'],
      'sourcename'      => $line['source_name'],
      'sourceversion'   => $line['source_version'],
      'description'     => $line['description'],
      'timeexecuted'    => $line['timeexecuted'],
    );
    $analysis = MCL_CHADO_ANALYSIS::addAnalysis($this, $details);
    if ($analysis) {

      // Adds the user-specific properties.
      $this->addSpecialColumns('##', $analysis, $line);

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
      $this->updateMsg('E', "cv \"$cv_name\" does not exist.");
      return FALSE;
    }

    // Saves the user-specific properties.
    if (!$this->saveSpecialColumns('##', $cv->getCvID(), $this->getHeaders())) {
      $this->updateMsg('E', "Failed to save the special columns");
      return FALSE;
    }

    return TRUE;
  }
}
