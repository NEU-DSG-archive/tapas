<?php
/**
 * @file
 * Definition of the views_oai_pmh_plugin_row_auto class.
 */

// Load the definition of the views_plugin_row class.
module_load_include('inc', 'views', '/plugins/views_plugin_row');

class views_oai_pmh_plugin_row_auto extends views_plugin_row {
  
  /**
   * An array of views_oai_pmh_plugin_row_misc objects, one for each metadata type.
   * @var views_oai_pmh_plugin_row_misc 
   */
  private $_row_objects = array();
  
  /**
   * A flag indicating if the $_row_objects property is populated with objects.
   * @var bool
   */
  private $_row_objects_populated = FALSE;
  
  /**
   * A private copy of the currently-selected OAI metadataPrefix.
   * @var string 
   */
  private $_metadata_format = '';
  
  public function __construct() {
    // Create a new object for each type of metadata class and store it in our _row_objects array.
    foreach ($GLOBALS['views_oai_pmh'] as $key => $metadata_config) {
      $this->_row_objects[$metadata_config->metadata_prefix] = new views_oai_pmh_plugin_row_misc($key);
      // Call the object's custom constructor function, passing it the $key as the type of metadata we want it to represent.
      $this->_row_objects[$metadata_config->metadata_prefix]->construct($key);
    }
  }
  
  /**
   * Clones this object's properties into its row objects. Only performs this
   * action the first time the function is called; subsequent calls just run the
   * validity test.
   * 
   * @return boolean TRUE if the objects are OK, FALSE if not.
   */
  private function _check_row_objects() {
    // Filter out row plugins that aren't appropriate to this style plugin.
    $row_plugin = $this->view->display[$this->view->current_display]->handler->options['row_plugin'];
    if ($row_plugin != 'auto' && !in_array($row_plugin, $GLOBALS['views_oai_pmh'])) {
      return FALSE;
    }
    
    // Check to see if the $_row_objects array has not been populated already.
    if (!$this->_row_objects_populated) {
      // We'll use these two variables to determine the metadata type to use:
      // 1) The "metadataPrefix" parameter from the query string.
      $metadata_prefix = (array_key_exists('metadataPrefix', $_GET) && $_GET['metadataPrefix'] != '') ? $_GET['metadataPrefix'] : '';
      // 2) The active row plugin from the View (see above).
      
      // If we're set to use the Auto row plugin, then use the metadataPrefix parameter from the query string.
      if ($row_plugin == 'auto' && array_key_exists($metadata_prefix, $GLOBALS['views_oai_pmh'])) {
        $this->_metadata_format = $metadata_prefix;
      }
      // Otherwise use the row plugin.
      elseif ($row_plugin != '' && $row_plugin != 'auto') {
        $this->_metadata_format = $row_plugin;
      }
      // Use a default value as a fallback.
      else {
        $this->_metadata_format = $GLOBALS['views_oai_pmh_default'];
      }
      
      // Examine each value in the array of row objects, which should all be unconfigured at this point.
      foreach ($this->_row_objects as $key => $row_object) {
        // Populate each row object with the properties of this 'Auto' object.
        foreach ($this as $property => $value) {
          switch ($property) {
            // Don't apply the properties that are unique to the 'Auto' class.
            case '_row_objects':
            case '_row_objects_populated':
            case '_metadata_format':
              break;

            // We need to overwrite the theme-related properties with themes appropriate to the object type we're creating.
            case 'definition':
              $value['theme'] = $GLOBALS['views_oai_pmh'][$this->_metadata_format]->field_theme;
              $value['handler'] = $GLOBALS['views_oai_pmh'][$this->_metadata_format]->handler;
              $this->_row_objects[$key]->$property = $value;
              break;

            // Create/set the property in the row object.
            default:
              $this->_row_objects[$key]->$property = $value;
              break;
          }
        }
      }
      
      // Flag the object population code as complete so this block doesn't run again.
      $this->_row_objects_populated = TRUE;
    }
    
    // Check each individual array item for a valid object of type 'views_oai_pmh_plugin_row_misc', returning a failure if we encounter any errors.
    foreach ($this->_row_objects as $obj) {
      if (!is_object($obj) || get_class($obj) != 'views_oai_pmh_plugin_row_misc') {
        return FALSE;
      }
    }
    return TRUE;
  }
  
  function render($row) {
    if ($this->_check_row_objects()) {
      // We only want to "render" the appropriate object from our set.
      if (!empty($this->_row_objects[$this->_metadata_format]) && is_object($this->_row_objects[$this->_metadata_format])) {
        // Call the object's render() function and return its result.
        return $this->_row_objects[$this->_metadata_format]->render($row);
      }
    }
    return '';
  }
  
  function validate() {
    if ($this->_check_row_objects()) {
      foreach ($this->_row_objects as $obj) {
        $obj->validate();
      }
    }
    return NULL;
  }
  
  function options_form(&$form, &$form_state) {
    $form['oai_auto_labels'] = array(
      '#type' => 'fieldset',
      '#title' => t('Automatic OAI type detection'),
    );
    $form['oai_auto_labels']['info'] = array(
      '#markup' => '<p>In order for the automatic OAI type detection to work, the Drupal fields for each data format below must be assigned to their respective OAI element names.</p>',
    );
    
    if ($this->_check_row_objects()) {
      // Extend our form with the additional fields prescribed by each row style object.
      foreach ($this->_row_objects as $key => $obj) {
        $obj->options_form($form, $form_state);
      }
    }
    return NULL;
  }
  
  function options_submit(&$form, &$form_state) {
    if ($this->_check_row_objects()) {
      // Pass the $form and $form_state variables to each row style object's form submission handler.
      $all_form_item_options = array();
      foreach ($this->_row_objects as $obj) {
        // "Submit" the form and store the resulting set of options in an array.
        $all_form_item_options[] = $obj->options_submit($form, $form_state, FALSE);
      }
      
      // Combine the options arrays for each "form" into one merged array which we can process later.
      $merged_form_item_options = array();
      foreach ($all_form_item_options as $form_item_options) {
        foreach ($form_item_options as $form_item => $form_item_option) {
          foreach ($form_item_option as $field => $value) {
            if ($field == 'label' && $merged_form_item_options[$form_item][$field] != '') {
              // Join the item labels for each field.
              if ($value != '') {
                $merged_form_item_options[$form_item][$field] .= ', ' . $value;
              }
            }
            else {
              // Use whatever value we're given; apart from the label, which we're handling above, they'll all be the same.
              $merged_form_item_options[$form_item][$field] = $value;
            }
          }
        }
      }
      
      // Process each merged set of options for each field.
      foreach ($merged_form_item_options as $id => $options) {
        // The mappings can look really ugly if they are output alongside the
        // field names. To combat this, we wrap them in HTML with display:none.
        // We can pick them up later by removing the HTML code.
        if ($options['label'] != '') {
          $options['label'] = '<span class="js-hide">' . $options['label'] . '</span><span>' . format_plural((substr_count($options['label'], ', ') + 1), '1 mapping', '@count mappings') . '</span>';
        }
        // Save the field info in the view.
        $form_state['view']->set_item($form_state['display_id'], 'field', $id, $options);
      }
    }
    return NULL;
  }
}
