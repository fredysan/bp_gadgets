<?php

$plugin = array(
  'title' => t('BP: Card'),
  'description' => t('BP: Card'),
  'single' => FALSE,
  'admin info' => 'bp_gadgets_ctype_card_admin_info',
  'all contexts' => TRUE,
);

/**
 * Provide a summary description.
 */
function bp_gadgets_ctype_card_admin_info($conf, $context) {
  $block = new stdClass();
  $block->title = $conf . ' - ' . $context['title'];
  return $block;
}

/**
 * Let us to define multiple versions of the same ctools content type
 *
 * @return type
 */
function bp_gadgets_bp_gadgets_card_content_type_content_types() {
  return array(
    'card_img_left' => array(
      'title' => 'BP: Content Card - Image left',
      'edit form' => 'bp_gadgets_ctype_card_edit_form',
      'render callback' => 'bp_gadgets_ctype_card_img_left_render',
      'category' => array(t('BP')),
    ),
    'card_img_right' => array(
      'title' => 'BP: Content Card - Image right',
      'edit form' => 'bp_gadgets_ctype_card_edit_form',
      'render callback' => 'bp_gadgets_ctype_card_img_right_render',
      'category' => array(t('BP')),
    ),
    'card_login' => array(
      'title' => 'BP: Content Card - Login / Apply',
      'edit form' => 'bp_gadgets_ctype_card_edit_form',
      'render callback' => 'bp_gadgets_ctype_card_login_render',
      'category' => array(t('BP')),
    ),
    'card_hero' => array(
      'title' => 'BP: Content Card - Hero Section',
      'edit form' => 'bp_gadgets_ctype_card_edit_form',
      'render callback' => 'bp_gadgets_ctype_card_hero_render',
      'category' => array(t('BP')),
    ),
    'card_hero_login' => array(
      'title' => 'BP: Content Card - Hero Section with login',
      'edit form' => 'bp_gadgets_ctype_card_hero_login_edit_form',
      'render callback' => 'bp_gadgets_ctype_card_hero_login_render',
      'category' => array(t('BP')),
      'defaults' => array(
        'btns' => array(),
      ),
    ),
  );
}

/**
 * Content type settings form.
 */
