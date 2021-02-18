<?php
/**
 * The declaration of MCL_TEMPLATE_DELETE_ACCESSION_BIMS class.
 *
 */
class MCL_TEMPLATE_DELETE_ACCESSION_BIMS extends MCL_TEMPLATE {

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
    $desc = 'The description for delete stock sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'stock_name'  => array('req' => FALSE,  'width' => 10, 'desc' => "name of the stock to be deleted."),
      'genus'       => array('req' => FALSE,  'width' => 10, 'desc' => "genus of the stock to be deleted."),
      'species'     => array('req' => FALSE,  'width' => 10, 'desc' => "species of the stock to be deleted."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Gets the job paremters.
    $program_id = $this->getJob()->getParamByKey('program_id');

    // Gets the prefix.
    $prefix = BIMS_PROGRAM::getPrefixByID($program_id);

    // Gets BIMS_CHADO table.
    $bc_accession = new BIMS_CHADO_ACCESSION($program_id);

    // Gets FB required columns.
    $bims_program = BIMS_PROGRAM::byID($program_id);
    $bims_cols    = $bims_program->getBIMSCols();

    // Checks accession.
    $accession = $line[strtolower($bims_cols['accession'])];
    $bc_accession->checkAccession($this, $prefix . $accession, $line['genus'], $line['species']);

    // Checks organism.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the job paremters.
    $program_id = $this->getJob()->getParamByKey('program_id');

    // Gets the prefix.
    $prefix = BIMS_PROGRAM::getPrefixByID($program_id);

    // Gets BIMS_CHADO table.
    $bc_accession = new BIMS_CHADO_ACCESSION($program_id);

    // Gets FB required columns.
    $bims_program = BIMS_PROGRAM::byID($program_id);
    $bims_cols    = $bims_program->getBIMSCols();

    // Gets the organism.
    $organism = MCL_CHADO_ORGANISM::getOrganism($line['genus'], $line['species']);
    $organism_id = $organism->getOrganismID();

    // Gets the accession
    $accession    = $line[strtolower($bims_cols['accession'])];
    $accession_l  = $bims_program->getBIMSLabel('accession');
    $args = array(
      'uniquename'  => $prefix . $accession,
      'organism_id' => $organism_id,
    );
    $accession = $bc_accession->byTKey('accession', $args);

    // Deletes the accession.
    if ($accession) {
      $bc_accession->setStockID($accession->stock_id);
      if ($bc_accession->deleteAccession()) {
        $this->updateMsg('L', "The $accession_l has been deleted ($accession)");
      }
      else {
        $this->updateMsg('E', "Failed to delete the $accession_l ($accession)");
      }
    }
  }
}