<?php
/**
 * @file
 */
/**
 * MCL data info form.
 *
 * @param $form
 * @param $form_state
 */
function mcl_data_info_form($form, &$form_state, $type = NULL) {
  global $user;

  // Sets the maximum rows.
  $max_rows = 50;

  // Gets MCL_USER.
  $mcl_user = MCL_USER::byKey(array('user_id' => $user->uid));

  // Gets the value from the $form_state.
  $value = '';
  if (array_key_exists('values', $form_state)) {

    // TYPE : trait.
    if ($type == 'trait') {
      $value = $form_state['values']['data_info']['trait']['trait_ontology'];
    }
    // TYPE : descriptor.
    else if ($type == 'descriptor') {
      $value = $form_state['values']['data_info']['descriptor']['trait_descriptor_set'];
    }
  }

  // Creates a form.
  $form = array();
  $form['#tree'] = TRUE;

  // Adds data info links.
  $form['data_info_links'] = array(
    '#markup' => mcl_form_get_data_info_links(),
  );

  // Data info.
  $form['data_info'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => 'Data Information : ' . $type,
  );

  // Trait.
  if ($type == 'trait') {
    _mcl_data_info_form_trait($form['data_info']['trait'], $value);
  }

  // Trait.
  if ($type == 'descriptor') {
    _mcl_data_info_form_descriptor($form['data_info']['descriptor'], $value);
  }

  // Invalid data type.
  else {
    $form['data_info']['invalid'] = array(
      '#markup' => 'Invalid data type',
    );
  }

  // Sets form properties.
  $form['#prefix'] = '<div id="mcl-data-info-form">';
  $form['#suffix'] = '</div>';
  return $form;
}

/**
 * Ajax function which returns the form via ajax.
 *
 * @param $form
 * @param $form_state
 */
function mcl_data_info_form_ajax_callback($form, &$form_state) {
  return $form;
}

/**
 * Returns the trait information.
 *
 * @param array $form
 * @param string $def_cv_id
 */
