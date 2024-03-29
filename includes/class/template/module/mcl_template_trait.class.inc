<?php
/**
 * The declaration of MCL_TEMPLATE_TRAIT class.
 *
 */
class MCL_TEMPLATE_TRAIT extends MCL_TEMPLATE {

 /**
  *  Class data members.
  */
  public $valid_trait_categories = array(
    'biochemical trait', 'plant growth and development trait',
    'plant morphology trait', 'plant quality trait', 'plant vigor trait',
    'sterility or fertility trait', 'stress trait', 'yield trait', 'plant morphology trait',
    'biological process trait', 'other miscellaneous trait'
  );

  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'TRAIT';
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
    $desc = 'The description for trait sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {;
    $headers = array(
      'trait_name'      => array('req' => TRUE,  'width' => 20, 'desc' => "Trait ontology term for the database or crop. QTLs, MTLs and phenotypic descriptors used in breeding programs can be associated with trait names."),
      'trait_category'  => array('req' => FALSE, 'width' => 20, 'desc' => "The root term of the Plant Tait Ontology for the trait name. Choose one from the list (biochemical trait | plant growth and development trait | plant morphology trait | plant quality trait | plant vigor trait | sterility or fertility trait | stress trait | yield trait | plant morphology trait | biological process trait | other miscellaneous trait)."),
      'abbreviation'    => array('req' => FALSE, 'width' => 14, 'desc' => "Abbreviation of the trait ontology term (trait_name)."),
      'definition'      => array('req' => FALSE, 'width' => 50, 'desc' => "Definition of trait ontology term (trait_name)."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['abbreviation'] = -1;
    $cvterms['relationship']['is_a']    = -1;
    $cvterms['SITE_TRAIT_ONTOLOGY']['biochemical trait']                  = -1;
    $cvterms['SITE_TRAIT_ONTOLOGY']['plant growth and development trait'] = -1;
    $cvterms['SITE_TRAIT_ONTOLOGY']['plant morphology trait']             = -1;
    $cvterms['SITE_TRAIT_ONTOLOGY']['plant quality trait']                = -1;
    $cvterms['SITE_TRAIT_ONTOLOGY']['plant vigor trait']                  = -1;
    $cvterms['SITE_TRAIT_ONTOLOGY']['sterility or fertility trait']       = -1;
    $cvterms['SITE_TRAIT_ONTOLOGY']['stress trait']                       = -1;
    $cvterms['SITE_TRAIT_ONTOLOGY']['yield trait']                        = -1;
    $cvterms['SITE_TRAIT_ONTOLOGY']['plant morphology trait']             = -1;
    $cvterms['SITE_TRAIT_ONTOLOGY']['biological process trait']           = -1;
    $cvterms['SITE_TRAIT_ONTOLOGY']['other miscellaneous trait']          = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks for trait category.
    MCL_CHADO_TRAIT::checkCategory($this, $line['trait_category']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Adds a trait.
    $trait = MCL_CHADO_TRAIT::addTrait($this, $line['trait_name'], $line['definition']);
    if ($trait) {

      // Adds properties.
      $trait->addProp($this, 'SITE_CV', 'abbreviation', $line['abbreviation']);

      // Adds trait categories.
      $trait->addCategory($this, $line['trait_category'], $this->cvterms['relationship']['is_a'], "[,;]");
    }
  }
}
