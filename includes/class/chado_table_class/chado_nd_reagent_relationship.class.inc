<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_ND_REAGENT_RELATIONSHIP class.
 *
 */
class CHADO_ND_REAGENT_RELATIONSHIP  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_ND_REAGENT_RELATIONSHIP.
   */
  protected $member_arr                 = NULL;
  protected $nd_reagent_relationship_id = NULL;
  protected $subject_reagent_id         = NULL;
  protected $object_reagent_id          = NULL;
  protected $type_id                    = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'nd_reagent_relationship_id' => 'serial',
    'subject_reagent_id'         => 'integer',
    'object_reagent_id'          => 'integer',
    'type_id'                    => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'nd_reagent_relationship_id' => TRUE,
    'subject_reagent_id'         => TRUE,
    'object_reagent_id'          => TRUE,
    'type_id'                    => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr                 = $details;
    $this->nd_reagent_relationship_id = array_key_exists('nd_reagent_relationship_id', $details) ? $details['nd_reagent_relationship_id'] : '';
    $this->subject_reagent_id         = array_key_exists('subject_reagent_id', $details)         ? $details['subject_reagent_id']         : '';
    $this->object_reagent_id          = array_key_exists('object_reagent_id', $details)          ? $details['object_reagent_id']          : '';
    $this->type_id                    = array_key_exists('type_id', $details)                    ? $details['type_id']                    : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_ND_REAGENT_RELATIONSHIP by key(s).
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
    $sql = "SELECT * FROM chado.nd_reagent_relationship WHERE " . implode(" AND " , $where_arr);
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
    return nd_reagent_relationship_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'object_reagent_id' => array('fk_table' => 'nd_reagent', 'fk_attr' => 'nd_reagent_id'),
      'subject_reagent_id' => array('fk_table' => 'nd_reagent', 'fk_attr' => 'nd_reagent_id'),
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
        $fields['subject_reagent_id'] = $this->subject_reagent_id;
        $fields['object_reagent_id']  = $this->object_reagent_id;
        $fields['type_id']            = $this->type_id;


        // Inserts the record.
        $this->nd_reagent_relationship_id = db_insert('chado.nd_reagent_relationship')
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
      $fields['subject_reagent_id'] = $this->subject_reagent_id;
      $fields['object_reagent_id']  = $this->object_reagent_id;
      $fields['type_id']            = $this->type_id;


      // Updates the record.
      db_update('chado.nd_reagent_relationship')
        ->fields($fields)
        ->condition('nd_reagent_relationship_id', $this->nd_reagent_relationship_id, '=')
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

      db_delete('chado.nd_reagent_relationship')
        ->condition('nd_reagent_relationship_id', $this->nd_reagent_relationship_id, '=')
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
   * Retrieves the nd_reagent_relationship_id.
   *
   * @retrun serial
   */
  public function getNdReagentRelationshipID() {
    return $this->nd_reagent_relationship_id;
  }

  /**
   * Sets the nd_reagent_relationship_id.
   *
   * @param serial $nd_reagent_relationship_id
   */
  public function setNdReagentRelationshipID($nd_reagent_relationship_id) {
    $this->nd_reagent_relationship_id = $nd_reagent_relationship_id;
  }

  /**
   * Retrieves the subject_reagent_id.
   *
   * @retrun integer
   */
  public function getSubjectReagentID() {
    return $this->subject_reagent_id;
  }

  /**
   * Sets the subject_reagent_id.
   *
   * @param integer $subject_reagent_id
   */
  public function setSubjectReagentID($subject_reagent_id) {
    $this->subject_reagent_id = $subject_reagent_id;
  }

  /**
   * Retrieves the object_reagent_id.
   *
   * @retrun integer
   */
  public function getObjectReagentID() {
    return $this->object_reagent_id;
  }

  /**
   * Sets the object_reagent_id.
   *
   * @param integer $object_reagent_id
   */
  public function setObjectReagentID($object_reagent_id) {
    $this->object_reagent_id = $object_reagent_id;
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