function _mcl_data_info_form_descriptor(&$form, $def_cv_id = '') {

  // Gets 'trait_descriptor_set' group.
  $data_group = MCL_DATA_GROUP::byKey(array('name' => 'trait_descriptor_set'));
  if (!$data_group) {
    $form['no_groop'] = array(
      '#markup' => '<em>No trait descriptor set group found</em>',
    );
    return;
  }

  // Gets the available trait descriptor sets.
  $options = $data_group->getGroupMembers(MCL_OPTION);
  if (!$options) {
    $form['no_groop'] = array(
        '#markup' => '<em>No available trait descriptor set found</em>',
    );
    return;
  }

  // Adds the dropdown for trait descriptor sets.
  if (!$def_cv_id) {
    $def_cv_id = key($options);
  }
  $form['trait_descriptor_set'] = array(
    '#title'          => 'Trait Descriptor Set',
    '#type'           => 'select',
    '#options'        => $options,
    '#default_value'  => $def_cv_id,
    '#attributes'     => array('style' => 'width: 200px;'),
    '#ajax'           => array(
      'callback' => "mcl_data_info_form_ajax_callback",
      'wrapper'  => 'mcl-data-info-form',
      'effect'   => 'fade',
      'method'   => 'replace',
    ),
  );

  // Gets trait information.
  $sql = "
    SELECT CAT.name AS trait_category, CAT.cvterm_id AS cvterm_id_category,
      TRAIT.cvterm_id AS cvterm_id_trait, TRAIT.name AS trait, A.alais,
      AB.value AS trait_abbr, C.name AS descriptor_name,
      C.cvterm_id AS cvterm_id_descriptor, C.definition, F.value AS format
    FROM {chado.cvterm} C
      INNER JOIN {chado.cv} CV on CV.cv_id = C.cv_id
      LEFT JOIN (
        SELECT cvterm_id, value
        FROM {chado.cvtermprop}
        WHERE type_id = (
          SELECT cvterm_id
          FROM {chado.cvterm} C
            INNER JOIN {chado.cv} on CV.cv_id = C.cv_id
          WHERE C.name = :cvterm_format AND cv.name = :cv_main
        )
      ) F on F.cvterm_id = C.cvterm_id
      LEFT JOIN (
        SELECT cvterm_id, string_agg(synonym, ', ') AS alais
        FROM {chado.cvtermsynonym} CS
        GROUP BY cvterm_id
      ) A on A.cvterm_id = C.cvterm_id
      LEFT JOIN (
        SELECT C1.name, C1.cvterm_id, CR.subject_id
        FROM {chado.cvterm} C1
          INNER JOIN {chado.cvterm_relationship} CR on CR.object_id = C1.cvterm_id
          INNER JOIN {chado.cvterm} C2 on C2.cvterm_id = CR.type_id
          INNER JOIN {chado.cv} CV2 on CV2.cv_id = C2.cv_id
        WHERE CV2.name = :cv_main AND C2.name = :cvterm_belongs_to
      ) TRAIT on TRAIT.subject_id = C.cvterm_id
      LEFT JOIN (
        SELECT cvterm_id, value
        FROM {chado.cvtermprop}
        WHERE type_id = (
          SELECT cvterm_id
          FROM {chado.cvterm} C
            INNER JOIN {chado.cv}  on CV.cv_id = C.cv_id
          WHERE C.name = :cvterm_abb AND cv.name = :cv_main)
        ) AB on AB.cvterm_id = TRAIT.cvterm_id
      LEFT JOIN (
        SELECT C1.name, C1.cvterm_id, CR.subject_id
        FROM {chado.cvterm} C1
          INNER JOIN {chado.cvterm_relationship} CR on CR.object_id = C1.cvterm_id
          INNER JOIN {chado.cvterm} C2 on C2.cvterm_id = CR.type_id
          INNER JOIN {chado.cv} CV2 on CV2.cv_id = C2.cv_id
        WHERE CV2.name = :cv_relationship AND C2.name = :cvterm_is_a
      ) CAT on CAT.subject_id = TRAIT.cvterm_id
    WHERE CV.cv_id = :cv_id_tds
    ORDER BY CAT.name, trait, descriptor_name
  ";
  $args = array(
    ':cv_id_tds'          => $def_cv_id,
    ':cv_main'            => 'MAIN',
    ':cv_relationship'    => 'relationship',
    ':cvterm_format'      => 'format',
    ':cvterm_abb'         => 'abbreviation',
    ':cvterm_is_a'        => 'is_a',
    ':cvterm_belongs_to'  => 'belongs_to',
  );
  $results = db_query($sql, $args);
  $rows = array();
  while ($obj = $results->fetchObject()) {
    $category   = ($obj->trait_category) ? $obj->trait_category : '--';
    $trait      = ($obj->trait) ? $obj->trait : '--';
    $rows []= array(
      $category,
      '<em>' . $obj->cvterm_id_category . '</em>',
      $trait,
      '<em>' . $obj->cvterm_id_trait . '</em>',
      $obj->descriptor_name,
      '<em>' . $obj->cvterm_id_descriptor . '</em>',
      $obj->format,
    );
  }

  // Sets the headers.
  $headers = array(
    'Category',
    'cvterm ID (category)',
    'Trait',
    'cvterm ID (trait)',
    'Descriptor',
    'cvterm ID (descriptor)',
    'Format',
  );

  // Creates trait table.
  $table_vars = array(
    'header'      => $headers,
    'rows'        => $rows,
    'empty'       => '<em>No descriptor found for the selected trait descriptor set</em>',
    'attributes'  => array(),
  );
  $form['table'] = array(
    '#markup' => theme('table', $table_vars),
  );
}

/**
 * Returns the trait information.
 *
 * @param array $form
 * @param string $def_cv_id
 */
