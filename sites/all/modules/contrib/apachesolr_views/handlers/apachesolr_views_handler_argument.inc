<?php

/**
 * Class that allows searching the site with Apache Solr through a view.
 */
class apachesolr_views_handler_argument extends views_handler_argument {
//ADDED from https://drupal.org/files/multiple_values_contextual_filter-2085217-3.patch in regards to https://drupal.org/node/2085217
//  public function query() {
//    $this->query->add_filter($this->real_field, $this->argument);

  function init(&$view, &$options) {
    parent::init($view, $options);

    // Ensure defaults for these, during summaries and stuff:
    $this->operator = 'or';
    $this->value = array();
  }

  public function option_definition() {
    $options = parent::option_definition();

    $options['break_phrase'] = array('default' => FALSE);
    $options['not'] = array('default' => FALSE);

    return $options;
  }

  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Allow passing multiple values.
    $form['break_phrase'] = array(
      '#type' => 'checkbox',
      '#title' => t('Allow multiple values'),
      '#description' => t('If selected, users can enter multiple values in the form of 1+2+3 (for OR) or 1,2,3 (for AND).'),
      '#default_value' => $this->options['break_phrase'],
      '#fieldset' => 'more',
    );

    $form['not'] = array(
      '#type' => 'checkbox',
      '#title' => t('Exclude'),
      '#description' => t('If selected, the numbers entered for the filter will be excluded rather than limiting the view.'),
      '#default_value' => !empty($this->options['not']),
      '#fieldset' => 'more',
    );
  }

  public function query($group_by = FALSE) {

    if (!empty($this->options['break_phrase'])) {
      views_break_phrase_string($this->argument, $this);
    }
    else {
      $this->value = array($this->argument);
    }

    if (is_array($this->value)) {
      $this->value = implode(' ' . strtoupper($this->operator) . ' ', $this->value);
    }

    $operator = empty($this->options['not']) ? FALSE : TRUE;

    $this->query->add_where(0, "$this->real_field", $this->value, $operator);
//END
  }
}
