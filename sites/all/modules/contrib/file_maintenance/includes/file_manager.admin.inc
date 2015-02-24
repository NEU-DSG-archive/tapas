<?php

/**
 *
 * @file
 * Functions for drupal admin.
 */

// $Id$

include_once('file_manager.core.inc');

define('URI_QUERY_PARAMETER', 'path');

/**
 * Default values
 */
define('FILE_MANAGER_FILES_DISPLAYED', 'all');
define('FILE_MANAGER_SHOW_TEMP_DIRS', TRUE);
define('FILE_MANAGER_TEMP_DIRS', "public://styles/\npublic://languages/");
define('FILE_MANAGER_RECURSIVE', TRUE);
// TODO
define('FILE_MANAGER_FILE_LIST_COLUMN_', TRUE);

/**
 *
 *
 * @param $form
 * @param $form_state
 */
function file_manager_settings_form($form, &$form_state) {
  $form['file_manager_temp_dirs'] = array(
    '#type' => 'textarea',
    '#title' => t('Directories in the file system that are used for caching (e.g. for image styles).'),
    '#default_value' => variable_get('file_manager_temp_dirs', FILE_MANAGER_TEMP_DIRS),
    '#size' => 60,
    '#description' => t(
      'Enter the paths that contain temporary files.'
      . ' <strong>One path per line</strong>'),
  );
  return system_settings_form($form);
}

/**
 * Modified version of core's (defective) file_usage_list() (file.inc)
 *
 * @param stdClass $file
 */
function file_manager_file_usage_list(stdClass $file) {
  $result = db_select('file_usage', 'f')
    ->fields('f', array('module', 'type', 'id', 'count'))
    ->condition('fid', $file->fid)
    ->condition('count', 0, '>')
    ->execute();
  $references = array();
  foreach ($result as $usage) {
    $references[$usage->module][$usage->type][$usage->id] = $usage->count;
  }
  return $references;
}
