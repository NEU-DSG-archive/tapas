<?php

/**
 * @file
 * Defines request URL generator.
 */

/**
 * Interface for request url generators.
 */
class AuthcacheP13nDefaultRequestUrlGenerator implements AuthcacheP13nRequestUrlGeneratorInterface {
  /**
   * Personalization fragment frontcontroller relative to DRUPAL_ROOT.
   */
  protected $frontControllerPath;

  /**
   * @var AuthcacheP13nCacheGranularity
   */
  protected $cacheGranularity;

  /**
   * Construct new request URL generator.
   *
   * @param String $front_controller
   *   Path to the personalization fragment frontcontroller relative to
   *   DRUPAL_ROOT.
   * @param int $cache_granularity
   *   Cache granularity.
   */
  public function __construct(
    $front_controller,
    AuthcacheP13nCacheGranularity $cache_granularity
  ) {
    $this->frontControllerPath = $front_controller;
    $this->cacheGranularity = $cache_granularity;
  }

  /**
   * {@inheritdoc}
   */
  public function url($route_id, $arg) {
    global $base_url;

    url('', array('prefix' => &$prefix));
    $query = array(
      'a' => $arg,
      'q' => empty($prefix) ? '' : $prefix,
      'r' => $route_id,
    );

    // Add path for per-page caching.
    if ($this->cacheGranularity->is(AuthcacheP13nCacheGranularity::PER_PAGE)) {
      $query['q'] .= $_GET['q'];
    }

    return url($base_url . '/' . $this->frontControllerPath, array(
      'absolute' => TRUE,
      'query' => $query,
    ));
  }
}
