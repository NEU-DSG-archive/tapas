<?php
// $Id: regcode_ie.views.inc,v 1.1.2.1 2010/03/07 14:58:16 aidan Exp $


/**
 * Implements hook_views_default_views().
 */
function regcode_ie_views_default_views_alter(&$views) {
  if (isset($views['regcode'])) {
    $view = $views['regcode'];

    $handler = $view->new_display('feed', 'CSV', 'csv');
    $handler->override_option('items_per_page', 0);
    $handler->override_option('style_plugin', 'views_csv');
    $handler->override_option('style_options', array(
      'mission_description' => FALSE,
      'description' => '',
      'attach_text' => 'CSV',
      'provide_file' => 1,
      'filename' => 'registration_codes.csv',
      'parent_sort' => 0,
      'seperator' => ',',
      'quote' => 1,
      'header' => 1,
    ));
    $handler->override_option('row_plugin', '');
    $handler->override_option('path', 'admin/user/regcode/export');
    $handler->override_option('menu', array(
      'type' => 'none',
      'title' => '',
      'description' => '',
      'weight' => 0,
      'name' => 'navigation',
    ));
    $handler->override_option('tab_options', array(
      'type' => 'none',
      'title' => '',
      'description' => '',
      'weight' => 0,
    ));
    $handler->override_option('displays', array(
      'page_admin' => 'page_admin',
      'default' => 0,
      'page_1' => 0,
    ));
    $handler->override_option('sitename_title', FALSE);
  }
}
