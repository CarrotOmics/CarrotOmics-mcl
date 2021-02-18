<?php
/**
 * The declaration of MCL_TEMPLATE_GENOTYPE_SNP_LONG_FORM_BIMS class.
 *
 */
class MCL_TEMPLATE_GENOTYPE_SNP_LONG_FORM_BIMS extends MCL_TEMPLATE {

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
    $desc = "
      This is the template to upload SNP genotyping data. The marker names are listed as headers with prefix '$'.
      There are no place to specify the organism for the markers. So this template is used only when marker names
      are absolutely unique in the feature table.
    ";
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'dataset_name'    => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the genotyping dataset. It should match a dataset_name in 'Dataset' sheet."),
      'accession'       => array('req' => TRUE,  'width' => 15, 'var' => TRUE, 'desc' => "ID of the accession that has been phenotyped. It should match an 'accession' column entry in the 'Accession' sheet or 'progeny_name' column entry 'Progeny' sheet."),
      'genus'           => array('req' => FALSE, 'width' => 10, 'desc' => "Genus of the stock"),
      'species'         => array('req' => FALSE, 'width' => 10, 'desc' => "Species of the stock"),
      'marker_name'     => array('req' => TRUE,  'width' => 10, 'desc' => "The name of marker"),
      'genotype'        => array('req' => FALSE, 'width' => 10, 'desc' => "Type the alleles with '|' in between (e.g. A|T)"),
      'evaluator'       => array('req' => FALSE, 'width' => 10, 'desc' => "Person who did the genotyping. It should match 'contact_name' of the Contact sheet."),
      '##property_name' => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns (##) : followed by name (cvterm name) of the property."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['sequence']['SNP'] = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Gets the program ID.
    $program_id = $this->getJob()->getParamByKey('program_id');

    // Gets the BIMS columns.
    $bims_program = BIMS_PROGRAM::byID($program_id);
    $bims_cols    = $bims_program->getBIMSCols();

    // Gets the prefix.
    $prefix = BIMS_PROGRAM::getPrefixByID($program_id);

    // Checks the dataset.
    $dataset_name = $line['dataset_name'];
    if (!array_key_exists($dataset_name, $GLOBALS['ref_table']['dataset'])) {
      $bc_project   = new BIMS_CHADO_PROJECT($program_id);
      $GLOBALS['ref_table']['dataset'][$dataset_name] = NULL;
      $dataset = $bc_project->byTKey('project', array('name' => $prefix . $dataset_name));
      if ($dataset) {
        $GLOBALS['ref_table']['dataset'][$dataset_name] = $dataset->project_id;
      }
      else {
        $this->updateMsg('E', "$dataset_name does not exist in project table");
      }
    }
    else if (!$GLOBALS['ref_table']['dataset'][$dataset_name]) {
      $this->updateMsg('E', "$dataset_name does not exist in project table");
    }

    // Checks the accession.
    $accession_name = $line[strtolower($bims_cols['accession'])];
    if (!array_key_exists($accession_name, $GLOBALS['ref_table']['accession'])) {
      $bc_accession = new BIMS_CHADO_ACCESSION($program_id);
      $GLOBALS['ref_table']['accession'][$accession_name] = NULL;
      $accession = $bc_accession->byTKey('accession', array('uniquename' => $prefix . $accession_name));
      if ($accession) {
        $GLOBALS['ref_table']['accession'][$accession_name] = $accession->stock_id;
      }
      else {
        $this->updateMsg('E', "$accession_name does not exist in accession table");
      }
    }
    else if (!$GLOBALS['ref_table']['accession'][$accession_name]) {
      $this->updateMsg('E', "$accession_name does not exist in accession table");
    }

    // Checks the marker.
    $marker_name = $line['marker_name'];
    if (!array_key_exists($marker_name, $GLOBALS['ref_table']['marker'])) {
      $bc_feature = new BIMS_CHADO_FEATURE($program_id);
      $GLOBALS['ref_table']['marker'][$marker_name] = NULL;
      $marker = $bc_feature->byTKey('feature', array('uniquename' => $prefix . $marker_name));
      if ($marker) {
        $GLOBALS['ref_table']['marker'][$marker_name] = $marker->feature_id;
      }
      else {
        $this->updateMsg('E', "$marker_name does not exist in feature table");
      }
    }
    else if (!$GLOBALS['ref_table']['marker'][$marker_name]) {
      $this->updateMsg('E', "$marker_name does not exist in feature table");
    }

    // Checks the contact.
    if (array_key_exists('evaluator', $line) && $line['evaluator']) {
      $contact_name = $line['evaluator'];
      if (!array_key_exists($contact_name, $GLOBALS['ref_table']['contact'])) {
        $GLOBALS['ref_table']['contact'][$contact_name] = NULL;
        $contact = MCL_CHADO_CONTACT::byName($contact_name);
        if ($contact) {
          $GLOBALS['ref_table']['contact'][$contact_name] = $contact->getContactID();
        }
        else {
          $this->updateMsg('E', "$contact_name does not exist in contact table");
        }
      }
      else if (!$GLOBALS['ref_table']['contact'][$contact_name]) {
        $this->updateMsg('E', "$contact_name does not exist in contact table");
      }
    }
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the referece table.
    $ref_table = $GLOBALS['ref_table'];

    // Gets Job.
    $job = $this->getJob();

    // Gets the job paremters.
    $program_id = $job->getParamByKey('program_id');

    // Gets BIMS_CHADO table.
    $bc_gc        = new BIMS_CHADO_GENOTYPE_CALL($program_id);
    $bc_genotype  = new BIMS_CHADO_GENOTYPE($program_id);

     // Gets the BIMS columns.
    $bims_program = BIMS_PROGRAM::byID($program_id);
    $bims_cols    = $bims_program->getBIMSCols();

    // Gets the prefix.
    $prefix = BIMS_PROGRAM::getPrefixByID($program_id);

    // Gets SQL files.
    $sql_gc  = $job->getJobDir() . '/' . $job->getJobID() . '-genotype_call.sql';
    $sql_gcp = $job->getJobDir() . '/' . $job->getJobID() . '-genotype_callprop.sql';

    // Adds the genotype.
    $genotype_snp = $line['genotype'];
    $details = array(
      'uniquename'  => $genotype_snp,
      'name'        => $genotype_snp,
      'type_id'     => $this->cvterms['sequence']['SNP'],
      'description' => $genotype_snp,
    );
    $genotype_id = $bc_genotype->addGenotype($this, $details);
    if ($genotype_id) {
      $project_id   = $this->_getID($ref_table, $program_id, $prefix, 'dataset', $line['dataset_name']);
      $stock_id     = $this->_getID($ref_table, $program_id, $prefix, 'accession', $line[strtolower($bims_cols['accession'])]);
      $feature_id   = $this->_getID($ref_table, $program_id, $prefix, 'marker', $line['marker_name']);
      $contact_id   = $this->_getID($ref_table, $program_id, $prefix, 'contact', $line['evaluator'], TRUE);

      // Checks for a duplication in genotype_call table and in this template.
      if (mcl_get_pg_version('minor') < 8) {
        $keys = array(
          'project_id'  => $project_id,
          'stock_id'    => $stock_id,
          'feature_id'  => $feature_id,
        );

        // Checks in genotype_call table.
        if ($bc_gc->getGenotypeCall($keys)) {
          return;
        }

        // Checks in this template.
        $dup_key = implode('.', $keys);
        if (array_key_exists($dup_key, $GLOBALS['dup_genotype'])) {
          return;
        }
        $GLOBALS['dup_genotype'][$dup_key] = TRUE;
      }

      // Adds genotype.
      $genotype_call_id = $bc_gc->getNextval('genotype_call');
      $insert_sql_gc = "EXECUTE bims_genotype_call_insert($genotype_call_id, $project_id, $stock_id, $feature_id, $contact_id, $genotype_id, '$genotype_snp');\n";
      file_put_contents($sql_gc, $insert_sql_gc, FILE_APPEND);

      // Adds the user-specific genotype properties.
      $insert_sql_gcp = $this->_addGenotypeCallProp($bc_gc, '##', $line, $genotype_call_id);
      if ($insert_sql_gcp) {
        file_put_contents($sql_gcp, $insert_sql_gcp, FILE_APPEND);
      }
    }
  }

