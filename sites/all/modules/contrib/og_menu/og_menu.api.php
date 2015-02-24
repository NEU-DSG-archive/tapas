<?php
/**
 * Implements hook_og_menu_admin_menu_overview_form_tableselect().
 * Useful for other module that extend the functionality of the og menu admin
 * overview form.
 * When a module returns TRUE, the menu table will be rendered with checkboxes
 * in the left collumn.
 *
 * @return boolean
 */
function og_menu_og_menu_admin_menu_overview_form_tableselect() {
  return TRUE;
}
