<?php

/**
 * Submit handler for the embed administration form.
 * @param $form
 * @param $form_state
 */
function timelinemap_admin_embeds_controller($form, $form_state)
{
  $checked = FALSE;
  if (!empty($form_state['values']['visualizations'])) {
    foreach ($form_state['values']['visualizations'] as $value) {
      if ($value) {
        $checked = TRUE;
        break;
      }
    }
  }
  if (!empty($form_state['values']['op']) && $form_state['values']['op'] == 'Delete selected' && $checked)
    return _timelinemap_admin_embeds_delete_confirm($form_state);

  return _timelinemap_admin_embeds_form($form, $form_state);
}

/**
 * Confirmation form for deleting the embed file
 * and corresponding database records
 * @param $form_state
 */
function _timelinemap_admin_embeds_delete_confirm($form, $form_state)
{
  $desc = 'This action CAN NOT be undone.';
  // Tell the submit handler to process the form
  $form['process'] = array('#type' => 'hidden', '#value' => 'true');
  // Make sure the form redirects in the end
  $form['destination'] = array('#type' => 'hidden', '#value' => 'admin/settings/timelinemap/embeds');
  $to_delete = array();
  foreach ($form_state['values']['visualizations'] as $key => $value) {
    if ($value) {
      $to_delete[] = $value;
    }
  }
  $form['embeds_to_delete'] = array('#type' => 'hidden', '#value' => implode(',', $to_delete));
  return confirm_form($form,
                      t('Are you sure you want to delete the selected embed files?'),
                      'admin/settings/timelinemap/embeds',
                      $desc,
                      'Delete',
                      'Return');
}

/**
 * Theme function for the the embed files table
 * @param $form
 */
function theme_timelinemap_get_my_visualizations($form)
{
  $form = $form['form'];
  $select_header = theme('table_select_header_cell');
  $header = array(
    $select_header,
    t('Title'),
    t('Embed code'),
    t('Date published'),
    t('Operations'),
  );
  /*  if(module_exists('vidi'))
$header[] = t('Operations'); */

  $output = '';
  //$output .= drupal_render($form['options']);
  $rows = array();
  $counter = 0;
  if (isset($form['title'])) {
    foreach ($form['title'] as $key => $value) {
      $row = array();
      $row[] = drupal_render($form['visualizations'][$key]);
      $row[] = drupal_render($form['title'][$key]);
      $row[] = drupal_render($form['code'][$key]);
      $row[] = drupal_render($form['date'][$key]);
      $row[] = drupal_render($form['operations'][$key]);
      $rows[] = $row;
      $counter++;
    }
  }
  if ($counter == 0) {
    /* if(module_exists('vidi')) */
    $rows[] = array(array('data' => t('No posts available.'), 'colspan' => '5'));
    /*  else
$rows[] = array(array('data' => t('No posts available.'), 'colspan' => '4')); */
  }

  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  if ($form['pager']['#description']) {
    $output .= drupal_render($form['pager']);
  }

  return $output;
}

/**
 * Form for the administration of user created embed files.
 * Everyone who can make an embed file can administer their files.
 * @param $form_state
 */
