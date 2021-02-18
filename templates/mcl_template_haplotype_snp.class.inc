<?php
/**
 * The declaration of MCL_TEMPLATE_HAPLOTYPE_SNP class.
 *
 */
class MCL_TEMPLATE_HAPLOTYPE_SNP extends MCL_TEMPLATE {

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
      'haplotype_block' => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the haplotype block."),
      'genus'           => array('req' => FALSE, 'width' => 10, 'desc' => "Genus of the haplotype block."),
      'species'         => array('req' => FALSE, 'width' => 10, 'desc' => "Species of the haplotype block."),
      'marker_name'     => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the genetic marker."),
      '#name'           => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns (#) : followed by name."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['sequence']['genetic_marker']  = -1;
    $cvterms['sequence']['haplotype']       = -1;
    $cvterms['sequence']['haplotype_block'] = -1;
    $cvterms['sequence']['SNP']             = -1;
    $cvterms['sequence']['variant_of']      = -1;
    $cvterms['sequence']['contains']        = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks dataset.
    MCL_CHADO_DATASET::checkDataset($this, $line['dataset_name']);

    // Checks organisms for haplotype block.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);

    // Checks haplotype block.
    MCL_CHADO_FEATURE::checkFeature($this, $line['haplotype_block'], $line['genus'], $line['species'], $this->cvterms['sequence']['haplotype_block']);

    // Checks marker.
    MCL_CHADO_FEATURE::checkFeature($this, $line['marker_name'], '', '', $this->cvterms['sequence']['genetic_marker']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the marker.
    $marker = MCL_CHADO_FEATURE::getFeature($line['marker_name'], '', '', $this->cvterms['sequence']['genetic_marker']);

    // Gets the haplotype block.
    $hb = MCL_CHADO_FEATURE::getFeature($line['haplotype_block'], $line['genus'], $line['species'], $this->cvterms['sequence']['haplotype_block']);

    // Adds the relationships.
    if ($marker && $hb) {

      // Gets the genotype columns.
      $genotype_columns = $this->getJob()->getParamByKey('genotype_columns');
      foreach ($genotype_columns as $column) {

        // Adds a haplotype
        $uniquename = $hb->getUniquename() . '_' . $column;
        $haplotype = MCL_CHADO_FEATURE::addFeature($this, $uniquename, $column, $this->cvterms['sequence']['haplotype'], $marker->getOrganismID());
        if ($haplotype) {

          // Adds the relationship : haplotype_block 'contains' marker.
          $hb->addRelatedFeature($this, $marker, $this->cvterms['sequence']['contains']);

          // Adds the relationship : haplotype is 'variant_of' haplotype_block.
          $haplotype->addRelatedFeature($this, $hb, $this->cvterms['sequence']['variant_of']);

          // Adds the relationship : haplotype 'contains' marker.
          $haplotype->addRelatedFeature($this, $marker, $this->cvterms['sequence']['contains']);

          // Gets the feature_relationship_id.
          $keys = array(
            'subject_id'  => $haplotype->getFeatureID(),
            'object_id'   => $marker->getFeatureID(),
            'type_id'     => $this->cvterms['sequence']['contains'],
            'rank'        => 0,
          );
          $feature_rel = CHADO_FEATURE_RELATIONSHIP::byKey($keys);
          if ($feature_rel) {

            // Adds the SNP.
            $snp = $line['#'.$column];
            if ($snp) {
              MCL_CHADO_FEATURE::addRelationshipProp($this, $feature_rel->getFeatureRelationshipID(), 'sequence', 'SNP', $snp);
            }
          }
        }
      }
    }
  }

  /**
   * @see MCL_TEMPLATE::postProcessECD()
   */
  public function postProcessECD() {

    // Saves the genotyping columns.
    $genotype_columns = array();
    foreach ($this->getHeaders() as $column => $value) {
      if (preg_match("/^#(.*)$/", $column, $matches)) {
        $genotype_columns []= $matches[1];
      }
    }
    $this->getJob()->setParamByKey('genotype_columns', $genotype_columns);
    return $this->getJob()->update();
  }
}