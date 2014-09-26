<?php

/**
 * @file
 * General BOTCHA functionality and helper functions.
 */

function botcha_is_captcha_installed() {
  return module_exists('captcha');
}

function botcha_get_captcha_point($form_id, $symbolic = FALSE) {
  if (!function_exists('captcha_get_form_id_setting')) {
    module_load_include('inc', 'captcha');
  }
  // FIXME: implement failsafe here (if Captcha internals change...)
  return captcha_get_form_id_setting($form_id, $symbolic);
}
/**
 * Helper function for adding/updating a BOTCHA point.
 *
 * @param $form_id the form ID to configure.
 * @param botcha_type the setting for the given form_id, can be:
 *   - 'none' to disable BOTCHA,
 *   - 'default' to use the default challenge type
 *   - NULL to remove the entry for the BOTCHA type
 * @return nothing
 */
function botcha_set_form_id_setting($form_id, $botcha_type) {
  //watchdog('debug','IN botcha_set_form_id_setting form_id='.$form_id.' botcha_type='.$botcha_type);
  if ($botcha_type == 'none') {
    db_delete('botcha_points')->condition('form_id', $form_id)->execute();
    $id = db_insert('botcha_points')
      ->fields(array(
          'form_id' => $form_id,
          'botcha_type' => NULL,
        ))
      ->execute();
  }
  elseif ($botcha_type != NULL) {
    db_delete('botcha_points')->condition('form_id', $form_id)->execute();
    $id = db_insert('botcha_points')
      ->fields(array(
          'form_id' => $form_id,
          'botcha_type' => $botcha_type,
        ))
      ->execute();
  }
  else {
    db_delete('botcha_points')->condition('form_id', $form_id)->execute();
  }
//  else {
//    drupal_set_message(t('Failed to set a BOTCHA type for form %form_id: could not interpret value "@botcha_type"',
//      array('%form_id' => $form_id, '@botcha_type' => (string) $botcha_type)), 'warning');
//  }
}

/**
 * Get the BOTCHA setting for a given form_id.
 *
 * @param $form_id the form_id to query for
 * @param $symbolic flag to return as (symbolic) strings instead of object.
 *
 * @return NULL if no setting is known
 *   or a botcha_point object with fields 'type'.
 *   If argument $symbolic is true, returns (symbolic) as 'none', 'default', etc.
 */
function botcha_get_form_id_setting($form_id, $symbolic = FALSE) {
  $botcha_point = db_select('botcha_points', 'b')->fields('b', array('botcha_type'))->condition('form_id', $form_id)->execute()->fetchObject();

  if (empty($botcha_point)) {
    return NULL;
  }
  elseif ($botcha_point->botcha_type == NULL && $symbolic) {
    $botcha_point->botcha_type = 'none';
  }

  if ($symbolic) {
    $botcha_point = $botcha_point->botcha_type;
  }
  return $botcha_point;
}


/**
 * Helper function to get placement information for a given form_id.
 * @param $form_id the form_id to get the placement information for.
 * @param $form if a form corresponding to the given form_id, if there
 *   is no placement info for the given form_id, this form is examined to
 *   guess the placement.
 * @return placement info array (@see _botcha_insert_botcha_element() for more
 *   info about the fields 'path', 'key' and 'weight'.
 */
function _botcha_get_botcha_placement($form_id, $form) {
  // Get BOTCHA placement map from cache. Two levels of cache:
  // static variable in this function and storage in the variables table.
  static $placement_map = NULL;
  // Try first level cache.
  if ($placement_map === NULL) {
    // If first level cache missed: try second level cache.
    $placement_map = variable_get('botcha_placement_map_cache', NULL);

    if ($placement_map === NULL) {
      // If second level cache missed: start from a fresh placement map.
      $placement_map = array();
      // Prefill with some hard coded default entries.

      // The comment form can have a 'Preview' button, or both a 'Submit' and 'Preview' button,
      // which is tricky for automatic placement detection. Luckily, Drupal core sets their
      // weight (19 and 20), so we just have to specify a slightly smaller weight.
      $placement_map['comment_form'] = array(
        'path' => array(),
        'key' => NULL,
        'weight' => 18.9,
      );

//FIXME: port over from CAPTCHA D7:
      // Additional note: the node forms also have the posibility to only show a 'Preview' button.
      // However, the 'Submit' button is always present, but is just not rendered ('#access' = FALSE)
      // in those cases. The the automatic button detection should be sufficient for node forms.

      // $placement_map['user_login'] = array('path' => array(), 'key' => NULL, 'weight' => 1.9);
      // TODO: also make the placement 'overridable' from the admin UI?


      // If second level cache missed: initialize the placement map
      // and let other modules hook into this with the hook_botcha_placement_map hook.
      // By default however, probably all Drupal core forms are already correctly
      // handled with the best effort guess based on the 'actions' element (see below).
//      $placement_map = module_invoke_all('botcha_placement_map');
    }
  }

  // Query the placement map.
  if (array_key_exists($form_id, $placement_map)) {
    $placement = $placement_map[$form_id];
  }
  // If no placement info is available in placement map:
  // search the form for buttons and guess placement from it.
  else {
    $buttons = _botcha_search_buttons($form);
    if (count($buttons)) {
      // Pick first button.
      // TODO: make this more sophisticated? Use cases needed.
      $placement = $buttons[0];
    }
    else {
      // Use NULL when no buttons were found.
      $placement = NULL;
    }

    // Store calculated placement in caches.
    $placement_map[$form_id] = $placement;
    variable_set('botcha_placement_map_cache', $placement_map);
  }

  return $placement;
}

