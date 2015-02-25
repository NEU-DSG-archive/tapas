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
 * Alters which Highcharts options will be run as JavaScript callbacks.
 *
 * Methods don't work when passed through Drupal's JavaScript settings, becasue
 * they are converted to strings. Because Highcharts option types can not be
 * ascertained automatically (and there is no reference we can rely on for
 * programmatic evaling behavior), we place responsibility on modules
 * implementing Highcharts to specify which options will be run as methods.
 *
 * @param array $methods
 *   By reference. The $methods array returned by highcharts_get_methods().
 *   Defaults to 'tooltip.formatter'.
 * @param object $options
 *   A highcharts options object.
 *   See the @link http://api.highcharts.com Highcharts reference. @endlink
 *
 * @see highcharts_get_methods()
 * @see highcharts_render()
 */
function hook_highcharts_methods_alter(&$methods, $options) {
  // Example.
  if ($options->plotOptions->pie->showInLegend == TRUE) {
    $methods[] = 'plotOptions.pie.dataLabels.formatter';
  }
}

/**
 * @} End of "addtogroup hooks".
 */
