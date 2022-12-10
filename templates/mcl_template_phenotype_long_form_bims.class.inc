<?php
/**
 * The declaration of MCL_TEMPLATE_PHENOTYPE_LONG_FORM_BIMS class.
 *
 */
class MCL_TEMPLATE_PHENOTYPE_LONG_FORM_BIMS extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'PHENOTYPE';
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
    $desc = 'The description for PHENOTYPE_BIMS_LONG_FORM sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'dataset_name'    => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the genotyping dataset (project). It should match a dataset_name in 'Dataset' sheet."),
      'accession'       => array('req' => TRUE,  'width' => 15, 'var' => TRUE, 'desc' => "ID of the accession that has been phenotyped. It should match an 'accession' column entry in the 'Accession' sheet or 'progeny_name' column entry 'Progeny' sheet."),
      'genus'           => array('req' => FALSE, 'width' => 10, 'desc' => "Genus of the accession"),
      'species'         => array('req' => FALSE, 'width' => 10, 'desc' => "Species of the accession"),
      'unique_id'       => array('req' => TRUE,  'width' => 10, 'var' => TRUE, 'desc' => "Unique ID of sample"),
      'primary_order'   => array('req' => TRUE,  'width' => 10, 'var' => TRUE, 'desc' => "The primary order of a sample"),
      'secondary_order' => array('req' => TRUE,  'width' => 10, 'var' => TRUE, 'desc' => "The secondary order of a sample"),
      'site_name'       => array('req' => FALSE, 'width' => 10, 'desc' => "Site information where the accession for the phenotyping is planted. It should match 'site_name' in the 'Site' sheet."),
      'evaluator'       => array('req' => FALSE, 'width' => 15, 'desc' => "Person who did the phenotyping. It should match 'contact_name' of the Contact sheet."),
      'clone_id'        => array('req' => FALSE, 'width' => 10, 'desc' => "ID of a specific clone if available (eg. individual tree)."),
      'rootstock'       => array('req' => FALSE, 'width' => 10, 'desc' => "Name of the rootstock if the plant is grafted to a rootstock. It should match an 'accession' column of the 'Accession' sheet."),
      'plant_date'      => array('req' => FALSE, 'width' => 10, 'desc' => "Date of the planting."),
      'data_year'       => array('req' => FALSE, 'width' => 10, 'desc' => "Phenotyping date if only year is known."),
      'evaluation_date' => array('req' => FALSE, 'width' => 10, 'desc' => "Date of phenotype evaluation."),
      'pick_date'       => array('req' => FALSE, 'width' => 10, 'desc' => "Date of the sample collection if the collection is done on a different date than the phenotype evaluation."),
      'previous_entry'  => array('req' => FALSE, 'width' => 10, 'desc' => "Accession of the previous entry."),
      'barcode'         => array('req' => FALSE, 'width' => 10, 'desc' => "Barcode"),
      'fiber_pkg'       => array('req' => FALSE, 'width' => 10, 'desc' => "Group of samples for phenotyping, can contain samples from multiple germplasm."),
      'storage_time'    => array('req' => FALSE, 'width' => 10, 'desc' => "Time between collection and phenotyping."),
      'storage_regime'  => array('req' => FALSE, 'width' => 10, 'desc' => "The condition of sample storage between the collection and phenotyping."),
      'comments'        => array('req' => FALSE, 'width' => 10, 'desc' => "Any comments for the phenotyping."),
      'trait'           => array('req' => TRUE,  'width' => 10, 'desc' => "The name of trait"),
      'value'           => array('req' => FALSE, 'width' => 10, 'desc' => "The value of the evaluation data"),
      '##property_name' => array('req' => FALSE, 'width' => 10, 'desc' => "Followed by property (cvterm) name. The data will be stored in property table."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['rootstock_id']             = -1;
    $cvterms['SITE_CV']['nd_geolocation_id']        = -1;
    $cvterms['SITE_CV']['sample']                   = -1;
    $cvterms['SITE_CV']['sample_of']                = -1;
    $cvterms['SITE_CV']['clone']                    = -1;
    $cvterms['SITE_CV']['clone_of']                 = -1;
    $cvterms['SITE_CV']['data_year']                = -1;
    $cvterms['SITE_CV']['plant_date']               = -1;
    $cvterms['SITE_CV']['pick_date']                = -1;
    $cvterms['SITE_CV']['evaluation_date']          = -1;
    $cvterms['SITE_CV']['storage_time']             = -1;
    $cvterms['SITE_CV']['storage_regime']           = -1;
    $cvterms['SITE_CV']['previous_entry']           = -1;
    $cvterms['SITE_CV']['barcode']                  = -1;
    $cvterms['SITE_CV']['fiber_pkg']                = -1;
    $cvterms['SITE_CV']['comments']                 = -1;
    $cvterms['SITE_CV']['source']                   = -1;
    $cvterms['SITE_CV']['previous_entry']           = -1;
    $cvterms['BIMS_FIELD_BOOK']['unique_id']        = -1;
    $cvterms['BIMS_FIELD_BOOK']['primary_order']    = -1;
    $cvterms['BIMS_FIELD_BOOK']['secondary_order']  = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Lists the job paremters.
    $program_id = $this->getJob()->getParamByKey('program_id');
    $cv_arr     = $this->getJob()->getParamByKey('cv_arr');

    // Gets the prefix.
    $prefix = BIMS_PROGRAM::getPrefixByID($program_id);

    // Gets BIMS_CHADO tables.
    $bc_accession  = new BIMS_CHADO_ACCESSION($program_id);
    $bc_project    = new BIMS_CHADO_PROJECT($program_id);
    $bc_site       = new BIMS_CHADO_SITE($program_id);

    // Gets the FB required columns.
    $bims_program = BIMS_PROGRAM::byID($program_id);
    $bims_cols    = $bims_program->getBIMSCols();

    // Checks dataset name.
    $bc_project->checkProject($this, $prefix . $line['dataset_name']);

    // Checks site.
    $bc_site->checkSite($this, $prefix . $line['site_name']);

    // Checks organism.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);

    // Checks accession.
    $accession = $line[strtolower($bims_cols['accession'])];
    $bc_accession->checkAccession($this, $prefix . $accession, $line['genus'], $line['species']);

    // Checks trait (descriptor).
    MCL_CHADO_TRAIT::checkTrait($this, $line['trait'], $cv_arr['descriptor']['name']);

    // Checks contact.
    MCL_CHADO_CONTACT::checkContact($this, $line['evaluator']);

    // Checks the evaluation data in descriptor column.
    MCL_CHADO_CVTERM::checkFBDescriptorCol($this, $line, $cv_arr['descriptor']['name']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the job paremters.
    $program_id = $this->getJob()->getParamByKey('program_id');
    $cv_arr     = $this->getJob()->getParamByKey('cv_arr');

    // Gets the prefix.
    $prefix = BIMS_PROGRAM::getPrefixByID($program_id);

    // Gets BIMS_CHADO tables.
    $bc_pc        = new BIMS_CHADO_PHENOTYPE_CALL($program_id);
    $bc_accession = new BIMS_CHADO_ACCESSION($program_id);
    $bc_sample    = new BIMS_CHADO_ACCESSION($program_id);
    $bc_project   = new BIMS_CHADO_PROJECT($program_id);
    $bc_site      = new BIMS_CHADO_SITE($program_id);
    $bc_image     = new BIMS_CHADO_IMAGE($program_id);

    // Gets FB required columns.
    $bims_program     = BIMS_PROGRAM::byID($program_id);
    $bims_cols        = $bims_program->getBIMSCols();
    $accession        = $line[strtolower($bims_cols['accession'])];
    $unique_id        = $line[strtolower($bims_cols['unique_id'])];
    $primary_order    = $line[strtolower($bims_cols['primary_order'])];
    $secondary_order  = $line[strtolower($bims_cols['secondary_order'])];

    // Gets db.
    $db = MCL_CHADO_DB::getDB(MCL_SITE_VAR::getValueByName('SITE_DB'));

    // Gets nd_geolocation_id.
    $nd_geolocation_id = MCL_DUMMY_VAR::getIdByName('ND_GEOLOCATION_ID');
    if ($line['site_name']) {
      $nd_geolocation_id = $bc_site->getLocationIDBySite($prefix . $line['site_name'], TRUE);
    }

    // Gets the project.
    $dataset = $bc_project->byTKey('project', array('name' => $prefix . $line['dataset_name']));

    // Gets the contact ID.
    $contact_name = $line['evaluator'] ? $line['evaluator'] : 'bims.anonymous';
    $contact      = MCL_CHADO_CONTACT::byName($contact_name);
    $contact_id   = $contact->getContactID();

    // Gets the stock.
    $accession = $bc_accession->byTKey('accession', array('uniquename' => $prefix . $accession));
    if ($accession) {

      // Creates sample ID.
      $sample_id = sprintf("%d.%d.%s", $dataset->project_id, $accession->stock_id, $unique_id);

      // Adds a new sample.
      $details = array(
        'uniquename'  => $prefix . $sample_id,
        'name'        => $sample_id,
        'organism_id' => $accession->organism_id,
        'type_id'     => $this->cvterms['SITE_CV']['sample'],
      );
      $stock_id_sample = $bc_accession->addAccession($this, $details);
      $bc_sample->setStockID($stock_id_sample);

      // Add relationships and properties if it is a new sample.
      if (!array_key_exists($sample_id, $GLOBALS['DUP_SAMPLES'])) {
        $GLOBALS['DUP_SAMPLES'][$sample_id] = TRUE;

        // Adds relationships between stock and sample.
        $bc_sample->addRelatedAccession($this, $accession, $this->cvterms['SITE_CV']['sample_of']);

        // Adds sample properties.
        $bc_sample->addProp($this, 'BIMS_FIELD_BOOK', 'unique_id', $unique_id);
        $bc_sample->addProp($this, 'BIMS_FIELD_BOOK', 'primary_order', $primary_order);
        $bc_sample->addProp($this, 'BIMS_FIELD_BOOK', 'secondary_order', $secondary_order);
        $bc_sample->addProp($this, 'SITE_CV', 'nd_geolocation_id', $nd_geolocation_id);
        $bc_sample->addProp($this, 'SITE_CV', 'plant_date', $line['plant_date']);
        $bc_sample->addProp($this, 'SITE_CV', 'pick_date', $line['pick_date']);
        $bc_sample->addProp($this, 'SITE_CV', 'storage_time', $line['storage_time']);
        $bc_sample->addProp($this, 'SITE_CV', 'barcode', $line['barcode']);
        $bc_sample->addProp($this, 'SITE_CV', 'fiber_pkg', $line['fiber_pkg']);
        $bc_sample->addProp($this, 'SITE_CV', 'storage_regime', $line['storage_regime']);
        $bc_sample->addProp($this, 'SITE_CV', 'data_year', $line['data_year']);
        $bc_sample->addProp($this, 'SITE_CV', 'evaluation_date', $line['evaluation_date']);
        $bc_sample->addProp($this, 'SITE_CV', 'comments', $line['comments']);

        // Adds the user-specific properties.
        $this->addSpecialColumns('##', $bc_sample, $line);

        // Adds the previous-entry.
        $bc_sample->addPreviousEntry($this, $line['previous-entry']);
      }

      // Gets the trait and the value.
      $cvterm_name  = $line['trait'];
      $value        = $line['value'];

      // Sets the default value if provided.
      if (!$value && $default_values[$cvterm_name]) {
        $value = $default_values[$cvterm_name];
      }

      // Adds the phenotyping data.
      if ($value || $value == '0') {

        // Gets the descriptor.
        $cvterm_id  = MCL_CHADO_CVTERM::getCvterm($cv_arr['descriptor']['name'], $cvterm_name)->getCvtermID();
        $descriptor = MCL_CHADO_TRAIT::byID($cvterm_id);

        // Adds photos.
        if ($descriptor->getFormat() == 'photo') {
          $bc_image->addSampleImage($this, $bc_sample->getStockID(), $cvterm_id, $value);
        }

        // Adds a data point.
        $args = array(
          'project_id'        => $dataset->project_id,
          'nd_geolocation_id' => $nd_geolocation_id,
          'stock_id'          => $bc_sample->getStockID(),
          'cvterm_id'         => $cvterm_id,
          'value'             => $value,
          'contact_id'        => $contact_id,
          'time'              => date("Y-m-d G:i:s"),
        );
        $phenotype_call_id = $bc_pc->addData($this, $args);
        if (!$phenotype_call_id) {
          $this->updateMsg('E', "Error : failed to add phenotype_call.");
        }
      }
    }
  }

  /**
   * @see MCL_TEMPLATE::postProcessECD()
   */
  public function postProcessECD() {

    // Lists the job paremters.
    $program_id = $this->getJob()->getParamByKey('program_id');

    // Gets the job paremters.
    $cv_arr = $this->getJob()->getParamByKey('cv_arr');

    // Saves the user-specific properties.
    if (!$this->saveSpecialColumns('##', $cv_arr['sample']['cv_id'], $this->getHeaders())) {
      $this->updateMsg('E', "Failed to save the user-specific property columns [##]");
      return FALSE;
    }

    // Checks if the custom table exists.
    $phenotype_call = new BIMS_CHADO_PHENOTYPE_CALL($program_id);
    if (!$phenotype_call->exists()) {
      $this->updateMsg('E', "The custom table does not exist.");
      return FALSE;
    }

    // Gets the site CV name.
    $site_cv_name = MCL_SITE_VAR::getValueByName('SITE_CV');

    // Checks db.
    MCL_CHADO_DB::checkDB($this, MCL_SITE_VAR::getValueByName('SITE_DB'));

    // Initiates $GLOBAL['DUP_SAMPLE'].
    $GLOBALS['DUP_SAMPLES'] = array();
    return TRUE;
  }

  /**
   * @see MCL_TEMPLATE::mapVarColumn()
   */
  public function mapVarColumn($column) {

    // Gets the BIMS columns.
    $program_id = $this->getJob()->getParamByKey('program_id');
    $bims_program = BIMS_PROGRAM::byID($program_id);
    $bims_cols = $bims_program->getBIMSCols();

    // Returns the label for the BIMS column.
    if (array_key_exists($column, $bims_cols)) {
      return $bims_program->getBIMSLabel($column, BIMS_LOWER);
    }
    return '';
  }

  /**
   * @see MCL_TEMPLATE::validColumns()
   */
  public function validColumns() {

    // Gets BIMS_PROGRAM.
    $program_id = $this->getJob()->getParamByKey('program_id');
    $bims_program = BIMS_PROGRAM::byID($program_id);

    // Returns FB required columns.
    return array_values($bims_program->getBIMSCols());
  }
}
