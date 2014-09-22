<?php
/**
 * @file
 * Defines the interface for mapping URLs to requests and vice versa.
 */

/**
 * Interface for mapping URLs to requests and vice versa.
 */
class AuthcacheP13nDefaultRequestRouter implements AuthcacheP13nRequestRouterInterface {
  /**
   * Object type for request handlers.
   */
  const HANDLER_TYPE = 'handler';

  /**
   * Object type for request url generators.
   */
  const URL_GENERATOR_TYPE = 'url generator';

  /**
   * Map of short type name constants to interfaces.
   */
  protected $typemap;

  /**
   * Default constructor for default request router.
   */
  public function __construct() {
    $this->typemap = array(
      AuthcacheP13nDefaultRequestRouter::HANDLER_TYPE => 'AuthcacheP13nRequestHandlerInterface',
      AuthcacheP13nDefaultRequestRouter::URL_GENERATOR_TYPE => 'AuthcacheP13nRequestUrlGeneratorInterface',
    );
  }

  /**
   * {@inheritdoc}
   */
  public function exists($route_id) {
    $existing_routes = variable_get('authcache_p13n_default_request_router_routes', array());
    return isset($existing_routes[$route_id]);
  }

  /**
   * {@inheritdoc}
   */
  public function getHandler($route_id) {
    $handler = $this->lookup(AuthcacheP13nDefaultRequestRouter::HANDLER_TYPE, $route_id);

    if (!$handler) {
      watchdog('AuthcacheP13nDefaultRequestRouter', 'Failed to find request handler for route %r', array('%r' => $route_id), WATCHDOG_ERROR);
    }

    return $handler;
  }

  /**
   * {@inheritdoc}
   */
  public function generateURL($route_id, $arg = NULL) {
    $generator = $this->lookup(AuthcacheP13nDefaultRequestRouter::URL_GENERATOR_TYPE, $route_id);

    if ($generator) {
      return $generator->url($route_id, $arg);
    }
    else {
      watchdog('AuthcacheP13nDefaultRequestRouter', 'Failed to find url generator for route %r', array('%r' => $route_id), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  /**
   * {@inheritdoc}
   */
  public function rebuild() {
    // Collect all request definitions.
    $resources = authcache_p13n_request_resources();
    $processors = authcache_p13n_resource_processors();
    $handlers = array();
    $generators = array();

    foreach ($resources as $route_id => $resource) {
      try {
        $factory = new AuthcacheP13nObjectFactory($resource, $processors);

        $handler = $factory->get('handler');
        $generator = $factory->get('url generator');

        cache_set($this->requestCid(AuthcacheP13nDefaultRequestRouter::HANDLER_TYPE, $route_id), $handler, 'cache_authcache_p13n');
        cache_set($this->requestCid(AuthcacheP13nDefaultRequestRouter::URL_GENERATOR_TYPE, $route_id), $generator, 'cache_authcache_p13n');

        $handlers[$route_id] = $handler;
        $generators[$route_id] = $generator;
      }
      catch (Exception $e) {
        watchdog_exception('AuthcacheP13nDefaultRequestRouter', $e);

        // Ensure that no route will be created for this request.
        unset ($resources[$route_id]);
      }
    }

    variable_set('authcache_p13n_default_request_router_routes', drupal_map_assoc(array_keys($handlers)));

    return array(
      AuthcacheP13nDefaultRequestRouter::HANDLER_TYPE => $handlers,
      AuthcacheP13nDefaultRequestRouter::URL_GENERATOR_TYPE => $generators,
    );
  }

  /**
   * Return a handler or generator appropriate for the given route.
   */
  protected function lookup($type, $route_id) {
    $result = FALSE;

    if ($this->exists($route_id) && isset($this->typemap[$type])) {
      $cache = cache_get($this->requestCid($type, $route_id), 'cache_authcache_p13n');
      if ($cache) {
        if (is_a($cache->data, $this->typemap[$type])) {
          $result = $cache->data;
        }
      }
      elseif (variable_get('authcache_p13n_default_request_router_autorebuild', TRUE)) {
        // Make sure we're fully bootstrapped.
        drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

        // Rebuild the router and find the handler/generator for the given
        // route.
        $registry = $this->rebuild();
        if (!empty($registry[$type][$route_id])) {
          $result = $registry[$type][$route_id];
        }
      }
    }

    return $result;
  }

  /**
   * Return the cache id for a given route_id.
   */
  protected function requestCid($type, $route_id) {
    return 'DefaultRequestRouter/' . $type . '/' . $route_id;
  }
}
