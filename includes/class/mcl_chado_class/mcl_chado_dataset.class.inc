<?php
/**
 * The declaration of MCL_CHADO_DATASET class.
 *
 */
class MCL_CHADO_DATASET extends CHADO_PROJECT {

 /**
  *  Class data members.
  */

  /**
   * @see CHADO_PROJECT::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_PROJECT::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * Returns MCL_CHADO_DATASET by ID.
   *
   * @param integer $project_id
   *
   * @return MCL_CHADO_DATASET
   */
  public static function byID($project_id) {
    return MCL_CHADO_DATASET::byKey(array('project_id' => $project_id));
  }

  /**
   * Returns the dataset by name.
   *
   * @param string $name
   *
   * @return MCL_CHADO_DATASET
   */
  public static function byName($name) {
    return MCL_CHADO_DATASET::byKey(array('name' => $name));
  }

  /**
   * @see CHADO_PROJECT::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * @see CHADO_CONTACT::delete()
   */
  public function delete() {

    // Gets the type and sub_type of the project.
    $type     = strtolower($this->getType());
    $sub_type = strtolower($this->getSubType());

    // Type : phenotype - breeding.
    $args = array(':project_id' => $this->getProjectID());
    if (preg_match("/^phenotyp/", $type) && $sub_type == 'breeding') {

      // Deletes samples from chado.stock.
      $sql = "
        DELETE FROM chado.stock
        WHERE stock_id IN (
          SELECT NES.stock_id
          FROM chado.nd_experiment_stock NES
            INNER JOIN chado.nd_experiment_project NEPRJ on NEPRJ.nd_experiment_id = NES.nd_experiment_id
          WHERE NEPRJ.project_id = :project_id
        )
      ";
      db_query($sql, $args);

      // Deletes data from chado.nd_experiemnt.
      $sql = "
        DELETE FROM chado.nd_experiment
        WHERE nd_experiment_id IN (
          SELECT NEPRJ.nd_experiment_id
          FROM chado.nd_experiment_project NEPRJ
          WHERE NEPRJ.project_id = :project_id
        )
      ";
      db_query($sql, $args);
    }

    // Type : genotype - haplotype.
    else if ($type == 'genotype' && $sub_type == 'haplotype') {

      // Deletes haplotypes from chado.feature.
      $sql = "
        SET SEARCH_PATH TO public, chado;
        DELETE FROM chado.feature
        WHERE feature_id IN (
          SELECT H.feature_id
          FROM chado.feature H
            INNER JOIN chado.cvterm HC on HC.cvterm_id = H.type_id
            INNER JOIN chado.feature_relationship FR_HB_H on FR_HB_H.subject_id = H.feature_id
            INNER JOIN chado.cvterm FR_HB_HC on FR_HB_HC.cvterm_id = FR_HB_H.type_id
            INNER JOIN chado.feature HB on HB.feature_id = FR_HB_H.object_id
            INNER JOIN chado.cvterm HBC on HBC.cvterm_id = HB.type_id
            INNER JOIN chado.feature_genotype FG on FG.feature_id = HB.feature_id
            INNER JOIN chado.nd_experiment_genotype NEG on NEG.genotype_id = FG.genotype_id
            INNER JOIN chado.nd_experiment_project NEPRJ on NEPRJ.nd_experiment_id = NEG.nd_experiment_id
          WHERE HC.name = :haplotype AND HBC.name = :haplotype_block
            AND FR_HB_HC.name = :variant_of AND NEPRJ.project_id = :project_id
        )
      ";
      $args_1 = array(
        ':haplotype'        => 'haplotype',
        ':haplotype_block'  => 'haplotype_block',
        ':project_id'       => $this->getProjectID(),
        ':variant_of'       => 'variant_of',
      );
      db_query($sql, $args_1);

      // Deletes data from chado.nd_experiment.
      $sql = "
        DELETE FROM chado.nd_experiment
        WHERE nd_experiment_id IN (
          SELECT NEPRJ.nd_experiment_id FROM chado.nd_experiment_project NEPRJ
          WHERE NEPRJ.project_id = :project_id
        )
      ";
      db_query($sql, $args);
    }

    // Type : genotype - SSR.
    else if ($type == 'genotype' && $sub_type == 'ssr') {

      // Deletes samples from chado.stock.
      $sql = "
        DELETE FROM chado.stock
        WHERE stock_id IN (
          SELECT NES.stock_id
          FROM chado.nd_experiment_stock NES
            INNER JOIN chado.nd_experiment_genotype NEG on NES.genotype_id = NES.genotype_id
            INNER JOIN chado.nd_experiment_project NEPRJ on NEPRJ.nd_experiment_id = NES.nd_experiment_id
          WHERE NEPRJ.project_id = :project_id
        )
      ";
      db_query($sql, $args);

      // Deletes data from chado.nd_experiemnt.
      $sql = "
        DELETE FROM chado.nd_experiment
        WHERE nd_experiment_id IN (
          SELECT NEPRJ.nd_experiment_id
          FROM chado.nd_experiment_project NEPRJ
          WHERE NEPRJ.project_id = :project_id
        )
      ";
      db_query($sql, $args);
    }
    return parent::delete();
  }

