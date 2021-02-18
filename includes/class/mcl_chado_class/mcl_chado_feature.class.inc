<?php
/**
 * The declaration of MCL_CHADO_FEATURE class.
 *
 */
class MCL_CHADO_FEATURE extends CHADO_FEATURE {

 /**
  *  Class data members.
  */
  /**
   * @see CHADO_FEATURE::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_FEATURE::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * Returns MCL_CHADO_FEATURE by ID.
   *
   * @param integer $feature_id
   *
   * @return MCL_CHADO_FEATURE
   */
  public static function byID($feature_id) {
    return MCL_CHADO_FEATURE::byKey(array('feature_id' => $feature_id));
  }

  /**
   * @see CHADO_FEATURE::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * @see CHADO_FEATURE::delete()
   */
  public function delete() {

    // Deletes the feature by its type.
    $feature_type = strtolower($this->getFeatureType());

    // Marker.
    if ($feature_type == 'genetic_marker') {
    }

    // QLT.
    else if ($feature_type == 'qtl') {
    }

    // MTL.
    else if ($feature_type == 'heritable_phenotypic_marker') {
    }

    // Haplotype.
    else if ($feature_type == 'haplotype') {
    }

    // Haplotype Block.
    else if ($feature_type == 'haplotype_block') {
    }
    return parent::delete();
  }

  /**
   * Checks the existence of feature by ID. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $feature_id
   * @param boolean $warning
   *
   * @return boolean
   */
  public static function checkFeatureID(MCL_TEMPLATE $mcl_tmpl = NULL, $feature_id, $warning = FALSE) {

    // Gets the feature.
    $feature = NULL;
    if (is_int($feature_id)) {
      $feature = MCL_CHADO_FEATURE::byID($feature_id);
    }
    if ($feature) {
      return TRUE;
    }
    if ($warning) {
      self::updateMsg($mcl_tmpl, 'W', "feature ID ($feature_id) not found in feature");
      return TRUE;
    }
    self::updateMsg($mcl_tmpl, 'E', "feature ID ($feature_id) not found in feature");
    return FALSE;
  }

