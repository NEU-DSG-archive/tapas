<?php
/**
 * @file
 * Views dynamic fields filter handler
 */

/**
 * Simple filter to pick and choose the fields to show in the view.
 */
class views_handler_filter_dynamic_fields extends views_handler_filter {

  var $no_single = TRUE;
  /**
   * Array to store combined fields as an associative array (parent => child)
   * E.g. using table style plugin, one can set the column of one field as that of another so they appear as one column in the output
   * @var ArrayIterator
   */
  var $combined_fields = array();
  /**
   * Array to store the fields to be used in the exposed filter
   * @var ArrayIterator
   */
  var $view_fields = array();

  function has_extra_options() { return TRUE; }

  function option_definition() {
    $options = parent::option_definition();
    // Fields to display in exposed filter
    $options['filterable_fields'] = array('default' => array());
    // Fields already displayed when user does no filtering http://drupal.org/node/1039760
    $options['defaults_filterable_fields'] = array('default' => array());
    // Enable draggable ordering of fields http://drupal.org/node/935910
    $options['sortability'] = array('default' => FALSE);
    // Otherwise choose between checkboxes or select. http://drupal.org/node/1039760
    $options['checkboxes'] = array('default' => TRUE);
    // Boolean to control display of Exclusion mode
    $options['reverse'] = array('default' => 0);
    // Label for the Exclusion mode
    $options['reverse_label'] = array('default' => '');
    return $options;
  }

