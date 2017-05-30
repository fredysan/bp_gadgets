<?php

function bp_gadgets_delete_cc_form($form, &$form_state) {
  // Fix "Call to undefined function"
  form_load_include($form_state, 'inc', 'bp_gadgets', 'plugins/content_types/includes/bp_gadgets_forms');

  $form['#prefix'] = '<div id="ajax-bp-delete-card-wrapper"><div class="form">';
  $form['#suffix'] = '</div></div>';

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Confirm'),
    '#attributes' => array(
      'class' => array('opacity yellow_to_green'),
      'form' => 'bp-gadgets-delete-cc-form',
    ),
    '#ajax' => array(
      'callback' => '_bp_gadgets_delete_cc_callback',
      'wrapper' => 'ajax-bp-delete-card-wrapper',
      'event' => 'click',
    ),
  );

  $form['hidden_id'] = array(
    '#type' => 'hidden',
    '#id' => 'cardId',
  );

  return $form;
}


function bp_gadgets_delete_cc_form_submit($form, &$form_state) {
  $response = false;

  $card_id = check_plain($form_state['values']['hidden_id']);
  $data = array(
    'cardId' => $card_id,
  );

  // TODO: Add FIS call to remove selected credit card
  if ($data) {
    $response = true;
    //$response = _bp_request_cc_removecard($data);
  }

  if ($response) {
    drupal_set_message(t('Your information has been saved.'));
  }
  else {
    drupal_set_message(t('Error removing card. Please try again.'), 'error');
  }
}

/**
 * Ajax callback function for the forms.
 */
function _bp_gadgets_delete_cc_callback($form, $form_state) {
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

/**
 * Add credit card form
 *
 * @global type $user
 * @param type $form
 * @param type $form_state
 * @return string
 */
function bp_gadgets_add_cc_form($form, &$form_state) {
  form_load_include($form_state, 'inc', 'bp_gadgets', 'plugins/content_types/includes/bp_gadgets_forms');

  global $user;

  $user_wrapper = entity_metadata_wrapper('user', $user);
  $firstname = $user_wrapper->field_user_first_name->value();
  $lastname = $user_wrapper->field_user_last_name->value();
  $mail = $user_wrapper->mail->value();
  $city = $user_wrapper->field_user_city->value();
  $address = $user_wrapper->field_user_address_line_1->value();
  $state = $user_wrapper->field_user_state->value();
  $zipcode = $user_wrapper->field_user_zip_code->value();

  $firstname = isset($firstname) ? trim($firstname) : '';
  $lastname = isset($lastname) ? trim($lastname) : '';
  $city = isset($city) ? trim($city) : '';
  $mail = isset($mail) ? trim($mail) : '';
  $state = isset($state) ? trim($state) : '';
  $zipcode = isset($zipcode) ? trim($zipcode) : '';

  $form['#prefix'] = '<div id="ajax-bp-earning-preferences-drcard-wrapper"><div class="form">';
  $form['#suffix'] = '</div></div>';

  $form['firstName'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#prefix' => '<div class="form-group col-xs-12 col-md-6"><div class="input-gray-box"><label class="control-label" for="firstName">First Name*</label>',
    '#suffix' => '</div></div>',
    '#default_value' => $firstname,
    '#attributes' => array(
      'disabled' => 'disabled',
      'id' => 'firstName',
      'data-group' => 'firstName-lastName',
    ),
  );

  $form['lastName'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#prefix' => '<div class="form-group col-xs-12 col-md-6"><div class="input-gray-box"><label class="control-label" for="lastName">Last Name*</label>',
    '#suffix' => '</div></div>',
    '#default_value' => $lastname,
    '#attributes' => array(
      'disabled' => 'disabled',
      'id' => 'lastName',
      'data-group' => 'firstName-lastName',
    ),
  );

  $form['show_errors_1'] = array(
    '#markup' => '<span class="invalid-firstName-lastName errorBox"></span>',
    '#prefix' => '<div class="col-xs-12 hidden-xs hidden-sm">',
    '#suffix' => '</div>',
  );

  $form['emailAddress'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#prefix' => '<div class="form-group col-xs-12 col-md-6"><div class="input-gray-box"><label class="control-label" for="emailAddress">Email Address*</label>',
    '#suffix' => '</div></div>',
    '#default_value' => $mail,
    '#attributes' => array(
      'disabled' => 'disabled',
      'id' => 'emailAddress',
      'data-group' => 'emailAddress-streetAddress',
    ),
  );

  $form['streetAddress'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#prefix' => '<div class="form-group col-xs-12 col-md-6"><div class="input-gray-box"><label class="control-label" for="streetAddress">Street Address*</label>',
    '#suffix' => '<span class="invalid-streetAddress visible-xs visible-sm"></span></div></div>',
    '#default_value' => $address,
    '#attributes' => array(
      'id' => 'streetAddress',
      'data-group' => 'emailAddress-streetAddress',
    ),
  );

  $form['show_errors_2'] = array(
    '#markup' => '<span class="invalid-emailAddress-streetAddress errorBox"></span>',
    '#prefix' => '<div class="col-xs-12 hidden-xs hidden-sm">',
    '#suffix' => '</div>',
  );

  $form['city'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#prefix' => '<div class="form-group col-xs-12 col-md-6"><div class="input-gray-box"><label class="control-label" for="city">City*</label>',
    '#suffix' => '<span class="invalid-city visible-xs visible-sm"></span></div></div>',
    '#default_value' => $city,
    '#attributes' => array(
      'id' => 'city',
      'data-group' => 'city-state',
    ),
  );

  $states = populate_states();

  $form['state'] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => $states,
    '#default_value' => $state,
    '#attributes' => array(
      'id' => 'state',
      'data-group' => 'city-state',
    ),
    '#prefix' => '<div class="form-group col-xs-12 col-md-6"><div class="input-gray-box"><label class="control-label" for="state">State*</label><div class="form-type-select">',
    '#suffix' => '</div></div></div>',
  );

  $form['show_errors_3'] = array(
    '#markup' => '<span class="invalid-city-state errorBox"></span>',
    '#prefix' => '<div class="col-xs-12 hidden-xs hidden-sm">',
    '#suffix' => '</div>',
  );

  $form['zipCode'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#prefix' => '<div class="row"><div class="col-xs-12"><div class="form-group col-xs-12 col-md-6"><div class="input-gray-box"><label class="control-label" for="zipCode">Zip Code*</label>',
    '#suffix' => '</div></div><div class="col-xs-12"><span class="invalid-zipCode errorBox withMargin"></span></div></div></div>

    </div>',
    '#default_value' => $zipcode,
    '#attributes' => array(
      'id' => 'zipCode',
    ),
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Confirm'),
    '#attributes' => array(
      'class' => array('opacity yellow_to_green'),
      'form' => 'bp-gadgets-add-cc-form',
    ),
    '#ajax' => array(
      'callback' => '_bp_gadgets_add_cc_form_callback',
      'wrapper' => 'ajax-bp-earning-preferences-drcard-wrapper',
      'event' => 'ajax_submit_form',
      'prevent' => 'click',
    ),
  );

  $form['actions']['cancel'] = array(
    '#markup' => '<button type="button" data-dismiss="modal" class="btn btn opacity transparent_to_green">Cancel</button>',
  );

  $form['actions']['#prefix'] = '<div class="modal-footer"><div class="buttons">';
  $form['actions']['#suffix'] = '</div></div>';

  $form['#validate'][0] = 'bp_gadgets_add_cc_form_validate';
  return $form;
}

