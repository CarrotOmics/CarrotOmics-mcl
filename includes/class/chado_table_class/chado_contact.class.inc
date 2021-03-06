<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_CONTACT class.
 *
 */
class CHADO_CONTACT  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_CONTACT.
   */
  protected $member_arr  = NULL;
  protected $contact_id  = NULL;
  protected $type_id     = NULL;
  protected $name        = NULL;
  protected $description = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'contact_id'  => 'serial',
    'type_id'     => 'integer',
    'name'        => 'character_varying(255)',
    'description' => 'character_varying(255)',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'contact_id'  => TRUE,
    'type_id'     => FALSE,
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
    $this->contact_id  = array_key_exists('contact_id', $details)  ? $details['contact_id']  : '';
    $this->type_id     = array_key_exists('type_id', $details)     ? $details['type_id']     : '';
    $this->name        = array_key_exists('name', $details)        ? $details['name']        : '';
    $this->description = array_key_exists('description', $details) ? $details['description'] : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_CONTACT by key(s).
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
    $sql = "SELECT * FROM chado.contact WHERE " . implode(" AND " , $where_arr);
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
    return contact_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'type_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'contact_c1' => array('name'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'contact_id' => array(
        array('table' => 'arraydesign', 'attr' => 'manufacturer_id'),
        array('table' => 'assay', 'attr' => 'operator_id'),
        array('table' => 'biomaterial', 'attr' => 'biosourceprovider_id'),
        array('table' => 'contact_image', 'attr' => 'contact_id'),
        array('table' => 'contact_relationship', 'attr' => 'object_id'),
        array('table' => 'contact_relationship', 'attr' => 'subject_id'),
        array('table' => 'contactprop', 'attr' => 'contact_id'),
        array('table' => 'feature_contact', 'attr' => 'contact_id'),
        array('table' => 'featuremap_contact', 'attr' => 'contact_id'),
        array('table' => 'library_contact', 'attr' => 'contact_id'),
        array('table' => 'nd_experiment_contact', 'attr' => 'contact_id'),
        array('table' => 'project_contact', 'attr' => 'contact_id'),
        array('table' => 'pubauthor_contact', 'attr' => 'contact_id'),
        array('table' => 'quantification', 'attr' => 'operator_id'),
        array('table' => 'stockcollection', 'attr' => 'contact_id'),
        array('table' => 'study', 'attr' => 'contact_id'),
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
        $fields['name']        = $this->name;
        $fields['description'] = $this->description;
        if (is_numeric($this->type_id)) { $fields['type_id'] = $this->type_id; }

        // Inserts the record.
        $this->contact_id = db_insert('chado.contact')
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
      $fields['name']        = $this->name;
      $fields['description'] = $this->description;
      if (is_numeric($this->type_id)) { $fields['type_id'] = $this->type_id; }

      // Updates the record.
      db_update('chado.contact')
        ->fields($fields)
        ->condition('contact_id', $this->contact_id, '=')
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

      db_delete('chado.contact')
        ->condition('contact_id', $this->contact_id, '=')
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
   * Retrieves the contact_id.
   *
   * @retrun serial
   */
  public function getContactID() {
    return $this->contact_id;
  }

  /**
   * Sets the contact_id.
   *
   * @param serial $contact_id
   */
  public function setContactID($contact_id) {
    $this->contact_id = $contact_id;
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
   * @retrun character_varying(255)
   */
  public function getName() {
    return $this->name;
  }

  /**
   * Sets the name.
   *
   * @param character_varying(255) $name
   */
  public function setName($name) {
    $this->name = $name;
  }

  /**
   * Retrieves the description.
   *
   * @retrun character_varying(255)
   */
  public function getDescription() {
    return $this->description;
  }

  /**
   * Sets the description.
   *
   * @param character_varying(255) $description
   */
  public function setDescription($description) {
    $this->description = $description;
  }
}