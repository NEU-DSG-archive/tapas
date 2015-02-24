<?php

/**
 * @file
 * Implementation of botcha administration forms.
 */

/**
 * Return an array with the available BOTCHA recipes, for use as options array
 * for a select form elements.
 *
 * @param $add_special_options if true: also add a 'none' and 'default' option
 *
 * @return an associative array mapping string value to the name of the BOTCHA cookbook.
 */
function _botcha_available_cookbooks() {
  $botcha_types = array(
    'none' => '[' . t('none') . ']',
    'default' => '[' . t('default') . ']',
  );
  return $botcha_types;
}

/**
 * Module settings form.
 */
function _botcha_admin_settings(&$form_state) {
  module_load_include('inc', 'botcha');

  $form = array();

  $form['botcha_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret key'),
    '#default_value' => variable_get('botcha_secret', botcha_generate_secret_key() ),
    '#description' => t('It is recommended to enter some random text into the secret key. This setting makes your site\'s BOTCHA challenges unique and harder to break.')
      . '<br />' . t('If you leave this field empty and save configuration, a new random key will be generated for you.')
    ,
  );

  // Configuration of which forms to protect, with what recipe.
  $form['botcha_form_protection'] = array(
    '#type' => 'fieldset',
    '#title' => t('Form protection'),
    '#description' => t('Select which forms to protect with BOTCHA')
//      . ' ' . t('Select the recipe you want for each of the listed forms (identified by their so called <em>form_id</em>\'s). You can easily add arbitrary forms with textfield at the bottom of the table or with the help of the \'%BOTCHA_admin_links\' option below.',
//        array('%BOTCHA_admin_links' => t('Add BOTCHA administration links to forms')))
    ,
  );

  if (botcha_is_captcha_installed()) {
    // Checkbox to put BOTCHA on same forms as CAPTCHA.
    $form['botcha_form_protection']['botcha_on_captcha_forms'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add BOTCHA to forms selected for CAPTCHA'),
      '#default_value' => variable_get('botcha_on_captcha_forms', TRUE),
      '#description' => t('This option makes it easy to manage BOTCHA settings on forms. When enabled, all forms that are configured to have CAPTCHA on them (<a href="@captcha">see configuration</a>) will also be selected for BOTCHA protection.!more',
        array(
          '@captcha' => url('admin/config/people/captcha'),
          '!more' => module_exists('captcha') ? '' : '<br />' . t('<b>Note:</b> <a href="@captcha_home">CAPTCHA module</a> is not installed. This setting will have no effect.',
            array('@captcha_home' => 'http://drupal.org/project/captcha')
          ),
        )),
    );
  }

  // List known form_ids.
  $form['botcha_form_protection']['botcha_form_id_overview'] = array(
    '#theme' => 'botcha_admin_settings_botcha_points',
    '#tree' => TRUE,
  );
  $form['botcha_form_protection']['botcha_form_id_overview']['botcha_botcha_points'] = array();
  $botcha_type_options = _botcha_available_cookbooks();
  $result = db_select('botcha_points', 'cp')->fields('cp')->orderBy('form_id')->execute();
  foreach ($result as $botcha_point) {
    $form['botcha_form_protection']['botcha_form_id_overview']['botcha_botcha_points'][$botcha_point->form_id] = array();
    $form['botcha_form_protection']['botcha_form_id_overview']['botcha_botcha_points'][$botcha_point->form_id]['form_id'] = array(
      '#markup' => $botcha_point->form_id,
    );
    // Select widget for BOTCHA type.
    if (isset($botcha_point->botcha_type) && $botcha_point->botcha_type) {
      $botcha_type = $botcha_point->botcha_type;
    }
    else {
      $botcha_type = 'none';
    }
    $form['botcha_form_protection']['botcha_form_id_overview']['botcha_botcha_points'][$botcha_point->form_id]['botcha_type'] = array(
      '#type' => 'select',
      '#default_value' => $botcha_type,
      '#options' => $botcha_type_options,
    );
    // Additional operations.
    $form['botcha_form_protection']['botcha_form_id_overview']['botcha_botcha_points'][$botcha_point->form_id]['operations'] = array(
      '#markup' => implode(", ", array(
        l(t('delete'), "admin/config/people/botcha/botcha_point/{$botcha_point->form_id}/delete"),
      )),
    );
  }

  // Form items for new form_id.
  $form['botcha_form_protection']['botcha_form_id_overview']['botcha_new_botcha_point'] = array();
  // Textfield for form_id.
  $form['botcha_form_protection']['botcha_form_id_overview']['botcha_new_botcha_point']['form_id'] = array(
    '#type' => 'textfield',
    '#size' => 16,
  );
  // Select widget for BOTCHA type.
  $form['botcha_form_protection']['botcha_form_id_overview']['botcha_new_botcha_point']['botcha_type'] = array(
    '#type' => 'select',
    '#default_value' => 'default',
    '#options' => $botcha_type_options,
  );

  // Field for the BOTCHA administration mode.
  $form['botcha_form_protection']['botcha_administration_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add BOTCHA administration links to forms'),
    '#default_value' => variable_get('botcha_administration_mode', FALSE),
    '#description' => t('This option makes it easy to manage BOTCHA settings on forms. When enabled, users with the "%adminbotcha" permission will see a fieldset with BOTCHA administration links on all forms, except on administrative pages.', array('%adminbotcha' => t('administer BOTCHA settings'))),
  );
  // Field for the BOTCHAs on admin pages.
  $form['botcha_form_protection']['botcha_allow_on_admin_pages'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow BOTCHAs and BOTCHA administration links on administrative pages'),
    '#default_value' => variable_get('botcha_allow_on_admin_pages', FALSE),
    '#description' => t('This option makes it possible to add BOTCHAs to forms on administrative pages. BOTCHAs are disabled by default on administrative pages (which shouldn\'t be accessible to untrusted users normally) to avoid the related overhead. In some situations, e.g. in the case of demo sites, it can be usefull to allow BOTCHAs on administrative pages.'),
  );

  // BOTCHA Statistics & Logging
  $form['botcha_statistics'] = array(
    '#type' => 'fieldset',
    '#title' => t('Statistics & logging'),
    '#description' => t('BOTCHA collects statistics of form submissions and it can report different events into the system log.'),
  );
  $dblog_link = l(t('log'), 'admin/reports/dblog');
  $form['botcha_statistics']['botcha_loglevel'] = array(
    '#type' => 'select',
    '#title' => t('Log level'),
    '#default_value' => variable_get('botcha_loglevel', 1),
    '#options' => array(
      0 => t('0: no log'),
      1 => t('1: blocked/bad submissions only'),
      2 => t('2: ... and why blocked'),
      3 => t('3: ... and good submissions'),
      4 => t('4: ... and protected forms'),
      5 => t('5: ... and extra submission details'),
      6 => t('6: ... and misc development items'),
    ),
    '#description' => t('Select what information to report into the !log.', array('!log' => $dblog_link)),
  );
  // Button for resetting the BOTCHA statistics.
  $form['botcha_statistics']['botcha_statistics_group'] = array(
    '#type' => 'item',
    '#title' => t('BOTCHA statistics'),
    '#description' => t('Reset all accumulated statistics of form submissions.'),
  );
  // Show statistic counters.
  $block_cnt = variable_get('botcha_form_blocked_counter', 0);
  $build_cnt = variable_get('botcha_form_passed_counter', 0) + $block_cnt;
  $form['botcha_statistics']['botcha_statistics_group']['botcha_statistics'] = array(
    '#type' => 'item',
    '#value' => format_plural(
      $block_cnt,
      'Already 1 blocked form submission',
      'Already @count blocked form submissions'
    )
    . ($build_cnt > 0 ? ' ' . t('(!percent% of total !build_cnt processed)',
      array('!percent' => sprintf("%0.3f", 100 * $block_cnt / $build_cnt), '!build_cnt' => $build_cnt)) : ''),
  );
  $form['botcha_statistics']['botcha_statistics_group']['botcha_statistics_reset'] = array(
    '#type' => 'button',
    '#value' => t('Reset BOTCHA statistics'),
    '#submit' => array('botcha_statistics_reset'),
  );
  // Handle the button for resetting the BOTCHA statistics.
  // This is done here instead of in a submit handler because the button is
  // not a submitting button.
  if (isset($form_state['input']['op']) && $form_state['input']['op']
    == $form['botcha_statistics']['botcha_statistics_group']['botcha_statistics_reset']['#value']) {
    variable_set('botcha_form_passed_counter', 0);
    variable_set('botcha_form_blocked_counter', 0);
    drupal_set_message(t('BOTCHA statistics have been reset.'));
  }

  return $form;
}

