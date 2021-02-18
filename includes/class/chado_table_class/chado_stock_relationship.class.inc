<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_STOCK_RELATIONSHIP class.
 *
 */
class CHADO_STOCK_RELATIONSHIP  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_STOCK_RELATIONSHIP.
   */
  protected $member_arr            = NULL;
  protected $stock_relationship_id = NULL;
  protected $subject_id            = NULL;
  protected $object_id             = NULL;
  protected $type_id               = NULL;
  protected $value                 = NULL;
  protected $rank                  = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'stock_relationship_id' => 'serial',
    'subject_id'            => 'integer',
    'object_id'             => 'integer',
    'type_id'               => 'integer',
    'value'                 => 'text',
    'rank'                  => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'stock_relationship_id' => TRUE,
    'subject_id'            => TRUE,
    'object_id'             => TRUE,
    'type_id'               => TRUE,
    'value'                 => FALSE,
    'rank'                  => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr            = $details;
    $this->stock_relationship_id = array_key_exists('stock_relationship_id', $details) ? $details['stock_relationship_id'] : '';
    $this->subject_id            = array_key_exists('subject_id', $details)            ? $details['subject_id']            : '';
    $this->object_id             = array_key_exists('object_id', $details)             ? $details['object_id']             : '';
    $this->type_id               = array_key_exists('type_id', $details)               ? $details['type_id']               : '';
    $this->value                 = array_key_exists('value', $details)                 ? $details['value']                 : '';
    $this->rank                  = array_key_exists('rank', $details)                  ? $details['rank']                  : 0;
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_STOCK_RELATIONSHIP by key(s).
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
    $sql = "SELECT * FROM chado.stock_relationship WHERE " . implode(" AND " , $where_arr);
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
    return stock_relationship_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'object_id' => array('fk_table' => 'stock', 'fk_attr' => 'stock_id'),
      'subject_id' => array('fk_table' => 'stock', 'fk_attr' => 'stock_id'),
      'type_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'stock_relationship_c1' => array('subject_id','object_id','type_id','rank'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'stock_relationship_id' => array(
        array('table' => 'stock_relationship_cvterm', 'attr' => 'stock_relationship_id'),
        array('table' => 'stock_relationship_pub', 'attr' => 'stock_relationship_id'),
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
        $fields['subject_id'] = $this->subject_id;
        $fields['object_id']  = $this->object_id;
        $fields['type_id']    = $this->type_id;
        $fields['value']      = $this->value;
        $fields['rank']       = (is_numeric($this->rank)) ? $this->rank : 0;


        // Inserts the record.
        $this->stock_relationship_id = db_insert('chado.stock_relationship')
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
      $fields['subject_id'] = $this->subject_id;
      $fields['object_id']  = $this->object_id;
      $fields['type_id']    = $this->type_id;
      $fields['value']      = $this->value;
      $fields['rank']       = (is_numeric($this->rank)) ? $this->rank : 0;


      // Updates the record.
      db_update('chado.stock_relationship')
        ->fields($fields)
        ->condition('stock_relationship_id', $this->stock_relationship_id, '=')
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

      db_delete('chado.stock_relationship')
        ->condition('stock_relationship_id', $this->stock_relationship_id, '=')
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
   * Retrieves the stock_relationship_id.
   *
   * @retrun serial
   */
  public function getStockRelationshipID() {
    return $this->stock_relationship_id;
  }

  /**
   * Sets the stock_relationship_id.
   *
   * @param serial $stock_relationship_id
   */
  public function setStockRelationshipID($stock_relationship_id) {
    $this->stock_relationship_id = $stock_relationship_id;
  }

  /**
   * Retrieves the subject_id.
   *
   * @retrun integer
   */
  public function getSubjectID() {
    return $this->subject_id;
  }

  /**
   * Sets the subject_id.
   *
   * @param integer $subject_id
   */
  public function setSubjectID($subject_id) {
    $this->subject_id = $subject_id;
  }

  /**
   * Retrieves the object_id.
   *
   * @retrun integer
   */
  public function getObjectID() {
    return $this->object_id;
  }

  /**
   * Sets the object_id.
   *
   * @param integer $object_id
   */
  public function setObjectID($object_id) {
    $this->object_id = $object_id;
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