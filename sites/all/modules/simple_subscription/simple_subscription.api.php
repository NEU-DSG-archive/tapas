<?php
// $Id$
/**
 * @file
 * Hooks provided by simple subscription.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Allow modules to do something with the simple subscription form results
 *
 * @param array $form_data
 *   An array with the form results. Currently only contains the submitted email address.
 */
function hook_simple_subscription($form_data) {

$email = $form_data['simple-subscription-submit-values']['email'];

//do something with this ...
}

/**
 * @} End of "addtogroup hooks".
 */