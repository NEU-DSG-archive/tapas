<?php
/**
 * @file
 * Definition of the views_oai_pmh_plugin_style_auto class.
 */

// Include the definition of the views_oai_pmh_plugin_style_misc class.
include_once('views_oai_pmh_plugin_style_misc.inc');

class views_oai_pmh_plugin_style_auto extends views_oai_pmh_plugin_style {
  
  /**
   * An array of views_oai_pmh_plugin_style_misc objects, one for each metadata type.
   * @var views_oai_pmh_plugin_style_misc 
   */
  private $_style_objects = array();
  
  /**
   * A flag indicating if the $_style_objects property is populated with objects.
   * @var bool
   */
  private $_style_objects_populated = FALSE;
  
  /**
   * A private copy of the currently-selected OAI metadataPrefix.
   * @var type 
   */
  private $_metadata_format = '';
  
  public function __construct() {
    // Create a new object for each type of metadata class and store it in our _row_objects array.
    foreach ($GLOBALS['views_oai_pmh'] as $key => $metadata_config) {
      $this->_style_objects[$metadata_config->metadata_prefix] = new views_oai_pmh_plugin_style_misc();
      // Call the object's custom constructor function, passing it the $key as the type of metadata we want it to represent.
      $this->_style_objects[$metadata_config->metadata_prefix]->construct($key);
    }
  }
  
  /**
   * Clones this object's properties into its style objects. Only performs this
   * action the first time the function is called; subsequent calls just run the
   * validity test.
   * 
   * @return boolean TRUE if the objects are OK, FALSE if not.
   */
  private function _check_style_objects() {
    // Check to see if the $_style_objects array has not been populated already.
    if (!$this->_style_objects_populated) {
      
      // We'll use these two variables to determine the metadata type to use:
      // 1) The "metadataPrefix" parameter from the query string.
      $metadata_prefix = (array_key_exists('metadataPrefix', $_GET) && $_GET['metadataPrefix'] != '') ? $_GET['metadataPrefix'] : '';
      // 2) The active row plugin from the View.
      $row_plugin = $this->view->display[$this->view->current_display]->handler->options['row_plugin'];
      
      // If we're set to use the Auto row plugin, then use the metadataPrefix parameter from the query string.
      if ($row_plugin == 'auto' && array_key_exists($metadata_prefix, $GLOBALS['views_oai_pmh'])) {
        $this->_metadata_format = $metadata_prefix;
      }
      // Otherwise use the row plugin.
      elseif ($row_plugin != '' && $row_plugin != 'auto') {
        // Check each data definition for the correct metadata prefix to use with this row plugin.
        foreach ($GLOBALS['views_oai_pmh'] as $data_definition) {
          if ($data_definition->row_plugin == $row_plugin) {
            $this->_metadata_format = $data_definition->metadata_prefix;
          }
        }
      }
      // Use a default value as a fallback.
      else {
        $this->_metadata_format = $GLOBALS['views_oai_pmh_default'];
      }
      
      // Examine each value in the array of style objects, indexed by the keys from the global array of OAI data types.
      foreach ($GLOBALS['views_oai_pmh'] as $key => $unused) {
        // Populate each style object with the properties of this 'Auto' object.
        foreach ($this as $property => $value) {
          switch ($property) {
            // Don't apply the properties that are unique to the 'Auto' class.
            case '_style_objects':
            case '_style_objects_populated':
            case '_metadata_format':
              break;

            // Create/set the property in the row object.
            default:
              $this->_style_objects[$key]->$property = $value;
              break;
          }
        }
      }
      
      // Flag the object population code as complete so this block doesn't run again.
      $this->_style_objects_populated = TRUE;
    }
    
    // Check each individual array item for a valid object that is a descendant of the 'views_oai_pmh_plugin_style' class, returning a failure if we encounter any errors.
    foreach ($this->_style_objects as $obj) {
      if (!is_object($obj) || get_parent_class($obj) != 'views_oai_pmh_plugin_style') {
        return FALSE;
      }
    }
    return TRUE;
  }
  
  function render_records() {
    if ($this->_check_style_objects()) {
      // Call render_records() on the appropriate object in the $_style_objects array.
      $records = $this->_style_objects[$this->_metadata_format]->render_records();
      return $records;
    }
    return NULL;
  }
}
