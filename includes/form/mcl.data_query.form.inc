<?php
/**
 * @file
 */
/**
 * MCL data query form.
 *
 * @param $form
 * @param $form_state
 */
function mcl_data_query_form($form, &$form_state, $type = NULL) {
  global $user;

  // Sets the maximum rows.
  $max_rows = 50;

  // Gets MCL_USER.
  $mcl_user = MCL_USER::byKey(array('user_id' => $user->uid));

  // Creates a form.
  $form = array();
  $form['#tree'] = TRUE;

  // Saves the type.
  $form['query_type'] = array(
    '#type'   => 'value',
    '#value'  => $type,
  );

  // Adds data query links.
  $form['data_query_links'] = array(
    '#markup' => mcl_form_get_data_query_links(),
  );

  // Data query.
  $form['data_query'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => 'Data Query : ' . $type,
  );
  $disabled = FALSE;

  // TYPE : Cvterm.
  if ($type == 'cvterm') {
    $form['data_query']['cvterm_names'] = array(
      '#type'       => 'textarea',
      '#title'      => 'cvterm names',
      '#rows'       => 3,
      '#attributes' => array('style' => 'width:400px;'),
    );
    $form['data_query']['cvterm_ids'] = array(
      '#type'       => 'textarea',
      '#title'      => 'cvterm IDs',
      '#rows'       => 3,
      '#attributes' => array('style' => 'width:400px;'),
    );
  }

  // TYPE : Feature.
  else if ($type == 'feature') {
    $form['data_query']['uniquename'] = array(
      '#type'       => 'textfield',
      '#title'      => 'feature name',
      '#attributes' => array('style' => 'width:400px;'),
    );
    $form['data_query']['feature_id'] = array(
      '#type'       => 'textfield',
      '#title'      => 'feature ID',
      '#attributes' => array('style' => 'width:400px;'),
    );
  }

  // TYPE : Stock.
  else if ($type == 'stock') {
    $form['data_query']['uniquename'] = array(
      '#type'       => 'textfield',
      '#title'      => 'stock name',
      '#attributes' => array('style' => 'width:400px;'),
    );
    $form['data_query']['stock_id'] = array(
      '#type'       => 'textfield',
      '#title'      => 'stock ID',
      '#attributes' => array('style' => 'width:400px;'),
    );
  }

  // TYPE : Organism.
  else if ($type == 'organism') {
    $form['data_query']['genus'] = array(
      '#type'       => 'textfield',
      '#title'      => 'genus',
      '#attributes' => array('style' => 'width:400px;'),
    );
    $form['data_query']['species'] = array(
      '#type'       => 'textfield',
      '#title'      => 'species',
      '#attributes' => array('style' => 'width:400px;'),
    );
    $form['data_query']['organism_id'] = array(
      '#type'       => 'textfield',
      '#title'      => 'organism ID',
      '#attributes' => array('style' => 'width:400px;'),
    );
  }

  // TYPE : Contact.
  else if ($type == 'contact') {
    $form['data_query']['name'] = array(
      '#type'       => 'textfield',
      '#title'      => 'contact mame',
      '#attributes' => array('style' => 'width:400px;'),
    );
    $form['data_query']['contact_id'] = array(
      '#type'       => 'textfield',
      '#title'      => 'contact ID',
      '#attributes' => array('style' => 'width:400px;'),
    );
  }

  // TYPE : Publication.
  else if ($type == 'pub') {
    $form['data_query']['title'] = array(
      '#type'       => 'textfield',
      '#title'      => 'title',
      '#attributes' => array('style' => 'width:400px;'),
    );
    $form['data_query']['pub_id'] = array(
      '#type'       => 'textfield',
      '#title'      => 'pub ID',
      '#attributes' => array('style' => 'width:400px;'),
    );
  }

  // TYPE : Invalid data type.
  else {
    $form['data_query']['invalid'] = array(
      '#markup' => "<p>Invalid data type '<em>$type</em></p>",
    );
    $disabled = TRUE;
  }

  // Adds the search button.
  $form['data_query']['search_btn'] = array(
    '#type'       => 'button',
    '#value'      => 'Search',
    '#name'       => 'search_btn',
    '#suffix'     => '<br /><br />',
    '#disabled'   => $disabled,
    '#attributes' => array('style' => 'width:100px;'),
    '#ajax'       => array(
      'callback' => "mcl_data_query_form_ajax_callback",
      'wrapper'  => 'mcl-data-query-form',
      'effect'   => 'fade',
      'method'   => 'replace',
    ),
  );

  // Adds form elements for search results.
  if (!$disabled && array_key_exists('values', $form_state)) {
    $query_type = $form_state['values']['query_type'];

    // Calls function to display the results.
    $func_name = '_mcl_get_result_' . $query_type;
    if (function_exists($func_name)) {
      call_user_func_array($func_name, array(&$form, $form_state['values']['data_query'], $max_rows));
     }
    else {
      $form['data_query']['invalid'] = array(
        '#markup' => '<b>invalid query type : <em>$query_type</em></b>',
      );
    }
  }

  // Sets form properties.
  $form['#prefix'] = '<div id="mcl-data-query-form">';
  $form['#suffix'] = '</div>';
  return $form;
}

