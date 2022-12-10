<?php
/**
 * The declaration of MCL_TEMPLATE_HAPLOTYPE_EFFECT class.
 *
 */
class MCL_TEMPLATE_HAPLOTYPE_EFFECT extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'HAPLOTYPE';
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
      'dataset_name'  => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the phenotyping dataset (project). It should match a 'dataset_name' column entry in the 'Dataset' sheet."),
      'marker_name'   => array('req' => TRUE,  'width' => 10, 'desc' => "Name of marker (QTL/MTL)."),
      'mean'          => array('req' => TRUE,  'width' => 10, 'desc' => ""),
      'std'           => array('req' => TRUE,  'width' => 10, 'desc' => ""),
    );

    // Adds labels.
    $headers['std']['label'] = 'STD';
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['genotype']   = -1;
    $cvterms['SITE_CV']['mean']       = -1;
    $cvterms['SITE_CV']['std']        = -1;
    $cvterms['SITE_CV']['project_id'] = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks dataset.
    MCL_CHADO_DATASET::checkDataset($this, $line['dataset_name']);

    // Checks organisms.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {}
}
