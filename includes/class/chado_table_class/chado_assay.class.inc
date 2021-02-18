<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_ASSAY class.
 *
 */
class CHADO_ASSAY  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_ASSAY.
   */
  protected $member_arr           = NULL;
  protected $assay_id             = NULL;
  protected $arraydesign_id       = NULL;
  protected $protocol_id          = NULL;
  protected $assaydate            = NULL;
  protected $arrayidentifier      = NULL;
  protected $arraybatchidentifier = NULL;
  protected $operator_id          = NULL;
  protected $dbxref_id            = NULL;
  protected $name                 = NULL;
  protected $description          = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'assay_id'             => 'serial',
    'arraydesign_id'       => 'integer',
    'protocol_id'          => 'integer',
    'assaydate'            => 'timestamp_without_time_zone',
    'arrayidentifier'      => 'text',
    'arraybatchidentifier' => 'text',
    'operator_id'          => 'integer',
    'dbxref_id'            => 'integer',
    'name'                 => 'text',
    'description'          => 'text',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'assay_id'             => TRUE,
    'arraydesign_id'       => TRUE,
    'protocol_id'          => FALSE,
    'assaydate'            => FALSE,
    'arrayidentifier'      => FALSE,
    'arraybatchidentifier' => FALSE,
    'operator_id'          => TRUE,
    'dbxref_id'            => FALSE,
    'name'                 => FALSE,
    'description'          => FALSE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr           = $details;
    $this->assay_id             = array_key_exists('assay_id', $details)             ? $details['assay_id']             : '';
    $this->arraydesign_id       = array_key_exists('arraydesign_id', $details)       ? $details['arraydesign_id']       : '';
    $this->protocol_id          = array_key_exists('protocol_id', $details)          ? $details['protocol_id']          : '';
    $this->assaydate            = array_key_exists('assaydate', $details)            ? $details['assaydate']            : NULL;
    $this->arrayidentifier      = array_key_exists('arrayidentifier', $details)      ? $details['arrayidentifier']      : '';
    $this->arraybatchidentifier = array_key_exists('arraybatchidentifier', $details) ? $details['arraybatchidentifier'] : '';
    $this->operator_id          = array_key_exists('operator_id', $details)          ? $details['operator_id']          : '';
    $this->dbxref_id            = array_key_exists('dbxref_id', $details)            ? $details['dbxref_id']            : '';
    $this->name                 = array_key_exists('name', $details)                 ? $details['name']                 : '';
    $this->description          = array_key_exists('description', $details)          ? $details['description']          : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_ASSAY by key(s).
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
    $sql = "SELECT * FROM chado.assay WHERE " . implode(" AND " , $where_arr);
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
    return assay_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'arraydesign_id' => array('fk_table' => 'arraydesign', 'fk_attr' => 'arraydesign_id'),
      'dbxref_id' => array('fk_table' => 'dbxref', 'fk_attr' => 'dbxref_id'),
      'operator_id' => array('fk_table' => 'contact', 'fk_attr' => 'contact_id'),
      'protocol_id' => array('fk_table' => 'protocol', 'fk_attr' => 'protocol_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'assay_c1' => array('name'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'assay_id' => array(
        array('table' => 'acquisition', 'attr' => 'assay_id'),
        array('table' => 'assay_biomaterial', 'attr' => 'assay_id'),
        array('table' => 'assay_project', 'attr' => 'assay_id'),
        array('table' => 'assayprop', 'attr' => 'assay_id'),
        array('table' => 'control', 'attr' => 'assay_id'),
        array('table' => 'study_assay', 'attr' => 'assay_id'),
        array('table' => 'studyfactorvalue', 'attr' => 'assay_id'),
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
        $fields['arraydesign_id']       = $this->arraydesign_id;
        $fields['arrayidentifier']      = $this->arrayidentifier;
        $fields['arraybatchidentifier'] = $this->arraybatchidentifier;
        $fields['operator_id']          = $this->operator_id;
        $fields['name']                 = $this->name;
        $fields['description']          = $this->description;
        if (is_numeric($this->protocol_id)) { $fields['protocol_id'] = $this->protocol_id; }
        if (!empty($this->assaydate))       { $fields['assaydate']   = $this->assaydate; }
        if (is_numeric($this->dbxref_id))   { $fields['dbxref_id']   = $this->dbxref_id; }

        // Inserts the record.
        $this->assay_id = db_insert('chado.assay')
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
      $fields['arraydesign_id']       = $this->arraydesign_id;
      $fields['arrayidentifier']      = $this->arrayidentifier;
      $fields['arraybatchidentifier'] = $this->arraybatchidentifier;
      $fields['operator_id']          = $this->operator_id;
      $fields['name']                 = $this->name;
      $fields['description']          = $this->description;
      if (is_numeric($this->protocol_id)) { $fields['protocol_id'] = $this->protocol_id; }
      if (!empty($this->assaydate))       { $fields['assaydate']   = $this->assaydate; }
      if (is_numeric($this->dbxref_id))   { $fields['dbxref_id']   = $this->dbxref_id; }

      // Updates the record.
      db_update('chado.assay')
        ->fields($fields)
        ->condition('assay_id', $this->assay_id, '=')
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

      db_delete('chado.assay')
        ->condition('assay_id', $this->assay_id, '=')
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
   * Retrieves the assay_id.
   *
   * @retrun serial
   */
  public function getAssayID() {
    return $this->assay_id;
  }

  /**
   * Sets the assay_id.
   *
   * @param serial $assay_id
   */
  public function setAssayID($assay_id) {
    $this->assay_id = $assay_id;
  }

  /**
   * Retrieves the arraydesign_id.
   *
   * @retrun integer
   */
  public function getArraydesignID() {
    return $this->arraydesign_id;
  }

  /**
   * Sets the arraydesign_id.
   *
   * @param integer $arraydesign_id
   */
  public function setArraydesignID($arraydesign_id) {
    $this->arraydesign_id = $arraydesign_id;
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
   * Retrieves the assaydate.
   *
   * @retrun timestamp_without_time_zone
   */
  public function getAssaydate() {
    return $this->assaydate;
  }

  /**
   * Sets the assaydate.
   *
   * @param timestamp_without_time_zone $assaydate
   */
  public function setAssaydate($assaydate) {
    $this->assaydate = $assaydate;
  }

  /**
   * Retrieves the arrayidentifier.
   *
   * @retrun text
   */
  public function getArrayidentifier() {
    return $this->arrayidentifier;
  }

  /**
   * Sets the arrayidentifier.
   *
   * @param text $arrayidentifier
   */
  public function setArrayidentifier($arrayidentifier) {
    $this->arrayidentifier = $arrayidentifier;
  }

  /**
   * Retrieves the arraybatchidentifier.
   *
   * @retrun text
   */
  public function getArraybatchidentifier() {
    return $this->arraybatchidentifier;
  }

  /**
   * Sets the arraybatchidentifier.
   *
   * @param text $arraybatchidentifier
   */
  public function setArraybatchidentifier($arraybatchidentifier) {
    $this->arraybatchidentifier = $arraybatchidentifier;
  }

  /**
   * Retrieves the operator_id.
   *
   * @retrun integer
   */
  public function getOperatorID() {
    return $this->operator_id;
  }

  /**
   * Sets the operator_id.
   *
   * @param integer $operator_id
   */
  public function setOperatorID($operator_id) {
    $this->operator_id = $operator_id;
  }

  /**
   * Retrieves the dbxref_id.
   *
   * @retrun integer
   */
  public function getDbxrefID() {
    return $this->dbxref_id;
  }

  /**
   * Sets the dbxref_id.
   *
   * @param integer $dbxref_id
   */
  public function setDbxrefID($dbxref_id) {
    $this->dbxref_id = $dbxref_id;
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

  /**
   * Retrieves the description.
   *
   * @retrun text
   */
  public function getDescription() {
    return $this->description;
  }

  /**
   * Sets the description.
   *
   * @param text $description
   */
  public function setDescription($description) {
    $this->description = $description;
  }
}