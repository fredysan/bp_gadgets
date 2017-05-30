<?php

/**
 * Return array with the url and title for the Add/Remove favorites link
 *
 * @param int $station_nid
 * @return array
 */
function _bp_gadgets_get_favorites_link($station_nid) {
  $url = '';
  $title = '';
  $link = array();

  module_load_include('inc', 'bp_station_finder', 'bp_station_finder.api');

  if (!empty($station_nid)) {
    if (_bp_station_finder_is_favorite($station_nid)) {
      $url = 'user/favorites/ajax/' . $station_nid . '/delete';
      $title = t('Remove From Favorites');
    }
    else {
      $url .= 'user/favorites/ajax/' . $station_nid . '/add';
      $title = t('Add to Favorites');
    }

    $link = array(
      '#type' => 'link',
      '#title' => $title,
      '#href' => $url,
      '#options' => array(
        'attributes' => array(
          'class' => array('custom-urls', 'favorites', 'favorites-' . $station_nid),
        ),
      ),
    );
  }

  return $link;
}

/**
 * Update local user phonenumber.
 */
function _bp_gadgets_set_user_mobile_phone($data) {
  global $user;

  if (!empty($user->uid)){
    $user_wrapper = entity_metadata_wrapper('user', $user);
    $mobileNumber = check_plain($data['mobileNumber']);
    $user_wrapper->field_user_mobile_number->set($mobileNumber);
    $user_wrapper->field_user_loyalty_id->set(trim(str_replace('-', '', $mobileNumber)));
    $user_wrapper->save();
  }
}

/**
 * Update user phonenumber.
 */
function _bp_update_set_fis_user_mobile_phone($data) {

  global $user;

  $account = user_load($user->uid);
  $user_wrapper = entity_metadata_wrapper('user', $user);
  $mobile_phone = $data['mobileNumber'];

  $parameters = array(
      'fisUniqueId' => $user_wrapper->field_user_fis_unique_id->value(),
      'loyaltyId' => trim(str_replace('-', '', $mobile_phone)),
      'firstName' => $user_wrapper->field_user_first_name->value(),
      'lastName' => $user_wrapper->field_user_last_name->value(),
      'address' => $user_wrapper->field_user_address_line_1->value(),
      'city' => $user_wrapper->field_user_city->value(),
      'state' => $user_wrapper->field_user_state->value(),
      'zipCode' => $user_wrapper->field_user_zip_code->value(),
      'email' => $user_wrapper->mail->value(),
      'mobilePhoneNumber' => bp_join_localize_us_number(trim($mobile_phone)),
      'receiveOffers' => $user_wrapper->field_user_receive_offers->value(),
    );

  module_load_include('api.inc', 'bp_fis');
  $response = _bp_fis_api_update_user($parameters);

  return $response;
}

/**
 * Returns the list of possible button classes
 *
 * @return array
 */
function _bp_gadgets_ctype_get_button_classes() {
  $classes = array(
    'gray_to_green' => 'Gray to Green',
    'yellow_to_green' => 'Yellow to Green',
    'transparent_white_border_to_green' => 'Transparent(White Border) to Green',
    'transparent_to_green' => 'Transparent to Green',
    'transparent_to_white' => 'Transparent to White',
    'white_to_green' => 'White to Green',
    'white_to_transparent' => 'White to transparent',
  );

  return $classes;
}

/**
 * Fivestar form
 *
 * @param type $node
 * @return array
 */
function _bp_gadgets_fivestar_form($node) {
  global $user;

  $settings = array(
    'content_type' => 'node',
    'content_id' => $node->nid,
    'entity' => $node,
    'stars' => 5,
    'field_name' => 'field_station_fivestar',
    'autosubmit' => FALSE,
    'allow_clear' => FALSE,
    'langcode' => $node->language,
    'text' => 'none', // options are none, average, smart, user, and dual
    'tag' => 'vote',
    'style' => 'user', // options are average, dual, smart, user
    'widget' => array('name' => 'bpstars', 'css' => drupal_get_path('module', 'bp_gadgets') . '/widgets/bpstars/bpstars.css')
  );

  $fivestar_values = fivestar_get_votes('node', $node->nid, 'vote', $user->uid);

  $form = drupal_get_form('fivestar_custom_widget', $fivestar_values, $settings);

  return $form;
}

/**
* Check if the value of station is saved on the database
* @param string $station_address
* @return boolean
*/
function bp_gadgets_station_exists($station_address){
  $hasStation = db_select('field_data_field_station_name', 'sn')
        ->fields('sn')
        ->condition('field_station_name_value', $station_address,'=')
        ->execute()
        ->fetchAssoc();

    if(!$hasStation) {
      return FALSE;
    }

    return TRUE;
}


