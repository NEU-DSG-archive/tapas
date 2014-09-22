<?php

/**
 * @file
 * Contains classes for Highcharts Views options.
 */

/**
 * Base class for Highcharts Views options.
 */
abstract class ViewsHighchartsOptions extends HighchartsOptions {

  /**
   * The view object.
   *
   * @var view
   */
  public $view;

  /**
   * The settings form defaults from views_plugin_style options.
   *
   * @var array
   */
  protected $settingsFormDefaults;

  /**
   * Builds a Views Highcharts options template object.
   *
   * Note do not @inheritdoc because the params differ from parent class.
   *
   * @param array $options
   *   The $options param of HighchartsOptions::__construct().
   * @param view $view
   *   A Views view object. This lets us call additional methods on the view.
   *
   * @return ViewsHighchartsOptions
   */
  public function __construct(view $view, \stdClass $options = NULL) {
    $this->view = $view;
    // Initializing the style plugin is necessary to load settings form defaults
    // from plugin options on the chart template settings form.
    $this->view->init_style();
    $this->settingsFormDefaults = $this->view->style_plugin->options['format'][get_called_class()];
    parent::__construct($this->data(), $options);
  }

  /**
   * Adds Highcharts options template settings to the Views UI.
   */
  abstract public function settingsForm(&$form, &$form_state);

  /**
   * Gets field labels on this view, as an efficient alternative to render().
   *
   * This function exists because using render() to get labels from
   * $view->style_plugin->render_fields() is very taxing on 10000+ rows.
   *
   * @return array
   *   An array of field labels keyed by Views field ID, or an empty array.
   */
  protected function getFieldLabels() {
    $labels = array();
    if (!empty($this->view->field)) {
      foreach ($this->view->field as $key => $field) {
        $labels[$key] = $field->options['label'];
      }
    }
    return $labels;
  }

  /**
   * Gets raw field values.
   *
   * See rationale in getFieldLabels().
   *
   * @return array
   *   An array of view result rows, containing Views field values keyed by
   *   Views field ID, or an empty array. These raw field values are exactly
   *   like their result row object element counterparts returned by
   *   views_get_view_result(), except these are keyed by Views field ID so
   *   labels can be matched to results without render().
   *
   * @see ViewsHighchartsOptions::getRenderedFields()
   */
  protected function getFieldValues() {
    $display = $this->view->current_display;
    $values = &drupal_static(__FUNCTION__);
    if (!isset($values[$display])) {
      $values = array();
      foreach (array_keys($this->view->result) as $row_index) {
        foreach (array_keys($this->view->field) as $field_id) {
          $values[$display][$row_index][$field_id] = $this->view->style_plugin->get_field_value($row_index, $field_id);
        }
      }
    }
    return $values[$display];
  }

  /**
   * Gets rendered field values.
   *
   * There are cases where we do want the rendered fields, and this offeres that
   * while still providing a key for labels to match results.
   *
   * @return array
   *   An array of view result rows, containing rendered Views fields keyed by
   *   Views field ID, or an empty array.
   *
   * @see ViewsHighchartsOptions::getFieldValues()
   */
  protected function getRenderedFields() {
    $values = &drupal_static(__FUNCTION__);
    if (!isset($values)) {
      $values = array();
      foreach (array_keys($this->view->result) as $row_index) {
        foreach (array_keys($this->view->field) as $field_id) {
          $values[$row_index][$field_id] = $this->view->render_field($field_id, $row_index);
        }
      }
    }
    return $values;
  }

