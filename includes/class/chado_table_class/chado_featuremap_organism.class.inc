<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_FEATUREMAP_ORGANISM class.
 *
 */
class CHADO_FEATUREMAP_ORGANISM  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_FEATUREMAP_ORGANISM.
   */
  protected $member_arr             = NULL;
  protected $featuremap_organism_id = NULL;
  protected $featuremap_id          = NULL;
  protected $organism_id            = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'featuremap_organism_id' => 'serial',
    'featuremap_id'          => 'integer',
    'organism_id'            => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'featuremap_organism_id' => TRUE,
    'featuremap_id'          => TRUE,
    'organism_id'            => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr             = $details;
    $this->featuremap_organism_id = array_key_exists('featuremap_organism_id', $details) ? $details['featuremap_organism_id'] : '';
    $this->featuremap_id          = array_key_exists('featuremap_id', $details)          ? $details['featuremap_id']          : '';
    $this->organism_id            = array_key_exists('organism_id', $details)            ? $details['organism_id']            : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_FEATUREMAP_ORGANISM by key(s).
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
    $sql = "SELECT * FROM chado.featuremap_organism WHERE " . implode(" AND " , $where_arr);
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
    return featuremap_organism_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'featuremap_id' => array('fk_table' => 'featuremap', 'fk_attr' => 'featuremap_id'),
      'organism_id' => array('fk_table' => 'organism', 'fk_attr' => 'organism_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'featuremap_organism_c1' => array('featuremap_id','organism_id'),
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
        $fields['featuremap_id'] = $this->featuremap_id;
        $fields['organism_id']   = $this->organism_id;


        // Inserts the record.
        $this->featuremap_organism_id = db_insert('chado.featuremap_organism')
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
      $fields['featuremap_id'] = $this->featuremap_id;
      $fields['organism_id']   = $this->organism_id;


      // Updates the record.
      db_update('chado.featuremap_organism')
        ->fields($fields)
        ->condition('featuremap_organism_id', $this->featuremap_organism_id, '=')
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

      db_delete('chado.featuremap_organism')
        ->condition('featuremap_organism_id', $this->featuremap_organism_id, '=')
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
   * Retrieves the featuremap_organism_id.
   *
   * @retrun serial
   */
  public function getFeaturemapOrganismID() {
    return $this->featuremap_organism_id;
  }

  /**
   * Sets the featuremap_organism_id.
   *
   * @param serial $featuremap_organism_id
   */
  public function setFeaturemapOrganismID($featuremap_organism_id) {
    $this->featuremap_organism_id = $featuremap_organism_id;
  }

  /**
   * Retrieves the featuremap_id.
   *
   * @retrun integer
   */
  public function getFeaturemapID() {
    return $this->featuremap_id;
  }

  /**
   * Sets the featuremap_id.
   *
   * @param integer $featuremap_id
   */
  public function setFeaturemapID($featuremap_id) {
    $this->featuremap_id = $featuremap_id;
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
}