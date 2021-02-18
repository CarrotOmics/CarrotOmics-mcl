<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_ND_REAGENT class.
 *
 */
class CHADO_ND_REAGENT  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_ND_REAGENT.
   */
  protected $member_arr    = NULL;
  protected $nd_reagent_id = NULL;
  protected $name          = NULL;
  protected $type_id       = NULL;
  protected $feature_id    = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'nd_reagent_id' => 'serial',
    'name'          => 'character_varying(80)',
    'type_id'       => 'integer',
    'feature_id'    => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'nd_reagent_id' => TRUE,
    'name'          => TRUE,
    'type_id'       => TRUE,
    'feature_id'    => FALSE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr    = $details;
    $this->nd_reagent_id = array_key_exists('nd_reagent_id', $details) ? $details['nd_reagent_id'] : '';
    $this->name          = array_key_exists('name', $details)          ? $details['name']          : '';
    $this->type_id       = array_key_exists('type_id', $details)       ? $details['type_id']       : '';
    $this->feature_id    = array_key_exists('feature_id', $details)    ? $details['feature_id']    : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_ND_REAGENT by key(s).
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
    $sql = "SELECT * FROM chado.nd_reagent WHERE " . implode(" AND " , $where_arr);
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
    return nd_reagent_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
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
      'nd_reagent_id' => array(
        array('table' => 'nd_protocol_reagent', 'attr' => 'reagent_id'),
        array('table' => 'nd_reagent_relationship', 'attr' => 'object_reagent_id'),
        array('table' => 'nd_reagent_relationship', 'attr' => 'subject_reagent_id'),
        array('table' => 'nd_reagentprop', 'attr' => 'nd_reagent_id'),
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
        $fields['name']    = $this->name;
        $fields['type_id'] = $this->type_id;
        if (is_numeric($this->feature_id)) { $fields['feature_id'] = $this->feature_id; }

        // Inserts the record.
        $this->nd_reagent_id = db_insert('chado.nd_reagent')
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
      $fields['name']    = $this->name;
      $fields['type_id'] = $this->type_id;
      if (is_numeric($this->feature_id)) { $fields['feature_id'] = $this->feature_id; }

      // Updates the record.
      db_update('chado.nd_reagent')
        ->fields($fields)
        ->condition('nd_reagent_id', $this->nd_reagent_id, '=')
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

      db_delete('chado.nd_reagent')
        ->condition('nd_reagent_id', $this->nd_reagent_id, '=')
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
   * Retrieves the nd_reagent_id.
   *
   * @retrun serial
   */
  public function getNdReagentID() {
    return $this->nd_reagent_id;
  }

  /**
   * Sets the nd_reagent_id.
   *
   * @param serial $nd_reagent_id
   */
  public function setNdReagentID($nd_reagent_id) {
    $this->nd_reagent_id = $nd_reagent_id;
  }

  /**
   * Retrieves the name.
   *
   * @retrun character_varying(80)
   */
  public function getName() {
    return $this->name;
  }

  /**
   * Sets the name.
   *
   * @param character_varying(80) $name
   */
  public function setName($name) {
    $this->name = $name;
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