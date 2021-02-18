<?php
/**
 * The declaration of MCL_CHADO_CONTACT class.
 *
 */
class MCL_CHADO_CONTACT extends CHADO_CONTACT {

 /**
  *  Class data members.
  */
  /**
   * @see CHADO_CONTACT::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_CONTACT::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * Returns MCL_CHADO_CONTACT by ID.
   *
   * @param integer $contact_id
   *
   * @return MCL_CHADO_CONTACT
   */
  public static function byID($contact_id) {
    return MCL_CHADO_CONTACT::byKey(array('contact_id' => $contact_id));
  }

  /**
   * Returns the contact by name.
   *
   * @param string $name
   *
   * @return MCL_CHADO_CONTACT
   */
  public static function byName($name) {
    return MCL_CHADO_CONTACT::byKey(array('name' => trim($name)));
  }

  /**
   * @see CHADO_CONTACT::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * @see CHADO_CONTACT::delete()
   */
  public function delete() {
    return parent::delete();
  }

  /**
   * Checks the existence of contact. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $contact
   * @param string $separator
   * @param boolean $warning
   *
   * @return boolean
   */
  public static function checkContact(MCL_TEMPLATE $mcl_tmpl = NULL, $contact, $separator = '', $warning = FALSE) {
    $flag = TRUE;
    if ($contact) {

      // Gets the contacts.
      $names = preg_split(self::getSepRegex($separator), $contact, -1, PREG_SPLIT_NO_EMPTY);
      foreach ($names as $name) {
        $name = trim($name);
        $contact = MCL_CHADO_CONTACT::byName($name);
        if (!$contact) {
          if ($warning) {
            self::updateMsg($mcl_tmpl, 'W', "(name) = ($name) not found in contact");
          }
          else {
            self::updateMsg($mcl_tmpl, 'E', "(name) = ($name) not found in contact");
            $flag = FALSE;
          }
        }
      }
    }
    return $flag;
  }

  /**
   * Checks the existence of contact. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $contact
   * @param string $separator
   * @param boolean $warning
   *
   * @return boolean
   */
  public static function checkContactID(MCL_TEMPLATE $mcl_tmpl = NULL, $contact_id, $separator = '', $warning = FALSE) {
    $flag = TRUE;
    if ($contact_id) {

      // Gets the contacts.
      $ids = preg_split(self::getSepRegex($separator), $contact_id, -1, PREG_SPLIT_NO_EMPTY);
      foreach ($ids as $id) {
        $contact = MCL_CHADO_CONTACT::byID($id);
        if (!$contact) {
          if ($warning) {
            self::updateMsg($mcl_tmpl, 'W', "(ID) = ($id) not found in contact");
          }
          else {
            self::updateMsg($mcl_tmpl, 'E', "(ID) = ($id) not found in contact");
            $flag = FALSE;
          }
        }
      }
    }
    return $flag;
  }

  /**
   * Adds a contact.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $name
   *
   * @return MCL_CHADO_CONTACT
   */
  public static function addContact(MCL_TEMPLATE $mcl_tmpl = NULL, $name, $type_id, $description) {

    // Sets the arguments.
    $args = array(
      'name' => $name,
    );

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $contact = MCL_CHADO_CONTACT::byKey($args);
    if ($contact) {
      self::addMsg($mcl_tmpl, 'D', 'contact', $args);
    }
    else {

      // Adds a new contact.
      $args['type_id']      = $type_id;
      $args['description']  = $description;
      $contact = new MCL_CHADO_CONTACT($args);
      if ($contact->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'contact', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'contact', $args);
        return NULL;
      }
    }
    return $contact;
  }

  /**
   * Adds a property by type ID (cvterm ID).
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $type_id
   * @param string $value
   * @param string $separator
   *
   * @return boolean
   */
  public function addPropByID(MCL_TEMPLATE $mcl_tmpl = NULL, $type_id, $value, $separator = '') {
    if ($value != '') {
      return $this->addProperty($mcl_tmpl, 'contactprop', 'contact_id', $this->contact_id, $type_id, $value, $separator);
    }
    return TRUE;
  }

  /**
   * Adds a property.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $cv_name
   * @param string $cvterm_name
   * @param string $value
   * @param string $separator
   *
   * @return boolean
   */
  public function addProp(MCL_TEMPLATE $mcl_tmpl = NULL, $cv_name, $cvterm_name, $value, $separator = '') {
    if ($value != '') {
      $type_id = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name)->getCvtermID();
      return $this->addProperty($mcl_tmpl, 'contactprop', 'contact_id', $this->contact_id, $type_id, $value, $separator);
    }
    return TRUE;
  }

  /**
   * @see CHADO_TABLE::getMergeUniqueKeys()
   */
  public function getMergeUniqueKeys($chado_table) {

    // Sets unique keys for contactprop.
    if ($chado_table == 'chado.contactprop') {
      $ukey_alias = array(
        'field'   => 'type_id',
        'value'   => MCL_CHADO_CVTERM::getCvterm('SITE_CV', 'alias')->getCvtermID(),
        'ukeys'   => array('contact_id', 'type_id', 'value'),
        'update'  => array('rank' => array('type' => 'max', 'ukeys' => array('contact_id', 'type_id'))),
      );
      $ukey_keywords = array(
        'field'   => 'type_id',
        'value'   => MCL_CHADO_CVTERM::getCvterm('SITE_CV', 'keywords')->getCvtermID(),
        'ukeys'   => array('contact_id', 'type_id', 'value'),
        'update'  => array('rank' => array('type' => 'max', 'ukeys' => array('contact_id', 'type_id'))),
      );
      return array($ukey_alias, $ukey_keywords);
    }
    return array();
  }

  /**
   * Merges the contacts.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_CONTACT $contact
   * @param boolean $verbose
   */
  public function mergeContact(MCL_TEMPLATE $mcl_tmpl = NULL, MCL_CHADO_CONTACT $contact, $verbose = FALSE) {

    // Sets the information of the contacts.
    $params = array(
      'label'   => 'CONTACT',
      'id_base' => $this->contact_id,
      'id'      => $contact->getContactID(),
    );

    // Gets all reference tables.
    $ref_tables = MCL_CHADO_CONTACT::getRelTable();
    foreach ((array)$ref_tables['contact_id'] as $ref_table) {

      // Processes the merge.
      if (!$this->procMerge($mcl_tmpl, $params, $ref_table, $verbose)) {
        if ($mcl_tmpl) {
          $table_name = $ref_table['table'];
          $mcl_tmpl->updateMsg('E', "Error : failed to merge data for $table");
        }
        break;
      }
    }
  }
}