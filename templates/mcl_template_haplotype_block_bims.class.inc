<?php
/**
 * The declaration of MCL_TEMPLATE_HAPLOTYPE_BLOCK_BIMS class.
 *
 */
class MCL_TEMPLATE_HAPLOTYPE_BLOCK_BIMS extends MCL_TEMPLATE {

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
      'genus'               => array('req' => TRUE,  'width' => 10, 'desc' => "Genus of the haplotype block"),
      'species'             => array('req' => TRUE,  'width' => 10, 'desc' => "Species of the haplotype block"),
      'trait_name'          => array('req' => TRUE,  'width' => 10, 'desc' => "Descriptive name for trait, it should be one of the crop trait ontology and should match a term in 'Trait' sheet."),
      'chromosome'          => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the chromosom"),
      'genus_chromosome'    => array('req' => TRUE,  'width' => 10, 'desc' => "Genus of the chromosome"),
      'species_chromosome'  => array('req' => TRUE,  'width' => 10, 'desc' => "Species of the chromosome"),
      'start'               => array('req' => TRUE,  'width' => 10, 'desc' => "The starting position"),
      'end'                 => array('req' => TRUE,  'width' => 10, 'desc' => "The ending position"),
      'strand'              => array('req' => FALSE, 'width' => 10, 'desc' => "The orientation/directionality of the location. Should be 0, -1 or +1."),
      'reference'           => array('req' => FALSE, 'width' => 10, 'desc' => "pub_id of the haplotype block"),
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

    // Gets the job paremters.
    $program_id = $this->getJob()->getParamByKey('program_id');
    $cv_arr     = $this->getJob()->getParamByKey('cv_arr');

    // Gets the prefix.
    $prefix = BIMS_PROGRAM::getPrefixByID($program_id);

    // Gets BIMS_CHADO tables.
    $bc_feature = new BIMS_CHADO_FEATURE($program_id);

    // Checks the organims of haplotype block.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);

    // Checks the chromosome.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus_chromosome'], $line['species_chromosome']);
    MCL_CHADO_FEATURE::checkFeature($this, $line['chromosome'], $line['genus_chromosome'], $line['species_chromosome'], $this->cvterms['sequence']['chromosome']);

    // Checks trait.
    MCL_CHADO_TRAIT::checkTrait($this, $line['trait_name'], NULL, "[,;]");

    // Checks the location.
    $this->_check_locations($this, $line['start'], $line['end'], $line['strand']);

    // Checks references.
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
  private function _check_locations($mcl_tmpl, $start, $end, $strand) {
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
    if ($strand) {
      if (!($strand == '0' || $strand == '+1' || $strand == '-1')) {
        self::updateMsg($mcl_tmpl, 'E', "The strand should be 0, -1 or +1");
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the job paremters.
    $program_id = $this->getJob()->getParamByKey('program_id');

    // Gets the prefix.
    $prefix = BIMS_PROGRAM::getPrefixByID($program_id);

    // Gets BIMS_CHADO tables.
    $bc_feature = new BIMS_CHADO_FEATURE($program_id);

    // Gets the organism.
    $organism = MCL_CHADO_ORGANISM::getOrganism($line['genus'], $line['species']);
    $organism_id = $organism->getOrganismID();

    // Gets the chromosome.
    $chromosome = MCL_CHADO_FEATURE::getFeature($line['chromosome'], $line['genus_chromosome'], $line['species_chromosome'], $this->cvterms['sequence']['chromosome']);

    // Adds a haplotype block data.
    if ($chromosome) {

      // Adds a haplotype block.
      $details = array(
        'uniquename'  => $prefix . $line['haplotype_block'],
        'name'        => $line['haplotype_block'],
        'organism_id' => $organism_id,
        'type_id'     => $this->cvterms['sequence']['haplotype_block'],
      );
      $feature_id = $bc_feature->addFeature($this, $details);
      if (!$feature_id) {
        return FALSE;
      }
      $bc_feature->setFeatureID($feature_id);

      // Adds the trait.
      $bc_feature->addTrait($this, $line['trait_name'], NULL, "[,;]");

      // Adds the location.
      $bc_feature->addLocation($this, $chromosome->getFeatureID(), $line['start'], $line['end'], $line['strand']);

      // Adds the reference.
      $bc_feature->addReference($this, $pub, "[,;]");
    }
  }
}