<?php
/**
 * The declaration of MCL_TEMPLATE_DELETE_STOCK class.
 *
 */
class MCL_TEMPLATE_DELETE_STOCK extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_UPLOAD::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'DELETE';
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
    $desc = 'The description for delete stock sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'stock_id'    => array('req' => FALSE,  'width' => 10, 'desc' => "ID of the stock to be deleted. If stock ID is provied, the rest of columns will be ignored"),
      'stock_name'  => array('req' => FALSE,  'width' => 10, 'desc' => "name of the stock to be deleted."),
      'type'        => array('req' => FALSE,  'width' => 10, 'desc' => "type of the stock to be deleted."),
      'genus'       => array('req' => FALSE,  'width' => 10, 'desc' => "genus of the stock to be deleted."),
      'species'     => array('req' => FALSE,  'width' => 10, 'desc' => "species of the stock to be deleted."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks stock.
    if (array_key_exists('stock_id', $line)) {
      MCL_CHADO_STOCK::checkStockID($this, $line['stock_id'], TRUE);
    }
    else if (array_key_exists('stock_name', $line)) {
      MCL_CHADO_STOCK::checkStock($this, $line['stock_name'], $line['genus'], $line['species'], $line['type'], '', TRUE);
    }
    else {
      $this->updateMsg('E', "The column 'stock_id' or 'stock_name' is missing.");
    }
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the stock.
    $stock = NULL;
    if (array_key_exists('stock_id', $line)) {
      $stock = MCL_CHADO_STOCK::byID($line['stock_id']);
    }
    else {
      $stock = MCL_CHADO_STOCK::getStock($line['stock_name'], $line['genus'], $line['species'], $line['type']);
    }

    // Deletes the stock.
    if ($stock) {
      $stock_name = $stock->getUniquename();
      if ($stock->delete()) {
        $this->updateMsg('L', "The stock has been deleted ($stock_name)");
      }
      else {
        $this->updateMsg('E', "Failed to delete the stock ($stock_name)");
      }
    }
  }
}