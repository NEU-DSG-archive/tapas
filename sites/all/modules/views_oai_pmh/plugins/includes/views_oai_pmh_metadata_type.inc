<?php
/**
 * @file
 * Definition of the views_oai_pmh_metadata_type class.
 */

/**
 * Provides a mechanism for defining the OAI metadata types. Create an instance
 * of this class with the values set, add it to the global array
 * $GLOBALS['views_oai_pmh'] and the module will handle the rest.
 * 
 * We use the separate file views_oai_pmh_metadata_type_definitions.inc to
 * create the metadata type definitions.
 */
class views_oai_pmh_metadata_type {
  /**
   * The metadata prefix value used in the OAI-PMH request. This will be the same as the object's key in the $GLOBALS['views_oai_pmh'] array.
   * @var type 
   */
  public $metadata_prefix;
  
  /**
   * A descriptive name of the data format.
   * @var type 
   */
  public $name;
  
  /**
   * An array of the elements pertaining to this data format.
   * @var array
   */
  public $elements;
  
  /**
   * The Drupal theme implementation for this data type's fields.
   * @var string
   */
  public $field_theme = 'views_oai_pmh_row_misc_fields';
  
  /**
   * The Drupal theme implementation for this data type's records.
   * @var string
   */
  public $record_theme = 'views_oai_pmh_record';
  
  /**
   * The handler implementation for this data type.
   * @var string
   */
  public $handler = 'views_oai_pmh_plugin_row_misc';
  
  /**
   * The index of the style plugin related to this data format, as defined in 'views_oai_pmh.views.inc'.
   * @var string
   */
  public $style_plugin;
  
  /**
   * The index of the row plugin related to this data format, as defined in 'views_oai_pmh.views.inc'.
   * @var string
   */
  public $row_plugin;
  
  /**
   * The group name for elements in the Views form relating to this data format, e.g. 'oai_dc_fields' for Dublin Core fields.
   * @var string
   */
  public $form_group_name;
  
  /**
   * A string of help text describing this metadata type.
   * @var string 
   */
  public $help;
  
  /**
   * An object of type views_oai_pmh_xml_node that acts as the root element of each OAI record in the generated XML.
   * @var object
   */
  public $base_xml_node;
  
  /**
   * Class constructor. 
   */
  public function __construct($metadata_prefix,
                              $name = '',
                              $elements = array(),
                              $style_plugin = '',
                              $row_plugin = '',
                              $form_group_name = '',
                              $help = '') {
    // Populate the object properties.
    $this->metadata_prefix = $metadata_prefix;
    $this->name = $name;
    $this->elements = $elements;
    $this->style_plugin = $style_plugin;
    $this->row_plugin = $row_plugin;
    $this->form_group_name = $form_group_name;
    $this->help = $help;
    $this->base_xml_node = NULL;
    // Add the 'none' option to the elements list along with its translation.
    $this->elements['none'] = t('None');
  }
}