/**
 * Ajax function which returns the form via ajax.
 *
 * @param $form
 * @param $form_state
 */
function mcl_data_query_form_ajax_callback($form, &$form_state) {
  return $form;
}

/**
 * Returns the results.
 *
* @param array $form
* @param array $form_state
* @param integer $max_rows
 */
function _mcl_get_result_cvterm(&$form, $form_state, $max_rows) {

  // Gets cvterms by name or ID.
  $cvterm_names = trim($form_state['cvterm_names']);
  $cvterm_ids   = trim($form_state['cvterm_ids']);
  $cvterms = array();
  if ($cvterm_names) {
    $tmp = preg_split("/\n/", $cvterm_names, -1, PREG_SPLIT_NO_EMPTY);
    foreach ($tmp as $cvterm_name) {
      $sql = "
        SELECT C.cvterm_id, C.name AS cvterm_name, CV.cv_id, CV.name AS cv_name
        FROM chado.cvterm C
          INNER JOIN chado.cv CV on CV.cv_id = C.cv_id
        WHERE LOWER(C.name) = LOWER(:name)
      ";
      $results = db_query($sql, array(':name' => $cvterm_name));
      while ($obj = $results->fetchObject()) {
        $key = $obj->cvterm_name. $obj->cvterm_id;
        $cvterms[] = array($obj->cvterm_id, $obj->cvterm_name, $obj->cv_id, $obj->cv_name);
      }
    }
  }
  if ($cvterm_ids) {
    $tmp = preg_split("/\n/", $cvterm_ids, -1, PREG_SPLIT_NO_EMPTY);
    foreach ($tmp as $cvterm_id) {
      $sql = "
        SELECT C.cvterm_id, C.name AS cvterm_name, CV.cv_id, CV.name AS cv_name
        FROM chado.cvterm C
          INNER JOIN chado.cv CV on CV.cv_id = C.cv_id
        WHERE C.cvterm_id = :cvterm_id
      ";
      $obj = db_query($sql, array(':cvterm_id' => $cvterm_id))->fetchObject();
      if ($obj) {
        $key = $obj->cvterm_name. $obj->cvterm_id;
        $cvterms[] = array($obj->cvterm_id, $obj->cvterm_name, $obj->cv_id, $obj->cv_name);
      }
    }
  }

  // Sorts the cvterms.
  ksort($cvterms);

  // Lists cvterms.
  $rows = array();
  foreach ((array)$cvterms as $key => $row) {
    $rows []= $row;
  }

  // Creates the cvterm table.
  $form['data_query']['results']['cvterm'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => 'cvterms',
  );
  $table_vars = array(
    'header'      => array('cvterm ID', 'cvterm name', 'cv ID', 'cv name'),
    'rows'        => $rows,
    'empty'       => 'No matched cvterm found',
    'attributes'  => array(),
  );
  $form['data_query']['results']['cvterm']['table'] = array(
    '#markup' => theme('table', $table_vars),
  );
}

/**
 * Returns the results.
 *
* @param array $form
* @param array $form_state
* @param integer $max_rows
 */