  /**
   * Checks the existence of feature. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $feature
   * @param string $genus
   * @param string $species
   * @param integer $type_id
   * @param string $separator
   * @param boolean $warning
   *
   * @return boolean
   */
  public static function checkFeature(MCL_TEMPLATE $mcl_tmpl = NULL, $feature, $genus, $species, $type_id, $separator = '', $warning = FALSE) {
    $flag = TRUE;

    // Gets the type.
    $type = MCL_CHADO_CVTERM::byID($type_id);
    if (!$type) {
      self::updateMsg($mcl_tmpl, 'E', "$type_id not found in cvterm");
      return FALSE;
    }

    // Checks the organism_id.
    $organism_id = '';
    if ($genus && $species) {
      $organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
      if ($organism) {
        $organism_id  = $organism->getOrganismID();
      }
      else {
        self::updateMsg($mcl_tmpl, 'E', "$genus $species not found in organism");
        return FALSE;
      }
    }

    // Gets the uniquenames.
    $uniquenames = preg_split(self::getSepRegex($separator), $feature, -1, PREG_SPLIT_NO_EMPTY);
    foreach ($uniquenames as $uniquename) {
      $uniquename = trim($uniquename);

      // Sets the arguments.
      $args = array(
        'uniquename'  => $uniquename,
        'type_id'     => $type_id,
      );
      if ($organism_id) {
        $args['organism_id'] = $organism_id;
      }

      // If '::' found in uniquename, update uniquename and organism_id.
      if (preg_match("/^(.*?)::(.*?)$/", $uniquename, $matches)) {
        $uniquename         = trim($matches[2]);
        $parts = preg_split("/\s+/", trim($matches[1]));
        $genus              = $parts[0];
        $species            = $parts[1];
        $infraspecific_type = $parts[2];
        $infraspecific_name = $parts[3];
        $diff_organism = MCL_CHADO_ORGANISM::getOrganismInfra($genus, $species, $infraspecific_type, $infraspecific_name);
        if ($diff_organism) {
          $args['organism_id'] = $diff_organism->getOrganismID();
        }
        else {
          self::updateMsg($mcl_tmpl, 'E', "$genus $species $infraspecific_type $infraspecific_name not found in organism");
          $flag = FALSE;
        }
      }

      // Checks the feature.
      $num = MCL_CHADO_FEATURE::countFeature($mcl_tmpl, $uniquename, $genus, $species, $type_id);
      if ($num > 1) {
        self::updateMsg($mcl_tmpl, 'E', "The uniquename '$uniquename' is ambiguous. Please not use this uniquename");
        $flag = FALSE;
      }
      else if ($num < 1) {
        $feature_type = $type->getName();
        if ($warning) {
          self::updateMsg($mcl_tmpl, 'W', "The uniquename '$uniquename'  ($feature_type) does not exist in chado.feature");
        }
        else {
          self::updateMsg($mcl_tmpl, 'E', "The uniquename '$uniquename'  ($feature_type) does not exist in chado.feature");
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Checks the existence of feature. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $feature
   * @param string $genus
   * @param string $species
   * @param string $infraspecific_type
   * @param string $infraspecific_name
   * @param integer $type_id
   * @param string $separator
   * @param boolean $warning
   *
   * @return boolean
   */
  public static function checkFeatureInfra(MCL_TEMPLATE $mcl_tmpl = NULL, $feature, $genus, $species, $infraspecific_type, $infraspecific_name, $type_id, $separator = '', $warning = FALSE) {
    $flag = TRUE;

    // Gets the type.
    $type = MCL_CHADO_CVTERM::byID($type_id);
    if (!$type) {
      self::updateMsg($mcl_tmpl, 'E', "$type_id not found in cvterm");
      return FALSE;
    }

    // Checks the organism_id.
    $organism_id = '';
    if ($genus && $species) {
      $organism = MCL_CHADO_ORGANISM::getOrganismInfra($genus, $species, $infraspecific_type, $infraspecific_name);
      if ($organism) {
        $organism_id  = $organism->getOrganismID();
      }
      else {
        self::updateMsg($mcl_tmpl, 'E', "$genus $species $infraspecific_type $infraspecific_name not found in organism");
        return FALSE;
      }
    }

    // Gets the uniquenames.
    $uniquenames = preg_split(self::getSepRegex($separator), $feature, -1, PREG_SPLIT_NO_EMPTY);
    foreach ($uniquenames as $uniquename) {
      $uniquename = trim($uniquename);

      // Sets the arguments.
      $args = array(
        'uniquename'  => $uniquename,
        'type_id'     => $type_id,
      );
      if ($organism_id) {
        $args['organism_id'] = $organism_id;
      }

      // If '::' found in uniquename, update uniquename and organism_id.
      if (preg_match("/^(.*?)::(.*?)$/", $uniquename, $matches)) {
        $uniquename         = trim($matches[2]);
        $parts = preg_split("/\s+/", trim($matches[1]));
        $genus              = $parts[0];
        $species            = $parts[1];
        $infraspecific_type = $parts[2];
        $infraspecific_name = $parts[3];
        $diff_organism = MCL_CHADO_ORGANISM::getOrganismInfra($genus, $species, $infraspecific_type, $infraspecific_name);
        if ($diff_organism) {
          $args['organism_id'] = $diff_organism->getOrganismID();
        }
        else {
          self::updateMsg($mcl_tmpl, 'E', "$genus $species $infraspecific_type $infraspecific_name not found in organism");
          $flag = FALSE;
        }
      }

      // Checks the feature.
      $num = MCL_CHADO_FEATURE::countFeatureInfra($mcl_tmpl, $uniquename, $genus, $species, $infraspecific_type, $infraspecific_name, $type_id);
      if ($num > 1) {
        self::updateMsg($mcl_tmpl, 'E', "The uniquename '$uniquename' is ambiguous. Please do not use this uniquename");
        $flag = FALSE;
      }
      else if ($num < 1) {
        $feature_type = $type->getName();
        if ($warning) {
          self::updateMsg($mcl_tmpl, 'W', "The uniquename '$uniquename'  ($feature_type) does not exist in chado.feature");
        }
        else {
          self::updateMsg($mcl_tmpl, 'E', "The uniquename '$uniquename'  ($feature_type) does not exist in chado.feature");
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Count the number of the feature with the same uniquename, type_id and/or
   * organism_id
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $uniquename
   * @param string $genus
   * @param string $species
   * @param integer $type_id
   */
  public static function countFeature(MCL_TEMPLATE $mcl_tmpl = NULL, $uniquename, $genus, $species, $type_id) {

    // Checks the organism_id.
    $organism_id = '';
    if ($genus && $species) {
      $organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
      if ($organism) {
        $organism_id  = $organism->getOrganismID();
      }
      else {
        self::updateMsg($mcl_tmpl, 'E', "$genus $species not found in organism");
        return 0;
      }
    }

    // Sets the arguments.
    $args = array(
      'uniquename'  => $uniquename,
      'type_id'     => $type_id,
    );

    // Searches for alias.
    $sql = "
      SELECT COUNT(F.feature_id)
      FROM {chado.feature} F
      WHERE LOWER(F.uniquename) = LOWER(:uniquename) AND F.type_id = :type_id
    ";
    if ($organism_id) {
      $args['organism_id'] = $organism_id;
      $sql .= " AND F.organism_id = :organism_id";
    }
    return db_query($sql, $args)->fetchField();
  }

  /**
   * Count the number of the feature with the same uniquename, type_id and/or
   * organism_id
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $uniquename
   * @param string $genus
   * @param string $species
   * @param string $infraspecific_type
   * @param string $infraspecific_name
   * @param integer $type_id
   */
  public static function countFeatureInfra(MCL_TEMPLATE $mcl_tmpl = NULL, $uniquename, $genus, $species, $infraspecific_type, $infraspecific_name, $type_id) {

    // Checks the organism_id.
    $organism_id = '';
    if ($genus && $species) {
      $organism = MCL_CHADO_ORGANISM::getOrganismInfra($genus, $species, $infraspecific_type, $infraspecific_name);
      if ($organism) {
        $organism_id  = $organism->getOrganismID();
      }
      else {
        self::updateMsg($mcl_tmpl, 'E', "$genus $species $infraspecific_type $infraspecific_name not found in organism");
        return 0;
      }
    }

    // Sets the arguments.
    $args = array(
      'uniquename'  => $uniquename,
      'type_id'     => $type_id,
    );

    // Searches for alias.
    $sql = "
      SELECT COUNT(F.feature_id)
      FROM {chado.feature} F
      WHERE LOWER(F.uniquename) = LOWER(:uniquename) AND F.type_id = :type_id
    ";
    if ($organism_id) {
      $args['organism_id'] = $organism_id;
      $sql .= " AND F.organism_id = :organism_id";
    }
    return db_query($sql, $args)->fetchField();
  }

  /**
   * Checks the feature uniquename for uniqueness for the provided type. Search for the
   * same uniquename with different organism. Log the error if the uniquename
   * is used with different organism.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $uniquename
   * @param string $genus
   * @param string $species
   * @param integer $type_id
   *
   * @return boolean
   */
  public static function checkFeatureUniquenameForUniqueness(MCL_TEMPLATE $mcl_tmpl = NULL, $uniquename, $genus, $species, $type_id) {
    if ($uniquename) {

      // Gets the organism.
      $organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
      if ($organism) {

        // Gets all the features with the same uniquename.
        $sql = "
          SELECT F.* FROM {chado.feature} F
          WHERE LOWER(F.uniquename) = LOWER(:uniquename) AND F.type_id = :type_id
        ";
        $args =  array(
          ':uniquename' => $uniquename,
          ':type_id'    => $type_id,
        );
        $results = db_query($sql,$args);
        $found = FALSE;
        $count = 0;
        while ($feature = $results->fetchObject()) {
          $count++;

          // Checks for organism_id.
          if ($feature->organism_id == $organism->getOrganismID()) {
            $found = TRUE;
          }
        }
        if ($count && !$found) {
          self::updateMsg($mcl_tmpl, 'E', "$uniquename is used with different organism. Please change the name.");
          return FALSE;
        }
      }
    }
    return TRUE;
  }

  /**
   * Checks the feature uniquename for uniqueness for the provided type. Search for the
   * same uniquename with different organism. Log the error if the uniquename
   * is used with different organism.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $uniquename
   * @param string $genus
   * @param string $species
   * @param string $infraspecific_type
   * @param string $infraspecific_name
   * @param integer $type_id
   *
   * @return boolean
   */
  public static function checkFeatureUniquenameForUniquenessInfra(MCL_TEMPLATE $mcl_tmpl = NULL, $uniquename, $genus, $species, $infraspecific_type, $infraspecific_name, $type_id) {
    if ($uniquename) {

      // Gets the organism.
      $organism = MCL_CHADO_ORGANISM::getOrganismInfra($genus, $species, $infraspecific_type, $infraspecific_name);
      if ($organism) {

        // Gets all the features with the same uniquename.
        $sql = "
          SELECT F.* FROM {chado.feature} F
          WHERE LOWER(F.uniquename) = LOWER(:uniquename) AND F.type_id = :type_id
        ";
        $args =  array(
          ':uniquename' => $uniquename,
          ':type_id'    => $type_id,
        );
        $results = db_query($sql,$args);
        $found = FALSE;
        $count = 0;
        while ($feature = $results->fetchObject()) {
          $count++;

          // Checks for organism_id.
          if ($feature->organism_id == $organism->getOrganismID()) {
            $found = TRUE;
          }
        }
        if ($count && !$found) {
          self::updateMsg($mcl_tmpl, 'E', "$uniquename is used with different organism. Please change the name.");
          return FALSE;
        }
      }
    }
    return TRUE;
  }

  /**
   * Checks the existence of feature. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $feature
   * @param string $genus
   * @param string $species
   * @param integer $type_id
   * @param string $separator
   *
   * @return boolean
   */
  public static function checkFeatureAlias(MCL_TEMPLATE $mcl_tmpl = NULL, $feature, $genus, $species, $type_id, $separator = '') {
    $flag = TRUE;

    // Checks the organism_id.
    $organism_id = '';
    if ($genus && $species) {
      $organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
      if ($organism) {
        $organism_id  = $organism->getOrganismID();
      }
      else {
        self::updateMsg($mcl_tmpl, 'E', "$genus $species not found in chado.organism");
        return FALSE;
      }
    }

    // Gets the uniquenames.
    $uniquenames = preg_split(self::getSepRegex($separator), $feature, -1, PREG_SPLIT_NO_EMPTY);
    foreach ($uniquenames as $uniquename) {
      $uniquename = trim($uniquename);

      // Sets the arguments.
      $args = array(
        'uniquename'  => $uniquename,
        'type_id'     => $type_id,
      );
      if ($organism_id) {
        $args['organism_id'] = $organism_id;
      }

      // If '::' found in uniquename, update uniquename and organism_id.
      if (preg_match("/^(.*?)::(.*?)$/", $uniquename, $matches)) {
        $uniquename         = trim($matches[2]);
        $parts = preg_split("/\s+/", trim($matches[1]));
        $genus              = $parts[0];
        $species            = $parts[1];
        $infraspecific_type = $parts[2];
        $infraspecific_name = $parts[3];
        $diff_organism = MCL_CHADO_ORGANISM::getOrganismInfra($genus, $species, $infraspecific_type, $infraspecific_name);
        if ($diff_organism) {
          $args['organism_id'] = $diff_organism->getOrganismID();
        }
        else {
          self::updateMsg($mcl_tmpl, 'E', "$genus $species $infraspecific_type $infraspecific_name not found in organism");
          $flag = FALSE;
        }
      }

      // Checks the feature.
      $feature = MCL_CHADO_FEATURE::byKey($args);
      if (!$feature) {

        // Searches for alias.
        $sql = "
          SELECT COUNT(S.synonym_id)
          FROM {chado.synonym} S
            INNER JOIN {chado.feature_synonym} FS on FS.synonym_id = S.synonym_id
          WHERE LOWER(S.name) = LOWER(:name)
        ";
        $num = db_query($sql, array(':name' => $uniquename))->fetchField();
        if ($num > 1) {
          self::updateMsg($mcl_tmpl, 'E', "The alias '$uniquename' is ambiguous. Please not use this alias");
          $flag = FALSE;
        }
        else if ($num < 1) {
          self::updateMsg($mcl_tmpl, 'E', self::arrStr($args) . " not found in feature");
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Checks the residues. If the residues has allele (e.g. [A|T]), check if
   * it meets the following conditions.
   *   1. it has only one allele.
   *   2. it has only one pair of brackets ([]).
   *   3. The allele has a valid IUPAC code.
   *
   * @param MCL_TEMPLATE MCL_TEMPLATE
   *
   * @return boolean
   */
  public static function checkResidues(MCL_TEMPLATE $mcl_tmpl = NULL, $residues) {

    // Counts how many brackets in the residues.
    $count_L = preg_match_all('/\[/', $residues);
    $count_R = preg_match_all("/\]/", $residues);

    // Checks for the corresponding IUPAC code if the residue has only one allele.
    if ($count_L == 1 && $count_R == 1) {

      // Checks if the allele has the corresponding IUPAC code.
      if (preg_match("/\[(.+?)\]/", $residues, $matches)) {
        $allele = $matches[1];
        if (self::getIUPAC($allele)) {
          return TRUE;
        }
        else {

          // Ignores IUPAC code so that an allele like [-/T] can be stored.
          return TRUE;
          self::updateMsg($mcl_tmpl, 'E', "The allele [$allele] in the residue does not have valid IUPAC code");
          return FALSE;
        }
      }
      else {
        self::updateMsg($mcl_tmpl, 'E', "There is a mis-matching brackets in the residue");
        return FALSE;
      }
    }
    else if ($count_L != $count_R) {
      self::updateMsg($mcl_tmpl, 'E', "There are mis-matching brackets in the residue");
      return FALSE;
    }
    return TRUE;
  }


  /**
   * Checks the sequenes.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param array $line
   * @param string $prefix
   *
   * @return boolean
   */
  public static function checkSequences(MCL_TEMPLATE $mcl_tmpl = NULL, $line, $prefix) {
    $flag = TRUE;

    // Builds the pattern.
    $pattern = "^$prefix(\d+)_(seq|name)";

    // Gets name and seq columns.
    $sequences = array();
    foreach ($line as $column => $value) {
      if (preg_match("/$pattern/i", $column, $matches)) {
        $num  = $matches[1];
        $type = $matches[2];
        $name = $prefix . $num . '_' . $type;
        $sequences[$num][$type] = $line[$name];
      }
    }

    // Checks for the matching name and seq columns.
    foreach ($sequences as $num => $info) {

      // Checks the existance of name and seq columns.
      if (!array_key_exists('seq', $info) || !array_key_exists('name', $info)) {
        if (array_key_exists('seq', $info) && $info['seq']) {
          self::updateMsg($mcl_tmpl, 'E', $prefix . $num . "_name columns is missing.");
          $flag = FALSE;
        }
        else if (array_key_exists('name', $info) && $info['name']) {
          self::updateMsg($mcl_tmpl, 'E', $prefix . $num . "_seq columns is missing.");
          $flag = FALSE;
        }
      }

      // Both name and seq colummns exist, so checks for their values.
      else {

        // If one of matching columns is empty, writes error in the log.
        if (empty($info['seq']) xor empty($info['name'])) {
          self::updateMsg($mcl_tmpl, 'E', "Either name or seq of $prefix$num is empty");
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Parses the residues. If the residues has allele (e.g. [A|T]), it does
   * the followings.
   *   1. Replace it with IUPAC code.
   *   2. Gets five prime flanking sequence
   *   3. Gets three prime flanking sequence
   *   4. Gets allele
   *
   * @param string $residues
   *
   * @return array
   */
  public static function parseResidues($residues) {
    $seq_info = array();

    // Counts how many brackets in the residues.
    $count_L = preg_match_all('/\[/', $residues);
    $count_R = preg_match_all("/\]/", $residues);

    // Checks if the residues has allele.
    if ($count_L == 1 && $count_R == 1) {
      if (preg_match('/^(.*)\[(.*?)\](.*)$/', $residues, $matches)) {

        // Parses out the sequence information.
        $five_prime   = trim($matches[1]);
        $allele       = trim($matches[2]);
        $three_prime  = trim($matches[3]);
        $iupac_code   = self::getIUPAC($allele);

        // Sets the info.
        $seq_info['allele']       = str_replace(' ', '', $allele);
        $seq_info['iupac_code']   = $iupac_code;
        $seq_info['five_prime']   = $five_prime;
        $seq_info['three_prime']  = $three_prime;

        // If the allele has the corrensponding IUPAC code, replace with it.
        // Otherwise, just saves the residue.
        if ($iupac_code) {
          $seq_info['residues'] = $five_prime . $iupac_code . $three_prime;
        }
        else {
          $seq_info['residues'] = $residues;
        }
      }
      else {
        $seq_info['residues'] = $residues;
      }
    }
    else {
      $seq_info['residues'] = $residues;
    }
    return $seq_info;
  }

  /**
   * Returns IUPAC code of the given string.
   *
   * @param string $nucleotides
   *
   * IUPAC codes
   *
   * Nucleotide Code:  Base:
   * ==================
   * A.................Adenine
   * C.................Cytosine
   * G.................Guanine
   * T (or U)..........Thymine (or Uracil)
   * R.................A or G
   * Y.................C or T
   * S.................G or C
   * W.................A or T
   * K.................G or T
   * M.................A or C
   * B.................C or G or T
   * D.................A or G or T
   * H.................A or C or T
   * V.................A or C or G
   * N.................any base
   * . or -............gap
   *
   * @return string
   */
  public static function getIUPAC($nucleotides) {
    $iupac_code = '';
    if ($nucleotides) {

      // Cleans up and splits the nucleotides to bases.
      $sep = array(' ', '/', '\\', '|');
      $nucleotides = str_replace($sep, '', $nucleotides);

      // Sorts them and finds matched IUPAC code.
      $temp = str_split($nucleotides);
      asort($temp);
      $code = implode('', $temp);
      if ($code == 'AG') {
        $iupac_code = 'R';
      }
      else if ($code == 'CT') {
        $iupac_code = 'Y';
      }
      else if ($code == 'CG') {
        $iupac_code = 'S';
      }
      else if ($code == 'AT') {
        $iupac_code = 'W';
      }
      else if ($code == 'GT') {
        $iupac_code = 'K';
      }
      else if ($code == 'AC') {
        $iupac_code = 'M';
      }
      else if ($code == 'CGT') {
        $iupac_code = 'B';
      }
      else if ($code == 'AGT') {
        $iupac_code = 'D';
      }
      else if ($code == 'ACT') {
        $iupac_code = 'H';
      }
      else if ($code == 'ACG') {
        $iupac_code = 'V';
      }
    }
    return $iupac_code;
  }

  /**
   * Returns IUPAC code of the given string.
   *
   * @param string $nucleotides
   *
   * IUPAC codes
   *
   * Nucleotide Code:  Base:
   * ==================
   * A.................Adenine
   * C.................Cytosine
   * G.................Guanine
   * T (or U)..........Thymine (or Uracil)
   * R.................A or G
   * Y.................C or T
   * S.................G or C
   * W.................A or T
   * K.................G or T
   * M.................A or C
   * B.................C or G or T
   * D.................A or G or T
   * H.................A or C or T
   * V.................A or C or G
   * N.................any base
   * . or -............gap
   *
   * @return string
   */
  public static function getBases($iupac_code, $separator = '/') {
    $bases = '';
    if ($iupac_code) {
      if ($iupac_code == 'R') {
        $bases = 'A' . $separator . 'G';
      }
      else if ($iupac_code == 'Y') {
        $bases = 'C' . $separator . 'T';
      }
      else if ($iupac_code == 'S') {
        $bases = 'C' . $separator . 'G';
      }
      else if ($iupac_code == 'W') {
        $bases = 'A' . $separator . 'T';
      }
      else if ($iupac_code == 'K') {
        $bases = 'G' . $separator . 'T';
      }
      else if ($iupac_code == 'M') {
        $bases = 'A' . $separator . 'G';
      }
      else if ($iupac_code == 'B') {
        $bases = 'C' . $separator . 'G'. $separator . 'T';
      }
      else if ($iupac_code == 'D') {
        $bases = 'A' . $separator . 'G'. $separator . 'T';
      }
      else if ($iupac_code == 'H') {
        $bases = 'A' . $separator . 'C'. $separator . 'T';
      }
      else if ($iupac_code == 'V') {
        $bases = 'A' . $separator . 'C' . $separator . 'G';
      }
    }
    return $bases;
  }

  /**
   * Returns the feature by uniquename, genus, species and type_id.
   *
   * @param string $uniquename
   * @param string $genus
   * @param string $species
   * @param integer $type_id
   *
   * @return MCL_CHADO_FEATURE
   */
  public static function getFeature($uniquename, $genus, $species, $type_id) {

    // Gets the organism_id.
    $organism_id = '';
    if ($genus && $species) {
      $organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
      if ($organism) {
        $organism_id = $organism->getOrganismID();
      }
      else {
       return NULL;
      }
    }

    // Gets the feature.
    $args = array(
      'uniquename'  => $uniquename,
      'type_id'     => $type_id,
    );
    if ($organism_id) {
      $args['organism_id'] = $organism_id;
    }
    return MCL_CHADO_FEATURE::byKey($args);
  }

  /**
   * Returns the feature by uniquename, genus, species, optional infraspecific nomenclature, and type_id.
   *
   * @param string $uniquename
   * @param string $genus
   * @param string $species
   * @param string $infraspecific_type
   * @param string $infraspecific_name
   * @param integer $type_id
   *
   * @return MCL_CHADO_FEATURE
   */
  public static function getFeatureInfra($uniquename, $genus, $species, $infraspecific_type, $infraspecific_name, $type_id) {
    // Gets the organism_id.
    $organism_id = '';
    if ($genus && $species) {
      $organism = MCL_CHADO_ORGANISM::getOrganismInfra($genus, $species, $infraspecific_type, $infraspecific_name);
      if ($organism) {
        $organism_id = $organism->getOrganismID();
      }
      else {
       return NULL;
      }
    }

    // Gets the feature.
    $args = array(
      'uniquename'  => $uniquename,
      'type_id'     => $type_id,
    );
    if ($organism_id) {
      $args['organism_id'] = $organism_id;
    }
    return MCL_CHADO_FEATURE::byKey($args);
  }

  /**
   * Returns the type of the feature.
   *
   * @return string
   */
  public function getFeatureType() {
    $cvterm = MCL_CHADO_CVTERM::byID($this->type_id);
    return $cvterm ? $cvterm->getName() : '';
  }

  /**
   * Returns the type ID of the feature.
   *
   * @param string $feature_type
   *
   * @return integer
   */
  public static function getFeatureTypeID($feature_type) {

    // Gets the cvterm for the feature type.
    $cvterm       = NULL;
    $cv_name      = 'sequence';
    $cvterm_name  = '';
    if ($feature_type == 'marker') {
      $cvterm_name = 'genetic_marker';
    }
    else if ($feature_type == 'qtl') {
      $cvterm_name = 'qtl';
    }
    else if ($feature_type == 'mcl') {
      $cvterm_name = 'heritable_phenotypic_marker';
    }
    if ($cv_name && $cvterm_name) {
      $cvterm = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name);
    }
    return $cvterm ? $cvterm->getCvtermID() : NULL;
  }

  /**
   * Returns the feature by uniquename, genus, species and type_id.
   * Search uniquename for alias if it does not exist in feature table.
   *
   * @param string $uniquename
   * @param string $genus
   * @param string $species
   * @param integer $type_id
   *
   * @return MCL_CHADO_FEATURE
   */
  public static function getFeatureAlias($uniquename, $genus, $species, $type_id) {

    // Gets the organism_id.
    $organism_id = '';
    if ($genus && $species) {
      $organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
      if ($organism) {
        $organism_id = $organism->getOrganismID();
      }
      else {
        return NULL;
      }
    }

    // Gets the feature.
    $args = array(
      'uniquename'  => $uniquename,
      'type_id'     => $type_id,
    );
    if ($organism_id) {
      $args['organism_id'] = $organism_id;
    }
    $feature = MCL_CHADO_FEATURE::byKey($args);
    if ($feature) {
      return $feature;
    }

    // Search for alias.
    $sql = "
      SELECT COUNT(S.synonym_id)
      FROM {chado.synonym} S
        INNER JOIN {chado.feature_synonym} FS on FS.synonym_id = S.synonym_id
      WHERE LOWER(S.name) = LOWER(:name)
    ";
    $num = db_query($sql, array(':name' => $uniquename))->fetchField();
    if ($num == 1) {
      $sql = "
        SELECT FS.feature_id
        FROM {chado.synonym} S
          INNER JOIN {chado.feature_synonym} FS on FS.synonym_id = S.synonym_id
        WHERE LOWER(S.name) = LOWER(:name)
      ";
      $feature_id = db_query($sql, array(':name' => $uniquename))->fetchField();
      return MCL_CHADO_FEATURE::byKey(array('feature_id' => $feature_id));
    }
    return NULL;
  }

  /**
   * Returns the all alias of the feature.
   *
   * @return array
   */
  public function getAlias() {

    // Search for alias.
    $sql = "
      SELECT S.name
      FROM {chado.synonym} S
        INNER JOIN {chado.feature_synonym} FS on FS.synonym_id = S.synonym_id
      WHERE FS.feature_id = :feature_id
      ORDER BY S.name
    ";
    $results = db_query($sql, array(':feature_id' => $this->feature_id));
    $alias = array();
    while ($name = $results->fetchField()) {
      $alias []= $name;
    }
    return $alias;
  }

  /**
   * Returns the number of features with the same uniquename and type_id but
   * different organism_id.
   *
   * @param string $uniquename
   * @param integer $type_id
   *
   * @return integer
   */
  public static function countFeatures($uniquename, $type_id) {

    // Gets the features.
    $sql = "
      SELECT COUNT(F.feature_id)
      FROM chado.feature F
      WHERE LOWER(F.uniquename) = LOWER(:uniquename)
        AND F.type_id = :type_id
    ";
    $args = array(
      'uniquename'  => $uniquename,
      'type_id'     => $type_id,
    );
    $result = db_query($sql, $args);
    return $result->fetchField();
  }

  /**
   * Returns the feature by uniquename, genus, species and type_id.
   *
   * @param string $cv_name
   * @param string $cvterm_name
   *
   * @return string
   */
  public function getFeatureprop($cv_name, $cvterm_name) {

    // Gets cvterm.
    $cvterm = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name);
    if ($cvterm) {
      $details = array(
        'feature_id' => $this->feature_id,
        'type_id'    => $cvterm->getCvtermID(),
      );
      $featureprop = CHADO_FEATUREPROP::byKey($details);
      if ($featureprop) {
        return $featureprop->getValue();
      }
    }
    return '';
  }

  /**
   * Adds a feature.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $uniquename
   * @param integer $type_id
   * @param integer $organism_id
   * @param string $name
   *
   * @return MCL_CHADO_FEATURE
   */
  public static function addFeature(MCL_TEMPLATE $mcl_tmpl = NULL, $uniquename, $name, $type_id, $organism_id) {

    // Sets the arguments.
    $args = array(
      'uniquename'  => $uniquename,
      'type_id'     => $type_id,
      'organism_id' => $organism_id,
    );

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $feature = MCL_CHADO_FEATURE::byKey($args);
    if ($feature) {
      self::addMsg($mcl_tmpl, 'D', 'feature', $args);
    }
    else {

      // Adds a new feature.
      $args['name'] = $name;
      $feature = new MCL_CHADO_FEATURE($args);
      if ($feature->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'feature', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'feature', $args);
        return NULL;
      }
    }
    return $feature;
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
      return $this->addProperty($mcl_tmpl, 'featureprop', 'feature_id', $this->feature_id, $type_id, $value, $separator);
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
      return $this->addProperty($mcl_tmpl, 'featureprop', 'feature_id', $this->feature_id, $type_id, $value, $separator);
    }
    return TRUE;
  }

  /**
   * Adds related feature to feature_relationship.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_FEATURE $feature
   * @param integer $type_id
   * @param boolean $swap
   *
   * @return boolean
   */
  public function addRelatedFeature(MCL_TEMPLATE $mcl_tmpl = NULL, $feature, $type_id, $swap = FALSE) {
    if ($feature) {
      if ($swap) {
        return $this->addRelationship($mcl_tmpl, 'feature_relationship', 'subject_id', $feature->getFeatureID(), 'object_id', $this->feature_id, $type_id);
      }
      else {
        return $this->addRelationship($mcl_tmpl, 'feature_relationship', 'subject_id', $this->feature_id, 'object_id', $feature->getFeatureID(), $type_id);
      }
    }
    return TRUE;
  }

  /**
   * Adds related features to feature_relationship.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $features
   * @param integer $feature_type_id
   * @param integer $relationship_type_id
   * @param boollean $use_organism_id
   * @param string $separator
   * @param boolean $swap
   *
   * @return boolean
   */
  public function addRelatedFeatures(MCL_TEMPLATE $mcl_tmpl = NULL, $features, $feature_type_id, $use_organism_id, $relationship_type_id, $separator = '', $swap = FALSE) {
    $flag = TRUE;
    if ($features) {
      $uniquenames = preg_split($this->getSepRegex($separator), $features, -1, PREG_SPLIT_NO_EMPTY);
      foreach ($uniquenames as $uniquename) {
        $uniquename = trim($uniquename);

        // Sets the arguments.
        $args = array(
          'uniquename'  => $uniquename,
          'type_id'     => $feature_type_id,
        );
        if ($use_organism_id) {
          $organism_id = $this->organism_id;

          // If '::' found in uniquename, update uniquename and organism_id.
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
          $args['organism_id'] = $organism_id;
        }

        // Adds a feature.
        $feature = MCL_CHADO_FEATURE::byKey($args);
        if ($feature) {
          if ($swap) {
            if (!$this->addRelationship($mcl_tmpl, 'feature_relationship', 'subject_id', $feature->getFeatureID(), 'object_id', $this->feature_id, $relationship_type_id)) {
              $flag = FALSE;
            }
          }
          else {
            if (!$this->addRelationship($mcl_tmpl, 'feature_relationship', 'subject_id', $this->feature_id, 'object_id', $feature->getFeatureID(), $relationship_type_id)) {
              $flag = FALSE;
            }
          }
        }
      }
    }
    return $flag;
  }

  /**
   * Adds a sequnce to this feature.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param array $line
   * @param string $prefix
   * @param integer $type_id_feature
   * @param integer $type_id_relationship
   * @param boolean $swap
   *   Swaps subject_id and object_id.
   *
   * @return boolean
   */
  public function addSequence(MCL_TEMPLATE $mcl_tmpl = NULL, $line, $prefix, $type_id_feature, $type_id_relationship, $swap = FALSE) {
    $flag = TRUE;

    // Builds the pattern.
    $pattern = "^$prefix(\d+)_(seq|name)";

    // Gets name and seq columns.
    $sequences = array();
    foreach ($line as $column => $value) {
      if (preg_match("/$pattern/i", $column, $matches)) {
        $num  = $matches[1];
        $type = $matches[2];
        $name = $prefix . $num . '_' . $type;
        $sequences[$num][$type] = $line[$name];
      }
    }

    // Skips the unmatched sequences.
    $matched_seqs = array();
    foreach ($sequences as $num => $info) {
      $name = $prefix . $num;
      $col_name = $name . '_name';
      $col_seq = $name . '_seq';

      // Checks the existance of name and seq columns.
      if (!array_key_exists('seq', $info) || !array_key_exists('name', $info)) {
        if (array_key_exists('seq', $info) && $info['seq']) {
          $this->updateMsg($mcl_tmpl, 'W', "$col_name columns is missing. So it will be skipped");
        }
        else if (array_key_exists('name', $info) && $info['name']) {
          $this->updateMsg($mcl_tmpl, 'W', "$col_seq columns is missing. So it will be skipped");
        }
      }

      // Both name and seq colummns exist, so checks for their values.
      else {

        // If both matching columns are empty, just skip without warning.
        if (empty($info['seq']) && empty($info['name']))  {
          continue;
        }

        // If one of matching columns is empty, adds warning.
        else if (empty($info['seq']) || empty($info['name'])) {
          $this->updateMsg($mcl_tmpl, 'W', "Either $col_name or $col_seq are missing. So it will be skipped");
        }

        // Adds the matching columns.
        else {
          $matched_seqs[$info['name']] = $info['seq'];
        }
      }
    }

    // Adds sequences.
    foreach ($matched_seqs as $seq_name => $seq) {
      $args = array(
        'uniquename'  => $this->getUniquename() . '.' . $seq_name,
        'name'        => $seq_name,
        'type_id'     => $type_id_feature,
        'organism_id' => $this->organism_id,
      );
      $feature = MCL_CHADO_FEATURE::byKey($args);
      if ($feature) {
        $this->addMsg($mcl_tmpl, 'D', 'feature', $args);
      }
      else {

        // Adds a sequence.
        $args['residues'] = $seq;
        $feature = new MCL_CHADO_FEATURE($args);
        if ($feature->insert()) {
          $this->addMsg($mcl_tmpl, 'N', 'feature', $args);

          // Adds a relationship.
          $this->addRelatedFeature($mcl_tmpl, $feature, $type_id_relationship, $swap);
        }
        else {
          $this->addMsg($mcl_tmpl, 'E', 'feature', $args);
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Adds accessions to dbxref and feature_dbxref.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $db_id
   * @param string $dbxref
   * @param string $separator
   *
   * @return boolean
   */
  public function addDBXref(MCL_TEMPLATE $mcl_tmpl = NULL, $db_id, $dbxref, $separator = '') {
    $flag = TRUE;
    if ($dbxref) {
      $accessions = preg_split($this->getSepRegex($separator), $dbxref, -1, PREG_SPLIT_NO_EMPTY);
      foreach ($accessions as $accession) {
        $accession = trim($accession);
        $dbxref = MCL_CHADO_DBXREF::addDBXref($mcl_tmpl, $db_id, $accession);
        if ($dbxref) {
          if (!$this->addLink($mcl_tmpl, 'feature_dbxref', 'feature_id', $this->getFeatureID(), 'dbxref_id', $dbxref->getDbxrefID())) {
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
   * Adds a trait to feature_cvterm.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $trait_name
   * @param string $trait_descriptor_set
   * @param string $separator
   *
   * @return boolean
   */
  public function addTrait(MCL_TEMPLATE $mcl_tmpl = NULL, $trait_name, $trait_descriptor_set = NULL, $separator = '') {
    $no_error = TRUE;

    // Updates the name of the trait descriptor set.
    if (!$trait_descriptor_set) {
      $trait_descriptor_set = MCL_SITE_VAR::getValueByName('SITE_TRAIT_ONTOLOGY');
    }

    // Adds the traits.
    $trait_names = preg_split(self::getSepRegex($separator), $trait_name, -1, PREG_SPLIT_NO_EMPTY);
    foreach ($trait_names as $name) {
      $name = trim($name);

      // Gets and add the trait.
      $trait = MCL_CHADO_CVTERM::getCvterm($trait_descriptor_set, $name);
      if ($trait) {
        $args = array(
          'pub_id'      => MCL_DUMMY_VAR::getIdByName('PUB_ID'),
          'feature_id'  => $this->feature_id,
          'cvterm_id'   => $trait->getCvtermID(),
        );

        // Checks for duplication.
        $chado_feature_cvterm = CHADO_FEATURE_CVTERM::byKey($args);
        if ($chado_feature_cvterm) {
          $this->addMsg($mcl_tmpl, 'D', 'feature_cvterm', $args);
        }
        else {

          // Adds a new relationship.
          $chado_feature_cvterm = new CHADO_FEATURE_CVTERM($args);
          if ($chado_feature_cvterm->insert()) {
            $this->addMsg($mcl_tmpl, 'N', 'feature_cvterm', $args);
          }
          else {
            $this->addMsg($mcl_tmpl, 'E', 'feature_cvterm', $args);
            $no_error = FALSE;
          }
        }
      }
    }
    return $no_error;
  }

  /**
   * Adds an alias to synoym and feature_synoym.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $type_id
   * @param string $alias
   * @param string $separator
   *
   * @return boolean
   */
  public function addAlias(MCL_TEMPLATE $mcl_tmpl = NULL, $type_id, $alias, $separator = '') {
    $flag = TRUE;

    // Sets the arguments.
    $args = array(
      'type_id' => $type_id,
      'alias'   => $alias,
    );

    // Checks arguements.
    if (!$this->checkReqArgs($args)) {
      return NULL;
    }

    // Sets the default values.
    $synonym_sgml = '';

    // Adds a synonym.
    $names = preg_split($this->getSepRegex($separator), $alias, -1, PREG_SPLIT_NO_EMPTY);
    foreach ($names as $name) {
      $name = trim($name);

      // Checks for duplication [synonym].
      $args = array(
        'type_id' => $type_id,
        'name'    => $name,
      );
      $chado_synonym = CHADO_SYNONYM::byKey($args);
      if ($chado_synonym) {
        $this->addMsg($mcl_tmpl, 'D', 'synonym', $args);
      }
      else {

        // Adds a new synonym.
        $args['synonym_sgml'] = $synonym_sgml;
        $chado_synonym = new CHADO_SYNONYM($args);
        if ($chado_synonym->insert()) {
          $this->addMsg($mcl_tmpl, 'N', 'synonym', $args);
        }
        else {
          $this->addMsg($mcl_tmpl, 'E', 'synonym', $args);
          $flag = FALSE;
          continue;
        }
      }

      // Checks for duplication [feature_synonym].
      $args = array(
        'pub_id'      => MCL_DUMMY_VAR::getIdByName('PUB_ID'),
        'feature_id'  => $this->feature_id,
        'synonym_id'  => $chado_synonym->getSynonymID(),
      );
      $chado_feature_synonym = CHADO_FEATURE_SYNONYM::byKey($args);
      if ($chado_feature_synonym) {
        $this->addMsg($mcl_tmpl, 'D', 'feature_synonym', $args);
      }
      else {

        // Adds a new relationship.
        $chado_feature_synonym = new CHADO_FEATURE_SYNONYM($args);
        if ($chado_feature_synonym->insert()) {
          $this->addMsg($mcl_tmpl, 'N', 'feature_synonym', $args);
        }
        else {
          $this->addMsg($mcl_tmpl, 'E', 'feature_synonym', $args);
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Adds a marker locus.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $marker_locus
   * @param array $cvterms
   * @param string $separator
   * @param string $suffix
   *   The suffix is added to feature.uniquename.
   */
  public function addLocus(MCL_TEMPLATE $mcl_tmpl = NULL, $marker_locus, $cvterms, $separator = '', $suffix = '') {
    $locus_arr = array();
    if ($marker_locus) {

      // Adds marker loci.
      $marker_loci = preg_split($this->getSepRegex($separator), $marker_locus, -1, PREG_SPLIT_NO_EMPTY);
      foreach ($marker_loci as $locus_name) {
        $locus_name = trim($locus_name);

        // Adds a marker locus.
        $uniquename = $suffix ? "$locus_name-$suffix" : $locus_name;
        $locus = MCL_CHADO_FEATURE::addFeature($mcl_tmpl, $uniquename, $locus_name, $cvterms['SITE_CV']['marker_locus'], $this->getOrganismID());

        // Adds a relationship.
        if ($locus) {
          $locus_arr []= $locus;
          $this->addRelatedFeature($mcl_tmpl, $locus, $cvterms['relationship']['instance_of'], TRUE);
        }
      }
    }
    return $locus_arr;
  }

  /**
   * Adds a synoym and feature_synoym.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_SYNONYM $synonym
   *
   * @return boolean
   */
  public function addSynonym(MCL_TEMPLATE $mcl_tmpl = NULL, MCL_CHADO_SYNONYM $synonym) {
    if ($synonym) {

      // Checks for duplication.
      $args = array(
        'synonym_id'  => $synonym->getSynonymID(),
        'feature_id'  => $this->feature_id,
        'pub_id'      => MCL_DUMMY_VAR::getIdByName('PUB_ID'),
      );
      $chado_feature_synonym = CHADO_FEATURE_SYNONYM::byKey($args);
      if ($chado_feature_synonym) {
        $this->addMsg($mcl_tmpl, 'D', 'feature_synonym', $args);
      }
      else {

        // Adds a new relationship.
        $chado_feature_synonym = new CHADO_FEATURE_SYNONYM($args);
        if ($chado_feature_synonym->insert()) {
          $this->addMsg($mcl_tmpl, 'N', 'feature_synonym', $args);
        }
        else {
          $this->addMsg($mcl_tmpl, 'E', 'feature_synonym', $args);
          return FALSE;
        }
      }
    }
    return TRUE;
  }

  /**
   * Adds a contact to feature_contact.
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
        $contact = MCL_CHADO_CONTACT::byName($name);
        if ($contact) {
          if (!$this->addLink($mcl_tmpl, 'feature_contact', 'feature_id', $this->feature_id, 'contact_id', $contact->getContactID())) {
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
   * Adds dataset to feature_project.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $dataset_name
   * @param string $separator
   *
   * @return boolean
   */
  public function addDataset(MCL_TEMPLATE $mcl_tmpl = NULL, $dataset_name, $separator = '') {
    $flag = TRUE;
    if ($dataset_name) {
      $names = preg_split($this->getSepRegex($separator), $dataset_name, -1, PREG_SPLIT_NO_EMPTY);
      foreach ($names as $name) {
        $name = trim($name);
        $dataset = MCL_CHADO_DATASET::byName($name);
        if ($dataset) {
          if (!$this->addLink($mcl_tmpl, 'feature_project', 'feature_id', $this->feature_id, 'project_id', $dataset->getProjectID())) {
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
   * Adds a site to feature_nd_geolocation.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $site_name
   * @param string $separator
   *
   * @return boolean
   */
  public function addSite(MCL_TEMPLATE $mcl_tmpl = NULL, $site_name, $separator = '') {
    $flag = TRUE;
    if ($dataset_name) {
      $names = preg_split($this->getSepRegex($separator), $site_name, -1, PREG_SPLIT_NO_EMPTY);
      foreach ($names as $name) {
        $nd_geolocation_id =  MCL_CHADO_LOCATION::getLocationIDBySite(trim($name));
        if ($nd_geolocation_id) {
          if (!$this->addLink($mcl_tmpl, 'feature_nd_geolocation', 'feature_id', $this->feature_id, 'nd_geolocation_id', $nd_geolocation_id)) {
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
   * Adds a stock to feature_stock.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $stock
   * @param string $genus
   * @param string $species
   * @param string $separator
   *
   * @return boolean
   */
  public function addStock(MCL_TEMPLATE $mcl_tmpl = NULL, $stock, $genus, $species, $type_id, $separator = '') {
    $flag = TRUE;
    if ($stock) {

      // Gets orgaims_id.
      $organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
      if ($organism) {

        $uniquenames = preg_split($this->getSepRegex($separator), $stock, -1, PREG_SPLIT_NO_EMPTY);
        foreach ($uniquenames as $uniquename) {
          $uniquename   = trim($uniquename);
          $organism_id  = $organism->getOrganismID();

          // If '::' found in uniquename, update uniquename and organism_id.
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

          // Adds a stock.
          $stock = MCL_CHADO_STOCK::byKey(array('uniquename' => $uniquename, 'organism_id' => $organism_id));
          if ($mcl_stock) {
            if (!$this->addLink($mcl_tmpl, 'feature_stock', 'feature_id', $this->feature_id, 'stock_id', $stock->getStockID(), $type_id)) {
              $flag = FALSE;
            }
          }
          else {
            $flag = FALSE;
          }
        }
      }
      else {
        $this->updateMsg('E', "(genus, species) = ($genus, $species) not found in organism");
        $flag = FALSE;
      }
    }
    return $flag;
  }

  /**
   * Adds a stock to feature_stock.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $stock
   * @param string $genus
   * @param string $species
   * @param string $infraspecific_type
   * @param string $infraspecific_name
   * @param string $separator
   *
   * @return boolean
   */
  public function addStockInfra(MCL_TEMPLATE $mcl_tmpl = NULL, $stock, $genus, $species, $infraspecific_type, $infraspecific_name, $type_id, $separator = '') {
    $flag = TRUE;
    if ($stock) {

      // Gets organism_id.
      $organism = MCL_CHADO_ORGANISM::getOrganismInfra($genus, $species, $infraspecific_type, $infraspecific_name);
      if ($organism) {

        $uniquenames = preg_split($this->getSepRegex($separator), $stock, -1, PREG_SPLIT_NO_EMPTY);
        foreach ($uniquenames as $uniquename) {
          $uniquename   = trim($uniquename);
          $organism_id  = $organism->getOrganismID();

          // If '::' found in uniquename, update uniquename and organism_id.
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

          // Adds a stock.
          $stock = MCL_CHADO_STOCK::byKey(array('uniquename' => $uniquename, 'organism_id' => $organism_id));
          if ($mcl_stock) {
            if (!$this->addLink($mcl_tmpl, 'feature_stock', 'feature_id', $this->feature_id, 'stock_id', $stock->getStockID(), $type_id)) {
              $flag = FALSE;
            }
          }
          else {
            $flag = FALSE;
          }
        }
      }
      else {
        $this->updateMsg('E', "(genus, species) = ($genus, $species) not found in organism");
        $flag = FALSE;
      }
    }
    return $flag;
  }

  /**
   * Adds a genotype to feature_genotype.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_GENOTYPE $genotype
   * @param integer $cvterm_id
   *
   * @return boolean
   */
  public function addGenotype(MCL_TEMPLATE $mcl_tmpl = NULL, MCL_CHADO_GENOTYPE $genotype, $cvterm_id) {

    // Checks feature_genotype for duplication.
    $args = array(
      'feature_id'  => $this->feature_id,
      'genotype_id' => $genotype->getGenotypeID(),
      'cvterm_id'   => $cvterm_id,
    );
    $chado_feature_genotype = CHADO_FEATURE_GENOTYPE::byKey($args);
    if ($chado_feature_genotype) {
      $this->addMsg($mcl_tmpl, 'D', 'feature_genotype', $args);
    }
    else {

      // Adds feature_genotype.
      $args['cgroup'] = 1;
      $args['rank']   = 0;
      $chado_feature_genotype = new CHADO_FEATURE_GENOTYPE($args);
      if ($chado_feature_genotype->insert()) {
        $this->addMsg($mcl_tmpl, 'N', 'feature_genotype', $args);
      }
      else {
        $this->addMsg($mcl_tmpl, 'E', 'feature_genotype', $args);
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Adds an image to feature_image.
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
          if (!$this->addLink($mcl_tmpl, 'feature_image', 'feature_id', $this->feature_id, 'eimage_id', $image->getEimageID())) {
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
   * Adds a reference to feature_pub.
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
          if (!$this->addLink($mcl_tmpl, 'feature_pub', 'feature_id', $this->feature_id, 'pub_id', $pub_id)) {
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
   * Adds a location to chado.featureloc.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $srcfeature_id
   * @param integer $start
   * @param integer $end
   * @param strint $strand
   * @param boolean $required
   *
   * @return boolean
   */
  public function addLocation(MCL_TEMPLATE $mcl_tmpl = NULL, $srcfeature_id, $fmin, $fmax, $strand, $required = TRUE) {

    // Checks the locations.
    if ($required && $mcl_tmpl) {
      if (!$mcl_tmpl->checkRange($fmin, $fmax)) {
        $mcl_tmpl->updateMsg('E', "(fmin, fmax) = ($fmin, $fmax). Either or both positions are missing");
        return FALSE;
      }
    }

    // Sets the search_path for chado.featureloc table.
    db_query("SET SEARCH_PATH=chado,public;");

    // Checks featureloc for duplication.
    $details = array('feature_id' => $this->feature_id);
    $chado_featureloc = CHADO_FEATURELOC::byKey($details);
    if ($chado_featureloc) {
      $this->addMsg($mcl_tmpl, 'D', 'featureloc', $details);
    }
    else {

      // Adds the location.
      $details['srcfeature_id'] = $srcfeature_id;
      $details['fmin']          = $fmin;
      $details['fmax']          = $fmax;
      $details['strand']        = $strand;
      $chado_featureloc         = new CHADO_FEATURELOC($details);
      if ($chado_featureloc->insert()) {
        $this->addMsg($mcl_tmpl, 'N', 'featureloc', $details);
      }
      else {
        $this->addMsg($mcl_tmpl, 'E', 'featureloc', $details);
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Returns the location.
   *
   * @param string $chromosome_name
   * @param integer $loc_start
   * @param integer $loc_stop
   *
   * @return string
   */
  public static function getLocation($chromosome_name, $loc_start, $loc_stop) {
    if ($chromosome_name) {
      if (($loc_start == '0' || $loc_start) && ($loc_stop == '0' || $loc_stop)) {
        return sprintf("%s:%d..%d", $chromosome_name, $loc_start, $loc_stop);
      }
    }
    return '';
  }

  /**
   * Adds a feature_relationship property.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $feature_relationship_id
   * @param string $cv_name
   * @param string $cvterm_name
   * @param string $value
   *
   * @return boolean
   */
  public static function addRelationshipProp(MCL_TEMPLATE $mcl_tmpl = NULL, $feature_relationship_id, $cv_name, $cvterm_name, $value, $rank = 0) {

    // Gets type_id.
    $type_id = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name)->getCvtermID();

    // Checks feature_relationshipprop for duplication.
    $details = array(
      'feature_relationship_id' => $feature_relationship_id,
      'type_id'                 => $type_id,
      'rank'                    => $rank,
    );
    $chado_feature_relationshipprop = CHADO_FEATURE_RELATIONSHIPPROP::byKey($details);
    if (!$chado_feature_relationshipprop) {

      // Adds the feature_relationshipprop.
      $details['value'] = $value;
      $chado_feature_relationshipprop = new CHADO_FEATURE_RELATIONSHIPPROP($details);
      if ($chado_feature_relationshipprop->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'feature_relationshipprop', $details);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'feature_relationshipprop', $args);
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * @see CHADO_TABLE::getMergeUniqueKeys()
   */
  public function getMergeUniqueKeys($chado_table) {

    // Sets unique keys for pubprop.
    if ($chado_table == 'chado.featuremap_pub') {
      $ukey_featuremap_pub = array(
        'ukeys' => array('featuremap_id', 'pub_id'),
      );
      return array($ukey_featuremap_pub);
    }
    return array();
  }

  /**
   * Returns the cvterm ID of the feature type. The type should be 'marker',
   * 'QTL' or 'MTL'.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $feature_type
   *
   * @return integer|NULL
   */
  public static function getMergeFeatureTypeID(MCL_TEMPLATE $mcl_tmpl, $feature_type) {

    //Checks the feature type for 'marker', 'QTL' or 'MTL'.
    $cvterm = NULL;
    $feature_type_lc = strtolower($feature_type);
    if($feature_type_lc == 'marker') {
      $cvterm = MCL_CHADO_CVTERM::getCvterm('sequnece', 'genetic_marker');
    }
    if($feature_type_lc == 'qtl') {
      $cvterm = MCL_CHADO_CVTERM::getCvterm('sequnece', 'qtl');
    }
    if($feature_type_lc == 'mtl') {
      $cvterm = MCL_CHADO_CVTERM::getCvterm('sequnece', 'heritable_phenotypic_marker');
    }
    if ($cvterm) {
      return $cvterm->getCvtermID();
    }
    return NULL;
  }

  /**
   * Merges the features.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_FEATURE $feature
   * @param boolean $verbose
   */
  public function mergeFeature(MCL_TEMPLATE $mcl_tmpl = NULL, MCL_CHADO_FEATURE $feature, $verbose = FALSE) {

    // Sets the information of the features.
    $params = array(
      'label'   => 'FEATURE',
      'id_base' => $this->feature_id,
      'id'      => $feature->getFeatureID(),
    );

    // Gets all reference tables.
    $ref_tables = MCL_CHADO_FEATURE::getRelTable();
    foreach ((array)$ref_tables['feature_id'] as $ref_table) {

      // Processes the merge.
      if (!$this->procMerge($mcl_tmpl, $params, $ref_table, $verbose)) {
        break;
      }
    }
  }
}
