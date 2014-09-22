<?php

/**
 * @file
 * Implementation of botcha form logic.
 */

define('BOTCHA_SECRET', variable_get('botcha_secret', '3288039533f40382398a85d52a8da366'));

// FIXME: Move all recipes into .recipes.inc
/**
 * Botcha recipe API
 *
 * @$recipe->name                Recipe name
 *   Names should be lowercase underscored, and start with class of recipe.
 *   Some classes are:
 *   - honeypot_ (traps that humans would not change, but bots would)
 *   - obscure_  (crete some obscure behavior that bots would not reproduce)
 *   - other classes can be defined as necessary
 * @$recipe->description         Brief description of the recipe
 * @$recipe->description_bots    (optional) Description of how bots would fall for it
 * @$recipe->description_how     (optional) What the recipe does
 * @$recipe->form_elements       (optional) Array in Form API format.
 *   Components named '!***' can be used for passing calculated values.
 *   They are discarded before adding to the form.
 * @$recipe->url_elements        (optional) Similar to form_elements, but for modifying the URL.
 * @$recipe->css                 (optional) CSS to add to the page
 * @$recipe->js                  (optional) Javascript to add to the page
 * @$recipe->error_field         Name of the field in the form to use in error messages (to mask botcha fields).
 *   FIXME: Use botcha_form_field($form, $names, $fallback) to filter correctness of the error_field
 * @$recipe->error_text          Text to give users if botcha recipe blocks submission.
 *   It should give some help to real human users in cases of disabled Javascript or CSS.
 *
 * END Botcha recipe API
 */

/**
 * Standardized Botcha error messages
 */
function _botcha_error_text() {
  // FIXME: make this configurable, i18n
  return t('You must be a human, not a spam bot, to submit forms on this website.') . ' ' .
    t('If you insist that you are a human, please try again.') . ' ' .
    t('If error persists, contact webmaster using contact link at the bottom of this page and give all the details of this error (your browser, version, OS).');
}
function _botcha_error_text_javascript() {
  return t('Please enable Javascript to use this form.');
}
function _botcha_error_text_errorcode($seed) {
  return t('Error #@err.', array('@err' => substr($seed, 1, 2) . substr($seed, 1, 1) . substr($seed, 2, 1) . substr($seed, 5, 2) . substr($seed, 6, 1) . substr($seed, 0, 1)));
}
function _botcha_error_noscript() {
  return '<noscript>' . t('Please enable Javascript to use this form.') . '</noscript>';
}
/**
 * Pick a field to obscure botcha and not give our honeypot away.
 * Furthermore, any validation errors on this field will remove our error.
 */
function _botcha_pick_patsy_field($form, $fields) {
  if (!is_array($fields)) {
    $fields = array($fields);
  }
  foreach ($fields as $field) {

    // FIXME: should check presence of this field...
    return $field;
  }
  //FIXME: If none of the suggested fields are present, return first visible field
}

/**
 * Helper function - build a url (allows full URI in $url)
 */
function _botcha_url($url, $options = array()) {
  global $base_url;
  $path = '';
  $query = '';
  $abs_base = $base_url . '/';
  $absolute = (0 === strpos($url, $abs_base)); // Figure out if absolute url is given to keep it that way.
  $base = $absolute ? $abs_base_url : base_path();
  $url = (0 === strpos($url, $base)) ? substr($url, strlen($base)) : ltrim($url, '/');
  // convert to local variables:
  // $scheme $host $port $user $pass $path $query $fragment
  extract(parse_url(urldecode($url)));

  // For non-clean URLs we need to convert query to array
  if (!empty($query) && !is_array($query)) {
    $params = explode('&', $query);
    $query = array();
    foreach ($params as $param) {
      $param = explode('=', $param, 2);
      $query[$param[0]] = isset($param[1]) ? rawurldecode($param[1]) : '';
    }
  }
  foreach ($options as $key => $value) {
    ${$key} = (is_array($value) && is_array(${$key}))
      ? array_merge(${$key}, $value)
      : $value;
  }

//  $result = url($path, array('query' => $query, 'fragment' => $fragment, 'absolute' => $absolute));
  // Unfortunately, url() messes up when $path has language prefix already.
  // Here we reproduce a part of url() to do the job right
  $fragment = !empty($fragment) ? '#' . $fragment : '';

  if (is_array($query)) {
    $query = drupal_http_build_query($query);
  }
  $prefix = !empty($prefix) ? $prefix : '';
  $prefix = empty($path) ? rtrim($prefix, '/') : $prefix;
  $path = drupal_encode_path($prefix . $path);
  if (variable_get('clean_url', '0')) {
    // With Clean URLs.
    $result = !empty($query) ? ($base . $path . '?' . $query . $fragment) : $base . $path . $fragment;
  }
  else {
    // Without Clean URLs.
    $variables = array();
    if (!empty($path)) {
      $variables[] = 'q=' . $path;
    }
    if (!empty($query)) {
      $variables[] = $query;
    }
    $query = join('&', $variables);
    if (!empty($query)) {
      static $script;
      if (!isset($script)) {
        // On some web servers, such as IIS, we can't omit "index.php". So, we
        // generate "index.php?q=foo" instead of "?q=foo" on anything that is not
        // Apache.
        $script = (strpos($_SERVER['SERVER_SOFTWARE'], 'Apache') === FALSE) ? 'index.php' : '';
      }
      $result = $base . $script . '?' . $query . $fragment;
    }
    else {
      $result = $base . $fragment;
    }
  }
  return $result;
}

