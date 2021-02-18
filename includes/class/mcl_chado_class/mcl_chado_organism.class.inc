<?php
/**
 * The declaration of MCL_CHADO_ORGANISM class.
 *
 */
class MCL_CHADO_ORGANISM extends CHADO_ORGANISM {

 /**
  *  Class data members.
  */

  /**
   * @see CHADO_ORGANISM::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_ORGANISM::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * Returns MCL_CHADO_ORGANISM by ID.
   *
   * @param integer $organism_id
   *
   * @return MCL_CHADO_ORGANISM
   */
  public static function byID($organism_id) {
    return MCL_CHADO_ORGANISM::byKey(array('organism_id' => $organism_id));
  }

  /**
   * @see CHADO_ORGANISM::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * @see CHADO_ORGANISM::delete()
   */
  public function delete() {
    return parent::delete();
  }

  /**
   * Returns all organism_id for the given crop.
   *
   * @param integer $crop_id
   *
   * @return array of organism objects
   */
  public static function getOrganismsByCropID($crop_id) {

    // Gets all orgainsm for the selected crop.
    $crop_organism_ids = MCL_CROP::getOrganismIDs($crop_id);
    $organism_arr = array();
    foreach ($crop_organism_ids as $organism_id) {
      $organism = CHADO_ORGANISM::byKey(array('organism_id' => $organism_id));
      $organism_arr[$organism->getOrganismID()] = $organism->getGenus() . ' ' . $organism->GetSpecies();
    }
    return $organism_arr;
  }

  /**
   * Checks the existence of orgainsm. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $genus
   * @param string $species
   * @param string $separator
   * @param boolean $warning
   *
   * @return boolean
   */
  public static function checkOrganism(MCL_TEMPLATE $mcl_tmpl = NULL, $genus, $species, $separator = '', $warning = FALSE) {
    $flag = TRUE;
    if ($genus xor $species) {
      self::updateMsg($mcl_tmpl, 'E', "Either of genus or species must not be an empty.");
      $flag = FALSE;
    }
    else if ($genus && $species) {

      $sps = preg_split(self::getSepRegex($separator), $species, -1, PREG_SPLIT_NO_EMPTY);
      foreach ($sps as $sp) {
        $sp = trim($sp);

        // Gets the orgainsm.
        $orgainsm = MCL_CHADO_ORGANISM::getOrganism($genus, $sp);
        if (!$orgainsm) {
          if ($warning) {
            self::updateMsg($mcl_tmpl, 'W', "(genus, species) = ($genus, $sp) not found in organism");
          }
          else {
            self::updateMsg($mcl_tmpl, 'E', "(genus, species) = ($genus, $sp) not found in organism");
            $flag = FALSE;
          }
        }
      }
    }
    return $flag;
  }

