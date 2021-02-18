<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_BIOMATERIAL_TREATMENT class.
 *
 */
class CHADO_BIOMATERIAL_TREATMENT  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_BIOMATERIAL_TREATMENT.
   */
  protected $member_arr               = NULL;
  protected $biomaterial_treatment_id = NULL;
  protected $biomaterial_id           = NULL;
  protected $treatment_id             = NULL;
  protected $unittype_id              = NULL;
  protected $value                    = NULL;
  protected $rank                     = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'biomaterial_treatment_id' => 'serial',
    'biomaterial_id'           => 'integer',
    'treatment_id'             => 'integer',
    'unittype_id'              => 'integer',
    'value'                    => 'real',
    'rank'                     => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'biomaterial_treatment_id' => TRUE,
    'biomaterial_id'           => TRUE,
    'treatment_id'             => TRUE,
    'unittype_id'              => FALSE,
    'value'                    => FALSE,
    'rank'                     => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr               = $details;
    $this->biomaterial_treatment_id = array_key_exists('biomaterial_treatment_id', $details) ? $details['biomaterial_treatment_id'] : '';
    $this->biomaterial_id           = array_key_exists('biomaterial_id', $details)           ? $details['biomaterial_id']           : '';
    $this->treatment_id             = array_key_exists('treatment_id', $details)             ? $details['treatment_id']             : '';
    $this->unittype_id              = array_key_exists('unittype_id', $details)              ? $details['unittype_id']              : '';
    $this->value                    = array_key_exists('value', $details)                    ? $details['value']                    : '';
    $this->rank                     = array_key_exists('rank', $details)                     ? $details['rank']                     : 0;
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_BIOMATERIAL_TREATMENT by key(s).
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
    $sql = "SELECT * FROM chado.biomaterial_treatment WHERE " . implode(" AND " , $where_arr);
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
    return biomaterial_treatment_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'biomaterial_id' => array('fk_table' => 'biomaterial', 'fk_attr' => 'biomaterial_id'),
      'treatment_id' => array('fk_table' => 'treatment', 'fk_attr' => 'treatment_id'),
      'unittype_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'biomaterial_treatment_c1' => array('biomaterial_id','treatment_id'),
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
        $fields['biomaterial_id'] = $this->biomaterial_id;
        $fields['treatment_id']   = $this->treatment_id;
        $fields['rank']           = (is_numeric($this->rank)) ? $this->rank : 0;
        if (is_numeric($this->unittype_id)) { $fields['unittype_id'] = $this->unittype_id; }
        if (is_numeric($this->value))       { $fields['value']       = $this->value; }

        // Inserts the record.
        $this->biomaterial_treatment_id = db_insert('chado.biomaterial_treatment')
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
      $fields['biomaterial_id'] = $this->biomaterial_id;
      $fields['treatment_id']   = $this->treatment_id;
      $fields['rank']           = (is_numeric($this->rank)) ? $this->rank : 0;
      if (is_numeric($this->unittype_id)) { $fields['unittype_id'] = $this->unittype_id; }
      if (is_numeric($this->value))       { $fields['value']       = $this->value; }

      // Updates the record.
      db_update('chado.biomaterial_treatment')
        ->fields($fields)
        ->condition('biomaterial_treatment_id', $this->biomaterial_treatment_id, '=')
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

      db_delete('chado.biomaterial_treatment')
        ->condition('biomaterial_treatment_id', $this->biomaterial_treatment_id, '=')
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
   * Retrieves the biomaterial_treatment_id.
   *
   * @retrun serial
   */
  public function getBiomaterialTreatmentID() {
    return $this->biomaterial_treatment_id;
  }

  /**
   * Sets the biomaterial_treatment_id.
   *
   * @param serial $biomaterial_treatment_id
   */
  public function setBiomaterialTreatmentID($biomaterial_treatment_id) {
    $this->biomaterial_treatment_id = $biomaterial_treatment_id;
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
   * Retrieves the treatment_id.
   *
   * @retrun integer
   */
  public function getTreatmentID() {
    return $this->treatment_id;
  }

  /**
   * Sets the treatment_id.
   *
   * @param integer $treatment_id
   */
  public function setTreatmentID($treatment_id) {
    $this->treatment_id = $treatment_id;
  }

  /**
   * Retrieves the unittype_id.
   *
   * @retrun integer
   */
  public function getUnittypeID() {
    return $this->unittype_id;
  }

  /**
   * Sets the unittype_id.
   *
   * @param integer $unittype_id
   */
  public function setUnittypeID($unittype_id) {
    $this->unittype_id = $unittype_id;
  }

  /**
   * Retrieves the value.
   *
   * @retrun real
   */
  public function getValue() {
    return $this->value;
  }

  /**
   * Sets the value.
   *
   * @param real $value
   */
  public function setValue($value) {
    $this->value = $value;
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