<?php
/**
 * The declaration of MCL_TEMPLATE_STOCK_COTTON class.
 *
 */
class MCL_TEMPLATE_STOCK_COTTON extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'STOCK';
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
   * @see MCL_TEMPLATE::getDefDataValidTypes()
   */
  public function getDefDataValidTypes() {
    $values = array(
      'breeding_research_material',
      'variety',
      'cultivar',
      'population',
      'wild_unimproved',
      'tbd',
      'landrace',
      'germplasm',
      'species',
      'breeding_germplasm',
    );
    $stock_type = array(
      'cv_name'     => 'SITE_CV',
      'values'      => $values,
      'description' => 'Types for a stock',
    );
    return array('stock_type' => $stock_type);
  }

  /**
   * @see MCL_TEMPLATE::defineDescription()
   */
  public function defineDescription() {
    $desc = 'The description for stock sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'stock_name'        => array('req' => TRUE,  'width' => 10, 'desc' => "Primary ID or name of stock. The most important ID or name (unique and stable at least within the breeding program) should be in this column. For mapping population, follow the naming convention of each database."),
      'germplasm_type'    => array('req' => TRUE,  'width' => 10, 'desc' => "Select from the three choices (cultivar, breeding_research_material, or wild_unimproved) for an individual germplasm or 'population' for a group of individual."),
      'genus'             => array('req' => TRUE,  'width' => 10, 'desc' => "Genus to which the stock belongs to."),
      'species'           => array('req' => TRUE,  'width' => 10, 'desc' => "Species name. Enter 'sp.' to represent one unknown species, 'spp.' to represent multiple unknown species."),
      'grin_id'           => array('req' => FALSE, 'width' => 10, 'desc' => "Accession ID (ACID) in the GRIN database, if it is known. Multiple GRIN ID, separated by comma, can be added."),
      'subspecies'        => array('req' => FALSE, 'width' => 10, 'desc' => "Subspecies name."),
      'description'       => array('req' => FALSE, 'width' => 10, 'desc' => "Stock_name for the pollen parent of this entry.  This stock_name must also have its own record in this table (it should be entered before being listed as a parent)."),
      'secondary_id'      => array('req' => TRUE,  'width' => 10, 'desc' => "Alternate ID or name that is commonly used to refer to the entry."),
      'maternal_parent'   => array('req' => FALSE, 'width' => 10, 'desc' => "Stock_name for the seed parent of this entry.  This stock_name must also have its own record in this table (it should be entered before being listed as a parent)."),
      'paternal_parent'   => array('req' => FALSE, 'width' => 10, 'desc' => "Stock_name for the pollen parent of this entry.  This stock_name must also have its own record in this table (it should be entered before being listed as a parent)."),
      'mutation_parent'   => array('req' => FALSE, 'width' => 10, 'desc' => "Stock_name of the mutation parent. This stock_name must also have its own record in this table (it should be entered before being listed as a parent)."),
      'selfing_parent'    => array('req' => FALSE, 'width' => 10, 'desc' => "Stock_name of the selfing parent. This stock_name must also have its own record in this table (it should be entered before being listed as a parent)."),
      'alias'             => array('req' => FALSE, 'width' => 10, 'desc' => "A concatenation of all other less commonly used names or designations for the entry.  the aliases. The format is 'Alias type1:Alias1; Alias type2:Alias2'. When there is no specific type for aliases, just write the aliases without the specific type.  (eg. Collector:98HT-227; Site:W6 21306; ABC-1; 21306) The format is 'Alias type1:Alias1; Alias type2:Alias2'. When there is no specific type for an alias, just write the aliases without the specific type. (eg. Collector:98HT-227; Site:W6 21306; ABC-1; 21306)."),
      'cultivar'          => array('req' => FALSE, 'width' => 10, 'desc' => "Cultivar name if It is a named, cultivated variety."),
      'pedigree'          => array('req' => FALSE, 'width' => 10, 'desc' => "Any additional pedigree information: When the exact parents are not known, or any additional information beyond just parents (eg. Red-fruit sport of Jonathan, Pyrus sp. X Cydonia sp., etc)."),
      'origin_detail'     => array('req' => FALSE, 'width' => 10, 'desc' => "The origin for the variety (especially for wild variety)."),
      'origin_country'    => array('req' => FALSE, 'width' => 10, 'desc' => "The original country for the variety (especially for wild variety)."),
      'population_size'   => array('req' => FALSE, 'width' => 10, 'desc' => "Only for type 'population'."),
      'germplasm_center'  => array('req' => FALSE, 'width' => 10, 'desc' => "Stock center or lab where the stock is distributed.  Otherwise, provide the details of the contact person in contact column (from whom the material can be obtained) in the 'Contact' sheet."),
      'description'       => array('req' => FALSE, 'width' => 10, 'desc' => "any description for the stock."),
      'comments'          => array('req' => FALSE, 'width' => 10, 'desc' => "Any comments on the stock."),
      'image'             => array('req' => FALSE, 'width' => 10, 'desc' => "Image_ID of any associated image (Detailed info in 'Image' sheet.) Multiple images, separated by comma, can be added."),
      'reference'         => array('req' => FALSE, 'width' => 10, 'desc' => "pub_id if any publication is associated with the stock."),
      '##property_name'   => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns (##) : followed by name (cvterm name) of the property."),
    );

    // Adds labels.
    $headers['grin_id']['label']      = 'GRIN_ID';
    $headers['secondary_id']['label'] = 'secondary_ID';
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    //$cvterms['SITE_CV']['stock_type']                 = -1;
    $cvterms['SITE_CV']['alias']                      = -1;
    $cvterms['SITE_CV']['secondary_id']               = -1;
    $cvterms['SITE_CV']['germplasm_center']           = -1;
    $cvterms['SITE_CV']['cultivar']                   = -1;
    $cvterms['SITE_CV']['is_a_maternal_parent_of']    = -1;
    $cvterms['SITE_CV']['is_a_paternal_parent_of']    = -1;
    $cvterms['SITE_CV']['is_a_mutation_parent_of']    = -1;
    $cvterms['SITE_CV']['is_a_selfing_parent_of']     = -1;
    $cvterms['SITE_CV']['maternal_parent']            = -1;
    $cvterms['SITE_CV']['paternal_parent']            = -1;
    $cvterms['SITE_CV']['mutation_parent']            = -1;
    $cvterms['SITE_CV']['selfing_parent']             = -1;
    $cvterms['SITE_CV']['pedigree']                   = -1;
    $cvterms['SITE_CV']['description']                = -1;
    $cvterms['SITE_CV']['origin_country']             = -1;
    $cvterms['SITE_CV']['origin_detail']              = -1;
    $cvterms['SITE_CV']['comments']                   = -1;
    $cvterms['SITE_CV']['population_size']            = -1;
    $cvterms['SITE_CV']['subspecies']                 = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks DB for GRIN.
    MCL_CHADO_DB::checkDB($this, 'GRIN');

    // Checks organism.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);

    // Checks aliases.
    MCL_CHADO_STOCK::checkAlias($this, $line['alias'], "[,;]");

    // Checks images.
    MCL_CHADO_IMAGE::checkImageID($this, $line['image'], "[,;]");

    // Checks references.
    MCL_CHADO_PUB::checkPubID($this, $line['reference'], "[,;]");

    // Checks germplasm type.
    if (!MCL_DATA_VALID_TYPE::validate('stock_type', $line['germplasm_type'])) {
      $this->updateMsg('E', 'Invalid stock type : ' . $line['germplasm_type']);
    }
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the organism.
    $organism = MCL_CHADO_ORGANISM::getOrganism($line['genus'], $line['species']);
    $organism_id = $organism->getOrganismID();

    // Gets stock type.
    $type_id = MCL_CHADO_CVTERM::getCvterm('SITE_CV', $line['germplasm_type'])->getCvtermID();

    // Adds a stock.
    $stock = MCL_CHADO_STOCK::addStockBy2ndID($this, $line['stock_name'], $line['secondary_id'], $organism_id, $type_id);
    if ($stock) {

      // Gets and add the DB.
      $db = MCL_CHADO_DB::getDB('GRIN');
      $stock->addDB($this, $db, $line['grin_id'], "[,;]");

      // Adds properties.
      //$stock->addProp($this, 'SITE_CV', 'stock_type', $line['germplasm_type']);
      $stock->addProp($this, 'SITE_CV', 'secondary_id', $line['secondary_id']);
      $stock->addProp($this, 'SITE_CV', 'description', $line['description']);
      $stock->addProp($this, 'SITE_CV', 'cultivar', $line['cultivar']);
      $stock->addProp($this, 'SITE_CV', 'pedigree', $line['pedigree']);
      $stock->addProp($this, 'SITE_CV', 'origin_detail', $line['origin_detail']);
      $stock->addProp($this, 'SITE_CV', 'origin_country', $line['origin_country']);
      $stock->addProp($this, 'SITE_CV', 'population_size', $line['population_size']);
      $stock->addProp($this, 'SITE_CV', 'subspecies', $line['subspecies']);
      $stock->addProp($this, 'SITE_CV', 'germplasm_center', $line['germplasm_center']);
      $stock->addProp($this, 'SITE_CV', 'comments', $line['comments']);

      // Adds the user-specific properties.
      $this->addSpecialColumns('##', $stock, $line);

      // Adds aliases.
      $stock->addAlias($this, $line['alias'], "[,;]");

      // Adds parents.
      $stock->addParent($this, $line['maternal_parent'], '', $this->cvterms['SITE_CV']['is_a_maternal_parent_of']);
      $stock->addParent($this, $line['paternal_parent'], '', $this->cvterms['SITE_CV']['is_a_paternal_parent_of']);
      $stock->addParent($this, $line['mutation_parent'], '', $this->cvterms['SITE_CV']['is_a_mutation_parent_of']);
      $stock->addParent($this, $line['selfing_parent'], '', $this->cvterms['SITE_CV']['is_a_selfing_parent_of']);

      // Adds images.
      $stock->addImageID($this, $line['image'], "[,;]");

      // Adds references.
      $stock->addReference($this, $line['reference'], "[,;]");
    }
  }

  /**
   * @see MCL_TEMPLATE::postProcessECD()
   */
  public function postProcessECD() {

    // Lists the job paremters.
    $cv_name = $this->getJob()->getParamByKey('cv');

    // Gets the cv.
    $cv = MCL_CHADO_CV::getCV($cv_name, 'SITE_CV');
    if (!$cv) {
      $this->updateMsg('E', "cv does not exist.");
     return FALSE;
    }

    // Saves the user-specific properties.
    if (!$this->saveSpecialColumns('##', $cv->getCvID(), $this->getHeaders())) {
      $this->updateMsg('E', "Failed to save the special columns");
      return FALSE;
    }

    // Initializes $GLOBALS.
    $GLOBALS['stocks'] = array();
    return TRUE;
  }

  /**
   * @see MCL_TEMPLATE::postProcessDU()
   */
  public function postProcessDU() {

    // Adds all missing parents.
    $parents = $GLOBALS['stocks'];
    $no_error = TRUE;
    foreach ((array)$parents as $stock_id => $info) {

      // Gets the parent info.
      $child_stock_id = $info['child_stock_id'];
      $uniquename     = $info['uniquename'];
      $type_id_rel    = $info['type_id_rel'];

      // Checks the parent.
      $parent = MCL_CHADO_STOCK::byKey(array('uniquename'  => $uniquename));
      if ($parent) {

        // Adds the parent.
        $accession = MCL_CHADO_STOCK::byKey(array('stock_id' => $child_stock_id));
        $accession->addParent($this, $uniquename, $organism_id, $type_id_rel);
      }
      else {
        $this->updateMsg('E', "$uniquename does not exist");
        $no_error = FALSE;
      }
    }
    return $no_error;
  }
}