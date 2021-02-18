<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_LIBRARYPROP_PUB class.
 *
 */
class CHADO_LIBRARYPROP_PUB  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_LIBRARYPROP_PUB.
   */
  protected $member_arr         = NULL;
  protected $libraryprop_pub_id = NULL;
  protected $libraryprop_id     = NULL;
  protected $pub_id             = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'libraryprop_pub_id' => 'serial',
    'libraryprop_id'     => 'integer',
    'pub_id'             => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'libraryprop_pub_id' => TRUE,
    'libraryprop_id'     => TRUE,
    'pub_id'             => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr         = $details;
    $this->libraryprop_pub_id = array_key_exists('libraryprop_pub_id', $details) ? $details['libraryprop_pub_id'] : '';
    $this->libraryprop_id     = array_key_exists('libraryprop_id', $details)     ? $details['libraryprop_id']     : '';
    $this->pub_id             = array_key_exists('pub_id', $details)             ? $details['pub_id']             : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_LIBRARYPROP_PUB by key(s).
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
    $sql = "SELECT * FROM chado.libraryprop_pub WHERE " . implode(" AND " , $where_arr);
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
    return libraryprop_pub_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'libraryprop_id' => array('fk_table' => 'libraryprop', 'fk_attr' => 'libraryprop_id'),
      'pub_id' => array('fk_table' => 'pub', 'fk_attr' => 'pub_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'libraryprop_pub_c1' => array('libraryprop_id','pub_id'),
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
        $fields['libraryprop_id'] = $this->libraryprop_id;
        $fields['pub_id']         = $this->pub_id;


        // Inserts the record.
        $this->libraryprop_pub_id = db_insert('chado.libraryprop_pub')
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
      $fields['libraryprop_id'] = $this->libraryprop_id;
      $fields['pub_id']         = $this->pub_id;


      // Updates the record.
      db_update('chado.libraryprop_pub')
        ->fields($fields)
        ->condition('libraryprop_pub_id', $this->libraryprop_pub_id, '=')
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

      db_delete('chado.libraryprop_pub')
        ->condition('libraryprop_pub_id', $this->libraryprop_pub_id, '=')
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
   * Retrieves the libraryprop_pub_id.
   *
   * @retrun serial
   */
  public function getLibrarypropPubID() {
    return $this->libraryprop_pub_id;
  }

  /**
   * Sets the libraryprop_pub_id.
   *
   * @param serial $libraryprop_pub_id
   */
  public function setLibrarypropPubID($libraryprop_pub_id) {
    $this->libraryprop_pub_id = $libraryprop_pub_id;
  }

  /**
   * Retrieves the libraryprop_id.
   *
   * @retrun integer
   */
  public function getLibrarypropID() {
    return $this->libraryprop_id;
  }

  /**
   * Sets the libraryprop_id.
   *
   * @param integer $libraryprop_id
   */
  public function setLibrarypropID($libraryprop_id) {
    $this->libraryprop_id = $libraryprop_id;
  }

  /**
   * Retrieves the pub_id.
   *
   * @retrun integer
   */
  public function getPubID() {
    return $this->pub_id;
  }

  /**
   * Sets the pub_id.
   *
   * @param integer $pub_id
   */
  public function setPubID($pub_id) {
    $this->pub_id = $pub_id;
  }
}