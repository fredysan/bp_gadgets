<?php

$plugin = array(
  'title' => t('BP Fuels: Invigotate Hero'),
  'description' => t('BP Invigotate Hero'),
  'single' => TRUE,
  'render callback' => 'bp_gadgets_ctype_invigorate_hero_render',
  'defaults' => array(),
  'edit form' => 'bp_gadgets_invigorate_hero_edit_form',
  'category' => array(t('BP')),
);

/**
 * Content type settings form.
 */
function bp_gadgets_invigorate_hero_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textarea',
    '#rows' => 3,
    '#default_value' => isset($conf['title']) ? $conf['title'] : '',
  );
  return $form;
}

/**
 * Content type submit handler.
 */
function bp_gadgets_invigorate_hero_edit_form_submit($form, &$form_state) {
  foreach (array('title') as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

function bp_gadgets_ctype_invigorate_hero_render($subtype, $conf, $args, $context) {
  $items = array(
    'title' => isset($conf['title']) ? filter_xss_admin($conf['title']) : '',
  );
  $block = new stdClass();
  $block->title = t('Invigorate hero');
  $block->content = theme('block_bp_invigorate_hero', array('items' => $items));

  return $block;
}
