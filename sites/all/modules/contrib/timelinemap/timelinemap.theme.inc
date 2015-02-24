<?php

/**
 * @file
 * Theme functions for timelinemap.
 */

/**
 * Implementation of template_process for views-view-timelinemap.
 */
function template_preprocess_timelinemap_view_timelinemap(&$vars)
{
  $view = $vars['view'];

  if (_timelinemap_include()) {
    // preprocess css information for the template
    $vars['class'] = $vars['view']->name;
    $vars['div_id'] = $view->name . '-' . $view->current_display;
  }
}

/**
 * Inlude all js and css files that needs the timelinemap.
 *
 */
function _timelinemap_include()
{
  static $timelinemap_initialized = FALSE;
  if ($timelinemap_initialized) {
    return TRUE;
  }
  drupal_add_js(array('timelinemap_path' => drupal_get_path('module', 'timelinemap')), 'setting');

  $timelinemap_mod_path = drupal_get_path('module', 'timelinemap');

  $gmap_apikey = variable_get('googlemap_api_key', '');
  $timelinemap_apikey = $gmap_apikey ? $gmap_apikey : variable_get('timelinemap_api_key', '');

  drupal_add_js('http://maps.google.com/maps?file=api&v=2&key=' . $timelinemap_apikey, 'external');
  drupal_add_js('http://static.simile.mit.edu/timeline/api/timeline-api.js', 'external');


  drupal_add_css($timelinemap_mod_path . '/css/timelinemap.css');

  drupal_add_js($timelinemap_mod_path . '/js/timemap.js');
  drupal_add_js($timelinemap_mod_path . '/js/timelinemap.js');

  $timelinemap_initialized = TRUE;
  return TRUE;
}