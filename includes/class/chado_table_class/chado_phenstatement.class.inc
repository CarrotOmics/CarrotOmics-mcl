<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_PHENSTATEMENT class.
 *
 */
class CHADO_PHENSTATEMENT  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_PHENSTATEMENT.
   */
  protected $member_arr       = NULL;
  protected $phenstatement_id = NULL;
  protected $genotype_id      = NULL;
  protected $environment_id   = NULL;
  protected $phenotype_id     = NULL;
  protected $type_id          = NULL;
  protected $pub_id           = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'phenstatement_id' => 'serial',
    'genotype_id'      => 'integer',
    'environment_id'   => 'integer',
    'phenotype_id'     => 'integer',
    'type_id'          => 'integer',
    'pub_id'           => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'phenstatement_id' => TRUE,
    'genotype_id'      => TRUE,
    'environment_id'   => TRUE,
    'phenotype_id'     => TRUE,
    'type_id'          => TRUE,
    'pub_id'           => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr       = $details;
    $this->phenstatement_id = array_key_exists('phenstatement_id', $details) ? $details['phenstatement_id'] : '';
    $this->genotype_id      = array_key_exists('genotype_id', $details)      ? $details['genotype_id']      : '';
    $this->environment_id   = array_key_exists('environment_id', $details)   ? $details['environment_id']   : '';
    $this->phenotype_id     = array_key_exists('phenotype_id', $details)     ? $details['phenotype_id']     : '';
    $this->type_id          = array_key_exists('type_id', $details)          ? $details['type_id']          : '';
    $this->pub_id           = array_key_exists('pub_id', $details)           ? $details['pub_id']           : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_PHENSTATEMENT by key(s).
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
    $sql = "SELECT * FROM chado.phenstatement WHERE " . implode(" AND " , $where_arr);
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
    return phenstatement_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'environment_id' => array('fk_table' => 'environment', 'fk_attr' => 'environment_id'),
      'genotype_id' => array('fk_table' => 'genotype', 'fk_attr' => 'genotype_id'),
      'phenotype_id' => array('fk_table' => 'phenotype', 'fk_attr' => 'phenotype_id'),
      'pub_id' => array('fk_table' => 'pub', 'fk_attr' => 'pub_id'),
      'type_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'phenstatement_c1' => array('genotype_id','phenotype_id','environment_id','type_id','pub_id'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'phenstatement_id' => array(
        array('table' => 'phenstatementprop', 'attr' => 'phenstatement_id'),
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
        $fields['genotype_id']    = $this->genotype_id;
        $fields['environment_id'] = $this->environment_id;
        $fields['phenotype_id']   = $this->phenotype_id;
        $fields['type_id']        = $this->type_id;
        $fields['pub_id']         = $this->pub_id;


        // Inserts the record.
        $this->phenstatement_id = db_insert('chado.phenstatement')
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
      $fields['genotype_id']    = $this->genotype_id;
      $fields['environment_id'] = $this->environment_id;
      $fields['phenotype_id']   = $this->phenotype_id;
      $fields['type_id']        = $this->type_id;
      $fields['pub_id']         = $this->pub_id;


      // Updates the record.
      db_update('chado.phenstatement')
        ->fields($fields)
        ->condition('phenstatement_id', $this->phenstatement_id, '=')
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

      db_delete('chado.phenstatement')
        ->condition('phenstatement_id', $this->phenstatement_id, '=')
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
   * Retrieves the phenstatement_id.
   *
   * @retrun serial
   */
  public function getPhenstatementID() {
    return $this->phenstatement_id;
  }

  /**
   * Sets the phenstatement_id.
   *
   * @param serial $phenstatement_id
   */
  public function setPhenstatementID($phenstatement_id) {
    $this->phenstatement_id = $phenstatement_id;
  }

  /**
   * Retrieves the genotype_id.
   *
   * @retrun integer
   */
  public function getGenotypeID() {
    return $this->genotype_id;
  }

  /**
   * Sets the genotype_id.
   *
   * @param integer $genotype_id
   */
  public function setGenotypeID($genotype_id) {
    $this->genotype_id = $genotype_id;
  }

  /**
   * Retrieves the environment_id.
   *
   * @retrun integer
   */
  public function getEnvironmentID() {
    return $this->environment_id;
  }

  /**
   * Sets the environment_id.
   *
   * @param integer $environment_id
   */
  public function setEnvironmentID($environment_id) {
    $this->environment_id = $environment_id;
  }

  /**
   * Retrieves the phenotype_id.
   *
   * @retrun integer
   */
  public function getPhenotypeID() {
    return $this->phenotype_id;
  }

  /**
   * Sets the phenotype_id.
   *
   * @param integer $phenotype_id
   */
  public function setPhenotypeID($phenotype_id) {
    $this->phenotype_id = $phenotype_id;
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
}