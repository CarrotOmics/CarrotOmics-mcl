<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_DB class.
 *
 */
class CHADO_DB  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_DB.
   */
  protected $member_arr  = NULL;
  protected $db_id       = NULL;
  protected $name        = NULL;
  protected $description = NULL;
  protected $urlprefix   = NULL;
  protected $url         = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'db_id'       => 'serial',
    'name'        => 'character_varying(255)',
    'description' => 'character_varying(255)',
    'urlprefix'   => 'character_varying(255)',
    'url'         => 'character_varying(255)',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'db_id'       => TRUE,
    'name'        => TRUE,
    'description' => FALSE,
    'urlprefix'   => FALSE,
    'url'         => FALSE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr  = $details;
    $this->db_id       = array_key_exists('db_id', $details)       ? $details['db_id']       : '';
    $this->name        = array_key_exists('name', $details)        ? $details['name']        : '';
    $this->description = array_key_exists('description', $details) ? $details['description'] : '';
    $this->urlprefix   = array_key_exists('urlprefix', $details)   ? $details['urlprefix']   : '';
    $this->url         = array_key_exists('url', $details)         ? $details['url']         : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_DB by key(s).
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
    $sql = "SELECT * FROM chado.db WHERE " . implode(" AND " , $where_arr);
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
    return db_id;
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
      'db_c1' => array('name'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'db_id' => array(
        array('table' => 'stockcollection_db', 'attr' => 'db_id'),
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
        $fields['urlprefix']   = $this->urlprefix;
        $fields['url']         = $this->url;


        // Inserts the record.
        $this->db_id = db_insert('chado.db')
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
      $fields['urlprefix']   = $this->urlprefix;
      $fields['url']         = $this->url;


      // Updates the record.
      db_update('chado.db')
        ->fields($fields)
        ->condition('db_id', $this->db_id, '=')
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

      db_delete('chado.db')
        ->condition('db_id', $this->db_id, '=')
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
   * Retrieves the db_id.
   *
   * @retrun serial
   */
  public function getDbID() {
    return $this->db_id;
  }

  /**
   * Sets the db_id.
   *
   * @param serial $db_id
   */
  public function setDbID($db_id) {
    $this->db_id = $db_id;
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

  /**
   * Retrieves the urlprefix.
   *
   * @retrun character_varying(255)
   */
  public function getUrlprefix() {
    return $this->urlprefix;
  }

  /**
   * Sets the urlprefix.
   *
   * @param character_varying(255) $urlprefix
   */
  public function setUrlprefix($urlprefix) {
    $this->urlprefix = $urlprefix;
  }

  /**
   * Retrieves the url.
   *
   * @retrun character_varying(255)
   */
  public function getUrl() {
    return $this->url;
  }

  /**
   * Sets the url.
   *
   * @param character_varying(255) $url
   */
  public function setUrl($url) {
    $this->url = $url;
  }
}