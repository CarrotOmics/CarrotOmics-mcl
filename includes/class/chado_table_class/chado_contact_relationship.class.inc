<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_CONTACT_RELATIONSHIP class.
 *
 */
class CHADO_CONTACT_RELATIONSHIP  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_CONTACT_RELATIONSHIP.
   */
  protected $member_arr              = NULL;
  protected $contact_relationship_id = NULL;
  protected $type_id                 = NULL;
  protected $subject_id              = NULL;
  protected $object_id               = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'contact_relationship_id' => 'serial',
    'type_id'                 => 'integer',
    'subject_id'              => 'integer',
    'object_id'               => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'contact_relationship_id' => TRUE,
    'type_id'                 => TRUE,
    'subject_id'              => TRUE,
    'object_id'               => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr              = $details;
    $this->contact_relationship_id = array_key_exists('contact_relationship_id', $details) ? $details['contact_relationship_id'] : '';
    $this->type_id                 = array_key_exists('type_id', $details)                 ? $details['type_id']                 : '';
    $this->subject_id              = array_key_exists('subject_id', $details)              ? $details['subject_id']              : '';
    $this->object_id               = array_key_exists('object_id', $details)               ? $details['object_id']               : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_CONTACT_RELATIONSHIP by key(s).
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
    $sql = "SELECT * FROM chado.contact_relationship WHERE " . implode(" AND " , $where_arr);
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
    return contact_relationship_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'object_id' => array('fk_table' => 'contact', 'fk_attr' => 'contact_id'),
      'subject_id' => array('fk_table' => 'contact', 'fk_attr' => 'contact_id'),
      'type_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'contact_relationship_c1' => array('subject_id','object_id','type_id'),
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
        $fields['type_id']    = $this->type_id;
        $fields['subject_id'] = $this->subject_id;
        $fields['object_id']  = $this->object_id;


        // Inserts the record.
        $this->contact_relationship_id = db_insert('chado.contact_relationship')
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
      $fields['type_id']    = $this->type_id;
      $fields['subject_id'] = $this->subject_id;
      $fields['object_id']  = $this->object_id;


      // Updates the record.
      db_update('chado.contact_relationship')
        ->fields($fields)
        ->condition('contact_relationship_id', $this->contact_relationship_id, '=')
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

      db_delete('chado.contact_relationship')
        ->condition('contact_relationship_id', $this->contact_relationship_id, '=')
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
   * Retrieves the contact_relationship_id.
   *
   * @retrun serial
   */
  public function getContactRelationshipID() {
    return $this->contact_relationship_id;
  }

  /**
   * Sets the contact_relationship_id.
   *
   * @param serial $contact_relationship_id
   */
  public function setContactRelationshipID($contact_relationship_id) {
    $this->contact_relationship_id = $contact_relationship_id;
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
}