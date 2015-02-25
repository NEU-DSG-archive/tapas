<?php
/**
 * Simple caching of query results for Views displays, but per-user instead of
 * global for all users.
 */
class views_plugin_cache_time_per_user extends views_plugin_cache_time {

  function get_results_key() {
    global $user;

    if (!isset($this->_results_key)) {

      $build_info = $this->view->build_info;
      $query_plugin = $this->view->display_handler->get_plugin('query');

      foreach (array('query','count_query') as $index) {
        // If the default query back-end is used generate SQL query strings from
        // the query objects.
        if ($build_info[$index] instanceof SelectQueryInterface) {
          $query = clone $build_info[$index];
          $query->preExecute();
          $build_info[$index] = (string) $query;
        }
      }

      // Add a 'user' key so this result is unique per user.
      $key_data = array(
        'build_info' => $build_info,
        'roles' => array_keys($user->roles),
        'super-user' => $user->uid == 1, // special caching for super user.
        'user' => $user->uid,
        'timezone' => $user->timezone,
        'language' => $GLOBALS['language'],
        'base_url' => $GLOBALS['base_url'],
      );
      foreach (array('exposed_info', 'page', 'sort', 'order', 'items_per_page', 'offset') as $key) {
        if (isset($_GET[$key])) {
          $key_data[$key] = $_GET[$key];
        }
      }

      $this->_results_key = $this->view->name . ':' . $this->display->id . ':results:' . md5(serialize($key_data));

    }

    return $this->_results_key;
  }

  function get_output_key() {
    global $user;

    if (!isset($this->_output_key)) {
      // Add a 'user' key so this result is unique per user.
      $key_data = array(
        'result' => $this->view->result,
        'roles' => array_keys($user->roles),
        'super-user' => $user->uid == 1, // special caching for super user.
        'user' => $user->uid,
        'timezone' => $user->timezone,
        'theme' => $GLOBALS['theme'],
        'language' => $GLOBALS['language'],
        'base_url' => $GLOBALS['base_url'],
      );

      $this->_output_key = $this->view->name . ':' . $this->display->id . ':output:' . md5(serialize($key_data));
    }

    return $this->_output_key;
  }
}

