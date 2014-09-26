<?php

/**
 * @file
 * Views plugins definition for timelinemap.
 */

/**
 * Implementation of hook_views_style_plugins(). Adds view types to views UI interface.
 */
function timelinemap_views_plugins()
{
  return array(
    'style' => array(
      'timelinemap' => array(
        'title' => t('Timeline map'),
        'help' => t('Displays data as timelinemap.'),
        'handler' => 'timelinemap_plugin_style_timelinemap',
        'theme' => 'timelinemap_view_timelinemap',
        'theme file' => 'timelinemap.theme.inc',
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'help topic' => 'style-list',
        'uses row plugin' => TRUE,
        'even empty' => TRUE,
      ),
    ),
  );
}
