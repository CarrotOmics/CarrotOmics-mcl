<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_PHYLONODE_PUB class.
 *
 */
class CHADO_PHYLONODE_PUB  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_PHYLONODE_PUB.
   */
  protected $member_arr       = NULL;
  protected $phylonode_pub_id = NULL;
  protected $phylonode_id     = NULL;
  protected $pub_id           = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'phylonode_pub_id' => 'serial',
    'phylonode_id'     => 'integer',
    'pub_id'           => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'phylonode_pub_id' => TRUE,
    'phylonode_id'     => TRUE,
    'pub_id'           => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr       = $details;
    $this->phylonode_pub_id = array_key_exists('phylonode_pub_id', $details) ? $details['phylonode_pub_id'] : '';
    $this->phylonode_id     = array_key_exists('phylonode_id', $details)     ? $details['phylonode_id']     : '';
    $this->pub_id           = array_key_exists('pub_id', $details)           ? $details['pub_id']           : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_PHYLONODE_PUB by key(s).
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
    $sql = "SELECT * FROM chado.phylonode_pub WHERE " . implode(" AND " , $where_arr);
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
    return phylonode_pub_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'phylonode_id' => array('fk_table' => 'phylonode', 'fk_attr' => 'phylonode_id'),
      'pub_id' => array('fk_table' => 'pub', 'fk_attr' => 'pub_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'phylonode_pub_phylonode_id_pub_id_key' => array('phylonode_id','pub_id'),
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
        $fields['pub_id']       = $this->pub_id;


        // Inserts the record.
        $this->phylonode_pub_id = db_insert('chado.phylonode_pub')
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
      $fields['pub_id']       = $this->pub_id;


      // Updates the record.
      db_update('chado.phylonode_pub')
        ->fields($fields)
        ->condition('phylonode_pub_id', $this->phylonode_pub_id, '=')
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

      db_delete('chado.phylonode_pub')
        ->condition('phylonode_pub_id', $this->phylonode_pub_id, '=')
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
   * Retrieves the phylonode_pub_id.
   *
   * @retrun serial
   */
  public function getPhylonodePubID() {
    return $this->phylonode_pub_id;
  }

  /**
   * Sets the phylonode_pub_id.
   *
   * @param serial $phylonode_pub_id
   */
  public function setPhylonodePubID($phylonode_pub_id) {
    $this->phylonode_pub_id = $phylonode_pub_id;
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
   * Retrieves the pub_id.
   *
   * @retrun integer
   */
  public function getPubID() {
    return $this->pub_id;
  }

  /**
   * Sets the pub_id.
   *
   * @param integer $pub_id
   */
  public function setPubID($pub_id) {
    $this->pub_id = $pub_id;
  }
}