<?php
/**
 * The declaration of MCL_CHADO_CVTERM class.
 *
 */
class MCL_CHADO_CVTERM extends CHADO_CVTERM {

 /**
  *  Class data members.
  */
  /**
   * @see CHADO_CVTERM::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_CVTERM::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * Returns MCL_CHADO_CVTERM by ID.

   * @param integer $cvterm_id
   *
   * @return MCL_CHADO_CVTERM
   */
  public static function byID($cvterm_id) {
    return MCL_CHADO_CVTERM::byKey(array('cvterm_id' => $cvterm_id));
  }

  /**
   * @see CHADO_CVTERM::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * @see CHADO_CVTERM::delete()
   */
  public function delete() {

    // Saves dbxref_id and deletes dbxref after deleting this cvterm.
    $dbxref_id = $this->dbxref_id;

    // Deletes this cvterm.
    if (parent::delete()) {

      // Deletes the dbxref.
      db_delete('chado.dbxref')
        ->condition('dbxref_id', $dbxref_id, '=')
        ->execute();
      return TRUE;
    }
    return FALSE;
  }

  /**
   * @see CHADO_CVTERM::update()
   */
  public function update() {

    // Updates dbxref.accession.
    $dbxref = MCL_CHADO_DBXREF::byID($this->dbxref_id);
    if ($dbxref) {
      $accession = $this->name;
      $dbxref->setAccession($accession);
      if ($dbxref->update()) {
        return parent::update();
      }
    }
    return FALSE;
  }