function _timelinemap_admin_embeds_form($form, &$form_state)
{
  $limit = 20;
  //If the form fail validation, remember the #action attribute
  //so it doesn't redirect to the ahah callback
  /*if (!isset($form_state['storage']['old_action'])) {
    $form_state['storage']['old_action'] = isset($form['#action']) ? $form['#action'] : $_SERVER['REQUEST_URI'];
  }  */

  $script = "function popup(mylink, windowname){if (! window.focus)return true; var href; if (typeof(mylink) == 'string') href=mylink; else href=mylink.href; window.open(href, windowname, 'width=1000,height=800,scrollbars=yes');return false;}";
  drupal_add_js($script, 'inline');

  $query = db_select('timelinemap_embeds', 'te')
          ->fields('te', array('fid', 'title', 'link_vidi', 'link_view', 'visualisation_code'))
          ->fields('f', array('filename', 'uri', 'timestamp'))
          ->extend('PagerDefault')
          ->limit($limit);
  $query->leftJoin('file_managed', 'f', 'te.fid = f.fid');

  if (user_access('administer embeds')) {
    $form['filter'] = array(
      '#type' => 'fieldset',
      '#title' => t('Fiter embeds'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );
    $results = db_query('SELECT uid, name FROM {users}');
    $users = array();
    foreach ($results as $result) {
      $users[$result->uid] = $result->uid != 0 ? $result->name : "anonymous";
    }
    $form['filter']['per_user'] = array(
      '#type' => 'select',
      '#multiple' => TRUE,
      //'#default_value' => $form_state['values']['per_user'],
      '#default_value' => isset($_SESSION['timelinemap_embed_filters']) ? $_SESSION['timelinemap_embed_filters'] : '',
      '#title' => t('Fiter embeds'),
      '#options' => $users
    );

    $form['filter_button'] = array(
      '#type' => 'submit',
      '#value' => t('Filter')
    );

    $form['filter_button_clear'] = array(
      '#type' => 'submit',
      '#value' => t('Clear filters'),
    );

    if (!empty($_SESSION['tagmap_embed_filters'])) {
      $conditions = db_or();
      foreach ($_SESSION['tagmap_embed_filters'] as $value) {
        $conditions->condition('f.uid', $value);
      }
      $query->condition($conditions);
    }

    /*
if(isset($_GET['per_user']))
$form_state['values']['per_user'] = $_GET['per_user'];
$sql = "SELECT te.fid, te.title, te.link_vidi, te.link_view, te.visualisation_code, te.datasource, f.filename, f.filepath, f.timestamp FROM timelinemap_embeds te LEFT JOIN files f ON te.fid = f.fid";
if(!empty($form_state['values']['per_user'])){
$sql .= " WHERE";

foreach ($form_state['values']['per_user'] as $value)
 $sql .= " f.uid = %d OR";

$sql = substr($sql, 0, strlen($sql) - 2);
$results = pager_query($sql, $limit, 0, NULL, $form_state['values']['per_user']);
}
else
$results = pager_query($sql, $limit, 0, NULL);*/
  }
  else {
    global $user;
    /*    $sql = "SELECT te.fid, te.title, te.link_vidi, te.link_view, te.visualisation_code, te.datasource, f.filename, f.filepath, f.timestamp FROM timelinemap_embeds te LEFT JOIN files f ON te.fid = f.fid WHERE f.uid = %d";
$results = pager_query($sql, $limit, 0, NULL, $user->uid);  */
    $query->condition('f.uid', $user->uid);
  }
  $results = $query->execute();
  $destination = drupal_get_destination();
  $visualizations = array();
  foreach ($results as $result) {
    $visualizations[$result->fid] = '';
    $form['data']['title'][$result->fid] = array('#type' => 'item', '#description' => l($result->title, file_create_url($result->uri), array('attributes' => array('onClick' => 'return popup(this, \'My visualization\')'))));
    $form['data']['date'][$result->fid] = array('#type' => 'item', '#description' => format_date($result->timestamp));
    $form['data']['code'][$result->fid] = array('#type' => 'item', '#description' => $result->visualisation_code, '#prefix' => '<div style="font-size:11px;">', '#suffix' => '</div>');
    $vidilink = FALSE;

    /*
    $visualizations[$result->fid] = '';
    $form['data']['title'][$result->fid] = array('#value' => l($result->title, $result->filepath, array('attributes' => array('onClick' => 'return popup(this, \'My visualization\')'))));
    $form['data']['date'][$result->fid] =  array('#value' => format_date($result->timestamp));
    $form['data']['code'][$result->fid] =  array('#value' => $result->visualisation_code, '#prefix' => '<div style="font-size:9px;">', '#suffix' => '</div>',);
    */

    $vidilink = FALSE;
    $op_str = '';
    if (module_exists('vidi') && $result->link_vidi) {
      $op_str = l('edit embed', $result->link_vidi, array('query' => array('fid' => $result->fid, 'datasource' => $result->datasource)));
      $vidilink = TRUE;
    }
    if ($result->link_view) {
      $exists = FALSE;
      $link_arr = explode('/', $result->link_view);
      if ($view = views_get_view($link_arr[4])) {
        if (array_key_exists($link_arr[5], $view->display))
          $exists = TRUE;
      }

      if ($vidilink && $exists) {
        $op_str .= ' | ';
      }
      if ($exists && user_access('administer views'))
        $op_str .= l('edit view', $result->link_view);
    }
    $form['data']['operations'][$result->fid] = array('#type' => 'item', '#description' => $op_str);

  }
  $form['data']['visualizations'] = array('#type' => 'checkboxes', '#options' => $visualizations);
  $form['data']['pager'] = array('#type' => 'item', '#description' => theme('pager'));
  $form['data']['#theme'] = 'timelinemap_get_my_visualizations';
  $form['data']['#prefix'] = '<div id="timelinemap-embed-table-ahah">';
  $form['data']['#suffix'] = '</div>';
  $form['timelinemap_delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete selected'),
  );
  return $form;
}

/**
 * Ahah callback for list of embeded files
 * (filters the embeds by user)
 */
/*
function timelinemap_embed_ahah (){
  $form_state = array('storage' => NULL, 'submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];

  $form = form_get_cache($form_build_id, $form_state);

  $args = $form['#parameters'];
  $form_id = array_shift($args);
  $form_state['post'] = $form['#post'] = $_POST;
  $form['#programmed'] = $form['#redirect'] = FALSE;

  drupal_process_form($form_id, $form, $form_state);
  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);

  $data = $form['data'];
  $_GET['q'] = 'admin/content/timelinemap/embeds';
  $data['pager'] = array(
    '#value' => theme('pager', NULL, 3)
  );
  unset($data['#prefix'], $data['#suffix']);
  $output = theme('status_messages') . drupal_render($data);

  drupal_json(array('status' => TRUE, 'data' => $output));
}*/

