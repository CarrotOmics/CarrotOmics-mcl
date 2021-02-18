<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_CVTERMSYNONYM class.
 *
 */
class CHADO_CVTERMSYNONYM  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_CVTERMSYNONYM.
   */
  protected $member_arr       = NULL;
  protected $cvtermsynonym_id = NULL;
  protected $cvterm_id        = NULL;
  protected $synonym          = NULL;
  protected $type_id          = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'cvtermsynonym_id' => 'serial',
    'cvterm_id'        => 'integer',
    'synonym'          => 'character_varying(1024)',
    'type_id'          => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'cvtermsynonym_id' => TRUE,
    'cvterm_id'        => TRUE,
    'synonym'          => TRUE,
    'type_id'          => FALSE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr       = $details;
    $this->cvtermsynonym_id = array_key_exists('cvtermsynonym_id', $details) ? $details['cvtermsynonym_id'] : '';
    $this->cvterm_id        = array_key_exists('cvterm_id', $details)        ? $details['cvterm_id']        : '';
    $this->synonym          = array_key_exists('synonym', $details)          ? $details['synonym']          : '';
    $this->type_id          = array_key_exists('type_id', $details)          ? $details['type_id']          : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_CVTERMSYNONYM by key(s).
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
    $sql = "SELECT * FROM chado.cvtermsynonym WHERE " . implode(" AND " , $where_arr);
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
    return cvtermsynonym_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'cvterm_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
      'type_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'cvtermsynonym_c1' => array('cvterm_id','synonym'),
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
        $fields['cvterm_id'] = $this->cvterm_id;
        $fields['synonym']   = $this->synonym;
        if (is_numeric($this->type_id)) { $fields['type_id'] = $this->type_id; }

        // Inserts the record.
        $this->cvtermsynonym_id = db_insert('chado.cvtermsynonym')
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
      $fields['cvterm_id'] = $this->cvterm_id;
      $fields['synonym']   = $this->synonym;
      if (is_numeric($this->type_id)) { $fields['type_id'] = $this->type_id; }

      // Updates the record.
      db_update('chado.cvtermsynonym')
        ->fields($fields)
        ->condition('cvtermsynonym_id', $this->cvtermsynonym_id, '=')
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

      db_delete('chado.cvtermsynonym')
        ->condition('cvtermsynonym_id', $this->cvtermsynonym_id, '=')
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
   * Retrieves the cvtermsynonym_id.
   *
   * @retrun serial
   */
  public function getCvtermsynonymID() {
    return $this->cvtermsynonym_id;
  }

  /**
   * Sets the cvtermsynonym_id.
   *
   * @param serial $cvtermsynonym_id
   */
  public function setCvtermsynonymID($cvtermsynonym_id) {
    $this->cvtermsynonym_id = $cvtermsynonym_id;
  }

  /**
   * Retrieves the cvterm_id.
   *
   * @retrun integer
   */
  public function getCvtermID() {
    return $this->cvterm_id;
  }

  /**
   * Sets the cvterm_id.
   *
   * @param integer $cvterm_id
   */
  public function setCvtermID($cvterm_id) {
    $this->cvterm_id = $cvterm_id;
  }

  /**
   * Retrieves the synonym.
   *
   * @retrun character_varying(1024)
   */
  public function getSynonym() {
    return $this->synonym;
  }

  /**
   * Sets the synonym.
   *
   * @param character_varying(1024) $synonym
   */
  public function setSynonym($synonym) {
    $this->synonym = $synonym;
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