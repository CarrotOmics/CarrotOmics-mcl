<?php
/**
 * The declaration of MCL_TEMPLATE_DELETE_PUB class.
 *
 */
class MCL_TEMPLATE_DELETE_PUB extends MCL_TEMPLATE {

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
    $desc = 'The description for merge pub sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'pub_id' => array('req' => TRUE,  'width' => 10, 'desc' => "pub ID of the publication to be deleted."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks publication.
    MCL_CHADO_PUB::checkPubID($this, $line['pub_id'], '', TRUE);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the publication.
    $pub_id = $line['pub_id'];
    $pub = MCL_CHADO_PUB::byID($pub_id);

    // Deletes the publication.
    if ($pub) {
      if ($pub->delete()) {
        $this->updateMsg('L', "The publication has been deleted ($pub_id)");
      }
      else {
        $this->updateMsg('E', "Failed to delete the publication ($pub_id)");
      }
    }
  }
}