function _mcl_get_result_feature(&$form, $form_state, $max_rows) {

  // Gets the features.
  $uniquename = trim($form_state['uniquename']);
  $feature_id = trim($form_state['feature_id']);
  $features = array();
  if ($uniquename) {
    $sql = "
      SELECT F.feature_id, F.uniquename, O.genus, O.species, C.name AS type,
        CV.name AS cv_name
      FROM chado.feature F
        INNER JOIN chado.organism O on O.organism_id = F.organism_id
        INNER JOIN chado.cvterm C on C.cvterm_id = F.type_id
        INNER JOIN chado.cv CV on CV.cv_id = C.cv_id
      WHERE LOWER(F.uniquename) = LOWER(:uniquename)
    ";
    $results = db_query($sql, array(':uniquename' => $uniquename));
    while ($obj = $results->fetchObject()) {
      $features[$obj->feature_id] = $obj;
    }
  }
  if ($feature_id) {
    $sql = "
      SELECT F.feature_id, F.uniquename, O.genus, O.species, C.name AS type,
        CV.name AS cv_name
      FROM chado.feature F
        INNER JOIN chado.organism O on O.organism_id = F.organism_id
        INNER JOIN chado.cvterm C on C.cvterm_id = F.type_id
        INNER JOIN chado.cv CV on CV.cv_id = C.cv_id
      WHERE feature_id = :feature_id
    ";
    $results = db_query($sql, array(':feature_id' => $feature_id));
    while ($obj = $results->fetchObject()) {
      $features[$obj->feature_id] = $obj;
    }
  }

  // Adds a fieldset for the feature.
  $form['data_query']['results']['feature'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => "Feature",
  );

  // Checks the features.
  if (empty($features)) {
    $form['data_query']['results']['feature']['table'] = array(
      '#markup' => "<em>No matched feature found in the feature table.</em>",
    );
    return;
  }

  // Searches for related tables for each feature.
  foreach ($features as $feature_id => $feature) {
    $uniquename = $feature->uniquename;
    $genus      = $feature->genus;
    $species    = $feature->species;
    $type       = $feature->type;
    $cv_name    = $feature->cv_name;

  // Adds a fieldset for the feature.
   $desc = "
    <div style='margin-left:10px;'>
      <b>feature ID :</b> $feature_id<br />
      <b>Genus :</b> $genus<br />
      <b>Species :</b> $species<br />
      <b>Type :</b> $type ($cv_name)<br />
    </div>
  ";
    $form['data_query']['results']['feature'][$feature_id] = array(
      '#type'         => 'fieldset',
      '#collapsed'    => TRUE,
      '#collapsible'  => TRUE,
      '#title'        => "$uniquename [$feature_id]",
      '#description'  => $desc,
    );

    $sql = "SET SEARCH_PATH TO chado, public;";
    db_query($sql);

    // Checks feature_id for the related tables.
    $rel_tables = MCL_CHADO_FEATURE::getRelTable();
    $no_rel_table = TRUE;
    $count = 1;
    foreach ($rel_tables['feature_id'] as $table_info) {
      $fk_table   = $table_info['table'];
      $fk_attr    = $table_info['attr'];
      $fieldset = $fk_table . $count++;

      // Gets all foreign keys for all attributes of this table.
      $classs_name = 'CHADO_' . strtoupper($fk_table);
      $fkeys = $classs_name::getFK();

      // Checks if feature_id exists in the related table.
      $sql = "SELECT COUNT($fk_attr) FROM chado.$fk_table WHERE $fk_attr = :feature_id";
      $num = db_query($sql, array(':feature_id' => $feature_id))->fetchField();
      if ($num) {
        $no_rel_table = FALSE;
        $form['data_query']['results']['feature'][$feature_id][$fieldset] = array(
          '#type'         => 'fieldset',
          '#collapsed'    => TRUE,
          '#collapsible'  => TRUE,
          '#title'        => "$fk_table ($num)",
          '#description'  => "<div style='margin:10px;'><b>Foreign Key : $fk_attr</b></div>",
        );

        // Default message.
        $markup = "$fk_table has more than the maximum rows ($max_rows) to be displayed.";

        // Creates a data table only if the number of the rows is less than 50.
        if ($num < $max_rows) {
          $sql = "SELECT * FROM chado.$fk_table WHERE $fk_attr = :feature_id";
          $results = db_query($sql, array(':feature_id' => $feature_id));
          $tables = '';
          while ($arr = $results->fetch(PDO::FETCH_ASSOC)) {
            $rows = array();
            foreach ($arr as $key => $value) {

              // Updates the value (add more info).
              $red_star = '';
              if (array_key_exists($key, $fkeys)) {
                $red_star = '<span style="color:red">*</span> ';
                $info = CHADO_TABLE::mapID($fkeys[$key], $value);
                if ($info) {
                  $value .= " [$info]";
                }
              }
              $rows []= array($red_star . $key, $value);
            }
            $table_vars = array(
              'header'      => array('attr', 'value'),
              'rows'        => $rows,
              'attributes'  => array(),
            );
            $tables .= theme('table', $table_vars);
          }
          $markup = $tables;
        }
        $form['data_query']['results']['feature'][$feature_id][$fieldset]['tables'] = array(
          '#markup' => $markup,
        );
      }
    }
    if ($no_rel_table) {
      $form['data_query']['results']['feature'][$feature_id]['tables'] = array(
        '#markup' => '<div style="margin-top:15px;"><em>No data found in the related tables.</em></div>',
      );
    }
  }
}

/**
 * Returns the results.
 *
* @param array $form
* @param array $form_state
* @param integer $max_rows
 */
