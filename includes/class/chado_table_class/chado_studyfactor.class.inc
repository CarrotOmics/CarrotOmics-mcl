<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_STUDYFACTOR class.
 *
 */
class CHADO_STUDYFACTOR  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_STUDYFACTOR.
   */
  protected $member_arr     = NULL;
  protected $studyfactor_id = NULL;
  protected $studydesign_id = NULL;
  protected $type_id        = NULL;
  protected $name           = NULL;
  protected $description    = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'studyfactor_id' => 'serial',
    'studydesign_id' => 'integer',
    'type_id'        => 'integer',
    'name'           => 'text',
    'description'    => 'text',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'studyfactor_id' => TRUE,
    'studydesign_id' => TRUE,
    'type_id'        => FALSE,
    'name'           => TRUE,
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
    $this->studyfactor_id = array_key_exists('studyfactor_id', $details) ? $details['studyfactor_id'] : '';
    $this->studydesign_id = array_key_exists('studydesign_id', $details) ? $details['studydesign_id'] : '';
    $this->type_id        = array_key_exists('type_id', $details)        ? $details['type_id']        : '';
    $this->name           = array_key_exists('name', $details)           ? $details['name']           : '';
    $this->description    = array_key_exists('description', $details)    ? $details['description']    : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_STUDYFACTOR by key(s).
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
    $sql = "SELECT * FROM chado.studyfactor WHERE " . implode(" AND " , $where_arr);
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
    return studyfactor_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'studydesign_id' => array('fk_table' => 'studydesign', 'fk_attr' => 'studydesign_id'),
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
    return array(
      'studyfactor_id' => array(
        array('table' => 'studyfactorvalue', 'attr' => 'studyfactor_id'),
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
        $fields['studydesign_id'] = $this->studydesign_id;
        $fields['name']           = $this->name;
        $fields['description']    = $this->description;
        if (is_numeric($this->type_id)) { $fields['type_id'] = $this->type_id; }

        // Inserts the record.
        $this->studyfactor_id = db_insert('chado.studyfactor')
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
      $fields['studydesign_id'] = $this->studydesign_id;
      $fields['name']           = $this->name;
      $fields['description']    = $this->description;
      if (is_numeric($this->type_id)) { $fields['type_id'] = $this->type_id; }

      // Updates the record.
      db_update('chado.studyfactor')
        ->fields($fields)
        ->condition('studyfactor_id', $this->studyfactor_id, '=')
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

      db_delete('chado.studyfactor')
        ->condition('studyfactor_id', $this->studyfactor_id, '=')
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
   * Retrieves the studyfactor_id.
   *
   * @retrun serial
   */
  public function getStudyfactorID() {
    return $this->studyfactor_id;
  }

  /**
   * Sets the studyfactor_id.
   *
   * @param serial $studyfactor_id
   */
  public function setStudyfactorID($studyfactor_id) {
    $this->studyfactor_id = $studyfactor_id;
  }

  /**
   * Retrieves the studydesign_id.
   *
   * @retrun integer
   */
  public function getStudydesignID() {
    return $this->studydesign_id;
  }

  /**
   * Sets the studydesign_id.
   *
   * @param integer $studydesign_id
   */
  public function setStudydesignID($studydesign_id) {
    $this->studydesign_id = $studydesign_id;
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