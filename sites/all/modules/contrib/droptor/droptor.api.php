<?php

/**
 * @file
 * Hooks provided by the Droptor module.
 *
 * Checklists not appearing in Droptor.com?
 *  1. Did you define 'checked' in your checklist item array, or do you have a 
 *     function defined with the same name as the array key?
 *  2. Did you clear the cache?
 *  3. Have you synced your site in Droptor.com since you added these checklist
 *     items?
 *  Still stuck? Get help: www.droptor.com/support
 */

/**
 * Define one or more custom checklists to appear in your Droptor account. You
 * place these hooks in example.module or example.droptor.inc.
 *
 * @return
 *  An array of custom checklists whose keys are unique internal machine names
 *  for each custom checklist you want to add. Each key should be in the format 
 *  yourmodulename-droptor-function-name. Example: For a module called "glue"
 *  that defines a custom checklist that checks whether it's beer time, your
 *  array key would be glue-droptor-is-it-beer-time. Keys should be alphanumeric
 *  and dashes only.
 *
 *  Each checklist item is an array with the following keys:
 *  'checked description'   => Required. The text that should appear in Droptor
 *                             when the checklist item is met (true)
 *  'unchecled decription'  => Required. The text that should appear in Droptor
 *                             when the checklist item is not met (false)
 *  'fix link'              => Optional. When the checklist item is not true,
 *                             Droptor will include this URL as a link in the
 *                             dashboard.
 *  'chceked'               => Optional. True of false to indicate whether the
 *                             checklist item is met or not. If not provided
 *                             then then the Droptor module will look for a 
 *                             function in your module with the same name as
 *                             the array key for this checklist item, with
 *                             dashes converted to underscores.
 */
function hook_droptor_checklists() {
  return array(
    // This item defines "checked", so Droptor won't look for a function. 
    'mymodule-droptor-live-paypal-api' => array(
      'checked description' => t('Site is using the live Paypal API.'),
      'unchecked description' => t('Site is not using the live Paypal API.'),
      'fix link' => url('admin/config/zujava/settings', array('absolute' => TRUE)),
      'checked' => variable_get('some_rando_var', FALSE),
    ),

    // Since no array key 'checked' is defined, Droptor will look for a
    // function called "mymodule_droptor_recent_node_edit", as that is the value
    // of the array key, with dashes converted to underscores.
    'mymodule-droptor-recent-node-edit' => array(
      'checked description' => t('Nodes have been edited in the last 24 hours.'),
      'unchecked description' => t('Nodes are not getting updated!'),
      'fix link' => url('admin/config/zujava/settings', array('absolute' => TRUE)),
    ),
  );
}

/**
 * Callback for custom Droptor checklist item to determine the "checked" value. 
 * The function name should match the array key of the checklist
 * item it defined, but with underscores instead of dashes. For example, the
 * array key for this checklist item is "mymodule-droptor-recent-node-edit" 
 * so the Droptor module will look for a function called 
 * "mymodule_droptor_recent_node_edit".
 *
 * @return
 *  True (check the checklist item) or false (don't check the item).
 */
function mymodule_droptor_recent_node_edit() {
  $latest_edit = db_query('SELECT max(updated) FROM {node}')->fetchField();
  return (time() - $oldest_date) > (60*60*24);
}