function _mcl_get_result_stock(&$form, $form_state, $max_rows) {

  // Gets the stocks.
  $uniquename = trim($form_state['uniquename']);
  $stock_id   = trim($form_state['stock_id']);
  $stocks = array();
  if ($uniquename) {
    $sql = "
      SELECT S.stock_id, S.uniquename, O.genus, O.species, C.name AS type,
        CV.name AS cv_name
      FROM chado.stock S
        INNER JOIN chado.organism O on O.organism_id = S.organism_id
        INNER JOIN chado.cvterm C on C.cvterm_id = S.type_id
        INNER JOIN chado.cv CV on CV.cv_id = C.cv_id
      WHERE LOWER(S.uniquename) = LOWER(:uniquename)
    ";
    $results = db_query($sql, array(':uniquename' => $uniquename));
    while ($obj = $results->fetchObject()) {
      $stocks[$obj->stock_id] = $obj;
    }
  }
  if ($stock_id) {
    $sql = "
      SELECT S.stock_id, S.uniquename, O.genus, O.species, C.name AS type,
        CV.name AS cv_name
      FROM chado.stock S
        INNER JOIN chado.organism O on O.organism_id = S.organism_id
        INNER JOIN chado.cvterm C on C.cvterm_id = S.type_id
        INNER JOIN chado.cv CV on CV.cv_id = C.cv_id
      WHERE S.stock_id = :stock_id
    ";
    $results = db_query($sql, array(':stock_id' => $stock_id));
    while ($obj = $results->fetchObject()) {
      $stocks[$obj->stock_id] = $obj;
    }
  }

  // Adds a fieldset for the stock.
  $form['data_query']['results']['stock'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => 'Stock',
  );

  // Checks the stocks.
  if (empty($stocks)) {
    $form['data_query']['results']['stock']['table'] = array(
      '#markup' => "<em>No matched stock found in the stock table.</em>",
    );
    return;
  }

  // Searches for related tables for each stock.
  foreach ($stocks as $stock_id => $stock) {
    $uniquename = $stock->uniquename;
    $genus      = $stock->genus;
    $species    = $stock->species;
    $type       = $stock->type;
    $cv_name    = $stock->cv_name;

    // Adds a fieldset for the stock.
    $desc = "
      <div style='margin-left:10px;'>
        <b>stock ID :</b> $stock_id<br />
        <b>Uniquename :</b> $uniquename<br />
        <b>Genus :</b> $genus<br />
        <b>Species :</b> $species<br />
        <b>Type :</b> $type ($cv_name)<br />
      </div>
    ";
    $form['data_query']['results']['stock'][$stock_id] = array(
      '#type'         => 'fieldset',
      '#collapsed'    => TRUE,
      '#collapsible'  => TRUE,
      '#title'        => "$uniquename [$stock_id]",
      '#description'  => $desc,
    );

    // Checks stock for the related tables.
    $rel_tables = MCL_CHADO_STOCK::getRelTable();
    $no_rel_table = TRUE;
    $count = 1;
    foreach ($rel_tables['stock_id'] as $table_info) {
      $fk_table = $table_info['table'];
      $fk_attr  = $table_info['attr'];
      $fieldset = $fk_table . $count++;

      // Gets all foreign keys for all attributes of this table.
      $classs_name = 'CHADO_' . strtoupper($fk_table);
      $fkeys = $classs_name::getFK();

      // Checks if stock_id exists in the related table.
      $sql = "SELECT COUNT($fk_attr) FROM chado.$fk_table WHERE $fk_attr = :stock_id";
      $num = db_query($sql, array(':stock_id' => $stock_id))->fetchField();

      // Creates fieldset.
      if ($num) {
        $no_rel_table = FALSE;
        $form['data_query']['results']['stock'][$stock_id][$fieldset] = array(
          '#type'         => 'fieldset',
          '#collapsed'    => TRUE,
          '#collapsible'  => TRUE,
          '#title'        => "$fk_table ($num)",
          '#description'  => "<div style='margin:10px;'><b>Foreign Key : $fk_attr</b></div>",
        );

        // Default message.
        $markup = "$fk_table has more than the maximum rows ($max_rows) to be displayed.";

        // Creates a data table only if the number of the rows is less than 50.
        if ($num < $max_rows) {
          $sql = "SELECT * FROM chado.$fk_table WHERE $fk_attr = :stock_id";
          $results = db_query($sql, array(':stock_id' => $stock_id));
          $tables = '';
          while ($arr = $results->fetch(PDO::FETCH_ASSOC)) {
            $rows = array();
            foreach ($arr as $key => $value) {

              // Updates the value (add more info).
              $red_star = '';
              if (array_key_exists($key, $fkeys)) {
                $red_star = '<span style="color:red">*</span> ';
                $info = CHADO_TABLE::mapID($fkeys[$key], $value);
                if ($info) {
                  $value .= " [$info]";
                }
              }
              $rows []= array($red_star . $key, $value);
            }
            $table_vars = array(
              'header'      => array('attr', 'value'),
              'rows'        => $rows,
              'attributes'  => array(),
            );
            $tables .= theme('table', $table_vars);
          }
          $markup = $tables;
        }
        $form['data_query']['results']['stock'][$stock_id][$fieldset]['tables'] = array(
          '#markup' => $markup,
        );
      }
    }
    if ($no_rel_table) {
      $form['data_query']['results']['stock'][$stock_id]['tables'] = array(
        '#markup' => '<div style="margin-top:15px;"><em>No data found in the related tables.</em></div>',
      );
    }
  }
}

/**
 * Returns the results.
 *
 * @param array $form
 * @param array $form_state
 * @param integer $max_rows
 */
