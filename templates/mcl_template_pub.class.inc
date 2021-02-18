<?php
/**
 * The declaration of MCL_TEMPLATE_PUB class.
 *
 */
class MCL_TEMPLATE_PUB extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'PUB';
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
      "Academic Dissertations",
      "Book",
      "Book Chapter",
      "Biography",
      "Case Reports",
      "Clinical Trial",
      "Clinical Trial, Phase I",
      "Clinical Trial, Phase II",
      "Clinical Trial, Phase III",
      "Conference Proceedings",
      "Comparative Study",
      "Comment",
      "Controlled Clinical Trial",
      "Editorial",
      "English Abstract",
      "Evaluation Studies",
      "Historical Article",
      "In Vitro",
      "Journal",
      "Journal Article",
      "Letter",
      "Meta-Analysis",
      "Multicenter Study",
      "News",
      "Patent",
      "Portraits",
      "Proceedings Article",
      "Published Erratum",
      "Randomized Controlled Trial",
      "Research Support, Non-U.S. Gov't",
      "Research Support, N.I.H., Intramural",
      "Research Support, N.I.H., Extramural",
      "Research Support, U.S. Gov't, P.H.S.",
      "Research Support, U.S. Gov't, Non-P.H.S.",
      "Retraction of Publication",
      "Review",
      "Technical Report",
      "Thesis",
      "Twin Study",
      "Validation Studies",
    );
    $pub_type = array(
      'cv_name'     => 'tripal_pub',
      'values'      => $values,
      'description' => 'Types for a pub',
    );
    return array('pub_type' => $pub_type);
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
      'authors'         => array('req' => FALSE, 'width' => 10, 'desc' => "The list of the authors of this publication, separated by comma."),
      'title'           => array('req' => FALSE, 'width' => 10, 'desc' => "The title of the publication."),
      'type'            => array('req' => TRUE,  'width' => 10, 'desc' => "The type of the publication."),
      'journal_name'    => array('req' => FALSE, 'width' => 10, 'desc' => "The name of journal (Maxium 255 charactrers)."),
      'journal_abb'     => array('req' => FALSE, 'width' => 10, 'desc' => "The abbreviation of the journal."),
      'journal_code'    => array('req' => FALSE, 'width' => 10, 'desc' => "The code of the journal."),
      'journal_country' => array('req' => FALSE, 'width' => 10, 'desc' => "The published country of the journal."),
      'series_name'     => array('req' => FALSE, 'width' => 10, 'desc' => "The name of series."),
      'citation'        => array('req' => FALSE, 'width' => 10, 'desc' => "Enter the full citation for this publication or leave blank and one will be generated automatically if possible. For PubMed style citations list the last name of the author followed by initials. Each author should be separated by a comma. Next comes the title, followed by the series title (e.g. journal name), publication date (4 digit year, 3 character Month, day), volume, issue and page numbers. You may also use HTML to provide a link in the citation."),
      'alias'           => array('req' => FALSE, 'width' => 10, 'desc' => "Alias of the publication."),
      'pub_date'        => array('req' => FALSE, 'width' => 10, 'desc' => "The date the work was published. This should be in the format: YYYY Mon Day, where YYYY is a 4 digit year, Mon is a 3-letter English abbreviation for the month followed by the day."),
      'pub_year'        => array('req' => FALSE, 'width' => 10, 'desc' => "The year the work was published."),
      'epub_date'       => array('req' => FALSE, 'width' => 10, 'desc' => "The date the work was published. This should be in the format: YYYY Mon Day, where YYYY is a 4 digit year, Mon is a 3-letter English abbreviation for the month followed by the day."),
      'publisher'       => array('req' => FALSE, 'width' => 10, 'desc' => "The name of the publisher."),
      'language'        => array('req' => FALSE, 'width' => 10, 'desc' => "The language of the publication electrically."),
      'doi'             => array('req' => FALSE, 'width' => 10, 'desc' => "DOI (Digital Object Identifier) of the publication."),
      'url'             => array('req' => FALSE, 'width' => 10, 'desc' => "URL of the publication."),
      'pmid'            => array('req' => FALSE, 'width' => 10, 'desc' => "PMID (PubMed IDentifier). It is an unique identifier number used in PubMed."),
      'issn'            => array('req' => FALSE, 'width' => 10, 'desc' => "ISSN of the publicaion (Maxium 255 charactrers)."),
      'eissn'           => array('req' => FALSE, 'width' => 10, 'desc' => "eISSN of the publicaion (Maxium 255 charactrers)."),
      'pissn'           => array('req' => FALSE, 'width' => 10, 'desc' => "pISSN of the publicaion (Maxium 255 charactrers)."),
      'isbn'            => array('req' => FALSE, 'width' => 10, 'desc' => "ISBN of the publicaion (Maxium 255 charactrers)."),
      'conference_name' => array('req' => FALSE, 'width' => 10, 'desc' => "The name of conference."),
      'volume'          => array('req' => FALSE, 'width' => 10, 'desc' => "The serives volume number. (Maxium 255 charactrers)."),
      'volume_title'    => array('req' => FALSE, 'width' => 10, 'desc' => "The volume title of the publication. (Maxium 255 charactrers)."),
      'issue'           => array('req' => FALSE, 'width' => 10, 'desc' => "The series issue number. (Maxium 255 charactrers)."),
      'pages'           => array('req' => FALSE, 'width' => 10, 'desc' => "The page numbers for the publication. (e.g.) 2-23"),
      'keywords'        => array('req' => FALSE, 'width' => 10, 'desc' => "The keywords for the publication."),
      'comments'        => array('req' => FALSE, 'width' => 10, 'desc' => "The comments for the publication.."),
      'abstract'        => array('req' => FALSE, 'width' => 10, 'desc' => "The abstract of the publication."),
      '##property_name' => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns (##) : followed by name (cvterm name) of the property."),
    );

    // Adds labels.
    $headers['doi']['label']    = 'DOI';
    $headers['url']['label']    = 'URL';
    $headers['pmid']['label']   = 'PMID';
    $headers['isbn']['label']   = 'ISBN';
    $headers['issn']['label']   = 'ISSN';
    $headers['eissn']['label']  = 'eISSN';
    $headers['pissn']['label']  = 'pISSN';
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['tripal_pub']['language']              = -1;
    $cvterms['tripal_pub']['publication type']      = -1;
    $cvterms['tripal_pub']['journal name']          = -1;
    $cvterms['tripal_pub']['journal code']          = -1;
    $cvterms['tripal_pub']['journal abbreviation']  = -1;
    $cvterms['tripal_pub']['doi']                   = -1;
    $cvterms['tripal_pub']['authors']               = -1;
    $cvterms['tripal_pub']['alias']                 = -1;
    $cvterms['tripal_pub']['publication code']      = -1;
    $cvterms['tripal_pub']['journal code']          = -1;
    $cvterms['tripal_pub']['journal country']       = -1;
    $cvterms['tripal_pub']['accepted date']         = -1;
    $cvterms['tripal_pub']['conference name']       = -1;
    $cvterms['tripal_pub']['isbn']                  = -1;
    $cvterms['tripal_pub']['issn']                  = -1;
    $cvterms['tripal_pub']['eissn']                 = -1;
    $cvterms['tripal_pub']['pissn']                 = -1;
    $cvterms['tripal_pub']['epub date']             = -1;
    $cvterms['tripal_pub']['publisher']             = -1;
    $cvterms['tripal_pub']['url']                   = -1;
    $cvterms['tripal_pub']['keywords']              = -1;
    $cvterms['tripal_pub']['comments']              = -1;
    $cvterms['tripal_pub']['abstract']              = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks DB for GRIN.
    MCL_CHADO_DB::checkDB($this, 'PMID');

    // Checks the authors.
    MCL_CHADO_PUB::checkAuthors($this, $line['authors']);

    // Checks the publication date.
    MCL_CHADO_PUB::checkPubDate($this, $line['pub_date']);

    // Checks for the length of the data.
    $this->checkLength(255, 'journal_name', $line['journal_name']);
    $this->checkLength(255, 'volume', $line['volume']);
    $this->checkLength(255, 'issue', $line['issue']);
    $this->checkLength(255, 'pages', $line['pages']);
    $this->checkLength(255, 'pub_year', $line['pub_year']);

    // Checks for publication type.
    if (!MCL_DATA_VALID_TYPE::validate('pub_type', $line['type'])) {
      $this->updateMsg('E', 'Invalid pub_type : ' . $line['type']);
    }
  }

    /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the citation.
    $citation = MCL_CHADO_PUB::getCitation($this, $line);

    // Gets the type ID for this publication.
    $type_id = MCL_CHADO_CVTERM::getCvterm('tripal_pub', $line['type'])->getCvtermID();

    // Adds a publication.
    $details = array(
      'uniquename'    => $citation,
      'title'         => $line['title'],
      'type_id'       => $type_id,
      'volume_title'  => $line['volume_title'],
      'volume'        => $line['volume'],
      'series_name'   => $line['series_name'],
      'issue'         => $line['issue'],
      'pyear'         => $line['pub_year'],
      'pages'         => $line['pages'],
    );
    $pub = MCL_CHADO_PUB::addPub($this, $details);
    if ($pub) {

      // Adds the authors.
      $pub->addAuthors($this, $line['authors'], "[,]");

      // Gets and add the PMID.
      $db = MCL_CHADO_DB::getDB('PMID');
      $pub->addDB($this, $db, $line['pmid'], $line['pmid']);

      // Adds the publication date.
      $pub->addPubDate($this, $line['pub_date']);

      // Adds the properties.
      $pub->addProp($this, 'tripal_pub', 'authors', $line['authors']);
      $pub->addProp($this, 'tripal_pub', 'language', $line['language']);
      $pub->addProp($this, 'tripal_pub', 'journal name', $line['journal_name']);
      $pub->addProp($this, 'tripal_pub', 'journal code', $line['journal_code']);
      $pub->addProp($this, 'tripal_pub', 'journal country', $line['journal_country']);
      $pub->addProp($this, 'tripal_pub', 'journal abbreviation', $line['journal_abb']);
      $pub->addProp($this, 'tripal_pub', 'doi', $line['doi']);
      $pub->addProp($this, 'tripal_pub', 'url', $line['url']);
      $pub->addProp($this, 'tripal_pub', 'citation', $citation);
      $pub->addProp($this, 'tripal_pub', 'alias', $line['alias']);
      $pub->addProp($this, 'tripal_pub', 'publication code', $line['pub_code']);
      $pub->addProp($this, 'tripal_pub', 'accepted date', $line['accepted_date']);
      $pub->addProp($this, 'tripal_pub', 'conference name', $line['conference_name']);
      $pub->addProp($this, 'tripal_pub', 'isbn', $line['isbn']);
      $pub->addProp($this, 'tripal_pub', 'issn', $line['issn']);
      $pub->addProp($this, 'tripal_pub', 'eissn', $line['eissn']);
      $pub->addProp($this, 'tripal_pub', 'pissn', $line['pissn']);
      $pub->addProp($this, 'tripal_pub', 'epub date', $line['epub_date']);
      $pub->addProp($this, 'tripal_pub', 'publisher', $line['publisher']);
      $pub->addProp($this, 'tripal_pub', 'keywords', $line['keywords']);
      $pub->addProp($this, 'tripal_pub', 'comments', $line['comments']);
      $pub->addProp($this, 'tripal_pub', 'abstract', $line['abstract']);

      // Adds the user-specific properties.
      $this->addSpecialColumns('##', $pub, $line);
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
