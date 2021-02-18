<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_FEATURE_EXPRESSION class.
 *
 */
class CHADO_FEATURE_EXPRESSION  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_FEATURE_EXPRESSION.
   */
  protected $member_arr            = NULL;
  protected $feature_expression_id = NULL;
  protected $expression_id         = NULL;
  protected $feature_id            = NULL;
  protected $pub_id                = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'feature_expression_id' => 'serial',
    'expression_id'         => 'integer',
    'feature_id'            => 'integer',
    'pub_id'                => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'feature_expression_id' => TRUE,
    'expression_id'         => TRUE,
    'feature_id'            => TRUE,
    'pub_id'                => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr            = $details;
    $this->feature_expression_id = array_key_exists('feature_expression_id', $details) ? $details['feature_expression_id'] : '';
    $this->expression_id         = array_key_exists('expression_id', $details)         ? $details['expression_id']         : '';
    $this->feature_id            = array_key_exists('feature_id', $details)            ? $details['feature_id']            : '';
    $this->pub_id                = array_key_exists('pub_id', $details)                ? $details['pub_id']                : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_FEATURE_EXPRESSION by key(s).
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
    $sql = "SELECT * FROM chado.feature_expression WHERE " . implode(" AND " , $where_arr);
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
    return feature_expression_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'expression_id' => array('fk_table' => 'expression', 'fk_attr' => 'expression_id'),
      'feature_id' => array('fk_table' => 'feature', 'fk_attr' => 'feature_id'),
      'pub_id' => array('fk_table' => 'pub', 'fk_attr' => 'pub_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'feature_expression_c1' => array('expression_id','feature_id','pub_id'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'feature_expression_id' => array(
        array('table' => 'feature_expressionprop', 'attr' => 'feature_expression_id'),
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
        $fields['expression_id'] = $this->expression_id;
        $fields['feature_id']    = $this->feature_id;
        $fields['pub_id']        = $this->pub_id;


        // Inserts the record.
        $this->feature_expression_id = db_insert('chado.feature_expression')
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
      $fields['expression_id'] = $this->expression_id;
      $fields['feature_id']    = $this->feature_id;
      $fields['pub_id']        = $this->pub_id;


      // Updates the record.
      db_update('chado.feature_expression')
        ->fields($fields)
        ->condition('feature_expression_id', $this->feature_expression_id, '=')
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

      db_delete('chado.feature_expression')
        ->condition('feature_expression_id', $this->feature_expression_id, '=')
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
   * Retrieves the feature_expression_id.
   *
   * @retrun serial
   */
  public function getFeatureExpressionID() {
    return $this->feature_expression_id;
  }

  /**
   * Sets the feature_expression_id.
   *
   * @param serial $feature_expression_id
   */
  public function setFeatureExpressionID($feature_expression_id) {
    $this->feature_expression_id = $feature_expression_id;
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

  /**
   * Retrieves the pub_id.
   *
   * @retrun integer
   */
  public function getPubID() {
    return $this->pub_id;
  }

  /**
   * Sets the pub_id.
   *
   * @param integer $pub_id
   */
  public function setPubID($pub_id) {
    $this->pub_id = $pub_id;
  }
}