function _mcl_get_result_organism(&$form, $form_state, $max_rows) {

  // Gets the organisms.
  $genus        = trim($form_state['genus']);
  $species      = trim($form_state['species']);
  $organism_id  = trim($form_state['organism_id']);
  $organisms    = array();
  if ($genus && $species) {
    $sql = "
      SELECT O.organism_id, O.genus, O.species
      FROM chado.organism O
      WHERE LOWER(O.genus) = LOWER(:genus) AND LOWER(O.species) = LOWER(:species)
    ";
    $args = array(
      ':genus'    => $genus,
      ':species'  => $species,
    );
    $results = db_query($sql, $args);
    while ($obj = $results->fetchObject()) {
      $organisms[$obj->organism_id] = $obj;
    }
  }
  if ($organism_id) {
    $sql = "
      SELECT O.organism_id, O.genus, O.species
      FROM chado.organism O
      WHERE O.organism_id = :organism_id
    ";
    $results = db_query($sql, array(':organism_id' => $organism_id));
    while ($obj = $results->fetchObject()) {
      $organisms[$obj->organism_id] = $obj;
    }
  }

  // Adds a fieldset for the organism.
  $form['data_query']['results']['organism'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => 'Organism',
  );

  // Checkes the organisms.
  if (empty($organisms)) {
    $form['data_query']['results']['organism']['table'] = array(
      '#markup' => "<em>No matched organism found in the organism table.</em>",
    );
    return;
  }

  // Searches for related tables for each organism.
  foreach ($organisms as $organism_id => $organism) {
    $genus    = $organism->genus;
    $species  = $organism->species;

    // Adds a fieldset for the organism.
    $desc = "
      <div style='margin-left:10px;'>
        <b>organism ID :</b> $organism_id<br />
        <b>Genus :</b> $genus<br />
        <b>Species :</b> $species<br />
      </div>
    ";
    $form['data_query']['results']['organism'][$organism_id] = array(
      '#type'         => 'fieldset',
      '#collapsed'    => TRUE,
      '#collapsible'  => TRUE,
      '#title'        => "Organism ID : $organism_id",
      '#description'  => $desc,
    );

    // Checks organism for the related tables.
    $rel_tables = MCL_CHADO_ORGANISM::getRelTable();
    $no_rel_table = TRUE;
    $count = 1;
    foreach ($rel_tables['organism_id'] as $table_info) {
      $fk_table = $table_info['table'];
      $fk_attr  = $table_info['attr'];
      $fieldset = $fk_table . $count++;

      // Gets all foreign keys for all attributes of this table.
      $classs_name = 'CHADO_' . strtoupper($fk_table);
      $fkeys = $classs_name::getFK();

      // Checks if contact_id exists in the related table.
      $sql = "SELECT COUNT($fk_attr) FROM chado.$fk_table WHERE $fk_attr = :organism_id";
      $num = db_query($sql, array(':organism_id' => $organism_id))->fetchField();

      // Creates fieldset.
      if ($num) {
        $no_rel_table = FALSE;
        $form['data_query']['results']['organism'][$organism_id][$fieldset] = array(
          '#type'         => 'fieldset',
          '#collapsed'    => TRUE,
          '#collapsible'  => TRUE,
          '#title'        => "$fk_table ($num)",
          '#description'  => "<div style='margin:10px;'><b>Foreign Key : $fk_attr</b></div>",
        );

        // Default message.
        $markup = "$fk_table has more than the maximum rows ($max_rows) to be displayed.";

        // Creates a data table only if the number of the rows is less than 50.
        if ($num < $max_rows) {
          $sql = "SELECT * FROM chado.$fk_table WHERE $fk_attr = :organism_id";
          $results = db_query($sql, array(':organism_id' => $organism_id));
          $tables = '';
          while ($arr = $results->fetch(PDO::FETCH_ASSOC)) {
            $rows = array();
            foreach ($arr as $key => $value) {

              // Updates the value (add more info).
              $red_star = '';
              if (array_key_exists($key, $fkeys)) {
                $red_star = '<span style="color:red">*</span> ';
                $info = CHADO_TABLE::mapID($fkeys[$key], $value);
                if ($info) {
                  $value .= " [$info]";
                }
              }
              $rows []= array($red_star . $key, $value);
            }
            $table_vars = array(
              'header'      => array('attr', 'value'),
              'rows'        => $rows,
              'attributes'  => array(),
            );
            $tables .= theme('table', $table_vars);
          }
          $markup = $tables;
        }
        $form['data_query']['results']['organism'][$organism_id][$fieldset]['tables'] = array(
          '#markup' => $markup,
        );
      }
    }
    if ($no_rel_table) {
      $form['data_query']['results']['organism'][$organism_id]['tables'] = array(
        '#markup' => '<div style="margin-top:15px;"><em>No data found in the related tables.</em></div>',
      );
    }
  }
}

/**
 * Returns the results.
 *
* @param array $form
* @param array $form_state
* @param integer $max_rows
 */
