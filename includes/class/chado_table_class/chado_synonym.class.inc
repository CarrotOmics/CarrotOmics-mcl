<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_SYNONYM class.
 *
 */
class CHADO_SYNONYM  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_SYNONYM.
   */
  protected $member_arr   = NULL;
  protected $synonym_id   = NULL;
  protected $name         = NULL;
  protected $type_id      = NULL;
  protected $synonym_sgml = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'synonym_id'   => 'serial',
    'name'         => 'character_varying(255)',
    'type_id'      => 'integer',
    'synonym_sgml' => 'character_varying(255)',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'synonym_id'   => TRUE,
    'name'         => TRUE,
    'type_id'      => TRUE,
    'synonym_sgml' => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr   = $details;
    $this->synonym_id   = array_key_exists('synonym_id', $details)   ? $details['synonym_id']   : '';
    $this->name         = array_key_exists('name', $details)         ? $details['name']         : '';
    $this->type_id      = array_key_exists('type_id', $details)      ? $details['type_id']      : '';
    $this->synonym_sgml = array_key_exists('synonym_sgml', $details) ? $details['synonym_sgml'] : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_SYNONYM by key(s).
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
    $sql = "SELECT * FROM chado.synonym WHERE " . implode(" AND " , $where_arr);
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
    return synonym_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'type_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'synonym_c1' => array('name','type_id'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'synonym_id' => array(
        array('table' => 'feature_synonym', 'attr' => 'synonym_id'),
        array('table' => 'cell_line_synonym', 'attr' => 'synonym_id'),
        array('table' => 'library_synonym', 'attr' => 'synonym_id'),
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
        $fields['name']         = $this->name;
        $fields['type_id']      = $this->type_id;
        $fields['synonym_sgml'] = $this->synonym_sgml;


        // Inserts the record.
        $this->synonym_id = db_insert('chado.synonym')
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
      $fields['name']         = $this->name;
      $fields['type_id']      = $this->type_id;
      $fields['synonym_sgml'] = $this->synonym_sgml;


      // Updates the record.
      db_update('chado.synonym')
        ->fields($fields)
        ->condition('synonym_id', $this->synonym_id, '=')
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

      db_delete('chado.synonym')
        ->condition('synonym_id', $this->synonym_id, '=')
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
   * Retrieves the synonym_id.
   *
   * @retrun serial
   */
  public function getSynonymID() {
    return $this->synonym_id;
  }

  /**
   * Sets the synonym_id.
   *
   * @param serial $synonym_id
   */
  public function setSynonymID($synonym_id) {
    $this->synonym_id = $synonym_id;
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
   * Retrieves the synonym_sgml.
   *
   * @retrun character_varying(255)
   */
  public function getSynonymSgml() {
    return $this->synonym_sgml;
  }

  /**
   * Sets the synonym_sgml.
   *
   * @param character_varying(255) $synonym_sgml
   */
  public function setSynonymSgml($synonym_sgml) {
    $this->synonym_sgml = $synonym_sgml;
  }
}