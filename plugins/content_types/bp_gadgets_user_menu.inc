<?php

$plugin = array(
  'title' => t('BP: User Menu'),
  'description' => t('BP: User Menu'),
  'single' => TRUE,
  'render callback' => 'bp_gadgets_ctype_user_menu_render',
  'category' => array(t('BP')),
  'defaults' => array(),
);

/**
 * Function callback for plugin: BP: User Menu
 *
 * @param $subtype
 * @param $conf
 * @param $args
 * @param $context
 *
 * @return \stdClass
 */
function bp_gadgets_ctype_user_menu_render($subtype, $conf, $args, $context) {
  global $user;
  $menu = menu_tree_all_data('menu-user-menu-2');
  
  $links = array();
  foreach ($menu as $item) {
    $link = array(
      'href' => url($item['link']['href']),
      'title' => $item['link']['title'],
      'active' => FALSE,
    );
    
    if (drupal_match_path(current_path(), $item['link']['href'])) {
      $link['active'] = TRUE;
    }
    
    $links[] = $link;
  }

  $vars = array(
    'links' => $links,
  );

  $block = new stdClass();
  $block->title = NULL;
  $block->content = theme('block_bp_user_menu', array('v' => $vars));

  return $block;
}
