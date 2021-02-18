<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_PHENOTYPE_COMPARISON class.
 *
 */
class CHADO_PHENOTYPE_COMPARISON  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_PHENOTYPE_COMPARISON.
   */
  protected $member_arr              = NULL;
  protected $phenotype_comparison_id = NULL;
  protected $genotype1_id            = NULL;
  protected $environment1_id         = NULL;
  protected $genotype2_id            = NULL;
  protected $environment2_id         = NULL;
  protected $phenotype1_id           = NULL;
  protected $phenotype2_id           = NULL;
  protected $pub_id                  = NULL;
  protected $organism_id             = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'phenotype_comparison_id' => 'serial',
    'genotype1_id'            => 'integer',
    'environment1_id'         => 'integer',
    'genotype2_id'            => 'integer',
    'environment2_id'         => 'integer',
    'phenotype1_id'           => 'integer',
    'phenotype2_id'           => 'integer',
    'pub_id'                  => 'integer',
    'organism_id'             => 'integer',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'phenotype_comparison_id' => TRUE,
    'genotype1_id'            => TRUE,
    'environment1_id'         => TRUE,
    'genotype2_id'            => TRUE,
    'environment2_id'         => TRUE,
    'phenotype1_id'           => TRUE,
    'phenotype2_id'           => FALSE,
    'pub_id'                  => TRUE,
    'organism_id'             => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr              = $details;
    $this->phenotype_comparison_id = array_key_exists('phenotype_comparison_id', $details) ? $details['phenotype_comparison_id'] : '';
    $this->genotype1_id            = array_key_exists('genotype1_id', $details)            ? $details['genotype1_id']            : '';
    $this->environment1_id         = array_key_exists('environment1_id', $details)         ? $details['environment1_id']         : '';
    $this->genotype2_id            = array_key_exists('genotype2_id', $details)            ? $details['genotype2_id']            : '';
    $this->environment2_id         = array_key_exists('environment2_id', $details)         ? $details['environment2_id']         : '';
    $this->phenotype1_id           = array_key_exists('phenotype1_id', $details)           ? $details['phenotype1_id']           : '';
    $this->phenotype2_id           = array_key_exists('phenotype2_id', $details)           ? $details['phenotype2_id']           : '';
    $this->pub_id                  = array_key_exists('pub_id', $details)                  ? $details['pub_id']                  : '';
    $this->organism_id             = array_key_exists('organism_id', $details)             ? $details['organism_id']             : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_PHENOTYPE_COMPARISON by key(s).
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
    $sql = "SELECT * FROM chado.phenotype_comparison WHERE " . implode(" AND " , $where_arr);
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
    return phenotype_comparison_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'organism_id' => array('fk_table' => 'organism', 'fk_attr' => 'organism_id'),
      'pub_id' => array('fk_table' => 'pub', 'fk_attr' => 'pub_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'phenotype_comparison_c1' => array('genotype1_id','environment1_id','genotype2_id','environment2_id','phenotype1_id','pub_id'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'phenotype_comparison_id' => array(
        array('table' => 'phenotype_comparison_cvterm', 'attr' => 'phenotype_comparison_id'),
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
        $fields['genotype1_id']    = $this->genotype1_id;
        $fields['environment1_id'] = $this->environment1_id;
        $fields['genotype2_id']    = $this->genotype2_id;
        $fields['environment2_id'] = $this->environment2_id;
        $fields['phenotype1_id']   = $this->phenotype1_id;
        $fields['pub_id']          = $this->pub_id;
        $fields['organism_id']     = $this->organism_id;
        if (is_numeric($this->phenotype2_id)) { $fields['phenotype2_id'] = $this->phenotype2_id; }

        // Inserts the record.
        $this->phenotype_comparison_id = db_insert('chado.phenotype_comparison')
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
      $fields['genotype1_id']    = $this->genotype1_id;
      $fields['environment1_id'] = $this->environment1_id;
      $fields['genotype2_id']    = $this->genotype2_id;
      $fields['environment2_id'] = $this->environment2_id;
      $fields['phenotype1_id']   = $this->phenotype1_id;
      $fields['pub_id']          = $this->pub_id;
      $fields['organism_id']     = $this->organism_id;
      if (is_numeric($this->phenotype2_id)) { $fields['phenotype2_id'] = $this->phenotype2_id; }

      // Updates the record.
      db_update('chado.phenotype_comparison')
        ->fields($fields)
        ->condition('phenotype_comparison_id', $this->phenotype_comparison_id, '=')
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

      db_delete('chado.phenotype_comparison')
        ->condition('phenotype_comparison_id', $this->phenotype_comparison_id, '=')
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
   * Retrieves the phenotype_comparison_id.
   *
   * @retrun serial
   */
  public function getPhenotypeComparisonID() {
    return $this->phenotype_comparison_id;
  }

  /**
   * Sets the phenotype_comparison_id.
   *
   * @param serial $phenotype_comparison_id
   */
  public function setPhenotypeComparisonID($phenotype_comparison_id) {
    $this->phenotype_comparison_id = $phenotype_comparison_id;
  }

  /**
   * Retrieves the genotype1_id.
   *
   * @retrun integer
   */
  public function getGenotype1ID() {
    return $this->genotype1_id;
  }

  /**
   * Sets the genotype1_id.
   *
   * @param integer $genotype1_id
   */
  public function setGenotype1ID($genotype1_id) {
    $this->genotype1_id = $genotype1_id;
  }

  /**
   * Retrieves the environment1_id.
   *
   * @retrun integer
   */
  public function getEnvironment1ID() {
    return $this->environment1_id;
  }

  /**
   * Sets the environment1_id.
   *
   * @param integer $environment1_id
   */
  public function setEnvironment1ID($environment1_id) {
    $this->environment1_id = $environment1_id;
  }

  /**
   * Retrieves the genotype2_id.
   *
   * @retrun integer
   */
  public function getGenotype2ID() {
    return $this->genotype2_id;
  }

  /**
   * Sets the genotype2_id.
   *
   * @param integer $genotype2_id
   */
  public function setGenotype2ID($genotype2_id) {
    $this->genotype2_id = $genotype2_id;
  }

  /**
   * Retrieves the environment2_id.
   *
   * @retrun integer
   */
  public function getEnvironment2ID() {
    return $this->environment2_id;
  }

  /**
   * Sets the environment2_id.
   *
   * @param integer $environment2_id
   */
  public function setEnvironment2ID($environment2_id) {
    $this->environment2_id = $environment2_id;
  }

  /**
   * Retrieves the phenotype1_id.
   *
   * @retrun integer
   */
  public function getPhenotype1ID() {
    return $this->phenotype1_id;
  }

  /**
   * Sets the phenotype1_id.
   *
   * @param integer $phenotype1_id
   */
  public function setPhenotype1ID($phenotype1_id) {
    $this->phenotype1_id = $phenotype1_id;
  }

  /**
   * Retrieves the phenotype2_id.
   *
   * @retrun integer
   */
  public function getPhenotype2ID() {
    return $this->phenotype2_id;
  }

  /**
   * Sets the phenotype2_id.
   *
   * @param integer $phenotype2_id
   */
  public function setPhenotype2ID($phenotype2_id) {
    $this->phenotype2_id = $phenotype2_id;
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