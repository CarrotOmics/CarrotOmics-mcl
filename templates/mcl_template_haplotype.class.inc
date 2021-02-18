<?php
/**
 * The declaration of MCL_TEMPLATE_HAPLOTYPE class.
 *
 */
class MCL_TEMPLATE_HAPLOTYPE extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'HAPLOTYPE';
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
      'dataset_name'    => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the phenotyping dataset. It should match a 'dataset_name' column entry in the 'Dataset' sheet."),
      'stock_name'      => array('req' => TRUE,  'width' => 10, 'desc' => "ID of the stock that has been phenotyped. It should match a 'stock_name' column entry in the 'Stock' sheet or 'progeny_name' column entry 'Progeny' sheet."),
      'genus'           => array('req' => FALSE, 'width' => 10, 'desc' => "Genus of the stock."),
      'species'         => array('req' => FALSE, 'width' => 10, 'desc' => "Species of the stock."),
      'haplotype_block' => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the haplotype block. The organism of haplotype_block should be the same as the stock"),
      '#hap'            => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns (#) : followed by hapN where N is an integer."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['genotype']         = -1;
    $cvterms['SITE_CV']['genotyping']       = -1;
    $cvterms['sequence']['haplotype_block'] = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks dataset.
    MCL_CHADO_DATASET::checkDataset($this, $line['dataset_name']);

    // Checks organisms for stock.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);

    // Checks stock.
    MCL_CHADO_STOCK::checkStock($this, $line['stock_name'], $line['genus'], $line['species']);

    // Checks haplotype block.
    MCL_CHADO_FEATURE::checkFeature($this, $line['haplotype_block'], $line['genus'], $line['species'], $this->cvterms['sequence']['haplotype_block']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the dataset.
    $dataset = MCL_CHADO_DATASET::byName($line['dataset_name']);

    // Gets the stock.
    $stock = MCL_CHADO_STOCK::getStock($line['stock_name'], $line['genus'], $line['species']);

    // Gets the haplotype block.
    $hb = MCL_CHADO_FEATURE::getFeature($line['haplotype_block'], $line['genus'], $line['species'], $this->cvterms['sequence']['haplotype_block']);

    // Gets the default nd_geolocation.
    $nd_geolocation_id = MCL_CHADO_LOCATION::getLocationIDBySite('');

    // Adds a haplotype.
    if ($dataset && $stock && $hb) {

      // Gets the gentoype and add it to genotype table.
      $genotype_columns = $this->getJob()->getParamByKey('genotype_columns');
      $genotype_arr = array();
      foreach ((array)$genotype_columns as $idx => $column) {
        if ($line[$column]) {
          $genotype_arr []= $line[$column];
        }
      }

      // Adds a genotype.
      if (!empty($genotype_arr)) {
        $description  = implode('|', $genotype_arr);
        $uniquename   = $hb->getUniquename() . '_' . $description;
        $genotype     = MCL_CHADO_GENOTYPE::addGenotype($this, $uniquename, $this->cvterms['SITE_CV']['genotype'], $description);
        if ($genotype) {

          // Adds the nd_experiment.
          $dup_key = $dataset->getProjectID() . '_' . $stock->getStockID() . '_' . $hb->getFeatureID();
          $nd_experiment = MCL_CHADO_ND_EXPERIMENT::addNdExperiment($this, $this->cvterms['SITE_CV']['genotyping'], $nd_geolocation_id, $this->cvterms['SITE_CV']['genotype'], $dup_key);
          if ($nd_experiment) {

            // Adds the relationship between the nd_experiment and the stock.
            $nd_experiment->addStock($this, $stock, $stock->getTypeID());

            // Adds the relationship between the nd_experiment and the genotype.
            $nd_experiment->addGenotype($this, $genotype);

            // Adds the relationship between the nd_experiment and the project
            $nd_experiment->addDataset($this, $line['dataset_name']);

            // Adds the relationship between the haplotype block and the genotype.
            $hb->addGenotype($this, $genotype, $hb->getTypeID());
          }
        }
      }
    }
  }

  /**
   * @see MCL_TEMPLATE::postProcessECD()
   */
  public function postProcessECD() {

    // Saves the genotyping columns.
    $genotype_columns = array();
    foreach ($this->getHeaders() as $column => $value) {
      if (preg_match("/^#hap(\d+)$/i", $column, $matches)) {
        $idx = intval($matches[1]);
        $genotype_columns[$idx]= $column;
      }
    }
    ksort($genotype_columns, SORT_NUMERIC);
    $this->getJob()->setParamByKey('genotype_columns', $genotype_columns);
    return $this->getJob()->update();
  }
}