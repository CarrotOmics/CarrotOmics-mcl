<?php
/**
 * The declaration of MCL_TEMPLATE class.
 *
 */
class MCL_TEMPLATE extends PUBLIC_MCL_TEMPLATE {

 /**
  *  Class data members.
  */
  protected $template_type  = NULL;
  protected $headers        = NULL;
  protected $headers_sp     = NULL;
  protected $cvterms        = NULL;
  protected $csv            = NULL;
  protected $job            = NULL;
  protected $line_no        = NULL;
  protected $stats          = NULL;
  private   $tmp            = array();

  /**
   * @see PUBLIC_MCL_TEMPLATE::__construct()
   */
  public function __construct($details) {
    parent::__construct($details);
    $this->csv  = array_key_exists('csv', $details) ? $details['csv'] : NULL;
    $this->job  = array_key_exists('job', $details) ? $details['job'] : NULL;

    // Updates the template type.
    if (array_key_exists('template_type', $details)) {
      $this->template_type =  MCL_TEMPLATE_TYPE::byKey(array('type' => $details['template_type']));
    }
    if (!$this->template_type && $this->template_type_id) {
      $this->template_type = MCL_TEMPLATE_TYPE::byKey(array('template_type_id' => $this->template_type_id));
    }

    // Initialize variables.
    $this->line_no = -1;

    // Sets the headers.
    $this->headers = $this->defineHeaders();

    // Sets the cvterms.
    $this->cvterms = $this->defineCvterms();
  }

  /**
   * @see PUBLIC_MCL_TEMPLATE::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
      return new self($parent->getMemberArr());
    }
  }

  /**
   * Returns MCL_TEMPLATE by ID.
   *
   * @param integer $template_id
   *
   * @return MCL_TEMPLATE
   */
  public static function byID($template_id) {
    return MCL_TEMPLATE::byKey(array('template_id' => $template_id));
  }

  /**
   * @see PUBLIC_MCL_TEMPLATE::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * Return the description of of template properties for both Excel and CSV
   * files.
   *
   * @return string
   *  The description of a template.
   */
  public static function getProperties() {

    // Sets the headers.
    $headers = array(
      'Prefix',
      array('data' => 'Column Type', 'style' => 'width:80px;'),
      'Description'
    );

    // Special column table.
    $desc_REQ     = 'The star symbol indicates a required column. All the values in this column must be provided';
    $desc_DESC    = 'A column name followed by single pound sign must be a cvterm name. The cvterm is used as a descriptor of evaluation data in the column.' .
                    '<br />(<em>e.g.</em>) #brix #pH #fruit_weight';
    $desc_SEQ     = 'A column name followed by a tilde symbol indicates that the column holds sequnce information.';
    $desc_MARKER  = 'A column name followed by a dollar symbol indicates that the column holds marker name.';
    $desc_STOCK   = 'A column name followed by a percent symbol indicates that the column holds stock name.';
    $desc_PROP    = 'A column name followed by double special characters must be a cvterm name. The cvterm is used as a type of a property table.' .
                    '<br />The following special characters can be used : #, @, % and $' .
                    '<br />(<em>e.g.</em>) ##cvterm_name @@cvterm_name %%cvterm_name and $$cvterm_name';
    $rows = array(
      array('*', 'required', $desc_REQ),
      array('#', 'descriptor', $desc_DESC),
      array('$', 'marker', $desc_MARKER),
      array('%', 'stock', $desc_STOCK),
      array('~', 'sequence', $desc_SEQ),
      array('(sp)(sp)', 'property', $desc_PROP),
    );
    $table_vars = array(
      'header'      => $headers,
      'rows'        => $rows,
      'attributes'  => array(),
    );
    $table = theme('table', $table_vars);

    // Excel format properties.
    $items = array();
    $items[] = "Column names are case-insensitive.";
    $items[] = "Data sheet name must match a template name.";
    $items[] = "special column - special charcter at the begining of column name.\n$table";
    $excel_prop = theme('item_list', array('items' => $items));

    // CSV format properties.
    $items = array();
    $items[] = "The first line of CSV file must start with '#' followed by a template name.";
    $items[] = "The second line of a CSV file must with '#' followed by headers.";
    $items[] = "Data section starts from the third line. Each column should be double quoted and separated by commas.";
    $csv_prop = theme('item_list', array('items' => $items));

    $desc = <<< DESC
    <div><b>Data in Excel format</b></div>
    <div>$excel_prop</div>
    <div><b>Data in CSV format</b></div>
    <div>$csv_prop</div>
    <div style="margin-left:30px;">(<em>e.g.</em>)<br />
    #contact<br />
    #"*name","first_name","last_name","alias","*type","lab","institution"<br />
    "N","F","L","", "person","Main Lab", "WSU"<br />
    </div>
DESC;
    return $desc;
  }

  /**
   * Gets the default data validation types of this template.
   *
   * @return array
   */
  public function getDefDataValidTypes() {
    // To be overridden by descendant class.
    return array();
  }

  /**
   * Enables the template.
   *
   * @return boolean
   */
  public function enable() {

    // Adds data vaild types.
    if (!$this->addDataValidTypes()) {
      return FALSE;
    }

    // Sets mcl_template.public to be '1'.
    $this->setPublic(1);
    if (!$this->update()) {
      $error_msg .= 'Failed to update mcl_template';
    }
    drupal_set_message($error_msg);
    return ($error_msg == '');
  }

  /**
   * Enables the template.
   *
   * @param bool $remove_data_valid_types
   *
   * @return boolean
   */
  public function disable($remove_data_valid_types = FALSE) {

    // Removes the data valild types.
    if ($remove_data_valid_types) {
      if (!$this->removeDataValidTypes()) {
        return FALSE;
      }
    }

    // Sets mcl_template.public to be '0'.
    $this->setPublic(0);
    return $this->update();
  }

