<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_ND_PROTOCOL_REAGENT class.
 *
 */
class CHADO_ND_PROTOCOL_REAGENT  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_ND_PROTOCOL_REAGENT.
   */
  protected $member_arr             = NULL;
  protected $nd_protocol_reagent_id = NULL;
  protected $nd_protocol_id         = NULL;
  protected $reagent_id             = NULL;
  protected $type_id                = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'nd_protocol_reagent_id' => 'serial',
    'nd_protocol_id'         => 'integer',
    'reagent_id'             => 'integer',
    'type_id'                => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'nd_protocol_reagent_id' => TRUE,
    'nd_protocol_id'         => TRUE,
    'reagent_id'             => TRUE,
    'type_id'                => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr             = $details;
    $this->nd_protocol_reagent_id = array_key_exists('nd_protocol_reagent_id', $details) ? $details['nd_protocol_reagent_id'] : '';
    $this->nd_protocol_id         = array_key_exists('nd_protocol_id', $details)         ? $details['nd_protocol_id']         : '';
    $this->reagent_id             = array_key_exists('reagent_id', $details)             ? $details['reagent_id']             : '';
    $this->type_id                = array_key_exists('type_id', $details)                ? $details['type_id']                : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_ND_PROTOCOL_REAGENT by key(s).
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
    $sql = "SELECT * FROM chado.nd_protocol_reagent WHERE " . implode(" AND " , $where_arr);
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
    return nd_protocol_reagent_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'nd_protocol_id' => array('fk_table' => 'nd_protocol', 'fk_attr' => 'nd_protocol_id'),
      'reagent_id' => array('fk_table' => 'nd_reagent', 'fk_attr' => 'nd_reagent_id'),
      'type_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
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
        $fields['nd_protocol_id'] = $this->nd_protocol_id;
        $fields['reagent_id']     = $this->reagent_id;
        $fields['type_id']        = $this->type_id;


        // Inserts the record.
        $this->nd_protocol_reagent_id = db_insert('chado.nd_protocol_reagent')
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
      $fields['nd_protocol_id'] = $this->nd_protocol_id;
      $fields['reagent_id']     = $this->reagent_id;
      $fields['type_id']        = $this->type_id;


      // Updates the record.
      db_update('chado.nd_protocol_reagent')
        ->fields($fields)
        ->condition('nd_protocol_reagent_id', $this->nd_protocol_reagent_id, '=')
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

      db_delete('chado.nd_protocol_reagent')
        ->condition('nd_protocol_reagent_id', $this->nd_protocol_reagent_id, '=')
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
   * Retrieves the nd_protocol_reagent_id.
   *
   * @retrun serial
   */
  public function getNdProtocolReagentID() {
    return $this->nd_protocol_reagent_id;
  }

  /**
   * Sets the nd_protocol_reagent_id.
   *
   * @param serial $nd_protocol_reagent_id
   */
  public function setNdProtocolReagentID($nd_protocol_reagent_id) {
    $this->nd_protocol_reagent_id = $nd_protocol_reagent_id;
  }

  /**
   * Retrieves the nd_protocol_id.
   *
   * @retrun integer
   */
  public function getNdProtocolID() {
    return $this->nd_protocol_id;
  }

  /**
   * Sets the nd_protocol_id.
   *
   * @param integer $nd_protocol_id
   */
  public function setNdProtocolID($nd_protocol_id) {
    $this->nd_protocol_id = $nd_protocol_id;
  }

  /**
   * Retrieves the reagent_id.
   *
   * @retrun integer
   */
  public function getReagentID() {
    return $this->reagent_id;
  }

  /**
   * Sets the reagent_id.
   *
   * @param integer $reagent_id
   */
  public function setReagentID($reagent_id) {
    $this->reagent_id = $reagent_id;
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
}