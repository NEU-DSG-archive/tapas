<?php
/**
 * @file
 * Theme related functions for processing our output style plugins.
 *
 * Views bug: http://drupal.org/node/593336
 */

/**
 * This 'misc' field theme function is capable of rendering its output in any of
 * the defined OAI formats.
 */
function theme_views_oai_pmh_row_misc_fields($vars) {
  $view = $vars['view'];
  $metadata_format = $vars['metadata_format'];
  
  // Prepare a reference array for storing common XML element attributes, such as language, and their values.
  $attribute_values = array();
  
  // Loop through the fields for this view.
  $previous_inline = FALSE;
  $vars['fields'] = array(); // ensure it's at least an empty array.
  $row_index = $view->row_index;
  
  foreach ($view->field as $id => $field) {
    // render this even if set to exclude so it can be used elsewhere.
    $field_output = $view->style_plugin->get_field($row_index, $id);
    $empty = $field_output !== 0 && empty($field_output);
    
    // Get the label or labels for this field.
    $label_string = $view->field[$id]->label();
    // See if the label contains formatting HTML that we need to remove.
    if (strpos($label_string, '<span') !== FALSE) {
    // Grab the contents of the first 'span' tag in the label.
      $label_string = substr($label_string, 0, strpos($label_string, '<span>', 1));
      // Strip the span tag, leaving us with just the labels.
      $label_string = strip_tags($label_string);
    }
    // Explode the label on commas.
    $labels = explode(', ', $label_string);
    $label = '';
    foreach ($labels as $l) {
      if (array_key_exists($l, $GLOBALS['views_oai_pmh'][$metadata_format]->elements)) {
        // This value is found in the elements list.
        $label = $l;
        break;
      }
    }
    
    // Check to see if this label is for an attribute. Attributes are formatted
    // like this: "(attribute)oai_xxx@language"
    $attribute_prefix = '(attribute)';
    $attribute_name = '';
    $is_attribute = FALSE;
    if (substr($label, 0, strlen($attribute_prefix)) == $attribute_prefix) {
      // Flag that this is an attribute.
      $is_attribute = TRUE;
      // Get the common name of the attribute.
      $pos = strpos($label, '@');
      if ($pos !== FALSE) {
        $attribute_name = substr($label, $pos + 1);
      }
    }
    
    if ($is_attribute) {
      // Store the default values for XML element attributes in our reference array.
      $attribute_values[$attribute_name] = $field_output;
    }
    
    if ($is_attribute || $label == '') {
      // Force the display to exclude this field - we don't want to render attributes or empty labels as XML tags.
      $field->options['exclude'] = 1;
    }
    
    if (empty($field->options['exclude']) && (!$empty || (empty($field->options['hide_empty']) && empty($vars['options']['hide_empty'])))) {
      // Explode the label to give us a node path array, e.g. "a/b/c" becomes
      // array('a', 'b', 'c').
      $xml_node_path = explode('/', $label);
      
      // See if any of the XML elements in this path are defined with any
      // attributes.
      // 
      // Note: in a case like <a><b attr="val"><c>Hello</c><d>Hi</d></b></a>,
      // the <b> tag's attributes will be calculated twice: once for <c> and
      // again for <d>. This is necessary for processing tags in this way but
      // does add a little overhead.
      $attributes = array();
      for ($i = 0; $i <= count($xml_node_path); $i++) {
        $attributes[$i] = array();
      }
      
      for ($i = 0; $i < count($xml_node_path); $i++) {
        $attrs = array();
        // See if this XML node definition contains any attributes.
        $xml_node_path_attrs = explode('@', $xml_node_path[$i], 2);
        $xml_node_path[$i] = $xml_node_path_attrs[0];
        if (!empty($xml_node_path_attrs[1])) {
          $attrs = $xml_node_path_attrs[1];
        }
        // Process this XML node's attributes.
        if (!empty($attrs)) {
          // There may be multiple attributes for this one element so attempt to separate them out.
          $attrs = explode('@', $attrs);
          // Check each attribute against our reference array; if we have a value for it, use that, otherwise omit the attribute.
          foreach ($attrs as $attr) {
            // Only add the attribute to our list of attributes for this node if we have a value available for it.
            if (array_key_exists($attr, $attribute_values)) {
              $attributes[$i][$attr] = $attribute_values[$attr];
            }
          }
        }
      }
      
      // Add the attributes to their nodes, optionally creating the nodes if they haven't yet been created.
      for ($i = 0; $i < count($xml_node_path); $i++) {
        if (is_array($attributes[$i]) && count($attributes[$i]) > 0) {
          $vars['nodes']->set_attributes_at(array_slice($xml_node_path, 0, $i+1), $attributes[$i]);
        }
      }
      
      // Multiple values per field (Views 3):
      if (isset($field->multiple)) {
        $separator = $field->options['separator'];
        foreach (explode($separator, $field_output) as $value) {
          $vars['nodes']->insert_at($xml_node_path, $value, $attributes[$i]);
        }
      }
      // Multiple values per field (Views 2):
      elseif (isset($field->content_field['multiple'])) {
        $field_nid = $view->style_plugin->get_field_id($row_index)->nid;
        $field_values = $field->field_values[$field_nid];
        foreach ($field_values as $field_value) {
          if ($field_value['value'] != '') {
            $vars['nodes']->insert_at($xml_node_path, $field_value['value'], $attributes[$i]);
          }
        }
      }
      // Taxonomy terms:
      elseif (isset($field->items)) {
        $taxonomy_items = reset($field->items);
        foreach ($taxonomy_items as $taxonomy_item) {
          $vars['nodes']->insert_at($xml_node_path, $taxonomy_item['name'], $attributes[$i]);
        }
      }
      // Single value:
      else {
        $vars['nodes']->insert_at($xml_node_path, $field_output, $attributes[$i]);
      }
    }
  }

  // Start the draw procedure on the XML nodes, starting at an indent level of 2
  // to make the output look pretty, and return the resulting string.
  return $vars['nodes']->draw(2);
}

