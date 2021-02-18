<?php
/**
 * Manages MCL config page.
 *
 * @param array $form
 * @param array $form_state
 *
 * @ingroup mcl_admin
 */
function mcl_admin_config_form($form, &$form_state) {

  // Set the breadcrumb.
  $breadcrumb = array();
  $breadcrumb[] = l('Home', '<front>');
  $breadcrumb[] = l('Administration', 'admin');
  $breadcrumb[] = l('MCL', 'admin/mcl');

  // Create the setting form.
  $form = array();

  // MCL variables.
  $form['mcl_vars'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => 'MCL variables',
  );

  // Drupal file paths.
  $file_path_public   = drupal_realpath('public://');
  $file_path_private  = drupal_realpath('private://');

  // Uploading file directory.
  $form['mcl_vars']['mcl_file_dir'] = array(
    '#markup' => t("<br /><b>Uploading File Directory</b><br />" .
        "Uploaded files are saved in drupal public direcotry ($file_path_public)<br /><br />")
  );

  // MCL working directory.
  $form['mcl_vars']['mcl_working_dir'] = array(
    '#title'          => t('MCL Working Directory'),
    '#type'           => t('textfield'),
    '#description'    => t("Please specify the working directory for MCL."),
    '#required'       => TRUE,
    '#default_value'  => mcl_get_config_setting('mcl_working_dir'),
  );

  // PHPZip Library directory.
  $form['mcl_vars']['mcl_lib_phpzip_dir'] = array(
    '#title'          => t('PHPZip Library Directory'),
    '#type'           => t('textfield'),
    '#description'    => t("Please specify the library directory for PHPZip."),
    '#required'       => TRUE,
    '#default_value'  => mcl_get_config_setting('mcl_lib_phpzip_dir'),
  );

  // PHPExcel Library directory.
  $form['mcl_vars']['mcl_lib_phpexcel_dir'] = array(
    '#title'          => t('PHPExcel Library Directory'),
    '#type'           => t('textfield'),
    '#description'    => t("Please specify the library directory of PHPExcel."),
    '#required'       => TRUE,
    '#default_value'  => mcl_get_config_setting('mcl_lib_phpexcel_dir'),
  );

  // Drush binary.
  $form['mcl_vars']['mcl_drush_binary'] = array(
    '#title'          => t('Drush Binary'),
    '#type'           => t('textfield'),
    '#description'    => t("The full path to the drush binary on this server."),
    '#required'       => TRUE,
    '#default_value'  => mcl_get_config_setting('mcl_drush_binary'),
  );
  return system_settings_form($form);
}

/**
 * Admin form validate.
 *
 * @ingroup mcl_admin
 */
function mcl_admin_config_form_validate($form, &$form_state) {

  // Gets variables.
  $mcl_working_dir      = trim($form_state['values']['mcl_working_dir']);
  $mcl_lib_phpzip_dir   = trim($form_state['values']['mcl_lib_phpzip_dir']);
  $mcl_lib_phpexcel_dir = trim($form_state['values']['mcl_lib_phpexcel_dir']);
  $mcl_drush_binary     = trim($form_state['values']['mcl_drush_binary']);

  // Checks the working directory.
  if (!is_writable($mcl_working_dir)) {
    form_set_error('mcl_vars][mcl_working_dir', t("The working directory, $mcl_working_dir, does not exists or is not writeable by the web server."));
    return;
  }

  // Checks the PHPZip library directory.
  if (!file_exists($mcl_lib_phpzip_dir)) {
    form_set_error('mcl_vars][mcl_working_dir', t("PHPZip library directory, $mcl_lib_phpzip_dir, does not exists."));
    return;
  }

  // Checks the PHPExcel library directory.
  if (!file_exists($mcl_lib_phpexcel_dir)) {
    form_set_error('mcl_vars][mcl_working_dir', t("PHPExcel library directory, $mcl_lib_phpexcel_dir, does not exists."));
    return;
  }

  // Checks drush binary.
  if (!file_exists($mcl_drush_binary)) {
    form_set_error('mcl_vars][mcl_drush_binary', t("The drush path does not appear to be correct. Cannot find the binary."));
    return;
  }

  // Updates variables.
  variable_set('mcl_working_dir', $mcl_working_dir);
  variable_set('mcl_lib_phpzip_dir', $mcl_lib_phpzip_dir);
  variable_set('mcl_lib_phpexcel_dir', $mcl_lib_phpexcel_dir);
  variable_set('mcl_drush_binary', $mcl_drush_binary);
}
