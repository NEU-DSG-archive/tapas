<?php

/**
 * @file
 * Defines request filter.
 */

/**
 * Interface for request url generators.
 */
interface AuthcacheP13nRequestUrlGeneratorInterface {
  /**
   * Generate an url for the given request and parameter.
   */
  public function url($route_id, $arg);
}
