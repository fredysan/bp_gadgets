<?php

$plugin = array(
  'title' => t('BP: Rewards history'),
  'description' => t('BP Rewards history'),
  'single' => TRUE,
  'render callback' => 'bp_gadgets_ctype_rewards_history_render',
  'defaults' => array(),
  'edit form' => 'bp_gadgets_ctype_rewards_history_edit_form',
  'category' => array(t('BP')),
);

/**
 * Content type settings form.
 */
function bp_gadgets_ctype_rewards_history_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  return $form;
}

/**
 * Content type submit handler.
 */
function bp_gadgets_ctype_rewards_history_edit_form_submit($form, &$form_state) {
  
}

function bp_gadgets_ctype_rewards_history_render($subtype, $conf, $args, $context) {
  $block = NULL;
  $transactions = _bp_gadgets_ctype_get_grouped_transactions();

  if (!empty($transactions)) {
    $vars = array(
      'grouped_transactions' => $transactions,
    );

    $block = new stdClass();
    $block->title = t('Rewards History');
    $block->content = theme('block_bp_rewards_history', array('v' => $vars));
  }

  return $block;
}

function _bp_gadgets_ctype_get_grouped_transactions() {
  module_load_include('inc', 'bp_fis', 'bp_fis.api');
  $transactions = _bp_fis_api_get_user_transactions();
  
  $groups = array();
  foreach ($transactions as $transaction) {
    $date = _bp_gadgets_ctype_rewards_history_format_date($transaction->activityDate);
    module_load_include('inc', 'bp_station_finder', 'bp_station_finder.api');
    $station_nid = _bp_station_finder_get_station_nid($transaction->location->locationCode);
    $station_address = $transaction->location->locationAddress;
    module_load_include('inc', 'bp_gadgets', 'plugins/content_types/includes/bp_gadgets_common');
    $url_favorites = _bp_gadgets_get_favorites_link($station_nid);
    $cpg_earned = bp_gadgets_format_dollars($transaction->credit) . ' ' . t('per gallon earned');
    $cpg_redeemed = bp_gadgets_format_dollars($transaction->debit) . ' ' . t('per gallon redeemed');
    $gallons = $transaction->location->actualGallonsPump . ' ' . t('Gallons');

    $key = format_date(strtotime($transaction->activityDate), 'custom', 'Y-m');
    if (!array_key_exists($key, $groups)) {
      $groups[$key] = array(
        'month' => format_date(strtotime($transaction->activityDate), 'custom', 'F Y'),
        'transactions' => array(),
      );
    }
    $groups[$key]['transactions'][] = array(
      'date' => $date,
      'type' => $transaction->transactionType,
      'adjustment_reason' => ucfirst(strtolower($transaction->adjReason)),
      'station_address' => $station_address,
      'cpg_earned' => $cpg_earned,
      'cpg_redeemed' => $cpg_redeemed,
      'url_favorites' => $url_favorites,
      'gallons' => $gallons,
    );
  }

  return $groups;
}

/**
 * Returns a formated date
 * 
 * @param string $date
 * @return string
 */
function _bp_gadgets_ctype_rewards_history_format_date($date) {
  $fdate = '';
  if (!empty($date)) {
    $timestamp = strtotime($date);
    if (!empty($timestamp)) {
      $format = 'F j';
      $fdate = format_date($timestamp, 'custom', $format);
    }
  }
  return $fdate;
}
