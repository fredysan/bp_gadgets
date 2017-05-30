<?php

$plugin = array(
  'title' => t('BP Fuels: Invigotate Fireworks'),
  'description' => t('BP Invigotate Fireworks'),
  'single' => TRUE,
  'render callback' => 'bp_gadgets_ctype_invigorate_fireworks_render',
  'defaults' => array(),
  'edit form' => 'bp_gadgets_invigorate_fireworks_edit_form',
  'category' => array(t('BP')),
);

/**
 * Content type settings form.
 */
function bp_gadgets_invigorate_fireworks_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  
  $form['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textarea',
    '#rows' => 3,
    '#default_value' => isset($conf['title']) ? $conf['title'] : '',
  );
  
  $form['subtitle'] = array(
    '#title' => t('Subtitle'),
    '#type' => 'textarea',
    '#rows' => 3,
    '#default_value' => isset($conf['subtitle']) ? $conf['subtitle'] : '',
  );
  
  return $form;
}

/**
 * Content type submit handler.
 */
function bp_gadgets_invigorate_fireworks_edit_form_submit($form, &$form_state) {
  foreach (array('title', 'subtitle') as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

function bp_gadgets_ctype_invigorate_fireworks_render($subtype, $conf, $args, $context) {
  $items = array(
    'title' => $conf['title'],
    'subtitle' => $conf['subtitle'],
  );
  $block = new stdClass();
  $block->title = t('Invigorate fireworks');
  $block->content = theme('block_bp_invigorate_fireworks', array('items' => $items));

  return $block;
}
