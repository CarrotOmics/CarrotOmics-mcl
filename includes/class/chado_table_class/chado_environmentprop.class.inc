<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_ENVIRONMENTPROP class.
 *
 */
class CHADO_ENVIRONMENTPROP  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_ENVIRONMENTPROP.
   */
  protected $member_arr         = NULL;
  protected $environmentprop_id = NULL;
  protected $environment_id     = NULL;
  protected $type_id            = NULL;
  protected $value              = NULL;
  protected $rank               = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'environmentprop_id' => 'serial',
    'environment_id'     => 'integer',
    'type_id'            => 'integer',
    'value'              => 'text',
    'rank'               => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'environmentprop_id' => TRUE,
    'environment_id'     => TRUE,
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
    $this->environmentprop_id = array_key_exists('environmentprop_id', $details) ? $details['environmentprop_id'] : '';
    $this->environment_id     = array_key_exists('environment_id', $details)     ? $details['environment_id']     : '';
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
   * Generate CHADO_ENVIRONMENTPROP by key(s).
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
    $sql = "SELECT * FROM chado.environmentprop WHERE " . implode(" AND " , $where_arr);
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
    return environmentprop_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'environment_id' => array('fk_table' => 'environment', 'fk_attr' => 'environment_id'),
      'type_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'environmentprop_c1' => array('environment_id','type_id','rank'),
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
        $fields['environment_id'] = $this->environment_id;
        $fields['type_id']        = $this->type_id;
        $fields['value']          = $this->value;
        $fields['rank']           = (is_numeric($this->rank)) ? $this->rank : 0;


        // Inserts the record.
        $this->environmentprop_id = db_insert('chado.environmentprop')
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
      $fields['environment_id'] = $this->environment_id;
      $fields['type_id']        = $this->type_id;
      $fields['value']          = $this->value;
      $fields['rank']           = (is_numeric($this->rank)) ? $this->rank : 0;


      // Updates the record.
      db_update('chado.environmentprop')
        ->fields($fields)
        ->condition('environmentprop_id', $this->environmentprop_id, '=')
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

      db_delete('chado.environmentprop')
        ->condition('environmentprop_id', $this->environmentprop_id, '=')
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
   * Retrieves the environmentprop_id.
   *
   * @retrun serial
   */
  public function getEnvironmentpropID() {
    return $this->environmentprop_id;
  }

  /**
   * Sets the environmentprop_id.
   *
   * @param serial $environmentprop_id
   */
  public function setEnvironmentpropID($environmentprop_id) {
    $this->environmentprop_id = $environmentprop_id;
  }

  /**
   * Retrieves the environment_id.
   *
   * @retrun integer
   */
  public function getEnvironmentID() {
    return $this->environment_id;
  }

  /**
   * Sets the environment_id.
   *
   * @param integer $environment_id
   */
  public function setEnvironmentID($environment_id) {
    $this->environment_id = $environment_id;
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