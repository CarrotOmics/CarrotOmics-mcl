<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_PROJECT_RELATIONSHIP class.
 *
 */
class CHADO_PROJECT_RELATIONSHIP  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_PROJECT_RELATIONSHIP.
   */
  protected $member_arr              = NULL;
  protected $project_relationship_id = NULL;
  protected $subject_project_id      = NULL;
  protected $object_project_id       = NULL;
  protected $type_id                 = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'project_relationship_id' => 'serial',
    'subject_project_id'      => 'integer',
    'object_project_id'       => 'integer',
    'type_id'                 => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'project_relationship_id' => TRUE,
    'subject_project_id'      => TRUE,
    'object_project_id'       => TRUE,
    'type_id'                 => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr              = $details;
    $this->project_relationship_id = array_key_exists('project_relationship_id', $details) ? $details['project_relationship_id'] : '';
    $this->subject_project_id      = array_key_exists('subject_project_id', $details)      ? $details['subject_project_id']      : '';
    $this->object_project_id       = array_key_exists('object_project_id', $details)       ? $details['object_project_id']       : '';
    $this->type_id                 = array_key_exists('type_id', $details)                 ? $details['type_id']                 : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_PROJECT_RELATIONSHIP by key(s).
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
    $sql = "SELECT * FROM chado.project_relationship WHERE " . implode(" AND " , $where_arr);
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
    return project_relationship_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'object_project_id' => array('fk_table' => 'project', 'fk_attr' => 'project_id'),
      'subject_project_id' => array('fk_table' => 'project', 'fk_attr' => 'project_id'),
      'type_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'project_relationship_c1' => array('subject_project_id','object_project_id','type_id'),
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
        $fields['subject_project_id'] = $this->subject_project_id;
        $fields['object_project_id']  = $this->object_project_id;
        $fields['type_id']            = $this->type_id;


        // Inserts the record.
        $this->project_relationship_id = db_insert('chado.project_relationship')
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
      $fields['subject_project_id'] = $this->subject_project_id;
      $fields['object_project_id']  = $this->object_project_id;
      $fields['type_id']            = $this->type_id;


      // Updates the record.
      db_update('chado.project_relationship')
        ->fields($fields)
        ->condition('project_relationship_id', $this->project_relationship_id, '=')
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

      db_delete('chado.project_relationship')
        ->condition('project_relationship_id', $this->project_relationship_id, '=')
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
   * Retrieves the project_relationship_id.
   *
   * @retrun serial
   */
  public function getProjectRelationshipID() {
    return $this->project_relationship_id;
  }

  /**
   * Sets the project_relationship_id.
   *
   * @param serial $project_relationship_id
   */
  public function setProjectRelationshipID($project_relationship_id) {
    $this->project_relationship_id = $project_relationship_id;
  }

  /**
   * Retrieves the subject_project_id.
   *
   * @retrun integer
   */
  public function getSubjectProjectID() {
    return $this->subject_project_id;
  }

  /**
   * Sets the subject_project_id.
   *
   * @param integer $subject_project_id
   */
  public function setSubjectProjectID($subject_project_id) {
    $this->subject_project_id = $subject_project_id;
  }

  /**
   * Retrieves the object_project_id.
   *
   * @retrun integer
   */
  public function getObjectProjectID() {
    return $this->object_project_id;
  }

  /**
   * Sets the object_project_id.
   *
   * @param integer $object_project_id
   */
  public function setObjectProjectID($object_project_id) {
    $this->object_project_id = $object_project_id;
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