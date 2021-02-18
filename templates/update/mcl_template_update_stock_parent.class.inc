<?php
/**
 * The declaration of MCL_TEMPLATE_UPDATE_STOCK_PARENT class.
 *
 */
class MCL_TEMPLATE_UPDATE_STOCK_PARENT extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'UPDATE';
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
    $desc = 'The description for updating stock parent sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'stock_name'        => array('req' => TRUE,  'width' => 10, 'desc' => "Primary ID or name of stock. The most important ID or name (unique and stable at least within the breeding program) should be in this column. For mapping population, follow the naming convention of each database."),
      'genus'             => array('req' => FALSE, 'width' => 10, 'desc' => "Genus to which the stock belongs to."),
      'species'           => array('req' => FALSE, 'width' => 10, 'desc' => "Species name. Enter 'sp.' to represent one unknown species, 'spp.' to represent multiple unknown species."),
      'maternal_parent'   => array('req' => FALSE, 'width' => 10, 'desc' => "Maternal parent ID."),
      'maternal_genus'    => array('req' => FALSE, 'width' => 10, 'desc' => "Genus to which the maternal parent belongs to."),
      'maternal_species'  => array('req' => FALSE, 'width' => 10, 'desc' => "Species to which the maternal parent belongs to."),
      'paternal_parent'   => array('req' => FALSE, 'width' => 10, 'desc' => "Paternal parent ID."),
      'paternal_genus'    => array('req' => FALSE, 'width' => 10, 'desc' => "Genus to which the paternal parent belongs to."),
      'paternal_species'  => array('req' => FALSE, 'width' => 10, 'desc' => "Species to which the paternal parent belongs to."),
      'mutation_parent'   => array('req' => FALSE, 'width' => 10, 'desc' => "Mutation parent ID."),
      'mutation_genus'    => array('req' => FALSE, 'width' => 10, 'desc' => "Genus to which the mutation parent belongs to."),
      'mutation_species'  => array('req' => FALSE, 'width' => 10, 'desc' => "Species to which the mutation parent belongs to."),
      'selfing_parent'    => array('req' => FALSE, 'width' => 10, 'desc' => "Selfing parent ID."),
      'selfing_genus'     => array('req' => FALSE, 'width' => 10, 'desc' => "Genus to which the selfing parent belongs to."),
      'selfing_species'   => array('req' => FALSE, 'width' => 10, 'desc' => "Species to which the selfing parent belongs to."),
      'pedigree'          => array('req' => FALSE, 'width' => 10, 'desc' => "Any additional pedigree information: When the exact parents are not known, or any additional information beyond just parents (eg. Red-fruit sport of Jonathan, Pyrus sp. X Cydonia sp., etc)."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['is_a_maternal_parent_of']    = -1;
    $cvterms['SITE_CV']['is_a_paternal_parent_of']    = -1;
    $cvterms['SITE_CV']['is_a_mutation_parent_of']    = -1;
    $cvterms['SITE_CV']['is_a_selfing_parent_of']     = -1;
    $cvterms['SITE_CV']['maternal_parent']            = -1;
    $cvterms['SITE_CV']['paternal_parent']            = -1;
    $cvterms['SITE_CV']['mutation_parent']            = -1;
    $cvterms['SITE_CV']['selfing_parent']             = -1;
    $cvterms['SITE_CV']['pedigree']                   = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks organisms.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['maternal_genus'], $line['maternal_species']);
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['paternal_genus'], $line['paternal_species']);
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['mutation_genus'], $line['mutation_species']);
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['paternal_genus'], $line['selfing_species']);

    // Checks stocks.
    MCL_CHADO_STOCK::checkStock($this, $line['stock_name'], $line['genus'], $line['species']);
    MCL_CHADO_STOCK::checkStock($this, $line['maternal_parent'], $line['maternal_genus'], $line['maternal_species']);
    MCL_CHADO_STOCK::checkStock($this, $line['paternal_parent'], $line['paternal_genus'], $line['paternal_species']);
    MCL_CHADO_STOCK::checkStock($this, $line['mutation_parent'], $line['mutation_genus'], $line['mutation_species']);
    MCL_CHADO_STOCK::checkStock($this, $line['paternal_parent'], $line['selfing_genus'], $line['selfing_species']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the stock.
    $stock = MCL_CHADO_STOCK::getStock($line['stock_name'], $line['genus'], $line['species']);
    if ($stock) {

      // Updates maternal and paternal parents.
      $stock->updateParent($this, $this->cvterms['SITE_CV']['is_a_maternal_parent_of'], $line['maternal_parent'], $line['maternal_genus'], $line['maternal_species'], TRUE);
      $stock->updateParent($this, $this->cvterms['SITE_CV']['is_a_paternal_parent_of'], $line['paternal_parent'], $line['paternal_genus'], $line['paternal_species'], TRUE);
      $stock->updateParent($this, $this->cvterms['SITE_CV']['is_a_mutation_parent_of'], $line['mutation_parent'], $line['mutation_genus'], $line['mutation_species'], TRUE);
      $stock->updateParent($this, $this->cvterms['SITE_CV']['is_a_selfing_parent_of'], $line['selfing_parent'], $line['selfing_genus'], $line['selfing_species'], TRUE);

      // Updates the pedigree.
      $stock->updateProp($this, 'SITE_CV', 'pedigree', $line['pedigree'], TRUE);
    }
  }
}