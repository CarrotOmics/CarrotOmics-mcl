<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_MAGEML class.
 *
 */
class CHADO_MAGEML  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_MAGEML.
   */
  protected $member_arr   = NULL;
  protected $mageml_id    = NULL;
  protected $mage_package = NULL;
  protected $mage_ml      = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'mageml_id'    => 'serial',
    'mage_package' => 'text',
    'mage_ml'      => 'text',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'mageml_id'    => TRUE,
    'mage_package' => TRUE,
    'mage_ml'      => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr   = $details;
    $this->mageml_id    = array_key_exists('mageml_id', $details)    ? $details['mageml_id']    : '';
    $this->mage_package = array_key_exists('mage_package', $details) ? $details['mage_package'] : '';
    $this->mage_ml      = array_key_exists('mage_ml', $details)      ? $details['mage_ml']      : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_MAGEML by key(s).
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
    $sql = "SELECT * FROM chado.mageml WHERE " . implode(" AND " , $where_arr);
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
    return mageml_id;
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
    return NULL;
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'mageml_id' => array(
        array('table' => 'magedocumentation', 'attr' => 'mageml_id'),
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
        $fields['mage_package'] = $this->mage_package;
        $fields['mage_ml']      = $this->mage_ml;


        // Inserts the record.
        $this->mageml_id = db_insert('chado.mageml')
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
      $fields['mage_package'] = $this->mage_package;
      $fields['mage_ml']      = $this->mage_ml;


      // Updates the record.
      db_update('chado.mageml')
        ->fields($fields)
        ->condition('mageml_id', $this->mageml_id, '=')
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

      db_delete('chado.mageml')
        ->condition('mageml_id', $this->mageml_id, '=')
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
   * Retrieves the mageml_id.
   *
   * @retrun serial
   */
  public function getMagemlID() {
    return $this->mageml_id;
  }

  /**
   * Sets the mageml_id.
   *
   * @param serial $mageml_id
   */
  public function setMagemlID($mageml_id) {
    $this->mageml_id = $mageml_id;
  }

  /**
   * Retrieves the mage_package.
   *
   * @retrun text
   */
  public function getMagePackage() {
    return $this->mage_package;
  }

  /**
   * Sets the mage_package.
   *
   * @param text $mage_package
   */
  public function setMagePackage($mage_package) {
    $this->mage_package = $mage_package;
  }

  /**
   * Retrieves the mage_ml.
   *
   * @retrun text
   */
  public function getMageMl() {
    return $this->mage_ml;
  }

  /**
   * Sets the mage_ml.
   *
   * @param text $mage_ml
   */
  public function setMageMl($mage_ml) {
    $this->mage_ml = $mage_ml;
  }
}