/**
 * Botcha recipe.
 * Simple honeypot, no randomization, for test purposes.
 */
function _botcha_recipe_test1($form, $secret, $error_field) {
  $myseed = 'itr_r2' . substr($secret, 0, 7);
  $spf = md5($myseed . substr('secret', 0, -4));
  $field_class = 'botcha_field';
  $field_name = 'botcha_response';
  $secure_token = 'Test 123';
  $field_dflt = 'Test 123'; // What server sends in the field.

  $recipe = new stdClass;
  $recipe->name = 'test_honeypot_field';
  $recipe->description = t('Insert CSS+honeypot field');
  $recipe->description_bots = t('Bots will will mess with the field value');
  $recipe->description_how =
    t('%parts is added to the form.', array('%parts' => t('Honeypot field') . ',CSS'))
    . ' ' . t('CSS hides the input field.');
  $recipe->error_field = $error_field;
  $recipe->error_text =
    _botcha_error_text_errorcode($spf) . '<li>' .
    _botcha_error_text();

  $recipe->form_elements = array(
    $field_name => array(
      '#type' => 'textfield',
      '#title' => t('Enter your name'), // Leave the bot enough hints for it to guess it is a good name field
      '#default_value' => $field_dflt, // Store part of secure_token
      '#description' => t('Your first name.'),
      // This is for human users without CSS.
      '#prefix' => '<div class="' . $field_class . '">'
          // [#1004218] Addition for screen readers. Apparently, they don't hande CSS display:none.
          . '<span class="description"> (' . t('If you\'re a human, don\'t change the following field') . ')</span>',
      '#suffix' => '</div>',
      '#attributes' => array('class' => array($field_class), 'autocomplete' => 'off'),
      '#weight' => -20,
      '!valid_token' => $secure_token,
    ),
  );

  $recipe->css = 'div.' . $field_class . ' { display: none; visibility: hidden; }';

  return $recipe;
}

/**
 * Botcha recipe.
 */
function _botcha_recipe1($form, $secret, $error_field) {
  $myseed = 'itr_r1' . substr($secret, 0, 7);
  $spf = md5($myseed . substr('secret', 0, -4));

  $recipe = new stdClass;
  $recipe->name = 'noresubmit';
  $recipe->description = t('Prevent form resubmission');
  $recipe->description_bots = t('Bots will try to resubmit old form');
  $recipe->description_how =
    t('prepared form is remembered, and only one submission is allowed.');
  $recipe->error_field = $error_field;
  $recipe->error_text =
    t('Form session reuse detected.')
    . ' ' . t('An old form was submitted again, which may happen if it was retrieved from browser history using "Back" button.')
    . '<br />' . t('Please try again - fill all entries on this page without going "Back".')
  ;
  $recipe->proc = 'check_cache';
  return $recipe;
}

/**
 * Botcha recipe.
 */
