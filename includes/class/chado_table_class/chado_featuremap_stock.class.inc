<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_FEATUREMAP_STOCK class.
 *
 */
class CHADO_FEATUREMAP_STOCK  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_FEATUREMAP_STOCK.
   */
  protected $member_arr          = NULL;
  protected $featuremap_stock_id = NULL;
  protected $featuremap_id       = NULL;
  protected $stock_id            = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'featuremap_stock_id' => 'serial',
    'featuremap_id'       => 'integer',
    'stock_id'            => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'featuremap_stock_id' => TRUE,
    'featuremap_id'       => TRUE,
    'stock_id'            => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr          = $details;
    $this->featuremap_stock_id = array_key_exists('featuremap_stock_id', $details) ? $details['featuremap_stock_id'] : '';
    $this->featuremap_id       = array_key_exists('featuremap_id', $details)       ? $details['featuremap_id']       : '';
    $this->stock_id            = array_key_exists('stock_id', $details)            ? $details['stock_id']            : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_FEATUREMAP_STOCK by key(s).
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
    $sql = "SELECT * FROM chado.featuremap_stock WHERE " . implode(" AND " , $where_arr);
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
    return featuremap_stock_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'featuremap_id' => array('fk_table' => 'featuremap', 'fk_attr' => 'featuremap_id'),
      'stock_id' => array('fk_table' => 'stock', 'fk_attr' => 'stock_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'featuremap_stock_c1' => array('featuremap_id','stock_id'),
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
        $fields['featuremap_id'] = $this->featuremap_id;
        $fields['stock_id']      = $this->stock_id;


        // Inserts the record.
        $this->featuremap_stock_id = db_insert('chado.featuremap_stock')
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
      $fields['featuremap_id'] = $this->featuremap_id;
      $fields['stock_id']      = $this->stock_id;


      // Updates the record.
      db_update('chado.featuremap_stock')
        ->fields($fields)
        ->condition('featuremap_stock_id', $this->featuremap_stock_id, '=')
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

      db_delete('chado.featuremap_stock')
        ->condition('featuremap_stock_id', $this->featuremap_stock_id, '=')
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
   * Retrieves the featuremap_stock_id.
   *
   * @retrun serial
   */
  public function getFeaturemapStockID() {
    return $this->featuremap_stock_id;
  }

  /**
   * Sets the featuremap_stock_id.
   *
   * @param serial $featuremap_stock_id
   */
  public function setFeaturemapStockID($featuremap_stock_id) {
    $this->featuremap_stock_id = $featuremap_stock_id;
  }

  /**
   * Retrieves the featuremap_id.
   *
   * @retrun integer
   */
  public function getFeaturemapID() {
    return $this->featuremap_id;
  }

  /**
   * Sets the featuremap_id.
   *
   * @param integer $featuremap_id
   */
  public function setFeaturemapID($featuremap_id) {
    $this->featuremap_id = $featuremap_id;
  }

  /**
   * Retrieves the stock_id.
   *
   * @retrun integer
   */
  public function getStockID() {
    return $this->stock_id;
  }

  /**
   * Sets the stock_id.
   *
   * @param integer $stock_id
   */
  public function setStockID($stock_id) {
    $this->stock_id = $stock_id;
  }
}