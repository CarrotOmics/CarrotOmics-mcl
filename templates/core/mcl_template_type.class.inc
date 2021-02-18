<?php
/**
 * The declaration of MCL_TEMPLATE_TYPE class.
 *
 */
class MCL_TEMPLATE_TYPE extends PUBLIC_MCL_TEMPLATE_TYPE {

 /**
  *  Class data members.
  */
  /**
   * @see PUBLIC_MCL_TEMPLATE_TYPE::__construct()
   */
  public function __construct($details) {
    parent::__construct($details);
  }

  /**
   * @see PUBLIC_MCL_TEMPLATE_TYPE::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
      return new self($parent->getMemberArr());
    }
  }

  /**
   * Returns MCL_TEMPLATE_TYPE by type.
   *
   * @param string $type
   */
  public static function byType($type) {
    return self::byKey(array('type' => $type));
  }

  /**
   * @see PUBLIC_MCL_TEMPLATE_TYPE::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * Adds a new template type.
   *
   * @param array $details
   *
   * @return boolean
   */
  public static function addTemplateType($details = array()) {

    // Checks for duplication.
    $mcl_template_type = MCL_TEMPLATE_TYPE::byType($details['type']);
    if (!$mcl_template_type) {

      // Adds a new template type.
      $mcl_template_type = new MCL_TEMPLATE_TYPE($details);
      return $mcl_template_type->insert();
    }
    return TRUE;
  }

  /**
   * Returns template information in array. Adds the information for
   * creating two excel sheets for header definition and data template.
   *
   * @param string $order_by
   *
   * @return array of MCL_TEMPLATE_TYPE
   */
  public static function getTemplateTypes($order_by = '') {

    // Updates $order_by.
    $order_by_str = 'ORDER BY TT.type';
    if ($order_by) {
      $order_by_str = 'ORDER BY TT.' . $order_by;
    }

    // Gets template types
    $sql = "SELECT template_type_id FROM {mcl_template_type} TT $order_by_str";
    $mcl_template_types = array();
    $results = db_query($sql);
    while ($template_type_id = $results->fetchField()) {
      $mcl_template_types []= MCL_TEMPLATE_TYPE::byKey(array('template_type_id' => $template_type_id));
    }
    return $mcl_template_types;
  }

  /**
   * Returns the default template types.
   *
   * @return boolen
   */
  public static function getDefTemplateTypes() {

    // Default template types.
    return array(
      'PUB',
      'ORGANISM',
      'DB',
      'CV',
      'TRAIT',
      'CVTERM',
      'ORGANISM',
      'CONTACT',
      'ANALYSIS',
      'LIBRARY',
      'DATASET',
      'DESCRIPTOR',
      'SITE',
      'ENVIRONMENT',
      'STOCK',
      'STOCK_POST',
      'CROSS',
      'PROGENY',
      'MARKER',
      'MARKER_POST',
      'MTL',
      'QTL',
      'HAPLOTYPE_BLOCK',
      'MAP',
      'MAP_POSITION',
      'FIELD_BOOK',
      'PHENOTYPE',
      'GENOTYPE',
      'GENOTYPE_POST',
      'HAPLOTYPE',
      'IMAGE',
      'IMAGE_PHENOTYPE',
      'IMAGE_GENOTYPE',
      'UPDATE',
      'DELETE',
      'MERGE',
    );
  }

  /**
   * Updates the template types.
   *
   * @param array $ranks
   *
   * @return boolen
   */
  public static function updateTemplateTypes($ref_ranks = array()) {

    // Updates the reference ranks.
    if (!array_key_exists('UPDATE', $ref_ranks)) {
      $ref_ranks['UPDATE'] = 999999;
    }
    if (!array_key_exists('DELETE', $ref_ranks)) {
      $ref_ranks['DELETE'] = 999999;
    }
    if (!array_key_exists('MERGE', $ref_ranks)) {
      $ref_ranks['MERGE'] = 999999;
    }

    // Gets the template type ranks.
    $template_types = MCL_TEMPLATE_TYPE::getDefTemplateTypes();
    $rank = 10;
    foreach ($template_types as $template_type) {

      // Gets the rank for the template type.
      if (array_key_exists($template_type, $ref_ranks)) {
        $rank = $ref_ranks[$template_type];
      }
      else {
        $rank += 10;
      }

      // Checks if this template type exists.
      $details = array('type' => $template_type);
      $mcl_template_type = MCL_TEMPLATE_TYPE::byKey($details);

      // Updates the rank of the template type.
      if ($mcl_template_type) {
        $mcl_template_type->setRank($rank);
        if (!$mcl_template_type->update()) {
          print "\nError : Failed to update the rank of the template type\n\n";
          return FALSE;
        }
      }

      // Adds a template type.
      else {

        $details['rank'] = $rank;
        $mcl_template_type = new MCL_TEMPLATE_TYPE($details);
        if (!$mcl_template_type->insert()) {
          print "\nError : Failed to add a template type\n\n";
          return FALSE;
        }
      }
    }
    return TRUE;
  }
}