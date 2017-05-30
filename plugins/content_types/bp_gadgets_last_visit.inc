<?php

$plugin = array(
  'title' => t('BP: Last visit'),
  'description' => t('BP: Last visit'),
  'single' => TRUE,
  'render callback' => 'bp_gadgets_ctype_last_visit_render',
  'defaults' => array(),
  'edit form' => 'bp_gadgets_ctype_last_visit_edit_form',
  'category' => array(t('BP')),
);

/**
 * Content type settings form.
 */
function bp_gadgets_ctype_last_visit_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  return $form;
}

/**
 * Content type submit handler.
 */
function bp_gadgets_ctype_last_visit_edit_form_submit($form, &$form_state) {

}

function bp_gadgets_ctype_last_visit_render($subtype, $conf, $args, $context) {
  module_load_include('inc', 'bp_fis', 'bp_fis.api');
  module_load_include('inc', 'bp_station_finder', 'bp_station_finder.api');

  $transaction = _bp_fis_api_get_last_transaction();
  $content = '';

  if (!empty($transaction)) {
    drupal_add_js(drupal_get_path('module', 'bp_gadgets') . '/plugins/content_types/js/favorites.js');
    module_load_include('inc', 'bp_gadgets', 'plugins/content_types/includes/bp_gadgets_common');

    $date = _bp_gadgets_ctype_last_visit_format_date($transaction->activityDate);
    $station_nid = _bp_station_finder_get_station_nid($transaction->location->locationCode);

    $station_address = $transaction->location->locationAddress;
    $station_address_saved = bp_gadgets_station_exists($station_address);
    $cpg_earned = bp_gadgets_format_dollars($transaction->credit) . ' ' . t('per gallon earned');
    $cpg_redeemed = bp_gadgets_format_dollars($transaction->debit) . ' ' . t('per gallon redeemed');
    $gallons = $transaction->location->actualGallonsPump . ' ' . t('Gallons');

    if (!empty($station_nid)) {
      $url_favorites = _bp_gadgets_get_favorites_link($station_nid);
      $station_node = node_load($station_nid);
      $fivestar = _bp_gadgets_fivestar_form($station_node);
    }

    $vars = array(
      'date' => $date,
      'station_address' => $station_address,
      'station_address_saved' => $station_address_saved,
      'station_nid' => $station_address,
      'url_favorites' => $url_favorites,
      'cpg_earned' => $cpg_earned,
      'cpg_redeemed' => $cpg_redeemed,
      'gallons' => $gallons,
      'fivestar' => $fivestar,
    );

    $content = theme('block_bp_last_visit', array('v' => $vars));
  }

  $block = new stdClass();
  $block->title = t('Last Visit');
  $block->content = $content;

  return $block;
}

/**
 * Returns a formated date
 *
 * @param string $date
 * @return string
 */
function _bp_gadgets_ctype_last_visit_format_date($date) {
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

/**
 * Returns the last visited location
 *
 * @param object $transaction
 * @return string
 */
function _bp_gadgets_ctype_last_visit_location($transaction) {
  $location = '';
  $location_code = $transaction->location->locationCode;
  if (!empty($location_code)) {

  }
  return $location;
}

