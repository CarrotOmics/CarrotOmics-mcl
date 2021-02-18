<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_STUDYFACTORVALUE class.
 *
 */
class CHADO_STUDYFACTORVALUE  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_STUDYFACTORVALUE.
   */
  protected $member_arr          = NULL;
  protected $studyfactorvalue_id = NULL;
  protected $studyfactor_id      = NULL;
  protected $assay_id            = NULL;
  protected $factorvalue         = NULL;
  protected $name                = NULL;
  protected $rank                = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'studyfactorvalue_id' => 'serial',
    'studyfactor_id'      => 'integer',
    'assay_id'            => 'integer',
    'factorvalue'         => 'text',
    'name'                => 'text',
    'rank'                => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'studyfactorvalue_id' => TRUE,
    'studyfactor_id'      => TRUE,
    'assay_id'            => TRUE,
    'factorvalue'         => FALSE,
    'name'                => FALSE,
    'rank'                => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr          = $details;
    $this->studyfactorvalue_id = array_key_exists('studyfactorvalue_id', $details) ? $details['studyfactorvalue_id'] : '';
    $this->studyfactor_id      = array_key_exists('studyfactor_id', $details)      ? $details['studyfactor_id']      : '';
    $this->assay_id            = array_key_exists('assay_id', $details)            ? $details['assay_id']            : '';
    $this->factorvalue         = array_key_exists('factorvalue', $details)         ? $details['factorvalue']         : '';
    $this->name                = array_key_exists('name', $details)                ? $details['name']                : '';
    $this->rank                = array_key_exists('rank', $details)                ? $details['rank']                : 0;
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_STUDYFACTORVALUE by key(s).
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
    $sql = "SELECT * FROM chado.studyfactorvalue WHERE " . implode(" AND " , $where_arr);
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
    return studyfactorvalue_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'assay_id' => array('fk_table' => 'assay', 'fk_attr' => 'assay_id'),
      'studyfactor_id' => array('fk_table' => 'studyfactor', 'fk_attr' => 'studyfactor_id'),
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
        $fields['studyfactor_id'] = $this->studyfactor_id;
        $fields['assay_id']       = $this->assay_id;
        $fields['factorvalue']    = $this->factorvalue;
        $fields['name']           = $this->name;
        $fields['rank']           = (is_numeric($this->rank)) ? $this->rank : 0;


        // Inserts the record.
        $this->studyfactorvalue_id = db_insert('chado.studyfactorvalue')
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
      $fields['studyfactor_id'] = $this->studyfactor_id;
      $fields['assay_id']       = $this->assay_id;
      $fields['factorvalue']    = $this->factorvalue;
      $fields['name']           = $this->name;
      $fields['rank']           = (is_numeric($this->rank)) ? $this->rank : 0;


      // Updates the record.
      db_update('chado.studyfactorvalue')
        ->fields($fields)
        ->condition('studyfactorvalue_id', $this->studyfactorvalue_id, '=')
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

      db_delete('chado.studyfactorvalue')
        ->condition('studyfactorvalue_id', $this->studyfactorvalue_id, '=')
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
   * Retrieves the studyfactorvalue_id.
   *
   * @retrun serial
   */
  public function getStudyfactorvalueID() {
    return $this->studyfactorvalue_id;
  }

  /**
   * Sets the studyfactorvalue_id.
   *
   * @param serial $studyfactorvalue_id
   */
  public function setStudyfactorvalueID($studyfactorvalue_id) {
    $this->studyfactorvalue_id = $studyfactorvalue_id;
  }

  /**
   * Retrieves the studyfactor_id.
   *
   * @retrun integer
   */
  public function getStudyfactorID() {
    return $this->studyfactor_id;
  }

  /**
   * Sets the studyfactor_id.
   *
   * @param integer $studyfactor_id
   */
  public function setStudyfactorID($studyfactor_id) {
    $this->studyfactor_id = $studyfactor_id;
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
   * Retrieves the factorvalue.
   *
   * @retrun text
   */
  public function getFactorvalue() {
    return $this->factorvalue;
  }

  /**
   * Sets the factorvalue.
   *
   * @param text $factorvalue
   */
  public function setFactorvalue($factorvalue) {
    $this->factorvalue = $factorvalue;
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