  /**
   * Returns the ID of the given type in reference table. If not found,
   * gets ID and updates the table.
   *
   * @param array $ref_table
   * @param integer $program_id
   * @param string $prefix
   * @param string $type
   * @param string $value
   * @param bool $null_flag
   *
   * @return integer
   */
  private function _getID(&$ref_table, $program_id, $prefix, $type, $value, $null_flag = FALSE) {

    // Returns the ID for provide the type and the value.
    if (array_key_exists($value, $ref_table[$type])) {
      return $ref_table[$type][$value];
    }

    // Updates the reference table.
    if ($type == 'dataset') {
      $bc_project = new BIMS_CHADO_PROJECT($program_id);
      $dataset = $bc_project->byTKey('project', array('name' => $prefix . $value));
      if ($dataset) {
        $ref_table[$type][$value] = $dataset->project_id;
      }
      else if ($null_flag) {
        $ref_table[$type][$value] = 'NULL';
      }
    }
    else if ($type == 'accession') {
      $bc_accession = new BIMS_CHADO_ACCESSION($program_id);
      $accession = $bc_accession->byTKey('accession', array('uniquename' => $prefix . $value));
      if ($accession) {
        $ref_table[$type][$value] = $accession->stock_id;
      }
      else if ($null_flag) {
        $ref_table[$type][$value] = 'NULL';
      }
    }
    else if ($type == 'marker') {
      $bc_feature = new BIMS_CHADO_FEATURE($program_id);
      $marker = $bc_feature->byTKey('feature', array('uniquename' => $prefix . $value));
      if ($marker) {
        $ref_table[$type][$value] = $marker->feature_id;
      }
      else if ($null_flag) {
        $ref_table[$type][$value] = 'NULL';
      }
    }
    else if ($type == 'contact') {
      $contact = MCL_CHADO_CONTACT::byName($value);
      if ($contact) {
        $ref_table[$type][$value] = $contact->getContactID();
      }
      else if ($null_flag) {
        $ref_table[$type][$value] = 'NULL';
      }
    }
    return $ref_table[$type][$value];
  }

