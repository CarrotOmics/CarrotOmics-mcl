<?php
/**
 * The declaration of MCL_CHADO_ANALYSIS class.
 *
 */
class MCL_CHADO_ANALYSIS extends CHADO_ANALYSIS {

  /**
   *  Class data members.
   */
  /**
   * @see CHADO_ANALYSIS::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_ANALYSIS::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * Returns MCL_CHADO_ANALYSIS by ID.
   *
   * @param integer $analysis_id
   *
   * @return MCL_CHADO_ANALYSIS
   */
  public static function byID($analysis_id) {
    return MCL_CHADO_ANALYSIS::byKey(array('analysis_id' => $analysis_id));
  }

  /**
   * Returns MCL_CHADO_ANALYSIS by name.
   *
   * @param string $name
   *
   * @return MCL_CHADO_ANALYSIS
   */
  public static function byName($name) {
    return MCL_CHADO_ANALYSIS::byKey(array('name' => $name));
  }

  /**
   * @see CHADO_ANALYSIS::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * Checks the existence of analysis. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $name
   *
   * @return boolean
   */
  public static function checkAnalysis(MCL_TEMPLATE $mcl_tmpl = NULL, $analysis_name) {
    if ($analysis_name) {

      // Gets the analysis.
      $analysis = MCL_CHADO_ANALYSIS::byName($analysis_name);
      if (!$analysis) {
        self::updateMsg($mcl_tmpl, 'E', "(name) = ($analysis_name) not found in chado.analysis table");
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Adds an analysis.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param array $args
   *
   * @return MCL_CHADO_ANALYSIS
   */
  public static function addAnalysis(MCL_TEMPLATE $mcl_tmpl = NULL, $args) {

    // Checks the arguments.
    $details = array('name' => $args['name']);
    if ($args['program']) {
      $details['program'] = $args['program'];
    }
    if ($args['program_version']) {
      $details['programversion'] = $args['program_version'];
    }
    if ($args['source_name']) {
      $details['sourcename'] = $args['source_name'];
    }
    if (!self::checkReqArgs($details)) {
      return NULL;
    }

    // Checks for duplication.
    $analysis = MCL_CHADO_ANALYSIS::byKey($args);
    if ($analysis) {
      self::addMsg($mcl_tmpl, 'D', 'analysis', $args);
    }
    else {

      // Adds a new analysis.
      $analysis = new MCL_CHADO_ANALYSIS($args);
      if ($analysis->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'analysis', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'analysis', $args);
        return NULL;
      }
    }
    return $analysis;
  }

  /**
   * Adds a feature.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_FEATURE $feature
   *
   * @return boolean
   */
  public function addFeature(MCL_TEMPLATE $mcl_tmpl = NULL, MCL_CHADO_FEATURE $feature) {

    // Adds a relationship to annalysisfeature table.
    $args = array(
      'analysis_id' => $this->analysis_id,
      'feature_id'  => $feature->getFeatureID(),
    );

    // Checks for duplication.
    $af = CHADO_ANALYSISFEATURE::byKey($args);
    if ($af) {
      self::addMsg($mcl_tmpl, 'D', 'analysisfeature', $args);
    }
    else {

      // Adds a new relationship.
      $af = new CHADO_ANALYSISFEATURE($args);
      if ($af->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'analysisfeature', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'analysisfeature', $args);
        return NULL;
      }
    }
    return $af;
  }
}
