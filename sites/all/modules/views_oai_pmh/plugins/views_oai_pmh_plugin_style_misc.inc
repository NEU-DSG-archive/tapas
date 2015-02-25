<?php
/**
 * @file
 * Definition of the views_oai_pmh_plugin_style_misc class.
 */

class views_oai_pmh_plugin_style_misc extends views_oai_pmh_plugin_style {
  /**
   * The metadata type that the object represents, e.g. 'oai_dc' for Dublin Core.
   * @var string
   */
  private $_metadata_format = '';
  
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
  }
  
  function render_records() {
    if (!empty($this->display->handler->oai_args['errors'])) {
      return '';
    }
    
    if ($this->uses_row_plugin() && empty($this->row_plugin)) {
      vpr('views_plugin_style_default: Missing row plugin');
      return '';
    }
    
    $records = '';
    if ($this->uses_row_plugin()) {
      $rows = array();
      foreach ($this->view->result as $row_index => $row) {
        $this->view->row_index = $row_index;
        // Render the individual row.
        $fields = $this->row_plugin->render($row);
        // Theme the rendered row.
        $themed_record = theme(
          $GLOBALS['views_oai_pmh'][$this->_metadata_format]->record_theme,
          array('nid' => $row->nid, 'node_changed' => $row->node_changed, 'fields' => $fields));
        $records .= $themed_record;
      }
    }
    return $records;
  }
}
