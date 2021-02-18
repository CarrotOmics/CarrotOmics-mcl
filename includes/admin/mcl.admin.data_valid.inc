<?php
/**
 * Manages MCL data validation.
 *
 * @param array $form
 * @param array $form_state
 *
 * @ingroup mcl_admin
 */
function mcl_admin_data_valid_form($form, &$form_state) {

  // Creates a form.
  $form = array();
  $form['#tree'] = TRUE;

  // Data validation types.
  $form['data_valid_type'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => 'Data validation Type',
  );
  $desc = "
    The table below shows the validation types that currrently defined in the
    MCL templates. It is sometimes required that data in certain columns must
    be restricted. Suppose you want to restrict type of location as following
    types. <br /><br /><em>(e.g.) Locatoin types<br />greenhouse, open field,
    orchard and seedling block</em>.
  ";
  $form['data_valid_type']['table'] = array(
    '#markup' => $desc,
  );

  // Adds data validation type form.
  _mcl_get_mcl_data_valid_type_form($form);

  // Data validations.
  $form['data_valid'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => TRUE,
    '#title'        => 'Data validation',
  );
  $desc = "
    The members of each validation type are listed in the table below.
    Please add a new member or delete the existing member.
  ";
  $form['data_valid']['table'] = array(
    '#markup' => $desc,
  );

  // Adds data validation form.
  _mcl_get_mcl_data_valid_form($form);

  // Sets properties of the form.
  $form['#prefix'] = '<div id="mcl-admin-data-valid-form">';
  $form['#suffix'] = '</div>';
  return $form;
}

/**
 * Returns the form for data validation type.
 *
 * @param array $form
 */
function _mcl_get_mcl_data_valid_type_form(&$form) {

  // Gets all data valid types.
  $mcl_dvts = MCL_DATA_VALID_TYPE::getDataValidTypes();
  $rows = array();
  foreach ($mcl_dvts as $mcl_dvt) {
    $data_valid_type_id = $mcl_dvt->getDataValidTypeID();
    $template_id        = $mcl_dvt->getTemplateID();
    $cv_id              = $mcl_dvt->getCvID();
    $type               = $mcl_dvt->getType();

    // Gets MCL_TEMPLATE.
    $status   = 'Invalid template ID';
    $required = 'N/A';
    $mcl_template = MCL_TEMPLATE::byID($template_id);
    if ($mcl_template) {
      $template_name = $mcl_template->getTemplate();

      // Skips the disabled template.
      $status = $mcl_template->isPublic() ? 'public' : 'private';

      // Gets the cv name.
      $cv_name = '';
      if ($cv_id) {
        $cv = MCL_CHADO_CV::byKey(array('cv_id' => $cv_id));
        $cv_name = $cv->getName();
      }
      $required = ($cv_name) ? $cv_name : '<em>Not required</em>';
    }

    // Creates 'Delete' link.
    $confirm_attr = array(
      'attributes' => array(
        'id'    => 'delete_data_valid_type_' . $data_valid_type_id,
        'class' => array('use-ajax', 'mcl-confirm'),
      )
    );
    $delete_link = l('Delete', "mcl/delete_data_valid_type/$data_valid_type_id" , $confirm_attr);
    $row = array(
      $template_id,
      $template_name,
      $type,
      $required,
      "<em>$status</em>",
      $delete_link,
    );
    $rows[] = $row;
  }
  $table_vars = array(
    'header'      => array('ID', 'Template Name', 'Type', 'CV name', 'Status of Template', 'Action'),
    'rows'        => $rows,
    'empty'       => 'No data validation type has been defined.',
    'attributes'  => array(),
  );
  $form['data_valid_type']['type_table'] = array(
    '#markup' => '<div style="margin-top:20px;">' . theme('table', $table_vars)  . '</div>',
  );
}

/**
 * Returns the form for data validation.
 *
 * @param array $form
 */