/**
 * Custom theme function for a table of (form_id -> BOTCHA type) settings
 */
function theme_botcha_admin_settings_botcha_points($variables) {
  $form = $variables['form'];
  $header = array('form_id', t('Protection type'), t('Operations'));
  $rows = array();
  // Existing BOTCHA points.
  foreach (element_children($form['botcha_botcha_points']) as $key) {
    $row = array();
    $row[] = drupal_render($form['botcha_botcha_points'][$key]['form_id']);
    $row[] = drupal_render($form['botcha_botcha_points'][$key]['botcha_type']);
    $row[] = drupal_render($form['botcha_botcha_points'][$key]['operations']);
    $rows[] = $row;
  }
  // For new BOTCHA point.
  $row = array();
  $row[] = drupal_render($form['botcha_new_botcha_point']['form_id']);
  $row[] = drupal_render($form['botcha_new_botcha_point']['botcha_type']);
  $row[] = '';
  $rows[] = $row;

  $output = theme('table', array('header' => $header, 'rows' => $rows));
  return $output;
}

function botcha_admin_settings($form, &$form_state) {
//  $form = system_settings_form(_botcha_admin_settings($form_state));
  // We can't use system_settings_form() here because it will put all extra stuff into variables, that we want to avoid.
  $form = _botcha_admin_settings($form_state);

  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['buttons']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
  );

