<?php
/**
 * The declaration of MCL_CHADO_PUB class.
 *
 */
class MCL_CHADO_PUB extends CHADO_PUB {

 /**
  *  Class data members.
  */

  /**
   * @see CHADO_PUB::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_PUB::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * @see CHADO_PUB::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * @see CHADO_PUB::delete()
   */
  public function delete() {

    // Deletes the dbxref.
    $sql = "SELECT dbxref_id FROM chado.pub_dbxref WHERE pub_id = :pub_id";
    $args = array(':pub_id' => $this->pub_id);
    $results = db_query($sql, $args);
    while ($dbxref_id = $results->fetchField()) {
      $dbxref = CHADO_DBXREF::byKey(array('dbxref_id' => $dbxref_id));
      if (!$dbxref->delete()) {
        return FALSE;
      }
    }

    // Deletes this publication.
    return parent::delete();
  }

  /**
   * Checks the type of a publication.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $author
   * @param string $separator
   *
   * @return boolean
   */
  public static function checkAuthors(MCL_TEMPLATE $mcl_tmpl = NULL, $authors, $separator = ',') {
    if ($authors) {

      // Parses out the the authors.
      $author_list = preg_split(self::getSepRegex($separator), $authors, -1, PREG_SPLIT_NO_EMPTY);
      $rank = 0;
      foreach ($author_list as $author) {
        $author = trim($author);

        // Splits the author to givenames and surname and checks the names.
        $tmp = explode(' ', $author, 2);
        $mcl_tmpl->checkLength(100, 'surname', $tmp[0]);
        $mcl_tmpl->checkLength(100, 'givennames', $tmp[1]);
      }
    }
    return TRUE;
  }

