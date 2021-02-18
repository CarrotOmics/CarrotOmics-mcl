<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_PUB class.
 *
 */
class CHADO_PUB  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_PUB.
   */
  protected $member_arr  = NULL;
  protected $pub_id      = NULL;
  protected $title       = NULL;
  protected $volumetitle = NULL;
  protected $volume      = NULL;
  protected $series_name = NULL;
  protected $issue       = NULL;
  protected $pyear       = NULL;
  protected $pages       = NULL;
  protected $miniref     = NULL;
  protected $uniquename  = NULL;
  protected $type_id     = NULL;
  protected $is_obsolete = NULL;
  protected $publisher   = NULL;
  protected $pubplace    = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'pub_id'      => 'serial',
    'title'       => 'text',
    'volumetitle' => 'text',
    'volume'      => 'character_varying(255)',
    'series_name' => 'character_varying(255)',
    'issue'       => 'character_varying(255)',
    'pyear'       => 'character_varying(255)',
    'pages'       => 'character_varying(255)',
    'miniref'     => 'character_varying(255)',
    'uniquename'  => 'text',
    'type_id'     => 'integer',
    'is_obsolete' => 'boolean',
    'publisher'   => 'character_varying(255)',
    'pubplace'    => 'character_varying(255)',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'pub_id'      => TRUE,
    'title'       => FALSE,
    'volumetitle' => FALSE,
    'volume'      => FALSE,
    'series_name' => FALSE,
    'issue'       => FALSE,
    'pyear'       => FALSE,
    'pages'       => FALSE,
    'miniref'     => FALSE,
    'uniquename'  => TRUE,
    'type_id'     => TRUE,
    'is_obsolete' => FALSE,
    'publisher'   => FALSE,
    'pubplace'    => FALSE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr  = $details;
    $this->pub_id      = array_key_exists('pub_id', $details)      ? $details['pub_id']      : '';
    $this->title       = array_key_exists('title', $details)       ? $details['title']       : '';
    $this->volumetitle = array_key_exists('volumetitle', $details) ? $details['volumetitle'] : '';
    $this->volume      = array_key_exists('volume', $details)      ? $details['volume']      : '';
    $this->series_name = array_key_exists('series_name', $details) ? $details['series_name'] : '';
    $this->issue       = array_key_exists('issue', $details)       ? $details['issue']       : '';
    $this->pyear       = array_key_exists('pyear', $details)       ? $details['pyear']       : '';
    $this->pages       = array_key_exists('pages', $details)       ? $details['pages']       : '';
    $this->miniref     = array_key_exists('miniref', $details)     ? $details['miniref']     : '';
    $this->uniquename  = array_key_exists('uniquename', $details)  ? $details['uniquename']  : '';
    $this->type_id     = array_key_exists('type_id', $details)     ? $details['type_id']     : '';
    $this->is_obsolete = array_key_exists('is_obsolete', $details) ? $details['is_obsolete'] : '';
    $this->publisher   = array_key_exists('publisher', $details)   ? $details['publisher']   : '';
    $this->pubplace    = array_key_exists('pubplace', $details)    ? $details['pubplace']    : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_PUB by key(s).
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
    $sql = "SELECT * FROM chado.pub WHERE " . implode(" AND " , $where_arr);
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
    return pub_id;
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
      'pub_c1' => array('uniquename'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'pub_id' => array(
        array('table' => 'feature_cvterm', 'attr' => 'pub_id'),
        array('table' => 'feature_pub', 'attr' => 'pub_id'),
        array('table' => 'feature_synonym', 'attr' => 'pub_id'),
        array('table' => 'cell_line_cvterm', 'attr' => 'pub_id'),
        array('table' => 'cell_line_feature', 'attr' => 'pub_id'),
        array('table' => 'cell_line_library', 'attr' => 'pub_id'),
        array('table' => 'cell_line_pub', 'attr' => 'pub_id'),
        array('table' => 'cell_line_synonym', 'attr' => 'pub_id'),
        array('table' => 'cell_lineprop_pub', 'attr' => 'pub_id'),
        array('table' => 'expression_pub', 'attr' => 'pub_id'),
        array('table' => 'feature_cvterm_pub', 'attr' => 'pub_id'),
        array('table' => 'feature_expression', 'attr' => 'pub_id'),
        array('table' => 'feature_relationship_pub', 'attr' => 'pub_id'),
        array('table' => 'feature_relationshipprop_pub', 'attr' => 'pub_id'),
        array('table' => 'featureloc_pub', 'attr' => 'pub_id'),
        array('table' => 'featuremap_pub', 'attr' => 'pub_id'),
        array('table' => 'featureprop_pub', 'attr' => 'pub_id'),
        array('table' => 'library_cvterm', 'attr' => 'pub_id'),
        array('table' => 'library_pub', 'attr' => 'pub_id'),
        array('table' => 'library_synonym', 'attr' => 'pub_id'),
        array('table' => 'libraryprop_pub', 'attr' => 'pub_id'),
        array('table' => 'nd_experiment_pub', 'attr' => 'pub_id'),
        array('table' => 'organism_pub', 'attr' => 'pub_id'),
        array('table' => 'phendesc', 'attr' => 'pub_id'),
        array('table' => 'phenotype_comparison', 'attr' => 'pub_id'),
        array('table' => 'phenotype_comparison_cvterm', 'attr' => 'pub_id'),
        array('table' => 'phenstatement', 'attr' => 'pub_id'),
        array('table' => 'phylonode_pub', 'attr' => 'pub_id'),
        array('table' => 'phylotree_pub', 'attr' => 'pub_id'),
        array('table' => 'project_pub', 'attr' => 'pub_id'),
        array('table' => 'protocol', 'attr' => 'pub_id'),
        array('table' => 'pub_dbxref', 'attr' => 'pub_id'),
        array('table' => 'pub_relationship', 'attr' => 'object_id'),
        array('table' => 'pub_relationship', 'attr' => 'subject_id'),
        array('table' => 'pubauthor', 'attr' => 'pub_id'),
        array('table' => 'pubprop', 'attr' => 'pub_id'),
        array('table' => 'stock_cvterm', 'attr' => 'pub_id'),
        array('table' => 'stock_pub', 'attr' => 'pub_id'),
        array('table' => 'stock_relationship_cvterm', 'attr' => 'pub_id'),
        array('table' => 'stock_relationship_pub', 'attr' => 'pub_id'),
        array('table' => 'stockprop_pub', 'attr' => 'pub_id'),
        array('table' => 'study', 'attr' => 'pub_id'),
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
        $fields['title']       = $this->title;
        $fields['volumetitle'] = $this->volumetitle;
        $fields['volume']      = $this->volume;
        $fields['series_name'] = $this->series_name;
        $fields['issue']       = $this->issue;
        $fields['pyear']       = $this->pyear;
        $fields['pages']       = $this->pages;
        $fields['miniref']     = $this->miniref;
        $fields['uniquename']  = $this->uniquename;
        $fields['type_id']     = $this->type_id;
        $fields['publisher']   = $this->publisher;
        $fields['pubplace']    = $this->pubplace;
        if (preg_match("/^(true|false)/i", $this->is_obsolete)) { $fields['is_obsolete'] = '$this->is_obsolete'; }

        // Inserts the record.
        $this->pub_id = db_insert('chado.pub')
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
      $fields['title']       = $this->title;
      $fields['volumetitle'] = $this->volumetitle;
      $fields['volume']      = $this->volume;
      $fields['series_name'] = $this->series_name;
      $fields['issue']       = $this->issue;
      $fields['pyear']       = $this->pyear;
      $fields['pages']       = $this->pages;
      $fields['miniref']     = $this->miniref;
      $fields['uniquename']  = $this->uniquename;
      $fields['type_id']     = $this->type_id;
      $fields['publisher']   = $this->publisher;
      $fields['pubplace']    = $this->pubplace;
      if (preg_match("/^(true|false)/i", $this->is_obsolete)) { $fields['is_obsolete'] = '$this->is_obsolete'; }

      // Updates the record.
      db_update('chado.pub')
        ->fields($fields)
        ->condition('pub_id', $this->pub_id, '=')
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

      db_delete('chado.pub')
        ->condition('pub_id', $this->pub_id, '=')
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
   * Retrieves the pub_id.
   *
   * @retrun serial
   */
  public function getPubID() {
    return $this->pub_id;
  }

  /**
   * Sets the pub_id.
   *
   * @param serial $pub_id
   */
  public function setPubID($pub_id) {
    $this->pub_id = $pub_id;
  }

  /**
   * Retrieves the title.
   *
   * @retrun text
   */
  public function getTitle() {
    return $this->title;
  }

  /**
   * Sets the title.
   *
   * @param text $title
   */
  public function setTitle($title) {
    $this->title = $title;
  }

  /**
   * Retrieves the volumetitle.
   *
   * @retrun text
   */
  public function getVolumetitle() {
    return $this->volumetitle;
  }

  /**
   * Sets the volumetitle.
   *
   * @param text $volumetitle
   */
  public function setVolumetitle($volumetitle) {
    $this->volumetitle = $volumetitle;
  }

  /**
   * Retrieves the volume.
   *
   * @retrun character_varying(255)
   */
  public function getVolume() {
    return $this->volume;
  }

  /**
   * Sets the volume.
   *
   * @param character_varying(255) $volume
   */
  public function setVolume($volume) {
    $this->volume = $volume;
  }

  /**
   * Retrieves the series_name.
   *
   * @retrun character_varying(255)
   */
  public function getSeriesName() {
    return $this->series_name;
  }

  /**
   * Sets the series_name.
   *
   * @param character_varying(255) $series_name
   */
  public function setSeriesName($series_name) {
    $this->series_name = $series_name;
  }

  /**
   * Retrieves the issue.
   *
   * @retrun character_varying(255)
   */
  public function getIssue() {
    return $this->issue;
  }

  /**
   * Sets the issue.
   *
   * @param character_varying(255) $issue
   */
  public function setIssue($issue) {
    $this->issue = $issue;
  }

  /**
   * Retrieves the pyear.
   *
   * @retrun character_varying(255)
   */
  public function getPyear() {
    return $this->pyear;
  }

  /**
   * Sets the pyear.
   *
   * @param character_varying(255) $pyear
   */
  public function setPyear($pyear) {
    $this->pyear = $pyear;
  }

  /**
   * Retrieves the pages.
   *
   * @retrun character_varying(255)
   */
  public function getPages() {
    return $this->pages;
  }

  /**
   * Sets the pages.
   *
   * @param character_varying(255) $pages
   */
  public function setPages($pages) {
    $this->pages = $pages;
  }

  /**
   * Retrieves the miniref.
   *
   * @retrun character_varying(255)
   */
  public function getMiniref() {
    return $this->miniref;
  }

  /**
   * Sets the miniref.
   *
   * @param character_varying(255) $miniref
   */
  public function setMiniref($miniref) {
    $this->miniref = $miniref;
  }

  /**
   * Retrieves the uniquename.
   *
   * @retrun text
   */
  public function getUniquename() {
    return $this->uniquename;
  }

  /**
   * Sets the uniquename.
   *
   * @param text $uniquename
   */
  public function setUniquename($uniquename) {
    $this->uniquename = $uniquename;
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
   * Retrieves the is_obsolete.
   *
   * @retrun boolean
   */
  public function getIsObsolete() {
    return $this->is_obsolete;
  }

  /**
   * Sets the is_obsolete.
   *
   * @param boolean $is_obsolete
   */
  public function setIsObsolete($is_obsolete) {
    $this->is_obsolete = $is_obsolete;
  }

  /**
   * Retrieves the publisher.
   *
   * @retrun character_varying(255)
   */
  public function getPublisher() {
    return $this->publisher;
  }

  /**
   * Sets the publisher.
   *
   * @param character_varying(255) $publisher
   */
  public function setPublisher($publisher) {
    $this->publisher = $publisher;
  }

  /**
   * Retrieves the pubplace.
   *
   * @retrun character_varying(255)
   */
  public function getPubplace() {
    return $this->pubplace;
  }

  /**
   * Sets the pubplace.
   *
   * @param character_varying(255) $pubplace
   */
  public function setPubplace($pubplace) {
    $this->pubplace = $pubplace;
  }
}