function _botcha_recipe2($form, $secret, $error_field) {
  $myseed = 'itr_r2' . substr($secret, 0, 7);
  $spf = md5($myseed . substr('secret', 0, -4));
  $field_class = 'a' . substr($spf, 1, 4) . '_field'; // 'a' fix for Firefox - it breaks on ".<number>" class in CSS filter!
  $field_name = substr($spf, 0, 3) . '_name';
  $field_prefx = substr($spf, 10, mt_rand(3, 6));
  $secure_token = substr($spf, 4, -2) . '_form';
  $js_name  = substr($myseed, 0, 10) . substr($spf, 6, 8); // Script name
  // Chop the token in 3 parts
  $chop1 = 2;
  $chop2 = mt_rand(5, 8);
  $js_tok1    = substr($secure_token, 0, $chop1);
  $field_dflt = $field_prefx . substr($secure_token, $chop1, $chop2); // What server sends to JS in the field.
  $js_tok2    = substr($secure_token, $chop1 + $chop2);
  // JS has to reconstruct the token form tok1, part of field_dflt, tok2
  $js_match = substr($field_dflt, 0, strlen($field_prefx) + mt_rand(2, $chop2)); // What JS matches in the field
  $js_pos   = strlen($field_prefx); // What position is the part of the token

  $recipe = new stdClass;
  $recipe->name = 'honeypot_js_field';
  $recipe->description = t('Insert JS+CSS+honeypot field');
  $recipe->description_bots = t('Bots will not run JS or will mess with the field value');
  $recipe->description_how =
    t('%parts is added to the form.', array('%parts' => t('Honeypot field') . ',CSS , JS'))
    . ' ' . t('CSS hides the input field.')
    . ' ' . t('JS enters key value into the field.');
  $recipe->error_field = $error_field;
  $recipe->error_text =
    _botcha_error_text_errorcode($spf) . '<li>' .
    _botcha_error_text_javascript() . '<li>' .
    _botcha_error_text();

  $recipe->form_elements = array(
    $field_name => array(
      '#type' => 'textfield',
      '#title' => t('Enter your name'), // Leave the bot enough hints for it to guess it is a good name field
      '#default_value' => $field_dflt, // Store part of secure_token
      '#description' => t('Your first name.'),
      // This is for human users without CSS.
      '#prefix' => '<div class="' . $field_class . '">'
          // [#1004218] Addition for screen readers. Apparently, they don't hande CSS display:none.
          . '<span class="description"> (' . t('If you\'re a human, don\'t change the following field') . ')</span>'
    ,
      '#suffix' => '</div>' . _botcha_error_noscript(),
      '#attributes' => array('class' => array($field_class), 'autocomplete' => 'off'),
      '#weight' => -20,
      '!valid_token' => $secure_token,
    ),
  );

  $selector = "input.$field_class";

  $recipe->css = 'div.' . $field_class . ' { display: none; visibility: hidden; }';

  $recipe->js = <<<END
(function ($) {
  Drupal.behaviors.{$js_name} = {
    attach: function (context, settings) {
      $("{$selector}").each(function() {
        f=$(this)[0];
        if (f.value.indexOf("{$js_match}")==0){f.value="{$js_tok1}"+f.value.substring({$js_pos})+"{$js_tok2}";}
      });
    }
  };
}(jQuery));
END;
// Note: There is a jQuery+Firefox+FireBug bug ($(document).ready)
// on POST under FireBug - http://bugs.jquery.com/ticket/7688

  return $recipe;
}

/**
 * Botcha recipe.
 */
