<?php
/**
 * @file
 * Defines setting handler for contact default values
 */

/**
 * Setting handler for contact default values
 *
 * @see block.module
 */
class AuthcacheContactSetting implements AuthcacheP13nSettingInterface {
  /**
   * Return contact settings.
   */
  public function get($params, $context) {
    global $user;

    return array(
      'name' => $user->name,
      'mail' => $user->mail,
    );
  }
}
