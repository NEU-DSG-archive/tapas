<?php

/**
 * @file
 *
 */

/**
 * Slider widget for range filtering.
 */
class FacetapiWidgetSlider extends FacetapiWidget {

  /**
   * JavaScript settings.
   *
   * @var array
   */
  protected $jsSettings = array(
    'makeSlider' => 1,
  );

  /**
   * Renders the form.
   */
  public function execute() {

    $facet = $this->facet->getFacet();
    $adapter = $this->build['#adapter'];
    // Get our searched values from the active item
    if (isset($facet)) {
      $searched_values = $adapter->getActiveItems($facet);
      if (!empty($searched_values)) {
        $searched_values = reset($searched_values);
        $values = $searched_values['value'];

        // Remove the two brackets
        // @todo Optimize this logic
        $values = str_replace('[', '', $values);
        $values = str_replace(']', '', $values);
        $values = explode(' TO ', $values);
        $slider_min_handle = $values[0];
        $slider_max_handle = $values[1];
        if ($slider_min_handle == $slider_max_handle) {
          return FALSE;
        }
      }
    }

    // Sets the key, captures the facet's render array.
    // We do not move this to the top since we do not want to
    // initialize a build if it is not needed.
    $this->key = $this->build['#facet']['name'];
    $facet = $this->facet->getFacet();
    $build = $this->facet->getBuild();
    $slider = reset($build);
    $slider_min = $slider['#range_min'];
    $slider_max = $slider['#range_max'];
    $slider_min_global = $slider['#global_range_min'];
    $slider_max_global = $slider['#global_range_max'];
    if ($slider_min == $slider_max) {
      // Remove the slider because no results can be given
      $this->build[$this->facet['field alias']] = NULL;
      return;
    }
    $this->jsSettings['sliderMin'] = $slider_min_global;
    $this->jsSettings['sliderMax'] = $slider_max_global;
    if (isset($slider_min_handle) && isset($slider_max_handle)) {
      $this->jsSettings['sliderMinHandle'] = $slider_min_handle;
      $this->jsSettings['sliderMaxHandle'] = $slider_max_handle;
    }
    else {
      $this->jsSettings['sliderMinHandle'] = $slider_min_global;
      $this->jsSettings['sliderMaxHandle'] = $slider_max_global;
    }

    if ($slider_max - $slider_min < 20  && $facet['map options']['field']['type'] != 'number_integer') {
      $this->jsSettings['sliderStep'] = ($slider_max - $slider_min) / 20;
    }
    else {
      $this->jsSettings['sliderStep'] = 1;
    }
    $this->build[$this->facet['field alias']] = drupal_get_form('facetapi_slider_widget_form_' . $this->facet['field alias'], $adapter, $this->build);
  }

  /**
   * Form builder function for the facet block.
   */
  public static function widgetForm($form, $form_state, $form_id, FacetapiAdapter $adapter, $build = array()) {
    $slider = array();
    if (isset($build['#facet']) && isset($build[$build['#facet']['field alias']]) && isset($build[$build['#facet']['field alias']][$build['#facet']['field']])) {
      $slider = $build[$build['#facet']['field alias']][$build['#facet']['field']];
    }
    $form['slider']['min'] = array(
      '#type' => 'textfield',
      '#title' => t('Min'),
      '#size' => 5,
      '#default_value' => isset($slider['#range_min']) ? $slider['#range_min'] : 0,
      '#attributes' => array(
        'class' => array('facetapi-slider-min'),
      ),
    );

    $form['slider']['max'] = array(
      '#type' => 'textfield',
      '#title' => t('Max'),
      '#size' => 5,
      '#default_value' => isset($slider['#range_max']) ? $slider['#range_max'] : 0,
      '#attributes' => array(
        'class' => array('facetapi-slider-max'),
      ),
    );
    $form['slider']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    );
    $exclude = array('q' => 1, 'page' => 1);
    if (isset($build['#facet']['field alias'])) {
      $exclude[$build['#facet']['field alias']] = 1;
    }
    $form['slider']['field_alias'] = array(
      '#type' => 'hidden',
      '#default_value' => isset($build['#facet']['field alias']) ? $build['#facet']['field alias'] : '',
    );
    $form['slider']['field_type'] = array(
      '#type' => 'hidden',
      '#default_value' => isset($build['#facet']['field type']) ? $build['#facet']['field type'] : '',
    );
    $form['slider']['get'] = array(
      '#type' => 'hidden',
      '#default_value' => json_encode(array_diff_key($_GET, $exclude)),
    );
    $form['slider']['path'] = array(
      '#type' => 'hidden',
      '#default_value' => $_GET['q'],
    );
    // Only add JS if we are on the actual search page.
    if (isset($build['#attributes'])) {
      $form['#attributes'] = $build['#attributes'];
      $form['slider']['#attached']['library'][] = array('system', 'ui.slider');
      $form['slider']['#attached']['js'][] = drupal_get_path('module', 'facetapi_slider') . '/facetapi_slider.js';
    }
    $form['#action'] = url('facetapi/widget/submit/' . $form_id . '/' . $adapter->getSearcher());
    return $form;
  }

  public static function widgetFormValidate($form, &$form_state) {
    //Nothing to do...
  }

  public static function widgetFormSubmit($form, &$form_state) {
    $adapter = $form['#facetapi_adapter'];
    $url_processor = $adapter->getUrlProcessor();
    $filter_key = $url_processor->getFilterKey();

    $query_string = (array) json_decode($form_state['values']['get'], TRUE);
    if (empty($query_string[$filter_key])) {
      $query_string[$filter_key] = array();
    }
    $my_field = rawurlencode($form_state['values']['field_alias']);
    // Remove any existing filters for my field.
    foreach ($query_string[$filter_key] as $idx => $filter) {
      $parts = explode(':', $filter, 2);
      if ($parts[0] == $my_field) {
        unset($query_string[$filter_key][$idx]);
      }
    }
    $form_state['values']['min'] = round($form_state['values']['min']);
    $form_state['values']['max'] = round($form_state['values']['max']);
    $query_string[$filter_key][] = $my_field . ':' . "[{$form_state['values']['min']} TO {$form_state['values']['max']}]";
    $form_state['redirect'] = array($form_state['values']['path'], array('query' => $query_string));
  }

}
