<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_STUDY class.
 *
 */
class CHADO_STUDY  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_STUDY.
   */
  protected $member_arr  = NULL;
  protected $study_id    = NULL;
  protected $contact_id  = NULL;
  protected $pub_id      = NULL;
  protected $dbxref_id   = NULL;
  protected $name        = NULL;
  protected $description = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'study_id'    => 'serial',
    'contact_id'  => 'integer',
    'pub_id'      => 'integer',
    'dbxref_id'   => 'integer',
    'name'        => 'text',
    'description' => 'text',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'study_id'    => TRUE,
    'contact_id'  => TRUE,
    'pub_id'      => FALSE,
    'dbxref_id'   => FALSE,
    'name'        => TRUE,
    'description' => FALSE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr  = $details;
    $this->study_id    = array_key_exists('study_id', $details)    ? $details['study_id']    : '';
    $this->contact_id  = array_key_exists('contact_id', $details)  ? $details['contact_id']  : '';
    $this->pub_id      = array_key_exists('pub_id', $details)      ? $details['pub_id']      : '';
    $this->dbxref_id   = array_key_exists('dbxref_id', $details)   ? $details['dbxref_id']   : '';
    $this->name        = array_key_exists('name', $details)        ? $details['name']        : '';
    $this->description = array_key_exists('description', $details) ? $details['description'] : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_STUDY by key(s).
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
    $sql = "SELECT * FROM chado.study WHERE " . implode(" AND " , $where_arr);
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
    return study_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'contact_id' => array('fk_table' => 'contact', 'fk_attr' => 'contact_id'),
      'dbxref_id' => array('fk_table' => 'dbxref', 'fk_attr' => 'dbxref_id'),
      'pub_id' => array('fk_table' => 'pub', 'fk_attr' => 'pub_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'study_c1' => array('name'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'study_id' => array(
        array('table' => 'study_assay', 'attr' => 'study_id'),
        array('table' => 'studydesign', 'attr' => 'study_id'),
        array('table' => 'studyprop', 'attr' => 'study_id'),
      ),
    );
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
        $fields['contact_id']  = $this->contact_id;
        $fields['name']        = $this->name;
        $fields['description'] = $this->description;
        if (is_numeric($this->pub_id))    { $fields['pub_id']    = $this->pub_id; }
        if (is_numeric($this->dbxref_id)) { $fields['dbxref_id'] = $this->dbxref_id; }

        // Inserts the record.
        $this->study_id = db_insert('chado.study')
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
      $fields['contact_id']  = $this->contact_id;
      $fields['name']        = $this->name;
      $fields['description'] = $this->description;
      if (is_numeric($this->pub_id))    { $fields['pub_id']    = $this->pub_id; }
      if (is_numeric($this->dbxref_id)) { $fields['dbxref_id'] = $this->dbxref_id; }

      // Updates the record.
      db_update('chado.study')
        ->fields($fields)
        ->condition('study_id', $this->study_id, '=')
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

      db_delete('chado.study')
        ->condition('study_id', $this->study_id, '=')
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
   * Retrieves the study_id.
   *
   * @retrun serial
   */
  public function getStudyID() {
    return $this->study_id;
  }

  /**
   * Sets the study_id.
   *
   * @param serial $study_id
   */
  public function setStudyID($study_id) {
    $this->study_id = $study_id;
  }

  /**
   * Retrieves the contact_id.
   *
   * @retrun integer
   */
  public function getContactID() {
    return $this->contact_id;
  }

  /**
   * Sets the contact_id.
   *
   * @param integer $contact_id
   */
  public function setContactID($contact_id) {
    $this->contact_id = $contact_id;
  }

  /**
   * Retrieves the pub_id.
   *
   * @retrun integer
   */
  public function getPubID() {
    return $this->pub_id;
  }

  /**
   * Sets the pub_id.
   *
   * @param integer $pub_id
   */
  public function setPubID($pub_id) {
    $this->pub_id = $pub_id;
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
   * Retrieves the description.
   *
   * @retrun text
   */
  public function getDescription() {
    return $this->description;
  }

  /**
   * Sets the description.
   *
   * @param text $description
   */
  public function setDescription($description) {
    $this->description = $description;
  }
}