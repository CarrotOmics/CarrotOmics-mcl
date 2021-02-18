<?php
/**
 * The declaration of MCL_TEMPLATE_MERGE_CONTACT class.
 *
 */
class MCL_TEMPLATE_MERGE_CONTACT extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_UPLOAD::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'MERGE';
    parent::__construct($details);
  }

  /**
   * @see MCL_TEMPLATE::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
      return new self($parent->getMemberArr());
    }
  }

  /**
   * @see MCL_TEMPLATE::defineDescription()
   */
  public function defineDescription() {
    $desc = 'The description for merge contact sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'contact_base'  => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the base contact"),
      'contact'       => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the contact to be removed."),
      'delete'        => array('req' => FALSE, 'width' => 10, 'desc' => "'yes' or 'no' - 'yes' indicates that the base contact will be removed."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks base contact.
    MCL_CHADO_CONTACT::checkContact($this, $line['contact_base']);

    // Checks contact.
    MCL_CHADO_CONTACT::checkContact($this, $line['contact']);

    // Checks delete.
    $this->checkDeleteFlag($line['delete']);

    // Keeps contact_id for merge check.
    $contact_base = MCL_CHADO_CONTACT::byName($line['contact_base']);
    $contact      = MCL_CHADO_CONTACT::byName($line['contact']);
    if ($contact_base && $contact_base) {
      $this->mergeCheck($contact_base->getContactID(), $contact->getContactID());
    }
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the base contact.
    $contact_base = MCL_CHADO_CONTACT::byName($line['contact_base']);

    // Getst the contact.
    $contact = MCL_CHADO_CONTACT::byName($line['contact']);

    // Merges the contacts if not the same.
    if ($contact->getContactID() != $contact_base->getContactID()) {
      $contact_base->mergeContact($this, $contact);
    }
    else {
      $this->updateMsg('W', "Both are the same contacts");
    }

    // Deletes the merged contact.
    if (strtolower($line['delete']) == 'yes') {
      $GLOBALS['delete_contacts'][$contact->getContactID()] = $contact;
    }
  }

  /**
   * @see MCL_TEMPLATE::preProcessDU()
   */
  public function preProcessDU() {
    $GLOBALS['delete_contacts'] = array();
    return TRUE;
  }

  /**
   * @see MCL_TEMPLATE::postProcessDU()
   */
  public function postProcessDU() {

    // Deletes the contacts.
    foreach ((array)$GLOBALS['delete_contacts'] as $contact_id => $contact) {
      $this->updateMsg('R', $contact->getRestoreObj());
      if (!$contact->delete()) {
        $this->updateMsg('E', "Failed to delete contact [ID = $contact_id]");
      }
    }
    return TRUE;
  }
}