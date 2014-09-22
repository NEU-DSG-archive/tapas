<?php
// $Id: regcode.api.php,v 1.1.4.4 2011/01/08 17:12:45 aidan Exp $

/** 
 * @file
 * Example hook functions for hooks provided by the core regcode module
 */


/**
 * Called when a registration code is used
 */ 
function hook_regcode_used(&$edit, &$account, &$code) {
  if (is_object($code)) {
    drupal_set_message(t('Thanks %name, the code %code was used.', array(
      '%name' => $account->name,
      '%code' => $edit['regcode_code'])));
  }
}


/**
 * Called when validating registration code use
 */
function hook_regcode_validate($edit, $account) {
  if (empty($edit['loves_bacon'])) {
    form_set_error('regcode_code', t('This just isn\'t working out between us.'));
  }
}


/**
 * Called when a registration code is loaded
 */
function hook_regcode_load(&$code) {
  $code->group = 'foo';
}


/**
 * Called to gather field information
 */
function hook_regcode_fields() {
  return array(
    'group' => array(
      'description' => t('The group this code should be assigned to.'),
      'title'       => t('Group'),
    ),
  );
}