function _botcha_recipe3($form, $secret, $error_field) {
  $form_id = $form['#id'];

  $myseed = 'itr_r3' . substr($secret, 0, 7);
  $spf = md5($myseed . substr('secret', 0, -4));
  $field_class = 'a' . substr($spf, 1, 4) . '_field'; // 'a' for Firefox ignores ".<number>" class in CSS filter!
  $field_name = substr($spf, 0, 3) . '_name';
  $field_name_url = substr($spf, 1, 4) . '_name';
  $field_prefx = substr($spf, 10, mt_rand(3, 6));
  $secure_token = substr($spf, 4, -2) . '_form';
  $js_name  = substr($myseed, 0, 10) . substr($spf, 6, 8); // Script name
  // Chop the token in 3 parts
  $chop1 = 2;
  $chop2 = mt_rand(5, 8);
  $js_tok1    = substr($secure_token, 0, $chop1);
  $field_dflt = $field_prefx . substr($secure_token, $chop1, $chop2); // What server sends to JS in the field.
  $js_tok2    = substr($secure_token, $chop1 + $chop2);
  // JS has to reconstruct the token form tok1, part of field_dflt, tok2
  $js_match = substr($field_dflt, 0, strlen($field_prefx) + mt_rand(2, $chop2)); // What JS matches in the field
  $js_pos   = strlen($field_prefx); // What position is the part of the token

  $recipe = new stdClass;
  $recipe->name = 'obscure_url_field';
  $recipe->description = t('Insert a new field into form action URL');
  $recipe->description_bots = t('Bots will not run JS and miss the field');
  $recipe->description_how =
    t('%parts is added to the form.', array('%parts' => 'JS'))
    . ' ' . t('JS enters key value into the field.');
  $recipe->error_field = $error_field;
  $recipe->error_text =
    _botcha_error_text_errorcode($spf) . '<li>' .
    _botcha_error_text_javascript() . '<li>' .
    _botcha_error_text();

  // Add hidden field to keep part of the token
  $recipe->form_elements = array(
    $field_name => array(
      '#type' => 'hidden',
      '#default_value' => $field_dflt, // Store part of secure_token
      '#attributes' => array('class' => array($field_class)),
      '#weight' => 20,
    ),
  );

  // Describe URL field. JS will return token in URL field.
  $recipe->url_elements = array(
    $field_name_url => array(
      '#type' => 'textfield',
      '#default_value' => '',
      '!valid_token' => $secure_token,
    ),
  );

  $selector = "input.$field_class";
  $submit = _botcha_url($form['#action'],
    array('query' => array($field_name_url => '__replace__')));
  $submit = preg_replace('/__replace__/',
    $js_tok1 . '\'+v+\'' . $js_tok2   // $secure_token
  , $submit);

  $recipe->js = <<<END
(function ($) {
  Drupal.behaviors.{$js_name} = {
    attach: function (context, settings) {
      $("{$selector}").each(function() {
        f=$(this)[0];
        if (f.value.indexOf("{$js_match}")==0){
          v=f.value.substring({$js_pos});
          form=$(this).parents("form#{$form_id}")[0];
          $(form)[0].action ='{$submit}';
        }
      });
    }
  };
}(jQuery));
END;

  return $recipe;
}

/**
 * Botcha recipe.
 * FIXME: WORK IN PROGRESS. Making a css->js->form recipe
 */
function _botcha_recipe4($form, $secret, $error_field) {
  $myseed = 'itr_r4' . substr($secret, 0, 8);
  $spf = md5($myseed . substr('secret', 0, -4));
  $field_class = 'a' . substr($spf, 1, 4) . '_field'; // 'a' fix for Firefox - it breaks on ".<number>" class in CSS filter!
  $field_name = substr($spf, 0, 3) . '_name';
  $field_prefx = substr($spf, 10, mt_rand(3, 6));
  $secure_token = substr($spf, 4, -2) . '_form';
  $js_name  = substr($myseed, 0, 10) . substr($spf, 6, 8); // Script name
  // Chop the token in 3 parts
  $chop1 = 2;
  $chop2 = mt_rand(5, 8);
  $js_tok1    = substr($secure_token, 0, $chop1);
  $field_dflt = $field_prefx . substr($secure_token, $chop1, $chop2); // What server sends to JS in the field.
  $css_tok2    = substr($secure_token, $chop1 + $chop2);
  // JS has to reconstruct the token form tok1, part of field_dflt, tok2
  $js_match = substr($field_dflt, 0, strlen($field_prefx) + mt_rand(2, $chop2)); // What JS matches in the field
  $js_pos   = strlen($field_prefx); // What position is the part of the token

  $recipe = new stdClass;
  $recipe->name = 'honeypot_js_css2field';
  $recipe->description = t('Insert JS+CSS+honeypot field');
  $recipe->description_bots = t('Bots will not run JS or not load CSS or will mess with the field value');
  $recipe->description_how =
    t('%parts is added to the form.', array('%parts' => t('Honeypot field') . ',CSS , JS'))
    . ' ' . t('CSS carries secret data and hides the input field.')
    . ' ' . t('JS enters key value into the field.');
  $recipe->error_field = $error_field;
  $recipe->error_text =
    _botcha_error_text_errorcode($spf) . '<li>' .
    _botcha_error_text_javascript() . '<li>' .
    _botcha_error_text();

  $recipe->form_elements = array(
    $field_name => array(
      '#type' => 'textfield',
      '#title' => t('Enter your name'), // Leave the bot enough hints for it to guess it is a good name field
      '#default_value' => $field_dflt, // Store part of secure_token
      '#description' => t('Your first name.'),
      // This is for human users without CSS.
      '#prefix' => '<div class="' . $field_class . '">'
          // [#1004218] Addition for screen readers. Apparently, they don't hande CSS display:none.
          . '<span class="description"> (' . t('If you\'re a human, don\'t change the following field') . ')</span>'
    ,
      '#suffix' => '</div>' . _botcha_error_noscript(),
      '#attributes' => array('class' => array($field_class), 'autocomplete' => 'off'),
      '#weight' => -20,
      '!valid_token' => $secure_token,
    ),
  );

  $selector = "input.$field_class";

  $recipe->css = 'div.' . $field_class . ' { display: none; visibility: hidden; } input.' . $field_class . ' { font-family: sans-serif,"' . $css_tok2 . '" !important; }';

  $recipe->js = <<<END
(function ($) {
  Drupal.behaviors.{$js_name} = {
    attach: function (context, settings) {
      $("{$selector}").each(function() {
        f=$(this)[0];
        tok2=$.trim($(f).css('fontFamily').split(',')[1]);
        if(tok2[0] == "'" || tok2[0] == '"') tok2=tok2.substring(1, tok2.length-1);
        if (f.value.indexOf("{$js_match}")==0){f.value="{$js_tok1}"+f.value.substring({$js_pos})+tok2;}
      });
    }
  };
}(jQuery));
END;
// Note: There is a jQuery+Firefox+FireBug bug ($(document).ready)
// on POST under FireBug - http://bugs.jquery.com/ticket/7688

  return $recipe;
}

