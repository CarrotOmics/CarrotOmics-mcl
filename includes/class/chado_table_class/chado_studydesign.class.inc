<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_STUDYDESIGN class.
 *
 */
class CHADO_STUDYDESIGN  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_STUDYDESIGN.
   */
  protected $member_arr     = NULL;
  protected $studydesign_id = NULL;
  protected $study_id       = NULL;
  protected $description    = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'studydesign_id' => 'serial',
    'study_id'       => 'integer',
    'description'    => 'text',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'studydesign_id' => TRUE,
    'study_id'       => TRUE,
    'description'    => FALSE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr     = $details;
    $this->studydesign_id = array_key_exists('studydesign_id', $details) ? $details['studydesign_id'] : '';
    $this->study_id       = array_key_exists('study_id', $details)       ? $details['study_id']       : '';
    $this->description    = array_key_exists('description', $details)    ? $details['description']    : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_STUDYDESIGN by key(s).
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
    $sql = "SELECT * FROM chado.studydesign WHERE " . implode(" AND " , $where_arr);
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
    return studydesign_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'study_id' => array('fk_table' => 'study', 'fk_attr' => 'study_id'),
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
    return array(
      'studydesign_id' => array(
        array('table' => 'studydesignprop', 'attr' => 'studydesign_id'),
        array('table' => 'studyfactor', 'attr' => 'studydesign_id'),
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
        $fields['study_id']    = $this->study_id;
        $fields['description'] = $this->description;


        // Inserts the record.
        $this->studydesign_id = db_insert('chado.studydesign')
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
      $fields['study_id']    = $this->study_id;
      $fields['description'] = $this->description;


      // Updates the record.
      db_update('chado.studydesign')
        ->fields($fields)
        ->condition('studydesign_id', $this->studydesign_id, '=')
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

      db_delete('chado.studydesign')
        ->condition('studydesign_id', $this->studydesign_id, '=')
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
   * Retrieves the studydesign_id.
   *
   * @retrun serial
   */
  public function getStudydesignID() {
    return $this->studydesign_id;
  }

  /**
   * Sets the studydesign_id.
   *
   * @param serial $studydesign_id
   */
  public function setStudydesignID($studydesign_id) {
    $this->studydesign_id = $studydesign_id;
  }

  /**
   * Retrieves the study_id.
   *
   * @retrun integer
   */
  public function getStudyID() {
    return $this->study_id;
  }

  /**
   * Sets the study_id.
   *
   * @param integer $study_id
   */
  public function setStudyID($study_id) {
    $this->study_id = $study_id;
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