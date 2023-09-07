<?php
/**
 * @file
*/
/**
 * MCL form.
*
* @param $form
* @param $form_state
*/
function mcl_form($form, &$form_state) {
  global $user;

  // Creates a form.
  $form = array();
  $form['#tree'] = TRUE;

  // Checks for an user.
  if (!property_exists($user, 'name') or !$user->name) {
    $form['mcl']['anonymous'] = array(
      '#markup' => '<div>Please log in</div>',
    );
    return $form;
  }

  // Performs the pre-check.
  if (!mcl_pre_check()) {

    // Checks for the site variables.
    if (!_mcl_form_check_site_variables($form)) {
      return $form;
    }

    // Checks for the dummy variables.
    if (!_mcl_form_check_dummy_variables($form)) {
      return $form;
    }
    mcl_pre_check_done();
  }

  // MCL.
  $form['mcl'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => 'Mainlab Chado Loader',
  );

  // Descriptions of MCL.
  $desc_drush = '';
  if (in_array('administrator', $user->roles)) {
    $desc_drush = "<br />Click '<b><em>Drush commands</b><em> to see the list of drush commands that available for MCL.";
  }
  $desc = "
    MCL (Mainlab Chado Loader) is a module that enables a user to upload biological data to chado database schema. Users are required to transfer their biological data into various types of data template files. MCL, then, uploads these data template files into a chado schema.<br /><br />
    <div style='margin-left:20px;margin-bottom:20px;'>
      Click '<b><em>Templates</b><em> to see the list of templates that MCL currently supports.
      <br /> Click '<b><em>Upload Data</b><em> to upload your data or see status of your currently uploaing jobs.
      $desc_drush</div>
  ";
  $form['mcl']['desc'] = array(
    '#markup' => $desc,
  );

  // Gets MCL_USER.
  $mcl_user = MCL_USER::byID($user->uid);

  // Lists of the page links.
  $items = array();
  $items[] = l('Templates', '/mcl/template_list');
  if ($mcl_user) {
    $items[] = l('Upload Data', '/mcl/upload_data');
  }
  else {
    $items[] = "Upload Data (Please make yourself as MCL user in <a href='admin/mcl/user'>admin page</a>)";
  }
  $form['mcl']['page_link'] = array(
    '#markup' => theme('item_list', array('items' => $items)),
  );

  // Admin menu.
  if (user_access('admin_mcl')) {
    $form['mcl']['mcl_admin'] = array(
      '#type'         => 'fieldset',
      '#collapsed'    => TRUE,
      '#collapsible'  => TRUE,
      '#title'        => 'Admin Menu',
    );
    $items = array();
    $items []= l('Drush commands', '/mcl/drush');
    $items []= mcl_form_get_data_query_links();
    $items []= mcl_form_get_data_info_links();
    $form['mcl']['mcl_admin']['page_link'] = array(
      '#markup' => theme('item_list', array('items' => $items)),
    );
  }

  // Sets form properties.
  $form['#prefix'] = '<div id="mcl-form">';
  $form['#suffix'] = '</div>';
  return $form;
}

/**
 * Checks the site variables.
 *
 * @param array $form
 *
 * @return boolean
 */
function _mcl_form_check_site_variables(&$form) {

  // Checks the site variables.
  $objs = MCL_SITE_VAR::getVars(MCL_OBJECT);
  $error_arr = array();
  foreach ((array)$objs as $obj) {
    if (!$obj->value) {
      $error_arr []= $obj->name;
    }
  }

  // Shows the error message if exists.
  if (!empty($error_arr)) {
    $error_msg = "The following variabls does not have values.<br />";
    $error_msg .= "Please set the values in <a href='admin/mcl/global_var'>admin page</a>.";
    $error_msg .= theme('item_list', array('items' => $error_arr));
    $form['mcl'] = array(
      '#type'         => 'fieldset',
      '#collapsed'    => FALSE,
      '#collapsible'  => FALSE,
      '#title'        => 'Required : MCL site variables',
      '#description'  => "<div>$error_msg</div>",
    );
    return FALSE;
  }
  return TRUE;
}

/**
 * Checks the dummy variables.
 *
 * @param array $form
 *
 * @return boolean
 */
function _mcl_form_check_dummy_variables(&$form) {

  // Checks the site variables.
  $objs = MCL_DUMMY_VAR::getVars(MCL_OBJECT);
  $error_arr = array();
  foreach ((array)$objs as $obj) {
    if (!$obj->id && $obj->id != 0) {
      $error_arr []= $obj->name;
    }
  }

  // Shows the error message if exists.
  if (!empty($error_arr)) {
    $error_msg = "The following dummy variabls does not have IDs. They are needed for certain table in Chado.<br />";
    $error_msg .= "Please set the IDs in <a href='admin/mcl/global_var'>admin page</a>.";
    $error_msg .= theme('item_list', array('items' => $error_arr));
    $form['mcl'] = array(
      '#type'         => 'fieldset',
      '#collapsed'    => FALSE,
      '#collapsible'  => FALSE,
      '#title'        => 'Required : MCL dummy variables',
      '#description'  => "<div>$error_msg</div>",
    );
    return FALSE;
  }
  return TRUE;
}

/**
 * Ajax function which returns the form via ajax.
 *
 * @param $form
 * @param $form_state
 */
function mcl_form_ajax_callback($form, $form_state) {
  return $form;
}

/**
 * Validates the form.
 *
 * @param $form
 * @param $form_state
 */
function mcl_form_validate($form, &$form_state) {}

/**
 * Submits the form.
 *
 * @param $form
 * @param $form_state
 */
function mcl_form_submit($form, &$form_state) {}
