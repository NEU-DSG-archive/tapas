<?php
/**
 * @file
 * Definition of the views_oai_pmh_plugin_row_misc class.
 */

class views_oai_pmh_plugin_row_misc extends views_plugin_row {
  /**
   * The metadata type that the object represents, e.g. 'oai_dc' for Dublin Core.
   * @var string
   */
  private $_metadata_format = '';

  /**
   * The root node of the XML tree representing this structure.
   * @var views_oai_pmh_xml_node
   */
  public $nodes;

  /**
   * Class constructor-like function. 'Views' calls this when it creates the
   * object. Takes a metadata format type, such as 'oai_dc' for Dublin Core, as
   * a parameter. This is used to grab the necessary details of the format
   * whenever required from the $GLOBALS['views_oai_pmh'] configuration array.
   * If the value is not provided, it will attempt to detect it automatically
   * from the query string.
   * 
   * @param string $type The metadata format type.
   */
  public function construct($type = '') {
    if ($type) {
      // Save the given metadata format value.
      $this->_metadata_format = $type;
    }
    else {
      // See if a particular data type is being requested in the query string.
      $metadata_prefix = (array_key_exists('metadataPrefix', $_GET) && $_GET['metadataPrefix'] != '') ? $_GET['metadataPrefix'] : '';
      if (array_key_exists($metadata_prefix, $GLOBALS['views_oai_pmh'])) {
        // Save the metadata format in this object's property.
        $this->_metadata_format = $metadata_prefix;
      }
      elseif (array_key_exists('#metadata_format', $this->definition) && $this->definition['#metadata_format'] != '' ) {
        // See if the selected data type contains the metadata format we need to use.
        $this->_metadata_format = $this->definition['#metadata_format'];
      }
    }
    
    // Create our root XML node to build upon using the global data type definition.
    $this->nodes = new views_oai_pmh_xml_node(
      $GLOBALS['views_oai_pmh'][$this->_metadata_format]->base_xml_node->key,
      $GLOBALS['views_oai_pmh'][$this->_metadata_format]->base_xml_node->value,
      $GLOBALS['views_oai_pmh'][$this->_metadata_format]->base_xml_node->attributes);
  }
  
  /**
   * Theme up the given row and return the resulting code.
   * 
   * @param array $row The row to render.
   * @return string Themed content for the given row.
   */
  public function render($row) {
    $theme_options = array(
      'view' => $this->view,
      'options' => $this->options,
      'row' => $row,
      'metadata_format' => $this->_metadata_format,
      'nodes' => &$this->nodes, // The root XML node.
    );
    $return = theme($this->theme_functions(), $theme_options);
    
    // Reset the internal nodes list to just the top level node.
    $this->nodes->value = NULL;
    
    return $return;
  }
  
  /**
   * Check the values of the Views form.
   * 
   * @return array An array of errors; empty if none.
   */
  function validate() {
    // Call the parent class' validation function.
    $errors = parent::validate();
    $link = $this->display->handler->option_link('Row options', 'row_options');
    $field_handlers = $this->display->handler->get_handlers('field');
    foreach ($field_handlers as $id => $field) {
      $ui_label = $field->ui_name();
      $label_string = $field->label();
      
      // See if the label contains formatting HTML that we need to remove.
      if (strpos($label_string, '<span') !== FALSE) {
      // Grab the contents of the first 'span' tag in the label.
        $label_string = substr($label_string, 0, strpos($label_string, '<span>', 1));
        // Strip the span tag, leaving us with just the labels.
        $label_string = strip_tags($label_string);
      }
      // Break the label string into its component labels, which are separated by commas.
      $labels[$id] = explode(', ', $label_string);
      $found_label = FALSE;
      foreach ($labels[$id] as $label) {
        // Does this label exist in our array of element names?
        if (array_key_exists($label, $GLOBALS['views_oai_pmh'][$this->_metadata_format]->elements)) {
          $found_label = TRUE;
          break;
        }
      }
      // Check that we found a valid label for this data type.
      if (!$found_label) {
        $errors[] = t(
          'The field "@title" does not have a @name label associated with it. Go to the !link page to fix it.',
          array(
            '@title' => $ui_label,
            '@name' => $GLOBALS['views_oai_pmh'][$this->_metadata_format]->name,
            '!link' => $link));
      }
    }
    return $errors;
  }
  
