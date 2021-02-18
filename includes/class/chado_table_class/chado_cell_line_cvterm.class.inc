<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_CELL_LINE_CVTERM class.
 *
 */
class CHADO_CELL_LINE_CVTERM  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_CELL_LINE_CVTERM.
   */
  protected $member_arr          = NULL;
  protected $cell_line_cvterm_id = NULL;
  protected $cell_line_id        = NULL;
  protected $cvterm_id           = NULL;
  protected $pub_id              = NULL;
  protected $rank                = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'cell_line_cvterm_id' => 'serial',
    'cell_line_id'        => 'integer',
    'cvterm_id'           => 'integer',
    'pub_id'              => 'integer',
    'rank'                => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'cell_line_cvterm_id' => TRUE,
    'cell_line_id'        => TRUE,
    'cvterm_id'           => TRUE,
    'pub_id'              => TRUE,
    'rank'                => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr          = $details;
    $this->cell_line_cvterm_id = array_key_exists('cell_line_cvterm_id', $details) ? $details['cell_line_cvterm_id'] : '';
    $this->cell_line_id        = array_key_exists('cell_line_id', $details)        ? $details['cell_line_id']        : '';
    $this->cvterm_id           = array_key_exists('cvterm_id', $details)           ? $details['cvterm_id']           : '';
    $this->pub_id              = array_key_exists('pub_id', $details)              ? $details['pub_id']              : '';
    $this->rank                = array_key_exists('rank', $details)                ? $details['rank']                : 0;
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_CELL_LINE_CVTERM by key(s).
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
    $sql = "SELECT * FROM chado.cell_line_cvterm WHERE " . implode(" AND " , $where_arr);
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
    return cell_line_cvterm_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'cell_line_id' => array('fk_table' => 'cell_line', 'fk_attr' => 'cell_line_id'),
      'cvterm_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
      'pub_id' => array('fk_table' => 'pub', 'fk_attr' => 'pub_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'cell_line_cvterm_c1' => array('cell_line_id','cvterm_id','pub_id','rank'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'cell_line_cvterm_id' => array(
        array('table' => 'cell_line_cvtermprop', 'attr' => 'cell_line_cvterm_id'),
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
        $fields['cell_line_id'] = $this->cell_line_id;
        $fields['cvterm_id']    = $this->cvterm_id;
        $fields['pub_id']       = $this->pub_id;
        $fields['rank']         = (is_numeric($this->rank)) ? $this->rank : 0;


        // Inserts the record.
        $this->cell_line_cvterm_id = db_insert('chado.cell_line_cvterm')
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
      $fields['cell_line_id'] = $this->cell_line_id;
      $fields['cvterm_id']    = $this->cvterm_id;
      $fields['pub_id']       = $this->pub_id;
      $fields['rank']         = (is_numeric($this->rank)) ? $this->rank : 0;


      // Updates the record.
      db_update('chado.cell_line_cvterm')
        ->fields($fields)
        ->condition('cell_line_cvterm_id', $this->cell_line_cvterm_id, '=')
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

      db_delete('chado.cell_line_cvterm')
        ->condition('cell_line_cvterm_id', $this->cell_line_cvterm_id, '=')
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
   * Retrieves the cell_line_cvterm_id.
   *
   * @retrun serial
   */
  public function getCellLineCvtermID() {
    return $this->cell_line_cvterm_id;
  }

  /**
   * Sets the cell_line_cvterm_id.
   *
   * @param serial $cell_line_cvterm_id
   */
  public function setCellLineCvtermID($cell_line_cvterm_id) {
    $this->cell_line_cvterm_id = $cell_line_cvterm_id;
  }

  /**
   * Retrieves the cell_line_id.
   *
   * @retrun integer
   */
  public function getCellLineID() {
    return $this->cell_line_id;
  }

  /**
   * Sets the cell_line_id.
   *
   * @param integer $cell_line_id
   */
  public function setCellLineID($cell_line_id) {
    $this->cell_line_id = $cell_line_id;
  }

  /**
   * Retrieves the cvterm_id.
   *
   * @retrun integer
   */
  public function getCvtermID() {
    return $this->cvterm_id;
  }

  /**
   * Sets the cvterm_id.
   *
   * @param integer $cvterm_id
   */
  public function setCvtermID($cvterm_id) {
    $this->cvterm_id = $cvterm_id;
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