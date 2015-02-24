<?php

/**
 * @file
 * A simple mechanism for rules to make http requests.
 */

/**
 * Implements hook_rules_action_info().
 */
function rules_http_client_rules_action_info() {
  $items = array();
  $items['request_url'] = array(
    'label' => t('Request HTTP data'),
    'base' => 'rules_http_client_request_url',
    'group' => t('Data'),
    'parameter' => array(
      'url' => array(
        'type' => 'uri',
        'label' => 'URL',
      ),
    ),
    'provides' => array(
      'http_response' => array(
        'type' => 'text',
        'label' => t('HTTP data'),
      ),
    ),
  );
  return $items;
}

/**
 * Performs the request action.
 */
function rules_http_client_request_url($url) {
  $response = drupal_http_request($url);
  if (isset($response->error)) {
    return;
  }
  else {
    return array('http_response' => $response->data);
  }
}
