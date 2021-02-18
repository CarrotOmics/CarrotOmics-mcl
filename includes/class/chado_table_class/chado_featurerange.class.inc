<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_FEATURERANGE class.
 *
 */
class CHADO_FEATURERANGE  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_FEATURERANGE.
   */
  protected $member_arr      = NULL;
  protected $featurerange_id = NULL;
  protected $featuremap_id   = NULL;
  protected $feature_id      = NULL;
  protected $leftstartf_id   = NULL;
  protected $leftendf_id     = NULL;
  protected $rightstartf_id  = NULL;
  protected $rightendf_id    = NULL;
  protected $rangestr        = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'featurerange_id' => 'serial',
    'featuremap_id'   => 'integer',
    'feature_id'      => 'integer',
    'leftstartf_id'   => 'integer',
    'leftendf_id'     => 'integer',
    'rightstartf_id'  => 'integer',
    'rightendf_id'    => 'integer',
    'rangestr'        => 'character_varying(255)',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'featurerange_id' => TRUE,
    'featuremap_id'   => TRUE,
    'feature_id'      => TRUE,
    'leftstartf_id'   => TRUE,
    'leftendf_id'     => FALSE,
    'rightstartf_id'  => FALSE,
    'rightendf_id'    => TRUE,
    'rangestr'        => FALSE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr      = $details;
    $this->featurerange_id = array_key_exists('featurerange_id', $details) ? $details['featurerange_id'] : '';
    $this->featuremap_id   = array_key_exists('featuremap_id', $details)   ? $details['featuremap_id']   : '';
    $this->feature_id      = array_key_exists('feature_id', $details)      ? $details['feature_id']      : '';
    $this->leftstartf_id   = array_key_exists('leftstartf_id', $details)   ? $details['leftstartf_id']   : '';
    $this->leftendf_id     = array_key_exists('leftendf_id', $details)     ? $details['leftendf_id']     : '';
    $this->rightstartf_id  = array_key_exists('rightstartf_id', $details)  ? $details['rightstartf_id']  : '';
    $this->rightendf_id    = array_key_exists('rightendf_id', $details)    ? $details['rightendf_id']    : '';
    $this->rangestr        = array_key_exists('rangestr', $details)        ? $details['rangestr']        : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_FEATURERANGE by key(s).
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
    $sql = "SELECT * FROM chado.featurerange WHERE " . implode(" AND " , $where_arr);
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
    return featurerange_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'feature_id' => array('fk_table' => 'feature', 'fk_attr' => 'feature_id'),
      'featuremap_id' => array('fk_table' => 'featuremap', 'fk_attr' => 'featuremap_id'),
      'leftendf_id' => array('fk_table' => 'feature', 'fk_attr' => 'feature_id'),
      'leftstartf_id' => array('fk_table' => 'feature', 'fk_attr' => 'feature_id'),
      'rightendf_id' => array('fk_table' => 'feature', 'fk_attr' => 'feature_id'),
      'rightstartf_id' => array('fk_table' => 'feature', 'fk_attr' => 'feature_id'),
    );
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
        $fields['featuremap_id'] = $this->featuremap_id;
        $fields['feature_id']    = $this->feature_id;
        $fields['leftstartf_id'] = $this->leftstartf_id;
        $fields['rightendf_id']  = $this->rightendf_id;
        $fields['rangestr']      = $this->rangestr;
        if (is_numeric($this->leftendf_id))    { $fields['leftendf_id']    = $this->leftendf_id; }
        if (is_numeric($this->rightstartf_id)) { $fields['rightstartf_id'] = $this->rightstartf_id; }

        // Inserts the record.
        $this->featurerange_id = db_insert('chado.featurerange')
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
      $fields['featuremap_id'] = $this->featuremap_id;
      $fields['feature_id']    = $this->feature_id;
      $fields['leftstartf_id'] = $this->leftstartf_id;
      $fields['rightendf_id']  = $this->rightendf_id;
      $fields['rangestr']      = $this->rangestr;
      if (is_numeric($this->leftendf_id))    { $fields['leftendf_id']    = $this->leftendf_id; }
      if (is_numeric($this->rightstartf_id)) { $fields['rightstartf_id'] = $this->rightstartf_id; }

      // Updates the record.
      db_update('chado.featurerange')
        ->fields($fields)
        ->condition('featurerange_id', $this->featurerange_id, '=')
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

      db_delete('chado.featurerange')
        ->condition('featurerange_id', $this->featurerange_id, '=')
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
   * Retrieves the featurerange_id.
   *
   * @retrun serial
   */
  public function getFeaturerangeID() {
    return $this->featurerange_id;
  }

  /**
   * Sets the featurerange_id.
   *
   * @param serial $featurerange_id
   */
  public function setFeaturerangeID($featurerange_id) {
    $this->featurerange_id = $featurerange_id;
  }

  /**
   * Retrieves the featuremap_id.
   *
   * @retrun integer
   */
  public function getFeaturemapID() {
    return $this->featuremap_id;
  }

  /**
   * Sets the featuremap_id.
   *
   * @param integer $featuremap_id
   */
  public function setFeaturemapID($featuremap_id) {
    $this->featuremap_id = $featuremap_id;
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

  /**
   * Retrieves the leftstartf_id.
   *
   * @retrun integer
   */
  public function getLeftstartfID() {
    return $this->leftstartf_id;
  }

  /**
   * Sets the leftstartf_id.
   *
   * @param integer $leftstartf_id
   */
  public function setLeftstartfID($leftstartf_id) {
    $this->leftstartf_id = $leftstartf_id;
  }

  /**
   * Retrieves the leftendf_id.
   *
   * @retrun integer
   */
  public function getLeftendfID() {
    return $this->leftendf_id;
  }

  /**
   * Sets the leftendf_id.
   *
   * @param integer $leftendf_id
   */
  public function setLeftendfID($leftendf_id) {
    $this->leftendf_id = $leftendf_id;
  }

  /**
   * Retrieves the rightstartf_id.
   *
   * @retrun integer
   */
  public function getRightstartfID() {
    return $this->rightstartf_id;
  }

  /**
   * Sets the rightstartf_id.
   *
   * @param integer $rightstartf_id
   */
  public function setRightstartfID($rightstartf_id) {
    $this->rightstartf_id = $rightstartf_id;
  }

  /**
   * Retrieves the rightendf_id.
   *
   * @retrun integer
   */
  public function getRightendfID() {
    return $this->rightendf_id;
  }

  /**
   * Sets the rightendf_id.
   *
   * @param integer $rightendf_id
   */
  public function setRightendfID($rightendf_id) {
    $this->rightendf_id = $rightendf_id;
  }

  /**
   * Retrieves the rangestr.
   *
   * @retrun character_varying(255)
   */
  public function getRangestr() {
    return $this->rangestr;
  }

  /**
   * Sets the rangestr.
   *
   * @param character_varying(255) $rangestr
   */
  public function setRangestr($rangestr) {
    $this->rangestr = $rangestr;
  }
}