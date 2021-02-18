<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_FEATUREPROP_PUB class.
 *
 */
class CHADO_FEATUREPROP_PUB  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_FEATUREPROP_PUB.
   */
  protected $member_arr         = NULL;
  protected $featureprop_pub_id = NULL;
  protected $featureprop_id     = NULL;
  protected $pub_id             = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'featureprop_pub_id' => 'serial',
    'featureprop_id'     => 'integer',
    'pub_id'             => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'featureprop_pub_id' => TRUE,
    'featureprop_id'     => TRUE,
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
    $this->featureprop_pub_id = array_key_exists('featureprop_pub_id', $details) ? $details['featureprop_pub_id'] : '';
    $this->featureprop_id     = array_key_exists('featureprop_id', $details)     ? $details['featureprop_id']     : '';
    $this->pub_id             = array_key_exists('pub_id', $details)             ? $details['pub_id']             : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_FEATUREPROP_PUB by key(s).
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
    $sql = "SELECT * FROM chado.featureprop_pub WHERE " . implode(" AND " , $where_arr);
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
    return featureprop_pub_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'featureprop_id' => array('fk_table' => 'featureprop', 'fk_attr' => 'featureprop_id'),
      'pub_id' => array('fk_table' => 'pub', 'fk_attr' => 'pub_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'featureprop_pub_c1' => array('featureprop_id','pub_id'),
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
        $fields['featureprop_id'] = $this->featureprop_id;
        $fields['pub_id']         = $this->pub_id;


        // Inserts the record.
        $this->featureprop_pub_id = db_insert('chado.featureprop_pub')
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
      $fields['featureprop_id'] = $this->featureprop_id;
      $fields['pub_id']         = $this->pub_id;


      // Updates the record.
      db_update('chado.featureprop_pub')
        ->fields($fields)
        ->condition('featureprop_pub_id', $this->featureprop_pub_id, '=')
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

      db_delete('chado.featureprop_pub')
        ->condition('featureprop_pub_id', $this->featureprop_pub_id, '=')
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
   * Retrieves the featureprop_pub_id.
   *
   * @retrun serial
   */
  public function getFeaturepropPubID() {
    return $this->featureprop_pub_id;
  }

  /**
   * Sets the featureprop_pub_id.
   *
   * @param serial $featureprop_pub_id
   */
  public function setFeaturepropPubID($featureprop_pub_id) {
    $this->featureprop_pub_id = $featureprop_pub_id;
  }

  /**
   * Retrieves the featureprop_id.
   *
   * @retrun integer
   */
  public function getFeaturepropID() {
    return $this->featureprop_id;
  }

  /**
   * Sets the featureprop_id.
   *
   * @param integer $featureprop_id
   */
  public function setFeaturepropID($featureprop_id) {
    $this->featureprop_id = $featureprop_id;
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