<?php
/**
 * @file
 * Defines authcache personalized setting getter.
 */

/**
 * Interface for personalization setting getters.
 */
interface AuthcacheP13nSettingInterface {
  /**
   * Return a setting.
   *
   * @param var $params
   *   Params set by the client.
   *
   * @return string
   *   Return an associative array.
   */
  public function get($params, $context);
}
