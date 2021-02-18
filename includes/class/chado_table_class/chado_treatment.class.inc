<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_TREATMENT class.
 *
 */
class CHADO_TREATMENT  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_TREATMENT.
   */
  protected $member_arr     = NULL;
  protected $treatment_id   = NULL;
  protected $rank           = NULL;
  protected $biomaterial_id = NULL;
  protected $type_id        = NULL;
  protected $protocol_id    = NULL;
  protected $name           = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'treatment_id'   => 'serial',
    'rank'           => 'integer',
    'biomaterial_id' => 'integer',
    'type_id'        => 'integer',
    'protocol_id'    => 'integer',
    'name'           => 'text',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'treatment_id'   => TRUE,
    'rank'           => TRUE,
    'biomaterial_id' => TRUE,
    'type_id'        => TRUE,
    'protocol_id'    => FALSE,
    'name'           => FALSE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr     = $details;
    $this->treatment_id   = array_key_exists('treatment_id', $details)   ? $details['treatment_id']   : '';
    $this->rank           = array_key_exists('rank', $details)           ? $details['rank']           : 0;
    $this->biomaterial_id = array_key_exists('biomaterial_id', $details) ? $details['biomaterial_id'] : '';
    $this->type_id        = array_key_exists('type_id', $details)        ? $details['type_id']        : '';
    $this->protocol_id    = array_key_exists('protocol_id', $details)    ? $details['protocol_id']    : '';
    $this->name           = array_key_exists('name', $details)           ? $details['name']           : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_TREATMENT by key(s).
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
    $sql = "SELECT * FROM chado.treatment WHERE " . implode(" AND " , $where_arr);
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
    return treatment_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'biomaterial_id' => array('fk_table' => 'biomaterial', 'fk_attr' => 'biomaterial_id'),
      'protocol_id' => array('fk_table' => 'protocol', 'fk_attr' => 'protocol_id'),
      'type_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
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
    return array(
      'treatment_id' => array(
        array('table' => 'biomaterial_treatment', 'attr' => 'treatment_id'),
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
        $fields['rank']           = (is_numeric($this->rank)) ? $this->rank : 0;
        $fields['biomaterial_id'] = $this->biomaterial_id;
        $fields['type_id']        = $this->type_id;
        $fields['name']           = $this->name;
        if (is_numeric($this->protocol_id)) { $fields['protocol_id'] = $this->protocol_id; }

        // Inserts the record.
        $this->treatment_id = db_insert('chado.treatment')
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
      $fields['rank']           = (is_numeric($this->rank)) ? $this->rank : 0;
      $fields['biomaterial_id'] = $this->biomaterial_id;
      $fields['type_id']        = $this->type_id;
      $fields['name']           = $this->name;
      if (is_numeric($this->protocol_id)) { $fields['protocol_id'] = $this->protocol_id; }

      // Updates the record.
      db_update('chado.treatment')
        ->fields($fields)
        ->condition('treatment_id', $this->treatment_id, '=')
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

      db_delete('chado.treatment')
        ->condition('treatment_id', $this->treatment_id, '=')
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
   * Retrieves the treatment_id.
   *
   * @retrun serial
   */
  public function getTreatmentID() {
    return $this->treatment_id;
  }

  /**
   * Sets the treatment_id.
   *
   * @param serial $treatment_id
   */
  public function setTreatmentID($treatment_id) {
    $this->treatment_id = $treatment_id;
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
   * Retrieves the type_id.
   *
   * @retrun integer
   */
  public function getTypeID() {
    return $this->type_id;
  }

  /**
   * Sets the type_id.
   *
   * @param integer $type_id
   */
  public function setTypeID($type_id) {
    $this->type_id = $type_id;
  }

  /**
   * Retrieves the protocol_id.
   *
   * @retrun integer
   */
  public function getProtocolID() {
    return $this->protocol_id;
  }

  /**
   * Sets the protocol_id.
   *
   * @param integer $protocol_id
   */
  public function setProtocolID($protocol_id) {
    $this->protocol_id = $protocol_id;
  }

  /**
   * Retrieves the name.
   *
   * @retrun text
   */
  public function getName() {
    return $this->name;
  }

  /**
   * Sets the name.
   *
   * @param text $name
   */
  public function setName($name) {
    $this->name = $name;
  }
}