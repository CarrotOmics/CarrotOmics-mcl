<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_CHANNEL class.
 *
 */
class CHADO_CHANNEL  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_CHANNEL.
   */
  protected $member_arr = NULL;
  protected $channel_id = NULL;
  protected $name       = NULL;
  protected $definition = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'channel_id' => 'serial',
    'name'       => 'text',
    'definition' => 'text',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'channel_id' => TRUE,
    'name'       => TRUE,
    'definition' => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr = $details;
    $this->channel_id = array_key_exists('channel_id', $details) ? $details['channel_id'] : '';
    $this->name       = array_key_exists('name', $details)       ? $details['name']       : '';
    $this->definition = array_key_exists('definition', $details) ? $details['definition'] : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_CHANNEL by key(s).
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
    $sql = "SELECT * FROM chado.channel WHERE " . implode(" AND " , $where_arr);
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
    return channel_id;
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
      'channel_c1' => array('name'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'channel_id' => array(
        array('table' => 'acquisition', 'attr' => 'channel_id'),
        array('table' => 'assay_biomaterial', 'attr' => 'channel_id'),
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
        $fields['name']       = $this->name;
        $fields['definition'] = $this->definition;


        // Inserts the record.
        $this->channel_id = db_insert('chado.channel')
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
      $fields['name']       = $this->name;
      $fields['definition'] = $this->definition;


      // Updates the record.
      db_update('chado.channel')
        ->fields($fields)
        ->condition('channel_id', $this->channel_id, '=')
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

      db_delete('chado.channel')
        ->condition('channel_id', $this->channel_id, '=')
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
   * Retrieves the channel_id.
   *
   * @retrun serial
   */
  public function getChannelID() {
    return $this->channel_id;
  }

  /**
   * Sets the channel_id.
   *
   * @param serial $channel_id
   */
  public function setChannelID($channel_id) {
    $this->channel_id = $channel_id;
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
   * Retrieves the definition.
   *
   * @retrun text
   */
  public function getDefinition() {
    return $this->definition;
  }

  /**
   * Sets the definition.
   *
   * @param text $definition
   */
  public function setDefinition($definition) {
    $this->definition = $definition;
  }
}