<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_EXPRESSION_CVTERM class.
 *
 */
class CHADO_EXPRESSION_CVTERM  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_EXPRESSION_CVTERM.
   */
  protected $member_arr           = NULL;
  protected $expression_cvterm_id = NULL;
  protected $expression_id        = NULL;
  protected $cvterm_id            = NULL;
  protected $rank                 = NULL;
  protected $cvterm_type_id       = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'expression_cvterm_id' => 'serial',
    'expression_id'        => 'integer',
    'cvterm_id'            => 'integer',
    'rank'                 => 'integer',
    'cvterm_type_id'       => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'expression_cvterm_id' => TRUE,
    'expression_id'        => TRUE,
    'cvterm_id'            => TRUE,
    'rank'                 => TRUE,
    'cvterm_type_id'       => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr           = $details;
    $this->expression_cvterm_id = array_key_exists('expression_cvterm_id', $details) ? $details['expression_cvterm_id'] : '';
    $this->expression_id        = array_key_exists('expression_id', $details)        ? $details['expression_id']        : '';
    $this->cvterm_id            = array_key_exists('cvterm_id', $details)            ? $details['cvterm_id']            : '';
    $this->rank                 = array_key_exists('rank', $details)                 ? $details['rank']                 : 0;
    $this->cvterm_type_id       = array_key_exists('cvterm_type_id', $details)       ? $details['cvterm_type_id']       : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_EXPRESSION_CVTERM by key(s).
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
    $sql = "SELECT * FROM chado.expression_cvterm WHERE " . implode(" AND " , $where_arr);
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
    return expression_cvterm_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'cvterm_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
      'cvterm_type_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
      'expression_id' => array('fk_table' => 'expression', 'fk_attr' => 'expression_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'expression_cvterm_c1' => array('expression_id','cvterm_id','cvterm_type_id'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'expression_cvterm_id' => array(
        array('table' => 'expression_cvtermprop', 'attr' => 'expression_cvterm_id'),
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
        $fields['expression_id']  = $this->expression_id;
        $fields['cvterm_id']      = $this->cvterm_id;
        $fields['rank']           = (is_numeric($this->rank)) ? $this->rank : 0;
        $fields['cvterm_type_id'] = $this->cvterm_type_id;


        // Inserts the record.
        $this->expression_cvterm_id = db_insert('chado.expression_cvterm')
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
      $fields['expression_id']  = $this->expression_id;
      $fields['cvterm_id']      = $this->cvterm_id;
      $fields['rank']           = (is_numeric($this->rank)) ? $this->rank : 0;
      $fields['cvterm_type_id'] = $this->cvterm_type_id;


      // Updates the record.
      db_update('chado.expression_cvterm')
        ->fields($fields)
        ->condition('expression_cvterm_id', $this->expression_cvterm_id, '=')
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

      db_delete('chado.expression_cvterm')
        ->condition('expression_cvterm_id', $this->expression_cvterm_id, '=')
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
   * Retrieves the expression_cvterm_id.
   *
   * @retrun serial
   */
  public function getExpressionCvtermID() {
    return $this->expression_cvterm_id;
  }

  /**
   * Sets the expression_cvterm_id.
   *
   * @param serial $expression_cvterm_id
   */
  public function setExpressionCvtermID($expression_cvterm_id) {
    $this->expression_cvterm_id = $expression_cvterm_id;
  }

  /**
   * Retrieves the expression_id.
   *
   * @retrun integer
   */
  public function getExpressionID() {
    return $this->expression_id;
  }

  /**
   * Sets the expression_id.
   *
   * @param integer $expression_id
   */
  public function setExpressionID($expression_id) {
    $this->expression_id = $expression_id;
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

  /**
   * Retrieves the cvterm_type_id.
   *
   * @retrun integer
   */
  public function getCvtermTypeID() {
    return $this->cvterm_type_id;
  }

  /**
   * Sets the cvterm_type_id.
   *
   * @param integer $cvterm_type_id
   */
  public function setCvtermTypeID($cvterm_type_id) {
    $this->cvterm_type_id = $cvterm_type_id;
  }
}