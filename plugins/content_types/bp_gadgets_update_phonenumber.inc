<?php

$plugin = array(
  'title' => t('BP: Update Phone Number'),
  'description' => t('BP: Update Phone Number'),
  'single' => TRUE,
  'edit form' => 'bp_gadgets_ctype_update_phonenumber_edit_form',
  'render callback' => 'bp_gadgets_ctype_update_phonenumber_render',
  'category' => array(t('BP')),
  'defaults' => array(
    'title' => '',
  ),
);

/**
 * Content type settings form.
 */
function bp_gadgets_ctype_update_phonenumber_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#default_value' => $conf['title'],
  );

  return $form;
}

/**
 * Content type submit handler.
 */
function bp_gadgets_ctype_update_phonenumber_edit_form_submit($form, &$form_state) {
  foreach (array('title') as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Returns the rendered block
 */
function bp_gadgets_ctype_update_phonenumber_render($subtype, $conf, $args, $context) {
  $form = drupal_get_form('_bp_gadgets_ctype_update_phonenumber_form');

  $vars = array(
    'title' => $conf['title'],
    'form' => $form,
  );
  $block = new stdClass();
  $block->title = t('Update Phone Number');
  $block->content = theme('block_bp_update_phonenumber', array('v' => $vars));

  return $block;
}

/**
 * Update mobile phone form
 *
 * @global type $user
 * @param type $form
 * @param type $form_state
 * @return string
 */
function _bp_gadgets_ctype_update_phonenumber_form($form, &$form_state) {
  // Fix "Call to undefined function" error
  form_load_include($form_state, 'inc', 'bp_gadgets', 'plugins/content_types/bp_gadgets_update_phonenumber');

  global $user;
  $user_wrapper = entity_metadata_wrapper('user', $user);
  $mobilenumber = $user_wrapper->field_user_mobile_number->value();

  $form['#prefix'] = '<div id="ajax-bp-update-user-phonenumber-wrapper">';
  $form['#suffix'] = '</div>';

  $form['mobile_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Mobile Phone'),
    '#prefix' => '<div class="row"><div class="col-xs-12 col-sm-8 form-group"><div class="input-gray-box">',
    '#suffix' => '</div></div>
      <div class="error-wrapper col-xs-12">
        <span class="invalid-mobile_number errorBox"></span>
      </div></div>',
    '#required' => TRUE,
    '#label-class' => 'control-label label',
    '#default_value' => $mobilenumber,
    '#id' => "mobile_number",
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Changes'),
    '#prefix' => '<div class="row"><div class="col-xs-12">',
    '#suffix' => '</div></div>',
    '#attributes' => array(
      'class' => array('add-card btn yellow_to_green opacity'),
    ),
    '#ajax' => array(
      'callback' => 'bp_gadgets_update_user_phonenumber_submit_callback',
      'wrapper' => 'ajax-bp-update-user-phonenumber-wrapper',
      'method' => 'replace',
      'effect' => 'none',
      'event' => 'ajax_submit_form',
      'prevent' => 'click',
    ),
  );

  return $form;
}

function bp_gadgets_update_user_phonenumber_submit_callback($form, &$form_state) {
  if (form_get_errors()) {
    return $form;
  }
  ctools_include('ajax');
  $commands = array();
  $commands[] = ctools_ajax_command_reload();

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

function _bp_gadgets_ctype_update_phonenumber_form_validate($form, &$form_state) {
  $mobile = check_plain(trim($form_state['values']['mobile_number']));

  if (strlen($mobile) <= 6) {
    form_set_error('mobile_number', 'Invalid number, please try again.');
  }
}

/**
 * Update mobile phone number form submit
 *
 * @param type $form
 * @param type $form_state
 */
function _bp_gadgets_ctype_update_phonenumber_form_submit($form, &$form_state) {
  $mobile = bp_join_localize_us_number(check_plain(trim($form_state['values']['mobile_number'])));
  $parameters = array(
    'mobileNumber' => $mobile,
  );

  module_load_include('inc', 'bp_gadgets', 'plugins/content_types/includes/bp_gadgets_common');
  $response = _bp_update_set_fis_user_mobile_phone($parameters);
  $update_response = NULL;

  if ($response->code == 200 && !empty($response->data)) {
    $data = json_decode($response->data);

    if (isset($data->UserUpdateResponse)) {
      $update_response = $data->UserUpdateResponse;
    }
  }

  if (isset($update_response->responseStatus) && $update_response->responseStatus == TRUE) {
    $form_state['response'] = $update_response->responseStatus;
    _bp_gadgets_set_user_mobile_phone($parameters);
    drupal_set_message(t('Your mobile number has been saved.'), 'confirmation');
  }
  else {
    $error_code = 0;
    if (isset($update_response->errorCode)) {
      $error_code = $update_response->errorCode;
    }
    $message = bp_gadgets_update_phonenumber_get_error_message($error_code);
    drupal_set_message(t($message), 'error');
  }
}

/**
 * Get API Error Message
 */
function bp_gadgets_update_phonenumber_get_error_message($error_code) {
  $message = 'An error occured. Please try again later.';
  switch ($error_code) {
    case "REQUEST_IS_NULL_ERROR":
      $message = 'There was a problem with the request.';
      break;
    case "NON_EXISTING_FIS_UNIQUE_ID":
      $message = 'Your identification was not found.';
      break;
    case "CLIENT_NOT_REGISTERED_ERROR":
      $message = 'Email address doesn\'t exists.';
      break;
    case "EMAIL_NOT_UNIQUE_ERROR":
      $message = 'Email address already exist for another account.';
      break;
    case "LOYALTY_ID_NOT_UNIQUE_ERROR":
      $message = 'Loyalty identification already exist for another account.';
      break;
    case "SYSTEM_ERROR":
      $message = 'We have problems with the system right now, please try again.';
      break;
    default:
      $message = 'There was a problem with the request.';
      break;
  }
  return $message;
}
