<?php

function bp_gadgets_faq() {
  $vid = 2; // "faq_type" vocabulary
  $terms = taxonomy_get_tree($vid, 0, 1);
  $faq_tabs = array();

  foreach ($terms as $key => $term) {
    $tab = array();
    $tab['title'] = $term->name;
    $tab['id'] = str_replace(' ', '', strtolower($term->name));
    $tab['classes'] = '';
    $tab['content'] = _bp_gadgets_faq_get_tab_content($term->tid);
    $faq_tabs[] = $tab;
  }
  $faq_tabs[0]['classes'] = 'active';

  return theme('page_faq', array(
    'tabs' => $faq_tabs,
  ));
}

function _bp_gadgets_faq_get_tab_content($tab_tid) {
  $content = '';
  // Get the full list of faqs filtered by top level term ids
  $rows = views_get_view_result('faq', 'block_1', array($tab_tid));

  // group FAQs by term id
  $groups = array();
  foreach ($rows as $row) {
    $group_id = $row->field_field_faq_type[0]['raw']['tid'];
    if (!array_key_exists($group_id, $groups)) {
      $groups[$group_id] = array();
    }
    $groups[$group_id][] = $row;
  }

  // Move the tab_tid group to the begining
  $groups = array($tab_tid => $groups[$tab_tid]) + $groups;

  // Create list of Jump links ()
  // Those correspond to the second taxonomy level of the FAQ type vocabulary
  $jump_items = array();
  $jump_links = array();
  foreach ($groups as $tid => $group) {
    $term = taxonomy_term_load($tid);
    // 
    if ($tab_tid != $term->tid) {
      $jump_items[$tid] = array(
        'id' => 'jump-link-' . $tid,
        'title' => $term->name,
      );

      $jump_links[] = l($term->name, '', array(
        'fragment' => 'faq-section-' . $term->tid,
        'external' => TRUE,
        'attributes' => array('data-jumper' => NULL),
      ));
    }
  }

  if (!empty($jump_links)) {
    $params = array(
      'items' => $jump_links,
      'attributes' => array(
        'class' => 'jump_links',
      )
    );
    $jump_list = theme('item_list', $params);
    $content .= $jump_list;
  }

  foreach ($groups as $tid => $group) {
    $content .= '<div class="collapse-rows-group">';

    if ($tab_tid != $tid) {
      $content .= '<h4 id="faq-section-' . $tid . '">' . $jump_items[$tid]['title'] . '</h4>';
    }

    foreach ($group as $key => $faq) {
      $vars = array(
        'id' => 'faq-' . $faq->nid,
        'question' => $faq->field_field_question[0]['rendered']['#markup'],
        'answer' => $faq->field_field_answer[0]['rendered']['#markup'],
      );
      $content .= theme('page_faq_item', $vars);
    }
    $content .= '</div>';
  }

  return $content;
}
