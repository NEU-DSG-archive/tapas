<?php

/**
 * @file
 * Rules Definitions
 */

/*------------------DATA----------------------*/
/**
 * Implements hook_rules_data_info().
 */
function menu_rules_rules_data_info() {
  $data = array();
  $data['menu_rules_menu_item'] = array(
    'label' => t('Menu item'),
    'wrap' => TRUE,
    'property info' => array(
      'mlid' => array(
        'type' => 'integer',
        'label' => 'Menu link id',
      ),
      'plid' => array(
        'type' => 'integer',
        'label' => 'Parent menu link id',
      ),
      'menu_name' => array(
        'type' => 'text',
        'label' => 'Menu',
      ),
      'link_path' => array(
        'type' => 'uri',
        'label' => 'Link path',
      ),
      'link_title' => array(
        'type' => 'text',
        'label' => 'Link title',
      ),
    ),
  );
  return $data;
}
/*------------------EVENTS----------------------*/
/**
 * Implements hook_rules_event_info().
 */
function menu_rules_rules_event_info() {
  $events = array();

  $events['menu_rules_menu_link_insert'] = array(
    'label' => t('A menu link was inserted'),
    'variables' => array(
      'menu_link' => array('type' => 'menu', 'label' => t('The menu link that was inserted')),
    ),
    'group' => t('Menu'),
  );
  $events['menu_rules_menu_link_update'] = array(
    'label' => t('A menu link was updated'),
    'variables' => array(
      'menu_link' => array('type' => 'menu', 'label' => t('The menu link that was updated')),
    ),
    'group' => t('Menu'),
  );

  $events['menu_rules_menu_link_delete'] = array(
    'label' => t('A menu link was deleted'),
    'variables' => array(
      'menu_link' => array('type' => 'menu', 'label' => t('The menu link that was deleted')),
    ),
    'group' => t('Menu'),
  );
  $events['menu_rules_menu_link_change'] = array(
    'label' => t('A menu link was changed in some way (inserted/updated/deleted)'),
    'variables' => array(
      'menu_link' => array('type' => 'menu-link', 'label' => t('The menu link that was changed')),
      'op' => array('type' => 'text', 'label' => t('The operation that has been executed on the link (insert/update/delete)')),
    ),
    'group' => t('Menu'),
  );
  return $events;
}
/*------------------CONDITIONS----------------------*/
/**
 * Implements hook_rules_condition_info().
 */
function menu_rules_rules_condition_info() {
  $conditions = array();
  $conditions['menu_rules_node_has_menu_entry'] = array(
    'label' => t('Node has menu entry'),
    'parameter' => array(
      'nid' => array(
        'type' => 'text',
        'label' => t('Nid'),
        'default value' => '[node:nid]',
      ),
    ),
    'group' => t('Menu'),
    'base' => 'menu_rules_node_has_menu_entry',
  );
  return $conditions;
}
/**
 * Callback for condition menu_rules_node_has_menu_entry.
 */
function menu_rules_node_has_menu_entry($nid) {
  $query = db_select('menu_links')->condition('link_path', 'node/' . $nid);
  $num = $query->countQuery()->execute()->fetchField();
  if($num) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}
/*------------------ACTIONS----------------------*/
/**
 * Implements hook_rules_action_info().
 */
