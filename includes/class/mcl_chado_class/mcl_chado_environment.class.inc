<?php
/**
 * The declaration of MCL_CHADO_ENVIRONMENT class.
 *
 */
class MCL_CHADO_ENVIRONMENT extends CHADO_ENVIRONMENT {

 /**
  *  Class data members.
  */

  /**
   * @see CHADO_ENVIRONMENT::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_ENVIRONMENT::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * Returns MCL_CHADO_ENVIRONMENT by ID.
   *
   * @param integer $environment_id
   *
   * @return MCL_CHADO_ENVIRONMENT
   */
  public static function byID($environment_id) {
    return MCL_CHADO_ENVIRONMENT::byKey(array('environment_id' => $environment_id));
  }

  /**
   * @see CHADO_ENVIRONMENT::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * Checks the existence of environment. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $uniquename
   *
   * @return boolean
   */
  public static function checkEnvironment(MCL_TEMPLATE $mcl_tmpl = NULL, $uniquename) {
    if ($uniquename) {

      // Gets the environment.
      $environment = MCL_CHADO_ENVIRONMENT::getEnvironment($uniquename);
      if (!$environment) {
        self::updateMsg($mcl_tmpl, 'E', "(uniquename) = ($uniquename) not found in environment");
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Returns the environment by uniquename.
   *
   * @param string $uniquename
   *
   * @return MCL_CHADO_ENVIRONMENT
   */
  public static function getEnvironment($uniquename) {
    return MCL_CHADO_ENVIRONMENT::byKey(array('uniquename' => $uniquename));
  }

  /**
   * Adds an environment.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $uniquename
   *
   * @return MCL_CHADO_ENVIRONMENT
   */
  public static function addEnvironment(MCL_TEMPLATE $mcl_tmpl = NULL, $uniquename) {

    // Sets the arguments.
    $args = array('uniquename' => $uniquename);

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $environment = MCL_CHADO_ENVIRONMENT::byKey($args);
    if ($environment) {
      self::addMsg($mcl_tmpl, 'D', 'environment', $args);
    }
    else {

      // Adds a new environment.
      $environment = new MCL_CHADO_ENVIRONMENT($args);
      if ($environment->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'environment', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'environment', $args);
        return NULL;
      }
    }
    return $environment;
  }

  /**
   * Adds N/A environment.
   *
   * @return MCL_CHADO_ENVIRONMENT
   */
  public static function addNA() {

    // Checks if it already exists.
    $environment = MCL_CHADO_ENVIRONMENT::getEnvironment('N/A', 'N/A');
    if (!$environment) {
      $details = array(
        'uniquename'  => 'N/A',
        'description' => 'Dummy environment ID.',
      );
      $environment = new MCL_CHADO_ENVIRONMENT($details);
      if (!$environment->insert()) {
        return NULL;
      }
    }
    return $environment;
  }
}