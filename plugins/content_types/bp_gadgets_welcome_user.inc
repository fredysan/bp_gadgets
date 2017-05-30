<?php

$plugin = array(
  'title' => t('BP: Welcome User'),
  'description' => t('BP: Welcome User'),
  'single' => TRUE,
  'edit form' => 'bp_gadgets_ctype_welcome_user_edit_form',
  'render callback' => 'bp_gadgets_ctype_welcome_user_render',
  'category' => array(t('BP')),
  'defaults' => array(
    'image_fid' => NULL,
  ),
);

/**
 * Content type settings form.
 */
function bp_gadgets_ctype_welcome_user_edit_form($form, &$form_state) {
  // Fix "First argument is expected to be a valid callback"
  form_load_include($form_state, 'inc', 'bp_gadgets', 'plugins/content_types/bp_gadgets_welcome_user');

  $conf = $form_state['conf'];

  $form['image_md_fid'] = array(
    '#title' => t('Image Desktop'),
    '#type' => 'managed_file',
    '#default_value' => $conf['image_md_fid'],
    '#upload_location' => 'public://',
    '#required' => TRUE,
    '#weight' => 1,
  );
  $form['image_sm_fid'] = array(
    '#title' => t('Image Tablet'),
    '#type' => 'managed_file',
    '#default_value' => $conf['image_sm_fid'],
    '#upload_location' => 'public://',
    '#required' => TRUE,
    '#weight' => 1,
  );
  $form['image_xs_fid'] = array(
    '#title' => t('Image Mobile'),
    '#type' => 'managed_file',
    '#default_value' => $conf['image_xs_fid'],
    '#upload_location' => 'public://',
    '#required' => TRUE,
    '#weight' => 1,
  );

  return $form;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 */
function bp_gadgets_ctype_welcome_user_edit_form_submit($form, &$form_state) {
  $keys = array(
    'image_md_fid',
    'image_sm_fid',
    'image_xs_fid',
  );
  
  foreach ($keys as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
  
  foreach ($keys as $key) {
    if (!empty($form_state['conf'][$key])) {
      $file = file_load($form_state['conf'][$key]);
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      file_usage_add($file, 'file', 'bp_gadgets', $form_state['conf'][$key]);
    }
  }
}

/**
 * Function callback for plugin: BP: Welcome Back
 *
 * @param $subtype
 * @param $conf
 * @param $args
 * @param $context
 *
 * @return \stdClass
 */
function bp_gadgets_ctype_welcome_user_render($subtype, $conf, $args, $context) {
  global $user;

  $description = '';
  $message = '';
  $link = '';

  //drupal_set_message('<pre>'.print_r($_GET,1).'</pre>');

  // Handle promo messaging
  if(isset($_GET['join']) && !empty($_GET['join'])) {
    $cc = 1;
    $dr = 1;
    $nj = 0;
    if(isset($_GET['cc']) && empty($_GET['cc'])) {
      $cc = 0;
    }
    if(isset($_GET['dr']) && empty($_GET['dr'])) {
      $dr = 0;
    }
    if(isset($_GET['nj']) && !empty($_GET['nj'])) {
      $nj = 1;
    }

    if(!empty($nj) && (empty($cc) || empty($dr))) {
      $message = 'Congratulations on joining. However, there was an error. Please link a credit card or request a BP Driver Rewards card.';
      
      if(empty($cc)) {
        $link = l('Try Again', '#', array(
          'attributes' => array(
            'class' => 'link-card',
            'data-toggle' => 'modal',
            'data-target' => '#newCardModal',
          )
        ));
      }
      elseif(empty($dr)) {
        $link = l('Try Again', '#', array(
          'attributes' => array(
            'class' => 'link-card',
            'data-toggle' => 'modal',
            'data-target' => '#requestDRCardModal',
          )
        ));
      }
      else {
        $link = '';
      }
    }
    elseif(!empty($cc) && !empty($dr)) {
      $message = 'Congratulations on joining the BP Driver Rewards Program!';
      $link = l('Link My Card', '#', array(
        'attributes' => array(
          'class' => 'link-card',
          'data-toggle' => 'modal',
          'data-target' => '#newCardModal',
        )
      ));
    }
    elseif (empty($cc) && !empty($dr)) {
      $message = 'Congratulations on joining. However, there was an error in linking your credit card.';
      $link = l('Link My Card', '#', array(
        'attributes' => array(
          'class' => 'link-card',
          'data-toggle' => 'modal',
          'data-target' => '#newCardModal',
        )
      ));
    }
  }
  else{
    module_load_include('api.inc', 'bp_fis');
    $user_cards = _bp_fis_api_get_user_credit_cards();
    if(empty($user_cards)) {
      $message = 'Link your card. Earn bonuses.';
      $link = l('Link My Card', '#', array(
        'attributes' => array(
          'class' => 'link-card',
          'data-toggle' => 'modal',
          'data-target' => '#newCardModal',
        )
      ));
    }
  }
  
  if(!empty($message) || !empty($description)) {
    bp_gadgets_set_message($message, 'promo', $description, $link);
  }
  

  if ($user->uid == 1) {
    $user_first_name = 'Admin';
  }
  else {
    $user_wrapper = entity_metadata_wrapper('user', $user);
    $user_first_name = trim($user_wrapper->field_user_first_name->value());
  }

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

  //Check if the user just joined.
  if(isset($_GET['join']) && !empty($_GET['join'])) {
    $welcome_text = 'Welcome';
  }
  else {
    $welcome_text = 'Welcome Back';
  }

  $vars = array(
    'user_first_name' => $user_first_name,
    'url_image_md' => $image_url_md,
    'url_image_sm' => $image_url_sm,
    'url_image_xs' => $image_url_xs,
    'welcome_message' => $welcome_text,
  );

  $block = new stdClass();
  $block->title = NULL;
  $block->content = theme('block_bp_welcome_user', array('v' => $vars));

  return $block;
}
