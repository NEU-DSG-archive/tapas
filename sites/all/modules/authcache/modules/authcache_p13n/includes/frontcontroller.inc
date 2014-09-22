<?php
/**
 * @file
 * Defines the main function for the front controller.
 */

/**
 * Dispatch incomming requests to the appropriate request handler.
 */
function authcache_p13n_frontcontroller_handle_request() {
  // Front controller configuration.
  $check_header = variable_get('authcache_p13n_checkheader', TRUE);
  $routerclass = variable_get('authcache_p13n_router', 'AuthcacheP13nDefaultRequestRouter');

  ob_start();

  try {
    // This frontcontroller is only capable of responding to GET and HEAD
    // requests.
    if (!($_SERVER['REQUEST_METHOD'] == 'GET' || $_SERVER['REQUEST_METHOD'] == 'HEAD')) {
      drupal_add_http_header('Allow', 'GET, HEAD');
      throw new AuthcacheP13nRequestException('405 Method Not Allowed');
    }

    // Check whether the authcache HTTP header is present on the request.
    if ($check_header && empty($_SERVER['HTTP_X_AUTHCACHE'])) {
      watchdog('authcache_p13n', 'Front controller: X-Authcache header is missing on request', array(), WATCHDOG_ERROR);
      throw new AuthcacheP13nRequestInvalidInput();
    }

    // Check whether the client supplied a request-id.
    if (empty($_GET['r'])) {
      watchdog('authcache_p13n', 'Front controller: Missing r-parameter on request', array(), WATCHDOG_ERROR);
      throw new AuthcacheP13nRequestInvalidInput();
    }

    // Resolve and instantiate request handler.
    $router = new $routerclass();
    $handler = $router->getHandler($_GET['r']);
    unset($_GET['r']);
    if ($handler === FALSE) {
      throw new AuthcacheP13nRequestInvalidInput();
    }

    // Send response.
    $handler->handle($_GET);
    if ($_SERVER['REQUEST_METHOD'] == 'HEAD') {
      // Discard output if client does not care about it.
      ob_end_clean();
    }
  }
  catch(Exception $e) {
    $status = is_a($e, 'AuthcacheP13nRequestException') ? $e->getMessage() : '500 Internal Server Error';
    drupal_add_http_header('Status', $status);

    // Discard output.
    ob_end_clean();
  }
}