function menu_rules_rules_action_info() {
  $actions = array();
  $actions['menu_rules_create_arbitrary_item'] = array(
    'label' => t('Create a menu item'),
    'parameter' => array(
      'menu_parent' => array(
        'type' => 'text',
        'label' => t('Menu parent item'),
        'options list' => 'menu_rules_get_menu_parents',
        'default value' => 'main-menu:0',
      ),
      'menu_link_title' => array(
        'type' => 'text',
        'label' => t('Menu link title'),
        'default value' => '',
      ),
      'menu_path' => array(
        'type' => 'text',
        'label' => t('Menu path'),
        'default value' => '',
      ),
      'language' => array(
        'type' => 'text',
        'label' => t('Language'),
        'default value' => '',
        'optional' => TRUE,
      ),
      'display_messages' => array(
        'type' => 'boolean',
        'label' => t('Display a message upon success or error'),
        'default value' => FALSE,
      ),
    ),
    'provides' => array(
      'new_menu_item' => array(
        'label' => t('New menu item'),
        'type' => 'menu_rules_menu_item',
        'save' => FALSE,
      ),
    ),
    'group' => t('Menu'),
    'base' => 'menu_rules_create_arbitrary_item',
  );
  $actions['menu_rules_create_item'] = array(
    'label' => t('Create a menu item for node'),
    'parameter' => array(
      'menu_parent' => array(
        'type' => 'text',
        'label' => t('Menu parent item'),
        'options list' => 'menu_rules_get_menu_parents',
        'default value' => 'main-menu:0',
      ),
      'menu_link_title' => array(
        'type' => 'text',
        'label' => t('Menu link title'),
        'default value' => '[node:title]',
      ),
      'nid' => array(
        'type' => 'text',
        'label' => t('Nid'),
        'default value' => '[node:nid]',
      ),
      'language' => array(
        'type' => 'text',
        'label' => t('Language'),
        'default value' => '[node:language]',
      ),
      'display_messages' => array(
        'type' => 'boolean',
        'label' => t('Display a message upon success or error'),
        'default value' => FALSE,
      ),
    ),
    'provides' => array(
      'new_menu_item' => array(
        'label' => t('New menu item'),
        'type' => 'menu_rules_menu_item',
        'save' => FALSE,
      ),
    ),
    'group' => t('Menu'),
    'base' => 'menu_rules_create_item',
  );
  $actions['menu_rules_update_item'] = array(
    'label' => t('Update a menu item for node'),
    'parameter' => array(
      'create_if_empty' => array(
        'type' => 'boolean',
        'label' => t("Create a new menu link if the node doesn't have one already"),
        'default value' => TRUE,
      ),
      'menu_parent' => array(
        'type' => 'text',
        'label' => t('Menu parent'),
        'options list' => 'menu_rules_get_menu_parents',
        'default value' => 'main-menu:0',
        'description' => t('This value is only used when creating a new menu link. If the existing link is updated, the parent is not changed.'),
      ),
      'menu_link_title' => array(
        'type' => 'text',
        'label' => t('Menu link title'),
        'default value' => '[node:title]',
      ),
      'respect_overridden_title' => array(
        'type' => 'boolean',
        'label' => t('Respect overridden menu link title'),
        'description' => t('This choice allows for overriding the menu link title. If this checkbox is not checked, every time the node is updated the menu link will be changed to the value of the Menu link title field'),
        'default value' => TRUE,
      ),
      'node' => array(
        'type' => 'node',
        'label' => t('Node'),
        'default value' => '[node]',
      ),
      'language' => array(
        'type' => 'text',
        'label' => t('Language'),
        'default value' => '[node:language]',
      ),
      'display_messages' => array(
        'type' => 'boolean',
        'label' => t('Display a message upon success or error'),
        'default value' => FALSE,
      ),
    ),
    'provides' => array(
      'updated_menu_item' => array(
        'label' => t('New menu item'),
        'type' => 'menu_rules_menu_item',
        'save' => FALSE,
      ),
    ),
    'group' => t('Menu'),
    'base' => 'menu_rules_update_item',
  );
  $actions['menu_rules_menu_rebuild'] = array(
    'label' => t('Rebuild all menus'),
    'group' => t('Menu'),
    'base' => 'menu_rules_menu_rebuild',
  );
  return $actions;
}
/**
 * Generates option list for action menu_rules_create_item.
 */
function menu_rules_get_menu_parents() {
  $options = menu_parent_options(menu_get_menus(), 0);
  return $options;
}
/**
 * Callback for menu_rules_create_arbitrary_item
 */
