<?php
/**
 * Manages MCL templates.
 *
 * @param array $form
 * @param array $form_state
 *
 * @ingroup mcl_admin
 */
function mcl_admin_template_form($form, &$form_state) {

  // Creates a form.
  $form = array();
  $form['#tree'] = TRUE;

  // Gets the template directory.
  $template_path = drupal_get_path('module', 'mcl') . '/templates';

  // MCL templates.
  $desc = "
      Manage data templates for Mainlab Chado Loader.<br />The template file
      are stored in <div style='margin:10px;'><em>$template_path</em></div>
      Please click 'Scan' after a new template is defined.
  ";
  $form['manage_template'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => 'MCL Data Templates',
    '#description'  => "<div style='margin-top:10px;margin-bottom:10px;'>$desc</div>",
  );

  // Adds the 'Scan' button
  $form['manage_template']['scan_btn'] = array(
    '#type'   => 'submit',
    '#name'   => 'scan_btn',
    '#value'  => 'Scan Directory',
  );

  // Lists all templates.
  _mcl_admin_template_form_template_list($form['manage_template']);

  // Sets properties of the form.
  $form['#prefix'] = '<div id="mcl-admin-template-form">';
  $form['#suffix'] = '</div>';
  $form['#submit'][] = 'mcl_admin_template_form_submit';
  return $form;
}

/**
 * Lists the availalbe templates.
 *
 * @param array $form
 */
function _mcl_admin_template_form_template_list(&$form) {

  // Lists the enabled templates.
  $desc = "The table shows the currently available templates.";
  $form['template_list'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => 'Data Templates',
    '#description'  => "<div style='margin-top:10px;margin-bottom:15px;'>$desc</div>",
  );
  $form['template_list']['#theme'] = 'mcl_admin_template_form_template_list';

  //Gest all MCL templates.
  $mcl_templates = MCL_TEMPLATE::getTemplates();
  foreach ($mcl_templates as $mcl_template) {
    $form['template_list'][$mcl_template->getTemplateID()] = array(
      '#title'          => '',
      '#type'           => 'checkbox',
      '#default_value'  => 0,
      '#attributes'     => array('style' => 'margin-left:10px;'),
    );
  }

  // Adds the dropdown list for the operations.
  $options = array(
    ''        => 'Please choose an operation',
    'public'  => 'Make public',
    'private' => 'Make private',
    'add'     => 'Add validation types',
    'remove'  => 'Remove validation types',
  );
  $form['template_list']['op_ddl'] = array(
    '#type'       => 'select',
    '#name'       => 'op_ddl',
    '#options'    => $options,
    '#attributes' => array('style' => 'width:210px;'),
  );
  $form['template_list']['perform_btn'] = array(
    '#type'       => 'submit',
    '#name'       => 'perform_btn',
    '#value'      => 'Perform',
    '#attributes' => array('style' => 'width:120px;'),
  );
}

/**
 * Ajax function which returns the form via ajax.
 *
 * @param $form
 * @param $form_state
 */
function mcl_admin_template_form_ajax_callback($form, &$form_state) {
  return $form;
}

/**
 * Validates the form.
 *
 * @ingroup mcl_admin
 */
function mcl_admin_template_form_validate($form, &$form_state) {

  // Gets the trigger element.
  $trigger_elem = $form_state['triggering_element']['#name'];

  // If "Perform" was clicked.
  if ($trigger_elem == "perform_btn") {

    // Gets the operation.
    $op = $form_state['input']['op_ddl'];

    // Checks the operation.
    if (!$op) {
      form_set_error('manage_template][template_list][', "Please choose an operation");
      return;
    }

    // Checks the template selections.
    $selected = FALSE;
    foreach ((array)$form_state['values']['manage_template']['template_list'] as $template_id => $bool) {
      if ($bool == 1) {
        $selected = TRUE;
      }
    }
    if (!$selected) {
      form_set_error('manage_template][template_list][', "Please choose at least one template");
      return;
    }
  }
}

/**
 * Submits the form.
 *
 * @ingroup mcl_admin
 */
