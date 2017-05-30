<?php

$plugin = array(
  'title' => t('BP: Welcome Back'),
  'description' => t('BP Welcome Back'),
  'single' => TRUE,
  'edit form' => 'bp_gadgets_ctype_welcome_back_edit_form',
  'render callback' => 'bp_gadgets_ctype_welcome_back_render',
  'category' => array(t('BP')),
  'defaults' => array(),
);

/**
 * Content type settings form.
 */
function bp_gadgets_ctype_welcome_back_edit_form($form, &$form_state) {
  // Fix "First argument is expected to be a valid callback"
  form_load_include($form_state, 'inc', 'bp_gadgets', 'plugins/content_types/bp_gadgets_welcome_back');

  $conf = $form_state['conf'];

  $form['pane_id'] = array(
    '#title' => t('Content Unique ID'),
    '#type' => 'textfield',
    '#default_value' => $conf['pane_id'],
    '#required' => TRUE,
    '#weight' => 0,
  );

  return $form;
}

/**
 * Submit for the ctype settings
 *
 * @param type $form
 * @param type $form_state
 */
function bp_gadgets_ctype_welcome_back_edit_form_submit($form, &$form_state) {
  $keys = array(
    'pane_id',
  );
  foreach ($keys as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}


/**
 * Function callback for plugin: BP: Welcome Back
 *
 * @param $subtype
 * @param $conf
 * @param $args
 * @param $context
 *
 * @return \stdClass
 */
function bp_gadgets_ctype_welcome_back_render($subtype, $conf, $args, $context, $incoming_content) {
  global $user;

  if ($user->uid == 1) {
    $user_first_name = 'Admin';
  }
  else {
    $user_wrapper = entity_metadata_wrapper('user', $user);
    $user_first_name = $user_wrapper->field_user_first_name->value();
  }

  module_load_include('inc', 'bp_fis', 'bp_fis.api');
  module_load_include('inc', 'bp_station_finder', 'bp_station_finder.api');
  module_load_include('inc', 'bp_gadgets', 'plugins/content_types/includes/bp_gadgets_common');

  $cents = _bp_fis_api_get_rewards_balance();
  $rewards_balance = bp_gadgets_format_cents($cents);
  $last_transaction = _bp_fis_api_get_last_transaction('Gas Purchase');

  $last_visit = NULL;
  $gallons = NULL;
  if (!empty($last_transaction)) {
    $last_visit = $last_transaction->location->locationAddress;
    $station_address_saved = bp_gadgets_station_exists($station_address);
    $gallons = $last_transaction->location->actualGallonsPump;

    $station_nid = _bp_station_finder_get_station_nid($last_transaction->location->locationCode);
    $fivestar = NULL;
    if (!empty($station_nid)) {
      $station_node = node_load($station_nid);
      $fivestar = _bp_gadgets_fivestar_form($station_node);
    }
  }

  $welcome_text = 'Welcome Back, ';

  $block = new stdClass();
  $block->title = NULL;
  $block->content = theme('block_bp_welcome_back', array(
    'pane_id' => $conf['pane_id'],
    'user_first_name' => $user_first_name,
    'reward_balance' => $rewards_balance,
    'last_visit' => $last_visit,
    'station_address_saved' => $station_address_saved,
    'gallons' => $gallons,
    'fivestar' => $fivestar,
    'welcome_text' => $welcome_text,
  ));

  return $block;
}
