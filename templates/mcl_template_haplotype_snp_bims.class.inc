<?php
/**
 * The declaration of MCL_TEMPLATE_HAPLOTYPE_SNP_BIMS class.
 *
 */
class MCL_TEMPLATE_HAPLOTYPE_SNP_BIMS extends MCL_TEMPLATE {

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
    $desc = 'The description for haplotype_SNP sheet';
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

    // Gets the job paremters.
    $program_id = $this->getJob()->getParamByKey('program_id');

    // Gets the prefix.
    $prefix = BIMS_PROGRAM::getPrefixByID($program_id);

    // Checks organisms for haplotype block.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);

    // Gets BIMS_CHADO_FEATURE tables.
    $bc_feature = new BIMS_CHADO_FEATURE($program_id);

    // Checks the haplotype block.
    $bc_feature->checkFeature($this, $prefix . $line['haplotype_block'], $line['genus'], $line['species'], $this->cvterms['sequence']['haplotype_block']);

    // Checks the marker.
    $bc_feature->checkFeature($this, $prefix . $line['marker_name'], $line['genus'], $line['species'], $this->cvterms['sequence']['genetic_marker']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the job paremters.
    $program_id = $this->getJob()->getParamByKey('program_id');

    // Gets the prefix.
    $prefix = BIMS_PROGRAM::getPrefixByID($program_id);

    // Gets BIMS_CHADO_FEATURE tables.
    $bc_feature = new BIMS_CHADO_FEATURE($program_id);

    // Gets the haplotype block.
    $keys = array(
      'uniquename'  => $prefix . $line['haplotype_block'],
      'type_id'     => $this->cvterms['sequence']['haplotype_block'],
    );
    if ($line['genus'] && $line['species']) {

      // Gets and sets the organism.
      $organism = MCL_CHADO_ORGANISM::getOrganism($line['genus'], $line['species']);
      $keys['organism_id'] = $organism->getOrganismID();
    }
    $hb = $bc_feature->byTKey('feature', $keys);

    // Gets the marker.
    $keys = array(
      'uniquename'  => $prefix . $line['marker_name'],
      'type_id'     => $this->cvterms['sequence']['genetic_marker'],
    );
    $marker = $bc_feature->byTKey('feature', $keys);

    // Adds the relationships.
    if ($marker && $hb) {

      // Gets the genotype columns.
      $genotype_columns = $this->getJob()->getParamByKey('genotype_columns');
      foreach ($genotype_columns as $column) {

        // Adds the haplotype
        $details = array(
          'uniquename'  => $hb->uniquename . '_' . $column,
          'name'        => $column,
          'type_id'     => $this->cvterms['sequence']['haplotype'],
          'organism_id' => $organism->getOrganismID(),
        );
        $haplotype_fid = $bc_feature->addFeature($this, $details);
        if ($haplotype_fid) {

          // Adds the relationship : haplotype_block 'contains' marker.
          $bc_feature->addRelatedFeature($this, $hb->feature_id, $marker->feature_id, $this->cvterms['sequence']['contains']);

          // Adds the relationship : haplotype is 'variant_of' haplotype_block.
          $bc_feature->addRelatedFeature($this, $haplotype_fid, $hb->feature_id, $this->cvterms['sequence']['variant_of']);

          // Adds the relationship : haplotype 'contains' marker.
          $feature_relationship_id = $bc_feature->addRelatedFeature($this, $haplotype_fid, $marker->feature_id, $this->cvterms['sequence']['contains']);

          // Adds the SNP.
          if ($feature_relationship_id) {
            $snp = $line['#' . $column];
            if ($snp) {
              $bc_feature->addRelatedFeatureprop($this, $feature_relationship_id, $this->cvterms['sequence']['SNP'], $snp);
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