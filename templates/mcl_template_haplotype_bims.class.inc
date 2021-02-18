<?php
/**
 * The declaration of MCL_TEMPLATE_HAPLOTYPE_BIMS class.
 *
 */
class MCL_TEMPLATE_HAPLOTYPE_BIMS extends MCL_TEMPLATE {

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
      'dataset_name'    => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the phenotyping dataset. It should match a 'dataset_name' column entry in the 'Dataset' sheet."),
      'accession'       => array('req' => TRUE,  'width' => 10, 'desc' => "ID of the stock that has been phenotyped. It should match a 'stock_name' column entry in the 'Stock' sheet or 'progeny_name' column entry 'Progeny' sheet."),
      'genus'           => array('req' => TRUE,  'width' => 10, 'desc' => "Genus of the stock"),
      'species'         => array('req' => TRUE,  'width' => 10, 'desc' => "Species of the stock"),
      'haplotype_block' => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the haplotype block"),
      '#hap'            => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns (#) : followed by hapN where N is an integer."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['genotype']         = -1;
    $cvterms['SITE_CV']['genotyping']       = -1;
    $cvterms['sequence']['haplotype_block'] = -1;
    $cvterms['sequence']['allele']          = -1;
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

    // Gets BIMS_CHADO tables.
    $bc_project   = new BIMS_CHADO_PROJECT($program_id);
    $bc_accession = new BIMS_CHADO_ACCESSION($program_id);
    $bc_feature   = new BIMS_CHADO_FEATURE($program_id);

    // Gets FB required columns.
    $bims_program = BIMS_PROGRAM::byID($program_id);
    $bims_cols    = $bims_program->getBIMSCols();

    // Checks dataset.
    $bc_project->checkProject($this, $prefix . $line['dataset_name']);

    // Checks accession.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);
    $accession = $line[strtolower($bims_cols['accession'])];
    $bc_accession->checkAccession($this, $prefix . $accession, $line['genus'], $line['species']);

    // Checks haplotype block.
    $bc_feature->checkFeature($this, $prefix . $line['haplotype_block'], '', '', $this->cvterms['sequence']['haplotype_block']);
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
    $bc_project       = new BIMS_CHADO_PROJECT($program_id);
    $bc_accession     = new BIMS_CHADO_ACCESSION($program_id);
    $bc_feature       = new BIMS_CHADO_FEATURE($program_id);
    $bc_nd_experiment = new BIMS_CHADO_ND_EXPERIMENT($program_id);
    $bc_genotype      = new BIMS_CHADO_GENOTYPE($program_id);

    // Gets FB required columns.
    $bims_program     = BIMS_PROGRAM::byID($program_id);
    $bims_cols        = $bims_program->getBIMSCols();

    // Gets the dataset.
    $dataset = $bc_project->byTKey('project', array('name' => $prefix . $line['dataset_name']));

    // Gets the organism of the accession.
    $organism = MCL_CHADO_ORGANISM::getOrganism($line['genus'], $line['species']);
    $organism_id = $organism->getOrganismID();

    // Gets the accession.
    $accession = $line[strtolower($bims_cols['accession'])];
    $keys = array(
      'uniquename'  => $prefix . $accession,
      'organism_id' => $organism_id,
    );
    $accession = $bc_accession->byTKey('accession', $keys);

    // Gets the haplotype_block.
    $keys = array(
      'uniquename'  => $prefix . $line['haplotype_block'],
      'type_id'     => $this->cvterms['sequence']['haplotype_block'],
    );
    $hb = $bc_feature->byTKey('feature', $keys);

    // Adds a haplotype.
    if ($dataset && $accession && $hb) {
      $bc_feature->setFeatureID($hb->feature_id);

      // Gets the gentoype and add it to genotype table.
      $genotype_columns = $this->getJob()->getParamByKey('genotype_columns');
      $genotype_arr = array();
      foreach ((array)$genotype_columns as $idx => $column) {
        if ($line[$column]) {
          $genotype_arr []= $line[$column];
        }
      }

      // Adds a genotype.
      if (!empty($genotype_arr)) {
        $description  = implode('|', $genotype_arr);
        $details = array(
          'uniquename'  => $hb->uniquename . '_' . $description,
          'type_id'     => $this->cvterms['SITE_CV']['genotype'],
          'description' => $description,
        );
        $genotype_id = $bc_genotype->addGenotype($this, $details);
        if (!$genotype_id) {
          return FALSE;
        }

        // Adds a nd_experiment.
        $dup_key = sprintf("%d-%d-%d", $dataset->project_id, $accession->stock_id, $hb->feature_id);
        $details = array(
          'type_id'         => $this->cvterms['sequence']['allele'],
          'project_id'      => $dataset->project_id,
          'stock_id'        => $accession->stock_id,
          'genotype_id'     => $genotype_id,
          'dup_key'         => $dup_key,
          'dup_key_type_id' => $this->cvterms['SITE_CV']['genotype'],
        );
        $nd_experiment_id = $bc_nd_experiment->addNdExperiment($this, $details);
        if (!$nd_experiment_id) {
          return FALSE;
        }

        // Adds a genotype for marker.
        $bc_feature->addGenotype($this, $genotype_id, $hb->type_id);
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
      if (preg_match("/^#hap(\d+)$/i", $column, $matches)) {
        $idx = intval($matches[1]);
        $genotype_columns[$idx]= $column;
      }
    }
    ksort($genotype_columns, SORT_NUMERIC);
    $this->getJob()->setParamByKey('genotype_columns', $genotype_columns);
    return $this->getJob()->update();
  }
}