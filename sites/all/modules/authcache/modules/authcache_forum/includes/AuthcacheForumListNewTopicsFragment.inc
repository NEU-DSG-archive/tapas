<?php
/**
 * @file
 * Defines a personalization fragment for retrieving new-flags on a list of
 * forum topics.
 */

/*
 * Fragment for retrieving new-flags on a list of forum topics.
 */
class AuthcacheForumListNewTopicsFragment implements AuthcacheP13nFragmentInterface, AuthcacheP13nFragmentLoaderInterface {
  /**
   * Return a list of unread forum topics for the current user.
   */
  public function load($keys, $context) {
    global $user;
    drupal_language_initialize();
    include_once DRUPAL_ROOT . '/includes/common.inc';
    include_once DRUPAL_ROOT . '/' . variable_get('path_inc', 'includes/path.inc');
    include_once DRUPAL_ROOT . '/modules/field/field.module';
    // Need NODE_NEW_LIMIT definition.
    include_once DRUPAL_ROOT . '/modules/node/node.module';
    include_once DRUPAL_ROOT . '/modules/forum/forum.module';
    // XSS filter for l().
    include_once DRUPAL_ROOT . '/modules/filter/filter.module';

    $result = array();

    foreach ($keys as $tid) {
      $result[$tid] = (int) _forum_topics_unread($tid, $user->uid);
    }

    return $result;
  }

  /**
   * Number of new forum topics for user.
   *
   * @see forum.module
   */
  public function render($tid, $new_topics, $context) {
    return $new_topics ? l(format_plural($new_topics, '1 new', '@count new'), "forum/$tid", array('fragment' => 'new')) : '';
  }
}
