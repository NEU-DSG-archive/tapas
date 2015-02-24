API
===

There is one primary function, `highcharts_render()` (returns HTML for a
highcharts container and renders a highcharts options object within it).

Modules may create highchart options objects
(see http://www.highcharts.com/ref/), leveraging anything available in the
Highcharts API.

Additionally, modules may implement `hook_highcharts_methods_alter()` to define
additional options that should be evaluated as methods.

Example options object:

```php
/**
 * Pie-basic highcharts options object.
 *
 * @see highcharts_render()
 */
function custom_pie_basic_options() {
  $options = new stdClass();

  // Chart.
  $options->chart = (object)array(
    'renderTo' => 'container',
    'plotBackgroundColor' => NULL,
    'plotBorderWidth' => NULL,
    'plotShadow' => FALSE,
  );

  // Title.
  $options->title->text = t('Pies');

  // Tooltip.
  // Normally formatter is a function callback. For now we'll make it a string.
  // @todo whenever this is user defined (views config, etc) be sure to sanitize
  // this string before passing to highcharts_render().
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
  $options->series = array();
  $series = new StdClass();
  $series->type = 'pie';
  $series->name = 'Slices';
  $series->data = array();
  $series->data[] = array('Banana creme', 45.0);
  $series->data[] = array('Pumpkin', 26.8);

  // Selected item is an object.
  $selected = new stdClass();
  $selected->name = 'Apple';
  $selected->y = 12.8;
  $selected->sliced = TRUE;
  $selected->selected = TRUE;
  $series->data[] = $selected;

  $series->data[] = array('Lemon merengue', 8.5);
  $series->data[] = array('Mincemeat', 6.2);
  $series->data[] = array('Others', 0.7);

  $options->series[] = $series;

  // Diable credits.
  $options->credits->enabled = FALSE;

  return $options;
}
```

Example implementation of new Drupal Highcharts API functions:

```php
/**
 * Pie-basic highcharts block.
 *
 * @return hook_block_view() definition.
 */
function _custom_block_view_pie_basic() {
  $options = custom_pie_basic_options();
  if (is_object($options)) {
    // Optionally add styles or any other valid attributes, suitable for
    // drupal_attributes().
    $attributes = array('style' => array('height: 400px;'));

    // Return block definition.
    return array(
      'subject' => check_plain($options->title->text),
      'content' => highcharts_render($options, $attributes),
    );
  }
}
```
