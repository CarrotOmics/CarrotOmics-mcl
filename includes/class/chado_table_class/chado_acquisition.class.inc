<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_ACQUISITION class.
 *
 */
class CHADO_ACQUISITION  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_ACQUISITION.
   */
  protected $member_arr      = NULL;
  protected $acquisition_id  = NULL;
  protected $assay_id        = NULL;
  protected $protocol_id     = NULL;
  protected $channel_id      = NULL;
  protected $acquisitiondate = NULL;
  protected $name            = NULL;
  protected $uri             = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'acquisition_id'  => 'serial',
    'assay_id'        => 'integer',
    'protocol_id'     => 'integer',
    'channel_id'      => 'integer',
    'acquisitiondate' => 'timestamp_without_time_zone',
    'name'            => 'text',
    'uri'             => 'text',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'acquisition_id'  => TRUE,
    'assay_id'        => TRUE,
    'protocol_id'     => FALSE,
    'channel_id'      => FALSE,
    'acquisitiondate' => FALSE,
    'name'            => FALSE,
    'uri'             => FALSE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr      = $details;
    $this->acquisition_id  = array_key_exists('acquisition_id', $details)  ? $details['acquisition_id']  : '';
    $this->assay_id        = array_key_exists('assay_id', $details)        ? $details['assay_id']        : '';
    $this->protocol_id     = array_key_exists('protocol_id', $details)     ? $details['protocol_id']     : '';
    $this->channel_id      = array_key_exists('channel_id', $details)      ? $details['channel_id']      : '';
    $this->acquisitiondate = array_key_exists('acquisitiondate', $details) ? $details['acquisitiondate'] : NULL;
    $this->name            = array_key_exists('name', $details)            ? $details['name']            : '';
    $this->uri             = array_key_exists('uri', $details)             ? $details['uri']             : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_ACQUISITION by key(s).
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
    $sql = "SELECT * FROM chado.acquisition WHERE " . implode(" AND " , $where_arr);
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
    return acquisition_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'assay_id' => array('fk_table' => 'assay', 'fk_attr' => 'assay_id'),
      'channel_id' => array('fk_table' => 'channel', 'fk_attr' => 'channel_id'),
      'protocol_id' => array('fk_table' => 'protocol', 'fk_attr' => 'protocol_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'acquisition_c1' => array('name'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'acquisition_id' => array(
        array('table' => 'acquisition_relationship', 'attr' => 'object_id'),
        array('table' => 'acquisition_relationship', 'attr' => 'subject_id'),
        array('table' => 'acquisitionprop', 'attr' => 'acquisition_id'),
        array('table' => 'quantification', 'attr' => 'acquisition_id'),
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
        $fields['assay_id'] = $this->assay_id;
        $fields['name']     = $this->name;
        $fields['uri']      = $this->uri;
        if (is_numeric($this->protocol_id)) { $fields['protocol_id']     = $this->protocol_id; }
        if (is_numeric($this->channel_id))  { $fields['channel_id']      = $this->channel_id; }
        if (!empty($this->acquisitiondate)) { $fields['acquisitiondate'] = $this->acquisitiondate; }

        // Inserts the record.
        $this->acquisition_id = db_insert('chado.acquisition')
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
      $fields['assay_id'] = $this->assay_id;
      $fields['name']     = $this->name;
      $fields['uri']      = $this->uri;
      if (is_numeric($this->protocol_id)) { $fields['protocol_id']     = $this->protocol_id; }
      if (is_numeric($this->channel_id))  { $fields['channel_id']      = $this->channel_id; }
      if (!empty($this->acquisitiondate)) { $fields['acquisitiondate'] = $this->acquisitiondate; }

      // Updates the record.
      db_update('chado.acquisition')
        ->fields($fields)
        ->condition('acquisition_id', $this->acquisition_id, '=')
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

      db_delete('chado.acquisition')
        ->condition('acquisition_id', $this->acquisition_id, '=')
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
   * Retrieves the acquisition_id.
   *
   * @retrun serial
   */
  public function getAcquisitionID() {
    return $this->acquisition_id;
  }

  /**
   * Sets the acquisition_id.
   *
   * @param serial $acquisition_id
   */
  public function setAcquisitionID($acquisition_id) {
    $this->acquisition_id = $acquisition_id;
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
   * Retrieves the channel_id.
   *
   * @retrun integer
   */
  public function getChannelID() {
    return $this->channel_id;
  }

  /**
   * Sets the channel_id.
   *
   * @param integer $channel_id
   */
  public function setChannelID($channel_id) {
    $this->channel_id = $channel_id;
  }

  /**
   * Retrieves the acquisitiondate.
   *
   * @retrun timestamp_without_time_zone
   */
  public function getAcquisitiondate() {
    return $this->acquisitiondate;
  }

  /**
   * Sets the acquisitiondate.
   *
   * @param timestamp_without_time_zone $acquisitiondate
   */
  public function setAcquisitiondate($acquisitiondate) {
    $this->acquisitiondate = $acquisitiondate;
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
}