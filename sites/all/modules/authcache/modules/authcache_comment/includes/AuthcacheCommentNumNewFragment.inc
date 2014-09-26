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
class AuthcacheCommentNumNewFragment implements AuthcacheP13nFragmentInterface {
  /**
   * Number of new comments for given nids.
   */
  public function render($nid, $subject, $context) {
    include_once DRUPAL_ROOT . '/modules/node/node.module';
    include_once DRUPAL_ROOT . '/modules/comment/comment.module';

    return comment_num_new($nid);
  }
}
