<?php

function url_redirect_delete_form($form, &$form_state) {
  $delete_path = check_plain($_GET['path']);
  $path_data = _path_edit($delete_path);
  if ($path_data) {
    $form['output'] = array(
      '#markup' => "Are you to delete " . $delete_path . '? <br>',
    );
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
    );
    $form['no'] = array(
      '#type' => 'submit',
      '#value' => t('No'),
    );
    return $form;
  }
  else {
    drupal_set_message('Path specified is not correct for deletion.', 'error');
  }
}

function url_redirect_delete_form_submit($form, &$form_state) {
  if ($form_state['values']['op'] == 'No') {
    drupal_goto('admin/config/administration/url_redirect/list');
  }
  if ($form_state['values']['op'] == 'Delete') {
    $delete_path = check_plain($_GET['path']);
    db_delete('url_redirect')
      ->condition('path', $delete_path)
      ->execute();
    drupal_set_message(t("The path '@path' is Deletes.", array('@path' => $delete_path)));
    drupal_goto('admin/config/administration/url_redirect/list');
  }
}
