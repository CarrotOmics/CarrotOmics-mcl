<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_BIOMATERIAL class.
 *
 */
class CHADO_BIOMATERIAL  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_BIOMATERIAL.
   */
  protected $member_arr           = NULL;
  protected $biomaterial_id       = NULL;
  protected $taxon_id             = NULL;
  protected $biosourceprovider_id = NULL;
  protected $dbxref_id            = NULL;
  protected $name                 = NULL;
  protected $description          = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'biomaterial_id'       => 'serial',
    'taxon_id'             => 'integer',
    'biosourceprovider_id' => 'integer',
    'dbxref_id'            => 'integer',
    'name'                 => 'text',
    'description'          => 'text',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'biomaterial_id'       => TRUE,
    'taxon_id'             => FALSE,
    'biosourceprovider_id' => FALSE,
    'dbxref_id'            => FALSE,
    'name'                 => FALSE,
    'description'          => FALSE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr           = $details;
    $this->biomaterial_id       = array_key_exists('biomaterial_id', $details)       ? $details['biomaterial_id']       : '';
    $this->taxon_id             = array_key_exists('taxon_id', $details)             ? $details['taxon_id']             : '';
    $this->biosourceprovider_id = array_key_exists('biosourceprovider_id', $details) ? $details['biosourceprovider_id'] : '';
    $this->dbxref_id            = array_key_exists('dbxref_id', $details)            ? $details['dbxref_id']            : '';
    $this->name                 = array_key_exists('name', $details)                 ? $details['name']                 : '';
    $this->description          = array_key_exists('description', $details)          ? $details['description']          : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_BIOMATERIAL by key(s).
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
    $sql = "SELECT * FROM chado.biomaterial WHERE " . implode(" AND " , $where_arr);
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
    return biomaterial_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'biosourceprovider_id' => array('fk_table' => 'contact', 'fk_attr' => 'contact_id'),
      'dbxref_id' => array('fk_table' => 'dbxref', 'fk_attr' => 'dbxref_id'),
      'taxon_id' => array('fk_table' => 'organism', 'fk_attr' => 'organism_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'biomaterial_c1' => array('name'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'biomaterial_id' => array(
        array('table' => 'assay_biomaterial', 'attr' => 'biomaterial_id'),
        array('table' => 'biomaterial_dbxref', 'attr' => 'biomaterial_id'),
        array('table' => 'biomaterial_relationship', 'attr' => 'object_id'),
        array('table' => 'biomaterial_relationship', 'attr' => 'subject_id'),
        array('table' => 'biomaterial_treatment', 'attr' => 'biomaterial_id'),
        array('table' => 'biomaterialprop', 'attr' => 'biomaterial_id'),
        array('table' => 'treatment', 'attr' => 'biomaterial_id'),
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
        $fields['description'] = $this->description;
        if (is_numeric($this->taxon_id))             { $fields['taxon_id']             = $this->taxon_id; }
        if (is_numeric($this->biosourceprovider_id)) { $fields['biosourceprovider_id'] = $this->biosourceprovider_id; }
        if (is_numeric($this->dbxref_id))            { $fields['dbxref_id']            = $this->dbxref_id; }

        // Inserts the record.
        $this->biomaterial_id = db_insert('chado.biomaterial')
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
      $fields['description'] = $this->description;
      if (is_numeric($this->taxon_id))             { $fields['taxon_id']             = $this->taxon_id; }
      if (is_numeric($this->biosourceprovider_id)) { $fields['biosourceprovider_id'] = $this->biosourceprovider_id; }
      if (is_numeric($this->dbxref_id))            { $fields['dbxref_id']            = $this->dbxref_id; }

      // Updates the record.
      db_update('chado.biomaterial')
        ->fields($fields)
        ->condition('biomaterial_id', $this->biomaterial_id, '=')
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

      db_delete('chado.biomaterial')
        ->condition('biomaterial_id', $this->biomaterial_id, '=')
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
   * Retrieves the biomaterial_id.
   *
   * @retrun serial
   */
  public function getBiomaterialID() {
    return $this->biomaterial_id;
  }

  /**
   * Sets the biomaterial_id.
   *
   * @param serial $biomaterial_id
   */
  public function setBiomaterialID($biomaterial_id) {
    $this->biomaterial_id = $biomaterial_id;
  }

  /**
   * Retrieves the taxon_id.
   *
   * @retrun integer
   */
  public function getTaxonID() {
    return $this->taxon_id;
  }

  /**
   * Sets the taxon_id.
   *
   * @param integer $taxon_id
   */
  public function setTaxonID($taxon_id) {
    $this->taxon_id = $taxon_id;
  }

  /**
   * Retrieves the biosourceprovider_id.
   *
   * @retrun integer
   */
  public function getBiosourceproviderID() {
    return $this->biosourceprovider_id;
  }

  /**
   * Sets the biosourceprovider_id.
   *
   * @param integer $biosourceprovider_id
   */
  public function setBiosourceproviderID($biosourceprovider_id) {
    $this->biosourceprovider_id = $biosourceprovider_id;
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