<?php

function url_redirect_settings_form($form, &$form_state) {
  $form = array();
  $form['goto_list'] = array(
    '#markup' => l(t('Url Redirect List'), 'admin/config/administration/url_redirect/list'),
  );
  $form['url'] = array(
    '#type' => 'fieldset',
    '#title' => t('Url Redirect'),
    '#collapsible' => TRUE, // Added
    '#collapsed' => FALSE, // Added
    '#attached' => array(
      'js' => array(
        'vertical-tabs' => drupal_get_path('module', 'url_redirect') . '/js/url_redirect.js',
      ),
    ),
  );
  $form['url']['path'] = array(
    '#type' => 'textfield',
    '#title' => 'Path',
    '#attributes' => array('placeholder' => 'Enter Path'),
    '#required' => TRUE,
  );
  $form['url']['redirect_path'] = array(
    '#type' => 'textfield',
    '#title' => 'Redirect Path',
    '#attributes' => array('placeholder' => 'Enter Redirect Path'),
    '#required' => TRUE,
  );
  $status = array(0 => 'Disabled', 1 => 'Enabled');
  $form['url']['checked_for'] = array(
    '#type' => 'radios',
    '#options' => drupal_map_assoc(array(t('Role'), t('User'))),
    '#title' => t('Select Redirect path for'),
    '#required' => TRUE,
  );
  $option = user_roles($membersonly = TRUE, $permission = NULL);
  $users = _user_fetch();
  $form['url']['roles'] = array(
    '#type' => 'select',
    '#options' => $option,
    '#title' => t('Select Roles.'),
    '#multiple' => TRUE,
  );
  $form['url']['user'] = array(
    '#prefix' => '<div class="users-checkbox">',
    '#type' => 'select',
    '#title' => t('Select Users.'),
    '#options' => $users,
    '#multiple' => TRUE,
    '#suffix' => '</div>',
  );
  $form['url']['status'] = array(
    '#type' => 'radios',
    '#options' => $status,
    '#title' => t('Status'),
    '#required' => TRUE,
  );
  $form['url']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  $form['url']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
  );

  return $form;
}

function url_redirect_settings_form_validate($form, &$form_state) {
  // For settings page.
  if ($form_state['values']['op'] == 'Reset') {
    drupal_goto(current_path());
  }
  if ($form_state['values']['op'] == 'Submit') {
    $path = $form_state['values']['path'];
    $path_check = _path_check($path);
    if ($path_check > 0) {
      form_set_error('path', t("The path '@link_path' already used for redirect.", array('@link_path' => $path)));
    }
    $redirect_path = $form_state['values']['redirect_path'];
    if (!drupal_valid_path($path)) {
      form_set_error('path', t("The path '@link_path' is either invalid or you do not have access to it.", array('@link_path' => $path)));
    }
    if (!drupal_valid_path($redirect_path)) {
      form_set_error('redirect_path', t("The redirect path '@link_path' is either invalid or you do not have access to it.", array('@link_path' => $redirect_path)));
    }
    $checked_for = $form_state['values']['checked_for'];
    if ($checked_for == 'User') {
      $user_values = $form_state['values']['user'];
      if (!$user_values) {
        form_set_error('user', t("Select Atleast one user."));
      }
    }
    if ($checked_for == 'Role') {
      $roles_values = $form_state['values']['roles'];
      if (!$roles_values) {
        form_set_error('roles', t("Select Atleast one Role."));
      }
    }
    $form_state['#rebuild'] = TRUE;
  }
}

function url_redirect_settings_form_submit($form, &$form_state) {
  $path = $form_state['values']['path'];
  $redirect_path = $form_state['values']['redirect_path'];
  $checked_for = $form_state['values']['checked_for'];
  if ($checked_for == 'User') {
    $user_values = $form_state['values']['user'];
    if ($user_values) {
      $users_values = json_encode($user_values);
      $role_values = '';
    }
  }
  if ($checked_for == 'Role') {
    $roles_values = $form_state['values']['roles'];
    if ($roles_values) {
      $role_values = json_encode($roles_values);
      $users_values = '';
    }
  }
  $status = $form_state['values']['status'];
  $insert = db_insert('url_redirect')
      ->fields(array(
        'path' => check_plain($path),
        'roles' => $role_values,
        'users' => $users_values,
        'redirect_path' => check_plain($redirect_path),
        'status' => $status,
        'check_for' => $checked_for,
      ))
      ->execute();
  drupal_goto('admin/config/administration/url_redirect/list');
}
