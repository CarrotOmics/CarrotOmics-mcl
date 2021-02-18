<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_CELL_LINE class.
 *
 */
class CHADO_CELL_LINE  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_CELL_LINE.
   */
  protected $member_arr       = NULL;
  protected $cell_line_id     = NULL;
  protected $name             = NULL;
  protected $uniquename       = NULL;
  protected $organism_id      = NULL;
  protected $timeaccessioned  = NULL;
  protected $timelastmodified = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'cell_line_id'     => 'serial',
    'name'             => 'character_varying(255)',
    'uniquename'       => 'character_varying(255)',
    'organism_id'      => 'integer',
    'timeaccessioned'  => 'timestamp_without_time_zone',
    'timelastmodified' => 'timestamp_without_time_zone',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'cell_line_id'     => TRUE,
    'name'             => FALSE,
    'uniquename'       => TRUE,
    'organism_id'      => TRUE,
    'timeaccessioned'  => TRUE,
    'timelastmodified' => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr       = $details;
    $this->cell_line_id     = array_key_exists('cell_line_id', $details)     ? $details['cell_line_id']     : '';
    $this->name             = array_key_exists('name', $details)             ? $details['name']             : '';
    $this->uniquename       = array_key_exists('uniquename', $details)       ? $details['uniquename']       : '';
    $this->organism_id      = array_key_exists('organism_id', $details)      ? $details['organism_id']      : '';
    $this->timeaccessioned  = array_key_exists('timeaccessioned', $details)  ? $details['timeaccessioned']  : '';
    $this->timelastmodified = array_key_exists('timelastmodified', $details) ? $details['timelastmodified'] : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_CELL_LINE by key(s).
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
    $sql = "SELECT * FROM chado.cell_line WHERE " . implode(" AND " , $where_arr);
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
    return cell_line_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'organism_id' => array('fk_table' => 'organism', 'fk_attr' => 'organism_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'cell_line_c1' => array('uniquename','organism_id'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'cell_line_id' => array(
        array('table' => 'cell_line_cvterm', 'attr' => 'cell_line_id'),
        array('table' => 'cell_line_dbxref', 'attr' => 'cell_line_id'),
        array('table' => 'cell_line_feature', 'attr' => 'cell_line_id'),
        array('table' => 'cell_line_library', 'attr' => 'cell_line_id'),
        array('table' => 'cell_line_pub', 'attr' => 'cell_line_id'),
        array('table' => 'cell_line_relationship', 'attr' => 'object_id'),
        array('table' => 'cell_line_relationship', 'attr' => 'subject_id'),
        array('table' => 'cell_line_synonym', 'attr' => 'cell_line_id'),
        array('table' => 'cell_lineprop', 'attr' => 'cell_line_id'),
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
        $fields['name']             = $this->name;
        $fields['uniquename']       = $this->uniquename;
        $fields['organism_id']      = $this->organism_id;
        $fields['timeaccessioned']  = (!empty($this->timeaccessioned))  ? $this->timeaccessioned  : 'now()';
        $fields['timelastmodified'] = (!empty($this->timelastmodified)) ? $this->timelastmodified : 'now()';


        // Inserts the record.
        $this->cell_line_id = db_insert('chado.cell_line')
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
      $fields['name']             = $this->name;
      $fields['uniquename']       = $this->uniquename;
      $fields['organism_id']      = $this->organism_id;
      $fields['timeaccessioned']  = (!empty($this->timeaccessioned))  ? $this->timeaccessioned  : 'now()';
      $fields['timelastmodified'] = (!empty($this->timelastmodified)) ? $this->timelastmodified : 'now()';


      // Updates the record.
      db_update('chado.cell_line')
        ->fields($fields)
        ->condition('cell_line_id', $this->cell_line_id, '=')
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

      db_delete('chado.cell_line')
        ->condition('cell_line_id', $this->cell_line_id, '=')
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
   * Retrieves the cell_line_id.
   *
   * @retrun serial
   */
  public function getCellLineID() {
    return $this->cell_line_id;
  }

  /**
   * Sets the cell_line_id.
   *
   * @param serial $cell_line_id
   */
  public function setCellLineID($cell_line_id) {
    $this->cell_line_id = $cell_line_id;
  }

  /**
   * Retrieves the name.
   *
   * @retrun character_varying(255)
   */
  public function getName() {
    return $this->name;
  }

  /**
   * Sets the name.
   *
   * @param character_varying(255) $name
   */
  public function setName($name) {
    $this->name = $name;
  }

  /**
   * Retrieves the uniquename.
   *
   * @retrun character_varying(255)
   */
  public function getUniquename() {
    return $this->uniquename;
  }

  /**
   * Sets the uniquename.
   *
   * @param character_varying(255) $uniquename
   */
  public function setUniquename($uniquename) {
    $this->uniquename = $uniquename;
  }

  /**
   * Retrieves the organism_id.
   *
   * @retrun integer
   */
  public function getOrganismID() {
    return $this->organism_id;
  }

  /**
   * Sets the organism_id.
   *
   * @param integer $organism_id
   */
  public function setOrganismID($organism_id) {
    $this->organism_id = $organism_id;
  }

  /**
   * Retrieves the timeaccessioned.
   *
   * @retrun timestamp_without_time_zone
   */
  public function getTimeaccessioned() {
    return $this->timeaccessioned;
  }

  /**
   * Sets the timeaccessioned.
   *
   * @param timestamp_without_time_zone $timeaccessioned
   */
  public function setTimeaccessioned($timeaccessioned) {
    $this->timeaccessioned = $timeaccessioned;
  }

  /**
   * Retrieves the timelastmodified.
   *
   * @retrun timestamp_without_time_zone
   */
  public function getTimelastmodified() {
    return $this->timelastmodified;
  }

  /**
   * Sets the timelastmodified.
   *
   * @param timestamp_without_time_zone $timelastmodified
   */
  public function setTimelastmodified($timelastmodified) {
    $this->timelastmodified = $timelastmodified;
  }
}