<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_LIBRARY_DBXREF class.
 *
 */
class CHADO_LIBRARY_DBXREF  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_LIBRARY_DBXREF.
   */
  protected $member_arr        = NULL;
  protected $library_dbxref_id = NULL;
  protected $library_id        = NULL;
  protected $dbxref_id         = NULL;
  protected $is_current        = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'library_dbxref_id' => 'serial',
    'library_id'        => 'integer',
    'dbxref_id'         => 'integer',
    'is_current'        => 'boolean',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'library_dbxref_id' => TRUE,
    'library_id'        => TRUE,
    'dbxref_id'         => TRUE,
    'is_current'        => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr        = $details;
    $this->library_dbxref_id = array_key_exists('library_dbxref_id', $details) ? $details['library_dbxref_id'] : '';
    $this->library_id        = array_key_exists('library_id', $details)        ? $details['library_id']        : '';
    $this->dbxref_id         = array_key_exists('dbxref_id', $details)         ? $details['dbxref_id']         : '';
    $this->is_current        = array_key_exists('is_current', $details)        ? $details['is_current']        : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_LIBRARY_DBXREF by key(s).
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
    $sql = "SELECT * FROM chado.library_dbxref WHERE " . implode(" AND " , $where_arr);
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
    return library_dbxref_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'dbxref_id' => array('fk_table' => 'dbxref', 'fk_attr' => 'dbxref_id'),
      'library_id' => array('fk_table' => 'library', 'fk_attr' => 'library_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'library_dbxref_c1' => array('library_id','dbxref_id'),
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
        $fields['library_id'] = $this->library_id;
        $fields['dbxref_id']  = $this->dbxref_id;
        $fields['is_current'] = (preg_match("/^(true|false)/i", $this->is_current)) ? "'" . $this->is_current . "'" : 'true';


        // Inserts the record.
        $this->library_dbxref_id = db_insert('chado.library_dbxref')
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
      $fields['library_id'] = $this->library_id;
      $fields['dbxref_id']  = $this->dbxref_id;
      $fields['is_current'] = (preg_match("/^(true|false)/i", $this->is_current)) ? "'" . $this->is_current . "'" : 'true';


      // Updates the record.
      db_update('chado.library_dbxref')
        ->fields($fields)
        ->condition('library_dbxref_id', $this->library_dbxref_id, '=')
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

      db_delete('chado.library_dbxref')
        ->condition('library_dbxref_id', $this->library_dbxref_id, '=')
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
   * Retrieves the library_dbxref_id.
   *
   * @retrun serial
   */
  public function getLibraryDbxrefID() {
    return $this->library_dbxref_id;
  }

  /**
   * Sets the library_dbxref_id.
   *
   * @param serial $library_dbxref_id
   */
  public function setLibraryDbxrefID($library_dbxref_id) {
    $this->library_dbxref_id = $library_dbxref_id;
  }

  /**
   * Retrieves the library_id.
   *
   * @retrun integer
   */
  public function getLibraryID() {
    return $this->library_id;
  }

  /**
   * Sets the library_id.
   *
   * @param integer $library_id
   */
  public function setLibraryID($library_id) {
    $this->library_id = $library_id;
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