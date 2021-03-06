<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_GENOTYPE_CALL class.
 *
 */
class CHADO_GENOTYPE_CALL  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_GENOTYPE_CALL.
   */
  protected $member_arr       = NULL;
  protected $genotype_call_id = NULL;
  protected $project_id       = NULL;
  protected $feature_id       = NULL;
  protected $stock_id         = NULL;
  protected $genotype_id      = NULL;
  protected $meta_data        = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'genotype_call_id' => 'serial',
    'project_id'       => 'integer',
    'feature_id'       => 'integer',
    'stock_id'         => 'integer',
    'genotype_id'      => 'integer',
    'meta_data'        => 'text',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'genotype_call_id' => TRUE,
    'project_id'       => TRUE,
    'feature_id'       => TRUE,
    'stock_id'         => TRUE,
    'genotype_id'      => TRUE,
    'meta_data'        => FALSE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr       = $details;
    $this->genotype_call_id = array_key_exists('genotype_call_id', $details) ? $details['genotype_call_id'] : '';
    $this->project_id       = array_key_exists('project_id', $details)       ? $details['project_id']       : '';
    $this->feature_id       = array_key_exists('feature_id', $details)       ? $details['feature_id']       : '';
    $this->stock_id         = array_key_exists('stock_id', $details)         ? $details['stock_id']         : '';
    $this->genotype_id      = array_key_exists('genotype_id', $details)      ? $details['genotype_id']      : '';
    $this->meta_data        = array_key_exists('meta_data', $details)        ? $details['meta_data']        : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_GENOTYPE_CALL by key(s).
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
    $sql = "SELECT * FROM chado.genotype_call WHERE " . implode(" AND " , $where_arr);
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
    return NULL;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'feature_id' => array('fk_table' => 'feature', 'fk_attr' => 'feature_id'),
      'genotype_id' => array('fk_table' => 'genotype', 'fk_attr' => 'genotype_id'),
      'project_id' => array('fk_table' => 'project', 'fk_attr' => 'project_id'),
      'stock_id' => array('fk_table' => 'stock', 'fk_attr' => 'stock_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'genotype_call_c1' => array('project_id','stock_id','feature_id','genotype_id'),
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
        $fields['project_id']  = $this->project_id;
        $fields['feature_id']  = $this->feature_id;
        $fields['stock_id']    = $this->stock_id;
        $fields['genotype_id'] = $this->genotype_id;
        $fields['meta_data']   = $this->meta_data;


        // Inserts the record.
        db_insert('chado.genotype_call')
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
      $fields['project_id']  = $this->project_id;
      $fields['feature_id']  = $this->feature_id;
      $fields['stock_id']    = $this->stock_id;
      $fields['genotype_id'] = $this->genotype_id;
      $fields['meta_data']   = $this->meta_data;


      // Updates the record.
      db_update('chado.genotype_call')
        ->fields($fields)
        ->condition('project_id', $this->project_id, '=')
        ->condition('stock_id', $this->stock_id, '=')
        ->condition('feature_id', $this->feature_id, '=')
        ->condition('genotype_id', $this->genotype_id, '=')
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

      db_delete('chado.genotype_call')
        ->condition('project_id', $this->project_id, '=')
        ->condition('stock_id', $this->stock_id, '=')
        ->condition('feature_id', $this->feature_id, '=')
        ->condition('genotype_id', $this->genotype_id, '=')
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
   * Retrieves the genotype_call_id.
   *
   * @retrun serial
   */
  public function getGenotypeCallID() {
    return $this->genotype_call_id;
  }

  /**
   * Sets the genotype_call_id.
   *
   * @param serial $genotype_call_id
   */
  public function setGenotypeCallID($genotype_call_id) {
    $this->genotype_call_id = $genotype_call_id;
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

  /**
   * Retrieves the genotype_id.
   *
   * @retrun integer
   */
  public function getGenotypeID() {
    return $this->genotype_id;
  }

  /**
   * Sets the genotype_id.
   *
   * @param integer $genotype_id
   */
  public function setGenotypeID($genotype_id) {
    $this->genotype_id = $genotype_id;
  }

  /**
   * Retrieves the meta_data.
   *
   * @retrun text
   */
  public function getMetaData() {
    return $this->meta_data;
  }

  /**
   * Sets the meta_data.
   *
   * @param text $meta_data
   */
  public function setMetaData($meta_data) {
    $this->meta_data = $meta_data;
  }
}