<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of PUBLIC_MCL_DATA_VALID_TYPE class.
 *
 */
class PUBLIC_MCL_DATA_VALID_TYPE  {

  /**
   *  Data members for PUBLIC_MCL_DATA_VALID_TYPE.
   */
  protected $member_arr         = NULL;
  protected $data_valid_type_id = NULL;
  protected $type               = NULL;
  protected $cv_id              = NULL;
  protected $template_id        = NULL;
  protected $description        = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'data_valid_type_id' => 'serial',
    'type'               => 'character_varying(255)',
    'cv_id'              => 'integer',
    'template_id'        => 'integer',
    'description'        => 'text',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'data_valid_type_id' => TRUE,
    'type'               => TRUE,
    'cv_id'              => FALSE,
    'template_id'        => TRUE,
    'description'        => FALSE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr         = $details;
    $this->data_valid_type_id = array_key_exists('data_valid_type_id', $details) ? $details['data_valid_type_id'] : '';
    $this->type               = array_key_exists('type', $details)               ? $details['type']               : '';
    $this->cv_id              = array_key_exists('cv_id', $details)              ? $details['cv_id']              : '';
    $this->template_id        = array_key_exists('template_id', $details)        ? $details['template_id']        : '';
    $this->description        = array_key_exists('description', $details)        ? $details['description']        : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate PUBLIC_MCL_DATA_VALID_TYPE by key(s).
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
    $sql = "SELECT * FROM mcl_data_valid_type WHERE " . implode(" AND " , $where_arr);
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
    return data_valid_type_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return NULL;
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'mcl_data_valid_type_type_template_id_key' => array('type','template_id'),
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
        $fields['type']        = $this->type;
        $fields['template_id'] = $this->template_id;
        $fields['description'] = $this->description;
        if (is_numeric($this->cv_id)) { $fields['cv_id'] = $this->cv_id; }

        // Inserts the record.
        $this->data_valid_type_id = db_insert('mcl_data_valid_type')
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
      $fields['type']        = $this->type;
      $fields['template_id'] = $this->template_id;
      $fields['description'] = $this->description;
      if (is_numeric($this->cv_id)) { $fields['cv_id'] = $this->cv_id; }

      // Updates the record.
      db_update('mcl_data_valid_type')
        ->fields($fields)
        ->condition('data_valid_type_id', $this->data_valid_type_id, '=')
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

      db_delete('mcl_data_valid_type')
        ->condition('data_valid_type_id', $this->data_valid_type_id, '=')
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
   * Retrieves the data_valid_type_id.
   *
   * @retrun serial
   */
  public function getDataValidTypeID() {
    return $this->data_valid_type_id;
  }

  /**
   * Sets the data_valid_type_id.
   *
   * @param serial $data_valid_type_id
   */
  public function setDataValidTypeID($data_valid_type_id) {
    $this->data_valid_type_id = $data_valid_type_id;
  }

  /**
   * Retrieves the type.
   *
   * @retrun character_varying(255)
   */
  public function getType() {
    return $this->type;
  }

  /**
   * Sets the type.
   *
   * @param character_varying(255) $type
   */
  public function setType($type) {
    $this->type = $type;
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
   * Retrieves the template_id.
   *
   * @retrun integer
   */
  public function getTemplateID() {
    return $this->template_id;
  }

  /**
   * Sets the template_id.
   *
   * @param integer $template_id
   */
  public function setTemplateID($template_id) {
    $this->template_id = $template_id;
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