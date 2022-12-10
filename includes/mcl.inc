<?php
/**
 * Checks the configuration variables in Mainlab Chado Loader.
 */
function mcl_check_settings() {
  $issues = '';

  // Checks working directory.
  //  $working_dir  = mcl_get_config_setting('working_dir');
  //  if (!is_writable($working_dir)) {
  //    $issues .= '<li>' . t("The working directory, $working_dir, does not exist or is not writeable by the web server.") . '</li>';
  //  }
  if ($issues) {
    drupal_set_message($issues);
  }
}

/**
 * Checks if the pre-check is needed to be performed.
 */
function mcl_pre_check() {
  $module_path = drupal_get_path('module', 'mcl');
  $flag = "$module_path/pre_check.flag";
  return file_exists($flag);
}

/**
 * Sets as the pre-check has been done.
 */
function mcl_pre_check_done() {
  $module_path = drupal_get_path('module', 'mcl');
  $flag = "$module_path/pre_check.flag";
  return touch($flag);
}

/**
 * Checks if the schema exists.
 *
 * @param string $schema
 *
 * @return boolean
 */
function mcl_exist_schema($schema) {

  // Checks the schema.
  $sql = "
    SELECT LOWER(schema_name)
    FROM information_schema.schemata
    WHERE LOWER(schema_name) = LOWER(:schema)
  ";
  $schema_name = db_query($sql, array(':schema' => $schema))->fetchField();
  return ($schema_name) ? TRUE :FALSE;
}

/**
 * Returns the all the tables in the schema. The information is stored in
 * information_schema.tables table.
 *
 * @param string $schema
 * @param string $regex
 *
 * @return array
 */
function mcl_get_all_tables($schema, $regex = '') {

  // Adds the regular expression.
  $exclude = '';
  if ($regex) {
    $exclude = " AND table_name !~ '^($regex)'";
  }

  // Gets all tables in the schema.
  $sql = "
    SELECT table_name FROM information_schema.tables
    WHERE LOWER(table_schema) = LOWER(:schema)
      AND LOWER(table_type) = :table_type $exclude
    ORDER BY table_name
  ";
  $args = array(
    ':schema'     => $schema,
    ':table_type' => 'base table',
  );
  $results = db_query($sql, $args);
  $tables = array();
  while ($table_name = $results->fetchField()) {
    $tables []= $table_name;
  }
  return $tables;
}

/**
 * Returns the number of the columns in the provided table.
 *
 * @param string $schema
 * @param string $table_name
 * @param string $schema
 *
 * @return integer
 */
function mcl_get_num_columns($schema, $table_name) {
  $sql = "
    SELECT COUNT(column_name)
    FROM information_schema.columns
    WHERE LOWER(table_schema) = LOWER(:schema)
      AND LOWER(table_name) = LOWER(:table_name)
  ";
  $args = array(
    ':schema'     => $schema,
    ':table_name' => $table_name,
  );
  return db_query($sql, $args)->fetchField();
}

/**
 * Returns the table information.
 *
 * @param string $schema
 * @param string $table_name
 *
 * @return string
 */
function mcl_get_table_info($schema, $table_name) {
  $info = "Table Information for $table_name\n\n";

  // Primary key.
  $p_key = mcl_get_table_constraint($schema, $table_name, 'primary');

  // Foreign keys.
  $fkeys = mcl_get_table_constraint($schema, $table_name, 'foreign');

  // Unique keys.
  $ukeys = mcl_get_table_constraint($schema, $table_name, 'unique');

  $info .= "No information found\n";
  return $info;
}

/**
 * Returns the table information.
 *
 * @param string $schema
 * @param string $table_name
 * @param string $const
 *
 * @return array
 */
