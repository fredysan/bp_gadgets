<?php

$plugin = array(
  'title' => t('BP: Featured Articles'),
  'description' => t('BP: Featured Articles'),
  'single' => TRUE,
  'render callback' => 'bp_gadgets_ctype_featured_articles_render',
  'defaults' => array(),
  'edit form' => 'bp_gadgets_ctype_featured_articles_edit_form',
  'category' => array(t('BP')),
  'defaults' => array(),
);

/**
 * Content type settings form.
 */
function bp_gadgets_ctype_featured_articles_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  
  $form['info'] = array(
    '#markup' => l('Rearange the content', 'admin/content/sort/featured-articles-home'),
    '#prefix' => '<p>',
    '#suffix' => '</p>',
  );
  
  return $form;
}

/**
 * Content type submit handler.
 */
function bp_gadgets_ctype_featured_articles_edit_form_submit($form, &$form_state) {
  
}

function bp_gadgets_ctype_featured_articles_render($subtype, $conf, $args, $context) {
  $view = views_get_view('featured_articles');
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
    $row['title'] = $node->title;
    $row['headline'] = isset($node->field_headline['und'][0]['value']) ? $node->field_headline['und'][0]['value'] : '';
    $row['url_image'] = image_style_url('img_style_10', $node->field_thumbnail['und'][0]['uri']);
    $row['url_node'] = url(drupal_get_path_alias('node/' . $node->nid), array('absolute' => FALSE));
    $row['nid'] = $node->nid;
    $rows[] = $row;
  }
  
  $items = array();
  $items['url_more'] = $view->display_handler->display->display_options['link_url'];
  $items['text_more'] = $view->display_handler->display->display_options['use_more_text'];
  
  $block = new stdClass();
  $block->title = t('Featured Articles');
  $block->content = theme('block_bp_featured_articles', array('rows' => $rows, 'items' => $items));

  return $block;
}