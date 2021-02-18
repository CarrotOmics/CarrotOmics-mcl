<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_STOCKCOLLECTION_DB class.
 *
 */
class CHADO_STOCKCOLLECTION_DB  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_STOCKCOLLECTION_DB.
   */
  protected $member_arr            = NULL;
  protected $stockcollection_db_id = NULL;
  protected $stockcollection_id    = NULL;
  protected $db_id                 = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'stockcollection_db_id' => 'serial',
    'stockcollection_id'    => 'integer',
    'db_id'                 => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'stockcollection_db_id' => TRUE,
    'stockcollection_id'    => TRUE,
    'db_id'                 => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr            = $details;
    $this->stockcollection_db_id = array_key_exists('stockcollection_db_id', $details) ? $details['stockcollection_db_id'] : '';
    $this->stockcollection_id    = array_key_exists('stockcollection_id', $details)    ? $details['stockcollection_id']    : '';
    $this->db_id                 = array_key_exists('db_id', $details)                 ? $details['db_id']                 : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_STOCKCOLLECTION_DB by key(s).
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
    $sql = "SELECT * FROM chado.stockcollection_db WHERE " . implode(" AND " , $where_arr);
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
    return stockcollection_db_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'db_id' => array('fk_table' => 'db', 'fk_attr' => 'db_id'),
      'stockcollection_id' => array('fk_table' => 'stockcollection', 'fk_attr' => 'stockcollection_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'stockcollection_db_stockcollection_id_db_id_key' => array('stockcollection_id','db_id'),
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
        $fields['db_id']              = $this->db_id;


        // Inserts the record.
        $this->stockcollection_db_id = db_insert('chado.stockcollection_db')
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
      $fields['db_id']              = $this->db_id;


      // Updates the record.
      db_update('chado.stockcollection_db')
        ->fields($fields)
        ->condition('stockcollection_db_id', $this->stockcollection_db_id, '=')
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

      db_delete('chado.stockcollection_db')
        ->condition('stockcollection_db_id', $this->stockcollection_db_id, '=')
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
   * Retrieves the stockcollection_db_id.
   *
   * @retrun serial
   */
  public function getStockcollectionDbID() {
    return $this->stockcollection_db_id;
  }

  /**
   * Sets the stockcollection_db_id.
   *
   * @param serial $stockcollection_db_id
   */
  public function setStockcollectionDbID($stockcollection_db_id) {
    $this->stockcollection_db_id = $stockcollection_db_id;
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
   * Retrieves the db_id.
   *
   * @retrun integer
   */
  public function getDbID() {
    return $this->db_id;
  }

  /**
   * Sets the db_id.
   *
   * @param integer $db_id
   */
  public function setDbID($db_id) {
    $this->db_id = $db_id;
  }
}