<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_STOCKCOLLECTIONPROP class.
 *
 */
class CHADO_STOCKCOLLECTIONPROP  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_STOCKCOLLECTIONPROP.
   */
  protected $member_arr             = NULL;
  protected $stockcollectionprop_id = NULL;
  protected $stockcollection_id     = NULL;
  protected $type_id                = NULL;
  protected $value                  = NULL;
  protected $rank                   = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'stockcollectionprop_id' => 'serial',
    'stockcollection_id'     => 'integer',
    'type_id'                => 'integer',
    'value'                  => 'text',
    'rank'                   => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'stockcollectionprop_id' => TRUE,
    'stockcollection_id'     => TRUE,
    'type_id'                => TRUE,
    'value'                  => FALSE,
    'rank'                   => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr             = $details;
    $this->stockcollectionprop_id = array_key_exists('stockcollectionprop_id', $details) ? $details['stockcollectionprop_id'] : '';
    $this->stockcollection_id     = array_key_exists('stockcollection_id', $details)     ? $details['stockcollection_id']     : '';
    $this->type_id                = array_key_exists('type_id', $details)                ? $details['type_id']                : '';
    $this->value                  = array_key_exists('value', $details)                  ? $details['value']                  : '';
    $this->rank                   = array_key_exists('rank', $details)                   ? $details['rank']                   : 0;
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_STOCKCOLLECTIONPROP by key(s).
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
    $sql = "SELECT * FROM chado.stockcollectionprop WHERE " . implode(" AND " , $where_arr);
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
    return stockcollectionprop_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'stockcollection_id' => array('fk_table' => 'stockcollection', 'fk_attr' => 'stockcollection_id'),
      'type_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'stockcollectionprop_c1' => array('stockcollection_id','type_id','rank'),
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
        $fields['stockcollection_id'] = $this->stockcollection_id;
        $fields['type_id']            = $this->type_id;
        $fields['value']              = $this->value;
        $fields['rank']               = (is_numeric($this->rank))  ? $this->rank : 0;


        // Inserts the record.
        $this->stockcollectionprop_id = db_insert('chado.stockcollectionprop')
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
      $fields['stockcollection_id'] = $this->stockcollection_id;
      $fields['type_id']            = $this->type_id;
      $fields['value']              = $this->value;
      $fields['rank']               = (is_numeric($this->rank))  ? $this->rank : 0;


      // Updates the record.
      db_update('chado.stockcollectionprop')
        ->fields($fields)
        ->condition('stockcollectionprop_id', $this->stockcollectionprop_id, '=')
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

      db_delete('chado.stockcollectionprop')
        ->condition('stockcollectionprop_id', $this->stockcollectionprop_id, '=')
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
   * Retrieves the stockcollectionprop_id.
   *
   * @retrun serial
   */
  public function getStockcollectionpropID() {
    return $this->stockcollectionprop_id;
  }

  /**
   * Sets the stockcollectionprop_id.
   *
   * @param serial $stockcollectionprop_id
   */
  public function setStockcollectionpropID($stockcollectionprop_id) {
    $this->stockcollectionprop_id = $stockcollectionprop_id;
  }

  /**
   * Retrieves the stockcollection_id.
   *
   * @retrun integer
   */
  public function getStockcollectionID() {
    return $this->stockcollection_id;
  }

  /**
   * Sets the stockcollection_id.
   *
   * @param integer $stockcollection_id
   */
  public function setStockcollectionID($stockcollection_id) {
    $this->stockcollection_id = $stockcollection_id;
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