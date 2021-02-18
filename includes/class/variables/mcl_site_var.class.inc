<?php
/**
 * The declaration of MCL_SITE_VAR class.
 *
 */
class MCL_SITE_VAR extends PUBLIC_MCL_SITE_VAR {

 /**
  *  Class data members.
  */

  /**
   * @see PUBLIC_MCL_SITE_VAR::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see PUBLIC_MCL_SITE_VAR::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * @see PUBLIC_MCL_SITE_VAR::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
      return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * Returns MCL_SITE_VAR by ID.
   *
   * @param integer $site_var_id
   *
   * @return MCL_SITE_VAR
   */
  public static function byID($site_var_id) {
    return MCL_SITE_VAR::byKey(array('site_var_id' => $site_var_id));
  }

  /**
   * Returns MCL_SITE_VAR by name.
   *
   * @param string $name
   *
   * @return MCL_SITE_VAR
   */
  public static function byName($name) {
    return MCL_SITE_VAR::byKey(array('name' => $name));
  }

  /**
   * Adds a new variable.
   *
   * @param array $details
   *
   * @return boolean
   */
  public static function addVar($details) {

    // Checks for duplication.
    $key = array('name' => $details['name']);
    $mcl_site_var = MCL_SITE_VAR::byKey($key);
    if (!$mcl_site_var) {

      // Adds a new variable.
      $mcl_site_var = new MCL_SITE_VAR($details);
      if (!$mcl_site_var->insert()) {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Returns all mcl_dummy_var.
   *
   * @param integer $flag
   *
   * @return array
   */
  public static function getVars($flag = MCL_OBJECT) {
    $results = db_query("SELECT * FROM {mcl_site_var}");
    $vars = array();
    while ($obj = $results->fetchObject()) {
      if ($flag == MCL_OPTION) {
        $vars[$obj->site_var_id]= $obj->name;
      }
      else {
        $vars []= $obj;
      }
    }
    return $vars;
  }

  /**
   * Returns mcl_site_var.value.
   *
   * @param string $name
   *
   * @return string
   */
  public static function getValueByName($name) {
    $mcl_site_var = MCL_SITE_VAR::byKey(array('name' => $name));
    if ($mcl_site_var) {
      return $mcl_site_var->getValue();
    }
    return '';
  }

  /**
   * Returns mcl_site_var.id.
   *
   * @param string $name
   *
   * @return string
   */
  public static function getIdByName($name) {
    $mcl_site_var = MCL_SITE_VAR::byKey(array('name' => $name));
    if ($mcl_site_var) {
      return $mcl_site_var->getId();
    }
    return '';
  }
}