<?php
/**
 * The declaration of MCL_TEMPLATE_BREEDING_VALUE class.
 *
 */
class MCL_TEMPLATE_BREEDING_VALUE extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'PHENOTYPE';
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
    $desc = 'The description for stockcollection sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'dataset_name'      => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the phenotyping dataset. It should match a 'dataset_name' column entry in the 'Dataset' sheet."),
      'stock_name'        => array('req' => TRUE,  'width' => 10, 'desc' => "ID of the stock that has been phenotyped. It should match a 'stock_name' column entry in the 'Stock' sheet or 'progeny_name' column entry 'Progeny' sheet."),
      'genus'             => array('req' => FALSE, 'width' => 10, 'desc' => "Genus of the stock"),
      'species'           => array('req' => FALSE, 'width' => 10, 'desc' => "Species of the stock"),
      '#descriptor_name'  => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns (#) : followed by descriptor name."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['statistical'] = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks dataset.
    MCL_CHADO_DATASET::checkDataset($this, $line['dataset_name']);

    // Checks organisms.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);

    // Checks stock.
    MCL_CHADO_STOCK::checkStock($this, $line['stock_name'], $line['genus'], $line['species']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the stock.
    $stock = MCL_CHADO_STOCK::getStock($line['stock_name'], $line['genus'], $line['species']);
    if ($stock) {

      // Adds phenotyping data for each descriptor.
      $descriptors = $this->getJob()->getParamByKey('#');
      foreach ($descriptors as $cvterm_id => $descriptor) {

        // Adds a nd_experiment.
        $nd_experiment = MCL_CHADO_ND_EXPERIMENT::addNdExperiment($this, $this->cvterms['SITE_CV']['statistical'], 0, $cvterm_id, $stock->getStockID());
        if ($nd_experiment) {

          // Gets the value and add it to phenotype table.
          $name       = strtolower($descriptor['name']);
          $value      = $line['#' . $name];
          $uniquename = $cvterm_id . '_' . $value;
          $phenotype  = MCL_CHADO_PHENOTYPE::addPhenotype($this, $uniquename, $cvterm_id, $value);

          // Adds a stock.
          $nd_experiment->addStock($this, $stock, $stock->getTypeID());

          // Adds a dataset.
          $nd_experiment->addDataset($this, $line['dataset_name']);

          // Adds a phenotype.
          $nd_experiment->addPhenotype($this, $phenotype);
        }
      }
    }
  }

  /**
   * @see MCL_TEMPLATE::postProcessECD()
   */
  public function postProcessECD() {

    // Gets the cv.
    $cv = MCL_CHADO_CV::getCV($this->getJob()->getParamByKey('cv'));
    if (!$cv) {
      $this->updateMsg('E', "cv does not exist.");
      return FALSE;
    }

    // Saves the descriptors.
    if (!$this->saveSpecialColumns('#', $cv->getCvID(), $this->getHeaders())) {
      $this->updateMsg('E', "Failed to save the special columns");
      return FALSE;
    }
    return TRUE;
  }
}