<?php
/**
 * @file
 * Definition of the views_oai_pmh_xml_node class.
 */

/**
 * A class designed to represent an XML node, specially designed to help the
 * Views OAI PMH module render OAI fields as XML elements. 
 */
class views_oai_pmh_xml_node {
  /**
   * @var string The name of the XML node, e.g. <key>value</key>.
   */
  public $key;

  /**
   * @var string|array The content of the XML node. If $value is a string,
   * the node is rendered as <key>value</key>. If value is an array, each
   * entry in the array is expected to be another views_oai_pmh_xml_node object.
   * These are rendered as nested XML tags. If value is empty, the node is
   * rendered as <key />.
   */
  public $value;

  /**
   * @var array An array of key/value pair attributes to assign to this XML node.
   */
  public $attributes;

  /**
   * Class constructor.
   * 
   * Takes an element name as a required parameter in order that is contains the
   * bare minimum information to render as an XML node.
   * 
   * @param string $key
   *  The name of the new XML node.
   * @param string|array $value
   *  (optional) The content of the new XML node.
   * @param array $attributes 
   *  (optional) An array of key/value pair attributes of the new XML node.
   */
  public function __construct($key = '', $value = array(), $attributes = array()) {
    $this->key = $key;
    $this->value = $value;
    $this->attributes = $attributes;
  }

  /**
   * Returns a reference to the specified node, which is a child of this one, by
   * its key. Creates the node if it doesn't exist.
   * 
   * @param string $key
   *  The name of the XML node to retrieve.
   * @return views_oai_pmh_xml_node
   *  The requested XML node object.
   */
  public function get_node($key) {
    if (is_array($this->value) && array_key_exists($key, $this->value)) {
      return $this->value[$key];
    }
    return $this->value[$key] = new views_oai_pmh_xml_node($key);
  }

  /**
   * Inserts a new node at a given position in the XML tree.
   * 
   * Accepts an array of node names as an 'address', indicating where to insert
   * the new node. The value of the created node is passed in the $value
   * parameter; an array of attributes for it in the $attributes parameter.
   * 
   * Note: Only assigns attributes if the $attributes array is non-empty. This
   * is so elements with existing sets of attributes don't have them overwritten
   * by accident.
   * 
   * @param array $node_address
   *  An array of XML node names indicating the address of the new node in the
   *  XML node tree. For example, passing array('a', 'b', 'c') as the parameter
   *  would create an XML node called 'c' and position it in a structure at:
   *  <a><b><c /></b></a>.
   * @param string|array $value
   *  The content of the new XML node.
   * @param array $attributes 
   *  (optional) An array of key/value pair attributes of the new XML node.
   */
  public function insert_at($node_address, $value, $attributes = array()) {
    $node = &$this;
    $prev = NULL;
    for ($i = 0; $i < count($node_address); $i++) {
      $prev = $node;
      $node = &$node->get_node($node_address[$i]);
    }
    // Get the deepest entry in the $node_address array to give us the desired XML element name.
    $node_name = $node_address[count($node_address)-1];
    
    // If the node already contains an array of values then just add this one to the list.
    if (is_array($node->value)) {
      // Unless given its own set of attributes, this node should inherit its parent's attributes.
      if (empty($attributes)) {
        $attributes = $node->attributes;
      }
      $node->value[] = new views_oai_pmh_xml_node($node_name, $value, $attributes);
    }
    // Otherwise this node doesn't contain any values yet, so add them in.
    else {
      $node->value = $value;
      if (!empty($attributes)) {
        $node->attributes = $attributes;
        // See if this node contains multiple values.
        if (is_array($node->value)) {
          // Find out the type of key used to index the $this->value array
          reset($node->value);
          $first_key = key($node->value);
          if (is_numeric($first_key)) {
            foreach ($node->value as $child) {
              $child->attributes = $attributes;
            }
          }
        }
      }
    }
  }

