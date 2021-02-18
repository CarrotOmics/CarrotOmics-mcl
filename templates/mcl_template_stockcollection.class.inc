<?php
/**
 * The declaration of MCL_TEMPLATE_STOCKCOLLECTION class.
 *
 */
class MCL_TEMPLATE_STOCKCOLLECTION extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'STOCK';
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
    $desc = 'The description for stockcollection sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'code'            => array('req' => TRUE,  'width' => 10, 'desc' => "Code of the stockcollection."),
      'name'            => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the stockcollection."),
      'description'     => array('req' => FALSE, 'width' => 10, 'desc' => "any description for the stockcollection."),
      'url'             => array('req' => FALSE, 'width' => 10, 'desc' => "URL of the stockcollection."),
      'urlprefix'       => array('req' => FALSE, 'width' => 10, 'desc' => "Prefix of URL."),
      'contact'         => array('req' => FALSE, 'width' => 10, 'desc' => "Contact for the stockcollection (Please provide the contact info of these researchers in 'Contact' sheet)."),
      '##property_name' => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns (##) : followed by name (cvterm name) of the property."),
    );

    // Adds labels.
    $headers['url']['label'] = 'URL';
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['description']  = -1;
    $cvterms['SITE_CV']['stock_center'] = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks contact.
    MCL_CHADO_CONTACT::checkContact($this, $line['contact']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the contact.
    $contact_id = NULL;
    if ($line['contact']) {
      $contact = MCL_CHADO_CONTACT::byName($line['contact']);
      $contact_id = $contact->getContactID();
    }

    // Adds a stockcollection.
    $stockcollection = MCL_CHADO_STOCKCOLLECTION::addStockcollection($this, $line['code'], $line['name'], $this->cvterms['SITE_CV']['stock_center'], $contact_id);
    if ($stockcollection) {

      // Adds properties.
      $stockcollection->addProp($this, 'SITE_CV', 'description', $line['description']);

      // Adds the DB.
      $db = MCL_CHADO_DB::addDB($this, $line['code'], $line['urlprefix'], $line['url'], 'STOCKCOLLECTION DB');
      if ($db) {

        // Adds the DB to the stockcollection.
        $stockcollection->addDB($this, $db->getDbID());
      }

      // Adds the user-specific properties.
      $this->addSpecialColumns('##', $stockcollection, $line);
    }
  }

  /**
   * @see MCL_TEMPLATE::postProcessECD()
   */
  public function postProcessECD() {

    // Gets the cv.
    $cv = MCL_CHADO_CV::getCV($cv_name, 'SITE_CV');
    if (!$cv) {
      $this->updateMsg('E', "cv does not exist.");
      return FALSE;
    }

    // Saves the user-specific properties.
    if (!$this->saveSpecialColumns('##', $cv->getCvID(), $this->getHeaders())) {
      $this->updateMsg('E', "Failed to save the special columns");
      return FALSE;
    }
    return TRUE;
  }
}