/**
 * Submit handler for the embed administration form.
 * @param $form
 * @param $form_state
 */
function timelinemap_admin_embeds_controller_submit($form, &$form_state)
{
  if ($form_state['clicked_button']['#id'] != 'edit-filter-button' && $form_state['clicked_button']['#id'] != 'edit-filter-button-clear') {
    if (isset($form_state['values']['embeds_to_delete'])) {
      $for_deleting = explode(',', $form_state['values']['embeds_to_delete']);
      $txn = db_transaction();
      foreach ($for_deleting as $value) {
        $file = file_load($value);
        if (!file_delete($file)) {
          drupal_set_message(t('An error occurred while deleting the file fid @id from the file_managed table!', array('@id' => $value)), 'error');
          $txn->rollback();
          return TRUE;
        }
        $sql = "DELETE FROM {tagmap_embeds} WHERE fid = :fid";
        if (!db_query($sql, array(':fid' => $value))) {
          drupal_set_message(t('An error occurred while deleting the record with fid @id from the tagmap_embeds table!', array('@id' => $value)), 'error');
          $txn->rollback();
          return TRUE;
        }
        drupal_set_message(t('The file @name was deleted successfully!', array('@name' => $file->filename)), 'status');
      }


      return TRUE;
    }
    $form_state['rebuild'] = TRUE;
  }
  elseif ($form_state['clicked_button']['#id'] == 'edit-filter-button') {
    //    $form_state['rebuild'] = TRUE;
    $_SESSION['tagmap_embed_filters'] = $form_state['values']['per_user'];
  }
  elseif ($form_state['clicked_button']['#id'] == 'edit-filter-button-clear') {
    //    $form_state['rebuild'] = TRUE;
    unset($_SESSION['tagmap_embed_filters']);
  }
}

/**
 * @param $uid
 * @param $filename. The name of the embed file.
 * @param $filepath. Relative path of the embed file.
 * @param $storage. The state of the form from the VIDI module. Used to populate the form with correct data.
 * @param $vidi_link. link to the VIDI module form.
 * @param $view_link. Link to the corresponding view and display.
 * @param $datasource. Timelinemap type (basic or pathline).
 * @param $result. Embed code.
 * @param $update. Is it an update or a creation of a new embed file.
 * @param $title. The title of the visualization. Displayed on the embed administration form.
 */
function _timelinemap_save_file_data($uid, $file, $storage, $vidi_link, $view_link, $datasource, $result, $update, $title)
{
  $timelinemap_embed_data = new stdClass();
  $timelinemap_embed_data->fid = $file->fid;
  $timelinemap_embed_data->title = $title;
  $timelinemap_embed_data->visualisation_code = $result;

  if (!empty($storage)) {
    $timelinemap_embed_data->form_state_storage = $storage;
  }
  if (!empty($vidi_link)) {
    $timelinemap_embed_data->link_vidi = $vidi_link;
  }
  if (!empty($view_link)) {
    $timelinemap_embed_data->link_view = $view_link;
  }
  if (!$update) {
    drupal_write_record('timelinemap_embeds', $timelinemap_embed_data);
  }
  else {
    drupal_write_record('timelinemap_embeds', $timelinemap_embed_data, 'fid');
  }
}

/**
 * Ajax calback that makes the embed file,
 * inserts corresponding data in the database
 * and returns the embed code to the user.
 */
