<?php

/**
 * @file
 * Implementation of the field filter handler
 */

class views_fieldfilter_handler_filter_fields extends views_handler_filter {
  var $value_form_type = 'checkboxes';
  var $value_title = 'Fields';
  
  function construct() {
    parent::construct();
    $this->value_title = t('Fields');
    $this->value_options = NULL;
  }

  function option_definition() {
    $options = parent::option_definition();
    
    $options['exposed']['fieldfield_fields'] = array();
	//$options['expose']['fieldfield_fields'] = array();
    
    return $options;
  }
  
  function query() {
    foreach ($this->value_options as $name => $label) {
      $this->view->field[$name]->options['exclude'] = isset($this->value[$name]) ? 0 : 1;
    }
  }
  
  function expose_form(&$form, &$form_state) {
//ADDED from https://drupal.org/files/AJAXerrorwphp54-1985530.patch in regards to https://drupal.org/node/1985530
    //parent::expose_form(&$form, &$form_state);
	parent::expose_form($form, $form_state);
//END
    
    $options = array();
    $view =& $this->view;
    
    $fields = isset($view->display[$view->current_display]->display_options['fields']) ? $view->display[$view->current_display]->display_options['fields'] : $view->display['default']->display_options['fields'];
    
    foreach ($fields as $name => $field) {
		
      //ADDED change machine names to labels
	  //$options[$name] = !empty($field->options['label']) ? $field->options['label'] : $name;
	  $options[$name] = !empty($field['label']) ? $field['label'] : $name;
	  //END
    }
    
    $form['exposed']['fieldfield_fields'] = array(
	//$form['expose']['fieldfield_fields'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Fields to filter'),
      '#default_value' => $this->options['exposed']['fieldfield_fields'],
	  //'#default_value' => $this->options['expose']['fieldfield_fields'],
      '#options' => $options,
      
      '#description' => t('Select which fields can be affected by field filter. If none are selected all fields will be available to be turned on and off.'),
    );
  }
  
  function get_value_options() {
    $options = array();
    $view =& $this->view;
    $fields = isset($view->display[$view->current_display]->display_options['fields']) ? $view->display[$view->current_display]->display_options['fields'] : $view->display['default']->display_options['fields'];
//ADDED because darn thing was displaying all filters not just selected. Some weird conflict between options['exposed']['fieldfield_fields'] and options['fieldfield_fields']. exposed is now default
    //$includes = isset($this->options['fieldfield_fields']) ? $this->options['fieldfield_fields'] : array_keys($fields);
    $includes = isset($this->options['exposed']['fieldfield_fields']) ? array_filter($this->options['exposed']['fieldfield_fields']) : array_keys($fields);
    		  //exit(print_r($includes));
//END

    foreach ($fields as $name => $field) {
      if (in_array($name, $includes)) {
//ADDED from https://drupal.org/files/views-field-filter-fix-machine-names-being-shown-not-labels-2014077-1.patch in regards to https://drupal.org/node/2014077
//        $options[$name] = !empty($field->options['label']) ? $field->options['label'] : $name;
        $options[$name] = !empty($field['label']) ? $field['label'] : $name;
//END
      }
    }
    
    $this->value_options = $options;
  }

  function value_form(&$form, &$form_state) {
    $form['value'] = array();
    $options = array();

    if (empty($form_state['exposed'])) {
      // Add a select all option to the value form.
      $options = array('all' => t('Select all'));
    }

    $this->get_value_options();
    $options += $this->value_options;
    $default_value = (array) $this->value;
    
    $form['value'] = array(
      '#type' => $this->value_form_type,
      '#title' => $this->value_title,
      '#options' => $options,
      '#default_value' => $default_value,
      // These are only valid for 'select' type, but do no harm to checkboxes.
      '#multiple' => TRUE,
      '#size' => count($options) > 8 ? 8 : count($options),
    );
    
  }
  
  function value_submit($form, &$form_state) {
    // Drupal's FAPI system automatically puts '0' in for any checkbox that
    // was not set, and the key to the checkbox if it is set.
    // Unfortunately, this means that if the key to that checkbox is 0,
    // we are unable to tell if that checkbox was set or not.

    // Luckily, the '#value' on the checkboxes form actually contains
    // *only* a list of checkboxes that were set, and we can use that
    // instead.

    $form_state['values']['options']['value'] = $form['value']['#value'];
  }
  
  function admin_summary() {
    if (!empty($this->options['exposed'])) {
      return t('exposed');
    }
    $this->get_value_options();

    if (!is_array($this->value)) {
      return;
    }

    $operator = check_plain($info[$this->operator]['short']);
    $values = '';

    foreach ($this->value as $value) {
      if ($values !== '') {
        $values .= ', ';
      }
      if (drupal_strlen($values) > 8) {
        $values .= '...';
        break;
      }
      $values .= check_plain($this->value_options[$value]);
    }

    return (($values !== '') ? ' ' . $values : '');
  }
}