<?php

/**
 * @file
 * Defines View Style Plugins for Highcharts views.
 */

/**
 * Implements hook_views_plugins().
 */
function highcharts_views_views_plugins() {
  return array(
    'style' => array(
      'highcharts' => array(
        'title' => t('Highcharts'),
        'help' => t('Display the results as a highchart.'),
        'handler' => 'highcharts_views_plugin_style_highcharts',
        'uses options' => TRUE,
        'uses row plugin' => TRUE,
        'uses grouping' => FALSE,
        'type' => 'normal',
        'parent' => 'list',
      ),
    ),
  );
}
