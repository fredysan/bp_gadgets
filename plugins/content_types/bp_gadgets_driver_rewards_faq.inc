<?php

$plugin = array(
  'title' => t('BP Driver Rewards: FAQ'),
  'description' => t('BP Driver Rewards: FAQ'),
  'single' => TRUE,
  'render callback' => 'bp_gadgets_ctype_driver_rewards_faq_render',
  'edit form' => 'bp_gadgets_ctype_driver_rewards_faq_edit_form',
  'defaults' => array(
    'title' => '',
    'text' => '',
    'bg_color' => '666666',
  ),
  'category' => array(t('BP')),
);

/**
 * Content type settings form.
 */
function bp_gadgets_ctype_driver_rewards_faq_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textarea',
    '#rows' => 3,
    '#default_value' => $conf['title'],
    '#weight' => 1,
  );
  $form['text'] = array(
    '#title' => t('Text'),
    '#type' => 'textarea',
    '#rows' => 3,
    '#default_value' => $conf['text'],
    '#weight' => 2,
  );
  $form['bg_color'] = array(
    '#title' => t('Background color (######)'),
    '#type' => 'textfield',
    '#size' => '6',
    '#maxlength' => '6',
    '#default_value' => $conf['bg_color'],
    '#weight' => 3,
  );

  $form['btns'] = array(
    '#title' => t('Buttons'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
    '#weight' => 5,
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
  }

  return $form;
}

/**
 * Content type submit handler.
 */
function bp_gadgets_ctype_driver_rewards_faq_edit_form_submit($form, &$form_state) {
  $keys = array(
    'title',
    'text',
    'bg_color',
    'btns',
  );
  foreach ($keys as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

function bp_gadgets_ctype_driver_rewards_faq_render($subtype, $conf, $args, $context) {
  $buttons = _bp_gadgets_ctype_driver_rewards_faq_get_buttons($conf);
  $vars = array(
    'title' => $conf['title'],
    'text' => $conf['text'],
    'bg_color' => $conf['bg_color'],
    'buttons' => $buttons,
  );
  $block = new stdClass();
  $block->title = t('Driver Rewards FAQ');
  $block->content = theme('block_bp_driver_rewards_faq', array('v' => $vars));

  return $block;
}

/**
 * Return buttons with the right format
 *
 * @param type $conf
 * @return array
 */
function _bp_gadgets_ctype_driver_rewards_faq_get_buttons($conf) {
  $buttons = array();

  if (isset($conf['btns'])) {
    foreach ($conf['btns'] as $btn) {
      if (!empty($btn['title']) && !empty($btn['url'])) {
        $url = drupal_parse_url($btn['url']);
        $button['url'] = url($url['path'], array('query' => $url['query']));
        $button['title'] = $btn['title'];

        if(user_is_logged_in()){
          if($button['title'] != "Join Now"){
            $buttons[] = $button;
          }
        } else {
          $buttons[] = $button;
        }
      }
    }
  }

  return $buttons;
}
