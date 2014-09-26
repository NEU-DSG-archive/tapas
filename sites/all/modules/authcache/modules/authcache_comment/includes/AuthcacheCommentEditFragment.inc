<?php
/**
 * @file
 * Defines a personalization fragment for retrieving edit links of comments.
 * on a node.
 */

/**
 * Personalization fragment for retrieving the edit link of comments.
 *
 * @see node.module
 */
class AuthcacheCommentEditFragment implements AuthcacheP13nFragmentInterface, AuthcacheP13nFragmentValidatorInterface {
  /**
   * Parse keys into cid and uid.
   */
  public function validate($keys) {
    $result = array();

    foreach (array_keys($keys) as $key) {
      list($cid, $uid) = explode('-', $key, 2);
      $cid = (int) $cid;
      $uid = (int) $uid;

      if (empty($cid) || empty($uid)) {
        authcache_p13n_validation_error('Invalid cid or uid');
      }

      $result[$key] = array($cid, $uid);
    }

    return $result;
  }

  /**
   * Render one new marker.
   */
  public function render($key, $params, $context) {
    global $user;
    list ($cid, $comment_uid) = $params;

    // Function l():
    drupal_language_initialize();
    include_once DRUPAL_ROOT . '/includes/common.inc';
    require_once DRUPAL_ROOT . '/' . variable_get('path_inc', 'includes/path.inc');
    // Function user_access():
    include_once DRUPAL_ROOT . '/modules/user/user.module';

    $comment_access = (($user->uid && $user->uid == $comment_uid && user_access('edit own comments')) || user_access('administer comments'));
    return $comment_access ? l(t('edit'), 'comment/' . $cid . '/edit') : '';
  }
}