function mcl_get_table_constraint($schema, $table_name, $const) {
  $const = strtolower($const);
  $const_arr = array();
  if (preg_match("/(foreign|unique|primary)/", $const)) {
    $sql = "
      SELECT tc.constraint_type, tc.constraint_name,
        kcu.column_name, ccu.table_schema AS foreign_table_schema,
        ccu.table_name AS foreign_table_name,
        ccu.column_name AS foreign_column_name
      FROM information_schema.table_constraints AS tc
        JOIN information_schema.key_column_usage AS kcu
          ON tc.constraint_name = kcu.constraint_name AND tc.table_schema = kcu.table_schema
        JOIN information_schema.constraint_column_usage AS ccu
          ON ccu.constraint_name = tc.constraint_name AND ccu.table_schema = tc.table_schema
      WHERE LOWER(tc.constraint_type) = :const AND LOWER(tc.table_name) = LOWER(:table_name)
      ORDER BY key_type
    ";
    $args = array(
      ':const'      => "$const key",
      ':table_name' => $table_name,
    );
    $results = db_query($sql, $args);
    while ($obj = $results->fetchObject()) {
      if ($const == 'primary') {
          return $obj;
      }
      $const_arr []= $obj;
    }
  }
  return $const_arr;
}

/**
 * Return database information in a string.
 *
 * @param boolean $url
 *
 * @return string
 */
function mcl_get_db_string($url = TRUE) {
  global $databases;
  $db = $databases['default']['default'];
  if ($url) {
    $vars = array(
      $db['driver'],
      $db['username'],
      $db['password'],
      $db['host'],
      $db['database'],
    );
    return vsprintf("%s://%s:%s@%s/%s", $vars);
  }
  $vars = array(
    $db['database'],
    $db['host'],
    $db['username'],
    $db['password'],
  );
  return vsprintf(" --dbname=%s --host=%s --user=%s --password=%s ", $vars);
}

/**
 * Returns the version of the postgresql.
 *
 * @param string $type
 *
 * @return string
 */
function mcl_get_pg_version($type = '') {

  // Gets version of postgres.
  $version = db_query("SELECT version()")->fetchField();
  if (preg_match("/PostgreSQL\s+(\d+)\.(\d+)\.(\d+)\s/i", $version, $matches)) {
    $major  = $matches[1];
    $minor  = $matches[2];
    $diff   = $matches[3];
    if ($type == 'major') {
      return $major;
    }
    else if ($type == 'minor') {
      return $minor;
    }
    else if ($type == 'diff') {
      return $diff;
    }
    else {
      return "$major.$minor.$diff";
    }
  }
  return '';
}

/**
* Changes the extension of the filepath.
*
* @param string $filepath
* @param string $ext
*
* @return string
*/
function mcl_change_ext($filepath, $ext = '') {
  if (preg_match("/(.*)\.(.*)$/", $filepath, $matches)) {
    return ($ext) ? $matches[1] . '.' . $ext : $matches[1];
  }
  return ($ext) ? $filepath . '.' . $ext : $filepath;
}

/**
 * Detects the encoding of the file using 'file'.
 *
 * @param string $filepath
 *
 * @return string
 */
function mcl_detect_file_encoding($filepath) {
  if ($filepath) {
    $output = array();
    exec('file -i ' . $filepath, $output);
    if (isset($output[0])){
      $ex = explode('charset=', $output[0]);
      return isset($ex[1]) ? $ex[1] : '';
    }
  }
  return '';
}

/**
 * Stringifies an associative array.
 *
 * @param array assoc
 * @param boolean $pair
 *
 * @return string.
 */
function mcl_stringify_assoc($assoc, $type = MCL_LIST) {
  $arr_str = '';
  if (!empty($assoc)) {
    if ($type == MCL_PAIR) {
      $pairs = array();
      foreach ($assoc as $key => $value) {
        $pairs []= "$key = $value";
      }
      $arr_str = '(' . implode(', ', $pairs) . ')';
    }
    else {
      $keys   = array_keys($assoc);
      $values = array_values($assoc);
      $arr_str = '(' . implode(', ', $keys) . ') = (' . implode(', ', $values) . ')';
    }
  }
  return $arr_str;
}

