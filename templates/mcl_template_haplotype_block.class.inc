<?php
/**
 * The declaration of MCL_TEMPLATE_HAPLOTYPE_BLOCK class.
 *
 */
class MCL_TEMPLATE_HAPLOTYPE_BLOCK extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'HAPLOTYPE_BLOCK';
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
      'haplotype_block'     => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the haplotype block"),
      'genus'               => array('req' => FALSE, 'width' => 10, 'desc' => "Genus of the haplotype block"),
      'species'             => array('req' => FALSE, 'width' => 10, 'desc' => "Species of the haplotype block"),
      'trait_name'          => array('req' => TRUE,  'width' => 10, 'desc' => "Descriptive name for a trait, it should be one of the crop trait ontology and should match a term in 'Trait' sheet."),
      'chromosome'          => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the chromosom"),
      'genus_chromosome'    => array('req' => FALSE, 'width' => 10, 'desc' => "Genus of the chromosome"),
      'species_chromosome'  => array('req' => FALSE, 'width' => 10, 'desc' => "Species of the chromosome"),
      'start'               => array('req' => TRUE,  'width' => 10, 'desc' => "The starting position"),
      'end'                 => array('req' => TRUE,  'width' => 10, 'desc' => "The ending position"),
      'strand'              => array('req' => FALSE, 'width' => 10, 'desc' => "The orientation/directionality of the location. Should be 0, -1 or +1."),
      'reference'           => array('req' => FALSE, 'width' => 10, 'desc' => "pub_id of the haplotype block."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['sequence']['chromosome']      = -1;
    $cvterms['sequence']['haplotype_block'] = -1;
    $cvterms['sequence']['contains']        = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks the organisms for the haplotype block.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);

    // Checks the organisms for the chromosome.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus_chromosome'], $line['species_chromosome']);

    // Checks the trait.
    MCL_CHADO_TRAIT::checkTrait($this, $line['trait_name'], NULL, "[,;]");

    // Checks the chromosome.
    MCL_CHADO_FEATURE::checkFeature($this, $line['chromosome'], $line['genus_chromosome'], $line['species_chromosome'], $this->cvterms['sequence']['chromosome']);

    // Checks the location.
    $this->_check_locations($this, $line['start'], $line['end']);

    // Checks the references.
    MCL_CHADO_PUB::checkPubID($this, $line['reference'], "[,;]");
  }

  /**
   * Checks the locations.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $start
   * @param integer $end
   *
   * @return boolean
   */
  private function _check_locations($mcl_tmpl, $start, $end) {
    if (!$start) {
      self::updateMsg($mcl_tmpl, 'E', "The starting location is not provided");
      return FALSE;
    }
    if (!$end) {
      self::updateMsg($mcl_tmpl, 'E', "The ending location is not provided");
      return FALSE;
    }
    if ($start > $end) {
      self::updateMsg($mcl_tmpl, 'E', "The starting and ending locations are not valid");
      return FALSE;
    }
    return TRUE;
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the organism.
    $organism = MCL_CHADO_ORGANISM::getOrganism($line['genus'], $line['species']);

    // Gets the chromosome.
    $chromosome = MCL_CHADO_FEATURE::getFeature($line['chromosome'], $line['genus_chromosome'], $line['species_chromosome'], $this->cvterms['sequence']['chromosome']);

    // Adds a haplotype block
    $haplotype_block = MCL_CHADO_FEATURE::addFeature($this, $line['haplotype_block'], $line['haplotype_block'], $this->cvterms['sequence']['haplotype_block'], $organism->getOrganismID());
    if ($haplotype_block && $chromosome) {

      // Adds the trait.
      $haplotype_block->addTrait($this, $line['trait_name'], NULL, "[,;]");

       // Adds the location.
      $haplotype_block->addLocation($this, $chromosome->getFeatureID(), $line['start'], $line['end'], $line['strand']);

      // Adds the reference.
      $haplotype_block->addReference($this, $line['reference'], "[,;]");
    }
  }
}