function _mcl_get_mcl_data_valid_form(&$form, $target_data_valid_type_id = '') {

  // Gets the site variables.
  $mcl_dvts = MCL_DATA_VALID_TYPE::getDataValidTypes(FALSE);
  foreach ($mcl_dvts as $mcl_dvt) {
    $data_valid_type_id = $mcl_dvt->getDataValidTypeID();
    $template_id        = $mcl_dvt->getTemplateID();
    $type               = $mcl_dvt->getType();
    $cv_id              = $mcl_dvt->getCvID();

    // Sets IDs.
    $id_fieldset  = 'fs_' . $data_valid_type_id;
    $id_value     = 'value_' . $data_valid_type_id;

    if ($target_data_valid_type_id) {
      if ($data_valid_type_id != $target_data_valid_type_id) {
        continue;
      }
      $form['data_valid'][$id_fieldset]['#collapsed'] = FALSE;
    }
    else {
      // Creates a fieldset.
      $form['data_valid'][$id_fieldset] = array(
        '#type'         => 'fieldset',
        '#collapsed'    => TRUE,
        '#collapsible'  => TRUE,
        '#title'        => "[$template_id] $type",
      );
    }

    // Creates a table.
    $data_valid_objs = $mcl_dvt->getDataValidValues();
    $rows = array();
    foreach ($data_valid_objs as $obj) {
      $data_valid_id = $obj->data_valid_id;

      // Creates a "Delete" link.
      $confirm_attr = array(
        'attributes' => array(
          'id' => 'delete_data_valid_' . $data_valid_id,
          'class' => array('use-ajax', 'mcl-confirm'),
        )
      );
      $delete_link = l('Delete', "mcl/delete_data_valid/$data_valid_id" , $confirm_attr);

      if ($cv_id) {
        $row = array(
          $obj->name,
          $obj->cvterm_id,
          $delete_link,
        );
      }
      else {
        $row = array(
          $obj->name,
          $delete_link,
        );
      }
      $rows[] = $row;
    }

    // Sets the headers.
    $headers = array('Value', 'Action');
    if ($cv_id) {
      $headers = array('Value', 'cvterm_id', 'Action');
    }
    $table_vars = array(
      'header'      => $headers,
      'rows'        => $rows,
      'attributes'  => array(),
    );
    $form['data_valid'][$id_fieldset]['table'] = array(
      '#markup' => theme('table', $table_vars),
    );

    // Adds the textfield and button.
    $id_add_btn = 'data_valid_btn_' . $data_valid_type_id . '_0';
    if ($cv_id) {
      $id_add_btn = 'data_valid_btn_' . $data_valid_type_id . '_' . $cv_id;
    }
    $form['data_valid'][$id_fieldset][$id_value] = array(
      '#type'       => 'textfield',
      '#title'      => 'Name of type',
      '#attributes' => array('style' => 'width:250px;'),
    );
    $form['data_valid'][$id_fieldset][$id_add_btn] = array(
      '#type'   => 'submit',
      '#name'   => $id_add_btn,
      '#value'  => 'Add a new type',
    );
  }
}

/**
 * Ajax function which returns the form via ajax.
 *
 * @param $form
 * @param $form_state
 */
function mcl_admin_data_valid_form_ajax_callback($form, &$form_state) {
  return $form;
}

/**
 * Validates the form.
 *
 * @ingroup mcl_admin
 */