function template_preprocess_views_oai_pmh_response(&$vars) {
  global $base_url;

  $vars['oai_response_date'] = format_xml_elements(array('responseDate' => $vars['oai_args']['response_date']));
  $request = array('key' => 'request',
        					 'value' => $base_url);

  $request['attributes'] = array();
  if (!empty($vars['oai_args']['verb'])) {
    $request['attributes']['verb'] = $vars['oai_args']['verb'];
  }
  if (!empty($vars['oai_args']['metadataPrefix'])) {
    $request['attributes']['metadataPrefix'] = $vars['oai_args']['metadataPrefix'];
  }

  $vars['oai_request'] = format_xml_elements(array($request));
  $vars['oai_verb'] = !empty($vars['oai_args']['verb']) ? $vars['oai_args']['verb']: '';
  $vars['oai_base_url'] = $base_url;
  $vars['oai_errors'] = format_xml_elements($vars['oai_args']['errors']);

  $vars['oai_content'] = $vars['oai_content'];
  if (!empty($vars['oai_content'])) {
    $vars['oai_resumption_token'] = $vars['resumption_token'];
  }
}

function theme_views_oai_pmh_identify_body($vars) {
  global $base_url;
  $view = $vars['view'];
  $path = $view->display_handler->get_path();
  $datestamp = db_query("SELECT min(changed) FROM {node} ")->fetchField();

  $ident = array(
  	'repositoryName' => variable_get('site_name', 'Views OAI'),
    'baseURL' => $base_url . '/' . $path,
    'protocolVersion' => '2.0',
    'adminEmail' => variable_get('site_mail', ini_get('sendmail_from')),
    'earliestDatestamp' => gmstrftime('%Y-%m-%dT%H:%M:%SZ', $datestamp),
    'deletedRecord' => 'no',
    'granularity' => 'YYYY-MM-DDThh:mm:ssZ',
    'compression' => 'gzip',
  );

  return format_xml_elements($ident);
}

function theme_views_oai_pmh_metadataformats_body($vars) {
  $formats = '';
  $formats .= format_xml_elements(array('metadataFormat' => array(
  	'metadataPrefix' => 'oai_dc',
    'schema' => 'http://www.openarchives.org/OAI/2.0/oai_dc.xsd',
    'metadataNamespace' => 'http://www.openarchives.org/OAI/2.0/oai_dc/',
  )));
  return $formats;
}

function theme_views_oai_pmh_record_header($vars) {
  global $base_url;
  static $parts = array();
  if (empty($parts)) {
    $parts = parse_url($base_url);
  }
  $nid = $vars['nid'];
  $datestamp = $vars['node_changed'];

  $info['header']['identifier'] = 'oai:' . $parts['host'] . ':' . $nid;
  $info['header']['datestamp'] = gmstrftime('%Y-%m-%dT%H:%M:%SZ', $datestamp);
//  $info['header']['setSpec'] = '';

  return format_xml_elements($info);
}

/**
 * Theme function capable of handling all types of OAI records.
 */
function theme_views_oai_pmh_record($vars) {
  $nid = $vars['nid'];
  $datestamp = $vars['node_changed'];
  $header = theme('views_oai_pmh_record_header', array('nid' => $nid, 'node_changed' => $datestamp));

  return '<record>
' . $header .
'  <metadata>
' . $vars['fields'] .
'  </metadata>
</record>
';
}

/**
 * Returns a valid XML tag formed from the given input.
 *
 * @param $tag The string that should be made into a valid XML tag.
 * @return The valid XML tag or an empty string if the string contained no valid
 * XML tag characters.
 */
function _views_oai_pmh_xml_tag_clean($tag) {

  // This regex matches characters that are not valid in XML tags, and the
  // unicode ones that are. We don't bother with unicode, because it would so
  // the preg_replace down a lot.
  static $invalid_tag_chars_regex = '#[^\:A-Za-z_\-.0-9]+#';

  // These characters are not valid at the start of an XML tag:
  static $invalid_start_chars = '-.0123456789';

  // Convert invalid chars to '-':
  $tag = preg_replace($invalid_tag_chars_regex, '-', $tag);

  // Need to trim invalid characters from the start of the string:
  $tag = ltrim($tag, $invalid_start_chars);

  return $tag;
}

function theme_oai_field_mapper_form($vars) {
  $form = $vars['form'];
  foreach (element_children($form) as $key ) {
    $rows[] = array(
      drupal_render($form[$key]['drupal_label']),
      drupal_render($form[$key])
    );
  }
  $header = array(
    t('Drupal field label'),
    t('OAI type element name'),
  );
  $output = theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($form);
  return $output;
}

function _views_oai_pmh_xmlstr($string, $charset = 'iso8859-1', $xmlescaped = 'false')
{
  $xmlstr = stripslashes(trim($string));
  // just remove invalid characters
  $pattern ="/[\x-\x8\xb-\xc\xe-\x1f]/";
  $xmlstr = preg_replace($pattern, '', $xmlstr);

  // escape only if string is not escaped
  if (!$xmlescaped) {
    $xmlstr = htmlspecialchars($xmlstr, ENT_QUOTES);
  }

  return $xmlstr;
}
