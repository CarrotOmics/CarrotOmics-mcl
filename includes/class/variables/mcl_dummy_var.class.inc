<?php
/**
 * The declaration of MCL_DUMMY_VAR class.
 *
 */
class MCL_DUMMY_VAR extends PUBLIC_MCL_DUMMY_VAR {

 /**
  *  Class data members.
  */

  /**
   * @see PUBLIC_MCL_DUMMY_VAR::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see PUBLIC_MCL_DUMMY_VAR::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * @see PUBLIC_MCL_DUMMY_VAR::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
      return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * Returns MCL_DUMMY_VAR by ID.
   *
   * @param integer $dummy_var_id
   *
   * @return MCL_DUMMY_VAR
   */
  public static function byID($dummy_var_id) {
    return MCL_DUMMY_VAR::byKey(array('dummy_var_id' => $dummy_var_id));
  }

  /**
   * Returns MCL_DUMMY_VAR by name.
   *
   * @param string $name
   *
   * @return MCL_DUMMY_VAR
   */
  public static function byName($name) {
    return MCL_DUMMY_VAR::byKey(array('name' => $name));
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
    $mcl_dummy_var = MCL_DUMMY_VAR::byKey($key);
    if (!$mcl_dummy_var) {

      // Adds a new variable.
      $mcl_dummy_var = new MCL_DUMMY_VAR($details);
      if (!$mcl_dummy_var->insert()) {
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
   * @return string
   */
  public static function getVars($flag = MCL_OBJECT) {
    $sql = "SELECT * FROM {mcl_dummy_var}";
    $results = db_query($sql);
    $vars = array();
    while ($obj = $results->fetchObject()) {
      if ($flag == MCL_OPTION) {
        $vars[$obj->dummy_var_id]= $obj->name;
      }
      else {
        $vars []= $obj;
      }
    }
    return $vars;
  }

  /**
   * Returns mcl_dummy_var.id
   *
   * @param string $name
   *
   * @return string
   */
  public static function getIdByName($name) {
    $mcl_dummy_var = MCL_DUMMY_VAR::byKey(array('name' => $name));
    if ($mcl_dummy_var) {
      return $mcl_dummy_var->getId();
    }
    return '';
  }
}