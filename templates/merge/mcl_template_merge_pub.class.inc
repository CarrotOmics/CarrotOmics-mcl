<?php
/**
 * The declaration of MCL_TEMPLATE_MERGE_PUB class.
 *
 */
class MCL_TEMPLATE_MERGE_PUB extends MCL_TEMPLATE {

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
    $desc = 'The description for merge pub sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'pub_id_base' => array('req' => TRUE,  'width' => 10, 'desc' => "pub ID of the base publication"),
      'pub_id'      => array('req' => TRUE,  'width' => 10, 'desc' => "pub ID of the publication to be removed."),
      'delete'      => array('req' => FALSE, 'width' => 10, 'desc' => "'yes' or 'no' - 'yes' indicates that the base pub will be removed."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks base publication.
    MCL_CHADO_PUB::checkPubID($this, $line['pub_id_base']);

    // Checks publication.
    MCL_CHADO_PUB::checkPubID($this, $line['pub_id']);

    // Checks delete.
    $this->checkDeleteFlag($line['delete']);

    // Keeps pub_id for merge check.
    $this->mergeCheck($line['pub_id_base'], $line['pub_id']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the base publication.
    $pub_base = MCL_CHADO_PUB::byID($line['pub_id_base']);

    // Gets the publication.
    $pub = MCL_CHADO_PUB::byID($line['pub_id']);

    // Merges the publications if not the same.
    if ($pub->getPubID() != $pub_base->getPubID()) {
      $pub_base->mergePub($this, $pub);
    }
    else {
      $this->updateMsg('W', "Both are the same publications");
    }

    // Deletes the merged publication.
    if (strtolower($line['delete']) == 'yes') {
      $GLOBALS['delete_pubs'][$pub->getPubID()] = $pub;
    }
  }

  /**
   * @see MCL_TEMPLATE::preProcessDU()
   */
  public function preProcessDU() {
    $GLOBALS['delete_pubs'] = array();
    return TRUE;
  }

  /**
   * @see MCL_TEMPLATE::postProcessDU()
   */
  public function postProcessDU() {

    // Deletes the publications.
    foreach ((array)$GLOBALS['delete_pubs'] as $pub_id => $pub) {
      $this->updateMsg('R', $pub->getRestoreObj());
      if (!$pub->delete()) {
        $this->updateMsg('E', "Failed to delete publication [ID = $pub_id]");
      }
    }
    return TRUE;
  }
}