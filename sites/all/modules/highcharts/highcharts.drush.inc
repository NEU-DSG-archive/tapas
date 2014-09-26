<?php

/**
 * @file
 * Drush integration for Highcharts.
 */

/**
 * Highcharts JS library URI.
 */
define('HIGHCHARTS_URI', 'http://www.highcharts.com/downloads/zips/Highcharts-2.3.5.zip');

/**
 * Implements hook_drush_command().
 */
function highcharts_drush_command() {
  $items = array();

  $items['highcharts-download'] = array(
    'callback' => 'drush_highcharts_download',
    'description' => dt("Downloads the Highcharts library."),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'aliases' => array('highchartsdownload'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function highcharts_drush_help($section) {
  switch ($section) {
    case 'drush:highcharts-download':
      return dt("Downloads the Highcharts library. The default location is sites/all/libraries/highcharts.");
  }
}

/**
 * Drush callback to download the Highcharts library.
 *
 * @see highcharts_drush_command()
 */
function drush_highcharts_download() {
  if (!drush_shell_exec('type unzip')) {
    return drush_set_error(dt('Missing dependency: unzip. Install it before using this command.'));
  }

  // Libraries 1.x will return a path even if it doesn't exist while 2.x will
  // return FALSE. See highcharts_library().
  $path = module_exists('libraries') && libraries_get_path('highcharts') !== FALSE ? libraries_get_path('highcharts') : 'sites/all/libraries/highcharts';

  // Create highcharts library paths recursively if any do not exist.
  if (!is_dir($path)) {
    drush_op('mkdir', $path, 0777, TRUE);
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
  }

  // Set the directory to the download location.
  $olddir = getcwd();
  chdir($path);

  // Download the zip archive.
  $filename = basename(HIGHCHARTS_URI);
  if (!drush_shell_exec('wget ' . HIGHCHARTS_URI . ' -O ' . $filename)) {
    drush_shell_exec('curl -O ' . HIGHCHARTS_URI);
  }

  if (is_file($filename)) {
    // Decompress the zip archive.
    drush_shell_exec('unzip -qq -o ' . $filename);
    // Remove the zip archive.
    drush_op('unlink', $filename);
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);

  // Ensure highcharts library is installed.
  $library = drupal_get_library('highcharts', 'highcharts-library');
  if (file_exists(DRUPAL_ROOT . '/' . key($library['js']))) {
    drush_log(dt('@title has been downloaded to @path', array('@title' => $library['title'], '@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download @title to @path.', array('@title' => $library['title'], '@path' => $path)), 'error');
  }
}
