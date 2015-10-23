<?php

function url_redirect_list_form($form, &$form_state) {
  global $base_url;
  $form = array();
  $form['goto_list'] = array(
    '#markup' => l(t('Add Url Redirect'), 'admin/config/administration/url_redirect'),
  );
  $form['path'] = array(
    '#title' => t('Path'),
    '#type' => 'textfield',
    '#default_value' => isset($_GET['path']) ? $_GET['path'] : '',
  );
  $form['redirect_path'] = array(
    '#title' => t('Redirect Path'),
    '#type' => 'textfield',
    '#default_value' => isset($_GET['redirect_path']) ? $_GET['redirect_path'] : '',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Filter',
  );
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => 'Reset',
  );
  $query = db_select('url_redirect', 'u')
    ->fields('u');
  if (!empty($_GET['path'])) {
    $query->condition('path', $_GET['path'], '=');
  }
  if (!empty($_GET['redirect_path'])) {
    $query->condition('date', $_GET['redirect_path'], '=');
  }
  $result = $query->execute()->fetchAll();
  $header = array('Path', 'Redirect Path', 'Checked For', 'Roles', 'Users', 'Status', '', '');
  $rows = array();
  $output = '';
  foreach ($result as $url) {
    $edit = $base_url . '/admin/config/administration/url_redirect/edit?path=' . $url->path;
    $delete = $base_url . '/admin/config/administration/url_redirect/delete?path=' . $url->path;
    $row[] = $url->path;
    $row[] = $url->redirect_path;
    $row[] = $url->check_for;
    if ($url->roles) {
      $roles_names = array_keys((array) json_decode($url->roles));
      $roles = '';
      foreach ($roles_names as $rid) {
        $rolename = db_select('role', 'r')
          ->fields('r', array('name'))
          ->condition('rid', $rid);
        $roleresult = $rolename->execute()->fetchField();
        if ($roleresult) {
          $roles .= $roleresult . '(' . $rid . ')' . ', ';
        }
      }
      $row[] = rtrim($roles, ', ');
    }
    else {
      $row[] = 'N/A';
    }
    if ($url->users) {
      $user_names = array_keys((array) json_decode($url->users));
      $names = '';
      foreach ($user_names as $uid) {
        $username = db_select('users', 'u')
          ->fields('u', array('name'))
          ->condition('uid', $uid);
        $user_result = $username->execute()->fetchField();
        if ($user_result) {
          $names .= $user_result . '(' . $uid . ')' . ', ';
        }
      }
      $row[] = rtrim($names, ', ');
    }
    else {
      $row[] = 'N/A';
    }
    if ($url->status == 1) {
      $row[] = 'Enabled';
    }
    else {
      $row[] = 'Disabled';
    }
    $row[] = '<a href="' . $edit . '">Edit</a>';
    $row[] = '<a href="' . $delete . '">Delete</a>';
    $rows[] = $row;
  }
  if (count($rows) > 0) {
    $per_page = 25;
    $current_page = pager_default_initialize(count($rows), $per_page);
    $chunks = array_chunk($rows, $per_page, TRUE);
    $output = theme('table', array('header' => $header, 'rows' => $chunks[$current_page]));
    $output .= theme('pager', array('quantity', count($rows)));
    $form['output'] = array(
      '#markup' => $output,
    );
  }
  else {
    $form['output'] = array(
      '#markup' => t('No Paths available.'),
    );
  }
  return $form;
}

/**
 * Implements hook_form_state().
 */
function url_redirect_list_form_submit($form, &$form_state) {
  // Goto current path if reset.
  if ($form_state['values']['op'] == 'Reset') {
    drupal_goto(current_path());
  }
  // Pass values to url.
  if ($form_state['values']['op'] == 'Filter') {
    $filter_path = $form_state['values']['path'];
    $filter_redirect_path = $form_state['values']['redirect_path'];
    $params['path'] = check_plain($filter_path);
    $params['redirect_path'] = check_plain($filter_redirect_path);
    drupal_goto('admin/config/administration/url_redirect/list', array('query' => $params));
  }
}
