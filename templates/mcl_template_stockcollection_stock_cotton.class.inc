<?php
/**
 * The declaration of MCL_TEMPLATE_STOCKCOLLECTION_STOCK_COTTON class.
 *
 */
class MCL_TEMPLATE_STOCKCOLLECTION_STOCK_COTTON extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'STOCK_POST';
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
      'code'                      => array('req' => TRUE,  'width' => 10, 'desc' => "Code of the stockcollection."),
      'secondary_id'              => array('req' => TRUE,  'width' => 10, 'desc' => "The second ID of the stock."),
      'stock_name_in_collection'  => array('req' => FALSE, 'width' => 10, 'desc' => "Name of the stock in a collection."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['stock_center'] = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks stockcollection.
    MCL_CHADO_STOCKCOLLECTION::checkStockcollection($this, $line['code'], $this->cvterms['SITE_CV']['stock_center']);

    // Checks Secondary ID.
    MCL_CHADO_STOCK::checkSecondaryID($this, $line['secondary_id'], '', '');
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the stockcollection.
    $stockcollection = MCL_CHADO_STOCKCOLLECTION::getStockcollection($line['code'], $this->cvterms['SITE_CV']['stock_center']);
    if ($stockcollection) {

      // Gets the stock.
      $stock = MCL_CHADO_STOCK::byKey(array('name' => $line['secondary_id']));
      if ($stock) {
        if ($stockcollection->addStock($this, $stock->getStockID())) {

          // Gets the db.
          $db = $stockcollection->getDB();
          if ($db) {

            // Adds a dbxref.
            $dbxref = MCL_CHADO_DBXREF::addDBXref($this, $db->getDbID(), $line['stock_name_in_collection']);
            if ($dbxref) {

              // Adds a stock_dbxref.
              $stock->addDBXref($this, $dbxref);
            }
          }
        }
      }
    }
  }
}