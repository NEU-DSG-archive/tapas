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
class AuthcacheCommentNewFragment implements AuthcacheP13nFragmentInterface, AuthcacheP13nFragmentValidatorInterface, AuthcacheP13nFragmentLoaderInterface {
  /**
   * Parse keys into nid and timestamp.
   */
  public function validate($keys) {
    $result = array();

    foreach (array_keys($keys) as $key) {
      list($nid, $timestamp) = explode('-', $key, 2);
      $nid = (int) $nid;
      $timestamp = (int) $timestamp;

      if (empty($nid) || empty($timestamp)) {
        authcache_p13n_validation_error('Invalid nid or timestamp');
      }

      $result[$key] = array($nid, $timestamp);
    }

    return $result;
  }

  /**
   * Determine which comments need to be marked.
   */
  public function load($keys, $context) {
    global $user;

    $result = array();

    // Define MARK_X
    include_once './includes/theme.inc';
    // Function node_mark().
    include_once './modules/node/node.module';

    if (!empty($_SESSION['authcache_comment_node_last_viewed'])) {
      // After a page was cleaned from the cache, the first user requesting it
      // will trigger a node_tag_new and update its last viewed timestamp.
      // Therefore we store the timestamp of the last visit during page-build
      // in the session and retrieve it here and inject it into the static
      // cache of the node_last_viewed function.
      $history = &drupal_static('node_last_viewed', array());
      $history = $_SESSION['authcache_comment_node_last_viewed'];
    }
    unset($_SESSION['authcache_comment_node_last_viewed']);

    $nids = array();
    foreach ($keys as $key => $parsed) {
      list($nid, $timestamp) = $parsed;
      $result[$key] = node_mark($nid, $timestamp);
      $nids[$nid] = $nid;
    }

    if ($user->uid) {
      foreach ($nids as $nid) {
        // Update the 'last viewed' timestamp of the specified node for current
        // user. We do not want to use node_tag_view here because it requires a
        // node_load. The following code is directly copied from node_tag_view.
        db_merge('history')
          ->key(array(
            'uid' => $user->uid,
            'nid' => $nid,
          ))
          ->fields(array('timestamp' => REQUEST_TIME))
          ->execute();
      }
    }

    return $result;
  }

  /**
   * Render one new marker.
   */
  public function render($key, $new, $context) {
    return !empty($new) ? t('new') : '';
  }
}
