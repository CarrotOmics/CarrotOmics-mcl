<?php
/**
 * The declaration of MCL_CHADO_STOCKCOLLECTION class.
 *
 */
class MCL_CHADO_STOCKCOLLECTION extends CHADO_STOCKCOLLECTION {

 /**
  *  Class data members.
  */
  /**
   * @see MCL_CHADO_STOCKCOLLECTION::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see MCL_CHADO_STOCKCOLLECTION::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * @see MCL_CHADO_STOCKCOLLECTION::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }


  /**
   * Checks the existence of stockcollection and db.
   * If not, write the error messasge to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $uniquename
   * @param integer $type_id
   *
   * @return boolean
   */
  public static function checkStockcollection(MCL_TEMPLATE $mcl_tmpl = NULL, $uniquename, $type_id) {
    if ($uniquename) {

      // Gets the stockcollection.
      $stockcollection = MCL_CHADO_STOCKCOLLECTION::getStockcollection($uniquename, $type_id);
      if (!$stockcollection) {
        self::updateMsg($mcl_tmpl, 'E', "(uniquename, type_id) = ($uniquename, $type_id) not found in stockcollection");
        return FALSE;
      }

      // Gets the db.
      $db = $stockcollection->getDB();
      if (!$db) {
        self::updateMsg($mcl_tmpl, 'E', "$uniquename does not have db");
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Returns the stockcollection.
   *
   * @param string $uniquename
   * @param integer $type_id
   *
   * @return MCL_CHADO_STOCKCOLLECTION
   */
  public static function getStockcollection($uniquename, $type_id) {
    $keys = array(
      'uniquename'  => $uniquename,
      'type_id'     => $type_id,
    );
    return MCL_CHADO_STOCKCOLLECTION::byKey($keys);
  }

  /**
   * Adds a new stockcollection.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $uniquename
   * @param string $name
   * @param integer $type_id
   * @param integer $contact_id
   *
   * @return MCL_CHADO_STOCKCOLLECTION
   */
  public static function addStockcollection(MCL_TEMPLATE $mcl_tmpl = NULL, $uniquename, $name, $type_id, $contact_id = NULL) {

    // Sets the arguments.
    $args = array(
      'uniquename'  => $uniquename,
      'type_id'     => $type_id,
    );

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $stockcollection = MCL_CHADO_STOCKCOLLECTION::byKey($args);
    if ($stockcollection) {
      self::addMsg($mcl_tmpl, 'D', 'stockcollection', $args);
    }
    else {

      // Adds a new stockcollection.
      $args['name'] = $name;
      if ($contact_id) {
        $args['contact_id'] = $contact_id;
      }
      $stockcollection = new MCL_CHADO_STOCKCOLLECTION($args);
      if ($stockcollection->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'stockcollection', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'stockcollection', $args);
        return NULL;
      }
    }
    return $stockcollection;
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
      return $this->addProperty($mcl_tmpl, 'stockcollectionprop', 'stockcollection_id', $this->stockcollection_id, $type_id, $value, $separator);
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
      return $this->addProperty($mcl_tmpl, 'stockcollectionprop', 'stockcollection_id', $this->stockcollection_id, $type_id, $value, $separator);
    }
    return TRUE;
  }

  /**
   * Adds db to stockcollection_db.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $db_id
   *
   * @return boolean
   */
  public function addDB(MCL_TEMPLATE $mcl_tmpl = NULL, $db_id) {
    if ($db_id) {
      return $this->addLink($mcl_tmpl, 'stockcollection_db', 'stockcollection_id', $this->stockcollection_id, 'db_id', $db_id);
    }
    return TRUE;
  }

  /**
   * Gets db from stockcollection_db.
   *
   * @return MCL_CHADO_DB
   */
  public function getDB() {
    $key = array(
      'stockcollection_id' => $this->stockcollection_id,
    );
    return CHADO_STOCKCOLLECTION_DB::byKey($key);
  }


  /**
   * Adds a stock to the stockcollection.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $stock_id
   *
   * @return boolean
   */
  public function addStock(MCL_TEMPLATE $mcl_tmpl = NULL, $stock_id) {
    if ($stock_id) {
      $this->addLink($mcl_tmpl, 'stockcollection_stock', 'stockcollection_id', $this->stockcollection_id, 'stock_id', $stock_id);
    }
    return TRUE;
  }
}