<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_CVPROP class.
 *
 */
class CHADO_CVPROP  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_CVPROP.
   */
  protected $member_arr = NULL;
  protected $cvprop_id  = NULL;
  protected $cv_id      = NULL;
  protected $type_id    = NULL;
  protected $value      = NULL;
  protected $rank       = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'cvprop_id' => 'serial',
    'cv_id'     => 'integer',
    'type_id'   => 'integer',
    'value'     => 'text',
    'rank'      => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'cvprop_id' => TRUE,
    'cv_id'     => TRUE,
    'type_id'   => TRUE,
    'value'     => FALSE,
    'rank'      => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr = $details;
    $this->cvprop_id  = array_key_exists('cvprop_id', $details) ? $details['cvprop_id'] : '';
    $this->cv_id      = array_key_exists('cv_id', $details)     ? $details['cv_id']     : '';
    $this->type_id    = array_key_exists('type_id', $details)   ? $details['type_id']   : '';
    $this->value      = array_key_exists('value', $details)     ? $details['value']     : '';
    $this->rank       = array_key_exists('rank', $details)      ? $details['rank']      : 0;
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_CVPROP by key(s).
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
    $sql = "SELECT * FROM chado.cvprop WHERE " . implode(" AND " , $where_arr);
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
    return cvprop_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'type_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'cvprop_c1' => array('cv_id','type_id','rank'),
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
        $fields['cv_id']   = $this->cv_id;
        $fields['type_id'] = $this->type_id;
        $fields['value']   = $this->value;
        $fields['rank']    = (is_numeric($this->rank)) ? $this->rank : 0;


        // Inserts the record.
        $this->cvprop_id = db_insert('chado.cvprop')
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
      $fields['cv_id']   = $this->cv_id;
      $fields['type_id'] = $this->type_id;
      $fields['value']   = $this->value;
      $fields['rank']    = (is_numeric($this->rank)) ? $this->rank : 0;


      // Updates the record.
      db_update('chado.cvprop')
        ->fields($fields)
        ->condition('cvprop_id', $this->cvprop_id, '=')
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

      db_delete('chado.cvprop')
        ->condition('cvprop_id', $this->cvprop_id, '=')
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
   * Retrieves the cvprop_id.
   *
   * @retrun serial
   */
  public function getCvpropID() {
    return $this->cvprop_id;
  }

  /**
   * Sets the cvprop_id.
   *
   * @param serial $cvprop_id
   */
  public function setCvpropID($cvprop_id) {
    $this->cvprop_id = $cvprop_id;
  }

  /**
   * Retrieves the cv_id.
   *
   * @retrun integer
   */
  public function getCvID() {
    return $this->cv_id;
  }

  /**
   * Sets the cv_id.
   *
   * @param integer $cv_id
   */
  public function setCvID($cv_id) {
    $this->cv_id = $cv_id;
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