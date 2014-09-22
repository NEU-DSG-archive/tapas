<?php
/**
 * @file
 * API documentation for hooks exposed by Authcache Debug.
 */

/**
 * Collect information displayed in the authcache debug widget.
 *
 * Return an array of key-value pairs which will be displayed in the debug
 * widget. This information is stored for each cached page request into the
 * cache bin cache_authcache_debug.
 *
 * @return array
 *   An array of key-value pairs which should be shown in the debug widget.
 */
function hook_authcache_debug_info() {
  global $user;

  return array(
    'Cache User E-Mail' => $user->uid ? $user->mail : 'anonymous',
  );
}

/**
 * Alter debug information before its being stored.
 *
 * @see hook_authcache_debug_info()
 */
function hook_authcache_debug_info_alter(&$debuginfo) {
  // Remove potentially sensitive data form debug info widget.
  unset($debuginfo['Cache User']);
  unset($debuginfo['Cache Users']);
}
