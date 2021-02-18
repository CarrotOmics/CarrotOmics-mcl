<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_PHENOTYPE_CALL class.
 *
 */
class CHADO_PHENOTYPE_CALL  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_PHENOTYPE_CALL.
   */
  protected $member_arr        = NULL;
  protected $phenotype_call_id = NULL;
  protected $project_id        = NULL;
  protected $stock_id          = NULL;
  protected $cvterm_id         = NULL;
  protected $phenotype_id      = NULL;
  protected $nd_geolocation_id = NULL;
  protected $time              = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'phenotype_call_id' => 'serial',
    'project_id'        => 'integer',
    'stock_id'          => 'integer',
    'cvterm_id'         => 'integer',
    'phenotype_id'      => 'integer',
    'nd_geolocation_id' => 'integer',
    'time'              => 'timestamp_without_time_zone',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'phenotype_call_id' => TRUE,
    'project_id'        => TRUE,
    'stock_id'          => TRUE,
    'cvterm_id'         => TRUE,
    'phenotype_id'      => TRUE,
    'nd_geolocation_id' => TRUE,
    'time'              => FALSE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr        = $details;
    $this->phenotype_call_id = array_key_exists('phenotype_call_id', $details) ? $details['phenotype_call_id'] : '';
    $this->project_id        = array_key_exists('project_id', $details)        ? $details['project_id']        : '';
    $this->stock_id          = array_key_exists('stock_id', $details)          ? $details['stock_id']          : '';
    $this->cvterm_id         = array_key_exists('cvterm_id', $details)         ? $details['cvterm_id']         : '';
    $this->phenotype_id      = array_key_exists('phenotype_id', $details)      ? $details['phenotype_id']      : '';
    $this->nd_geolocation_id = array_key_exists('nd_geolocation_id', $details) ? $details['nd_geolocation_id'] : '';
    $this->time              = array_key_exists('time', $details)              ? $details['time']              : NULL;
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_PHENOTYPE_CALL by key(s).
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
    $sql = "SELECT * FROM chado.phenotype_call WHERE " . implode(" AND " , $where_arr);
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
    return phenotype_call_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'cvterm_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
      'nd_geolocation_id' => array('fk_table' => 'nd_geolocation', 'fk_attr' => 'nd_geolocation_id'),
      'phenotype_id' => array('fk_table' => 'phenotype', 'fk_attr' => 'phenotype_id'),
      'project_id' => array('fk_table' => 'project', 'fk_attr' => 'project_id'),
      'stock_id' => array('fk_table' => 'stock', 'fk_attr' => 'stock_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'phenotype_call_c1' => array('project_id','stock_id','cvterm_id','nd_geolocation_id'),
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
        $fields['project_id']        = $this->project_id;
        $fields['stock_id']          = $this->stock_id;
        $fields['cvterm_id']         = $this->cvterm_id;
        $fields['phenotype_id']      = $this->phenotype_id;
        $fields['nd_geolocation_id'] = $this->nd_geolocation_id;
        if (!empty($this->time)) { $fields['time'] = $this->time; }

        // Inserts the record.
        $this->phenotype_call_id = db_insert('chado.phenotype_call')
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
      $fields['project_id']        = $this->project_id;
      $fields['stock_id']          = $this->stock_id;
      $fields['cvterm_id']         = $this->cvterm_id;
      $fields['phenotype_id']      = $this->phenotype_id;
      $fields['nd_geolocation_id'] = $this->nd_geolocation_id;
      if (!empty($this->time)) { $fields['time'] = $this->time; }

      // Updates the record.
      db_update('chado.phenotype_call')
        ->fields($fields)
        ->condition('phenotype_call_id', $this->phenotype_call_id, '=')
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

      db_delete('chado.phenotype_call')
        ->condition('phenotype_call_id', $this->phenotype_call_id, '=')
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
   * Retrieves the phenotype_call_id.
   *
   * @retrun serial
   */
  public function getPhenotypeCallID() {
    return $this->phenotype_call_id;
  }

  /**
   * Sets the phenotype_call_id.
   *
   * @param serial $phenotype_call_id
   */
  public function setPhenotypeCallID($phenotype_call_id) {
    $this->phenotype_call_id = $phenotype_call_id;
  }

  /**
   * Retrieves the project_id.
   *
   * @retrun integer
   */
  public function getProjectID() {
    return $this->project_id;
  }

  /**
   * Sets the project_id.
   *
   * @param integer $project_id
   */
  public function setProjectID($project_id) {
    $this->project_id = $project_id;
  }

  /**
   * Retrieves the stock_id.
   *
   * @retrun integer
   */
  public function getStockID() {
    return $this->stock_id;
  }

  /**
   * Sets the stock_id.
   *
   * @param integer $stock_id
   */
  public function setStockID($stock_id) {
    $this->stock_id = $stock_id;
  }

  /**
   * Retrieves the cvterm_id.
   *
   * @retrun integer
   */
  public function getCvtermID() {
    return $this->cvterm_id;
  }

  /**
   * Sets the cvterm_id.
   *
   * @param integer $cvterm_id
   */
  public function setCvtermID($cvterm_id) {
    $this->cvterm_id = $cvterm_id;
  }

  /**
   * Retrieves the phenotype_id.
   *
   * @retrun integer
   */
  public function getPhenotypeID() {
    return $this->phenotype_id;
  }

  /**
   * Sets the phenotype_id.
   *
   * @param integer $phenotype_id
   */
  public function setPhenotypeID($phenotype_id) {
    $this->phenotype_id = $phenotype_id;
  }

  /**
   * Retrieves the nd_geolocation_id.
   *
   * @retrun integer
   */
  public function getNdGeolocationID() {
    return $this->nd_geolocation_id;
  }

  /**
   * Sets the nd_geolocation_id.
   *
   * @param integer $nd_geolocation_id
   */
  public function setNdGeolocationID($nd_geolocation_id) {
    $this->nd_geolocation_id = $nd_geolocation_id;
  }

  /**
   * Retrieves the time.
   *
   * @retrun timestamp_without_time_zone
   */
  public function getTime() {
    return $this->time;
  }

  /**
   * Sets the time.
   *
   * @param timestamp_without_time_zone $time
   */
  public function setTime($time) {
    $this->time = $time;
  }
}