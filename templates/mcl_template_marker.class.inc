<?php
/**
 * The declaration of MCL_TEMPLATE_MARKER class.
 *
 */
class MCL_TEMPLATE_MARKER extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'MARKER';
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
    $desc = 'The description for marker sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'marker_name'               => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the marker."),
      'marker_type'               => array('req' => TRUE,  'width' => 10, 'desc' => "Marker type such as SSR, RFLP, RAPD, STS, SCAR, overgo probe and Isozyme. Refer to the available marker_type page for each database."),
      'genus'                     => array('req' => TRUE,  'width' => 10, 'desc' => "The genus name of the organism from which the marker was developed."),
      'species'                   => array('req' => TRUE,  'width' => 10, 'desc' => "The species name of the organism from which the marker was developed. Enter 'sp.' to represent one unknown species, 'spp.' to represent multiple unknown species."),
      'infraspecific_type'        => array('req' => FALSE, 'width' => 10, 'desc' => "Infraspecific type of the stock as defined in the taxonomic_rank CV"),
      'infraspecific_name'        => array('req' => FALSE, 'width' => 10, 'desc' => "Infraspecific name of the stock"),
      'alias'                     => array('req' => FALSE, 'width' => 10, 'desc' => "Other names used for the marker. Multiple aliases, separated by a semicolon (;), can be added."),
      'marker_locus'              => array('req' => FALSE, 'width' => 10, 'desc' => "Name of the marker locus. Multiple loci, separated by a semicolon (;), can be added.."),
      'marker_sequence'           => array('req' => FALSE, 'width' => 10, 'desc' => "Sequence of the marker when available. For SNP, it should be in the format of 5'flanking sequence[A/T] 3'flanking sequence."),
      'gb_id'                     => array('req' => FALSE, 'width' => 10, 'desc' => "Associated GenBank ID of the marker or the source of the marker (eg. EST)."),
      'dbsnp_id'                  => array('req' => FALSE, 'width' => 10, 'desc' => "Associated dbSNP_ID. The submitted SNPs (SS) number should be used."),
      'rs_id'                     => array('req' => FALSE, 'width' => 10, 'desc' => "The refSNP (RS) number."),
      'gene'                      => array('req' => FALSE, 'width' => 10, 'desc' => "Associated genes."),
      'mrna'                      => array('req' => FALSE, 'width' => 10, 'desc' => "Associated mRNAs."),
      'five_prime_flanking_seq'   => array('req' => FALSE, 'width' => 10, 'desc' => "5' flanking sequence of SNP or Indel."),
      'three_prime_flanking_seq'  => array('req' => FALSE, 'width' => 10, 'desc' => "3' flanking sequence of SNP or Indel."),
      'shared_species'            => array('req' => FALSE, 'width' => 10, 'desc' => "Species names of organisms that have been found to have the same SNP. If there are more than one other species, using ';' to separate."),
      'repeat_motif'              => array('req' => FALSE, 'width' => 10, 'desc' => "Repeat motif for SSR markers."),
      'snp_chip_name'             => array('req' => FALSE, 'width' => 10, 'desc' => "SNP chip name when SNPs belong to a chip (the name should match 'name' in Library sheet."),
      'snp_chip_id'               => array('req' => FALSE, 'width' => 10, 'desc' => "SNP chip ID if there is a specific ID for the specific SNP chip entered in 'SNP_chip_ID'."),
