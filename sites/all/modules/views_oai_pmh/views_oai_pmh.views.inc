<?php
/**
 * @file
 * Views include file with views hooks.
 */

include_once('plugins/includes/views_oai_pmh_xml_node.inc'); // XML generation class

/**
 * Include the metadata type definitions.
 */
include_once('plugins/includes/views_oai_pmh_metadata_type.inc');
include_once('plugins/includes/views_oai_pmh_metadata_type_definitions.inc');

/**
 * Include the row plugin classes.
 */
include_once('plugins/views_oai_pmh_plugin_row_auto.inc'); // Row Auto class
include_once('plugins/views_oai_pmh_plugin_row_misc.inc'); // Row Misc class

/**
 * Implementation of hook_views_plugins().
 */
function views_oai_pmh_views_plugins() {
  $path = drupal_get_path('module', 'views_oai_pmh');
  $vpath = drupal_get_path('module', 'views');

  $style_defaults = array(
    'path' => $path . '/plugins',
    'parent' => 'views_oai_pmh',
    'theme' => 'views_oai_pmh_response',
    'theme path' => $path . '/theme',
    'theme file' => 'views_oai_pmh.theme.inc',
    'uses row plugin' => TRUE,
    'uses fields' => TRUE,
    'uses options' => TRUE,
    'type' => 'oai-pmh',
  );

  // Define an array of theme functions used by the 'views_oai_pmh_auto' style plugin.
  $auto_themes = array();
  if (is_array($GLOBALS['views_oai_pmh'])) {
    foreach ($GLOBALS['views_oai_pmh'] as $data_type) {
      $auto_themes[$data_type->record_theme] = 'style';
    }
  }
  
  // Define a set of row plugins for Views, one for each of the OAI data types we have defined in our global array.
  $row_plugins = array();
  if (is_array($GLOBALS['views_oai_pmh'])) {
    foreach ($GLOBALS['views_oai_pmh'] as $metadata_format => $data_type) {
      $row_plugins[$data_type->row_plugin] = array(
        'title' => $data_type->name,
        'help' => $data_type->help,
        'path' => $path . '/plugins',
        'handler' => $data_type->handler,
        'parent' => 'views_plugin_row',
        'theme' => $data_type->field_theme,
        'theme path' => $path . '/theme',
        'theme file' => 'views_oai_pmh.theme.inc',
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'oai-pmh',
        'help topic' => 'style-row-fields',
        '#metadata_format' => $metadata_format,
      );
    }
  }
  
  return array(
    'display' => array(
      'views_oai_pmh' => array(
        'title' => t('OAI-PMH'),
        'help' => t('Export the view results to an OAI feed'),
        'path' => $path . '/plugins',
        'handler' => 'views_oai_pmh_plugin_display',
        'parent' => 'page',
        'uses hook menu' => TRUE,
        'use ajax' => FALSE,
        'use pager' => TRUE,
        'accept attachments' => FALSE,
        'admin' => t('OAI-PMH'),
        'help topic' => 'display-oai',
      ),
    ),
    'style' => array(
      'views_oai_pmh' => array(
        // this isn't really a display but is necessary so the file can
        // be included.
        'no ui' => TRUE,
        'handler' => 'views_oai_pmh_plugin_style',
        'path' => $path . '/plugins',
        'theme path' => $path . '/theme',
        'theme file' => 'views_oai_pmh.theme.inc',
        'additional themes' => array(
          'views_oai_pmh_identify_body' => 'style',
      		'views_oai_pmh_metadataformats_body' => 'style',
          'views_oai_pmh_record_header' => 'style',
        ),
        'additional themes base' => 'views_oai_pmh',
       ),
      'views_oai_pmh_auto' => array(
        'title' => t('OAI-PMH (auto)'),
        'help' => t('Export the display as an AUTO OAI-PMH feed'),
        'handler' => 'views_oai_pmh_plugin_style_auto',
        'oai headers' => array('Content-Type' => 'text/xml'),
        'additional themes' => array_merge($auto_themes, array(
          'views_oai_pmh_auto_record' => 'style',
          'views_oai_pmh_plugin_row_auto' => 'style',
          'views_oai_pmh_plugin_row_auto_fields' => 'style',
        )),
        'additional themes base' => 'views_oai_pmh',
      ) + $style_defaults,
    ),
    'row' => array_merge(array(
      'parent' => array(
        // this isn't really a display but is necessary so the file can
        // be included.
        'no ui' => TRUE,
        'path' => $vpath . '/plugins',
        'handler' => 'views_plugin_row',
        'parent' => '',
      ),
      'auto' => array(
        'title' => t('Auto Fields'),
        'help' => t('Detects the requested field type and outputs them appropriately.'),
        'path' => $path . '/plugins',
        'handler' => 'views_oai_pmh_plugin_row_auto',
        'theme' => 'views_oai_pmh_row_auto_fields',
        'parent' => 'views_plugin_row',
        'theme path' => $path . '/theme',
        'theme file' => 'views_oai_pmh.theme.inc',
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'oai-pmh',
        'help topic' => 'style-row-fields',
      ),
    ), $row_plugins),
  );
}
