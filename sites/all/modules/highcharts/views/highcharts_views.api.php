<?php

/**
 * @file
 * Hooks provided by the Highcharts module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Defines Views Highchart options classes.
 *
 * @return array
 *   An associative array of chart templates, keyed by class callback (which
 *   must be an instance of ViewsHighchartsOptions), containing:
 *   - title: A string used to identify the chart template.
 *
 * @see highcharts_views_plugin_style_highcharts::options_form()
 * @see highcharts_views_plugin_style_highcharts::render()
 * @see CallbackHighchartsViewsCharts
 */
function hook_highcharts_views_charts() {
  return array(
    'CallbackHighchartsViewsCharts' => array(
      'class' => TRUE,
      'title' => t('Pie basic with credits'),
    ),
  );
}

/**
 * @} End of "addtogroup hooks".
 */

/**
 * @addtogroup callbacks
 * @{
 */

/**
 * Returns a Highcharts template.
 *
 * Callback for hook_highcharts_views_charts().
 *
 * See @link http://drupal.org/node/1250500 the D8 callback documentation issue. @endlink
 *
 * See @link http://php.net/manual/en/language.types.callable.php class callbacks. @endlink
 */
class CallbackHighchartsViewsCharts extends ViewsHighchartsOptionsPieBasic {

  /**
   * @see HighchartsOptions::render()
   */
  public function render() {
    // You could add your own php representation of any combination of
    // Highcharts options. For simplicity, and to point to an existing example
    // of this, we'll make use of a ViewsHighchartsOptions instance as a
    // starting point.
    $options = parent::render();
    // We can now add, for example, a trivial change to all pie charts using
    // this template. This would be silly though, as we can override these
    // values through the Views UI using the existing parent class.
    $options->credits->enabled = TRUE;
    $options->credits->text = 'Example.com';
    $options->credits->href = 'http://www.example.com';

    return $options;
  }

}

/**
 * @} End of "addtogroup callbacks".
 */