/**
 * Compares the two strings. It returns '0' if it is the same.
 *
 * @param $str_1
 * @param $str_2
 * @param $case_sensitive
 *
 * return integer
 */
function mcl_compare_string($str_1, $str_2, $case_sensitive = FALSE) {
  $str_1 = trim($str_1);
  $str_2 = trim($str_2);
  if ($case_sensitive) {
    if ($str_1 == $str_2) {
      return 0;
    }
  }
  else {
    if (strtolower($str_1) == strtolower($str_2)) {
      return 0;
    }
  }
  return 1;
}

/**
 * Writes the message to drupal log. if the message is an array,
 * writes the contents of the array.
 *
 * @param string $message
 * @param integer $type
 */
function mcl_log($message, $type = WATCHDOG_ERROR) {
  if (is_array($message)) {
    $message = print_r($message, TRUE);
  }
  watchdog('MCL', $message, array(), $type);
}

/**
 * Prints the message on screen.
 *
 * @param string $msg
 * @param integer $indent
 */
function mcl_print($msg, $indent = 0, $line_feed = 0.1, $period_flag = TRUE) {

  // Sets new lines.
  $before = 0;
  $after  = $line_feed;
  if (preg_match("/(\d+).(\d+)/", $line_feed, $matches)) {
    $before  = $matches[1];
    $after   = $matches[2];
  }
  $nl_before  = str_repeat("\n", $before);
  $nl_after   = str_repeat("\n", $after);

  // Sets indents.
  $space = str_repeat(' ', $indent  * 4);

  // Adds a period.
  $period = ($period_flag) ? '.' : '';
  if ($GLOBALS['verbose']) {
    print $nl_before . $space . $msg . $period . $nl_after;
  }

  // CarrotOmics addition, always log this output to an accessible file location
  $path = '/tmp';
  if (is_dir('/local/sites/www.carrotomics.org')) {
    $path = '/local/sites/www.carrotomics.org';
  }
  $fp = fopen($path . '/mcl.log', 'a');
  fwrite($fp, $nl_before . $space . $msg . $period . $nl_after );
  fclose($fp);
}

/**
 * Displays the memory usage.
 *
 * @param string $message
 */
function mcl_display_memory_usage($message = '') {
  print "\n\tcurrent memory limit: ".ini_get("memory_limit");
  print "\n\tPeak memory usage: " . (memory_get_peak_usage(true) / 1024 / 1024) . " MB\n";
  if ($message) {
    print "\n\t$message\n\n";
  }
}

/**
 * Returns if the given value is associative array.
 *
 * @param various $value
 *
 * @return boolean
 */
function mcl_is_assoc($value) {
  if (array() === $value) {
    return false;
  }
  return array_keys($value) !== range(0, count($value) - 1);
}

/**
 * Returns if the given string is real number format.
 *
 * @param string $string
 *
 * @return boolean
 */
function mcl_is_real($string) {
  return preg_match("/^-?(?:\d+|\d*\.\d+)$/", $string);
}

/**
 * Returns if the given string is real number format.
 *
 * @param string $string
 *
 * @return boolean
 */
function mcl_is_int($string) {
  return is_int(intval($string));
}

/**
 * Empties a dirctory
 *
 * @param string $dir
 *
 * @return boolean
 */
function mcl_empty_dir($dir) {
  if (mcl_remove_dir($dir)) {
    return mcl_create_dir($dir);
  }
  return FALSE;
}

/**
 * Creates a dirctory
 *
 * @param string $dir
 *
 * @return boolean
 */
function mcl_create_dir($dir) {

  // Creates a dirctory.
  if(!file_prepare_directory($dir, FILE_CREATE_DIRECTORY)) {
    drupal_set_message("Cannot create directory : $dir");
    watchdog('mcl', "Fail to create directory: %dir.", array('%dir' => $dir), WATCHDOG_ERROR);
    return FALSE;
  }
  return TRUE;
}