//  if (!empty($_POST) && form_get_errors()) {
//    drupal_set_message(t('The settings have not been saved because of the errors.'), 'error');
//  }
//  $form['#submit'][] = 'system_settings_form_submit';
  $form['#theme'] = 'system_settings_form';

//  $form['#validate'][] = 'botcha_admin_settings_validate';
//  $form['#submit'][] = 'botcha_admin_settings_submit';
  return $form;
}

/**
 * Validation handler for botcha_admin_settings form.
 */
function botcha_admin_settings_validate($form, &$form_state) {
  $form_id = $form_state['values']['botcha_form_id_overview']['botcha_new_botcha_point']['form_id'];
  if (!preg_match('/^[a-z0-9_]*$/', $form_id)) {
    form_set_error('botcha_form_id_overview][botcha_new_botcha_point][form_id', t('Illegal form_id'));
  }
}

/**
 * Submission function for botcha_admin_settings form.
 */
function botcha_admin_settings_submit($form, &$form_state) {
  module_load_include('inc', 'botcha');
  // Generate the secret key
  if (empty($form_state['values']['botcha_secret'])) {
    // Generate unique secret for this site
    $secret = botcha_generate_secret_key();
    $form_state['values']['botcha_secret'] = $secret;
    drupal_set_message(t('New BOTCHA secret key have been generated.'));
  }
  // Do what system_settings_form() would do with regular variable fields
  variable_set('botcha_secret', $form_state['values']['botcha_secret']);
  variable_set('botcha_on_captcha_forms', !empty($form_state['values']['botcha_on_captcha_forms']) ? $form_state['values']['botcha_on_captcha_forms'] : FALSE);
  variable_set('botcha_administration_mode', $form_state['values']['botcha_administration_mode']);
  variable_set('botcha_allow_on_admin_pages', $form_state['values']['botcha_allow_on_admin_pages']);
  variable_set('botcha_loglevel', $form_state['values']['botcha_loglevel']);

  // Process BOTCHA points
  if (isset($form_state['values']['botcha_form_id_overview']['botcha_botcha_points'])) {
    foreach ($form_state['values']['botcha_form_id_overview']['botcha_botcha_points'] as $botcha_point_form_id => $data) {
      botcha_set_form_id_setting($botcha_point_form_id, $data['botcha_type']);
    }
  }

  // Add new BOTCHA point?
  $botcha_point_form_id = $form_state['values']['botcha_form_id_overview']['botcha_new_botcha_point']['form_id'];
  if (!empty($botcha_point_form_id)) {
    $botcha_type = $form_state['values']['botcha_form_id_overview']['botcha_new_botcha_point']['botcha_type'];
    botcha_set_form_id_setting($botcha_point_form_id, $botcha_type);
    drupal_set_message(t('Added BOTCHA of type %type for form %form_id.', array('%type' => $botcha_type, '%form_id' => $botcha_point_form_id)), 'status');
  }

  drupal_set_message(t('The BOTCHA settings were saved.'), 'status');
}


/**
 * Central handler for BOTCHA point administration (adding, disabling, deleting)
 */
