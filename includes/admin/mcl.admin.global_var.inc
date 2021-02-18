<?php
/**
 * Manages MCL global variables.
 *
 * @param array $form
 * @param array $form_state
 *
 * @ingroup mcl_admin
 */
function mcl_admin_global_var_form($form, &$form_state) {

  // Creates a form.
  $form = array();
  $form['#tree'] = TRUE;

  // MCL site variables.
  $form['site_var'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => TRUE,
    '#title'        => 'Site variables',
  );
  $form['site_var']['table'] = array(
    '#markup' => _mcl_get_mcl_site_var_table(TRUE),
  );

  // MCL dummy variables.
  $form['default_var'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => TRUE,
    '#title'        => 'Dummy variables',
  );
  $form['default_var']['table'] = array(
    '#markup' => _mcl_get_mcl_dummy_var_table(FALSE),
  );

  // Sets properties of the form.
  $form['#prefix'] = '<div id="mcl-admin-global-var-form">';
  $form['#suffix'] = '</div>';
  return $form;
}

/**
 * Lists all site variables in a table.
 *
 * @return string
 */
function _mcl_get_mcl_site_var_table() {

  // Gets the site variables.
  $vars = MCL_SITE_VAR::getVars(MCL_OBJECT);
  $rows = array();
  foreach ($vars as $obj) {
    $site_var_id  = $obj->site_var_id;

    // Creates the links.
    $l_edit = l('Edit', "/admin/mcl/site_var/$site_var_id");
    $confirm_attr = array(
      'attributes' => array(
        'id'    => 'delete_site_var_' . $site_var_id,
        'class' => array('use-ajax', 'mcl-confirm'),
      )
    );
    $l_delete = l('Delete', "mcl/delete_site_var/$site_var_id" , $confirm_attr);

    // Updates the value.
    $value = $obj->value;
    if (!$value && $value != 0) {
      $value = '&lt;<em>required</em>&gt;';
    }

    // Adds a variable row.
    $row = array(
      $obj->name,
      $value,
      $obj->description,
      "$l_edit $l_delete",
    );
    $rows[] = $row;
  }
  $table_vars = array(
    'header'      => array('Name', 'Value', 'Description', 'Action'),
    'rows'        => $rows,
    'empty'       => 'No site variable found',
    'attributes'  => array(),
  );
  $link = l('Add a site variable', '/admin/mcl/site_var');
  return "<div>$link</div>" . theme('table', $table_vars);
}

/**
 * Lists all dummy variables in a table.
 *
 * @return string
 */
function _mcl_get_mcl_dummy_var_table() {

  // Gets the site variables.
  $vars = MCL_DUMMY_VAR::getVars(MCL_OBJECT);
  $rows = array();
  foreach ($vars as $obj) {
    $dummy_var_id = $obj->dummy_var_id;

    // Creates the links.
    $l_edit = l('Edit', "/admin/mcl/dummy_var/$dummy_var_id");
    $confirm_attr = array(
      'attributes' => array(
        'id'    => 'delete_dummy_var_' . $dummy_var_id,
        'class' => array('use-ajax', 'mcl-confirm'),
      )
    );
    $l_delete = l('Delete', "mcl/delete_dummy_var/$dummy_var_id" , $confirm_attr);

    // Updates ID.
    $id = $obj->id;
    if (!$id && $id != 0) {
      $id = '&lt;<em>required</em>&gt;';
    }

    // Adds a variable row.
    $row = array(
      $obj->name,
      $id,
      $obj->description,
      "$l_edit $l_delete",
    );
    $rows[] = $row;
  }
  $table_vars = array(
    'header'      => array('Name', 'ID', 'Description', 'Action'),
    'rows'        => $rows,
    'empty'       => 'No dummy variable found',
    'attributes'  => array(),
  );
  $link = l('Add a dummy variable', '/admin/mcl/dummy_var');
  return "<div>$link</div>" . theme('table', $table_vars);
}

/**
 * Ajax function which returns the form via ajax.
 *
 * @param $form
 * @param $form_state
 */
