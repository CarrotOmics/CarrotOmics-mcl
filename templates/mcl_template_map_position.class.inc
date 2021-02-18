<?php
/**
 * The declaration of MCL_TEMPLATE_MAP_POSITION class.
 *
 */
class MCL_TEMPLATE_MAP_POSITION extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'MAP_POSITION';
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
    $desc = 'The description for map_position sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'marker_qtl_mtl'      => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the marker, QTL or MTL. Please provide the detailed data in 'Marker', 'QTL', 'MTL' or 'Haplotype_Block' sheet."),
      'locus_name'          => array('req' => FALSE, 'width' => 10, 'desc' => "When a RFLP (eg. AA07) corresponds to more than one locus, the marker name (eg. AA07) is recorded in marker/qtl/mtl column and the specific name for each map position (eg. AA07A, AA07B) is recorded in this 'locus_name' column. Use AA07 as a marker name and the loci names as aliases for the marker name in 'Marker' Sheet. When a MTL is mapped to multiple locations, use the MTL_name in 'MTL' sheet in 'marker/qtl/mtl' column and the locus name of the specific map position in 'locus_name' colum."),
      'map_name'            => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the genetic map."),
      'linkage_group'       => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the linkage group."),
      'linkage_group_order' => array('req' => FALSE, 'width' => 10, 'desc' => "The order of linkage groups for display. Some maps have linkage groups orders of which are not obvious such as G1V, G1G, G2V, etc."),
      'chr_name'            => array('req' => FALSE, 'width' => 10, 'desc' => "Chromosome name for the linkage group when it\'s known (eg. At1, Dt1, etc in cotton)."),
      'chr_number'          => array('req' => FALSE, 'width' => 10, 'desc' => "Chromosome number for the linkage group when it\'s known (eg. chr1, chr26, etc)."),
      'chromosome_arm'      => array('req' => FALSE, 'width' => 10, 'desc' => "Name of the chromosome_arm for fish map."),
      'bin_name'            => array('req' => FALSE, 'width' => 10, 'desc' => "Name of the bin if the locus is belongs to a bin. If a molecular marker belongs to multiple bins, list all of them in separate rows."),
      'start'               => array('req' => FALSE, 'width' => 10, 'desc' => "The position of the locus (cM) or start position of the QTL."),
      'stop'                => array('req' => FALSE, 'width' => 10, 'desc' => "The end position of the QTL or marker when a marker corresponds to a region in a genome."),
      'qtl_peak'            => array('req' => FALSE, 'width' => 10, 'desc' => "Center position or the most likely position of the QTL."),
      'comments'            => array('req' => FALSE, 'width' => 10, 'desc' => "Any comments for the map position."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['bin']                          = -1;
    $cvterms['SITE_CV']['start']                        = -1;
    $cvterms['SITE_CV']['stop']                         = -1;
    $cvterms['SITE_CV']['qtl_peak']                     = -1;
    $cvterms['SITE_CV']['comments']                     = -1;
    $cvterms['SITE_CV']['chr_name']                     = -1;
    $cvterms['SITE_CV']['chr_number']                   = -1;
    $cvterms['SITE_CV']['marker_locus']                 = -1;
    $cvterms['sequence']['genetic_marker']              = -1;
    $cvterms['sequence']['qtl']                         = -1;
    $cvterms['sequence']['heritable_phenotypic_marker'] = -1;
    $cvterms['sequence']['haplotype_block']             = -1;
    $cvterms['sequence']['linkage_group']               = -1;
    $cvterms['sequence']['chromosome_arm']              = -1;
    $cvterms['relationship']['contained_in']            = -1;
    $cvterms['relationship']['instance_of']             = -1;
    $cvterms['relationship']['adjacent_to']             = -1;
    $cvterms['relationship']['located_in']              = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks map.
    MCL_CHADO_FEATUREMAP::checkMap($this, $line['map_name']);

    // Gets the feature type.
    $type = $this->_getFeatureType($this, $line['marker_qtl_mtl']);

    // Checks all positions.
    $this->_checkMapPositon($line, $type);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Skip if there is no position.
    if (!$this->_hasLocation($line)) {
      return;
    }

    // Gets the feature.
    $feature = $this->_getFeature($line['marker_qtl_mtl']);
    if (!$feature) {
      return;
    }

    // Updates $organism_id and $uniquename.
    $uniquename = $feature->getUniquename();
    $organism_id = $feature->getOrganismID();

    // Adds a locus if this is a genetic marker. If locus name is empty,
    // use the name under marker_qtl_mtl column
    $locus = NULL;
    if ($feature->getTypeID() == $this->cvterms['sequence']['genetic_marker']) {
      $locus_name = ($line['locus_name']) ? $line['locus_name'] : $uniquename;

      // Adds suffix as start location.
      $locus_arr = $feature->addLocus($this, $locus_name, $this->cvterms, '', $start);
      $locus = array_shift($locus_arr);
    }

    // Gets the map.
    $map = MCL_CHADO_FEATUREMAP::byKey(array('name' => $line['map_name']));

    // Adds a linkage group.
    $linkage_group = $line['linkage_group'];

    // Gets organism ID for a linkage group.
    $organism_id_lg = MCL_DUMMY_VAR::getIdByName('ORGANISM_ID');

    // Adds a linkage group.
    $lg_uniquename = $map->getName() . '.' . $linkage_group;
    $linkage_group = MCL_CHADO_FEATURE::addFeature($this, $lg_uniquename, $linkage_group, $this->cvterms['sequence']['linkage_group'], $organism_id_lg);

    // Adds properties.
    $linkage_group->addProp($this, 'SITE_CV', 'chr_name', $line['chr_name']);
    $linkage_group->addProp($this, 'SITE_CV', 'chr_number', $line['chr_number']);

    // Adds a chromosome arm.
    $chromosome_arm_name = $line['chromosome_arm'];
    if ($chromosome_arm_name) {
      $chromosome_arm = MCL_CHADO_FEATURE::addFeature($this, $chromosome_arm_name, $chromosome_arm_name, $this->cvterms['sequence']['chromosome_arm'], $organism_id);
      if ($chromosome_arm) {

        // Adds a relationship.
        $feature->addRelatedFeature($this, $chromosome_arm, $this->cvterms['relationship']['contained_in']);
      }
    }

    // Adds a bin.
    $bin_name = $line['bin_name'];
    if ($bin_name) {
      $bin_uniquename = $map->getName() . '.' . $bin_name;
      $bin = MCL_CHADO_FEATURE::addFeature($this, $bin_uniquename, '', $this->cvterms['SITE_CV']['bin'], $organism_id_lg);
      if ($bin) {

        // Adds a relationship.
        if ($locus) {
          $locus->addRelatedFeature($this, $bin, $this->cvterms['relationship']['contained_in']);
        }
        else {
          $feature->addRelatedFeature($this, $bin, $this->cvterms['relationship']['contained_in']);
        }
      }
    }

    // Adds a featurepos.
    if ($linkage_group) {

      // Sets the target feature.
      $target_feature = ($locus) ? $locus : $feature;

      // Adds a featurepos.
      $featurepos = MCL_CHADO_FEATUREPOS::addFeaturepos($this, $target_feature, $map, $linkage_group);
      if ($featurepos) {

        // Adds properties.
        $featurepos->addProp($this, 'SITE_CV', 'start', $line['start']);
        $featurepos->addProp($this, 'SITE_CV', 'stop', $line['stop']);
        $featurepos->addProp($this, 'SITE_CV', 'qtl_peak', $line['qtl_peak']);
        $featurepos->addProp($this, 'SITE_CV', 'comments', $line['comments']);

        // Adds a featurepos for the bin if exists.
        $data = trim($start.$stop);
        if ($data && $linkage_group && $bin) {

          // Adds a featurepos.
          $featurepos = MCL_CHADO_FEATUREPOS::addFeaturepos($this, $bin, $map, $linkage_group);
          if ($featurepos) {

            // Adds properties.
            $featurepos->addProp($this, 'SITE_CV', 'start', $line['start']);
            $featurepos->addProp($this, 'SITE_CV', 'stop', $line['stop']);
          }
        }
      }
    }
  }

  /**
   * Checks if it has the location data.
   *
   * @param array $line
   *
   * @return boolean
   */
  private function _hasLocation($line) {

    // Skip if there is no position.
    $start    = $line['start'];
    $stop     = $line['stop'];
    $qtl_peak = $line['qtl_peak'];
    $data     = trim($start.$stop.$qtl_peak);
    if (!($data || $data == '0')) {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Gets the feature.
   *
   * @param string $uniquename
   *
   * @return MCL_CHADO_FEATURE
   */
  private function _getFeature($uniquename) {

    // Checks the uniquename for Haplotype Block.
    $feature = MCL_CHADO_FEATURE::getFeature($uniquename, '', '', $this->cvterms['sequence']['haplotype_block']);
    if ($feature) {
      return $feature;
    }

    // Checks the uniquename for MTL.
    $feature = MCL_CHADO_FEATURE::getFeature($uniquename, '', '', $this->cvterms['sequence']['heritable_phenotypic_marker']);
    if ($feature) {
      return $feature;
    }

    // Checks the uniquename for QTL.
    $feature = MCL_CHADO_FEATURE::getFeature($uniquename, '', '', $this->cvterms['sequence']['qtl']);
    if ($feature) {
      return $feature;
    }

    // Checks the uniquename for marker.
    $feature = MCL_CHADO_FEATURE::getFeatureAlias($uniquename, '', '', $this->cvterms['sequence']['genetic_marker']);
    if ($feature) {
      return $feature;
    }
    return NULL;
  }

  /**
   * Gets the type of feature.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $uniquename
   *
   * @return string
   *   The type of the feature.
   */
  private function _getFeatureType(MCL_TEMPLATE $mcl_tmpl = NULL, $uniquename) {

    // Checks the uniquename for Haplotype Block.
    $hb = MCL_CHADO_FEATURE::getFeature($uniquename, '', '', $this->cvterms['sequence']['haplotype_block']);
    if ($hb) {
      return 'haplotype_block';
    }

    // Checks the uniquename for MTL.
    $mtl = MCL_CHADO_FEATURE::getFeature($uniquename, '', '', $this->cvterms['sequence']['heritable_phenotypic_marker']);
    if ($mtl) {
      return 'mtl';
    }

    // Checks the uniquename for QTL.
    $qtl = MCL_CHADO_FEATURE::getFeature($uniquename, '', '', $this->cvterms['sequence']['qtl']);
    if ($qtl) {
      return 'qtl';
    }

    // Checks the uniquename for marker.
    $marker = MCL_CHADO_FEATURE::checkFeatureAlias($mcl_tmpl, $uniquename, '', '', $this->cvterms['sequence']['genetic_marker']);
    if ($marker) {
      return 'marker';
    }

    // Logs the error message.
    $mcl_tmpl->updateMsg('E', "Error - $uniquename is not 'Genetic marker', 'QTL', 'MTL' or 'Haplotype Block'");
    return '';
  }

  /**
   * Checks the map positions for Genetic marker, QTL and MTL.
   *
   * [QTL]
   * QTL can have 'qtl_peack' and/or 'start and stop' positions.
   * Throws an error if it finds any of the followings
   *   - only start or stop exists
   *   - both start and stop exist but they holds the same value (It should be 'qtl_peack').
   *   - start > stop if both exist.
   *   - qtl_peak out of the range (start - stop) if start, stop and qtl_peak exist.
   *
   * [MTL]
   * MTL can only have 'start' position.
   * Throws an error if it finds any of the followings
   *   - qtl_peak value exists
   *   - stop exists
   *
   * [Genetic marker]
   * Genetic marker can have 'start' and 'stop' positions or 'start' only.
   * Throws an error if it finds any of the followings
   *   - qtl_peak value exists
   *   - only stop exists
   *
   * [Haplotype Block]
   * Haplotype Block can have 'start' and 'stop' positions only.
   * Throws an error if it finds any of the followings
   *   - qtl_peak value exists
   *   - only start or stop is provided.
   *
   * @param array $line
   * @param string $type
   *
   * @retrun boolean
   */
  private function _checkMapPositon($line, $type) {

    // Gets the uniquename of the feature.
    $uniquename = $line['marker_qtl_mtl'];

    // Ignores if the type is not provided.
    if (!$type) {
      return TRUE;
    }

    // Ignores if the no locatoin data.
    if (!$this->_hasLocation($line)) {
      return TRUE;
    }

    // Checks the value of positions. The values must be numeric.
    $flag_start = FALSE;
    if ($line['start'] || $line['start'] == '0') {
      $flag_start = TRUE;
      if (!is_numeric($line['start'])) {
        $this->updateMsg('E', "start position '" . $line['start'] . "' is not number");
      }
    }
    $flag_stop = FALSE;
    if ($line['stop'] || $line['stop'] == '0') {
      $flag_stop = TRUE;
      if (!is_numeric($line['stop'])) {
        $this->updateMsg('E', "stop position '" . $line['stop'] . "' is not number");
      }
    }
    $flag_qtl = FALSE;
    if ($line['qtl_peak'] || $line['qtl_peak'] == '0') {
      $flag_qtl = TRUE;
      if (!is_numeric($line['qtl_peak'])) {
        $this->updateMsg('E', "qtl_peak position '" . $line['qtl_peak']  . "' is not number");
      }
    }

    // Checks if start positon is bigger than stop.
    if ($flag_start && $flag_stop) {
      if ($line['start'] > $line['stop']) {
        $this->updateMsg('E', 'The start position is bigger than the stop');
      }
    }

    // QTL.
    if ($type == 'qtl') {

      // Only start exists.
      if ($flag_start && !$flag_stop) {
        $this->updateMsg('E', 'Only start position exists. Please add stop position');
      }

      // Only stop exists.
      else if (!$flag_start && $flag_stop) {
        $this->updateMsg('E', 'Only stop position exists. Please add start position');
      }

      // Both start and stop positions exist.
      else if ($flag_start && $flag_stop) {

        // If the position of both has the same value, need to move to 'qtl_peak'.
        if ($line['start'] == $line['stop']) {
          $this->updateMsg('E', 'The positions of start and stop are the same. Please use qtl_peak instead');
        }

        // Check if qtl_peak is in the range.
        if ($flag_qtl) {
          if ($line['start'] > $line['qtl_peak'] || $line['stop'] < $line['qtl_peak']) {
            $this->updateMsg('E', "qtl_peak '" . $line['qtl_peak'] . "' must be between start and stop");
          }
        }
      }
    }

    // Marker.
    else if ($type == 'marker') {

      // qtl_peak value exists.
      if ($flag_qtl) {
        $this->updateMsg('E', 'Marker cannot have qtl_peak');
      }

      // Only stop position exists.
      if (!$flag_start && $flag_stop) {
        $this->updateMsg('E', 'Only stop position exists. Please add start position');
      }
    }

    // MTL.
    else if ($type == 'mtl') {

      // qtl_peak value exists.
      if ($flag_qtl) {
        $this->updateMsg('E', 'MTL cannot have qtl_peak');
      }

      // stop position exists.
      if ($flag_stop) {
        $this->updateMsg('E', 'stop position exists. Please remove the stop position');
      }
    }

    // Haplotype Block.
    else if ($type == 'haplotype_block') {

      // qtl_peak value exists.
      if ($flag_qtl) {
        $this->updateMsg('E', 'Haplotype Block cannot have qtl_peak');
      }

      // only start or stop is provided.
      if (!($flag_start && $flag_stop)) {
        $this->updateMsg('E', 'Both start and stop positions must be provided');
      }
    }
    return TRUE;
  }

}