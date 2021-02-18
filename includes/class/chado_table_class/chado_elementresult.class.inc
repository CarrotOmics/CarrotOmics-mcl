<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_ELEMENTRESULT class.
 *
 */
class CHADO_ELEMENTRESULT  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_ELEMENTRESULT.
   */
  protected $member_arr        = NULL;
  protected $elementresult_id  = NULL;
  protected $element_id        = NULL;
  protected $quantification_id = NULL;
  protected $signal            = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'elementresult_id'  => 'serial',
    'element_id'        => 'integer',
    'quantification_id' => 'integer',
    'signal'            => 'double',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'elementresult_id'  => TRUE,
    'element_id'        => TRUE,
    'quantification_id' => TRUE,
    'signal'            => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr        = $details;
    $this->elementresult_id  = array_key_exists('elementresult_id', $details)  ? $details['elementresult_id']  : '';
    $this->element_id        = array_key_exists('element_id', $details)        ? $details['element_id']        : '';
    $this->quantification_id = array_key_exists('quantification_id', $details) ? $details['quantification_id'] : '';
    $this->signal            = array_key_exists('signal', $details)            ? $details['signal']            : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_ELEMENTRESULT by key(s).
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
    $sql = "SELECT * FROM chado.elementresult WHERE " . implode(" AND " , $where_arr);
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
    return elementresult_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'element_id' => array('fk_table' => 'element', 'fk_attr' => 'element_id'),
      'quantification_id' => array('fk_table' => 'quantification', 'fk_attr' => 'quantification_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'elementresult_c1' => array('element_id','quantification_id'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'elementresult_id' => array(
        array('table' => 'elementresult_relationship', 'attr' => 'object_id'),
        array('table' => 'elementresult_relationship', 'attr' => 'subject_id'),
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
        $fields['element_id']        = $this->element_id;
        $fields['quantification_id'] = $this->quantification_id;
        $fields['signal']            = $this->signal;


        // Inserts the record.
        $this->elementresult_id = db_insert('chado.elementresult')
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
      $fields['element_id']        = $this->element_id;
      $fields['quantification_id'] = $this->quantification_id;
      $fields['signal']            = $this->signal;


      // Updates the record.
      db_update('chado.elementresult')
        ->fields($fields)
        ->condition('elementresult_id', $this->elementresult_id, '=')
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

      db_delete('chado.elementresult')
        ->condition('elementresult_id', $this->elementresult_id, '=')
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
   * Retrieves the elementresult_id.
   *
   * @retrun serial
   */
  public function getElementresultID() {
    return $this->elementresult_id;
  }

  /**
   * Sets the elementresult_id.
   *
   * @param serial $elementresult_id
   */
  public function setElementresultID($elementresult_id) {
    $this->elementresult_id = $elementresult_id;
  }

  /**
   * Retrieves the element_id.
   *
   * @retrun integer
   */
  public function getElementID() {
    return $this->element_id;
  }

  /**
   * Sets the element_id.
   *
   * @param integer $element_id
   */
  public function setElementID($element_id) {
    $this->element_id = $element_id;
  }

  /**
   * Retrieves the quantification_id.
   *
   * @retrun integer
   */
  public function getQuantificationID() {
    return $this->quantification_id;
  }

  /**
   * Sets the quantification_id.
   *
   * @param integer $quantification_id
   */
  public function setQuantificationID($quantification_id) {
    $this->quantification_id = $quantification_id;
  }

  /**
   * Retrieves the signal.
   *
   * @retrun double
   */
  public function getSignal() {
    return $this->signal;
  }

  /**
   * Sets the signal.
   *
   * @param double $signal
   */
  public function setSignal($signal) {
    $this->signal = $signal;
  }
}