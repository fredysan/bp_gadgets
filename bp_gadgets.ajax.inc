<?php

function bp_gadgets_ajax_get_favorites_link($nid) {
  module_load_include('inc', 'bp_gadgets', 'plugins/content_types/includes/bp_gadgets_common');
  
  $link = render(_bp_gadgets_get_favorites_link($nid));
  
  $response = new stdClass();
  $response->status = TRUE;
  $response->html = $link;
  
  drupal_json_output($response);
}