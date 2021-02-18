<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_ELEMENT class.
 *
 */
class CHADO_ELEMENT  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_ELEMENT.
   */
  protected $member_arr     = NULL;
  protected $element_id     = NULL;
  protected $feature_id     = NULL;
  protected $arraydesign_id = NULL;
  protected $type_id        = NULL;
  protected $dbxref_id      = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'element_id'     => 'serial',
    'feature_id'     => 'integer',
    'arraydesign_id' => 'integer',
    'type_id'        => 'integer',
    'dbxref_id'      => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'element_id'     => TRUE,
    'feature_id'     => FALSE,
    'arraydesign_id' => TRUE,
    'type_id'        => FALSE,
    'dbxref_id'      => FALSE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr     = $details;
    $this->element_id     = array_key_exists('element_id', $details)     ? $details['element_id']     : '';
    $this->feature_id     = array_key_exists('feature_id', $details)     ? $details['feature_id']     : '';
    $this->arraydesign_id = array_key_exists('arraydesign_id', $details) ? $details['arraydesign_id'] : '';
    $this->type_id        = array_key_exists('type_id', $details)        ? $details['type_id']        : '';
    $this->dbxref_id      = array_key_exists('dbxref_id', $details)      ? $details['dbxref_id']      : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_ELEMENT by key(s).
   */
  public static function byKey($keys) {

    // Returns NULL if non-integer value are given to the data member whose
    // data type is 'integer'.
    $where_arr = array();
    $args      = array();
    foreach ($keys as $key => $value) {

      // Gets data type.
      $data_type = self::$data_types[$key];

      // Gets 'NOT NULL'.
      $is_not_null = self::$not_nulls[$key];

      // Checks the value if data type is 'integer'.
      if (in_array($data_type, array('serial', 'integer'))) {

        // Returns NULL if $value is empty string and it is 'NOT NULL'.
        if ($value == '' && $value != '0') {
          if ($is_not_null) {
            return NULL;
          }
          continue;
        }
        else if (!is_int((int)$value)) {
          return NULL;
        }
      }

      // Adds LOWER() function to the characters.
      if (preg_match("/^(char|text)/", $data_type)) {
        $where_arr []= " LOWER($key) = LOWER(:$key) ";
      }
      else {
        $where_arr []= " $key = :$key ";
      }
      $args[":$key"] = $keys[$key];
    }

    // Gets the table properties.
    $sql = "SELECT * FROM chado.element WHERE " . implode(" AND " , $where_arr);
    $details = db_query($sql, $args)->fetch(PDO::FETCH_ASSOC);
    if ($details) {
      return new self($details);
    }
    return NULL;
  }

  /**
   * Returns the primary key.
   */
  public static function getPK() {
    return element_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'arraydesign_id' => array('fk_table' => 'arraydesign', 'fk_attr' => 'arraydesign_id'),
      'dbxref_id' => array('fk_table' => 'dbxref', 'fk_attr' => 'dbxref_id'),
      'feature_id' => array('fk_table' => 'feature', 'fk_attr' => 'feature_id'),
      'type_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'element_c1' => array('feature_id','arraydesign_id'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'element_id' => array(
        array('table' => 'element_relationship', 'attr' => 'object_id'),
        array('table' => 'element_relationship', 'attr' => 'subject_id'),
        array('table' => 'elementresult', 'attr' => 'element_id'),
      ),
    );
  }

  /**
   * Adds a new record.
   */
  public function insert() {

    $transaction = db_transaction();
    try {

      // TODO:Checks for duplication.
      $dup = FALSE;

      // Inserts the record if not duplicated.
      if (!$dup) {

        // Populates all fields. The attribute with 'NOT NULL' would be skipped
        // if the value is empty.
        $fields = array();
        $fields['arraydesign_id'] = $this->arraydesign_id;
        if (is_numeric($this->feature_id)) { $fields['feature_id'] = $this->feature_id; }
        if (is_numeric($this->type_id))    { $fields['type_id']    = $this->type_id; }
        if (is_numeric($this->dbxref_id))  { $fields['dbxref_id']  = $this->dbxref_id; }

        // Inserts the record.
        $this->element_id = db_insert('chado.element')
          ->fields($fields)
          ->execute();
      }
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog('mod', $e->getMessage(), array(), WATCHDOG_ERROR);
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Updates the record.
   */
  public function update() {

    $transaction = db_transaction();
    try {

      // Populates all fields. The attribute with 'NOT NULL' would be skipped
      // if the value is empty.
      $fields = array();
      $fields['arraydesign_id'] = $this->arraydesign_id;
      if (is_numeric($this->feature_id)) { $fields['feature_id'] = $this->feature_id; }
      if (is_numeric($this->type_id))    { $fields['type_id']    = $this->type_id; }
      if (is_numeric($this->dbxref_id))  { $fields['dbxref_id']  = $this->dbxref_id; }

      // Updates the record.
      db_update('chado.element')
        ->fields($fields)
        ->condition('element_id', $this->element_id, '=')
        ->execute();
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog('mod', $e->getMessage(), array(), WATCHDOG_ERROR);
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Deletes the record.
   */
  public function delete() {

    $transaction = db_transaction();
    try {

      db_delete('chado.element')
        ->condition('element_id', $this->element_id, '=')
        ->execute();
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog('mod', $e->getMessage(), array(), WATCHDOG_ERROR);
      return FALSE;
    }
    return TRUE;
  }

  /*
   * Defines the data query functions below.
   */
  /**
   * Retrieves the data type of the member.
   *
   * @retrun various
   */
  public function getDataType($member) {
    if (is_array(self::$data_types) && array_key_exists($member, self::$data_types)) {
      return self::$data_types[$member];
    }
    return '';
  }
  /**
   * Retrieves the 'NOT NULL' of the member.
   *
   * @retrun various
   */
  public function isNotNull($member) {
    if (is_array(self::$not_nulls) && array_key_exists($member, self::$not_nulls)) {
      return self::$not_nulls[$member];
    }
    return TRUE;
  }

  /*
   * Defines getters and setters below.
   */
  /**
   * Retrieves the data member array.
   *
   * @retrun array
   */
  public function getMemberArr() {
    return $this->member_arr;
  }

  /**
   * Sets the data member array.
   *
   * @param array $member_arr
   */
  public function setMemberArr($member_arr) {
    $this->member_arr = $member_arr;
  }

  /**
   * Retrieves the element_id.
   *
   * @retrun serial
   */
  public function getElementID() {
    return $this->element_id;
  }

  /**
   * Sets the element_id.
   *
   * @param serial $element_id
   */
  public function setElementID($element_id) {
    $this->element_id = $element_id;
  }

  /**
   * Retrieves the feature_id.
   *
   * @retrun integer
   */
  public function getFeatureID() {
    return $this->feature_id;
  }

  /**
   * Sets the feature_id.
   *
   * @param integer $feature_id
   */
  public function setFeatureID($feature_id) {
    $this->feature_id = $feature_id;
  }

  /**
   * Retrieves the arraydesign_id.
   *
   * @retrun integer
   */
  public function getArraydesignID() {
    return $this->arraydesign_id;
  }

  /**
   * Sets the arraydesign_id.
   *
   * @param integer $arraydesign_id
   */
  public function setArraydesignID($arraydesign_id) {
    $this->arraydesign_id = $arraydesign_id;
  }

  /**
   * Retrieves the type_id.
   *
   * @retrun integer
   */
  public function getTypeID() {
    return $this->type_id;
  }

  /**
   * Sets the type_id.
   *
   * @param integer $type_id
   */
  public function setTypeID($type_id) {
    $this->type_id = $type_id;
  }

  /**
   * Retrieves the dbxref_id.
   *
   * @retrun integer
   */
  public function getDbxrefID() {
    return $this->dbxref_id;
  }

  /**
   * Sets the dbxref_id.
   *
   * @param integer $dbxref_id
   */
  public function setDbxrefID($dbxref_id) {
    $this->dbxref_id = $dbxref_id;
  }
}