function botcha_point_admin($botcha_point_form_id = NULL, $op = NULL) {
  module_load_include('inc', 'botcha');

  // if $botcha_point_form_id and action $op given: do the action
  if ($botcha_point_form_id) {
    switch ($op) {
      case 'disable':
        return drupal_get_form('botcha_point_disable_confirm', $botcha_point_form_id, FALSE);
      case 'delete':
        return drupal_get_form('botcha_point_disable_confirm', $botcha_point_form_id, TRUE);
    }
    // return edit form for BOTCHA point
    return drupal_get_form('botcha_point_admin_form', $botcha_point_form_id);
  }
  // return add form for BOTCHA point
  return drupal_get_form('botcha_point_admin_form');
}

/**
 * Form for adding BOTCHA point.
 */
function botcha_point_admin_form($form, $form_state, $botcha_point_form_id = NULL) {
  $form = array();
  $default_botcha_type = 'default';
  if (isset($botcha_point_form_id)) {
    // use given BOTCHA point form_id
    $form['botcha_point_form_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Form ID'),
      '#description' => t('The Drupal form_id of the form to add the BOTCHA to.'),
      '#value' => check_plain($botcha_point_form_id),
      '#disabled' => TRUE,
    );
    $botcha_point = botcha_get_form_id_setting($botcha_point_form_id);
    if ($botcha_point) {
      $default_botcha_type = $botcha_point->botcha_type;
    }
  }
  else {
    // textfield for BOTCHA point form_id
    $form['botcha_point_form_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Form ID'),
      '#description' => t('The Drupal form_id of the form to add the BOTCHA to.'),
    );
  }
  // select widget for BOTCHA type
  $form['botcha_type'] = array(
    '#type' => 'select',
    '#title' => t('Challenge type'),
    '#description' => t('The BOTCHA type to use for this form'),
    '#default_value' => $default_botcha_type,
    '#options' => _botcha_available_cookbooks(),
  );
  // redirect to general BOTCHA settings page after submission
  $form_state['#redirect'] = 'admin/config/people/botcha';
  // submit button
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}


/**
 * validation function for botcha_point_admin_form
 */
function botcha_point_admin_form_validate($form, $form_state) {
  if (!preg_match('/^[a-z0-9_]+$/', $form_state['values']['botcha_point_form_id'])) {
    form_set_error('botcha_point_form_id', t('Illegal form_id'));
  }
}


/**
 * Submit function for botcha_point_admin_form.
 */
function botcha_point_admin_form_submit($form, $form_state) {
//?  module_load_include('inc', 'botcha');
  $botcha_point_form_id = $form_state['values']['botcha_point_form_id'];
  $botcha_type = $form_state['values']['botcha_type'];
  botcha_set_form_id_setting($botcha_point_form_id, $botcha_type);
  drupal_set_message(t('Saved BOTCHA point settings.'), 'status');
}

/**
 * Confirm dialog for disabling/deleting a BOTCHA point
 */
function botcha_point_disable_confirm($form, &$form_state, $botcha_point_form_id, $delete) {
  $form = array();
  $form['botcha_point_form_id'] = array(
    '#type' => 'value',
    '#value' => $botcha_point_form_id,
  );
  $form['botcha_point_delete'] = array(
    '#type' => 'value',
    '#value' => $delete,
  );
  if ($delete) {
    $message = t('Are you sure you want to delete the BOTCHA for form_id %form_id?', array('%form_id' => $botcha_point_form_id));
    $yes = t('Delete');
  }
  else {
    $message = t('Are you sure you want to disable the BOTCHA for form_id %form_id?', array('%form_id' => $botcha_point_form_id));
    $yes = t('Disable');
  }
  return confirm_form($form, $message, 'admin/config/people/botcha', t(''), $yes);
}

/**
 * Submission handler of BOTCHA point disabling/deleting confirm_form.
 */
function botcha_point_disable_confirm_submit($form, &$form_state) {
//?  module_load_include('inc', 'botcha');
  $botcha_point_form_id = $form_state['values']['botcha_point_form_id'];
  $delete = $form_state['values']['botcha_point_delete'];
  if ($delete) {
    botcha_set_form_id_setting($botcha_point_form_id, NULL);
    drupal_set_message(t('Deleted BOTCHA for form %form_id.', array('%form_id' => $botcha_point_form_id)));
  }
  else {
    botcha_set_form_id_setting($botcha_point_form_id, 'none');
    drupal_set_message(t('Disabled BOTCHA for form %form_id.', array('%form_id' => $botcha_point_form_id)));
  }
  $form_state['redirect'] = 'admin/config/people/botcha/botcha';
}

/**
 * Generate a random secret key.
 */
function botcha_generate_secret_key() {
  return md5(uniqid(mt_rand(), TRUE));
}

//END