/**
 * Removes all files and sub-diretories.
 *
 * @param $target_dir
 *
 * @return boolean TRUE|FALSE
 */
function mcl_remove_dir($target_dir) {

  if (is_dir($target_dir)) {

    // Gets the files in the target directory.
    $objects = scandir($target_dir);
    foreach ($objects as $object) {

      // Ignores the . and .. objects
      if ($object == "." or $object == "..") {
        continue;
      }

      // If the object is another directory then recurse.
      if (filetype($target_dir . "/" . $object) == "dir") {
        mcl_remove_dir($target_dir . "/" . $object);
      }
      // Otherwise delete the file.
      else {
        unlink($target_dir . "/" . $object);
      }
    }
    reset($objects);

    // Finaly, removes the directory.
    return rmdir($target_dir);
  }
  return false;
}

/**
 * Returns MCL public folder URL.
 *
 * @return string
 */
function mcl_get_public_URL() {
  return file_create_url('public://') . '/mcl';
}

/**
 * Returns the number of lines in the file.
 *
 * @param string $filepath
 * @param boolean $use_count_func
 *
 * @return integer
 */
function mcl_get_num_lines($filepath, $use_count_func = FALSE) {
  if (file_exists($filepath)) {
    if ($use_count_func) {
      return count(file($filepath)) - 1;
    }
    else {
      $num_lines = exec("wc -l $filepath");
      if ($num_lines) {
        return $num_lines;
      }
    }
  }
  return 0;
}

/**
 * Returns MCL public folder URL.
 *
 * @param string $filepath
 *
 * @return string
 */
function mcl_get_rel_filepath($filepath) {

  // Gets the relative path to the file.
  $mcl_file_dir = mcl_get_config_setting('mcl_file_dir');
  return str_replace($mcl_file_dir, '', $filepath);
}

/**
 * Returns the all files under the provided directory.
 *
 * @param string $target_dir
 * @param string $pattern
 *
 * @return array of string
 */
function mcl_retrive_files($target_dir, $pattern) {
  $dir = new RecursiveDirectoryIterator($target_dir);
  $ite = new RecursiveIteratorIterator($dir);
  $files = new RegexIterator($ite, $pattern, RegexIterator::GET_MATCH);
  $file_list = array();
  foreach($files as $file) {
    $file_list = array_merge($file_list, $file);
  }
  return $file_list;
}

/**
 * Function taken from php.net
 * @param $bytes
 *   The size of the file in bytes
 * @param $precision
 *   The number of decimal places to use in the final number if needed
 * @return string
 *   A formatted string indicating the size of the file
 */
function mcl_format_bytes($bytes, $precision = 2) {
  $units = array('B', 'KB', 'MB', 'GB', 'TB');

  $bytes = max($bytes, 0);
  $pow = floor(($bytes ? log($bytes) : 0) / log(1024));
  $pow = min($pow, count($units) - 1);

  // Uncomment one of the following alternatives
  $bytes /= pow(1024, $pow);
  // $bytes /= (1 << (10 * $pow));

  return round($bytes, $precision) . ' ' . $units[$pow];
}

/**
 * Format the date in integer to the provided date format.
 *
 * @param integer $date_int
 * @param string $format
 *
 * @return string
 *   A formatted date string.
 */
function mcl_format_date($date_int, $format = "m/d/Y") {
  return date($format, mktime(0, 0 , 0 , 1, $date_int - 1, 1900));
}

/**
 * Download all log files.
 *
 * @param integer $job_id
 */
