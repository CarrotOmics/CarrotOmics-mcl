<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_ANALYSIS class.
 *
 */
class CHADO_ANALYSIS  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_ANALYSIS.
   */
  protected $member_arr     = NULL;
  protected $analysis_id    = NULL;
  protected $name           = NULL;
  protected $description    = NULL;
  protected $program        = NULL;
  protected $programversion = NULL;
  protected $algorithm      = NULL;
  protected $sourcename     = NULL;
  protected $sourceversion  = NULL;
  protected $sourceuri      = NULL;
  protected $timeexecuted   = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'analysis_id'    => 'serial',
    'name'           => 'character_varying(255)',
    'description'    => 'text',
    'program'        => 'character_varying(255)',
    'programversion' => 'character_varying(255)',
    'algorithm'      => 'character_varying(255)',
    'sourcename'     => 'character_varying(255)',
    'sourceversion'  => 'character_varying(255)',
    'sourceuri'      => 'text',
    'timeexecuted'   => 'timestamp_without_time_zone',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'analysis_id'    => TRUE,
    'name'           => FALSE,
    'description'    => FALSE,
    'program'        => TRUE,
    'programversion' => TRUE,
    'algorithm'      => FALSE,
    'sourcename'     => FALSE,
    'sourceversion'  => FALSE,
    'sourceuri'      => FALSE,
    'timeexecuted'   => TRUE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr     = $details;
    $this->analysis_id    = array_key_exists('analysis_id', $details)    ? $details['analysis_id']    : '';
    $this->name           = array_key_exists('name', $details)           ? $details['name']           : '';
    $this->description    = array_key_exists('description', $details)    ? $details['description']    : '';
    $this->program        = array_key_exists('program', $details)        ? $details['program']        : '';
    $this->programversion = array_key_exists('programversion', $details) ? $details['programversion'] : '';
    $this->algorithm      = array_key_exists('algorithm', $details)      ? $details['algorithm']      : '';
    $this->sourcename     = array_key_exists('sourcename', $details)     ? $details['sourcename']     : '';
    $this->sourceversion  = array_key_exists('sourceversion', $details)  ? $details['sourceversion']  : '';
    $this->sourceuri      = array_key_exists('sourceuri', $details)      ? $details['sourceuri']      : '';
    $this->timeexecuted   = array_key_exists('timeexecuted', $details)   ? $details['timeexecuted']   : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_ANALYSIS by key(s).
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
    $sql = "SELECT * FROM chado.analysis WHERE " . implode(" AND " , $where_arr);
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
    return analysis_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return NULL;
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'analysis_c1' => array('program','programversion','sourcename'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'analysis_id' => array(
        array('table' => 'analysis_organism', 'attr' => 'analysis_id'),
        array('table' => 'analysisfeature', 'attr' => 'analysis_id'),
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
        $fields['name']           = $this->name;
        $fields['description']    = $this->description;
        $fields['program']        = $this->program;
        $fields['programversion'] = $this->programversion;
        $fields['algorithm']      = $this->algorithm;
        $fields['sourcename']     = $this->sourcename;
        $fields['sourceversion']  = $this->sourceversion;
        $fields['sourceuri']      = $this->sourceuri;
        $fields['timeexecuted']   = (!empty($this->timeexecuted)) ? $this->timeexecuted : 'now()';


        // Inserts the record.
        $this->analysis_id = db_insert('chado.analysis')
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
      $fields['name']           = $this->name;
      $fields['description']    = $this->description;
      $fields['program']        = $this->program;
      $fields['programversion'] = $this->programversion;
      $fields['algorithm']      = $this->algorithm;
      $fields['sourcename']     = $this->sourcename;
      $fields['sourceversion']  = $this->sourceversion;
      $fields['sourceuri']      = $this->sourceuri;
      $fields['timeexecuted']   = (!empty($this->timeexecuted)) ? $this->timeexecuted : 'now()';


      // Updates the record.
      db_update('chado.analysis')
        ->fields($fields)
        ->condition('analysis_id', $this->analysis_id, '=')
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

      db_delete('chado.analysis')
        ->condition('analysis_id', $this->analysis_id, '=')
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
   * Retrieves the analysis_id.
   *
   * @retrun serial
   */
  public function getAnalysisID() {
    return $this->analysis_id;
  }

  /**
   * Sets the analysis_id.
   *
   * @param serial $analysis_id
   */
  public function setAnalysisID($analysis_id) {
    $this->analysis_id = $analysis_id;
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
   * Retrieves the description.
   *
   * @retrun text
   */
  public function getDescription() {
    return $this->description;
  }

  /**
   * Sets the description.
   *
   * @param text $description
   */
  public function setDescription($description) {
    $this->description = $description;
  }

  /**
   * Retrieves the program.
   *
   * @retrun character_varying(255)
   */
  public function getProgram() {
    return $this->program;
  }

  /**
   * Sets the program.
   *
   * @param character_varying(255) $program
   */
  public function setProgram($program) {
    $this->program = $program;
  }

  /**
   * Retrieves the programversion.
   *
   * @retrun character_varying(255)
   */
  public function getProgramversion() {
    return $this->programversion;
  }

  /**
   * Sets the programversion.
   *
   * @param character_varying(255) $programversion
   */
  public function setProgramversion($programversion) {
    $this->programversion = $programversion;
  }

  /**
   * Retrieves the algorithm.
   *
   * @retrun character_varying(255)
   */
  public function getAlgorithm() {
    return $this->algorithm;
  }

  /**
   * Sets the algorithm.
   *
   * @param character_varying(255) $algorithm
   */
  public function setAlgorithm($algorithm) {
    $this->algorithm = $algorithm;
  }

  /**
   * Retrieves the sourcename.
   *
   * @retrun character_varying(255)
   */
  public function getSourcename() {
    return $this->sourcename;
  }

  /**
   * Sets the sourcename.
   *
   * @param character_varying(255) $sourcename
   */
  public function setSourcename($sourcename) {
    $this->sourcename = $sourcename;
  }

  /**
   * Retrieves the sourceversion.
   *
   * @retrun character_varying(255)
   */
  public function getSourceversion() {
    return $this->sourceversion;
  }

  /**
   * Sets the sourceversion.
   *
   * @param character_varying(255) $sourceversion
   */
  public function setSourceversion($sourceversion) {
    $this->sourceversion = $sourceversion;
  }

  /**
   * Retrieves the sourceuri.
   *
   * @retrun text
   */
  public function getSourceuri() {
    return $this->sourceuri;
  }

  /**
   * Sets the sourceuri.
   *
   * @param text $sourceuri
   */
  public function setSourceuri($sourceuri) {
    $this->sourceuri = $sourceuri;
  }

  /**
   * Retrieves the timeexecuted.
   *
   * @retrun timestamp_without_time_zone
   */
  public function getTimeexecuted() {
    return $this->timeexecuted;
  }

  /**
   * Sets the timeexecuted.
   *
   * @param timestamp_without_time_zone $timeexecuted
   */
  public function setTimeexecuted($timeexecuted) {
    $this->timeexecuted = $timeexecuted;
  }
}