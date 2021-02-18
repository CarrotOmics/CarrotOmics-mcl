<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_BIOMATERIAL_DBXREF class.
 *
 */
class CHADO_BIOMATERIAL_DBXREF  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_BIOMATERIAL_DBXREF.
   */
  protected $member_arr            = NULL;
  protected $biomaterial_dbxref_id = NULL;
  protected $biomaterial_id        = NULL;
  protected $dbxref_id             = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'biomaterial_dbxref_id' => 'serial',
    'biomaterial_id'        => 'integer',
    'dbxref_id'             => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'biomaterial_dbxref_id' => TRUE,
    'biomaterial_id'        => TRUE,
    'dbxref_id'             => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr            = $details;
    $this->biomaterial_dbxref_id = array_key_exists('biomaterial_dbxref_id', $details) ? $details['biomaterial_dbxref_id'] : '';
    $this->biomaterial_id        = array_key_exists('biomaterial_id', $details)        ? $details['biomaterial_id']        : '';
    $this->dbxref_id             = array_key_exists('dbxref_id', $details)             ? $details['dbxref_id']             : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_BIOMATERIAL_DBXREF by key(s).
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
    $sql = "SELECT * FROM chado.biomaterial_dbxref WHERE " . implode(" AND " , $where_arr);
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
    return biomaterial_dbxref_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'biomaterial_id' => array('fk_table' => 'biomaterial', 'fk_attr' => 'biomaterial_id'),
      'dbxref_id' => array('fk_table' => 'dbxref', 'fk_attr' => 'dbxref_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'biomaterial_dbxref_c1' => array('biomaterial_id','dbxref_id'),
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
        $fields['biomaterial_id'] = $this->biomaterial_id;
        $fields['dbxref_id']      = $this->dbxref_id;


        // Inserts the record.
        $this->biomaterial_dbxref_id = db_insert('chado.biomaterial_dbxref')
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
      $fields['biomaterial_id'] = $this->biomaterial_id;
      $fields['dbxref_id']      = $this->dbxref_id;


      // Updates the record.
      db_update('chado.biomaterial_dbxref')
        ->fields($fields)
        ->condition('biomaterial_dbxref_id', $this->biomaterial_dbxref_id, '=')
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

      db_delete('chado.biomaterial_dbxref')
        ->condition('biomaterial_dbxref_id', $this->biomaterial_dbxref_id, '=')
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
   * Retrieves the biomaterial_dbxref_id.
   *
   * @retrun serial
   */
  public function getBiomaterialDbxrefID() {
    return $this->biomaterial_dbxref_id;
  }

  /**
   * Sets the biomaterial_dbxref_id.
   *
   * @param serial $biomaterial_dbxref_id
   */
  public function setBiomaterialDbxrefID($biomaterial_dbxref_id) {
    $this->biomaterial_dbxref_id = $biomaterial_dbxref_id;
  }

  /**
   * Retrieves the biomaterial_id.
   *
   * @retrun integer
   */
  public function getBiomaterialID() {
    return $this->biomaterial_id;
  }

  /**
   * Sets the biomaterial_id.
   *
   * @param integer $biomaterial_id
   */
  public function setBiomaterialID($biomaterial_id) {
    $this->biomaterial_id = $biomaterial_id;
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