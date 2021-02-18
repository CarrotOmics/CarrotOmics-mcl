<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_ARRAYDESIGNPROP class.
 *
 */
class CHADO_ARRAYDESIGNPROP  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_ARRAYDESIGNPROP.
   */
  protected $member_arr         = NULL;
  protected $arraydesignprop_id = NULL;
  protected $arraydesign_id     = NULL;
  protected $type_id            = NULL;
  protected $value              = NULL;
  protected $rank               = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'arraydesignprop_id' => 'serial',
    'arraydesign_id'     => 'integer',
    'type_id'            => 'integer',
    'value'              => 'text',
    'rank'               => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'arraydesignprop_id' => TRUE,
    'arraydesign_id'     => TRUE,
    'type_id'            => TRUE,
    'value'              => FALSE,
    'rank'               => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr         = $details;
    $this->arraydesignprop_id = array_key_exists('arraydesignprop_id', $details) ? $details['arraydesignprop_id'] : '';
    $this->arraydesign_id     = array_key_exists('arraydesign_id', $details)     ? $details['arraydesign_id']     : '';
    $this->type_id            = array_key_exists('type_id', $details)            ? $details['type_id']            : '';
    $this->value              = array_key_exists('value', $details)              ? $details['value']              : '';
    $this->rank               = array_key_exists('rank', $details)               ? $details['rank']               : 0;
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_ARRAYDESIGNPROP by key(s).
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
    $sql = "SELECT * FROM chado.arraydesignprop WHERE " . implode(" AND " , $where_arr);
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
    return arraydesignprop_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'arraydesign_id' => array('fk_table' => 'arraydesign', 'fk_attr' => 'arraydesign_id'),
      'type_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'arraydesignprop_c1' => array('arraydesign_id','type_id','rank'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return NULL;
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
        $fields['type_id']        = $this->type_id;
        $fields['value']          = $this->value;
        $fields['rank']           = (is_numeric($this->rank)) ? $this->rank : 0;


        // Inserts the record.
        $this->arraydesignprop_id = db_insert('chado.arraydesignprop')
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
      $fields['type_id']        = $this->type_id;
      $fields['value']          = $this->value;
      $fields['rank']           = (is_numeric($this->rank)) ? $this->rank : 0;


      // Updates the record.
      db_update('chado.arraydesignprop')
        ->fields($fields)
        ->condition('arraydesignprop_id', $this->arraydesignprop_id, '=')
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

      db_delete('chado.arraydesignprop')
        ->condition('arraydesignprop_id', $this->arraydesignprop_id, '=')
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
   * Retrieves the arraydesignprop_id.
   *
   * @retrun serial
   */
  public function getArraydesignpropID() {
    return $this->arraydesignprop_id;
  }

  /**
   * Sets the arraydesignprop_id.
   *
   * @param serial $arraydesignprop_id
   */
  public function setArraydesignpropID($arraydesignprop_id) {
    $this->arraydesignprop_id = $arraydesignprop_id;
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
   * Retrieves the value.
   *
   * @retrun text
   */
  public function getValue() {
    return $this->value;
  }

  /**
   * Sets the value.
   *
   * @param text $value
   */
  public function setValue($value) {
    $this->value = $value;
  }

  /**
   * Retrieves the rank.
   *
   * @retrun integer
   */
  public function getRank() {
    return $this->rank;
  }

  /**
   * Sets the rank.
   *
   * @param integer $rank
   */
  public function setRank($rank) {
    $this->rank = $rank;
  }
}