<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of PUBLIC_MCL_SITE_VAR class.
 *
 */
class PUBLIC_MCL_SITE_VAR  {

  /**
   *  Data members for PUBLIC_MCL_SITE_VAR.
   */
  protected $member_arr  = NULL;
  protected $site_var_id = NULL;
  protected $name        = NULL;
  protected $table_name  = NULL;
  protected $key         = NULL;
  protected $id          = NULL;
  protected $value       = NULL;
  protected $description = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'site_var_id' => 'serial',
    'name'        => 'character_varying(255)',
    'table_name'  => 'character_varying(255)',
    'key'         => 'character_varying(255)',
    'id'          => 'integer',
    'value'       => 'character_varying(1024)',
    'description' => 'text',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'site_var_id' => TRUE,
    'name'        => TRUE,
    'table_name'  => FALSE,
    'key'         => FALSE,
    'id'          => FALSE,
    'value'       => FALSE,
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
    $this->site_var_id = array_key_exists('site_var_id', $details) ? $details['site_var_id'] : '';
    $this->name        = array_key_exists('name', $details)        ? $details['name']        : '';
    $this->table_name  = array_key_exists('table_name', $details)  ? $details['table_name']  : '';
    $this->key         = array_key_exists('key', $details)         ? $details['key']         : '';
    $this->id          = array_key_exists('id', $details)          ? $details['id']          : '';
    $this->value       = array_key_exists('value', $details)       ? $details['value']       : '';
    $this->description = array_key_exists('description', $details) ? $details['description'] : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate PUBLIC_MCL_SITE_VAR by key(s).
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
    $sql = "SELECT * FROM mcl_site_var WHERE " . implode(" AND " , $where_arr);
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
    return site_var_id;
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
      'mcl_site_var_ukey_001_key' => array('name'),
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
        $fields['name']        = $this->name;
        $fields['table_name']  = $this->table_name;
        $fields['key']         = $this->key;
        $fields['value']       = $this->value;
        $fields['description'] = $this->description;
        if (is_numeric($this->id)) { $fields['id'] = $this->id; }

        // Inserts the record.
        $this->site_var_id = db_insert('mcl_site_var')
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
      $fields['table_name']  = $this->table_name;
      $fields['key']         = $this->key;
      $fields['value']       = $this->value;
      $fields['description'] = $this->description;
      if (is_numeric($this->id)) { $fields['id'] = $this->id; }

      // Updates the record.
      db_update('mcl_site_var')
        ->fields($fields)
        ->condition('site_var_id', $this->site_var_id, '=')
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

      db_delete('mcl_site_var')
        ->condition('site_var_id', $this->site_var_id, '=')
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
   * Retrieves the site_var_id.
   *
   * @retrun serial
   */
  public function getSiteVarID() {
    return $this->site_var_id;
  }

  /**
   * Sets the site_var_id.
   *
   * @param serial $site_var_id
   */
  public function setSiteVarID($site_var_id) {
    $this->site_var_id = $site_var_id;
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
   * Retrieves the table_name.
   *
   * @retrun character_varying(255)
   */
  public function getTableName() {
    return $this->table_name;
  }

  /**
   * Sets the table_name.
   *
   * @param character_varying(255) $table_name
   */
  public function setTableName($table_name) {
    $this->table_name = $table_name;
  }

  /**
   * Retrieves the key.
   *
   * @retrun character_varying(255)
   */
  public function getKey() {
    return $this->key;
  }

  /**
   * Sets the key.
   *
   * @param character_varying(255) $key
   */
  public function setKey($key) {
    $this->key = $key;
  }

  /**
   * Retrieves the id.
   *
   * @retrun integer
   */
  public function getId() {
    return $this->id;
  }

  /**
   * Sets the id.
   *
   * @param integer $id
   */
  public function setId($id) {
    $this->id = $id;
  }

  /**
   * Retrieves the value.
   *
   * @retrun character_varying(1024)
   */
  public function getValue() {
    return $this->value;
  }

  /**
   * Sets the value.
   *
   * @param character_varying(1024) $value
   */
  public function setValue($value) {
    $this->value = $value;
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