/**
 * Helper function for searching the buttons in a form.
 *
 * @param $form the form to search button elements in
 * @return an array of paths to the buttons.
 *   A path is an array of keys leading to the button, the last
 *   item in the path is the weight of the button element
 *   (or NULL if undefined).
 */
function _botcha_search_buttons($form) {
  $buttons = array();
  foreach (element_children($form) as $key) {
    // Look for submit or button type elements.
    if (isset($form[$key]['#type']) && ($form[$key]['#type'] == 'submit' || $form[$key]['#type'] == 'button')) {
      $weight = isset($form[$key]['#weight']) ? $form[$key]['#weight'] : NULL;
      $buttons[] = array(
        'path' => array(),
        'key' => $key,
        'weight' => $weight,
      );
    }
    // Process children recurively.
    $children_buttons = _botcha_search_buttons($form[$key]);
    foreach ($children_buttons as $b) {
      $b['path'] = array_merge(array($key), $b['path']);
      $buttons[] = $b;
    }
  }
  return $buttons;
}

/**
 * Helper function to insert a BOTCHA element in a form before a given form element.
 * @param $form the form to add the BOTCHA element to.
 * @param $placement information where the BOTCHA element should be inserted.
 *   $placement should be an associative array with fields:
 *     - 'path': path (array of path items) of the container in the form where the
 *       BOTCHA element should be inserted.
 *     - 'key': the key of the element before which the BOTCHA element
 *       should be inserted. If the field 'key' is undefined or NULL, the BOTCHA will
 *       just be appended to the container.
 *     - 'weight': if 'key' is not NULL: should be the weight of the element defined by 'key'.
 *       If 'key' is NULL and weight is not NULL: set the weight property of the BOTCHA element
 *       to this value.
 * @param $botcha_element the BOTCHA element to insert.
 */
function _botcha_insert_botcha_element(&$form, $placement, $botcha_element) {
  // Get path, target and target weight or use defaults if not available.
  $target_key = isset($placement['key']) ? $placement['key'] : NULL;
  $target_weight = isset($placement['weight']) ? $placement['weight'] : NULL;
  $path = isset($placement['path']) ? $placement['path'] : array();

  // Walk through the form along the path.
  $form_stepper = &$form;
  foreach ($path as $step) {
    if (isset($form_stepper[$step])) {
      $form_stepper = & $form_stepper[$step];
    }
    else {
      // Given path is invalid: stop stepping and
      // continue in best effort (append instead of insert).
      $target_key = NULL;
      break;
    }
  }

  // If no target is available: just append the BOTCHA element to the container.
  if ($target_key == NULL || !array_key_exists($target_key, $form_stepper)) {
    // Optionally, set weight of BOTCHA element.
    if ($target_weight != NULL) {
      $botcha_element['#weight'] = $target_weight;
    }
    $form_stepper['botcha'] =  $botcha_element;
  }
  // If there is a target available: make sure the BOTCHA element comes right before it.
  else {
    // If target has a weight: set weight of BOTCHA element a bit smaller
    // and just append the BOTCHA: sorting will fix the ordering anyway.
    if ($target_weight != NULL) {
      $botcha_element['#weight'] = $target_weight - .1;
      $form_stepper['botcha'] =  $botcha_element;
    }
    else {
      // If we can't play with weights: insert the BOTCHA element at the right position.
      // Because PHP lacks a function for this (array_splice() comes close,
      // but it does not preserve the key of the inserted element), we do it by hand:
      // chop of the end, append the BOTCHA element and put the end back.
      $offset = array_search($target_key, array_keys($form_stepper));
      $end = array_splice($form_stepper, $offset);
      $form_stepper['botcha'] =  $botcha_element;
      foreach ($end as $k => $v) {
        $form_stepper[$k] = $v;
      }
    }
  }
}

