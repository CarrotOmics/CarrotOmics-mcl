<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_EXPRESSION class.
 *
 */
class CHADO_EXPRESSION  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_EXPRESSION.
   */
  protected $member_arr    = NULL;
  protected $expression_id = NULL;
  protected $uniquename    = NULL;
  protected $md5checksum   = NULL;
  protected $description   = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'expression_id' => 'serial',
    'uniquename'    => 'text',
    'md5checksum'   => 'character(32)',
    'description'   => 'text',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'expression_id' => TRUE,
    'uniquename'    => TRUE,
    'md5checksum'   => FALSE,
    'description'   => FALSE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr    = $details;
    $this->expression_id = array_key_exists('expression_id', $details) ? $details['expression_id'] : '';
    $this->uniquename    = array_key_exists('uniquename', $details)    ? $details['uniquename']    : '';
    $this->md5checksum   = array_key_exists('md5checksum', $details)   ? $details['md5checksum']   : '';
    $this->description   = array_key_exists('description', $details)   ? $details['description']   : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_EXPRESSION by key(s).
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
    $sql = "SELECT * FROM chado.expression WHERE " . implode(" AND " , $where_arr);
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
    return expression_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return NULL;
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'expression_c1' => array('uniquename'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'expression_id' => array(
        array('table' => 'expression_cvterm', 'attr' => 'expression_id'),
        array('table' => 'expression_image', 'attr' => 'expression_id'),
        array('table' => 'expression_pub', 'attr' => 'expression_id'),
        array('table' => 'expressionprop', 'attr' => 'expression_id'),
        array('table' => 'feature_expression', 'attr' => 'expression_id'),
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
        $fields['uniquename']  = $this->uniquename;
        $fields['md5checksum'] = $this->md5checksum;
        $fields['description'] = $this->description;


        // Inserts the record.
        $this->expression_id = db_insert('chado.expression')
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
      $fields['uniquename']  = $this->uniquename;
      $fields['md5checksum'] = $this->md5checksum;
      $fields['description'] = $this->description;


      // Updates the record.
      db_update('chado.expression')
        ->fields($fields)
        ->condition('expression_id', $this->expression_id, '=')
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

      db_delete('chado.expression')
        ->condition('expression_id', $this->expression_id, '=')
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
   * Retrieves the expression_id.
   *
   * @retrun serial
   */
  public function getExpressionID() {
    return $this->expression_id;
  }

  /**
   * Sets the expression_id.
   *
   * @param serial $expression_id
   */
  public function setExpressionID($expression_id) {
    $this->expression_id = $expression_id;
  }

  /**
   * Retrieves the uniquename.
   *
   * @retrun text
   */
  public function getUniquename() {
    return $this->uniquename;
  }

  /**
   * Sets the uniquename.
   *
   * @param text $uniquename
   */
  public function setUniquename($uniquename) {
    $this->uniquename = $uniquename;
  }

  /**
   * Retrieves the md5checksum.
   *
   * @retrun character(32)
   */
  public function getMd5checksum() {
    return $this->md5checksum;
  }

  /**
   * Sets the md5checksum.
   *
   * @param character(32) $md5checksum
   */
  public function setMd5checksum($md5checksum) {
    $this->md5checksum = $md5checksum;
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