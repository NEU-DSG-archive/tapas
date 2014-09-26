<?php
/**
 * @file
 */
 
/**
 *
 */
function theme_views_fieldfilter_admin_table($variables) {
  $table = array(
    'header' => array(
      t('Field'),
      t('Select'),
      t('State'),
    ),
  );
  foreach (element_children($variables['form']) as $name) {
    $table['rows'][] = array(
      drupal_render($variables['form'][$name]['label']),
      drupal_render($variables['form'][$name]['selected']),
      drupal_render($variables['form'][$name]['state']),      
    );
  }
  
  return theme('table', $table);
}