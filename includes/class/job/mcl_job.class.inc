<?php
/**
 * The declaration of MCL_JOB class.
 *
 */
class MCL_JOB extends PUBLIC_MCL_JOB {

 /**
  *  Class data members.
  */
  protected $prop_arr     = NULL;
  protected $param_arr    = NULL;
  protected $working_dir  = NULL;
  protected $user_dir     = NULL;
  protected $job_dir      = NULL;
  protected $log_dir      = NULL;
  protected $std_dir      = NULL;
  protected $file_dir     = NULL;
  protected $pid          = NULL;

  // Keeps track of the log name.
  protected $log_name     = NULL;

  // Message types.
  protected $msg_types = array(
    'E' => 'Error',
    'W' => 'Warning',
    'N' => 'New Data',
    'D' => 'Duplicate',
    'U' => 'Update',
    'M' => 'Merge',
    'R' => 'Restore',
  );

  // Job status table.
  protected $status_label = array(
    '0'     => 'waiting',
    '10'    => 'started',
    '100'   => 'completed',
    '-100'  => 'cancelled',
    '-200'  => 'failed',
  );
  protected $status_int = array(
    'waiting'   =>    0,
    'started'   =>   10,
    'completed' =>  100,
    'cancelled' => -100,
    'failed'    => -200,
  );

  /**
   * @see PUBLIC_MCL_JOB::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);

    // Updates property array ($this->prop_arr).
    if ($this->prop == '') {
      $this->prop_arr = array();
    }
    else {
      $this->prop_arr = json_decode($this->prop, TRUE);
    }

    // Updates property array ($this->param_arr).
    if ($this->param == '') {
      $this->param_arr = array();
    }
    else {
      $this->param_arr = json_decode($this->param, TRUE);
    }

    // Updates directories.
    $working_dir = $this->getPropByKey('working_dir');
    if ($working_dir) {
      $this->working_dir = $working_dir;
    }
    $user_dir = $this->getPropByKey('user_dir');
    if ($user_dir) {
      $this->user_dir = $user_dir;
    }
    $job_dir = $this->getPropByKey('job_dir');
    if ($job_dir) {
      $this->job_dir = $job_dir;
    }
    $log_dir = $this->getPropByKey('log_dir');
    if ($log_dir) {
      $this->log_dir = $log_dir;
    }
    $std_dir = $this->getPropByKey('std_dir');
    if ($std_dir) {
      $this->std_dir = $std_dir;
    }
    $file_dir = $this->getPropByKey('file_dir');
    if ($file_dir) {
      $this->file_dir = $file_dir;
    }
    $pid = $this->getPropByKey('pid');
    if ($pid) {
      $this->pid = $pid;
    }
  }

  /**
   * @see PUBLIC_MCL_JOB::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * @see PUBLIC_MCL_JOB::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
      return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * Returns MCL_JOB by Job ID.
   *
   * @param integer $job_id
   *
   * @return MCL_JOB|NULL
   */
  public static function byID($job_id) {
    return self::byKey(array('job_id' => $job_id));
  }

