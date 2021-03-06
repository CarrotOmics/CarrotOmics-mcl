<?php
/**
 * The declaration of MCL_CHADO_GENOTYPE class.
 *
 */
class MCL_CHADO_GENOTYPE extends CHADO_GENOTYPE {

 /**
  *  Class data members.
  */

  /**
   * @see CHADO_GENOTYPE::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_GENOTYPE::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * @see CHADO_GENOTYPE::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * Checks the alleles.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param array $data_line
   * @param string $regex_cols
   * @param string $regex_sep
   *
   * @return boolean
   */
  public static function checkAlleles(MCL_TEMPLATE $mcl_tmpl = NULL, $data_line, $regex_cols,  $regex_sep = '/\|/') {
    $flag = TRUE;

    // Checks allele columns.
    foreach ($data_line as $column => $info) {
      if (preg_match($regex_cols, $column)) {
        $allele_str = $data_line[$column];
        if (!preg_match("/\|/", $allele_str)) {
          $this->updateMsg('E', "Invalide allele '$allele_str at " . $data_line['line_no']);
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Adds a genotype.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $uniquename
   * @param integer $type_id
   * @param string $description
   *
   * @return MCL_CHADO_GENOTYPE
   */
  public static function addGenotype(MCL_TEMPLATE $mcl_tmpl = NULL, $uniquename, $type_id, $description) {

    // Sets the arguments.
    $args = array(
      'uniquename' => $uniquename,
      'type_id'    => $type_id,
    );

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $genotype = MCL_CHADO_GENOTYPE::byKey($args);
    if ($genotype) {
      self::addMsg($mcl_tmpl, 'D', 'genotype', $args);
    }
    else {

      // Adds a new genotype.
      $args['description'] = $description;
      $genotype = new MCL_CHADO_GENOTYPE($args);
      if ($genotype->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'genotype', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'genotype', $args);
        return NULL;
      }
    }
    return $genotype;
  }

  /**
   * Adds a SNP genotype.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $type_id
   * @param string $description
   *
   * @return MCL_CHADO_GENOTYPE
   */
  public static function addGenotypeSNP(MCL_TEMPLATE $mcl_tmpl = NULL, $type_id, $description) {

    // Sets the uniquename and updates the description if no bar provided.
    $uniquename = $description;
//    if (preg_match("/^([a-z\-])(a-z\-)$/i", $description, $matches)) {
//      $uniquename = $description = $matches[1] . '|' . $matches[2];
//    }

    // Sets the arguments.
    $args = array(
      'uniquename' => $uniquename,
      'type_id'    => $type_id,
    );

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $genotype = MCL_CHADO_GENOTYPE::byKey($args);
    if ($genotype) {
      self::addMsg($mcl_tmpl, 'D', 'genotype', $args);
    }
    else {

      // Adds a new genotype.
      $args['description'] = $description;
      $genotype = new MCL_CHADO_GENOTYPE($args);
      if ($genotype->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'genotype', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'genotype', $args);
        return NULL;
      }
    }
    return $genotype;
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
      return $this->addProperty($mcl_tmpl, 'genotypeprop', 'genotype_id', $this->genotype_id, $type_id, $value, $separator);
    }
    return TRUE;
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
      $type_id = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name)->getCvtermID();
      return $this->addProperty($mcl_tmpl, 'genotypeprop', 'genotype_id', $this->genotype_id, $type_id, $value, $separator);
    }
    return TRUE;
  }
}