function bp_gadgets_ctype_card_edit_form($form, &$form_state) {
  // Fix "First argument is expected to be a valid callback"
  form_load_include($form_state, 'inc', 'bp_gadgets', 'plugins/content_types/bp_gadgets_card');
  module_load_include('inc', 'bp_gadgets', 'plugins/content_types/includes/bp_gadgets_common');

  $conf = $form_state['conf'];

  $form['pane_id'] = array(
    '#title' => t('Content Unique ID'),
    '#type' => 'textfield',
    '#default_value' => $conf['pane_id'],
    '#required' => TRUE,
    '#weight' => 0,
  );
  $form['caption'] = array(
    '#title' => t('Caption'),
    '#type' => 'textfield',
    '#default_value' => $conf['caption'],
    '#weight' => 1,
  );
  $form['image_fid'] = array(
    '#title' => t('Background Image for Desktop'),
    '#type' => 'managed_file',
    '#default_value' => $conf['image_fid'],
    '#upload_location' => 'public://',
    '#required' => TRUE,
    '#weight' => 2,
  );
  $form['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textarea',
    '#rows' => 3,
    '#default_value' => $conf['title'],
    '#required' => TRUE,
    '#weight' => 6,
  );
  $form['text'] = array(
    '#title' => t('Text'),
    '#type' => 'textarea',
    '#rows' => 3,
    '#default_value' => $conf['text'],
    '#weight' => 7,
  );

  $form['btns'] = array(
    '#title' => t('Buttons'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
    '#weight' => 8,
  );

  for ($i = 0; $i < 2; $i++) {
    $form['btns'][$i] = array(
      '#title' => t('Button ' . ($i + 1)),
      '#type' => 'fieldset',
      '#tree' => TRUE,
    );
    $form['btns'][$i]['title'] = array(
      '#title' => t('Title'),
      '#type' => 'textfield',
      '#default_value' => $conf['btns'][$i]['title'],
    );
    $form['btns'][$i]['url'] = array(
      '#title' => t('Url'),
      '#type' => 'textfield',
      '#default_value' => $conf['btns'][$i]['url'],
    );
    $form['btns'][$i]['class'] = array(
      '#title' => t('Design'),
      '#type' => 'select',
      '#options' => _bp_gadgets_ctype_get_button_classes(),
      '#default_value' => $conf['btns'][$i]['class'],
    );
  }

  $form['links'] = array(
    '#title' => t('Links'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
    '#weight' => 9,
  );

  for ($i = 0; $i < 2; $i++) {
    $form['links'][$i] = array(
      '#title' => t('Link ' . ($i + 1)),
      '#type' => 'fieldset',
      '#tree' => TRUE,
    );
    $form['links'][$i]['title'] = array(
      '#title' => t('Title'),
      '#type' => 'textfield',
      '#default_value' => $conf['links'][$i]['title'],
    );
    $form['links'][$i]['url'] = array(
      '#title' => t('Url'),
      '#type' => 'textfield',
      '#default_value' => $conf['links'][$i]['url'],
    );
  }
  
  return $form;
}

/**
 *
 * @param type $form
 * @param type $form_state
 */
function bp_gadgets_ctype_card_edit_form_submit($form, &$form_state) {
  $keys = array(
    'pane_id',
    'caption',
    'image_fid',
    'title',
    'text',
    'btns',
    'links',
  );
  foreach ($keys as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }

  if (!empty($form_state['conf']['image_fid'])) {
    $file = file_load($form_state['conf']['image_fid']);
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    file_usage_add($file, 'file', 'bp_gadgets', $form_state['conf']['image_fid']);
  }
}

/**
 *
 * @param type $form
 * @param type $form_state
 * @return type
 */
function bp_gadgets_ctype_card_hero_login_edit_form($form, &$form_state) {
  $form = bp_gadgets_ctype_card_edit_form($form, $form_state);

  $conf = $form_state['conf'];
  
  // Hero login shows three different images
  unset($form['image_fid']);
  // Hero Login card has predefined buttons
  unset($form['btns']);

  $form['image_md_fid'] = array(
    '#title' => t('Image Desktop'),
    '#type' => 'managed_file',
    '#default_value' => $conf['image_md_fid'],
    '#upload_location' => 'public://',
    '#required' => TRUE,
    '#weight' => 2,
  );
  $form['image_sm_fid'] = array(
    '#title' => t('Image Tablet'),
    '#type' => 'managed_file',
    '#default_value' => $conf['image_sm_fid'],
    '#upload_location' => 'public://',
    '#required' => TRUE,
    '#weight' => 3,
  );
  $form['image_xs_fid'] = array(
    '#title' => t('Image Mobile'),
    '#type' => 'managed_file',
    '#default_value' => $conf['image_xs_fid'],
    '#upload_location' => 'public://',
    '#required' => TRUE,
    '#weight' => 4,
  );
  $form['image2_fid'] = array(
    '#title' => t('Product Image'),
    '#type' => 'managed_file',
    '#default_value' => $conf['image2_fid'],
    '#upload_location' => 'public://',
    '#required' => FALSE,
    '#weight' => 5,
  );

  
  return $form;
}

/**
 *
 * @param type $form
 * @param type $form_state
 */
function bp_gadgets_ctype_card_hero_login_edit_form_submit($form, &$form_state) {
  $keys = array(
    'pane_id',
    'caption',
    'image_md_fid',
    'image_sm_fid',
    'image_xs_fid',
    'image2_fid',
    'title',
    'text',
    'links',
  );
  foreach ($keys as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }

  $images = array(
    'image_md_fid',
    'image_sm_fid',
    'image_xs_fid',
    'image2_fid',
  );
  foreach ($images as $image) {
    if (!empty($form_state['conf'][$image])) {
      $file = file_load($form_state['conf'][$image]);
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      file_usage_add($file, 'file', 'bp_gadgets', $form_state['conf'][$image]);
    }
  }
}

/**
 * Render for the Image left card
 *
 * @param type $subtype
 * @param type $conf
 * @param type $args
 * @param type $context
 * @return \stdClass
 */
function bp_gadgets_ctype_card_img_left_render($subtype, $conf, $args, $context) {
  $vars = _bp_gadgets_ctype_card_get_items($conf);

  $block = new stdClass();
  $block->title = t('Card: Image Left');
  $block->content = theme('block_bp_card_img_left', array('v' => $vars));

  return $block;
}

/**
 * Render for the Image right card
 *
 * @param type $subtype
 * @param type $conf
 * @param type $args
 * @param type $context
 * @return \stdClass
 */
function bp_gadgets_ctype_card_img_right_render($subtype, $conf, $args, $context) {
  $vars = _bp_gadgets_ctype_card_get_items($conf);

  $block = new stdClass();
  $block->title = t('Card: Image Right');
  $block->content = theme('block_bp_card_img_right', array('v' => $vars));

  return $block;
}

/**
 *
 * @param type $subtype
 * @param type $conf
 * @param type $args
 * @param type $context
 * @return \stdClass
 */
function bp_gadgets_ctype_card_login_render($subtype, $conf, $args, $context) {
  $vars = _bp_gadgets_ctype_card_get_items($conf);

  $block = new stdClass();
  $block->title = t('Card: Login / Apply');
  $block->content = theme('block_bp_card_login', array('v' => $vars));

  return $block;
}

/**
 *
 * @param type $subtype
 * @param type $conf
 * @param type $args
 * @param type $context
 * @return \stdClass
 */
function bp_gadgets_ctype_card_hero_render($subtype, $conf, $args, $context) {
  $image_url_lg = '';
  $image_url_md = '';
  $image_url_sm = '';
  if (!empty($conf['image_fid'])) {
    $file = file_load($conf['image_fid']);
    $image_url_lg = image_style_url('img_style_2', $file->uri);
    $image_url_md = image_style_url('img_style_3', $file->uri);
    $image_url_sm = image_style_url('img_style_4', $file->uri);
  }

  $buttons = _bp_gadgets_ctype_card_get_buttons($conf);
  $links = _bp_gadgets_ctype_card_get_links($conf);

  $vars = array(
    'pane_id' => $conf['pane_id'],
    'url_image_lg' => $image_url_lg,
    'url_image_md' => $image_url_md,
    'url_image_sm' => $image_url_sm,
    'caption' => $conf['caption'],
    'title' => $conf['title'],
    'text' => $conf['text'],
    'buttons' => $buttons,
    'links' => $links,
  );

  $block = new stdClass();
  $block->title = t('Card: Hero');
  $block->content = theme('block_bp_card_hero', array('v' => $vars));

  return $block;
}

/**
 *
 * @param type $subtype
 * @param type $conf
 * @param type $args
 * @param type $context
 * @return \stdClass
 */
function bp_gadgets_ctype_card_hero_login_render($subtype, $conf, $args, $context) {
  $image_url_lg = '';
  $image_url_md = '';
  $image_url_sm = '';
  
  if (!empty($conf['image_md_fid'])) {
    $file = file_load($conf['image_md_fid']);
    $image_url_md = file_create_url($file->uri);
  }
  if (!empty($conf['image_sm_fid'])) {
    $file = file_load($conf['image_sm_fid']);
    $image_url_sm = file_create_url($file->uri);
  }
  if (!empty($conf['image_xs_fid'])) {
    $file = file_load($conf['image_xs_fid']);
    $image_url_xs = file_create_url($file->uri);
  }

  $product_image_url = NULL;
  if (!empty($conf['image2_fid'])) {
    $file = file_load($conf['image2_fid']);
    $product_image_url = file_create_url($file->uri);
  }

  $links = _bp_gadgets_ctype_card_get_links($conf);

  $vars = array(
    'pane_id' => $conf['pane_id'],
    'url_image_md' => $image_url_md,
    'url_image_sm' => $image_url_sm,
    'url_image_xs' => $image_url_xs,
    'url_product_image' => $product_image_url,
    'caption' => $conf['caption'],
    'title' => $conf['title'],
    'text' => $conf['text'],
    'links' => $links,
    'url_join' => url('join-bp-driver-rewards'),
  );

  $block = new stdClass();
  $block->title = t('Card: Hero');
  $block->content = theme('block_bp_card_hero_login', array('v' => $vars));

  return $block;
}

/**
 * Return variables that will be pass to templates
 *
 * @param type $conf
 * @return type
 */
function _bp_gadgets_ctype_card_get_items($conf) {
  $image_url = '';
  if (!empty($conf['image_fid'])) {
    $file = file_load($conf['image_fid']);
    $image_url_lg = image_style_url('img_style_7', $file->uri);
    $image_url_md = image_style_url('img_style_8', $file->uri);
    $image_url_sm = image_style_url('img_style_9', $file->uri);
  }

  $buttons = _bp_gadgets_ctype_card_get_buttons($conf);
  $links = _bp_gadgets_ctype_card_get_links($conf);

  $items = array(
    'pane_id' => isset($conf['pane_id']) ? $conf['pane_id'] : '',
    'image_url_lg' => $image_url_lg,
    'image_url_md' => $image_url_md,
    'image_url_sm' => $image_url_sm,
    'caption' => $conf['caption'],
    'title' => $conf['title'],
    'text' => $conf['text'],
    'buttons' => $buttons,
    'links' => $links,
  );

  return $items;
}

/**
 * Return buttons with the right format
 *
 * @param type $conf
 * @return array
 */
function _bp_gadgets_ctype_card_get_buttons($conf) {
  $buttons = array();
  if (isset($conf['btns'])) {
    foreach ($conf['btns'] as $btn) {
      if (!empty($btn['title']) && !empty($btn['url'])) {
        $url = drupal_parse_url($btn['url']);
        $button['url'] = url($url['path'], array('query' => $url['query']));
        $button['title'] = $btn['title'];
        $buttons[] = $button;
      }
    }
  }

  return $buttons;
}

/**
 * Return buttons with the right format
 *
 * @param type $conf
 * @return array
 */
function _bp_gadgets_ctype_card_get_links($conf) {
  $links = array();
  if (isset($conf['links'])) {
    foreach ($conf['links'] as $link) {
      if (!empty($link['title']) && !empty($link['url'])) {
        $url = drupal_parse_url($link['url']);
        $link['url'] = url($url['path'], array('query' => $url['query']));
        $links[] = $link;
      }
    }
  }

  return $links;
}