function mcl_admin_data_valid_form_validate($form, &$form_state) {

  // Gets the trigger element.
  $trigger_elem = $form_state['triggering_element']['#name'];

  // If "data_valid_btn" button is clicked.
  if (preg_match("/^data_valid_btn_(\d+)_(\d+)$/", $trigger_elem, $matches)) {
    $data_valid_type_id = $matches[1];
    $cv_id              = $matches[2];

    // Gets the value.
    $id_fieldset  = 'fs_' . $data_valid_type_id;
    $id_value     = 'value_' . $data_valid_type_id;
    $value        = trim($form_state['values']['data_valid'][$id_fieldset][$id_value]);

    // Checks the value for empty.
    if (!$value) {
      $msg = ($cv_id) ? 'Please type cvterm ID.' : 'Please type a value.';
      form_set_error("data_valid][$id_fieldset][$id_value", $msg);
      return;
    }

    // Checks the duplication for non-cvterm.
    if (!$cv_id) {
      $keys = array(
        'data_valid_type_id'  => $data_valid_type_id,
        'name'                => $value,
      );
      $data_valid = PUBLIC_MCL_DATA_VALID::byKey(array('name' => $value));
      if ($data_valid) {
        form_set_error("data_valid][$id_fieldset][$id_value", "'$value' has already exists.");
        return;
      }
    }

    // Checks the duplication for cvterm.
    else {

      // Checks for cv.
      $cv = MCL_CHADO_CV::byKey(array('cv_id' => $cv_id));
      if (!$cv) {
        $msg = "cv ID '$cv_id' does not exist in cv table.";
        form_set_error("data_valid][$id_fieldset][$id_value", $msg);
        return;
      }

      // Checks 'trait_category' if data_valid_type is trait category.
      $data_valid_type = MCL_DATA_VALID_TYPE::byID($data_valid_type_id);
      if ($data_valid_type->getType() == 'trait_category') {
        $cvterm_tc = MCL_CHADO_CVTERM::getCvterm('SITE_CV', 'trait_category');
        if (!$cvterm_tc) {
          form_set_error("data_valid][$id_fieldset][$id_value", "The cvterm ('trait_category') does not exists in chado.cvterm table. Please add it.");
          return;
        }
      }

      // Checks the duplication for cvterm in BIMS.
      $cvterm = MCL_CHADO_CVTERM::getCvterm($cv->getName(), $value);
      if ($cvterm) {
        $keys = array(
          'data_valid_type_id'  => $data_valid_type_id,
          'cvterm_id'           => $cvterm->getCvtermID(),
        );
        $data_valid = PUBLIC_MCL_DATA_VALID::byKey($keys);
        if ($data_valid) {
          form_set_error("data_valid][$id_fieldset][$id_value", "'$value' has already exists.");
          return;
        }
      }
    }
  }
}

/**
 * Submits the form.
 *
 * @ingroup mcl_admin
 */
function mcl_admin_data_valid_form_submit($form, &$form_state) {

  // Gets the trigger element.
  $trigger_elem = $form_state['triggering_element']['#name'];

  // If "data_valid_btn" button is clicked.
  if (preg_match("/data_valid_btn_(\d+)_(\d+)/", $trigger_elem, $matches)) {
    $data_valid_type_id = $matches[1];
    $cv_id              = $matches[2];

    // Gets the value.
    $id_fieldset  = 'fs_' . $data_valid_type_id;
    $id_value     = 'value_' . $data_valid_type_id;
    $value        = trim($form_state['values']['data_valid'][$id_fieldset][$id_value]);

    // Add the new type for a cvterm.
    $details = array();
    $err = '';
    if ($cv_id) {

      // Gets cv and data_valid_type.
      $data_valid_type = MCL_DATA_VALID_TYPE::byID($data_valid_type_id);
      $cv = MCL_CHADO_CV::byID($cv_id);

      // Adds new type as a 'trait category'.
      $cvterm = NULL;
      if ($data_valid_type->getType() == 'trait_category') {
        $cvterm = MCL_CHADO_TRAIT::addTrait(NULL, $value, '', '', TRUE);
      }

      // Adds other types.
      else {
        $cvterm = MCL_CHADO_CVTERM::addCvterm(NULL, 'SITE_DB', $cv->getName(), $value, '');
      }
      if ($cvterm) {
        $details = array(
          'data_valid_type_id'  => $data_valid_type_id,
          'name'                => $value,
          'cvterm_id'           => $cvterm->getCvtermID(),
        );
      }
      else {
        $err = "Error : Failed to add a new cvterm ($value).";
      }
    }

    // Add the new type for a non-cvterm.
    else {
      $details = array(
        'data_valid_type_id' => $data_valid_type_id,
        'name' => $value,
      );
    }

    // Adds the new type.
    if ($err) {
      drupal_set_message("Error : $err");
    }
    else {
      $data_valid = new PUBLIC_MCL_DATA_VALID($details);
      if ($data_valid->insert()) {
        drupal_set_message("'$value' has been added");
      }
      else {
        drupal_set_message("Failed to add '$value'");
      }
    }
  }
}
?>
