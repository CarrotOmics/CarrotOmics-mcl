<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_PHENOTYPE_CVTERM class.
 *
 */
class CHADO_PHENOTYPE_CVTERM  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_PHENOTYPE_CVTERM.
   */
  protected $member_arr          = NULL;
  protected $phenotype_cvterm_id = NULL;
  protected $phenotype_id        = NULL;
  protected $cvterm_id           = NULL;
  protected $rank                = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'phenotype_cvterm_id' => 'serial',
    'phenotype_id'        => 'integer',
    'cvterm_id'           => 'integer',
    'rank'                => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'phenotype_cvterm_id' => TRUE,
    'phenotype_id'        => TRUE,
    'cvterm_id'           => TRUE,
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
    $this->phenotype_cvterm_id = array_key_exists('phenotype_cvterm_id', $details) ? $details['phenotype_cvterm_id'] : '';
    $this->phenotype_id        = array_key_exists('phenotype_id', $details)        ? $details['phenotype_id']        : '';
    $this->cvterm_id           = array_key_exists('cvterm_id', $details)           ? $details['cvterm_id']           : '';
    $this->rank                = array_key_exists('rank', $details)                ? $details['rank']                : 0;
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_PHENOTYPE_CVTERM by key(s).
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
    $sql = "SELECT * FROM chado.phenotype_cvterm WHERE " . implode(" AND " , $where_arr);
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
    return phenotype_cvterm_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'cvterm_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
      'phenotype_id' => array('fk_table' => 'phenotype', 'fk_attr' => 'phenotype_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'phenotype_cvterm_c1' => array('phenotype_id','cvterm_id','rank'),
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
        $fields['phenotype_id'] = $this->phenotype_id;
        $fields['cvterm_id']    = $this->cvterm_id;
        $fields['rank']         = (is_numeric($this->rank)) ? $this->rank : 0;


        // Inserts the record.
        $this->phenotype_cvterm_id = db_insert('chado.phenotype_cvterm')
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
      $fields['phenotype_id'] = $this->phenotype_id;
      $fields['cvterm_id']    = $this->cvterm_id;
      $fields['rank']         = (is_numeric($this->rank)) ? $this->rank : 0;


      // Updates the record.
      db_update('chado.phenotype_cvterm')
        ->fields($fields)
        ->condition('phenotype_cvterm_id', $this->phenotype_cvterm_id, '=')
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

      db_delete('chado.phenotype_cvterm')
        ->condition('phenotype_cvterm_id', $this->phenotype_cvterm_id, '=')
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
   * Retrieves the phenotype_cvterm_id.
   *
   * @retrun serial
   */
  public function getPhenotypeCvtermID() {
    return $this->phenotype_cvterm_id;
  }

  /**
   * Sets the phenotype_cvterm_id.
   *
   * @param serial $phenotype_cvterm_id
   */
  public function setPhenotypeCvtermID($phenotype_cvterm_id) {
    $this->phenotype_cvterm_id = $phenotype_cvterm_id;
  }

  /**
   * Retrieves the phenotype_id.
   *
   * @retrun integer
   */
  public function getPhenotypeID() {
    return $this->phenotype_id;
  }

  /**
   * Sets the phenotype_id.
   *
   * @param integer $phenotype_id
   */
  public function setPhenotypeID($phenotype_id) {
    $this->phenotype_id = $phenotype_id;
  }

  /**
   * Retrieves the cvterm_id.
   *
   * @retrun integer
   */
  public function getCvtermID() {
    return $this->cvterm_id;
  }

  /**
   * Sets the cvterm_id.
   *
   * @param integer $cvterm_id
   */
  public function setCvtermID($cvterm_id) {
    $this->cvterm_id = $cvterm_id;
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