function _mcl_get_result_contact(&$form, $form_state, $max_rows) {

  // Gets the contacts.
  $name       = trim($form_state['name']);
  $contact_id = trim($form_state['contact_id']);
  $contacts   = array();
  if ($name) {
    $sql = "
      SELECT CT.contact_id, CT.name, C.name AS type, CV.name AS cv_name
      FROM chado.contact CT
        INNER JOIN chado.cvterm C on C.cvterm_id = CT.type_id
        INNER JOIN chado.cv CV on CV.cv_id = C.cv_id
      WHERE LOWER(CT.name) = LOWER(:name)
    ";
    $results = db_query($sql, array(':name' => $name));
    while ($obj = $results->fetchObject()) {
      $contacts[$obj->contact_id] = $obj;
    }
  }
  if ($contact_id) {
    $sql = "
      SELECT CT.contact_id, CT.name, C.name AS type, CV.name AS cv_name
      FROM chado.contact CT
        INNER JOIN chado.cvterm C on C.cvterm_id = CT.type_id
        INNER JOIN chado.cv CV on CV.cv_id = C.cv_id
      WHERE CT.contact_id = :contact_id
    ";
    $results = db_query($sql, array(':contact_id' => $contact_id));
    while ($obj = $results->fetchObject()) {
      $contacts[$obj->contact_id] = $obj;
    }
  }

  // Adds a fieldset for the contact.
  $form['data_query']['results']['contact'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => 'Contact',
  );

  // Checkes the contacts.
  if (empty($contacts)) {
    $form['data_query']['results']['contact']['table'] = array(
      '#markup' => "<em>No matched contact found in the contact table.</em>",
    );
    return;
  }

  // Searches for related tables for each contact.
  foreach ($contacts as $contact_id => $contact) {
    $name     = $contact->name;
    $type     = $contact->type;
    $cv_name  = $contact->cv_name;

    // Adds a fieldset for the contact.
    $desc = "
      <div style='margin-left:10px;'>
        <b>contact ID :</b> $contact_id<br />
        <b>Name :</b> $name<br />
        <b>Type :</b> $type ($cv_name)<br />
      </div>
    ";
    $form['data_query']['results']['contact'][$contact_id] = array(
      '#type'         => 'fieldset',
      '#collapsed'    => TRUE,
      '#collapsible'  => TRUE,
      '#title'        => "Contact ID : $contact_id",
      '#description'  => $desc,
    );

    // Checks contact for the related tables.
    $rel_tables = MCL_CHADO_CONTACT::getRelTable();
    $no_rel_table = TRUE;
    $count = 1;
    foreach ($rel_tables['contact_id'] as $table_info) {
      $fk_table = $table_info['table'];
      $fk_attr  = $table_info['attr'];
      $fieldset = $fk_table . $count++;

      // Gets all foreign keys for all attributes of this table.
      $classs_name = 'CHADO_' . strtoupper($fk_table);
      $fkeys = $classs_name::getFK();

      // Checks if contact_id exists in the related table.
      $sql = "SELECT COUNT($fk_attr) FROM chado.$fk_table WHERE $fk_attr = :contact_id";
      $num = db_query($sql, array(':contact_id' => $contact_id))->fetchField();

      // Creates fieldset.
      if ($num) {
        $no_rel_table = FALSE;
        $form['data_query']['results']['contact'][$contact_id][$fieldset] = array(
          '#type'         => 'fieldset',
          '#collapsed'    => TRUE,
          '#collapsible'  => TRUE,
          '#title'        => "$fk_table ($num)",
          '#description'  => "<div style='margin:10px;'><b>Foreign Key : $fk_attr</b></div>",
        );

        // Default message.
        $markup = "$fk_table has more than the maximum rows ($max_rows) to be displayed.";

        // Creates a data table only if the number of the rows is less than 50.
        if ($num < $max_rows) {
          $sql = "SELECT * FROM chado.$fk_table WHERE $fk_attr = :contact_id";
          $results = db_query($sql, array(':contact_id' => $contact_id));
          $tables = '';
          while ($arr = $results->fetch(PDO::FETCH_ASSOC)) {
            $rows = array();
            foreach ($arr as $key => $value) {

              // Updates the value (add more info).
              $red_star = '';
              if (array_key_exists($key, $fkeys)) {
                $red_star = '<span style="color:red">*</span> ';
                $info = CHADO_TABLE::mapID($fkeys[$key], $value);
                if ($info) {
                  $value .= " [$info]";
                }
              }
              $rows []= array($red_star . $key, $value);
            }
            $table_vars = array(
              'header'      => array('attr', 'value'),
              'rows'        => $rows,
              'attributes'  => array(),
            );
            $tables .= theme('table', $table_vars);
          }
          $markup = $tables;
        }
        $form['data_query']['results']['contact'][$contact_id][$fieldset]['tables'] = array(
          '#markup' => $markup,
        );
      }
    }
    if ($no_rel_table) {
      $form['data_query']['results']['contact'][$contact_id]['tables'] = array(
        '#markup' => '<div style="margin-top:15px;"><em>No data found in the related tables.</em></div>',
      );
    }
  }
}

