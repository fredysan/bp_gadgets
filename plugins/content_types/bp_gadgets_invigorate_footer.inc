<?php

$plugin = array(
  'title' => t('BP Fuels: Invigotate Footer'),
  'description' => t('BP Invigotate Footer'),
  'single' => TRUE,
  'render callback' => 'bp_gadgets_ctype_invigorate_footer_render',
  'defaults' => array(),
  'edit form' => 'bp_gadgets_invigorate_footer_edit_form',
  'category' => array(t('BP')),
);

/**
 * Content type settings form.
 */
function bp_gadgets_invigorate_footer_edit_form($form, &$form_state) {
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
  $form['disclaimer'] = array(
    '#title' => t('Disclaimer'),
    '#type' => 'textarea',
    '#rows' => 3,
    '#default_value' => isset($conf['disclaimer']) ? $conf['disclaimer'] : '',
  );

  return $form;
}

/**
 * Content type submit handler.
 */
function bp_gadgets_invigorate_footer_edit_form_submit($form, &$form_state) {
  foreach (array('title', 'text', 'disclaimer') as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

function bp_gadgets_ctype_invigorate_footer_render($subtype, $conf, $args, $context) {
  $items = array(
    'title' => $conf['title'],
    'text' => $conf['text'],
    'disclaimer' => $conf['disclaimer'],
  );
  $block = new stdClass();
  $block->title = t('Invigorate footer');
  $block->content = theme('block_bp_invigorate_footer', array('items' => $items));

  return $block;
}