function _botcha_recipes($form, $botcha, $secret) {
  // Caching
  static $recipes_cache = array();
  $cache_id = $form['#build_id'] . '_' . $botcha . '_' . $secret;
  if (isset($recipes_cache[$cache_id])) {
    if (BOTCHA_LOGLEVEL >= 6) {
      watchdog(BOTCHA_LOG,
      'Found cached recipes book for %cache_id',
      array('%cache_id' => $cache_id), WATCHDOG_INFO);
    }
    return $recipes_cache[$cache_id];
  }
  if (BOTCHA_LOGLEVEL >= 6) {
    watchdog(BOTCHA_LOG,
    'Built new recipes book for %cache_id',
    array('%cache_id' => $cache_id), WATCHDOG_INFO);
  }

  if ($botcha == 'test') {
    $recipe_book = array(
      '_botcha_recipe_test1',
      '_botcha_recipe1',
    );
  }
  else {
    $recipe_book = array(
      '_botcha_recipe1',
      '_botcha_recipe2',
      '_botcha_recipe3',
//      '_botcha_recipe4',
    );

    if ($botcha == '') {
      $botcha = 'default';
    }
    elseif ($botcha != 'default') {
      $botcha = split(',', $botcha);
    }
  }

  $error_field = _botcha_pick_patsy_field($form, 'mail');

  $recipes = array();
  foreach ($recipe_book as $recipe_page) {
    if (!function_exists($recipe_page)) {
      continue;
    }
    $recipe = $recipe_page($form, $secret, $error_field);
    if (!is_array($botcha) || (isset($recipe->name) && isset($botcha[$recipe->name]))) {
      $recipe->source = $recipe_page;
      $recipe->secret = $secret;
      $recipes[] = $recipe;
    }
  }
  if ($cache_id) {
    $recipes_cache[$cache_id] = $recipes;
  }
  return $recipes;
}

/**
 * Filter out sensitive form data from values for logging
 */
function _botcha_filter_form_values_log($values) {
  if (is_array($values) && isset($values['pass']) && is_string($values['pass'])) {
    $values['pass'] = '********';
  }
  return $values;
}

/**
 * Filter out sensitive form data for logging
 * Recursive.
 */
function _botcha_filter_form_log($form, $level = 0) {
  if (is_array($form) && !is_string($form)) {
    foreach ($form as $key => $value) {
      if ($key == '#post') {
        $form[$key] = ($level == 0)
          ? _botcha_filter_form_values_log($value)
          : t('...[redundand entry - removed]');
      }
      elseif ($key == '#parameters') {
        foreach ($value as $n => $arg) {
          $value[$n] = _botcha_filter_form_log($arg, -1);
        }
        $form[$key] = $value;
      }
      elseif (!element_property($key)) {
        // Handle recursion for $key == '#parameters'
        if ($level == -1) {
          $form[$key] = ($key == 'pass') ? '********' : _botcha_filter_form_log($value, -1);
        }
        else {
          $form[$key] = _botcha_filter_form_log($value, $level + 1);
          if ($key == 'pass' && isset($form[$key]['#value'])) {
            $form[$key]['#value'] = '********';
          }
        }
      }
    }
  }
  return $form;
}

