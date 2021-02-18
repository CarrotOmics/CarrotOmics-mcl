<?php
/**
 * The declaration of MCL_TEMPLATE_MARKER_LOC class.
 *
 */
class MCL_TEMPLATE_MARKER_LOC extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'MARKER_POST';
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
    $desc = 'The description for marker_loc sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'genome'      => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the genome."),
      'chromosome'  => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the chromosome."),
      'marker_name' => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the marker."),
      'genus'       => array('req' => FALSE, 'width' => 10, 'desc' => "The genus name of the organism from which the marker was developed."),
      'species'     => array('req' => FALSE, 'width' => 10, 'desc' => "The species name of the organism from which the marker was developed. Enter 'sp.' to represent one unknown species, 'spp.' to represent multiple unknown species."),
      'fmin'        => array('req' => FALSE, 'width' => 10, 'desc' => "The leftmost/minimal boundar"),
      'fmax'        => array('req' => FALSE, 'width' => 10, 'desc' => "The rightmost/maximal boundary"),
      'strand'      => array('req' => FALSE, 'width' => 10, 'desc' => "Orientation of the location (should be 0, -1 or +1)"),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['sequence']['genetic_marker']  = -1;
    $cvterms['sequence']['chromosome']      = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks genome.
    MCL_CHADO_ANALYSIS::checkAnalysis($this, $line['genome']);

    // Checks chromosome.
    MCL_CHADO_FEATURE::checkFeature($this, $line['chromosome'], NULL, NULL, $this->cvterms['sequence']['chromosome']);

    // Checks marker.
    MCL_CHADO_FEATURE::checkFeature($this, $line['marker_name'], $line['genus'], $line['species'], $this->cvterms['sequence']['genetic_marker']);

    // Checks locations.
    $this->checkRange($line['fmin'], $line['fmax']);

    // Checks strand.
    $this->checkStrand($line['strand']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the genome.
    $genome = MCL_CHADO_ANALYSIS::byName($line['genome']);

    // Gets the chromosome.
    $chromosome = MCL_CHADO_FEATURE::getFeature($line['chromosome'], '', '', $this->cvterms['sequence']['chromosome']);

    // Gets the marker.
    $marker = MCL_CHADO_FEATURE::getFeature($line['marker_name'], $line['genus'], $line['species'], $this->cvterms['sequence']['genetic_marker']);

    // Adds the marker locations.
    if ($line['fmax'] || $line['fmin']) {
      $marker->addLocation($this, $chromosome->getFeatureID(), $line['fmin'], $line['fmax'], $line['strand']);
    }
  }
}