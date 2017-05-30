<?php

$plugin = array(
  'title' => t('BP Fuels: Invigotate Tips'),
  'description' => t('BP Invigotate Tips'),
  'single' => TRUE,
  'render callback' => 'bp_gadgets_ctype_invigorate_tips_render',
  'defaults' => array(),
  'edit form' => 'bp_gadgets_invigorate_tips_edit_form',
  'category' => array(t('BP')),
);

/**
 * Content type settings form.
 */
function bp_gadgets_invigorate_tips_edit_form($form, &$form_state) {
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
    $form[$section_name]['title'] = array(
      '#title' => t('Title'),
      '#type' => 'textarea',
      '#rows' => 3,
      '#default_value' => isset($conf[$section_name]['title']) ? $conf[$section_name]['title'] : '',
    );
    $form[$section_name]['text'] = array(
      '#title' => t('Text'),
      '#type' => 'textarea',
      '#rows' => 3,
      '#default_value' => isset($conf[$section_name]['text']) ? $conf[$section_name]['text'] : '',
    );
    $form[$section_name]['legal_text'] = array(
      '#title' => t('Legal text'),
      '#type' => 'textarea',
      '#rows' => 2,
      '#default_value' => isset($conf[$section_name]['legal_text']) ? $conf[$section_name]['legal_text'] : '',
    );
  }
  
  $form['section3']['video'] = array(
    '#type' => 'textfield',
    '#title' => t('Video URL'),
    '#default_value' => isset($conf['section3']['video']) ? $conf['section3']['video'] : '',
    '#maxlength' => 120,
    '#required' => TRUE,
  );
  
  return $form;
}

/**
 * Content type submit handler.
 */
function bp_gadgets_invigorate_tips_edit_form_submit($form, &$form_state) {
  foreach (array('section1', 'section2', 'section3', 'section4') as $key) {
    $form_state['conf'][$key] = array(
      'title' => $form_state['values'][$key]['title'],
      'text' => $form_state['values'][$key]['text'],
      'legal_text' => $form_state['values'][$key]['legal_text'],
    );
    if (isset($form_state['values'][$key]['video'])) {
      $form_state['conf'][$key]['video'] = $form_state['values'][$key]['video'];
    }
  }
}

function bp_gadgets_ctype_invigorate_tips_render($subtype, $conf, $args, $context) {
  $items = array(
    'section1' => $conf['section1'],
    'section2' => $conf['section2'],
    'section3' => $conf['section3'],
    'section4' => $conf['section4'],
  );
  
  $video_url = file_create_url(file_build_uri($items['section3']['video']));
  $items['section3']['video'] = $video_url;
  
  $block = new stdClass();
  $block->title = t('Invigorate tips');
  $block->content = theme('block_bp_invigorate_tips', array('items' => $items));

  return $block;
}
