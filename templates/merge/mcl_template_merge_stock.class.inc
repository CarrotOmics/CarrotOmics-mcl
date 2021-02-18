<?php
/**
 * The declaration of MCL_TEMPLATE_MERGE_STOCK class.
 *
 */
class MCL_TEMPLATE_MERGE_STOCK extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_UPLOAD::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'MERGE';
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
    $desc = 'The description for merge stock sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'stock_name_base' => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the base stock."),
      'genus_base'      => array('req' => TRUE,  'width' => 10, 'desc' => "Genus of the base stock"),
      'species_base'    => array('req' => TRUE,  'width' => 10, 'desc' => "Species name of the base stock."),
      'stock_type_base' => array('req' => TRUE,  'width' => 10, 'desc' => "Type of the base stock."),
      'stock_name'      => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the stock to be removed."),
      'genus'           => array('req' => TRUE,  'width' => 10, 'desc' => "Genus of the stockto be removed."),
      'species'         => array('req' => TRUE,  'width' => 10, 'desc' => "Species name of the stock to be removed."),
      'stock_type'      => array('req' => TRUE,  'width' => 10, 'desc' => "Type of the stock to be removed."),
      'delete'          => array('req' => FALSE, 'width' => 10, 'desc' => "'yes' or 'no' - 'yes' indicates that the merged stock will be removed."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks the organism of the base stock.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus_base'], $line['species_base']);

    // Checks the base stock..
    MCL_CHADO_STOCK::checkStock($this, $line['stock_name_base'], $line['genus_base'], $line['species_base'], $line['stock_type_base']);

    // Checks the organism.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);

    // Checks the stock.
    MCL_CHADO_STOCK::checkStock($this, $line['stock_name'], $line['genus'], $line['species'], $line['stock_type']);

    // Checks delete.
    $this->checkDeleteFlag($line['delete']);

    // Keeps stock_id for merge check.
    $stock_base = MCL_CHADO_STOCK::getStock($line['stock_name_base'], $line['genus_base'], $line['species_base'],  $line['stock_type_base']);
    $stock      = MCL_CHADO_STOCK::getStock($line['stock_name'], $line['genus'], $line['species'],  $line['stock_type']);
    if ($stock_base && $stock) {
      $this->mergeCheck($stock_base->getStockID(), $stock->getStockID());
    }
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the base stock.
    $org = MCL_CHADO_ORGANISM::getOrganism($line['genus_base'], $line['species_base']);
    $keys = array(
      'uniquename'  => $line['stock_name_base'],
      'organism_id' => $org->getOrganismID(),
      'type_id'     => MCL_CHADO_CVTERM::getCvterm('SITE_CV', $line['stock_type_base'])->getCvtermID(),
    );
    $stock_base = MCL_CHADO_STOCK::byKey($keys);

    // Getst the stock.
    $org = MCL_CHADO_ORGANISM::getOrganism($line['genus'], $line['species']);
    $keys = array(
      'uniquename'  => $line['stock_name'],
      'organism_id' => $org->getOrganismID(),
      'type_id'     => MCL_CHADO_CVTERM::getCvterm('SITE_CV', $line['stock_type'])->getCvtermID(),
    );
    $stock = MCL_CHADO_STOCK::byKey($keys);

    // Merges the stocks if not the same.
    if ($stock->getStockID() != $stock_base->getStockID()) {
      $stock_base->mergeStock($this, $stock);
    }
    else {
      $this->updateMsg('W', "Both are the same stocks");
    }

    // Deletes the merged stock.
    if (strtolower($line['delete']) == 'yes') {
      $GLOBALS['delete_stocks'][$stock->getStockID()] = $stock;
    }
  }

  /**
   * @see MCL_TEMPLATE::preProcessDU()
   */
  public function preProcessDU() {
    $GLOBALS['delete_stocks'] = array();
    return TRUE;
  }

  /**
   * @see MCL_TEMPLATE::postProcessDU()
   */
  public function postProcessDU() {

    // Deletes the stocks.
    foreach ((array)$GLOBALS['delete_stocks'] as $stock_id => $stock) {
      $this->updateMsg('R', $stock->getRestoreObj());
      if (!$stock->delete()) {
        $this->updateMsg('E', "Failed to delete stock [ID = $stock_id]");
      }
    }
    return TRUE;
  }
}