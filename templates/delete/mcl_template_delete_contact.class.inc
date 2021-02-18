<?php
/**
 * The declaration of MCL_TEMPLATE_DELETE_CONTACT class.
 *
 */
class MCL_TEMPLATE_DELETE_CONTACT extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_UPLOAD::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'DELETE';
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
    $desc = 'The description for delete contact sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'contact_id'  => array('req' => FALSE,  'width' => 10, 'desc' => "ID of the contact to be deleted. If contact ID is provied, the rest of columns will be ignored"),
      'contact'     => array('req' => FALSE,  'width' => 10, 'desc' => "name of the contact to be deleted."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks contact.
    if (array_key_exists('contact_id', $line)) {
      MCL_CHADO_CONTACT::checkContactID($this, $line['contact_id'], TRUE);
    }
    else if (array_key_exists('contact', $line)) {
      MCL_CHADO_CONTACT::checkContact($this, $line['contact'], '', TRUE);
    }
    else {
      $this->updateMsg('E', "The column 'contact_id' or 'contact' is missing.");
    }
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the contact.
    $contact = NULL;
    if (array_key_exists('contact_id', $line)) {
      $contact = MCL_CHADO_CONTACT::byID($line['contact_id']);
    }
    else {
      $contact = MCL_CHADO_CONTACT::byName($line['contact']);
    }

    // Deletes the contact.
    if ($contact) {
      $contact_name = $contact->getName();
      if ($contact->delete()) {
        $this->updateMsg('L', "The contact has been deleted ($contact_name)");
      }
      else {
        $this->updateMsg('E', "Failed to delete the contact ($contact_name)");
      }
    }
  }
}