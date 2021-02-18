<?php
/**
 * The declaration of MCL_DATA_VALID_TYPE class.
 *
 */
class MCL_DATA_VALID_TYPE extends PUBLIC_MCL_DATA_VALID_TYPE {

 /**
  *  Class data members.
  */
  protected $prop_arr = NULL;

  /**
   * @see MCL_DATA_VALID_TYPE::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see MCL_DATA_VALID_TYPE::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * Returns MCL_DATA_VALID_TYPE by ID.
   *
   * @param integer $data_valid_type_id
   *
   * @return MCL_DATA_VALID_TYPE
   */
  public static function byID($data_valid_type_id) {
    return MCL_DATA_VALID_TYPE::byKey(array('data_valid_type_id' => $data_valid_type_id));
  }

  /**
   * Returns MCL_DATA_VALID_TYPE by type.
   *
   * @param string $type
   *
   * @return MCL_DATA_VALID_TYPE
   */
  public static function byType($type) {
    return MCL_DATA_VALID_TYPE::byKey(array('type' => $type));
  }

  /**
   * @see MCL_DATA_VALID_TYPE::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * @see MCL_DATA_VALID_TYPE::insert()
   */
  public function insert() {

    // Insert a new file.
    return parent::insert();
  }

  /**
   * @see PUBLIC_MCL_FILE::update()
   */
  public function update() {

    // Updates the user properties.
    return parent::update();
  }

  /**
   * @see PUBLIC_MCL_FILE::delete()
   */
  public function delete() {

    // Deletes all records in mcl_data_valid fist.
    db_delete('mcl_data_valid')
      ->condition('data_valid_type_id', $this->data_valid_type_id)
      ->execute();
    parent::delete();
  }

  /**
   * Returns if type is a cvterm.
   */
  public function isCvterm() {
    return ($this->cv_id) ? TRUE : FALSE;
  }

  /**
   * Adds a data valid type and its values.
   *
   * @param array $params
   *
   * @return srting
   */
  public static function addType($params) {

    // Gets the parameters.
    $data_valid_type  = $params['data_valid_type'];
    $template_id      = $params['template_id'];
    $cv_name          = $params['cv_name'];
    $description      = $params['description'];
    $values           = $params['values'];

    // Adds a data_valid_type.
    $keys = array(
      'type'        => $data_valid_type,
      'template_id' => $template_id,
    );
    $mcl_dvt = MCL_DATA_VALID_TYPE::byKey($keys);
    if (!$mcl_dvt) {
      $details = array(
        'type'        => $data_valid_type,
        'template_id' => $template_id,
        'description' => $description,
      );
      if ($cv_name) {
        $cv = MCL_CHADO_CV::getCV($cv_name);
        if (!$cv) {
          return "CV ($cv_name) does not exist. Please add '$cv_name'.";
        }
        $details['cv_id'] = $cv->getCvID();
      }
      $mcl_dvt = new MCL_DATA_VALID_TYPE($details);
      if (!$mcl_dvt->insert()) {
        return "Failed to add a data valid type ($data_valid_type).";
      }
    }

    // Adds the values.
    foreach ((array)$values as $value) {

      // Sets the properties for a data valid.
      $details = array(
        'data_valid_type_id'  => $mcl_dvt->getDataValidTypeID(),
        'name'                => $value,
      );

      // If this is cvterm, adds the cvterm ID.
      if ($cv_name) {
        $cvterm = MCL_CHADO_CVTERM::getCvterm($cv_name, $value);
        if (!$cvterm) {
          return "cvterm ($cv_name, $value) does not exist.";
        }
        $details['cvterm_id'] = $cvterm->getCvtermID();
      }

      // Adds a data valid.
      $p_mdv = PUBLIC_MCL_DATA_VALID::byKey($details);
      if (!$p_mdv) {
        $p_mdv = new PUBLIC_MCL_DATA_VALID($details);
        if (!$p_mdv->insert()) {
          return "Failed to add a data valid ($value).";
        }
      }
    }
    return '';
  }

  /**
   * Returns the options for the data valid types.
   *
   * @param string $type
   *
   * @return array
   */
  public static function getOptions($type) {

    // Gets MCL_DATA_VALID_TYPE.
    $mcl_data_valid_type  = MCL_DATA_VALID_TYPE::byKey(array('type' => $type));
    $data_valid_type_id   = $mcl_data_valid_type->getDataValidTypeID();
    $cv_id                = $mcl_data_valid_type->getCvID();

    // Gets all cvterm ID or values.
    $sql = "
      SELECT name, cvterm_id
      FROM {mcl_data_valid}
      WHERE data_valid_type_id = :data_valid_type_id
      ORDER BY name
    ";
    $result = db_query($sql, array('data_valid_type_id' => $data_valid_type_id));
    $options = array();
    while ($obj = $result->fetchObject()) {
      if ($cv_id) {
        $options[$obj->cvterm_id] = $obj->name;
      }
      else {
        $options[$obj->name] = $obj->name;
      }
    }
    return $options;
  }

  /**
   * Returns all data valid types.
   *
   * @return array of MCL_DATA_VALID_TYPE objects.
   */
  public static function getDataValidTypes() {

    $sql = "SELECT DVT.data_valid_type_id FROM {mcl_data_valid_type} DVT ORDER BY DVT.type";
    $result = db_query($sql);
    $data_valid_types = array();
    while ($data_valid_type_id = $result->fetchField()) {
      $data_valid_types[] = MCL_DATA_VALID_TYPE::byKey(array('data_valid_type_id' => $data_valid_type_id));
    }
    return $data_valid_types;
  }

  /**
   * Returns all data valid values of this type.
   *
   * @return array of objects.
   */
  public function getDataValidValues() {
    $objects = array();

    // Gets all values.
    $sql = "
      SELECT DV.*
      FROM {mcl_data_valid} DV
      WHERE DV.data_valid_type_id = :data_valid_type_id
      ORDER BY DV.name
    ";
    $args = array(
      'data_valid_type_id' => $this->data_valid_type_id,
    );
    $result = db_query($sql, $args);
    while ($obj = $result->fetchObject()) {
      $objects []= $obj;
    }
    return $objects;
  }

  /**
   * Validates the value for the provided data valid type.
   *
   * @param string $data_valid_type
   * @param string $value
   *
   * @return boolean
   */
  public static function validate($data_valid_type, $value) {
    if ($value != '') {

      // Gets data_valid_type_id.
      $mcl_data_valid_type  = MCL_DATA_VALID_TYPE::byKey(array('type' => $data_valid_type));
      if (!$mcl_data_valid_type) {
        return FALSE;
      }
      else {
        $data_valid_type_id = $mcl_data_valid_type->getDataValidTypeID();

        // Checks the value.
        $sql = "
          SELECT COUNT(data_valid_id)
          FROM {mcl_data_valid}
          WHERE data_valid_type_id = :data_valid_type_id AND LOWER(name) = LOWER(:name)
        ";
        $args = array(
          ':data_valid_type_id' => $data_valid_type_id,
          ':name' => $value,
        );
        $num = db_query($sql, $args)->fetchField();
        return ($num) ? TRUE : FALSE;
      }
    }
    return TRUE;
  }
}