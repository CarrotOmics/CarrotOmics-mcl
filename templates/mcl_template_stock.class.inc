<?php
/**
 * The declaration of MCL_TEMPLATE_STOCK class.
 *
 */
class MCL_TEMPLATE_STOCK extends MCL_TEMPLATE {

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
      'infraspecific_type'=> array('req' => FALSE, 'width' => 10, 'desc' => "Infraspecific type of the stock as defined in the taxonomic_rank CV"),
      'infraspecific_name'=> array('req' => FALSE, 'width' => 10, 'desc' => "Infraspecific name of the stock"),
      'germplasm_id'      => array('req' => FALSE, 'width' => 10, 'desc' => "Accession ID (ACID) in a germplasm database, if it is known. e.g. \"GRIN:PI 123456\" Multiple ACID values, separated by comma, can be added."),
      'description'       => array('req' => FALSE, 'width' => 10, 'desc' => "Stock_name for the pollen parent of this entry.  This stock_name must also have its own record in this table (it should be entered before being listed as a parent)."),
      'secondary_id'      => array('req' => FALSE, 'width' => 10, 'desc' => "Alternate ID or name that is commonly used to refer to the entry."),
      'maternal_parent'   => array('req' => FALSE, 'width' => 10, 'desc' => "Stock name of the maternal parnet. It must exist in stock table or listed in this sheet. If the organims of the parent and the child are different, add the organism as prefix of name of parent separated by double colon '::'."),
      'paternal_parent'   => array('req' => FALSE, 'width' => 10, 'desc' => "Stock name of the paternal parnet. It must exist in stock table or listed in this sheet. If the organims of the parent and the child are different, add the organism as prefix of name of parent separated by double colon '::'."),
      'mutation_parent'   => array('req' => FALSE, 'width' => 10, 'desc' => "Stock name of the mutation parent. It must exist in stock table or listed in this sheet. If the organims of the parent and the child are different, add the organism as prefix of name of parent separated by double colon '::'."),
      'selfing_parent'    => array('req' => FALSE, 'width' => 10, 'desc' => "Stock name of the selfing parent. It must exist in stock table or listed in this sheet. If the organims of the parent and the child are different, add the organism as prefix of name of parent separated by double colon '::'."),
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
    $headers['germplasm_id']['label'] = 'Germplasm_ID';
    $headers['secondary_id']['label'] = 'secondary_ID';
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
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
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks DB for existence of supplied germplasm_id prefix
    if ($line['germplasm_id']) {
      $accessions = preg_split("/[,;]/", $line['germplasm_id'] );
      foreach ($accessions as $accession) {
        $parts = explode(":", $accession);
        if ($parts[1]) {
          MCL_CHADO_DB::checkDB($this, $parts[0]);
        }
        else {
          $this->updateMsg('E', 'Invalid germplasm_id "'.$accession.'": ' . $line['germplasm_id']);
        }
      }
    }

    // Checks organism.
    MCL_CHADO_ORGANISM::checkOrganismInfra($this, $line['genus'], $line['species'], $line['infraspecific_type'], $line['infraspecific_name'], "[;]");

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
    $organism = MCL_CHADO_ORGANISM::getOrganismInfra($line['genus'], $line['species'], $line['infraspecific_type'], $line['infraspecific_name']);
    $organism_id = $organism->getOrganismID();

    // Gets stock type.
    $type_id = MCL_CHADO_CVTERM::getCvterm('SITE_CV', $line['germplasm_type'])->getCvtermID();

    // Updates name.
    $name = ($line['secondary_id']) ? $line['secondary_id'] : $line['stock_name'];

    // Adds a stock.
    $stock = MCL_CHADO_STOCK::addStock($this, $line['stock_name'], $name, $organism_id, $type_id);
    if ($stock) {

      // Gets and add the DB.
      if ($line['germplasm_id']) {
        $accessions = preg_split("/[,;]/", $line['germplasm_id'] );
        foreach ($accessions as $accession) {
          $parts = explode(":", $accession);
          if ($parts[1]) {
            $db = MCL_CHADO_DB::getDB($parts[0]);
            $stock->addDB($this, $db, $parts[1], "[,;]");
          }
        }
      }

      // Adds properties.
      $stock->addProp($this, 'SITE_CV', 'secondary_id', $line['secondary_id']);
      $stock->addProp($this, 'SITE_CV', 'description', $line['description']);
      $stock->addProp($this, 'SITE_CV', 'cultivar', $line['cultivar']);
      $stock->addProp($this, 'SITE_CV', 'pedigree', $line['pedigree']);
      $stock->addProp($this, 'SITE_CV', 'origin_country', $line['origin_country']);
      $stock->addProp($this, 'SITE_CV', 'origin_detail', $line['origin_detail']);
      $stock->addProp($this, 'SITE_CV', 'population_size', $line['population_size']);
      $stock->addProp($this, 'taxonomic_rank', 'type_id', $line['infraspecific_type']);
      $stock->addProp($this, 'SITE_CV', 'germplasm_center', $line['germplasm_center']);
      $stock->addProp($this, 'SITE_CV', 'comments', $line['comments']);

      // Adds the user-specific properties.
      $this->addSpecialColumns('##', $stock, $line);

      // Adds aliases.
      $stock->addAlias($this, $line['alias'], "[,;]");

      // Adds parents.
      $stock->addParent($this, $line['maternal_parent'], $organism_id, $this->cvterms['SITE_CV']['is_a_maternal_parent_of']);
      $stock->addParent($this, $line['paternal_parent'], $organism_id, $this->cvterms['SITE_CV']['is_a_paternal_parent_of']);
      $stock->addParent($this, $line['mutation_parent'], $organism_id, $this->cvterms['SITE_CV']['is_a_mutation_parent_of']);
      $stock->addParent($this, $line['selfing_parent'], $organism_id, $this->cvterms['SITE_CV']['is_a_selfing_parent_of']);

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
      $organism_id    = $info['organism_id'];
      $type_id_rel    = $info['type_id_rel'];

      // Checks the parent. If '::' found in uniquename, update uniquename
      // and organism_id.
      // Example "Daucus carota::B493B" or "Daucus carota subspecies sativus::B493B"
      if (preg_match("/^(.*?)::(.*?)$/", $uniquename, $matches)) {
        $uniquename         = trim($matches[2]);
        $parts = preg_split("/\s+/", trim($matches[1]));
        $genus              = $parts[0];
        $species            = $parts[1];
        $infraspecific_type = $parts[2];
        $infraspecific_name = $parts[3];
        $diff_organism = MCL_CHADO_ORGANISM::getOrganismInfra($genus, $species, $infraspecific_type, $infraspecific_name);
        $organism_id = $diff_organism->getOrganismID();
      }
      $keys = array(
        'uniquename'  => $uniquename,
        'organism_id' => $organism_id,
      );
      $parent = MCL_CHADO_STOCK::byKey($keys);
      if ($parent) {

        // Adds the parent.
        $stock = MCL_CHADO_STOCK::byKey(array('stock_id' => $child_stock_id));
        $stock->addParent($this, $uniquename, $organism_id, $type_id_rel);
      }
      else {
        $this->updateMsg('E', "$uniquename [organism ID = $organism_id] does not exist");
        $no_error = FALSE;
      }
    }
    return $no_error;
  }
}