  /**
   * Checks the existence of cvterm. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $cv_name
   * @param string $cvterm_name
   *
   * @return boolean
   */
  public static function checkCvterm(MCL_TEMPLATE $mcl_tmpl = NULL, $cv_name, $cvterm_name) {
    if ($cv_name && $cvterm_name) {

      // Gets the cvterm.
      $cvterm = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name);
      if (!$cvterm) {
        self::updateMsg($mcl_tmpl, 'E', "(cv_name, cvterm_name) = ($cv_name, $cvterm_name) not found in cvterm");
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Checks the format of the code. The codes should have pairs of value and
   * label separated by simi-colon. A pair should be separated by a colon.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $codes
   *
   * @return boolean
   */
  public static function checkCodes(MCL_TEMPLATE $mcl_tmpl = NULL, $codes) {
    if ($codes) {
      $pairs = preg_split("/;/", $codes, -1, PREG_SPLIT_NO_EMPTY);
      foreach ($pairs as $pair) {
        $tmp = preg_split("/:/", $pair);
        if (sizeof($tmp) != 2 || (trim($tmp[0]) == '' || trim($tmp[1]) == '')) {
          self::updateMsg($mcl_tmpl, 'E', "Please check the format of the codes ($codes)");
          return FALSE;
        }
      }
    }
    return TRUE;
  }

  /**
   * Checks the format of the values for quantitative_code and code.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $data_type
   * @param string $values
   *
   * @return boolean
   */
  public static function checkValues(MCL_TEMPLATE $mcl_tmpl = NULL, $data_type, $values) {
    $flag = TRUE;

    // Checks only for quantitative_code and code.
    if (preg_match("/^(code|quantitative_code)$/i", $data_type)) {
      if ($values) {

        // Parses out the values to the codes.
        $codes = preg_split("/[;]/", $values, -1, PREG_SPLIT_NO_EMPTY);
        foreach ($codes as $code) {
          $tmp    = preg_split("/[:=]/", $code, -1, PREG_SPLIT_NO_EMPTY);
          $rank   = trim($tmp[0]);
          $value  = trim($tmp[1]);

          // Checks a rank.
          if (is_infinite($rank)) {
            self::updateMsg($mcl_tmpl, 'E', "Rank should be an integer ($rank was given)");
            $flag = FALSE;
          }

          // Checks a value.
          if (!$value) {
            self::updateMsg($mcl_tmpl, 'E', "Value must be non-empty");
            $flag = FALSE;
          }
        }
      }
      else {
        self::updateMsg($mcl_tmpl, 'E', "values cannot be empty for $data_type.");
        $flag = FALSE;
      }
    }
    return $flag;
  }


  /**
   * Check categorical values.
   *
   * Examples of categorical values.
   *   White/Yellow/Red
   *   melting/non-melting/slow softening
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $format
   * @param string $categories
   *
   * @return boolean
   */
  public static function checkCategories(MCL_TEMPLATE $mcl_tmpl = NULL, $format, $categories) {
    if ($format == 'categorical' || $format == 'multicat') {
      if (!$categories) {
        self::updateMsg($mcl_tmpl, 'E', "'categories' column cannot be empty.");
        return FALSE;
      }

      // Checks for an error.
      $error = self::checkFBCategories($categories);
      if ($error) {
        self::updateMsg($mcl_tmpl, 'E', "Error : $error.");
        return FALSE;
      }
    }
    else {
      if ($categories) {
        self::updateMsg($mcl_tmpl, 'E', "'categories' column must be empty for '$format' format.");
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Returns the format of the cvterm if exists.
   *
   * @return string
   */
  public function getFormat() {

    // Gets the format of the cvterm stored in cvtermprop.
    return $this->getProp('SITE_CV', 'format');
  }

  /**
   * Returns the searchable of the cvterm if exists.
   *
   * @return string
   */
  public function getSearchable() {

    // Gets the searchable of the cvterm stored in cvtermprop.
    return $this->getProp('SITE_CV', 'searchable');
  }

  /**
   * Returns the cvterm by cv and cvterm names.

   * @param string $cv_name
   * @param string $cvterm_name
   *
   * @return MCL_CHADO_CVTERM
   */
  public static function getCvterm($cv_name, $cvterm_name) {
    $cv_name = trim($cv_name);
    $cv = MCL_CHADO_CV::getCV($cv_name);
    if ($cv) {
      $cvterm_name = trim($cvterm_name);
      return MCL_CHADO_CVTERM::byKey(array('cv_id' => $cv->getCvID(), 'name' => $cvterm_name));
    }
    return NULL;
  }

  /**
   * Returns the cvterm by cv ID and cvterm name.
   *
   * @param integer $cvterm_id
   * @param string $cvterm_name
   *
   * @return MCL_CHADO_CVTERM
   */
  public static function getCvtermByCvID($cv_id, $cvterm_name) {
    $cv = MCL_CHADO_CV::byID($cv_id);
    if ($cv) {
      return MCL_CHADO_CVTERM::byKey(array('cv_id' => $cv_id, 'name' => $cvterm_name));
    }
    return NULL;
  }

  /**
   * Returns the Field Book descriptor.
   *
   * @param integer $site_cv_name
   * @param integer $cvterm_id
   * @param string $cvterm_name
   *
   * @return array
   */
  public static function getFBDescriptor($site_cv_name, $cv_id, $cvterm_name) {
    $cvterm = MCL_CHADO_CVTERM::getCvtermByCvID($cv_id, $cvterm_name);
    if ($cvterm) {
      $cvterm_id = $cvterm->getCvtermID();
      $sql = "
        SELECT C.cvterm_id, C.name, FORMAT.value AS format,
          MINIMUM.value AS minimum, MAXIMUM.value AS maximum,
          DEF_VAL.value AS defaultValue, CAT.value AS categories
        FROM {chado.cvterm} C
          INNER JOIN (
            SELECT CP.cvterm_id, CP.value
            FROM chado.cvtermprop CP
              INNER JOIN chado.cvterm C on C.cvterm_id = CP.type_id
              INNER JOIN chado.cv CV on CV.cv_id = C.cv_id
            WHERE LOWER(C.name) = :format AND LOWER(CV.name) = LOWER(:site_cv_name)
          ) FORMAT on FORMAT.cvterm_id = C.cvterm_id
          LEFT JOIN (
            SELECT CP.cvterm_id, CP.value
            FROM chado.cvtermprop CP
              INNER JOIN chado.cvterm C on C.cvterm_id = CP.type_id
              INNER JOIN chado.cv CV on CV.cv_id = C.cv_id
            WHERE LOWER(C.name) = :minimum AND LOWER(CV.name) = LOWER(:cv_name)
          ) MINIMUM on MINIMUM.cvterm_id = C.cvterm_id
          LEFT JOIN (
            SELECT CP.cvterm_id, CP.value
            FROM chado.cvtermprop CP
              INNER JOIN chado.cvterm C on C.cvterm_id = CP.type_id
              INNER JOIN chado.cv CV on CV.cv_id = C.cv_id
            WHERE LOWER(C.name) = :maximum AND LOWER(CV.name) = LOWER(:cv_name)
          ) MAXIMUM on MAXIMUM.cvterm_id = C.cvterm_id
          LEFT JOIN (
            SELECT CP.cvterm_id, CP.value
            FROM chado.cvtermprop CP
              INNER JOIN chado.cvterm C on C.cvterm_id = CP.type_id
              INNER JOIN chado.cv CV on CV.cv_id = C.cv_id
            WHERE LOWER(C.name) = :defaultValue AND LOWER(CV.name) = LOWER(:cv_name)
          ) DEF_VAL on DEF_VAL.cvterm_id = C.cvterm_id
          LEFT JOIN (
            SELECT CP.cvterm_id, CP.value
            FROM chado.cvtermprop CP
              INNER JOIN chado.cvterm C on C.cvterm_id = CP.type_id
              INNER JOIN chado.cv CV on CV.cv_id = C.cv_id
            WHERE LOWER(C.name) = :categories AND LOWER(CV.name) = LOWER(:cv_name)
          ) CAT on CAT.cvterm_id = C.cvterm_id
        WHERE C.cvterm_id = :cvterm_id
        ORDER BY C.name
      ";
      $args = array(
        ':site_cv_name' => $site_cv_name,
        ':cv_name'      => 'BIMS_FIELD_BOOK_FORMAT_PROP',
        ':format'       => 'format',
        ':minimum'      => 'minimum',
        ':maximum'      => 'maximum',
        ':defaultValue' => 'defaultValue',
        ':categories'   => 'categories',
        ':cvterm_id'    => $cvterm_id
      );
      return db_query($sql, $args)->fetch(PDO::FETCH_ASSOC);
    }
    return NULL;
  }

  /**
   * Returns MCL_CHADO_CV.
   *
   * @return MCL_CHADO_CV|NULL
   */
  public function getCv() {
    $cv = MCL_CHADO_CV::byID($this->cv_id);
    if ($cv) {
      return $cv;
    }
    return NULL;
  }

  /**
   * Returns the cvtermprop.

   * @param string $cv_name
   * @param string $cvterm_name
   *
   * @return string
   */
  public function getCvtermprop($cv_name, $cvterm_name) {

    // Gets Updates cv.name
    $cvterm = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name);
    if ($cvterm) {
      $args = array(
        'cvterm_id' => $this->cvterm_id,
        'type_id'   => $cvterm->getCvtermID(),
      );
      $prop = CHADO_CVTERMPROP::byKey($args);
      if ($prop) {
        return $prop->getValue();
      }
    }
    return '';
  }

  /**
   * Returns the all cvterms that belong to the provide cv.

   * @param integer $cv_id
   * @param integer $flag
   *
   * @return array
   */
  public static function getCvtermsByCvID($cv_id, $flag = MCL_CLASS) {

    // Gets the cvterms.
    $sql = "SELECT C.* FROM {chado.cvterm} C WHERE C.cv_id = :cv_id ORDER BY C.name";
    $result = db_query($sql, array(':cv_id' => $cv_id));
    $cvterms = array();
    while ($obj = $result->fetchObject()) {
      if ($flag == MCL_CLASS) {
        $cvterms []= MCL_CHADO_CVTERM::byID($obj->cvterm_id);
      }
      else if ($flag == MCL_OPTION) {
        $cvterms [$obj->cvterm_id] = $obj->name;
      }
      else if ($flag == MCL_OBJECT) {
        $cvterms []= $obj;
      }
    }
    return $cvterms;
  }

  /**
   * Adds null cvterm.
   *
   * @return MCL_CHADO_CVTERM
   */
  public static function addNull() {

    // Gets null.
    $cv     = MCL_CHADO_CV::addNull();
    $dbxref = MCL_CHADO_DBXREF::addNull();

    // Checks if it already exists.
    $cvterm = MCL_CHADO_CVTERM::getCvterm('null', 'null');
    if (!$cvterm) {
      $details = array(
        'name'      => 'null',
        'cv_id'     => $cv->getCvID(),
        'dbxref_id' => $dbxref->getDbxrefID(),
      );
      $cvterm = new MCL_CHADO_CVTERM($details);
      if (!$cvterm->insert()) {
        return NULL;
      }
    }
    return $cvterm;
  }

  /**
   * Adds a cvterm.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $db_name
   * @param string $cv_name
   * @param string $cvterm_name
   * @param string $definition
   * @param string $accession
   *
   * @return MCL_CHADO_CVTERM
   */
  public static function addCvterm(MCL_TEMPLATE $mcl_tmpl = NULL, $db_name, $cv_name, $cvterm_name, $definition = '', $accession = '') {

    // Updates DB and CV names.
    if (!$db_name) {
      $db_name = 'SITE_DB';
    }
    if (!$cv_name) {
      $cv_name = 'SITE_CV';
    }

    // Checks parameters.
    $args = array(
      'db_name'     => $db_name,
      'cv_name'     => $cv_name,
      'cvterm_name' => $cvterm_name,
    );

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $cvterm = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name);
    if ($cvterm) {
      self::updateMsg($mcl_tmpl, 'D', "(cv.name, cvterm.name) = ($cv_name, $cvterm_name) found in cvterm");
    }
    else {

      // Gets db.
      $db = MCL_CHADO_DB::getDB($db_name);
      if (!$db) {
        self::updateMsg($mcl_tmpl, 'E', "(db.name) = ($db_name) not found in db");
        return NULL;
      }

      // Gets cv.
      $cv = MCL_CHADO_CV::getCV($cv_name);
      if (!$cv) {
        self::updateMsg($mcl_tmpl, 'E', "(cv.name) = ($cv_name) not found in cv");
        return NULL;
      }

      // Sets the accession.
      if (!$accession) {
        $accession = $cvterm_name;
      }

      // Gets dbxref.
      $args = array(
        'db_id'     => $db->getDbID(),
        'accession' => $accession,
        'version'   => $cv->getCvID(),
      );
      $dbxref = MCL_CHADO_DBXREF::byKey($args);
      if (!$dbxref) {

        // Adds a new dbxref.
        $dbxref = new MCL_CHADO_DBXREF($args);
        if ($dbxref->insert()) {
          self::addMsg($mcl_tmpl, 'N', 'dbxref', $args);
        }
        else {
          self::addMsg($mcl_tmpl, 'E', 'dbxref', $args);
          return NULL;
        }
      }

      // Checks for duplication.
      $dbxref_id = $dbxref->getDbxrefID();
      $cvterm = MCL_CHADO_CVTERM::byKey(array('dbxref_id' => $dbxref_id));
      if ($cvterm) {
        self::updateMsg($mcl_tmpl, 'D', "(dbxref_id) = ($dbxref_id) found in cvterm");
      }
      else {
        $args = array(
          'dbxref_id'   => $dbxref_id,
          'cv_id'       => $cv->getCvID(),
          'name'        => $cvterm_name,
          'definition'  => $definition,
        );
        $cvterm = new MCL_CHADO_CVTERM($args);
        if ($cvterm->insert()) {
          self::addMsg($mcl_tmpl, 'N', 'cvterm', $args);
        }
        else {
          self::addMsg($mcl_tmpl, 'E', 'cvterm', $args);
          return NULL;
        }
      }
    }
    return $cvterm;
  }

  /**
   * Adds a property by type ID (cvterm ID).
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $type_id
   * @param string $value
   * @param string $separator
   *
   * @return boolean
   */
  public function addPropByID(MCL_TEMPLATE $mcl_tmpl = NULL, $type_id, $value, $separator = '') {
    if ($value != '') {
      return $this->addProperty($mcl_tmpl, 'cvtermprop', 'cvterm_id', $this->cvterm_id, $type_id, $value, $separator);
    }
    return TRUE;
  }

  /**
   * Gets the property.
   *
   * @param string $cv_name
   * @param string $cvterm_name
   *
   * @return string
   */
  public function getProp($cv_name, $cvterm_name) {
    $type_id = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name)->getCvtermID();
    return $this->getProperty('cvtermprop', 'cvterm_id', $this->cvterm_id, $type_id);
  }

  /**
   * Gets the property by the type ID.
   *
   * @param integer $type_id
   *
   * @return string
   */
  public function getPropByTypeID($type_id) {
    return $this->getProperty('cvtermprop', 'cvterm_id', $this->cvterm_id, $type_id);
  }

  /**
   * Adds a property.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $cv_name
   * @param string $cvterm_name
   * @param string $value
   * @param string $separator
   *
   * @return boolean
   */
  public function addProp(MCL_TEMPLATE $mcl_tmpl = NULL, $cv_name, $cvterm_name, $value, $separator = '') {
    if ($value != '') {
      $cvterm = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name);
      if ($cvterm) {
        $type_id = $cvterm->getCvtermID();
        return $this->addProperty($mcl_tmpl, 'cvtermprop', 'cvterm_id', $this->cvterm_id, $type_id, $value, $separator);
      }
    }
    return TRUE;
  }

  /**
   * Updates the property.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $cv_name
   * @param string $cvterm_name
   * @param string $value
   *
   * @return boolean
   */
  public function updateProp(MCL_TEMPLATE $mcl_tmpl = NULL, $cv_name, $cvterm_name, $value) {
    if ($value != '') {
      $type_id = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name)->getCvtermID();
      return $this->updateProperty($mcl_tmpl, 'cvtermprop', 'cvterm_id', $this->cvterm_id, $type_id, $value);
    }
    return TRUE;
  }

