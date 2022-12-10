<?php
/**
 * Adds namespace.
 */
/**
 * The declaration of CHADO_TABLE class.
 *
 */
class CHADO_TABLE  {

  /**
   *  Data members for CHADO_TABLE.
   */
  protected static $data_categories = array(
    'NUMERIC' => array('serial', 'smallint', 'bigint', 'integer', 'double', 'real'),
    'DATE'    => array('timestamp', 'timestamp_without_time_zone'),
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {}

  /**
   * Implements the class destructor.
   */
  public function __destruct() {}

  /**
   * Generate CHADO_TABLE object by key(s).
   *
   * @return descendance of CHADO_TABLE object.
   */
  public static function byKey($keys) {
    // To be overridden by Child class.
  }

  /**
   * Returns the name of the table.
   *
   * @param boolean $schema
   *
   * @return array
   */
  public function getTableName($schema = FALSE) {
    $table_name = get_class($this);
    if (preg_match("/CHADO_(.*)$/i", $table_name, $matches)) {
      $table_name = $matches[1];
    }
    if ($schema) {
      $table_name = 'chado.' . $table_name;
    }
    return strtolower($table_name);
  }

  /**
   * Returns the all tables in Chado schema.
   *
   * @return array
   */
  public static function getAllTableInChado() {

    // Gets all tables in the schema.
    $sql = "
      SELECT table_name FROM information_schema.tables
      WHERE LOWER(table_schema) = :schema
      AND LOWER(table_type) = :table_type $exclude
      ORDER BY table_name
    ";
    $args = array(
      ':schema'     => 'chado',
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
   * Returns the valid chado tables.
   *
   * @return array
   */
  public static function getChadoTables() {
    return array(
      'acquisition',
      'acquisitionprop',
      'acquisition_relationship',
      'analysis',
      'analysisfeature',
      'analysisfeatureprop',
      'analysis_organism',
      'analysisprop',
      'arraydesign',
      'arraydesignprop',
      'assay',
      'assay_biomaterial',
      'assay_project',
      'assayprop',
      'biomaterial',
      'biomaterial_dbxref',
      'biomaterialprop',
      'biomaterial_relationship',
      'biomaterial_treatment',
      'cell_line',
      'cell_line_cvterm',
      'cell_line_cvtermprop',
      'cell_line_dbxref',
      'cell_line_feature',
      'cell_line_library',
      'cell_lineprop',
      'cell_lineprop_pub',
      'cell_line_pub',
      'cell_line_relationship',
      'cell_line_synonym',
      'chadoprop',
      'channel',
      'contact',
      'contact_image',
      'contactprop',
      'contact_relationship',
      'control',
      'cv',
      'cvprop',
      'cvterm',
      'cvterm_dbxref',
      'cvterm_image',
      'cvtermpath',
      'cvtermprop',
      'cvterm_relationship',
      'cvtermsynonym',
      'db',
      'dbxref',
      'dbxrefprop',
      'eimage',
      'eimageprop',
      'element',
      'element_relationship',
      'elementresult',
      'elementresult_relationship',
      'environment',
      'environment_cvterm',
      'environmentprop',
      'expression',
      'expression_cvterm',
      'expression_cvtermprop',
      'expression_image',
      'expressionprop',
      'expression_pub',
      'feature',
      'feature_contact',
      'feature_cvterm',
      'feature_cvterm_dbxref',
      'feature_cvtermprop',
      'feature_cvterm_pub',
      'feature_dbxref',
      'feature_expression',
      'feature_expressionprop',
      'feature_genotype',
      'feature_image',
      'featureloc',
      'featureloc_pub',
      'featuremap',
      'featuremap_contact',
      'featuremap_dbxref',
      'featuremap_organism',
      'featuremapprop',
      'featuremap_pub',
      'featuremap_stock',
      'feature_nd_geolocation',
      'feature_phenotype',
      'featurepos',
      'featureposprop',
      'feature_project',
      'featureprop',
      'featureprop_pub',
      'feature_pub',
      'feature_pubprop',
      'featurerange',
      'feature_relationship',
      'feature_relationshipprop',
      'feature_relationshipprop_pub',
      'feature_relationship_pub',
      'feature_stock',
      'feature_synonym',
      'genotype',
      'genotype_call',
      'genotypeprop',
      'library',
      'library_contact',
      'library_cvterm',
      'library_dbxref',
      'library_feature',
      'libraryprop',
      'libraryprop_pub',
      'library_pub',
      'library_stock',
      'library_synonym',
      'magedocumentation',
      'mageml',
      'nd_experiment',
      'nd_experiment_contact',
      'nd_experiment_dbxref',
      'nd_experiment_genotype',
      'nd_experiment_phenotype',
      'nd_experiment_project',
      'nd_experimentprop',
      'nd_experiment_protocol',
      'nd_experiment_pub',
      'nd_experiment_stock',
      'nd_experiment_stock_dbxref',
      'nd_experiment_stockprop',
      'nd_geolocation',
      'nd_geolocationprop',
      'nd_protocol',
      'nd_protocolprop',
      'nd_protocol_reagent',
      'nd_reagent',
      'nd_reagentprop',
      'nd_reagent_relationship',
      'organism',
      'organism_dbxref',
      'organism_image',
      'organismprop',
      'organism_pub',
      'organism_relationship',
      'phendesc',
      'phenotype',
      'phenotype_call',
      'phenotype_comparison',
      'phenotype_comparison_cvterm',
      'phenotype_cvterm',
      'phenstatement',
      'phenstatementprop',
      'phylonode',
      'phylonode_dbxref',
      'phylonode_organism',
      'phylonodeprop',
      'phylonode_pub',
      'phylonode_relationship',
      'phylotree',
      'phylotree_pub',
      'project',
      'project_contact',
      'projectprop',
      'project_pub',
      'project_relationship',
      'protocol',
      'protocolparam',
      'pub',
      'pubauthor',
      'pubauthor_contact',
      'pub_dbxref',
      'pubprop',
      'pub_relationship',
      'quantification',
      'quantificationprop',
      'quantification_relationship',
      'stock',
      'stockcollection',
      'stockcollection_db',
      'stockcollectionprop',
      'stockcollection_stock',
      'stock_cvterm',
      'stock_cvtermprop',
      'stock_dbxref',
      'stock_dbxrefprop',
      'stock_genotype',
      'stock_image',
      'stockprop',
      'stockprop_pub',
      'stock_pub',
      'stock_relationship',
      'stock_relationship_cvterm',
      'stock_relationship_pub',
      'study',
      'study_assay',
      'studydesign',
      'studydesignprop',
      'studyfactor',
      'studyfactorvalue',
      'studyprop',
      'studyprop_feature',
      'synonym',
      'tableinfo',
      'treatment',
    );
  }

  /**
   * Returns the non-chado tables.
   *
   * @return array
   */
  public static function getNonChadoTables() {

    // Gets all tables in Chaod.
    $all_tables   = self::getAllTableInChado();
    $valid_tables = self::getChadoTables();
    $non_chado = array();
    foreach($all_tables as $table_name) {
      if (!in_array($table_name, $valid_tables)) {
        $non_chado []= $table_name;
      }
    }
    return $non_chado;
  }

  /**
   * Returns the related tables.
   *
   * @return array
   */
  public static function getRelTable() {
    // To be overridden by Child class.
    return array();
  }

  /**
   * Returns the associated data.
   *
   * @param string $primary_key
   * @param integer $primary_id
   * @param boolean $all_flag
   *
   * @return array
   */
  public static function getAssocData($primary_key, $primary_id, $all_flag = FALSE) {

    // Gets the related tables.
    $class_name = get_called_class();
    $ref_tables = $class_name::getRelTable();

    // Search the associated data in the related table.
    $assoc_data = array();
    $total      = 0;
    foreach ((array)$ref_tables[$primary_key] as $ref_table) {

      // Gets the table info.
      $table_name = $ref_table['table'];
      $attr_name  = $ref_table['attr'];
      $data_key   = "$table_name-$attr_name";

      // Sets the chado table and class name.
      $class_name   = 'CHADO_' . strtoupper($table_name);
      $chado_table  = 'chado.' . $table_name;

      // Initializes the $assoc_data.
      $assoc_data[$data_key] = array(
        'table' => $chado_table,
        'attr'  => $attr_name,
        'num'   => 0,
        'data'  => array(),
      );

      // Checks the table.
      if (!db_table_exists($chado_table)) {
        print "The table '$chado_table' does not exist\n";
        continue;
      }

      // Checks the class name.
      if (!class_exists($class_name)) {
        print "The class file for '$chado_table' does not exist\n";
        continue;
      }

      // Checks if the associated data exist in the current table
      $ref_table_obj = $class_name::byKey(array($attr_name => $primary_id));
      if ($ref_table_obj) {

        // Gets the user-defined unique keys.
        $user_unique_keys = $ref_table_obj->getMergeUniqueKeys($chado_table);

        // Gets the unique keys from table definition.
        $tbl_unique_keys = array();
        $uk = $ref_table_obj->getUK();
        foreach ((array)$uk as $name => $attrs) {
          if (in_array($attr_name, $attrs)) {
            $tbl_unique_keys = $attrs;
          }
        }

        // Checks the table unique key.
        if (empty($tbl_unique_keys) && empty($user_unique_keys)) {
          continue;
        }

        // Search for the associated data.
        $sql      = " SELECT * FROM {$chado_table} WHERE $attr_name = :$attr_name";
        $args     = array(":$attr_name" => $primary_id);
        $results  = db_query($sql, $args);
        while ($arr = $results->fetchAssoc()) {

          // Finds the associated data.
          $data = array();

          // Gets the data by the user defined unique key.
          if (!empty($user_unique_keys)) {
            foreach ($user_unique_keys as $key) {
              $data["$key"] = $arr[$key];
            }
          }

          // Gets the data by the table defined unique key.
          if (empty($data)) {
            foreach ($tbl_unique_keys as $key) {
              $data["$key"] = $arr[$key];
            }
          }

          // Adds the associated data.
          if (!empty($data)) {
            $total++;
            $assoc_data[$data_key]['num']++;
            $assoc_data[$data_key]['data'][]= $data;
          }
        }
      }
    }

    // Clean up $assoc_data.
    if (!$all_flag) {
      foreach ((array)$assoc_data as $data_key => $info) {
        if ($info['num'] == 0) {
          unset($assoc_data[$data_key]);
        }
      }
    }
    return array('total' => $total, 'assoc_data' => $assoc_data);
  }

  /**
   * Returns the primary key.
   *
   * @return array
   */
  public static function getPK() {
    // To be overridden by Child class.
    return array();
  }

  /**
   * Returns the foreign keys.
   *
   * @return array
   */
  public static function getFK() {
    // To be overridden by Child class.
    return array();
  }

  /**
   * Returns the unique keys.
   *
   * @return array
   */
  public static function getUK() {
    // To be overridden by Child class.
    return array();
  }

  /**
   * Adds a new record.
   *
   * return boolean
   */
  public function insert() {
    // To be overridden by Child class.
      return FALSE;
  }

  /**
   * Updates the record.
   *
   * return boolean
   */
  public function update() {
    // To be overridden by Child class.
      return FALSE;
  }

  /**
   * Deletes the record.
   *
   * return boolean
   */
  public function delete() {
    // To be overridden by Child class.
    return FALSE;
  }

  /**
   * Returns the value of class member.
   *
   * @param string $member
   *
   * @return string
   */
  public function getMemberValue($member) {
    return $this->{$member};
  }

  /**
   * Returns the regular expression of the separator.
   *
   * @param string $separator
   */
  public static function getSepRegex($separator) {
    return ($separator) ? "/$separator/" : '/@@@@@/';
  }

  /**
   * Checks the data type of the given attribute.
   *
   * @retrun MCL_TABLE_ATTR
   */
  public function isDataTypeNumeric($attr_name) {
    $data_type = $this->getDataType($attr_name);
    return in_array($data_type, self::$data_categories['NUMERIC']);
  }
  /**
   * Retrieves the data type of the member.
   *
   * @retrun various
   */
  public function getDataType($member) {
    print "Caaalled\n";
  }

  /**
   * Checks argments for empty.
   *
   * @param array $args
   *
   * @return boolean
   */
  public static function checkReqArgs($args) {
    foreach ($args as $key => $value) {
      if ($value == '') {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Creates a string from array.
   *
   * @param array $args
   *
   * @return string
   */
  public static function arrStr($args) {
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
   * Adds the message.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $key
   * @param string $table
   * @param array $args
   */
  public static function addMsg(MCL_TEMPLATE $mcl_tmpl = NULL, $key, $table, $args) {
    if ($mcl_tmpl) {
      $mcl_tmpl->addMsg($key, $table, $args);
    }
  }

  /**
   * Updates the message.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $key
   * @param string $msg
   */
  public static function updateMsg(MCL_TEMPLATE $mcl_tmpl = NULL, $key, $msg) {
    if ($mcl_tmpl) {
      $mcl_tmpl->updateMsg($key, $msg);
    }
  }

  /**
   * Updates the value of the provided attribute.
   *
   * @param string $attr_name
   * @param string $value
   *
   * @return boolean
   */
  public function updateAttr($attr_name, $value) {

    // Checks if the attribute exists first.
    if (!property_exists($this, $attr_name)) {
      return FALSE;
    }

    // Assigns the value and updates the value.
    $this->{$attr_name} = $value;

    // Updates the rank if exist.
    $rank = array();
    if (0) {
      $table_name = $this->getTableName();
      $sql = "
        SELECT MAX(rank) + 1 AS max_rank
        FROM {$table_name}
        WHERE 1=1
      ";
      $args = array();
      foreach ($rank as $attr) {
        $sql = " AND $attr = :$attr ";
        $args[":$attr"] = $this->{$attr};
      }
      $this->{rank} = db_query($sql, $args)->fetchField();




      print "$sql\n";print_r($args);  exit;
    }



    return $this->update();
  }

  /**
   * Gets the property.
   *
   * @param string $table_name
   * @param string $target_field
   * @param integer $target_id
   * @param integer $type_id
   *
   * @return string
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
   * Adds one or more properties
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $table_name
   * @param string $target_field
   * @param integer $target_id
   * @param integer $type_id
   * @param string $value
   * @param string $separator
   *
   * @return boolean
   */
  public function addProperty(MCL_TEMPLATE $mcl_tmpl = NULL, $table_name, $target_field, $target_id, $type_id, $value, $separator = '') {
    if ($value || $value == '0') {

      // Gets the class name for the table.
      $class_name = strtoupper('CHADO_' . $table_name);

      // Adds new properties.
      if ($separator) {
        $value_arr = preg_split($this->getSepRegex($separator), $value, -1, PREG_SPLIT_NO_EMPTY);

        // Checks duplication before adding new properties.
        // We add multiple valuse with different ranks. So check duplication
        // for type_id andvalue.
        $err_flag = FALSE;
        foreach ($value_arr as $val) {
          $val = trim($val);
          $args = array(
            $target_field => $target_id,
            'type_id'     => $type_id,
            'value'       => $val,
          );
          $obj = $class_name::byKey($args);
          if ($obj) {
            $this->addMsg($mcl_tmpl, 'D', $table_name, $args);
          }
          else {
            $args['rank'] = $this->getNextRank($table_name, $target_field, $target_id, $type_id);
            $obj = new $class_name($args);
            if ($obj->insert()) {
              $this->addMsg($mcl_tmpl, 'N', $table_name, $args);
            }
            else {
              $this->addMsg($mcl_tmpl, 'E', $table_name, $args);
              $err_flag = TRUE;
            }
          }
        }
        if (!$err_flag) {
          return TRUE;
        }
      }
      else {
        // Checks duplication before adding a new property.
        //   We assume that the property holds one value. So check duplication
        //   for only type_id.
        $args = array(
          $target_field => $target_id,
          'type_id'     => $type_id,
        );
        $obj = $class_name::byKey($args);
        if ($obj) {
          $this->addMsg($mcl_tmpl, 'D', $table_name, $args);
        }
        else {
          $args['value'] = $value;
          $obj = new $class_name($args);
          if ($obj->insert()) {
            $this->addMsg($mcl_tmpl, 'N', $table_name, $args);
            return TRUE;
          }
          else {
            $this->addMsg($mcl_tmpl, 'E', $table_name, $args);
          }
        }
      }
    }
    return FALSE;
  }

  /**
   * Removes a property with a rank.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $table_name
   * @param string $target_field
   * @param integer $target_id
   * @param integer $type_id
   * @param integer $rank
   *
   * @return boolean
   */
  public function removePropertyRank(MCL_TEMPLATE $mcl_tmpl = NULL, $table_name, $target_field, $target_id, $type_id, $rank) {

    // Gets the class name for the table.
    $class_name = strtoupper('CHADO_' . $table_name);

    // Gets the property object.
    $args = array(
      $target_field => $target_id,
      'type_id'     => $type_id,
      'rank'        => $rank,
    );
    $obj = $class_name::byKey($args);
    if ($obj) {
      return $obj->delete();
    }
    return FALSE;
  }

  /**
   * Removes all properties of the provided type.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $table_name
   * @param string $target_field
   * @param integer $target_id
   * @param integer $type_id
   *
   * @return boolean
   */
  public function removeProperty(MCL_TEMPLATE $mcl_tmpl = NULL, $table_name, $target_field, $target_id, $type_id) {

    // Deletes all properties for the provided type (cvterm).
    if (!preg_match("/^chado\./", $table_name)) {
      $table_name = 'chado.' . $table_name;
    }
    db_delete($table_name)
      ->condition($target_field, $target_id, '=')
      ->condition('$type_id', $type_id, '=')
      ->execute();
    return TRUE;
  }

  /**
   * Returns the next empty rank.
   *
   * @param string $table_name
   * @param string $target_field
   * @param integer $target_id
   * @param integer $type_id
   *
   * @return boolean
   */
  public function getNextRank($table_name, $target_field, $target_id, $type_id) {
    $sql = "
      SELECT COUNT(rank)
      FROM chado.$table_name
      WHERE type_id = :type_id AND $target_field = $target_id
    ";
    $count = db_query($sql, array(':type_id' => $type_id))->fetchField();
    if (!$count) {
      return 0;
    }
    $sql = "
      SELECT MAX(rank)
      FROM chado.$table_name
      WHERE type_id = :type_id AND $target_field = $target_id
    ";
    $rank = db_query($sql, array(':type_id' => $type_id))->fetchField();
    return $rank + 1;
  }

  /**
   * Adds one property with the specified rank.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $table_name
   * @param string $target_field
   * @param integer $target_id
   * @param integer $type_id
   * @param string $value
   * @param integer $rank
   *
   * @return boolean
   */
  public function addPropertyRanked(MCL_TEMPLATE $mcl_tmpl = NULL, $table_name, $target_field, $target_id, $type_id, $value, $rank) {
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
        $this->addMsg($mcl_tmpl, 'D', $table_name, $args);
      }
      else {

        // Adds a property.
        $obj = new $class_name($args);
        if ($obj->insert()) {
          $this->addMsg($mcl_tmpl, 'N', $table_name, $args);
          return TRUE;
        }
        else {
          $this->addMsg($mcl_tmpl, 'E', $table_name, $args);
        }
      }
    }
  }

  /**
   * Updates one or more properties
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $table_name
   * @param string $target_field
   * @param integer $target_id
   * @param integer $type_id
   * @param string $value
   * @param string $separator
   *
   * @return boolean
   */
  public function updateProperty(MCL_TEMPLATE $mcl_tmpl = NULL, $table_name, $target_field, $target_id, $type_id, $value) {
    if ($value || $value == '0') {

      // Gets the table class.
      $args = array(
        $target_field => $target_id,
        'type_id'     => $type_id,
      );
      $class_name = strtoupper('CHADO_' . $table_name);
      $obj = $class_name::byKey($args);

      // If the property exists, update it. Otherwise add a new one.
      if ($obj) {
        $obj->setValue($value);
        return $obj->update();
      }
      else {
        $args['value'] = $value;
        $obj = new $class_name($args);
        return $obj->insert();
      }
    }
    return FALSE;
  }

  /**
   * Adds a relationship.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $table_name
   * @param string $subject_field
   * @param integer $subject_id
   * @param string $object_field
   * @param string $object_id
   * @param integer $type_id
   *
   * @return boolean
   */
  public function addRelationship(MCL_TEMPLATE $mcl_tmpl = NULL, $table_name, $subject_field, $subject_id, $object_field, $object_id, $type_id) {
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
        $this->addMsg($mcl_tmpl, 'D', $table_name, $args);
      }
      else {

        // Adds a new relationship.
        $obj = new $class_name($args);
        if ($obj->insert()) {
          $this->addMsg($mcl_tmpl, 'N', $table_name, $args);
          return TRUE;
        }
        else {
          $this->addMsg($mcl_tmpl, 'E', $table_name, $args);
        }
      }
    }
    return FALSE;
  }

  /**
   * Adds a link (Adding to a linking table).
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $table_name
   * @param string $field_1
   * @param integer $id_1
   * @param string $field_2
   * @param string $id_2
   *
   * @return boolean
   */
  public function addLink(MCL_TEMPLATE $mcl_tmpl = NULL, $table_name, $field_1, $id_1, $field_2, $id_2, $type_id = NULL) {
    if ($id_1 != '' && $id_2 != '') {

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
        $this->addMsg($mcl_tmpl, 'D', $table_name, $args);
      }
      else {

        // Adds a new link.
        $obj = new $class_name($args);
        if ($obj->insert()) {
          $this->addMsg($mcl_tmpl, 'N', $table_name, $args);
          return TRUE;
        }
        else {
          $this->addMsg($mcl_tmpl, 'E', $table_name, $args);
        }
      }
    }
    return FALSE;
  }


  /**
   * Returns the string for the provide ID.
   *
   * @param array $prop
   *
   * @return string
   */
  public static function mapID($prop, $value) {
    $fk_table   = $prop['fk_table'];
    $fk_attr    = $prop['fk_attr'];
    $class_name = 'CHADO_' . strtoupper($fk_table);
    if (preg_match("/^CHADO_(PUB|STOCK|FEATURE|GENOTYPE)$/", $class_name)) {
      $obj = $class_name::byKey(array($fk_attr => $value));
      if ($obj) {
        return $obj->getUniquename();
      }
    }
    else if (preg_match("/^CHADO_(CVTERM|PROJECT|FEATUREMAP|SYNONYM)$/", $class_name)) {
      $obj = $class_name::byKey(array($fk_attr => $value));
      if ($obj) {
        return $obj->getName();
      }
    }
    return '';
  }

  /**
   * Returns the restore SQL for property of the table.
   *
   * @param object $ref_table_obj
   * @param string $chado_table
   * @param array $prev_keys
   * @param array $cur_keys
   *
   * @return string.
   */
  function getRestoreSQL($ref_table_obj, $chado_table, $cur_keys, $prev_keys) {

    // Gets 'SET' clause.
    $arr_set = array();
    foreach ($prev_keys as $key => $value) {
      if ($ref_table_obj->isDataTypeNumeric($key)) {
        $arr_set []= sprintf(" %s = %d ", $key, $value);
      }
      else {
        $escaped = pg_escape_string($value);
        $arr_set []= " $key = '$escaped' ";
      }
    }

    // Gets 'WHERE' clause.
    $arr_where = array();
    foreach ($cur_keys as $key => $value) {
      if ($ref_table_obj->isDataTypeNumeric($key)) {
        $arr_where []= sprintf(" %s = %d ", $key, $value);
      }
      else {
        $escaped = pg_escape_string($value);
        $arr_where []= " $key = '$escaped' ";
      }
    }

    // Creates the restore statement.
    $sql_set    = implode(',', $arr_set);
    $sql_where  = implode(' AND ', $arr_where);
    return "UPDATE $chado_table SET $sql_set WHERE $sql_where;";
  }

  /**
   * Returns the restore SQL for the data.
   *
   * @return string.
   */
  function getRestoreObj() {

    // Gets the table and its members.
    $table_name = $this->getTableName(TRUE);
    $members = $this->getMemberArr();

    // Creates the SQL to restore the data of this object.
    $field_arr = array();
    $value_arr = array();
    foreach ($members as $field => $value) {
      if ($value || $value == '0') {
        $field_arr []= $field;
        $value_arr []= pg_escape_string($value);
      }
    }
    $sql = "INSERT INTO {$table_name} (" . implode(',', $field_arr);
    $sql .= ") VALUES ('" . implode("','", $value_arr) . "');";
    return $sql;
  }

  /**
   * Returns the user-defined unique keys of the provided table.
   *
   * @param string $chado_table
   *
   * @return array
   */
  public function getMergeUniqueKeys($chado_table) {
    // To be overridden by Child class.
    return array();
  }

  /**
   * Processes the merge.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param array $params
   * @param array $ref_table
   * @param boolean $verbose
   *
   * @return boolean
   */
  protected function procMerge(MCL_TEMPLATE $mcl_tmpl = NULL, $params, $ref_table, $verbose = FALSE) {

    // Sets the local variables.
    $table_name = $ref_table['table'];
    $attr_name  = $ref_table['attr'];
    $label      = $params['label'];
    $id_base    = $params['id_base'];
    $id_del     = $params['id'];

    // Sets the chado table and class name.
    $class_name = 'CHADO_' . strtoupper($table_name);
    $chado_table = 'chado.' . $table_name;

    // Checks the table. If the table does not exist, log and return TRUE.
    if (!db_table_exists($chado_table)) {
      $msg = "The table '$chado_table' does not exist";
      $dummy = ($mcl_tmpl) ? $mcl_tmpl->updateMsg('L', $msg) : mcl_print($msg, 1, 3, '', FALSE);
      return TRUE;
    }

    // Gets the class name.
    if (!class_exists($class_name)) {
      $msg = "CHADO class [$class_name] does not exist.";
      $dummy = ($mcl_tmpl) ? $mcl_tmpl->updateMsg('E', $msg) : mcl_print("Error : $msg", 1, 3);
      return FALSE;
    }

    // Gets the class object.
    $ref_table_obj = $class_name::byKey(array($attr_name => $id_del));
    if ($ref_table_obj) {
      $msg = "$chado_table has data to be merged for [$attr_name : $id_del]";
      $dummy = ($mcl_tmpl) ? $mcl_tmpl->updateMsg('L', $msg) : mcl_print("LOG : $msg", 1, 3);
    }
    else {
      $msg = "$chado_table does not have data to be merged.";
      if ($mcl_tmpl) {
        $mcl_tmpl->updateMsg('L', $msg);
      }
      else if ($verbose) {
        mcl_print("LOG : $msg", 1, 3);
      }
      return TRUE;
    }

    // Gets the user-defined unique keys.
    $user_unique_keys = $this->getMergeUniqueKeys($chado_table);

    // Gets the unique keys from table definition.
    $tbl_unique_keys = array();
    $uk = $ref_table_obj->getUK();
    foreach ((array)$uk as $name => $attrs) {
      if (in_array($attr_name, $attrs)) {
        $tbl_unique_keys = $attrs;
      }
    }

    // Checks the table unique key.
    if (empty($tbl_unique_keys) && empty($user_unique_keys)) {
      $msg = "$chado_table does not have a unique key.";
      $dummy = ($mcl_tmpl) ? $mcl_tmpl->updateMsg('E', $msg) : mcl_print("LOG : $msg", 1, 3);
      return FALSE;
    }

    // Checks for duplication.
    $sql          = " SELECT * FROM {$chado_table} WHERE $attr_name = :$attr_name";
    $args         = array(":$attr_name" => $id_del);
    $results      = db_query($sql, $args);
    $num_merges   = 0;
    $num_changes  = 0;
    $test_log     = '';
    while ($arr = $results->fetchAssoc()) {
      $num_merges++;
      $keys_base    = array();
      $keys_del     = array();
      $keys_update  = array();

      // The user defined unique key will be used if found.
      if (!empty($user_unique_keys)) {
        foreach ($user_unique_keys as $ukey) {
          if (array_key_exists('field', $ukey)) {
            $f = $ukey['field'];
            $v = $ukey['value'];
            if ($arr[$f] == $v) {
              foreach ($ukey['ukeys'] as $key) {
                $keys_base["$key"]  = $arr[$key];
                $keys_del["$key"]   = $arr[$key];
              }
              $keys_update = $ukey['update'];
              break;
            }
          }
          else {
            foreach ($ukey['ukeys'] as $key) {
              $keys_base["$key"]  = $arr[$key];
              $keys_del["$key"]   = $arr[$key];
            }
          }
        }
      }

      // The table defined unique key will be used.
      if (empty($keys_base)) {
        $flag_ukeys = FALSE;
        foreach ($tbl_unique_keys as $key) {
          $keys_base["$key"]  = $arr[$key];
          $keys_del["$key"]   = $arr[$key];
        }
      }

      // Updates $keys and search for a duplication.
      $keys_base["$attr_name"] = $id_base;
      $obj_base = $class_name::byKey($keys_base);

      // The base object does not have the data. So add it by updating
      // the deleting object data.
      if (!$obj_base) {

        // Updates the attribute. Since it is used user-defined unique keys,
        // it needs to add more information before updating the value.
        if (!empty($keys_update)) {

          // Updates the keys.
          $updated_attrs = $this->_updateMergeKeys($keys_update, $chado_table, $keys_base);
          $updated_attrs["$attr_name"] = $id_base;

          // Updates the data.
          $db_query = db_update($chado_table)->fields($updated_attrs);
          foreach ($keys_del as $k => $v) {
            $db_query->condition($k, $v, '=');
          }
          $db_query->execute();

          // Logs the changes.
          $this->_logMerge($mcl_tmpl, $ref_table_obj, $chado_table, $keys_base, $keys_del, $test_log);
        }

        // Updates the attribute.
        else {

          // Gets the deleting object.
          $obj_del = $class_name::byKey($keys_del);

          // Updates the value of the deleting object.
          if ($obj_del->updateAttr($attr_name, $id_base)) {
            $num_changes++;

            // Logs the changes.
            $this->_logMerge($mcl_tmpl, $ref_table_obj, $chado_table, $keys_base, $keys_del, $test_log);
          }
          else {
            $msg = "Failed to update data ($label [$attr_name:$id_del])";
            $mcl_tmpl->updateMsg('E', $msg);
            return FALSE;
          }
        }
      }
      else {
        $msg = "  Skipped : $chado_table.$attr_name [$id_del => $id_base] : $data_str exists.";
        if ($mcl_tmpl) {
          $mcl_tmpl->updateMsg('D', $msg);

          // Keeps the data for recovery.

        }
        else {
          $test_log .= "$msg\n\n";
        }
      }
    }
    $msg = "$test_log\nMerge : num_changes / num_merges = ($num_changes/$num_merges)";
    $dummy = ($mcl_tmpl) ? $mcl_tmpl->updateMsg('L', $msg) : mcl_print($msg, 1, 3, '', FALSE);
    return TRUE;
  }

  /**
   * Updates the keys.
   *
   * @param array $keys_update
   * @param string $chado_table
   * @param array $keys
   *
   * @return array
   */
  private function _updateMergeKeys($keys_update, $chado_table, $keys) {

    $updated_attrs = array();
    foreach ((array)$keys_update as $attr => $info) {
      $type   = $info['type'];
      $ukeys  = $info['ukeys'];
      if ($type == 'max') {
        $where_arr = array();
        foreach ($ukeys as $field) {
          $where_arr []= sprintf(" %s = '%s' ", $field, $keys[$field]);
        }
        $where_str = implode(' AND ', $where_arr);
        $sql = "SELECT MAX($attr) FROM $chado_table WHERE $where_str";
        $updated_attrs[$attr] = db_query($sql)->fetchField() + 1;
      }
    }
    return $updated_attrs;
  }

  /**
   * Logs the merge messages.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param object $ref_table_obj
   * @param string $chado_table
   * @param array $prev_keys
   * @param array $cur_keys
   * @param string $log
   *
   * @return string
   */
  private function _logMerge(MCL_TEMPLATE $mcl_tmpl = NULL, $ref_table_obj, $chado_table, $keys_base, $keys_del, &$log) {

    // Gets the messages for update and restore.
    $sql    = $this->getRestoreSQL($ref_table_obj, $chado_table, $keys_base, $keys_del);
    $msg_u  = "  Updated : $chado_table.$attr_name\n";
    $msg_r  = "  Restore : $sql\n";
    $mcl_tmpl->updateMsg('M', $msg_u);
    $mcl_tmpl->updateMsg('R', $msg_u . $msg_r);

    // Writes the message to the log.
    if ($mcl_tmpl) {
      $mcl_tmpl->updateMsg('M', $msg_u);
      $mcl_tmpl->updateMsg('R', $msg_u . $msg_r);
    }
    else {
      $test_log .= "$msg_u\n$msg_r\n";
    }
  }

  /**
   * Processes the merge.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param array $params
   * @param array $ref_table
   * @param boolean $verbose
   *
   * @return boolean
   */
  protected function procMerge_tmp(MCL_TEMPLATE $mcl_tmpl = NULL, $params, $ref_table, $verbose = FALSE) {

    // Sets the local variables.
    $table_name = $ref_table['table'];
    $attr_name  = $ref_table['attr'];
    $label      = $params['label'];
    $id_base    = $params['id_base'];
    $id_del     = $params['id'];

    // Sets the chado table and class name.
    $class_name = 'CHADO_' . strtoupper($table_name);
    $chado_table = 'chado.' . $table_name;

    // Checks the table. If the table does not exist, log and return TRUE.
    if (!db_table_exists($chado_table)) {
      $msg = "The table '$chado_table' does not exist";
      $dummy = ($mcl_tmpl) ? $mcl_tmpl->updateMsg('L', $msg) : mcl_print($msg, 1, 3, '', FALSE);
      return TRUE;
    }

    // Gets the class name.
    if (!class_exists($class_name)) {
      $msg = "CHADO class [$class_name] does not exist.";
      $dummy = ($mcl_tmpl) ? $mcl_tmpl->updateMsg('E', $msg) : mcl_print("Error : $msg", 1, 3);
      return FALSE;
    }

    // Gets the class object.
    $ref_table_obj = $class_name::byKey(array($attr_name => $id_del));
    if ($ref_table_obj) {
      $msg = "$chado_table has data to be merged for [$attr_name : $id_del]";
      $dummy = ($mcl_tmpl) ? $mcl_tmpl->updateMsg('L', $msg) : mcl_print("LOG : $msg", 1, 3);
    }
    else {
      $msg = "$chado_table does not have data to be merged.";
      if ($mcl_tmpl) {
        $mcl_tmpl->updateMsg('L', $msg);
      }
      else if ($verbose) {
        mcl_print("LOG : $msg", 1, 3);
      }
      return TRUE;
    }

    // Gets the user-defined unique keys.
    $user_unique_keys = $this->getMergeUniqueKeys($chado_table);

    // Gets the unique keys from table definition.
    $tbl_unique_keys = array();
    $uk = $ref_table_obj->getUK();
    foreach ((array)$uk as $name => $attrs) {
      if (in_array($attr_name, $attrs)) {
        $tbl_unique_keys = $attrs;
      }
    }

    // Checks the table unique key.
    if (empty($tbl_unique_keys) && empty($user_unique_keys)) {
      $msg = "$chado_table does not have a unique key.";
      $dummy = ($mcl_tmpl) ? $mcl_tmpl->updateMsg('L', $msg) : mcl_print("LOG : $msg", 1, 3);
      return FALSE;
    }

    // Checks for duplication.
    $sql          = " SELECT * FROM {$chado_table} WHERE $attr_name = :$attr_name";
    $args         = array(":$attr_name" => $id_del);
    $results      = db_query($sql, $args);
    $keys_base    = array();
    $keys_del     = array();
    $num_merges   = 0;
    $num_changes  = 0;
    $test_log     = '';
    while ($arr = $results->fetchAssoc()) {
      $num_merges++;
      $keys_base  = array();
      $keys_del   = array();

      // The user defined unique key will be used if found.
      if (!empty($user_unique_keys)) {
        foreach ($user_unique_keys as $ukey) {
          if (array_key_exists('field', $ukey)) {
            $f = $ukey['field'];
            $v = $ukey['value'];
            if ($arr[$f] == $v) {
              foreach ($ukey['ukeys'] as $key) {
                $keys_base["$key"]  = $arr[$key];
                $keys_del["$key"]   = $arr[$key];
              }
              break;
            }
          }
          else {
            foreach ($ukey['ukeys'] as $key) {
              $keys_base["$key"]  = $arr[$key];
              $keys_del["$key"]   = $arr[$key];
            }
          }
        }
      }

      // The table defined unique key will be used.
      if (empty($keys_base)) {
        foreach ($tbl_unique_keys as $key) {
          $keys_base["$key"]  = $arr[$key];
          $keys_del["$key"]   = $arr[$key];
        }
      }

      // Updates $keys and search for a duplication.
      $keys_del["$attr_name"] = $id_base;
      $obj_del = $class_name::byKey($keys_del);
      if (!$obj_del) {

        // Adds the restore SQL.
        $sql    = $this->getRestoreSQL($ref_table_obj, $chado_table, $keys_del, $keys_base);
        $msg_u  = "  Updated : $chado_table.$attr_name [$id_del => $id_base]\n";
        $msg_r  = "  Restore : $sql\n";

        // Updates the data.
        if ($mcl_tmpl) {

          // Gets the base object.
          $obj_base = $class_name::byKey($keys_base);

          // Updates the value fo the attribute.
          if ($obj_base->updateAttr($attr_name, $id_base)) {
            $num_changes++;
            $mcl_tmpl->updateMsg('M', $msg_u);
            $mcl_tmpl->updateMsg('R', $msg_u . $msg_r);
          }
          else {
            $msg = "Failed to update data ($label [$attr_name:$id_del])";
            $mcl_tmpl->updateMsg('E', $msg);
            return FALSE;
          }
        }
        else {
          $test_log .= "$msg_u\n$msg_r\n";
        }
      }
      else {
        $msg = "  Skipped : $chado_table.$attr_name [$id_del => $id_base] : $data_str exists.";
        if ($mcl_tmpl) {
          $mcl_tmpl->updateMsg('D', $msg);

          // Keeps the data for recovery.

        }
        else {
          $test_log .= "$msg\n\n";
        }
      }
    }
    $msg = "$test_log\nMerge : num_changes / num_merges = ($num_changes/$num_merges)";
    $dummy = ($mcl_tmpl) ? $mcl_tmpl->updateMsg('L', $msg) : mcl_print($msg, 1, 3, '', FALSE);
    return TRUE;
  }
}
