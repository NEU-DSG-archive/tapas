<?php
/**
 * @file
 * Authcache personalized fragment for build-ids of cached forms.
 */

/**
 * Generate new per-user form-cache entry by copying them from prototypes.
 *
 * @see form.inc
 */
class AuthcacheFormBuildIdFragment implements AuthcacheP13nFragmentInterface {
  /**
   * Render hidden field for form build-id.
   */
  public function render($form_build_id, $ignored, $context) {
    include_once DRUPAL_ROOT . '/includes/common.inc';
    include_once DRUPAL_ROOT . '/includes/form.inc';

    // Retrieve prototype from form cache. Authcache prototype forms are
    // prefixed with "form_authcache-".
    if ($cached = cache_get('form_authcache-' . $form_build_id, 'cache_form')) {
      $form = $cached->data;

      if ($cached = cache_get('form_state_authcache-' . $form_build_id, 'cache_form')) {
        $form_state = $cached->data;

        $new_build_id = 'form-' . drupal_hash_base64(uniqid(mt_rand(), TRUE) . mt_rand());
        $form['#build_id'] = $new_build_id;
        $form['form_build_id']['#value'] = $new_build_id;
        $form['form_build_id']['#id'] = $new_build_id;

        // Store new form for the current user in the form cache. Note that
        // form_set_cache will replace the per-user #cache_token before
        // storing it.
        form_set_cache($new_build_id, $form, $form_state);

        // Write the hidden build-id element.
        return format_string('<input type="hidden" name="form_build_id" value="!new_build_id"></input>', array('!new_build_id' => $new_build_id));
      }
    }
  }
}