function mcl_admin_template_form_submit($form, &$form_state) {

  // Gets the trigger element.
  $trigger_elem = $form_state['triggering_element']['#name'];

  // If "Perform" was clicked.
  $error_arr = array();
  if ($trigger_elem == "perform_btn") {

    // Gets the operation.
    $op = $form_state['input']['op_ddl'];

    // Makes the selected templates public or private.
    if (preg_match("/(public|private)/", $op)) {
      $error_arr = array();
      foreach ((array)$form_state['values']['manage_template']['template_list'] as $template_id => $bool) {
        if (!preg_match("/^(\d+)$/", $template_id) || $bool == 0) {
          continue;
        }
        $mcl_template = MCL_TEMPLATE::byKey(array('template_id' => $template_id));

        // Makes it public.
        if ($op == "public") {
          if (!$mcl_template->enable()) {
            $error_arr []= $mcl_template->getTemplate();
          }
        }

        // Makes it private.
        else {
          if (!$mcl_template->disable()) {
            $error_arr []= $mcl_template->getTemplate();
          }
        }
      }
      if (empty($error_arr)) {
        drupal_set_message("The template(s) have been made to $op");
      }
    }

    // Adds or removes the validation types.
    else if (preg_match("/(add|remove)/", $op)) {

      // Adds/Removes the data validation types.
      foreach ((array)$form_state['values']['manage_template']['template_list'] as $template_id => $bool) {
        if (!preg_match("/^(\d+)$/", $template_id) || $bool == 0) {
          continue;
        }
        $mcl_template = MCL_TEMPLATE::byKey(array('template_id' => $template_id));

        // Adds the default data valid types.
        $op_label = '';
        if ($op == "add") {
          $op_label = "added";
          if (!$mcl_template->addDataValidTypes()) {
            $error_arr []= $mcl_template->getTemplate();
          }
        }

        // Removes the data valid types.
        else {
          $op_label = "removed";
          if (!$mcl_template->removeDataValidTypes()) {
            $error_arr []= $mcl_template->getTemplate();
          }
        }
      }
      if (empty($error_arr)) {
        drupal_set_message("The data validation types(s) have been $op_label");
      }
    }
  }

  // Scans the template directory and add new templates or remove non-existence
  // ones from the database.
  else if ($trigger_elem == "scan_btn") {
    MCL_TEMPLATE::updateTemplates(TRUE);
    drupal_set_message('The template directory have been scaned');
  }
}

/**
 * Theme function for the form.
 */
function theme_mcl_admin_template_form_template_list($variables) {
  $element= $variables['element'];

  // Gets the template directory.
  $template_path = drupal_get_path('module', 'mcl') . '/templates';

  // Gets all templates and populates the table with the templates.
  $error_msg = '';
  $rows = array();
  foreach ((array)$element as $template_id => $info) {
    if (!preg_match("/^(\d+)$/", $template_id)) {
      continue;
    }

    // Gets MCL_TEMPLATE.
    $template = MCL_TEMPLATE::getTemplateByID($template_id);
    if (!$template) {
      continue;
    }
    $template_name_lc = strtolower($template->getTemplate());

    // Checks the existence of the class file.
    $type_lc = strtolower($template->getTemplateType());
    $sub_folder = '';
    if (preg_match("/(merge|delete|update)$/", $type_lc)) {
      $sub_folder = "/$type_lc";
    }
    $class_file = 'mcl_template_' . $template_name_lc . '.class.inc';
    if (!file_exists("$template_path$sub_folder/$class_file")) {
      $error_msg .= "<div>Class file for '$template_name' does not exist.</div>";
    }

    // Updates the properties.
    $public = $template->isPublic() ? 'YES' : 'NO';
    $dvt_arr = $template->getDataValidTypes();
    $data_valid_types = 'NO';
    if (!empty($dvt_arr)) {
      $types = array_keys($dvt_arr);
      $type_arr = array();
      foreach ($types as $type) {
        $keys = array(
          'template_id' => $template_id,
          'type'        => $type,
        );
        $data_valid_type = MCL_DATA_VALID_TYPE::byKey($keys);
        $style = '';
        if (!$data_valid_type) {
          $style = " style='color:red' ";;
        }
        $type_arr []= "<span $style >$type</span>";
      }
      $data_valid_types = implode('<br />', $type_arr);
    }

    // Adds the checkbox.
    $checkbox = drupal_render($element[$template_id]);

    // Adds template row.
    $row = array(
      $checkbox,
      $template->getTemplateID(),
      $template->getTemplateType(FALSE),
      $template->getTemplate(),
      $public,
      $data_valid_types,
      $template->defineDescription(),
    );
    $rows []= $row;
  }

  // Sets the headers.
  $headers = array(
    array('data' => '', 'style' => 'width:10px'),
    array('data' => 'ID', 'style' => 'width:1%'),
    array('data' => 'Template Type', 'style' => 'width:15%'),
    array('data' => 'Template', 'style' => 'width:10%'),
    array('data' => 'Public', 'style' => 'width:8%'),
    array('data' => 'Data&nbsp;Valid Types', 'style' => 'width:8%'),
      'Description',
  );
  $table_vars = array(
    'header'      => $headers,
    'rows'        => $rows,
    'empty'       => 'No disabled template found',
    'attributes'  => array(),
  );

  // Adds a button.
  $layout = '<div style="">' . drupal_render($element['op_ddl'])  .'</div>';
  $layout .= '<div style="">' . drupal_render($element['perform_btn']) . '</div>';
  if ($error_msg) {
    $layout .= "<div style='margin-top:8px;'><span style='color:red'>*</span> $error_msg</div>";
  }
  $layout .= theme('table', $table_vars);
  return $layout;
}
