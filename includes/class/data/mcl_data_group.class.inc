<?php
/**
 * The declaration of MCL_DATA_GROUP class.
 *
 */
class MCL_DATA_GROUP extends PUBLIC_MCL_DATA_GROUP {

 /**
  *  Class data members.
  */
  protected $prop_arr = NULL;

  /**
   * @see MCL_DATA_GROUP::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see MCL_DATA_GROUP::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * Returns MCL_DATA_GROUP by ID.
   *
   * @param integer $data_group_id
   *
   * @return MCL_DATA_GROUP
   */
  public static function byID($data_group_id) {
    return MCL_DATA_GROUP::byKey(array('data_group_id' => $data_group_id));
  }

  /**
   * Returns MCL_DATA_GROUP by name.
   *
   * @param integer $name
   *
   * @return MCL_DATA_GROUP
   */
  public static function byName($name) {
    return MCL_DATA_GROUP::byKey(array('name' => $name));
  }

  /**
   * @see MCL_DATA_GROUP::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * @see MCL_DATA_GROUP::insert()
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
      ->condition('data_group_id', $this->data_group_id)
      ->execute();
    parent::delete();
  }

  /**
   * Adds a data group.
   *
   * @param array $details
   *
   * @return MCL_DATA_GROUP|NULL
   */
  public static function addDataGroup($details) {

    // Checks for a duplication.
    $data_group = self::byName($details['name']);
    if (!$data_group) {
      $data_group = new MCL_DATA_GROUP($details);
      if (!$data_group->insert()) {
        return NULL;
      }
    }
    return $data_group;
  }

  /**
   * Adds a data group member.
   *
   * @param string $type
   * @param string $key
   * @param string $value
   *
   * @return boolean
   */
  public function addGroupMember($type, $key, $value) {

    // Checks for a duplication.
    $details = array(
      'data_group_id' => $this->data_group_id,
      'type'          => $type,
      'key'           => $key,
    );
    $group_member = PUBLIC_MCL_DATA_GROUP_MEMBER::byKey($details);
    if (!$group_member) {
      $details['value'] = $value;

      // Adds a member.
      $group_member = new PUBLIC_MCL_DATA_GROUP_MEMBER($details);
      return $group_member->insert();
    }
    return TRUE;
  }

  /**
   * Returns the members of the group.
   *
   * @param integer $flag
   *
   * @return various
   */
  public function getGroupMembers($flag = MCL_OBJECT) {

    // Gets the members of the group.
    $sql = "
      SELECT GM.*
      FROM {mcl_data_group_member} GM
      WHERE data_group_id = :data_group_id
      ORDER BY GM.value
    ";
    $result = db_query($sql, array('data_group_id' => $this->data_group_id));
    $members = array();
    while ($obj = $result->fetchObject()) {
      if ($flag == MCL_CLASS) {
        $members []= PUBLIC_MCL_DATA_GROUP_MEMBER::byKey(array('data_group_member_id' => $obj->data_group_member_id));
      }
      else if ($flag == MCL_OPTION) {
        $members[$obj->key] = $obj->value;
      }
      else if ($flag == MCL_VALUE) {
        $members []= $obj->value;
      }
      else {
        $members []= $obj;
      }
    }
    return $members;
  }

  /**
   * Returns all data groups.
   *
   * @param integer $flag
   *
   * @return various
   */
  public static function getGroups($flag = MCL_OBJECT) {

    // Gets the all data groups.
    $sql = "SELECT G.* FROM {mcl_data_group} G ORDER BY G.name";
    $result = db_query($sql);
    $groups = array();
    while ($obj = $result->fetchObject()) {
      if ($flag == MCL_CLASS) {
        $members []= MCL_DATA_GROUP::byID($obj->data_group_id);
      }
      else if ($flag == MCL_OPTION) {
        $members[$obj->data_group_id] = $obj->name;
      }
      else {
        $members []= $obj;
      }
    }
    return $members;
  }
}