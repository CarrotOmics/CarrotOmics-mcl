<?php
/**
 * The declaration of MCL_TEMPLATE_DELETE_ORGANISM class.
 *
 */
class MCL_TEMPLATE_DELETE_ORGANISM extends MCL_TEMPLATE {

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
    $desc = 'The description for delete organism sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'organism_id' => array('req' => FALSE,  'width' => 10, 'desc' => "ID of the organism to be deleted. If organism ID is provied, the rest of columns will be ignored"),
      'genus'       => array('req' => FALSE,  'width' => 10, 'desc' => "genus of the organism to be deleted."),
      'species'     => array('req' => FALSE,  'width' => 10, 'desc' => "species of the organism to be deleted."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks organism.
    if (array_key_exists('organism_id', $line)) {
      MCL_CHADO_ORGANISM::checkOrganismID($this, $line['organism_id']);
    }
    else if (array_key_exists('genus', $line) && array_key_exists('species', $line)) {
      MCL_CHADO_ORGANISM::checkOrganism($this, $this, $line['genus'], $line['species']);
    }
    else {
      $this->updateMsg('E', "The column 'organism_id' or 'genus' and 'species' are missing.");
    }
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the organism.
    $organism = NULL;
    if (array_key_exists('organism_id', $line)) {
      $organism = MCL_CHADO_ORGANISM::byID($line['organism_id']);
    }
    else {
      $organism = MCL_CHADO_ORGANISM::getOrganism($line['genus'], $line['species']);
    }

    // Deletes the organism.
    if ($organism) {
      $genus    = $organism->getGenus();
      $species  = $organism->getSpecies();
      if ($organism->delete()) {
        $this->updateMsg('L', "The organism has been deleted ($genus, $species)");
      }
      else {
        $this->updateMsg('E', "Failed to delete the organism ($genus, $species)");
      }
    }
  }
}