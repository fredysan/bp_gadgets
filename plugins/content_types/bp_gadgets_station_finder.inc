<?php

$plugin = array(
  'title' => t('BP: Station Finder'),
  'description' => t('BP Station Finder'),
  'single' => TRUE,
  'render callback' => 'bp_gadgets_ctype_station_finder_render',
  'edit form' => 'bp_gadgets_station_finder_edit_form',
  'category' => array(t('BP')),
  'defaults' => array(
    'title' => t('Find a BP Station'),
  ),
);

/**
 * Content type settings form.
 */
function bp_gadgets_station_finder_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $conf['title'],
  );
  
  $form['text'] = array(
    '#type' => 'textarea',
    '#title' => t('Subtitle'),
    '#default_value' => $conf['text'],
  );

  return $form;
}

/**
 * Content type submit handler.
 */
function bp_gadgets_station_finder_edit_form_submit($form, &$form_state) {
  $keys = array(
    'title',
    'text',
  );
  foreach ($keys as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Function callback for plugin: BP: Station Finder
 *
 * @param $subtype
 * @param $conf
 * @param $args
 * @param $context
 *
 * @return \stdClass
 */
function bp_gadgets_ctype_station_finder_render($subtype, $conf, $args, $context) {
  $title = $conf['title'];
  $subtitle = $conf['text'];
  $form = drupal_get_form('bp_gadgets_ctype_station_finder_form');

  $block = new stdClass();
  $block->title = '';
  $block->content = theme('block_bp_station_finder', array(
    'title' => $title,
    'subtitle' => $subtitle,
    'form' => drupal_render($form),
  ));

  drupal_add_js('//maps.googleapis.com/maps/api/js?key=' . variable_get('gsl_google_api_key', NULL) . '&libraries=places,geometry');

  return $block;
}

/**
 * @param $form
 * @param $form_state
 *
 * @return mixed
 */
function bp_gadgets_ctype_station_finder_form($form, &$form_state) {
  $form['search'] = array(
    '#type' => 'textfield',
    '#size' => NULL,
    '#attributes' => array(
      'placeholder' => t('Enter City, State or ZIP Code'),
      'id' => 'edit-search',
      'v-on:blur'=> 'onSubmitSearchComponent'
    ),
    '#prefix' => '<div class="input-box"><label class="screen-reader-text" for="edit-search">'.t('Enter City, State or ZIP Code').'</label>',
    '#suffix' => '</div>',
    '#field_suffix' => '<span class="icon icon-search" tabindex="0" v-on:click="submitSearchForm" v-on:keyup.enter="submitSearchForm"></span>',
  );

  $form['actions'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
    '#prefix' => '<div class="button">',
    '#suffix' => '</div>',
    '#attributes' => array(
      'class' => array('opacity yellow_to_green'),
    ),
  );

  $form['#method'] = 'get';
  $form['#action'] = 'station-finder';

  return $form;
}
