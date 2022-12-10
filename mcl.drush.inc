<?php
/**
 * @file
 * Contains function relating to drush-integration of this module.
 */
/**
 * Global variables.
 */
$GLOBALS['verbose'] = TRUE;

/**
 * Describes each drush command implemented by the module.
 *
 * @return
 *   The first line of description when executing the help for a given command
 */
function mcl_drush_help($command) {
  switch ($command) {}
}

/**
 * Registers a drush command and constructs the full help for that command.
 *
 * @return
 *   And array of command descriptions
 */
function mcl_drush_command() {
  $items = array();
  $items['mcl-debug'] = array(
    'description' => t('Debug.'),
    'examples' => array(
      'Standard example' => 'drush mcl-debug',
    ),
  );
  $items['mcl-check-chado'] = array(
    'description' => t('Checks Chado schema'),
    'options' => array(
      'exclude' => t('Specify the regular expression of table name of the tables to be excluded'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-check-chado',
    ),
  );
  $items['mcl-check-cvterms'] = array(
    'description' => t('Checks requried cvterms'),
    'options' => array(
      'add' => t('Add the missing cvterms'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-check-cvterms --add',
    ),
  );
  $items['mcl-dump-schema'] = array(
    'description' => t('Dumps a database schema.'),
    'arguments' => array(
      'schema' => t('The schema name.'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-dump-schema [schema]',
    ),
    'options' => array(
      'working_dir' => t('Specifies the working directory.'),
    ),
    'aliases' => array('mcl-dump'),
  );
  $items['mcl-get-markers'] = array(
    'description' => t('Search markers in feature table by various type.'),
    'arguments' => array(
      'filepath'  => t('The filepath.'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-get-markers [filepath]',
    ),
  );
  $items['mcl-search-cv'] = array(
    'description' => t('Search cv by name or ID.'),
    'arguments' => array(
      'name'  => t('The cv name.'),
    ),
    'options' => array(
      'id' => t('cv ID.'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-search-cv [12345] --id',
    ),
  );
  $items['mcl-search-cvterm'] = array(
    'description' => t('Search cvterm by name or ID.'),
    'arguments' => array(
      'name'  => t('The cvterm name.'),
    ),
    'options' => array(
      'id' => t('cvterm ID.'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-search-cvterm [12345] --id',
    ),
  );
  $items['mcl-process-file'] = array(
    'description' => t('Process a file.'),
    'arguments' => array(
      'filepath'  => t('The filepath.'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-process-file [filepath]',
    ),
    'options' => array(
      'out' => t('Create an output file.'),
    ),
  );
  $items['mcl-table-info'] = array(
    'description' => 'Displays the information of the table',
    'arguments' => array(
      'table_name'  => t('The name of a table (no prefix needed for the tables in public schema). (e.g. user, chado.contact)'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-table-info [table_name]',
    ),
  );
  $items['mcl-run'] = array(
    'description' => t('Runs job in mcl_job table.'),
    'examples' => array(
      'Standard example' => 'drush mcl-run',
    ),
  );
  $items['mcl-upload-data'] = array(
    'description' => t('Uploads data file.'),
    'arguments'   => array(
      'username'  => t('Drupal user name.'),
      'filepath'  => t('The filename of the uploading file with path.'),
    ),
    'options' => array(
      'working_dir'     => t('Specifies the working directory.'),
      'transaction_on'  => t('Turns the transaction on.'),
      'transaction_off' => t('Turns the transaction off.'),
      'job_name'        => t('Specifies the name of a job.'),
      'move_input'      => t('Moves the input file to the file folder in the job directory.'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-upload-data [username] [filepath] --working_dir=/yourpath',
    ),
  );
  $items['mcl-rerun-job'] = array(
    'description' => t('Re-Run the uploading job.'),
    'arguments' => array(
      'job_id'    => t('The job ID.'),
      'filepath'  => t('The filename of the uploading file with path.'),
    ),
    'options' => array(
      'force'           => t('Forces to re-run the passed steps.'),
      'move_input'      => t('Moves the input file to the file folder in the job directory.'),
      'transaction_on'  => t('Turns the transaction on.'),
      'transaction_off' => t('Turns the transaction off.'),
      'status'          => t('The status to start with.'),
      'working_dir'     => t('Specifies the working directory.'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-rerun-job [job_id] [filepath] --status=0 --force --working_dir=/yourpath',
    ),
  );
  $items['mcl-upload-test-data'] = array(
    'description' => t('Uploading the test data.'),
    'arguments' => array(
      'username'  => t('The user name.'),
    ),
    'options' => array(),
    'examples' => array(
      'Standard example' => 'drush mcl-upload-test-data <username>',
    ),
  );
  $items['mcl-update-test-data'] = array(
    'description' => t('Updating the test data.'),
    'arguments' => array(
      'username'  => t('The user name.'),
    ),
    'options' => array(),
    'examples' => array(
      'Standard example' => 'drush mcl-update-test-data <username>',
    ),
  );
  $items['mcl-delete-test-data'] = array(
    'description' => t('Deleting the test data.'),
    'arguments' => array(),
    'options' => array(),
    'examples' => array(
      'Standard example' => 'drush mcl-delete-test-data',
    ),
  );
  $items['mcl-get-template'] = array(
    'description' => t('Generates a template in Excel format.'),
    'arguments' => array(
      'template'  => t('The template name.'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-get-template [template]',
    ),
  );
  $items['mcl-update-template-types'] = array(
    'description' => t('Updates the template types'),
    'examples' => array(
      'Standard example' => 'drush mcl-update-template-types',
    ),
  );
  $items['mcl-update-templates'] = array(
    'description' => t('Updates the templates'),
    'examples' => array(
      'Standard example' => 'drush mcl-update-templates',
    ),
  );
  $items['mcl-submit-uploading-job'] = array(
    'description' => t('Submits an uploading job.'),
    'arguments' => array(
      'username'  => t('Drupal user name.'),
      'filepath'  => t('The filename of the uploading file with path.'),
    ),
    'options' => array(
      'working_dir' => t('Specify the working directory.'),
      'move_input'  => t('Moves the input file to the file folder in the job directory.'),
      'param'       => t('Adds the job parameter (JSON format).'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-submit-uploading-job [username] [filepath] --working_dir=/your_path',
    ),
  );
  $items['mcl-list-jobs'] = array(
    'description' => t('Views a list of your jobs.'),
    'arguments' => array(
      'username' => t('Drupal user name.'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-list-jobs [username]',
    ),
  );
  $items['mcl-job-info'] = array(
    'description' => t('Shows the information about a job.'),
    'arguments' => array(
      'job_id'    => t('The Job ID.'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-job-info [job_id]',
    ),
  );
  $items['mcl-change-job-owner'] = array(
    'description' => t('Change the owner of the job. The job directory will be moved to the new owner.'),
    'arguments' => array(
      'job_id'    => t('The Job ID.'),
      'username' => t('Drupal user name.'),
    ),
    'examples' => array(
      'Standard example' => 'mcl-change-job-owner [username] [job_id]',
    ),
  );
  $items['mcl-delete-job'] = array(
    'description' => t('Deletes a job.'),
    'arguments' => array(
      'job_id' => t('The Job ID of the job to be deleted.'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-delete-job [job_id]',
    ),
  );
  $items['mcl-delete-jobs'] = array(
    'description' => t('Deletes jobs.'),
    'arguments' => array(
      'job_ids' => t('Specify Job ID by the range.'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-delete-jobs 4-20',
    ),
  );
  $items['mcl-merge-test'] = array(
    'description' => t('Performs merge test on the given type and IDs.'),
    'arguments' => array(
      'type' => t('The type of data to be merged. (contact, stock, marker etc..)'),
      'id_base' => t('ID of the base data'),
      'id' => t('ID of the data to be merged (removed)'),
    ),
    'options' => array(
      'verbose' => t('verbose flag'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-merge-test [type] [id] [id_target]',
    ),
  );
  $items['mcl-generate-class'] = array(
    'description' => t('Generate class files.'),
    'arguments' => array(
      'module'      => t('The target module.'),
      'schema'      => t('The schema name.'),
      'dump_file'   => t('The filename of the Postgresql dump file. default : dupal.<schema>.sql'),
      'table_file'  => t('The filename of the list of tables. default : filter.drupal.<schema>.<module>.txt'),
    ),
    'options' => array(
      'all'         => t('All tables in a schema. Adds the regular expression of table name of the tables to be excluded followed by a equal sign.'),
      'namespace'   => t('Specifies the namespace.'),
      'extends'     => t('Specifies the extends.'),
      'move'        => t('Move the class files to the module directory.'),
      'working_dir' => t('Specifies the working directory.  default : <module_path>/schema'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-generate-class [module] [schema] [dump_file] [table_file] --working_dir=/yourpath',
    ),
    'aliases' => array('mcl-class'),
  );
  $items['mcl-add-cv'] = array(
    'description' => t('Add a cv.'),
    'arguments' => array(
      'cv_name' => t('The cv name.'),
    ),
    'options' => array(
      'def' => t('The definition of the cv'),
    ),
    'examples' => array(
      'Standard example' => 'mcl-add-cv [cv_name] --def=<definition>',
    ),
  );
  $items['mcl-add-cvterm'] = array(
    'description' => t('Add a cvterm.'),
    'arguments' => array(
      'cvterm_name' => t('The cvterm name.'),
      'cv_name' => t('The cv name.'),
    ),
    'options' => array(
      'def' => t('The definition of the cvterm'),
    ),
    'examples' => array(
      'Standard example' => 'mcl-add-cvterm [cvterm_name] [cv_name] --def=<definition>',
    ),
  );
  $items['mcl-add-cvtermprop'] = array(
    'description' => t('Add a cvtermproip.'),
    'arguments' => array(
      'cvterm_id' => t('The cvterm ID.'),
      'cvterm_name' => t('The cvterm name.'),
      'cv_name' => t('The cv name.'),
      'value' => t('The value of a property.'),
    ),
    'examples' => array(
      'Standard example' => 'mcl-add-cvtermprop [cvterm_id] [cvterm_name] [cv_name] [value]',
    ),
  );
  $items['mcl-add-cvterm-by-template'] = array(
    'description' => t('Add all the cvterms that required by the given template name.'),
    'arguments' => array(
      'cvterm_name' => t('The template name.'),
      'cv_name' => t('The cv name.'),
    ),
    'options' => array(
      'upload' => t('Add this option to upload the missing cvterms'),
    ),
    'examples' => array(
      'Standard example' => 'mcl-add-cvterm-by-template [template_name] [cv_name]',
    ),
  );
  $items['mcl-add-organism'] = array(
    'description' => t('Add an organism.'),
    'arguments' => array(
      'genus' => t('The genus of the organism.'),
      'species' => t('The species of the organism.'),
    ),
    'options' => array(
      'common' => t('The common name of the organism'),
    ),
    'examples' => array(
      'Standard example' => 'mcl-add-organism [genus] [species] --common=<common name>',
    ),
  );

  $items['mcl-add-field-book-ontology'] = array(
    'description' => t('Add Field Book ontology.'),
    'examples' => array(
      'Standard example' => 'mcl-add-field-book-ontology',
    ),
  );

  $items['mcl-add-data-group'] = array(
    'description' => t('Add a data group and members.'),
    'arguments' => array(
      'group' => t('The name of data group.'),
    ),
    'options' => array(
      'description' => t('The description of the data group'),
      'member' => t('List of members separated by semi-colons and Tuple (type, key and value) is separted by colons)'),
    ),
    'examples' => array(
      'Standard example' => 'mcl-add-data-group [group] --members=<type1:key1:value1>:<type2:key2:value2>',
    ),
  );
  $items['mcl-delete-cvterm'] = array(
    'description' => t('Delete a cvterm.'),
    'arguments' => array(
      'cvterm_name' => t('The cvterm name.'),
      'cv_name' => t('The cv name.'),
    ),
    'examples' => array(
      'Standard example' => 'mcl-delete-cvterm [cvterm_name] [cv_name]',
    ),
  );
  $items['mcl-delete-cvterm-by-id'] = array(
    'description' => t('Delete a cvterm by ID.'),
    'arguments' => array(
      'cvterm_id' => t('The cvterm ID.'),
    ),
    'examples' => array(
      'Standard example' => 'mcl-delete-cvterm-by-id [cvterm_id]',
    ),
  );
  $items['mcl-delete-project'] = array(
    'description' => t('Delete a project by ID or name.'),
    'arguments' => array(
      'project_id' => t('The project ID.'),
    ),
    'options' => array(
      'name' => 'delete proejct by a project name',
    ),
    'examples' => array(
      'Standard example' => 'mcl-delete-project-by-id [project_id]',
    ),
  );
  $items['mcl-delete-phenotype-data-by-projects'] = array(
    'description' => t('Delete phenotyping data by projects.'),
    'arguments' => array(
      'project_ids' => t('The project IDs separated by colons.'),
    ),
    'options' => array(
      'delete' => t('Deletes data'),
      'project' => t('Deletes projects after deleting phenotyping data.'),
      'nd_geolocation' => t('Deletes nd_geolocation.'),
      'descriptor' => t('Deletes descriptor.'),
    ),
    'examples' => array(
      'Standard example' => 'mcl-delete-phenotype-data-by-projects [project_ids] --project --nd_geolocation --descriptor',
    ),
  );
  $items['mcl-delete-phenotype-data-by-cv'] = array(
    'description' => t('Delete phenotyping data by cv.'),
    'arguments' => array(
      'cv_name' => t('The cv name.'),
    ),
    'options' => array(
      'delete' => t('Deletes data'),
      'nd_geolocation' => t('Deletes nd_geolocation.'),
      'descriptor' => t('Deletes descriptor.'),
    ),
    'examples' => array(
      'Standard example' => 'mcl-delete-phenotype-data-by-projects [project_ids] --project --nd_geolocation --descriptor',
    ),
  );
  $items['mcl-list-cvterm'] = array(
    'description' => t('List all cvterms of the give cv name.'),
    'arguments' => array(
      'cv_name' => t('The cv name.'),
    ),
    'examples' => array(
      'Standard example' => 'mcl-list-cvterm [cv_name]',
    ),
  );
  $items['mcl-list-project'] = array(
    'description' => t('List all projects with type and sub-type.'),
    'examples' => array(
      'Standard example' => 'mcl-list-project',
    ),
  );
  $items['mcl-list-assoc-data'] = array(
    'description' => t('List all associated data.'),
    'arguments' => array(
      'type'        => t('The type of the data (stock, feature etc..).'),
      'primary_key' => t('The primary key of the data (stock_id, feature_id etc..).'),
      'primary_id'  => t('The primary ID of the data.'),
    ),
    'examples' => array(
      'Standard example' => 'mcl-list-assoc-data stock <stock_name> --organism_id=<organism_id>',
    ),
  );
  $items['mcl-list-assoc-data-by-file'] = array(
    'description' => t('List all associated data in a CSV file.'),
    'arguments' => array(
      'filepath' => t('The filepath.'),
    ),
    'examples' => array(
      'Standard example' => 'mcl-list-assoc-data-by-file <filepath>',
    ),
  );
  $items['mcl-fix-date'] = array(
    'description' => t('Fix date (integer to date).'),
    'arguments' => array(
      'cv_name'     => t('The cv name.'),
      'cvterm_name' => t('The cvterm name.'),
      'table_name'  => t('The table name.'),
    ),
    'options' => array(),
    'examples' => array(
      'Standard example' => 'drush mcl-fix-date [cv_name] [cvterm_name] [table_name]',
    ),
  );
  $items['mcl-fix-data'] = array(
    'description' => t('Fix data.'),
    'arguments' => array(
      'inc_file'   => t('The include filename.'),
      'input_file' => t('The input filename.'),
    ),
    'options' => array(),
    'examples' => array(
      'Standard example' => 'drush mcl-fix-data [input_file]',
    ),
  );
  $items['mcl-call-func'] = array(
    'description' => t('Call the provided function name.'),
    'arguments' => array(
      'function_name' => t('The function name.'),
    ),
    'options' => array(
      'param' => t('The parameters of calling function in JSON format.'),
    ),
    'examples' => array(
       'Standard example' => 'drush mcl-call-func [function_name] --param=<parameters in JSON>',
    ),
  );
  $items['mcl-run-cron'] = array(
    'description' => dt('Runs the cron job manually.'),
    'arguments' => array(
      'type' => dt('The type of cron job (hour, day or week).'),
    ),
    'examples' => array(
      'Standard example' => 'drush do-run-cron [hour]',
    ),
  );
  return $items;
}

/**
 * Callback for dump-schema command.
 */
function drush_mcl_dump_schema($schema) {

  // Checks schema.
  mcl_check_schema($schema);

  // Sets the target file.
  $target_file = drupal_get_path('module', 'mcl') . "/schema/drupal.$schema.sql";

  // Sets the command line.
  $cmd = "pg_dump -U postgres -h localhost --no-owner --schema=$schema --schema-only drupal7 > $target_file";
  mcl_print("Execute the following command:", 1);
  mcl_print(">$cmd", 2);
  exec($cmd);
  mcl_print("DONE", 1, 3);
}

/**
 * Callback for mcl-check-chado.
 */
function drush_mcl_check_chado() {

  // Gets the option.
  $exclude = drush_get_option('exclude');

  // Checks Chado schema.
  mcl_check_schema('chado');

  // Gets all the tables from information_schema table.
  $all_tables = mcl_get_all_tables('chado');
  if (empty($all_tables)) {
    mcl_print("Error : There no table exists in Chado schema", 1, 3);
    exit;
  }

  // Gets the non-chado tables.
  $non_chado = CHADO_TABLE::getNonChadoTables();

  // Sets the regular expression to exclude tables.
  $regex = $exclude ? '/^(' . $exclude . ')/' : '';

  // Checks if all tables have the class files.
  mcl_print("Checking class name for the tables", 1, 2.2);
  $skipped = array();
  $class_names = array();
  foreach ($all_tables as $table_name) {

    // Skips the tables that matches with 'exclude'.
    if ($regex && preg_match($regex, $table_name)) {
      $skipped []= "\t  - $table_name";
      continue;
    }

    // Skips the non-chado tables.
    if (in_array($table_name, $non_chado)) {
      continue;
    }

    // Checks the class file for the table.
    $class_name = strtoupper('CHADO_' . $table_name);
    if (!class_exists($class_name)) {
      mcl_print("Error : The class file of '$table_name' does not exist", 1, 3);
      exit;
    }
    $class_names[$class_name] = $table_name;
  }
  if (!empty($skipped)) {
    $msg = "The following tables does not have class.\n\n";
    $msg .= implode("\n", $skipped);
    mcl_print($msg, 1, 1);
  }

  // Checks the unique constraints.
  mcl_print("Checking unique key constraint of the tables", 1, 1.2);
  foreach ($class_names as $class_name => $table_name) {
    $ukey = $class_name::getUK();
    if (empty($ukey)) {
      mcl_print("Error : '$table_name' does not have unique key", 1, 1);
    }
  }
  mcl_print("DONE", 1, 3);
}

/**
 * Callback for mcl-check-cvterms.
 */
function drush_mcl_check_cvterms() {

  // Gets the option.
  $opt_add = drush_get_option('add');

  // Gets all cvterms from the templates.
  $mcl_templates = MCL_TEMPLATE::getTemplates(TRUE);
  $all_cvterms = array();
  foreach ($mcl_templates as $mcl_template) {
    $cvterms = $mcl_template->defineCvterms();
    foreach ((array)$cvterms as $cv_name => $cvterm_names) {
      if (!array_key_exists($cv_name, $all_cvterms)) {
        $all_cvterms[$cv_name] = array();
      }
      foreach ((array)$cvterm_names as $cvterm_name => $num) {
        if (!array_key_exists($cvterm_name, $all_cvterms[$cv_name])) {
          $all_cvterms[$cv_name][$cvterm_name] = 0;
        }
        $all_cvterms[$cv_name][$cvterm_name]++;
      }
    }
  }

  // Checks all cvterms.
  $transaction = db_transaction();
  try {
    $missing_cvterms = array();
    foreach((array)$all_cvterms as $cv_name => $cvterms) {

      // Checks the cv.
     $cv = MCL_CHADO_CV::getCV($cv_name);
      if (!$cv) {
        throw new Exception("Error : Error : cv name '$cv_name' does not exist.");
      }
      foreach ((array)$cvterms as $cvterm_name => $num) {
        $num_cvterms++;

        // Checks the cvterm.
        $cvterm = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name);

        // Adds the cvterm.
        if (!$cvterm) {
          if ($opt_add) {
            $cvterm = MCL_CHADO_CVTERM::addCvterm(NULL, 'SITE_DB', $cv_name, $cvterm_name);
            if ($cvterm) {
              mcl_print("$cvterm_name [$cv_name] has been added", 1, 1, '', FALSE);
            }
            else {
              throw new Exception("Error : Fail to add the cvterm ($cvterm_name)");
            }
          }
          else {
            $missing_cvterms []= "$cvterm_name [$cv_name]";
          }
        }
      }
    }
  }
  catch (Exception $e) {
    $transaction->rollback();
    mcl_print("Error : " . $e->getMessage(), 1, 3);
    exit;
  }

  // Shows the missing cvterms.
  $miss_count = sizeof($missing_cvterms);
  if ($miss_count) {
    mcl_print("List of the missing cvterms", 1, 2.2);
    foreach ($missing_cvterms as $info) {
      mcl_print($info, 1, 1, '', FALSE);
    }
    mcl_print("$miss_count cvterms are missing", 1, 2.3);
  }
  else {
    mcl_print("All the required cvterms ($num_cvterms) exists in chado.cvterm table.", 1, 2.3);
  }
}

/**
 * Callback for mcl-table-info.
 */
function drush_mcl_table_info($table_name) {

  // Sets the schema and table name.
  $schema = 'public';
  if (preg_match("/^(.*?)\.(.*)$/", $table_name, $matches)) {
    $schema     = $matches[1];
    $table_name = $matches[2];
  }

  // Checks the table.
  $table = $schema . '.' . $table_name;
  if (!db_table_exists($table)) {
    mcl_print("The table '$table' does not exist.", 1, 1.2);
    exit;
  }

  // Checks the table class.
  $class_name = strtoupper($schema . '_' . $table_name);
  if (!class_exists($class_name)) {
    mcl_print("The table class for '$class_name' does not exist.", 1, 1.2);
    exit;
  }

  // Shows the table info.
  $table_info = mcl_get_table_info($schema, $table_name);
  mcl_print($table_info, 0, 2, FALSE);
}

/**
 * Callback for mcl-run.
 */
function drush_mcl_run() {

  // Shows job type if specifed.
  mcl_print("Running uploading job in queue.", 1, 2);

  // Launches jobs.
  $manager = new MCL_JOB_MANAGER();
  $manager->processJobs('upload');
  mcl_print("DONE", 1, 3);
}

/**
 * Callback for mcl-rerun-job command.
 */
function drush_mcl_rerun_job($job_id, $filepath = '') {

  // Checks the job ID.
  $mcl_job = mcl_check_job_id($job_id);

  // Sets the filepath if not provided.
  if ($filepath == '') {
    $mcl_file = mcl_check_file_id($mcl_job->getFileID());
    $filepath = $mcl_file->getFilepath();
  }

  // Checks the filepath.
  mcl_check_filepath($filepath);

  // Updates the file path in case of file path and name changed.
  $mcl_file_id = $mcl_job->getParamByKey('file_id');
  $mcl_file = MCL_FILE::byKey(array('file_id' => $mcl_file_id));
  if (!$mcl_file) {
    mcl_print("Error : Failed to get MCL_FILE ($mcl_file_id)", 1, 3);
  }
  if (!$mcl_file->updateFileInfo($filepath)) {
    mcl_print("Error : Failed to update file information for '$filepath'", 1, 3);
  }

  // Gets and sets process ID.
  $pid = getmypid();
  $mcl_job->setPID($pid);

  // Gets ans sets the last run date.
  $mcl_job->setPropByKey('last_run', date("Y-m-d G:i:s"));

  // Checks the status option.
  $status = drush_get_option('status');
  if ($status) {
    if (is_numeric($status)) {

      // Sets the status.
      $mcl_job->setStatus($status);
    }
    else {
      mcl_print("Error : The status ($status) is not numeric", 1, 3);
      exit;
    }
  }

  // Checks the force option.
  $force = drush_get_option('force');
  $force = ($force) ? TRUE : FALSE;
  $mcl_job->setForce($force);

  // Checks the move_input option.
  $move_input = drush_get_option('move_input');
  $move_input = ($move_input) ? TRUE : FALSE;

  // Checks the transaction option.
  $transaction_on   = drush_get_option('transaction_on');
  $transaction_off  = drush_get_option('transaction_off');
  if ($transaction_on || $transaction_off) {
    $trans = ($transaction_on) ? TRUE : FALSE;
    $mcl_job->setPropByKey('transaction', $trans);
  }

  // Checks the working_dir option.
  $working_dir = drush_get_option('working_dir');
  if ($working_dir) {
    if (!mcl_check_working_dir($working_dir)) {
      exit;
    }
    $mcl_job->changeWorkingDir($working_dir);
  }


  // Overwrites the input file.
  if ($filepath) {

    // Gets the new filepath.
    $new_filepath = $mcl_job->getJobDir() . '/files/' . basename($filepath);

    // Moves the file.
    if ($filepath != $new_filepath) {
      $cmd = ($move_input) ? "mv \"$filepath\" \"$new_filepath\"" : "cp \"$filepath\" \"$new_filepath\"";
      exec($cmd, $return_var);
      if (!empty($return_var)) {
        mcl_print("Error : Fail to overwrite the file", 1, 2);
        mcl_print($cmd, 1, 3);
        exit;
      }
    }
    else {
      mcl_print("The target and destination files are the same so use the current one", 1, 2);
    }

    // Updates the file information.
    $file_id = $mcl_job->getParamByKey('file_id');
    $mcl_file = MCL_FILE::byKey(array('file_id' => $file_id));
    $mcl_file->updateFileInfo($new_filepath);
    $mcl_job->setUploadFile($mcl_file);
  }

  // Sets the status to be 0 if
  // - status is negative.
  // - filepath is not empty.
  if ($mcl_job->getStatus() < 0 || $filepath) {
    $mcl_job->setStatus(0);
  }

  // Gets process ID.
  $pid = getmypid();
  $mcl_job->setPID($pid);
  print "\tProcess ID = $pid\n\n";

  // Updates all changes of the job properties.
  $mcl_job->update();

  // Launches the job.
  mcl_print("Re-running the job ($job_id)", 1, 2);
  $manager = new MCL_JOB_MANAGER();
  $status = $manager->processJob($mcl_job);
  if ($status) {
    mcl_print("DONE : The uploading job was completed without an error.", 1, 4);
  }
  else {
    $job_id = $mcl_job->getJobID();
    $opt_working_dir = '--working_dir=' . $mcl_job->getWorkingDir();
    mcl_print("DONE : The job (Job ID = $job_id) was completed with some errors", 1, 2);
    mcl_print("Please fix the errors and re-run this job", 2, 2);
    mcl_print(">drush mcl-rerun-job $job_id $filepath $opt_working_dir", 2, 4, FALSE);
  }
}

/**
 * Callback for mcl-list-jobs.
 */
function drush_mcl_list_jobs($username) {

  // Checks the username.
  $mcl_user = mcl_check_username($username);

  // Gets all jobs that belong to this user.
  $manager = new MCL_JOB_MANAGER();
  $jobs = $manager->getJobs(array('user_id' => $mcl_user->getUserID()));
  $job_list = '';
  foreach ($jobs as $job) {
    $job_id = $job->getJobID();
    $name   = $job->getName();
    $status = $job->getStatus();
    $job_list .= "\t[$job_id] $name at $status\n";
  }

  // Print out the job list.
  if ($job_list) {
    mcl_print("List jobs for $username", 1);
    mcl_print("==================================", 1);
    mcl_print($job_list, 1);
    mcl_print("----------------------------------", 1, 3);
  }
  else {
    mcl_print("No job found for $username", 1, 3);
  }
}

/**
 * Callback for mcl-change-job-owner.
 */
function drush_mcl_change_job_owner($username, $job_id) {

  // Checks the username.
  $mcl_user = mcl_check_username($username);

  // Checks the job ID.
  $mcl_job = mcl_check_job_id($job_id);
  $old_username = $mcl_job->getUsername();

  // Change the owner of the job.
  if ($mcl_job->changeOwner(TRUE)) {
    mcl_print("The owner has been change '$old_username' to '$username'", 1, 3);
  }
  else {
    mcl_print("Error : Failed to change the owner of the job", 1, 3);
  }
}

/**
 * Callback for mcl-job-info.
 */
function drush_mcl_job_info($job_id) {

  // Checks the job ID.
  $mcl_job = mcl_check_job_id($job_id);

  // Gets and shows the job information.
  $info = $mcl_job->getJobInfo();
  mcl_print("Job Information", 1, 2);
  foreach ($info as $key => $value) {
    printf("%s : %s\n", $key, $value);
  }
  mcl_print("----------------", 1, 3);
}

/**
 * Callback for mcl-delete-job.
 */
function drush_mcl_delete_job($job_id) {

  // Checks the job ID.
  $mcl_job = mcl_check_job_id($job_id);

  // Deletes the job.
  $mcl_job = MCL_JOB::byKey(array('job_id' => $job_id));
  if ($mcl_job->delete()) {
    $msg =  "Job has been deleted";
  }
  else {
    $msg = "Fail to delete the job";
  }
  mcl_print($msg, 1, 3);
}

/**
 * Callback for mcl-delete-jobs.
 */
function drush_mcl_delete_jobs($job_ids) {

  // Checks arguments.
  if (preg_match("/(\d+)-(\d+)/", $job_ids, $matches)) {
    $start  = $matches[1];
    $stop   = $matches[2];

    // Deletes jobs.
    for ($job_id = $start; $job_id <= $stop; $job_id++) {
      $mcl_job = MCL_JOB::byKey(array('job_id' => $job_id));
      if ($mcl_job) {
        if ($mcl_job->delete()) {
          $msg =  "Job ID ($job_id) has been deleted";
        }
        else {
          $msg = "Fail to delete the job";
        }
        mcl_print($msg, 1, 3);
      }
      else {
        mcl_print("Job ID ($job_id) not exist", 1, 3);
      }
    }
  }
  else {
    mcl_print("Error : Please check arguments", 1, 3);
    exit;
  }
}

/**
 * Callback for mcl-add-cv.
 */
function drush_mcl_add_cv($cv_name) {

  // Gets the cv.
  $mcl_cv = MCL_CHADO_CV::getCV($cv_name);
  if ($mcl_cv) {
    $cv_id = $mcl_cv->getCvID();
    mcl_print("The cv '$cv_name' ($cv_id) has already existed", 1, 3);
    return;
  }

  // Gets the definition if exists.
  $definition = drush_get_option('def');

  // Add a new cv.
  $transaction = db_transaction();
  try {
    $mcl_cv = MCL_CHADO_CV::addCV(NULL, $cv_name, $definition);
    if (!$mcl_cv) {
      throw new Exception("Fail to add the cv ($cv_name)");
    }
    $cv_id = $mcl_cv->getCvID();
    mcl_print("The cv '$cv_name' ($cv_id) has been added", 1, 3);
  }
  catch (Exception $e) {
    $transaction->rollback();
    mcl_print("Error : " . $e->getMessage(), 1, 3);
    exit;
  }
  mcl_print("DONE", 1, 3);
}

/**
 * Callback for mcl-add-cvterm.
 */
function drush_mcl_add_cvterm($cvterm_name, $cv_name = 'SITE_CV') {

  // Check the cvterm.
  $check = MCL_CHADO_CVTERM::checkCvterm(NULL, $cv_name, $cvterm_name);
  if ($check) {
    mcl_print("The cvterm '$cvterm_name' ($cv_name) exists", 1, 2.3);
    return;
  }

  // Gets the definition if exists.
  $definition = drush_get_option('def');

  // Add a new cvterm.
  $transaction = db_transaction();
  try {
    $cvterm = MCL_CHADO_CVTERM::addCvterm(NULL, 'SITE_DB', $cv_name, $cvterm_name, $definition);
    if (!$cvterm) {
      throw new Exception("Fail to add the cvterm ($cvterm_name)");
    }
    $cvterm_id = $cvterm->getCvtermID();
    mcl_print("The cvterm '$cvterm_name' ($cvterm_id) has been added", 1, 2.3);
  }
  catch (Exception $e) {
    $transaction->rollback();
    mcl_print("Error : " . $e->getMessage(), 1, 3);
    exit;
  }
  mcl_print("DONE", 1, 3);
}

/**
 * Callback for mcl-add-cvtermprop.
 */
function drush_mcl_add_cvtermprop($cvterm_id, $cvterm_name, $cv_name = 'SITE_CV', $value) {

  // Checks the cvterm_id.
  $cvterm = MCL_CHADO_CVTERM::byID($cvterm_id);
  if (!$cvterm) {
    mcl_print("Error : The cvterm '$cvterm_id' does not exist", 1, 2.3);
    exit;
  }
  $name = $cvterm->getName();

  // Check the cvterm of the type.
  $type = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name);
  if (!$type) {
    mcl_print("Error : The type '$cvterm_name' ($cv_name) does not exist", 1, 2.3);
    exit;
  }
  $type_id = $type->getCvtermID();

  // Checks for duplication.
  $keys = array(
    'cvterm_id' => $cvterm_id,
    'type_id'   => $type_id,
  );
  $cvtermprop = CHADO_CVTERMPROP::byKey($keys);
  if ($cvtermprop) {
    mcl_print("The cvtermprop (cvterm_id, type_id) = ($cvterm_id, $type_id) exists in chado.cvtermprop table.", 1, 2.3);
    return;
  }

  // Add a new cvtermprop.
  $transaction = db_transaction();
  try {
    if (!$cvterm->addProp(NULL, $cv_name, $cvterm_name, $value)) {
        throw new Exception("Fail to add the cvtermprop");
      }
      mcl_print("The value '$value' has been add to the cvterm ($name) as a property", 1, 2.3);
  }
  catch (Exception $e) {
    $transaction->rollback();
    mcl_print("Error : " . $e->getMessage(), 1, 3);
    exit;
  }
  mcl_print("DONE", 1, 3);
}

/**
 * Callback for mcl-add-cvterm-by-template.
 */
function drush_mcl_add_cvterm_by_template($template_name, $cv_name = 'SITE_CV') {

  // Checks the name of template.
  $mcl_template = MCL_TEMPLATE::getByTemplate($template_name);
  if (!$mcl_template) {
    mcl_print("Error : The template '$template_name' does not exist", 1, 2.3);
    exit;
  }

  // Gets the options.
  $upload_flag = drush_get_option('upload') ? TRUE : FALSE;

  // Gets the site CV name.
  $site_cv_name = MCL_SITE_VAR::getValueByName('SITE_CV');

  // Adds all the cvterms required by this template.
  $cvterms = $mcl_template->defineCvterms();
  $missing_cvterms = array();
  mcl_print("List of the cvterms required by '$template_name'", 1, 2);
  foreach ((array)$cvterms as $cv_name => $cvterms) {
    foreach ((array)$cvterms as $cvterm_name => $val) {
      $cvterm = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name);
      $prefix = '[X]';
      if (!$cvterm) {
        $prefix = '[ ]';
        $missing_cvterms[$cvterm_name] = $cv_name;

      }
      mcl_print("$prefix $cv_name : $cvterm_name", 1);
    }
  }

  // Adds cvterms.
  if (empty($missing_cvterms)) {
    mcl_print("'$template_name' does not need to add cvterms", 1, 2.2);
  }
  else {
    if ($upload_flag) {
      mcl_print("The following cvterms will be added", 1, 2.2);
      foreach ($missing_cvterms as $cvterm_name => $cv_name) {
        if ($cv_name == $site_cv_name || $cv_name == 'SITE_CV') {
          drush_mcl_add_cvterm($cvterm_name, $cv_name);
        }
        else {
          mcl_print("MCL cannot add '$cvterm_name' for $cv_name ontology. Please add manually", 1, 2);
        }
      }
    }
    else {
      mcl_print("'$template_name' needs the following cvterms", 1, 2.2);
      foreach ($missing_cvterms as $cvterm_name => $cv_name) {
         mcl_print("$cv_name : $cvterm_name", 1);
      }
    }
  }
  mcl_print("DONE", 1, 2.3);
}

/**
 * Callback for mcl-add-organism.
 */
function drush_mcl_add_organism($genus, $species) {

  // Gets the options.
  $common = drush_get_option('common');

  // Check the genus and species.
  if (!$genus || !$species) {
    mcl_print("Error : Please specify the genus ($genus) and species ($species) of the organism", 1, 3);
    exit;
  }

  // Checks the organism.
  $check = MCL_CHADO_ORGANISM::checkOrganism(NULL, $genus, $species);
  if ($check) {
    mcl_print("The organism '$genus' '$species' exists", 1, 2.3);
    return;
  }

  // Adds a new organism.
  $transaction = db_transaction();
  try {
    $organism = MCL_CHADO_ORGANISM::addOrganism(NULL, $genus, $species);
    if (!$organism) {
      throw new Exception("Fail to add the organism ($genus $species)");
    }
    $organism_id = $organism->getOrganismID();
    mcl_print("The organism '$genus $species' ($organism_id) has been added", 1, 2.3);
  }
  catch (Exception $e) {
    $transaction->rollback();
    mcl_print("Error : " . $e->getMessage(), 1, 3);
    exit;
  }
  mcl_print("DONE", 1, 3);
}

/**
 * Callback for mcl-add-field-book-ontology.
 */
function drush_mcl_add_field_book_ontology() {

  // Adds field book ontologies.
  if (BIMS_CVTERM::addFieldBookOntology()) {
    mcl_print("Field Book ontology has been added", 1, 3);
  }
  else {
    mcl_print("Error : Failed to add Field Book ontology", 1, 3);
  }
}

/**
 * Callback for mcl-add-data-group.
 */
function drush_mcl_add_data_group($group) {

  // Gets the options.
  $opt_desc   = drush_get_option('description');
  $opt_member = drush_get_option('member');

  // Adds a data group.
  $details = array(
    'name'        => $group,
    'description' => $opt_desc,
  );
  $data_group = MCL_DATA_GROUP::addDataGroup($details);
  if ($data_group) {

    // Adds the members.
    if ($opt_member) {
      $members = explode(';', $opt_member);
      foreach ($members as $member) {
        $tmp = explode(':', $member);
        $data_group->addGroupMember($tmp[0], $tmp[1], $tmp[2]);
      }
    }
  }
  mcl_print("DONE", 1, 3);
}

/**
 * Callback for mcl-delete-cvterm.
 */
function drush_mcl_delete_cvterm($cvterm_name, $cv_name = 'SITE_CV') {

  // Gets MCL_CHADO_CVTERM.
  $mcl_cvterm = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name);
  if (!$mcl_cvterm) {
    mcl_print("Error : The cvterm ('$cvterm_name', '$cv_name') does not exist", 1, 3);
    exit;
  }

  // Deletes the cvterm.
  $cvterm_name = $mcl_cvterm->getName();
  $cvterm_id   = $mcl_cvterm->getCvtermID();
  if ($mcl_cvterm->delete()) {
    mcl_print("The cvterm '$cvterm_name' ($cvterm_id) has been deleted", 1, 3);
  }
  else {
    mcl_print("Error : Failed to delete '$cvterm_name' ($cvterm_id)", 1, 3);
  }
}

/**
 * Callback for mcl-delete-cvterm-by-id.
 */
function drush_mcl_delete_cvterm_by_id($cvterm_id) {

  // Gets MCL_CHADO_CVTERM.
  $mcl_cvterm = MCL_CHADO_CVTERM::byID($cvterm_id);
  if (!$mcl_cvterm) {
    mcl_print("Error : The cvterm ID ($cvterm_id) does not exist", 1, 3);
    exit;
  }

  // Deletes the cvterm.
  $cvterm_name = $mcl_cvterm->getName();
  if ($mcl_cvterm->delete()) {
    mcl_print("The cvterm '$cvterm_name' ($cvterm_id) has been deleted", 1, 3);
  }
  else {
    mcl_print("Error : Failed to delete '$cvterm_name' ($cvterm_id)", 1, 3);
  }
}

/**
 * Callback for mcl-delete-project.
 */
function drush_mcl_delete_project($variable) {

  // Gets the option.
  $opt_name = drush_get_option('name') ? TRUE : FALSE;

  // Gets MCL_CHADO_DATASET.
  $dataset = NULL;
  if ($opt_name) {
    $dataset = MCL_CHADO_DATASET::byName($variable);
    if (!$dataset) {
      mcl_print("Error : The project name ($variable) does not exist", 1, 3);
      exit;
    }
  }
  else {
    $dataset = MCL_CHADO_DATASET::byID($variable);
    if (!$dataset) {
      mcl_print("Error : The project ID ($variable) does not exist", 1, 3);
      exit;
    }
  }

  // Deletes the project.
  $project_id = $dataset->getProjectID();
  $name       = $dataset->getName();
  if ($dataset->delete()) {
    mcl_print("The project '$name' ($project_id) has been deleted", 1, 3);
  }
  else {
    mcl_print("Error : Failed to delete '$name' ($project_id)", 1, 3);
  }
}

/**
 * Callback for mcl-delete-phenotype-data-by-cv.
 *
 * Deletes the phenotyping data by cv name
 *
 * @param string $cv_name
 *  The name of cv.
 */
function drush_mcl_delete_phenotype_data_by_cv($cv_name) {

  // Check the cv name.
  if (!$cv_name) {
    mcl_print("Please provide cv name", 1, 3);
    exit;
  }
  $cv = MCL_CHADO_CV::byName($cv_name);
  if (!$cv) {
    mcl_print("The cv name ($cv_name) does not exist", 1, 3);
    exit;
  }

  // Gets the options and show the message.
  $delete         = drush_get_option('delete')          ? TRUE : FALSE;
  $nd_geolocation = drush_get_option('nd_geolocation')  ? TRUE : FALSE;
  $descriptor     = drush_get_option('descriptor')      ? TRUE : FALSE;
  if ($delete) {
    mcl_print("Delete phenotyping data by cv ($cv_name)", 1, 1.3);
  }
  else {
    mcl_print("Checking phenotyping data by cv ($cv_name)", 1, 1.3);
  }

  // Gets all the projects that belongs to the cv.
  $project_ids = array();
  $sql = "
    SELECT DISTINCT PRJ.project_id
    FROM {chado.project} PRJ
      INNER JOIN {chado.nd_experiment_project} NEPRJ on NEPRJ.project_id = PRJ.project_id
      INNER JOIN {chado.nd_experiment_phenotype} NEP on NEP.nd_experiment_id = NEPRJ.nd_experiment_id
      INNER JOIN {chado.phenotype} P on P.phenotype_id = NEP.phenotype_id
      INNER JOIN {chado.cvterm} C on C.cvterm_id = P.attr_id
    WHERE C.cv_id = :cv_id
  ";
  $results = db_query($sql, array(':cv_id' => $cv->getCvID()));
  while ($project_id = $results->fetchField()) {
    $project_ids []= $project_id;
  }

  // Calls the function to delete data by projects.
  if (empty($project_ids)) {
    mcl_print("No pheotyping data found for the cv ($cv_name)", 1, 2);
  }
  else {
    if ($delete) {
      mcl_print("Deletes the phenotyping data", 2, 2);
    }
    else {
      mcl_print("Checking the phenotyping data", 2, 2);
    }
    _drush_mcl_delete_phenotype_data($project_ids, $delete, TRUE, $nd_geolocation, $descriptor);
  }
}

/**
 * Callback for mcl-delete-phenotype-data-by-projects.
 *
 * Deletes the phenotyping data by projects. This won't delete any data
 * in phenotype table. It deletes the followings.
 *
 *   1. samples in stock table.
 *   2. locations in nd_geolocation table.
 *   3. phenotype experiment data in nd_experiment table.
 *   4. projects in project table.
 *
 * @param string project_ids
 *  A list of project ID separed by colon.
 */
function drush_mcl_delete_phenotype_data_by_projects($project_ids) {

  // Parses out project IDs.
  $tmp = preg_split('/:/', $project_ids, -1, PREG_SPLIT_NO_EMPTY);
  if (sizeof($tmp) == 0) {
    mcl_print("Please provide project ID", 1, 3);
    exit;
  }

  // Gets the options.
  $delete   = drush_get_option('delete')  ? TRUE : FALSE;
  $project  = drush_get_option('project') ? TRUE : FALSE;
  if ($delete) {
    mcl_print("Delete phenotyping data by projects", 1, 1.3);
  }
  else {
    mcl_print("Checking phenotyping data by projects", 1, 1.3);
  }

  // Deletes phenotyping data.
  _drush_mcl_delete_phenotype_data($tmp, $delete, $project, FALSE, FALSE);
}

/**
 * Deletes phenotyping data.
 *
 * @param array $project_ids
 * @param boolean $flag_project
 * @param boolean $flag_nd_geolocation
 * @param boolean $flag_descriptor
 */
function _drush_mcl_delete_phenotype_data($project_ids, $delete = FALSE, $flag_project = FALSE, $flag_nd_geolocation = FALSE, $flag_descriptor = FALSE) {

  $transaction = db_transaction();
  try {

    // Check project_ids.
    if (empty($project_ids)) {
      throw new Exception("Project ID are not provided\n\n");
    }

    // Deletes phenotyping data for each project.
    $op = ($delete) ? 'Deleting' : 'Checking';
    $num_project = sizeof($project_ids);
    mcl_print("$op phenotyping data for $num_project projects", 2, 2);

    // Gets cv from projects.
    $project_id_str = implode(",", $project_ids);
    $sql = "
        SELECT DISTINCT CV.cv_id, CV.name
        FROM chado.cv CV
          INNER JOIN chado.cvterm C on C.cv_id = CV.cv_id
          INNER JOIN chado.phenotype P on P.attr_id = C.cvterm_id
          INNER JOIN chado.nd_experiment_phenotype NEP on NEP.phenotype_id = P.phenotype_id
          INNER JOIN chado.nd_experiment_project NEPR on NEPR.nd_experiment_id = NEP.nd_experiment_id
        WHERE NEPR.project_id IN ($project_id_str)
      ";
    $results = db_query($sql);
    $cvs = array();
    while ($obj = $results->fetchObject()) {
      $cvs[$obj->cv_id] = $obj->name;
    }

    // Gets the nd_geolocation before deleting nd_experiment.
    $nd_geolocations  = array();
    $sql = "
      SELECT DISTINCT NG.nd_geolocation_id, NG.description
      FROM chado.nd_geolocation NG
        INNER JOIN chado.nd_experiment NE on NE.nd_geolocation_id = NG.nd_geolocation_id
        INNER JOIN chado.nd_experiment_project NEPR on NEPR.nd_experiment_id = NE.nd_experiment_id
      WHERE NEPR.project_id IN ($project_id_str)
    ";
    $results = db_query($sql);
    while ($obj = $results->fetchObject()) {
      $nd_geolocations[$obj->nd_geolocation_id] = $obj->description;
    }

    // Deletes the phenotyping data by projects.
    foreach ((array)$project_ids as $project_id) {
      $project = MCL_CHADO_DATASET::byId($project_id);
      if (!$project) {
        mcl_print("Project ID ($project_id) not found in project table. Skipped", 2, 2);
        continue;
      }

      // Deleting / Checking phenotyping data for a project.
      mcl_print("$op phenotyping data for " . $project->getName() . " [$project_id]", 2, 1.2);

      // 2. Deleting samples and clones.
      $args = array(
        ':cv_name'        => 'main',
        ':cvterm_name_s'  => 'sample',
        ':cvterm_name_c'  => 'clone',
        ':project_id'     => $project_id,
      );
      if ($delete) {
        mcl_print("Deleting samples in stock table", 2, 1);
        $sql = "
          DELETE FROM chado.stock WHERE stock_id IN (
            SELECT DISTINCT S.stock_id
            FROM chado.stock S
              INNER JOIN chado.cvterm C on C.cvterm_id = S.type_id
              INNER JOIN chado.cv CV on CV.cv_id = C.cv_id
              INNER JOIN chado.nd_experiment_stock NES on NES.stock_id = S.stock_id
              INNER JOIN chado.nd_experiment_project NEP on NEP.nd_experiment_id = NES.nd_experiment_id
            WHERE LOWER(CV.name) = :cv_name AND (LOWER(C.name) = :cvterm_name_s OR LOWER(C.name) = :cvterm_name_c)
              AND NEP.project_id = :project_id
          )
        ";
        db_query($sql, $args);
      }
      else {
        $sql = "
          SELECT COUNT(DISTINCT S.stock_id)
            FROM chado.stock S
              INNER JOIN chado.cvterm C on C.cvterm_id = S.type_id
              INNER JOIN chado.cv CV on CV.cv_id = C.cv_id
              INNER JOIN chado.nd_experiment_stock NES on NES.stock_id = S.stock_id
               INNER JOIN chado.nd_experiment_project NEP on NEP.nd_experiment_id = NES.nd_experiment_id
          WHERE LOWER(CV.name) = :cv_name AND (LOWER(C.name) = :cvterm_name_s OR LOWER(C.name) = :cvterm_name_c)
            AND NEP.project_id = :project_id
        ";
        $num = db_query($sql, $args)->fetchField();
        mcl_print("The number of samples : $num", 2, 1);
      }

      // 4. Deleting from nd_experiment.
      if ($delete) {
        mcl_print("Deleting nd_experiment in nd_experiment table", 2, 1);
        $sql = "
          DELETE FROM chado.nd_experiment WHERE nd_experiment_id IN (
            SELECT NEP.nd_experiment_id
            FROM chado.nd_experiment_project NEP
            WHERE NEP.project_id = :project_id
          )
        ";
        db_query($sql, array(':project_id' => $project_id));
      }
      else {
        $sql = "
          SELECT COUNT(NEP.nd_experiment_id)
          FROM chado.nd_experiment_project NEP
          WHERE NEP.project_id = :project_id
        ";
        $num = db_query($sql, array(':project_id' => $project_id))->fetchField();
        mcl_print("The number of nd_experiment : $num", 2, 1);
      }
    }

    // 5. Deleting from project.
    if ($delete) {
      if ($flag_project) {
        mcl_print("Deleting projects in project table", 2, 1);
        $sql = "DELETE FROM chado.project WHERE project_id IN ($project_id_str)";
        db_query($sql);
      }
    }
    else {
      $num = sizeof($project_ids);
      mcl_print("The number of project : $num", 2, 1);
    }

    // 6. Deleting the descriptors.
    if ($delete) {
      if ($flag_descriptor && !empty($cvs)) {
        foreach ($cvs as $cv_id => $name) {
          mcl_print("Deleting descriptors (phenotype/cvterm)", 2, 1);
          mcl_print("Deleting phenotype", 2, 1);
          $sql = "
            DELETE FROM chado.phenotype WHERE attr_id IN
              (SELECT cvterm_id FROM chado.cvterm WHERE cv_id = :cv_id)
            ";
          db_query($sql, array(':cv_id' => $cv_id));
          mcl_print("Deleting cvterm", 2, 1);
          $sql = "DELETE FROM chado.cvterm WHERE cv_id = :cv_id";
          db_query($sql, array(':cv_id' => $cv_id));
        }
      }
    }
    else {
      if (empty($cvs)) {
        mcl_print("No cv found", 2, 1);
      }
      else {
        foreach ($cvs as $cv_id => $name) {
          $sql = "
            SELECT COUNT(phenotype_id)
            FROM chado.phenotype
            WHERE attr_id IN
              (SELECT cvterm_id FROM chado.cvterm WHERE cv_id = :cv_id)
          ";
          $num = db_query($sql, array(':cv_id' => $cv_id))->fetchField();;
          mcl_print("The number of phenotype : $num", 2, 1);
          $sql = "SELECT COUNT(cvterm_id) FROM chado.cvterm WHERE cv_id = :cv_id";
          $num = db_query($sql, array(':cv_id' => $cv_id))->fetchField();
          mcl_print("The number of cvterm : $num", 2, 1);
        }
      }
    }

    // 7. Deleting from nd_geolocation.
    if ($delete) {
      if ($flag_nd_geolocation) {
        mcl_print("Deleting locations in nd_geolocation table", 2, 1);
        if (!empty($nd_geolocations)) {
          // Gets the default nd_geolcation_id.
          $def_nd_geolocation_id = MCL_DUMMY_VAR::getIdByName('ND_GEOLOCATION_ID');

          // Deletes all nd_geolocation except default one.
          foreach ($nd_geolocations as $nd_geolocation_id => $description ) {

            // Skips the default nd_geolcation_id.
            if ($def_nd_geolocation_id == $nd_geolocation_id) {
              mcl_print("The default nd_geolocation_id [$nd_geolocation_id] is skipped", 2, 1);
              continue;
            }

            // Deletes nd_geolocation.
            if ($nd_geolocation_id && preg_match("/^(not available|NA|N\/A)/i", $description)) {
              $nd_geolocation_ids = implode(",", array_keys($nd_geolocations));
              $sql = "DELETE FROM chado.nd_geolocation WHERE nd_geolocation_id IN ($nd_geolocation_ids)";
              db_query($sql);
            }
          }
        }
      }
    }
    else {
      $num = sizeof($nd_geolocations);
      mcl_print("The number of nd_geolocation : $num", 2, 1);
    }
  } catch (Exception $e) {
    $transaction->rollback();
    mcl_print("Error : " . $e->getMessage(), 1, 3);
    watchdog('mcl', $e->getMessage(), array(), WATCHDOG_ERROR);
  }
  mcl_print("DONE", 2, 1.3);
}

/**
 * Callback for mcl-list-cvterm.
 */
function drush_mcl_list_cvterm($cv_name) {

  // Gets the cv.
  $cv = MCL_CHADO_CV::getCV($cv_name);
  if (!$cv) {
    mcl_print("Error : The cv ($cv_name) does not exist", 1, 3);
    exit;
  }

  $sql = "SELECT C.* FROM {chado.cvterm} C WHERE C.cv_id = :cv_id ORDER BY C.name";
  $results = db_query($sql, array(":cv_id" => $cv->getCvID()));
  $table = '';
  $num_cvterms = 0;
  while ($obj = $results->fetchObject()) {
    $table .= sprintf("%d %s\t%d\t%s\n", $cv->getCvID(), $cv_name, $obj->cvterm_id, $obj->name);
    $num_cvterms++;
  }
  if ($table) {
    $headers = sprintf("%05s %s\t%s\t%s\n", 'cv_id', 'v_name', 'cvterm_id', 'cvterm_name');
    mcl_print($headers . $table, 0, 2.2, FALSE);
    mcl_print("$num_cvterms of cvterms belong to $cv_name", 1, 0.3);
  }
  else {
    mcl_print("No cvterms belong to $cv_name", 1, 2.3, TRUE);
  }
}

/**
 * Callback for mcl-list-project.
 */
function drush_mcl_list_project() {

  // Gets all projects.
  $sql = "
    SELECT P.project_id, P.name, TYPE.value AS type,
      SUBTYPE.value AS sub_type
    FROM {chado.project} P
      LEFT JOIN (
        SELECT PP.project_id, PP.value
        FROM {chado.projectprop} PP
          INNER JOIN {chado.cvterm} C on C.cvterm_id = PP.type_id
        WHERE C.name = 'project_type'
      ) TYPE on TYPE.project_id = P.project_id
      LEFT JOIN (
        SELECT PP.project_id, PP.value
        FROM {chado.projectprop} PP
          INNER JOIN {chado.cvterm} C on C.cvterm_id = PP.type_id
        WHERE C.name = 'sub_type'
      ) SUBTYPE on SUBTYPE.project_id = P.project_id
    ORDER BY TYPE.value, SUBTYPE.value, P.name
  ";
  $results = db_query($sql);
  $table_rows = '';
  $num_cvterms = 0;
  while ($obj = $results->fetchObject()) {
    $project_id = $obj->project_id;
    $table_rows .= sprintf("%d\t%s\t%s\t%s\n", $obj->project_id, $obj->type, $obj->sub_type, $obj->name);
  }
  if ($table_rows) {
    $headers = "\n=================================================================\n";
    $headers .= "Project ID\tType\tSub-Type\tName\n";
    $headers .= "-----------------------------------------------------------------\n";
    mcl_print($headers . $table_rows, 0, 2.2, FALSE);
    mcl_print("$num_projecdts found in chado.project table", 1, 0.3);
  }
  else {
    mcl_print("No project found", 1, 2.3, TRUE);
  }
}

/**
 * Callback for mcl-list-assoc-data.
 */
function drush_mcl_list_assoc_data($type, $primary_key, $primary_id) {

  // Gets the object of the provided type.
  $obj = NULL;

  // Validate the type.
  $class_name = strtoupper('MCL_CHADO_' . $type);
  if (class_exists($class_name)) {
    $obj = $class_name::byID($primary_id);

    // Checks the primary ID for the type.
    if (!$obj) {
      mcl_print("Error Invalid primary ID : '$primary_id' is invalid", 1, 3);
      exit;
    }
  }
  else {
    mcl_print("Error Invalid type : '$type' is not supported yet", 1, 3);
    exit;
  }

  // Gets the associated data.
  $ret_values = $class_name::getAssocData($primary_key, $primary_id);
  $total      = $ret_values['total'];
  $assoc_data = $ret_values['assoc_data'];
  mcl_print("The $type [$primary_key = $primary_id] has $total of the associated data", 1, 2.2);
  foreach ((array)$assoc_data as $data_key => $data) {
    $table_name = $data['table'];
    $attr_name  = $data['attr'];
    $num        = $data['num'];
    if ($num) {
      mcl_print("$table_name [$attr_name] has $num data", 1, 2);
    }
  }
  mcl_print("DONE", 1, 3);
}

/**
 * Callback for mcl-list-assoc-data-by-file.
 */
function drush_mcl_list_assoc_data_by_file($filepath) {

  // Checks the $filepath.
  mcl_check_filepath($filepath);

  // Opens the input file.
  if (!($fdr = fopen($filepath, 'r'))) {
    mcl_print("Error : Failed to open file [$filepath]", 1, 2);
    exit;
  }

  // Opens the output file.
  $outfile = $filepath . '.out';
  if (!($fdw = fopen($outfile, 'w'))) {
    mcl_print("Error : Failed to open file [$outfile]", 1, 2);
    exit;
  }

  // Processes the file.
  $flag_type   = FALSE;
  $flag_header = FALSE;
  $class_name   = '';
  $headers      = array();
  while (!feof($fdr)) {
    $line_arr = fgetcsv($fdr);
    if (empty($line_arr)) {
      continue;
    }

    // Checks the first row (type row).
    if (!$flag_type) {
      $flag_type = TRUE;

      // Checks the type line.
      if (!preg_match("/^#/", $line_arr[0])) {
        mcl_print("Error : The type line missing", 1, 3);
        exit;
      }

      // Validates the type.
      $type = substr($line_arr[0], 1);
      $class_name = strtoupper('MCL_CHADO_' . $type);
      if (!class_exists($class_name)) {
        mcl_print("Error Invalid type : '$type' is not supported yet", 1, 3);
        exit;
      }

      // Writes the type to the outfile.
      fputcsv($fdw, $line_arr);
      continue;
    }

    // Checks the headers.
    if (!$flag_header) {
      $flag_header = TRUE;
      $headers = $line_arr;

      // Writes the headers to the outfile.
      $line_arr []= 'count';
      $line_arr []= 'assoc';
      fputcsv($fdw, $line_arr);
      continue;
    }

    // Processes the data line.
    $keys = array();
    foreach ($headers as $idx => $value) {
      $keys[$value] = $line_arr[$idx];
    }
    $obj = $class_name::byKey($keys);
    if (!$obj) {
      $info = implode(', ', $line_arr);
      mcl_print("Error keys : The key dose not find the object [$info]", 1, 2);
      continue;
    }

    // Finds the associated data.
    $primary_key  = $obj->getPK();
    $primary_id   = $obj->getMemberValue($obj->getPK());
    $ret_values   = $class_name::getAssocData($primary_key, $primary_id, FALSE);
    $total        = $ret_values['total'];
    $assoc_data   = $ret_values['assoc_data'];

    // Shows the quick results.
    mcl_print("The $type [$primary_key = $primary_id] has $total of the associated data", 1, 2.2);

    // Writes out the data line.
    $line_arr []= $total;
    fputcsv($fdw, $line_arr);
    fputs($fdw, json_encode($assoc_data)."\n");
  }

  // Closes the files.
  fclose($fdr);
  fclose($fdw);
}

/**
 * Callback for mcl-upload-test-data.
 */
function drush_mcl_upload_test_data($username) {

  // Checks the username.
  $mcl_user = mcl_check_username($username);

  // Test data file is stored in DATA_FILES.
  $module_path = drupal_get_path('module', 'mcl');
  $filename = 'TEST_MCL_DATA.xlsx';
  $test_data =  "$module_path/TEST_FILES/$filename";
  if (!file_exists($test_data)) {
    print "\n\nTest data file not found in /TEST_FILES/$filename";
    exit;
  }

  // Check pub ID (1) and add 'mcl_pub' if pub ID (1) dose not exist.
  $pub = CHADO_PUB::byKey(array('pub_id' => 1));
  if (!$pub) {
    $pub = CHADO_PUB::byKey(array('uniquename' => 'mcl_pub'));
    if (!$pub) {
      $fields = array(
        'title'       => 'mcl_pub',
        'uniquename'  => 'mcl_pub',
        'pub_id'      => 1,
        'type_id'     => 1,
      );
      db_insert('chado.pub')
        ->fields($fields)
        ->execute();
    }
  }

  // Upload the test file.
  $param_json = '{"cv":"mcl_test_cv"}';
  drush_mcl_upload_data($username, $test_data, $param_json);
}

/**
 * Callback for mcl-update-test-data.
 */
function drush_mcl_update_test_data($username) {

  // Checks the username.
  $mcl_user = mcl_check_username($username);

  // Test data file is stored in DATA_FILES.
  $module_path = drupal_get_path('module', 'mcl');
  $filename = 'TEST_MCL_DATA_UPDATE.xlsx';
  $test_data =  "$module_path/TEST_FILES/$filename";
  if (!file_exists($test_data)) {
    print "\n\nTest data file not found in /TEST_FILES/$filename";
    exit;
  }

  // Upload the test file.
  $param_json = '{"cv":"mcl_test_cv"}';
  drush_mcl_upload_data($username, $test_data, $param_json);
}

/**
 * Callback for mcl-delete-test-data.
 */
function drush_mcl_delete_test_data() {

  // Sets the SQLs.
  $sql_organism_ids   = "SELECT organism_id FROM chado.organism WHERE genus LIKE 'mcl_test_%' AND species LIKE 'mcl_test_%'";
  $sql_cv_ids         = "SELECT cv_id FROM chado.cv WHERE LOWER(name) LIKE 'mcl_test_%'";
  $sql_db_ids         = "SELECT db_id FROM chado.db WHERE LOWER(name) LIKE 'mcl_test_%'";
  $sql_cvterm_ids     = "SELECT cvterm_id FROM chado.cvterm WHERE LOWER(name) LIKE 'mcl_test_%'";
  $sql_project_ids    = "SELECT project_id FROM chado.project WHERE LOWER(name) LIKE 'mcl_test_dataset_%'";
  $sql_featuremap_ids = "SELECT featuremap_id FROM chado.featuremap_organism WHERE organism_id IN ($sql_organism_ids)";

  // Using MCL_TAG.
  $cvterm_id_mcl_tag  = MCL_CHADO_CVTERM::getCvterm('SITE_CV', 'mcl_tag')->getCvtermID();
  $sql_pub_ids = "
    SELECT pub_id FROM chado.pub WHERE pub_id IN (
      SELECT pub_id FROM pubprop
      WHERE type_id = $cvterm_id_mcl_tag
  )";

  // SQLs array.
  $sqls = array(
    "SET search_path TO chado",
    "DELETE FROM chado.analysis WHERE LOWER(name) LIKE 'mcl_test_%'",
    "DELETE FROM chado.contact WHERE LOWER(name) LIKE 'mcl_test_%'",
    "DELETE FROM chado.nd_geolocation WHERE LOWER(description) LIKE 'mcl_test_%'",
    "DELETE FROM chado.library WHERE LOWER(name) LIKE 'mcl_test_%'",
    "DELETE FROM chado.eimage WHERE LOWER(eimage_data) LIKE 'mcl_test_%'",
    "DELETE FROM chado.environment WHERE LOWER(uniquename) LIKE 'mcl_test_%'",
    "DELETE FROM chado.genotype WHERE LOWER(uniquename) LIKE 'mcl_test_%'",
    "DELETE FROM chado.genotype WHERE genotype_id IN (
      SELECT genotype_id FROM chado.genotype_call
      WHERE project_id IN ($sql_project_ids)
    )",
    "DELETE FROM chado.phenotype WHERE attr_id IN ($sql_cvterm_ids)",
    "DELETE FROM chado.nd_experiment WHERE nd_experiment_id IN (
       SELECT NEPR.nd_experiment_id
       FROM chado.nd_experiment_project NEPR
       WHERE NEPR.project_id IN ($sql_project_ids)
    )",
    "DELETE FROM chado.genotype_call WHERE project_id IN ($sql_project_ids)",
    "DELETE FROM chado.synonym WHERE LOWER(name) LIKE 'mcl_test_%' OR synonym_id IN (
      SELECT synonym_id FROM chado.feature_synonym WHERE feature_id IN (
        SELECT feature_id FROM chado.feature WHERE organism_id IN ($sql_organism_ids)
    ))",
    "DELETE FROM chado.feature WHERE feature_id IN (
      SELECT map_feature_id FROM chado.featurepos
      WHERE featuremap_id IN ($sql_featuremap_ids)
    )",
    "DELETE FROM chado.featuremap WHERE featuremap_id IN ($sql_featuremap_ids)",
    "DELETE FROM chado.stockcollection WHERE stockcollection_id IN (
      SELECT stockcollection_id FROM stockcollection_db WHERE db_id IN ($sql_db_ids)
    )",
    "DELETE FROM chado.pubauthor WHERE pub_id IN ($sql_pub_ids)",
    "DELETE FROM chado.pub WHERE pub_id IN ($sql_pub_ids)",
    "DELETE FROM chado.feature WHERE feature_id IN (
      SELECT object_id FROM chado.feature_relationship
      WHERE subject_id IN (
        SELECT feature_id FROM feature WHERE organism_id IN ($sql_organism_ids)
    ))",
    "DELETE FROM chado.feature WHERE feature_id IN (
      SELECT subject_id FROM chado.feature_relationship
      WHERE object_id IN (
      SELECT feature_id FROM feature WHERE organism_id IN ($sql_organism_ids)
    ))",
    "DELETE FROM chado.feature WHERE organism_id IN ($sql_organism_ids)",
    "DELETE FROM chado.project WHERE project_id IN ($sql_project_ids)",
    "DELETE FROM chado.stock WHERE organism_id IN ($sql_organism_ids)",
    "DELETE FROM chado.organism WHERE organism_id IN ($sql_organism_ids)",
    "DELETE FROM chado.db WHERE db_id IN ($sql_db_ids)",
    "DELETE FROM chado.cvterm WHERE cvterm_id IN ($sql_cvterm_ids)",
    "DELETE FROM chado.dbxref WHERE LOWER(accession) LIKE 'mcl_test_%'",
    "DELETE FROM chado.cv WHERE cv_id IN ($sql_cv_ids)",
  );

  // Runs the delete SQLs.
  $transaction = db_transaction();
  try {
    foreach ($sqls as $sql) {
      db_query($sql);
    }
  } catch (Exception $e) {
    $transaction->rollback();
    mcl_print("Error : " . $e->getMessage(), 1, 3);
    exit;
  }

  mcl_print("Checking deleted test data", 1, 3);
  $sqls = array(
    'cv'        => "SELECT COUNT(cv_id) FROM chado.cv WHERE cv_id IN ($sql_cv_ids)",
    'db'        => "SELECT COUNT(db_id) FROM chado.db WHERE db_id IN ($sql_db_ids)",
    'cvterm'    => "SELECT COUNT(cvterm_id) FROM chado.cvterm WHERE cvterm_id IN ($sql_cvterm_ids)",
    'dbxref'    => "SELECT COUNT(dbxref_id) FROM chado.dbxref WHERE accession LIKE 'mcl_test_%'",
    'phenotype' => "SELECT COUNT(phenotype_id) FROM chado.phenotype WHERE attr_id IN ($sql_cvterm_ids)",
    'ghenotype' => "SELECT COUNT(genotype_id) FROM chado.genotype WHERE uniquename LIKE 'mcl_test_%'",
    'feature'   => "SELECT COUNT(feature_id) FROM chado.feature WHERE organism_id IN ($sql_organism_ids)",
    'stock'     => "SELECT COUNT(stock_id) FROM chado.stock WHERE organism_id IN ($sql_organism_ids)",
    'organism'  => "SELECT COUNT(organism_id) FROM chado.organism WHERE organism_id IN ($sql_organism_ids)",
    'pub'       => "SELECT COUNT(pub_id) FROM chado.pub WHERE pub_id IN ($sql_pub_ids)",

  );
  foreach ($sqls as $title => $sql) {
    $num = db_query($sql)->fetchField();
    mcl_print("$title : $num data", 1);
  }
  mcl_print("Done deleteing test data", 1, 2.2);
}

/**
 * Callback for mcl-get-template.
 */
function drush_mcl_get_template($template) {

  // Checks arguments.
  if ($template) {
    $mcl_template = MCL_TEMPLATE::byKey(array('template' => $template));
    if (!$mcl_template) {
      mcl_print("Error : template ($template) have not been defined", 1, 3);
      exit;
    }
  }
  else {
    mcl_print("Error : Please check arguments", 1, 3);
    exit;
  }

  // Sets the file path.
  $filepath = getcwd();

  // Gets MCL_TEMPLATE.
  $args = array(
    'template_id' => $mcl_template->getTemplateID(),
    'type'        => 'cmd',
    'filepath'    => $filepath,
  );
  if (mcl_download_template_excel($args)) {
    mcl_print("\n\nTemplate file has been created in ($filepath)", 1, 3);
  }
  else {
    mcl_print("Error : Failed to generate template file", 1, 3);
  }
}

/**
 * Callback for mcl-update-template-types.
 */
function drush_mcl_update_template_types() {

  // Updates the template types.
  MCL_TEMPLATE_TYPE::updateTemplateTypes();
  mcl_print("The template types have been updated", 1, 3);
}

/**
 * Callback for mcl-update-templates.
 */
function drush_mcl_update_templates() {

  // Updates the template types.
  MCL_TEMPLATE::updateTemplates(TRUE);
  mcl_print("The templates have been updated", 1, 3);
}

/**
 * Callback for mcl-submit-uploading-job.
 */
function drush_mcl_submit_uploading_job($username, $filepath) {

  // Checks the filepath.
  mcl_check_filepath($filepath);

  // Checks the username.
  $mcl_user = mcl_check_username($username);

  // Checks the working_dir_dir option.
  $working_dir = drush_get_option('working_dir');
  if ($working_dir) {
    if (!file_exists($working_dir)) {
      mcl_print("Error : The working directory ($working_dir) dose not exist", 1, 3);
      exit;
    }
  }

  // Gets the job_name option.
  $job_name = drush_get_option('job_name');

  // Prepares a uploading job.
  $mcl_job = mcl_prepare_uploading_job($mcl_user, $filepath, $working_dir, $job_name, FALSE);
  if ($mcl_job) {
    mcl_print("Error : Failed to prepare a uploading job", 1, 3);
    exit;
  }

  // Shows the message.
  $msg = "\n\n\t=========================================" .
      "\n\tUploading job has been submitted.\n\tThe notification email " .
      "would be sent to\n\tthe following address shortly.\n\n" .
      "\temail  : " . $mcl_user->getMail() .
      "\nJob ID : " . $mcl_job->getJobID() .
      "\n\t-----------------------------------------\n\n\n\n\n";
  print $msg;
}

/**
 * Callback for mcl-upload-data.
 */
function drush_mcl_upload_data($username, $filepath, $param_json = '') {

  // Checks the filepath.
  mcl_check_filepath($filepath);

  // Checks the username.
  $mcl_user = mcl_check_username($username);

  // Checks the working_dir option.
  $working_dir = drush_get_option('working_dir');
  if ($working_dir) {
    if (!mcl_check_working_dir($working_dir)) {
      exit;
    }
  }

  // Gets the job_name option.
  $job_name = drush_get_option('job_name');

  // Checks the transaction option.
  $transaction_on   = drush_get_option('transaction_on');
  $transaction_off  = drush_get_option('transaction_off');
  if ($transaction_on || $transaction_off) {
    $trans = ($transaction_on) ? TRUE : FALSE;
  }
  else {
    $trans = TRUE;
  }

  // Check the move_input option.
  $move_input = drush_get_option('move_input');
  $move_input = ($move_input) ? TRUE : FALSE;

  // Adds the parameters from the 3rd argument.
  $param = array();
  if ($param_json) {
    $param = json_decode($param_json, TRUE);
  }

  // Adds parameters from the file.
  $param_file = drush_get_option('param');
  if ($param_file) {
    if (!file_exists($param)) {
      mcl_print("Error : The parameter file doese not exist", 1, 3);
      exit;
    }
    $contents = file_get_contents($param_file);
    $tmp = json_decode($contents, TRUE);
    if (!is_array($param)) {
      mcl_print("Error : The parameter file doese not hava valid JSON string", 1, 3);
      exit;
    }
    foreach ($tmp as $key => $value) {
      $param[$key] = $value;
    }
  }

  // Prepares a uploading job.
  $mcl_job = mcl_prepare_uploading_job($mcl_user, $filepath, $move_input, $trans, $working_dir, $job_name, $param);
  if (!$mcl_job) {
    mcl_print("Error : Failed to prepare a uploading job", 1, 3);
    exit;
  }

  // Gets process ID.
  $pid = getmypid();
  $mcl_job->setPID($pid);
  $mcl_job->update();
  print "\tProcess ID = $pid\n\n";

  // Uploads the file.
  $job_id   = $mcl_job->getJobID();
  $job_name = $mcl_job->getName();
  mcl_print("Running $job_name (Job ID = $job_id)", 1, 2);
  $manager = new MCL_JOB_MANAGER();
  $status = $manager->processJob($mcl_job);
  if ($status) {
    mcl_print("\n\nDONE : The uploading job was completed without an error.", 1, 4);
  }
  else {
    $job_id = $mcl_job->getJobID();
    $opt_working_dir  = ($working_dir) ? " --working_dir=$working_dir " : '';
    $opt_move_input   = ($move_input) ? ' --move_input ' : '';
    $opt_transaction  = ($trans) ? ' --transaction_on ' : ' --transaction_off ';

    mcl_print("DONE : The job (Job ID = $job_id) was completed with some errors", 1);
    mcl_print("Please fix the errors and re-run this job ", 2, 2);

    // Shows re-run command on screen.
    $rerun_cmd = "drush mcl-rerun-job $job_id \"$filepath\" $opt_working_dir $opt_move_input $opt_trans";
    mcl_print(">$rerun_cmd", 2, 4, FALSE);

    // Saves the re-run command as job property.
    $mcl_job->setPropByKey('rerun_cmd', $rerun_cmd);
    $mcl_job->update();
  }
}

/**
 * Prepares for a uploading job.
 *
 * 1. Checks arguments.
 * 2. Copies an input file to job folder.
 *
 * @param MCL_USER $mcl_user
 * @param string $filepath *
 * @param boolean $move_input
 * @param string $working_dir
 * @param string $job_name
 *
 * @return MCL_JOB_UPLOAD
 */
function mcl_prepare_uploading_job(MCL_USER $mcl_user, $filepath, $move_input, $trans, $working_dir = '', $job_name = '', $param = array()) {
  $mcl_job  = NULL;

  // Sets the job directory as working directory.
  $prop = array();
  if ($working_dir) {
    $prop['working_dir'] = $working_dir;
  }

  // Sets the transaction.
  $prop['transaction'] = $trans;

  // Sets the job name.
  $name = 'Uploading-' . basename($filepath);
  if ($job_name) {
    $name = $job_name;
  }

  // Prepares for a uploading job.
  $transaction = db_transaction();
  try {

    // Creates a job for uploading data.
    $details = array(
      'name'        => $name,
      'type'        => 'upload',
      'class_name'  => 'MCL_JOB_UPLOAD',
      'status'      => 0,
      'user_id'     => $mcl_user->getUserID(),
      'submit_date' => date("Y-m-d G:i:s"),
      'prop'        => json_encode($prop),
      'param'       => json_encode($param),
    );
    $mcl_job = new MCL_JOB_UPLOAD($details);
    if ($mcl_job->insert()) {

      // Moves the input file to 'file' folder in the job folder.
      $filename     = basename($filepath);
      $new_filepath = $mcl_job->getFileDir() . "/$filename";
      $cmd          = ($move_input) ? "mv \"$filepath\" \"$new_filepath\"" : "cp \"$filepath\" \"$new_filepath\"";
      mcl_print("Copying the input files", 1);
      exec($cmd, $return_var);
      if (!empty($return_var)) {
        throw new Exception("Fail to copy file.\n\n$cmd\n\n");
      }

      // Adds the uploaded file to mcl_file.
      $details = array(
        'filename'    => $filename,
        'filepath'    => $new_filepath,
        'uri'         => '',
        'filesize'    => filesize($new_filepath),
        'user_id'     => $mcl_user->getUserID(),
        'job_id'      => $mcl_job->getJobID(),
        'submit_date' => date("Y-m-d G:i:s"),
      );
      $upload_file = new MCL_FILE($details);
      if ($upload_file->insert()) {

        // Updates the job properties.
        $mcl_job->setFileID($upload_file->getFileID());
        if (!$mcl_job->update()) {
          throw new Exception("Fail to update Job property.");
        }
      }
      else {
        throw new Exception("Fail to insert mcl_FILE.");
      }
    }
    else {
      throw new Exception("Fail to insert MCL_JOB.");
    }
  } catch (Exception $e) {
    $transaction->rollback();
    mcl_print("Error : " . $e->getMessage(), 1, 3);
    watchdog('mcl', $e->getMessage(), array(), WATCHDOG_ERROR);
    return array('flag' => FALSE);
  }
  return $mcl_job;
}

/**
 * Callback for mcl-merge-test.
 */
function drush_mcl_merge_test($type, $id_base, $id) {

  // Gets the 'verbose' option.
  $verbose = drush_get_option('verbose') ? TRUE : FALSE;

  // Gets and checks the class name.
  $class_name = strtoupper('MCL_CHADO_' . $type);
  if (!class_exists($class_name)) {
    mcl_print("Error : The class name '$class_name' does not exist", 1, 3);
    exit;
  }

  // Gets the base object.
  $obj_base = $class_name::byID($id_base);
  if (!$obj_base) {
    mcl_print("Error : The object ($id_base) not found", 1, 3);
    exit;
  }

  // Gets the object.
  $obj = $class_name::byID($id);
  if (!$obj) {
    mcl_print("Error : The object ($id) not found", 1, 3);
    exit;
  }

  // Checks the function name.
  $func_name = 'merge' . ucfirst($type);
  if (function_exists($func_name)) {
    mcl_print("Error : Function '$func_name' does not exist", 1, 3);
    exit;
  }

  // Performs the merge test.
  mcl_print("Testing merge $type : $id => $id_base", 1, 2.2);
  $obj_base->$func_name(NULL, $obj, $verbose);
  mcl_print("DONE", 1, 3);
}

/**
 * Callback for mcl-generate-class.
 */
function drush_mcl_generate_class($module = NULL, $schema = NULL, $dump_file = NULL, $table_file = NULL) {

  // Gets options.
  $move       = drush_get_option('move');
  $namespace  = drush_get_option('namespace');
  $extends    = drush_get_option('extends');
  $all        = drush_get_option('all');

  // Shows the command.
  if (!$module || !$schema) {
    print "\n\t(Example)\n";
    print "\t>drush mcl-class <module> <schema> <dump.sql> <filter.txt>\n\n";
    exit;
  }

  // Gets the module path.
  $module_path = drupal_get_path('module', 'mcl');

  // Checks arguments.
  mcl_check_module($module);
  mcl_check_schema($schema);

  // Updates the working directory. The defualt is schema directory.
  $working_dir = drush_get_option('working_dir');
  if (!$working_dir) {
    $working_dir = "$module_path/schema";
  }
  if (!file_exists($working_dir)) {
    mcl_print("Error : The working directory ($working_dir) does not exist", 1, 4);
    exit;
  }

  // Updates the dump file.
  if (!$dump_file) {
    $dump_file = "$module_path/schema/drupal.$schema.sql";
  }
  else {
    $dump_file = "$working_dir/$dump_file";
  }
  if (!file_exists($dump_file)) {
    mcl_print("Error : The dump file ($dump_file) not exist", 1, 4);
    exit;
  }

  // Gets the non-chado tables.
  $non_chado = CHADO_TABLE::getNonChadoTables();

  // Sets the tables to be filtered out. If $all is TRUE, the class definition
  // file of all the tables in the schema will be created.
  $tables = array();
  if ($all) {
    $regex = is_string($all) ? $all : '';
    $all_tables = mcl_get_all_tables($schema, $regex);
    if (empty($all_tables)) {
      mcl_print("Error : There no table exists in the schema ($schema)", 1, 4);
      exit;
    }
    foreach ($all_tables as $table_name) {

      // Skips the non-chado tables if the schema is 'chado'.
      if ($schema == 'chado' && in_array($table_name, $non_chado)) {
        continue;
      }

      // Adds the table.
      $tables[$table_name] = array('CLASS_NAME' => $table_name, 'EXISTS' => FALSE);
    }
  }
  else {
    if (!$table_file) {
      $table_file = "$module_path/schema/filter.drupal.$schema.$module.txt";
    }
    else {
      $table_file = "$working_dir/$table_file";
    }
    if (!file_exists($table_file)) {
      mcl_print("Error : The table file ($table_file) not exist", 1, 4);
      exit;
    }

    // Reads the table file.
    $tables = MCL_TABLE_INFO::readTableFile($table_file);
  }

  // Read the information of the tables in the schema.
  $mcl_table_info = new MCL_TABLE_INFO($dump_file, $tables);

  // Creates a tempolary directory and move the current class files.
  $tmp_dir = "$working_dir/tmp";
  mcl_create_dir($tmp_dir);

  // Generats class files.
  $writer = new MCL_TABLE_WRITER($mcl_table_info, $tmp_dir, $namespace, $extends);
  if ($writer->generateClassFiles()) {

    // Moves the class files to the module directory if exists.
    $moved = FALSE;
    $target_dir = drupal_get_path('module', $module) . '/includes/class/public_table_class';
    if ($move && file_exists($target_dir)) {
      exec("mv $tmp_dir/*.inc $target_dir");
      $moved = TRUE;
      mcl_print("The class files are generated and copied over $target_dir.", 1, 1, FALSE);
    }

    // Write the message.
    if (!$moved) {
      mcl_print("The class files are generated in to :", 1, 1, FALSE);
      mcl_print($tmp_dir, 1, 1);
      mcl_print("Please copy them over to the class directory", 1, 4);
    }
  }
  else {
    mcl_print("Failed to generate class files", 1, 4);
  }
}

/**
 * Callback for fixing date on prop table.
 *
 * @param string $cvterm_name.
 * @param string $cv_name.
 * @param string $table_name.
 */
function drush_mcl_fix_date($cv_name, $cvterm_name, $table_name) {

  // Checks the cv.
  if (!MCL_CHADO_CV::checkCV(NULL, $cv_name)) {
    mcl_print("Error : The cv ($cv_name) does not exist", 1, 1);
    exit;
  }

  // Checks the cvterm.
  if (!MCL_CHADO_CVTERM::checkCvterm(NULL, $cv_name, $cvterm_name)) {
    mcl_print("Error : The cvterm ($cvterm_name) does not exist", 1, 1);
    exit;
  }

  // Checks the table.
  if (!db_table_exists('chado.' . $table_name)) {
    mcl_print("Error : The table ($table_name) does not exist", 1, 1);
    exit;
  }

  // Gets MCL_CHADO_CVTERM.
  $cvterm = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name);

  // Fixes the date.
  if ($table_name == 'phenotype') {
    _mcl_fix_date_phenotype($cvterm, $table_name);
  }
  else if (preg_match("/prop$/", $table_name)) {
    _mcl_fix_date_prop($cvterm, $table_name);
  }
  mcl_print("DONE", 1, 3);
}

/**
 * Fixes the date on phenotype table.
 *
 * @param MCL_CHADO_CVTERM $cvterm
 * @param string $table_name
 */
function _mcl_fix_date_phenotype(MCL_CHADO_CVTERM $cvterm, $table_name) {

  // Gets all phenotype.
  $sql = "
    SELECT DISTINCT P.*
    FROM {chado.phenotype} P
    WHERE P.attr_id = :cvterm_id
  ";
  $results = db_query($sql, array(':cvterm_id' => $cvterm->getCvtermID()));
  $updates = '';
  while ($obj = $results->fetchObject()) {
    $phenotype_id = $obj->phenotype_id;
    $uniquename   = $obj->uniquename;
    $value        = $obj->value;
    if (preg_match("/^(\d\d\d\d\d)$/", $value, $matches)) {

      // Gets the date in number and converts the date (number to string
      $date_num = $matches[1];
      $date_str = date("n/j/Y", mktime(0, 0 , 0 , 1, $date_num - 1, 1900));

      // Gets the uniquename.
      preg_match("/^(.*)_\d+$/", $uniquename, $matches);
      $prefix = $matches[1];
      $uniquename = $prefix . "_$date_str";
      $updates .= "-- phenotype_id = $phenotype_id : value = $value\n";
      $updates .= "UPDATE chado.phenotype SET uniquename = '$uniquename', value = '$date_str' WHERE phenotype_id = $phenotype_id;\n";
    }
  }
  print "-- UPDATE SQL\n$updates\n";
}

/**
 * Fixes the date on phenotype table.
 *
 * @param MCL_CHADO_CVTERM $cvterm
 * @param string $table_names
 */
function _mcl_fix_date_prop(MCL_CHADO_CVTERM $cvterm, $table_name) {

  // Fixes the date. Converting 5 digits to date format (xx/xx/xxxx).
  $primary_key = $table_name . '_id';
  $cvterm_id = $cvterm->getCvtermID();
  $sql = "
    SELECT $primary_key, value
    FROM chado.$table_name P
    WHERE P.type_id = $cvterm_id
  ";
  print "$sql\n";
  $results = db_query($sql);
  $updates = '';
  while ($row = $results->fetchAssoc()) {
    $value = $row['value'];
    $id    = $row[$primary_key];
    if (preg_match("/^(\d\d\d\d\d)$/", $value, $matches)) {
      $date_num = $matches[1];
      $date_str = mcl_format_date($date_num);
      $updates .= "-- $primary_key = $id : value = $value\n";
      $updates .= "UPDATE chado.$table_name SET value = '$date_str' WHERE $primary_key = $id;\n";
    }
  }
  print "-- UPDATE SQL\n$updates\n";
}

/**
 * Callback for fixing data.
 *
 * @param string $inc_file.
 * @param string $input_file.
 */
function drush_mcl_fix_data($inc_file, $input_file = NULL) {

  // Checks arguments.
  mcl_check_empty_arg('inc_file', $inc_file);

  // Checks the include file.
  $fix_data_dir = drupal_get_path('module', 'mcl') . '/fix_data/';
  $inc_filepath = "$fix_data_dir/$inc_file";
  if (file_exists($inc_filepath)) {
    require_once($inc_filepath);

    // Gets the working directory.
    $working_dir = dirname($inc_filepath);

    // Checks the input file.
    $input_filepath = NULL;
    if ($input_file) {
      $input_filepath = "$working_dir/$input_file";
      if (!file_exists($input_filepath)) {
        print "The input file cannot found : $input_filepath\n\n";
        exit;
      }
    }
    $inc_filename = basename($inc_file);
    $func_name = str_replace('.inc', '', $inc_filename);
    call_user_func($func_name, $working_dir, $input_filepath);
  }
  else {
    print "The include file cannot found : $inc_filepath\n\n";
  }
}

/**
 * Callback for mcl-call-func.
 */
function drush_mcl_call_func($func_name) {

  // Checks arguments.
  mcl_check_function($func_name);

  // Checks the parameter option.
  $param = drush_get_option('param');
  $args = array();
  if ($param) {
    $args = json_decode($param);
  }

  // Call the function.
  call_user_func_array($func_name, $args);
  mcl_print("DONE", 1, 3);
}

/**
 * Checks working_dir directory.
 *
 * @param string $working_dir
 *
 * @return boolean
 */
function mcl_check_working_dir($working_dir) {

  // Checks the working directory.
  mcl_check_filepath($working_dir);

  // Checks for write permission.
  if (!is_writable($working_dir)) {
    mcl_print("Error : Cannot write on the working directory ($working_dir)", 1);
    mcl_print("Please change the write permission of the directory", 1, 3);
    return FALSE;
  }
  return TRUE;
}

/**
 * Checks the username.
 *
 * @param string $username
 *
 */
function mcl_check_username($username) {

  // Checks for MCL_USER.
  $mcl_user = MCL_USER::byKey(array('name' => $username));
  if (!$mcl_user) {
    mcl_print("Error : The username ($username) does not exist", 1, 3);
    exit;
  }
  return $mcl_user;
}

/**
 * Checks the job ID.
 *
 * @param integer $job_id
 *
 */
function mcl_check_job_id($job_id) {

  // Checks for an integer.
  if (!mcl_is_int($job_id)) {
    mcl_print("Error : Job ID ($job_id) must be an integer", 1, 3);
    exit;
  }

  // Checks for MCL_JOB.
  $mcl_job = MCL_JOB_UPLOAD::byID($job_id);
  if (!$mcl_job) {
    mcl_print("Error : Job ID ($job_id) does not exist", 1, 3);
    exit;
  }
  return $mcl_job;
}

/**
 * Checks the file ID.
 *
 * @param integer $file_id
 *
 */
function mcl_check_file_id($file_id) {

  // Checks for an integer.
  if (!mcl_is_int($file_id)) {
    mcl_print("Error : File ID ($file_id) must be an integer", 1, 3);
    exit;
  }

  // Checks for MCL_FILE.
  $mcl_file = MCL_FILE::byID($file_id);
  if (!$mcl_file) {
    mcl_print("Error : file ID ($file_id) does not exist", 1, 3);
    exit;
  }
  return $mcl_file;
}

/**
 * Checks the filepath.
 *
 * @param string $filepath
 */
function mcl_check_filepath($filepath) {

  // Checks the file path.
  if (!file_exists($filepath)) {
    mcl_print("Cannot find file\n\n\t$filepath", 1, 2);
    mcl_print("Please check file name or add full path to the filename", 1, 3);
    exit;
  }
  return TRUE;
}

/**
 * Checks the schema.
 *
 * @param string $schema
 */
function mcl_check_schema($schema) {

  // Checks the schema for empty.
  if (!$schema) {
    mcl_print("ERROR: please provide a name of schema", 1);
    exit;
  }

  // Checks if the schema exists.
  if (!mcl_exist_schema($schema)) {
    mcl_print("ERROR: The schema '$schema' does not exist", 1);
    exit;
  }
}

/**
 * Checks the function.
 *
 * @param string $function
 *
 */
function mcl_check_function($function) {

  // Checks the function name for empty.
  if (!$function) {
    mcl_print("ERROR: please provide a name of function", 1);
    exit;
  }

  // Checks if the function exists.
  if (!function_exists($function)) {
    mcl_print("ERROR: The function does not exist", 1);
    exit;
  }
}

/**
 * Checks the module.
 *
 * @param string $module
 *
 */
function mcl_check_module($module) {

  // Checks if module exists.
  if (!module_exists($module)) {
    mcl_print("Error : The module ($module) does not exist", 1, 3);
    exit;
  }
  return TRUE;
}

/**
 * Checks the argument for empty.
 *
 * @param string $key
 * @param string $value
 *
 * @return boolean
 */
function mcl_check_empty_arg($key, $value) {

  // Checks for empty value.
  if (empty($value)) {
    mcl_print("Error : $key is empty", 1, 3);
    exit;
  }
  return TRUE;
}

/**
 * Updatess residues in feature table.
 */
function _mcl_update_feature_residues() {
  $GLOBALS['verbose'] = FALSE;
  mcl_print("Updating feature.residues", 1, 1.2);
  _mcl_update_feature_residues_bracket();
  _mcl_update_feature_residues_with_code();
  mcl_print("DONE", 1, 3);
}

/**
 * Updatess residues in feature table.
 */
function _mcl_update_feature_residues_bracket() {
  mcl_print("Updating feature.residues with bracket", 1, 1.2);

  // Gets cvterms.
  $cvterm_id_genetic_marker = MCL_CHADO_CVTERM::getCvterm('sequence', 'genetic_marker')->getCvtermID();
  $cvterm_id_five_prime     = MCL_CHADO_CVTERM::getCvterm('sequence', 'five_prime_flanking_region')->getCvtermID();
  $cvterm_id_three_prime    = MCL_CHADO_CVTERM::getCvterm('sequence', 'three_prime_flanking_region')->getCvtermID();
  $cvterm_id_allele         = MCL_CHADO_CVTERM::getCvterm('sequence', 'allele')->getCvtermID();
  mcl_print("genetic_marker : $cvterm_id_genetic_marker", 1);
  mcl_print("five_prime     : $cvterm_id_five_prime", 1);
  mcl_print("three_prime    : $cvterm_id_three_prime", 1);
  mcl_print("Allele         : $cvterm_id_allele", 1, 2);

  $sql = "SELECT feature_id FROM chado.feature WHERE residues ~ '\[' AND type_id = $cvterm_id_genetic_marker";
  $results = db_query($sql);
  $count = 0;
  while ($feature_id = $results->fetchField()) {
    $count++;
    $marker = MCL_CHADO_FEATURE::byKey(array('feature_id' => $feature_id));
    $residues = $marker->getResidues();
    $flag = MCL_CHADO_FEATURE::checkResidues(NULL, $residues);
    if (!$flag) {
      print "-- $feature_id The allele has invalid format or no IUPAC ($residues)\n";
    }
    else {
      $seq_info     = MCL_CHADO_FEATURE::parseResidues($residues);
      $five_prime   = $seq_info['five_prime'];
      $allele       = $seq_info['allele'];
      $three_prime  = $seq_info['three_prime'];
      $residues     = $seq_info['residues'];

      // Updates feature.residues.
      print "UPDATE chado.feature SET residues = '$residues' WHERE feature_id = $feature_id;\n";

      // Adds prop if not exist.
      $prop = CHADO_FEATUREPROP::byKey(array('feature_id' => $feature_id, 'type_id' => $cvterm_id_five_prime));
      if (!$prop) {
        print "INSERT INTO chado.featureprop (feature_id, type_id, value) VALUES ($feature_id, $cvterm_id_five_prime, '$five_prime');\n";
      }
      else {
        print "-- $feature_id has five prime\n";
      }
      $prop = CHADO_FEATUREPROP::byKey(array('feature_id' => $feature_id, 'type_id' => $cvterm_id_three_prime));
      if (!$prop) {
        print "INSERT INTO chado.featureprop (feature_id, type_id, value) VALUES ($feature_id, $cvterm_id_three_prime, '$three_prime');\n";
      }
      else {
        print "-- $feature_id has three prime\n";
      }
      $prop = CHADO_FEATUREPROP::byKey(array('feature_id' => $feature_id, 'type_id' => $cvterm_id_allele));
      if (!$prop) {
        print "INSERT INTO chado.featureprop (feature_id, type_id, value) VALUES ($feature_id, $cvterm_id_allele, '$allele');\n";
      }
      else {
        print "-- $feature_id has allele\n";
      }
    }
  }
  mcl_print("$count of features were needed to be fixed", 1, 1.2);
}

/**
 * Updatess residues in feature table.
 */
function _mcl_update_feature_residues_with_code() {
  mcl_print("Updating feature.residues with IUPAC code", 1, 1.2);

  // Gets cvterms.
  $cvterm_id_genetic_marker = MCL_CHADO_CVTERM::getCvterm('sequence', 'genetic_marker')->getCvtermID();
  $cvterm_id_five_prime     = MCL_CHADO_CVTERM::getCvterm('sequence', 'five_prime_flanking_region')->getCvtermID();
  $cvterm_id_three_prime    = MCL_CHADO_CVTERM::getCvterm('sequence', 'three_prime_flanking_region')->getCvtermID();
  $cvterm_id_allele         = MCL_CHADO_CVTERM::getCvterm('sequence', 'allele')->getCvtermID();
  mcl_print("genetic_marker : $cvterm_id_genetic_marker", 1);
  mcl_print("five_prime     : $cvterm_id_five_prime", 1);
  mcl_print("three_prime    : $cvterm_id_three_prime", 1);
  mcl_print("Allele         : $cvterm_id_allele", 1, 2);

  $sql = "
    SELECT F.feature_id, F.uniquename, F.residues
    FROM chado.feature F
    WHERE F.residues ~ '[R|Y|S|W|K|M|B|D|H|V]' AND F.type_id = $cvterm_id_genetic_marker
  ";
  $results = db_query($sql);
  $count = 0;
  while ($feature_obj = $results->fetchObject()) {
    $feature_id   = $feature_obj->feature_id;
    $residues     = $feature_obj->residues;
    $num_matches  = preg_match_all("/[R|Y|S|W|K|M|B|D|H|V]/", $residues, $matches);
    if ($num_matches == 1) {
      $count++;
      preg_match("/(.*)([R|Y|S|W|K|M|B|D|H|V])(.*)$/", $residues, $matches);
      $five_prime   = $matches[1];
      $iupac_code   = $matches[2];
      $three_prime  = $matches[3];
      $bases        = MCL_CHADO_FEATURE::getBases($iupac_code);
      print "\n-- Residues ($iupac_code): $residues\n";

      // Adds prop if not exist.
      $prop = CHADO_FEATUREPROP::byKey(array('feature_id' => $feature_id, 'type_id' => $cvterm_id_five_prime));
      if (!$prop) {
        print "INSERT INTO chado.featureprop (feature_id, type_id, value) VALUES ($feature_id, $cvterm_id_five_prime, '$five_prime');\n";
      }
      else {
        print "-- $feature_id has five prime\n";
      }
      $prop = CHADO_FEATUREPROP::byKey(array('feature_id' => $feature_id, 'type_id' => $cvterm_id_three_prime));
      if (!$prop) {
        print "INSERT INTO chado.featureprop (feature_id, type_id, value) VALUES ($feature_id, $cvterm_id_three_prime, '$three_prime');\n";
      }
      else {
        print "-- $feature_id has three prime\n";
      }
      $prop = CHADO_FEATUREPROP::byKey(array('feature_id' => $feature_id, 'type_id' => $cvterm_id_allele));
      if (!$prop) {
        print "INSERT INTO chado.featureprop (feature_id, type_id, value) VALUES ($feature_id, $cvterm_id_allele, '$bases');\n";
      }
      else {
        print "-- $feature_id has allele\n";
      }
    }
    else {
      print "-- Error : The residues of $feature_id has $num_matches IUPAC codes.\n";
    }
  }
  mcl_print("$count of features were needed to be fixed", 1, 1.2);
}

/**
 * Updatess residues in feature table.
 */
function _mcl_fix_phenotype_value() {
  mcl_print("Fixing phenotype data for precision", 1, 1.2);

  // Gets all numeric numbers.
  $sql = "
    SELECT *
    FROM {chado.phenotype}
    WHERE value ~ '\.\d+{3}$'
  ";
  $results = db_query($sql);
  $phenotypes = array();
  while ($obj = $results->fetchObject()) {
    $new_value = round($obj->value, 3, PHP_ROUND_HALF_UP);

    $phenotypes[$obj->phenotype_id] = array(
      'uniquename'      => $obj->uniquename,
      'value'           => $obj->value,
      'new_value'       => $new_value,
      'new_uniquename'  => $obj->attr_id . '.' . $new_value,
    );
  }
  mcl_print("DONE", 1, 3);
}

/**
 * Callback for mcl-get-markers command.
 */
function drush_mcl_get_markers($filepath) {

  // Checks the existance of the input file.
  if (!file_exists($filepath)) {
    $tmp = drupal_get_path('module', 'mcl') . "/DATA_FILES/$filepath";
    if (!file_exists($tmp)) {
      mcl_print("Error : Could not find the input file [$filepath]", 1, 2);
      exit;
    }
    $filepath = $tmp;
  }

  // Opens the input file.
  if (!($fdr = fopen($filepath, 'r'))) {
    mcl_print("Error : Failed to open file [$filepath]", 1, 2);
    exit;
  }

  // Opens the output file.
  $outfile = $filepath . '.out';
  if (!($fdw = fopen($outfile, 'w'))) {
    mcl_print("Error : Failed to open file [$outfile]", 1, 2);
    exit;
  }

  // Gets cvterms.
  $cvterm_id_ss_id = MCL_CHADO_CVTERM::getCvterm('SITE_CV', 'ss_id')->getCvtermID();
  $cvterm_id_alias = MCL_CHADO_CVTERM::getCvterm('SITE_CV', 'alias')->getCvtermID();

  // Processes the file.
  while (!feof($fdr)) {
    $line = trim(fgets($fdr));
    if (!$line) {
      continue;
    }
    $tmp = explode("\t", $line);
    $key = trim(strtolower($tmp));

    // Searches the key in synonym table.
    $sql = "
      SELECT F.uniquename
      FROM chado.feature F
        INNER JOIN chado.feature_synonym FS on FS.feature_id = F.feature_id
        INNER JOIN chado.synonym S on S.synonym_id = FS.synonym_id
      WHERE S.type_id IN ($cvterm_id_ss_id, $cvterm_id_alias) AND LOWER(S.name) = '$key';
    ";
    $uniquename = db_query($sql)->fetchField();

    // Writes uniquename.
    fputcsv($fdw, array($key, $uniquename));
  }

  // Closes the files.
  fclose($fdr);
  fclose($fdw);
  mcl_print("DONE", 1, 3);
}

/**
 * Callback for mcl-search-cv command.
 */
function drush_mcl_search_cv($name) {

  // Gets the options.
  $opt_id = drush_get_option('id') ? TRUE : FALSE;

  // Gets the cv by ID.
  $objs = array();
  if ($opt_id) {
    $sql = "
      SELECT CV.name, CV.cv_id FROM {chado.cv} CV
      WHERE CV.cv_id = :cv_id
    ";
    $objs []= db_query($sql, array(':cv_id' => $name))->fetchObject();
  }

  // Gets the cv by name.
  else {
    $sql = "
      SELECT CV.name, CV.cv_id FROM {chado.cv} CV
      WHERE LOWER(CV.name) = LOWER(:name)
    ";
    $results = db_query($sql, array(':name' => $name));
    while ($obj = $results->fetchObject()) {
      $objs []= $obj;
    }
  }

  // Displays the info of the cvterm.
  if (empty($objs)) {
    mcl_print("cv '$name' could not be found", 1, 1.2);
  }
  else {
    mcl_print("cv-ID\tCV-name", 1, 1.1, FALSE);
    foreach ($objs as $obj) {
      $info = sprintf("%d\t%s", $obj->cv_id, $obj->name);
      mcl_print($info, 1, 1, FALSE);
    }
    mcl_print('', 1, 1, FALSE);
  }
}

/**
 * Callback for mcl-search-cvterm command.
 */
function drush_mcl_search_cvterm($name) {

  // Gets the options.
  $opt_id = drush_get_option('id') ? TRUE : FALSE;

  // Gets the cvterm by ID.
  $objs = array();
  if ($opt_id) {
    $sql = "
      SELECT C.name AS cvterm_name, C.cvterm_id, CV.name AS cv_name, CV.cv_id
      FROM {chado.cvterm} C
        INNER JOIN {chado.cv} CV on CV.cv_id = C.cv_id
      WHERE C.cvterm_id = :cvterm_id
    ";
    $objs []= db_query($sql, array(':cvterm_id' => $name))->fetchObject();
  }

  // Gets the cvterm by name.
  else {
    $sql = "
      SELECT C.name AS cvterm_name, C.cvterm_id, CV.name AS cv_name, CV.cv_id
      FROM {chado.cvterm} C
        INNER JOIN {chado.cv} CV on CV.cv_id = C.cv_id
        WHERE LOWER(C.name) = LOWER(:name)
    ";
    $results = db_query($sql, array(':name' => $name));
    while ($obj = $results->fetchObject()) {
      $objs []= $obj;
    }
  }

  // Displays the info of the cvterm.
  if (empty($objs)) {
    mcl_print("cvterm '$name' could not be found", 1, 1.2);
  }
  else {
    print_r($objs);
    mcl_print("cv-ID\tCV-name\tcvterm-ID\tcvterm-name", 1, 1, FALSE);
    foreach ($objs as $obj) {
      $info = sprintf("%d\t%s\t%d\t%d", $obj->cv_id, $obj->cv_name, $obj->cvterm_id, $obj->cvterm_name);
      mcl_print($info, 1, 1, FALSE);
    }
    mcl_print('', 1, 1, FALSE);
  }
}

/**
 * Callback for mcl-process-file command.
 */
function drush_mcl_process_file($filepath) {

  // Gets the options.
  $out = drush_get_option('out');

  // Checks the existance of the input file.
  if (!file_exists($filepath)) {
    $tmp = drupal_get_path('module', 'mcl') . "/DATA_FILES/$filepath";
    if (!file_exists($tmp)) {
      mcl_print("Error : Could not find the input file [$filepath]", 1, 2);
      exit;
    }
    $filepath = $tmp;
  }

  // Opens the input file.
  if (!($fdr = fopen($filepath, 'r'))) {
    mcl_print("Error : Failed to open file [$filepath]", 1, 2);
    exit;
  }

  // Opens the output file.
  $fdw = NULL;
  if ($out) {
    $outfile = $filepath . '.out';
    if (!($fdw = fopen($outfile, 'w'))) {
      mcl_print("Error : Failed to open file [$outfile]", 1, 2);
      exit;
    }
  }

  // Processes the file.
  _drush_mcl_process_file($fdr, $fdw);

  // Closes the files.
  fclose($fdr);
  if ($out) {
    fclose($fdw);
  }
  mcl_print("DONE", 1, 3);
}

/**
 * Helper funciton of drush_mcl_process_file.
 */
function _drush_mcl_process_file($fdr, $fdw = NULL) {

  // Gets cvterms.
  $cvterm_id_gm = MCL_CHADO_CVTERM::getCvterm('sequence', 'genetic_marker')->getCvtermID();

  // Processes the file.
  while (!feof($fdr)) {
    $line = trim(fgets($fdr));
    if (!$line) {
      continue;
    }
    $tmp        = explode("\t", $line);
    $ss_id      = $tmp[0];
    $uniquename = strtolower($tmp[1]);
    $sr_id      = $tmp[4];

    $sql = "
      SELECT F.uniquename, O.genus, O.species
      FROM chado.feature F
        INNER JOIN chado.organism O on O.organism_id = F.organism_id
      WHERE F.type_id = $cvterm_id_gm AND LOWER(uniquename) = '$uniquename';
    ";
    $obj = db_query($sql)->fetchObject();
    if ($obj) {
      $fields = array($obj->uniquename, $obj->genus, $obj->species, $ss_id, $sr_id);
      fputcsv($fdw, $fields);
    }
  }
}

/**
 * Callback for mcl-run-cron command.
 */
function drush_mcl_run_cron($type) {

  // Gets the drush binaray.
  $drush = mcl_get_config_setting('mcl_drush_binary');

  // Runs the cron job.
  if ($type == 'hour') {
    mcl_cron_hourly($drush);
  }
  else if ($type == 'day') {
    mcl_cron_daily($drush);
  }
  else if ($type == 'week') {
    mcl_cron_weekly($drush);
  }
  else {
    print "ERROR : Invalid type ($type) - [hour|day|week].\n\n";
    exit;
  }
  print "\nThe ($type) cron job has been run.\n\n\n";
}

/**
 * Callback for mcl-debug command.
 */
function drush_mcl_debug() {
  mcl_print("MCL-DEBUG", 1, 3);
  /***********************************************/

  /***********************************************/
  mcl_print("DONE", 1, 3);
}

/* [mcl-debug]
 * Test regular expressions.
 */
function _drush_mcl_debug_test_regex() {

  $str = '123.23.add.d';
  preg_match("/(.*)\.(.*)$/", $str, $matches);
  print "$str\n";
  print "M:>" . $matches[1] . "\n\n";

 /*
  $str = 'ADD CONSTRAINT abc PRIMARY KEY (PRIMARY KEY );';
  preg_match("/^ADD CONSTRAINT (.*) PRIMARY KEY \((.*)\);$/i", $str, $matches);
  print "$str\n";
  print "M1:>" . $matches[1] . "\n\n";
  print "M2:>" . $matches[2] . "\n\n";*/
}

/*
 * [mcl-debug]
 * Fix the SS ID.
 */
function _drush_mcl_debug_xxx_marker_ss_id() {
  global $user;

  // Some variables.
  $mcl_user     = MCL_USER::byKey(array('name' => 'ltaein'));
  $user_dir     = $mcl_user->getUserDir();
  $module_path  = drupal_get_path('module', 'mcl');
  mcl_print("Debuging", 1, 3);

  // Read ss.txt.
  $infile   = "$module_path/DATA_FILES/ss.txt";
  $outfile  = $infile . '.out';
  if (!($fdr = fopen($infile, 'r'))) {
    return FALSE;
  }
  if (!($fdw = fopen($outfile, 'w'))) {
    return FALSE;
  }
  while (!feof($fdr)) {
    $line = trim(fgets($fdr));
    if (!$line) {
      continue;
    }
    $sql = "
      SELECT F.uniquename FROM chado.feature F
      WHERE LOWER(F.uniquename) = LOWER('$line')
    ";
    $uniquename = db_query($sql)->fetchField();
    if (!$uniquename) {
      $sql = "
        SELECT F.uniquename
        FROM chado.feature F
          INNER JOIN chado.feature_synonym FS on FS.feature_id = F.feature_id
          INNER JOIN chado.synonym S on S.synonym_id = FS.synonym_id
          INNER JOIN chado.cvterm C on C.cvterm_id = S.type_id
        WHERE LOWER(S.name) = LOWER('$line') AND C.name = 'ss_id'
      ";
      $uniquename = db_query($sql)->fetchField();
    }
    if (!$uniquename) {
      print "$line does not exist in feature\n";
      fputs($fdw, "$line\t$uniquename\n");
      continue;
    }
    if (!$uniquename) {
      $uniquename = '###';
    }
    fputs($fdw, "$line\t$uniquename\n");
  }
  fclose($fdr);
  fclose($fdw);
}

/*
 * [mcl-debug]
 * Deletes duplicated feature.
 */
function _drush_mcl_debug_20181221($module_path) {

  //gdr_map_postion_dups.csv
  $filename = 'GDR_DUP_markers.csv';
  $infile   = "$module_path/TEST_FILES/$filename";
  $outfile  = "$infile.out";
  if (!($fdr = fopen($infile, 'r'))) {
    return FALSE;
  }
  if (!($fdw = fopen($outfile, 'w'))) {
    return FALSE;
  }
  $count = 0;
  while (!feof($fdr)) {
    $line_arr = fgetcsv($fdr);
    if (!empty($line_arr)) {
      $count++;

      $locus_id = $line_arr[0];
      $fid_del  = $line_arr[1];
      $fid_keep = $line_arr[3];
      $sql = "
        SELECT feature_relationship_id
        FROM chado.feature_relationship
        WHERE subject_id = $locus_id AND object_id = $fid_del
      ";
      $feature_relationship_id = db_query($sql)->fetchField();
      fputs($fdw, "DELETE FROM chado.feature_relationship WHERE feature_relationship_id = $feature_relationship_id;");
    }
  }
  fclose($fdr);
  fclose($fdw);
}

/*
 * [mcl-debug]
 */
function _drush_mcl_debug_20181210_xxx($module_path) {

  //gdr_map_postion_dups.csv
  $filename = 'gdr_map_postion_dups.csv';
  $infile   = "$module_path/TEST_FILES/$filename";
  $outfile  = "$infile.out";
  if (!($fdr = fopen($infile, 'r'))) {
    return FALSE;
  }
  if (!($fdw = fopen($outfile, 'w'))) {
    return FALSE;
  }
  $count = 0;
  while (!feof($fdr)) {
    $line_arr = fgetcsv($fdr);
    if (!empty($line_arr)) {
      $count++;

      $m_feature_ids  = $line_arr[6];
      $tmp = explode(":", $m_feature_ids);
      $locus_ids      = $line_arr[7];
      $tmp_l = explode(":", $locus_ids);

      $all_aliases = array();
      $all_markers = array();
      foreach ($tmp as $idx => $m_feature_id) {
        $marker = MCL_CHADO_FEATURE::byID($m_feature_id);
        $info = array(
            'feature_id' => $m_feature_id,
            'locus_id'   => $tmp_l[$idx],
            'alias'      => array(),
        );
        $aliases = $marker->getAlias();
        if (!empty($aliases)) {
          $alias_tmp = array();
          foreach ($aliases as $alias) {
            $alias_tmp[strtolower($alias)] = TRUE;
          }
          $info['alias'] = $alias_tmp;
        }
        $all_markers[strtolower($marker->getUniquename())] = $info;
      }
      $conflicts = array();
      foreach ($all_markers as $lc_uniquename_i => $info_i) {
        foreach ($all_markers as $lc_uniquename_j => $info_j) {
          if ($lc_uniquename_i != $lc_uniquename_j) {
            if (array_key_exists($lc_uniquename_i, $info_j['alias'])) {
              $fid_i = $info_i['feature_id'];
              $lid_i = $info_i['locus_id'];
              $fid_j = $info_j['feature_id'];
              $lid_j = $info_j['locus_id'];
              $key = "$fid_i-$fid_j";
              if (!array_key_exists($key, $conflicts)) {
                $key_2 = "$fid_j-$fid_i";
                if (!array_key_exists($key_2, $conflicts)) {
                  $conflicts[$key] = "$lid_i\t$fid_i\t$lc_uniquename_i\t$lid_j\t$fid_j\t$lc_uniquename_j";
                }
              }
            }
          }
        }
      }

      if (!empty($conflicts)) {
        fputs($fdw, "=================\n");
        foreach ($conflicts as $key => $values) {
          fputs($fdw, "$values\n");
        }
      }
    }
  }
  fclose($fdr);
  fclose($fdw);
}

/*
 * [mcl-debug]
 */
function _drush_mcl_debug_xxxx_process($file_key, $arr, $name) {

  /*
   * mcl_debug_process('ncgc', $sid_ncgc, 'name');
  mcl_debug_process('cn', $sid_cn, 'name');
  mcl_debug_process('uz', $sid_uz, 'name');
  mcl_debug_process('tn', $name_tn, 'uniquename');
  mcl_debug_process('rbtn', $name_rbtn, 'uniquename');

   */
  $list = strtolower("'" . implode("','", $arr) . "'");
  $sql = "
    select S.stock_id, S.name, S.uniquename, O.genus, O.species, C.name AS type, M.uniquename AS maternal, P.uniquename AS paternal, string_agg(A.value, ';') AS alias,
    CUL.value AS cultivar, PED.value AS pedigree, ORI.value AS origin
    from chado.stock S
      inner join chado.organism O on O.organism_id = S.organism_id
      inner join chado.cvterm C on C.cvterm_id = S.type_id
      left join (
        select S.uniquename, SR.object_id, SR.subject_id
        from chado.stock S
          inner join chado.stock_relationship SR on SR.subject_id = S.stock_id
        where SR.type_id = 36961
      ) M on M.object_id = S.stock_id
      left join (
        select S.uniquename, SR.object_id, SR.subject_id
        from chado.stock S
          inner join chado.stock_relationship SR on SR.subject_id = S.stock_id
        where SR.type_id = 36962
      ) P on P.object_id = S.stock_id
      left join (
        select SP.value, SP.stock_id
        from chado.stockprop SP
        where SP.type_id = 36977
      ) A on A.stock_id = S.stock_id
      left join (
        select SP.value, SP.stock_id
        from chado.stockprop SP
        where SP.type_id = 36950
      ) CUL on CUL.stock_id = S.stock_id
      left join (
        select SP.value, SP.stock_id
        from chado.stockprop SP
        where SP.type_id = 36958
      ) PED on PED.stock_id = S.stock_id
      left join (
        select SP.value, SP.stock_id
        from chado.stockprop SP
        where SP.type_id = 36970
      ) ORI on ORI.stock_id = S.stock_id
      Where LOWER(S.$name) IN ($list)
      group by S.stock_id, S.name, S.uniquename, O.genus, O.species, C.name, M.uniquename, P.uniquename, CUL.value, PED.value, ORI.value
      order by S.name
  ";
  $results = db_query($sql);
  $data = array();
  while ($obj = $results->fetchObject()) {
    $key = ($name == 'name') ? $obj->name : $obj->uniquename;
    $data[$key] = array(
      'name'        => $obj->name,
      'uniquename'  => $obj->uniquename,
      'type'        => $obj->type,
      'genus'       => $obj->genus,
      'species'     => $obj->species,
      'maternal'    => $obj->maternal,
      'paternal'    => $obj->paternal,
      'alias'       => $obj->alias,
      'cultivar'    => $obj->cultivar,
      'pedigree'    => $obj->pedigree,
      'origin'      => $obj->origin,
    );
  }
  $outfile = "/main/sites/hort-wp/www.cottongen.org/htdocs/sites/all/modules/mcl/test/$file_key.csv";
  if (!($fdw = fopen($outfile, 'w'))) {
    return FALSE;
  }
  foreach ($data as $key => $prop) {
    fputcsv($fdw, $prop);
  }
  fputcsv($fdw, array());
  fputcsv($fdw, array());
  fputcsv($fdw, array());
  foreach ($arr as $key) {

    if (!array_key_exists($key, $data)) {
      print "ERROR : $key not found in DB.\n\n";
      fputcsv($fdw, array($key));
      continue;

    }
    $row = array($data[$key]['name'], $data[$key]['uniquename'], $data[$key]['genus'], $data[$key]['species']);
    fputcsv($fdw, $row);
  }
  print "\n\n$outfile  created\n";
  fclose($fdw);
}