  /**
   * Checks the date of a publication. The date the work was published.
   * This should be in the format: YYYY Mon Day, where YYYY is a 4 digit year,
   * Mon is a 3-letter English abbreviation for the month followed by the day.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $pub_date
   *
   * @return boolean
   */
  public static function checkPubDate(MCL_TEMPLATE $mcl_tmpl = NULL, $pub_date) {
    if ($pub_date) {

      // Parses out the the date.
      $tmp = preg_split("/\s+/", $pub_date, -1, PREG_SPLIT_NO_EMPTY);
      if (sizeof($tmp) != 3) {
        $mcl_tmpl->updateMsg('E', "Error : The invalid date format. This should be in the format YYYY Mon Day. (e.g.) 2014 JAN 1.");
        return FALSE;
      }
      if (!preg_match("/^\d\d\d\d$/", $tmp[0])) {
        $mcl_tmpl->updateMsg('E', "Error : The invalid year format. A year must be a 4 digit year.");
        return FALSE;
      }
      if (!preg_match("/^(jan|feb|mar|apr|may|jun|jul|aug|sep|oct|nov|dec)$/i", $tmp[1])) {
        $mcl_tmpl->updateMsg('E', "Error : The invalid month format. A year month be a 3-letter English abbreviation for the month. (e.g.) June is 'JUN'.");
        return FALSE;
      }
      if (!preg_match("/^\d?\d$/", $tmp[2])) {
        $mcl_tmpl->updateMsg('E', "Error : The invalid day format. A day must be an intger.");
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Checks the pages of a publication.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $start
   * @param string $end
   *
   * @return boolean
   */
  public static function checkPages(MCL_TEMPLATE $mcl_tmpl = NULL, $start, $end) {

    // Checks the ending page.
    if ($end || $end == '0') {
      if ($start == '') {
        self::updateMsg($mcl_tmpl, 'E', "The starting page is missing");
        return FALSE;
      }
      else if ($start > $end) {
        self::updateMsg($mcl_tmpl, 'E', "The ending page number should be bigger than the staring page number");
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Returns the formatted pages.
   *
   * @param string $start
   * @param string $end
   * @param string $separator
   *
   * @return string
   */
  public static function getPagesFormatted($start, $end, $separator = '-') {

    // Checks the ending page.
    if ($end || $end == '0') {
      if ($start || $start == '0') {
        if ($start == $end) {
          return $start;
        }
        return "$start $separator $end";
      }
    }
    else if ($start || $start == '0') {
      return $start;
    }
    return '';
  }

  /**
   * Checks the existence of db. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $reference
   * @param string $separator
   * @param boolean $warning
   *
   * @return boolean
   */
  public static function checkPubID(MCL_TEMPLATE $mcl_tmpl = NULL, $reference, $separator = '', $warning = FALSE) {
    $flag = TRUE;
    if ($reference) {

      // Checks the pub.
      $pub_ids = preg_split(self::getSepRegex($separator), $reference, -1, PREG_SPLIT_NO_EMPTY);
      foreach ($pub_ids as $pub_id) {
        $pub_id = trim($pub_id);
        if (!preg_match("/^\d+$/", $pub_id)) {
          self::updateMsg($mcl_tmpl, 'E', "pub_id is not integer");
          $flag = FALSE;
        }
        else {
          $pub = MCL_CHADO_PUB::byID($pub_id);
          if (!$pub) {
            if ($warning) {
              self::updateMsg($mcl_tmpl, 'W', "(pub_id) = ($pub_id) not found in pub");
            }
            else {
              self::updateMsg($mcl_tmpl, 'E', "(pub_id) = ($pub_id) not found in pub");
              $flag = FALSE;
            }
          }
        }
      }
    }
    return $flag;
  }

  /**
   * Returns the pub by ID.
   *
   * @param integer pub_id
   *
   * @return MCL_CHADO_PUB
   */
  public static function byID($pub_id) {
    return MCL_CHADO_PUB::byKey(array('pub_id' => $pub_id));
  }

  /**
   * Returns the pub by name.
   *
   * @param string $uniquename
   *
   * @return MCL_CHADO_PUB
   */
  public static function byName($pub_id) {
    return MCL_CHADO_PUB::byKey(array('uniquename' => $uniquename));
  }

  /**
   * Adds a publication.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param array $details
   *
   * @return MCL_CHADO_PUB
   */
  public static function addPub(MCL_TEMPLATE $mcl_tmpl = NULL, $details) {

    // Gets the uniquename.
    $uniquename = $details['uniquename'];

    // Sets the arguments.
    $args = array('uniquename' => $uniquename);

    // Checks for duplication.
    $pub = MCL_CHADO_PUB::byKey($args);
    if ($pub) {
      self::addMsg($mcl_tmpl, 'D', 'pub', $args);
    }
    else {

      // Adds the properties.
      foreach ($details as $key => $value) {
        if ($value) {
          $args[$key] = $value;
        }
      }

      // Adds a new pub.
      $pub = new MCL_CHADO_PUB($args);
      if ($pub->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'pub', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'pub', $args);
        return NULL;
      }
    }
    return $pub;
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
      if ($cvterm) {
        $type_id = $cvterm->getCvtermID();
        return $this->addProperty($mcl_tmpl, 'pubprop', 'pub_id', $this->pub_id, $type_id, $value, $separator);
      }
    }
    return TRUE;
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
      return $this->addProperty($mcl_tmpl, 'pubprop', 'pub_id', $this->pub_id, $type_id, $value, $separator);
    }
    return TRUE;
  }

  /**
   * Removes a property.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $cv_name
   * @param string $cvterm_name
   *
   * @return boolean
   */
  public function removeProp(MCL_TEMPLATE $mcl_tmpl = NULL, $cv_name, $cvterm_name) {
    $cvterm = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name);
    if ($cvterm) {
      $type_id = $cvterm->getCvtermID();
      return $this->removeProperty($mcl_tmpl, 'pubprop', 'pub_id', $this->pub_id, $type_id);
    }
    return TRUE;
  }

  /**
   * Adds null pub.
   *
   * @return MCL_CHADO_PUB
   */
  public static function addNull() {

    // Gets null cvterm.
    $cvterm = MCL_CHADO_CVTERM::addNull();

    // Checks if it already exists.
    $pub = MCL_CHADO_PUB::byName('null');
    if (!$pub) {
      $details = array(
        'uniquename'  => 'null',
        'type_id'     => $cvterm->getCvtermID(),
      );
      $pub = new MCL_CHADO_PUB($details);
      if (!$pub->insert()) {
        return NULL;
      }
    }
    return $pub;
  }

  /**
   * Adds PMID.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_DB $db
   * @param string $accession
   *
   * @return boolean
   */
  public function addDB(MCL_TEMPLATE $mcl_tmpl = NULL, MCL_CHADO_DB $db, $accession) {
    if ($accession) {

      // Adds a dbxref.
      $dbxref = MCL_CHADO_DBXREF::addDBXref($mcl_tmpl, $db->getDbID(), $accession);
      if (!$dbxref) {
        return FALSE;
      }

      // Adds dbxref to stock_dbxref table.
      return $this->addDBXref($mcl_tmpl, $dbxref);
    }
    return TRUE;
  }

  /**
   * Adds a dbxref to stock_dbxref.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_DBXREF $dbxref
   *
   * @return boolean
   */
  public function addDBXref(MCL_TEMPLATE $mcl_tmpl = NULL, MCL_CHADO_DBXREF $dbxref) {
    if ($dbxref) {
      return $this->addLink($mcl_tmpl, 'pub_dbxref', 'pub_id', $this->pub_id, 'dbxref_id', $dbxref->getDbxrefID());
    }
    return TRUE;
  }

  /**
   * Adds the authors.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $author
   * @param string $separator
   */
  public function addAuthors(MCL_TEMPLATE $mcl_tmpl = NULL, $authors, $separator = '') {
    if ($authors) {

      // Parses out the the authors.
      $author_list = preg_split(self::getSepRegex($separator), $authors, -1, PREG_SPLIT_NO_EMPTY);
      $rank = 0;
      foreach ($author_list as $author) {
        $this->addAuthor($mcl_tmpl, trim($author), $rank++);
      }
    }
  }

  /**
   * Adds an author.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $author
   * @param integer $rank
   */
  public function addAuthor(MCL_TEMPLATE $mcl_tmpl = NULL, $author, $rank = 0) {

    // Sets the arguments.
    $args = array(
      'pub_id' => $this->pub_id,
      'rank'   => $rank,
    );

    // Checks for duplication.
    $pubauthor = CHADO_PUBAUTHOR::byKey($args);
    if ($pubauthor) {
      self::addMsg($mcl_tmpl, 'D', 'pubauthor', $args);
    }
    else {

      // Splits the author to givenames and surname.
      $tmp = explode(' ', $author, 2);
      $args['surname']    = trim($tmp[0]);
      $args['givennames'] = trim($tmp[1]);

      // Adds a new pubauthor.
      $pubauthor = new CHADO_PUBAUTHOR($args);
      if ($pubauthor->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'pubauthor', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'pubauthor', $args);
        return NULL;
      }
    }
    return $pubauthor;
  }

  /**
   * Adds the publication date.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $pub_date
   */
  public function addPubDate(MCL_TEMPLATE $mcl_tmpl = NULL, $pub_date) {
    if ($pub_date) {

      // Parses out the the date.
      $tmp = preg_split("/\s+/", $pub_date, -1, PREG_SPLIT_NO_EMPTY);
      $year = trim($tmp[0]);
      $mon  = trim($tmp[1]);
      $day  = trim($tmp[2]);

      // Adds the publication.
      $this->addProp($mcl_tmpl, 'tripal_pub', 'publication date', "$year $mon $day");
    }
  }

  /**
   * Returns the type of the publication.
   *
   * @return string
   */
  public function getType() {
    $cvterm = MCL_CHADO_CVTERM::byID($this->type_id);
    if ($cvterm) {
      return $cvterm->getName();
    }
    return '';
  }

  /**
   * Adds a Citation. If not provide, generate a citation. It does not
   * generate the one if any of the followings is missing.
   * For PubMed style citations list the last name of the author followed by initials.
   * Each author should be separated by a comma. Next comes the title, followed
   * by the series title (e.g. journal name), publication date (4 digit year,
   * 3 character Month, day), volume, issue and page numbers.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param array $line
   *
   * @return string
   */
  public static function getCitation(MCL_TEMPLATE $mcl_tmpl = NULL, $line) {
    $citation = $line['citation'];
    if (!$citation) {

      // Gets the type of publication.
      $type = strtolower($line['type']);
      if ($type == 'journal') {
        $citation = self::getCitationJ($mcl_tmpl, $line);
      }
      else if ($type == 'book') {
        $citation = self::getCitationB($mcl_tmpl, $line);
      }
      else if ($type == 'journal article') {
        $citation = self::getCitationJA($mcl_tmpl, $line);
      }
      else if ($type == 'review') {
        $citation = self::getCitationReview($mcl_tmpl, $line);
      }
      else if (preg_match("/^research support/", $type)) {
        $citation = self::getCitationRS($mcl_tmpl, $line);
      }
      else if ($type == 'letter') {
        $citation = self::getCitationLetter($mcl_tmpl, $line);
      }
      else if ($type == 'conference proceedings') {
        $citation = self::getCitationCP($mcl_tmpl, $line);
      }
      else {
        $citation = self::getCitationDefault($mcl_tmpl, $line);
      }
    }

    // Adds the period at the end if not.
    $citation = trim($citation);
    if (!preg_match("/\.$/", $citation)) {
      $citation .= '.';
    }
    return $citation;
  }


  /**
   * Helper function to create a citation for 'Journal'.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param array $line
   *
   * @return string
   */
  private static function getCitationJ(MCL_TEMPLATE $mcl_tmpl = NULL, $line) {

    // Generates the citation.
    $citation = '';
    if ($line['authors']) {
      $citation .= $line['authors'] . '. ';
    }
    if ($line['journal_name']) {
      $citation .= $line['journal_name'] . '. ';
    }
    if ($line['journal_abb']) {
      $citation .= $line['journal_abb'] . '. ';
    }
    if ($line['series_name']) {
      $citation .= $line['series_name'] . '. ';
    }
    if ($line['series_abb']) {
      $citation .= $line['series_abb'] . '. ';
    }
    if ($line['pub_date']) {
      $citation .= $line['pub_date'] . ' ';
    }
    if ($line['pub_year']) {
      $citation .= $line['pub_year'] . ' ';
    }
    if ($line['volume'] || $line['issue'] || $line['pages']) {
      $citation .= '; ';
    }
    if ($line['volume']) {
      $citation .= $line['volume'];
    }
    if ($line['issue']) {
      $citation .= '(' . $line['issue'] . ')';
    }
    if ($line['pages']) {
      if ($line['volume']) {
        $citation .= ':';
      }
      $citation .= $line['pages'];
    }
    return $citation;
  }

  /**
   * Helper function to create a citation for 'Book'.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param array $line
   *
   * @return string
   */
  private static function getCitationB(MCL_TEMPLATE $mcl_tmpl = NULL, $line) {

    // Generates the citation.
    $citation = '';
    if ($line['authors']) {
      $citation .= $line['authors'] . '. ';
    }
    if ($line['title']) {
      $citation .= $line['title'] . '. ';
    }
    if ($line['journal_abb']) {
      $citation .= $line['journal_abb'] . '. ';
    }
    if ($line['series_name']) {
      $citation .= $line['series_name'] . '. ';
    }
    if ($line['series_abb']) {
      $citation .= $line['series_abb'] . '. ';
    }
    if ($line['pub_date']) {
      $citation .= $line['pub_date'] . ' ';
    }
    if ($line['pub_year']) {
      $citation .= $line['pub_year'];
    }
    if ($line['volume'] || $line['issue'] || $line['pages']) {
      $citation .= '; ';
    }
    if ($line['volume']) {
      $citation .= $line['volume'];
    }
    if ($line['issue']) {
      $citation .= '(' . $line['issue'] . ')';
    }
    if ($line['pages']) {
      if ($line['volume']) {
        $citation .= ':';
      }
      $citation .= $line['pages'];
    }
    return $citation;
  }

  /**
   * Helper function to create a citation for 'Journal Article'.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param array $line
   *
   * @return string
   */
  private static function getCitationJA(MCL_TEMPLATE $mcl_tmpl = NULL, $line) {

    // Generates the citation.
    $citation = '';
    if ($line['authors']) {
      $citation .= $line['authors'] . '. ';
    }
    $citation .= $line['title'] . '. ';
    if ($line['journal_name']) {
      $citation .= $line['journal_name'] . '. ';
    }
    if ($line['journal_abb']) {
      $citation .= $line['journal_abb'] . '. ';
    }
    if ($line['series_name']) {
      $citation .= $line['series_name'] . '. ';
    }
    if ($line['series_abb']) {
      $citation .= $line['series_abb'] . '. ';
    }
    if ($line['pub_date']) {
      $citation .= $line['pub_date'] . ' ';
    }
    $citation .= $line['pub_year'];
    if ($line['volume'] || $line['issue'] || $line['pages']) {
      $citation .= '; ';
    }
    if ($line['volume']) {
      $citation .= $line['volume'];
    }
    if ($line['issue']) {
      $citation .= '(' . $line['issue'] . ')';
    }
    if ($line['pages']) {
      if ($line['volume']) {
        $citation .= ':';
      }
      $citation .= $line['pages'];
    }
    return $citation;
  }

  /**
   * Helper function to create a citation for 'Review'.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param array $line
   *
   * @return string
   */
  private static function getCitationReview(MCL_TEMPLATE $mcl_tmpl = NULL, $line) {

    // Generates the citation.
    $citation = '';
    if ($line['authors']) {
      $citation .= $line['authors'] . '. ';
    }
    $citation .= $line['title'] . '. ';
    if ($line['journal_name']) {
      $citation .= $line['journal_name'] . '. ';
    }
    if ($line['journal_abb']) {
      $citation .= $line['journal_abb'] . '. ';
    }
    if ($line['series_name']) {
      $citation .= $line['series_name'] . '. ';
    }
    if ($line['series_abb']) {
      $citation .= $line['series_abb'] . '. ';
    }
    if ($line['pub_date']) {
      $citation .= $line['pub_date'] . ' ';
    }
    $citation .= $line['pub_year'];
    if ($line['volume'] || $line['issue'] || $line['pages']) {
      $citation .= '; ';
    }
    if ($line['volume']) {
      $citation .= $line['volume'];
    }
    if ($line['issue']) {
      $citation .= '(' . $line['issue'] . ')';
    }
    if ($line['pages']) {
      if ($line['volume']) {
        $citation .= ':';
      }
      $citation .= $line['pages'];
    }
    return $citation;
  }

  /**
   * Helper function to create a citation for 'Research Support'.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $authors
   * @param string $title
   * @param array $line
   *
   * @return string
   */
  private static function getCitationRS(MCL_TEMPLATE $mcl_tmpl = NULL, $line) {

    // Generates the citation.
    $citation = '';
    if ($line['authors']) {
      $citation .= $line['authors'] . '. ';
    }
    $citation .= $line['title'] . '. ';
    if ($line['journal_name']) {
      $citation .= $line['journal_name'] . '. ';
    }
    if ($line['pub_date']) {
      $citation .= $line['pub_date'] . ' ';
    }
    $citation .= $line['pub_year'];
    return $citation;
  }

  /**
   * Helper function to create a citation for 'Letter'.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $authors
   * @param string $title
   * @param array $line
   *
   * @return string
   */
  private static function getCitationLetter(MCL_TEMPLATE $mcl_tmpl = NULL, $line) {

    // Generates the citation.
    $citation = '';
    if ($line['authors']) {
      $citation .= $line['authors'] . '. ';
    }
    $citation .= $line['title'] . '. ';
    if ($line['journal_name']) {
      $citation .= $line['journal_name'] . '. ';
    }
    if ($line['journal_abb']) {
      $citation .= $line['journal_abb'] . '. ';
    }
    if ($line['series_name']) {
      $citation .= $line['series_name'] . '. ';
    }
    if ($line['series_abb']) {
      $citation .= $line['series_abb'] . '. ';
    }
    if ($line['pub_date']) {
      $citation .= $line['pub_date'] . ' ';
    }
    $citation .= $line['pub_year'];
    if ($line['volume'] || $line['issue'] || $line['pages']) {
      $citation .= '; ';
    }
    if ($line['volume']) {
      $citation .= $line['volume'];
    }
    if ($line['issue']) {
      $citation .= '(' . $line['issue'] . ')';
    }
    if ($line['pages']) {
      if ($line['volume']) {
        $citation .= ':';
      }
      $citation .= $line['pages'];
    }
    return $citation;
  }

  /**
   * Helper function to create a citation for 'Conference Proceedings'.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $authors
   * @param string $title
   * @param array $line
   *
   * @return string
   */
  private static function getCitationCP(MCL_TEMPLATE $mcl_tmpl = NULL, $line) {

    // Generates the citation.
    $citation = '';
    if ($line['authors']) {
      $citation .= $line['authors'] . '. ';
    }
    $citation .= $line['title'] . '. ';
    if ($line['conference_name']) {
      $citation .= $line['conference_name'] . '. ';
    }
    if ($line['series_name']) {
      $citation .= $line['series_name'] . '. ';
    }
    if ($line['series_abb']) {
      $citation .= $line['series_abb'] . '. ';
    }
    if ($line['pub_date']) {
      $citation .= $line['pub_date'] . ' ';
    }
    $citation .= $line['pub_year'];
    if ($line['volume'] || $line['issue'] || $line['pages']) {
      $citation .= '; ';
    }
    if ($line['volume']) {
      $citation .= $line['volume'];
    }
    if ($line['issue']) {
      $citation .= '(' . $line['issue'] . ')';
    }
    if ($line['pages']) {
      if ($line['volume']) {
        $citation .= ':';
      }
      $citation .= $line['pages'];
    }
    return $citation;
  }

  /**
   * Helper function to create a default citation.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $authors
   * @param string $title
   * @param array $line
   *
   * @return string
   */
  private static function getCitationDefault(MCL_TEMPLATE $mcl_tmpl = NULL, $line) {

    // Generates the citation.
    $citation = '';
    if ($line['authors']) {
      $citation .= $line['authors'] . '. ';
    }
    $citation .= $line['title'] . '. ';
    if ($line['series_name']) {
      $citation .= $line['series_name'] . '. ';
    }
    if ($line['series_abb']) {
      $citation .= $line['series_abb'] . '. ';
    }
    if ($line['pub_date']) {
      $citation .= $line['pub_date'] . ' ';
    }
    $citation .= $line['pub_year'];
    if ($line['volume'] || $line['issue'] || $line['pages']) {
      $citation .= '; ';
    }
    if ($line['volume']) {
      $citation .= $line['volume'];
    }
    if ($line['issue']) {
      $citation .= '(' . $line['issue'] . ')';
    }
    if ($line['pages']) {
      if ($line['volume']) {
        $citation .= ':';
      }
      $citation .= $line['pages'];
    }
    return $citation;
  }

  /**
   * @see CHADO_TABLE::getMergeUniqueKeys()
   */
  public function getMergeUniqueKeys($chado_table) {

    // Sets unique keys for pubprop.
    if ($chado_table == 'chado.pubprop') {
      $ukey_keywords = array(
        'field' => 'type_id',
        'value' => MCL_CHADO_CVTERM::getCvterm('SITE_CV', 'keywords')->getCvtermID(),
        'ukeys' => array('pub_id', 'type_id', 'value'),
      );
      return array($ukey_keywords);
    }
    else if ($chado_table == 'chado.featuremap_pub') {
      $ukey_featuremap_pub = array(
        'ukeys' => array('featuremap_id', 'pub_id'),
      );
      return array($ukey_featuremap_pub);
    }
    return array();
  }

  /**
   * Merges the publications.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_PUB $pub
   * @param boolean $verbose
   */
  public function mergePub(MCL_TEMPLATE $mcl_tmpl = NULL, MCL_CHADO_PUB $pub, $verbose = FALSE) {

    // Skips the merging on the following tables.
    $skip_tables = array('pubauthor');

    // Sets the information of the publications.
    $params = array(
      'label'   => 'PUB',
      'id_base' => $this->pub_id,
      'id'      => $pub->getPubID(),
    );

    // Gets all reference tables.
    $ref_tables = MCL_CHADO_PUB::getRelTable();
    foreach ((array)$ref_tables['pub_id'] as $ref_table) {

      // Skips the tables.
      if (in_array($ref_table['table'], $skip_tables)) {
        if ($verbose) {
          mcl_print($ref_table['table'] . " is skipped", 1, 1);
        }
        continue;
      }

      // Processes the merge.
      if (!$this->procMerge($mcl_tmpl, $params, $ref_table, $verbose)) {
        break;
      }
    }
  }
}