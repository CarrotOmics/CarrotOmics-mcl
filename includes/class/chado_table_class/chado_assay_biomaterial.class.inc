<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_ASSAY_BIOMATERIAL class.
 *
 */
class CHADO_ASSAY_BIOMATERIAL  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_ASSAY_BIOMATERIAL.
   */
  protected $member_arr           = NULL;
  protected $assay_biomaterial_id = NULL;
  protected $assay_id             = NULL;
  protected $biomaterial_id       = NULL;
  protected $channel_id           = NULL;
  protected $rank                 = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'assay_biomaterial_id' => 'serial',
    'assay_id'             => 'integer',
    'biomaterial_id'       => 'integer',
    'channel_id'           => 'integer',
    'rank'                 => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'assay_biomaterial_id' => TRUE,
    'assay_id'             => TRUE,
    'biomaterial_id'       => TRUE,
    'channel_id'           => FALSE,
    'rank'                 => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr           = $details;
    $this->assay_biomaterial_id = array_key_exists('assay_biomaterial_id', $details) ? $details['assay_biomaterial_id'] : '';
    $this->assay_id             = array_key_exists('assay_id', $details)             ? $details['assay_id']             : '';
    $this->biomaterial_id       = array_key_exists('biomaterial_id', $details)       ? $details['biomaterial_id']       : '';
    $this->channel_id           = array_key_exists('channel_id', $details)           ? $details['channel_id']           : '';
    $this->rank                 = array_key_exists('rank', $details)                 ? $details['rank']                 : 0;
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_ASSAY_BIOMATERIAL by key(s).
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
    $sql = "SELECT * FROM chado.assay_biomaterial WHERE " . implode(" AND " , $where_arr);
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
    return assay_biomaterial_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'assay_id' => array('fk_table' => 'assay', 'fk_attr' => 'assay_id'),
      'biomaterial_id' => array('fk_table' => 'biomaterial', 'fk_attr' => 'biomaterial_id'),
      'channel_id' => array('fk_table' => 'channel', 'fk_attr' => 'channel_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'assay_biomaterial_c1' => array('assay_id','biomaterial_id','channel_id','rank'),
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
        $fields['assay_id']       = $this->assay_id;
        $fields['biomaterial_id'] = $this->biomaterial_id;
        $fields['rank']           = (is_numeric($this->rank)) ? $this->rank : 0;
        if (is_numeric($this->channel_id)) { $fields['channel_id'] = $this->channel_id; }

        // Inserts the record.
        $this->assay_biomaterial_id = db_insert('chado.assay_biomaterial')
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
      $fields['assay_id']       = $this->assay_id;
      $fields['biomaterial_id'] = $this->biomaterial_id;
      $fields['rank']           = (is_numeric($this->rank)) ? $this->rank : 0;
      if (is_numeric($this->channel_id)) { $fields['channel_id'] = $this->channel_id; }

      // Updates the record.
      db_update('chado.assay_biomaterial')
        ->fields($fields)
        ->condition('assay_biomaterial_id', $this->assay_biomaterial_id, '=')
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

      db_delete('chado.assay_biomaterial')
        ->condition('assay_biomaterial_id', $this->assay_biomaterial_id, '=')
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
   * Retrieves the assay_biomaterial_id.
   *
   * @retrun serial
   */
  public function getAssayBiomaterialID() {
    return $this->assay_biomaterial_id;
  }

  /**
   * Sets the assay_biomaterial_id.
   *
   * @param serial $assay_biomaterial_id
   */
  public function setAssayBiomaterialID($assay_biomaterial_id) {
    $this->assay_biomaterial_id = $assay_biomaterial_id;
  }

  /**
   * Retrieves the assay_id.
   *
   * @retrun integer
   */
  public function getAssayID() {
    return $this->assay_id;
  }

  /**
   * Sets the assay_id.
   *
   * @param integer $assay_id
   */
  public function setAssayID($assay_id) {
    $this->assay_id = $assay_id;
  }

  /**
   * Retrieves the biomaterial_id.
   *
   * @retrun integer
   */
  public function getBiomaterialID() {
    return $this->biomaterial_id;
  }

  /**
   * Sets the biomaterial_id.
   *
   * @param integer $biomaterial_id
   */
  public function setBiomaterialID($biomaterial_id) {
    $this->biomaterial_id = $biomaterial_id;
  }

  /**
   * Retrieves the channel_id.
   *
   * @retrun integer
   */
  public function getChannelID() {
    return $this->channel_id;
  }

  /**
   * Sets the channel_id.
   *
   * @param integer $channel_id
   */
  public function setChannelID($channel_id) {
    $this->channel_id = $channel_id;
  }

  /**
   * Retrieves the rank.
   *
   * @retrun integer
   */
  public function getRank() {
    return $this->rank;
  }

  /**
   * Sets the rank.
   *
   * @param integer $rank
   */
  public function setRank($rank) {
    $this->rank = $rank;
  }
}