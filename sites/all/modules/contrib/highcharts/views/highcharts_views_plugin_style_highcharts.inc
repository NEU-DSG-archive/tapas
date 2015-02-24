<?php

/**
 * @file
 * Contains the highcharts style plugin.
 */

/**
 * Style plugin uses views ui to configure views data for rendering highcharts.
 *
 * @ingroup views_style_plugins
 */
class highcharts_views_plugin_style_highcharts extends views_plugin_style {

  /**
   * Chart templates.
   *
   * @var array
   */
  var $chart_templates = array();

  /**
   * Initialize the style plugin.
   */
  function init(&$view, &$display, $options = NULL) {
    parent::init($view, $display, $options);

    $this->chart_templates = $this->getChartTemplates();
  }

  /**
   * Set default options.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['format'] = array(
      'contains' => array(
        // Do not set a default chart series type.
        'chart_template' => array('default' => NULL),
      ),
    );

    $options['attributes'] = array(
      'contains' => array(
        'class' => array('default' => ''),
        'style' => array('default' => ''),
      ),
    );

    // Do not set default values for 'object'. These are advanced options which
    // override the selected template only if a value is set.
    // $options['object'] = array();

    return $options;
  }

  /**
   * Provide a form to edit options for this plugin.
   */
  function options_form(&$form, &$form_state) {
    // Chart templates for basic settings.
    foreach ($this->chart_templates as $key => $chart_template) {
      $chart_templates[$key] = $chart_template['title'];
    }

    // Get fields for selection.
    $chart_handlers = $this->display->handler->get_handlers('field');
    $chart_fields = array();
    foreach ($chart_handlers as $key => $value) {
      $chart_fields[$key] = $value->options['ui_name'] ? $value->options['ui_name'] : $value->options['field'];
    }

    // Basic settings.
    $form['format'] = array(
      '#title' => t('Basic settings'),
      '#type' => 'fieldset',
    );
    $form['format']['chart_template'] = array(
      '#title' => t('Chart Template'),
      '#type' => 'select',
      '#options' => $chart_templates,
      '#multiple' => FALSE,
      '#default_value' => $this->options['format']['chart_template'],
      '#empty_option' => t('None (use advanced settings only)'),
      '#id' => 'format_chart_type'
    );

    // Get all chart template settings forms, each visible on selection.
    // TODO get value via ajax callback on select, rather than loading them all.
    foreach ($this->chart_templates as $key => $chart_template) {
      $chart_templates[$key] = $chart_template['title'];

      // Add template settings container, keyed by template.
      $form['format'][$key] = array(
        '#title' => t('%template settings', array('%template' => $chart_template['title'])),
        '#type' => 'fieldset',
        '#states' => array(
          'visible' => array(
           ':input[name="style_options\\[format\\]\\[chart_template\\]"]' => array('value' => $key),
          ),
        ),
      );
      // Add each template's settingsForm elements to it's container.
      $this->getChartTemplateSettingsForm($key, $form['format'][$key], $form_state);
      $elements = element_children($form['format'][$key]);
      if (empty($elements)) {
        $form['format'][$key]['#description'] = t('No settings for this chart template.');
      }
    }

    // HTML attributes.
    // @see theme_highcharts_chart()
    $form['attributes'] = array(
      "#title" => t('Container attributes'),
      "#type" => "fieldset",
    );

    $form['attributes']['class'] = array(
      "#title" => t('Class'),
      "#type" => 'textfield',
      '#default_value' => $this->options['attributes']['class'],
    );

    $form['attributes']['style'] = array(
      "#title" => t('Style'),
      "#type" => 'textfield',
      '#default_value' => $this->options['attributes']['style'],
    );

    // @todo add override options object form.
    // Advanced settings (Highcharts options object).
    $form['object'] = array(
      '#title' => t('Advanced settings'),
      '#description' => t('Warning, only modify these if you know what you are doing. These settings will override the selected chart type callback.'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    // Bring in object form, passing the form default values.
    $object_form = highcharts_views_object_form($this->options['object']);
    $form['object'] += $object_form;
  }

  /**
   * Validate the options form.
   * @todo what can we really validate here?
   */
  function options_validate(&$form, &$form_state) {

  }

  /**
   * Handle any special handling on the validate form.
   */
  function options_submit(&$form, &$form_state) {

  }

  /**
   * Gets chart templates.
   */
  private function getChartTemplates() {
    // Allow modules to add chart templates.
    return module_invoke_all('highcharts_views_charts');
  }

  /**
   * Instantiates a chart template object.
   *
   * @param string $chart_template
   *   A chart template class callback from hook_highcharts_views_charts().
   *
   * @return ViewsHighchartsOptions|stdClass
   *   An instantiated Highcharts options object, or empty stdClass object.
   */
  private function loadChartTemplate($chart_template = NULL) {
    $options = new stdClass();
    if (!empty($chart_template) && $this->validateChartTemplate($chart_template)) {
      $class = new $chart_template($this->view, $options);
      $options = $class->render();
    }
    return $options;
  }

  /**
   * Gets chart a template views options form.
   *
   * @param string $chart_template
   *   A chart template class callback from hook_highcharts_views_charts().
   * @param array $form
   *   The $form param of views_plugin::options_form.
   * @param type $form_state
   *   The $form_state param of views_plugin::options_form.
   */
  private function getChartTemplateSettingsForm($chart_template, &$form, &$form_state) {
    if (!empty($chart_template) && $this->validateChartTemplate($chart_template)) {
      $options = new stdClass();
      $class = new $chart_template($this->view, $options);
      $class->settingsForm($form, $form_state);
    }
  }

  /**
   * Validates a chart template class, and logs faliures.
   *
   * @param string $chart_template
   *   A chart template class callback, which must be an instance of
   *   ViewsHighchartsOptions and declared in hook_highcharts_views_charts().
   *
   * @return bool
   *   Whether or not the class is a valid chart template.
   */
  private function validateChartTemplate($chart_template) {
    $messages = array();

    // Ensure class is declared in hook_highcharts_views_charts().
    if (empty($this->chart_templates[$chart_template])) {
      $messages[] = 'The chart template class @chart_template must be declared in hook_highcharts_views_charts().';
    }

    // Support class object instantiation with defined parameters.
    if (empty($messages)) {
      // Ensure class extends ViewsHighchartsOptions.
      $reflection = new ReflectionClass($chart_template);
      if ($reflection->isSubclassOf('ViewsHighchartsOptions')) {
        return TRUE;
      }
      else {
        $messages[] = 'The chart template class @chart_template can not be instantiated.';
      }
    }

    if (!empty($messages)) {
      watchdog('views_highcharts', implode(' ', $messages), array('@chart_template' => $chart_template));
      return FALSE;
    }
  }

  /**
   * Hijack rendering Views data for Highcharts display.
   *
   * See ViewsHighchartsOptions::getFieldLabels() for rationale.
   */
  function render() {
    // Initialize the Highcharts options object from view options_form chart
    // template settings.
    $options = $this->loadChartTemplate($this->options['format']['chart_template']);

    // Override the Highcharts options object returned by the selected chart
    // type callback, using the view options_form() settings.
    if (!empty($this->options['object'])) {
      $options = highcharts_views_options_override($options, $this->options['object']);
    }

    // Highcharts container HTML attributes.
    // Note this option differs from the built-in Highcharts options object
    // settings (height, className, style, etc), which sets those attributes on
    // the SVG element. The container height attribute keeps the chart from
    // overflowing beyond other JS-calculated containers such as Homebox.
    $attributes = $this->options['attributes'];

    return highcharts_render($options, $attributes);
  }

}
