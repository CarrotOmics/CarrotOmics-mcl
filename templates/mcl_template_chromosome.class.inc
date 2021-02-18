<?php
/**
 * The declaration of MCL_TEMPLATE_CHROMOSOME class.
 *
 */
class MCL_TEMPLATE_CHROMOSOME extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'MARKER';
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
    $desc = 'The description for chromosome sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'genome'             => array('req' => TRUE, 'width' => 10, 'desc' => "Name of the genome. It should match a 'name' column entry in the 'Analysis' sheet."),
      'chromosome'         => array('req' => TRUE, 'width' => 10, 'desc' => "Name of the chromosome."),
      'genus'              => array('req' => TRUE, 'width' => 10, 'desc' => "Genus of the chromosome"),
      'species'            => array('req' => TRUE, 'width' => 10, 'desc' => "Species of the chromosome"),
      'infraspecific_type' => array('req' => FALSE, 'width' => 10, 'desc' => "Infraspecific type of the stock as defined in the taxonomic_rank CV"),
      'infraspecific_name' => array('req' => FALSE, 'width' => 10, 'desc' => "Infraspecific name of the stock"),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['sequence']['chromosome'] = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks genome.
    MCL_CHADO_ANALYSIS::checkAnalysis($this, $line['genome']);

    // Checks organism.
    MCL_CHADO_ORGANISM::checkOrganismInfra($this, $line['genus'], $line['species'], $line['infraspecific_type'], $line['infraspecific_name']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the genome.
    $genome = MCL_CHADO_ANALYSIS::byName($line['genome']);

    // Gets the organism.
    $organism = MCL_CHADO_ORGANISM::getOrganismInfra($line['genus'], $line['species'], $line['infraspecific_type'], $line['infraspecific_name']);

    // Adds a chromosome.
    $chromosome = MCL_CHADO_FEATURE::addFeature($this, $line['chromosome'], $line['chromosome'], $this->cvterms['sequence']['chromosome'], $organism->getOrganismID());
    if ($chromosome) {

      // Adds the relationship with the genome.
      $genome->addFeature($this, $chromosome);
    }
  }
}
