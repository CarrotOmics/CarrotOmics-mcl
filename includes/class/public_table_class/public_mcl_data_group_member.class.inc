<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of PUBLIC_MCL_DATA_GROUP_MEMBER class.
 *
 */
class PUBLIC_MCL_DATA_GROUP_MEMBER  {

  /**
   *  Data members for PUBLIC_MCL_DATA_GROUP_MEMBER.
   */
  protected $member_arr           = NULL;
  protected $data_group_member_id = NULL;
  protected $data_group_id        = NULL;
  protected $type                 = NULL;
  protected $key                  = NULL;
  protected $value                = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'data_group_member_id' => 'serial',
    'data_group_id'        => 'integer',
    'type'                 => 'character_varying(255)',
    'key'                  => 'character_varying(255)',
    'value'                => 'text',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'data_group_member_id' => TRUE,
    'data_group_id'        => TRUE,
    'type'                 => TRUE,
    'key'                  => TRUE,
    'value'                => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr           = $details;
    $this->data_group_member_id = array_key_exists('data_group_member_id', $details) ? $details['data_group_member_id'] : '';
    $this->data_group_id        = array_key_exists('data_group_id', $details)        ? $details['data_group_id']        : '';
    $this->type                 = array_key_exists('type', $details)                 ? $details['type']                 : '';
    $this->key                  = array_key_exists('key', $details)                  ? $details['key']                  : '';
    $this->value                = array_key_exists('value', $details)                ? $details['value']                : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate PUBLIC_MCL_DATA_GROUP_MEMBER by key(s).
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
    $sql = "SELECT * FROM mcl_data_group_member WHERE " . implode(" AND " , $where_arr);
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
    return data_group_member_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return NULL;
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'mcl_data_group_member_ukey_001_key' => array('data_group_id','type','key'),
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
        $fields['data_group_id'] = $this->data_group_id;
        $fields['type']          = $this->type;
        $fields['key']           = $this->key;
        $fields['value']         = $this->value;


        // Inserts the record.
        $this->data_group_member_id = db_insert('mcl_data_group_member')
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
      $fields['data_group_id'] = $this->data_group_id;
      $fields['type']          = $this->type;
      $fields['key']           = $this->key;
      $fields['value']         = $this->value;


      // Updates the record.
      db_update('mcl_data_group_member')
        ->fields($fields)
        ->condition('data_group_member_id', $this->data_group_member_id, '=')
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

      db_delete('mcl_data_group_member')
        ->condition('data_group_member_id', $this->data_group_member_id, '=')
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
   * Retrieves the data_group_member_id.
   *
   * @retrun serial
   */
  public function getDataGroupMemberID() {
    return $this->data_group_member_id;
  }

  /**
   * Sets the data_group_member_id.
   *
   * @param serial $data_group_member_id
   */
  public function setDataGroupMemberID($data_group_member_id) {
    $this->data_group_member_id = $data_group_member_id;
  }

  /**
   * Retrieves the data_group_id.
   *
   * @retrun integer
   */
  public function getDataGroupID() {
    return $this->data_group_id;
  }

  /**
   * Sets the data_group_id.
   *
   * @param integer $data_group_id
   */
  public function setDataGroupID($data_group_id) {
    $this->data_group_id = $data_group_id;
  }

  /**
   * Retrieves the type.
   *
   * @retrun character_varying(255)
   */
  public function getType() {
    return $this->type;
  }

  /**
   * Sets the type.
   *
   * @param character_varying(255) $type
   */
  public function setType($type) {
    $this->type = $type;
  }

  /**
   * Retrieves the key.
   *
   * @retrun character_varying(255)
   */
  public function getKey() {
    return $this->key;
  }

  /**
   * Sets the key.
   *
   * @param character_varying(255) $key
   */
  public function setKey($key) {
    $this->key = $key;
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
}