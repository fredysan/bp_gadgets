<?php

$plugin = array(
  'title' => t('BP: Claim Discount'),
  'description' => t('BP: Claim Discount'),
  'single' => TRUE,
  'edit form' => 'bp_gadgets_ctype_claim_discount_edit_form',
  'render callback' => 'bp_gadgets_ctype_claim_discount_render',
  'category' => array(t('BP')),
  'defaults' => array(
    'code' => '',
  ),
);

/**
 * Returns ctype configuration form
 * 
 * @param type $form
 * @param type $form_state
 * @return int
 */
function bp_gadgets_ctype_claim_discount_edit_form($form, &$form_state) {
  // Fix "First argument is expected to be a valid callback"
  form_load_include($form_state, 'inc', 'bp_gadgets', 'plugins/content_types/bp_gadgets_claim_discount');

  $conf = $form_state['conf'];

  $form['help_text'] = array(
    '#title' => t('Help Text'),
    '#type' => 'textarea',
    '#rows' => 3,
    '#default_value' => $conf['help_text'],
    '#weight' => 1,
  );

  return $form;
}

/**
 * Configuration form submit function
 * 
 * @param type $form
 * @param type $form_state
 */
function bp_gadgets_ctype_claim_discount_edit_form_submit($form, &$form_state) {
  $keys = array(
    'help_text',
  );
  foreach ($keys as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Function callback for plugin: BP: Claim Discount
 *
 * @param $subtype
 * @param $conf
 * @param $args
 * @param $context
 *
 * @return \stdClass
 */
function bp_gadgets_ctype_claim_discount_render($subtype, $conf, $args, $context) {
  // Initialize claim a discount errors count
  $_SESSION['claim_discount_errors'] = 0;

  $form = drupal_get_form('bp_gadgets_ctype_claim_discount_form', $conf);
  $vars = array(
    'form' => drupal_render($form),
  );

  $block = new stdClass();
  $block->title = NULL;
  $block->content = theme('block_bp_claim_discount', array('v' => $vars));

  return $block;
}

/**
 * Returns the claim a discount form
 * 
 * @param type $form
 * @param type $form_state
 * @return string
 */
function bp_gadgets_ctype_claim_discount_form($form, &$form_state, $conf) {
  // Fix Call to undefined function erros
  form_load_include($form_state, 'inc', 'bp_gadgets', 'plugins/content_types/bp_gadgets_claim_discount');

  $form = array();

  $form['#prefix'] = '<div id="ajax-claim-discount-form-wrapper">';
  $form['#suffix'] = '</div>';

  $form['code'] = array(
    '#title' => t('Enter 12 or 19-Digit Code'),
    '#type' => 'textfield',
    '#required' => FALSE,
    '#attributes' => array(
      'id' => 'code',
    ),
    '#default_value' => '',
    '#suffix' => '<div class="error-wrapper col-xs-12 light_bg"><span class="invalid-code errorBox"></span></div>',
  );

  if (array_key_exists('help_text', $conf) && !empty($conf['help_text'])) {
    $form['code']['#description_display'] = 'before';
    $form['code']['#description'] = '<div data-html="true" class="help-text icon-help-tooltip" data-toggle="popover" data-trigger="click" data-content="' . htmlentities($conf['help_text']) . '"></div>';
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#attributes' => array(
      'class' => array('btn', 'transparent_to_green', 'opacity'),
    ),
    '#ajax' => array(
      'callback' => 'bp_gadgets_ctype_claim_discount_modal_callback',
      'wrapper' => 'ajax-claim-discount-form-wrapper',
      'method' => 'replace',
      'effect' => 'none',
      'event' => 'ajax_submit_form',
      'prevent' => 'click',
    ),
  );

  return $form;
}

/**
 * Claim a discount form submit function
 * 
 * @param type $form
 * @param type $form_state
 */
function bp_gadgets_ctype_claim_discount_form_submit($form, &$form_state) {
  if (isset($form_state['response'])) {
    _bp_fis_update_user_rewards($form_state['response']->reward);
    drupal_set_message(t('Promotion successfully added to your account.'), 'confirmation');
  }
}

/**
 * Claim a discount validation function after submit
 * 
 * @param type $form
 * @param type $form_state
 */
function bp_gadgets_ctype_claim_discount_form_validate($form, &$form_state) {
  $code = trim($form_state['values']['code']);

  if (preg_match('/^[0-9]{12}$/', $code) || preg_match('/^[0-9]{19}$/', $code)) {
    module_load_include('api.inc', 'bp_fis');
    $response = _bp_fis_api_claim_discount($code);

    if (empty($response)) {
      form_set_error('form', t('There was a problem with the request. Please, refresh the page and try again.'));
    }
    else {
      if (!$response->responseStatus) {
        switch ($response->errorCode) {
          case "INVALID_CARD_NUMBER_ERROR":
            form_set_error('code', t('This code has already been claimed.'));
            break;
          case "CARD_ALREADY_REDEEMED_ERROR":
            form_set_error('code', t('This code has already been claimed.'));
            break;
          case "CARD_ALREADY_POOLED_ERROR":
            form_set_error('code', t('This code has already been claimed.'));
            break;
          case "CARD_NUMBER_NOT_EXIST_ERROR":
            form_set_error('code', t('This code doesn\'t match our records. Please try again.'));
            break;
          case "NOT_CARD_OR_CODE_PROGRAM_ERROR":
            form_set_error('code', t('This code doesn\'t match our records. Please try again.'));
            break;
          case "CARD_NUMBER_EXPIRED_ERROR":
            form_set_error('code', t('This code is no longer valid.'));
            break;
          default:
            form_set_error('code', t('Invalid code. Try again.'));
            break;
        }
        return;
      }
      else {
        $form_state['response'] = $response;
      }
    }
  }
  else {
    form_set_error('code', t('Invalid Code. Try Again.'));
  }
}

/**
 * AJAX Callback triggered when claim a discount form have been submitted
 * 
 * @param type $form
 * @param type $form_state
 * @return type
 */
function bp_gadgets_ctype_claim_discount_modal_callback($form, &$form_state) {
  if (form_get_errors()) {
    // Show an error message when the user fails 3 consecutive times claiming a discount 
    if (!isset($_SESSION['claim_discount_errors'])) {
      $_SESSION['claim_discount_errors'] = 1;
    }
    else {
      $_SESSION['claim_discount_errors'] += 1;
    }

    if ($_SESSION['claim_discount_errors'] >2) {
      return array(
        '#markup' => 'There was a problem with the request.  Please, refresh the page and try again.  If you continue to have issues, please contact Customer Service.',
      );
    }
    else {
      return $form;
    }
  }
  else {
    // If no errors, reset the counter
    $_SESSION['claim_discount_errors'] = 0;
  }
  
  // Rebuild the rewards balance pane
  module_load_include('inc', 'bp_gadgets', 'plugins/content_types/bp_gadgets_rewards_balance');
  $rewards_pane = bp_gadgets_ctype_rewards_balance_render('bp_gadgets_rewards_balance', array(), NULL, NULL);
  $rewards_content = $rewards_pane->content;
  
  // Rebuild the Rewards balance menu
  module_load_include('inc', 'bp_fis', 'bp_fis.api');
  $rewards_balance = _bp_fis_api_get_rewards_balance();
  $formatted_rewards_balance = bp_gadgets_format_cents($rewards_balance);
  $rewards_menu_content = '<li id="rewards-balance-menu-wrapper"><a href="#">Rewards Balance<span class="value">'.$formatted_rewards_balance.'</span></a></li>';

  // Show the claim a discount form and its success messages
  if (!form_get_errors()) {
    $form['code']['#default_value'] = '';
    $form['code']['#value'] = '';
  }
  
  $html_form = drupal_render($form);
  $commands[] = ajax_command_insert(NULL, $html_form);
  $commands[] = ajax_command_prepend(NULL, theme('status_messages'));
  

  // Update the Rewards balance block
  $commands[] = ajax_command_replace('#rewards-balance-wrapper', $rewards_content);
  $commands[] = ajax_command_replace('#rewards-balance-menu-wrapper', $rewards_menu_content);
  $commands[] = ajax_command_invoke('.claim-a-discount', 'trigger', array('submitButton'));
  $commands[] = ajax_command_invoke('.my-reward-status', 'trigger', array('submitedButton'));

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}
