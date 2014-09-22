<?php
/**
 * @file
 * Defines AuthcacheP13nSettingBuilder
 */

/**
 * Content builder for personalized Drupal.settings.
 */
class AuthcacheP13nSettingBuilder implements AuthcacheP13nContentBuilderInterface, AuthcacheP13nRequestValidatorInterface {
  /**
   * @var AuthcacheP13nSetting
   */
  protected $setting;

  /**
   * @var string
   */
  protected $target;

  /**
   * @var AuthcacheP13nSettingValidator
   */
  protected $validator;

  /**
   * @var AuthcacheP13nSettingAccess
   */
  protected $access;

  /**
   * Construct new instance.
   *
   * @param AuthcacheP13nSetting $setting
   *   A setting instance.
   * @param AuthcacheP13nSettingValidator $validator
   *   (optional) A setting validator instance.
   * @param AuthcacheP13nSettingAccess $access
   *   (optional) A setting access checker instance.
   * @param string $target
   *   The name of the target attribute on Drupal.settings
   */
  public function __construct($setting, $validator, $access, $target) {
    $this->setting = $setting;
    $this->validator = $validator;
    $this->access = $access;
    $this->target = $target;
  }

  /**
   * {@inheritdoc}
   */
  public function validate($req) {
    // Normalize request parameter.
    $req['a'] = empty($req['a']) ? NULL : $req['a'];

    // Run validator.
    if ($this->validator) {
      $req['a'] = $this->validator->validate($req['a']);
    }

    return $req;
  }

  /**
   * {@inheritdoc}
   */
  public function build($req, $context) {
    global $user;

    $params = $req['a'];

    // Run access check.
    if ($this->access && !$this->access->check($user, $params, $context)) {
      throw new AuthcacheP13nRequestAccessDenied();
    }
    else {
      return array($this->target => $this->setting->get($params, $context));
    }
  }
}
