<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_ORGANISM class.
 *
 */
class CHADO_ORGANISM  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_ORGANISM.
   */
  protected $member_arr         = NULL;
  protected $organism_id        = NULL;
  protected $abbreviation       = NULL;
  protected $genus              = NULL;
  protected $species            = NULL;
  protected $infraspecific_name = NULL;
  protected $type_id            = NULL;
  protected $common_name        = NULL;
  protected $comment            = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'organism_id'        => 'serial',
    'abbreviation'       => 'character_varying(255)',
    'genus'              => 'character_varying(255)',
    'species'            => 'character_varying(255)',
    'infraspecific_name' => 'character_varying(255)',
    'type_id'            => 'integer',
    'common_name'        => 'character_varying(255)',
    'comment'            => 'text',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'organism_id'        => TRUE,
    'abbreviation'       => FALSE,
    'genus'              => TRUE,
    'species'            => TRUE,
    'infraspecific_name' => FALSE,
    'type_id'            => FALSE,
    'common_name'        => FALSE,
    'comment'            => FALSE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr         = $details;
    $this->organism_id        = array_key_exists('organism_id', $details)  ? $details['organism_id']  : '';
    $this->abbreviation       = array_key_exists('abbreviation', $details) ? $details['abbreviation'] : '';
    $this->genus              = array_key_exists('genus', $details)        ? $details['genus']        : '';
    $this->species            = array_key_exists('species', $details)      ? $details['species']      : '';
    $this->infraspecific_name = array_key_exists('infraspecific_name', $details) ? $details['infraspecific_name'] : '';
    $this->type_id            = array_key_exists('type_id', $details)      ? $details['type_id']      : '';
    $this->common_name        = array_key_exists('common_name', $details)  ? $details['common_name']  : '';
    $this->comment            = array_key_exists('comment', $details)      ? $details['comment']      : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_ORGANISM by key(s).
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

      // infraspecific_name is optional so exclude from WHERE if not used
      if (($key != 'infraspecific_name') or ($value)) {
        // Adds LOWER() function to the characters.
        if (preg_match("/^(char|text)/", $data_type)) {
          $where_arr []= " LOWER($key) = LOWER(:$key) ";
        }
        else {
          $where_arr []= " $key = :$key ";
        }
        $args[":$key"] = $keys[$key];
      }
    }

    // Gets the table properties.
    $sql = "SELECT * FROM chado.organism WHERE " . implode(" AND " , $where_arr);
    $details = db_query($sql, $args)->fetch(PDO::FETCH_ASSOC);
    if ($details) {
      return new self($details);
    }
    mcl_print("WARNING: CHADO_ORGANISM::byKey sql query returning NULL from $sql", 2, 1.1); // CarrotOmics addition
    return NULL;
  }

  /**
   * Returns the primary key.
   */
  public static function getPK() {
    return organism_id;
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
      'organism_c1' => array('genus','species','type_id','infraspecific_name'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'organism_id' => array(
        array('table' => 'feature', 'attr' => 'organism_id'),
        array('table' => 'analysis_organism', 'attr' => 'organism_id'),
        array('table' => 'biomaterial', 'attr' => 'taxon_id'),
        array('table' => 'cell_line', 'attr' => 'organism_id'),
        array('table' => 'cvterm', 'attr' => 'type_id'),
        array('table' => 'featuremap_organism', 'attr' => 'organism_id'),
        array('table' => 'library', 'attr' => 'organism_id'),
        array('table' => 'organism_dbxref', 'attr' => 'organism_id'),
        array('table' => 'organism_image', 'attr' => 'organism_id'),
        array('table' => 'organism_pub', 'attr' => 'organism_id'),
        array('table' => 'organism_relationship', 'attr' => 'object_organism_id'),
        array('table' => 'organism_relationship', 'attr' => 'subject_organism_id'),
        array('table' => 'organismprop', 'attr' => 'organism_id'),
        array('table' => 'phenotype_comparison', 'attr' => 'organism_id'),
        array('table' => 'phylonode_organism', 'attr' => 'organism_id'),
        array('table' => 'stock', 'attr' => 'organism_id'),
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
        $fields['abbreviation']       = $this->abbreviation;
        $fields['genus']              = $this->genus;
        $fields['species']            = $this->species;
        $fields['infraspecific_name'] = $this->infraspecific_name;
        $fields['type_id']            = $this->type_id;
        $fields['common_name']        = $this->common_name;
        $fields['comment']            = $this->comment;


        // Inserts the record.
        $this->organism_id = db_insert('chado.organism')
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
      $fields['abbreviation']       = $this->abbreviation;
      $fields['genus']              = $this->genus;
      $fields['species']            = $this->species;
      $fields['infraspecific_name'] = $this->infraspecific_name;
      $fields['type_id']            = $this->type_id;
      $fields['common_name']        = $this->common_name;
      $fields['comment']            = $this->comment;


      // Updates the record.
      db_update('chado.organism')
        ->fields($fields)
        ->condition('organism_id', $this->organism_id, '=')
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

      db_delete('chado.organism')
        ->condition('organism_id', $this->organism_id, '=')
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
   * Retrieves the organism_id.
   *
   * @retrun serial
   */
  public function getOrganismID() {
    return $this->organism_id;
  }

  /**
   * Sets the organism_id.
   *
   * @param serial $organism_id
   */
  public function setOrganismID($organism_id) {
    $this->organism_id = $organism_id;
  }

  /**
   * Retrieves the abbreviation.
   *
   * @retrun character_varying(255)
   */
  public function getAbbreviation() {
    return $this->abbreviation;
  }

  /**
   * Sets the abbreviation.
   *
   * @param character_varying(255) $abbreviation
   */
  public function setAbbreviation($abbreviation) {
    $this->abbreviation = $abbreviation;
  }

  /**
   * Retrieves the genus.
   *
   * @retrun character_varying(255)
   */
  public function getGenus() {
    return $this->genus;
  }

  /**
   * Sets the genus.
   *
   * @param character_varying(255) $genus
   */
  public function setGenus($genus) {
    $this->genus = $genus;
  }

  /**
   * Retrieves the species.
   *
   * @retrun character_varying(255)
   */
  public function getSpecies() {
    return $this->species;
  }

  /**
   * Sets the species.
   *
   * @param character_varying(255) $species
   */
  public function setSpecies($species) {
    $this->species = $species;
  }

  /**
   * Retrieves the infraspecific_name.
   *
   * @retrun character_varying(255)
   */
  public function getInfraspecific_name() {
    return $this->Infraspecific_name;
  }

  /**
   * Sets the infraspecific_name.
   *
   * @param character_varying(255) $infraspecific_name
   */
  public function setInfraspecific_name($infraspecific_name) {
    $this->infraspecific_name = $infraspecific_name;
  }

  /**
   * Retrieves the type_id.
   *
   * @return integer
   */
  public function getType_id() {
    return $this->type_id;
  }

  /**
   * Sets the type_id.
   *
   * @param integer $type_id
   */
  public function setType_id($type_id) {
    $this->type_id = $type_id;
  }

  /**
   * Retrieves the common_name.
   *
   * @retrun character_varying(255)
   */
  public function getCommonName() {
    return $this->common_name;
  }

  /**
   * Sets the common_name.
   *
   * @param character_varying(255) $common_name
   */
  public function setCommonName($common_name) {
    $this->common_name = $common_name;
  }

  /**
   * Retrieves the comment.
   *
   * @retrun text
   */
  public function getComment() {
    return $this->comment;
  }

  /**
   * Sets the comment.
   *
   * @param text $comment
   */
  public function setComment($comment) {
    $this->comment = $comment;
  }
}
