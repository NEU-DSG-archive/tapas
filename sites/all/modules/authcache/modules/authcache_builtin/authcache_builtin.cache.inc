<?php

/**
 * @file
 * Attempt to deliver a cached version of a page depending on the users role.
 *
 * This file gets included by _drupal_bootstrap_page_cache in bootstrap.inc
 * during the drupal bootstrap stage DRUPAL_BOOTSTRAP_PAGE_CACHE. This script
 * delegates the request to the underlying cache handler and attempts to deliver
 * a cached version of a page for this request.
 *
 * In the event of a cache-miss or if a page is not cachable, execution is
 * passed back to _drupal_bootstrap_page_cache and the page will be served
 * using a full bootstrap.
 *
 * @see _drupal_bootstrap_page_cache in boostrap.inc
 */

$delivered = authcache_builtin_cacheinc_retrieve_cache_page();
if ($delivered) {
  exit;
}

/**
 * Check whether client is allowed to skip the cache for this request.
 *
 * The default implementation allows cache bypass for all clients having a
 * session cookie.
 *
 * @see: _authcache_default_nocache_set()
 */
function _authcache_builtin_cacheinc_default_nocache_get() {
  if (isset($_COOKIE['nocache']) && isset($_COOKIE[session_name()])) {
    return TRUE;
  }
}

/**
 * Retrieve the authcache key for a session from the key-cache.
 */
function _authcache_builtin_cacheinc_cache_key_get($session_id) {
  $cache = cache_get($session_id, 'cache_authcache_key');

  if ($cache === FALSE || $cache->expire > 0 && $cache->expire < REQUEST_TIME) {
    // Need a full bootstrap when key was not found or entry expired.
    return FALSE;
  }
  else {
    return $cache->data;
  }
}

/**
 * Retrieve the authcache key for a session from a cookie.
 */
function _authcache_builtin_cacheinc_cookie_key_get($session_id) {
  if (!isset($_COOKIE['authcache'])) {
    return FALSE;
  }
  else {
    return $_COOKIE['authcache'];
  }
}

/**
 * Send cached page to browser, if found.
 *
 * @return bool
 *   TRUE if page was delivered, FALSE otherwise
 */
function authcache_builtin_cacheinc_retrieve_cache_page() {
  global $base_root;

  // Connect to database and initialize configuration if not disabled from
  // within settings.php
  if (!variable_get('authcache_builtin_cache_without_database')) {
    drupal_bootstrap(DRUPAL_BOOTSTRAP_VARIABLES, FALSE);
  }

  // The following three basic exclusion rules are mirrored in
  // authcache_authcache_request_exclude() in authcache.module
  // BEGIN: basic exclusion rules.
  if (drupal_is_cli()) {
    return FALSE;
  }

  // Only GET and HEAD requests allowed.
  if (!($_SERVER['REQUEST_METHOD'] == 'GET' || $_SERVER['REQUEST_METHOD'] == 'HEAD')) {
    return FALSE;
  }

  // Drupal core page caching for anonymous users active.
  if (variable_get('cache') || variable_get('page_cache_without_database')) {
    return FALSE;
  }

  // Not invoked using an allowed front controller.
  $frontscripts = variable_get('authcache_frontcontroller_whitelist',
    array(DRUPAL_ROOT . '/index.php'));
  $frontscripts = array_map('realpath', $frontscripts);
  if (!in_array(realpath($_SERVER['SCRIPT_FILENAME']), $frontscripts)) {
    return FALSE;
  }
  // END: basic exclusion rules.
  //
  // Determine whether caching is disabled for this request.
  $nocache_get = variable_get('authcache_builtin_nocache_get', '_authcache_builtin_cacheinc_default_nocache_get');
  if (is_callable($nocache_get) && call_user_func($nocache_get)) {
    return FALSE;
  }

  // Try to retrieve a key when the session cookie is present on the request.
  $key = NULL;
  if (isset($_COOKIE[session_name()])) {
    $session_id = $_COOKIE[session_name()];
    switch (variable_get('authcache_key_method', 'cache')) {
      case 'cache':
        $key = _authcache_builtin_cacheinc_cache_key_get($session_id);
        break;

      case 'cookie':
        $key = _authcache_builtin_cacheinc_cookie_key_get($session_id);
        break;

      default:
        $key_get = variable_get('authcache_builtin_key_get');
        if (!is_callable($key_get)) {
          return FALSE;
        }

        $key = call_user_func($key_get, $session_id);
        break;
    }

    // When key needs to be regenerated, return and trigger a full bootstrap.
    if ($key === FALSE) {
      return FALSE;
    }
  }

  // Fallback to anonymous content.
  if (!$key) {
    $generator = variable_get('authcache_key_generator');
    if (is_callable($generator)) {
      $key = call_user_func($generator);
    }
    else {
      $key = $base_root;
    }
  }

  // Attempt to retrieve page from cache.
  $cid = $key . request_uri();
  $cache = cache_get($cid, 'cache_page');

  if (empty($cache)) {
    header('X-Drupal-Cache: MISS');
    return FALSE;
  }
  else {
    // Render cache benchmark.
    if (isset($_COOKIE['cache_render'])) {
      setcookie('cache_render', timer_read('page'), 0, ini_get('session.cookie_path'), ini_get('session.cookie_domain'), ini_get('session.cookie_secure') == '1');
    }

    // Explicitely set Cache-Control header, otherwise max-age will be set to 0
    // by drupal_serve_page_from_cache.
    drupal_add_http_header('Cache-Control', 'public, max-age=' . variable_get('page_cache_maximum_age', 0));
    header('X-Drupal-Cache: HIT');
    drupal_serve_page_from_cache($cache);
    return TRUE;
  }
}
