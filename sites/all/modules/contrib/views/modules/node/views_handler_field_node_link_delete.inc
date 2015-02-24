<?php

/**
 * @file
 * Definition of views_handler_field_node_link_delete.
 */

/**
 * Field handler to present a link to delete a node.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_node_link_delete extends views_handler_field_node_link {

  /**
   * Renders the link.
   */
  function render_link($node, $values) {
    // Ensure user has access to delete this node.
    if (!node_access('delete', $node)) {
      return;
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "node/$node->nid/delete";
//ADDED see https://drupal.org/node/2004960
if(isset($node->og_group_ref['und'][0]['target_id'])){ 
    $this->options['alter']['query']['destination'] = 'node/'.$node->og_group_ref['und'][0]['target_id'];
}else{
//ORIGINAL
    $this->options['alter']['query'] = drupal_get_destination();
//END ORIGINAL
}
//END

    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    return $text;
  }
}