function timelinemap_embed()
{
  global $user;
  global $base_url;
  $id = $_POST['id'];
  $rel = $_POST['rel'];
  $div_id = $_POST['divId'];
  $settings = $_POST['settings'];
  $timelinecontainer_style = $_POST['timelinecontainerStyle'];
  $mapcontainer = $_POST['mapcontainer'];
  if ($div_id) {
    $css = '<link type="text/css" rel="stylesheet" media="all" href="' . $base_url . '/' . drupal_get_path('module', 'timelinemap') . '/css/timelinemap.css"/>';
    $js = '<script type="text/javascript" src="' . $base_url . '/misc/jquery.js"></script>';
    $js .= '<script type="text/javascript" src="' . $base_url . '/misc/drupal.js"></script>';
    $gmap_apikey = variable_get('googlemap_api_key', '');
    $timelinemap_apikey = $gmap_apikey ? $gmap_apikey : variable_get('timelinemap_api_key', '');
    $timelinemap_gapi = 'http://maps.google.com/maps?file=api&v=2&key=' . $timelinemap_apikey;
    $timelinemap_sapi = 'http://static.simile.mit.edu/timeline/api/timeline-api.js';
    $js .= '<script type="text/javascript" src="' . $timelinemap_gapi . '"></script>';
    $js .= '<script type="text/javascript" src="' . $timelinemap_sapi . '"></script>';
    $js .= '<script type="text/javascript" src="' . $base_url . '/' . drupal_get_path('module', 'timelinemap') . '/js/timemap.js"></script>';
    $js .= '<script src="' . check_url(url('http://js.mapbox.com/g/2/mapbox.js')) . '" type="text/javascript"></script>';
    if ($_POST['source'] == 'viditimelinemap')
      $js .= '<script type="text/javascript" src="' . $base_url . '/' . drupal_get_path('module', 'vidi') . '/js/vidi_timelinemap.js"></script>';
    else
      $js .= '<script type="text/javascript" src="' . $base_url . '/' . drupal_get_path('module', 'timelinemap') . '/js/timelinemap.js"></script>';

    $js .= '<script type="text/javascript">' . $settings . '</script>';

    $output = '<html><head><meta http-equiv="Content-Type" content="text/html; charset=utf-8" />' . $css . $js . '<style>img, a img {border:none}</style></head>';
    if ($_POST['source'] == 'viditimelinemap')
      $output .= '<body><div id="timelinemap-all-' . $div_id . '" class = "viditimelinemap-all">';
    else
      $output .= '<body><div id="timelinemap-all-' . $div_id . '" class = "timelinemap-all">';
    $output .= '<div class = "timelinecontainer" style = "' . $timelinecontainer_style . '">' .
               '<div id = "timeline-' . $div_id . '" class = "timeline"></div>' .
               '</div>' .
               '<div class = "mapcontainer" style = "' . $mapcontainer . '">' .
               '<div id = "map-' . $div_id . '" class = "map"></div>' .
               '</div>' .
               '</div>' .
               '</body>';
    $output .= '</html>';

    $directory = file_build_uri('timelinemap_embed');
    file_prepare_directory($dir, FILE_CREATE_DIRECTORY);
    $directory = file_build_uri('timelinemap_embed/' . $user->uid);
    file_prepare_directory($directory, TRUE);
    if (empty($_POST['overwrite'])) {
      $filename = $user->name . '_timelinemap_embed' . '.html';
      $file = file_save_data($output, $directory . '/' . $filename, FILE_EXISTS_RENAME);
      $update = FALSE;
    } else {
      $filename = $_POST['overwrite'] . '.html';
      $file = file_save_data($output, $directory . '/' . $filename, FILE_EXISTS_REPLACE);
      $update = TRUE;
    }

    $result = "<div class=\"embed-instructions\"><strong>Embed link:</strong><br/>";
    $result .= "<div style = 'border: 1px solid #cdcdcd; padding: 3px; color: #666;' class=\"embed-code\" >";
    $result .= "&lt;iframe scrolling = \"no\" width=\"" . $_POST['attrwidth'] . "\" height=\"" . $_POST['attrheight'] . "\" src=\"";
    $result .= file_create_url($file->uri);
    $result .= "\"&gt;&lt;/iframe&gt</div></div>";
    _timelinemap_save_file_data($user->uid, $file, $_POST['storage'], $_POST['vidiLink'], $_POST['viewLink'], $_POST['datasource'], $result, $update, $_POST['title']);
    echo $result;
  }
}

/**
 * Submit handler for the embed administration form.
 * @param $form
 * @param $form_state
 */
function timelinemap_my_visualizations_submit($form, &$form_state)
{
  if (isset($form_state['values']['embeds_all']['embeds_to_delete'])) {
    $for_deleting = explode(',', $form_state['values']['embeds_all']['embeds_to_delete']);

    foreach ($for_deleting as $value) {
      $filepath = db_result(db_query("SELECT filepath FROM {files} WHERE fid=%d", $value));
      $sql = "DELETE FROM {timelinemap_embeds} WHERE fid = %d";
      if (!db_query($sql, $value)) {
        drupal_set_message(t('An error occured while deleting the record with fid @id from the timelinemap_embeds table!', array('@id' => $value)), 'error');
        return TRUE;
      }
      $sql = "DELETE FROM {files} WHERE fid = %d";
      if (!db_query($sql, $value)) {
        drupal_set_message(t('An error occured while deleting the record with fid @id  from the files table!', array('@id' => $value)), 'error');
        return TRUE;
      }
      if (!file_delete($filepath)) {
        drupal_set_message(t('The file @path was NOT deleted!', array('@path' => $filepath)), 'error');
        return TRUE;
      }
    }
    drupal_set_message(t('The file @path was deleted successfully!', array('@path' => $filepath)), 'status');
    return TRUE;
  }
  $form_state['rebuild'] = TRUE;
}