  /**
   * Build the Views options form.
   */
  function options_form(&$form, &$form_state) {
    // Call the parent class' form builder function.
    parent::options_form($form, $form_state);
    $field_handlers = $this->display->handler->get_handlers('field');
    foreach ($field_handlers as $id => $handler) {
      $ui_labels[$id] = $handler->ui_name();
      if ($label_string = $handler->label()) {
        // See if the label contains formatting HTML that we need to remove.
        if (strpos($label_string, '<span') !== FALSE) {
        // Grab the contents of the first 'span' tag in the label.
          $label_string = substr($label_string, 0, strpos($label_string, '<span>', 1));
          // Strip the span tag, leaving us with just the labels.
          $label_string = strip_tags($label_string);
        }
        // Break the label string into its component labels, which are separated by commas.
        $labels[$id] = explode(', ', $label_string);
      }
    }
    // Copy the name of the form field into a local variable for easy reference.
    $form_group_name = $GLOBALS['views_oai_pmh'][$this->_metadata_format]->form_group_name;
    $form[$form_group_name] = array(
      '#type' => 'fieldset',
      '#title' => t('Drupal field to @name mapping', array('@name' => $GLOBALS['views_oai_pmh'][$this->_metadata_format]->name)),
      '#theme' => 'oai_field_mapper_form',
    );
    foreach ($ui_labels as $id => $label) {
      // Determine the default value for this element name.
      $default_value = 'none';
      if (isset($labels[$id]) && is_array($labels[$id])) {
        foreach ($labels[$id] as $label) {
          // Does this label exist in our elements names array above?
          if (array_key_exists($label, $GLOBALS['views_oai_pmh'][$this->_metadata_format]->elements)) {
            $default_value = $label;
          }
        }
      }
      $form[$form_group_name][$id] = array(
        '#type' => 'select',
        '#options' => $GLOBALS['views_oai_pmh'][$this->_metadata_format]->elements,
        '#default_value' => $default_value,
      );
      $form[$form_group_name][$id]['drupal_label'] = array(
        '#markup' => $ui_labels[$id],
      );
    }
  }
  
  /**
   * Save the changes made in the Views form.
   * 
   * @param array $form
   * @param array $form_state
   * @param bool $apply_changes Set to FALSE to return the form changes as a
   *  response to the function call. TRUE (default) makes the changes directly
   *  to the $form_state variable.
   * @return array An array of the form changes.
   */
  function options_submit(&$form, &$form_state, $apply_changes = TRUE) {
    // It is very important to call the parent function here.
    parent::options_submit($form, $form_state);
    
    // Create an array to save the changes that we make to the form.
    $form_item_options = array();
    
    $section = $form_state['section'];
    switch ($section) {
      case 'row_options':
        $field_handlers = $this->display->handler->get_handlers('field');
        $form_group_name = $GLOBALS['views_oai_pmh'][$this->_metadata_format]->form_group_name;
        $labels = $form_state['values'][$section][$form_group_name];
        foreach ($field_handlers as $id => $field) {
          $options = $field->options;
          if ($labels[$id] != 'none') {
            $options['custom_label'] = TRUE;
            $options['label'] = $labels[$id];
            $options['hide_empty'] = 1;
          }
          else {
            unset($options['custom_label']);
            $options['label'] = '';

          }
          $form_item_options[$id] = $options;
          
          // See if we should be applying the changes to the form state now, or
          // returning it to the calling function. The default behaviour is to
          // apply the changes now; the alternative of returning the changes to
          // the calling function is provided for the Auto class to combine all
          // the results of multiple calls to this function in various classes.
          if ($apply_changes) {
            // Save the field info in the view.
            $form_state['view']->set_item($form_state['display_id'], 'field', $id, $options);
          }
        }
        break;
    }
    
    if (!$apply_changes) {
      return $form_item_options;
    }
    
    return NULL;
  }
}
