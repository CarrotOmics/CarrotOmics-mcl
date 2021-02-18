<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_MAGEDOCUMENTATION class.
 *
 */
class CHADO_MAGEDOCUMENTATION  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_MAGEDOCUMENTATION.
   */
  protected $member_arr           = NULL;
  protected $magedocumentation_id = NULL;
  protected $mageml_id            = NULL;
  protected $tableinfo_id         = NULL;
  protected $row_id               = NULL;
  protected $mageidentifier       = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'magedocumentation_id' => 'serial',
    'mageml_id'            => 'integer',
    'tableinfo_id'         => 'integer',
    'row_id'               => 'integer',
    'mageidentifier'       => 'text',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'magedocumentation_id' => TRUE,
    'mageml_id'            => TRUE,
    'tableinfo_id'         => TRUE,
    'row_id'               => TRUE,
    'mageidentifier'       => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr           = $details;
    $this->magedocumentation_id = array_key_exists('magedocumentation_id', $details) ? $details['magedocumentation_id'] : '';
    $this->mageml_id            = array_key_exists('mageml_id', $details)            ? $details['mageml_id']            : '';
    $this->tableinfo_id         = array_key_exists('tableinfo_id', $details)         ? $details['tableinfo_id']         : '';
    $this->row_id               = array_key_exists('row_id', $details)               ? $details['row_id']               : '';
    $this->mageidentifier       = array_key_exists('mageidentifier', $details)       ? $details['mageidentifier']       : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_MAGEDOCUMENTATION by key(s).
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
    $sql = "SELECT * FROM chado.magedocumentation WHERE " . implode(" AND " , $where_arr);
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
    return magedocumentation_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'mageml_id' => array('fk_table' => 'mageml', 'fk_attr' => 'mageml_id'),
      'tableinfo_id' => array('fk_table' => 'tableinfo', 'fk_attr' => 'tableinfo_id'),
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
        $fields['mageml_id']      = $this->mageml_id;
        $fields['tableinfo_id']   = $this->tableinfo_id;
        $fields['row_id']         = $this->row_id;
        $fields['mageidentifier'] = $this->mageidentifier;


        // Inserts the record.
        $this->magedocumentation_id = db_insert('chado.magedocumentation')
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
      $fields['mageml_id']      = $this->mageml_id;
      $fields['tableinfo_id']   = $this->tableinfo_id;
      $fields['row_id']         = $this->row_id;
      $fields['mageidentifier'] = $this->mageidentifier;


      // Updates the record.
      db_update('chado.magedocumentation')
        ->fields($fields)
        ->condition('magedocumentation_id', $this->magedocumentation_id, '=')
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

      db_delete('chado.magedocumentation')
        ->condition('magedocumentation_id', $this->magedocumentation_id, '=')
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
   * Retrieves the magedocumentation_id.
   *
   * @retrun serial
   */
  public function getMagedocumentationID() {
    return $this->magedocumentation_id;
  }

  /**
   * Sets the magedocumentation_id.
   *
   * @param serial $magedocumentation_id
   */
  public function setMagedocumentationID($magedocumentation_id) {
    $this->magedocumentation_id = $magedocumentation_id;
  }

  /**
   * Retrieves the mageml_id.
   *
   * @retrun integer
   */
  public function getMagemlID() {
    return $this->mageml_id;
  }

  /**
   * Sets the mageml_id.
   *
   * @param integer $mageml_id
   */
  public function setMagemlID($mageml_id) {
    $this->mageml_id = $mageml_id;
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
   * Retrieves the mageidentifier.
   *
   * @retrun text
   */
  public function getMageidentifier() {
    return $this->mageidentifier;
  }

  /**
   * Sets the mageidentifier.
   *
   * @param text $mageidentifier
   */
  public function setMageidentifier($mageidentifier) {
    $this->mageidentifier = $mageidentifier;
  }
}