function mcl_admin_global_var_form_ajax_callback($form, &$form_state) {
  return $form;
}

/**
 * Validates the form.
 *
 * @ingroup mcl_admin
 */
function mcl_admin_global_var_form_validate($form, &$form_state) {}

/**
 * Submits the form.
 *
 * @ingroup mcl_admin
 */
function mcl_admin_global_var_form_submit($form, &$form_state) {}

/**
 * The form for the site variable.
 *
 * @param array $form
 * @param array $form_state
 *
 * @ingroup mcl_admin
 */
function mcl_admin_site_var_form($form, &$form_state, $site_var_id = NULL) {

  // Creates a form.
  $form = array();
  $form['#tree'] = TRUE;

  // Site variable.
  $op = $site_var_id ? 'update' : 'add';
  $form['site_var'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => TRUE,
    '#title'        => ucfirst($op) . ' site variable',
  );
  $form['site_var_id'] = array(
    '#type'   => 'value',
    '#value'  => $site_var_id,
  );

  // Lists the properties of the variable.
  $def_name       = '';
  $def_table_name = '';
  $def_key        = '';
  $def_id         = '';
  $def_value      = '';
  $def_desc       = '';
  if ($op == 'update') {
    $mcl_site_var   = MCL_SITE_VAR::byID($site_var_id);
    $def_name       = $mcl_site_var->getName();
    $def_id         = $mcl_site_var->getId();
    $def_table_name = $mcl_site_var->getTableName();
    $def_key        = $mcl_site_var->getKey();
    $def_value      = $mcl_site_var->getValue();
    $def_desc       = $mcl_site_var->getDescription();
  }
  $form['site_var']['name'] = array(
    '#type'           => 'textfield',
    '#title'          => 'Name',
    '#description'    => 'Type name of a variable',
    '#default_value'  => $def_name,
    '#attributes'     => array('style' => 'width: 250px;'),
    '#required'       => TRUE,
  );
  $form['site_var']['table_name'] = array(
    '#type'           => 'textfield',
    '#title'          => 'Table name',
    '#description'    => 'Table name of a variable',
    '#default_value'  => $def_table_name,
    '#attributes'     => array('style' => 'width: 250px;'),
  );
  $form['site_var']['key'] = array(
    '#type'           => 'textfield',
    '#title'          => 'Key',
    '#description'    => 'Key (filed of the table)',
    '#default_value'  => $def_key,
    '#attributes'     => array('style' => 'width: 250px;'),
  );
  $form['site_var']['id'] = array(
    '#type'           => 'textfield',
    '#title'          => 'ID',
    '#description'    => 'ID of a variable',
    '#default_value'  => $def_id,
    '#attributes'     => array('style' => 'width: 250px;'),
  );
  $form['site_var']['value'] = array(
    '#type'           => 'textfield',
    '#title'          => 'Value',
    '#description'    => 'value of a variable',
    '#default_value'  => $def_value,
    '#attributes'     => array('style' => 'width: 250px;'),
    '#required'       => TRUE,
  );
  $form['site_var']['description'] = array(
    '#type'           => 'textarea',
    '#title'          => 'Description',
    '#rows'           => 4,
    '#cols'           => 50,
    '#description'    => 'Description of a variable',
    '#default_value'  => $def_desc,
    '#attributes'     => array('style' => ''),
  );
  $form['site_var']['op_btn'] = array(
    '#type'   => 'submit',
    '#name'   => $op . '_btn',
    '#value'  => strtoupper($op),
  );

  // Sets properties of the form.
  $form['#prefix'] = '<div id="mcl-admin-site-var-form">';
  $form['#suffix'] = '</div>';
  return $form;
}

/**
 * Ajax function which returns the form via ajax.
 *
 * @param $form
 * @param $form_state
 */
function mcl_admin_site_var_form_ajax_callback($form, &$form_state) {
  return $form;
}

/**
 * Validates the form.
 *
 * @ingroup mcl_admin
 */