/**
 * Returns the publicatoin results.
 *
 * @param array $form
 * @param array $form_state
 * @param integer $max_rows
 *
 * @return boolean
 */
function _mcl_get_result_pub(&$form, $form_state, $max_rows) {

  // Gets all publications by the title and/or pub ID.
  $title  = trim($form_state['title']);
  $pub_id = trim($form_state['pub_id']);
  $pubs   = array();
  if ($title) {
    $sql = "
      SELECT P.*, C.name AS type, CV.name AS cv_name
      FROM chado.pub P
        INNER JOIN chado.cvterm C on C.cvterm_id = P.type_id
        INNER JOIN chado.cv CV on CV.cv_id = C.cv_id
      WHERE P.title ~* '^$title'
    ";
    $results = db_query($sql);
    while ($obj = $results->fetchObject()) {
      $pubs[$obj->pub_id] = $obj;
    }
  }
  if ($pub_id) {
    $sql = "
      SELECT P.*, C.name AS type, CV.name AS cv_name
      FROM chado.pub P
        INNER JOIN chado.cvterm C on C.cvterm_id = P.type_id
        INNER JOIN chado.cv CV on CV.cv_id = C.cv_id
      WHERE P.pub_id = :pub_id
    ";
    $results = db_query($sql, array(':pub_id' => $pub_id));
    while ($obj = $results->fetchObject()) {
      $pubs[$obj->pub_id] = $obj;
    }
  }

  // Checks the publications.
  if (empty($pubs)) {
    $form['data_query']['results']['pub']['table'] = array(
      '#markup' => "<em>No matched publication found in the pub table.</em>",
    );
    return;
  }

  // Searches for related tables for each publication.
  foreach ((array)$pubs as $pub_id => $pub) {
    $title    = $pub->title;
    $type     = $pub->type;
    $cv_name  = $pub->cv_name;

    // Adds a fieldset for the publication.
    $desc = "
      <div style='margin-left:10px;margin-bottom:15px;'>
      <b>Title :</b> $title<br />
      <b>Type :</b> $type ($cv_name)<br />
      </div>
    ";
    $form['data_query']['results']['pub'][$pub_id] = array(
      '#type'         => 'fieldset',
      '#collapsed'    => TRUE,
      '#collapsible'  => TRUE,
      '#title'        => "Pub ID : $pub_id",
      '#description'  => $desc,
    );

    // Checks publication for the related tables.
    $rel_tables = MCL_CHADO_PUB::getRelTable();
    $no_rel_table = TRUE;
    $count = 1;
    $data_lines = '';
    foreach ($rel_tables['pub_id'] as $table_info) {
      $fk_table = $table_info['table'];
      $fk_attr  = $table_info['attr'];
      $fieldset = $fk_table . $count++;

      // Gets all foreign keys for all attributes of this table.
      $classs_name = 'CHADO_' . strtoupper($fk_table);
      $fkeys = $classs_name::getFK();

      // Checks the table.
      $table_name = "chado.$fk_table";
      if (!db_table_exists($table_name)) {
        //drupal_set_message("The table '$table_name' does not exist so it skipped");
        continue;
      }

      // Checks if pub_id exists in the related table.
      $sql = "SELECT COUNT($fk_attr) FROM $table_name WHERE $fk_attr = :pub_id";
      $num = db_query($sql, array(':pub_id' => $pub_id))->fetchField();
      $too_many_rows = ($num > $max_rows) ? TURE : FALSE;

      // Creates fieldset.
      if ($num) {
        $no_rel_table = FALSE;
        $form['data_query']['results']['pub'][$pub_id][$fieldset] = array(
          '#type'         => 'fieldset',
          '#collapsed'    => TRUE,
          '#collapsible'  => TRUE,
          '#title'        => "$fk_table ($num)",
          '#description'  => "<div style='margin:10px;'><b>Foreign Key : $fk_attr</b></div>",
        );

        // Creates a data table only if the number of the rows is less than 50.
        $sql = "SELECT * FROM chado.$fk_table WHERE $fk_attr = :pub_id";
        $results = db_query($sql, array(':pub_id' => $pub_id));
        $tables = '';
        while ($arr = $results->fetch(PDO::FETCH_ASSOC)) {
          $rows     = array();
          $data_arr = array();
          foreach ($arr as $key => $value) {

            // Add a key pair.
            if ($value == '') {
              if (is_int($value)) {
                $data_arr []= "$key : 0";
              }
              else {
                $data_arr []= "$key : ''";
              }
            }
            else {
              $data_arr []= "$key : $value";
            }

            // Updates the value (add more info).
            $red_star = '';
            if (array_key_exists($key, $fkeys)) {
              $red_star = '<span style="color:red">*</span> ';
              $info = CHADO_TABLE::mapID($fkeys[$key], $value);
              if ($info) {
                $value .= " [$info]";
              }
            }
            $rows []= array($red_star . $key, $value);
          }
          $table_vars = array(
            'header'      => array('attr', 'value'),
            'rows'        => $rows,
            'attributes'  => array(),
          );
          $tables .= theme('table', $table_vars);

          // Adds to $data_lines.
          if (!empty($data_arr)) {
            $data_lines .= implode(',', $data_arr) . "\n";
          }
        }
        if ($too_many_rows) {
          $tables = "$fk_table has more than the maximum rows ($max_rows) to be displayed in a table.";
        }
        $form['data_query']['results']['pub'][$pub_id][$fieldset]['tables'] = array(
          '#markup' => $tables,
        );
      }
    }
    if ($no_rel_table) {
      $form['data_query']['results']['pub'][$pub_id]['tables'] = array(
        '#markup' => '<div style="margin-top:15px;"><em>No data found in the related tables.</em></div>',
      );
    }
    else {
      $form['data_query']['results']['pub']['related_data'] = array(
        '#type'         => 'fieldset',
        '#collapsed'    => TRUE,
        '#collapsible'  => TRUE,
        '#title'        => "Data",
        '#description'  => "<em>All related data for pub ID ($pub_id)</em>",
      );
      $form['data_query']['results']['pub']['related_data']['data'] = array(
        '#markup' => "<textarea style='width:100%;height:100px;border:solid lightgray 1px;'>$data_lines</textarea>",
      );
    }
  }
  return TRUE;
}

