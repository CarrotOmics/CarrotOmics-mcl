<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_ND_EXPERIMENT_STOCKPROP class.
 *
 */
class CHADO_ND_EXPERIMENT_STOCKPROP  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_ND_EXPERIMENT_STOCKPROP.
   */
  protected $member_arr                 = NULL;
  protected $nd_experiment_stockprop_id = NULL;
  protected $nd_experiment_stock_id     = NULL;
  protected $type_id                    = NULL;
  protected $value                      = NULL;
  protected $rank                       = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'nd_experiment_stockprop_id' => 'serial',
    'nd_experiment_stock_id'     => 'integer',
    'type_id'                    => 'integer',
    'value'                      => 'text',
    'rank'                       => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'nd_experiment_stockprop_id' => TRUE,
    'nd_experiment_stock_id'     => TRUE,
    'type_id'                    => TRUE,
    'value'                      => FALSE,
    'rank'                       => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr                 = $details;
    $this->nd_experiment_stockprop_id = array_key_exists('nd_experiment_stockprop_id', $details) ? $details['nd_experiment_stockprop_id'] : '';
    $this->nd_experiment_stock_id     = array_key_exists('nd_experiment_stock_id', $details)     ? $details['nd_experiment_stock_id']     : '';
    $this->type_id                    = array_key_exists('type_id', $details)                    ? $details['type_id']                    : '';
    $this->value                      = array_key_exists('value', $details)                      ? $details['value']                      : '';
    $this->rank                       = array_key_exists('rank', $details)                       ? $details['rank']                       : 0;
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_ND_EXPERIMENT_STOCKPROP by key(s).
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
    $sql = "SELECT * FROM chado.nd_experiment_stockprop WHERE " . implode(" AND " , $where_arr);
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
    return nd_experiment_stockprop_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'nd_experiment_stock_id' => array('fk_table' => 'nd_experiment_stock', 'fk_attr' => 'nd_experiment_stock_id'),
      'type_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'nd_experiment_stockprop_c1' => array('nd_experiment_stock_id','type_id','rank'),
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
        $fields['nd_experiment_stock_id'] = $this->nd_experiment_stock_id;
        $fields['type_id']                = $this->type_id;
        $fields['value']                  = $this->value;
        $fields['rank']                   = (is_numeric($this->rank))      ? $this->rank : 0;


        // Inserts the record.
        $this->nd_experiment_stockprop_id = db_insert('chado.nd_experiment_stockprop')
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
      $fields['nd_experiment_stock_id'] = $this->nd_experiment_stock_id;
      $fields['type_id']                = $this->type_id;
      $fields['value']                  = $this->value;
      $fields['rank']                   = (is_numeric($this->rank))      ? $this->rank : 0;


      // Updates the record.
      db_update('chado.nd_experiment_stockprop')
        ->fields($fields)
        ->condition('nd_experiment_stockprop_id', $this->nd_experiment_stockprop_id, '=')
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

      db_delete('chado.nd_experiment_stockprop')
        ->condition('nd_experiment_stockprop_id', $this->nd_experiment_stockprop_id, '=')
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
   * Retrieves the nd_experiment_stockprop_id.
   *
   * @retrun serial
   */
  public function getNdExperimentStockpropID() {
    return $this->nd_experiment_stockprop_id;
  }

  /**
   * Sets the nd_experiment_stockprop_id.
   *
   * @param serial $nd_experiment_stockprop_id
   */
  public function setNdExperimentStockpropID($nd_experiment_stockprop_id) {
    $this->nd_experiment_stockprop_id = $nd_experiment_stockprop_id;
  }

  /**
   * Retrieves the nd_experiment_stock_id.
   *
   * @retrun integer
   */
  public function getNdExperimentStockID() {
    return $this->nd_experiment_stock_id;
  }

  /**
   * Sets the nd_experiment_stock_id.
   *
   * @param integer $nd_experiment_stock_id
   */
  public function setNdExperimentStockID($nd_experiment_stock_id) {
    $this->nd_experiment_stock_id = $nd_experiment_stock_id;
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