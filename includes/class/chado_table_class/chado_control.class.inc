<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_CONTROL class.
 *
 */
class CHADO_CONTROL  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_CONTROL.
   */
  protected $member_arr   = NULL;
  protected $control_id   = NULL;
  protected $type_id      = NULL;
  protected $assay_id     = NULL;
  protected $tableinfo_id = NULL;
  protected $row_id       = NULL;
  protected $name         = NULL;
  protected $value        = NULL;
  protected $rank         = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'control_id'   => 'serial',
    'type_id'      => 'integer',
    'assay_id'     => 'integer',
    'tableinfo_id' => 'integer',
    'row_id'       => 'integer',
    'name'         => 'text',
    'value'        => 'text',
    'rank'         => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'control_id'   => TRUE,
    'type_id'      => TRUE,
    'assay_id'     => TRUE,
    'tableinfo_id' => TRUE,
    'row_id'       => TRUE,
    'name'         => FALSE,
    'value'        => FALSE,
    'rank'         => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr   = $details;
    $this->control_id   = array_key_exists('control_id', $details)   ? $details['control_id']   : '';
    $this->type_id      = array_key_exists('type_id', $details)      ? $details['type_id']      : '';
    $this->assay_id     = array_key_exists('assay_id', $details)     ? $details['assay_id']     : '';
    $this->tableinfo_id = array_key_exists('tableinfo_id', $details) ? $details['tableinfo_id'] : '';
    $this->row_id       = array_key_exists('row_id', $details)       ? $details['row_id']       : '';
    $this->name         = array_key_exists('name', $details)         ? $details['name']         : '';
    $this->value        = array_key_exists('value', $details)        ? $details['value']        : '';
    $this->rank         = array_key_exists('rank', $details)         ? $details['rank']         : 0;
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_CONTROL by key(s).
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
    $sql = "SELECT * FROM chado.control WHERE " . implode(" AND " , $where_arr);
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
    return control_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'assay_id' => array('fk_table' => 'assay', 'fk_attr' => 'assay_id'),
      'tableinfo_id' => array('fk_table' => 'tableinfo', 'fk_attr' => 'tableinfo_id'),
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
        $fields['type_id']      = $this->type_id;
        $fields['assay_id']     = $this->assay_id;
        $fields['tableinfo_id'] = $this->tableinfo_id;
        $fields['row_id']       = $this->row_id;
        $fields['name']         = $this->name;
        $fields['value']        = $this->value;
        $fields['rank']         = (is_numeric($this->rank)) ? $this->rank : 0;


        // Inserts the record.
        $this->control_id = db_insert('chado.control')
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
      $fields['type_id']      = $this->type_id;
      $fields['assay_id']     = $this->assay_id;
      $fields['tableinfo_id'] = $this->tableinfo_id;
      $fields['row_id']       = $this->row_id;
      $fields['name']         = $this->name;
      $fields['value']        = $this->value;
      $fields['rank']         = (is_numeric($this->rank)) ? $this->rank : 0;


      // Updates the record.
      db_update('chado.control')
        ->fields($fields)
        ->condition('control_id', $this->control_id, '=')
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

      db_delete('chado.control')
        ->condition('control_id', $this->control_id, '=')
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
   * Retrieves the control_id.
   *
   * @retrun serial
   */
  public function getControlID() {
    return $this->control_id;
  }

  /**
   * Sets the control_id.
   *
   * @param serial $control_id
   */
  public function setControlID($control_id) {
    $this->control_id = $control_id;
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
   * Retrieves the tableinfo_id.
   *
   * @retrun integer
   */
  public function getTableinfoID() {
    return $this->tableinfo_id;
  }

  /**
   * Sets the tableinfo_id.
   *
   * @param integer $tableinfo_id
   */
  public function setTableinfoID($tableinfo_id) {
    $this->tableinfo_id = $tableinfo_id;
  }

  /**
   * Retrieves the row_id.
   *
   * @retrun integer
   */
  public function getRowID() {
    return $this->row_id;
  }

  /**
   * Sets the row_id.
   *
   * @param integer $row_id
   */
  public function setRowID($row_id) {
    $this->row_id = $row_id;
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
   * Retrieves the value.
   *
   * @retrun text
   */
  public function getValue() {
    return $this->value;
  }

  /**
   * Sets the value.
   *
   * @param text $value
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