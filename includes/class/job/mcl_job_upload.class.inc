<?php
/**
 * The declaration of MCL_JOB_UPLOAD class.
 *
 */
class MCL_JOB_UPLOAD extends MCL_JOB {

 /**
  *  Class data members.
  */
  protected $upload_file  = NULL;
  protected $csv_dir      = NULL;
  private   $excludes     = array();
  private   $force        = FALSE;

  /**
   * @see MCL_JOB::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);

    // Adds status for an uploading job.
    $this->status_label['20']         = 'copying';
    $this->status_label['30']         = 'entry error checking';
    $this->status_label['40']         = 'data error checking';
    $this->status_label['60']         = 'uploading';
    $this->status_label['80']         = 'callback';
    $this->status_int['copying']      = 20;
    $this->status_int['entry error']  = 30;
    $this->status_int['data error']   = 40;
    $this->status_int['uploading']    = 60;
    $this->status_int['callback']     = 80;

    // Creates a MCL_FILE object for the uploaded file.
    $file_id = $this->getParamByKey('file_id');
    if ($file_id) {
      $this->upload_file = MCL_FILE::byKey(array('file_id' => $file_id));
    }

    // Sets job properties.
    $this->force = $this->getPropByKey('force');
    $this->csv_dir = $this->getPropByKey('csv_dir');

    // Creates folders for CSV files.
    if ($this->csv_dir && !file_exists($this->csv_dir)) {
      mcl_create_dir($this->csv_dir);
    }
  }

  /**
   * @see MCL_JOB::byID()
   */
  public static function byID($job_id) {
    return self::byKey(array('job_id' => $job_id));
  }

  /**
   * @see MCL_JOB::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
      return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * @see MCL_JOB::insert()
   */
  public function insert() {
    if (parent::insert()) {
      // Creates folders for CSV files.
      $csv_dir = $this->job_dir . '/csv';
      if (!file_exists($csv_dir)) {
        mcl_create_dir($csv_dir);
      }
      $this->setCSVDir($csv_dir);
      return $this->update();
    }
    return FALSE;
  }