function mcl_admin_site_var_form_validate($form, &$form_state) {

  // Gets the trigger element.
  $trigger_elem = $form_state['triggering_element']['#name'];

  // Gets the properties.
  $site_var_id  = $form_state['values']['site_var_id'];
  $name         = trim($form_state['values']['site_var']['name']);

  // If 'Update' was clicked.
  if ($trigger_elem == "update_btn") {

    // Checks for duplication.
    $mcl_site_var = MCL_SITE_VAR::byName($name);
    if ($mcl_site_var->getSiteVarID() != $site_var_id) {
      form_set_error('MCL', "The name ($name) has been used. Please change the name");
      return;
    }
  }

  // If 'Add' was clicked.
  else if ($trigger_elem == "add_btn") {

    // Checks for duplication.
    $mcl_site_var = MCL_SITE_VAR::byName($name);
    if ($mcl_site_var) {
      form_set_error('MCL', "The name ($name) has been used. Please change the name");
      return;
    }
  }
}

/**
 * Submits the form.
 *
 * @ingroup mcl_admin
 */
function mcl_admin_site_var_form_submit($form, &$form_state) {

  // Gets the trigger element.
  $trigger_elem = $form_state['triggering_element']['#name'];

  // Gets the properties.
  $site_var_id  = $form_state['values']['site_var_id'];
  $name         = trim($form_state['values']['site_var']['name']);
  $table_name   = trim($form_state['values']['site_var']['table_name']);
  $key          = trim($form_state['values']['site_var']['key']);
  $id           = trim($form_state['values']['site_var']['id']);
  $value        = trim($form_state['values']['site_var']['value']);
  $desc         = trim($form_state['values']['site_var']['description']);

  // If 'Update' was clicked.
  if ($trigger_elem == "update_btn") {

    // Updates the propertes.
    $mcl_site_var = MCL_SITE_VAR::byID($site_var_id);
    $mcl_site_var->setName($name);
    $mcl_site_var->setTableName($table_name);
    $mcl_site_var->setKey($key);
    $mcl_site_var->setId($id);
    $mcl_site_var->setValue($value);
    $mcl_site_var->setDescription($desc);
    if ($mcl_site_var->update()) {
      drupal_set_message("The properties of the site variable have been updated.");
      drupal_goto("admin/mcl/global_var");
    }
    else {
      drupal_set_message("Error : Failed to update the site variable.", 'error');
    }
  }

  // If 'Add' was clicked.
  else if ($trigger_elem == "add_btn") {
    $details = array(
      'name'        => $name,
      'table_name'  => $table_name,
      'key'         => $key,
      'id'          => $id,
      'value'       => $value,
      'description' => $desc,
    );
    if (MCL_SITE_VAR::addVar($details)) {
      drupal_set_message("The new site variable have been added.");
      drupal_goto("admin/mcl/global_var");
    }
    else {
      drupal_set_message("Error : Failed to add the site variable.", 'error');
    }
  }
}

/**
 * The form for the dummy variables.
 *
 * @param array $form
 * @param array $form_state
 *
 * @ingroup mcl_admin
 */
function mcl_admin_dummy_var_form($form, &$form_state, $dummy_var_id = NULL) {

  // Creates a form.
  $form = array();
  $form['#tree'] = TRUE;

  // Dummy variable.
  $op = $dummy_var_id ? 'update' : 'add';
  $form['dummy_var'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => TRUE,
    '#title'        => ucfirst($op) . 'Dummy variable',
  );
  $form['dummy_var_id'] = array(
    '#type'   => 'value',
    '#value'  => $dummy_var_id,
  );

  // Lists the properties of the variable.
  $def_name = '';
  $def_id   = '';
  $def_desc = '';
  if ($op == 'update') {
    $mcl_dummy_var = MCL_DUMMY_VAR::byID($dummy_var_id);
    $def_name = $mcl_dummy_var->getName();
    $def_id   = $mcl_dummy_var->getId();
    $def_desc = $mcl_dummy_var->getDescription();
  }
  $form['dummy_var']['name'] = array(
    '#type'           => 'textfield',
    '#title'          => 'Name',
    '#description'    => 'Name of a variable',
    '#default_value'  => $def_name,
    '#attributes'     => array('style' => 'width: 250px;'),
    '#required'       => TRUE,
  );
  $form['dummy_var']['id'] = array(
    '#type'           => 'textfield',
    '#title'          => 'ID',
    '#description'    => 'ID of a variable',
    '#default_value'  => $def_id,
    '#attributes'     => array('style' => 'width: 250px;'),
    '#required'       => TRUE,
  );
  $form['dummy_var']['description'] = array(
    '#type'           => 'textarea',
    '#title'          => 'Description',
    '#rows'           => 4,
    '#cols'           => 50,
    '#description'    => 'Type description of a variable',
    '#default_value'  => $def_desc,
    '#attributes'     => array('style' => ''),
  );
  $form['dummy_var']['op_btn'] = array(
    '#type'   => 'submit',
    '#name'   => $op . '_btn',
    '#value'  => strtoupper($op),
  );

  // Sets properties of the form.
  $form['#prefix'] = '<div id="mcl-admin-dummy-var-form">';
  $form['#suffix'] = '</div>';
  return $form;
}

