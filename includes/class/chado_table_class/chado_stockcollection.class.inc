<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_STOCKCOLLECTION class.
 *
 */
class CHADO_STOCKCOLLECTION  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_STOCKCOLLECTION.
   */
  protected $member_arr         = NULL;
  protected $stockcollection_id = NULL;
  protected $type_id            = NULL;
  protected $contact_id         = NULL;
  protected $name               = NULL;
  protected $uniquename         = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'stockcollection_id' => 'serial',
    'type_id'            => 'integer',
    'contact_id'         => 'integer',
    'name'               => 'character_varying(255)',
    'uniquename'         => 'text',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'stockcollection_id' => TRUE,
    'type_id'            => TRUE,
    'contact_id'         => FALSE,
    'name'               => FALSE,
    'uniquename'         => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr         = $details;
    $this->stockcollection_id = array_key_exists('stockcollection_id', $details) ? $details['stockcollection_id'] : '';
    $this->type_id            = array_key_exists('type_id', $details)            ? $details['type_id']            : '';
    $this->contact_id         = array_key_exists('contact_id', $details)         ? $details['contact_id']         : '';
    $this->name               = array_key_exists('name', $details)               ? $details['name']               : '';
    $this->uniquename         = array_key_exists('uniquename', $details)         ? $details['uniquename']         : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_STOCKCOLLECTION by key(s).
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
    $sql = "SELECT * FROM chado.stockcollection WHERE " . implode(" AND " , $where_arr);
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
    return stockcollection_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'contact_id' => array('fk_table' => 'contact', 'fk_attr' => 'contact_id'),
      'type_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'stockcollection_c1' => array('uniquename','type_id'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'stockcollection_id' => array(
        array('table' => 'stockcollection_db', 'attr' => 'stockcollection_id'),
        array('table' => 'stockcollection_stock', 'attr' => 'stockcollection_id'),
        array('table' => 'stockcollectionprop', 'attr' => 'stockcollection_id'),
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
        $fields['type_id']    = $this->type_id;
        $fields['name']       = $this->name;
        $fields['uniquename'] = $this->uniquename;
        if (is_numeric($this->contact_id)) { $fields['contact_id'] = $this->contact_id; }

        // Inserts the record.
        $this->stockcollection_id = db_insert('chado.stockcollection')
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
      $fields['name']       = $this->name;
      $fields['uniquename'] = $this->uniquename;
      if (is_numeric($this->contact_id)) { $fields['contact_id'] = $this->contact_id; }

      // Updates the record.
      db_update('chado.stockcollection')
        ->fields($fields)
        ->condition('stockcollection_id', $this->stockcollection_id, '=')
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

      db_delete('chado.stockcollection')
        ->condition('stockcollection_id', $this->stockcollection_id, '=')
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
   * Retrieves the stockcollection_id.
   *
   * @retrun serial
   */
  public function getStockcollectionID() {
    return $this->stockcollection_id;
  }

  /**
   * Sets the stockcollection_id.
   *
   * @param serial $stockcollection_id
   */
  public function setStockcollectionID($stockcollection_id) {
    $this->stockcollection_id = $stockcollection_id;
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
   * Retrieves the uniquename.
   *
   * @retrun text
   */
  public function getUniquename() {
    return $this->uniquename;
  }

  /**
   * Sets the uniquename.
   *
   * @param text $uniquename
   */
  public function setUniquename($uniquename) {
    $this->uniquename = $uniquename;
  }
}