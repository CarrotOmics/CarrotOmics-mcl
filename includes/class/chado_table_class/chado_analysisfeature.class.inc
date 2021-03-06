<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_ANALYSISFEATURE class.
 *
 */
class CHADO_ANALYSISFEATURE  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_ANALYSISFEATURE.
   */
  protected $member_arr         = NULL;
  protected $analysisfeature_id = NULL;
  protected $feature_id         = NULL;
  protected $analysis_id        = NULL;
  protected $rawscore           = NULL;
  protected $normscore          = NULL;
  protected $significance       = NULL;
  protected $identity           = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'analysisfeature_id' => 'serial',
    'feature_id'         => 'integer',
    'analysis_id'        => 'integer',
    'rawscore'           => 'double',
    'normscore'          => 'double',
    'significance'       => 'double',
    'identity'           => 'double',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'analysisfeature_id' => TRUE,
    'feature_id'         => TRUE,
    'analysis_id'        => TRUE,
    'rawscore'           => FALSE,
    'normscore'          => FALSE,
    'significance'       => FALSE,
    'identity'           => FALSE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr         = $details;
    $this->analysisfeature_id = array_key_exists('analysisfeature_id', $details) ? $details['analysisfeature_id'] : '';
    $this->feature_id         = array_key_exists('feature_id', $details)         ? $details['feature_id']         : '';
    $this->analysis_id        = array_key_exists('analysis_id', $details)        ? $details['analysis_id']        : '';
    $this->rawscore           = array_key_exists('rawscore', $details)           ? $details['rawscore']           : '';
    $this->normscore          = array_key_exists('normscore', $details)          ? $details['normscore']          : '';
    $this->significance       = array_key_exists('significance', $details)       ? $details['significance']       : '';
    $this->identity           = array_key_exists('identity', $details)           ? $details['identity']           : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_ANALYSISFEATURE by key(s).
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
    $sql = "SELECT * FROM chado.analysisfeature WHERE " . implode(" AND " , $where_arr);
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
    return analysisfeature_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'analysis_id' => array('fk_table' => 'analysis', 'fk_attr' => 'analysis_id'),
      'feature_id' => array('fk_table' => 'feature', 'fk_attr' => 'feature_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'analysisfeature_feature_id_analysis_id_key' => array('feature_id','analysis_id'),
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
        $fields['feature_id']  = $this->feature_id;
        $fields['analysis_id'] = $this->analysis_id;
        if (is_numeric($this->rawscore))     { $fields['rawscore']     = $this->rawscore; }
        if (is_numeric($this->normscore))    { $fields['normscore']    = $this->normscore; }
        if (is_numeric($this->significance)) { $fields['significance'] = $this->significance; }
        if (is_numeric($this->identity))     { $fields['identity']     = $this->identity; }

        // Inserts the record.
        $this->analysisfeature_id = db_insert('chado.analysisfeature')
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
      $fields['feature_id']  = $this->feature_id;
      $fields['analysis_id'] = $this->analysis_id;
      if (is_numeric($this->rawscore))     { $fields['rawscore']     = $this->rawscore; }
      if (is_numeric($this->normscore))    { $fields['normscore']    = $this->normscore; }
      if (is_numeric($this->significance)) { $fields['significance'] = $this->significance; }
      if (is_numeric($this->identity))     { $fields['identity']     = $this->identity; }

      // Updates the record.
      db_update('chado.analysisfeature')
        ->fields($fields)
        ->condition('analysisfeature_id', $this->analysisfeature_id, '=')
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

      db_delete('chado.analysisfeature')
        ->condition('analysisfeature_id', $this->analysisfeature_id, '=')
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
   * Retrieves the analysisfeature_id.
   *
   * @retrun serial
   */
  public function getAnalysisfeatureID() {
    return $this->analysisfeature_id;
  }

  /**
   * Sets the analysisfeature_id.
   *
   * @param serial $analysisfeature_id
   */
  public function setAnalysisfeatureID($analysisfeature_id) {
    $this->analysisfeature_id = $analysisfeature_id;
  }

  /**
   * Retrieves the feature_id.
   *
   * @retrun integer
   */
  public function getFeatureID() {
    return $this->feature_id;
  }

  /**
   * Sets the feature_id.
   *
   * @param integer $feature_id
   */
  public function setFeatureID($feature_id) {
    $this->feature_id = $feature_id;
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
   * Retrieves the rawscore.
   *
   * @retrun double
   */
  public function getRawscore() {
    return $this->rawscore;
  }

  /**
   * Sets the rawscore.
   *
   * @param double $rawscore
   */
  public function setRawscore($rawscore) {
    $this->rawscore = $rawscore;
  }

  /**
   * Retrieves the normscore.
   *
   * @retrun double
   */
  public function getNormscore() {
    return $this->normscore;
  }

  /**
   * Sets the normscore.
   *
   * @param double $normscore
   */
  public function setNormscore($normscore) {
    $this->normscore = $normscore;
  }

  /**
   * Retrieves the significance.
   *
   * @retrun double
   */
  public function getSignificance() {
    return $this->significance;
  }

  /**
   * Sets the significance.
   *
   * @param double $significance
   */
  public function setSignificance($significance) {
    $this->significance = $significance;
  }

  /**
   * Retrieves the identity.
   *
   * @retrun double
   */
  public function getIdentity() {
    return $this->identity;
  }

  /**
   * Sets the identity.
   *
   * @param double $identity
   */
  public function setIdentity($identity) {
    $this->identity = $identity;
  }
}