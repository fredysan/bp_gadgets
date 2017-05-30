<?php

$plugin = array(
  'title' => t('BP My Account: Earning Preferences'),
  'description' => t('BP My Account: Earning Preferences'),
  'single' => TRUE,
  'render callback' => 'bp_gadgets_ctype_account_earning_preferences_render',
  'edit form' => 'bp_gadgets_ctype_account_earning_preferences_edit_form',
  'defaults' => array(
    'title' => '',
    'text' => '',
    'bg_color' => '666666',
  ),
  'category' => array(t('BP')),
);

function bp_gadgets_ctype_ep_update_user_phonenumber_form($form, &$form_state) {
  // Fix "Call to undefined function"
  form_load_include($form_state, 'inc', 'bp_gadgets', 'plugins/content_types/bp_gadgets_account_earning_preferences');

  global $user;
  $user_wrapper = entity_metadata_wrapper('user', $user);
  $mobilenumber = $user_wrapper->field_user_mobile_number->value();

  $form['block_title'] = array(
    '#markup' => '<div class="row"><div class="col-md-12 form-cols"><h2 class="title">Update Phone Number</h2></div></div>'
  );

  $form['#prefix'] = '<div id="ajax-bp-update-user-phonenumber-wrapper"><div class="form">';
  $form['#suffix'] = '</div></div>';

  $form['mobileNumber'] = array(
    '#type' => 'textfield',
    '#title' => t('Mobile Phone'),
    '#required' => TRUE,
    '#label-class' => 'control-label label',
    '#prefix' => '<div class="form-group"><div class="number"><div class="row"><div class="col-xs-12 col-sm-6 col-md-6 form-cols">',
    '#suffix' => '</div></div></div></div>',
    '#default_value' => $mobilenumber
  );

  $form['actions']['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Save Changes'),
    '#attributes' => array(
      'class' => array('add-card btn yellow_to_green opacity'),
    ),
    '#prefix' => '<div class="row"><div class="btn-container col-md-12 form-cols">',
    '#suffix' => '</div></div>',
    '#ajax' => array(
      'callback' => 'bp_gadgets_ctype_ep_update_user_phonenumber_ajax_callback',
      'wrapper' => 'ajax-bp-update-user-phonenumber-wrapper',
      'event' => 'click',
    ),
  );

  return $form;
}

function bp_gadgets_ctype_ep_update_user_phonenumber_form_validate($form, &$form_state) {
  if (strlen($form_state['values']['mobileNumber']) <= 6) {
    form_set_error('mobileNumber', 'Mobile number must be filled.');
  }
}

function bp_gadgets_ctype_ep_update_user_phonenumber_form_submit($form, &$form_state) {
  $response = false;
  $data = array(
    'mobileNumber' => $form_state['values']['mobileNumber'],
  );

  if ($data) {
    $response = _bp_update_user_phonenumber($data);
  }

  if ($response) {
    _bp_update_local_user_phonenumber($data);
    drupal_set_message(t('Your information has been saved.'), 'valid');
  }
  else {
    drupal_set_message(t('Error sending form. Try again.'));
  }
}

function bp_gadgets_ctype_ep_update_user_phonenumber_ajax_callback($form, $form_state) {
  if (form_get_errors()) {
    return $form;
  }

  ctools_include('ajax');
  $commands = array();
  $commands[] = ajax_command_prepend('#ajax-bp-update-user-phonenumber-wrapper', theme('status_messages'));
  //$commands[] = ctools_ajax_command_reload();

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

/**
 * Content type settings form.
 */
function bp_gadgets_ctype_account_earning_preferences_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['first_block_title'] = array(
    '#title' => t('First Title'),
    '#type' => 'textfield',
    '#size' => 60,
    '#maxlength' => '60',
    '#default_value' => $conf['first_block_title'],
    '#weight' => 1,
  );
  $form['first_block_text'] = array(
    '#title' => t('First block text'),
    '#type' => 'textarea',
    '#rows' => 2,
    '#default_value' => $conf['first_block_text'],
    '#weight' => 2,
  );
  $form['second_block_title'] = array(
    '#title' => t('Second block Title'),
    '#type' => 'textfield',
    '#size' => '60',
    '#maxlength' => '60',
    '#default_value' => $conf['second_block_title'],
    '#weight' => 3,
  );
  $form['second_block_text'] = array(
    '#title' => t('Second block text'),
    '#type' => 'textfield',
    '#size' => '60',
    '#maxlength' => '60',
    '#default_value' => $conf['second_block_text'],
    '#weight' => 4,
  );
  $form['third_block_title'] = array(
    '#title' => t('Third block title'),
    '#type' => 'textfield',
    '#size' => 60,
    '#maxlength' => '60',
    '#default_value' => $conf['third_block_title'],
    '#weight' => 5,
  );
  $form['third_block_text'] = array(
    '#title' => t('Third block Text'),
    '#type' => 'textarea',
    '#rows' => 2,
    '#default_value' => $conf['third_block_text'],
    '#weight' => 6,
  );
  $form['fourth_block_title'] = array(
    '#title' => t('Fourth block title'),
    '#type' => 'textfield',
    '#size' => 60,
    '#maxlength' => '60',
    '#default_value' => $conf['fourth_block_title'],
    '#weight' => 7,
  );

  return $form;
}

/**
 * Content type submit handler.
 */
function bp_gadgets_ctype_account_earning_preferences_edit_form_submit($form, &$form_state) {
  $keys = array(
    'first_block_title',
    'first_block_text',
    'second_block_title',
    'second_block_text',
    'third_block_title',
    'third_block_text',
    'fourth_block_title'
  );
  foreach ($keys as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

function bp_gadgets_ctype_account_earning_preferences_render($subtype, $conf, $args, $context) {

  module_load_include('inc', 'bp_fis', 'bp_fis.api');
  $linked_creditcards = _bp_fis_api_get_linked_creditcards();
  $block = new stdClass();
  $block->title = t('My Account - Earning Preferences');

  $update_user_phone_form = drupal_get_form('bp_gadgets_ctype_ep_update_user_phonenumber_form');
  $update_userphone = drupal_render($update_user_phone_form);

  $linked_cards = isset($linked_creditcards) ? $linked_creditcards : 0;

  $vars = array(
    'linked_creditcards' => $linked_cards,
    'first_block_title' => $conf['first_block_title'],
    'first_block_text' => $conf['first_block_text'],
    'second_block_title' => $conf['second_block_title'],
    'second_block_text' => $conf['second_block_text'],
    'third_block_title' => $conf['third_block_title'],
    'third_block_text' => $conf['third_block_text'],
    'fourth_block_title' => $conf['fourth_block_title'],
    'update_userphone' => $update_userphone
  );

  $block->content = theme('block_bp_account_earning_preferences', array('v' => $vars));

  return $block;
}
