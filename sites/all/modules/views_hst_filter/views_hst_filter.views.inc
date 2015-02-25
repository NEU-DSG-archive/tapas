<?php

/**
 * @file
 *
 * Provide views handler for views_hst_filter.module
 */
 
/**
 * Implements of hook_views_data().
 * Add filter for table taxonomy_index
 */
function views_hst_filter_views_data_alter(&$data) {
  $data['taxonomy_index']['views_hst_filter'] = array(
    'group' => t('Content'),
    'title' => t('Hierarchical Taxonomy Filter'),
    'help' => 'Hierarchical Taxonomy Filter',
    'filter' => array(
      'handler' => 'views_hst_filter_handler_filter',
      'hierarchy table' => 'taxonomy_term_hierarchy',
      'numeric' => TRUE,
      'skip base' => 'taxonomy_term_data',
      'allow empty' => TRUE,
    ),
  );
}