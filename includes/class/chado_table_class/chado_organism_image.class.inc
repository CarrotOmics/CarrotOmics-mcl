<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_ORGANISM_IMAGE class.
 *
 */
class CHADO_ORGANISM_IMAGE  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_ORGANISM_IMAGE.
   */
  protected $member_arr        = NULL;
  protected $organism_image_id = NULL;
  protected $organism_id       = NULL;
  protected $eimage_id         = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'organism_image_id' => 'serial',
    'organism_id'       => 'integer',
    'eimage_id'         => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'organism_image_id' => TRUE,
    'organism_id'       => TRUE,
    'eimage_id'         => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr        = $details;
    $this->organism_image_id = array_key_exists('organism_image_id', $details) ? $details['organism_image_id'] : '';
    $this->organism_id       = array_key_exists('organism_id', $details)       ? $details['organism_id']       : '';
    $this->eimage_id         = array_key_exists('eimage_id', $details)         ? $details['eimage_id']         : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_ORGANISM_IMAGE by key(s).
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
    $sql = "SELECT * FROM chado.organism_image WHERE " . implode(" AND " , $where_arr);
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
    return organism_image_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'eimage_id' => array('fk_table' => 'eimage', 'fk_attr' => 'eimage_id'),
      'organism_id' => array('fk_table' => 'organism', 'fk_attr' => 'organism_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'organism_image_organism_id_eimage_id_key' => array('organism_id','eimage_id'),
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
        $fields['organism_id'] = $this->organism_id;
        $fields['eimage_id']   = $this->eimage_id;


        // Inserts the record.
        $this->organism_image_id = db_insert('chado.organism_image')
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
      $fields['organism_id'] = $this->organism_id;
      $fields['eimage_id']   = $this->eimage_id;


      // Updates the record.
      db_update('chado.organism_image')
        ->fields($fields)
        ->condition('organism_image_id', $this->organism_image_id, '=')
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

      db_delete('chado.organism_image')
        ->condition('organism_image_id', $this->organism_image_id, '=')
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
   * Retrieves the organism_image_id.
   *
   * @retrun serial
   */
  public function getOrganismImageID() {
    return $this->organism_image_id;
  }

  /**
   * Sets the organism_image_id.
   *
   * @param serial $organism_image_id
   */
  public function setOrganismImageID($organism_image_id) {
    $this->organism_image_id = $organism_image_id;
  }

  /**
   * Retrieves the organism_id.
   *
   * @retrun integer
   */
  public function getOrganismID() {
    return $this->organism_id;
  }

  /**
   * Sets the organism_id.
   *
   * @param integer $organism_id
   */
  public function setOrganismID($organism_id) {
    $this->organism_id = $organism_id;
  }

  /**
   * Retrieves the eimage_id.
   *
   * @retrun integer
   */
  public function getEimageID() {
    return $this->eimage_id;
  }

  /**
   * Sets the eimage_id.
   *
   * @param integer $eimage_id
   */
  public function setEimageID($eimage_id) {
    $this->eimage_id = $eimage_id;
  }
}