/**
 * Main BOTCHA worker - process the form and apply BOTCHA protection
 */
function botcha_form_alter_botcha(&$form, &$form_state, $form_id, $botcha) {
  // When we are altering a form, there are one or two build IDs.
  // $form['#build_id'] is for the new form (always present)
  // $_POST['form_build_id'] is for previous form submission (only if the form is being submitted)
  // Herein lies the problem - we need to build new form based on form's build_id to present
  // to the user, but check the BOTCHA fields in previous form submission based on post's build_id.

  $build_id = $form['#build_id'];
  $build_id_subm = isset($_POST['form_build_id']) ? $_POST['form_build_id'] : FALSE;
  $form_state['botcha_submit_values'] = array();
  if ($build_id_subm) {
    // Generate Secret for submitted build id
    $secret = md5($build_id_subm . BOTCHA_SECRET);
    $recipes = _botcha_recipes($form, $botcha, $secret);
    // Save submitted values in our stash for later use in _validate, as we have to reset them here at _form_alter stage.
    // It won't be possible to reset after validation as there is no reliable mechanism in Form API,
    // i.e. form_set_value() does not change rendered form and form errors disable whole 'rebuild' business.
    foreach ($recipes as $recipe) {
      if (isset($recipe->form_elements)) {
        foreach ($recipe->form_elements as $field => $value) {
          if (isset($_POST[$field])) {
            $form_state['botcha_submit_values'][$field] = $_POST[$field];
          }
        }
      }
    }
    // Save recipes for later use in _validate
    $form_state['#botcha_recipes_subm'] = $recipes;
  }

  // Generate Secret for this build id
  $secret = md5($build_id . BOTCHA_SECRET);
  $recipes = _botcha_recipes($form, $botcha, $secret);
  // Save recipes for later use in _validate
  $form_state['#botcha_recipes'] = $recipes;

  // Common javascript?
//  drupal_add_js(drupal_get_path('module', 'botcha') . '/botcha.js');
  $added_botchas = array();
  $jss = array();
  $csss = array();
  foreach ($recipes as $recipe) {
    if (isset($recipe->form_elements)) {
      foreach ($recipe->form_elements as $field => $value) {
        unset($value['!valid_token']);
        $form[$field] = $value;
        if ($build_id_subm && isset($value['#default_value'])) {
          // Reset our controls to defaults here (as explained above).
          $form[$field]['#value'] = $value['#default_value'];
          $form_state['post'][$field] = $value['#default_value'];
          $_POST[$field] = $value['#default_value'];
        }
      }
    }

    if (isset($recipe->js)) {
      drupal_add_js($recipe->js, array('type' => 'inline', 'preprocess' => FALSE));
      $jss[] = $recipe->js;
    }
    if (isset($recipe->css)) {
      drupal_add_css('' . $recipe->css . '', array('type' => 'inline'));
      $csss[] = $recipe->css;
    }
    $added_botchas[] = $recipe->name;
  }
  // user_login forms open session in validate hooks instead of submit
  // we should be the first to validate - add our hook to the beginning
  if (is_array($form['#validate'])) {
    array_unshift($form['#validate'], '_botcha_form_validate');
  }
  else {
    $form['#validate'] = array('_botcha_form_validate');
  }
  $form_state['#botcha'] = $botcha;
//  // Add a submit handler to remove form state storage.
//  $form['#submit'][] = '_botcha_form_submit';

  if (BOTCHA_LOGLEVEL >= 4) {
    watchdog(BOTCHA_LOG,
    '%form_id form prepared by BOTCHA: added recipes - !botchas!more',
    array(
      '%form_id' => $form_id,
      '!botchas' => join(', ', $added_botchas),
      '!more' => ''
        . ((BOTCHA_LOGLEVEL >= 5) ? '<br /><br />' . 'POST=<pre>' . print_r(_botcha_filter_form_values_log($_POST), 1) . '</pre>' : '')
        . ((BOTCHA_LOGLEVEL >= 5) ? '<br /><br />' . 'GET=<pre>' . print_r(_botcha_filter_form_values_log($_GET), 1) . '</pre>' : '')
        . ((BOTCHA_LOGLEVEL >= 5) ? '<br /><br />' . 'SERVER=<pre>' . print_r($_SERVER, 1) . '</pre>' : '')
        . ((BOTCHA_LOGLEVEL >= 5) ? '<br /><br />' . 'form=<pre>' . print_r(_botcha_filter_form_log($form), 1) . '</pre>' : '')
        . ((BOTCHA_LOGLEVEL >= 5 && count($jss)) ? '<br /><br />' . 'JS=<pre>' . join("\n", $jss) . '</pre>' : '')
        . ((BOTCHA_LOGLEVEL >= 5 && count($csss)) ? '<br /><br />' . 'CSS=<pre>' . join("\n", $csss) . '</pre>' : ''),
    ),
    WATCHDOG_NOTICE
  );
  }

  if ($build_id_subm != $build_id) {
    $form_state['post']['form_build_id'] = $build_id;
    // Issue the client a new build_id, make sure that the form has it set in the hidden field
  }
  _botcha_set_form_cache($build_id); // Save build id

}