function _mcl_data_info_form_trait(&$form, $def_cv_id = '') {

  // Gets 'trait_descriptor_set' group.
  $data_group = MCL_DATA_GROUP::byKey(array('name' => 'trait_ontology'));
  if (!$data_group) {
    $form['no_groop'] = array(
      '#markup' => '<em>No trait ontology group found</em>',
    );
    return;
  }

  // Gets the available trait descriptor sets.
  $options = $data_group->getGroupMembers(MCL_OPTION);
  if (!$options) {
    $form['no_groop'] = array(
      '#markup' => '<em>No available trait found</em>',
    );
    return;
  }

  // Adds the dropdown for trait ontology.
  if (!$def_cv_id) {
    $def_cv_id = key($options);
  }
  $form['trait_ontology'] = array(
    '#title'          => 'Trait Ontology',
    '#type'           => 'select',
    '#options'        => $options,
    '#default_value'  => $def_cv_id,
    '#attributes'     => array('style' => 'width: 200px;'),
    '#ajax'           => array(
      'callback' => "mcl_data_info_form_ajax_callback",
      'wrapper'  => 'mcl-data-info-form',
      'effect'   => 'fade',
      'method'   => 'replace',
    ),
  );

  // Gets trait information.
  $sql = "
    SELECT CAT.name AS trait_category, CAT.cvterm_id AS cvterm_id_category,
      TRAIT.cvterm_id AS cvterm_id_trait, TRAIT.name AS trait, A.alais,
      TRAIT.definition, AB.value AS trait_abbr,
      CASE WHEN FNUM.NUM IS NULL THEN 0 ELSE FNUM.NUM END AS associated_features
    FROM {chado.cvterm} TRAIT
      INNER JOIN {chado.cv} CV on CV.cv_id = TRAIT.cv_id
      LEFT JOIN (
        SELECT cvterm_id, string_agg(synonym, ', ') AS alais
        FROM {chado.cvtermsynonym} CS
        GROUP BY cvterm_id
      ) A on A.cvterm_id = TRAIT.cvterm_id
      LEFT JOIN (
        SELECT cvterm_id, value
        FROM {chado.cvtermprop}
        WHERE type_id = (
          SELECT cvterm_id
          FROM {chado.cvterm} C
            INNER JOIN {chado.cv}  on CV.cv_id = C.cv_id
          WHERE C.name = :cvterm_abb AND cv.name = :cv_main)
        ) AB on AB.cvterm_id = TRAIT.cvterm_id
      LEFT JOIN (
        SELECT C1.name, C1.cvterm_id, CR.subject_id
        FROM {chado.cvterm} C1
          INNER JOIN {chado.cvterm_relationship} CR on CR.object_id = C1.cvterm_id
          INNER JOIN {chado.cvterm} C2 on C2.cvterm_id = CR.type_id
          INNER JOIN {chado.cv} CV2 on CV2.cv_id = C2.cv_id
        WHERE CV2.name = :cv_relationship AND C2.name = :cvterm_is_a
      ) CAT on CAT.subject_id = TRAIT.cvterm_id
      LEFT JOIN (
       SELECT COUNT(feature_id) AS NUM, cvterm_id
       FROM {chado.feature_cvterm}
       GROUP BY cvterm_id
      ) FNUM on FNUM.cvterm_id = TRAIT.cvterm_id
    WHERE CV.cv_id = :cv_id_to
    ORDER BY CAT.name, trait
  ";
  $args = array(
    ':cv_id_to'         => $def_cv_id,
    ':cv_main'          => 'MAIN',
    ':cv_relationship'  => 'relationship',
    ':cvterm_abb'       => 'abbreviation',
    ':cvterm_is_a'      => 'is_a',
  );
  $results = db_query($sql, $args);
  $rows = array();
  while ($obj = $results->fetchObject()) {
    $category   = ($obj->trait_category) ? $obj->trait_category : '--';
    $trait      = ($obj->trait) ? $obj->trait : '--';
    $rows []= array(
      $category,
      '<em>' . $obj->cvterm_id_category . '</em>',
      $trait,
      '<em>' . $obj->cvterm_id_trait . '</em>',
      $obj->alais,
      $obj->trait_abbr,
      $obj->definition,
      $obj->associated_features
    );
  }

  // Sets the headers.
  $headers = array(
    'Category',
    'cvterm ID (category)',
    'Trait',
    'cvterm ID (trait)',
    'Alias',
    'Abbreviation',
    'Definition',
    '# of Associated Features',
  );

  // Creates trait table.
  $table_vars = array(
    'header'      => $headers,
    'rows'        => $rows,
    'empty'       => '<em>No trait found for the selected trait ontology</em>',
    'attributes'  => array('style' => 'width:100%'),
  );
  $form['table'] = array(
    '#markup' => theme('table', $table_vars),
  );
}

/**
 * Validates the form.
 *
 * @param $form
 * @param $form_state
 */
function mcl_data_info_form_validate($form, &$form_state) {}

/**
 * Submits the form.
 *
 * @param $form
 * @param $form_state
 */
function mcl_data_info_form_submit($form, &$form_state) {}