  /**
   * Pre-set the filter fields and combined fields arrays
   * @see sites/all/modules/contrib/views/handlers/views_handler_filter#init()
   */
  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->view_fields = $view->get_items('field');
    $this->remove_combined_fields($view);
    // Select all fields by default when setting up a new filter
    $this->options['filterable_fields'] = empty($this->options['filterable_fields']) ? array_keys($this->view_fields) : $this->options['filterable_fields'];
  }

  /**
   * Prune fields to be displayed in the filter and capture combined fields separately
   * @param unknown_type $view
   */
  protected function remove_combined_fields($view) {
    $this->combined_fields = array();
    // @todo Test combined fields for D7
    $style_columns = isset($view->display_handler->default_display->options['style_options']['columns']) ? $view->display_handler->default_display->options['style_options']['columns'] : NULL;
    if (is_array($style_columns)) {
      foreach ($style_columns as $key => $name) {
        if (isset($this->view_fields[$key]) && $key != $name) {
          unset($this->view_fields[$key]);
          // The $key field will be acted upon if the $name (parent) field is acted upon by the user
          $this->combined_fields[$name] = $key;
        }
      }
    }
  }

  /**
   * Provide an 'exclusion mode' option
   * @see sites/all/modules/contrib/views/includes/views_handler#extra_options_form()
   */
  function extra_options_form(&$form, &$form_state) {
    // Get all non-excluded fields
    $view_fields = $this->view_fields;
    foreach ($view_fields as $field_name => $field) {
      if (empty($field['label'])) {
        $field_label = $field['id'];
      }
      else {
        $field_label = $field['label'];
      }
      // Do not allow pre-configured excluded fields in the filter
      if (isset($field['exclude']) && !$field['exclude']) {
        $fields[$field_name] = $field_label;
      }
      else {
        $fields_excluded[$field_name] = $field_label;
      }
    }

    $form['sortability'] = array(
      '#type' => 'checkbox',
      '#default_value' => $this->options['sortability'],
      '#title' => t('Enable sortability'),
      '#description' => t('Allow users to sort all the fields in the view. If selected the fields will be listed as a draggable table with checkboxes and all the fields will be included.'),
    );

    $form['checkboxes'] = array(
      '#type' => 'checkbox',
      '#default_value' => $this->options['checkboxes'],
      '#title' => t('List filters in checkboxes'),
      '#description' => t('If selected the fields will be listed as checkboxes, otherwise they will be listed in a select box. Not used if sortability is enabled.'),
    );

    $form['filterable_fields'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Fields'),
      '#description' => t('Select the fields that you want to make available for filtering. Unchecked fields will not be filterable and will always show up in the view.'),
      '#options' => $fields,
      '#default_value' => $this->options['filterable_fields'],
    );

    $form['defaults_filterable_fields'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Default displayed fields'),
      '#description' => t('Select the fields that you want displayed by default in the view when no filtering is applied. Any explicitly unchecked fields in the list above will also be included as permanent fields. Leave empty to choose all fields.'),
      '#options' => $fields,
      '#default_value' => $this->options['defaults_filterable_fields'],
    );

    // Fields may been excluded from the view in field settings
    if ($fields_excluded) {
      $form['filterable_fields_excluded'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Excluded fields'),
        '#options' => $fields_excluded,
        '#description' => t('You have configured these fields to be excluded from the view, hence they will not appear as filters.'),
        '#disabled' => TRUE,
      );
    }

    $form['reverse'] = array(
      '#type' => 'checkbox',
      '#default_value' => $this->options['reverse'],
      '#title' => t('Provide exclusion mode'),
      '#description' => t('Provide a checkbox to reverse the selection from inclusion to exclusion'),
    );
    $form['reverse_label'] = array(
      '#type' => 'textfield',
      '#default_value' => $this->options['reverse_label'],
      '#title' => t('Label for exclusion fields options'),
      '#size' => 40,
    );
  }

  /**
   * Provide a list of all fields that are for display
   */
  function value_form(&$form, &$form_state) {
    // All the non-permanently excluded fields of the view that we will display in the exposed filter
    $all_fields = array();
    // Just the names of all the non-permanently excluded fields
    $field_names = array();
    $view_fields = $this->view_fields;
    $combined_fields = $this->combined_fields;
    $views_fields_order = array_keys($view_fields);
    // The final fields that are part of the filter after going through a pruning process
    $displayed_field_names = array();

    $filterable_fields = array_filter($this->options['filterable_fields']);
    $defaults_filterable_fields = array_filter($this->options['defaults_filterable_fields']);

    // Create fields to be displayed as part of the filter
    foreach ($view_fields as $field_name => $field) {
      // Apply filterable fields options removing any field not in the set of filterable fields
      if (!empty($filterable_fields) && !isset($filterable_fields[$field_name])) {
        continue;
      }

      if (!isset($_SESSION[$this->view->name]['perm_exc']) && $field['exclude']) {
        // Fields already marked as 'exclude from display' in the original view should be omitted
        $_SESSION[$this->view->name]['perm_exc'][$field_name] = $field_name;
        $perm_execs++;
        continue;
      }
      if (!isset($_SESSION[$this->view->name]['perm_exc'][$field_name])) {
        // Use the field id as label if label is empty
        if (empty($field['label'])) {
          $all_fields[$field_name] = $field['id'];
        }
        else {
          $all_fields[$field_name] = $field['label'];
        }
        $field_names[] = $field_name;
        // Get fields to display in the plain (unfiltered) view
        if (isset($defaults_filterable_fields[$field_name]) && $defaults_filterable_fields[$field_name] == $field_name) {
          $field_names_defaults[$field_name] = $field_name;
        }
      }
    }
    // Also add any combined fields to the default fields to display as configured
    foreach ($combined_fields as $c_parent => $c_child) {
      if ($defaults_filterable_fields[$c_parent] == $c_parent) {
        $field_names_defaults[$c_parent] = $c_parent;
      }
    }
    // Initialize the permanent exclusion array if not set
    if (!isset($_SESSION[$this->view->name]['perm_exc'])) {
      $_SESSION[$this->view->name]['perm_exc'] = array();
    }
    // Display all the filter fields in the plain view if no explicit defaults are set in the view
    $field_names_defaults = empty($field_names_defaults) ? $all_fields : $field_names_defaults;

    //$this->options['expose']['single'] = FALSE; // otherwise views_handler_filter will change this to use 'select'

    if($form_state['exposed']) {
      // Get submitted filter data if any
      $exposed_input = $this->view->get_exposed_input();
      if (isset($exposed_input[$this->options['expose']['identifier']])) {
        $exposed_input_fields = $exposed_input[$this->options['expose']['identifier']];
        $exposed_field_names =  isset($exposed_input['field_names']) ? unserialize($exposed_input['field_names']) : $field_names;
      }
      $field_order = array();
      // If sortability has not been enabled, display the fields in as checkboxes/select list
      if (!$this->options['sortability']) {
        $form['value'] = array(
          '#type' => 'checkboxes',
          '#no_convert' => $this->options['checkboxes'],
          '#title' => $this->options['expose']['label'] ? t($this->options['expose']['label']) : t('Fields'),
          '#default_value' => $field_names_defaults,
          '#options' => $all_fields, // @todo all_fields dont have permanently excluded fields
          '#description' => t('Select the fields to display in the view'),
        );
        // TODO: Why is the default selection list not showing up here? (no sortability)
        if ($this->options['checkboxes'] && isset($exposed_input_fields)) {
          $form['value']['#value'] = $field_names_defaults;
        }
        else if ($this->options['checkboxes'] && !isset($exposed_input_fields)) {
            $form['value']['#value'] = $all_fields;
        }
        
        $displayed_field_names = $field_names;
      }
      else {
        // For a view that has already been filtered by the user
        if (!empty($exposed_input_fields)) {
          $delta = count($exposed_input_fields);
          $key = 0;
          // Regenerate the sortable table
          foreach ($exposed_input_fields as $oldid => $oldvalue) {
            $def_check = (isset($oldvalue['check']) && $oldvalue['check']) || empty($all_fields[$exposed_field_names[$oldid]]);
            $def_sort = $oldvalue['sort'] ? $oldvalue['sort'] : $key;
            $def_disabled = empty($all_fields[$exposed_field_names[$oldid]]);
            $def_title = $all_fields[$exposed_field_names[$oldid]] ? $all_fields[$exposed_field_names[$oldid]] : $view_fields[$exposed_field_names[$oldid]]['label'];
            $def_desc = empty($all_fields[$exposed_field_names[$oldid]]) ? t('Always visible') : '';
            $form['value'][$key]['check'] = array('#type' => 'checkbox', '#value' => $def_check, '#disabled' => $def_disabled, '#description' => $def_desc);
            $form['value'][$key]['title'] = array('#markup' => $def_title);
            $form['value'][$key]['sort'] = array('#type' => 'weight', '#delta' => $delta, '#default_value' =>  $def_sort);
            $displayed_field_names[] = $exposed_field_names[$oldid];
            $key++;
          }
        }
        // First time (or no exposed filter data)
        else {
          $delta = count($view_fields) - count($combined_fields);
          $key = 0;
          // Create the sortable table
          foreach ($view_fields as $field_title => $field_info) {
            // Skip over permanently excluded fields.
            if (isset($field_info['exclude']) && $field_info['exclude']) {
              continue;
            }
            // Display only default filterable fields if there is no exposed input
            $def_check = in_array($field_title, array_keys($field_names_defaults)) || empty($all_fields[$field_title]);
            $def_sort = $key;
            $def_desc = empty($all_fields[$field_title]) ? t('Always visible') : '';
            $def_disabled = empty($all_fields[$field_title]);
            $def_title = (isset($all_fields[$field_title]) && !empty($all_fields[$field_title])) ? $all_fields[$field_title] : $view_fields[$field_title]['label'];
            $form['value'][$key]['check'] = array('#type' => 'checkbox', '#value' => $def_check, '#disabled' => $def_disabled, '#description' => $def_desc);
            $form['value'][$key]['title'] = array('#markup' => $def_title);
            $form['value'][$key]['sort'] = array('#type' => 'weight', '#delta' => $delta, '#default_value' =>  $def_sort);
            $field_order[$key] = $def_sort;
            $displayed_field_names[] = $field_title;
            $key++;
          }
        }
        // Theme the draggable sort table
       $form['value']['#theme'] = 'views_dynamic_fields_sort_filter_fields';
      }

      $form['value']['#tree'] = TRUE;
      $form['value']['#title'] = $this->options['expose']['label'] ? t($this->options['expose']['label']) : t('Fields');
      // Store the fields being displayed in the filter for use as form data when processing filter submissions
      $form['field_names'] = array('#type' => 'hidden', '#value' => serialize($displayed_field_names));

      $form['combined'] = array(
        '#type' => 'hidden',
        '#value' => serialize($combined_fields),
        '#weight' => 9,
      );
      if ($this->options['reverse']) {
        $form['reverse'] = array(
          '#type' => 'checkbox',
          '#default_value' => $this->options['reverse'],
          '#title' => $this->options['reverse_label'] ? t($this->options['reverse_label']) : t('Show fields to be excluded'),
          '#description' => t('Check this to exclude the selected fields from the view'),
        );
      }
    }
    else {
      // Ensure there is something in the 'value'.
      $form['value'] = array(
        '#type' => 'value',
        '#value' => NULL
      );
    }
  }

  /**
   * Exclude fields from the view before the query is run
   * @see sites/all/modules/contrib/views/includes/views_handler#pre_query()
   */
  function pre_query() {
    // Fields submitted by user in the exposed filter
    $exposed_input = $this->view->get_exposed_input();
    
    if (isset($exposed_input[$this->options['expose']['identifier']])) {
      $exposed_input_fields = $exposed_input[$this->options['expose']['identifier']];
    }

    $defaults_filterable_fields = array_filter($this->options['defaults_filterable_fields']);
    $filterable_fields = array_filter($this->options['filterable_fields']);

    // Get all fields
    foreach ($this->view->field as $field_name => $field) {
      $field_names[] = $field_name;
      // Default view when no filtering is applied
      if (empty($exposed_input_fields)) {
//        if (count($defaults_filterable_fields) && $defaults_filterable_fields[$field_name] != $field_name && isset($filterable_fields[$field_name])) {
        if (!in_array($field_name, array_keys($defaults_filterable_fields)) && isset($filterable_fields[$field_name])) {
          $this->view->field[$field_name]->options['exclude'] = 1;
        }
      }
    }
    if (!empty($exposed_input_fields)) {
      // Exclude these fields
      $extra_exc = array(); // Holder for any combined fields we need to remove if their parent combined field is removed
      $combined_fields = isset($exposed_input['combined']) ? unserialize($exposed_input['combined']) : array();
      if (!$this->options['sortability']) {
        $this->pre_query_no_sort($exposed_input, $field_names, $combined_fields);
      }
      else {
        $this->pre_query_sort($exposed_input, $field_names, $combined_fields);
      }
    }
  }

  /**
   * Handle pre_query processing when sortability is not enabled
   */
  private function pre_query_no_sort($exposed_input, $field_names, $combined_fields) {
//ADDED from https://drupal.org/files/views_dynamic_fields-singleselect.diff in regards to https://drupal.org/node/1831858
    // When using single select the field is not in an array
    $dyfield = $exposed_input[$this->options['expose']['identifier']];
    if (!is_array($dyfield)) $dyfield = array($dyfield);
    // If exclusion mode is selected
    if (isset($exposed_input['reverse']) && $exposed_input['reverse']) {
      //$fields_to_exclude = $exposed_input[$this->options['expose']['identifier']];
      $fields_to_exclude = $dyfield;
    }
    else {
      //$fields_to_exclude = array_diff($field_names, $exposed_input[$this->options['expose']['identifier']]);
      $fields_to_exclude = array_diff($field_names, $dyfield);
//END
    }
    $filterable_fields = array_filter($this->options['filterable_fields']);
    // Also include/exclude the fields combined into any of the $exposed_input fields
    foreach($fields_to_exclude as $key => $efield) {
      // Add the combined field to the exclusion array if parent is filterable and excluded
      if (isset($combined_fields[$efield]) && $this->options['filterable_fields'][$efield] !== 0) {
        $fields_to_exclude[$combined_fields[$efield]] = $combined_fields[$efield];
      }
      // On the other hand, the combined field should be include in the view if the parent is
      if (in_array($efield, $combined_fields)) {
        unset($fields_to_exclude[$key]);
      }
    }
    // Mark the fields to be excluded to the view object
    foreach ($fields_to_exclude as $field_to_exclude) {
      // Exclude only the fields chosen as excludable in filter options
//ADDED from https://drupal.org/files/views_dynamic_fields-singleselect.diff in regards to https://drupal.org/node/1831858
      //if (empty($filterable_fields) || $this->options['filterable_fields'][$field_to_exclude] !== 0) {
      if (empty($filterable_fields) || (isset($this->options['filterable_fields'][$field_to_exclude]) && $this->options['filterable_fields'][$field_to_exclude] !== 0)) {
//END
        $this->view->field[$field_to_exclude]->options['exclude'] = 1;
      }
    }
  }

  /**
   * Handle pre_query processing when sortability is enabled
   */
  private function pre_query_sort($exposed_input, $field_names, $combined_fields) {
    $exposed_input_fields = $exposed_input[$this->options['expose']['identifier']];
    
    // Fields displayed in the filter
    $form_field_names = isset($exposed_input['field_names']) ? unserialize($exposed_input['field_names']) : $field_names;
    $orig_field_exposed = $order_includes = $order_excludes = array();
    foreach($exposed_input_fields as $id => $info) {
      $orig_field_exposed[$info['sort']] = $form_field_names[$id];
      // Unset fields that haven't been selected

      if (!isset($info['check']) || $info['check'] != 1) {
        unset($exposed_input_fields[$id]);
        $order_includes[$info['sort']] = $form_field_names[$id];
      }
      else {
        $order_excludes[$info['sort']] = $form_field_names[$id];
      }
    }
    // Sorted order of fields for display
    ksort($orig_field_exposed);
    ksort($order_includes);
    ksort($order_excludes);
    // If exclusion mode is selected
    if (isset($exposed_input['reverse']) && $exposed_input['reverse']) {
      $fields_to_exclude = $order_excludes;
    }
    else {
      $fields_to_exclude = $order_includes;
    }
    // Also include/exclude the fields combined into any of the $exposed_input fields
    foreach($fields_to_exclude as $key => $efield) {
      // Add the combined field to the exclusion array if parent is filterable and excluded
      if (isset($combined_fields[$efield]) && $this->options['filterable_fields'][$efield] !== 0) {
        $fields_to_exclude[$combined_fields[$efield]] = $combined_fields[$efield];
      }
      // On the other hand, the combined field should be include in the view if the parent is
      if (in_array($efield, $combined_fields)) {
        unset($fields_to_exclude[$key]);
      }
    }
    // Mark the fields to be excluded to the view object
    foreach ($fields_to_exclude as $field_to_exclude) {
      // Exclude only the fields chosen as excludable in filter options
      if (empty($filterable_fields) || $this->options['filterable_fields'][$field_to_exclude] !== 0) {
        $this->view->field[$field_to_exclude]->options['exclude'] = 1;
      }
    }
    // Reorder the view's fields
    $view_fields = $this->view->field;
    $view_fields_ordered = array();
    foreach ($orig_field_exposed as $weight => $field_title) {
      $view_fields_ordered[$field_title] = $view_fields[$field_title];
      unset($view_fields[$field_title]);
    }
    // Any still remaning? (hidden fields, already excluded fields)
    if (!empty($view_fields)) {
      foreach($view_fields as $field_title => $field_info) {
        $view_fields_ordered[$field_title]  = $field_info;
      }
    }
    $this->view->field = $view_fields_ordered;
  }


  /**
   * Do nothing in the query. This function needs to be defined and left to do nothing.
   * @see sites/all/modules/contrib/views/handlers/views_handler_filter#query()
   */
  function query() {
  }

}