function mcl_download_log($job_id) {

  // Gets MCL_UPLOAD_JOB.
  $mcl_job = MCL_JOB_UPLOAD::byID($job_id);
  $log_root_dir = dirname($mcl_job->getLogDir());

  // Sets zip path and name.
  $filename = "mcl-$job_id-logs.zip";
  $filepath = "$log_root_dir/$filename";

  // Create a zip file.
  $zip = new ZipArchive;
  $zip->open($filepath, ZipArchive::OVERWRITE|ZipArchive::CREATE);

  $files = mcl_retrive_files($mcl_job->getLogDir(), '/.*\.log$/');
  foreach ($files as $file) {
    $file_name = basename($file);
    $rel_path = str_replace("$log_root_dir/", '', dirname($file));
    $zip->addFile($file, "$rel_path/$file_name");
  }
  $zip->close();

  // Attaches the zip file.
  header("Content-Type: application/zip");
  header("Content-Disposition: attachment; filename=$filename");
  readfile($filepath);
  exit();
}

/**
 * Download a file.
 *
 * @param integer $file_id
 */
function mcl_download_file($file_id) {

  // Gets MCL_FILE and the filepath
  $mcl_file = MCL_FILE::byKey(array('file_id' => $file_id));
  $filepath = $mcl_file->getFilepath();
  $filename = basename($filepath);
  if (!file_exists($filepath)) {
    drupal_set_message("Error : $filepath not found");
    drupal_goto();
  }
  else {

    // Attaches the file.
    header("Content-Type: text/plain");
    header("Content-Disposition: attachment; filename=$filename");
    header('Content-Length: ' . filesize($filepath));
    readfile($filepath);
    exit();
  }
}

/**
 * Views log file.
 *
 * @param integer $job_id
 * @param string $filepath_str
 */
function mcl_view_log($job_id, $filepath_str) {

  // Gets MCL_UPLOAD_JOB.
  $mcl_job = MCL_JOB_UPLOAD::byID($job_id);

  $filepath = $mcl_job->getLogDir() . '/' . str_replace(':', '/', $filepath_str);

   // Open the log file.
  $contents = "Log file not found [$filepath].";
  if (file_exists($filepath)) {
    $contents = file_get_contents($filepath);
  }
  //drupal_json_output($contents);
  header("Content-Type: text/plain");
  echo $contents;
  exit();
}

/**
 * Returns data query links.
 *
 * @return string
 */
function mcl_form_get_data_query_links() {

  // Adds data query links.
  $data_query_items = array(
    array(
      'data' => l('Cvterm', '/mcl/data_query/cvterm'),
      'style' => array('list-style-type:disc;'),
    ),
    array(
      'data' => l('Feature', '/mcl/data_query/feature'),
      'style' => array('list-style-type:disc;'),
    ),
    array(
      'data' => l('Stock', '/mcl/data_query/stock'),
      'style' => array('list-style-type:disc;'),
    ),
    array(
      'data' => l('Organism', '/mcl/data_query/organism'),
      'style' => array('list-style-type:disc;'),
    ),
    array(
      'data' => l('Contact', '/mcl/data_query/contact'),
      'style' => array('list-style-type:disc;'),
    ),
    array(
      'data' => l('Publication', '/mcl/data_query/pub'),
      'style' => array('list-style-type:disc;'),
    ),
  );
  $data_query = theme('item_list', array('items' => $data_query_items));
  $tag = "<div style='margin-top:10px;'><b>Data Queries</b></div>";
  $tag .= "<div>$data_query</div>";
  return $tag;
}

/**
 * Returns data info links.
 *
 * @return string
 */
function mcl_form_get_data_info_links() {

  // Adds data info links.
  $data_info_items = array(
    array(
      'data' => l('Traits', '/mcl/data_info/trait') . " (Group by 'Trait Ontology')",
      'style' => array('list-style-type:disc;'),
    ),
    array(
      'data' => l('Trait descriptors', '/mcl/data_info/descriptor') . " (Group by 'Trait Descriptor Set')",
      'style' => array('list-style-type:disc;'),
    ),
  );
  $data_info = theme('item_list', array('items' =>$data_info_items));
  $tag = "<div style='margin-top:10px;'><b>Data Information</b></div>";
  $tag .= "<div>$data_info</div>";
  return $tag;
}