/**
 * Ajax function which returns the form via ajax.
 *
 * @param $form
 * @param $form_state
 */
function mcl_admin_dummy_var_form_ajax_callback($form, &$form_state) {
  return $form;
}

/**
 * Validates the form.
 *
 * @ingroup mcl_admin
 */
function mcl_admin_dummy_var_form_validate($form, &$form_state) {

  // Gets the trigger element.
  $trigger_elem = $form_state['triggering_element']['#name'];

  // Gets the properties.
  $dummy_var_id = $form_state['values']['dummy_var_id'];
  $name         = trim($form_state['values']['dummy_var']['name']);

  /// If 'Update' was clicked.
  if ($trigger_elem == "update_btn") {

    // Checks for duplication.
    $mcl_dummy_var = MCL_DUMMY_VAR::byName($name);
    if ($mcl_dummy_var->getDummyVarID() != $dummy_var_id) {
      form_set_error('MCL', "The name ($name) has been used. Please change the name");
      return;
    }
  }

  // If 'Add' was clicked.
  else if ($trigger_elem == "add_btn") {

    // Checks for duplication.
    $mcl_dummy_var = MCL_DUMMY_VAR::byName($name);
    if ($mcl_dummy_var) {
      form_set_error('MCL', "The name ($name) has been used. Please change the name");
      return;
    }
  }
}

/**
 * Submits the form.
 *
 * @ingroup mcl_admin
 */
function mcl_admin_dummy_var_form_submit($form, &$form_state) {

  // Gets the trigger element.
  $trigger_elem = $form_state['triggering_element']['#name'];

  // Gets the properties.
  $dummy_var_id = $form_state['values']['dummy_var_id'];
  $name         = trim($form_state['values']['dummy_var']['name']);
  $id           = trim($form_state['values']['dummy_var']['id']);
  $desc         = trim($form_state['values']['dummy_var']['description']);

  /// If 'Update' was clicked.
  if ($trigger_elem == "update_btn") {

    // Updates the propertes.
    $mcl_dummy_var = MCL_DUMMY_VAR::byID($dummy_var_id);
    $mcl_dummy_var->setName($name);
    $mcl_dummy_var->setId($id);
    $mcl_dummy_var->setDescription($desc);
    if ($mcl_dummy_var->update()) {
      drupal_set_message("The properties of the dummy variable have been updated.");
      drupal_goto("admin/mcl/global_var");
    }
    else {
      drupal_set_message("Error : Failed to update the dummy variable.", 'error');
    }
  }

  // If 'Add' was clicked.
  else if ($trigger_elem == "add_btn") {
    $details = array(
      'name'        => $name,
      'id'          => $id,
      'description' => $desc,
    );
    if (MCL_DUMMY_VAR::addVar($details)) {
      drupal_set_message("The new dummy variable have been added.");
      drupal_goto("admin/mcl/global_var");
    }
    else {
      drupal_set_message("Error : Failed to add the dummy variable.", 'error');
    }

  }
}
