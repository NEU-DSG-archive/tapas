<?php

/**
 * @file
 * Front controller for user specific content fragments.
 */

// Detect root directory of the Drupal installation.
$__file__ = __FILE__;
if ($__file__[0] != '/') {
  $__file__ = str_replace(DIRECTORY_SEPARATOR, '/', $__file__);
}
$drupalroot = preg_replace('#(/(sites|profiles)/([^/]+))?/modules(/.*)?/authcache_p13n/frontcontroller/index.php$#', '', $__file__);

if ($drupalroot == $__file__) {
  trigger_error('Authcache P13n front controller: failed to locate Drupal root directory', E_USER_ERROR);
  exit();
}

// Relocate contents of $_SERVER variabl to DRUPAL_ROOT by removing path
// componints pointing to the authcache front controller.
$preg = '|' . preg_quote(substr($__file__, strlen($drupalroot) + 1), '|') . '|';
$_SERVER = preg_replace($preg, 'index.php', $_SERVER);

// Change working directory and define the essential DRUPAL_ROOT constant.
chdir($drupalroot);
define('DRUPAL_ROOT', $drupalroot);
define('AUTHCACHE_P13N_ROOT', dirname(dirname($__file__)));

// Load settings.php
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
require_once AUTHCACHE_P13N_ROOT . '/includes/frontcontroller.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_SESSION);
authcache_p13n_frontcontroller_handle_request();
