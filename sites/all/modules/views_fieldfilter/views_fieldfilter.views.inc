<?php

/**
 * @file
 *  Views intergration for field filter
 */

/**
 * Implementation of hook_views_data().
 */
function views_fieldfilter_views_data() {
  $data = array();
  
  $data['views']['field_filter'] = array(
    'title' => t('Field filter'),
    'help' => t('Allow users to select which fields are to be displayed. works best as exposed filter.'),
    'filter' => array(
      'handler' => 'views_fieldfilter_handler_filter_fields',
    ),
  );
  
  return $data;
}

