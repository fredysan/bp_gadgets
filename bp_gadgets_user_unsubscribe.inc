<?php

/**
 * Call the FIS API method to unsubscribe email
 */
function bp_gadgets_user_unsubscribe($uuid) {
  $user_unsubscribe_form = drupal_get_form('bp_gadgets_user_unsubscribe_form', $uuid);
  $user_unsubscribe = drupal_render($user_unsubscribe_form);

  $vars = array(
    'form' => $user_unsubscribe
  );

  return theme('bp_gadgets_user_unsubscribe', array('vars' => $vars));
}

function bp_gadgets_user_unsubscribe_form($form, &$form_state, $uuid) {
  $form['html'] = array(
    '#type' => 'markup',
    '#markup' => t('<h1 class="title">Unsubscribe</h1>'),
  );
  $form['confirmation'] = array(
    '#prefix' => '<p class="text padding_text">',
    '#markup' => t('Are you sure you want to unsubscribe from the BP Rewards email? If so, click the submit button.'),
    '#suffix' => '</p>',
  );
  $form['uuid'] = array(
    '#type' => 'hidden',
    '#value' => $uuid,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#attributes' => array('class' => array('btn opacity yellow_to_green')),
  );

  $form['#submit'] = array('bp_gadgets_user_unsubscribe_form_submit');
  
  return $form;
}

function bp_gadgets_user_unsubscribe_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $uuid = trim($form_state['values']['uuid']);
  $fis_unique_id = bp_gadgets_user_unsubscribe_get_fis_unique_id($uuid);

  module_load_include('api.inc', 'bp_fis');
  $response = _bp_fis_api_unsubscribe_email($fis_unique_id);

  $form_state['submitted'] = TRUE;

  if ($response->code == 200 && !empty($response->data)) {
    drupal_set_message(t('Thank you. You have successfully unsubscribed from marketing communications from BP.'), 'status');
  } else {
    drupal_set_message(t('There was an error with your submission, please try again.'), 'error');
  }
}


/**
 * Returns the FisUniqueID for the given uuid
 *
 * @param type $uuid
 * @return string
 */
function bp_gadgets_user_unsubscribe_get_fis_unique_id($uuid) {
  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'user')
    ->propertyCondition('uuid', $uuid)
    ->range(0, 1)
    ->execute();

  $fis_unique_id = NULL;

  if (!empty($result['user'])) {
    $arr_uids = array_keys($result['user']);
    $user = user_load($arr_uids[0]);
    $user_wrapper = entity_metadata_wrapper('user', $user);
    $fis_unique_id = $user_wrapper->field_user_fis_unique_id->value();
  }

  return $fis_unique_id;
}