//      'analysis_name'             => array('req' => FALSE, 'width' => 10, 'desc' => "Name of the analysis (Maxium 255 charactrers). 'name' column entry in the 'Analysis' sheet"),
//      'trait_name'                => array('req' => FALSE, 'width' => 10, 'desc' => "Name of the trait. 'trait_name' column entry in the 'Trait' sheet"),
      'restriction_enzyme'        => array('req' => FALSE, 'width' => 10, 'desc' => "Restriction Enzyme used for CAPS marker or other markers that use restriction enzymes."),
      'product_length'            => array('req' => FALSE, 'width' => 10, 'desc' => "The PCR product length from the source species of the marker."),
      'max_length'                => array('req' => FALSE, 'width' => 10, 'desc' => "The maximum product size that have been observed."),
      'min_length'                => array('req' => FALSE, 'width' => 10, 'desc' => "The minimum product size that have been observed."),
      'is_codominant'             => array('req' => FALSE, 'width' => 10, 'desc' => "Yes if the marker is codominant, No otherwise."),
      'pcr_condition'             => array('req' => FALSE, 'width' => 10, 'desc' => "PCR condition for PCR-based markers."),
      'screening_method'          => array('req' => FALSE, 'width' => 10, 'desc' => "Gel type, % etc (eg. 2% agarose) for PCR product visualization and any other screening methods for other types of markers."),
      'monomorphic_population'    => array('req' => FALSE, 'width' => 10, 'desc' => "The name of the mapping population of which the marker was monomorphic."),
      'source_marker_name'        => array('req' => FALSE, 'width' => 10, 'desc' => "The name of the marker from which the marker is generated (STS, SSR or SCAR markers are often generated from other markers). The detail of the source marker should be given in a separate row."),
      'source_description'        => array('req' => FALSE, 'width' => 10, 'desc' => "Whether the marker was developed from the sequence of EST, BAC, cDNA, genomic clone, or WGS."),
      'indel_seq'                 => array('req' => FALSE, 'width' => 10, 'desc' => "sequences of indel (eg. ATT)."),
      'alleles'                   => array('req' => FALSE, 'width' => 10, 'desc' => "Alleles for the SNP marker (eg. A/T)."),
      'gel_picture_file_name'     => array('req' => FALSE, 'width' => 10, 'desc' => "File name of the gel picture if available."),
      'gel_picture_legend'        => array('req' => FALSE, 'width' => 10, 'desc' => "Legend of the gel picture if available."),
      'comments'                  => array('req' => FALSE, 'width' => 10, 'desc' => "Any comments about the marker."),
      'variety'                   => array('req' => FALSE, 'width' => 10, 'desc' => "The variety from which the marker was developed. It should match a 'stock_name' in the Stock sheet."),
      'image'                     => array('req' => FALSE, 'width' => 10, 'desc' => "Image_ID of any associated image (Detailed info in 'Image' sheet.) Multiple images, separated by comma, can be added."),
      'contact'                   => array('req' => FALSE, 'width' => 10, 'desc' => "Contact person for the marker (Please provide the contact info of these researchers in 'Contact' sheet)."),
      'reference'                 => array('req' => FALSE, 'width' => 10, 'desc' => "Reference where the marker is originally described (pub_id), Multiple references, separated by comma, can be added."),
      '~identifier'               => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns to store names and sequences of primers and probes. The column name should start with ~. An identifier must be either 'primer' or 'probe' , followed by a numerical number and 'name' or 'seq'. (e.g.) primer1_name, primer1_seq, primer2_name, primner2_seq ; probe1_name, probe1_seq, probe2_name, probe2_seq (More columns can be created when there are more than two primers and probes)."),
      '##property_name'           => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns (##) : followed by name (cvterm name) of the property."),
    );

    // Adds labels.
    $headers['gb_id']['label']          = 'GB_ID';
    $headers['rs_id']['label']          = 'RS_ID';
    $headers['mrna']['label']           = 'mRNA';
    $headers['dbsnp_id']['label']       = 'dbSNP_ID';
    $headers['pcr_condition']['label']  = 'PCR_condition';
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['alias']                        = -1;
    $cvterms['SITE_CV']['marker_type']                  = -1;
    $cvterms['SITE_CV']['restriction_enzyme']           = -1;
    $cvterms['SITE_CV']['product_length']               = -1;
    $cvterms['SITE_CV']['max_length']                   = -1;
    $cvterms['SITE_CV']['min_length']                   = -1;
    $cvterms['SITE_CV']['is_codominant']                = -1;
    $cvterms['SITE_CV']['repeat_motif']                 = -1;
    $cvterms['SITE_CV']['source']                       = -1;
    $cvterms['SITE_CV']['pcr_condition']                = -1;
    $cvterms['SITE_CV']['screening_method']             = -1;
    $cvterms['SITE_CV']['monomorphic_population']       = -1;
    $cvterms['SITE_CV']['filename']                     = -1;
    $cvterms['SITE_CV']['indel_seq']                    = -1;
    $cvterms['SITE_CV']['legend']                       = -1;
    $cvterms['SITE_CV']['comments']                     = -1;
    $cvterms['SITE_CV']['SNP_chip']                     = -1;
    $cvterms['SITE_CV']['marker_locus']                 = -1;
    $cvterms['SITE_CV']['ss_id']                        = -1;
    $cvterms['SITE_CV']['rs_id']                        = -1;
    $cvterms['sequence']['genetic_marker']              = -1;
    $cvterms['sequence']['gene']                        = -1;
    $cvterms['sequence']['mRNA']                        = -1;
    $cvterms['sequence']['primer']                      = -1;
    $cvterms['sequence']['probe']                       = -1;
    $cvterms['sequence']['associated_with']             = -1;
    $cvterms['sequence']['five_prime_flanking_region']  = -1;
    $cvterms['sequence']['three_prime_flanking_region'] = -1;
    $cvterms['sequence']['allele']                      = -1;
    $cvterms['sequence']['SNP']                         = -1;
    $cvterms['sequence']['associated_with']             = -1;
    $cvterms['relationship']['adjacent_to']             = -1;
    $cvterms['relationship']['instance_of']             = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks db.
    MCL_CHADO_DB::checkDB($this, 'dbSNP');
    MCL_CHADO_DB::checkDB($this, 'genbank');

    // Checks marker name for uniqueness.
    MCL_CHADO_FEATURE::checkFeatureUniquenameForUniquenessInfra($this, $line['marker_name'], $line['genus'], $line['species'], $line['infraspecific_type'], $line['infraspecific_name'], $this->cvterms['sequence']['genetic_marker']);

    // Checks marker locus name for uniqueness.
    MCL_CHADO_FEATURE::checkFeatureUniquenameForUniquenessInfra($this, $line['marker_locus'], $line['genus'], $line['species'], $line['infraspecific_type'], $line['infraspecific_name'], $this->cvterms['SITE_CV']['marker_locus']);

    // Checks organism.
    MCL_CHADO_ORGANISM::checkOrganismInfra($this, $line['genus'], $line['species'], $line['infraspecific_type'], $line['infraspecific_name']);

    // Checks residues.
    MCL_CHADO_FEATURE::checkResidues($this, $line['marker_sequence']);

    // Checks SNP_chip_name.
    MCL_CHADO_LIBRARY::checkLibrary($this, $line['snp_chip_name'], $this->cvterms['SITE_CV']['SNP_chip']);

    // Checks genes.
    MCL_CHADO_FEATURE::checkFeature($this, $line['gene'], '', '', $this->cvterms['sequence']['gene'], "[,;]");

    // Checks mRNAs.
    MCL_CHADO_FEATURE::checkFeature($this, $line['mrna'], '', '', $this->cvterms['sequence']['mRNA'], "[,;]");

    // Checks analysis.
    MCL_CHADO_ANALYSIS::checkAnalysis($this, $line['analysis_name']);

    // Checks trait.
    MCL_CHADO_CVTERM::checkCvterm($this, MCL_SITE_VAR::getValueByName('SITE_TRAIT_ONTOLOGY'), $line['trait_name']);

    // Checks contacts.
    MCL_CHADO_CONTACT::checkContact($this, $line['contact'], "[;]");

    // Checks images.
    MCL_CHADO_IMAGE::checkImageID($this, $line['image'], "[,;]");

    // Checks references.
    MCL_CHADO_PUB::checkPubID($this, $line['reference'], "[,;]");

    // Checks primer sequences.
    MCL_CHADO_FEATURE::checkSequences($this, $line, '~primer');

    // Checks probe sequences.
    MCL_CHADO_FEATURE::checkSequences($this, $line, '~probe');

    // Checks marker and locus names for duplications.
    $this->_checkDupForMarkerLocus($this, $line);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the organism.
    $organism = MCL_CHADO_ORGANISM::getOrganismInfra($line['genus'], $line['species'], $line['infraspecific_type'], $line['infraspecific_name']);

    // Adds a marker.
    $marker = MCL_CHADO_FEATURE::addFeature($this, $line['marker_name'], $line['marker_name'], $this->cvterms['sequence']['genetic_marker'], $organism->getOrganismID());
    if ($marker) {

      // Parses the residues.
      $seq_info = MCL_CHADO_FEATURE::parseResidues($line['marker_sequence'], FALSE);

      // Gets residues.
      $residues = $seq_info['residues'];

      // Gets 5-prime flanking sequence.
      $five_prime = $seq_info['five_prime'];
      if ($line['five_prime_flanking_seq']) {
        $five_prime = $line['five_prime_flanking_seq'];
      }

      // Gets 3-prime flanking sequence.
      $three_prime = $seq_info['three_prime'];
      if ($line['three_prime_flanking_seq']) {
        $three_prime = $line['three_prime_flanking_seq'];
      }

      // Gets allele.
      $allele = $seq_info['allele'];
      if ($line['alleles']) {
        $allele = $line['alleles'];
      }

      // Sets the marker_sequence to the residues.
      if ($residues == '') {

        // Gets the sequence from GenBank.
        if ($line['gb_id']) {
          // gb_id of multiple values separated by semicolon also works
          $url = 'https://eutils.ncbi.nlm.nih.gov/entrez/eutils/efetch.fcgi?db=nuccore&rettype=fasta&retmode=text&id=' . urlencode($line['gb_id']);
          // eutils api requires a 3/second limit on downloads
          usleep(333334);
          mcl_print("Genbank query \"$url\"", 2, 1.1);
          $residues = trim(file_get_contents($url));
          if (!$residues) {
            mcl_print("ERROR, retrieved nothing for gb_id ".$line['gb_id'], 2, 1.1);
            $this->updateMsg('E', "Error - Genbank query retrieved nothing for gb_id ".$line['gb_id']);
          }
          else if (substr($residues,0,1) != '>') {
            mcl_print("ERROR, retrieved result not FASTA for gb_id ".$line['gb_id']." \"$residues\"", 2, 1.1);
            $this->updateMsg('E', "Error - Genbank query retrieved non-FASTA for gb_id ".$line['gb_id']);
          }
        }

        // Concatenates five_prime_flanking_seq, alleles and three_prime_flanking_seq.
        else if ($five_prime && $alleles && $three_prime) {
          $residues = $five_prime . $alleles . $three_prime;
        }
      }

      // Updates the residues.
      if ($residues) {
        $marker->setResidues($residues);
        $marker->update();
      }

      // Adds prime flanking sequences.
      $marker->addProp($this, 'sequence', 'five_prime_flanking_region', $five_prime);
      $marker->addProp($this, 'sequence', 'three_prime_flanking_region', $three_prime);

      // Adds alleles.
      $type = (strtolower($line['marker_type']) == 'snp') ? 'SNP' : 'allele';
      $marker->addProp($this, 'sequence', $type, $allele);

      // Adds the properties.
      $marker->addProp($this, 'SITE_CV', 'marker_type', $line['marker_type']);
      $marker->addProp($this, 'SITE_CV', 'repeat_motif', $line['repeat_motif']);
      $marker->addProp($this, 'SITE_CV', 'restriction_enzyme', $line['restriction_enzyme']);
      $marker->addProp($this, 'SITE_CV', 'product_length', $line['product_length']);
      $marker->addProp($this, 'SITE_CV', 'max_length', $line['max_length']);
      $marker->addProp($this, 'SITE_CV', 'min_length', $line['min_length']);
      $marker->addProp($this, 'SITE_CV', 'is_codominant', $line['is_codominant']);
      $marker->addProp($this, 'SITE_CV', 'pcr_condition', $line['pcr_condition']);
      $marker->addProp($this, 'SITE_CV', 'screening_method', $line['screening_method']);
      $marker->addProp($this, 'SITE_CV', 'monomorphic_population', $line['monomorphic_population']);
      $marker->addProp($this, 'SITE_CV', 'source', $line['source']);
      $marker->addProp($this, 'SITE_CV', 'filename', $line['filename']);
      $marker->addProp($this, 'SITE_CV', 'indel_seq', $line['indel_seq']);
      $marker->addProp($this, 'SITE_CV', 'legend', $line['legend']);
      $marker->addProp($this, 'SITE_CV', 'comments', $line['comments']);

      // Adds analysis / trait.




      // Adds the user-specific properties.
      $this->addSpecialColumns('##', $marker, $line);

      // Adds aliases.
      $marker->addAlias($this, $this->cvterms['SITE_CV']['alias'], $line['alias'], "[,;]");

      // Adds marker locus.
      $marker->addLocus($this, $line['marker_locus'], $this->cvterms, "[,;]");

      // Adds gb_id (accession).
      $db = MCL_CHADO_DB::getDB('genbank');
      $marker->addDBXref($this, $db->getDbID(), $line['gb_id'], "[,;]");

      // Adds dbsnp_id (SS ID).
      $db = MCL_CHADO_DB::getDB('dbSNP');
      $marker->addDBXref($this, $db->getDbID(), $line['dbsnp_id'], "[,;]");

      // Adds SS ID as synonym.
      $synonym = MCL_CHADO_SYNONYM::addSynonym($this, $line['dbsnp_id'], $this->cvterms['SITE_CV']['ss_id']);
      if ($synonym) {
        $marker->addSynonym($this, $synonym);
      }

      // Adds RS ID as synonym.
      $synonym = MCL_CHADO_SYNONYM::addSynonym($this, $line['rs_id'], $this->cvterms['SITE_CV']['rs_id']);
      if ($synonym) {
        $marker->addSynonym($this, $synonym);
      }

      // Adds varieties.
      $marker->addStockInfra($this, $line['variety'], $line['genus'], $line['species'], $line['infraspecific_type'], $line['infraspecific_name'], $this->cvterms['SITE_CV']['source'], "[,;]");

      // Adds contacts.
      $marker->addContact($this, $line['contact'], "[;]");

      // Adds images.
      $marker->addImageID($this, $line['image'], "[,;]");

      // Creates and adds snp_chip_id.
      $library = MCL_CHADO_LIBRARY::getLibrary($line['snp_chip_name'], $this->cvterms['SITE_CV']['SNP_chip']);
      if ($library) {

        // Adds snp_chip_id as a synonym.
        $synonym = MCL_CHADO_SYNONYM::addSynonym($this, $line['snp_chip_id'], $this->cvterms['SITE_CV']['SNP_chip']);

        // Adds the synonym to marker and library.
        if ($synonym) {
          $marker->addSynonym($this, $synonym);
          $library->addSynonym($this, $synonym);
        }
      }

      // Adds genes.
      $marker->addRelatedFeatures($this, $line['gene'], $this->cvterms['sequence']['gene'], FALSE, $this->cvterms['sequence']['associated_with'], "[,;]");

      // Adds mRNAs.
      $marker->addRelatedFeatures($this, $line['mrna'], $this->cvterms['sequence']['mRNA'], FALSE, $this->cvterms['sequence']['associated_with'], "[,;]");

      // Adds references.
      $marker->addReference($this, $line['reference'], "[,;]");

      // Adds primer sequences.
      $marker->addSequence($this, $line, '~primer', $this->cvterms['sequence']['primer'], $this->cvterms['relationship']['adjacent_to']);

      // Adds probe sequences.
      $marker->addSequence($this, $line, '~probe', $this->cvterms['sequence']['probe'], $this->cvterms['sequence']['associated_with'], TRUE);
    }
  }

  /**
   * Adds the marker and locus to dup array.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param array $line
   */
  private function _checkDupForMarkerLocus(MCL_TEMPLATE $mcl_tmpl, $line) {

    // Gets marker and locus.
    $line_no      = $line['line_no'] + 1;
    $marker_name  = $line['marker_name'];
    $marker_locus = $line['marker_locus'];

    // Checks the marker name.
    if (array_key_exists($marker_name, $GLOBALS['dup_uniquename'])) {
      $msg = "Error : You have the duplicated marker name or the same marker " .
          "name ($marker_name) with different organism at line $line_no";
      $this->updateMsg('E', $msg, TRUE, $line['line_no'] - 1);
    }
    else {
      $GLOBALS['dup_uniquename'][$marker_name]++;
    }

    // Checks the locus names.
    if ($marker_locus) {
      $loci = preg_split("/[;,]/", $marker_locus, -1, PREG_SPLIT_NO_EMPTY);
      foreach ($loci as $locus_name) {
        $tmp []= $locus_name;
        if (array_key_exists($locus_name, $GLOBALS['dup_uniquename'])) {
          $msg = "Error : You have the duplicated locus name or the same " .
              "locus name ($locus_name) with different organism at line $line_no";
          $this->updateMsg('E', $msg, TRUE, $line['line_no'] - 1);
          $err_flag = TRUE;
        }
        else {
          $GLOBALS['dup_uniquename'][$locus_name]++;
        }
      }
    }
  }

  /**
   * @see MCL_TEMPLATE::preProcessECD()
   */
  public function preProcessECD() {

    // Initializes $GLOBALS. Keeps for marker_name and marker_locus columns.
    $GLOBALS['dup_uniquename'] = array();
    return TRUE;
  }

  /**
   * @see MCL_TEMPLATE::preProcessDU()
   */
  public function preProcessDU() {

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