/**
 * BOTCHA cache functions - set, get, clear
 */
function _botcha_set_form_cache($form_build_id) {
  // 6 hours cache life time for forms should be plenty.
  $expire = 21600;

  $data = array();
  $data['#cache_token'] = _botcha_get_token();
  // We use cache_form table.
  // Sneaky, but why build our own table since we are working side-by-side with form API?
  cache_set('botcha_' . $form_build_id, $data, 'cache_form', REQUEST_TIME + $expire);
}

function _botcha_get_form_cache($form_build_id) {
  if ($cached = cache_get('botcha_' . $form_build_id, 'cache_form')) {
    $data = $cached->data;
    if (isset($data['#cache_token']) && _botcha_valid_token($data['#cache_token'])) {
      return TRUE;
    }
  }
  return FALSE;
}

function _botcha_clear_form_cache($form_build_id) {
  $expire = 0; // expire immediately
  $data = NULL;
  cache_set('botcha_' . $form_build_id, $data, 'cache_form', REQUEST_TIME + $expire);
}

/**
 * Custom token (session) management for anonymous users.
 */
function _botcha_get_token($value = '') {
  if (empty($_SESSION['botcha_session'])) {
    $_SESSION['botcha_session'] = session_id();
  }
  return drupal_hmac_base64($value, $_SESSION['botcha_session'] . drupal_get_private_key() . drupal_get_hash_salt());
}

function _botcha_valid_token($token) {
  return ($token == _botcha_get_token());
}

/**
 * Custom form validation.
 */
