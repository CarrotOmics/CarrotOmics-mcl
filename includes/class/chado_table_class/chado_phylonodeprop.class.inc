<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_PHYLONODEPROP class.
 *
 */
class CHADO_PHYLONODEPROP  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_PHYLONODEPROP.
   */
  protected $member_arr       = NULL;
  protected $phylonodeprop_id = NULL;
  protected $phylonode_id     = NULL;
  protected $type_id          = NULL;
  protected $value            = NULL;
  protected $rank             = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'phylonodeprop_id' => 'serial',
    'phylonode_id'     => 'integer',
    'type_id'          => 'integer',
    'value'            => 'text',
    'rank'             => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'phylonodeprop_id' => TRUE,
    'phylonode_id'     => TRUE,
    'type_id'          => TRUE,
    'value'            => TRUE,
    'rank'             => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr       = $details;
    $this->phylonodeprop_id = array_key_exists('phylonodeprop_id', $details) ? $details['phylonodeprop_id'] : '';
    $this->phylonode_id     = array_key_exists('phylonode_id', $details)     ? $details['phylonode_id']     : '';
    $this->type_id          = array_key_exists('type_id', $details)          ? $details['type_id']          : '';
    $this->value            = array_key_exists('value', $details)            ? $details['value']            : '';
    $this->rank             = array_key_exists('rank', $details)             ? $details['rank']             : 0;
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_PHYLONODEPROP by key(s).
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
    $sql = "SELECT * FROM chado.phylonodeprop WHERE " . implode(" AND " , $where_arr);
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
    return phylonodeprop_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'phylonode_id' => array('fk_table' => 'phylonode', 'fk_attr' => 'phylonode_id'),
      'type_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'phylonodeprop_phylonode_id_type_id_value_rank_key' => array('phylonode_id','type_id','value','rank'),
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
        $fields['phylonode_id'] = $this->phylonode_id;
        $fields['type_id']      = $this->type_id;
        $fields['value']        = $this->value;
        $fields['rank']         = (is_numeric($this->rank)) ? $this->rank : 0;


        // Inserts the record.
        $this->phylonodeprop_id = db_insert('chado.phylonodeprop')
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
      $fields['phylonode_id'] = $this->phylonode_id;
      $fields['type_id']      = $this->type_id;
      $fields['value']        = $this->value;
      $fields['rank']         = (is_numeric($this->rank)) ? $this->rank : 0;


      // Updates the record.
      db_update('chado.phylonodeprop')
        ->fields($fields)
        ->condition('phylonodeprop_id', $this->phylonodeprop_id, '=')
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

      db_delete('chado.phylonodeprop')
        ->condition('phylonodeprop_id', $this->phylonodeprop_id, '=')
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
   * Retrieves the phylonodeprop_id.
   *
   * @retrun serial
   */
  public function getPhylonodepropID() {
    return $this->phylonodeprop_id;
  }

  /**
   * Sets the phylonodeprop_id.
   *
   * @param serial $phylonodeprop_id
   */
  public function setPhylonodepropID($phylonodeprop_id) {
    $this->phylonodeprop_id = $phylonodeprop_id;
  }

  /**
   * Retrieves the phylonode_id.
   *
   * @retrun integer
   */
  public function getPhylonodeID() {
    return $this->phylonode_id;
  }

  /**
   * Sets the phylonode_id.
   *
   * @param integer $phylonode_id
   */
  public function setPhylonodeID($phylonode_id) {
    $this->phylonode_id = $phylonode_id;
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