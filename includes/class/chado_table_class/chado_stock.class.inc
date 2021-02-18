<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_STOCK class.
 *
 */
class CHADO_STOCK  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_STOCK.
   */
  protected $member_arr  = NULL;
  protected $stock_id    = NULL;
  protected $dbxref_id   = NULL;
  protected $organism_id = NULL;
  protected $name        = NULL;
  protected $uniquename  = NULL;
  protected $description = NULL;
  protected $type_id     = NULL;
  protected $is_obsolete = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'stock_id'    => 'serial',
    'dbxref_id'   => 'integer',
    'organism_id' => 'integer',
    'name'        => 'character_varying(255)',
    'uniquename'  => 'text',
    'description' => 'text',
    'type_id'     => 'integer',
    'is_obsolete' => 'boolean',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'stock_id'    => TRUE,
    'dbxref_id'   => FALSE,
    'organism_id' => FALSE,
    'name'        => FALSE,
    'uniquename'  => TRUE,
    'description' => FALSE,
    'type_id'     => TRUE,
    'is_obsolete' => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr  = $details;
    $this->stock_id    = array_key_exists('stock_id', $details)    ? $details['stock_id']    : '';
    $this->dbxref_id   = array_key_exists('dbxref_id', $details)   ? $details['dbxref_id']   : '';
    $this->organism_id = array_key_exists('organism_id', $details) ? $details['organism_id'] : '';
    $this->name        = array_key_exists('name', $details)        ? $details['name']        : '';
    $this->uniquename  = array_key_exists('uniquename', $details)  ? $details['uniquename']  : '';
    $this->description = array_key_exists('description', $details) ? $details['description'] : '';
    $this->type_id     = array_key_exists('type_id', $details)     ? $details['type_id']     : '';
    $this->is_obsolete = array_key_exists('is_obsolete', $details) ? $details['is_obsolete'] : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_STOCK by key(s).
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
    $sql = "SELECT * FROM chado.stock WHERE " . implode(" AND " , $where_arr);
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
    return stock_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'dbxref_id' => array('fk_table' => 'dbxref', 'fk_attr' => 'dbxref_id'),
      'organism_id' => array('fk_table' => 'organism', 'fk_attr' => 'organism_id'),
      'type_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'stock_c1' => array('organism_id','uniquename','type_id'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'stock_id' => array(
        array('table' => 'feature_stock', 'attr' => 'stock_id'),
        array('table' => 'featuremap_stock', 'attr' => 'stock_id'),
        array('table' => 'genotype_call', 'attr' => 'stock_id'),
        array('table' => 'nd_experiment_stock', 'attr' => 'stock_id'),
        array('table' => 'phenotype_call', 'attr' => 'stock_id'),
        array('table' => 'stock_cvterm', 'attr' => 'stock_id'),
        array('table' => 'stock_dbxref', 'attr' => 'stock_id'),
        array('table' => 'stock_genotype', 'attr' => 'stock_id'),
        array('table' => 'stock_image', 'attr' => 'stock_id'),
        array('table' => 'stock_pub', 'attr' => 'stock_id'),
        array('table' => 'stock_relationship', 'attr' => 'object_id'),
        array('table' => 'stock_relationship', 'attr' => 'subject_id'),
        array('table' => 'stockcollection_stock', 'attr' => 'stock_id'),
        array('table' => 'stockprop', 'attr' => 'stock_id'),
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
        $fields['name']        = $this->name;
        $fields['uniquename']  = $this->uniquename;
        $fields['description'] = $this->description;
        $fields['type_id']     = $this->type_id;
        $fields['is_obsolete'] = (preg_match("/^(true|false)/i", $this->is_obsolete)) ? "'" . $this->is_obsolete . "'" : 'false';
        if (is_numeric($this->dbxref_id))   { $fields['dbxref_id']   = $this->dbxref_id; }
        if (is_numeric($this->organism_id)) { $fields['organism_id'] = $this->organism_id; }

        // Inserts the record.
        $this->stock_id = db_insert('chado.stock')
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
      $fields['name']        = $this->name;
      $fields['uniquename']  = $this->uniquename;
      $fields['description'] = $this->description;
      $fields['type_id']     = $this->type_id;
      $fields['is_obsolete'] = (preg_match("/^(true|false)/i", $this->is_obsolete)) ? "'" . $this->is_obsolete . "'" : 'false';
      if (is_numeric($this->dbxref_id))   { $fields['dbxref_id']   = $this->dbxref_id; }
      if (is_numeric($this->organism_id)) { $fields['organism_id'] = $this->organism_id; }

      // Updates the record.
      db_update('chado.stock')
        ->fields($fields)
        ->condition('stock_id', $this->stock_id, '=')
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

      db_delete('chado.stock')
        ->condition('stock_id', $this->stock_id, '=')
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
   * Retrieves the stock_id.
   *
   * @retrun serial
   */
  public function getStockID() {
    return $this->stock_id;
  }

  /**
   * Sets the stock_id.
   *
   * @param serial $stock_id
   */
  public function setStockID($stock_id) {
    $this->stock_id = $stock_id;
  }

  /**
   * Retrieves the dbxref_id.
   *
   * @retrun integer
   */
  public function getDbxrefID() {
    return $this->dbxref_id;
  }

  /**
   * Sets the dbxref_id.
   *
   * @param integer $dbxref_id
   */
  public function setDbxrefID($dbxref_id) {
    $this->dbxref_id = $dbxref_id;
  }

  /**
   * Retrieves the organism_id.
   *
   * @retrun integer
   */
  public function getOrganismID() {
    return $this->organism_id;
  }

  /**
   * Sets the organism_id.
   *
   * @param integer $organism_id
   */
  public function setOrganismID($organism_id) {
    $this->organism_id = $organism_id;
  }

  /**
   * Retrieves the name.
   *
   * @retrun character_varying(255)
   */
  public function getName() {
    return $this->name;
  }

  /**
   * Sets the name.
   *
   * @param character_varying(255) $name
   */
  public function setName($name) {
    $this->name = $name;
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
   * Retrieves the is_obsolete.
   *
   * @retrun boolean
   */
  public function getIsObsolete() {
    return $this->is_obsolete;
  }

  /**
   * Sets the is_obsolete.
   *
   * @param boolean $is_obsolete
   */
  public function setIsObsolete($is_obsolete) {
    $this->is_obsolete = $is_obsolete;
  }
}