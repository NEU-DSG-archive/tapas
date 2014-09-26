<?php

class ModuleStub {
  protected $name;

  public function __construct($name) {
    $this->name = $name;
  }

  public function hook($hookname, $return = NULL) {
    return HookStub::on($this->name . '_' . $hookname, $return);
  }
}

class HookStubProxy {
  protected $hookname;

  public function __construct($hookname) {
    $this->hookname = $hookname;
  }

  public function on($return = NULL) {
    return HookStub::on($this->hookname, $return);
  }

  public function off() {
    return HookStub::off($this->hookname);
  }

  public function record($args) {
    return HookStub::record($this->hookname, $args);
  }

  public function verify($checkfunc, &$message) {
    return HookStub::verify($this->hookname, $checkfunc, $message);
  }

  public function hookname() {
    return $this->hookname;
  }

  public function invocations() {
    return HookStub::invocations($this->hookname);
  }
}

class HookStub {
  protected $hookname;
  protected $invocations;
  protected $return;

  public function __construct($hookname, $return = NULL) {
    $this->hookname = $hookname;
    $this->invocations = array();
    $this->return = $return;
  }

  public static function on($hookname, $return = NULL) {
    variable_set('hookstub_' . $hookname, new HookStub($hookname, $return));
    return new HookStubProxy($hookname);
  }

  public static function off($hookname) {
    variable_del('hookstub_' . $hookname);
  }

  public static function record($hookname, $args) {
    if (($stub = variable_get('hookstub_' . $hookname))) {
      $stub->invocations[] = $args;
      variable_set('hookstub_' . $hookname, $stub);
      return $stub->return;
    }
  }

  public static function invocations($hookname) {
    if (($stub = variable_get('hookstub_' . $hookname))) {
      return $stub->invocations;
    }
  }

  public static function verify($hookname, $checkfunc, &$message) {
    if (($stub = variable_get('hookstub_' . $hookname))) {
      if ($checkfunc == NULL) {
        $checkfunc = HookStub::once();
      }
      return $checkfunc($stub->invocations, $message);
    }
    $message .= ' ' . t('No information available.');
  }

  public static function never() {
    return static::times(0);
  }

  public static function once() {
    return static::times(1);
  }

  public static function times($times) {
    return function($invocations, &$message) use ($times) {
      $message = format_plural($times, 'Expected @times invocation but got @count.', 'Expected @times invocations but got @count.', array('@times' => $times, '@count' => count($invocations)));
      return count($invocations) == $times;
    };
  }

  public static function args($args, $invoc = 0) {
    return function($invocations, &$message) use ($args, $invoc) {
      $message = t('On invocation number @invoc, expected @this but got @that.', array('@invoc' => $invoc, '@this' => var_export($args, TRUE), '@that' => var_export($invocations[$invoc], TRUE)));
      return $invocations[$invoc] == $args;
    };
  }
}
