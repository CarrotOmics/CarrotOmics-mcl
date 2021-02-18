<?php
/**
 * The declaration of MCL_CHADO_PHENSTATEMENT class.
 *
 */
class MCL_CHADO_PHENSTATEMENT extends CHADO_PHENSTATEMENT {

 /**
  *  Class data members.
  */
  /**
   * @see CHADO_PHENSTATEMENT::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_PHENSTATEMENT::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * Adds a phenstatement.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $genotype_id
   * @param integer $phenotype_id
   * @param integer $type_id
   * @param integer $environment_id
   * @param integer $pub_id
   *
   * @return MCL_CHADO_PHENSTATEMENT
   */
  public static function addPhenstatement(MCL_TEMPLATE $mcl_tmpl = NULL, $genotype_id, $phenotype_id, $type_id, $environment_id = '', $pub_id = '') {

    // Updates pub_id and environment_id.
    if ($environment_id == '') {
      $environment_id = MCL_DUMMY_VARS::getIdByName('ENVIRONMENT_ID');
    }
    if ($pub_id == '') {
      $pub_id = MCL_DUMMY_VARS::getIdByName('PUB_ID');
    }

    // Sets the arguments.
    $args = array(
      'genotype_id'     => $genotype_id,
      'phenotype_id'    => $phenotype_id,
      'type_id'         => $type_id,
      'environment_id'  => $environment_id,
      'pub_id'          => $pub_id,
    );

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $phenstatement = MCL_CHADO_PHENSTATEMENT::byKey($args);
    if ($phenotype) {
      self::addMsg($mcl_tmpl, 'D', 'phenstatement', $args);
    }
    else {

      // Adds a new phenstatement.
      $phenstatement = new MCL_CHADO_PHENSTATEMENT($args);
      if ($phenstatement->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'phenstatement', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'phenstatement', $args);
        return NULL;
      }
    }
    return $phenstatement;
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
      return $this->addProperty($mcl_tmpl, 'phenstatementprop', 'phenstatement_id', $this->phenstatement_id, $type_id, $value, $separator);
    }
    return TRUE;
  }
}