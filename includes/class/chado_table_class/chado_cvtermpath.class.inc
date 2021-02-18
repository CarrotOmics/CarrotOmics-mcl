<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_CVTERMPATH class.
 *
 */
class CHADO_CVTERMPATH  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_CVTERMPATH.
   */
  protected $member_arr    = NULL;
  protected $cvtermpath_id = NULL;
  protected $type_id       = NULL;
  protected $subject_id    = NULL;
  protected $object_id     = NULL;
  protected $cv_id         = NULL;
  protected $pathdistance  = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'cvtermpath_id' => 'serial',
    'type_id'       => 'integer',
    'subject_id'    => 'integer',
    'object_id'     => 'integer',
    'cv_id'         => 'integer',
    'pathdistance'  => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'cvtermpath_id' => TRUE,
    'type_id'       => FALSE,
    'subject_id'    => TRUE,
    'object_id'     => TRUE,
    'cv_id'         => TRUE,
    'pathdistance'  => FALSE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr    = $details;
    $this->cvtermpath_id = array_key_exists('cvtermpath_id', $details) ? $details['cvtermpath_id'] : '';
    $this->type_id       = array_key_exists('type_id', $details)       ? $details['type_id']       : '';
    $this->subject_id    = array_key_exists('subject_id', $details)    ? $details['subject_id']    : '';
    $this->object_id     = array_key_exists('object_id', $details)     ? $details['object_id']     : '';
    $this->cv_id         = array_key_exists('cv_id', $details)         ? $details['cv_id']         : '';
    $this->pathdistance  = array_key_exists('pathdistance', $details)  ? $details['pathdistance']  : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_CVTERMPATH by key(s).
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
    $sql = "SELECT * FROM chado.cvtermpath WHERE " . implode(" AND " , $where_arr);
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
    return cvtermpath_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'object_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
      'subject_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
      'type_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'cvtermpath_c1' => array('subject_id','object_id','type_id','pathdistance'),
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
        $fields['subject_id'] = $this->subject_id;
        $fields['object_id']  = $this->object_id;
        $fields['cv_id']      = $this->cv_id;
        if (is_numeric($this->type_id))      { $fields['type_id']      = $this->type_id; }
        if (is_numeric($this->pathdistance)) { $fields['pathdistance'] = $this->pathdistance; }

        // Inserts the record.
        $this->cvtermpath_id = db_insert('chado.cvtermpath')
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
      $fields['subject_id'] = $this->subject_id;
      $fields['object_id']  = $this->object_id;
      $fields['cv_id']      = $this->cv_id;
      if (is_numeric($this->type_id))      { $fields['type_id']      = $this->type_id; }
      if (is_numeric($this->pathdistance)) { $fields['pathdistance'] = $this->pathdistance; }

      // Updates the record.
      db_update('chado.cvtermpath')
        ->fields($fields)
        ->condition('cvtermpath_id', $this->cvtermpath_id, '=')
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

      db_delete('chado.cvtermpath')
        ->condition('cvtermpath_id', $this->cvtermpath_id, '=')
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
   * Retrieves the cvtermpath_id.
   *
   * @retrun serial
   */
  public function getCvtermpathID() {
    return $this->cvtermpath_id;
  }

  /**
   * Sets the cvtermpath_id.
   *
   * @param serial $cvtermpath_id
   */
  public function setCvtermpathID($cvtermpath_id) {
    $this->cvtermpath_id = $cvtermpath_id;
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
   * Retrieves the subject_id.
   *
   * @retrun integer
   */
  public function getSubjectID() {
    return $this->subject_id;
  }

  /**
   * Sets the subject_id.
   *
   * @param integer $subject_id
   */
  public function setSubjectID($subject_id) {
    $this->subject_id = $subject_id;
  }

  /**
   * Retrieves the object_id.
   *
   * @retrun integer
   */
  public function getObjectID() {
    return $this->object_id;
  }

  /**
   * Sets the object_id.
   *
   * @param integer $object_id
   */
  public function setObjectID($object_id) {
    $this->object_id = $object_id;
  }

  /**
   * Retrieves the cv_id.
   *
   * @retrun integer
   */
  public function getCvID() {
    return $this->cv_id;
  }

  /**
   * Sets the cv_id.
   *
   * @param integer $cv_id
   */
  public function setCvID($cv_id) {
    $this->cv_id = $cv_id;
  }

  /**
   * Retrieves the pathdistance.
   *
   * @retrun integer
   */
  public function getPathdistance() {
    return $this->pathdistance;
  }

  /**
   * Sets the pathdistance.
   *
   * @param integer $pathdistance
   */
  public function setPathdistance($pathdistance) {
    $this->pathdistance = $pathdistance;
  }
}