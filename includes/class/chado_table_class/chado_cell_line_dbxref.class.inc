<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_CELL_LINE_DBXREF class.
 *
 */
class CHADO_CELL_LINE_DBXREF  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_CELL_LINE_DBXREF.
   */
  protected $member_arr          = NULL;
  protected $cell_line_dbxref_id = NULL;
  protected $cell_line_id        = NULL;
  protected $dbxref_id           = NULL;
  protected $is_current          = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'cell_line_dbxref_id' => 'serial',
    'cell_line_id'        => 'integer',
    'dbxref_id'           => 'integer',
    'is_current'          => 'boolean',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'cell_line_dbxref_id' => TRUE,
    'cell_line_id'        => TRUE,
    'dbxref_id'           => TRUE,
    'is_current'          => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr          = $details;
    $this->cell_line_dbxref_id = array_key_exists('cell_line_dbxref_id', $details) ? $details['cell_line_dbxref_id'] : '';
    $this->cell_line_id        = array_key_exists('cell_line_id', $details)        ? $details['cell_line_id']        : '';
    $this->dbxref_id           = array_key_exists('dbxref_id', $details)           ? $details['dbxref_id']           : '';
    $this->is_current          = array_key_exists('is_current', $details)          ? $details['is_current']          : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_CELL_LINE_DBXREF by key(s).
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
    $sql = "SELECT * FROM chado.cell_line_dbxref WHERE " . implode(" AND " , $where_arr);
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
    return cell_line_dbxref_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'cell_line_id' => array('fk_table' => 'cell_line', 'fk_attr' => 'cell_line_id'),
      'dbxref_id' => array('fk_table' => 'dbxref', 'fk_attr' => 'dbxref_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'cell_line_dbxref_c1' => array('cell_line_id','dbxref_id'),
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
        $fields['cell_line_id'] = $this->cell_line_id;
        $fields['dbxref_id']    = $this->dbxref_id;
        $fields['is_current']   = (preg_match("/^(true|false)/i", $this->is_current)) ? "'" . $this->is_current . "'" : 'true';


        // Inserts the record.
        $this->cell_line_dbxref_id = db_insert('chado.cell_line_dbxref')
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
      $fields['cell_line_id'] = $this->cell_line_id;
      $fields['dbxref_id']    = $this->dbxref_id;
      $fields['is_current']   = (preg_match("/^(true|false)/i", $this->is_current)) ? "'" . $this->is_current . "'" : 'true';


      // Updates the record.
      db_update('chado.cell_line_dbxref')
        ->fields($fields)
        ->condition('cell_line_dbxref_id', $this->cell_line_dbxref_id, '=')
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

      db_delete('chado.cell_line_dbxref')
        ->condition('cell_line_dbxref_id', $this->cell_line_dbxref_id, '=')
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
   * Retrieves the cell_line_dbxref_id.
   *
   * @retrun serial
   */
  public function getCellLineDbxrefID() {
    return $this->cell_line_dbxref_id;
  }

  /**
   * Sets the cell_line_dbxref_id.
   *
   * @param serial $cell_line_dbxref_id
   */
  public function setCellLineDbxrefID($cell_line_dbxref_id) {
    $this->cell_line_dbxref_id = $cell_line_dbxref_id;
  }

  /**
   * Retrieves the cell_line_id.
   *
   * @retrun integer
   */
  public function getCellLineID() {
    return $this->cell_line_id;
  }

  /**
   * Sets the cell_line_id.
   *
   * @param integer $cell_line_id
   */
  public function setCellLineID($cell_line_id) {
    $this->cell_line_id = $cell_line_id;
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

  /**
   * Retrieves the is_current.
   *
   * @retrun boolean
   */
  public function getIsCurrent() {
    return $this->is_current;
  }

  /**
   * Sets the is_current.
   *
   * @param boolean $is_current
   */
  public function setIsCurrent($is_current) {
    $this->is_current = $is_current;
  }
}