  /**
   * Updates the property by type ID.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $type_id
   * @param string $value
   * @param boolean $empty
   *
   * @return boolean
   */
  public function updatePropByTypeID(MCL_TEMPLATE $mcl_tmpl = NULL, $type_id, $value) {
    if ($value != '') {
      return $this->updateProperty($mcl_tmpl, 'cvtermprop', 'cvterm_id', $this->cvterm_id, $type_id, $value);
    }
    return TRUE;
  }

  /**
   * Empties the property by type ID.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $type_id
   * @param boolean $flag_delete
   *
   * @return boolean
   */
  public function emptyPropByTypeID(MCL_TEMPLATE $mcl_tmpl = NULL, $type_id, $flag_delete = FALSE) {

    // Gets the cvtermprop.
    $keys = array(
      'cvterm_id' => $this->cvterm_id,
      'type_id'   => $type_id,
    );
    $cvtermprop = CHADO_CVTERMPROP::byKey($keys);
    if ($cvtermprop) {
      if ($flag_delete) {
        return $cvtermprop->delete();
      }
      $cvtermprop->setValue('');
      return $cvtermprop->update();
    }
    return TRUE;
  }

  /**
   * Returns the codes.
   *
   * @param boolean $array
   *
   * @return various
   */
  public function getCodes($array = FALSE) {
    $codes = $this->getProp('SITE_CV', 'codes');
    if ($codes) {
      if ($array) {
        $arr = array();
        $pairs = preg_split("/;/", $codes, -1, PREG_SPLIT_NO_EMPTY);
        foreach ($pairs as $pair) {
          $tmp = preg_split("/:/", $pair);
          if (sizeof($tmp) == 2) {
            $key = trim($tmp[0]);
            $val = trim($tmp[1]);
            if ($key != '' && $val != '') {
              $arr[strtolower($key)] = array('key' => $key, 'val' => $val);
            }
          }
        }
        return $arr;
      }
      return $code;
    }
    return ($array) ? array() : '';
  }

