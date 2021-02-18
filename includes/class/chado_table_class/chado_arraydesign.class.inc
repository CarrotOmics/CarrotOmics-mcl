<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_ARRAYDESIGN class.
 *
 */
class CHADO_ARRAYDESIGN  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_ARRAYDESIGN.
   */
  protected $member_arr         = NULL;
  protected $arraydesign_id     = NULL;
  protected $manufacturer_id    = NULL;
  protected $platformtype_id    = NULL;
  protected $substratetype_id   = NULL;
  protected $protocol_id        = NULL;
  protected $dbxref_id          = NULL;
  protected $name               = NULL;
  protected $version            = NULL;
  protected $description        = NULL;
  protected $array_dimensions   = NULL;
  protected $element_dimensions = NULL;
  protected $num_of_elements    = NULL;
  protected $num_array_columns  = NULL;
  protected $num_array_rows     = NULL;
  protected $num_grid_columns   = NULL;
  protected $num_grid_rows      = NULL;
  protected $num_sub_columns    = NULL;
  protected $num_sub_rows       = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'arraydesign_id'     => 'serial',
    'manufacturer_id'    => 'integer',
    'platformtype_id'    => 'integer',
    'substratetype_id'   => 'integer',
    'protocol_id'        => 'integer',
    'dbxref_id'          => 'integer',
    'name'               => 'text',
    'version'            => 'text',
    'description'        => 'text',
    'array_dimensions'   => 'text',
    'element_dimensions' => 'text',
    'num_of_elements'    => 'integer',
    'num_array_columns'  => 'integer',
    'num_array_rows'     => 'integer',
    'num_grid_columns'   => 'integer',
    'num_grid_rows'      => 'integer',
    'num_sub_columns'    => 'integer',
    'num_sub_rows'       => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'arraydesign_id'     => TRUE,
    'manufacturer_id'    => TRUE,
    'platformtype_id'    => TRUE,
    'substratetype_id'   => FALSE,
    'protocol_id'        => FALSE,
    'dbxref_id'          => FALSE,
    'name'               => TRUE,
    'version'            => FALSE,
    'description'        => FALSE,
    'array_dimensions'   => FALSE,
    'element_dimensions' => FALSE,
    'num_of_elements'    => FALSE,
    'num_array_columns'  => FALSE,
    'num_array_rows'     => FALSE,
    'num_grid_columns'   => FALSE,
    'num_grid_rows'      => FALSE,
    'num_sub_columns'    => FALSE,
    'num_sub_rows'       => FALSE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr         = $details;
    $this->arraydesign_id     = array_key_exists('arraydesign_id', $details)     ? $details['arraydesign_id']     : '';
    $this->manufacturer_id    = array_key_exists('manufacturer_id', $details)    ? $details['manufacturer_id']    : '';
    $this->platformtype_id    = array_key_exists('platformtype_id', $details)    ? $details['platformtype_id']    : '';
    $this->substratetype_id   = array_key_exists('substratetype_id', $details)   ? $details['substratetype_id']   : '';
    $this->protocol_id        = array_key_exists('protocol_id', $details)        ? $details['protocol_id']        : '';
    $this->dbxref_id          = array_key_exists('dbxref_id', $details)          ? $details['dbxref_id']          : '';
    $this->name               = array_key_exists('name', $details)               ? $details['name']               : '';
    $this->version            = array_key_exists('version', $details)            ? $details['version']            : '';
    $this->description        = array_key_exists('description', $details)        ? $details['description']        : '';
    $this->array_dimensions   = array_key_exists('array_dimensions', $details)   ? $details['array_dimensions']   : '';
    $this->element_dimensions = array_key_exists('element_dimensions', $details) ? $details['element_dimensions'] : '';
    $this->num_of_elements    = array_key_exists('num_of_elements', $details)    ? $details['num_of_elements']    : '';
    $this->num_array_columns  = array_key_exists('num_array_columns', $details)  ? $details['num_array_columns']  : '';
    $this->num_array_rows     = array_key_exists('num_array_rows', $details)     ? $details['num_array_rows']     : '';
    $this->num_grid_columns   = array_key_exists('num_grid_columns', $details)   ? $details['num_grid_columns']   : '';
    $this->num_grid_rows      = array_key_exists('num_grid_rows', $details)      ? $details['num_grid_rows']      : '';
    $this->num_sub_columns    = array_key_exists('num_sub_columns', $details)    ? $details['num_sub_columns']    : '';
    $this->num_sub_rows       = array_key_exists('num_sub_rows', $details)       ? $details['num_sub_rows']       : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_ARRAYDESIGN by key(s).
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
    $sql = "SELECT * FROM chado.arraydesign WHERE " . implode(" AND " , $where_arr);
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
    return arraydesign_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'dbxref_id' => array('fk_table' => 'dbxref', 'fk_attr' => 'dbxref_id'),
      'manufacturer_id' => array('fk_table' => 'contact', 'fk_attr' => 'contact_id'),
      'platformtype_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
      'protocol_id' => array('fk_table' => 'protocol', 'fk_attr' => 'protocol_id'),
      'substratetype_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'arraydesign_c1' => array('name'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'arraydesign_id' => array(
        array('table' => 'arraydesignprop', 'attr' => 'arraydesign_id'),
        array('table' => 'assay', 'attr' => 'arraydesign_id'),
        array('table' => 'element', 'attr' => 'arraydesign_id'),
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
        $fields['manufacturer_id']    = $this->manufacturer_id;
        $fields['platformtype_id']    = $this->platformtype_id;
        $fields['name']               = $this->name;
        $fields['version']            = $this->version;
        $fields['description']        = $this->description;
        $fields['array_dimensions']   = $this->array_dimensions;
        $fields['element_dimensions'] = $this->element_dimensions;
        if (is_numeric($this->substratetype_id))  { $fields['substratetype_id']  = $this->substratetype_id; }
        if (is_numeric($this->protocol_id))       { $fields['protocol_id']       = $this->protocol_id; }
        if (is_numeric($this->dbxref_id))         { $fields['dbxref_id']         = $this->dbxref_id; }
        if (is_numeric($this->num_of_elements))   { $fields['num_of_elements']   = $this->num_of_elements; }
        if (is_numeric($this->num_array_columns)) { $fields['num_array_columns'] = $this->num_array_columns; }
        if (is_numeric($this->num_array_rows))    { $fields['num_array_rows']    = $this->num_array_rows; }
        if (is_numeric($this->num_grid_columns))  { $fields['num_grid_columns']  = $this->num_grid_columns; }
        if (is_numeric($this->num_grid_rows))     { $fields['num_grid_rows']     = $this->num_grid_rows; }
        if (is_numeric($this->num_sub_columns))   { $fields['num_sub_columns']   = $this->num_sub_columns; }
        if (is_numeric($this->num_sub_rows))      { $fields['num_sub_rows']      = $this->num_sub_rows; }

        // Inserts the record.
        $this->arraydesign_id = db_insert('chado.arraydesign')
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
      $fields['manufacturer_id']    = $this->manufacturer_id;
      $fields['platformtype_id']    = $this->platformtype_id;
      $fields['name']               = $this->name;
      $fields['version']            = $this->version;
      $fields['description']        = $this->description;
      $fields['array_dimensions']   = $this->array_dimensions;
      $fields['element_dimensions'] = $this->element_dimensions;
      if (is_numeric($this->substratetype_id))  { $fields['substratetype_id']  = $this->substratetype_id; }
      if (is_numeric($this->protocol_id))       { $fields['protocol_id']       = $this->protocol_id; }
      if (is_numeric($this->dbxref_id))         { $fields['dbxref_id']         = $this->dbxref_id; }
      if (is_numeric($this->num_of_elements))   { $fields['num_of_elements']   = $this->num_of_elements; }
      if (is_numeric($this->num_array_columns)) { $fields['num_array_columns'] = $this->num_array_columns; }
      if (is_numeric($this->num_array_rows))    { $fields['num_array_rows']    = $this->num_array_rows; }
      if (is_numeric($this->num_grid_columns))  { $fields['num_grid_columns']  = $this->num_grid_columns; }
      if (is_numeric($this->num_grid_rows))     { $fields['num_grid_rows']     = $this->num_grid_rows; }
      if (is_numeric($this->num_sub_columns))   { $fields['num_sub_columns']   = $this->num_sub_columns; }
      if (is_numeric($this->num_sub_rows))      { $fields['num_sub_rows']      = $this->num_sub_rows; }

      // Updates the record.
      db_update('chado.arraydesign')
        ->fields($fields)
        ->condition('arraydesign_id', $this->arraydesign_id, '=')
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

      db_delete('chado.arraydesign')
        ->condition('arraydesign_id', $this->arraydesign_id, '=')
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
   * Retrieves the arraydesign_id.
   *
   * @retrun serial
   */
  public function getArraydesignID() {
    return $this->arraydesign_id;
  }

  /**
   * Sets the arraydesign_id.
   *
   * @param serial $arraydesign_id
   */
  public function setArraydesignID($arraydesign_id) {
    $this->arraydesign_id = $arraydesign_id;
  }

  /**
   * Retrieves the manufacturer_id.
   *
   * @retrun integer
   */
  public function getManufacturerID() {
    return $this->manufacturer_id;
  }

  /**
   * Sets the manufacturer_id.
   *
   * @param integer $manufacturer_id
   */
  public function setManufacturerID($manufacturer_id) {
    $this->manufacturer_id = $manufacturer_id;
  }

  /**
   * Retrieves the platformtype_id.
   *
   * @retrun integer
   */
  public function getPlatformtypeID() {
    return $this->platformtype_id;
  }

  /**
   * Sets the platformtype_id.
   *
   * @param integer $platformtype_id
   */
  public function setPlatformtypeID($platformtype_id) {
    $this->platformtype_id = $platformtype_id;
  }

  /**
   * Retrieves the substratetype_id.
   *
   * @retrun integer
   */
  public function getSubstratetypeID() {
    return $this->substratetype_id;
  }

  /**
   * Sets the substratetype_id.
   *
   * @param integer $substratetype_id
   */
  public function setSubstratetypeID($substratetype_id) {
    $this->substratetype_id = $substratetype_id;
  }

  /**
   * Retrieves the protocol_id.
   *
   * @retrun integer
   */
  public function getProtocolID() {
    return $this->protocol_id;
  }

  /**
   * Sets the protocol_id.
   *
   * @param integer $protocol_id
   */
  public function setProtocolID($protocol_id) {
    $this->protocol_id = $protocol_id;
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
   * Retrieves the version.
   *
   * @retrun text
   */
  public function getVersion() {
    return $this->version;
  }

  /**
   * Sets the version.
   *
   * @param text $version
   */
  public function setVersion($version) {
    $this->version = $version;
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
   * Retrieves the array_dimensions.
   *
   * @retrun text
   */
  public function getArrayDimensions() {
    return $this->array_dimensions;
  }

  /**
   * Sets the array_dimensions.
   *
   * @param text $array_dimensions
   */
  public function setArrayDimensions($array_dimensions) {
    $this->array_dimensions = $array_dimensions;
  }

  /**
   * Retrieves the element_dimensions.
   *
   * @retrun text
   */
  public function getElementDimensions() {
    return $this->element_dimensions;
  }

  /**
   * Sets the element_dimensions.
   *
   * @param text $element_dimensions
   */
  public function setElementDimensions($element_dimensions) {
    $this->element_dimensions = $element_dimensions;
  }

  /**
   * Retrieves the num_of_elements.
   *
   * @retrun integer
   */
  public function getNumOfElements() {
    return $this->num_of_elements;
  }

  /**
   * Sets the num_of_elements.
   *
   * @param integer $num_of_elements
   */
  public function setNumOfElements($num_of_elements) {
    $this->num_of_elements = $num_of_elements;
  }

  /**
   * Retrieves the num_array_columns.
   *
   * @retrun integer
   */
  public function getNumArrayColumns() {
    return $this->num_array_columns;
  }

  /**
   * Sets the num_array_columns.
   *
   * @param integer $num_array_columns
   */
  public function setNumArrayColumns($num_array_columns) {
    $this->num_array_columns = $num_array_columns;
  }

  /**
   * Retrieves the num_array_rows.
   *
   * @retrun integer
   */
  public function getNumArrayRows() {
    return $this->num_array_rows;
  }

  /**
   * Sets the num_array_rows.
   *
   * @param integer $num_array_rows
   */
  public function setNumArrayRows($num_array_rows) {
    $this->num_array_rows = $num_array_rows;
  }

  /**
   * Retrieves the num_grid_columns.
   *
   * @retrun integer
   */
  public function getNumGridColumns() {
    return $this->num_grid_columns;
  }

  /**
   * Sets the num_grid_columns.
   *
   * @param integer $num_grid_columns
   */
  public function setNumGridColumns($num_grid_columns) {
    $this->num_grid_columns = $num_grid_columns;
  }

  /**
   * Retrieves the num_grid_rows.
   *
   * @retrun integer
   */
  public function getNumGridRows() {
    return $this->num_grid_rows;
  }

  /**
   * Sets the num_grid_rows.
   *
   * @param integer $num_grid_rows
   */
  public function setNumGridRows($num_grid_rows) {
    $this->num_grid_rows = $num_grid_rows;
  }

  /**
   * Retrieves the num_sub_columns.
   *
   * @retrun integer
   */
  public function getNumSubColumns() {
    return $this->num_sub_columns;
  }

  /**
   * Sets the num_sub_columns.
   *
   * @param integer $num_sub_columns
   */
  public function setNumSubColumns($num_sub_columns) {
    $this->num_sub_columns = $num_sub_columns;
  }

  /**
   * Retrieves the num_sub_rows.
   *
   * @retrun integer
   */
  public function getNumSubRows() {
    return $this->num_sub_rows;
  }

  /**
   * Sets the num_sub_rows.
   *
   * @param integer $num_sub_rows
   */
  public function setNumSubRows($num_sub_rows) {
    $this->num_sub_rows = $num_sub_rows;
  }
}