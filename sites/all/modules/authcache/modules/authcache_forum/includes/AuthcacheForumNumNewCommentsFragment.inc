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
class AuthcacheForumNumNewCommentsFragment implements AuthcacheP13nFragmentInterface, AuthcacheP13nFragmentLoaderInterface {
  /**
   * Determine number of new comments on each topic.
   */
  public function load($keys, $context) {
    drupal_language_initialize();
    include_once DRUPAL_ROOT . '/includes/common.inc';
    include_once DRUPAL_ROOT . '/' . variable_get('path_inc', 'includes/path.inc');
    include_once DRUPAL_ROOT . '/modules/field/field.module';
    // Need NODE_NEW_LIMIT definition.
    include_once DRUPAL_ROOT . '/modules/node/node.module';
    include_once DRUPAL_ROOT . '/modules/forum/forum.module';
    // XSS filter for l().
    include_once DRUPAL_ROOT . '/modules/filter/filter.module';
    include_once DRUPAL_ROOT . '/modules/comment/comment.module';

    $result = array();

    foreach ($keys as $nid) {
      $last_visit = _forum_user_last_visit($nid);
      $result[$nid] = (int) comment_num_new($nid, $last_visit);
    }

    return $result;
  }

  /**
   * Number of new topic replies for user or topic is unread.
   *
   * @see forum.module
   */
  public function render($nid, $new_comments, $context) {
    return $new_comments ? l(format_plural($new_comments, '1 new', '@count new'), "node/$nid", array('fragment' => 'new')) : '';
  }
}
