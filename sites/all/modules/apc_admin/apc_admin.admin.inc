<?php
/**
 * @file
 * APC Administration Interface
 */

/**
 * Main settings and review administration screen.
 */
function apc_admin_settings_form($form, &$form_state) {
  global $base_url;

  $form = array();

  // Add css for admininstration.
  drupal_add_css(drupal_get_path('module', 'apc_admin') . '/apc_admin.css');

  // Check if APC enabled at all.
  if (!function_exists('apc_cache_info') || !(@apc_cache_info('opcode'))) {

    $form['message'] = array(
      '#markup' => t('No cache info available.  APC does not appear to be running. Enable it in your php.ini and comeback here.'),
    );

    return $form;
  }

  // Check if we have library installed.
  $secret_key = variable_get('apc_admin_secret_key', NULL);
  if (!$secret_key) {

    $form['apc_secret_key'] = array(
      '#type' => 'fieldset',
      '#title' => t('Generate Secret Key'),
    );

    $form['apc_secret_key']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Generate secret key'),
      '#submit' => array('apc_admin_generate_secret_key'),
    );

    return $form;
  }

  // Check if we have library on path.
  $apc_php_path = libraries_get_path('apc_admin') . '/' . $secret_key . '/apc.php';

  if (!is_file($apc_php_path)) {
    $profile = drupal_get_path('profile', drupal_get_profile());
    $config = conf_path();

    // Possible lib paths.
    $lib_paths = array(
      'libraries',
      "$profile/libraries",
      'sites/all/libraries',
      "$config/libraries",
    );

    $path_in_libraries = "apc_admin/$secret_key/apc.php";

    foreach ($lib_paths as $lib_path) {
      $example_lib_paths[] = $lib_path . '/' . $path_in_libraries;
    }

    drupal_set_message(
      t('You should place apc.php to your library directory, so the path to file would look like (choose any): !example_lib_paths',
        array(
          '!example_lib_paths' => theme('item_list', array('items' => $example_lib_paths)),
        )
      ),
      'warning'
    );
    return $form;
  }

  // Node author information for administrators.
  $form['apc_reset_cache'] = array(
    '#type' => 'fieldset',
    '#title' => t('Reset APC Cache'),
  );

  $form['apc_reset_cache']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Reset Cache'),
    '#submit' => array('apc_admin_reset_cache'),
  );

  $form['apc_php'] = array(
    '#type' => 'fieldset',
    '#title' => t('APC Administration'),
  );

  $form['apc_php']['apc_php_iframe'] = array(
    '#markup' => '<iframe src="' . $base_url . '/' . $apc_php_path . '"></iframe>',
  );

  return $form;
}

/**
 * Handle cache reset.
 */
function apc_admin_reset_cache($form, &$form_state) {
  apc_clear_cache();
  apc_clear_cache('user');
  apc_clear_cache('opcode');
  drupal_set_message(t('APC Cache cleared!'));
}

/**
 * Handle cache reset.
 */
function apc_admin_generate_secret_key($form, &$form_state) {
  // Generate and save our short secret key.
  global $drupal_hash_salt;
  $secret_key = substr(md5($drupal_hash_salt . time()), 1, 12);
  variable_set('apc_admin_secret_key', $secret_key);
  drupal_set_message(t('Secret key generated. Your secret key is @secret_key', array('@secret_key' => $secret_key)));
}
