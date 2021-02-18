<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_PHYLOTREE class.
 *
 */
class CHADO_PHYLOTREE  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_PHYLOTREE.
   */
  protected $member_arr   = NULL;
  protected $phylotree_id = NULL;
  protected $dbxref_id    = NULL;
  protected $name         = NULL;
  protected $type_id      = NULL;
  protected $analysis_id  = NULL;
  protected $comment      = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'phylotree_id' => 'serial',
    'dbxref_id'    => 'integer',
    'name'         => 'character_varying(255)',
    'type_id'      => 'integer',
    'analysis_id'  => 'integer',
    'comment'      => 'text',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'phylotree_id' => TRUE,
    'dbxref_id'    => TRUE,
    'name'         => FALSE,
    'type_id'      => FALSE,
    'analysis_id'  => FALSE,
    'comment'      => FALSE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr   = $details;
    $this->phylotree_id = array_key_exists('phylotree_id', $details) ? $details['phylotree_id'] : '';
    $this->dbxref_id    = array_key_exists('dbxref_id', $details)    ? $details['dbxref_id']    : '';
    $this->name         = array_key_exists('name', $details)         ? $details['name']         : '';
    $this->type_id      = array_key_exists('type_id', $details)      ? $details['type_id']      : '';
    $this->analysis_id  = array_key_exists('analysis_id', $details)  ? $details['analysis_id']  : '';
    $this->comment      = array_key_exists('comment', $details)      ? $details['comment']      : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_PHYLOTREE by key(s).
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
    $sql = "SELECT * FROM chado.phylotree WHERE " . implode(" AND " , $where_arr);
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
    return phylotree_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'dbxref_id' => array('fk_table' => 'dbxref', 'fk_attr' => 'dbxref_id'),
      'type_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return NULL;
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'phylotree_id' => array(
        array('table' => 'phylonode', 'attr' => 'phylotree_id'),
        array('table' => 'phylonode_relationship', 'attr' => 'phylotree_id'),
        array('table' => 'phylotree_pub', 'attr' => 'phylotree_id'),
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
        $fields['dbxref_id'] = $this->dbxref_id;
        $fields['name']      = $this->name;
        $fields['comment']   = $this->comment;
        if (is_numeric($this->type_id))     { $fields['type_id']     = $this->type_id; }
        if (is_numeric($this->analysis_id)) { $fields['analysis_id'] = $this->analysis_id; }

        // Inserts the record.
        $this->phylotree_id = db_insert('chado.phylotree')
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
      $fields['dbxref_id'] = $this->dbxref_id;
      $fields['name']      = $this->name;
      $fields['comment']   = $this->comment;
      if (is_numeric($this->type_id))     { $fields['type_id']     = $this->type_id; }
      if (is_numeric($this->analysis_id)) { $fields['analysis_id'] = $this->analysis_id; }

      // Updates the record.
      db_update('chado.phylotree')
        ->fields($fields)
        ->condition('phylotree_id', $this->phylotree_id, '=')
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

      db_delete('chado.phylotree')
        ->condition('phylotree_id', $this->phylotree_id, '=')
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
   * Retrieves the phylotree_id.
   *
   * @retrun serial
   */
  public function getPhylotreeID() {
    return $this->phylotree_id;
  }

  /**
   * Sets the phylotree_id.
   *
   * @param serial $phylotree_id
   */
  public function setPhylotreeID($phylotree_id) {
    $this->phylotree_id = $phylotree_id;
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
   * Retrieves the analysis_id.
   *
   * @retrun integer
   */
  public function getAnalysisID() {
    return $this->analysis_id;
  }

  /**
   * Sets the analysis_id.
   *
   * @param integer $analysis_id
   */
  public function setAnalysisID($analysis_id) {
    $this->analysis_id = $analysis_id;
  }

  /**
   * Retrieves the comment.
   *
   * @retrun text
   */
  public function getComment() {
    return $this->comment;
  }

  /**
   * Sets the comment.
   *
   * @param text $comment
   */
  public function setComment($comment) {
    $this->comment = $comment;
  }
}