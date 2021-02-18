<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_QUANTIFICATION class.
 *
 */
class CHADO_QUANTIFICATION  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_QUANTIFICATION.
   */
  protected $member_arr         = NULL;
  protected $quantification_id  = NULL;
  protected $acquisition_id     = NULL;
  protected $operator_id        = NULL;
  protected $protocol_id        = NULL;
  protected $analysis_id        = NULL;
  protected $quantificationdate = NULL;
  protected $name               = NULL;
  protected $uri                = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'quantification_id'  => 'serial',
    'acquisition_id'     => 'integer',
    'operator_id'        => 'integer',
    'protocol_id'        => 'integer',
    'analysis_id'        => 'integer',
    'quantificationdate' => 'timestamp_without_time_zone',
    'name'               => 'text',
    'uri'                => 'text',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'quantification_id'  => TRUE,
    'acquisition_id'     => TRUE,
    'operator_id'        => FALSE,
    'protocol_id'        => FALSE,
    'analysis_id'        => TRUE,
    'quantificationdate' => FALSE,
    'name'               => FALSE,
    'uri'                => FALSE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr         = $details;
    $this->quantification_id  = array_key_exists('quantification_id', $details)  ? $details['quantification_id']  : '';
    $this->acquisition_id     = array_key_exists('acquisition_id', $details)     ? $details['acquisition_id']     : '';
    $this->operator_id        = array_key_exists('operator_id', $details)        ? $details['operator_id']        : '';
    $this->protocol_id        = array_key_exists('protocol_id', $details)        ? $details['protocol_id']        : '';
    $this->analysis_id        = array_key_exists('analysis_id', $details)        ? $details['analysis_id']        : '';
    $this->quantificationdate = array_key_exists('quantificationdate', $details) ? $details['quantificationdate'] : NULL;
    $this->name               = array_key_exists('name', $details)               ? $details['name']               : '';
    $this->uri                = array_key_exists('uri', $details)                ? $details['uri']                : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_QUANTIFICATION by key(s).
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
    $sql = "SELECT * FROM chado.quantification WHERE " . implode(" AND " , $where_arr);
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
    return quantification_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'acquisition_id' => array('fk_table' => 'acquisition', 'fk_attr' => 'acquisition_id'),
      'operator_id' => array('fk_table' => 'contact', 'fk_attr' => 'contact_id'),
      'protocol_id' => array('fk_table' => 'protocol', 'fk_attr' => 'protocol_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'quantification_c1' => array('name','analysis_id'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'quantification_id' => array(
        array('table' => 'elementresult', 'attr' => 'quantification_id'),
        array('table' => 'quantification_relationship', 'attr' => 'object_id'),
        array('table' => 'quantification_relationship', 'attr' => 'subject_id'),
        array('table' => 'quantificationprop', 'attr' => 'quantification_id'),
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
        $fields['acquisition_id'] = $this->acquisition_id;
        $fields['analysis_id']    = $this->analysis_id;
        $fields['name']           = $this->name;
        $fields['uri']            = $this->uri;
        if (is_numeric($this->operator_id))    { $fields['operator_id']        = $this->operator_id; }
        if (is_numeric($this->protocol_id))    { $fields['protocol_id']        = $this->protocol_id; }
        if (!empty($this->quantificationdate)) { $fields['quantificationdate'] = $this->quantificationdate; }

        // Inserts the record.
        $this->quantification_id = db_insert('chado.quantification')
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
      $fields['acquisition_id'] = $this->acquisition_id;
      $fields['analysis_id']    = $this->analysis_id;
      $fields['name']           = $this->name;
      $fields['uri']            = $this->uri;
      if (is_numeric($this->operator_id))    { $fields['operator_id']        = $this->operator_id; }
      if (is_numeric($this->protocol_id))    { $fields['protocol_id']        = $this->protocol_id; }
      if (!empty($this->quantificationdate)) { $fields['quantificationdate'] = $this->quantificationdate; }

      // Updates the record.
      db_update('chado.quantification')
        ->fields($fields)
        ->condition('quantification_id', $this->quantification_id, '=')
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

      db_delete('chado.quantification')
        ->condition('quantification_id', $this->quantification_id, '=')
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
   * Retrieves the quantification_id.
   *
   * @retrun serial
   */
  public function getQuantificationID() {
    return $this->quantification_id;
  }

  /**
   * Sets the quantification_id.
   *
   * @param serial $quantification_id
   */
  public function setQuantificationID($quantification_id) {
    $this->quantification_id = $quantification_id;
  }

  /**
   * Retrieves the acquisition_id.
   *
   * @retrun integer
   */
  public function getAcquisitionID() {
    return $this->acquisition_id;
  }

  /**
   * Sets the acquisition_id.
   *
   * @param integer $acquisition_id
   */
  public function setAcquisitionID($acquisition_id) {
    $this->acquisition_id = $acquisition_id;
  }

  /**
   * Retrieves the operator_id.
   *
   * @retrun integer
   */
  public function getOperatorID() {
    return $this->operator_id;
  }

  /**
   * Sets the operator_id.
   *
   * @param integer $operator_id
   */
  public function setOperatorID($operator_id) {
    $this->operator_id = $operator_id;
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
   * Retrieves the quantificationdate.
   *
   * @retrun timestamp_without_time_zone
   */
  public function getQuantificationdate() {
    return $this->quantificationdate;
  }

  /**
   * Sets the quantificationdate.
   *
   * @param timestamp_without_time_zone $quantificationdate
   */
  public function setQuantificationdate($quantificationdate) {
    $this->quantificationdate = $quantificationdate;
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