  /**
   * Adds the default data valid types.
   *
   * @return boolean
   */
  public function addDataValidTypes() {

    // Gets MCL_TEMPLATE.
    $mcl_template     = MCL_TEMPLATE::getByTemplate($this->getTemplate());
    $data_valid_types = $mcl_template->getDefDataValidTypes();
    $error_msg        = '';
    foreach ((array)$data_valid_types as $data_valid_type => $info) {

      // Adds data vaild type.
      $details = array(
        'data_valid_type' => $data_valid_type,
        'template_id'     => $this->getTemplateID(),
        'cv_name'         => $info['cv_name'],
        'description'     => $info['description'],
        'values'          => $info['values'],
      );
      $error_msg .= MCL_DATA_VALID_TYPE::addType($details);
    }
    if ($error_msg) {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Removes all the data valid types.
   *
   * @return boolean
   */
  public function removeDataValidTypes() {
    db_delete('mcl_data_valid_type')
      ->condition('template_id', $this->template_id)
      ->execute();
    return TRUE;
  }

  /**
   * Return TRUE if this descendant class has data validation type.
   *
   * @return boolean
   */
  public function hasDataValidType() {
    $template = $this->getTemplateByID($this->template_id);
    $data_valid_types = $template->getDefDataValidTypes();
    return !empty($data_valid_types);
  }

  /**
   * Gets the data validation types of this template.
   *
   * @return array
   *
   * @return boolean
   */
  public function getDataValidTypes() {
    $template = $this->getTemplateByID($this->template_id);
    return $template->getDefDataValidTypes();
  }

  /**
   * Gets the description of this template.
   *
   * @return string
   *   The description of this template.
   */
  public function defineDescription() {
    // To be overridden by Child class.
    return '';
  }

  /**
   * Returns the regular expression for special columns.
   *
   * @return string
   */
  public function getRegExSpCol() {
    return "/^([#$%@~]{1,2}|allele\_\d+)/i";
  }

  /**
   * Gets headers used in this template.
   *
   * @return array of header columns.
   */
  public function defineHeaders() {
    // To be overridden by Child class.
    return array();
  }

  /**
   * Gets cvterms used in this template.
   *
   * @return array of cvterms.
   */
  public function defineCvterms() {
    // To be overridden by Child class.
    return array();
  }

  /**
   * Adds a template.
   *
   * @param string $template_type
   * @param string $template
   * @param integer $public
   *
   * @return boolean
   */
  public static function addTemplate($template_type, $template, $public = 0) {

    // Gets the template type.
    $mcl_template_type = MCL_TEMPLATE_TYPE::byKey(array('type' => $template_type));

    if (!$mcl_template_type) {
      return FALSE;
    }

    // Check for duplication.
    $mcl_template = PUBLIC_MCL_TEMPLATE::byKey(array('template' => $template));
    if (!$mcl_template) {

      // Adds a new template.
      $details = array(
        'template'          => $template,
        'template_type_id'  => $mcl_template_type->getTemplateTypeID(),
        'public'            => $public,
      );
      $mcl_template = new MCL_TEMPLATE($details);
      if (!$mcl_template->insert()) {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Updates templates.
   *
   * @param boolean $remove
   */
  public static function updateTemplates($remove = FALSE) {

    // Gets the template folder.
    $class_loader = new MCL_CLASS_LOADER();
    $template_dir = $class_loader->getTemplatePath();

    // Scans the template folder and adds new template if not exists in
    // mcl_template table.
    $files = file_scan_directory($template_dir, "/\.inc$/");
    $template_class = array();
    foreach ($files as $filepath => $file) {

      // Skips files in /templates/core directory.
      if (preg_match("/\/templates\/core/", $filepath)) {
        continue;
      }

      // Go though all class files.
      if (preg_match("/^mcl_template_(.*)\.class$/", $file->name, $matches)) {
        $template_name = $matches[1];
        $template_class[$template_name] = TRUE;

        // Checks the class.
        $class_name = 'MCL_TEMPLATE_' . strtoupper($template_name);
        if (!class_exists($class_name)) {
          drupal_set_message("Skipped : $class is not defined.");
          continue;
        }

        // Checks if the template exists in mcl_template table.
        $mcl_template = MCL_TEMPLATE::byKey(array('template' => $template_name));

        // If it exists, updates the tmeplate_type ID.
        if ($mcl_template) {
          $tmp_mcl_template     = new $class_name();
          $tmp_template_type_id = $tmp_mcl_template->getTemplateTypeID();
          if (!$tmp_template_type_id) {
            drupal_set_message("Template type of $class_name not found. Check the template type.");
            continue;
          }

          // Updates the template type ID if different.
          if ($mcl_template->getTemplateTypeID() != $tmp_template_type_id) {
            db_update('mcl_template')
              ->fields(array('template_type_id' => $tmp_template_type_id))
              ->condition('template_id', $mcl_template->getTemplateID(), '=')
              ->execute();
          }
        }

        // If it does not exist, add it.
        else {

          // Checks the template type.
          $mcl_template = new $class_name();
          if (!$mcl_template->getTemplateType(MCL_STRING)) {
            drupal_set_message("Template type of $class_name not found. Check the template type.");
            continue;
          }

          // Adds a new template.
          if (MCL_TEMPLATE::addTemplate($mcl_template->getTemplateType(), $template_name, 0)) {
            drupal_set_message("New template of $class_name has been added.");
          }
          else {
            drupal_set_message("Failed to add the template of $class_name.");
          }
        }
      }
    }

    // Removes the templates that don't have class file from mcl_template table.
    if ($remove) {
      $templates = MCL_TEMPLATE::getTemplates();
      foreach ($templates as $template) {
        $template_name = $template->getTemplate();
        if (!array_key_exists($template_name, $template_class)) {
          if ($template->delete()) {
            drupal_set_message("$template_name deleted.");
          }
          else {
            drupal_set_message("Failed to delete $template_name.");
          }
        }
      }
    }
  }

  /**
   * Returns template information in array. Adds the information for
   * creating two excel sheets for header definition and data template.
   *
   * @return array
   *  It holds information about this template.
   */
  public function getTemplateInfo() {

    // Adds info. for header definition.
    $def_sheet = array();
    $def_sheet['name']  = 'Headers';

    // Sets headers.
    $headers = array();
    $headers['A'] = array('heading' => 'Name',        'width' => 25);
    $headers['B'] = array('heading' => 'Required',    'width' => 10);
    $headers['C'] = array('heading' => 'Definition',  'width' => 100);
    $def_sheet['headers'] = $headers;

    // Sets data.
    $data = array();
    foreach ($this->headers as $heading => $prop) {
      $name = array(
        'col'   => 'A',
        'type'  => 'text',
        'value' => $heading,
      );
      $value = '';
      if (array_key_exists('req', $prop) && $prop['req']) {
        $value = 'REQ';
      }
      $req = array(
        'col'   => 'B',
        'type'  => 'text',
        'value' => $value,
      );
      $value = array_key_exists('desc', $prop) ? $prop['desc'] : '';
      $def = array(
        'col'   => 'C',
        'type'  => 'text',
        'value' => $value,
      );
      $data[] = array($name, $req, $def);
    }
    $def_sheet['data'] = $data;

    // Adds info. for data.
    $data_sheet = array();
    $data_sheet['name'] = $this->template;

    // Sets headers.
    $headers = array();
    $col_char = 'A';
    $regex = $this->getRegExSpCol();
    foreach ($this->headers as $heading => $prop) {
      $label  = $heading;
      $header = array();
      if (preg_match($regex, $heading)) {
        continue;
      }

      // Adds a label if exists.
      if (array_key_exists('label', $prop)) {
        $label = $prop['label'];
      }

      // Adds a prefix to heading.
      $prefix = (array_key_exists('req', $prop) && $prop['req']) ? '*' : '';
      $header['heading'] = $prefix . $label;

      // Adds width if defined.
      if (array_key_exists('width', $prop)) {
        $header['width'] = $prop['width'];
      }
      $headers[$col_char++] = $header;
    }
    $data_sheet['headers'] = $headers;

    // Adds both header definition and data sheets.
    return array($def_sheet, $data_sheet);
  }

  /**
   * Returns if the job has the given template.
   *
   * @param string $template
   *
   * @return boolean
   */
  public function hasTemplate($template) {
    $csv_files = $this->job->getPropByKey('csv_files');
    foreach ($csv_files as $file_id => $filename) {
      if (preg_match("/$template/i", $filename)) {
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * Checks if a template for this template exists. Also checks if
   * a class file for this template exists.
   *
   * @param string $template
   *
   * @return boolean
   */
  public static function validateTemplate($template) {

    // Checks if a template for this template exists.
    $mcl_template = MCL_TEMPLATE::byKey(array('template' => $template));
    if ($mcl_template) {

      // Checks if class file for this template is defined.
      $class_name = 'MCL_TEMPLATE_' . strtoupper($template);
      return class_exists($class_name);
    }
    return FALSE;
  }

  /**
   * Returns the valid columns.
   *
   * @param MCL_TEMPLATE $template_base
   *
   * @return array
   */
  public static function getValidColumns($template_base) {
    $template_obj = self::getByTemplate($template_base->getTemplate());
    $template_obj->setJob($template_base->getJob());
    return array_map('strtolower', $template_obj->validColumns());
  }

  /**
   * Returns the valid columns.
   *
   * @return array
   */
  public function validColumns() {
    // To be overridden by Child class.
    return array();
  }

  /**
   * Returns the name of a variable column.
   *
   * @param string $column
   *
   * @return string
   */
  public function mapVarColumn($column) {
    // To be overridden by Child class.
    return '';
  }

  /**
   * Gets the rank of the template. The rank is based on the type of template.
   *
   * @param string $template
   *
   * @return integer
   *  The rank of the template.
   *
   */
  public static function getRank($template) {

    // Gets the rank of the template.
    $mcl_template = MCL_TEMPLATE::byKey(array('template' => $template));
    return $mcl_template->template_type->getRank();
  }

  /**
   * Returns MCL_TEMPLATE object.
   *
   * @param MCL_JOB_UPLOAD $job
   * @param MCL_FILE_CSV $csv
   *
   * @return MCL_TEMPLATE
   */
  public static function getTemplateClass(MCL_JOB_UPLOAD $job, MCL_FILE_CSV $csv) {

    // Gets the child class object of MCL_TEMPLATE.
    $template = $csv->getTemplate();
    $class_name = 'MCL_TEMPLATE_' . strtoupper($template);
    $mcl_template = $class_name::byKey(array('template' => $template));
    if ($mcl_template) {
      $mcl_template->setJob($job);
      $mcl_template->setCSV($csv);
      return $mcl_template;
    }
    return NULL;
  }

  /**
   * Returns MCL_TEMPLATE object.
   *
   * @param string $template
   *
   * @return Descendance of MCL_TEMPLATE
   */
  public static function getByTemplate($template) {

    // Checks the template.
    $mcl_template = MCL_TEMPLATE::byKey(array('template' => $template));
    if (!$mcl_template) {
      return NULL;
    }

    // Gets the child class object of MCL_TEMPLATE.
    $class_name = 'MCL_TEMPLATE_' . strtoupper($template);
    if (!class_exists($class_name)) {
      return NULL;
    }
    return $class_name::byKey(array('template_id' => $mcl_template->getTemplateID()));
  }

  /**
   * Returns MCL_TEMPLATE object.
   *
   * @param integer $template_id
   *
   * @return Descendance of MCL_TEMPLATE
   */
  public static function getTemplateByID($template_id) {

    // Gets the template.
    $mcl_template = MCL_TEMPLATE::byKey(array('template_id' => $template_id));
    $template = $mcl_template->getTemplate();

    // Gets the child class object of MCL_TEMPLATE.
    $class_name = 'MCL_TEMPLATE_' . strtoupper($template);
    if (!class_exists($class_name)) {
      return NULL;
    }
    return $class_name::byKey(array('template_id' => $template_id));
  }

  /**
   * Returns the all templates.
   *
   * @param boolean $descendant
   *
   * @return array of MCL_TEMPLATE.
   */
  public static function getTemplates($descendant = FALSE) {
    $sql = "
      SELECT T.template_id
      FROM {mcl_template} T
        INNER JOIN {mcl_template_type} TT on TT.template_type_id = T.template_type_id
      ORDER BY TT.rank, T.template
    ";
    $mcl_templates = array();
    $result = db_query($sql);
    while ($template_id = $result->fetchField()) {
      $mcl_template = MCL_TEMPLATE::byKey(array('template_id' => $template_id));
      if ($descendant) {
        $mcl_template = MCL_TEMPLATE::getByTemplate($mcl_template->getTemplate());
      }
      $mcl_templates []= $mcl_template;
    }
    return $mcl_templates;
  }

  /**
   * Updates all the headers to be lowercase.
   *
   * @return boolean
   */
  public function lowerHeaders() {
    $headers = $this->defineHeaders();
    $updated_headers = array();
    foreach ($headers as $header => $prop) {
      $prop['col'] = -1;
      $updated_headers[strtolower($header)] = $prop;
    }
    return $updated_headers;
  }

  /**
   * Overwrites the headers.
   *
   * @param array $ref_arr
   */
  protected function overwriteHeaders($ref_arr) {

    // Updates the columns if defined.
    $headers = $this->getHeaders();
    $new_headers = array();
    foreach ($headers as $name => $info) {
      if (array_key_exists($name, $ref_arr) && $ref_arr[$name]) {
        $new_headers[$ref_arr[$name]] = $info;
      }
      else {
        $new_headers[$name] = $info;
      }
    }
    $this->setHeaders($new_headers);
  }

  /**
   * Updates cvterms array.
   *
   * @return boolean
   */
  public function updateCvterms() {
    $error = FALSE;

    // Gets cvterm IDs.
    foreach ($this->cvterms as $cv_name => $cvterms) {
      $tmp_cv_name = $cv_name;

      // Updates the name if name is 'SITE_CV' or 'SITE_TRAIT_ONTOLOGY'.
      if ($cv_name == 'SITE_CV') {
        $tmp_cv_name = MCL_SITE_VAR::getValueByName('SITE_CV');
      }
      else if ($cv_name == 'SITE_TRAIT_ONTOLOGY') {
        $tmp_cv_name = MCL_SITE_VAR::getValueByName('SITE_TRAIT_ONTOLOGY');
      }

      // Gets CV.
      $cv = CHADO_CV::byKey(array('name' => $tmp_cv_name));
      if (!$cv) {
        $this->updateMsg('E', "Error : Could not find cv_id for $tmp_cv_name");
        $error = TRUE;
        continue;
      }

      // Gets cvterms.
      $cv_id = $cv->getCvID();
      foreach ($cvterms as $cvterm_name => $dummy) {
        $cvterm = CHADO_CVTERM::byKey(array('name' => $cvterm_name, 'cv_id' => $cv_id));
        if ($cvterm) {
          $this->cvterms[$cv_name][$cvterm_name] = $cvterm->getCvtermID();
        }
        else {
          $this->updateMsg('E', "Error : Could not find cvterm : $cvterm_name [cv_id = $cv_id].");
          $error = TRUE;
        }
      }
    }
    return !$error;
  }

  /**
   * Returns the all the labels of the headers.
   *
   * @return array
   */
  public function getHeaderLabels() {
    $labels = array();
    foreach ($this->headers as $column => $prop) {
      if (array_key_exists('label', $prop)) {
        $labels[$prop['label']] = $column;
      }
    }
    return $labels;
  }

  /**
   * Cleanups the headers. Replace the labels with the column names.
   *
   * @param array $arr
   */
  public function cleanupHeaders($line) {

    // Gets the label.
    $header_labels = $this->getHeaderLabels();

    // Replace with the label if matched.
    $tmp = explode('","', trim($line, '"'));
    $size = sizeof($tmp);
    for ($i = 0; $i < $size; $i++) {
      $label = $tmp[$i];
      $required = '';
      if (preg_match("/^\*/", $label)) {
        $label = substr($label, 1);
        $required = '*';
      }
      if (array_key_exists($label, $header_labels)) {
        $tmp[$i] = $required . $header_labels[$label];
      }
    }
    return '"' . implode('","', $tmp) . '"';
  }

  /**
   * Returns the information of the headers in table.
   *
   * @param array $overwrite_cols
   *   Overwrites the headers with the headings with ones in the provided columns.
   *
   * @return string
   */
  public function getHeaderTable($overwrite_cols = NULL) {

    // Sets red star.
    $star = '<b><span style="color:red;">*</span></b>';

    // Gets the regular expression for special columns.
    $regex = $this->getRegExSpCol();

    // Updates the headings.
    if ($overwrite_cols) {
      $this->overwriteHeaders($overwrite_cols);
    }

    // Populates the rows with the headers.
    $rows = array();
    foreach ($this->headers as $heading => $prop) {

      // Shows a label if exists.
      if (array_key_exists('label', $prop)) {
        $heading = $prop['label'];
      }
      $heading = ($prop['req']) ? "$star $heading" : $heading;

      // If the column is special colmn, make it italic.
      if (preg_match($regex, $heading)) {
        $heading = "<em>$heading</em>";
      }
      $row = array($heading, $prop['desc']);
      $rows []= $row;
    }

    // Creates a table.
    $table_vars = array(
      'header'      => array(array('data' => 'Column', 'width' => '120'), 'Description'),
      'rows'        => $rows,
      'empty'       => 'No header found',
      'attributes'  => array(),
    );
    $table = theme('table', $table_vars);

    // Adds the footer.
    $footer = "<div style='left-margin:5px;'>$star&nbsp;Required field</div>";
    return $table.$footer;
  }

  /**
   * Returns the information of the cvterms in table.
   *
   * @return string
   */
  public function getCvtermTable() {

    // Populates the rows with the cvterms.
    $rows = array();
    foreach ($this->cvterms as $cv_name => $cvterms) {
      if ($cv_name == 'SITE_CV') {
        $cv_name = MCL_SITE_VAR::getValueByName('SITE_CV');
      }
      else if ($cv_name == 'SITE_TRAIT_ONTOLOGY') {
        $cv_name = MCL_SITE_VAR::getValueByName('SITE_TRAIT_ONTOLOGY');
      }
      ksort($cvterms);
      foreach ($cvterms as $cvterm_name => $cvterm_id) {
        $row = array($cv_name, $cvterm_name);
        $rows []= $row;
      }
    }
    $table_vars = array(
      'header'      => array('cv.name', 'cvterm.name'),
      'rows'        => $rows,
      'empty'       => 'No cvterm found',
      'attributes'  => array(),
    );
    return theme('table', $table_vars);
  }

  /**
   * Checks if this job stage has been passed.
   *
   * @return boolean
   */
  public function isPassed() {
    return $this->job->noError($this->template);
  }

  /**
   * Updates the message.
   *
   * @param string $key
   * @param string $msg
   * @param boolean $flag_line_no
   */
  public function updateMsg($key, $msg, $flag_line_no = TRUE, $line_no = '') {
    if ($flag_line_no) {
      if ($line_no == '') {
        $this->job->updateMsg($key, $msg, $this->line_no);
      }
      else {
        $this->job->updateMsg($key, $msg, $line_no);
      }
    }
    else {
      $this->job->updateMsg($key, $msg);
    }
  }

  /**
   * Adds the message.
   *
   * @param string $key
   * @param string $table
   * @param array $args
   */
  public function addMsg($key, $table, $args = array()) {
    $msg = '';
    if ($key == 'N') {
      $msg = "Added to $table : " . $this->arrStr($args);
    }
    else if ($key == 'E') {
      $msg = "Failed to upload to $table : " . $this->arrStr($args);
    }
    else if ($key == 'D') {
      $msg = "Duplicated found in $table : " . $this->arrStr($args);
    }
    else if ($key == 'U') {
      $msg = "Updated in $table : " . $this->arrStr($args);
    }
    $this->job->updateMsg($key, $msg, $this->line_no);
  }

  /**
   * Runs entry error check for regular template.
   *
   * The input file should have
   *   1st line : pound sign (#) and template name.
   *   2nd line : headers with asteric for required fields.
   *   3rd line : data section starts.
   *
   * Entry error checking includes the following errors.
   *   1. Validation on header names.
   *   2. duplicated headers.
   *   3. The number of columns in data section.
   *   4. Missing required fields.
   *   5. Missing value on the required fields.
   *
   *   @return boolean
   */
  public function runErrorCheckEntry() {

    // Calls preProcessECE().
    mcl_print("- Calling preProcessECE() function", 2, 1.1);
    if (!$this->preProcessECE()) {
      $this->updateMsg('E', "Error at preProcessECE()");
      return FALSE;
    }

    // Local variables.
    $header_flag      = TRUE;
    $template_flag    = TRUE;
    $num_cols_headers = -1;
    $line_no          = 0;
    $header_dup_arr   = array();
    $columns          = array();

    // Gets the headers.
    $headers = $this->getVarHeaders();

    // Gets the regular expression for special columns.
    $regex = $this->getRegExSpCol();

    // Opens the input file.
    $infile = $this->csv->getFilepath();
    if (!($fh = fopen($infile, 'r'))) {
      $this->updateMsg('E', "Error : Fail to open the input file : $infile.");
      return;
    }
    while(!feof($fh)) {
      $this->setLineNo($line_no++);
      $line = trim(fgets($fh));
      if (!$line) {
        continue;
      }

      // Skips the template line if exists.
      if ($template_flag) {
        $template_flag = FALSE;

        // If the line does not start with '#', it does not have tempalte line.
        // So set the header flag on.
        if (preg_match("/^#/", $line)) {
          continue;
        }
        $header_flag = TRUE;
      }

      // Handles the headers line.
      if ($header_flag) {
        $header_flag = FALSE;

        // Tokenizes the line.
        $columns  = $this->_tokenizeLine($line, TRUE);
        $num_cols = sizeof($columns);

        // Sets the number of columns of the headers.
        $num_cols_headers = $num_cols;

        // Gets the valid column specified in the descendant template.
        $valid_columns = self::getValidColumns($this);

        // Checks each column.
        for ($i = 0; $i < sizeof($columns); $i++) {
          $column_name_orig = $columns[$i];

          // Skips if column is emtpy.
          if (!$column_name_orig) {
            continue;
          }

          // Checks if column has valid name and saves the position of the column.
          // Ignore the column name starting with '#', '@', '~' or '$'.
          // Don't skip if it is a valid column specified in the job parameter.
          $column_name = strtolower($column_name_orig);
          if (!array_key_exists($column_name, $headers) && !preg_match($regex, $column_name) && !in_array($column_name, $valid_columns)) {
            if (!preg_match("/^allele_\d+/", $column_name)) {
              $label = $this->getLabel($column_name, $column_name_orig);
              $this->updateMsg('W', "Invalid header : '$label' is not valid header name, so this column would be ignored.");
              continue;
            }
          }

          // Checks header name for duplication.
          if (array_key_exists($column_name, $header_dup_arr)) {
            $dup_column       = $header_dup_arr[$column_name];
            $char_dup_column  = PHPExcel_Cell::stringFromColumnIndex($dup_column);
            $char_i           = PHPExcel_Cell::stringFromColumnIndex($i);
            $label            = $this->getLabel($column_name);
            $this->updateMsg('E', "Duplicated header : Found duplicated header '$label' at column $char_dup_column and $char_i. Please removed either one.");
          }
          else {
            $headers[$column_name]['col'] = $i;
            $header_dup_arr[$column_name] = $i;
          }
        }

        // Checks if required columns exists.
        foreach ($headers as $column => $prop) {
          if ($prop['req'] && (!array_key_exists('col', $prop) || $prop['col'] < 0)) {
            $label = $this->getLabel($column);
            $this->updateMsg('E', "Error : Required column [$label] does not exists.");
          }
        }
      }

      // Handles data lines.
      else {

        // Tokenizes the line. Skips the rest of data section if no data is in the line.
        $columns = $this->_tokenizeLine($line, FALSE);
        if (empty($columns)) {
          break;
        }

        // The number of columns in data line.
        if ($num_cols != $num_cols_headers) {
          $this->updateMsg('E', "Error : Invalid number of columns ($num_cols/$num_cols_headers)");
        }

        // Checks if the value of required column not empty.
        foreach ($headers as $column => $prop) {
          if (array_key_exists('req', $prop) && array_key_exists('col', $prop)) {
            if ($prop['req']) {
              $col    = $prop['col'];
              $value  = $columns[$col];
              if (!$value && $value === '') {
                $label = $this->getLabel($column);
                $this->updateMsg('E', "Error : Missing data on column [$label]");
              }
            }
          }
        }
      }
    }
    fclose($fh);

    // Updates the headers.
    $this->updateHeaders($headers);

    // Calls postProcessECE().
    mcl_print("- Calling postProcessECE() function", 2, 1.2);
    if (!$this->postProcessECE()) {
      $this->updateMsg('E', "Error at postProcessECE()");
      return FALSE;
    }
    return $this->isPassed();
  }

  /**
   * Returns the label of the given column if exist..
   *
   * @param string $column
   * @param string $colmn_orig
   *
   * @return string
   */
  private function getLabel($column, $column_orig = '') {
    if (array_key_exists($column, $this->headers)) {
      if (array_key_exists('label', $this->headers[$column])) {
        return $this->headers[$column]['label'];
      }
    }
    return $column_orig ? $column_orig : $column;
  }

  /**
   * Checks if the provided column is a valid column name for a column-template.
   *
   * @param string $column
   *
   * @return boolean
   */
  public function isValidColumnTemplateHeaders($column) {
    $valid_arr = array(
      'name', 'alias', 'cv_name', 'description'
    );
    return in_array($column, $valid_arr);
  }

  /**
   * Cleanup the input line and return it in an array.
   * Removes double quote. If $headers is TRUE, remove '*'. It returns
   * an empty array if no data present in a data line.
   *
   * @param string $line
   * @param boolean $headers
   *
   * @return array
   */
  private function _tokenizeLine($line, $headers = FALSE) {

    // Tokenizes the CSV line.
    $columns = str_getcsv($line);

    // Clean up the tokens.
    $data_str = '';
    foreach ($columns as &$column) {
      $column = trim(trim($column, '"'));
      if ($headers) {
        $column = trim(strtolower(str_replace('*', '', $column)));
      }
      $data_str .= $column;
    }
    if (trim($data_str)) {
      return $columns;
    }
    return array();
  }

  /**
   * Run data error check.
   *
   * Data error checking includes the following errors.
   * - existence of data
   * - consistency of data
   *
   * @return boolean
   */
  public function runErrorCheckData() {

    // Calls preProcessECD().
    mcl_print("- Calling preProcessECD() function", 2, 1.1);
    if (!$this->preProcessECD()) {
      $this->updateMsg('E', "Error at preProcessECD()");
      return FALSE;
    }

    // Calls the pre-process of merge check.
    mcl_print("- Calling preMergeCheck() function", 2, 1.1);
    if (!$this->preMergeCheck()) {
      $this->updateMsg('E', "Error at preMergeCheck()");
      return FALSE;
    }

    // Local variables.
    $header_flag    = TRUE;
    $template_flag  = TRUE;
    $line_no        = 0;
    $line_arr       = NULL;
    $columns        = NULL;

    // Gets the headers.
    $headers = $this->getHeaders();

    // Updates $cvterms for the template.
    if (!$this->updateCvterms()) {
      return FALSE;
    }

    // Gets the valid columns specified in the descendant template.
    $valid_columns = self::getValidColumns($this);

    // Gets the regular expression for special columns.
    $regex = $this->getRegExSpCol();

    // Gets the input file.
    $infile = $this->csv->getFilePath();

    // Opens the input file and get the number of lines in the file.
    if (!($fh = fopen($infile, 'r'))) {
      $this->updateMsg('E', "Error : Fail to open the input file : $infile.");
      return FALSE;
    }
    $total_lines = mcl_get_num_lines($infile);

    // Performs the error ckecking.
    $header_checked = FALSE;
    while(!feof($fh)) {
      $line_no++;
      $line = trim(fgets($fh));
      if (!$line) {
        continue;
      }

      // Skips the template line if exists..
      if ($template_flag) {
        $template_flag = FALSE;

        // If the line does not start with '#', it does not have tempalte line.
        // So set the header flag on.
        if (preg_match("/^#/", $line)) {
          continue;
        }
        $header_flag = TRUE;
      }

      // Handles the headers line.
      if ($header_flag) {
        $header_flag = FALSE;

        // Tokenizes the line.
        $columns = $this->_tokenizeLine($line, TRUE);

        // Saves the position of the column.
        for ($i =0; $i < sizeof($columns); $i++) {
          $column_name = $columns[$i];

          // Skips if column is emtpy.
          if (!$column_name) {
            continue;
          }

          // Updates the header column array.
          if (array_key_exists($column_name, $headers)) {
            $headers[$column_name]['col'] = $i;
          }

          // Handles the special coulmns.
          else if (preg_match($regex, $column_name, $matches)) {
            $headers[$column_name]['col'] = $i;
          }

          // Handles the valid columns provided in the job paramters.
          else if (in_array($column_name, $valid_columns)) {
            $headers[$column_name]['col'] = $i;
          }
        }

        // Cleanups the headers.
        foreach($headers as $column => $info) {
          if (!array_key_exists('col', $info)) {
            unset($headers[$column]);
          }
        }
      }

      // Handles data lines.
      else {

        // Tokenizes the line. Skips the rest of data section if no data is in the line.
        $columns = $this->_tokenizeLine($line, FALSE);
        if (empty($columns)) {
          break;
        }

        // Populates the data line array.
        $line_arr = array('line_no' => $line_no);
        foreach ($headers as $column => $prop) {
          if (array_key_exists('col', $prop) && $prop['col'] > -1) {
            $line_arr[$column] = trim($columns[$prop['col']]);
          }
        }

        // Checks the headers (only once).
        if (!$header_checked) {
          if (!$this->checkHeaders($line_arr)) {
            break;
          }
          $header_checked = TRUE;
        }

        // Checks the line.
        $this->setLineNo($line_no);
        $this->runErrorCheckDataLine($line_arr);
      }

      // Updates the progress of the job.
      if ($line_no % 500 == 0) {
        //mcl_display_memory_usage("MEMORY-CHECK [ECD] = $line_no");
        if ($total_lines) {
          $progress = sprintf("%s : %d / %d = %.03f %% [Error Checking Data]", $this->template, $line_no, $total_lines, $line_no / $total_lines * 100.0);
          $this->job->setProgress($progress);
        }
      }
    }
    fclose($fh);
    if (!$this->isPassed()) {
      return FALSE;
    }

    // Updates the headers.
    $this->updateHeaders($headers);

    // Calls postProcessECD().
    mcl_print("- Calling postProcessECD() function", 2, 1.2);
    if (!$this->postProcessECD()) {
      $this->updateMsg('E', "Error at postProcessECD()");
      return FALSE;
    }

    // Calls the post-process for merge check.
    if (!$this->postMergeCheck()) {
      $this->updateMsg('E', "Error at postMerge()");
      return FALSE;
    }
    return $this->isPassed();
  }

  /**
   * Pre-process for check merge.
   *
   * @return boolean
   */
  public function preMergeCheck() {

    // Initializes the array for merge check.
    if ($this->getTemplateType() == 'MERGE') {
      $GLOBALS['merge_check'] = array(
        'source' => array(),
        'target' => array(),
      );
    }
    return TRUE;
  }

  /**
   * Adds the data for merge check.
   *
   * @param string $source
   * @param string $target
   *
   */
  public function mergeCheck($source, $target) {
    if ($source && $target) {
      $GLOBALS['merge_check']['source'][$source]++;
      $GLOBALS['merge_check']['target'][$target]++;
    }
  }

  /**
   * Post-process for check merge.
   *
   * @return boolean
   */
  public function postMergeCheck() {

    // Checks for duplication.
    $flag = TRUE;
    if ($this->getTemplateType() == 'MERGE') {
      foreach ((array)$GLOBALS['merge_check']['target'] as $target=> $num) {
        if ($num > 1 || array_key_exists($target, $GLOBALS['merge_check']['source'])) {
          $this->updateMsg('E', "[$target] is listed multiple times.");
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Checks the headers.
   *
   * @param array $line
   *
   * @return boolean
   */
  public function checkHeaders($line) {
    // Defined in child class.
    return TRUE;
  }

  /**
   * Checks a data line for this template.
   *
   * @param associative array $line
   */
  public function runErrorCheckDataLine($line) {
    // Defined in child class.
  }

  /**
   * Uploads a CSV data file.
   *
   * @return boolean
   */
  public function uploadData() {

    // Calls preProcessECD().
    mcl_print("- Calling preProcessDU() function", 2, 1.2);
    if (!$this->preProcessDU()) {
      $this->updateMsg('E', 'Error at preProcessDU()');
      return FALSE;
    }

    // Local variables.
    $header_flag    = TRUE;
    $template_flag  = TRUE;
    $line_no        = 0;
    $columns        = NULL;
    $line_arr       = NULL;

    // Gets the headers.
    $headers = $this->getHeaders();

    // Updates the cvterms for the template.
    if (!$this->updateCvterms()) {
      $this->updateMsg('E', "Error : Updating cvterms.");
      return FALSE;
    }

    // Opens the input file.
    $infile = $this->csv->getFilePath();
    if (!($fh = fopen($infile, 'r'))) {
      $this->updateMsg('E', "Error : Fail to open the input file : $infile.");
      return FALSE;
    }

    // Gets the number of lines in the file.
    $total_lines = mcl_get_num_lines($infile, FALSE);

    // Gets the regular expression for special columns.
    $regex = $this->getRegExSpCol();

    // Upload data by line.
    while(!feof($fh)) {
      $line_no++;
      $line = trim(fgets($fh));
      if (!$line) {
        continue;
      }

      // Skips the template line.
      if ($template_flag) {
        $template_flag = FALSE;

        // If the line does not start with '#', it does not have tempalte line.
        // So set the header flag on.
        if (preg_match("/^#/", $line)) {
          continue;
        }
        $header_flag = TRUE;
      }

      // Handles the headers line.
      if ($header_flag) {
        $header_flag = FALSE;

        // Tokenizes the line.
        $columns = $this->_tokenizeLine($line, TRUE);
        if (empty($columns)) {
          break;
        }

        // Saves the position of the column.
        for ($i =0; $i < sizeof($columns); $i++) {
          $column_name = $columns[$i];
          if (array_key_exists($column_name, $headers) || preg_match($regex, $column_name)) {
            $headers[$column_name]['col'] = $i;
          }
        }
      }

      // Handles data lines.
      else {

        // Tokenizes the line.  Skips the rest of data section if no data is in the line.
        $columns = $this->_tokenizeLine($line, FALSE);
        if (empty($columns)) {
          break;
        }

        // Populates the data line array.
        $line_arr = array('line_no' => $line_no);
        foreach ($headers as $column => $prop) {
          if (array_key_exists('col', $prop) && $prop['col'] > -1) {
            $line_arr[$column] = trim($columns[$prop['col']]);
          }
        }
        $this->setLineNo($line_no);
        $this->uploadDataLine($line_arr);
      }

      // Updates the progress of the job.
      if ($line_no % 500 == 0) {
        //mcl_display_memory_usage("MEMORY-CHECK [UD] = $line_no");
        $perc = ($total_lines == 0) ? 0 : $line_no / $total_lines * 100.0;
        $progress = sprintf("%s : %d / %d = %.03f %% [Uploading Data]", $this->template, $line_no, $total_lines, $perc);
        $this->job->setProgress($progress);
      }
    }
    fclose($fh);

    // Calls postProcessDU().
    mcl_print("- Calling postProcessDU() function", 2, 1.2);
    if (!$this->postProcessDU()) {
      $this->updateMsg('E', 'Error at postProcessDU()');
      return FALSE;
    }
    return $this->isPassed();
  }

  /**
   * Upload a data line for this template.
   *
   * @param associative array $line
   */
  public function uploadDataLine($line) {
    // Defined in child class.
  }

  /**
   * The function to be called before runErrorCheckEntry().
   *
   * @return boolean
   */
  public function preProcessECE() {
    // Defined in child class.
    return TRUE;
  }

  /**
   * The function to be called before runErrorCheckData().
   *
   * @return boolean
   */
  public function preProcessECD() {
    // Defined in child class.
    return TRUE;
  }

  /**
   * The function to be called before uploadData().
   *
   * @return boolean
   */
  public function preProcessDU() {
    // Defined in child class.
    return TRUE;
  }

  /**
   * The function to be called after runErrorCheckEntry().
   *
   * @return boolean
   */
  public function postProcessECE() {
    // Defined in child class.
    return TRUE;
  }

  /**
   * The function to be called after runErrorCheckData().
   *
   * @return boolean
   */
  public function postProcessECD() {
    // Defined in child class.
    return TRUE;
  }

  /**
   * The function to be called after uploadData().
   *
   * @return boolean
   */
  public function postProcessDU() {
    // Defined in child class.
    return TRUE;
  }

  /**
   * Checks for a delete flag.
   * s
   * @param string $value
   *
   * @return boolean
   */
  public function checkDeleteFlag($value) {
    if ($value) {
      if (!preg_match("/^(yes|no)$/i", $value)) {
        $this->updateMsg('E', "Please set 'yes' or 'no'.s");
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Checks for a length of a value.
   *
   * @param integer $max_length
   * @param string $key
   * @param string $value
   *
   * @return boolean
   */
  public function checkLength($max_length, $key, $value) {
    if ($value && (strlen($value) >= $max_length)) {
      $this->updateMsg('E', "$key '$value' exceeds the maximum length ($max_length) of the data.");
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Checks the strand. It must be 1, +1, 0, -1.
   *
   * @param string $strand
   *
   * @return boolean
   */
  public function checkStrand($strand) {
    if ($strand || $strand == '0') {
      if (!preg_match("/^[+-]?[10]$/", $strand)) {
        $this->updateMsg('E', "The strand '$strand' is an invalid.");
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Checks for min and max values.
   *
   * @param integer $min
   * @param integer $max
   * @param boolean $required
   *
   * @return boolean
   */
  public function checkRange($min, $max, $required = TRUE) {
    if (($min || $min == '0') && ($max || $max == '0')) {
      if ($min > $max) {
        $this->updateMsg('E', "The min '$min' exceeds the max '$max'.");
        return FALSE;
      }
    }
    else if (($min || $min == '0') || ($max || $max == '0')) {
      if ($required) {
        $this->updateMsg('E', "Either min or max is missing.");
        return FALSE;
      }
    }
    else {
      if ($required) {
        $this->updateMsg('E', "You need to specify the min and/or max.");
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Checks for alpha numeric.
   *
   * @param string $str
   *
   * @return boolean
   */
  public function checkAlpahNumeric($str) {
    if (!preg_match("/^[a-zA-z0-9 _\-\.]+$/", $str)) {
      $this->updateMsg('E', "Please use alpha-numeric and [_-.]");
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Returns the special columns that start with the provided prefix.
   *
   * @param string $prefix
   *
   * @return array
   */
  public function getSpecialColumns($prefix) {

    // Sets the regular expression.
    $separator = "\\";
    $regex = '/^' . $separator . implode($separator, str_split($prefix)) . '.+/';

    // Gets the matced columns.
    $columns = array();
    foreach ($this->headers as $column_name => $prop) {
      if (preg_match($regex, $column_name)) {
        $columns[$column_name] = $prop;
      }
    }
    return $columns;
  }

  /**
   * Checks special columns.
   *
   * @param string $prefix
   * @param integer $cv_id
   * @param array $line
   *
   * @return boolean
   */
  public function saveSpecialColumns($prefix, $cv_id, $line) {
    if (preg_match("/##/", $prefix)) {
      return $this->_saveProperty($prefix, $cv_id, $line);
    }
    else if ($prefix == '#') {
      return $this->_saveDescriptor($prefix, $cv_id, $line);
    }
    return TRUE;
  }


  /**
   * Saves the user-specific properties in job.param.
   *
   * @param string $prefix
   * @param integer $cv_id
   * @param array $line
   *
   * @return boolean
   */
  private function _saveProperty($prefix, $cv_id, $line) {
    $flag = TRUE;

    // Goes through headers stored in the line array.
    $properties = array();
    foreach ((array)$line as $column => $value) {
      $regex = '/^' . $prefix . '[A-Za-z0-9\_]/';
      if (preg_match($regex, $column)) {
        $cvterm_name = str_replace($prefix, '', $column);

        // Gets the cvterm.
        $cvterm = MCL_CHADO_CVTERM::getCvtermByCvID($cv_id, $cvterm_name);
        if ($cvterm) {
          $properties[$cvterm->getCvtermID()] = strtolower($cvterm->getName());
        }
        else {
         // $this->updateMsg('E', "$cvterm_name (property) not found in cvterm");
         // $flag = FALSE;
        }
      }
    }
    if ($flag) {
      $this->getJob()->setParamByKey($prefix, $properties);
      $this->getJob()->update();
    }
   return $flag;
  }

  /**
   * Saves trait descriptors for cvterm_id. Save them in job.param.
   *
   * @param string $prefix
   * @param integer $cv_id
   * @param array $line
   *
   * @return boolean
   */
  private function _saveDescriptor($prefix, $cv_id, $line) {
    $flag = TRUE;

    // Gets the site CV name.
    $site_cv_name = MCL_SITE_VAR::getValueByName('SITE_CV');

    // Goes through headers stored in the line array.
    $descriptors = array();
    foreach ((array)$line as $column => $value) {

      if (preg_match("/^#[A-Za-z0-9\_]/", $column)) {
        $cvterm_name = substr($column, 1);

        // Gets the Field Book descriptor.
        $descriptor = MCL_CHADO_CVTERM::getFBDescriptor($site_cv_name, $cv_id, $cvterm_name);
        if ($descriptor) {
          $descriptors[$descriptor['cvterm_id']] = $descriptor;
        }
        else {
          $this->updateMsg('E', "cvterm \"$cvterm_name\" not found in site cv \"$site_cv_name\"");
        }
      }
    }

    // Saves the desciptor in the job param.
    if ($flag) {
      $this->getJob()->setParamByKey($prefix, $descriptors);
      $this->getJob()->update();
    }
    return $flag;
  }

  /**
   * Adds special columns.
   *
   * @param string $prefix
   * @param CHADO_TABLE $table_obj
   * @param array $line
   *
   * @return array
   */
  public function addSpecialColumns($prefix, $table_obj, $line) {

    // Gets the special columns and adds it as a property.
    $prop = array();
    $columns = $this->getJob()->getParamByKey($prefix);
    foreach ((array)$columns as $cvterm_id => $cvterm_name) {
      $value = $line[$prefix . $cvterm_name];
      if ($value != '') {
        $table_obj->addPropByID($this, $cvterm_id, $value);
        $prop[$cvterm_id] = $value;
      }
    }
    return $prop;
  }

  /**
   * Adds special columns with provided rank.
   *
   * @param string $prefix
   * @param CHADO_TABLE $table_obj
   * @param array $line
   * @param integer $rank
   *
   * @return array
   */
  public function addSpecialColumnsRank($prefix, $table_obj, $line, $rank = '') {

    // Gets the special columns and adds it as a property with the rank.
    $prop = array();
    if ($rank != '') {
      $columns = $this->getJob()->getParamByKey($prefix);
      foreach ((array)$columns as $cvterm_id => $cvterm_name) {
        $value = $line[$prefix . $cvterm_name];
        if ($value != '') {
          $table_obj->addPropRankByID($this, $cvterm_id, $value, $rank);
          $prop[$cvterm_id] = $value;
        }
      }
    }
    return $prop;
  }

  /**
   * Adds one or more properties
   *
   * @param string $table_name
   * @param string $target_field
   * @param integer $target_id
   * @param integer $type_id
   * @param string $value
   * @param string $separator
   */
  public function addProperty($table_name, $target_field, $target_id, $type_id, $value, $separator = '') {
    if ($value != '') {

      // Checks duplication before adding a new property.
      $args = array(
        $target_field => $target_id,
        'type_id'     => $type_id,
      );
      $class_name = strtoupper('CHADO_' . $table_name);
      $obj = $class_name::byKey($args);
      if ($obj) {
        $this->addMsg('D', $table_name, $args);
      }
      else {

        // Populates the array with values.
        $value_arr = preg_split(CHADO_TABLE::getSepRegex($separator), $value, -1, PREG_SPLIT_NO_EMPTY);

        // Adds each property.
        $rank = 0;
        foreach ($value_arr as $val) {
          $args['value']  = trim($val);
          $args['rank']   = $rank++;
          $obj = new $class_name($args);
          if ($obj->insert()) {
            $this->addMsg('N', $table_name, $args);
         }
          else {
            $this->addMsg('E', $table_name, $args);
          }
        }
      }
    }
  }

  /**
   * Adds one property with the specified rank.
   *
   * @param string $table_name
   * @param string $target_field
   * @param integer $target_id
   * @param integer $type_id
   * @param string $value
   * @param integer $rank
   */
  public function addPropertyRanked($table_name, $target_field, $target_id, $type_id, $value, $rank) {
    if ($value != '') {

      // Checks duplication before adding a new property.
      $args = array(
        $target_field => $target_id,
        'type_id'     => $type_id,
        'rank'        => $rank,
      );
      $class_name = strtoupper('CHADO_' . $table_name);
      $obj = $class_name::byKey($args);
      if ($obj) {
        $this->addMsg('D', $table_name, $args);
      }
      else {

        // Adds a property.
        $obj = new $class_name($args);
        if ($obj->insert()) {
          $this->addMsg('N', $table_name, $args);
        }
        else {
          $this->addMsg('E', $table_name, $args);
        }
      }
    }
  }

  /**
   * Gets the property.
   *
   * @param string $table_name
   * @param string $target_field
   * @param integer $target_id
   * @param integer $type_id
   */
  public function getProperty($table_name, $target_field, $target_id, $type_id) {

    // Gets the value of the property table.
    $args = array(
      $target_field => $target_id,
      'type_id'     => $type_id,
    );
    $class_name = strtoupper('CHADO_' . $table_name);
    $obj = $class_name::byKey($args);
    if ($obj) {
      return $obj->getValue();
    }
    else {
      return '';
    }
  }

  /**
   * Adds a relationship.
   *
   * @param string $table_name
   * @param string $subject_field
   * @param integer $subject_id
   * @param string $object_field
   * @param string $object_id
   * @param integer $type_id
   */
  public function addRelationship($table_name, $subject_field, $subject_id, $object_field, $object_id, $type_id) {
    if ($subject_id && $object_id && $type_id) {

      // Checks duplication before adding a new relationship.
      $args = array(
        $subject_field  => $subject_id,
        $object_field   => $object_id,
        'type_id'       => $type_id,
      );
      $class_name = strtoupper('CHADO_' . $table_name);
      $obj = $class_name::byKey($args);
      if ($obj) {
        $this->addMsg('D', $table_name, $args);
      }
      else {

        // Adds a new relationship.
        $obj = new $class_name($args);
        if ($obj->insert()) {
          $this->addMsg('N', $table_name, $args);
        }
        else {
          $this->addMsg('E', $table_name, $args);
        }
      }
    }
  }

  /**
   * Adds a link (Adding to a linking table).
   *
   * @param string $table_name
   * @param string $field_1
   * @param integer $id_1
   * @param string $field_2
   * @param string $id_2
   */
  public function addLink($table_name, $field_1, $id_1, $field_2, $id_2, $type_id = NULL) {
    if ($id_1 && $id_2) {

      // Checks for a duplication before adding a new relationship.
      $args = array(
        $field_1  => $id_1,
        $field_2  => $id_2,
      );
      if ($type_id) {
        $args['type_id'] = $type_id;
      }
      $class_name = strtoupper('CHADO_' . $table_name);
      $obj = $class_name::byKey($args);
      if ($obj) {
        $this->addMsg('D', $table_name, $args);
      }
      else {

        // Adds a new link.
        $obj = new $class_name($args);
        if ($obj->insert()) {
          $this->addMsg('N', $table_name, $args);
        }
        else {
          $this->addMsg('E', $table_name, $args);
        }
      }
    }
  }

  /**
   * Returns the template summary.
   */
  public function templateSummary() {

    $summary = "\n\n\t================================\n" .
    "\tTemplate : " . $this->template . "\n" .
    "\tType     : " . $this->template_type . "\n" .
    "\t--------------------------------\n";





    return $summary;
  }

  /**
   * Creates a string from array.
   *
   * @param assoc array $args
   * @param string $field_1
   * @param integer $id_1
   * @param string $field_2
   * @param string $id_2
   */
  public function arrStr($args) {
    if (empty($args)) {
      return '';
    }
    $keys = '';
    $values = '';
    $flag = TRUE;
    foreach ($args as $key => $value) {
      $prefix = ', ';
      if ($flag) {
        $prefix = '';
        $flag = FALSE;
      }
      $keys .= $prefix . $key;
      $values .= $prefix . $value;
    }
    return "($keys) = ($values)";
  }

  /**
   * Retrieves the cvterm_id.
   *
   * @param string $cv_name
   * @param string $cvterm_name
   *
   * @return integer
   */
  public function getCvtermID($cv_name, $cvterm_name) {
    return $this->cvterms[$cv_name][$cvterm_name];
  }

  /**
   * Checks argments for empty.
   *
   * @param array $args
   *
   * @return boolean
   */
  public function checkReqArgs($args) {
    foreach ($args as $key => $value) {
      if ($value == '') {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Checks if there are data in a row.
   *
   * @param array $line
   * @param string $regex
   *
   * @return boolean
   * Returns TRUE if data row are empty.
   */
  public function isEmptyRow($line, $regex = '') {
    $data_str = '';
    foreach ($line as $column => $value) {
      if (preg_match("/$regex/", $column)) {
        $data_str .= trim($line[$column]);
      }
    }
    return empty($data_str);
  }

  /**
   * Sets the status of MCL file.
   *
   * @param string $status_label
   * @param boolean $flag
   *
   * @return string
   */
  public function setPassed($status_label, $flag) {
    $status = $this->getJob()->getStatusByLabel($status_label);
    if ($status) {
      $this->getCSV()->setPassed($status, $flag);
    }
  }

  /**
   * Returns the headers after converting name of variable columns.
   *
   * @return array
   */
  public function getVarHeaders() {
    $updated_headers = array();
    foreach ((array)$this->headers as $column => $prop) {
      if (array_key_exists('var', $prop) && $prop['var']) {
        $var_column = $this->mapVarColumn($column);
        $updated_headers[$var_column] = $prop;
      }
      else {
        $updated_headers[$column] = $prop;
      }
    }
    return $updated_headers;
  }

  /*
   * Defines getters and setters below.
   *
   */
  /**
   * Return if this template has been enabled.
   *
   * @return boolean
   */
  public function isEnabled() {
    return $this->isPublic();
  }

  /**
   * Return if this template has been enabled.
   *
   * @return boolean
   */
  public function isPublic() {
    return $this->public ? TRUE :FALSE;
  }

  /**
   * Retrieves the template type ID.
   *
   * @return integer
   */
  public function getTemplateTypeID() {
    if (!$this->template_type) {
      return '';
    }
    return $this->template_type->getTemplateTypeID();
  }

  /**
   * Retrieves the template type.
   *
   * @param integer $flag
   *
   * @return various
   */
  public function getTemplateType($flag = MCL_STRING) {
    if (!$this->template_type) {
      return '';
    }
    if ($flag == MCL_OBJECT) {
      return $this->template_type;
    }
    return $this->template_type->getType();
  }

  /**
   * Sets the template type.
   *
   * @param string $template_type
   */
  public function setTemplateType($template_type) {
    $this->template_type = MCL_TEMPLATE_TYPE::byKey(array('type' => $template_type));
  }

  /**
   * Retrieves the headers.
   *
   * @return string
   */
  public function getHeaders() {
    return $this->headers;
  }

  /**
   * Sets the headers.
   *
   * @param string $headers
   */
  public function setHeaders($headers) {
    $this->headers = $headers;
  }

  /**
   * Updates the headers.
   *
   * @param string $headers
   */
  public function updateHeaders($headers) {
    $this->headers = $headers;
    $this->update();
  }

  /**
   * Retrieves the job.
   *
   * @return MCL_JOB
   */
  public function getJob() {
    return $this->job;
  }

  /**
   * Sets the job.
   *
   * @param MCL_JOB $job
   */
  public function setJob($job) {
    $this->job = $job;
  }

  /**
   * Retrieves the csv.
   *
   * @return MCL_CSV_FILE
   */
  public function getCSV() {
    return $this->csv;
  }

  /**
   * Sets the csv.
   *
   * @param MCL_CSV_FILE $csv
   */
  public function setCSV($csv) {
    $this->csv = $csv;
  }

  /**
   * Retrieves the line no.
   *
   * @return integer
   */
  public function getLineNo() {
    return $this->line_no;
  }

  /**
   * Sets the line no.
   *
   * @param integer $line_no
   */
  public function setLineNo($line_no) {
    $this->line_no = $line_no;
  }

  /**
   * Retrieves the value of the provided key in $this->tmp.
   *
   * @param string $key
   *
   * @return string
   */
  public function getTmpByKey($key) {
    if (array_key_exists($key, $this->tmp)) {
      return $this->tmp[$key];
    }
    return NULL;
  }

  /**
   * Sets the value of the provided key in $this->tmp.
   *
   * @param string $template_type
   * @param string $value
   */
  public function setTmpByKey($key, $value) {
    $this->tmp[$key] = $value;
  }
}
