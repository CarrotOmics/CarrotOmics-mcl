<?php
/**
 * The declaration of MCL_TEMPLATE_MERGE_ORGANISM class.
 *
 */
class MCL_TEMPLATE_MERGE_ORGANISM extends MCL_TEMPLATE {

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
    $desc = 'The description for merge organism sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'genus_base'    => array('req' => TRUE,  'width' => 10, 'desc' => "Genus of the base organism."),
      'species_base'  => array('req' => TRUE,  'width' => 10, 'desc' => "species of tthe base organism."),
      'genus'         => array('req' => TRUE,  'width' => 10, 'desc' => "Genus of the organism to be removed."),
      'species'       => array('req' => TRUE,  'width' => 10, 'desc' => "species of the organism to be removed."),
      'delete'        => array('req' => FALSE, 'width' => 10, 'desc' => "'yes' or 'no' - 'yes' indicates that the base organism will be removed."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks base organism.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus_base'], $line['species_base']);

    // Checks organism.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);

    // Checks delete.
    $this->checkDeleteFlag($line['delete']);

    // Keeps organism_id for merge check.
    $organism_base  = MCL_CHADO_ORGANISM::getOrganism($line['genus_base'], $line['species_base']);
    $organism       = MCL_CHADO_ORGANISM::getOrganism($line['genus'], $line['species']);
    if ($organism_base && $organism) {
      $this->mergeCheck($organism_base->getOrganismID(), $organism->getOrganismID());
    }
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the base organism.
    $organism_base = MCL_CHADO_ORGANISM::getOrganism($line['genus_base'], $line['species_base']);

    // Getst the organism.
    $organism = MCL_CHADO_ORGANISM::getOrganism($line['genus'], $line['species']);

    // Merges the organisms if not the same.
    if ($organism->getOrganismID() != $organism_base->getOrganismID()) {
      $organism_base->mergeOrganism($this, $organism);
    }
    else {
      $this->updateMsg('W', "Both are the same organisms");
    }

    // Deletes the merged organism.
    if (strtolower($line['delete']) == 'yes') {
      $GLOBALS['delete_organisms'][$organism->getOrganismID()] = $organism;
    }
  }

  /**
   * @see MCL_TEMPLATE::preProcessDU()
   */
  public function preProcessDU() {
    $GLOBALS['delete_organisms'] = array();
    return TRUE;
  }

  /**
   * @see MCL_TEMPLATE::postProcessDU()
   */
  public function postProcessDU() {

    // Deletes the organisms.
    foreach ((array)$GLOBALS['delete_organisms'] as $organism_id => $organism) {
      $this->updateMsg('R', $organism->getRestoreObj());
      if (!$organism->delete()) {
        $this->updateMsg('E', "Failed to delete organism [ID = $organism_id]");
      }
    }
    return TRUE;
  }
}