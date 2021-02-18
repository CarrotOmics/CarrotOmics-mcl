<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_ASSAY_PROJECT class.
 *
 */
class CHADO_ASSAY_PROJECT  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_ASSAY_PROJECT.
   */
  protected $member_arr       = NULL;
  protected $assay_project_id = NULL;
  protected $assay_id         = NULL;
  protected $project_id       = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'assay_project_id' => 'serial',
    'assay_id'         => 'integer',
    'project_id'       => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'assay_project_id' => TRUE,
    'assay_id'         => TRUE,
    'project_id'       => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr       = $details;
    $this->assay_project_id = array_key_exists('assay_project_id', $details) ? $details['assay_project_id'] : '';
    $this->assay_id         = array_key_exists('assay_id', $details)         ? $details['assay_id']         : '';
    $this->project_id       = array_key_exists('project_id', $details)       ? $details['project_id']       : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_ASSAY_PROJECT by key(s).
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
    $sql = "SELECT * FROM chado.assay_project WHERE " . implode(" AND " , $where_arr);
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
    return assay_project_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'assay_id' => array('fk_table' => 'assay', 'fk_attr' => 'assay_id'),
      'project_id' => array('fk_table' => 'project', 'fk_attr' => 'project_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'assay_project_c1' => array('assay_id','project_id'),
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
        $fields['assay_id']   = $this->assay_id;
        $fields['project_id'] = $this->project_id;


        // Inserts the record.
        $this->assay_project_id = db_insert('chado.assay_project')
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
      $fields['assay_id']   = $this->assay_id;
      $fields['project_id'] = $this->project_id;


      // Updates the record.
      db_update('chado.assay_project')
        ->fields($fields)
        ->condition('assay_project_id', $this->assay_project_id, '=')
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

      db_delete('chado.assay_project')
        ->condition('assay_project_id', $this->assay_project_id, '=')
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
   * Retrieves the assay_project_id.
   *
   * @retrun serial
   */
  public function getAssayProjectID() {
    return $this->assay_project_id;
  }

  /**
   * Sets the assay_project_id.
   *
   * @param serial $assay_project_id
   */
  public function setAssayProjectID($assay_project_id) {
    $this->assay_project_id = $assay_project_id;
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
}