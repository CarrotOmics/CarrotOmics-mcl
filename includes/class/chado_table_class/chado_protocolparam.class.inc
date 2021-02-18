<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_PROTOCOLPARAM class.
 *
 */
class CHADO_PROTOCOLPARAM  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_PROTOCOLPARAM.
   */
  protected $member_arr       = NULL;
  protected $protocolparam_id = NULL;
  protected $protocol_id      = NULL;
  protected $name             = NULL;
  protected $datatype_id      = NULL;
  protected $unittype_id      = NULL;
  protected $value            = NULL;
  protected $rank             = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'protocolparam_id' => 'serial',
    'protocol_id'      => 'integer',
    'name'             => 'text',
    'datatype_id'      => 'integer',
    'unittype_id'      => 'integer',
    'value'            => 'text',
    'rank'             => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'protocolparam_id' => TRUE,
    'protocol_id'      => TRUE,
    'name'             => TRUE,
    'datatype_id'      => FALSE,
    'unittype_id'      => FALSE,
    'value'            => FALSE,
    'rank'             => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr       = $details;
    $this->protocolparam_id = array_key_exists('protocolparam_id', $details) ? $details['protocolparam_id'] : '';
    $this->protocol_id      = array_key_exists('protocol_id', $details)      ? $details['protocol_id']      : '';
    $this->name             = array_key_exists('name', $details)             ? $details['name']             : '';
    $this->datatype_id      = array_key_exists('datatype_id', $details)      ? $details['datatype_id']      : '';
    $this->unittype_id      = array_key_exists('unittype_id', $details)      ? $details['unittype_id']      : '';
    $this->value            = array_key_exists('value', $details)            ? $details['value']            : '';
    $this->rank             = array_key_exists('rank', $details)             ? $details['rank']             : 0;
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_PROTOCOLPARAM by key(s).
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
    $sql = "SELECT * FROM chado.protocolparam WHERE " . implode(" AND " , $where_arr);
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
    return protocolparam_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'datatype_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
      'protocol_id' => array('fk_table' => 'protocol', 'fk_attr' => 'protocol_id'),
      'unittype_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return NULL;
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
        $fields['protocol_id'] = $this->protocol_id;
        $fields['name']        = $this->name;
        $fields['value']       = $this->value;
        $fields['rank']        = (is_numeric($this->rank)) ? $this->rank : 0;
        if (is_numeric($this->datatype_id)) { $fields['datatype_id'] = $this->datatype_id; }
        if (is_numeric($this->unittype_id)) { $fields['unittype_id'] = $this->unittype_id; }

        // Inserts the record.
        $this->protocolparam_id = db_insert('chado.protocolparam')
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
      $fields['protocol_id'] = $this->protocol_id;
      $fields['name']        = $this->name;
      $fields['value']       = $this->value;
      $fields['rank']        = (is_numeric($this->rank)) ? $this->rank : 0;
      if (is_numeric($this->datatype_id)) { $fields['datatype_id'] = $this->datatype_id; }
      if (is_numeric($this->unittype_id)) { $fields['unittype_id'] = $this->unittype_id; }

      // Updates the record.
      db_update('chado.protocolparam')
        ->fields($fields)
        ->condition('protocolparam_id', $this->protocolparam_id, '=')
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

      db_delete('chado.protocolparam')
        ->condition('protocolparam_id', $this->protocolparam_id, '=')
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
   * Retrieves the protocolparam_id.
   *
   * @retrun serial
   */
  public function getProtocolparamID() {
    return $this->protocolparam_id;
  }

  /**
   * Sets the protocolparam_id.
   *
   * @param serial $protocolparam_id
   */
  public function setProtocolparamID($protocolparam_id) {
    $this->protocolparam_id = $protocolparam_id;
  }

  /**
   * Retrieves the protocol_id.
   *
   * @retrun integer
   */
  public function getProtocolID() {
    return $this->protocol_id;
  }

  /**
   * Sets the protocol_id.
   *
   * @param integer $protocol_id
   */
  public function setProtocolID($protocol_id) {
    $this->protocol_id = $protocol_id;
  }

  /**
   * Retrieves the name.
   *
   * @retrun text
   */
  public function getName() {
    return $this->name;
  }

  /**
   * Sets the name.
   *
   * @param text $name
   */
  public function setName($name) {
    $this->name = $name;
  }

  /**
   * Retrieves the datatype_id.
   *
   * @retrun integer
   */
  public function getDatatypeID() {
    return $this->datatype_id;
  }

  /**
   * Sets the datatype_id.
   *
   * @param integer $datatype_id
   */
  public function setDatatypeID($datatype_id) {
    $this->datatype_id = $datatype_id;
  }

  /**
   * Retrieves the unittype_id.
   *
   * @retrun integer
   */
  public function getUnittypeID() {
    return $this->unittype_id;
  }

  /**
   * Sets the unittype_id.
   *
   * @param integer $unittype_id
   */
  public function setUnittypeID($unittype_id) {
    $this->unittype_id = $unittype_id;
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