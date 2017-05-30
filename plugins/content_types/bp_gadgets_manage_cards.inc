<?php

$plugin = array(
  'title' => t('BP: Manage linked cards'),
  'description' => t('BP: Manage linked cards'),
  'single' => TRUE,
  'edit form' => 'bp_gadgets_ctype_manage_cards_edit_form',
  'render callback' => 'bp_gadgets_ctype_manage_cards_render',
  'category' => array(t('BP')),
  'defaults' => array(
    'title' => '',
    'text' => '',
  ),
);

/**
 * Content type settings form.
 */
function bp_gadgets_ctype_manage_cards_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  
  $form['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => $conf['title'],
  );
  
  $form['text'] = array(
    '#title' => t('Text'),
    '#type' => 'textarea',
    '#rows' => 3,
    '#required' => TRUE,
    '#default_value' => $conf['text'],
  );
  
  return $form;
}

/**
 * Content type submit handler.
 */
function bp_gadgets_ctype_manage_cards_edit_form_submit($form, &$form_state) {
  foreach (array('title', 'text') as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Returns the rendered block
 */
function bp_gadgets_ctype_manage_cards_render($subtype, $conf, $args, $context) {
  
  module_load_include('inc', 'bp_fis', 'bp_fis.api');
  $linked_cards = _bp_fis_api_get_user_credit_cards();

  $linked_cards = bp_earning_preferences_format_credit_cards($linked_cards);
  
  module_load_include('inc', 'bp_gadgets', 'plugins/content_types/includes/bp_gadgets_forms');

  // This is remove cc form.
  $delete_cc_form = drupal_get_form('bp_gadgets_delete_cc_form');
  $delete_cc_modal = theme('modal_delete_cc', array('form' => $delete_cc_form));
  
  $vars = array(
    'title' => $conf['title'],
    'text' => $conf['text'],
    'linked_cards' => $linked_cards,
    'total_cards' => count($linked_cards),
    'delete_cc_modal' => $delete_cc_modal,
  );
  
  $block = new stdClass();
  $block->title = t('Manage linked cards');
  $block->content = theme('block_bp_manage_cc', array('v' => $vars));

  return $block;
}