function menu_rules_create_arbitrary_item($menu_parent, $menu_title, $menu_path, $menu_language = NULL, $display_messages = FALSE) {
  global $language;
  $item = array(
    'link_title' => $menu_title,
    'link_path' => $menu_path,
    'language' => empty($menu_language) ? $language : $menu_language,
    'weight' => 0,
  );
  if (is_numeric($menu_parent)) {
    $item['plid'] = $menu_parent;
  }
  elseif (strpos($menu_parent, ':')) {
    list($item['menu_name'], $item['plid']) = explode(':', $menu_parent);
  }
  if(menu_link_save($item)) {
    if($display_messages) {
      drupal_set_message(t('A new menu link has been created.'));
    }
    return array('new_menu_item' => $item);
  }
  else {
    if($display_messages) {
      drupal_set_message(t('Menu link could not be created.'));
    }
  }
}
/**
 * Callback for action menu_rules_create_item.
 */
function menu_rules_create_item($menu_parent, $menu_link_title, $nid, $language, $display_messages) {
  $item = array(
    'link_title' => $menu_link_title,
    'link_path' => 'node/' . $nid,
    'language' => $language,
    'weight' => 0,
  );
  if (is_numeric($menu_parent)) {
    $item['plid'] = $menu_parent;
  }
  elseif (strpos($menu_parent, ':')) {
    list($item['menu_name'], $item['plid']) = explode(':', $menu_parent);
  }
  list($item['menu_name'], $item['plid']) = explode(':', $menu_parent);
  if(menu_link_save($item)) {
    if($display_messages) {
      drupal_set_message(t('A new menu link has been created.'));
    }
    return array('new_menu_item' => $item);
  }
  else {
    if($display_messages) {
      drupal_set_message(t('Menu link could not be created.'));
    }
  }
}
 /**
 * Callback for action menu_rules_update_item.
 */
function menu_rules_update_item($create_if_empty, $menu_parent, $menu_link_title, $respect_overridden_title, $node, $language, $display_messages) {
  $nid = $node->nid;
  $original_node = $node->original;
  $old_link = array();
  $menu_entry = menu_rules_node_has_menu_entry($nid);
  if ($menu_entry) {
    $mlid = db_select('menu_links', 'ml')
      ->fields('ml', array('mlid'))
      ->condition('link_path', 'node/' . $nid)
      ->range(0, 1)
      ->execute()
      ->fetchField();
    $old_link = menu_link_load($mlid);
    $new_link = array();
    if(isset($node->menu)) {
      $new_link = $node->menu;
    }
    if($old_link && $new_link) {
      $old_link['options'] = serialize($old_link['options']);
      $update_link = true;
      if($respect_overridden_title) {
        if($new_link['link_title'] !== $old_link['link_title']) {
          $update_link = false;
        }
        if($old_link['link_title'] !== $original_node->title) {
          $update_link = false;
        }
      }
      if($update_link) {
        $item = array(
          'link_title' => $menu_link_title,
          'link_path' => 'node/' . $nid,
          'language' => $language,
          'mlid' => $mlid,
          'menu_name' => $old_link['menu_name'],
          'plid' => $old_link['plid'],
          'weight' => $old_link['weight'],
        );
        if(menu_link_save($item, $old_link)) {
          if($display_messages) {
            drupal_set_message(t('Menu link has been updated.'));
          }
          return array('updated_menu_item' => $item);
        }
        else {
          if($display_messages) {
            drupal_set_message(t('Menu link could not be updated.'));
          }
        }
      }
    }
  }
  else {
    if($create_if_empty) {
      return menu_rules_create_item($menu_parent, $menu_link_title, $nid, $language, $display_messages);
    }
    else {
      if($display_messages) {
        drupal_set_message(t('Could not update menu item as no menu item has been defined for this node.'));
      }
    }
  }
}
/**
 * Callback for action menu_rules_menu_rebuild.
 */
function menu_rules_menu_rebuild() {
  static $done;
  // Prevents multiple rebuilds during the same page load.
  if (!$done) {
    $done = TRUE;
    menu_rebuild();
  }
}
