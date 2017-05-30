<?php

$plugin = array(
  'title' => t('BP: Disclaimer'),
  'description' => t('BP Disclaimer'),
  'single' => TRUE,
  'render callback' => 'bp_gadgets_ctype_disclaimer_render',
  'defaults' => array(),
  'edit form' => 'bp_gadgets_ctype_disclaimer_edit_form',
  'category' => array(t('BP')),
);

/**
 * Content type settings form.
 */
function bp_gadgets_ctype_disclaimer_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  
  $form['text'] = array(
    '#title' => t('Text'),
    '#type' => 'text_format',
    '#format' => isset($conf['text']['format']) ? $conf['text']['format'] : 'filtered_html',
    '#rows' => 3,
    '#default_value' => isset($conf['text']['value']) ? $conf['text']['value'] : '',
  );
  
  return $form;
}

/**
 * Content type submit handler.
 */
function bp_gadgets_ctype_disclaimer_edit_form_submit($form, &$form_state) {
  foreach (array('text') as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

function bp_gadgets_ctype_disclaimer_render($subtype, $conf, $args, $context) {
  $vars = array(
    'text' => isset($conf['text']['value']) ? $conf['text']['value'] : '',
  );
  $block = new stdClass();
  $block->title = t('Disclaimer');
  $block->content = theme('block_bp_disclaimer', array('v' => $vars));

  return $block;
}