  /**
   * Adds genotype properties.
   *
   * @param BIMS_CHADO_GENOTYPE_CALL $bc_gc
   * @param string $prefix
   * @param array $line
   * @param integer $genotype_call_id
   *
   * @return string
   */
  private function _addGenotypeCallProp(BIMS_CHADO_GENOTYPE_CALL $bc_gc, $prefix, $line, $genotype_call_id) {

    $columns = $this->getJob()->getParamByKey($prefix);
    $sqls = '';
    foreach ((array)$columns as $cvterm_id => $cvterm_name) {
      $value = $line[$prefix . $cvterm_name];
      if ($value || $value == '0') {
        $value = pg_escape_string($value);

        // Checks for a duplication.
        $keys = array(
          'genotype_call_id'  => $genotype_call_id,
          'type_id'           => $cvterm_id,
        );
        if (mcl_get_pg_version('minor') < 8) {

          // Checks in genotype_callprop table.
          if ($bc_gc->getGenotypeCallprop($keys)) {
            continue;
          }

          // Checks in this template.
          $dup_key = implode('.', $keys);
          if (array_key_exists($dup_key, $GLOBALS['dup_genotypeprop'])) {
            return;
          }
          $GLOBALS['dup_genotypeprop'][$dup_key] = TRUE;
        }
        $genotype_callprop_id = $bc_gc->getNextval('genotype_callprop');
        $sqls .= "EXECUTE bims_genotype_callprop_insert($genotype_callprop_id, $genotype_call_id, $cvterm_id, '$value', 0);\n";
      }
    }
    return $sqls;
  }

  /**
   * @see MCL_TEMPLATE::preProcessECD()
   */
  public function preProcessECD() {

    // Initializes $GLOBALS.
    $GLOBALS['ref_table'] = array(
      'dataset'   => array(),
      'accession' => array(),
      'marker'    => array(),
      'contact'   => array(),
    );
    $GLOBALS['dup_genotype']      = array();
    $GLOBALS['dup_genotypeprop']  = array();
    return TRUE;
  }

  /**
   * @see MCL_TEMPLATE::mapVarColumn()
   */
  public function mapVarColumn($column) {

    // Gets the job paremters.
    $program_id = $this->getJob()->getParamByKey('program_id');

    // Checks the accession column.
    $bims_program = BIMS_PROGRAM::byID($program_id);
    if ($column == 'accession') {
      return $bims_program->getBIMSLabel($column, BIMS_LOWER);
    }
    return '';
  }

