<?php
/**
 * @file
 * Theme functions for timeline.
 */

/**
 * Implementation of template_process for views-view-timeline.
 */
function template_preprocess_views_view_timeline(&$vars) {

  if (!empty($vars['view']->live_preview)) {
    drupal_set_message(t('The timeline cannot be viewed in the preview.'), 'warning');
    return;
  }

  if (isset($vars['options']['debug'])) {
    $vars['debug'] = theme('timeline_debug', array('debug_array' => $vars['options']['debug']));
  }
    
  // check if data is available
  if ($vars['options']['timeline']['data_count'] > 0) {
    // create actual timeline
    if (_timeline_include($vars['options']['timeline']['theme'])) {

      // preprocess css information for the template
      $vars['id'] = $vars['options']['timeline']['class'];
      $vars['class'] = $vars['options']['timeline']['class'];
      $vars['height'] = $vars['options']['timeline']['height'];
      $vars['width'] = $vars['options']['timeline']['width'];
      $vars['controls'] = $vars['options']['misc']['controls'];
      $vars['timeline_theme'] = $vars['options']['timeline']['theme'];
    }
  }
  else {
    theme('timeline_nodata', array());
  }
}


/**
 * Overridable theme function.
 * This functions prints a warning when no data is available.
 */
function theme_timeline_nodata($vars) {
  drupal_set_message(t('There is no data available to show on the timeline'), 'warning');
}

/**
 * Overridable theme function.
 * This returns the title of each event bubble on the timeline.
 */