  /**
   * Allows you to add attributes to an existing XML node, so you are not forced
   * to add the attributes on creation.
   * 
   * Note: Only assigns attributes if the $attributes array is non-empty. This
   * is so elements with existing sets of attributes don't have them overwritten
   * by accident.
   * 
   * @param array $node_address
   *  An array of XML node names indicating the address of the node in the
   *  XML node tree. For example, passing array('a', 'b', 'c') as the parameter
   *  would create an XML node called 'c' and position it in a structure at:
   *  <a><b><c /></b></a>.
   * @param array $attributes 
   *  (optional) An array of key/value pair attributes of the new XML node.
   */
  public function set_attributes_at($node_address, $attributes) {
    if (!empty($attributes)) {
      $node = &$this;
      for ($i = 0; $i < count($node_address); $i++) {
        $node = &$node->get_node($node_address[$i]);
      }
      $node->attributes = $attributes;
      // See if this node contains multiple values.
      if (is_array($node->value)) {
        // Find out the type of key used to index the $this->value array
        reset($node->value);
        $first_key = key($node->value);
        if (is_numeric($first_key)) {
          foreach ($node->value as $child) {
            $child->attributes = $attributes;
          }
        }
      }
    }
  }

  /**
   * Creates the XML for this node. If the object's $value property is an array,
   * this method recursively calls the draw() method on each of the
   * views_oai_pmh_xml_node objects in the $value array and returns the combined
   * XML output.
   * 
   * @param int $indent_level
   *  The amount by which to indent this XML node in the output string.
   *  Recursive calls to draw() on child views_oai_pmh_xml_node objects increase
   *  the indent level by 1 each time.
   * @return string
   *  A string containing the generated XML code ready for output.
   */
  public function draw($indent_level = 0) {
    // Create an indent string appropriate to the current indent level.
    $indent = str_repeat('  ', $indent_level);

    // Build up a string containing this node's attributes.
    $attrs = array();
    if (is_array($this->attributes)) {
      foreach ($this->attributes as $key => $value) {
        $attrs[] = $key . '="' . $value . '"';
      }
    }
    $attrs = implode(' ', $attrs);
    if ($attrs) {
      $attrs = ' ' . $attrs;
    }

    // Construct the buffer string containing this node and all its descendants.
    $buffer = '';
    // Case 1: $this->value contains an array.
    if (is_array($this->value)) {
      // Increase the indent level for drawing our child objects.
      $indent_level++;
      // Find out the type of key used to index the $this->value array
      reset($this->value);
      $first_key = key($this->value);
      // Case 1A: The contained array holds named XML objects. Draw this object, then draw its children.
      if (!is_numeric($first_key)) {
        $tag = $this->process_tag_name($this->key);
        $buffer .= $indent . '<' . $tag . $attrs . '>' . "\n";
        foreach ($this->value as $child) {
          $buffer .= $child->draw($indent_level);
        }
        $buffer .= $indent . '</' . $tag . '>' . "\n";
      }
      // Case 1B: The contained array is a numerically-indexed array of XML objects. Do not draw this object, only draw its children.
      else {
        foreach ($this->value as $child) {
          $buffer .= $child->draw($indent_level);
        }
      }
    }
    // Case 2: $this->value contains an XML node. Output this node, then draw its child.
    elseif (is_a($this->value, 'views_oai_pmh_xml_node')) {
      $tag = $this->process_tag_name($this->key);
      $buffer .= $indent . '<' . $tag . $attrs . '>' . "\n";
      $buffer .= $this->value->draw($indent_level);
      $buffer .= $indent . '</' . $tag . '>' . "\n";
    }
    // Case 3: $this->value contains a non-empty string. Draw this node with its value.
    elseif ($this->value != '') {
      $tag = $this->process_tag_name($this->key);
      $buffer .= $indent . '<' . $tag . $attrs . '>' . $this->value . '</' . $tag . '>' . "\n";
    }
    // Case 4: $this->value is empty. Draw this node as an empty XML node.
    else {
      $tag = $this->process_tag_name($this->key);
      $buffer .= $indent . '<' . $tag . $attrs . ' />' . "\n";
    }

    return $buffer;
  }
  
  /**
   * Removes unwanted characters from an XML tag name, usually so that multiple
   * versions of the same tag can get the correct output. E.g. 'description*1'
   * and 'description*2' will both become 'description'.
   * 
   * @param string $tag The XML tag to process.
   * @return string The XML tag name, without any info that should be hidden.
   */
  public function process_tag_name($tag) {
    if (strpos($this->key, '*') !== FALSE) {
      $tag = substr($tag, 0, strpos($this->key, '*'));
    }
    return $tag;
  }
  
  /**
   * Gets the number of child elements stored in this XML node.
   * 
   * @return int
   *  The number of child elements.
   */
  public function num() {
    if (is_array($this->value)) {
      return count($this->value);
    }
    return 0;
  }
}