function __botcha_form_validate($form, &$form_state) {
  // FIXME: where does this empty value come from ? happens with comments
  unset($form_state['values']['']);
  $build_id = isset($_POST['form_build_id']) ? $_POST['form_build_id'] : $form['#build_id'];
  $recipes = isset($form_state['#botcha_recipes_subm']) ? $form_state['#botcha_recipes_subm'] : $form_state['#botcha_recipes'];
  $botcha_names = array();
  $i = 0;
  $fail = '';
  foreach ($recipes as $recipe) {
    if (isset($recipe->form_elements)) {
      foreach ($recipe->form_elements as $field => $value) {
        if (isset($value['!valid_token']) && $form_state['botcha_submit_values'][$field] !== $value['!valid_token']) {
          $fail = $form_state['botcha_submit_values'][$field] . '!=' . $value['!valid_token'];
//          unset($form_state['post'][$field], $form_state['values'][$field], $_POST[$field]);
          break 2; // No need to finish other botchas, we got a bot
        }
//        unset($form_state['post'][$field], $form_state['values'][$field], $_POST[$field]);
      }
    }
    if (isset($recipe->url_elements)) {
      foreach ($recipe->url_elements as $field => $value) {
        if (isset($value['!valid_token']) && $_GET[$field] !== $value['!valid_token']) {
          $fail = $_GET[$field] . '!=' . $value['!valid_token'];
          unset($_GET[$field]);
          break 2;
        }
        unset($_GET[$field]);
      }
    }
    if (!empty($recipe->proc)) {
      switch ($recipe->proc) {
        case 'check_cache':
          if (!_botcha_get_form_cache($build_id)) {
            $fail = 'botcha_resubmit';
            break 2; // No need to finish other botchas, we got a bot
          }
          break;
      }
    }
    $botcha_names[] = $recipe->name;
    $i++;
  }
  _botcha_clear_form_cache($build_id); // Invalidate cache so resubmit will not work
  if ($i < count($recipes)) {
    variable_set('botcha_form_blocked_counter', variable_get('botcha_form_blocked_counter', 0) + 1);
    form_set_error($recipe->error_field, $recipe->error_text);
    // show blocked submissions in log
    // FIXME: 1) more generic statement:
    if (BOTCHA_LOGLEVEL >= 1) {
      watchdog(BOTCHA_LOG,
      '%form_id post blocked by BOTCHA: submission looks like from a spambot.!more',
      array(
        '%form_id' => $form['#id'],
        '!more' => ''
          . ((BOTCHA_LOGLEVEL >= 2) ? '<br /><br />' . ' Failed [' . $fail . '] botcha \'' . $recipe->name . '\' #' . ($i + 1) . ' of ' . count($recipes) . ' recipes from "' . $form_state['#botcha'] . '" book.' : '')
          . ((BOTCHA_LOGLEVEL >= 5) ? '<br /><br />' . 'POST=<pre>' . print_r(_botcha_filter_form_values_log($_POST), 1) . '</pre>' : '')
          . ((BOTCHA_LOGLEVEL >= 5) ? '<br /><br />' . 'GET=<pre>' . print_r(_botcha_filter_form_values_log($_GET), 1) . '</pre>' : '')
          . ((BOTCHA_LOGLEVEL >= 5) ? '<br /><br />' . 'SERVER=<pre>' . print_r($_SERVER, 1) . '</pre>' : '')
          . ((BOTCHA_LOGLEVEL >= 5) ? '<br /><br />' . ' values=<pre>' . print_r(_botcha_filter_form_values_log($form_state['values']), 1) . '</pre>' : ''),
      ),
      WATCHDOG_WARNING
      );
    }
    $rules_event_name = 'botcha_form_rejected';
  }
  else {
    variable_set('botcha_form_passed_counter', variable_get('botcha_form_passed_counter', 0) + 1);
    // show good submissions in log
    if (BOTCHA_LOGLEVEL >= 3) {
      watchdog(BOTCHA_LOG,
      '%form_id post approved by BOTCHA.!more',
      array(
        '%form_id' => $form['#id'],
        '!more' => ''
          . ((BOTCHA_LOGLEVEL >= 3) ? ' Checked ' . count($recipes) . ' botchas (' . join(', ', $botcha_names) . ').' : '')
          . ((BOTCHA_LOGLEVEL >= 5) ? '<br /><br />' . 'POST=<pre>' . print_r(_botcha_filter_form_values_log($_POST), 1) . '</pre>' : '')
          . ((BOTCHA_LOGLEVEL >= 5) ? '<br /><br />' . 'GET=<pre>' . print_r(_botcha_filter_form_values_log($_GET), 1) . '</pre>' : '')
          . ((BOTCHA_LOGLEVEL >= 5) ? '<br /><br />' . 'SERVER=<pre>' . print_r($_SERVER, 1) . '</pre>' : '')
          . ((BOTCHA_LOGLEVEL >= 5) ? '<br /><br />' . ' form=<pre>' . print_r(_botcha_filter_form_log($form), 1) . '</pre>' : '')
          . ((BOTCHA_LOGLEVEL >= 5) ? '<br /><br />' . ' values=<pre>' . print_r(_botcha_filter_form_values_log($form_state['values']), 1) . '</pre>' : ''),
      ),
      WATCHDOG_INFO
    );
    }
    $rules_event_name = 'botcha_form_approved';
  }
  if (module_exists('rules')) {
    $arguments = array(
//      'form' => &$form,
//      'form_state' => &$form_state,
      'form_id' => $form['#id'],
      'total_recipes' => count($recipes),
      'passed_recipes' => $i,
      'passed_recipes_names' => join(', ', $botcha_names),
      'last_recipe_name' => $recipe->name,
      'fail' => $fail,
      'failed_field' => $recipe->error_field,
      'failed_error' => $recipe->error_text,
    );
    rules_invoke_event($rules_event_name, $arguments);
  }
}

//END
