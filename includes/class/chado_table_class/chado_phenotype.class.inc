<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_PHENOTYPE class.
 *
 */
class CHADO_PHENOTYPE  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_PHENOTYPE.
   */
  protected $member_arr    = NULL;
  protected $phenotype_id  = NULL;
  protected $uniquename    = NULL;
  protected $observable_id = NULL;
  protected $attr_id       = NULL;
  protected $value         = NULL;
  protected $cvalue_id     = NULL;
  protected $assay_id      = NULL;
  protected $name          = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'phenotype_id'  => 'serial',
    'uniquename'    => 'text',
    'observable_id' => 'integer',
    'attr_id'       => 'integer',
    'value'         => 'text',
    'cvalue_id'     => 'integer',
    'assay_id'      => 'integer',
    'name'          => 'text',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'phenotype_id'  => TRUE,
    'uniquename'    => TRUE,
    'observable_id' => FALSE,
    'attr_id'       => FALSE,
    'value'         => FALSE,
    'cvalue_id'     => FALSE,
    'assay_id'      => FALSE,
    'name'          => FALSE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr    = $details;
    $this->phenotype_id  = array_key_exists('phenotype_id', $details)  ? $details['phenotype_id']  : '';
    $this->uniquename    = array_key_exists('uniquename', $details)    ? $details['uniquename']    : '';
    $this->observable_id = array_key_exists('observable_id', $details) ? $details['observable_id'] : '';
    $this->attr_id       = array_key_exists('attr_id', $details)       ? $details['attr_id']       : '';
    $this->value         = array_key_exists('value', $details)         ? $details['value']         : '';
    $this->cvalue_id     = array_key_exists('cvalue_id', $details)     ? $details['cvalue_id']     : '';
    $this->assay_id      = array_key_exists('assay_id', $details)      ? $details['assay_id']      : '';
    $this->name          = array_key_exists('name', $details)          ? $details['name']          : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_PHENOTYPE by key(s).
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
    $sql = "SELECT * FROM chado.phenotype WHERE " . implode(" AND " , $where_arr);
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
    return phenotype_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'assay_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
      'attr_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
      'cvalue_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
      'observable_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'phenotype_c1' => array('uniquename'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'phenotype_id' => array(
        array('table' => 'feature_phenotype', 'attr' => 'phenotype_id'),
        array('table' => 'nd_experiment_phenotype', 'attr' => 'phenotype_id'),
        array('table' => 'phenotype_call', 'attr' => 'phenotype_id'),
        array('table' => 'phenotype_cvterm', 'attr' => 'phenotype_id'),
        array('table' => 'phenstatement', 'attr' => 'phenotype_id'),
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
        $fields['uniquename'] = $this->uniquename;
        $fields['value']      = $this->value;
        $fields['name']       = $this->name;
        if (is_numeric($this->observable_id)) { $fields['observable_id'] = $this->observable_id; }
        if (is_numeric($this->attr_id))       { $fields['attr_id']       = $this->attr_id; }
        if (is_numeric($this->cvalue_id))     { $fields['cvalue_id']     = $this->cvalue_id; }
        if (is_numeric($this->assay_id))      { $fields['assay_id']      = $this->assay_id; }

        // Inserts the record.
        $this->phenotype_id = db_insert('chado.phenotype')
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
      $fields['uniquename'] = $this->uniquename;
      $fields['value']      = $this->value;
      $fields['name']       = $this->name;
      if (is_numeric($this->observable_id)) { $fields['observable_id'] = $this->observable_id; }
      if (is_numeric($this->attr_id))       { $fields['attr_id']       = $this->attr_id; }
      if (is_numeric($this->cvalue_id))     { $fields['cvalue_id']     = $this->cvalue_id; }
      if (is_numeric($this->assay_id))      { $fields['assay_id']      = $this->assay_id; }

      // Updates the record.
      db_update('chado.phenotype')
        ->fields($fields)
        ->condition('phenotype_id', $this->phenotype_id, '=')
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

      db_delete('chado.phenotype')
        ->condition('phenotype_id', $this->phenotype_id, '=')
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
   * Retrieves the phenotype_id.
   *
   * @retrun serial
   */
  public function getPhenotypeID() {
    return $this->phenotype_id;
  }

  /**
   * Sets the phenotype_id.
   *
   * @param serial $phenotype_id
   */
  public function setPhenotypeID($phenotype_id) {
    $this->phenotype_id = $phenotype_id;
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
   * Retrieves the observable_id.
   *
   * @retrun integer
   */
  public function getObservableID() {
    return $this->observable_id;
  }

  /**
   * Sets the observable_id.
   *
   * @param integer $observable_id
   */
  public function setObservableID($observable_id) {
    $this->observable_id = $observable_id;
  }

  /**
   * Retrieves the attr_id.
   *
   * @retrun integer
   */
  public function getAttrID() {
    return $this->attr_id;
  }

  /**
   * Sets the attr_id.
   *
   * @param integer $attr_id
   */
  public function setAttrID($attr_id) {
    $this->attr_id = $attr_id;
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
   * Retrieves the cvalue_id.
   *
   * @retrun integer
   */
  public function getCvalueID() {
    return $this->cvalue_id;
  }

  /**
   * Sets the cvalue_id.
   *
   * @param integer $cvalue_id
   */
  public function setCvalueID($cvalue_id) {
    $this->cvalue_id = $cvalue_id;
  }

  /**
   * Retrieves the assay_id.
   *
   * @retrun integer
   */
  public function getAssayID() {
    return $this->assay_id;
  }

  /**
   * Sets the assay_id.
   *
   * @param integer $assay_id
   */
  public function setAssayID($assay_id) {
    $this->assay_id = $assay_id;
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
}