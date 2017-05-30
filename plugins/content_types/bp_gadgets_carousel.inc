<?php

$plugin = array(
  'title' => t('BP: Carousel'),
  'description' => t('BP Carousel'),
  'single' => TRUE,
  'render callback' => 'bp_gadgets_ctype_carousel_render',
  'defaults' => array(),
  'edit form' => 'bp_gadgets_ctype_carousel_edit_form',
  'category' => array(t('BP')),
  'defaults' => array(
    'rotation_delay' => '5'
  ),
);

/**
 * Content type settings form.
 */
function bp_gadgets_ctype_carousel_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  
  $form['rotation_delay'] = array(
    '#type' => 'textfield',
    '#title' => t('Rotation delay (Seconds)'),
    '#size' => 1,
    '#maxlength' => 1,
    '#number_type' => 'integer',
    '#element_validate' => array ('number_field_widget_validate'),
    '#default_value' => $conf['rotation_delay'],
  );
  
  $form['info'] = array(
    '#markup' => l('Rearange the content', 'admin/content/sort/carousel'),
    '#prefix' => '<p>',
    '#suffix' => '</p>',
  );
  
  return $form;
}

/**
 * Content type submit handler.
 */
function bp_gadgets_ctype_carousel_edit_form_submit($form, &$form_state) {
  foreach (array('rotation_delay') as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

function bp_gadgets_ctype_carousel_render($subtype, $conf, $args, $context) {
  // Rotation delay
  drupal_add_js(array('bp_gadgets' => array('carousel' => array('delay' => $conf['rotation_delay']))), array('type' => 'setting'));
  
  $view = views_get_view('slides');
  $view->set_display('block');
  $view->pre_execute();
  $view->execute();

  $nids = array();
  foreach ($view->result as $row) {
    $nids[] = $row->nid;
  }
  $nodes = node_load_multiple($nids);

  $rows = array();
  foreach ($nodes as $node) {
    $row = array();
    $row['title'] = $node->field_slide_title['und'][0]['value'];
    $row['subtitle'] = isset($node->field_slide_subtitle['und'][0]['value']) ? $node->field_slide_subtitle['und'][0]['value'] : '';
    $row['url_image'] = $node->field_slide_image['und'][0]['uri'];

    $links = array();
    if (isset($node->field_slide_links['und'])) {
      foreach ($node->field_slide_links['und'] as $link) {
        $l = array(
          'url' => url($link['url']),
          'title' => $link['title'],
        );
        $links[] = $l;
      }
    }
    $row['links'] = $links;

    $buttons = array();
    if (isset($node->field_slide_buttons['und'])) {
      foreach ($node->field_slide_buttons['und'] as $button) {
        $b = array(
          'url' => url($button['url']),
          'title' => $button['title'],
        );
        $buttons[] = $b;
      }
    }
    $row['buttons'] = $buttons;
    
    $rows[] = $row;
  }

  $block = new stdClass();
  $block->title = t('Carousel');
  $block->content = theme('block_bp_carousel', array('rows' => $rows));

  return $block;
}