  /**
   * Gets all Views fields allowed values, per bundle.
   *
   * @return array
   *   An array of allowed field values, keyed by field ID, then entity type,
   *   then bundle. Keys of the allowed values array are the raw stored values,
   *   values of the array are the display labels. See list_allowed_values() and
   *   taxonomy_allowed_values() for examples.
   *
   * @todo get this without having to depend on field_analytics.module.
   */
  protected function getFieldAllowedValues() {
    $values = &drupal_static(__FUNCTION__);
    if (!isset($values)) {
      // If fields are not initialized on the view yet (the options form), get
      // the field handler directly.
      $fields = !empty($this->view->field) ? $this->view->field : $this->view->display_handler->get_handlers('field');;
      $values = array();
      foreach($fields as $field_id => $field) {
        if (isset($field->field_info) && $field_info = $field->field_info) {
          foreach($field_info['bundles'] as $entity_type => $bundles) {
            foreach ($bundles as $bundle) {
              $instance = field_info_instance($entity_type, $field_info['field_name'], $bundle);
//ADDED from https://drupal.org/files/highcharts_views.options.inc_.patch in regard to https://drupal.org/node/1992206
//              $values[$field_id][$entity_type][$bundle] = field_analytics_get_allowed_values($instance);
              $values[$field_id][$entity_type][$bundle] = $this->field_get_allowed_values($instance);
//END
            }
          }
        }
      }
    }
    return $values;
  }
//ADDED from https://drupal.org/files/highcharts_views.options.inc_.patch in regard to https://drupal.org/node/1992206

  /**
   * Gets allowed values for all supported fields.
   *
   * @param array $instance
   *   A field instance.
   *
   * @return array|false
   *   An array of field allowed values, or FALSE.
   */
  function field_get_allowed_values($instance) {
    // Check allowed values.
    $field = field_info_field($instance['field_name']);

    // Support all core fields with allowed values.
    switch ($field['type']) {
      case 'list_integer':
      case 'list_float':
      case 'list_text':
      case 'list_boolean':
        $callback = 'list_allowed_values';
        break;
      case 'taxonomy_term_reference':
        $callback = 'taxonomy_allowed_values';
        break;
    }

    $allowed_values = FALSE;
    if (isset($callback) && function_exists($callback)) {
      $allowed_values = $callback($field, $instance, $instance['entity_type']);
    }

    return $allowed_values;
  }
//END
  /**
   * Gets a result count across all rows for all Views fields allowed values.
   *
   * @return array
   *   An array of views result counts of all allowed field values for all Views
   *   fields (for now only Field API fields are supported), keyed by field ID,
   *   then entity type, then bundle, then field value ID.
   *   - field ID: these are the array keys of $this->view->field
   *   - entity type: the entity type of the bundle containing the field
   *   - bundle: the bundle containing the field
   *   - field value ID: the ID of the field value (for example, on a
   *     taxonomy_term_reference field the value ID is the term ID)
   *
   * @todo add a $field_name level between $bundle and $value_id, for more
   *   options between multiple fields on the same bundle, in case they happen
   *   to share allowed field value IDs.
   *
   * @todo find a better way to get entity_type and bundle from views result
   *   row. This may actually not even be accurate, because fields can come from
   *   multiple bundles in the same row. Also, there may be other Views fields
   *   that are not field API fields, each with their own concept of allowed
   *   values. If we want to do this generically, we probably need to implement
   *   plugins, like views_handler_filter_term_node_tid.inc. See
   *   views_handler_filter_term_node_tid::value_form().
   */
  protected function getFieldAllowedValuesResultCount() {
    $totals = &drupal_static(__FUNCTION__);
    if (!isset($totals)) {
      $totals = array();
      foreach (array_keys($this->view->result) as $row_index) {
        if (isset($this->view->result[$row_index]->_field_data)) {
          $keys = reset($this->view->result[$row_index]->_field_data);
          $entity_type = $keys['entity_type'];
          $entity = $keys['entity'];
          list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

          // For now only support fieldable bundles.
          if ($bundle) {
            $allowed_values = $this->getFieldAllowedValues();
            foreach (array_keys($this->view->field) as $field_id) {
              $key_exists = NULL;
              $parents = array($field_id, $entity_type, $bundle);
              drupal_array_get_nested_value($allowed_values, $parents, $key_exists);
              if ($key_exists) {
                // Get values from this field in this views row.
                $values = $this->view->style_plugin->get_field_value($row_index, $field_id);
                foreach($values as $value_field) {
                  $value_id = reset($value_field);
                  // We may want to do various calculations here. Count for now.
                  // Autovivify the array keys if necessary.
                  $totals[$field_id] = isset($totals[$field_id]) ? $totals[$field_id] : array();
                  $totals[$field_id][$entity_type] = isset($totals[$field_id][$entity_type]) ? $totals[$field_id][$entity_type] : array();
                  $totals[$field_id][$entity_type][$bundle] = isset($totals[$field_id][$entity_type][$bundle]) ? $totals[$field_id][$entity_type][$bundle] : array();
                  // Increment the field value ID count.
                  $totals[$field_id][$entity_type][$bundle][$value_id] = isset($totals[$field_id][$entity_type][$bundle][$value_id]) ? $totals[$field_id][$entity_type][$bundle][$value_id] : 0;
                  $totals[$field_id][$entity_type][$bundle][$value_id] ++;
                }
              }
            }
          }
        }
      }
    }
    return $totals;
  }

}