  /**
   * @see MCL_TEMPLATE::preProcessDU()
   */
  public function preProcessDU() {

    // Checks the reference table table.
    if (!array_key_exists('ref_table', $GLOBALS)) {
      $this->updateMsg('E', "Error : No reference table found");
      return FALSE;
    }

    // Gets the job and its paremters.
    $job = $this->getJob();
    $program_id = $job->getParamByKey('program_id');
    $cv_arr     = $job->getParamByKey('cv_arr');

    // Gets BIMS_CHADO tables.
    $bc_gc = new BIMS_CHADO_GENOTYPE_CALL($program_id);

    // Creates the sql files.
    $sql_file = $job->getJobDir() . '/' . $job->getJobID() . '-genotype_call.sql';
    if (!($fdw_gc = fopen($sql_file, 'w'))) {
      $this->updateMsg('E', "Error : Failed to open SQL file ($sql_file) for writing");
      return FALSE;
    }
    $sql_file = $job->getJobDir() . '/' . $job->getJobID() . '-genotype_callprop.sql';
    if (!($fdw_gcp = fopen($sql_file, 'w'))) {
      $this->updateMsg('E', "Error : Failed to open SQL file ($sql_file) for writing");
      return FALSE;
    }

    // Adds the prepare statements for genotype_call.
    $table_name = $bc_gc->getTableName('genotype_call');
    $prepare = "PREPARE bims_genotype_call_insert(int, int, int, int, int, int, text) AS INSERT INTO $table_name (genotype_call_id, project_id, stock_id, feature_id, contact_id, genotype_id, value) VALUES($1, $2, $3, $4, $5, $6, $7)";
    if (mcl_get_pg_version('minor') > 9) {
      $prepare .= ' ON CONFLICT DO NOTHING';
    }
    fputs($fdw_gc, "$prepare\n");
    fclose($fdw_gc);

    // Adds the prepare statements for genotype_callprop.
    $table_name = $bc_gc->getTableName('genotype_callprop');
    $prepare = "PREPARE bims_genotype_callprop_insert(int, int, int, text, int) AS INSERT INTO $table_name (genotype_callprop_id, genotype_call_id, type_id, value, rank) VALUES($1, $2, $3, $4, $5)";
    if (mcl_get_pg_version('minor') > 9) {
      $prepare .= ' ON CONFLICT DO NOTHING';
    }
    fputs($fdw_gcp, "$prepare\n");
    fclose($fdw_gcp);

    // Saves the user-specific properties.
    if (!$this->saveSpecialColumns('##', $cv_arr['genotype']['cv_id'], $this->getHeaders())) {
      $this->updateMsg('E', "Failed to save the special columns");
      return FALSE;
    }
    return TRUE;
  }

  /**
   * @see MCL_TEMPLATE::postProcessDU()
   */
  public function postProcessDU() {

    // Adds to genotype_call.
    $job = $this->getJob();
    $sql_file = $job->getJobDir() . '/' . $job->getJobID() . '-genotype_call.sql';
    if (file_exists($sql_file)) {
      if (!($fdr = fopen($sql_file, 'r'))) {
        return FALSE;
      }
      while ($sql_line = fgets($fdr)) {
        $sql_line = trim($sql_line);
        if ($sql_line) {
          db_query($sql_line);
        }
      }
      db_query("DEALLOCATE bims_genotype_call_insert");
      fclose($fdr);

      // Adds to genotype_callprop.
      $sql_file = $job->getJobDir() . '/' . $job->getJobID() . '-genotype_callprop.sql';
      if (file_exists($sql_file)) {
        if (!($fdr = fopen($sql_file, 'r'))) {
          return FALSE;
        }
        while ($sql_line = fgets($fdr)) {
          $sql_line = trim($sql_line);
          if ($sql_line) {
            db_query($sql_line);
          }
        }
        db_query("DEALLOCATE bims_genotype_callprop_insert");
      }
    }
    return TRUE;
  }

  /**
   * @see MCL_TEMPLATE::postProcessDU()
   */
  public function postProcessDU2() {

    // Adds to genotype_call.
    $job = $this->getJob();
    $sql_file = $job->getJobDir() . '/' . $job->getJobID() . '-genotype_call.sql';
    if (file_exists($sql_file)) {

      //$drush_binary = mcl_get_config_setting('mcl_drush_binary');
      //$db_str = mcl_get_db_string(FALSE);

      //print ">$db_url\n$sql_file\n\n";
      //exec("`$drush_binary sql-connect --db-url=$db_str ` < $sql_file");
//      $cmd = "psql $db_str < $sql_file";
  //    exec($cmd);
      //exec("$drush_binary sql-query --file=$sql_file --db-url=$db_url");
      file_put_contents($sql_file, 'DEALLOCATE bims_genotype_call_insert;', FILE_APPEND);
      Database::getConnection()->exec(file_get_contents($sql_file));
    }

    // Adds to genotype_callprop.
    $sql_file = $job->getJobDir() . '/' . $job->getJobID() . '-genotype_callprop.sql';
    if (file_exists($sql_file)) {
      file_put_contents($sql_file, 'DEALLOCATE bims_genotype_callprop_insert;', FILE_APPEND);
      Database::getConnection()->exec(file_get_contents($sql_file));
    }
    return TRUE;
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