function theme_timeline_bubble_title($vars) {
//ADDED to replace the squished title with the display title since it wasn't getting the altered title from views
//	echo "<pre>";
//	print_r($vars);
//	echo "</pre>";
//	exit();
//return value is below for reference
/**Array
(
    [row] => stdClass Object
        (
            [nid] => 893
            [node_title] => matildagreathousealexander
            [field_data_field_title_node_entity_type] => node
            [field_data_field_dc_coverage_temporal_node_entity_type] => node
            [field_data_field_title_linked_node_entity_type] => node
            [views_php_5] => 
            [_field_data] => Array
                (
                    [nid] => Array
                        (
                            [entity_type] => node
                            [entity] => stdClass Object
                                (
                                    [vid] => 18866
                                    [uid] => 162
                                    [title] => matildagreathousealexander
                                    [log] => 
                                    [status] => 1
                                    [comment] => 1
                                    [promote] => 0
                                    [sticky] => 0
                                    [ds_switch] => 
                                    [nid] => 893
                                    [type] => tei_content
                                    [language] => und
                                    [created] => 1350673766
                                    [changed] => 1373309493
                                    [tnid] => 0
                                    [translate] => 0
                                    [revision_timestamp] => 1373309493
                                    [revision_uid] => 190
                                    [field_tei_xml] => Array
                                        (
                                            [und] => Array
                                                (
                                                    [0] => Array
                                                        (
                                                            [fid] => 1560
                                                            [display] => 1
                                                            [description] => 
                                                            [uid] => 162
                                                            [filename] => alexander.xml
                                                            [uri] => public://projects/victorianwomenwritersproject/alexander.xml
                                                            [filemime] => application/xml
                                                            [filesize] => 18667
                                                            [status] => 1
                                                            [timestamp] => 1350673766
                                                            [origname] => alexander.xml
                                                            [rdf_mapping] => Array
                                                                (
                                                                )

                                                        )

                                                )

                                        )

                                    [field_tei_xsl] => Array
                                        (
                                        )

                                    [field_additional_files] => Array
                                        (
                                        )

                                    [og_group_ref] => Array
                                        (
                                            [und] => Array
                                                (
                                                    [0] => Array
                                                        (
                                                            [target_id] => 876
                                                        )

                                                )

                                        )

                                    [group_content_access] => Array
                                        (
                                            [und] => Array
                                                (
                                                    [0] => Array
                                                        (
                                                            [value] => 0
                                                        )

                                                )

                                        )

                                    [field_dc_creator] => Array
                                        (
                                            [und] => Array
                                                (
                                                    [0] => Array
                                                        (
                                                            [value] => Luke, Stephanie
                                                            [format] => 
                                                            [safe_value] => Luke, Stephanie
                                                        )

                                                    [1] => Array
                                                        (
                                                            [value] => Flanders, Julia
                                                            [format] => 
                                                            [safe_value] => Flanders, Julia
                                                        )

                                                    [2] => Array
                                                        (
                                                            [value] => Scott, Sir Walter
                                                            [format] => 
                                                            [safe_value] => Scott, Sir Walter
                                                        )

                                                    [3] => Array
                                                        (
                                                            [value] => Hemingway, Ernest
                                                            [format] => 
                                                            [safe_value] => Hemingway, Ernest
                                                        )

                                                )

                                        )

                                    [field_dc_date] => Array
                                        (
                                            [und] => Array
                                                (
                                                    [0] => Array
                                                        (
                                                            [value] => 2012-10-19 00:00:00
                                                            [timezone] => America/New_York
                                                            [timezone_db] => America/New_York
                                                            [date_type] => datetime
                                                        )

                                                )

                                        )

                                    [field_dc_description] => Array
                                        (
                                            [und] => Array
                                                (
                                                    [0] => Array
                                                        (
                                                            [value] => This is a born-digital critical introduction, which has been encoded in TEI P5 following the recommendations for Level 3 encoding of the Best Practices for TEI in Libraries.
                                                            [format] => 
                                                            [safe_value] => This is a born-digital critical introduction, which has been encoded in TEI P5 following the recommendations for Level 3 encoding of the Best Practices for TEI in Libraries.
                                                        )

                                                )

                                        )

                                    [field_dc_format] => Array
                                        (
                                        )

                                    [field_dc_language] => Array
                                        (
                                            [und] => Array
                                                (
                                                    [0] => Array
                                                        (
                                                            [value] => eng
                                                        )

                                                )

                                        )

                                    [field_dc_publisher] => Array
                                        (
                                        )

                                    [field_dc_rights] => Array
                                        (
                                            [und] => Array
                                                (
                                                    [0] => Array
                                                        (
                                                            [value] => Copyright 2011, Author of this document. Indiana University provides the information contained on this web site for non-commercial, personal, or research use only. All other use, including but not limited to commercial or scholarly reproductions, redistribution, publication or transmission, whether by electronic means or otherwise, without prior written permission of the copyright holder is strictly prohibited. For inquiries about usage, please contact the Digital Library Program.
                                                            [format] => 
                                                            [safe_value] => Copyright 2011, Author of this document. Indiana University provides the information contained on this web site for non-commercial, personal, or research use only. All other use, including but not limited to commercial or scholarly reproductions, redistribution, publication or transmission, whether by electronic means or otherwise, without prior written permission of the copyright holder is strictly prohibited. For inquiries about usage, please contact the Digital Library Program.
                                                        )

                                                )

                                        )

                                    [field_dc_type] => Array
                                        (
                                            [und] => Array
                                                (
                                                    [0] => Array
                                                        (
                                                            [value] => Text
                                                        )

                                                )

                                        )

                                    [field_dc_title] => Array
                                        (
                                            [und] => Array
                                                (
                                                    [0] => Array
                                                        (
                                                            [value] => Matilda Greathouse Alexander
                                                            [format] => 
                                                            [safe_value] => Matilda Greathouse Alexander
                                                        )

                                                )

                                        )

                                    [field_collection_term] => Array
                                        (
                                            [und] => Array
                                                (
                                                    [0] => Array
                                                        (
                                                            [target_id] => 476
                                                        )

                                                )

                                        )

                                    [field_dc_subject_lcsh] => Array
                                        (
                                            [und] => Array
                                                (
                                                    [0] => Array
                                                        (
                                                            [tid] => 224
                                                        )

                                                    [1] => Array
                                                        (
                                                            [tid] => 207
                                                        )

                                                )

                                        )

                                    [field_dc_subject] => Array
                                        (
                                        )

                                    [field_dc_coverage_temporal] => Array
                                        (
                                            [und] => Array
                                                (
                                                    [0] => Array
                                                        (
                                                            [value] => 1842-01-01T00:00:00
                                                            [value2] => 1904-01-01T00:00:00
                                                            [timezone] => America/New_York
                                                            [timezone_db] => America/New_York
                                                            [date_type] => date
                                                            [db] => Array
                                                                (
                                                                    [value] => DateObject Object
                                                                        (
                                                                            [granularity] => Array
                                                                                (
                                                                                    [0] => year
                                                                                    [1] => month
                                                                                    [2] => day
                                                                                    [3] => hour
                                                                                    [4] => minute
                                                                                    [5] => second
                                                                                    [6] => timezone
                                                                                )

                                                                            [errors] => Array
                                                                                (
                                                                                )

                                                                            [serializedTime:DateObject:private] => 1842-01-01T00:00:00-05:00
                                                                            [serializedTimezone:DateObject:private] => America/New_York
                                                                            [timeOnly] => 
                                                                            [dateOnly] => 
                                                                            [originalTime] => 1842-01-01T00:00:00-05:00
                                                                            [date] => 1842-01-01 00:00:00
                                                                            [timezone_type] => 3
                                                                            [timezone] => America/New_York
                                                                        )

                                                                    [value2] => DateObject Object
                                                                        (
                                                                            [granularity] => Array
                                                                                (
                                                                                    [0] => year
                                                                                    [1] => month
                                                                                    [2] => day
                                                                                    [3] => hour
                                                                                    [4] => minute
                                                                                    [5] => second
                                                                                    [6] => timezone
                                                                                )

                                                                            [errors] => Array
                                                                                (
                                                                                )

                                                                            [serializedTime:DateObject:private] => 1904-01-01T00:00:00-05:00
                                                                            [serializedTimezone:DateObject:private] => America/New_York
                                                                            [timeOnly] => 
                                                                            [dateOnly] => 
                                                                            [originalTime] => 1904-01-01T00:00:00-05:00
                                                                            [date] => 1904-01-01 00:00:00
                                                                            [timezone_type] => 3
                                                                            [timezone] => America/New_York
                                                                        )

                                                                )

                                                        )

                                                )

                                        )

                                    [field_dc_coverage_spatial] => Array
                                        (
                                        )

                                    [field_dc_identifier] => Array
                                        (
                                            [und] => Array
                                                (
                                                    [0] => Array
                                                        (
                                                            [value] => https://www.ptapascit.services.brown.edu/node/893
                                                            [format] => 
                                                            [safe_value] => https://www.ptapascit.services.brown.edu/node/893
                                                        )

                                                )

                                        )

                                    [field_dc_source] => Array
                                        (
                                        )

                                    [field_dc_alternative] => Array
                                        (
                                        )

                                    [field_author_last_name_dc_creat] => Array
                                        (
                                            [und] => Array
                                                (
                                                    [0] => Array
                                                        (
                                                            [tid] => 477
                                                        )

                                                    [1] => Array
                                                        (
                                                            [tid] => 530
                                                        )

                                                    [2] => Array
                                                        (
                                                            [tid] => 456
                                                        )

                                                    [3] => Array
                                                        (
                                                            [tid] => 459
                                                        )

                                                )

                                        )

                                    [field_author_first_name_dc_creat] => Array
                                        (
                                            [und] => Array
                                                (
                                                    [0] => Array
                                                        (
                                                            [tid] => 478
                                                        )

                                                    [1] => Array
                                                        (
                                                            [tid] => 531
                                                        )

                                                    [2] => Array
                                                        (
                                                            [tid] => 461
                                                        )

                                                    [3] => Array
                                                        (
                                                            [tid] => 464
                                                        )

                                                )

                                        )

                                    [field_dc_contributor] => Array
                                        (
                                        )

                                    [field_has_corporate_author] => Array
                                        (
                                            [und] => Array
                                                (
                                                    [0] => Array
                                                        (
                                                            [value] => 0
                                                        )

                                                )

                                        )

                                    [field_author_corporate_dc_creato] => Array
                                        (
                                        )

                                    [field_transform_address_link] => Array
                                        (
                                            [und] => Array
                                                (
                                                    [0] => Array
                                                        (
                                                            [url] => sites/default/files/projects/victorianwomenwritersproject/alexander.xml-teibp.xsl.xml
                                                            [title] => alexander.xml-teibp.xsl.xml
                                                            [attributes] => Array
                                                                (
                                                                )

                                                        )

                                                )

                                        )

                                    [field_dc_coveragetemporal_partia] => Array
                                        (
                                        )

                                    [field_tei_xml_revisions_link] => Array
                                        (
                                        )

                                    [field_tei_xsl_revisions_link] => Array
                                        (
                                        )

                                    [field_tei_transform_address_link] => Array
                                        (
                                            [und] => Array
                                                (
                                                    [0] => Array
                                                        (
                                                            [url] => sites/default/files/projects/victorianwomenwritersproject/alexander.xml-teiHeader2dc1.xsl.xml
                                                            [title] => alexander.xml-teiHeader2dc1.xsl.xml
                                                            [attributes] => Array
                                                                (
                                                                )

                                                        )

                                                )

                                        )

                                    [field_tei_custom_transform_addre] => Array
                                        (
                                            [und] => Array
                                                (
                                                    [0] => Array
                                                        (
                                                            [url] => sites/default/files/projects/victorianwomenwritersproject/alexander.xml-genericTEI2XHTML4noJS.xsl.xml
                                                            [title] => alexander.xml-genericTEI2XHTML4noJS.xsl.xml
                                                            [attributes] => Array
                                                                (
                                                                )

                                                        )

                                                )

                                        )

                                    [field_tei_xml_link] => Array
                                        (
                                            [und] => Array
                                                (
                                                    [0] => Array
                                                        (
                                                            [url] => ./sites/default/files/projects/victorianwomenwritersproject/alexander.xml
                                                            [title] => alexander.xml
                                                            [attributes] => Array
                                                                (
                                                                )

                                                        )

                                                )

                                        )

                                    [field_teibp_transform_file] => Array
                                        (
                                        )

                                    [field_teidc_transform_file] => Array
                                        (
                                        )

                                    [field_teicustom_transform_file_1] => Array
                                        (
                                        )

                                    [field_dc_coverage_spatial_taxono] => Array
                                        (
                                        )

                                    [field_location] => Array
                                        (
                                        )

                                    [field_geodata] => Array
                                        (
                                        )

                                    [field_site_collection_vocab_term] => Array
                                        (
                                            [und] => Array
                                                (
                                                    [0] => Array
                                                        (
                                                            [tid] => 713
                                                        )

                                                )

                                        )

                                    [field_projects_vocab] => Array
                                        (
                                            [und] => Array
                                                (
                                                    [0] => Array
                                                        (
                                                            [tid] => 651
                                                        )

                                                )

                                        )

                                    [field_email_yet] => Array
                                        (
                                            [und] => Array
                                                (
                                                    [0] => Array
                                                        (
                                                            [value] => 0
                                                        )

                                                )

                                        )

                                    [field_title] => Array
                                        (
                                            [und] => Array
                                                (
                                                    [0] => Array
                                                        (
                                                            [value] => Matilda Greathouse Alexander
                                                            [format] => 
                                                            [safe_value] => Matilda Greathouse Alexander
*/

  //$row = $vars['row'];
  //$alias = $vars['alias'];
  //return $row->$alias;
  $row = $vars['row']->_field_data['nid']['entity']->field_title['und'][0]['safe_value'];
  return $row;
//END

}