  /**
   * Checks the existence of the orgainsms in the list. Semi-colons separte
   * orgainsms and colons separates genus and species.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $orgainsm_list
   *
   * @return boolean
   */
  public static function checkOrganismList(MCL_TEMPLATE $mcl_tmpl = NULL, $orgainsm_list) {
    $flag = TRUE;
    if ($orgainsm_list) {
      $orgainsms = preg_split('/;/', $orgainsm_list, -1, PREG_SPLIT_NO_EMPTY);
      foreach ($orgainsms as $orgainsm) {
        $tmp = explode(':', $orgainsm);
        if (!MCL_CHADO_ORGANISM::checkOrganism($tmp[0], $tmp[1])) {
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Checks the existence of the organisms. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $organism_id_list
   * @param string $separator
   *
   * @return boolean
   */
  public static function checkOrganismID(MCL_TEMPLATE $mcl_tmpl = NULL, $organism_id_list, $separator = '') {
    $flag = TRUE;
    if ($organism_id_list) {

      // Gets organism_id.
      $organism_ids = preg_split(self::getSepRegex($separator), $organism_id_list, -1, PREG_SPLIT_NO_EMPTY);
      foreach ($organism_ids as $organism_id) {
        $organism_id = trim($organism_id);
        if (!preg_match("/^\d+$/", $organism_id)) {
          self::updateMsg($mcl_tmpl, 'E', "$organism_id is not an integer. It must be organism_id");
          $flag = FALSE;
          continue;
        }
        $args = array('organism_id' => $organism_id);
        $organism = MCL_CHADO_ORGANISM::byID($organism_id);
        if (!$organism) {
          self::addMsg($mcl_tmpl, 'E', 'organism', $args);
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Returns the orgainsm by genus and species.
   *
   * @param string $genus
   * @param string $species
   *
   * @return MCL_CHADO_ORGANISM
   */
  public static function getOrganism($genus, $species) {
    $args = array(
      'genus'   => $genus,
      'species' => $species,
    );
    return MCL_CHADO_ORGANISM::byKey($args);
  }

  /**
   * Adds a orgainsm.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $genus
   * @param string $species
   *
   * @return MCL_CHADO_ORGANISM
   */
  public static function addOrganism(MCL_TEMPLATE $mcl_tmpl = NULL, $genus, $species) {

    // Sets the arguments.
    $args = array(
      'genus'   => $genus,
      'species' => $species,
    );

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $orgainsm = MCL_CHADO_ORGANISM::byKey($args);
    if ($orgainsm) {
      self::addMsg($mcl_tmpl, 'D', 'orgainsm', $args);
    }
    else {

      // Adds a new orgainsm.
      $args['type_id']      = $type_id;
      $args['description']  = $description;
      $orgainsm = new MCL_CHADO_ORGANISM($args);
      if ($orgainsm->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'orgainsm', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'orgainsm', $args);
        return NULL;
      }
    }
    return $orgainsm;
  }

  /**
   * Adds N/A organism.
   *
   * @return MCL_CHADO_ORGANISM
   */
  public static function addNA() {

    // Checks if it already exists.
    $organism = MCL_CHADO_ORGANISM::getOrganism('N/A', 'N/A');
    if (!$organism) {
      $details = array(
        'genus'   => 'N/A',
        'species' => 'N/A',
      );
      $organism = new MCL_CHADO_ORGANISM($details);
      if (!$organism->insert()) {
        return NULL;
      }
    }
    return $organism;
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
      return $this->addProperty($mcl_tmpl, 'organismprop', 'organism_id', $this->organism_id, $type_id, $value, $separator);
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
      return $this->addProperty($mcl_tmpl, 'organismprop', 'organism_id', $this->organism_id, $type_id, $value, $separator);
    }
    return TRUE;
  }

  /**
   * Adds a related organism.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_ORGANISM $organism
   * @param integer $type_id_relationship
   *
   * @return boolean
   */
  public function addRelatedOrganism(MCL_TEMPLATE $mcl_tmpl = NULL, MCL_CHADO_ORGANISM $organism, $type_id_relationship) {
    if ($organism) {
      return $this->addRelationship($mcl_tmpl, 'organism_relationship', 'subject_organism_id', $this->organism_id, 'object_organism_id', $organism->getOrganismID(), $type_id_relationship);
    }
    return TRUE;
  }

  /**
   * Adds the related organism by their organism IDs.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $organism_id_list
   * @param integer $type_id_relationship
   * @param string $separator
   *
   * @return boolean
   */
  public function addRelatedOrganismsByID(MCL_TEMPLATE $mcl_tmpl = NULL, $organism_id_list, $type_id_relationship, $separator = '') {
    if ($organism_id_list) {

      // Gets organism_id.
      $organism_ids = preg_split(self::getSepRegex($separator), $organism_id_list, -1, PREG_SPLIT_NO_EMPTY);
      foreach ($organism_ids as $organism_id) {
        $organism_id = trim($organism_id);
        if (!$this->addRelationship($mcl_tmpl, 'organism_relationship', 'subject_organism_id', $this->organism_id, 'object_organism_id', $organism_id, $type_id_relationship)) {
          return FALSE;
        }
      }
    }
    return TRUE;
  }

  /**
   * Adds the related organisms in the list. Semi-colons separte orgainsms and
   * colons separates genus and species.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $organism_list
   * @param integer $type_id_relationship
   *
   * @return boolean
   */
  public function addRelatedOrganismsInList(MCL_TEMPLATE $mcl_tmpl = NULL, $organism_list, $type_id_relationship) {
    if ($organism_list) {
      $orgainsms = preg_split('/;/', $organism_list, -1, PREG_SPLIT_NO_EMPTY);
      foreach ($orgainsms as $orgainsm) {
        $tmp = explode(':', $orgainsm);
        $genus    = trim($tmp[0]);
        $species  = trim($tmp[1]);
        $org = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
        if ($org) {
          if (!$this->addRelationship($mcl_tmpl, 'organism_relationship', 'subject_organism_id', $this->organism_id, 'object_organism_id', $org->getOrganismID(), $type_id_relationship)) {
            return FALSE;
          }
        }
        else {
          if ($mcl_tmpl) {

            // Saves the organisms that could not find in stock organism.
            // They may be listed in the later row of the same Excel file.
            // Adds them after processing the organism sheet.
            $missing_organism = array(
              'object_organism_id'  => $this->organism_id,
              'genus'               => $genus,
              'species'             => $species,
              'type_id_rel'         => $type_id_relationship,
            );
            $GLOBALS['organisms'][] = $missing_organism;
          }
          else {
            return FALSE;
          }
        }
      }
    }
    return TRUE;
  }

  /**
   * Adds a dbxref to organism_dbxref.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_DBXREF $dbxref
   *
   * @return boolean
   */
  public function addDBXref(MCL_TEMPLATE $mcl_tmpl = NULL, MCL_CHADO_DBXREF $dbxref) {
    return $this->addLink($mcl_tmpl, 'organism_dbxref', 'organism_id', $this->organism_id, 'dbxref_id', $dbxref->getDbxrefID());
  }

  /**
   * Adds image to organism_image.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $image_id_list
   * @param string $separator
   *
   * @return boolean
   */
  public function addImageID(MCL_TEMPLATE $mcl_tmpl = NULL, $image_id_list, $separator = '') {
    $flag = TRUE;
    if ($image_id_list) {
      $eimage_ids = preg_split($this->getSepRegex($separator), $image_id_list, -1, PREG_SPLIT_NO_EMPTY);
      foreach ($eimage_ids as $eimage_id) {
        $eimage_id = trim($eimage_id);
        $image = MCL_CHADO_IMAGE::byKey(array('eimage_id' => $eimage_id));
        if ($image) {
          if (!$this->addLink($mcl_tmpl, 'organism_image', 'organism_id', $this->organism_id, 'eimage_id', $image->getEimageID())) {
            $flag = FALSE;
          }
        }
        else {
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Adds reference to organism_pub.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $reference
   * @param string $separator
   *
   * @return boolean
   */
  public function addReference(MCL_TEMPLATE $mcl_tmpl = NULL, $reference, $separator = '') {
    $flag = TRUE;
    if ($reference != '') {
      $pub_ids = preg_split($this->getSepRegex($separator), $reference, -1, PREG_SPLIT_NO_EMPTY);
      foreach ($pub_ids as $pub_id) {
        $pub_id = trim($pub_id);
        $pub = MCL_CHADO_PUB::byID($pub_id);
        if ($pub) {
          if (!$this->addLink($mcl_tmpl, 'organism_pub', 'organism_id', $this->organism_id, 'pub_id', $pub_id)) {
            $flag = FALSE;
          }
        }
        else {
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * @see CHADO_TABLE::getMergeUniqueKeys()
   */
  public function getMergeUniqueKeys($chado_table) {
    return array();
  }

  /**
   * Merges the organisms.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_ORGANISM $organism
   * @param boolean $verbose
   */
  public function mergeOrganism(MCL_TEMPLATE $mcl_tmpl = NULL, MCL_CHADO_ORGANISM $organism, $verbose = FALSE) {

    // Sets the information of the organisms.
    $params = array(
      'label'   => 'ORGANISM',
      'id_base' => $this->organism_id,
      'id'      => $organism->getOrganismID(),
    );

    // Gets all reference tables.
    $ref_tables = MCL_CHADO_ORGANISM::getRelTable();
    foreach ((array)$ref_tables['organism_id'] as $ref_table) {

      // Processes the merge.
      if (!$this->procMerge($mcl_tmpl, $params, $ref_table, $verbose)) {
        break;
      }
    }
  }
}