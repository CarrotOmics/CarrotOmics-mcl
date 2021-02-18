<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_LIBRARY_FEATURE class.
 *
 */
class CHADO_LIBRARY_FEATURE  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_LIBRARY_FEATURE.
   */
  protected $member_arr         = NULL;
  protected $library_feature_id = NULL;
  protected $library_id         = NULL;
  protected $feature_id         = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'library_feature_id' => 'serial',
    'library_id'         => 'integer',
    'feature_id'         => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'library_feature_id' => TRUE,
    'library_id'         => TRUE,
    'feature_id'         => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr         = $details;
    $this->library_feature_id = array_key_exists('library_feature_id', $details) ? $details['library_feature_id'] : '';
    $this->library_id         = array_key_exists('library_id', $details)         ? $details['library_id']         : '';
    $this->feature_id         = array_key_exists('feature_id', $details)         ? $details['feature_id']         : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_LIBRARY_FEATURE by key(s).
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
    $sql = "SELECT * FROM chado.library_feature WHERE " . implode(" AND " , $where_arr);
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
    return library_feature_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'feature_id' => array('fk_table' => 'feature', 'fk_attr' => 'feature_id'),
      'library_id' => array('fk_table' => 'library', 'fk_attr' => 'library_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'library_feature_c1' => array('library_id','feature_id'),
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
        $fields['library_id'] = $this->library_id;
        $fields['feature_id'] = $this->feature_id;


        // Inserts the record.
        $this->library_feature_id = db_insert('chado.library_feature')
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
      $fields['library_id'] = $this->library_id;
      $fields['feature_id'] = $this->feature_id;


      // Updates the record.
      db_update('chado.library_feature')
        ->fields($fields)
        ->condition('library_feature_id', $this->library_feature_id, '=')
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

      db_delete('chado.library_feature')
        ->condition('library_feature_id', $this->library_feature_id, '=')
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
   * Retrieves the library_feature_id.
   *
   * @retrun serial
   */
  public function getLibraryFeatureID() {
    return $this->library_feature_id;
  }

  /**
   * Sets the library_feature_id.
   *
   * @param serial $library_feature_id
   */
  public function setLibraryFeatureID($library_feature_id) {
    $this->library_feature_id = $library_feature_id;
  }

  /**
   * Retrieves the library_id.
   *
   * @retrun integer
   */
  public function getLibraryID() {
    return $this->library_id;
  }

  /**
   * Sets the library_id.
   *
   * @param integer $library_id
   */
  public function setLibraryID($library_id) {
    $this->library_id = $library_id;
  }

  /**
   * Retrieves the feature_id.
   *
   * @retrun integer
   */
  public function getFeatureID() {
    return $this->feature_id;
  }

  /**
   * Sets the feature_id.
   *
   * @param integer $feature_id
   */
  public function setFeatureID($feature_id) {
    $this->feature_id = $feature_id;
  }
}