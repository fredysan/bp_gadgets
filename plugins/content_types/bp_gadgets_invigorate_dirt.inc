<?php

$plugin = array(
  'title' => t('BP Fuels: Invigotate Dirt'),
  'description' => t('BP Invigotate Dirt'),
  'single' => TRUE,
  'render callback' => 'bp_gadgets_ctype_invigorate_dirt_render',
  'defaults' => array(),
  'edit form' => 'bp_gadgets_invigorate_dirt_edit_form',
  'category' => array(t('BP')),
);

/**
 * Content type settings form.
 */
function bp_gadgets_invigorate_dirt_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  for ($i = 1; $i <= 4; $i++) {
    $section_name = 'section' . $i;
    $form[$section_name] = array(
      '#type' => 'fieldset',
      '#title' => t('Section ' . $i),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,
      '#weight' => $i,
    );
    $form[$section_name]['text'] = array(
      '#title' => t('Text'),
      '#type' => 'textarea',
      '#rows' => 3,
      '#default_value' => isset($conf[$section_name]['text']) ? $conf[$section_name]['text'] : '',
    );
    $form[$section_name]['disclaimer'] = array(
      '#title' => t('Disclaimer'),
      '#type' => 'textarea',
      '#rows' => 3,
      '#default_value' => isset($conf[$section_name]['disclaimer']) ? $conf[$section_name]['disclaimer'] : '',
    );
  }

  return $form;
}

/**
 * Content type submit handler.
 */
function bp_gadgets_invigorate_dirt_edit_form_submit($form, &$form_state) {
  foreach (array('section1', 'section2', 'section3', 'section4') as $key) {
    $form_state['conf'][$key] = array(
      'text' => $form_state['values'][$key]['text'],
      'disclaimer' => $form_state['values'][$key]['disclaimer'],
    );
  }
}

function bp_gadgets_ctype_invigorate_dirt_render($subtype, $conf, $args, $context) {
  $items = array(
    'section1' => $conf['section1'],
    'section2' => $conf['section2'],
    'section3' => $conf['section3'],
    'section4' => $conf['section4'],
  );
  $block = new stdClass();
  $block->title = t('Invigorate dirt');
  $block->content = theme('block_bp_invigorate_dirt', array('items' => $items));

  return $block;
}
