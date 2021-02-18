<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_PROTOCOL class.
 *
 */
class CHADO_PROTOCOL  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_PROTOCOL.
   */
  protected $member_arr          = NULL;
  protected $protocol_id         = NULL;
  protected $type_id             = NULL;
  protected $pub_id              = NULL;
  protected $dbxref_id           = NULL;
  protected $name                = NULL;
  protected $uri                 = NULL;
  protected $protocoldescription = NULL;
  protected $hardwaredescription = NULL;
  protected $softwaredescription = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'protocol_id'         => 'serial',
    'type_id'             => 'integer',
    'pub_id'              => 'integer',
    'dbxref_id'           => 'integer',
    'name'                => 'text',
    'uri'                 => 'text',
    'protocoldescription' => 'text',
    'hardwaredescription' => 'text',
    'softwaredescription' => 'text',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'protocol_id'         => TRUE,
    'type_id'             => TRUE,
    'pub_id'              => FALSE,
    'dbxref_id'           => FALSE,
    'name'                => TRUE,
    'uri'                 => FALSE,
    'protocoldescription' => FALSE,
    'hardwaredescription' => FALSE,
    'softwaredescription' => FALSE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr          = $details;
    $this->protocol_id         = array_key_exists('protocol_id', $details)         ? $details['protocol_id']         : '';
    $this->type_id             = array_key_exists('type_id', $details)             ? $details['type_id']             : '';
    $this->pub_id              = array_key_exists('pub_id', $details)              ? $details['pub_id']              : '';
    $this->dbxref_id           = array_key_exists('dbxref_id', $details)           ? $details['dbxref_id']           : '';
    $this->name                = array_key_exists('name', $details)                ? $details['name']                : '';
    $this->uri                 = array_key_exists('uri', $details)                 ? $details['uri']                 : '';
    $this->protocoldescription = array_key_exists('protocoldescription', $details) ? $details['protocoldescription'] : '';
    $this->hardwaredescription = array_key_exists('hardwaredescription', $details) ? $details['hardwaredescription'] : '';
    $this->softwaredescription = array_key_exists('softwaredescription', $details) ? $details['softwaredescription'] : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_PROTOCOL by key(s).
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
    $sql = "SELECT * FROM chado.protocol WHERE " . implode(" AND " , $where_arr);
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
    return protocol_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'dbxref_id' => array('fk_table' => 'dbxref', 'fk_attr' => 'dbxref_id'),
      'pub_id' => array('fk_table' => 'pub', 'fk_attr' => 'pub_id'),
      'type_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'protocol_c1' => array('name'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'protocol_id' => array(
        array('table' => 'acquisition', 'attr' => 'protocol_id'),
        array('table' => 'arraydesign', 'attr' => 'protocol_id'),
        array('table' => 'assay', 'attr' => 'protocol_id'),
        array('table' => 'protocolparam', 'attr' => 'protocol_id'),
        array('table' => 'quantification', 'attr' => 'protocol_id'),
        array('table' => 'treatment', 'attr' => 'protocol_id'),
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
        $fields['type_id']             = $this->type_id;
        $fields['name']                = $this->name;
        $fields['uri']                 = $this->uri;
        $fields['protocoldescription'] = $this->protocoldescription;
        $fields['hardwaredescription'] = $this->hardwaredescription;
        $fields['softwaredescription'] = $this->softwaredescription;
        if (is_numeric($this->pub_id))    { $fields['pub_id']    = $this->pub_id; }
        if (is_numeric($this->dbxref_id)) { $fields['dbxref_id'] = $this->dbxref_id; }

        // Inserts the record.
        $this->protocol_id = db_insert('chado.protocol')
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
      $fields['type_id']             = $this->type_id;
      $fields['name']                = $this->name;
      $fields['uri']                 = $this->uri;
      $fields['protocoldescription'] = $this->protocoldescription;
      $fields['hardwaredescription'] = $this->hardwaredescription;
      $fields['softwaredescription'] = $this->softwaredescription;
      if (is_numeric($this->pub_id))    { $fields['pub_id']    = $this->pub_id; }
      if (is_numeric($this->dbxref_id)) { $fields['dbxref_id'] = $this->dbxref_id; }

      // Updates the record.
      db_update('chado.protocol')
        ->fields($fields)
        ->condition('protocol_id', $this->protocol_id, '=')
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

      db_delete('chado.protocol')
        ->condition('protocol_id', $this->protocol_id, '=')
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
   * Retrieves the protocol_id.
   *
   * @retrun serial
   */
  public function getProtocolID() {
    return $this->protocol_id;
  }

  /**
   * Sets the protocol_id.
   *
   * @param serial $protocol_id
   */
  public function setProtocolID($protocol_id) {
    $this->protocol_id = $protocol_id;
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
   * Retrieves the uri.
   *
   * @retrun text
   */
  public function getUri() {
    return $this->uri;
  }

  /**
   * Sets the uri.
   *
   * @param text $uri
   */
  public function setUri($uri) {
    $this->uri = $uri;
  }

  /**
   * Retrieves the protocoldescription.
   *
   * @retrun text
   */
  public function getProtocoldescription() {
    return $this->protocoldescription;
  }

  /**
   * Sets the protocoldescription.
   *
   * @param text $protocoldescription
   */
  public function setProtocoldescription($protocoldescription) {
    $this->protocoldescription = $protocoldescription;
  }

  /**
   * Retrieves the hardwaredescription.
   *
   * @retrun text
   */
  public function getHardwaredescription() {
    return $this->hardwaredescription;
  }

  /**
   * Sets the hardwaredescription.
   *
   * @param text $hardwaredescription
   */
  public function setHardwaredescription($hardwaredescription) {
    $this->hardwaredescription = $hardwaredescription;
  }

  /**
   * Retrieves the softwaredescription.
   *
   * @retrun text
   */
  public function getSoftwaredescription() {
    return $this->softwaredescription;
  }

  /**
   * Sets the softwaredescription.
   *
   * @param text $softwaredescription
   */
  public function setSoftwaredescription($softwaredescription) {
    $this->softwaredescription = $softwaredescription;
  }
}