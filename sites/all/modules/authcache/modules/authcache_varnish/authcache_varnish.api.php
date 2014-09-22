<?php
/**
 * @file
 * API documentation for the Authcache Varnish module.
 */

/**
 * Verify that the current request is comming in through varnish.
 *
 * Use this hook to implement additional rules in order to determine whether
 * the current request really came in through the expected varnish instance.
 *
 * @return Boolean
 *   FALSE if the request did not pass through the expected varnish instance.
 */
function hook_authcache_varnish_request_validate() {
  if (!isset($_SERVER['HTTP_X_VARNISH_PARANOIA_INSTANCE_KEY'])) {
    return FALSE;
  }
  if ($_SERVER['HTTP_X_VARNISH_PARANOIA_INSTANCE_KEY'] != 'correct horse battery staple') {
    return FALSE;
  }
}
