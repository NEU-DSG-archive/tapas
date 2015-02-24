<?php
/**
 * Form callback for OG Menu configuration.
 */
function og_menu_default_links_config_form($form, &$form_state) {
  $bundles = og_get_all_group_bundle();

  // Add a field for each bundle.
  if ($bundles) {
    foreach($bundles as $entity_type => $entity_bundles) {
      foreach ($entity_bundles as $bundle => $bundle_label) {
        $group_type = strtolower('_' . $bundle);
        $form['og_menu_default_link_bundles']['og_menu_default_links_' . $entity_type . $group_type ] = array(
          '#type' => 'textarea',
          '#title' => t("%bundle Bundle", array('%bundle' => $bundle_label)),
          '#default_value' => variable_get('og_menu_default_links_' . $entity_type . $group_type, FALSE),
          '#description' => t("These links will be added to any new OG Menus added
            to <b>%bundle</b> groups.", array('%bundle' => $bundle_label)),
        );
      }
    }
  }

  if (module_exists('token')) {
    $form['token_help'] = array(
      '#title' => t('Replacement patterns'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#description' => t('Prefer raw-text replacements for text to avoid problems with HTML entities!'),
    );

    $form['token_help']['help'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('node'),
    );
  }

  return system_settings_form($form);
}