/**
 * Overridable theme function.
 * This creates the url that the title of each event bubble is linked to.
 */
function theme_timeline_bubble_url($vars) {
  $row = $vars['row'];
  $link_enabled = $vars['link_enabled'];

  if ($link_enabled) {
    return base_path() . drupal_get_path_alias('node/' . $row->nid);
  }
  return FALSE;
}

/*
 * Function to theme a textarea with a array inside
 */
function theme_timeline_debug($vars) {
  $debug_array = $vars['debug_array'];
  $output = '<div id="timeline_debug">';
  $output .= '<label><strong>DEBUG:</strong></label>';
  $output .= '<div class="resizable-textarea"><textarea id="edit-code" class="form-textarea resizable textarea-processed" name="code" rows ="10" cols="60">';
  $output .= var_export($debug_array, TRUE);
  $output .= '</textarea></div></div>';
  return $output;
}

/**
 * Inlude all js and css files that needs the timeline.
 *
 * @return: TRUE if everything could be included properly
 *          FALSE if Simile Exhibit Libraries couldn't be included properly
 */
function _timeline_include($timeline_theme) {
  static $loaded = FALSE;

  if (!$loaded) {

    // Check correct inclusion of local timeline library
    if ($timeline_library_status = timeline_validate_library()) {

      // some error messages are available, because of detected errors during validation
      if (is_string($timeline_library_status)) {
        drupal_set_message(check_plain($timeline_library_status));
        return FALSE;
      }

      // timeline library has been validated and will be added
      elseif ($timeline_library_status == TRUE) {

        $timeline_lib_path = libraries_get_path('simile_timeline');

        // We need $base_path for the js client not drupal_add_js().
        global $base_path;
        // Set some global js variables that the library is going to need for proper inclusion and funcionality
        $timeline_inline_js = "var Timeline_ajax_url = '" . $base_path . $timeline_lib_path . "/timeline_ajax/simile-ajax-api.js'; ";
        $timeline_inline_js .= " var Timeline_urlPrefix= '" . $base_path . $timeline_lib_path . "/timeline_js/'; ";
        if (variable_get('timeline_bundle', TRUE))
          $timeline_inline_js .= " var Timeline_parameters='bundle=true'";
        else
          $timeline_inline_js .= " var Timeline_parameters='bundle=false'";
        drupal_add_js($timeline_inline_js, array('type' => 'inline', 'weight' => 10));


        // Adding general timeline library
        drupal_add_js($timeline_lib_path . '/timeline_js/timeline-api.js', array('weight' => 11, 'preprocess' => FALSE));

        // include custom theme files (if activated)
        if ($timeline_theme != 'classic_theme' && $timeline_theme != FALSE) {
          ctools_include('plugins');
          $timeline_themes = ctools_get_plugins('timeline', 'themes');
          if (isset($timeline_themes[$timeline_theme])) {
            drupal_add_js($timeline_themes[$timeline_theme]['files']['js'], array('weight' => 12));
            drupal_add_css($timeline_themes[$timeline_theme]['files']['css']);
          }
        }

        drupal_add_css('timeline', 'timeline');
        drupal_add_js(drupal_get_path('module', 'timeline') . '/js/timeline.js', array('weight' => 13));
        $loaded = TRUE;

        return TRUE;
      }
    }
    drupal_set_message(t('Simile Timeline Libraries not found'), 'error');
    return FALSE;
  }
}
