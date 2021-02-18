<?php
/**
 * The declaration of MCL_TEMPLATE_DELETE_FEATURE class.
 *
 */
class MCL_TEMPLATE_DELETE_FEATURE extends MCL_TEMPLATE {

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
    $desc = 'The description for delete feature sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'feature_id'    => array('req' => FALSE,  'width' => 10, 'desc' => "ID of the feature to be deleted. If feature ID is provied, the rest of columns will be ignored"),
      'feature_name'  => array('req' => FALSE,  'width' => 10, 'desc' => "name of the feature to be deleted."),
      'type'          => array('req' => FALSE,  'width' => 10, 'desc' => "type of the feature to be deleted."),
      'genus'         => array('req' => FALSE,  'width' => 10, 'desc' => "genus of the feature to be deleted."),
      'species'       => array('req' => FALSE,  'width' => 10, 'desc' => "species of the feature to be deleted."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks the feature.
    if (array_key_exists('feature_id', $line)) {
      MCL_CHADO_FEATURE::checkFeatureID($this, $line['feature_id'], TRUE);
    }
    else if (array_key_exists('feature_name', $line)) {

      // Checks feature type.
      $type_id = MCL_CHADO_FEATURE::getFeatureTypeID($line['type']);
      if ($type_id) {

        // Checks feature by its properties.
        MCL_CHADO_FEATURE::checkFeature($this, $line['feature_name'], $line['genus'], $line['species'], $type_id, '', TRUE);
      }
      else {
        $this->updateMsg('E', 'Invalid feature type : ' . $line['type']);
      }
    }
    else {
      $this->updateMsg('E', "The column 'feature_id' or 'feature_name' is missing.");
    }
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the feature.
    $feature = NULL;
    if (array_key_exists('feature_id', $line)) {
      $feature = MCL_CHADO_FEATURE::byID($line['feature_id']);
    }
    else {
      $type_id = MCL_CHADO_FEATURE::getFeatureTypeID($line['type']);
      $feature = MCL_CHADO_FEATURE::getFeature($line['feature_name'], $line['genus'], $line['species'], $type_id);
    }

    // Deletes the feature.
    if ($feature) {
      $feature_name = $feature->getUniquename();
      if ($feature->delete()) {
        $this->updateMsg('L', "The feature has been deleted ($feature_name)");
      }
      else {
        $this->updateMsg('E', "Failed to delete the feature ($feature_name)");
      }
    }
  }
}