/**
 * Implements CallbackHighchartsViewsCharts.
 *
 * Views extension of Highcharts basic pie template.
 */
class ViewsHighchartsOptionsPieBasic extends ViewsHighchartsOptions {

  /**
   * (@inheritdoc)
   *
   * Adds pie-specific options.
   */
  public function settingsForm(&$form, &$form_state) {
    $form['selected'] = array(
      '#type' => 'select',
      '#title' => t('Selected'),
      '#options' => $this->getFieldAllowedValues(),
      '#empty_option' => t('-none-'),
      '#description' => t('Specify which pie slice will display as "selected".'),
      '#default_value' => $this->settingsFormDefaults['selected'],
    );
  }

  protected function data() {
    $data = array();
    $allowed_values = $this->getFieldAllowedValues();
    $totals = $this->getFieldAllowedValuesResultCount();
    // Format data array expected by Highcharts for pie chart types.
    foreach ($totals as $field_id => $entity_types) {
      foreach ($entity_types as $entity_type => $bundles) {
        foreach ($bundles as $bundle => $value_ids) {
          foreach ($value_ids as $value_id => $value) {
            $label = $allowed_values[$field_id][$entity_type][$bundle][$value_id];
            if ($value_id == $this->settingsFormDefaults['selected']) {
              $selected = new stdClass();
              $selected->name = $label;
              $selected->y = $value;
              $selected->sliced = TRUE;
              $selected->selected = TRUE;
              $data[] = $selected;
            }
            else {
              $data[] = array($label, $value);
            }
          }
        }
      }
    }
    return $data;
  }

  public function render() {
    // Instantiate existing class, but with our parsed data from the view.
    $template = new HighchartsOptionsPieBasic($this->data, $this->options);
    return $template->render();
  }

}

/**
 * Implements CallbackHighchartsViewsCharts.
 *
 * Views extension of Highcharts bar stacked template.
 */
class ViewsHighchartsOptionsBarStacked extends ViewsHighchartsOptions {

  public function settingsForm(&$form, &$form_state) {
  }

  protected function data() {
    // @todo add example. Also see ViewsHighchartsOptionsPieBasic::data().
    return array();
  }

  public function render() {
    $template = new HighchartsOptionsBarStacked($this->data, $this->options);
    return $template->render();
  }

}

/**
 * Implements CallbackHighchartsViewsCharts.
 *
 * Views extension of Highcharts bar stacked template.
 */
class ViewsHighchartsOptionsBarBasic extends ViewsHighchartsOptions {

  public function settingsForm(&$form, &$form_state) {
  }

  protected function data() {
    // @todo add example. Also see ViewsHighchartsOptionsPieBasic::data().
    return array();
  }

  public function render() {
    $template = new HighchartsOptionsBarBasic($this->data, $this->options);
    return $template->render();
  }

}

/**
 * Implements CallbackHighchartsViewsCharts.
 *
 * Views extension of Highcharts basic line template.
 */
class ViewsHighchartsOptionsLineBasic extends ViewsHighchartsOptions {

  public function settingsForm(&$form, &$form_state) {
  }

  public function data() {
    // @todo add example. Also see ViewsHighchartsOptionsPieBasic::data().
    return array();
  }

  public function render() {
    $template = new HighchartsOptionsLineBasic($this->data, $this->options);
    return $template->render();
  }

}
