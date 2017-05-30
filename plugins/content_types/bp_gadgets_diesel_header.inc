<?php

$plugin = array(
  'title' => t('BP Fuels: Diesel Header'),
  'description' => t('BP Fuels Diesel Header'),
  'single' => TRUE,
  'render callback' => 'bp_gadgets_ctype_diesel_header_render',
  'defaults' => array(),
  'edit form' => 'bp_gadgets_diesel_header_edit_form',
  'category' => array(t('BP')),
);

/**
 * Content type settings form.
 */
function bp_gadgets_diesel_header_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  
  $form['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textarea',
    '#rows' => 3,
    '#default_value' => isset($conf['title']) ? $conf['title'] : '',
  );
  $form['text'] = array(
    '#title' => t('Text'),
    '#type' => 'textarea',
    '#rows' => 3,
    '#default_value' => isset($conf['text']) ? $conf['text'] : '',
  );
  
  return $form;
}

/**
 * Content type submit handler.
 */
function bp_gadgets_diesel_header_edit_form_submit($form, &$form_state) {
  foreach (array('title', 'text') as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

function bp_gadgets_ctype_diesel_header_render($subtype, $conf, $args, $context) {
  $items = array(
    'title' => $conf['title'],
    'text' => $conf['text'],
  );
  $block = new stdClass();
  $block->title = t('Diesel Header');
  $block->content = theme('block_bp_diesel_header', array('items' => $items));

  return $block;
}