  /**
   * Checks the existence of dataset. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $name
   *
   * @return boolean
   */
  public static function checkDataset(MCL_TEMPLATE $mcl_tmpl = NULL, $name) {
    if ($name) {

      // Gets the dataset.
      $dataset = MCL_CHADO_DATASET::byName($name);
      if (!$dataset) {
        self::updateMsg($mcl_tmpl, 'E', "(name) = ($name) not found in project");
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Checks the existence of dataset by ID. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $project_id
   *
   * @return boolean
   */
  public static function checkDatasetByID(MCL_TEMPLATE $mcl_tmpl = NULL, $project_id) {
    if ($project_id) {

      // Gets the dataset.
      $dataset = MCL_CHADO_DATASET::byKey(array('project_id' => $project_id));
      if (!$dataset) {
        self::updateMsg($mcl_tmpl, 'E', "(project_id) = ($project_id) not found in project");
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Returns cv if this project is phenotype.
   *
   * @return MCL_CHADO_CV
   */
  public function getCv() {
    $mcl_chado_cv = NULL;
    if (preg_match("/^(breeding|phenotype)$/i", $this->getType())) {
      $sql = "
        SELECT C.cv_id
        FROM {chado.cvterm} C
          INNER JOIN {chado.phenotype} P on P.attr_id = C.cvterm_id
          INNER JOIN {chado.nd_experiment_phenotype} NEP on NEP.phenotype_id = P.phenotype_id
          INNER JOIN {chado.nd_experiment_project} NEPR on NEPR.nd_experiment_id = NEP.nd_experiment_id
        WHERE NEPR.project_id = :project_id
        LIMIT 1
      ";
      $args = array(':project_id' => $this->project_id);
      $results = db_query($sql, $args);
      $cv_id = $results->fetchField();
      if ($cv_id) {
        return MCL_CHADO_CV::byID($cv_id);
      }
    }
    return $mcl_chado_cv;
  }

  /**
   * Returns the type of the project.
   *
   * @param boolean $lower
   *
   * @return string
   */
  public function getType($lower = FALSE) {
    $type = $this->getProp('SITE_CV', 'project_type');
    return $lower ? strtolower($type) : $type;
  }

  /**
   * Returns the sub_type of the project.
   *
   * @param boolean $lower
   *
   * @return string
   */
  public function getSubType($lower = FALSE) {
    $sub_type = $this->getProp('SITE_CV', 'sub_type');
    return $lower ? strtolower($sub_type) : $sub_type;
  }

  /**
   * Returns property.
   *
   * @param string $cv_name
   * @param string $cvterm_name
   *
   * @return string
   */
  public function getProp($cv_name, $cvterm_name) {
    $type_id = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name)->getCvtermID();
    return $this->getProperty('projectprop', 'project_id', $this->project_id, $type_id);
  }

  /**
   * Adds a dataset.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $uniquename
   * @param integer $type_id
   * @param integer $organism_id
   * @param string $name
   *
   * @return MCL_CHADO_DATASET
   */
  public static function addDataset(MCL_TEMPLATE $mcl_tmpl = NULL, $name) {

    // Sets the arguments.
    $args = array('name' => $name);

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $dataset = MCL_CHADO_DATASET::byKey($args);
    if ($dataset) {
      self::addMsg($mcl_tmpl, 'D', 'project', $args);
    }
    else {

      // Adds a new dataset.
      $dataset = new MCL_CHADO_DATASET($args);
      if ($dataset->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'project', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'project', $args);
        return NULL;
      }
    }
    return $dataset;
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
      return $this->addProperty($mcl_tmpl, 'projectprop', 'project_id', $this->project_id, $type_id, $value, $separator);
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
      $cvterm = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name);
      if (!$cvterm) {
        return FALSE;
      }
      $type_id = $cvterm->getCvtermID();
      return $this->addProperty($mcl_tmpl, 'projectprop', 'project_id', $this->project_id, $type_id, $value, $separator);
    }
    return TRUE;
   }

  /**
   * Adds contact to project_contact.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $contact
   * @param string $separator
   *
   * @return boolean
   */
  public function addContact(MCL_TEMPLATE $mcl_tmpl = NULL, $contact, $separator = '') {
    $flag = TRUE;
    if ($contact) {
      $names = preg_split($this->getSepRegex($separator), $contact, -1, PREG_SPLIT_NO_EMPTY);
      foreach ($names as $name) {
        $name = trim($name);
        $contact = MCL_CHADO_CONTACT::byKey(array('name' => $name));
        if ($contact) {
          if (!$this->addLink($mcl_tmpl, 'project_contact', 'project_id', $this->project_id, 'contact_id', $contact->getContactID())) {
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
   * Adds a perent dataset.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $dataset_name
   *
   * @return boolean
   */
  public function addParentDataset(MCL_TEMPLATE $mcl_tmpl = NULL, $dataset_name) {
    if ($dataset_name) {
      $parent = MCL_CHADO_DATASET::byName($dataset_name);
      if ($parent) {
        $type_id = MCL_CHADO_CVTERM::getCvterm('SITE_CV', 'is_a_subproject_of')->getCvtermID();
        return $this->addLink($mcl_tmpl, 'project_relationship', 'subject_project_id', $this->project_id, 'object_project_id', $parent->getProjectID(), $type_id);
      }
    }
    return TRUE;
  }

  /**
   * Adds reference to project_pub.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $reference
   * @param string $separator
   *
   * @return boolean
   */
  public function addReference(MCL_TEMPLATE $mcl_tmpl = NULL, $reference, $separator = '') {

    $flag = TRUE;
    if ($reference) {
      $pub_ids = preg_split($this->getSepRegex($separator), $reference, -1, PREG_SPLIT_NO_EMPTY);
      foreach ($pub_ids as $pub_id) {
        $pub_id = trim($pub_id);
        $pub = MCL_CHADO_PUB::byID($pub_id);
        if ($pub) {
          if ($this->addLink($mcl_tmpl, 'project_pub', 'project_id', $this->project_id, 'pub_id', $pub_id)) {
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
}