  /**
   * Return the aliases.
   *
   * @param string $separator
   *
   * @return string
   */
  public function getAlias($separator = ';') {

    // Gets type_id of alias.
    $type_id = MCL_CHADO_CVTERM::getCvterm('SITE_CV', 'alias')->getCvtermID();

    // Gets all aliases.
    $sql = "
      SELECT CS.synonym
      FROM {chado.cvtermsynonym} CS
      WHERE CS.type_id = :type_id AND CS.cvterm_id = :cvterm_id
      ORDER BY CS.synonym
    ";
    $args = array(
      ':type_id'   => $type_id,
      ':cvterm_id' => $this->cvterm_id,
    );
    $results = db_query($sql, $args);
    $alias_arr = array();
    while ($cvterm_name = $results->fetchField()) {
      $alias_arr []= $cvterm_name;
    }

    // Returns the list of the aliases.
    return implode($separator, $alias_arr);
  }

  /**
   * Adds the aliases.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $cv_name
   * @param string $cvterm_name
   * @param string $alias
   * @param string $separator
   *
   * @return boolean
   */
  public function addAlias(MCL_TEMPLATE $mcl_tmpl = NULL, $cv_name, $cvterm_name, $alias, $separator = '') {
    $flag = TRUE;
    if ($alias) {

      // Gets cvterm.
      if (!MCL_CHADO_CVTERM::checkCvterm($mcl_tmpl, $cv_name, $cvterm_name)) {
        $this->updateMsg($mcl_tmpl, 'E', "(cv_name, cvterm_name) = ($cv_name, $cvterm_name) not found in cvterm");
        return FALSE;
      }
      $type_id = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name)->getCvtermID();

      // Populates the array with aliases.
      $alias_arr = array();
      if ($separator) {
        $alias_arr = preg_split($this->getSepRegex($separator), $alias, -1, PREG_SPLIT_NO_EMPTY);
      }
      else {
        $alias_arr []= $value;
      }

      // Adds each alias.
      $args = array(
        'cvterm_id' => $this->cvterm_id,
        'type_id'   => $type_id,
      );
      foreach ($alias_arr as $val) {
        $args['synonym']  = trim($val);
        $cvtermsynonym    = CHADO_CVTERMSYNONYM::byKey($args);
        if ($cvtermsynonym) {
          $this->addMsg($mcl_tmpl, 'D', 'cvtermsynonym', $args);
        }
        else {
          $cvtermsynonym = new CHADO_CVTERMSYNONYM($args);
          if ($cvtermsynonym->insert()) {
            $this->addMsg($mcl_tmpl, 'N', 'cvtermsynonym', $args);
          }
          else {
            $this->addMsg($mcl_tmpl, 'E', 'cvtermsynonym', $args);
            $flag = FALSE;
          }
        }
      }
    }
    return $flag;
  }

  /**
   * Updates the aliases.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $cv_name
   * @param string $cvterm_name
   * @param string $alias
   * @param string $separator
   *
   * @return boolean
   */
  public function updateAlias(MCL_TEMPLATE $mcl_tmpl = NULL, $cv_name, $cvterm_name, $alias, $separator = '') {
    if ($alias) {

      // Removes the existing properties first and add new ones.
      if (!$this->deleteAlias($cv_name, $cvterm_name)) {
        return FALSE;
      }
      return $this->addAlias($mcl_tmpl, $cv_name, $cvterm_name, $alias, $separator);
    }
    return TRUE;
  }

  /**
   * Deletes all the aliases for this cvterm.
   *
   * @param string $cv_name
   * @param string $cvterm_name
   *
   * @return boolean
   */
  public function deleteAlias($cv_name, $cvterm_name) {

    // Gets type_id.
    $type_id = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name)->getCvtermID();
    $args = array(
      'cvterm_id' => $this->cvterm_id,
      'type_id'   => $type_id,
    );

    // Deletes the aliases.
    $sql = "DELETE FROM {chado.cvtermsynonym} WHERE cvterm_id = :cvterm_id AND type_id = :type_id";
    db_query($sql, $args);
    return TRUE;
  }

  /**
   * Adds the provided values for quantitative_code, code and qualitative.
   *
   * Examples of quantitative_code.
   *   1:none;2:slight;3:medium;4:very greasy
   *   1:immature (starchy,granular);2:mature (no detectable startch);3:over mature (soft, lacks acid and contains overmature flavors)
   *
   * Examples of code.
   *   0:none;1:present
   *   1=UMN; 2=Cornell-Geneva; 3=WSU
   *
   * Examples of qualitative.
   *   Site; State;
   *   tier; field;
   *   tier; block;
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $data_type
   * @param string $values
   *
   * @return boolean
   */
  public function addValues(MCL_TEMPLATE $mcl_tmpl = NULL, $data_type, $values) {
    $flag = TRUE;
    if ($values) {

      // Adds the values for quantitative_code and code.
      if (preg_match("/^(code|quantitative_code)$/i", $data_type)) {

        // Gets type_id.
        $cvterm = MCL_CHADO_CVTERM::getCvterm('SITE_CV', 'code');
        if (!$cvterm) {
          $this->updateMsg($mcl_tmpl, 'E', "(cv_name, cvterm_name) = ('SITE_CV', 'code') does not exist in cvterm");
          return FALSE;
        }
        $cvterm_id_code = $cvterm->getCvtermID();

        // Saves the all codes in cvtermprop table.
        if (!$this->addProp($mcl_tmpl, 'SITE_CV', 'code_ref', $values)) {
          $flag = FALSE;
        }

        // Parses out the values to the codes.
        $codes = preg_split("/[;]/", $values, -1, PREG_SPLIT_NO_EMPTY);
        foreach ($codes as $code) {
          $tmp    = preg_split("/[:=]/", $code, -1, PREG_SPLIT_NO_EMPTY);
          $rank   = trim($tmp[0]);
          $value  = trim($tmp[1]);

          // Adds a property.
          if(!$this->addPropertyRanked($mcl_tmpl, 'cvtermprop', 'cvterm_id', $this->cvterm_id, $cvterm_id_code, $value, $rank)) {
            $flag = FALSE;
          }
        }
      }

      // Adds the values for qualitative.
      else if ($data_type == 'qualitative') {
        if (!$this->addProp($mcl_tmpl, 'SITE_CV', 'qualitative', $values, "[,;]")) {
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Links between a descriptor and a trait.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_CVTERM $cv_name
   *
   * @return boolean
   */
  public function linkTrait(MCL_TEMPLATE $mcl_tmpl = NULL, MCL_CHADO_CVTERM $trait = NULL) {
    if ($trait) {

      // Links a descriptor and a trait.
      // CarrotOmics change. if 'belongs_to' is not defined, code hangs here because 
      // we try to do a NULL->getCvtermID(). modify the code to fail with an error message
      $cvterm = MCL_CHADO_CVTERM::getCvterm('SITE_CV', 'belongs_to');
      if ($cvterm) {
        $type_id = $cvterm->getCvtermID();
        return $this->addLink($mcl_tmpl, 'cvterm_relationship', 'subject_id', $this->cvterm_id, 'object_id', $trait->getCvtermID(), $type_id);
      }
      else {
        drupal_set_message("Error : cvterm \"belongs_to\" is not defined in SITE_CV");
        mcl_print("*** ERROR cvterm \"belongs_to\" is not defined in SITE_CV", 0, 1.1);
        throw new exception("Error : cvterm \"belongs_to\" is not defined in SITE_CV");
      }
    }
    return TRUE;  // return code is not checked by caller
  }

  /**
   * Checks the categories.
   *
   * @param string $categories
   *
   * @return string
   */
  public static function checkFBCategories($categories) {
    $tmp = preg_split("/\//", $categories, -1, PREG_SPLIT_NO_EMPTY);
    $dup_arr = array();
    foreach ((array)$tmp as $cat) {
      if (array_key_exists($cat, $dup_arr)) {
        return "Duplicated category ($cat) found, please remove it";
      }
      $dup_arr[$cat] = TRUE;
    }
    return '';
  }

  /**
   * Checks the evaluation data in descriptor column.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param array $data_line
   * @param string $cv_name
   *
   * @return boolean
   */
  public static function checkFBDescriptorCol(MCL_TEMPLATE $mcl_tmpl = NULL, $data_line, $cv_name) {

    // Gets cvterm_name and value.
    $cvterm_name  = $data_line['trait'];
    $value        = $data_line['value'];

    // Gets the descriptors in $_GLOBAL.
    if (!array_key_exists('descriptors', $GLOBALS)) {
      $GLOBALS['descriptors'] = array();
    }

    // Gets the descriptor.
    if (!array_key_exists($cvterm_name, $GLOBALS['descriptors'])) {
      $cvterm       = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name);
      $descriptors  = $mcl_tmpl->getJob()->getParamByKey('#');
      $GLOBALS['descriptors'][$cvterm_name] = $descriptors[$cvterm->getCvtermID()];
    }
    $descriptor = $GLOBALS['descriptors'][$cvterm_name];

    // Updates the value if the value is empty and the descriptor has the default value.
    $defaultvalue = $descriptor['defaultvalue'];
    if ($defaultvalue && $value == '') {
      $value = $defaultvalue;
    }

    // Checks the value.
    if ($value || $value == '0') {
      return MCL_CHADO_CVTERM::checkFBDescriptorValue($mcl_tmpl, $descriptor, $value, $data_line['line_no'] - 1);
    }
    return TRUE;
  }

  /**
   * Checks the evaluation data in descriptor columns.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param array $data_line
   *
   * @return boolean
   */
  public static function checkFBDescriptorCols(MCL_TEMPLATE $mcl_tmpl = NULL, $data_line) {
    $flag = TRUE;

    // Checks the evaluation data for each descriptor.
    $descriptors = $mcl_tmpl->getJob()->getParamByKey('#');
    foreach ((array)$descriptors as $cvterm_id => $descriptor) {

      // Gets and checks the value.
      $name = strtolower($descriptor['name']);
      $value = $data_line['#' . $name];

      // Updates the value if the value is empty and the descriptor has the default value.
      $defaultvalue = $descriptor['defaultvalue'];
      if ($defaultvalue && $value == '') {
        $value = $defaultvalue;
      }

      // Checks the value.
      if ($value || $value == '0') {
        $bool = self::checkFBDescriptorValue($mcl_tmpl, $descriptor, $value, $data_line['line_no'] - 1);
        if (!$bool) {
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Checks the value for the descriptor.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param array $descriptor
   * @param string $value
   * @param integer $line_no
   *
   * @return boolean
   */
  public static function checkFBDescriptorValue(MCL_TEMPLATE $mcl_tmpl = NULL, $descriptor, $value, $row_no) {

    // Gets the variables.
    $cvterm_id    = $descriptor['cvterm_id'];
    $name         = $descriptor['name'];
    $format       = $descriptor['format'];
    $minimum      = $descriptor['minimum'];
    $maximum      = $descriptor['maximum'];
    $categories   = $descriptor['categories'];

    // Formats : numeric/percent/counter/categorical/boolean
    if (preg_match("/(numeric|percent|counter)/", $format)) {
      if ($format == 'numeric') {
        if (!is_numeric($value)) {
          self::updateMsg($mcl_tmpl, 'E', "$name ($format) : the value ($value) must be numeric at row $row_no");
          return FALSE;
        }
      }
      else if ($format == 'percent') {
        if (!is_numeric($value)) {
          self::updateMsg($mcl_tmpl, 'E', "$name ($format) : the value ($value) must be numeric at row $row_no");
          return FALSE;
        }
        if ($value > 100.0) {
          self::updateMsg($mcl_tmpl, 'E', "$name ($format) : the value ($value) must be between 0 and 100 at row $row_no");
          return FALSE;
        }
        else if ($value < 0.0) {
          self::updateMsg($mcl_tmpl, 'E', "$name ($format) : the value ($value) must be between 0 and 100 at row $row_no");
          return FALSE;
        }
      }
      else if ($format == 'counter') {
        if (is_int($value) && $value >= 0) {
          self::updateMsg($mcl_tmpl, 'E', "$name ($format) : the value ($value) must be a positive integer at row $row_no");
          return FALSE;
        }
      }

      // Check the ranges.
      if ($minimum && ($value < $minimum)) {
        self::updateMsg($mcl_tmpl, 'E', "[$value < $minimum]$name ($format) : the value ($value) is smaller than minimum ($minimum) at row $row_no");
        return FALSE;
      }
      if ($maximum && ($value > $maximum)) {
        self::updateMsg($mcl_tmpl, 'E', "$name ($format) : the value ($value) is bigger than maximum ($maximum) at row $row_no");
        return FALSE;
      }
    }
    else if ($format == 'boolean') {
      if (!preg_match("/^(true|false|1|0)$/i", $value)) {
        self::updateMsg($mcl_tmpl, 'E', "$name ($format) : the value ($value) must be 'true' or 'false' in lowercase at row $row_no");
        return FALSE;
      }
    }
    else if ($format == 'date') {
      if (preg_match("/^\d{1,2}\/\d{1,2}\/\d\d\d\d$/", $value)) {
        return TRUE;
      }
      else if (preg_match("/^\d\d\d\d\d$/", $value)) {
        self::updateMsg($mcl_tmpl, 'W', "$name ($format) : the value ($value) is not valid date format. It has 5-digit. The number is converted into the data. at row $row_no");
        return TRUE;
      }
      else {
        self::updateMsg($mcl_tmpl, 'E', "$name ($format) : the value ($value) is not valid date format. it must be m/d/yyyy. (e.g.) 1/1/2017 at row $row_no");
        return FALSE;
      }
    }
    else if ($format == 'categorical' || $format == 'multicat') {

      // Gets the categories and make them lowercase.
      $cats = preg_split("/\//", $categories, -1, PREG_SPLIT_NO_EMPTY);
      foreach ($cats as $idx => $val) {
        $cats[$idx] = strtolower($val);
      }

      // Separates the value and check each value.
      $values = preg_split("/:/", $value, -1, PREG_SPLIT_NO_EMPTY);
      $dup_arr = array();
      foreach ($values as $val) {
        if (!in_array(strtolower($val), $cats)) {
          !self::updateMsg($mcl_tmpl, 'E', "$name ($format) : the value ($val) must be one of '$categories' at row $row_no");
          return FALSE;
        }
        if (array_key_exists($val, $dup_arr)) {
          self::updateMsg($mcl_tmpl, 'E', "$name ($format) : The duplicated values ($val) exist at row $row_no");
          return FALSE;
        }
        $dup_arr[$val] = TRUE;
      }
    }
    return TRUE;
  }

  /**
   * Convert the date to Field Book date format.
   *
   * @param string $value
   *
   * @return string
   */
  public static function convertFBDate($value) {
    if (preg_match("/^(\d\d\d\d\d)$/", $value, $matches)) {
      $date_digit = $matches[1];
      $value = date("n/j/Y", mktime(0, 0 , 0 , 1, $date_digit - 1, 1900));
    }
    return $value;
  }
}