  /**
   * @see MCL_JOB::createNewLogDirs()
   */
  public function createNewLogDirs() {
    $dir_new_data = $this->getNewDataLogDir();
    $dir_prevous  = $this->getPreviousLogDir();
    $dir_recent   = $this->getRecentLogDir();

    // Creates 'new_data' and 'previous' folders if not exists.
    if (!mcl_create_dir($dir_prevous)) {
      return FALSE;
    }
    if (!mcl_create_dir($dir_new_data)) {
      return FALSE;
    }

    // Creates or re-create 'recent' folder.
    if (file_exists($dir_recent)) {
      $cmd = "rm -rf $dir_recent";
      exec($cmd, $output, $return_var);
      if ($return_var) {
        return FALSE;
      }
    }
    if (!mcl_create_dir($dir_recent)) {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * @see MCL_JOB::KeepLogs()
   */
  public function KeepLogs() {
    $dir_previous = $this->getPreviousLogDir();
    $dir_recent   = $this->getRecentLogDir();

    // Copies 'recent' folder to previous'.
    $target_dir = "$dir_previous/" . date("Y-m-d-G-i-s");
    $cmd = "mkdir $target_dir;cp -r \"$dir_recent\" \"$target_dir\"";
    exec($cmd, $output, $return_var);
    if ($return_var) {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * @see MCL_JOB::getLogDirByKey()
   */
  public function getLogDirByKey($key, $status, $parent_folder = 'recent') {
    return $this->getLogDir() . "/$parent_folder/$status-$key/$filename";
  }

  /**
   * @see MCL_JOB::getLogFileByKey()
   */
  public function getLogFileByKey($key, $parent_folder = 'recent') {

    // Gets and returns the filepath of the log file.
    $status_int = $this->getStatus();
    $sub_folder = "$parent_folder/$status_int-$key";
    $filename = sprintf("%d-%s-%s.log", $status_int, $key, $this->getLogName());
    return $this->getLogDir() . "/$sub_folder/$filename";
  }

  /**
   * @see MCL_JOB::noError()
   */
  public function noError() {
    $error_log = $this->getLogFileByKey('E');
    return !file_exists($error_log);
  }

  /**
   * @see MCL_JOB::updateMsg()
   */
  public function updateMsg($key, $msg, $line_no = 0) {

    // Sets the message.
    $line_info = ($line_no > 0) ? " at line $line_no\n" : "\n";
    $msg .= $line_info;

    // Updates the log file.
    $log_file = $this->getLogFileByKey($key);
    if (file_exists($log_file)) {
      file_put_contents($log_file, $msg, FILE_APPEND);
    }
    else {
      $pp_dir = $this->getLogDir() . "/recent";
      if (mcl_create_dir($pp_dir)) {
        $status_int = $this->getStatus();
        $p_dir = "$pp_dir/$status_int-$key";
        if (mcl_create_dir($p_dir)) {
          file_put_contents($log_file, $msg);
        }
      }
    }
  }

  /**
   * Moves all new data log to the new data folder.
   *
   * @return array
   */
   public function moveNewDataLog() {

    // Gets all the new data log files.
    $new_data_dir = $this->getNewDataLogDir();
    $new_log_dir  = $this->getLogDirByKey('N', 40);
    $date_str = date("Y-m-d-G-i-s");
    foreach (glob("$new_log_dir/*.log") as $filepath) {
      if (preg_match("/(.*)\.log$/", $filepath, $matches)) {
        $tmp = $matches[1];
        $filename = basename("$tmp-$date_str" . '.log');
        $cmd = "cp \"$filepath\" \"$new_data_dir/$filename\"";
        exec($cmd);
      }
    }
  }

  /**
   * @see MCL_JOB::getJobInfoTable()
   */
  public function getJobInfoTable() {

    // Gets MCL_FILE.
    $file = $this->getUploadFile();

    // Gets and set job information.
    $file_link = "No file found";
    if (file_exists($file->getFilepath())) {
      $file_link = l($file->getFilename(), file_create_url($file->getUri()));
    }
    $rows = array(
      array('<b>Name</b>', $this->getName()),
      array('<b>Type</b>', $this->getType()),
      array('<b>Status</b>', $this->getStatusLabel()),
      array('<b>Uploaded File</b>', $file_link),
      array('<b>Filesize</b>', $file->getFilesize()),
      array('<b>Submit Date</b>', $this->getSubmitDate()),
    );

    // Table variables.
    $table_vars = array(
      'header'      => NULL,
      'rows'        => $rows,
      'attributes'  => array('style' => 'max-width:500px;'),
    );
    return theme('table', $table_vars);
  }

  /**
   * @see MCL_JOB::getJobInfo()
   */
  public function getJobInfo() {

    // Gets directories.
    $dirs = "\n  Job     = " . $this->getJobDir() . "\n  Working = " . $this->getWorkingDir() .
            "\n  File    = " . $this->getFileDir() . "\n  CSV     = " . $this->getCSVDir() .
            "\n  Log     = " . $this->getLogDir() . "\n";

    // Adds base information.
    $info = array(
      'Job ID'      => $this->job_id,
      'Job Name'    => $this->getName(),
      'Status'      => $this->getStatusLabel(),
      'Directories' => $dirs,
      'Submit Date' => $this->getSubmitDate(),
    );

    // Adds re-run command.
    $rerun_cmd = $this->getPropByKey('rerun_cmd');
    if ($rerun_cmd) {
      $info['rerun_cmd'] = $rerun_cmd;
    }
    return $info;
  }

  /**
   * @see MCL_JOB::run()
   */
  public function run() {

    // Initializes the counter.
    $this->_initDataCounter();

    // Sets the transaction.
    $transaction = NULL;
    $trans = $this->getTransaction();
    if ($trans) {
      $transaction = db_transaction();
    }

    // Runs uploading job.
    try {
      $this->resetProgress();
      $job_id = $this->getJobID();

      // Initializes the job.
      if ($this->status == 10) {
        mcl_print("==================================", 1, 1, FALSE);
        mcl_print("Job [" . $this->getJobID() . "] has been started", 1);
        mcl_print("----------------------------------", 1, 2, FALSE);
        $this->setProgress("The job has been started");
        $this->setStatus(20);
        $this->update();
      }

      // Extracts CVS files and copy them.
      if ($this->status == 20) {

        // Copies all CSV files to the CSV file directory.
        mcl_print("==================================", 1, 1, FALSE);
        mcl_print("Phase I : Copying CSV files", 1);
        mcl_print("----------------------------------", 1, 1, FALSE);
        $this->setProgress("Start copying files ...");
        if ($this->_copyCSV()) {
          mcl_print("CSV file created", 1, 2);
          $this->setProgress("Done copying CSV files");
          $this->setStatus(30);
          $this->update();
        }
        else {
          throw new Exception("Error : Failed to copy CSV files.");
        }
      }

      // Runs entry error check.
      if ($this->status == 30) {

        // Runs entry error check each CSV files.
        mcl_print("==================================", 1, 1, FALSE);
        mcl_print("Phase II : Running entry error check", 1);
        mcl_print("----------------------------------", 1, 1, FALSE);
        $this->setProgress("Started running entry error check");
        if ($this->_runErrorCheckEntry()) {
          mcl_print("Entry error check completed without error", 1, 2);
          $this->setProgress("Done entry error check");
          $this->setStatus(40);
          $this->update();
        }
        else {
          throw new Exception("\n\tEntry Error exists : please check error log file.\n\n\n");
        }
      }

      // Uploads the data file. The data error check is conducted before
      // uploading data.
      if ($this->status == 40) {

        // Uploads CSV files into database.
        mcl_print("==================================", 1, 1, FALSE);
        mcl_print("Phase III : Uploading data files", 1);
        mcl_print("----------------------------------", 1, 1, FALSE);
        $this->setProgress("Started uploading data template files");
        if ($this->_uploadCSV()) {
          mcl_print("Data uploading completed without error", 1, 2);
          $this->setStatus(60);
          $this->update();
        }
        else {
          throw new Exception("\n\tUploading data failed : please check error log file.\n\n\n");
        }
      }
    }
    catch (Exception $e) {

      // If error occurs, rollbacks transactions.
      if ($transaction) {
        $transaction->rollback();
      }

      // Sets the status as 'FAILED'.
      $this->updateStatus(-200);

      // Keeps the error message.
      watchdog('mcl', $e->getMessage(), array(), WATCHDOG_ERROR);
      return FALSE;
    }

    // Runs callback drush commands.
    if ($this->status == 60) {

      // Checks if there are callback functions in job paramters.
      $callbacks = $this->_getCallbacks();
      if (!empty($callbacks)) {

        // Runs callaback drush commands.
        mcl_print("==================================", 1, 1, FALSE);
        mcl_print("Phase IV : Runs callback commands", 1);
        mcl_print("----------------------------------", 1, 1, FALSE);
        $this->setProgress("Started running callback commands");
        if ($this->_runCallbacks($callbacks)) {
          mcl_print("Done callbacks", 1, 2);
        }
        else {
          mcl_print("Running callback commands failed : please check error log file.", 1, 3);
        }
      }
      $this->setStatus(80);
      $this->update();
    }

    // Finalizing the uploading job.
    if ($this->status == 80) {

      // No error has occured. So copy the logs file for new data.
      $this->moveNewDataLog();

      // Saves the counter.
      $this->_keepDataCounter();

      // Updats the status to be 100.
      $this->updateStatus(100);
    }
    return TRUE;
  }

  /**
   * Initiates the data counter.
   */
  private function _initDataCounter() {
     $GLOBALS['MCL_DATA_COUNTER'] = array(
       'NEW_DATA' => array(),
       'DUP_DATA' => array(),
       'UPDATE_DATA' => array(),
     );
  }

  /**
   * Keeps the data counter.
   *
   * @return boolean
   */
  private function _keepDataCounter() {

    // Gets the previous counter.
    $mcl_data_count = $this->getPropByKey('MCL_DATA_COUNT');
    if (!is_array($mcl_data_count)) {
      $mcl_data_count = array();
    }

    // Gets the current counter and updates it. Then save it.
    if (array_key_exists('MCL_DATA_COUNTER', $GLOBALS)) {
      if (!empty($GLOBALS['MCL_DATA_COUNTER'])) {
        $date_str = date("Y-m-d-G-i-s");
        $mcl_data_count[$date_str] = $GLOBALS['MCL_DATA_COUNTER'];
        $this->setPropByKey('MCL_DATA_COUNT', $mcl_data_count);
        return $this->update();
      }
    }
    return TRUE;
  }

  /**
   * Copy all CSV files to the CSV file directory. If an uploaded
   * file is Excel, convert it to CSV file(s).
   *
   * @return boolean
   */
  private function _copyCSV() {

    // Sets the log name.
    $this->setLogName('COPY_CSV');

    // Gets the upload file.
    $filepath   = $this->upload_file->getFilepath();
    $file_type  = $this->upload_file->getType();

    // If the uploaded file is excel, convert all the sheets in the excel
    // file into csv files.
    if ($file_type == 'excel') {

      // Converts Excel file to CSV files.
      if (!$this->_convertExcel2CSV($filepath)) {
        $this->updateMsg('E', "Failed to convert to CVS files.");
        return FALSE;
      }
    }

    // Uncompresses the files and copy them to to CSV folder.
    else if (preg_match("/^(zip|gz)$/", $file_type)) {

      // Unzips and copy them to the CSV folder.
      if (!$this->_decompressToCSV($file_type, $filepath)) {
        $this->updateMsg('E', "Failed to unzip the input file.");
        return FALSE;
      }

      // Registers all file in the CSV folder.
      $no_error = TRUE;
      foreach (glob($this->csv_dir . '/*.csv') as $filepath) {

        // Checks the encodes. Convert the encoding to 'UTF-8' if not.
        $this->convertToUTF8($filepath, TRUE);

        // Gets the template name.
        $template = $this->_getTemplate($filepath);
        if ($template == '') {
          $this->updateMsg('E', "The template name can not be found.");
          $no_error = FALSE;
          continue;
        }

        // Checks the templates. If not defined yet, skip it.
        if (!MCL_TEMPLATE::getByTemplate($template)) {
          $this->updateMsg('W', "Warn : Template ($template) has not defined yet, so it was skipped.");
        }

        // Renames the file and copies it to the csv directory.
        $destination  = $this->csv_dir . '/' . $template . '.csv';
        if (rename($filepath, $destination)) {
          $this->_registerCSVFile($destination);
        }
        else {
          $this->updateMsg('E', "Failed to rename csv file [$template].");
          $no_error = FALSE;
        }
      }
      if (!$no_error) {
        return FALSE;
      }
    }

    // Copies a single CSV file to CSV folder.
    else if ($file_type == 'csv' || $file_type == 'trt') {

      // Checks the encodes. Convert the encoding to 'UTF-8' if not.
      $filepath_utf8 = $this->convertToUTF8($filepath, FALSE);
      if ($filepath_utf8) {
        $filepath = $filepath_utf8;
      }

      // Gets the template name.
      $template = $this->_getTemplate($filepath);
      if ($template == '') {
        $this->updateMsg('E', "The template name can not be found.");
        return FALSE;
      }

      // Checks the templates. If not defined yet, skip it.
      if (!MCL_TEMPLATE::getByTemplate($template)) {
        $this->updateMsg('W', "Warn : Template ($template) has not defined yet, so it was skipped.");
        return TRUE;
      }

      // Renames the file and copy or move it to the csv directory.
      $destination  = $this->csv_dir . '/' . $template . '.csv';
      if ($filepath_utf8) {
        if (rename($filepath, $destination)) {
          $this->_registerCSVFile($destination);
        }
      }
      else {
        if (copy($filepath, $destination)) {
          $this->_registerCSVFile($destination);
        }
      }
    }

    // Error : Invalid file type.
    else {
      $this->updateMsg('E', "Invalid file type ($filepath).");
    }
    return $this->noError();
  }

  /**
   * Converts the encoding of the file to UTF-8. If the file is UTF-8,
   * do nothing and returns empty string. If not UTF-8, convert it to UTF-8
   * and retrun the file path of the newly created file. If $overwrite is on,
   * return nothing and rename the newly created file to the original.
   *
   * @param string $filepath
   * @param boolen $overwrite
   *
   * @return string
   */
  public function convertToUTF8($filepath, $overwrite = FALSE) {
    $new_filepath = '';
    if (file_exists($filepath)) {

      // Gets the current encoding.
      $cur_encode = mcl_detect_file_encoding($filepath);
      if (!preg_match("/utf8/i", $cur_encode)) {

        // Converts the encoding.
        $new_filename = 'utf8-' . basename($filepath);
        $new_filepath = dirname($filepath) . "/$new_filename";
        $cmd = "iconv -f $cur_encode -t UTF-8 $filepath -o $new_filepath;";
        if ($overwrite) {
          $cmd .= "mv \"$new_filepath\" \"$filepath\"";
          $new_filepath = '';
        }
        exec($cmd);
      }
    }
    return $new_filepath;
  }

  /**
   * Gets and returns the template name. Checks the name in the job parameters
   * and then gets it from the filepath.
   *
   * @param string $filepath.
   *
   * @return string
   */
  private function _getTemplate($filepath) {

    // Checks and gets the template from the prop.
    $template = $this->getParamByKey('template');

    // Gets the first line of the file.
    if (!$template) {
      if (file_exists($filepath)) {
        $line = trim(fgets(fopen($filepath, 'r')));
        if (preg_match("/^#([a-zA-Z_]+)/", $line, $matches)) {
          $template = $matches[1];
        }
      }
    }
    return $template;
  }

  /**
   * Register a CSV file.
   *
   * @param string $filepath.
   * @param string $template.
   *
   * @return boolean
   */
  private function _registerCSVFile($filepath, $template = '') {

    // Gets the template name if not provided.
    if (!$template) {

      // Gets the template name.
      $template = $this->_getTemplate($filepath);
      if ($template == '') {
        $this->updateMsg('E', "The template name can not be found.");
        return FALSE;
      }

      // Checks the templates. If not defined yet, skip it.
      if (!MCL_TEMPLATE::getByTemplate($template)) {
        $this->updateMsg('W', "Warn : Template ($template) has not defined yet, so it was skipped.");
        return TRUE;
      }
    }

    // Creates MCL_FILE and adds it to this job if not exists.
    if (!$this->existCSV($template)) {
      $csv_file = $this->_createMCL_FILE_CSV($filepath, $template);
      if ($csv_file->insert()) {

        // Adds a new CSV file.
        if (!$this->_addCSVFile($csv_file)) {
          $this->updateMsg('E', "Failed to add CSV file.");
        }
      }
      else {
        $this->updateMsg('E', "Failed to create CSV file.");
      }
    }
    return TRUE;
  }

  /**
   * Decompresses the compressed file and copy all CSV files to CSV folder.
   *
   * @param string $file_type.
   * @param string $filepath.
   *
   * @return boolean
   */
  private function _decompressToCSV($file_type, $filepath) {

    // Decompresses the files and copy to the CSV folder.
    $csv_folder = $this->csv_dir;
    $cmd = '';

    // zip file.
    if ($file_type == 'zip') {
      $cmd = "unzip -j -o $filepath -d " . $this->csv_dir;
    }

    // gz file.
    else if ($file_type == 'gz') {
      $filepath_unzip = mcl_change_ext($filepath, '');
      $cmd = "gzip -dkf \"$filepath\";mv \"$filepath_unzip\" $csv_folder";
    }
    if ($cmd) {
      exec($cmd, $output, $return_val);
      return !$return_val;
    }
    return FALSE;
  }

  /**
   * Converts Exel file to CSV files.
   *
   * @param string $filepath.
   *
   * @return boolean
   */
  private function _convertExcel2CSV($filepath) {

    // PhpSpreadsheet dependencies and module, shared with excel/mcl_excel.class.inc
    require_once(dirname(__FILE__).'/../phpspreadsheet.php');

    // Sets the line ending.
    $line_ending = '&&&&&';

    // Gets Excel file type [Excel5 | ].
    $file_type = \PhpOffice\PhpSpreadsheet\IOFactory::identify($filepath);

    // Gets the default precision.
    $def_precision = ini_get('precision');

    // Read input Excel file.
    $reader = \PhpOffice\PhpSpreadsheet\IOFactory::createReader($file_type);
    $reader->setReadDataOnly(false);
    $excel = $reader->load($filepath);
    $num_sheets = $excel->getSheetCount();
    print "\tExcel has $num_sheets sheets\n\n";

    // Re-set the precision. It seems that PHPExcel changed the precision, keep just in case.
    ini_set('precision', $def_precision);

    // Empties CSV folder.
    mcl_empty_dir($this->csv_dir);

    // Gets the 'template-rename' array if it exists in job parameter.
    $param = $this->getParamByKey('template_rename');
    $template_map = array();
    if ($param) {
      $template_map = $param;
    }

    // Save each sheet as CSV file.
    $writer = \PhpOffice\PhpSpreadsheet\IOFactory::createWriter($excel, 'Csv');
    $dup_templates = array();
    for ($i = 0; $i < $num_sheets; $i++) {
      $this->setProgress("Copying a worksheet... [".($i+1)." of $num_sheets worksheets]");
      $excel->setActiveSheetIndex($i);
      $template = strtolower($excel->getActiveSheet()->getTitle());
      print "\tcopying $template\n\n";

      // Renames the template if necessary.
      if (array_key_exists($template, $template_map)) {
        $template = $template_map[$template];
      }

      // Checks for dupliacted sheet name.
      if (array_key_exists($template, $dup_templates)) {
        $this->updateMsg('E', "There are duplicated sheet names $template in your file.");
        return FALSE;
      }
      else {
        $dup_templates[$template] = 1;
      }

      // Validates sheet name.
      $tmpl = MCL_TEMPLATE::getByTemplate($template);
      if ($tmpl) {

        // Update the cell values.
        $this->_updateCellValues($excel);

        // Sets CSV filepath.
        $csv_filepath = $this->csv_dir . "/$template" . '.csv';

        // Write to a CSV file.
        $writer->setSheetIndex($i);
        $writer->setLineEnding($line_ending);
        $writer->setPreCalculateFormulas(true);
        $writer->save($csv_filepath);

        // Replace newline characters in cell value with white spaces.
        $file_contents = file_get_contents($csv_filepath);
        $file_contents = str_replace("\n", ' ', $file_contents);
        $file_contents = str_replace($line_ending, "\n", $file_contents);
        file_put_contents($csv_filepath,  $file_contents);

        // Opens the cvs file.
        if (!($fdr = fopen($csv_filepath, 'r'))) {
          return FALSE;
        }

        // Adds the template name at begining of the file.
        $file_contents = "#$template\n";

        // Cleanup the contents of the file.
        $header_line = TRUE;
        while (!feof($fdr)) {
          $line = trim(fgets($fdr));

          // Replaces the labels with their column name.
          if ($header_line) {
            $header_line = FALSE;
            $line = $tmpl->cleanupHeaders($line);
          }

          // Removes white space : &nbsp;
          $line = preg_replace('/\xc2\xa0/', ' ', $line);

          // Exits if no data in the line.
          $check = preg_replace("/[\",\s]/", '', $line);
          if ($check == '') {
            break;
          }

          // Adds the data line.
          $file_contents .= "$line\n";
        }
        fclose($fdr);

        // Updates the contents of the file.
        file_put_contents($csv_filepath,  $file_contents);

        // Creates MCL_FILE if not exists.
        if (!$this->existCSV($template)) {
          $csv_file = $this->_createMCL_FILE_CSV($csv_filepath, $template);
          if ($csv_file && $csv_file->insert()) {

            // Adds a new CSV file.
            if (!$this->_addCSVFile($csv_file)) {
              $this->updateMsg('E', "Failed to add CSV file.");
              return FALSE;
            }
          }
          else {
            $this->updateMsg('E', "Failed to create CSV file.");
            return FALSE;
          }
        }
      }
      else {

        // Skips the 'header(s)' templates.
        if (!preg_match("/^header/i", $template)) {
          $class_name = 'MCL_TEMPLATE_' . strtoupper($template);
          $msg = "\nThe sheet name '$template' is not a valid template name.\nIt could be the following reason.\n\n";
          $msg .= "  1. wrong spelling.\n";
          $msg .= "  2. $class_name is not defined.\n\n";
          $msg .= "'$template' is skipped.\n\n";
          $this->updateMsg('W', $msg);
        }
      }
    }

    // Updates csv_files in job property.
    if (!$this->updateCSV($dup_templates)) {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Updates the cell values.
   *
   * boolean => 'TRUE' or 'FALSE'
   * date    => integer to date.
   *
   * @param string $filepath
   * @param string $template
   *
   * @return MCL_FILE_CSV
   */
  private function _updateCellValues($excel) {

    // Gets the active sheet.
    $sheet_data   = $excel->getActiveSheet();
    $max_row      = $sheet_data->getHighestRow();
    $max_col      = $sheet_data->getHighestColumn();
    $max_col_idx  = \PhpOffice\PhpSpreadsheet\Cell\Coordinate::columnIndexFromString($max_col);
    $row_char     = 'A';
    for ($row = 1; $row <= $max_row; $row++) {
      $row_values = '';
      for ($col = 0; $col < $max_col_idx; ++$col) {
        $cell   = $sheet_data->getCellByColumnAndRow($col, $row);
        $value  = $cell->getValue();
        $type   = $cell->getDataType();

        // Updates the value.
        $row_values .= trim($value);
        if ($type == 'b') {
          $cell->setDataType(\PhpOffice\PhpSpreadsheet\Cell\DataType::TYPE_STRING);
          $bool = ($value) ? 'TRUE' : 'FALSE';
          $cell->setValue($bool);
        }
        else if ($type == 'f') {
          $value = $cell->getCalculatedValue();
          $cell->setValue($value);
        }
        else if (\PhpOffice\PhpSpreadsheet\Shared\Date::isDateTime($cell)) {
          $cell->getStyle()->getNumberFormat()->setFormatCode(\PhpOffice\PhpSpreadsheet\Style\NumberFormat::FORMAT_GENERAL);
          if (preg_match("/^\d{5,}$/", $value)) {
            $date_value = mcl_format_date($value, "m/d/Y");
            $cell->setValue($date_value);
          }
          else {
            $cell->setValue($value);
          }
        }
        else {
          // Sets the format of the rest of cell to be 'General'.
          $cell->getStyle()->getNumberFormat()->setFormatCode(\PhpOffice\PhpSpreadsheet\Style\NumberFormat::FORMAT_GENERAL);
          //$sheet_data->getStyle($row_char.$col)->getNumberFormat()->setFormatCode(\PhpOffice\PhpSpreadsheet\Style\NumberFormat::FORMAT_GENERAL);
        }
      }
      $row_char++;
      if (!$row_values) {
        return;
      }
    }
  }

  /**
   * Creates a MCL_FILE_CSV object.
   *
   * @param string $filepath
   * @param string $template
   *
   * @return MCL_FILE_CSV
   */
  private function _createMCL_FILE_CSV($filepath, $template) {

    // CSV file properites.
    $prop = array(
      'template' => $template,
    );

    // Create a MCL_FILE_CSV object.
    $details = array(
      'filepath'    => $filepath,
      'filename'    => basename($filepath),
      'filesize'    => filesize($filepath),
      'uri'         => file_build_uri(mcl_get_rel_filepath($filepath)),
      'submit_date' => date("Y-m-d G:i:s"),
      'user_id'     => $this->getUserID(),
      'job_id'      => $this->getJobID(),
      'prop'        => json_encode($prop),
    );
    return new MCL_FILE_CSV($details);
  }

  /**
   * Adds a new CSV file and updates 'csv_files' in mcl_job.prop.
   *
   * @param MCL_FILE $csv_file
   *
   * @return boolean
   */
  private function _addCSVFile(MCL_FILE $csv_file) {

    // Adds the CSV file to csv_files array in job properties.
    $csv_files = $this->getPropByKey('csv_files');
    if (!is_array($csv_files)) {
      $csv_files = array();
    }

    // Updates $csv_files array.
    foreach ($csv_files as $file_id => $csv_filename) {
      if ($csv_filename == $csv_file->getFilename()) {
        unset($csv_files[$file_id]);
      }
    }

    // Updates $this->csv_files.
    $csv_files[$csv_file->getFileID()] = $csv_file->getFilename();
    $this->setPropByKey('csv_files', $csv_files);
    return $this->update();
  }

  /**
   * Performs entry error check on CSV files.
   *
   * @return boolean
   *   Return TRUE if no error.
   */
  private function _runErrorCheckEntry() {
    $no_error_flag = TRUE;

    // Gets all CSV files.
    $csv_files = $this->getCSVFiles();

    // Performs the error check.
    foreach ($csv_files as $mcl_file_id => $filename) {
      mcl_print("[$mcl_file_id] $filename", 1);
      mcl_print("- Running the entry error checking : [$mcl_file_id] $filename", 2);

      // Gets the MCL_FILE_CSV object.
      $csv_file = MCL_FILE_CSV::byKey(array('file_id' => $mcl_file_id));

      // Skips if the error checking has been completed.
      if ($csv_file->hasPassed($this->status_int['entry error']) && !$this->getForce()) {
        mcl_print("  > Skipped - Entry error has already been checked", 2, 2);
        continue;
      }

      // Sets the logs.
      $this->setLogName($csv_file->getTemplate());

      // Runs the entry error checking.
      $tmpl_obj = MCL_TEMPLATE::getTemplateClass($this, $csv_file);
      if (!$tmpl_obj) {
        $no_error_flag = FALSE;
        mcl_print("Cannot found template class", 2, 2);
        continue;
      }
      $this->setProgress("Running entry error check for " . $tmpl_obj->getTemplate());
      if ($tmpl_obj->runErrorCheckEntry()) {
        $csv_file->setPassed($this->status_int['entry error'], TRUE);
      }
      $this->setProgress("Done entry error check for " . $tmpl_obj->getTemplate());

      // Checks for errors.
      if (!$this->noError()) {
        $no_error_flag = FALSE;
      }
    }
    return $no_error_flag;
  }

  /**
   * Uploads CSV files.
   *
   * 1. Run data error check.
   * 2. Upload the CSV file.
   *
   * @return boolean
   */
  private function _uploadCSV() {
    $job_id = $this->getJobID();

    // Gets all CSV files.
    $csv_files = $this->getCSVFiles();

    // Upload the data.
    foreach ($csv_files as $mcl_file_id => $filename) {

      // Skips the excluded templates.
      if (in_array($filename , $this->excludes)) {
        continue;
      }

      // Creates a MCL_FILE_CSV object.
      $csv_file = MCL_FILE_CSV::byKey(array('file_id' => $mcl_file_id));
      $template = $csv_file->getTemplate();

      // Sets the logs.
      $this->setLogName($template);

      // Gets MCL_TEMPLATE object.
      $tmpl_obj = MCL_TEMPLATE::getTemplateClass($this, $csv_file);

      // Runs the data error checking. Skips if the error checking has been completed.
      mcl_print("[$mcl_file_id] $filename", 1);
      mcl_print("- Running the data error checking", 2);
      if ($csv_file->hasPassed($this->status_int['data error']) && !$this->getForce()) {
        mcl_print("  > Skipped - Data error has already been checked", 2, 2);
      }
      else {

        // Runs the data error checking.
        $this->setProgress("Running data error check for " . $tmpl_obj->getTemplate());
        if ($tmpl_obj->runErrorCheckData()) {
          $csv_file->setPassed($this->status_int['data error'], TRUE);
          mcl_print("Data error check passed", 2, 2);
        }
        $this->setProgress("Done data error check for " . $tmpl_obj->getTemplate());

        // Check if an error occured. If error found, return FALSE.
        if (!$this->noError()) {
          return FALSE;
        }
      }

      // Uploads CSV data file.
      mcl_print("- Uploading data file", 2);

      // Skips if the data has been uploaded.
      if ($csv_file->hasPassed($this->status_int['uploading']) && !$this->getForce()) {
        mcl_print("  > Skipped - Data has already been uploaded", 2, 2);
        continue;
      }

      // Upload the data file.
      $this->setProgress("Uploading data for " . $tmpl_obj->getTemplate());
      if ($tmpl_obj->uploadData()) {
        $csv_file->setPassed($this->status_int['uploading'], TRUE);
        mcl_print("Data have been uploaded", 2, 2);
        $this->setProgress("Done uploading data for " . $tmpl_obj->getTemplate());
      }
      else {
        $this->setProgress("Failed to upload data for " . $tmpl_obj->getTemplate());

        // Detetes the log for new data.
        $this->deleteLog('N');
      }

      // Check if an error occured. If error found, returns FALSE.
      if (!$this->noError($template)) {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Returns the callback functions in job parameters.
   *
   * @return array
   */
  private function _getCallbacks() {

    // Searches in job param.
    $callbacks_json = $this->getParamByKey('callbacks');
    if ($callbacks_json) {
      $callbacks = json_decode($callbacks_json);
      if (is_array($callbacks)) {

        // Validate callback functions. Returns valid callback functions.
        $valid_callbacks = array();
        foreach ($callbacks as $callback) {
          // TODO: validate callback function.
          //if (function_exists($func_name)) {
          if (TRUE) {
            $valid_callbacks []= $callback;
          }
          else {
            $this->updateMsg('W', "Warn : Callback function ($callback) does not exist. Running callbacks Aborted");
            return array();
          }
        }
        return $valid_callbacks;
      }
      else {
        $this->updateMsg('W', "Warn : Callback functions are defined but they can not be decoded. So they are skipped.");
      }
    }
    return array();
  }

  /**
   * Runs callback functions.
   *
   * @param array $callbacks
   *
   * @return boolean
   */
  private function _runCallbacks($callbacks) {

    // Sets stdout and stderr log file names.
    $stdout = $this->getStdFile('stdout');
    $stderr = $this->getStdFile('stderr');

    // Runs callback drush commands.
    $drush = bims_get_config_setting('bims_drush_binary');
    foreach ($callbacks as $cmd) {
      mcl_print("Runs callback function: $cmd", 1, 2);

     // $pid = exec("$drush $cmd > $stdout 2>$stderr  &", $output, $return_var);
      $pid = exec("$drush $cmd > /dev/null 2>/dev/null  &", $output, $return_var);


      if ($return_var) {
        $this->updateMsg('W', "Warn : Failed to run callback.");
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Return all CSV files in order.
   *
   * @return array
   */
  public function getCSVFiles() {

    // Gets all CSV files.
    $csv_files = $this->getPropByKey('csv_files');

    // Ranks CSV files.
    $csv_files_unsorted = array();
    foreach ((array)$csv_files as $file_id => $csv_filename) {

      // Gets the rank of csv file.
      $mcl_file_csv = MCL_FILE_CSV::byKey(array('file_id' => $file_id));
      if (!$mcl_file_csv) {
        $this->updateMsg('E', "File ($file_id) not found.");
        continue;
      }
      $rank = MCL_TEMPLATE::getRank($mcl_file_csv->getTemplate());
      $info = array(
        'file_id'       => $file_id,
        'csv_filename'  => $csv_filename,
        'rank'          => $rank
      );
      $csv_files_unsorted[] = $info;
    }

    // Sorts CSV files.
    usort($csv_files_unsorted,
      function($a, $b) {
        if ($a['rank'] == $b['rank']) {
          return $a['csv_filename'] < $b['csv_filename'];
        }
        return $a['rank'] > $b['rank'];
      }
    );
    $csv_files_sorted = array();
    foreach ($csv_files_unsorted as  $info) {
      $csv_files_sorted[$info['file_id']] = $info['csv_filename'];
    }
    return $csv_files_sorted;
  }

  /**
   * Checks if CSV file exists in csv_files array.
   *
   * @param string $template
   *
   * @return boolean
   */
  public function existCSV($template) {
    $csv_exist    = FALSE;
    $update_flag  = FALSE;

    // Go through all files in $csv_files. Updates if $file_id
    // does not exist in mcl_file table.
    $csv_files = $this->getPropByKey('csv_files');
    $csv_files_updated = array();
    if (is_array($csv_files)) {
      foreach ($csv_files as $file_id => $filename) {
        $csv_file = MCL_FILE_CSV::byKey(array('file_id' => $file_id));
        if ($csv_file) {
          $csv_files_updated[$file_id] = $filename;
          if ($template == $csv_file->getTemplate()) {
            $csv_exist = TRUE;
          }
        }
        else {
          $update_flag = TRUE;
        }
      }
    }

    // Updates $csv_files if necessary.
    if ($update_flag) {
      $this->setPropByKey('csv_files', $csv_files_updated);
      $this->update();
    }
    return $csv_exist;
  }

  /**
   * Updates csv_files in the job properties.
   *
   * @param array $templates
   *
   * @return boolean
   */
  public function updateCSV($templates) {

    // Gets all CSV files.
    $csv_files = $this->getPropByKey('csv_files');
    if (is_array($csv_files)) {
      $new_csv_files = array();
      foreach ($csv_files as $file_id => $csv_filename) {
        $key = preg_replace('/\.csv$/', '', $csv_filename);
        if (array_key_exists($key, $templates)) {
          $new_csv_files[$file_id] = $csv_filename;
        }
      }
      $this->setPropByKey('csv_files', $new_csv_files);
      return $this->update();
    }
    return TRUE;
  }


  /**
   * Returns all logs in an array.
   *
   * @return array
   */
  public function getLogs() {
    $all_logs = array();

    // Gets the path to the public folder.
    $public_path  = drupal_realpath('public://');
    $public_url   = file_create_url('public://');

    // Adds log files  (Warning / Errror / Dup-Data) in recent directory.
    $recent_dir = $this->getRecentLogDir('recent');
    if (file_exists($recent_dir)) {
      $objects = scandir($recent_dir);
      foreach ($objects as $object) {
        if ($object === '.' or $object === '..') {
          continue;
        }
        if (is_dir("$recent_dir/$object")) {
          preg_match("/(\d+)-([A-Z])/", $object, $matches);
          $status_int = $matches[1];
          $type       = $matches[2];

          // Skips new data log.
          if ($type == 'N') {
            continue;
          }

          // Initialzes the array.
          if (!array_key_exists($type, $all_logs)) {
            $all_logs[$type] = array();
          }

          $files = file_scan_directory("$recent_dir/$object", "/\.log$/");
          $log_files = array();
          foreach ($files as $filepath => $file) {
            preg_match("/-([A-Za-z\_]+)\.log$/", $filepath, $matches2);
            $key = $matches2[1];
            $log_files []= array(
              'key'      => $key,
              'filepath' => $filepath,
              'log_file' => "recent:$object:" . basename($filepath),
            );
          }
        }

        // Adds log files.
        if (!empty($log_files)) {
          $all_logs[$type][$status_int] = $log_files;
        }
      }
    }

    // Adds log file for new data.
    // Gets log (new data) files in 'new_data' directory.
    $new_data_dir = $this->getNewDataLogDir('new_data');
    $files        = file_scan_directory($new_data_dir, "/\.log$/");
    $log_files    = array();
    krsort($files);
    foreach ($files as $filepath => $file) {
      preg_match("/\-([A-Za-z\_]+)-(\d+-\d+-\d+)-(\d+-\d+-\d+)\.log$/", $filepath, $matches);
      $key  = $matches[1];
      $date = $matches[2];
      $time = $matches[3];
      $log_files []= array(
        'key'       => $key,
        'date'      => "$date $time",
        'filepath'  => $filepath,
        'log_file'  => 'new_data:' . basename($filepath),
      );
    }

    // Adds log files.
    if (!empty($log_files)) {

      // Sort by date-time.
      // usort($log_files, $this->_cmpDate);
      $all_logs['N']['40'] = $log_files;
    }
    return $all_logs;
  }

  /**
   * Translates status int to label.
   *
   * @retrun boolean
   */
  public function _cmpDate() {
    return strcmp($b['date'], $a['date']);
  }

/**
   * Translates status int to label.
   *
   * @retrun boolean
   */
  public function trStatusIntToLabel($int) {
    return $this->status_label["$int"];
  }

  /**
   * Returns the log directory of the provided type.
   *
   * @retrun string
   */
  public function getLogDirByType($type) {
    return $this->getLogDir() . "/$type";
  }

  /**
   * Returns the recent log directory.
   *
   * @retrun string
   */
  public function getRecentLogDir() {
    return $this->getLogDir() . '/recent';
  }

  /**
   * Returns the previous log directory.
   *
   * @retrun string
   */
  public function getPreviousLogDir() {
    return $this->getLogDir() . '/previous';
  }

  /**
   * Returns the new data log directory.
   *
   * @retrun string
   */
  public function getNewDataLogDir() {
    return $this->getLogDir() .'/new_data';
  }

  /**
   * Returns the file ID of the uploaded file.
   *
   * @retrun integer
   */
  public function getFileID() {
    $file_id = $this->getParamByKey('file_id');
    if ($file_id) {
      return intval($file_id);
    }
    return NULL;
  }

  /**
   * Sets the file ID of the uploaded file.
   *
   * @retrun integer
   */
  public function setFileID($file_id) {

    // Creates a MCL_FILE object for the uploaded file.
    $mcl_file = MCL_FILE::byKey(array('file_id' => $file_id));
    if ($mcl_file) {
      $this->upload_file = $mcl_file;
      $this->setParamByKey('file_id', $file_id);
    }
  }

  /**
   * Returns the file ID of the uploaded file.
   *
   * @retrun MCL_FILE
   */
  public function getMCLFile() {
    $file_id = $this->getFileID();
    if (is_int($file_id)) {
      return MCL_FILE::byKey(array('file_id' => $file_id));
    }
    return NULL;
  }

  /**
   * Retrieves the force flag.
   *
   * @retrun boolean
   */
  public function getTransaction() {
    return $this->getPropByKey('transaction');
  }

  /**
   * Updates the status.
   *
   * @param integer $status
   */
  public function updateStatus($status) {

    // Status : Completed (100).
    if ($status == 100) {
      $this->sendCompleteEmail();
      $this->setCompleteDate(date("Y-m-d G:i:s"));
      $this->setProgress('COMPLETED');
    }

    // Status : Failed (-200).
    else if ($status == -200) {
      $this->sendCompleteEmail();
      $this->setCompleteDate(date("Y-m-d G:i:s"));
      $this->setProgress('FAILED');
    }

    // Sets and updates the status.
    $this->setStatus($status);
    $this->update();
  }

  /*
   * Defines getters and setters below.
   */
  /**
   * Returns the job status label.
   *
   * @return string
   */
  public function getStatusLabel() {
    return $this->status_label[$this->status];
  }

  /**
   * Retrieves the force flag.
   *
   * @retrun boolean
   */
  public function getForce() {
    return $this->force;
  }

  /**
   * Sets the force flag.
   *
   * @param boolean $force
   */
  public function setForce($force) {
    $this->force = $force;
    $this->setPropByKey('force', $force);
  }

  /**
   * Retrieves the directory of the CSV files.
   *
   * @retrun string
   */
  public function getCSVDir() {
    return $this->csv_dir;
  }

  /**
   * Sets the directory of the CSV files.
   *
   * @param string $csv_dir
   */
  public function setCSVDir($csv_dir) {
    $this->csv_dir = $csv_dir;
    $this->setPropByKey('csv_dir', $csv_dir);
  }

  /**
   * Retrieves the uploaded file.
   *
   * @retrun boolean
   */
  public function getUploadFile() {
    return $this->upload_file;
  }

  /**
   * Retrieves the filename of the uploaded file.
   *
   * @retrun string
   */
  public function getFilename() {
    return $this->upload_file->getFilename();
  }

  /**
   * Sets the uploaded file.
   *
   * @param MCL_FILE $upload_file
   */
  public function setUploadFile(MCL_FILE $upload_file) {
    $this->upload_file = $upload_file;
  }
}
