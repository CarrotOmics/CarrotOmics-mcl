<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_PUBAUTHOR class.
 *
 */
class CHADO_PUBAUTHOR  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_PUBAUTHOR.
   */
  protected $member_arr   = NULL;
  protected $pubauthor_id = NULL;
  protected $pub_id       = NULL;
  protected $rank         = NULL;
  protected $editor       = NULL;
  protected $surname      = NULL;
  protected $givennames   = NULL;
  protected $suffix       = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'pubauthor_id' => 'serial',
    'pub_id'       => 'integer',
    'rank'         => 'integer',
    'editor'       => 'boolean',
    'surname'      => 'character_varying(100)',
    'givennames'   => 'character_varying(100)',
    'suffix'       => 'character_varying(100)',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'pubauthor_id' => TRUE,
    'pub_id'       => TRUE,
    'rank'         => TRUE,
    'editor'       => FALSE,
    'surname'      => TRUE,
    'givennames'   => FALSE,
    'suffix'       => FALSE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr   = $details;
    $this->pubauthor_id = array_key_exists('pubauthor_id', $details) ? $details['pubauthor_id'] : '';
    $this->pub_id       = array_key_exists('pub_id', $details)       ? $details['pub_id']       : '';
    $this->rank         = array_key_exists('rank', $details)         ? $details['rank']         : '';
    $this->editor       = array_key_exists('editor', $details)       ? $details['editor']       : '';
    $this->surname      = array_key_exists('surname', $details)      ? $details['surname']      : '';
    $this->givennames   = array_key_exists('givennames', $details)   ? $details['givennames']   : '';
    $this->suffix       = array_key_exists('suffix', $details)       ? $details['suffix']       : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_PUBAUTHOR by key(s).
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
    $sql = "SELECT * FROM chado.pubauthor WHERE " . implode(" AND " , $where_arr);
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
    return pubauthor_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'pub_id' => array('fk_table' => 'pub', 'fk_attr' => 'pub_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'pubauthor_c1' => array('pub_id','rank'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'pubauthor_id' => array(
        array('table' => 'pubauthor_contact', 'attr' => 'pubauthor_id'),
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
        $fields['pub_id']     = $this->pub_id;
        $fields['rank']       = $this->rank;
        $fields['surname']    = $this->surname;
        $fields['givennames'] = $this->givennames;
        $fields['suffix']     = $this->suffix;
        if (preg_match("/^(true|false)/i", $this->editor)) { $fields['editor'] = '$this->editor'; }

        // Inserts the record.
        $this->pubauthor_id = db_insert('chado.pubauthor')
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
      $fields['pub_id']     = $this->pub_id;
      $fields['rank']       = $this->rank;
      $fields['surname']    = $this->surname;
      $fields['givennames'] = $this->givennames;
      $fields['suffix']     = $this->suffix;
      if (preg_match("/^(true|false)/i", $this->editor)) { $fields['editor'] = '$this->editor'; }

      // Updates the record.
      db_update('chado.pubauthor')
        ->fields($fields)
        ->condition('pubauthor_id', $this->pubauthor_id, '=')
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

      db_delete('chado.pubauthor')
        ->condition('pubauthor_id', $this->pubauthor_id, '=')
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
   * Retrieves the pubauthor_id.
   *
   * @retrun serial
   */
  public function getPubauthorID() {
    return $this->pubauthor_id;
  }

  /**
   * Sets the pubauthor_id.
   *
   * @param serial $pubauthor_id
   */
  public function setPubauthorID($pubauthor_id) {
    $this->pubauthor_id = $pubauthor_id;
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

  /**
   * Retrieves the editor.
   *
   * @retrun boolean
   */
  public function getEditor() {
    return $this->editor;
  }

  /**
   * Sets the editor.
   *
   * @param boolean $editor
   */
  public function setEditor($editor) {
    $this->editor = $editor;
  }

  /**
   * Retrieves the surname.
   *
   * @retrun character_varying(100)
   */
  public function getSurname() {
    return $this->surname;
  }

  /**
   * Sets the surname.
   *
   * @param character_varying(100) $surname
   */
  public function setSurname($surname) {
    $this->surname = $surname;
  }

  /**
   * Retrieves the givennames.
   *
   * @retrun character_varying(100)
   */
  public function getGivennames() {
    return $this->givennames;
  }

  /**
   * Sets the givennames.
   *
   * @param character_varying(100) $givennames
   */
  public function setGivennames($givennames) {
    $this->givennames = $givennames;
  }

  /**
   * Retrieves the suffix.
   *
   * @retrun character_varying(100)
   */
  public function getSuffix() {
    return $this->suffix;
  }

  /**
   * Sets the suffix.
   *
   * @param character_varying(100) $suffix
   */
  public function setSuffix($suffix) {
    $this->suffix = $suffix;
  }
}