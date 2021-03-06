<?php
/**
 * The declaration of MCL_TEMPLATE_SITE_COTTON class.
 *
 */
class MCL_TEMPLATE_SITE_COTTON extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'SITE';
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
      'orchard',
      'seedling block',
      'open field',
      'greenhouse',
    );
    $location_type = array(
      'cv_name'     => '',
      'values'      => $values,
      'description' => 'Types for a location',
    );
    return array('location_type' => $location_type);
  }

  /**
   * @see MCL_TEMPLATE::defineDescription()
   */
  public function defineDescription() {
    $desc = 'The description for site sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'site_name'               => array('req' => TRUE,  'width' => 10, 'desc' => "Curator-assigned name of the site."),
      'site_long_name'          => array('req' => FALSE, 'width' => 10, 'desc' => "long name of the site."),
      'latitude'                => array('req' => FALSE, 'width' => 10, 'desc' => "The decimal latitude coordinate of the georeference, using positive and negative sign to indicate N and S, respectively."),
      'longitude'               => array('req' => FALSE, 'width' => 10, 'desc' => "The decimal longitude coordinate of the georeference, using positive and negative sign to indicate E and W, respectively."),
      'altitude'                => array('req' => FALSE, 'width' => 10, 'desc' => "The altitude (elevation) of the location in meters. If the altitude is only known as a range, this is the average, and altitude_dev will hold half of the width of the range."),
      'geodetic_datum'          => array('req' => FALSE, 'width' => 10, 'desc' => "The geodetic system on which the geo-reference coordinates are based. For geo-references measured between 1984 and 2010, this will typically be WGS84."),
      'type'                    => array('req' => TRUE,  'width' => 10, 'desc' => "'orchard', 'nursery' or any other types of location."),
      'country'                 => array('req' => FALSE, 'width' => 10, 'desc' => "Country where the site belongs to."),
      'state'                   => array('req' => FALSE, 'width' => 10, 'desc' => "State where the site belongs to."),
      'region'                  => array('req' => FALSE, 'width' => 10, 'desc' => "Region where the site belongs to."),
      'address'                 => array('req' => FALSE, 'width' => 10, 'desc' => "The entire address except the country."),
      'comments'                => array('req' => FALSE, 'width' => 10, 'desc' => "Any further comments on the site."),
      'data_year'               => array('req' => FALSE, 'width' => 10, 'desc' => ""),
      'experimental_design'     => array('req' => FALSE, 'width' => 10, 'desc' => ""),
      'replications'            => array('req' => FALSE, 'width' => 10, 'desc' => ""),
      'plants_per_replication'  => array('req' => FALSE, 'width' => 10, 'desc' => ""),
      'plant_date'              => array('req' => FALSE, 'width' => 10, 'desc' => ""),
      'plot_distance'           => array('req' => FALSE, 'width' => 10, 'desc' => ""),
      'row_distance'            => array('req' => FALSE, 'width' => 10, 'desc' => ""),
      'plant_distance'          => array('req' => FALSE, 'width' => 10, 'desc' => ""),
      'irrigation'              => array('req' => FALSE, 'width' => 10, 'desc' => ""),
      'soil_type'               => array('req' => FALSE, 'width' => 10, 'desc' => ""),
      'agronomic_control'       => array('req' => FALSE, 'width' => 10, 'desc' => ""),
      'pick_date'               => array('req' => FALSE, 'width' => 10, 'desc' => ""),
      'evaluation_date'         => array('req' => FALSE, 'width' => 10, 'desc' => ""),
      '##property_name'         => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns (##) : followed by name (cvterm name) of the property."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {

    $cvterms = array();
    $cvterms['SITE_CV']['type']                   = -1;
    $cvterms['SITE_CV']['site_long_name']         = -1;
    $cvterms['SITE_CV']['country']                = -1;
    $cvterms['SITE_CV']['state']                  = -1;
    $cvterms['SITE_CV']['region']                 = -1;
    $cvterms['SITE_CV']['address']                = -1;
    $cvterms['SITE_CV']['comments']               = -1;
    $cvterms['SITE_CV']['data_year']              = -1;
    $cvterms['SITE_CV']['experimental_design']    = -1;
    $cvterms['SITE_CV']['replications']           = -1;
    $cvterms['SITE_CV']['plants_per_replication'] = -1;
    $cvterms['SITE_CV']['plant_date']             = -1;
    $cvterms['SITE_CV']['plot_distance']          = -1;
    $cvterms['SITE_CV']['row_distance']           = -1;
    $cvterms['SITE_CV']['plant_distance']         = -1;
    $cvterms['SITE_CV']['irrigation']             = -1;
    $cvterms['SITE_CV']['soil_type']              = -1;
    $cvterms['SITE_CV']['agronomic_control']      = -1;
    $cvterms['SITE_CV']['pick_date']              = -1;
    $cvterms['SITE_CV']['evaluation_date']        = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks for the contact type.
    if (!MCL_DATA_VALID_TYPE::validate('location_type', $line['type'])) {
      $this->updateMsg('E', 'Invalid location type : ' . $line['type']);
    }
  }

    /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Adds a location.
    $loc_info = array();
    if ($line['latitude']) {
      $loc_info['latitude'] = $line['latitude'];
    }
    if ($line['longitude']) {
      $loc_info['longitude'] = $line['longitude'];
    }
    if ($line['altitude']) {
      $loc_info['altitude'] = $line['altitude'];
    }
    if ($line['geodetic_datum']) {
      $loc_info['geodetic_datum'] = $line['geodetic_datum'];
    }
    $location = MCL_CHADO_LOCATION::addLocation($this, $line['site_name'], $loc_info);
    if ($location) {

      // Adds the properties.
      $location->addProp($this, 'SITE_CV', 'type', $line['type']);
      $location->addProp($this, 'SITE_CV', 'site_long_name', $line['site_long_name']);
      $location->addProp($this, 'SITE_CV', 'country', $line['country']);
      $location->addProp($this, 'SITE_CV', 'state', $line['state']);
      $location->addProp($this, 'SITE_CV', 'region', $line['region']);
      $location->addProp($this, 'SITE_CV', 'address', $line['address']);
      $location->addProp($this, 'SITE_CV', 'country', $line['country']);
      $location->addProp($this, 'SITE_CV', 'comments', $line['comments']);
      $location->addProp($this, 'SITE_CV', 'data_year', $line['data_year']);
      $location->addProp($this, 'SITE_CV', 'experimental_design', $line['experimental_design']);
      $location->addProp($this, 'SITE_CV', 'replications', $line['replications']);
      $location->addProp($this, 'SITE_CV', 'plants_per_replication', $line['plants_per_replication']);
      $location->addProp($this, 'SITE_CV', 'plant_date', $line['plant_date']);
      $location->addProp($this, 'SITE_CV', 'plot_distance', $line['plot_distance']);
      $location->addProp($this, 'SITE_CV', 'row_distance', $line['row_distance']);
      $location->addProp($this, 'SITE_CV', 'plant_distance', $line['plant_distance']);
      $location->addProp($this, 'SITE_CV', 'irrigation', $line['irrigation']);
      $location->addProp($this, 'SITE_CV', 'soil_type', $line['soil_type']);
      $location->addProp($this, 'SITE_CV', 'agronomic_control', $line['agronomic_control']);
      $location->addProp($this, 'SITE_CV', 'pick_date', $line['pick_date']);
      $location->addProp($this, 'SITE_CV', 'evaluation_date', $line['evaluation_date']);

      // Adds the user-specific properties.
      $this->addSpecialColumns('##', $location, $line);
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
    return TRUE;
  }
}