function bp_gadgets_add_cc_form_validate($form, &$form_state) {
  $response = false;
  $data = array(
    'streetAddress' => $form_state['values']['streetAddress'],
    'city' => $form_state['values']['city'],
  );

  if ($data) {
    $response = _bp_request_dr_card($data);
  }

  if($response->code == 200 && !empty($response->data)){
    $response = json_decode($response->data);
    $response = $response->UserRequestCardResponse;

    if(!$response->responseStatus){
      module_load_include('api.inc', 'bp_fis');

      $message = 'An error occured. Please try again later.';

      switch ($response->errorCode) {
        case "REQUEST_IS_NULL_ERROR":
          $message = 'There was a problem with the request.';
          form_set_error('form', $message);
          break;
        case "NON_EXISTING_FIS_UNIQUE_ID":
          $message = 'Error: FIS unique id doesn\'t exist.';
          form_set_error('form', $message);
          break;
        case "CLIENT_NOT_REGISTERED_ERROR":
          $message = 'Email address doesn\'t exists.';
          form_set_error('emailAddress', $message);
          break;
        case "INVALID_FIS_UNIQUE_ID":
          $message = 'Your identification number is invalid.';
          form_set_error('form', $message);
          break;
        case "ALREADY_LINKED_CARD":
          $message = 'You already have linked card.';
          form_set_error('form', $message);
          break;
        case "ALREADY_REQUESTED_CARD":
          $message = 'Error: You have already requested a card.';
          form_set_error('form', $message);
          break;
        case "SYSTEM_ERROR":
          $message = 'A system error occured, please try again.';
          form_set_error('form', $message);
          break;
        case 'PROFILEID_INVALID':
          $message = 'Error: Missing or invalid profile ID.';
          form_set_error('form', $message);
          break;
        case 'PROFILEID_NON_EXISTING':
          $message = 'Error: Non existing profile ID.';
          form_set_error('form', $message);
          break;
        case 'STREET_ADDRESS_INVALID':
          $message = 'Error: Invalid Street Address.';
          form_set_error('streetAddress', $message);
          break;
        case 'ADDRESS_REQUIRED_ERROR':
          $message = 'Error: Street Address required.';
          form_set_error('streetAddress', $message);
          break;
        case 'STREET_ADDRESS_MISSING':
          $message = 'Error: Missing Street Address.';
          form_set_error('streetAddress', $message);
          break;
        case 'CITY_INVALID':
          $message = 'Error: Invalid City.';
          form_set_error('city', $message);
          break;
        case 'CITY_MISSING':
          $message = 'Error: Missing City.';
          form_set_error('city', $message);
          break;
        default:
          $message = 'There was a problem with the request.';
          form_set_error('form', $message);
          break;
      }
    }
  }else{
    form_set_error('form', 'There was a problem with the request.');
  }
}

function bp_gadgets_add_cc_form_submit($form, &$form_state) {
  drupal_set_message(t('Your BP Driver Rewards card will arrive in the mail in the next 7 to 10 business days.'), 'confirmation');
}

function _bp_gadgets_add_cc_form_callback($form, &$form_state) {
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
