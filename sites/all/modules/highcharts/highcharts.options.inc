<?php

/**
 * @file
 * Contains classes for Highcharts options.
 */

/**
 * Base class for Highcharts options.
 */
abstract class HighchartsOptions {

  /**
   * The Highcharts options object.
   *
   * @var stdClass
   */
  protected $options;

  /**
   * Chart data rows.
   *
   * @var array
   */
  protected $data;

  /**
   * Sets Highcharts data points for the series.
   *
   * @return array
   *   The $data param of HighchartsOptions::__construct().
   */
  abstract protected function data();

  /**
   * Extending classes must define this method.
   *
   * Classes may use this for additional logic, or to pre-set option elements as
   * Highcharts options object templates.
   *
   * @return stdClass
   *   A php representation of a Highcharts library options object.
   */
  abstract protected function render();

  /**
   * Build a Highcharts options template object.
   *
   * @param array $data
   *   (optional) An array of data points for the series. The points can be
   *   given in three ways:
   *
   *   See @link http://api.highcharts.com/highcharts#series.data Highcharts library series data documentation. @endlink
   *
   * @param stdClass $options
   *   (optional) A php representation of a Highcharts library options object.
   *   Defaults to an initialized object.
   *
   * @return HighchartsOptions
   */
  public function __construct(array $data = array(), stdClass $options = NULL) {
    // Initialize the options if not already set.
    $this->options = is_null($options) ? new stdClass() : $options;

    $this->data = $data;
  }

}

/**
 * Highcharts basic pie template.
 *
 * Note the title text should be set on the returned options object. Example:
 *
 * @code
 *   $template = new HighchartsOptionsPieBasic($data);
 *   $options = $template->render();
 *   $options->title->text = t('Pies');
 * @endcode
 */
class HighchartsOptionsPieBasic extends HighchartsOptions {

  /**
   * For now pass along only the $data variable.
   *
   * Note that pie series types, data will normally be an array of arrays,
   * containing label value pairs. Example:
   *
   * @code
   *   $data = array();
   *   $data[] = array('Banana creme', 45.0);
   *   $data[] = array('Pumpkin', 26.8);
   *   $data[] = array('Lemon merengue', 8.5);
   *   $data[] = array('Mincemeat', 6.2);
   *   $data[] = array('Others', 0.7);
   * @endcode
   *
   * However this may differ. For instance, an optional selected item is
   * expected to be an object with additional properties set. Example:
   *
   * @code
   *   $selected = new stdClass();
   *   $selected->name = 'Apple';
   *   $selected->y = 12.8;
   *   $selected->sliced = TRUE;
   *   $selected->selected = TRUE;
   *   $data[] = $selected;
   * @endcode
   *
   * So we may want to add additional logic here at some point,
   * but for now leave that up to each implementing method.
   *
   * @return array $data
   *   See $data param of HighchartsOptions::__construct().
   */
  protected function data() {
    return $this->data;
  }

  public function render() {
    $options = $this->options;
    $data = $this->data();

    // Chart.
    $options->chart = (object)array(
      'renderTo' => 'container',
      'plotBackgroundColor' => NULL,
      'plotBorderWidth' => NULL,
      'plotShadow' => FALSE,
    );

    // Tooltip formatter. Normally formatter is a function callback.
    // @todo whenever this is user defined (views config, etc) be sure to
    // sanitize this string before passing to highcharts_render().
    $options->tooltip->formatter = "function() {return '<b>'+ this.point.name +'</b>: '+ this.percentage +' %';}";

    // Plot options.
    $options->plotOptions->pie = (object)array(
      'allowPointSelect' => TRUE,
      'cursor' => 'pointer',
      'dataLabels' => array(
        'enabled' => FALSE,
      ),
      'showInLegend' => TRUE,
    );

    // Series.
    $series = new StdClass();
    $series->type = 'pie';
    $series->data = $data;
    $options->series = array();
    $options->series[] = $series;

    // Diable credits.
    $options->credits->enabled = FALSE;

    return $options;
  }

}

/**
 * Highcharts bar stacked template.
 *
 * Note the xAxis catagories should be set. Example:
 *
 * @code
 *   $options->xAxis->categories = array('Apples', 'Oranges', 'Pears', 'Grapes', 'Bananas');
 * @endcode
 */
