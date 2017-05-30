<?php

$plugin = array(
  'title' => t('BP: Rewards Balance'),
  'description' => t('BP: Rewards Balance'),
  'single' => TRUE,
  'render callback' => 'bp_gadgets_ctype_rewards_balance_render',
  'defaults' => array(),
  'edit form' => 'bp_gadgets_ctype_rewards_balance_edit_form',
  'category' => array(t('BP')),
);

/**
 * Content type settings form.
 */
function bp_gadgets_ctype_rewards_balance_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  return $form;
}

/**
 * Content type submit handler.
 */
function bp_gadgets_ctype_rewards_balance_edit_form_submit($form, &$form_state) {
  
}

function bp_gadgets_ctype_rewards_balance_render($subtype, $conf, $args, $context) {
  module_load_include('inc', 'bp_fis', 'bp_fis.api');
  $rewards_balance = _bp_fis_api_get_rewards_balance();
  $balance_details = _bp_fis_api_get_reward_details();
  usort($balance_details, 'bp_gadgets_rewards_order');

  foreach ($balance_details as $key => &$detail) {
    $detail['amount'] = bp_gadgets_format_cents($detail['amount'], TRUE);
  }

  $show_more_classes = '';
  switch (count($balance_details)) {
    case 1:
      $show_more_classes = 'hide';
      break;
    case 2:
    case 3:
      $show_more_classes = 'hidden-sm hidden-md hidden-lg';
      break;
  }

  $vars = array(
    'rewards_balance' => bp_gadgets_format_cents($rewards_balance),
    'balance_details' => $balance_details,
    'show_more_classes' => $show_more_classes,
  );

  $block = new stdClass();
  $block->title = t('My Rewards');
  $block->content = theme('block_bp_rewards_balance', array('v' => $vars));

  return $block;
}

/**
 * Sorts the rewards list this way:
 * 
 * 1st. DR records
 * 2nd. Card, Code records sorted descendantly by poolDate
 */
function bp_gadgets_rewards_order($a, $b) {
  if ($a['type'] == $b['type']) {
    $firstDate = strtotime(($a['poolDate'] == 'N/A') ? '01/01/1900' : $a['poolDate']);
    $secondDate = strtotime(($b['poolDate'] == 'N/A') ? '01/01/1900' : $b['poolDate']);
    $diffTime = $firstDate - $secondDate;
    if ($diffTime == 0) {
      return 0;
    }
    return ($firstDate < $secondDate) ? 1 : -1;
  }
  
  return $a['type'] == 'BP Driver Rewards' ? -1 : 1;
}
