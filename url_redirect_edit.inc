<?php

function url_redirect_edit_form($form, &$form_state) {
  $form = array();
  $form['goto_list'] = array(
    '#markup' => l(t('Url Redirect List'), 'admin/config/administration/url_redirect/list'),
  );
  $edit_path = check_plain($_GET['path']);
  $path_data = _path_edit($edit_path);
  if ($path_data) {
    $check_form = $path_data['check_for'];
    $form['path'] = array(
      '#type' => 'textfield',
      '#title' => 'Path',
      '#attributes' => array('placeholder' => 'Enter Path'),
      '#required' => TRUE,
      '#default_value' => $path_data['path'],
      '#disabled' => TRUE,
    );
    $form['redirect_path'] = array(
      '#type' => 'textfield',
      '#title' => 'Redirect Path',
      '#attributes' => array('placeholder' => 'Enter Redirect Path'),
      '#required' => TRUE,
      '#default_value' => $path_data['redirect_path'],
    );
    $status = array(0 => 'Disabled', 1 => 'Enabled');
    $option = user_roles($membersonly = TRUE, $permission = NULL);
    $users = _user_fetch();
    if ($check_form == 'Role') {
      $roles = (array) json_decode($path_data['roles']);
      $form['checked_for'] = array(
        '#type' => 'radios',
        '#options' => drupal_map_assoc(array(t('Role'))),
        '#title' => t('Select Redirect path for'),
        '#required' => TRUE,
        '#default_value' => 'Role',
      );
      $form['roles'] = array(
        '#type' => 'select',
        '#options' => $option,
        '#title' => t('Select Roles.'),
        '#multiple' => TRUE,
        '#default_value' => $roles,
      );
    }
    if ($check_form == 'User') {
      $form['checked_for'] = array(
        '#type' => 'radios',
        '#options' => drupal_map_assoc(array(t('User'))),
        '#title' => t('Select Redirect path for'),
        '#required' => TRUE,
        '#default_value' => 'User',
      );
      $form['user'] = array(
        '#prefix' => '<div class="users-checkbox">',
        '#type' => 'select',
        '#title' => t('Select Users.'),
        '#options' => $users,
        '#multiple' => TRUE,
        '#suffix' => '</div>',
      );
    }
    $form['status'] = array(
      '#type' => 'radios',
      '#options' => $status,
      '#title' => t('Status'),
      '#required' => TRUE,
      '#default_value' => $path_data['status'],
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
    );
    return $form;
  }
  else {
    drupal_set_message('Path Specified is not correct to update', 'error');
  }
}

function url_redirect_edit_form_validate($form, &$form_state) {
  // For settings page.
  if ($form_state['values']['op'] == 'Delete') {
    drupal_goto(current_path());
  }
  if ($form_state['values']['op'] == 'Save') {
    $path = $form_state['values']['path'];
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

function url_redirect_edit_form_submit($form, &$form_state) {
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
  $insert = db_update('url_redirect')
      ->fields(array(
        'path' => check_plain($path),
        'roles' => $role_values,
        'users' => $users_values,
        'redirect_path' => check_plain($redirect_path),
        'status' => $status,
        'check_for' => $checked_for,
      ))
      ->condition('path', $path)
      ->execute();
  drupal_set_message(t("The path '@path' is Updated.", array('@path' => $path)));
  drupal_goto('admin/config/administration/url_redirect/list');
}
