<?php
// $Id: regcode.rules.inc,v 1.1.4.4 2010/03/12 19:41:48 aidan Exp $


/**
 * @file
 * Rules integration for the regcode module.
 */


/**
 * Implementation of module_rules_event_info()
 *
 * @ingroup rules
 */
function regcode_rules_event_info() {
  return array(
    'regcode_used' => array(
      'label' => t('User has used a registration code'),
      'module' => 'Regcode',
      'arguments' => array(
        'user' => array(
          'type' => 'user',
          'label' => t('The user using the code'),
        ),
        'regcode' => array(
          'type' => 'regcode',
          'label' => t('The regcode which was used'),
        ),
      ),
    ),
  );
}


/**
 * Implement module_rules_condition_info()
 *
 * @ingroup rules
 */
function regcode_rules_condition_info() {
  return array(
    'rules_condition_regcode_term_compare' => array(
      'label' => t('Tagged with'),
      'arguments' => array(
        'regcode' => array(
          'type' => 'regcode',
          'label' => t('Tag'),
        ),
      ),
      'module' => 'Regcode',
    ),
    'rules_condition_regcode_code_compare' => array(
      'label' => t('Code comparison'),
      'arguments' => array(
        'regcode' => array(
          'type' => 'regcode',
          'label' => t('Code'),
        ),
      ),
      'module' => 'Regcode',
    ),
  );
}


/**
 * Comparison function
 *
 * @ingroup rules
 */
function rules_condition_regcode_term_compare($regcode, $settings) {
  $present  = array_keys($regcode->tags);
  $expected = array_keys($settings['regcode_terms']);
  $match    = (bool) count(array_intersect($present, $expected));
  return $match;
}


/**
 * Form to select terms to match against
 *
 * @param array $settings
 * @param array $form
 * @ingroup rules
 */
function rules_condition_regcode_term_compare_form($settings, &$form) {
  $form['settings']['regcode_terms'] = array(
    '#type' => 'checkboxes',
    '#options' => regcode_get_vocab_terms(),
    '#title' => t('Tags'),
    '#required' => TRUE,
    '#description' => t('Condition evaluates to TRUE if registration code used was tagged with any of the selected terms.'),
    '#default_value' => (array) $settings['regcode_terms'],
  );
}


/**
 * Compare a submitted form
 *
 * @ingroup rules
 */
function rules_condition_regcode_term_compare_submit(&$settings, $form, $form_state) {
  $settings['regcode_terms'] = array_filter($settings['regcode_terms']);
}


/**
 * Comparison function
 *
 * @ingroup rules
 */
function rules_condition_regcode_code_compare($regcode, $settings) {
  return $regcode->code === $settings['regcode_code'];
}


/**
 * Form to select terms to match against
 *
 * @param array $settings
 * @param array $form
 * @ingroup rules
 */
function rules_condition_regcode_code_compare_form($settings, &$form) {
  $form['settings']['regcode_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Registration code'),
    '#required' => TRUE,
    '#description' => t('Condition evaluates to TRUE if registration code used matches this string exactly.'),
    '#default_value' => $settings['regcode_code'],
  );
}