/**
 * Validates the form.
 *
 * @param $form
 * @param $form_state
 */
function mcl_data_query_form_validate($form, &$form_state) {

  // Gets the trigger element.
  $trigger_elem = $form_state['triggering_element']['#name'];

  // If the 'program_list' was changed.
  if ($trigger_elem == 'search_btn') {
    $type = $form_state['values']['query_type'];

    // Cvterm.
    if ($type == 'cvterm') {
      $cvterm_names = trim($form_state['values']['data_query']['cvterm_names']);
      $cvterm_ids   = trim($form_state['values']['data_query']['cvterm_ids']);
      if (!$cvterm_names && !$cvterm_ids) {
        form_set_error('data_query][cvterm_names', t("Please type cvterm names or IDs"));
        return;
      }
    }

    // Feature.
    else if ($type == 'feature') {
      $uniquename = trim($form_state['values']['data_query']['uniquename']);
      $feature_id = trim($form_state['values']['data_query']['feature_id']);
      if (($uniquename && $feature_id) || (!$uniquename && !$feature_id)) {
        form_set_error('data_query][uniquename', t("Please type uniquename or feature ID"));
        return;
      }
      if ($feature_id && !preg_match("/^\d+$/", $feature_id)) {
        form_set_error('data_query][feature_id', t("Please type an integer for feature ID"));
        return;
      }
    }

    // Stock.
    else if ($type == 'stock') {
      $uniquename = trim($form_state['values']['data_query']['uniquename']);
      $stock_id   = trim($form_state['values']['data_query']['stock_id']);
      if (($uniquename && $stock_id) || (!$uniquename && !$stock_id)) {
        form_set_error('data_query][uniquename', t("Please type uniquename or stock ID"));
        return;
      }
      if ($stock_id && !preg_match("/^\d+$/", $stock_id)) {
        form_set_error('data_query][stock_id', t("Please type an integer for stock ID"));
        return;
      }
    }

    // Organism.
    else if ($type == 'organism') {
      $genus        = trim($form_state['values']['data_query']['genus']);
      $species      = trim($form_state['values']['data_query']['species']);
      $organism_id  = trim($form_state['values']['data_query']['organism_id']);
      if (!$organism_id && !($genus && $species)) {
        form_set_error('data_query][name', t("Please type genus and species or organism ID"));
        return;
      }
      if ($organism_id && !preg_match("/^\d+$/", $organism_id)) {
        form_set_error('data_query][contact_id', t("Please type an integer for organism ID"));
        return;
      }
    }

    // Contact.
    else if ($type == 'contact') {
      $name       = trim($form_state['values']['data_query']['name']);
      $contact_id = trim($form_state['values']['data_query']['contact_id']);
      if (($name && $contact_id) || (!$name && !$contact_id)) {
        form_set_error('data_query][name', t("Please type name or contact ID"));
        return;
      }
      if ($contact_id && !preg_match("/^\d+$/", $contact_id)) {
        form_set_error('data_query][contact_id', t("Please type an integer for contact ID"));
        return;
      }
    }

    // Publication.
    else if ($type == 'pub') {
      $title  = trim($form_state['values']['data_query']['title']);
      $pub_id = trim($form_state['values']['data_query']['pub_id']);
      if (($title && $pub_id) || (!$title && !$pub_id)) {
        form_set_error('data_query][title', t("Please type title or pub ID"));
        return;
      }
      if ($pub_id && !preg_match("/^\d+$/", $pub_id)) {
        form_set_error('data_query][pub_id', t("Please type an integer for pub ID"));
        return;
      }
    }
  }
}

/**
 * Submits the form.
 *
 * @param $form
 * @param $form_state
 */
function mcl_data_query_form_submit($form, &$form_state) {}
