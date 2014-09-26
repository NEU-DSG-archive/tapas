<?php
/**
 * @file
 * Defines a personalization fragment for retrieving the number of new comments
 * on a node.
 */

/**
 * Personalization fragment for retrieving the number of new comments on a node.
 *
 * @see node.module
 */
class AuthcacheForumIntervalFragment implements AuthcacheP13nFragmentInterface, AuthcacheP13nFragmentLoaderInterface {
  /**
   * Determine number of new comments on each topic.
   */
  public function load($keys, $context) {
    include_once DRUPAL_ROOT . '/includes/common.inc';
    return $keys;
  }

  /**
   * Number of new topic replies for user or topic is unread.
   *
   * @see forum.module
   */
  public function render($key, $timestamp, $context) {
    return format_interval(REQUEST_TIME - $timestamp);
  }
}