class HighchartsOptionsBarStacked extends HighchartsOptions {

  /**
   * {@inheritdoc}
   *
   * Bar type charts expect data as an array of objects with named values,
   * including:
   * - name: The label of the data point.
   * - data: an array of data points, corresponding to the xAxis categories.
   */
  protected function data() {
    return $this->data;
  }

  public function render() {
    $options = $this->options;
    $data = $this->data();

    // Chart.
    $options->chart = (object)array(
      'renderTo' => 'container',
      'type' => 'bar',
    );

    // xAxis.
    // $options->xAxis->categories = array();

    // yAxis.
    $options->yAxis->min = 0;
    $options->yAxis->title = t('Total');

    // Legend.
    $options->legend = (object)array(
      'backgroundColor' => '#FFFFFF',
      'reversed' => TRUE,
    );

    // Tooltip.
    $options->tooltip->formatter = "function() {
      return ''+
      this.series.name +': '+ this.y +'';
    }";

    // Plot options.
    $options->plotOptions->series->stacking = 'normal';

    // Series.
    $options->series = $data;

    // Diable credits.
    $options->credits->enabled = FALSE;

    return $options;
  }

}

class HighchartsOptionsBarBasic extends HighchartsOptions {

  protected function data() {
    return $this->data;
  }

  public function render() {
    $options = $this->options;
    $data = $this->data();

    // Chart.
    $options->chart = (object)array(
      'renderTo' => 'container',
      'type' => 'bar',
    );

    // xAxis.
    // $options->xAxis->categories = array();
    $options->xAxis->title->text = NULL;

    // yAxis.
    $options->yAxis->min = 0;
    $options->yAxis->title->text = t('Total');
    $options->yAxis->title->align = 'high';
    $options->yAxis->labels->overflow = 'justify';

    // Tooltip.
    $options->tooltip->formatter = "function() {
      return ''+
      this.series.name +': '+ this.y +'';
    }";

    // Plot options.
    $options->plotOptions->bar->dataLabels->enabled = TRUE;

    // Legend.
    $options->legend->reversed = TRUE;

    // Series.
    $options->series = $data;

    // Diable credits.
    $options->credits->enabled = FALSE;

    return $options;
  }

}

/**
 * Highcharts basic line template.
 */
class HighchartsOptionsLineBasic extends HighchartsOptions {

  /**
   * {@inheritdoc}
   *
   * Basic line series data example:
   *
   * @code
   *   series: [{
   *     name: 'Tokyo',
   *     data: [7.0, 6.9, 9.5, 14.5, 18.2, 21.5, 25.2, 26.5, 23.3, 18.3, 13.9, 9.6]
   *   }, {
   *     name: 'New York',
   *     data: [-0.2, 0.8, 5.7, 11.3, 17.0, 22.0, 24.8, 24.1, 20.1, 14.1, 8.6, 2.5]
   *   }, {
   *     name: 'Berlin',
   *     data: [-0.9, 0.6, 3.5, 8.4, 13.5, 17.0, 18.6, 17.9, 14.3, 9.0, 3.9, 1.0]
   *   }, {
   *     name: 'London',
   *     data: [3.9, 4.2, 5.7, 8.5, 11.9, 15.2, 17.0, 16.6, 14.2, 10.3, 6.6, 4.8]
   *   }]
   * @endcode
   */
  protected function data() {
    return $this->data;
  }

  public function render() {
    $options = $this->options;
    $data = $this->data();

    // Chart.
    $options->chart = (object)array(
      'renderTo' => 'container',
      'type' => 'line',
    );

    // xAxis.
    // $options->xAxis->categories = array();

    // yAxis.
    $options->yAxis->title = t('Total');
    $options->yAxis->plotLines = (object) array(
      'value' => 0,
      'width' => 1,
      'color' => '#808080',
    );

    // Tooltip.
    // $options->tooltip->valueSuffix = '°C';

    // Legend.
    $options->legend->layout = 'vertical';
    $options->legend->align = 'right';
    $options->legend->verticalAlign = 'top';
    $options->legend->x = -10;
    $options->legend->y = 100;
    $options->borderWidth = 0;

    // Series.
    $options->series = $data;

    // Diable credits.
    $options->credits->enabled = FALSE;

    return $options;
  }

}
