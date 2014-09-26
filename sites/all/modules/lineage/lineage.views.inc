<?php

/**
 * @file
 * Interface between lineage.module and views.module.
 */

/**
 * Implementation of hook_views_data().
 */
function lineage_views_data() {
  $data['taxonomy_term_lineage'] = array(
    'table' => array(
      'group' => t('Taxonomy term'),
      'join' => array(
        'taxonomy_term_data' => array(
          'left_field' => 'tid',
          'field' => 'tid',
        ),
      ),
    ),
    'lineage' => array(
      'title' => t('Hierarchy'),
      'help' => t('The taxonomy lineage hierarchy.'),
      'field' => array(
        'handler' => 'lineage_handler_field',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
        'help' => t('Taxonomy term lineage hierarchy.'),
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
        'help' => t('Taxonomy term lineage hierarchy.'),
      ),
    ),
    'depth' => array(
      'title' => t('Depth'),
      'help' => t('The taxonomy lineage depth.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
        'help' => t('Taxonomy term lineage depth.'),
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
        'help' => t('Taxonomy term lineage depth.'),
      ),
    ),
  );

  return $data;
}

/**
 * Implements hook_views_plugins().
 */
function lineage_views_plugins() {
  return array(
    'style' => array( // Declare the nested list style plugin.
      'lineage_nested' => array(
        'title' => t('Lineage nested list'),
        'theme' => 'views_view_lineage_nested',
        'help' => t('Displays rows in a nested list, grouped by term lineage'),
        'handler' => 'views_plugin_style_lineage_nested',
        'uses row plugin' => TRUE,
        'uses fields' => TRUE,
        'uses grouping' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'parent' => 'list',
      ),
    )
  );
}
