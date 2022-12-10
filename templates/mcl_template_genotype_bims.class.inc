<?php
/**
 * The declaration of MCL_TEMPLATE_GENOTYPE_BIMS class.
 *
 */
class MCL_TEMPLATE_GENOTYPE_BIMS extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'GENOTYPE';
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
    $desc = 'The description for genotype sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'dataset_name'    => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the genotyping dataset (project). It should match a dataset_name in 'Dataset' sheet."),
      'accession'       => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the stock genotyped. It should match a 'stock_name' of the 'Stock' sheet."),
      'genus'           => array('req' => TRUE,  'width' => 10, 'desc' => "Genus of the stock"),
      'species'         => array('req' => TRUE,  'width' => 10, 'desc' => "Species of the stock"),
      'clone_id'        => array('req' => FALSE, 'width' => 10, 'desc' => "ID of a clone if known."),
      'marker'          => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the marker used for genotyping. It should match a marker_name in the 'Marker' sheet."),
      'storage_id'      => array('req' => FALSE, 'width' => 10, 'desc' => "Any storage ID for the DNA sample."),
      'plot'            => array('req' => FALSE, 'width' => 10, 'desc' => "Field position where the plant material was collected from."),
      'row'             => array('req' => FALSE, 'width' => 10, 'desc' => "Field position where the plant material was collected from."),
      'position'        => array('req' => FALSE, 'width' => 10, 'desc' => "Field position where the plant material was collected from."),
      'collector'       => array('req' => FALSE, 'width' => 10, 'desc' => "People who collected the plant material. It should match 'contact_name' in the Contact sheet. Multiple names can be added with a semicolon (;) in between."),
      'collection_date' => array('req' => FALSE, 'width' => 10, 'desc' => "Date when the plant material was collected."),
      'genotyping_date' => array('req' => FALSE, 'width' => 10, 'desc' => "Date when genotypying."),
      'instrument'      => array('req' => FALSE, 'width' => 10, 'desc' => "Any instrument used for genotyping."),
      'size_standard'   => array('req' => FALSE, 'width' => 10, 'desc' => "Date of the genotyping experiment."),
      'data_year'       => array('req' => FALSE, 'width' => 10, 'desc' => "Genotyping date if only year is known."),
      'experimenter'    => array('req' => FALSE, 'width' => 10, 'desc' => "Person who conducted genotyping. It should match 'contact_name' in the Contact sheet. Multiple names can be added with a semicolon (;) in between."),
      'dye_color'       => array('req' => FALSE, 'width' => 10, 'desc' => "Dye color used for genotyping."),
      'comments'        => array('req' => FALSE, 'width' => 10, 'desc' => "Any comments on the specific genotype (of a specific sample with a specific marker)."),
      '#allele'         => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns (#) : followed by allele_N where N is an integer."),
    );

    // Adds labels.
    $headers['clone_id']['label']   = 'clone_ID';
    $headers['storage_id']['label'] = 'storage_ID';
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['genotyping']       = -1;
    $cvterms['SITE_CV']['clone']            = -1;
    $cvterms['SITE_CV']['clone_of']         = -1;
    $cvterms['SITE_CV']['sample']           = -1;
    $cvterms['SITE_CV']['sample_of']        = -1;
    $cvterms['SITE_CV']['sample_id']        = -1;
    $cvterms['SITE_CV']['storage_id']       = -1;
    $cvterms['SITE_CV']['plot']             = -1;
    $cvterms['SITE_CV']['row']              = -1;
    $cvterms['SITE_CV']['position']         = -1;
    $cvterms['SITE_CV']['pick_date']        = -1;
    $cvterms['SITE_CV']['collection_date']  = -1;
    $cvterms['SITE_CV']['genotyping_date']  = -1;
    $cvterms['SITE_CV']['collector']        = -1;
    $cvterms['SITE_CV']['instrument']       = -1;
    $cvterms['SITE_CV']['size_standard']    = -1;
    $cvterms['SITE_CV']['dye_color']        = -1;
    $cvterms['SITE_CV']['comments']         = -1;
    $cvterms['SITE_CV']['data_year']        = -1;
    $cvterms['sequence']['genetic_marker']  = -1;
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

    // Checks the dataset.
    $bc_project->checkProject($this, $prefix . $line['dataset_name']);

    // Checks the organism.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);

    // Checks the accession.
    $accession = $line[strtolower($bims_cols['accession'])];
    $bc_accession->checkAccession($this, $prefix . $accession, $line['genus'], $line['species']);

    // Checks the marker.
    $bc_feature->checkFeature($this, $prefix . $line['marker'], $line['genus'], $line['species'], $this->cvterms['sequence']['genetic_marker']);

    // Checks the collectors and the experimenters.
    MCL_CHADO_CONTACT::checkContact($this, $line['collector'], "[;]");
    MCL_CHADO_CONTACT::checkContact($this, $line['experimenter'], "[;]");
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
    $bc_sample        = new BIMS_CHADO_ACCESSION($program_id);
    $bc_feature       = new BIMS_CHADO_FEATURE($program_id);
    $bc_genotype      = new BIMS_CHADO_GENOTYPE($program_id);
    $bc_nd_experiment = new BIMS_CHADO_ND_EXPERIMENT($program_id);

    // Gets FB required columns.
    $bims_program = BIMS_PROGRAM::byID($program_id);
    $bims_cols    = $bims_program->getBIMSCols();

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

    // Gets the marker.
    $keys = array(
      'uniquename'  => $prefix . $line['marker'],
      'organism_id' => $organism_id,
      'type_id'     => $this->cvterms['sequence']['genetic_marker'],
    );
    $marker = $bc_feature->byTKey('feature', $keys);

    // Adds a genotype.
    if ($dataset && $accession && $marker) {

      // Gets the gentoype (allele).
      $genotype_columns = $this->getJob()->getParamByKey('genotype_columns');
      $tmp = array();
      foreach ($genotype_columns as $column) {
        if ($line[$column]) {
          $tmp []= $line[$column];
        }
      }
      sort($tmp);
      $allele_str = implode('|', $tmp);

      // Adds a genotype.
      $details = array(
        'uniquename'  => $marker->uniquename . '_' . $allele_str,
        'type_id'     => $this->cvterms['sequence']['allele'],
        'description' => $allele_str,
      );
      $genotype_id = $bc_genotype->addGenotype($this, $details);
      if (!$genotype_id) {
        return FALSE;
      }

      // Adds a sample.
      $sample_id = sprintf("%d-%d-%d", $dataset->project_id, $stock->stock_id, $marker->feature_id);
      $details = array(
        'uniquename'  => $prefix . $sample_id,
        'name'        => $sample_id,
        'organism_id' => $accession->organism_id,
        'type_id'     => $this->cvterms['SITE_CV']['sample'],
      );
      $stock_id_sample = $bc_accession->addAccession($this, $details);
      if (!$stock_id_sample) {
        return FALSE;
      }
      $bc_sample->setStockID($stock_id_sample);

      // Adds sample properties.
      $bc_sample->addProp($this, 'SITE_CV', 'storage_id', $line['storage_id']);
      $bc_sample->addProp($this, 'SITE_CV', 'plot', $line['plot']);
      $bc_sample->addProp($this, 'SITE_CV', 'row', $line['row']);
      $bc_sample->addProp($this, 'SITE_CV', 'position', $line['position']);
      $bc_sample->addProp($this, 'SITE_CV', 'collection_date', $line['collection_date']);

      // Adds the collectors.
      $bc_sample->addContact($this, $line['collector'], "[;]");

      // Adds stock relationship between sample and stock.
      $bc_sample->addRelatedAccession($this, $accession, $this->cvterms['SITE_CV']['sample_of']);

      // Adds a nd_experiment.
      $details = array(
        'type_id'         => $this->cvterms['sequence']['allele'],
        'project_id'      => $dataset->project_id,
        'stock_id'        => $stock_id_sample,
        'genotype_id'     => $genotype_id,
        'dup_key'         => $sample_id,
        'dup_key_type_id' => $this->cvterms['SITE_CV']['sample_id'],
      );
      $nd_experiment_id = $bc_nd_experiment->addNdExperiment($this, $details);
      if (!$nd_experiment_id) {
        return FALSE;
      }
      $bc_nd_experiment->setNdExperimentID($nd_experiment_id);

      // Adds nd_experiment properties.
      $bc_nd_experiment->addProp($this, 'SITE_CV', 'genotyping_date', $line['genotyping_date']);
      $bc_nd_experiment->addProp($this, 'SITE_CV', 'instrument', $line['instrument']);
      $bc_nd_experiment->addProp($this, 'SITE_CV', 'size_standard', $line['size_standard']);
      $bc_nd_experiment->addProp($this, 'SITE_CV', 'data_year', $line['data_year']);
      $bc_nd_experiment->addProp($this, 'SITE_CV', 'dye_color', $line['dye_color']);
      $bc_nd_experiment->addProp($this, 'SITE_CV', 'comments', $line['comments']);

      // Adds the experimenters.
      $bc_nd_experiment->addContact($this, $line['experimenter'], "[;]");

      // Adds a genotype for marker.
      $bc_feature->setFeatureID($marker->feature_id);
      $bc_feature->addGenotype($this, $genotype_id, $this->cvterms['sequence']['allele']);
    }
  }

  /**
   * @see MCL_TEMPLATE::postProcessECD()
   */
  public function postProcessECD() {

    // Saves the genotyping columns.
    $genotype_columns = array();
    foreach ($this->getHeaders() as $column => $value) {
      if (preg_match("/^(allele\_\d+)$/i", $column, $matches)) {
        $genotype_columns []= $column;
      }
    }
    $this->getJob()->setParamByKey('genotype_columns', $genotype_columns);
    return $this->getJob()->update();
  }
}
