<?php

/**
 * Display a list of available routes and frontcontroller settings.
 */
function authcache_p13n_admin_routes($form, &$form_state) {
  $router = authcache_p13n_request_get_router();
  $resources = authcache_p13n_request_resources();

  $form['help'] = array(
    '#title' => t('Frontcontroller'),
    '#type' => 'fieldset',
  );
  $form['help']['description'] = array(
    '#markup' =>
    '<p>' . t('The personalization frontcontroller is responsible for dispatching Ajax / ESI requests to an appropriate handler class and serving fragments to the browser or to a reverse caching proxy server.') . '</p>' .
    '<p>' . t('The table below lists all defined routes. The second table shows important variables and their values. Normally those settings need not to be changed. If this is nontheless necessary, customization may be done through <code>settings.php</code> file.') . '</p>',
  );

  $variables = array(
    'authcache_p13n_frontcontroller_path' => array(
      'title' => t('Front controller path'),
      'description' => t('The path to the frontcontroller script used to serve personalized content. By copying the safe frontcontroller script into the drupal root directory, it is possible to reduce the URL length. See the README.txt for more information.'),
      'value' => variable_get('authcache_p13n_frontcontroller_path', drupal_get_path('module', 'authcache_p13n') . '/frontcontroller/index.php'),
    ),
    'authcache_p13n_checkheader' => array(
      'title' => t('Check header flag'),
      'description' => t('Specifies whether the X-Authcache header should be examined on incomming requests to the personalization frontcontroller in order to prevent XSS attacks'),
      'value' => variable_get('authcache_p13n_checkheader', TRUE) ? t('Yes') : t('No'),
    ),
    'authcache_p13n_router' => array(
      'title' => t('Router class'),
      'description' => t('The class used to dispatch incomming personalization requests to appropriate handler objects.'),
      'value' => variable_get('authcache_p13n_router', 'AuthcacheP13nDefaultRequestRouter'),
    ),
    'authcache_p13n_default_request_router_autorebuild' => array(
      'title' => t('Autorebuild router'),
      'description' => t('Specifies whether the request router should try to rebuild the routing information when a request handler is missing'),
      'value' => variable_get('authcache_p13n_default_request_router_autorebuild', TRUE) ? t('Yes') : t('No'),
    ),
  );

  $header = array(t('Route ID'), t('Request handler'), t('URL generator'), t('Route exists'), '');
  $rows = array();
  foreach ($resources as $route_id => $resource) {
    $rows[] = array(
      $route_id,
      $resource['handler']['#class'],
      $resource['url generator']['#class'],
      $router->exists($route_id) ? t('Yes') : t('No'),
      l(t('Details'), 'admin/config/system/authcache/p13n/frontcontroller/route/' . $route_id),
    );
  }

  $form['routes'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  $header = array(t('Variable'), t('Value and description'));
  $rows = array();
  foreach ($variables as $varname => $record) {
    $rows[] = array(
      array(
        'valign' => 'top',
        'rowspan' => 2,
        'data' => '<strong>' . $record['title'] . '</strong>' . ' (' . check_plain($varname) . ')',
      ),
      $record['value'],
    );
    $rows[] = array(array(
      'data' => $record['description'],
    ));
  }

  $form['variables'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['rebuild'] = array(
    '#type' => 'submit',
    '#value' => t('Rebuild routes'),
  );

  return $form;
}

/**
 * Form API submit callback.
 *
 * @see authcache_p13n_admin_routes()
 */
function authcache_p13n_admin_routes_submit() {
  authcache_p13n_request_router_rebuild();
  drupal_set_message(t('Rebuilt router'));
}

/**
 * Display details for a given route.
 */
function authcache_p13n_admin_route_page() {
  $route_id = implode('/', func_get_args());
  $resources = authcache_p13n_request_resources();

  drupal_set_title(t('Definition for route @route_id', array('@route_id' => $route_id)));

  require_once(DRUPAL_ROOT . '/includes/utility.inc');
  $page['definition'] = array(
    '#markup' => '<code><pre>' . drupal_var_export($resources[$route_id]) . '</code></pre>',
  );

  return $page;
}

/**
 * Display a list of markup substitution configurations.
 */
function authcache_p13n_admin_markup_configs($form, &$form_state) {
  $configs = module_invoke_all('authcache_p13n_admin_config');

  $groups = array();
  foreach ($configs as $config) {
    $groups[$config['group']][] = $config;
  }

  $header = array(t('Name'), t('Location'), t('Client'), t('Cache lifetime'), t('Cache granularity'), t('Route ID'), '');
  foreach ($groups as $groupname => $group) {
    $rows[] = array(
      array(
        'class' => array('p13n-markup-substitution-group'),
        'colspan' => 7,
        'data' => $groupname,
      ),
    );
    foreach ($group as $config) {
      if (isset($config['message'])) {
        $rows[] = array(
          array(
            'class' => array('p13n-markup-substitution-message'),
            'colspan' => 7,
            'data' => '<em>' . $config['message'] . '</em>',
          ),
        );
      }
      else {
        $options = $config['config'];

        $location = !empty($config['location']) ? $config['location'] : '';
        $client_names = _authcache_p13n_admin_format_clients($options['clients']);
        $lifespan = !empty($options['lifespan']) ? format_interval($options['lifespan']) : '';
        $granularity = _authcache_p13n_admin_cache_granularity($options);
        $route_id = _authcache_p13n_admin_format_route_id($config);
        $edit_link = !empty($config['path']) ? l(t('Edit'), $config['path'], array('query' => array('destination' => $_GET['q']))) : '';

        $rows[] = array(
          array(
            'class' => array('p13n-markup-substitution-title'),
            'data' => $config['title'],
          ),
          $location,
          implode(', ', $client_names),
          $lifespan,
          implode(', ', $granularity),
          $route_id,
          $edit_link,
        );
      }
    }
  }

  $page['configs'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'authcache_p13n') . '/authcache_p13n.admin.css',
      ),
    ),
  );

  return $page;
}

function _authcache_p13n_admin_format_clients($config_clients) {
  $client_info = authcache_p13n_client_info();
  uasort($config_clients, 'drupal_sort_weight');

  $client_names = array();
  foreach ($config_clients as $key => $config_client) {
    if (!empty($client_info[$key]['title']) && (!isset($config_client['status']) || !empty($config_client['status']))) {
      $client_names[] = $client_info[$key]['title'];
    }
  }

  return $client_names;
}

function _authcache_p13n_admin_cache_granularity($options) {
  $granularity = array();

  if (!empty($options['peruser'])) {
    $granularity[] = t('Per user');
  }

  if (!empty($options['perpage'])) {
    $granularity[] = t('Per page');
  }

  return $granularity;
}

function _authcache_p13n_admin_format_route_id($config) {
  $route_id = '';

  $prefixmap = array(
    'fragment' => 'frag/',
    'setting' => 'setting/',
    'assembly' => 'asm/',
  );

  if (!empty($config['type']) && isset($prefixmap[$config['type']]) && !empty($config['id'])) {
    $route_id = $prefixmap[$config['type']] . $config['id'];
  }

  return $route_id;
}
