<?php
/**
 * The declaration of MCL_TEMPLATE_MERGE_FEATURE class.
 *
 */
class MCL_TEMPLATE_MERGE_FEATURE extends MCL_TEMPLATE {

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
    $desc = 'The description for merge feature sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'feature_type'      => array('req' => TRUE,  'width' => 10, 'desc' => "Type of the feature (marker, QTL or MTL)."),
      'feature_name_base' => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the base feature."),
      'genus_base'        => array('req' => TRUE,  'width' => 10, 'desc' => "Genus of the base feature"),
      'species_base'      => array('req' => TRUE,  'width' => 10, 'desc' => "Species name of the base feature."),
      'feature_name'      => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the feature to be removed."),
      'genus'             => array('req' => TRUE,  'width' => 10, 'desc' => "Genus of the feature to be removed."),
      'species'           => array('req' => TRUE,  'width' => 10, 'desc' => "Species name of the feature to be removed."),
      'delete'            => array('req' => FALSE, 'width' => 10, 'desc' => "'yes' or 'no' - 'yes' indicates that the merged feature will be removed."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks the type of merging features.
    $type_id = MCL_CHADO_FEATURE::getMergeFeatureTypeID($line['feature_type']);
    if (!$type_id) {
      $this->updateMsg('E', 'Invalid feature type : ' . $line['feature_type']);
      return;
    }

    // Checks the organism of the base feature.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus_base'], $line['species_base']);

    // Checks the base feature..
    MCL_CHADO_FEATURE::checkFeature($this, $line['feature_name_base'], $line['genus_base'], $line['species_base'], $type_id);

    // Checks the organism.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);

    // Checks the feature.
    MCL_CHADO_FEATURE::checkFeature($this, $line['feature_name'], $line['genus'], $line['species'], $type_id);

    // Checks delete.
    $this->checkDeleteFlag($line['delete']);

    // Keeps feature_id for merge check.
    $feature_base = MCL_CHADO_FEATURE::getFeature($line['feature_name_base'], $line['genus_base'], $line['species_base'], $type_id);
    $feature      = MCL_CHADO_FEATURE::getFeature($line['feature_name'], $line['genus'], $line['species'], $type_id);
    if ($feature_base && $feature) {
      $this->mergeCheck($feature_base->getFeatureID(), $feature->getFeatureID());
    }
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Checks the type of merging features and cvterm ID of the type.
    $type_id = MCL_CHADO_FEATURE::getMergeFeatureTypeID($line['feature_type']);

    // Gets the base feature.
    $org = MCL_CHADO_ORGANISM::getOrganism($line['genus_base'], $line['species_base']);
    $keys = array(
      'uniquename'  => $line['feature_name_base'],
      'organism_id' => $org->getOrganismID(),
      'type_id'     => $type_id,
    );
    $feature_base = MCL_CHADO_FEATURE::byKey($keys);

    // Getst the feature.
    $org = MCL_CHADO_ORGANISM::getOrganism($line['genus'], $line['species']);
    $keys = array(
      'uniquename'  => $line['feature_name'],
      'organism_id' => $org->getOrganismID(),
      'type_id'     => $type_id,
    );
    $feature = MCL_CHADO_FEATURE::byKey($keys);

    // Merges the features if not the same.
    if ($feature->getFeatureID() != $feature_base->getFeatureID()) {
      $feature_base->mergeFeature($this, $feature);
    }
    else {
      $this->updateMsg('W', "Both are the same features");
    }

    // Deletes the merged feature.
    if (strtolower($line['delete']) == 'yes') {
      $GLOBALS['delete_features'][$feature->getFeatureID()] = $feature;
    }
  }

  /**
   * @see MCL_TEMPLATE::preProcessDU()
   */
  public function preProcessDU() {
    $GLOBALS['delete_features'] = array();
    return TRUE;
  }

  /**
   * @see MCL_TEMPLATE::postProcessDU()
   */
  public function postProcessDU() {

    // Deletes the features.
    foreach ((array)$GLOBALS['delete_features'] as $feature_id => $feature) {
      $this->updateMsg('R', $feature->getRestoreObj());
      if (!$feature->delete()) {
        $this->updateMsg('E', "Failed to delete feature [ID = $feature_id]");
      }
    }
    return TRUE;
  }
}