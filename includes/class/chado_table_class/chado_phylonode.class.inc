<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_PHYLONODE class.
 *
 */
class CHADO_PHYLONODE  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_PHYLONODE.
   */
  protected $member_arr          = NULL;
  protected $phylonode_id        = NULL;
  protected $phylotree_id        = NULL;
  protected $parent_phylonode_id = NULL;
  protected $left_idx            = NULL;
  protected $right_idx           = NULL;
  protected $type_id             = NULL;
  protected $feature_id          = NULL;
  protected $label               = NULL;
  protected $distance            = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'phylonode_id'        => 'serial',
    'phylotree_id'        => 'integer',
    'parent_phylonode_id' => 'integer',
    'left_idx'            => 'integer',
    'right_idx'           => 'integer',
    'type_id'             => 'integer',
    'feature_id'          => 'integer',
    'label'               => 'character_varying(255)',
    'distance'            => 'double',
  );

  /**
   *  'NOT NULL' for the data members.
   */
  protected static $not_nulls = array(
    'phylonode_id'        => TRUE,
    'phylotree_id'        => TRUE,
    'parent_phylonode_id' => FALSE,
    'left_idx'            => TRUE,
    'right_idx'           => TRUE,
    'type_id'             => FALSE,
    'feature_id'          => FALSE,
    'label'               => FALSE,
    'distance'            => FALSE,
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr          = $details;
    $this->phylonode_id        = array_key_exists('phylonode_id', $details)        ? $details['phylonode_id']        : '';
    $this->phylotree_id        = array_key_exists('phylotree_id', $details)        ? $details['phylotree_id']        : '';
    $this->parent_phylonode_id = array_key_exists('parent_phylonode_id', $details) ? $details['parent_phylonode_id'] : '';
    $this->left_idx            = array_key_exists('left_idx', $details)            ? $details['left_idx']            : '';
    $this->right_idx           = array_key_exists('right_idx', $details)           ? $details['right_idx']           : '';
    $this->type_id             = array_key_exists('type_id', $details)             ? $details['type_id']             : '';
    $this->feature_id          = array_key_exists('feature_id', $details)          ? $details['feature_id']          : '';
    $this->label               = array_key_exists('label', $details)               ? $details['label']               : '';
    $this->distance            = array_key_exists('distance', $details)            ? $details['distance']            : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_PHYLONODE by key(s).
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
    $sql = "SELECT * FROM chado.phylonode WHERE " . implode(" AND " , $where_arr);
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
    return phylonode_id;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      'feature_id' => array('fk_table' => 'feature', 'fk_attr' => 'feature_id'),
      'parent_phylonode_id' => array('fk_table' => 'phylonode', 'fk_attr' => 'phylonode_id'),
      'phylotree_id' => array('fk_table' => 'phylotree', 'fk_attr' => 'phylotree_id'),
      'type_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id'),
    );
  }

  /**
   * Returns the unique keys.
   */
  public static function getUK() {
    return array(
      'phylonode_phylotree_id_left_idx_key' => array('phylotree_id','left_idx'),
      'phylonode_phylotree_id_right_idx_key' => array('phylotree_id','right_idx'),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'phylonode_id' => array(
        array('table' => 'phylonode', 'attr' => 'parent_phylonode_id'),
        array('table' => 'phylonode_dbxref', 'attr' => 'phylonode_id'),
        array('table' => 'phylonode_organism', 'attr' => 'phylonode_id'),
        array('table' => 'phylonode_pub', 'attr' => 'phylonode_id'),
        array('table' => 'phylonode_relationship', 'attr' => 'object_id'),
        array('table' => 'phylonode_relationship', 'attr' => 'subject_id'),
        array('table' => 'phylonodeprop', 'attr' => 'phylonode_id'),
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
        $fields['phylotree_id'] = $this->phylotree_id;
        $fields['left_idx']     = $this->left_idx;
        $fields['right_idx']    = $this->right_idx;
        $fields['label']        = $this->label;
        if (is_numeric($this->parent_phylonode_id)) { $fields['parent_phylonode_id'] = $this->parent_phylonode_id; }
        if (is_numeric($this->type_id))             { $fields['type_id']             = $this->type_id; }
        if (is_numeric($this->feature_id))          { $fields['feature_id']          = $this->feature_id; }
        if (is_numeric($this->distance))            { $fields['distance']            = $this->distance; }

        // Inserts the record.
        $this->phylonode_id = db_insert('chado.phylonode')
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
      $fields['phylotree_id'] = $this->phylotree_id;
      $fields['left_idx']     = $this->left_idx;
      $fields['right_idx']    = $this->right_idx;
      $fields['label']        = $this->label;
      if (is_numeric($this->parent_phylonode_id)) { $fields['parent_phylonode_id'] = $this->parent_phylonode_id; }
      if (is_numeric($this->type_id))             { $fields['type_id']             = $this->type_id; }
      if (is_numeric($this->feature_id))          { $fields['feature_id']          = $this->feature_id; }
      if (is_numeric($this->distance))            { $fields['distance']            = $this->distance; }

      // Updates the record.
      db_update('chado.phylonode')
        ->fields($fields)
        ->condition('phylonode_id', $this->phylonode_id, '=')
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

      db_delete('chado.phylonode')
        ->condition('phylonode_id', $this->phylonode_id, '=')
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
   * Retrieves the phylonode_id.
   *
   * @retrun serial
   */
  public function getPhylonodeID() {
    return $this->phylonode_id;
  }

  /**
   * Sets the phylonode_id.
   *
   * @param serial $phylonode_id
   */
  public function setPhylonodeID($phylonode_id) {
    $this->phylonode_id = $phylonode_id;
  }

  /**
   * Retrieves the phylotree_id.
   *
   * @retrun integer
   */
  public function getPhylotreeID() {
    return $this->phylotree_id;
  }

  /**
   * Sets the phylotree_id.
   *
   * @param integer $phylotree_id
   */
  public function setPhylotreeID($phylotree_id) {
    $this->phylotree_id = $phylotree_id;
  }

  /**
   * Retrieves the parent_phylonode_id.
   *
   * @retrun integer
   */
  public function getParentPhylonodeID() {
    return $this->parent_phylonode_id;
  }

  /**
   * Sets the parent_phylonode_id.
   *
   * @param integer $parent_phylonode_id
   */
  public function setParentPhylonodeID($parent_phylonode_id) {
    $this->parent_phylonode_id = $parent_phylonode_id;
  }

  /**
   * Retrieves the left_idx.
   *
   * @retrun integer
   */
  public function getLeftIdx() {
    return $this->left_idx;
  }

  /**
   * Sets the left_idx.
   *
   * @param integer $left_idx
   */
  public function setLeftIdx($left_idx) {
    $this->left_idx = $left_idx;
  }

  /**
   * Retrieves the right_idx.
   *
   * @retrun integer
   */
  public function getRightIdx() {
    return $this->right_idx;
  }

  /**
   * Sets the right_idx.
   *
   * @param integer $right_idx
   */
  public function setRightIdx($right_idx) {
    $this->right_idx = $right_idx;
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
   * Retrieves the label.
   *
   * @retrun character_varying(255)
   */
  public function getLabel() {
    return $this->label;
  }

  /**
   * Sets the label.
   *
   * @param character_varying(255) $label
   */
  public function setLabel($label) {
    $this->label = $label;
  }

  /**
   * Retrieves the distance.
   *
   * @retrun double
   */
  public function getDistance() {
    return $this->distance;
  }

  /**
   * Sets the distance.
   *
   * @param double $distance
   */
  public function setDistance($distance) {
    $this->distance = $distance;
  }
}