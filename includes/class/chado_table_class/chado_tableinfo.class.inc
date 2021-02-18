<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_TABLEINFO class.
 *
 */
class CHADO_TABLEINFO  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_TABLEINFO.
   */
  protected $member_arr          = NULL;
  protected $tableinfo_id        = NULL;
  protected $name                = NULL;
  protected $primary_key_column  = NULL;
  protected $is_view             = NULL;
  protected $view_on_table_id    = NULL;
  protected $superclass_table_id = NULL;
  protected $is_updateable       = NULL;
  protected $modification_date   = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'tableinfo_id'        => 'serial',
    'name'                => 'character_varying(30)',
    'primary_key_column'  => 'character_varying(30)',
    'is_view'             => 'integer',
    'view_on_table_id'    => 'integer',
    'superclass_table_id' => 'integer',
    'is_updateable'       => 'integer',
    'modification_date'   => 'date',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'tableinfo_id'        => TRUE,
    'name'                => TRUE,
    'primary_key_column'  => FALSE,
    'is_view'             => TRUE,
    'view_on_table_id'    => FALSE,
    'superclass_table_id' => FALSE,
    'is_updateable'       => TRUE,
    'modification_date'   => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr          = $details;
    $this->tableinfo_id        = array_key_exists('tableinfo_id', $details)        ? $details['tableinfo_id']        : '';
    $this->name                = array_key_exists('name', $details)                ? $details['name']                : '';
    $this->primary_key_column  = array_key_exists('primary_key_column', $details)  ? $details['primary_key_column']  : '';
    $this->is_view             = array_key_exists('is_view', $details)             ? $details['is_view']             : 0;
    $this->view_on_table_id    = array_key_exists('view_on_table_id', $details)    ? $details['view_on_table_id']    : '';
    $this->superclass_table_id = array_key_exists('superclass_table_id', $details) ? $details['superclass_table_id'] : '';
    $this->is_updateable       = array_key_exists('is_updateable', $details)       ? $details['is_updateable']       : 1;
    $this->modification_date   = array_key_exists('modification_date', $details)   ? $details['modification_date']   : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_TABLEINFO by key(s).
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
    $sql = "SELECT * FROM chado.tableinfo WHERE " . implode(" AND " , $where_arr);
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
    return tableinfo_id;
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
      'tableinfo_c1' => array('name'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'tableinfo_id' => array(
        array('table' => 'control', 'attr' => 'tableinfo_id'),
        array('table' => 'magedocumentation', 'attr' => 'tableinfo_id'),
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
        $fields['name']               = $this->name;
        $fields['primary_key_column'] = $this->primary_key_column;
        $fields['is_view']            = (is_numeric($this->is_view))       ? $this->is_view       : 0;
        $fields['is_updateable']      = (is_numeric($this->is_updateable)) ? $this->is_updateable : 1;
        $fields['modification_date']  = $this->modification_date;
        if (is_numeric($this->view_on_table_id))    { $fields['view_on_table_id']    = $this->view_on_table_id; }
        if (is_numeric($this->superclass_table_id)) { $fields['superclass_table_id'] = $this->superclass_table_id; }

        // Inserts the record.
        $this->tableinfo_id = db_insert('chado.tableinfo')
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
      $fields['name']               = $this->name;
      $fields['primary_key_column'] = $this->primary_key_column;
      $fields['is_view']            = (is_numeric($this->is_view))       ? $this->is_view       : 0;
      $fields['is_updateable']      = (is_numeric($this->is_updateable)) ? $this->is_updateable : 1;
      $fields['modification_date']  = $this->modification_date;
      if (is_numeric($this->view_on_table_id))    { $fields['view_on_table_id']    = $this->view_on_table_id; }
      if (is_numeric($this->superclass_table_id)) { $fields['superclass_table_id'] = $this->superclass_table_id; }

      // Updates the record.
      db_update('chado.tableinfo')
        ->fields($fields)
        ->condition('tableinfo_id', $this->tableinfo_id, '=')
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

      db_delete('chado.tableinfo')
        ->condition('tableinfo_id', $this->tableinfo_id, '=')
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
   * Retrieves the tableinfo_id.
   *
   * @retrun serial
   */
  public function getTableinfoID() {
    return $this->tableinfo_id;
  }

  /**
   * Sets the tableinfo_id.
   *
   * @param serial $tableinfo_id
   */
  public function setTableinfoID($tableinfo_id) {
    $this->tableinfo_id = $tableinfo_id;
  }

  /**
   * Retrieves the name.
   *
   * @retrun character_varying(30)
   */
  public function getName() {
    return $this->name;
  }

  /**
   * Sets the name.
   *
   * @param character_varying(30) $name
   */
  public function setName($name) {
    $this->name = $name;
  }

  /**
   * Retrieves the primary_key_column.
   *
   * @retrun character_varying(30)
   */
  public function getPrimaryKeyColumn() {
    return $this->primary_key_column;
  }

  /**
   * Sets the primary_key_column.
   *
   * @param character_varying(30) $primary_key_column
   */
  public function setPrimaryKeyColumn($primary_key_column) {
    $this->primary_key_column = $primary_key_column;
  }

  /**
   * Retrieves the is_view.
   *
   * @retrun integer
   */
  public function getIsView() {
    return $this->is_view;
  }

  /**
   * Sets the is_view.
   *
   * @param integer $is_view
   */
  public function setIsView($is_view) {
    $this->is_view = $is_view;
  }

  /**
   * Retrieves the view_on_table_id.
   *
   * @retrun integer
   */
  public function getViewOnTableID() {
    return $this->view_on_table_id;
  }

  /**
   * Sets the view_on_table_id.
   *
   * @param integer $view_on_table_id
   */
  public function setViewOnTableID($view_on_table_id) {
    $this->view_on_table_id = $view_on_table_id;
  }

  /**
   * Retrieves the superclass_table_id.
   *
   * @retrun integer
   */
  public function getSuperclassTableID() {
    return $this->superclass_table_id;
  }

  /**
   * Sets the superclass_table_id.
   *
   * @param integer $superclass_table_id
   */
  public function setSuperclassTableID($superclass_table_id) {
    $this->superclass_table_id = $superclass_table_id;
  }

  /**
   * Retrieves the is_updateable.
   *
   * @retrun integer
   */
  public function getIsUpdateable() {
    return $this->is_updateable;
  }

  /**
   * Sets the is_updateable.
   *
   * @param integer $is_updateable
   */
  public function setIsUpdateable($is_updateable) {
    $this->is_updateable = $is_updateable;
  }

  /**
   * Retrieves the modification_date.
   *
   * @retrun date
   */
  public function getModificationDate() {
    return $this->modification_date;
  }

  /**
   * Sets the modification_date.
   *
   * @param date $modification_date
   */
  public function setModificationDate($modification_date) {
    $this->modification_date = $modification_date;
  }
}