  /**
   * @see PUBLIC_MCL_JOB::insert()
   */
  public function insert() {

    // Updates the parent:$param and parent:$prop fields.
    $this->param = json_encode($this->param_arr);
    $this->prop = json_encode($this->prop_arr);

    // Insert a new job.
    if (parent::insert()) {

      // Get MCL user directory.
      $working_dir = $this->getPropByKey('working_dir');
      if (!$working_dir) {
        $working_dir = mcl_get_config_setting('mcl_working_dir') . '/user';
      }

      // Gets username.
      $drupal_user = MCL_USER::getDrupalUser($this->user_id);
      $username = $drupal_user->name;

      // Updates MCL directories.
      $this->user_dir = "$working_dir/$username";
      $this->job_dir  = $this->user_dir . '/' . $this->job_id;
      $this->log_dir  = $this->job_dir . '/logs';
      $this->std_dir  = $this->job_dir . '/std';
      $this->file_dir = $this->job_dir . '/files';
      $this->setPropByKey('working_dir', $this->user_dir);
      $this->setPropByKey('user_dir', $this->user_dir);
      $this->setPropByKey('job_dir', $this->job_dir);
      $this->setPropByKey('log_dir', $this->log_dir);
      $this->setPropByKey('std_dir', $this->std_dir);
      $this->setPropByKey('file_dir', $this->file_dir);
      $this->setPropByKey('pid', $this->pid);
      $this->update();

      // Creates directories.
      mcl_create_dir($this->job_dir);
      mcl_create_dir($this->log_dir);
      mcl_create_dir($this->std_dir);
      mcl_create_dir($this->file_dir);
    }
    else {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * @see PUBLIC_MCL_JOB::update()
   */
  public function update() {

    // Updates the parent:$param and parent:$prop fields.
    $this->param  = json_encode($this->param_arr);
    $this->prop   = json_encode($this->prop_arr);

    // Updates the user properties.
    return parent::update();
  }

  /**
   * @see PUBLIC_MCL_JOB::delete()
   */
  public function delete() {

    // Kills the running job.
    $pid = $this->getPropByKey('pid');
    if ($pid) {

      // Kills if it is running.
      if (file_exists("/proc/$pid" )) {
        exec("kill -9 $pid", $output, $status);
      }
    }

    // Deletes files associated with this job.
    MCL_FILE::deleteByJob($this);

    // Deletes the job directory.
    $job_dir = $this->getJobDir();
    if (file_exists($job_dir)) {
      mcl_remove_dir($job_dir);
    }

    // Delete this job.
    return parent::delete();
  }

  /**
   * Returns jobs that belong to the user.
   *
   * @param integer $user_id
   * @param string $type
   *
   * @return array of MCL_UPLOAD_JOB
   */

  public static function getJobsByUser($user_id, $type = NULL) {
    $sql = "
      SELECT J.job_id, J.class_name
      FROM {mcl_job} J
      WHERE J.user_id = :user_id
    ";
    $args =array (':user_id' => $user_id);

    // Adds conditions.
    if ($type) {
      $sql .= ' AND J.type = :type';
      $args[':type'] = $type;
    }

    // Filters BIMS.
    $sql .= " AND J.name NOT LIKE 'BIMS_UPLOAD-%'  ";

    $sql .= ' ORDER BY J.submit_date DESC';
    $results = db_query($sql, $args);
    $mcl_jobs = array();
    while ($obj = $results->fetchObject()) {
      $class_name = $obj->class_name;
      $mcl_jobs []= $class_name::byKey(array('job_id' => $obj->job_id));
    }
    return $mcl_jobs;
  }

  /**
   * Returns job inforamtion table.
   *
   * @return string
   */
  public function getJobInfoTable() {
    // To be overridden by Child class.
  }

  /**
   * Checks if an error has been occurred.
   * Return TRUE if no error message found.
   *
   * @return boolean
   */
  public function noError() {
    // To be overridden by Child class.
  }

  /**
   * Updates the message.
   *
   * @param string $key
   * @param string $msg
   * @param integer $line_no
   */
  public function updateMsg($key, $msg, $line_no = 0) {
    // To be overridden by Child class.
  }

  /**
   * Returns the filepath of the log file of the provided key.
   *
   * @param string $key
   * @param string $key
   * @param string $key
   *
   * @return string
   */
  public function getLogDirByKey($key, $status, $parent_folder = 'recent') {
    // To be overridden by Child class.
    return '';
  }

  /**
   * Returns the filepath of the log file of the provided key.
   *
   * @param string $key
   *
   * @return string
   */
  public function getLogFileByKey($key, $parent_folder = 'recent') {
    // To be overridden by Child class.
    return '';
  }

  /**
   * Deletes the log file of the provided type.
   *
   * @param string $type
   */
  public function deleteLog($key) {
    $log_file = $this->getLogFileByKey($key);
    if (file_exists($log_file)) {
      unlink($log_file);
    }
  }

  /**
   * Returns the name of the std file.
   *
   * @param string $ext
   *
   * @return string
   */
  public function getStdFile($ext) {
    return $this->getStdDir() . '/mcl-' . date("Y-m-d-G-i-s") . '.' . $ext;
  }

  /**
   * Creates folders for various types of logs.
   *
   * @return boolean
   */
  public function createNewLogDirs() {
    // To be overridden by Child class.
  }

  /**
   * Keeps the previous log files to other locaiton.
   */
  public function KeepLogs() {
    // To be overridden by Child class.
  }

  /**
   * Called before run().
   */
  public function preRun() {
    // To be overridden by Child class.
  }

  /**
   * Runs a job.
   *
   * @return boolean
   */
  public function run() {
    // To be overridden by Child class.
    return TRUE;
  }

  /**
   * Returns the job inforamtion in array.
   *
   * @return array
   */
  public function getJobInfo() {
    // To be overridden by Child class.
  }

  /**
   * Called after  run().
   *
   * @param array $param
   */
  public function postRun($param = NULL) {
    // To be overridden by Child class.
  }

  /**
   * Sends a completion email.
   *
   */
  public function sendCompleteEmail() {

    // Gets the owner.
    $mcl_user = MCL_USER::byKey(array('user_id' => $this->getUserID()));

    // Do not send if user selected not send.
    $send_email = $this->getParamByKey('send_email');
    if (!$send_email) {
      return;
    }

    // All system mails need to specify the module and template key (mirrored from
    // hook_mail()) that the message they want to send comes from.
    $module = 'mcl';
    $key = 'upload-complete';

    // Specify 'to' and 'from' addresses.
    $to   = $mcl_user->getMail();
    $from = variable_get('site_mail', 'admin@localhost.localdomain');

    // "params" loads in additional context for email content completion in
    // hook_mail(). In this case, we want to pass in the values the user entered
    // into the form, which include the message body in $form_values['message'].
    $params = array(
      'job_id' => $this->job_id,
    );

    // The language of the e-mail. This will one of three values:
    // * user_preferred_language(): Used for sending mail to a particular website
    //   user, so that the mail appears in their preferred language.
    // * global $language: Used when sending a mail back to the user currently
    //   viewing the site. This will send it in the language they're currently
    //   using.
    // * language_default(): Used when sending mail to a pre-existing, 'neutral'
    //   address, such as the system e-mail address, or when you're unsure of the
    //   language preferences of the intended recipient.
    //
    // Since in our case, we are sending a message to a random e-mail address that
    // is not necessarily tied to a user account, we will use the site's default
    // language.
    $language = language_default();

    // Whether or not to automatically send the mail when drupal_mail() is
    // called. This defaults to TRUE, and is normally what you want unless you
    // need to do additional processing before drupal_mail_send() is called.
    $send = TRUE;

    // Check if mail system has been configured before sending the mail.
    $system = drupal_mail_system($module, $key);
    if (!$system->DefaultMailSystem) {
      watchdog('mcl', t('The drupal mail system has not been configured.'), array(), WATCHDOG_ERROR);
      return;
    }

    // Send the mail, and check for success. Note that this does not guarantee
    // message delivery; only that there were no PHP-related issues encountered
    // while sending.
    $result = drupal_mail($module, $key, $to, $language, $params, $from, $send);
    if ($result['result'] != TRUE) {
      watchdog('mcl', t('There was a problem sending your message and it was not sent.'), array(), WATCHDOG_ERROR);
    }
  }

  /**
   * Changes the working directory. Needs to update job_dir, log_dir, std_dir
   * and file_dir.
   *
   * @param string $working_dir
   *
   * @return boolean
   */
  public function changeWorkingDir($working_dir) {
    $job_dir = "$working_dir/" . $this->job_id;
    $this->setWorkingDir($working_dir);
    $this->setJobDir($job_dir);
    $this->setLogDir("$job_dir/logs");
    $this->setStdDir("$job_dir/std");
    $this->setFileDir("$job_dir/files");
    return $this->update();
  }

  /**
   * Returns the description of message type.
   *
   * @param string $type
   *
   * @return string
   */
  public function getMessageType($type) {
    return $this->msg_types[$type];
  }

  /**
   * Resets the completion date.
   */
  public function resetCompleteDate() {
    $this->setCompleteDate(NULL);
    $conditions = array(
      array('field' => 'job_id', 'value' => $this->job_id),
    );
    MCL_TABLE::setValueToNULL('mcl_job', array('complete_date'), $conditions);
  }

  /**
   * Resets the progress file.
   */
  public function resetProgress() {
    $progress_file = $this->getJobDir() . '/progress';
    $progresstype = 'INITIALIZE';
    if (file_exists($progress_file)) {
      unlink($progress_file);
      $progresstype = 'RESET';
    }

    // Creates a progress file.
    // CarrotOmics change, prefix with timestamp
    file_put_contents($progress_file, date("Y-m-d H:i:s") . " $progresstype PROGRESS<br>\n");
  }

  /**
   * Retrieves the progress of the job.
   *
   * @retrun string
   */
  public function getProgress() {

    // Gets the process file.
    $progress_file = $this->getJobDir() . '/progress';
    if (file_exists($progress_file)) {
      return file_get_contents($progress_file);
    }
    else {
      return 'No progress report.';
    }
  }

  /**
   * Sets the progress of the job.
   *
   * @param string $progress
   * @param boolean $append
   */

  // CarrotOmics change, default to append mode
  public function setProgress($progress, $append = TRUE) {

    // CarrotOmics change, prefix with timestamp and line break
    $progress = date("Y-m-d H:i:s") . ' ' . $progress . "<br>\n";

    // Gets the process file.
    $progress_file = $this->getJobDir() . '/progress';

    // Appends the progress.
    if (file_exists($progress_file) && $append) {
      file_put_contents($progress_file, "$progress", FILE_APPEND);
    }

    // Initializes the progress.
    else {
      file_put_contents($progress_file, $progress);
    }
  }

  /**
   * Return the formatted job parameters.
   *
   * @param boolean $user_only
   *  Lists only parameters added by a user.
   * @return string
   */
  public function getJobParamFormated($user_only = FALSE) {

    // Gets MCL default parameters.
    $mcl_default_params = array('send_email', 'file_id', '#', '##');

    // Gets the job parameters.
    $param = $this->getParam();
    $job_param = '';
    if ($param) {
      $arr = json_decode($param, TRUE);
      foreach ($arr as $key => $value) {

        // Shows only user parameters.
        if (in_array($key, $mcl_default_params)) {
          continue;
        }

        // Adds to the list.
        if (is_array($value)) {
          if (mcl_is_assoc($value)) {
            $job_param .= "$key:" . implode(', ', array_keys($value)) . "";
          }
          else {
            $job_param .= "$key:" . implode(', ', $value) . "";
          }
        }
        else {
          $job_param .= "$key:$value";
        }
      }
    }
    return $job_param;
  }

  /**
   * Changes the owner of the job.
   *
   * @param integer $user_id
   * @param boolean $flag_files
   *   The files in the job directory will be moved if the flag is TRUE.
   *
   * @retrun boolean
   */
  public function changeOwner($user_id, $flag_files = FALSE) {

    // Checks the user ID.
    $new_user = MCL_USER::addUser($user_id);
    if ($new_user) {

      // Check user directory.
      $user_dir = $new_user->getUserDir();
      if (!file_exists($user_dir)) {
        return FALSE;
      }

      // Updates user_id and prop on mcl_job.
      $this->setUserID($user_id);

      // Updates mcl_job.prop.
      $job_dir  = "$user_dir/" . $this->job_id;
      $prop_arr = $this->getPropArr();
      $prop_arr['working_dir']  = $user_dir;
      $prop_arr['user_dir']     = $user_dir;
      $prop_arr['job_dir']      = $job_dir;
      $prop_arr['log_dir']      = "$job_dir/logs";
      $prop_arr['std_dir']      = "$job_dir/std";
      $prop_arr['file_dir']     = "$job_dir/files";
      $prop_arr['csv_dir']      = "$job_dir/csv";
      $this->setPropArr($prop_arr);
      if ($this->update()) {

        // Updates filepath and uri on mcl_file.
        $mcl_files = MCL_FILE::getFiles(NULL, NULL, $this->job_id);
        foreach ($mcl_files as $mcl_file) {

          // Updates the file information.
          $filepath = "$job_dir/files/" . $mcl_file->getFilename();
          if (!$mcl_file->updateFileInfo($filepath)) {
            return FALSE;
          }
        }

        // Moves the job directory.
        if ($flag_files) {
          $target = $this->getJobDir();
          $dest   = $new_user->getUserDir() . '/';
          exec("mv \"$target\" \"$dest\"");
        }
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * Retrieves the MCL_USER.
   *
   * @retrun string
   */
  public function getUser() {
    $user = MCL_USER::byID($this->user_id);
    if ($user) {
      return $user;
    }
    return NULL;
  }

  /**
   * Retrieves the username.
   *
   * @retrun string
   */
  public function getUsername() {
    $user = MCL_USER::getDrupalUser($this->user_id);
    return $user ? $user->name : '';
  }

  /*
   * Defines getters and setters below.
   */
  /**
   * Returns the array of status_label.
   *
   * @return string
   */
  public function getStatusLabelArr() {
    return $this->status_label;
  }

  /**
   * Returns the array of status_int.
   *
   * @return array
   */
  public function getStatusIntArr() {
    return $this->status_int;
  }

  /**
   * Returns the job status label.
   *
   * @return string
   */
  public function getStatusLabel() {
    return $this->status_label[$this->status];
  }

  /**
   * Retrieves the working directory.
   *
   * @retrun string
   */
  public function getWorkingDir() {
    return $this->working_dir;
  }

  /**
   * Sets the working directory.
   *
   * @param string $working_dir
   */
  public function setWorkingDir($working_dir) {
    $this->working_dir = $working_dir;
    $this->setPropByKey('working_dir', $working_dir);
  }

  /**
   * Retrieves the user directory.
   *
   * @retrun string
   */
  public function getUserDir() {
    return $this->user_dir;
  }

  /**
   * Sets the user directory.
   *
   * @param string $user_dir
   */
  public function setUserDir($user_dir) {
    $this->user_dir = $user_dir;
    $this->setPropByKey('user_dir', $user_dir);
  }

  /**
   * Retrieves the directory of the job.
   *
   * @retrun string
   */
  public function getJobDir() {
    return $this->job_dir;
  }

  /**
   * Sets the directory of the job.
   *
   * @param string $job_dir
   */
  public function setJobDir($job_dir) {
    $this->job_dir = $job_dir;
    $this->setPropByKey('job_dir', $job_dir);
  }

  /**
   * Retrieves the log name.
   *
   * @retrun string
   */
  public function getLogName() {
    return $this->log_name;
  }

  /**
   * Sets the log name.
   *
   * @param string $log_name
   */
  public function setLogName($log_name) {
    $this->log_name = $log_name;
  }

  /**
   * Retrieves the directory of the log files.
   *
   * @retrun string
   */
  public function getLogDir() {
    return $this->log_dir;
  }

  /**
   * Sets the directory of the log files.
   *
   * @param string $log_dir
   */
  public function setLogDir($log_dir) {
    $this->log_dir = $log_dir;
    $this->setPropByKey('log_dir', $log_dir);
  }

  /**
   * Retrieves the directory of the std files.
   *
   * @retrun string
   */
  public function getStdDir() {
    return $this->std_dir;
  }

  /**
   * Sets the directory of the std files.
   *
   * @param string $std_dir
   */
  public function setStdDir($std_dir) {
    $this->std_dir = $std_dir;
    $this->setPropByKey('std_dir', $std_dir);
  }

  /**
   * Retrieves the directory of the files.
   *
   * @retrun string
   */
  public function getFileDir() {
    return $this->file_dir;
  }

  /**
   * Sets the directory of the files.
   *
   * @param string $file_dir
   */
  public function setFileDir($file_dir) {
    $this->file_dir = $file_dir;
    $this->setPropByKey('file_dir', $file_dir);
  }

  /**
   * Retrieves the process ID.
   *
   * @retrun integer
   */
  public function getPID() {
    return $this->pid;
  }

  /**
   * Sets the process ID.
   *
   * @param integer $pid
   *
   * @retrun boolean
   */
  public function setPID($pid) {
    $this->pid = $pid;
    $this->setPropByKey('pid', $pid);
    return $this->update();
  }

  /**
   * Returns the value of the given key in param.
   *
   * @retrun various
   */
  public function getParamByKey($key) {
    if (array_key_exists($key, $this->param_arr)) {
      return $this->param_arr[$key];
    }
    return NULL;
  }

  /**
   * Sets the value of the given key in param.
   *
   * @retrun various
   */
  public function setParamByKey($key, $value) {
    $this->param_arr[$key] = $value;
  }

  /**
   * Returns the value of the given key in prop.
   *
   * @retrun various
   */
  public function getPropByKey($key) {
    if (array_key_exists($key, $this->prop_arr)) {
      return $this->prop_arr[$key];
    }
    return NULL;
  }

  /**
   * Sets the value of the given key in prop.
   *
   * @param string $key
   * @param various $value
   */
  public function setPropByKey($key, $value) {
    $this->prop_arr[$key] = $value;
  }

  /**
   * Returns the prop_arr.
   *
   * @return array
   */
  public function getPropArr() {
    return $this->prop_arr;
  }

  /**
   * Sets the prop_arr.
   *
   * @param array $prop_arr
   */
  public function setPropArr($prop_arr) {
    $this->prop_arr = $prop_arr;
  }
}
