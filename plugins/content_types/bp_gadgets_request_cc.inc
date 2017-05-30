<?php

$plugin = array(
  'title' => t('BP: Request a Card'),
  'description' => t('BP: Request a Card'),
  'single' => TRUE,
  'edit form' => 'bp_gadgets_ctype_request_card_edit_form',
  'render callback' => 'bp_gadgets_ctype_request_card_render',
  'defaults' => array(
    'title' => '',
    'text' => '',
  ),
  'category' => array(t('BP')),
);

/**
 * Content type settings form.
 */
function bp_gadgets_ctype_request_card_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#default_value' => $conf['title'],
  );

  $form['text'] = array(
    '#title' => t('Text'),
    '#type' => 'textarea',
    '#rows' => 3,
    '#default_value' => $conf['text'],
  );

  return $form;
}

/**
 * Content type submit handler.
 */
function bp_gadgets_ctype_request_card_edit_form_submit($form, &$form_state) {
  foreach (array('title', 'text') as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

function bp_gadgets_ctype_request_card_render($subtype, $conf, $args, $context) {
  $vars = array(
    'title' => $conf['title'],
    'text' => $conf['text'],
  );
  $block = new stdClass();
  $block->title = t('Request a Card');
  $block->content = theme('block_bp_request_cc', array('v' => $vars));

  return $block;
}
