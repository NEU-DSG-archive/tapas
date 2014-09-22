<?php

/**
 * @file
 * Highcharts options reference.
 */

/**
 * Gets a flat list of Highcharts options names.
 *
 * @return array
 *   An indexed array of Highchart options names in JavaScript object dot
 *   notation string format.
 */
function highchart_options_names() {
  $definition = highcharts_options_definition();
  $names = array();
  foreach ($definition as $section) {
    if (empty($section['members'])) {
      $names[] = $section['title'];
    }
    foreach($section['members'] as $member) {
      // Don't include members that are also their own section.
      $name = $section['title'] . '.' . $member['name'];
      if (!in_array($name, array_keys($definition))) {
        $names[] = $name;
      }
    }
  }
  sort($names, SORT_STRING);
  return $names;
}

/**
 * Gets a single Highcharts option definition from a dot notation string.
 *
 * @param string $name
 *   A Highcharts option names in JavaScript object dot notation string format.
 * @param array $definition
 *   Optional definition array from highcharts_options_definition().
 *
 * @return array
 *   A single Highcharts option definition.
 */
function highcharts_options_get_definition($name, $definition = array()) {
  // Only call once.
  $definition = !empty($definition) ? $definition : highcharts_options_definition();

  // Before recursion, this covers cases where sections do not have members.
  // After recursion, this returns the immediate parent section name.
  if (in_array($name, array_keys($definition))) {
    return $definition[$name];
  }

  // Otherwise, recurse to find the section for the requested member by popping
  // off one string part at a time until we find the parent.
  $parts = explode('.', $name);
  $method = array_pop($parts);
  $needle = implode('.', $parts);
  if ($section = highcharts_options_get_definition($needle, $definition)) {
    // Get method definition.
    if (isset($section['members']) && array_key_exists($method, $section['members'])) {
      return $section['members'][$method];
    }
  }
}

/**
 * Highcharts options reference scraped from http://www.highcharts.com/ref
 * (now http://api.highcharts.com/) on May 22, 2012.
 *
 * @return array
 *   An array of Highchart options reference information, keyed by section name,
 *   containing an associative array, containing:
 *   - title: the section title (same as key)
 *   - description: the section description. May contain HTML markup.
 *   - members: an array of options within this section, keyed by option name,
 *     containing:
 *     - name: the option name (same as key)
 *     - type: can be the name of any other option (an alias sharing the same
 *       definition), or types such as Array, Function, Mixed, Number or String.
 *     - default: default value such as '', null, false, true, etc.
 *     - desc: the option description. May contain HTML markup.
 */
function highcharts_options_definition() {
  return array(
    'chart' => array(
    'title' => 'chart',
    'description' => '
    Options regarding the chart area and plot area as well as general chart options.
    ',
    'members' => array(
      'alignTicks' => array(
        'name' => 'alignTicks',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        When using multiple axis, the ticks of two or more opposite axes will
        automatically be aligned by adding ticks to the axis or axes with the least
        ticks. This can be prevented by setting <code>alignTicks</code> to false.
        If the grid lines look messy, it\'s a good idea to hide them for the
        secondary axis by setting <code>gridLineWidth</code> to 0.
      ',
      ),
      'animation' => array(
        'name' => 'animation',
        'type' => 'Boolean|Object',
        'default' => 'true',
        'desc' => '
        <p>Set the overall animation for all chart updating. Animation can be disabled throughout
        the chart by setting it to false here. It can be overridden for each individual
        API method as a function parameter. The only animation not affected by this option is the
        initial series animation, see <a class="internal" href="#plotOptions-series">plotOptions.series</a>
        => animation.</p>

        <p>The animation can either be set as a boolean or a configuration object. If <code>true</code>,
        it will use the \'swing\' jQuery easing and a duration of 500 ms. If used as a configuration object,
        the following properties are supported:
        </p><dl>
          <dt>duration</dt>
          <dd>The duration of the animation in milliseconds.</dd>

          <dt>easing</dt>
          <dd>When using jQuery as the general framework, the easing can be set to <code>linear</code> or
          <code>swing</code>. More easing functions are available with the use of jQuery plug-ins, most notably
          the jQuery UI suite. See <a href="http://api.jquery.com/animate/">the jQuery docs</a>. When using
          MooTools as the general framework, use the property name <code>transition</code> instead
          of <code>easing</code>.</dd>
        </dl>
      ',
      ),
      'backgroundColor' => array(
        'name' => 'backgroundColor',
        'type' => 'Color',
        'default' => '"#FFFFFF"',
        'desc' => '
        The background color or gradient for the outer chart area.
      ',
      ),
      'borderColor' => array(
        'name' => 'borderColor',
        'type' => 'Color',
        'default' => '"#4572A7"',
        'desc' => '
        The color of the outer chart border. The border is painted using vector graphic
        techniques to allow rounded corners.
      ',
      ),
      'borderRadius' => array(
        'name' => 'borderRadius',
        'type' => 'Number',
        'default' => '5',
        'desc' => '
        The corner radius of the outer chart border.
      ',
      ),
      'borderWidth' => array(
        'name' => 'borderWidth',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        The pixel width of the outer chart border. The border is painted using vector graphic
        techniques to allow rounded corners.
      ',
      ),
      'className' => array(
        'name' => 'className',
        'type' => 'String',
        'default' => '""',
        'desc' => '
        A CSS class name to apply to the charts container <code>div</code>, allowing unique CSS styling
        for each chart.
      ',
      ),
      'defaultSeriesType' => array(
        'name' => 'defaultSeriesType',
        'type' => 'String',
        'default' => '',
        'desc' => '
        Alias of <code>type</code>.
      ',
      ),
      'events' => array(
        'name' => 'events',
        'type' => 'chart.events',
        'default' => '',
        'desc' => '
        Event listeners for chart events.
      ',
      ),
      'height' => array(
        'name' => 'height',
        'type' => 'Number',
        'default' => 'null',
        'desc' => '
        An explicit height for the chart. By default the height is calculated from the offset
        height of the containing element.
      ',
      ),
      'ignoreHiddenSeries' => array(
        'name' => 'ignoreHiddenSeries',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        If true, the axes will scale to the remaining visible series once one series
        is hidden. If false, hiding and showing a series will not affect the axes or
        the other series. For stacks, once one series within the stack is hidden, the
        rest of the stack will close in around it even if the axis is not affected.
      ',
      ),
      'inverted' => array(
        'name' => 'inverted',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => '
        Whether to invert the axes so that the x axis is horizontal and y axis is vertical.
        When true, the x axis is reversed by default. If a bar plot is present in the chart,
        it will be inverted automatically.
      ',
      ),
      'margin' => array(
        'name' => 'margin',
        'type' => 'Array',
        'default' => '[null]',
        'desc' => '
        <p>The margin between the outer edge of the chart and the plot area. The numbers in
        the array designate top, right, bottom and left respectively. Use the options
        <code>marginTop</code>, <code>marginRight</code>, <code>marginBottom</code> and
        <code>marginLeft</code> for shorthand setting of one option.</p>
        <p>Since version 2.1, the margin is 0 by default. The actual space is dynamically calculated
        from the offset of axis labels, axis title, title, subtitle and legend in addition
        to the <code>spacingTop</code>, <code>spacingRight</code>, <code>spacingBottom</code> and
        <code>spacingLeft</code> options.</p>
      ',
      ),
      'marginTop' => array(
        'name' => 'marginTop',
        'type' => 'Number',
        'default' => 'null',
        'desc' => '
        The margin between the top outer edge of the chart and the plot area. Use this to set a fixed
        pixel value for the margin as opposed to the default dynamic margin. See also <code>spacingTop</code>.
      ',
      ),
      'marginRight' => array(
        'name' => 'marginRight',
        'type' => 'Number',
        'default' => '50',
        'desc' => '
        The margin between the right outer edge of the chart and the plot area. Use this to set a fixed
        pixel value for the margin as opposed to the default dynamic margin. See also <code>spacingRight</code>.
      ',
      ),
      'marginBottom' => array(
        'name' => 'marginBottom',
        'type' => 'Number',
        'default' => '70',
        'desc' => '
        The margin between the bottom outer edge of the chart and the plot area. Use this to set a fixed
        pixel value for the margin as opposed to the default dynamic margin. See also <code>spacingBottom</code>.
      ',
      ),
      'marginLeft' => array(
        'name' => 'marginLeft',
        'type' => 'Number',
        'default' => '80',
        'desc' => '
        The margin between the left outer edge of the chart and the plot area. Use this to set a fixed
        pixel value for the margin as opposed to the default dynamic margin. See also <code>spacingLeft</code>.
      ',
      ),
      'plotBackgroundColor' => array(
        'name' => 'plotBackgroundColor',
        'type' => 'Color',
        'default' => 'null',
        'desc' => '
        The background color or gradient for the plot area.
      ',
      ),
      'plotBackgroundImage' => array(
        'name' => 'plotBackgroundImage',
        'type' => 'String',
        'default' => 'null',
        'desc' => '
        The URL for an image to use as the plot background. To set an image as the background
        for the entire chart, set a CSS background image to the container element.
      ',
      ),
      'plotBorderColor' => array(
        'name' => 'plotBorderColor',
        'type' => 'Color',
        'default' => '"#C0C0C0"',
        'desc' => '
        The color of the inner chart or plot area border.
      ',
      ),
      'plotBorderWidth' => array(
        'name' => 'plotBorderWidth',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        The pixel width of the plot area border.
      ',
      ),
      'plotShadow' => array(
        'name' => 'plotShadow',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => '
        Whether to apply a drop shadow to the plot area. Requires that plotBackgroundColor
        be set.
      ',
      ),
      'reflow' => array(
        'name' => 'reflow',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        Whether to reflow the chart to fit the width of the container div on resizing the window.
      ',
      ),
      'renderTo' => array(
        'name' => 'renderTo',
        'type' => 'String|Object',
        'default' => 'null',
        'desc' => '
        The HTML element where the chart will be rendered. If it is a string, the element by that
        id is used. The HTML element can also be passed by direct reference.
      ',
      ),
      'resetZoomButton' => array(
        'name' => 'resetZoomButton',
        'type' => 'chart.resetZoomButton',
        'default' => '',
        'desc' => '
      ',
      ),
      'selectionMarkerFill' => array(
        'name' => 'selectionMarkerFill',
        'type' => 'Color',
        'default' => 'rgba(69,114,167,0.25)',
        'desc' => '
        The background color of the marker square when selecting (zooming in on) an area of the chart.
      ',
      ),
      'shadow' => array(
        'name' => 'shadow',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => '
        Whether to apply a drop shadow to the outer chart area. Requires that
        backgroundColor be set.
      ',
      ),
      'showAxes' => array(
        'name' => 'showAxes',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => '
        Whether to show the axes initially. This only applies to empty charts where
        series are added dynamically,
        as axes are automatically added to cartesian series.
      ',
      ),
      'spacingTop' => array(
        'name' => 'spacingTop',
        'type' => 'Number',
        'default' => '10',
        'desc' => '
        <p>The space between the top edge of the chart and the content (plot area, axis title and labels, title, subtitle or
        legend in top position).</p>
      ',
      ),
      'spacingRight' => array(
        'name' => 'spacingRight',
        'type' => 'Number',
        'default' => '10',
        'desc' => '
        <p>The space between the right edge of the chart and the content (plot area, axis title and labels, title, subtitle or
        legend in top position).</p>
      ',
      ),
      'spacingBottom' => array(
        'name' => 'spacingBottom',
        'type' => 'Number',
        'default' => '15',
        'desc' => '
        <p>The space between the bottom edge of the chart and the content (plot area, axis title and labels, title, subtitle or
        legend in top position).</p>
      ',
      ),
      'spacingLeft' => array(
        'name' => 'spacingLeft',
        'type' => 'Number',
        'default' => '10',
        'desc' => '
        <p>The space between the left edge of the chart and the content (plot area, axis title and labels, title, subtitle or
        legend in top position).</p>
      ',
      ),
      'style' => array(
        'name' => 'style',
        'type' => 'CSSObject',
        'default' => '',
        'desc' => '
        Additional CSS styles to apply inline to the container <code>div</code>. Note that since the default
        font styles are applied in the renderer, it is ignorant of the individual chart
        options and must be set globally. Defaults
        to:
  <pre>style: {
    fontFamily: \'"Lucida Grande", "Lucida Sans Unicode", Verdana, Arial, Helvetica, sans-serif\', // default font
    fontSize: \'12px\'
  }</pre>
      ',
      ),
      'type' => array(
        'name' => 'type',
        'type' => 'String',
        'default' => '"line"',
        'desc' => '
        The default series type for the chart. Can be one of line, spline, area, areaspline,
        column, bar, pie and scatter.
      ',
      ),
      'width' => array(
        'name' => 'width',
        'type' => 'Number',
        'default' => 'null',
        'desc' => '
        An explicit width for the chart. By default the width is calculated from the offset
        width of the containing element.
      ',
      ),
      'zoomType' => array(
        'name' => 'zoomType',
        'type' => 'String',
        'default' => '""',
        'desc' => '
        Decides in what dimentions the user can zoom by dragging the mouse. Can
        be one of <code>x</code>, <code>y</code> or <code>xy</code>.
      ',
      ),
      ),
    ),
    'chart.events' => array(
    'title' => 'chart.events',
    'description' => '
    Event listeners for the chart.
    ',
    'members' => array(
      'addSeries' => array(
        'name' => 'addSeries',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        Fires when a series is added to the chart after load time, using the <code>addSeries</code>
        method. The <code>this</code> keyword refers to the
        chart object itself. One parameter, <code>event</code>, is passed to the function.
        This contains common event information based on jQuery or MooTools depending on
        which library is used as the base for Highcharts. Through <code>event.options</code>
        you can access the series options that was passed to the <code>addSeries</code>
        method. Returning false prevents the series from being added.
      ',
      ),
      'click' => array(
        'name' => 'click',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        <p>Fires when clicking on the plot background. The <code>this</code> keyword refers to the
        chart object itself. One parameter, <code>event</code>, is passed to the function.
        This contains common event information based on jQuery or MooTools depending on
        which library is used as the base for Highcharts.</p>
        <p>Information on the clicked spot can be found through <code>event.xAxis</code> and
        <code>event.yAxis</code>, which are arrays containing the axes of each dimension
        and each axis\' value at the clicked spot. The primary axes are <code>event.xAxis[0]</code>
        and <code>event.yAxis[0]</code>. Remember the unit of a datetime axis is milliseconds
        since 1970-01-01 00:00:00.</p>
  <pre>click: function(e) {
    console.log(
      Highcharts.dateFormat(\'%Y-%m-%d %H:%M:%S\', e.xAxis[0].value),
      e.yAxis[0].value
    )
  }</pre>
      ',
      ),
      'load' => array(
        'name' => 'load',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        <p>Fires when the chart is finished loading. The <code>this</code> keyword refers to the
        chart object itself. One parameter, <code>event</code>, is passed to the function.
        This contains common event information based on jQuery or MooTools depending on
        which library is used as the base for Highcharts.</p>
        <p>From version 2.0.4, there is also a second parameter to <code>Highcharts.Chart</code>
        where a callback function can be passed to be executed on chart.load.</p>
      ',
      ),
      'redraw' => array(
        'name' => 'redraw',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        Fires when the chart is redrawn, either after a call to chart.redraw() or after
        an axis, series or point is modified with the <code>redraw</code> option set to true.
        The <code>this</code> keyword refers to the
        chart object itself. One parameter, <code>event</code>, is passed to the function.
        This contains common event information based on jQuery or MooTools depending on
        which library is used as the base for Highcharts.
      ',
      ),
      'selection' => array(
        'name' => 'selection',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        <p>Fires when an area of the chart has been selected. Selection is enabled by setting
        the chart\'s zoomType. The <code>this</code> keyword refers to the
        chart object itself. One parameter, <code>event</code>, is passed to the function.
        This contains common event information based on jQuery or MooTools depending on
        which library is used as the base for Highcharts. The default action for the
        selection event is to zoom the
        chart to the selected area. It can be prevented by calling
        <code>event.preventDefault()</code>.</p>
        <p>Information on the selected area can be found through <code>event.xAxis</code> and
        <code>event.yAxis</code>, which are arrays containing the axes of each dimension
        and each axis\' min and max values. The primary axes are <code>event.xAxis[0]</code>
        and <code>event.yAxis[0]</code>. Remember the unit of a datetime axis is milliseconds
        since 1970-01-01 00:00:00.</p>

  <pre>selection: function(event) {
    // log the min and max of the primary, datetime x-axis
    console.log(
      Highcharts.dateFormat(\'%Y-%m-%d %H:%M:%S\', event.xAxis[0].min),
      Highcharts.dateFormat(\'%Y-%m-%d %H:%M:%S\', event.xAxis[0].max)
    );
    // log the min and max of the y axis
    console.log(event.yAxis[0].min, event.yAxis[0].max);
  }</pre>
      ',
      ),
      ),
    ),
    'chart.resetZoomButton' => array(
    'title' => 'chart.resetZoomButton',
    'description' => '
    The button that appears after a selection zoom, allowing the user to reset zoom.
    ',
    'members' => array(
      'position' => array(
        'name' => 'position',
        'type' => 'Object',
        'default' => '',
        'desc' => '
        The position of the button.
      ',
      ),
      'relativeTo' => array(
        'name' => 'relativeTo',
        'type' => 'String',
        'default' => '"plot"',
        'desc' => '
        What frame the button should be placed related to. Can be either "plot" or "chart".
      ',
      ),
      'theme' => array(
        'name' => 'theme',
        'type' => 'Object',
        'default' => '',
        'desc' => '
        A collection of attributes for the button. The object takes SVG attributes like
        <code>fill</code>, <code>stroke</code>, <code>stroke-width</code> or <code>r</code>,
        the border radius. The theme also supports <code>style</code>,
        a collection of CSS properties for the text. Equivalent attributes for the hover state
        are given in <code>theme.states.hover</code>.
      ',
      ),
      ),
    ),
    'title' => array(
    'title' => 'title',
    'description' => '
    The chart\'s main title.
    ',
    'members' => array(
      'align' => array(
        'name' => 'align',
        'type' => 'String',
        'default' => '"center"',
        'desc' => '
        The horizontal alignment of the title. Can be one of "left", "center" and "right".
      ',
      ),
      'floating' => array(
        'name' => 'floating',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => '
        When the title is floating, the plot area will not move to make space for it.
      ',
      ),
      'margin' => array(
        'name' => 'margin',
        'type' => 'Number',
        'default' => '15',
        'desc' => '
        The margin between the title and the plot area, or if a subtitle is present, the
        margin between the subtitle and the plot area.
      ',
      ),
      'text' => array(
        'name' => 'text',
        'type' => 'String',
        'default' => '"Chart title"',
        'desc' => '
        The title of the chart. To disable the title, set the <code>text</code> to <code>null</code>.
      ',
      ),
      'style' => array(
        'name' => 'style',
        'type' => 'CSSObject',
        'default' => '',
        'desc' => '
        CSS styles for the title. Use this for font styling, but use <code>align</code>,
        <code>x</code> and <code>y</code>for text alignment. Defaults to:
        <pre>{
    color: \'#3E576F\',
    fontSize: \'16px\'
  }</pre>
      ',
      ),
      'verticalAlign' => array(
        'name' => 'verticalAlign',
        'type' => 'String',
        'default' => '"top"',
        'desc' => '
        The vertical alignment of the title. Can be one of "top", "middle" and "bottom".
      ',
      ),
      'x' => array(
        'name' => 'x',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        The x position of the title relative to the alignment within chart.spacingLeft and chart.spacingRight.
      ',
      ),
      'y' => array(
        'name' => 'y',
        'type' => 'Number',
        'default' => '15',
        'desc' => '
        The y position of the title relative to the alignment within chart.spacingTop and chart.spacingBottom.
      ',
      ),
      ),
    ),
    'subtitle' => array(
    'title' => 'subtitle',
    'description' => '
    The chart\'s subtitle
    ',
    'members' => array(
      'align' => array(
        'name' => 'align',
        'type' => 'String',
        'default' => '"center"',
        'desc' => '
        The horizontal alignment of the subtitle. Can be one of "left", "center" and "right".
      ',
      ),
      'floating' => array(
        'name' => 'floating',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => '
        When the subtitle is floating, the plot area will not move to make space for it.
      ',
      ),
      'text' => array(
        'name' => 'text',
        'type' => 'String',
        'default' => '""',
        'desc' => '
        The subtitle of the chart.
      ',
      ),
      'style' => array(
        'name' => 'style',
        'type' => 'CSSObject',
        'default' => '',
        'desc' => '
        CSS styles for the title. Exact positioning of the title can be achieved
        by changing the margin property, or by adding <code>position: "absolute"</code> and
        left and top properties. Defaults to:
        <pre>{
    color: \'#3E576F\'
  }</pre>
      ',
      ),
      'verticalAlign' => array(
        'name' => 'verticalAlign',
        'type' => 'String',
        'default' => '"top"',
        'desc' => '
        The vertical alignment of the title. Can be one of "top", "middle" and "bottom".
      ',
      ),
      'x' => array(
        'name' => 'x',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        The x position of the subtitle relative to the alignment within chart.spacingLeft and chart.spacingRight.
      ',
      ),
      'y' => array(
        'name' => 'y',
        'type' => 'Number',
        'default' => '30',
        'desc' => '
        The y position of the subtitle relative to the alignment within chart.spacingTop and chart.spacingBottom.
      ',
      ),
      ),
    ),
    'xAxis' => array(
    'title' => 'xAxis',
    'description' => '
    <p>The X axis or category axis. Normally this is the horizontal axis, though if the
    chart is inverted this is the vertical axis. In case of multiple axes, the xAxis
    node is an array of configuration objects.</p>
    <p>See <a class="internal" href="#axis-object">the Axis object</a> for programmatic
    access to the axis.</p>
    ',
    'members' => array(
      'allowDecimals' => array(
        'name' => 'allowDecimals',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        Whether to allow decimals in this axis\' ticks. When counting integers, like
        persons or hits on a web page, decimals must be avoided in the axis tick
        labels.
      ',
      ),
      'alternateGridColor' => array(
        'name' => 'alternateGridColor',
        'type' => 'Color',
        'default' => 'null',
        'desc' => '
        When using an alternate grid color, a band is painted across the plot area
        between every other grid line.
      ',
      ),
      'categories' => array(
        'name' => 'categories',
        'type' => 'Array',
        'default' => '[]',
        'desc' => '
        If categories are present for the xAxis, names are used instead of numbers for that
        axis. Example:
  <pre>categories: [\'Apples\', \'Bananas\', \'Oranges\']</pre>
      ',
      ),
      'dateTimeLabelFormats' => array(
        'name' => 'dateTimeLabelFormats',
        'type' => 'Object',
        'default' => '',
        'desc' => '
        For a datetime axis, the scale will automatically adjust to the appropriate unit.
        This member gives the default string representations used for each unit. For an
        overview of the replacement codes, see dateFormat.
        Defaults to:
  <pre>{
    second: \'%H:%M:%S\',
    minute: \'%H:%M\',
    hour: \'%H:%M\',
    day: \'%e. %b\',
    week: \'%e. %b\',
    month: \'%b \'%y\',
    year: \'%Y\'
  }</pre>

      ',
      ),
      'endOnTick' => array(
        'name' => 'endOnTick',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => '
        Whether to force the axis to end on a tick. Use this option with the <code>maxPadding</code>
        option to control the axis end.
      ',
      ),
      'events' => array(
        'name' => 'events',
        'type' => 'xAxis.events',
        'default' => '',
        'desc' => '
        A collection of event handlers.
      ',
      ),
      'gridLineColor' => array(
        'name' => 'gridLineColor',
        'type' => 'Color',
        'default' => '"#C0C0C0"',
        'desc' => '
        Color of the grid lines extending the ticks across the plot area.
      ',
      ),
      'gridLineDashStyle' => array(
        'name' => 'gridLineDashStyle',
        'type' => 'String',
        'default' => 'Solid',
        'desc' => '
        The dash or dot style of the grid lines. For possible values, see <a href="http://jsfiddle.net/gh/get/jquery/1.7.1/highslide-software/highcharts.com/tree/master/samples/highcharts/plotoptions/series-dashstyle-all/">
        this demonstration</a>.
      ',
      ),
      'gridLineWidth' => array(
        'name' => 'gridLineWidth',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        The width of the grid lines extending the ticks across the plot area.
      ',
      ),
      'id' => array(
        'name' => 'id',
        'type' => 'String',
        'default' => 'null',
        'desc' => '
        An id for the axis. This can be used after render time to get a pointer
        to the axis object through <code>chart.get()</code>.
      ',
      ),
      'labels' => array(
        'name' => 'labels',
        'type' => 'xAxis.labels',
        'default' => '',
        'desc' => '
        Configuration object for the axis labels, usually displaying the number
        for each tick.
      ',
      ),
      'lineColor' => array(
        'name' => 'lineColor',
        'type' => 'Color',
        'default' => '"#C0D0E0"',
        'desc' => '
        The color of the line marking the axis itself.
      ',
      ),
      'lineWidth' => array(
        'name' => 'lineWidth',
        'type' => 'Number',
        'default' => '1',
        'desc' => '
        The width of the line marking the axis itself.
      ',
      ),
      'linkedTo' => array(
        'name' => 'linkedTo',
        'type' => 'Number',
        'default' => 'null',
        'desc' => '
        Index of another axis that this axis is linked to. When an axis is linked to
        a master axis, it will take the same extremes as the master, but as assigned by
        min or max or by setExtremes. It can be used to show additional info, or to ease
        reading the chart by duplicating the scales.
      ',
      ),
      'max' => array(
        'name' => 'max',
        'type' => 'Number',
        'default' => 'null',
        'desc' => '
        The maximum value of the axis. If <code>null</code>, the max value is
        automatically calculated. If the <code>endOnTick</code> option is
        true, the <code>max</code> value might be rounded up.
        The actual maximum value is also influenced by
        <a class="internal" href="#chart">chart.alignTicks</a>.
      ',
      ),
      'maxPadding' => array(
        'name' => 'maxPadding',
        'type' => 'Number',
        'default' => '0.01',
        'desc' => '
        Padding of the max value relative to the length of the axis. A padding of 0.05 will make
        a 100px axis 5px longer. This is useful when you don\'t want the highest data
        value to appear on the edge of the plot area. When the axis\' <code>max</code>
        option is set or a max extreme is set using <code>axis.setExtremes()</code>,
        the maxPadding will be ignored.
      ',
      ),
      'maxZoom' => array(
        'name' => 'maxZoom',
        'type' => 'Number',
        'default' => '',
        'desc' => '
        Deprecated. Renamed to <code>minRange</code> as of Highcharts 2.2.
      ',
      ),
      'min' => array(
        'name' => 'min',
        'type' => 'Number',
        'default' => 'null',
        'desc' => '
        The minimum value of the axis. If <code>null</code> the min value is
        automatically calculated. If the <code>startOnTick</code> option is
        true, the <code>min</code> value might be rounded down.
      ',
      ),
      'minorGridLineColor' => array(
        'name' => 'minorGridLineColor',
        'type' => 'Color',
        'default' => '#E0E0E0',
        'desc' => '
        Color of the minor, secondary grid lines.
      ',
      ),
      'minorGridLineDashStyle' => array(
        'name' => 'minorGridLineDashStyle',
        'type' => 'String',
        'default' => 'Solid',
        'desc' => '
        The dash or dot style of the minor grid lines. For possible values, see <a href="http://jsfiddle.net/gh/get/jquery/1.7.1/highslide-software/highcharts.com/tree/master/samples/highcharts/plotoptions/series-dashstyle-all/">
        this demonstration</a>.
      ',
      ),
      'minorGridLineWidth' => array(
        'name' => 'minorGridLineWidth',
        'type' => 'Number',
        'default' => '1',
        'desc' => '
        Width of the minor, secondary grid lines.
      ',
      ),
      'minorTickColor' => array(
        'name' => 'minorTickColor',
        'type' => 'Color',
        'default' => '#A0A0A0',
        'desc' => '
        Color for the minor tick marks.
      ',
      ),
      'minorTickInterval' => array(
        'name' => 'minorTickInterval',
        'type' => 'Number',
        'default' => 'null',
        'desc' => '
        <p>Tick interval in scale units for the minor ticks. On a linear axis, if <code>"auto"</code>,
        the minor tick interval is calculated as a fifth of the tickInterval. If
        <code>null</code>, minor ticks are not shown.</p>
        <p>On logarithmic axes, the unit is the power of the value. For example, setting
          the minorTickInterval to 1 puts one tick on each of 0.1, 1, 10, 100 etc. Setting
          the minorTickInterval to 0.1 produces 9 ticks between 1 and 10,
          10 and 100 etc. A minorTickInterval of "auto" on a log axis results in a best guess,
          attempting to enter approximately 5 minor ticks between each major tick.</p>
      ',
      ),
      'minorTickLength' => array(
        'name' => 'minorTickLength',
        'type' => 'Number',
        'default' => '2',
        'desc' => '
        The pixel length of the minor tick marks.
      ',
      ),
      'minorTickPosition' => array(
        'name' => 'minorTickPosition',
        'type' => 'String',
        'default' => 'outside',
        'desc' => '
        The position of the minor tick marks relative to the axis line. Can be
        one of <code>inside</code> and <code>outside</code>.
      ',
      ),
      'minorTickWidth' => array(
        'name' => 'minorTickWidth',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        The pixel width of the minor tick mark.
      ',
      ),
      'minPadding' => array(
        'name' => 'minPadding',
        'type' => 'Number',
        'default' => '0.01',
        'desc' => '
        Padding of the min value relative to the length of the axis. A padding of 0.05 will make
        a 100px axis 5px longer. This is useful when you don\'t want the lowest data
        value to appear on the edge of the plot area. When the axis\' <code>min</code>
        option is set or a min extreme is set using <code>axis.setExtremes()</code>,
        the minPadding will be ignored.
      ',
      ),
      'minRange' => array(
        'name' => 'minRange',
        'type' => 'Number',
        'default' => '',
        'desc' => '
        <p>The minimum range to display on this axis. The entire axis will not be allowed
        to span over a smaller interval than this. For example, for a datetime axis
        the main unit is milliseconds. If minRange is set to 3600000, you can\'t zoom
        in more than to one hour.</p>
        <p>The default minRange for the x axis is five times the smallest
        interval between any of the data points.</p>
        <p>On a logarithmic axis, the unit for the minimum range is the power. So a minRange of
          1 means that the axis can be zoomed to 10-100, 100-1000, 1000-10000 etc.</p>
      ',
      ),
      'offset' => array(
        'name' => 'offset',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        The distance in pixels from the plot area to the axis line. A positive offset
        moves the axis with it\'s line, labels and ticks away from the plot area. This
        is typically used when two or more axes are displayed on the same side of the
        plot.
      ',
      ),
      'opposite' => array(
        'name' => 'opposite',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => '
        Whether to display the axis on the opposite side of the normal. The normal
        is on the left side for vertical axes and bottom for horizontal, so the opposite
        sides will be right and top respectively. This is typically used with dual or
        multiple axes.
      ',
      ),
      'plotBands' => array(
        'name' => 'plotBands',
        'type' => 'Array.<plotBand>',
        'default' => 'null',
        'desc' => '
        An array of configuration objects for plot bands colouring parts of the
        plot area background.
      ',
      ),
      'plotLines' => array(
        'name' => 'plotLines',
        'type' => 'Array.<plotLine>',
        'default' => 'null',
        'desc' => '
        An array of configuration objects for plot lines marking specific values in
        the plot area.
      ',
      ),
      'reversed' => array(
        'name' => 'reversed',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => '
        Whether to reverse the axis so that the highest number is closest to origo.
        If the chart is inverted, the x axis is reversed by default.
      ',
      ),
      'showFirstLabel' => array(
        'name' => 'showFirstLabel',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        Whether to show the first tick label.
      ',
      ),
      'showLastLabel' => array(
        'name' => 'showLastLabel',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => '
        Whether to show the last tick label.
      ',
      ),
      'startOfWeek' => array(
        'name' => 'startOfWeek',
        'type' => 'Number',
        'default' => '1',
        'desc' => '
        For datetime axes, this decides where to put the tick between
        weeks. 0 = Sunday, 1 = Monday.
      ',
      ),
      'startOnTick' => array(
        'name' => 'startOnTick',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => '
        Whether to force the axis to start on a tick. Use this option with the <code>maxPadding</code>
        option to control the axis start.
      ',
      ),
      'tickColor' => array(
        'name' => 'tickColor',
        'type' => 'Color',
        'default' => '#C0D0E0',
        'desc' => '
        Color for the main tick marks.
      ',
      ),
      'tickInterval' => array(
        'name' => 'tickInterval',
        'type' => 'Mixed',
        'default' => 'null',
        'desc' => '
        <p>The interval of the tick marks in axis units. When <code>null</code>, the tick interval
        is computed to approximately follow the tickPixelInterval on linear and datetime axes.
        On categorized axes, a <code>null</code> tickInterval will default to 1, one category.
        Note that datetime axes are based on milliseconds, so for
        example an interval of one day is expressed as <code>24 * 3600 * 1000</code>.</p>
        <p>On logarithmic axes, the tickInterval is based on powers, so a tickInterval of 1 means
          one tick on each of 0.1, 1, 10, 100 etc. A tickInterval of 2 means a tick of 0.1, 10, 1000 etc.
          A tickInterval of 0.2 puts a tick on 0.1, 0.2, 0.4, 0.6, 0.8, 1, 2, 4, 6, 8, 10, 20, 40 etc.
          </p>
      ',
      ),
      'tickLength' => array(
        'name' => 'tickLength',
        'type' => 'Number',
        'default' => '5',
        'desc' => '
        The pixel length of the main tick marks.
      ',
      ),
      'tickmarkPlacement' => array(
        'name' => 'tickmarkPlacement',
        'type' => 'String',
        'default' => '"between"',
        'desc' => '
        For categorized axes only. If "on" the tick mark is placed in the center of
        the category, if "between" the tick mark is placed between categories.
      ',
      ),
      'tickPixelInterval' => array(
        'name' => 'tickPixelInterval',
        'type' => 'Number',
        'default' => '',
        'desc' => '
        If tickInterval is <code>null</code> this option sets the approximate pixel interval of the
        tick marks. Not applicable to categorized axis. Defaults to <code>72</code>
        for the Y axis and <code>100</code> for the X axis.
      ',
      ),
      'tickPosition' => array(
        'name' => 'tickPosition',
        'type' => 'String',
        'default' => '"outside"',
        'desc' => '
        The position of the major tick marks relative to the axis line. Can be
        one of <code>inside</code> and <code>outside</code>.
      ',
      ),
      'tickWidth' => array(
        'name' => 'tickWidth',
        'type' => 'Number',
        'default' => '1',
        'desc' => '
        The pixel width of the major tick marks.
      ',
      ),
      'title' => array(
        'name' => 'title',
        'type' => 'xAxis.title',
        'default' => '',
        'desc' => '
        Configuration object for the axis title.
      ',
      ),
      'type' => array(
        'name' => 'type',
        'type' => 'String',
        'default' => '"linear"',
        'desc' => '
        The type of axis. Can be one of "linear", "logarithmic" or "datetime". In a datetime
        axis, the numbers are given in milliseconds, and tick marks are placed
        on appropriate values like full hours or days.
      ',
      ),
      ),
    ),
    'xAxis.events' => array(
    'title' => 'xAxis.events',
    'description' => '
    Event handlers for the axis.
    ',
    'members' => array(
      'setExtremes' => array(
        'name' => 'setExtremes',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        Fires when the minimum and maximum is set for the axis, either by calling the
        <code>.setExtremes()</code> method or by selecting an area in the chart. The <code>this</code>
        keyword refers to the axis object itself. One parameter, <code>event</code>, is passed to the function.
        This contains common event information based on jQuery or MooTools depending on which
        library is used as the base for Highcharts. The new user set minimum and maximum values
        can be found by <code>event.min</code> and <code>event.max</code>.
      ',
      ),
      ),
    ),
    'xAxis.labels' => array(
    'title' => 'xAxis.labels',
    'description' => '
    The axis labels show the number or category for each tick.
    ',
    'members' => array(
      'align' => array(
        'name' => 'align',
        'type' => 'String',
        'default' => '"center"',
        'desc' => '
        What part of the string the given position is anchored to.
        Can be one of <code>"left"</code>,
        <code>"center"</code> or <code>"right"</code>. In inverted charts,
        x axis label alignment and y axis alignment are swapped.
      ',
      ),
      'enabled' => array(
        'name' => 'enabled',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        Enable or disable the axis labels.
      ',
      ),
      'formatter' => array(
        'name' => 'formatter',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        Callback JavaScript function to format the label. The value is
        given by <code>this.value</code>. Additional properties for <code>this</code> are
        <code>axis</code>, <code>chart</code>, <code>isFirst</code> and <code>isLast</code>.
        Defaults to:
  <pre>function() {
    return this.value;
  }</pre>
      ',
      ),
      'rotation' => array(
        'name' => 'rotation',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        Rotation of the labels in degrees.
      ',
      ),
      'staggerLines' => array(
        'name' => 'staggerLines',
        'type' => 'Number',
        'default' => 'null',
        'desc' => '
        Horizontal axes only. The number of lines to spread the labels over to make room
        or tighter labels.
      ',
      ),
      'step' => array(
        'name' => 'step',
        'type' => 'Number',
        'default' => 'null',
        'desc' => '
        To show only every <em>n</em>\'th label on the axis, set the step to <em>n</em>. Setting the step to 2 shows every other label.
      ',
      ),
      'style' => array(
        'name' => 'style',
        'type' => 'CSSObject',
        'default' => '',
        'desc' => '
        CSS styles for the label. Defaults to:
  <pre>style: {
    color: \'#6D869F\',
    fontWeight: \'bold\'
  }
  </pre>
      ',
      ),
      'x' => array(
        'name' => 'x',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        The x position offset of the label relative to the tick position
        on the axis.
      ',
      ),
      'y' => array(
        'name' => 'y',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        The y position offset of the label relative to the tick position
        on the axis.
      ',
      ),
      ),
    ),
    'xAxis.plotBands' => array(
    'title' => 'xAxis.plotBands',
    'description' => '
    A colored band stretching across the plot area marking an interval on the axis.
    ',
    'members' => array(
      'color' => array(
        'name' => 'color',
        'type' => 'Color',
        'default' => 'null',
        'desc' => '
        The color of the plot band.
      ',
      ),
      'events' => array(
        'name' => 'events',
        'type' => 'Object',
        'default' => '',
        'desc' => '
        An object defining mouse events for the plot band. Supported properties are <code>click</code>,
        <code>mouseover</code>, <code>mouseout</code>, <code>mousemove</code>.
      ',
      ),
      'from' => array(
        'name' => 'from',
        'type' => 'Number',
        'default' => 'null',
        'desc' => '
        The start position of the plot band in axis units.
      ',
      ),
      'id' => array(
        'name' => 'id',
        'type' => 'String',
        'default' => 'null',
        'desc' => '
        An id used for identifying the plot band in Axis.removePlotBand.
      ',
      ),
      'label' => array(
        'name' => 'label',
        'type' => 'xAxis.plotBands.label',
        'default' => '',
        'desc' => '
        Text labels for the plot bands.
      ',
      ),
      'to' => array(
        'name' => 'to',
        'type' => 'Number',
        'default' => 'null',
        'desc' => '
        The end position of the plot band in axis units.
      ',
      ),
      'zIndex' => array(
        'name' => 'zIndex',
        'type' => 'Number',
        'default' => 'null',
        'desc' => '
        The z index of the plot band within the chart.
      ',
      ),
      ),
    ),
    'xAxis.plotBands.label' => array(
    'title' => 'xAxis.plotBands.label',
    'description' => '
    Text labels for the plot bands
    ',
    'members' => array(
      'align' => array(
        'name' => 'align',
        'type' => 'String',
        'default' => '"center"',
        'desc' => '
        Horizontal alignment of the label. Can be one of "left", "center" or "right".
      ',
      ),
      'verticalAlign' => array(
        'name' => 'verticalAlign',
        'type' => 'String',
        'default' => '"top"',
        'desc' => '
        Vertical alignment of the label relative to the plot band. Can be one of "top", "middle" or "bottom".
      ',
      ),
      'rotation' => array(
        'name' => 'rotation',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        Rotation of the text label in degrees
      ',
      ),
      'style' => array(
        'name' => 'style',
        'type' => 'Object',
        'default' => '',
        'desc' => '
        CSS styles for the text label.
      ',
      ),
      'text' => array(
        'name' => 'text',
        'type' => 'String',
        'default' => '',
        'desc' => '
        The string text itself. A subset of HTML is supported.
      ',
      ),
      'textAlign' => array(
        'name' => 'textAlign',
        'type' => 'String',
        'default' => '',
        'desc' => '
        The text alignment for the label. While <code>align</code> determines
        where the texts anchor point is placed within the plot band, <code>textAlign</code> determines
        how the text is aligned against its anchor point. Possible values are
        "left", "center" and "right". Defaults to the same as
        the <code>align</code> option.
      ',
      ),
      'x' => array(
        'name' => 'x',
        'type' => 'Number',
        'default' => '',
        'desc' => '
        Horizontal position relative the alignment. Default varies by orientation.
      ',
      ),
      'y' => array(
        'name' => 'y',
        'type' => 'Number',
        'default' => '',
        'desc' => '
        Vertical position of the text baseline relative to the alignment. Default varies by orientation.
      ',
      ),
      ),
    ),
    'xAxis.plotLines' => array(
    'title' => 'xAxis.plotLines',
    'description' => '
    A line streching across the plot area, marking a specific value on one of the axes.
    ',
    'members' => array(
      'color' => array(
        'name' => 'color',
        'type' => 'Color',
        'default' => 'null',
        'desc' => '
        The color of the line.
      ',
      ),
      'dashStyle' => array(
        'name' => 'dashStyle',
        'type' => 'String',
        'default' => 'Solid',
        'desc' => '
        The dashing or dot style for the plot line. For possible values see <a href="http://jsfiddle.net/gh/get/jquery/1.7.1/highslide-software/highcharts.com/tree/master/samples/highcharts/plotoptions/series-dashstyle-all/">
        this overview</a>.
      ',
      ),
      'events' => array(
        'name' => 'events',
        'type' => 'Object',
        'default' => '',
        'desc' => '
        An object defining mouse events for the plot line. Supported properties are <code>click</code>,
        <code>mouseover</code>, <code>mouseout</code>, <code>mousemove</code>.
      ',
      ),
      'id' => array(
        'name' => 'id',
        'type' => 'String',
        'default' => 'null',
        'desc' => '
        An id used for identifying the plot line in Axis.removePlotLine.
      ',
      ),
      'label' => array(
        'name' => 'label',
        'type' => 'xAxis.plotLines.label',
        'default' => '',
        'desc' => '
        Text labels for the plot lines.
      ',
      ),
      'value' => array(
        'name' => 'value',
        'type' => 'Number',
        'default' => 'null',
        'desc' => '
        The position of the line in axis units.
      ',
      ),
      'width' => array(
        'name' => 'width',
        'type' => 'Number',
        'default' => 'null',
        'desc' => '
        The width or thickness of the plot line.
      ',
      ),
      'zIndex' => array(
        'name' => 'zIndex',
        'type' => 'Number',
        'default' => 'null',
        'desc' => '
        The z index of the plot line within the chart.
      ',
      ),
      ),
    ),
    'xAxis.plotLines.label' => array(
    'title' => 'xAxis.plotLines.label',
    'description' => '
    Text labels for the plot bands
    ',
    'members' => array(
      'align' => array(
        'name' => 'align',
        'type' => 'String',
        'default' => '"center"',
        'desc' => '
        Horizontal alignment of the label. Can be one of "left", "center" or "right".
      ',
      ),
      'verticalAlign' => array(
        'name' => 'verticalAlign',
        'type' => 'String',
        'default' => '"top"',
        'desc' => '
        Vertical alignment of the label relative to the plot band. Can be one of "top", "middle" or "bottom".
      ',
      ),
      'rotation' => array(
        'name' => 'rotation',
        'type' => 'Number',
        'default' => '',
        'desc' => '
        Rotation of the text label in degrees. Defaults to 0 for horizontal plot lines and 90 for vertical lines.
      ',
      ),
      'style' => array(
        'name' => 'style',
        'type' => 'Object',
        'default' => '',
        'desc' => '
        CSS styles for the text label.
      ',
      ),
      'textAlign' => array(
        'name' => 'textAlign',
        'type' => 'String',
        'default' => '',
        'desc' => '
        The text alignment for the label. While <code>align</code> determines
        where the texts anchor point is placed within the plot band, <code>textAlign</code> determines
        how the text is aligned against its anchor point. Possible values are
        "left", "center" and "right". Defaults to the same as
        the <code>align</code> option.
      ',
      ),
      'x' => array(
        'name' => 'x',
        'type' => 'Number',
        'default' => '',
        'desc' => '
        Horizontal position relative the alignment. Default varies by orientation.
      ',
      ),
      'y' => array(
        'name' => 'y',
        'type' => 'Number',
        'default' => '',
        'desc' => '
        Vertical position of the text baseline relative to the alignment. Default varies by orientation.
      ',
      ),
      ),
    ),
    'xAxis.title' => array(
    'title' => 'xAxis.title',
    'description' => '
    The axis title, showing next to the axis line.
    ',
    'members' => array(
      'align' => array(
        'name' => 'align',
        'type' => 'String',
        'default' => '"middle"',
        'desc' => '
        Alignment of the title relative to the axis values. Possible values
        are "low", "middle" or "high".
      ',
      ),
      'enabled' => array(
        'name' => 'enabled',
        'type' => 'String',
        'default' => '"middle"',
        'desc' => '
        Deprecated. Set the <code>text</code> to <code>null</code> to disable
        the title.
      ',
      ),
      'margin' => array(
        'name' => 'margin',
        'type' => 'Number',
        'default' => '',
        'desc' => '
        The pixel distance between the axis labels or line and the title. Defaults to 0 for
        horizontal axes, 10 for vertical
      ',
      ),
      'offset' => array(
        'name' => 'offset',
        'type' => 'Number',
        'default' => 'undefined',
        'desc' => '
        The distance of the axis title from the axis line. By default, this distance is
        computed from the offset width of the labels, the labels\' distance from
        the axis and the title\'s margin. However when the offset option is set, it
        overrides all this.
      ',
      ),
      'rotation' => array(
        'name' => 'rotation',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        The rotation of the text in degrees. 0 is horizontal, 270 is vertical
        reading from bottom to top.
      ',
      ),
      'style' => array(
        'name' => 'style',
        'type' => 'CSSObject',
        'default' => '',
        'desc' => '
        CSS styles for the title. When titles are rotated they are rendered using
        vector graphic techniques and not all styles are applicable. Most noteworthy,
        a bug in IE8 renders all rotated strings bold and italic. Defaults to:
  <pre>style: {
    color: \'#6D869F\',
    fontWeight: \'bold\'
  }</pre>
      ',
      ),
      'text' => array(
        'name' => 'text',
        'type' => 'String',
        'default' => 'null',
        'desc' => '
        The actual text of the axis title. It can contain basic HTML text markup like
        <b>, <i> and spans with style.
      ',
      ),
      ),
    ),
    'yAxis' => array(
    'title' => 'yAxis',
    'description' => '
    <p>The Y axis or value axis. Normally this is the vertical axis, though if the
    chart is inverted this is the horiontal axis. In case of multiple axes, the yAxis
    node is an array of configuration objects.</p>
    <p>See <a class="internal" href="#axis-object">the Axis object</a> for programmatic
    access to the axis.</p>
    ',
    'members' => array(
      'endOnTick' => array(
        'name' => 'endOnTick',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        Whether to force the axis to end on a tick. Use this option with the <code>maxPadding</code>
        option to control the axis end.
      ',
      ),
      'gridLineWidth' => array(
        'name' => 'gridLineWidth',
        'type' => 'Number',
        'default' => '1',
        'desc' => '
        The width of the grid lines extending the ticks across the plot area.
      ',
      ),
      'lineWidth' => array(
        'name' => 'lineWidth',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        The width of the line marking the axis itself.
      ',
      ),
      'maxPadding' => array(
        'name' => 'maxPadding',
        'type' => 'Number',
        'default' => '0.05',
        'desc' => '
        Padding of the max value relative to the length of the axis. A padding of 0.05 will make
        a 100px axis 5px longer. This is useful when you don\'t want the highest data
        value to appear on the edge of the plot area.
      ',
      ),
      'minPadding' => array(
        'name' => 'minPadding',
        'type' => 'Number',
        'default' => '0.05',
        'desc' => '
        Padding of the min value relative to the length of the axis. A padding of 0.05 will make
        a 100px axis 5px longer. This is useful when you don\'t want the lowest data
        value to appear on the edge of the plot area.
      ',
      ),
      'showLastLabel' => array(
        'name' => 'showLastLabel',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        Whether to show the last tick label.
      ',
      ),
      'stackLabels' => array(
        'name' => 'stackLabels',
        'type' => 'yAxis.stackLabels',
        'default' => '',
        'desc' => '
        Configuration object for the labels containing the stack total, usually displaying the total
        value for a stacked column or bar chart.
      ',
      ),
      'startOnTick' => array(
        'name' => 'startOnTick',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        Whether to force the axis to start on a tick. Use this option with the <code>maxPadding</code>
        option to control the axis start.
      ',
      ),
      'tickWidth' => array(
        'name' => 'tickWidth',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        The pixel width of the major tick marks.
      ',
      ),
      ),
    ),
    'yAxis.events' => array(
    'title' => 'yAxis.events',
    'description' => '',
    'members' => array(
      ),
    ),
    'yAxis.labels' => array(
    'title' => 'yAxis.labels',
    'description' => '',
    'members' => array(
      'align' => array(
        'name' => 'align',
        'type' => 'String',
        'default' => '"right"',
        'desc' => '
      What part of the string the given position is anchored to.
      Can be one of <code>"left"</code>, <code>"center"</code> or <code>"right"</code>.
      ',
      ),
      'x' => array(
        'name' => 'x',
        'type' => 'Number',
        'default' => '-8',
        'desc' => '
      The x position offset of the label relative to the tick position on the axis.
      ',
      ),
      'y' => array(
        'name' => 'y',
        'type' => 'Number',
        'default' => '3',
        'desc' => '
      The y position offset of the label relative to the tick position on the axis.
      ',
      ),
      ),
    ),
    'yAxis.stackLabels' => array(
    'title' => 'yAxis.stackLabels',
    'description' => '
    The stack labels show the total value for each bar in a stacked column or bar chart. The label will be placed on top of
    positive columns and below negative columns. In case of an inverted column chart or a bar chart the label is placed to
    the right of positive bars and to the left of negative bars.
    ',
    'members' => array(
      'align' => array(
        'name' => 'align',
        'type' => 'String',
        'default' => '',
        'desc' => '
        Defines the horizontal alignment of the stack total label.
        Can be one of <code>"left"</code>, <code>"center"</code> or <code>"right"</code>.
        The default value is calculated at runtime and depends on orientation and whether
        the stack is positive or negative.
      ',
      ),
      'textAlign' => array(
        'name' => 'textAlign',
        'type' => 'String',
        'default' => '',
        'desc' => '
        The text alignment for the label. While <code>align</code> determines
        where the texts anchor point is placed with regards to the stack, <code>textAlign</code> determines
        how the text is aligned against its anchor point. Possible values are
        <code>"left"</code>, <code>"center"</code> and <code>"right"</code>. The default value is calculated
        at runtime and depends on orientation and whether the stack is positive or negative.
      ',
      ),
      'verticalAlign' => array(
        'name' => 'verticalAlign',
        'type' => 'String',
        'default' => '',
        'desc' => '
        Defines the vertical alignment of the stack total label.
        Can be one of <code>"top"</code>,
        <code>"middle"</code> or <code>"bottom"</code>.
        The default value is calculated at runtime and depends on orientation and whether
        the stack is positive or negative.
      ',
      ),
      'enabled' => array(
        'name' => 'enabled',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => '
        Enable or disable the stack total labels.
      ',
      ),
      'formatter' => array(
        'name' => 'formatter',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        Callback JavaScript function to format the label. The value is
        given by <code>this.total</code>. Defaults to:
  <pre>function() {
    return this.total;
  }</pre>
      ',
      ),
      'style' => array(
        'name' => 'style',
        'type' => 'CSSObject',
        'default' => '',
        'desc' => '
        CSS styles for the label. Defaults to:
  <pre>style: {
    color: \'#666\',
    \'font-size\': \'11px\',
    \'line-height\': \'14px\'
  }
  </pre>
      ',
      ),
      'rotation' => array(
        'name' => 'rotation',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        Rotation of the labels in degrees.
      ',
      ),
      'x' => array(
        'name' => 'x',
        'type' => 'Number',
        'default' => '',
        'desc' => '
        The x position offset of the label relative to the left of the stacked bar. The default value is
        calculated at runtime and depends on orientation and whether the stack is positive or negative.
      ',
      ),
      'y' => array(
        'name' => 'y',
        'type' => 'Number',
        'default' => '',
        'desc' => '
        The y position offset of the label relative to the tick position
        on the axis. The default value is calculated at runtime and depends on orientation and whether
        the stack is positive or negative.
      ',
      ),
      ),
    ),
    'yAxis.title' => array(
    'title' => 'yAxis.title',
    'description' => '',
    'members' => array(
      'margin' => array(
        'name' => 'margin',
        'type' => 'Number',
        'default' => '40',
        'desc' => '
        The pixel distance between the axis labels and the title. Positive
        values are outside the axis line, negative are inside.
      ',
      ),
      'rotation' => array(
        'name' => 'rotation',
        'type' => 'Number',
        'default' => '270',
        'desc' => '
        The rotation of the text in degrees. 0 is horizontal, 270 is
        vertical reading from bottom to top.
      ',
      ),
      'text' => array(
        'name' => 'text',
        'type' => 'String',
        'default' => '"Y-values"',
        'desc' => '
        The actual text of the axis title. Horizontal texts can contain HTML,
        but rotated texts are painted using vector techniques and must be
        clean text. The Y axis title is disabled by setting the <code>text</code>
        option to <code>null</code>.
      ',
      ),
      ),
    ),
    'yAxis.plotBands' => array(
    'title' => 'yAxis.plotBands',
    'description' => '',
    'members' => array(
      ),
    ),
    'yAxis.plotLines' => array(
    'title' => 'yAxis.plotLines',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions' => array(
    'title' => 'plotOptions',
    'description' => '
      <p>The plotOptions is a wrapper object for config objects for each series type.
      The config objects for each series can also be overridden for each series
      item as given in the series array.</p>
      <p>Configuration options for the series are given in three levels. Options
      for all series in a chart are given in the <a class="internal" href="#plotOptions.series">plotOptions.series</a> object. Then options for all series
      of a specific type are given in the plotOptions of that type, for example plotOptions.line.
      Next, options for one single series are given in <a class="internal" href="#series">the
      series array</a>.</p>
    ',
    'members' => array(
      'area' => array(
        'name' => 'area',
        'type' => 'plotOptions.area',
        'default' => '',
        'desc' => '
        Area plot options
      ',
      ),
      'areaspline' => array(
        'name' => 'areaspline',
        'type' => 'plotOptions.areaspline',
        'default' => '',
        'desc' => '
        Areaspline plot options
      ',
      ),
      'bar' => array(
        'name' => 'bar',
        'type' => 'plotOptions.bar',
        'default' => '',
        'desc' => '
        Bar chart options
      ',
      ),
      'column' => array(
        'name' => 'column',
        'type' => 'plotOptions.column',
        'default' => '',
        'desc' => '
        Column chart options
      ',
      ),
      'line' => array(
        'name' => 'line',
        'type' => 'plotOptions.line',
        'default' => '',
        'desc' => '
        Line chart options
      ',
      ),
      'pie' => array(
        'name' => 'pie',
        'type' => 'plotOptions.pie',
        'default' => '',
        'desc' => '
        Pie chart options
      ',
      ),
      'series' => array(
        'name' => 'series',
        'type' => 'plotOptions.series',
        'default' => '',
        'desc' => '
        General options for all series regardless of type.
      ',
      ),
      'scatter' => array(
        'name' => 'scatter',
        'type' => 'plotOptions.scatter',
        'default' => '',
        'desc' => '
        Scatter chart options
      ',
      ),
      'spline' => array(
        'name' => 'spline',
        'type' => 'plotOptions.spline',
        'default' => '',
        'desc' => '
        Spline chart options
      ',
      ),
      ),
    ),
    'plotOptions.series' => array(
    'title' => 'plotOptions.series',
    'description' => '
      <p>General options for all series types.</p>

    ',
    'members' => array(
      'allowPointSelect' => array(
        'name' => 'allowPointSelect',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => '
        Allow this series\' points to be selected by clicking on the markers, bars or pie slices.
      ',
      ),
      'animation' => array(
        'name' => 'animation',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        Enable or disable the initial animation when a series is displayed. Since version 2.1, the animation
        can be set as a configuration object. Please note that this option only applies to the initial
        animation of the series itself. For other animations, see <a href="#chart" class="internal">
        #chart</a> => animation and the animation parameter under the API methods.
        The following properties are supported:
        <dl>
          <dt>duration</dt>
          <dd>The duration of the animation in milliseconds.</dd>

          <dt>easing</dt>
          <dd>When using jQuery as the general framework, the easing can be set to <code>linear</code> or
          <code>swing</code>. More easing functions are available with the use of jQuery plug-ins, most notably
          the jQuery UI suite. See <a href="http://api.jquery.com/animate/">the jQuery docs</a>. When using
          MooToos as the general framework, use the property name <code>transition</code> instead of <code>easing</code>.</dd>
        </dl>
      ',
      ),
      'color' => array(
        'name' => 'color',
        'type' => 'String',
        'default' => '',
        'desc' => '
        The main color or the series. In line type series it applies to the line and the point
        markers unless otherwise specified. In bar type series it applies to the bars
        unless a color is specified per point. The default value is pulled from the
        <code>options.colors</code> array.
      ',
      ),
      'connectNulls' => array(
        'name' => 'connectNulls',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => '
        Whether to connect a graph line across null points.
      ',
      ),
      'cropThreshold' => array(
        'name' => 'cropThreshold',
        'type' => 'Number',
        'default' => '300',
        'desc' => '
        When the series contains less points than the crop threshold, all points are drawn,
        event if the points fall outside the visible plot area at the current zoom. The advantage
        of drawing all points (including markers and columns), is that animation is performed on
        updates. On the other hand, when the series contains more points than the crop threshold,
        the series data is cropped to only contain points that fall within the plot area. The advantage
        of cropping away invisible points is to increase performance on large series.
      ',
      ),
      'cursor' => array(
        'name' => 'cursor',
        'type' => 'String',
        'default' => '\'\'',
        'desc' => '
        You can set the cursor to "pointer" if you have click events attached to
        the series, to signal to the user that the points and lines can be clicked.
      ',
      ),
      'dashStyle' => array(
        'name' => 'dashStyle',
        'type' => 'String',
        'default' => 'null',
        'desc' => '
        A name for the dash style to use for the graph. Applies only to series type having a graph,
        like <code>line</code>, <code>spline</code>, <code>area</code> and <code>scatter</code> in
        case it has a <code>lineWidth</code>. The value for the <code>dashStyle</code> include:
          <ul>
            <li>Solid</li>
            <li>ShortDash</li>
            <li>ShortDot</li>
            <li>ShortDashDot</li>
            <li>ShortDashDotDot</li>
            <li>Dot</li>
            <li>Dash</li>
            <li>LongDash</li>
            <li>DashDot</li>
            <li>LongDashDot</li>
            <li>LongDashDotDot</li>
          </ul>
      ',
      ),
      'dataLabels' => array(
        'name' => 'dataLabels',
        'type' => 'plotOptions.series.dataLabels',
        'default' => '',
        'desc' => '
        Defines the appearance of the data labels, static labels for each point.
      ',
      ),
      'enableMouseTracking' => array(
        'name' => 'enableMouseTracking',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        Enable or disable the mouse tracking for a specific series. This includes
        point tooltips and click events on graphs and points. For large datasets
        it improves performance.
      ',
      ),
      'events' => array(
        'name' => 'events',
        'type' => 'plotOptions.series.events',
        'default' => '',
        'desc' => '
        Event listeners for the series.
      ',
      ),
      'id' => array(
        'name' => 'id',
        'type' => 'String',
        'default' => 'null',
        'desc' => '
        An id for the series. This can be used after render time to get a pointer
        to the series object through <code>chart.get()</code>.
      ',
      ),
      'lineWidth' => array(
        'name' => 'lineWidth',
        'type' => 'Number',
        'default' => '2',
        'desc' => '
        Pixel with of the graph line.
      ',
      ),
      'marker' => array(
        'name' => 'marker',
        'type' => 'plotOptions.series.marker',
        'default' => '',
        'desc' => '
        Defines the appearance of the point markers.
      ',
      ),
      'point' => array(
        'name' => 'point',
        'type' => 'plotOptions.series.point',
        'default' => '',
        'desc' => '
      ',
      ),
      'pointStart' => array(
        'name' => 'pointStart',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        If no x values are given for the points in a series, pointStart defines
        on what value to start. For example, if a series contains one yearly value
        starting from 1945, set pointStart to 1945.
      ',
      ),
      'pointInterval' => array(
        'name' => 'pointInterval',
        'type' => 'Number',
        'default' => '1',
        'desc' => '
        <p>If no x values are given for the points in a series, pointInterval defines
        the interval of the x values. For example, if a series contains one value
        every decade starting from year 0, set pointInterval to 10.</p>
      ',
      ),
      'selected' => array(
        'name' => 'selected',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => '
        Whether to select the series initially. If <code>showCheckbox</code> is true,
        the checkbox next to the series name will be checked for a selected series.
      ',
      ),
      'shadow' => array(
        'name' => 'shadow',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        Whether to apply a drop shadow to the graph line.
      ',
      ),
      'showCheckbox' => array(
        'name' => 'showCheckbox',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => '
        If true, a checkbox is displayed
        next to the legend item to allow selecting the series. The state of the
        checkbox is determined by the <code>selected</code> option.
      ',
      ),
      'showInLegend' => array(
        'name' => 'showInLegend',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        Whether to display this particular series or series type in the legend.
      ',
      ),
      'stacking' => array(
        'name' => 'stacking',
        'type' => 'String',
        'default' => 'null',
        'desc' => '
        Whether to stack the values of each series on top of each other. Possible values
        are null to disable, "normal" to stack by value or "percent".
      ',
      ),
      'states' => array(
        'name' => 'states',
        'type' => 'plotOptions.series.states',
        'default' => '',
        'desc' => '
        A wrapper object for all the series options in specific states.
      ',
      ),
      'stickyTracking' => array(
        'name' => 'stickyTracking',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        Sticky tracking of mouse events. When true, the <code>mouseOut</code> event
        on a series isn\'t triggered until the mouse moves over another series, or out
        of the plot area. When false, the <code>mouseOut</code> event on a series is
        triggered when the mouse leaves the area around the series\' graph or markers.
        This also implies the tooltip. When <code>stickyTracking</code> is false, the
        tooltip will be hidden when moving the mouse between series.
      ',
      ),
      'tooltip' => array(
        'name' => 'tooltip',
        'type' => 'Object',
        'default' => '{}',
        'desc' => '
        A configuration object for the tooltip rendering of each single series. Properties are inherited
        from <a class="internal">#tooltip</a>. Overridable properties are <code>headerFormat</code>, <code>pointFormat</code>,
        <code>valueDecimals</code>, <code>xDateFormat</code>, <code>valuePrefix</code> and <code>valueSuffix</code>.
      ',
      ),
      'turboThreshold' => array(
        'name' => 'turboThreshold',
        'type' => 'Number',
        'default' => '1000',
        'desc' => '
        When a series contains a data array that is longer than this, only one dimensional arrays of numbers,
        or two dimensional arrays with x and y values are allowed. Also, only the first
        point is tested, and the rest are assumed to be the same format. This saves expensive
        data checking and indexing in long series.
      ',
      ),
      'visible' => array(
        'name' => 'visible',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        Set the initial visibility of the series.
      ',
      ),
      'zIndex' => array(
        'name' => 'zIndex',
        'type' => 'Number',
        'default' => 'null',
        'desc' => '
        Define the z index of the series.
      ',
      ),
      ),
    ),
    'plotOptions.series.marker' => array(
    'title' => 'plotOptions.series.marker',
    'description' => '',
    'members' => array(
      'enabled' => array(
        'name' => 'enabled',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        Enable or disable the point marker.
      ',
      ),
      'fillColor' => array(
        'name' => 'fillColor',
        'type' => 'Color',
        'default' => 'null',
        'desc' => '
        The fill color of the point marker. When <code>null</code>, the series\' or point\'s color
        is used.
      ',
      ),
      'lineColor' => array(
        'name' => 'lineColor',
        'type' => 'Color',
        'default' => '"#FFFFFF"',
        'desc' => '
        The color of the point marker\'s outline. When <code>null</code>, the series\' or point\'s color
        is used.
      ',
      ),
      'lineWidth' => array(
        'name' => 'lineWidth',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        The width of the point marker\'s outline.
      ',
      ),
      'radius' => array(
        'name' => 'radius',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        The radius of the point marker.
      ',
      ),
      'states' => array(
        'name' => 'states',
        'type' => 'plotOptions.series.marker.states',
        'default' => '',
        'desc' => '
        Interaction states for the point marker.
      ',
      ),
      'symbol' => array(
        'name' => 'symbol',
        'type' => 'String',
        'default' => 'null',
        'desc' => '
        A predefined shape or symbol for the marker. When null, the symbol is pulled from
        options.symbols. Other possible values are "circle", "square", "diamond", "triangle"
        and "triangle-down". Additionally, the URL to a graphic can be given on this form:
        "url(graphic.png)".
      ',
      ),
      ),
    ),
    'plotOptions.series.point' => array(
    'title' => 'plotOptions.series.point',
    'description' => 'Properties for each single point',
    'members' => array(
      'events' => array(
        'name' => 'events',
        'type' => 'plotOptions.series.point.events',
        'default' => '',
        'desc' => '
        Events for each single point
      ',
      ),
      ),
    ),
    'plotOptions.series.point.events' => array(
    'title' => 'plotOptions.series.point.events',
    'description' => 'Events for each single point',
    'members' => array(
      'click' => array(
        'name' => 'click',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        <p></p>Fires when a point is clicked. The <code>this</code> keyword refers to the
        point object itself. One parameter, <code>event</code>, is passed to the function.
        This contains common event information based on jQuery or MooTools depending on
        which library is used as the base for Highcharts.<p></p>
        <p>If the <code>series.allowPointSelect</code>
        option is true, the default action for the point\'s click event is to toggle the
        point\'s select state. Returning <code>false</code> cansels this action.</p>
      ',
      ),
      'mouseOver' => array(
        'name' => 'mouseOver',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        Fires when the mouse enters the area close to the point. The <code>this</code> keyword refers to the
        point object itself. One parameter, <code>event</code>, is passed to the function.
        This contains common event information based on jQuery or MooTools depending on
        which library is used as the base for Highcharts.
      ',
      ),
      'mouseOut' => array(
        'name' => 'mouseOut',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        Fires when the mouse leaves the area close to the point. The <code>this</code> keyword refers to the
        point object itself. One parameter, <code>event</code>, is passed to the function.
        This contains common event information based on jQuery or MooTools depending on
        which library is used as the base for Highcharts.
      ',
      ),
      'remove' => array(
        'name' => 'remove',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        Fires when the point is removed using the <code>.remove()</code> method. The <code>this</code> keyword refers to the
        point object itself. One parameter, <code>event</code>, is passed to the function.
        Returning <code>false</code> cancels the operation.
      ',
      ),
      'select' => array(
        'name' => 'select',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        Fires when the point is selected either programatically or following a click
        on the point. The <code>this</code> keyword refers to the
        point object itself. One parameter, <code>event</code>, is passed to the function.
        Returning <code>false</code> cancels the operation.
      ',
      ),
      'unselect' => array(
        'name' => 'unselect',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        Fires when the point is unselected either programatically or following a click
        on the point. The <code>this</code> keyword refers to the
        point object itself. One parameter, <code>event</code>, is passed to the function.
        Returning <code>false</code> cancels the operation.
      ',
      ),
      'update' => array(
        'name' => 'update',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        Fires when the point is updated programmatically through the <code>.update()</code>
        method. The <code>this</code> keyword refers to the
        point object itself. One parameter, <code>event</code>, is passed to the function. The
        new point options can be accessed through <code>event.options</code>.
        Returning <code>false</code> cancels the operation.
      ',
      ),
      ),
    ),
    'plotOptions.series.marker.states' => array(
    'title' => 'plotOptions.series.marker.states',
    'description' => '',
    'members' => array(
      'hover' => array(
        'name' => 'hover',
        'type' => 'plotOptions.series.marker.states.hover',
        'default' => '',
        'desc' => 'Configuration options for the hovered point marker. Members are
      inherited from the default line configuration, but single members can be overridden.
      ',
      ),
      'select' => array(
        'name' => 'select',
        'type' => 'plotOptions.series.marker.states.select',
        'default' => '',
        'desc' => 'Configuration options for the selected point marker. Members are
      inherited from the default line configuration, but single members can be overridden.
      ',
      ),
      ),
    ),
    'plotOptions.series.marker.states.hover' => array(
    'title' => 'plotOptions.series.marker.states.hover',
    'description' => '',
    'members' => array(
      'enabled' => array(
        'name' => 'enabled',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        Enable or disable the point marker.
      ',
      ),
      'fillColor' => array(
        'name' => 'fillColor',
        'type' => 'Color',
        'default' => 'null',
        'desc' => '
        The fill color of the point marker. When <code>null</code>, the series\' or point\'s color
        is used.
      ',
      ),
      'lineColor' => array(
        'name' => 'lineColor',
        'type' => 'Color',
        'default' => '"#FFFFFF"',
        'desc' => '
        The color of the point marker\'s outline. When <code>null</code>, the series\' or point\'s color
        is used.
      ',
      ),
      'lineWidth' => array(
        'name' => 'lineWidth',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        The width of the point marker\'s outline.
      ',
      ),
      'radius' => array(
        'name' => 'radius',
        'type' => 'Number',
        'default' => '',
        'desc' => 'The radius of the point marker. In hover state, it defaults
      to the normal state\'s radius + 2.
      ',
      ),
      ),
    ),
    'plotOptions.series.marker.states.select' => array(
    'title' => 'plotOptions.series.marker.states.select',
    'description' => '
      The appearance of the point marker when selected. In order to allow a point to be
      selected, set the <code>series.allowPointSelect</code> option to true.
    ',
    'members' => array(
      'enabled' => array(
        'name' => 'enabled',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        Enable or disable visible feedback for selection.
      ',
      ),
      'fillColor' => array(
        'name' => 'fillColor',
        'type' => 'Color',
        'default' => 'null',
        'desc' => '
        The fill color of the point marker. When <code>null</code>, the series\' or point\'s color
        is used.
      ',
      ),
      'lineColor' => array(
        'name' => 'lineColor',
        'type' => 'Color',
        'default' => '"#000000"',
        'desc' => '
        The color of the point marker\'s outline. When <code>null</code>, the series\' or point\'s color
        is used.
      ',
      ),
      'lineWidth' => array(
        'name' => 'lineWidth',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        The width of the point marker\'s outline.
      ',
      ),
      'radius' => array(
        'name' => 'radius',
        'type' => 'Number',
        'default' => '',
        'desc' => 'The radius of the point marker. In hover state, it defaults
      to the normal state\'s radius + 2.
      ',
      ),
      ),
    ),
    'plotOptions.series.dataLabels' => array(
    'title' => 'plotOptions.series.dataLabels',
    'description' => '',
    'members' => array(
      'align' => array(
        'name' => 'align',
        'type' => 'String',
        'default' => '"center"',
        'desc' => 'The alignment of the data label compared to the point. Can be
      one of "left", "center" or "right".',
      ),
      'backgroundColor' => array(
        'name' => 'backgroundColor',
        'type' => 'Color',
        'default' => 'undefined',
        'desc' => 'The background color or gradient for the data label.',
      ),
      'borderColor' => array(
        'name' => 'borderColor',
        'type' => 'Color',
        'default' => 'undefined',
        'desc' => 'The border color for the data label.',
      ),
      'borderRadius' => array(
        'name' => 'borderRadius',
        'type' => 'Number',
        'default' => '0',
        'desc' => 'The border radius in pixels for the data label.',
      ),
      'borderWidth' => array(
        'name' => 'borderWidth',
        'type' => 'Number',
        'default' => '0',
        'desc' => 'The border width in pixels for the data label.',
      ),
      'color' => array(
        'name' => 'color',
        'type' => 'String',
        'default' => 'null',
        'desc' => 'The text color for the data labels.',
      ),
      'enabled' => array(
        'name' => 'enabled',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => 'Enable or disable the data labels.',
      ),
      'formatter' => array(
        'name' => 'formatter',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        Callback JavaScript function to format the data label. Available data are:
              <table>
          <tbody><tr>
            <td><code>this.percentage</code></td>
            <td>Stacked series and pies only. The point\'s percentage of the total.</td>
          </tr>
          <tr>
            <td><code>this.point</code></td>
            <td>The point object. The point name, if defined, is available
        through <code>this.point.name</code>.</td>
          </tr>
          <tr>
            <td><code>this.series</code>:</td>
            <td>The series object. The series name is available
        through <code>this.series.name</code>.</td>
          </tr>
          <tr>
            <td><code>this.total</code></td>
            <td>Stacked series only. The total value at this point\'s x value.</td>
          </tr>
          <tr>
            <td><code>this.x</code>:</td>
            <td>The y value.</td>
          </tr>
          <tr>
            <td><code>this.y</code>:</td>
            <td>The y value.</td>
          </tr>
        </tbody></table>
        Default value:
  <pre>formatter: function() {
    return this.y;
  }</pre>
      ',
      ),
      'padding' => array(
        'name' => 'padding',
        'type' => 'Number',
        'default' => '2',
        'desc' => 'When either the <code>borderWidth</code> or the <code>backgroundColor</code> is set, this
        is the padding within the box.',
      ),
      'rotation' => array(
        'name' => 'rotation',
        'type' => 'Number',
        'default' => '0',
        'desc' => 'Text rotation in degrees.',
      ),
      'shadow' => array(
        'name' => 'shadow',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => 'The shadow of the box. Works best with <code>borderWidth</code> or <code>backgroundColor</code>',
      ),
      'staggerLines' => array(
        'name' => 'staggerLines',
        'type' => '',
        'default' => '',
        'desc' => 'n/a for data labels',
      ),
      'step' => array(
        'name' => 'step',
        'type' => '',
        'default' => '',
        'desc' => 'n/a for data labels',
      ),
      'style' => array(
        'name' => 'style',
        'type' => 'CSSObject',
        'default' => '',
        'desc' => 'Styles for the label.',
      ),
      'x' => array(
        'name' => 'x',
        'type' => 'Number',
        'default' => '0',
        'desc' => 'The x position offset of the label relative to the point.',
      ),
      'y' => array(
        'name' => 'y',
        'type' => 'Number',
        'default' => '-6',
        'desc' => 'The y position offset of the label relative to the point.',
      ),
      ),
    ),
    'plotOptions.series.events' => array(
    'title' => 'plotOptions.series.events',
    'description' => '',
    'members' => array(
      'click' => array(
        'name' => 'click',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        Fires when the series is clicked. The <code>this</code> keyword refers to the
        series object itself. One parameter, <code>event</code>, is passed to the function.
        This contains common event information based on jQuery or MooTools depending on
        which library is used as the base for Highcharts. Additionally, <code>event.point</code>
        holds a pointer to the nearest point on the graph.
      ',
      ),
      'checkboxClick' => array(
        'name' => 'checkboxClick',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        Fires when the checkbox next to the series\' name in the legend is clicked.. The <code>this</code> keyword refers to the
        series object itself. One parameter, <code>event</code>, is passed to the function.
        The state of the checkbox is found by <code>event.checked</code>. Return <code>false</code>
        to prevent the default action which is to toggle the select state of the series.
      ',
      ),
      'hide' => array(
        'name' => 'hide',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        Fires when the series is hidden after chart generation time, either by clicking
        the legend item or by calling <code>.hide()</code>.
      ',
      ),
      'legendItemClick' => array(
        'name' => 'legendItemClick',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        Fires when the legend item belonging to the series is clicked.
        The <code>this</code> keyword refers to the
        series object itself. One parameter, <code>event</code>, is passed to the function.
        This contains common event information based on jQuery or MooTools depending on
        which library is used as the base for Highcharts. The default action is to toggle
        the visibility of the series. This can be prevented by returning <code>false</code>
        or calling <code>event.preventDefault()</code>.
      ',
      ),
      'mouseOver' => array(
        'name' => 'mouseOver',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        Fires when the mouse enters the graph. The <code>this</code> keyword refers to the
        series object itself. One parameter, <code>event</code>, is passed to the function.
        This contains common event information based on jQuery or MooTools depending on
        which library is used as the base for Highcharts.
      ',
      ),
      'mouseOut' => array(
        'name' => 'mouseOut',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        Fires when the mouse leaves the graph. The <code>this</code> keyword refers to the
        series object itself. One parameter, <code>event</code>, is passed to the function.
        This contains common event information based on jQuery or MooTools depending on
        which library is used as the base for Highcharts. If the
        <a class="internal" href="#plotOptions-series">stickyTracking</a> option is true,
        <code>mouseOut</code> doesn\'t happen before the mouse enters another graph or leaves
        the plot area.
      ',
      ),
      'show' => array(
        'name' => 'show',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        Fires when the series is shown after chart generation time, either by clicking
        the legend item or by calling <code>.show()</code>.
      ',
      ),
      ),
    ),
    'plotOptions.series.states' => array(
    'title' => 'plotOptions.series.states',
    'description' => '',
    'members' => array(
      'hover' => array(
        'name' => 'hover',
        'type' => 'plotOptions.series.states.hover',
        'default' => '',
        'desc' => 'Configuration options for the hovered line. Members are
      inherited from the default line configuration, but single members can be overridden.
      ',
      ),
      ),
    ),
    'plotOptions.series.states.hover' => array(
    'title' => 'plotOptions.series.states.hover',
    'description' => '',
    'members' => array(
      'enabled' => array(
        'name' => 'enabled',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        Enable separate styles for the hovered series to visualize that the user hovers
        either the series itself or the legend.
      ',
      ),
      'lineWidth' => array(
        'name' => 'lineWidth',
        'type' => 'Number',
        'default' => '2',
        'desc' => '
        Pixel with of the graph line.
      ',
      ),
      'marker' => array(
        'name' => 'marker',
        'type' => 'plotOptions.series.marker',
        'default' => '',
        'desc' => '
        Defines the appearance of the point markers.
      ',
      ),
      ),
    ),
    'plotOptions.series.states.hover.marker' => array(
    'title' => 'plotOptions.series.states.hover.marker',
    'description' => '',
    'members' => array(
      'states' => array(
        'name' => 'states',
        'type' => '',
        'default' => '',
        'desc' => '',
      ),
      ),
    ),
    'plotOptions.line' => array(
    'title' => 'plotOptions.line',
    'description' => '',
    'members' => array(
      'step' => array(
        'name' => 'step',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => '
        Whether to apply steps to the line.
      ',
      ),
      ),
    ),
    'plotOptions.line.marker' => array(
    'title' => 'plotOptions.line.marker',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.line.marker.states' => array(
    'title' => 'plotOptions.line.marker.states',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.line.marker.states.hover' => array(
    'title' => 'plotOptions.line.marker.states.hover',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.line.marker.states.select' => array(
    'title' => 'plotOptions.line.marker.states.select',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.line.dataLabels' => array(
    'title' => 'plotOptions.line.dataLabels',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.line.events' => array(
    'title' => 'plotOptions.line.events',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.line.point' => array(
    'title' => 'plotOptions.line.point',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.line.point.events' => array(
    'title' => 'plotOptions.line.point.events',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.line.states' => array(
    'title' => 'plotOptions.line.states',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.line.states.hover' => array(
    'title' => 'plotOptions.line.states.hover',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.line.states.hover.marker' => array(
    'title' => 'plotOptions.line.states.hover.marker',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.spline' => array(
    'title' => 'plotOptions.spline',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.spline.marker' => array(
    'title' => 'plotOptions.spline.marker',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.spline.marker.states' => array(
    'title' => 'plotOptions.spline.marker.states',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.spline.marker.states.hover' => array(
    'title' => 'plotOptions.spline.marker.states.hover',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.spline.marker.states.select' => array(
    'title' => 'plotOptions.spline.marker.states.select',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.spline.dataLabels' => array(
    'title' => 'plotOptions.spline.dataLabels',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.spline.events' => array(
    'title' => 'plotOptions.spline.events',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.spline.point' => array(
    'title' => 'plotOptions.spline.point',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.spline.point.events' => array(
    'title' => 'plotOptions.spline.point.events',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.spline.states' => array(
    'title' => 'plotOptions.spline.states',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.spline.states.hover' => array(
    'title' => 'plotOptions.spline.states.hover',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.spline.states.hover.marker' => array(
    'title' => 'plotOptions.spline.states.hover.marker',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.scatter' => array(
    'title' => 'plotOptions.scatter',
    'description' => '',
    'members' => array(
      'lineWidth' => array(
        'name' => 'lineWidth',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        The width of the line connecting the data points.
      ',
      ),
      ),
    ),
    'plotOptions.scatter.marker' => array(
    'title' => 'plotOptions.scatter.marker',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.scatter.marker.states' => array(
    'title' => 'plotOptions.scatter.marker.states',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.scatter.marker.states.hover' => array(
    'title' => 'plotOptions.scatter.marker.states.hover',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.scatter.marker.states.select' => array(
    'title' => 'plotOptions.scatter.marker.states.select',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.scatter.dataLabels' => array(
    'title' => 'plotOptions.scatter.dataLabels',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.scatter.events' => array(
    'title' => 'plotOptions.scatter.events',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.scatter.point' => array(
    'title' => 'plotOptions.scatter.point',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.scatter.point.events' => array(
    'title' => 'plotOptions.scatter.point.events',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.scatter.states' => array(
    'title' => 'plotOptions.scatter.states',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.scatter.states.hover' => array(
    'title' => 'plotOptions.scatter.states.hover',
    'description' => '',
    'members' => array(
      'lineWidth' => array(
        'name' => 'lineWidth',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        The width of the line connecting the data points.
      ',
      ),
      ),
    ),
    'plotOptions.scatter.states.hover.marker' => array(
    'title' => 'plotOptions.scatter.states.hover.marker',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.area' => array(
    'title' => 'plotOptions.area',
    'description' => '',
    'members' => array(
      'fillColor' => array(
        'name' => 'fillColor',
        'type' => 'Color',
        'default' => 'null',
        'desc' => '
        Fill color or gradient for the area. When <code>null</code>, the series\' <code>color</code>  is
        used with the series\' <code>fillOpacity</code>.
      ',
      ),
      'fillOpacity' => array(
        'name' => 'fillOpacity',
        'type' => 'Number',
        'default' => '.75',
        'desc' => '
        Fill opacity for the area.
      ',
      ),
      'lineColor' => array(
        'name' => 'lineColor',
        'type' => 'Color',
        'default' => 'null',
        'desc' => '
        A separate color for the graph line. By default the line takes the <code>color</code>
        of the series, but the lineColor setting allows setting a separate
        color for the line without altering the <code>fillColor</code>.
      ',
      ),
      'threshold' => array(
        'name' => 'threshold',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        The Y axis value to serve as the base for the area, for distinguishing
        between values above and below a threshold.
      ',
      ),
      ),
    ),
    'plotOptions.area.marker' => array(
    'title' => 'plotOptions.area.marker',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.area.marker.states' => array(
    'title' => 'plotOptions.area.marker.states',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.area.marker.states.hover' => array(
    'title' => 'plotOptions.area.marker.states.hover',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.area.marker.states.select' => array(
    'title' => 'plotOptions.area.marker.states.select',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.area.dataLabels' => array(
    'title' => 'plotOptions.area.dataLabels',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.area.events' => array(
    'title' => 'plotOptions.area.events',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.area.point' => array(
    'title' => 'plotOptions.area.point',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.area.point.events' => array(
    'title' => 'plotOptions.area.point.events',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.area.states' => array(
    'title' => 'plotOptions.area.states',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.area.states.hover' => array(
    'title' => 'plotOptions.area.states.hover',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.area.states.hover.marker' => array(
    'title' => 'plotOptions.area.states.hover.marker',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.areaspline' => array(
    'title' => 'plotOptions.areaspline',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.areaspline.marker' => array(
    'title' => 'plotOptions.areaspline.marker',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.areaspline.marker.states' => array(
    'title' => 'plotOptions.areaspline.marker.states',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.areaspline.marker.states.hover' => array(
    'title' => 'plotOptions.areaspline.marker.states.hover',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.areaspline.marker.states.select' => array(
    'title' => 'plotOptions.areaspline.marker.states.select',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.areaspline.dataLabels' => array(
    'title' => 'plotOptions.areaspline.dataLabels',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.areaspline.events' => array(
    'title' => 'plotOptions.areaspline.events',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.areaspline.point' => array(
    'title' => 'plotOptions.areaspline.point',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.areaspline.point.events' => array(
    'title' => 'plotOptions.areaspline.point.events',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.areaspline.states' => array(
    'title' => 'plotOptions.areaspline.states',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.areaspline.states.hover' => array(
    'title' => 'plotOptions.areaspline.states.hover',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.areaspline.states.hover.marker' => array(
    'title' => 'plotOptions.areaspline.states.hover.marker',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.column' => array(
    'title' => 'plotOptions.column',
    'description' => '',
    'members' => array(
      'borderColor' => array(
        'name' => 'borderColor',
        'type' => 'Color',
        'default' => '"#FFFFFF"',
        'desc' => '
        The color of the border surronding each column or bar.
      ',
      ),
      'borderRadius' => array(
        'name' => 'borderRadius',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        The corner radius of the border surronding each column or bar.
      ',
      ),
      'borderWidth' => array(
        'name' => 'borderWidth',
        'type' => 'Number',
        'default' => '1',
        'desc' => '
        The width of the border surronding each column or bar.
      ',
      ),
      'colorByPoint' => array(
        'name' => 'colorByPoint',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => '
        When using automatic point colors pulled from the <code>options.colors</code>
        collection, this option determines whether the chart should receive
        one color per series or one color per point.
      ',
      ),
      'cropThreshold' => array(
        'name' => 'cropThreshold',
        'type' => 'Number',
        'default' => '50',
        'desc' => '
        When the series contains less points than the crop threshold, all points are drawn,
        event if the points fall outside the visible plot area at the current zoom. The advantage
        of drawing all points (including markers and columns), is that animation is performed on
        updates. On the other hand, when the series contains more points than the crop threshold,
        the series data is cropped to only contain points that fall within the plot area. The advantage
        of cropping away invisible points is to increase performance on large series.
      ',
      ),
      'groupPadding' => array(
        'name' => 'groupPadding',
        'type' => 'Number',
        'default' => '0.2',
        'desc' => '
        Padding between each value groups, in x axis units.
      ',
      ),
      'minPointLength' => array(
        'name' => 'minPointLength',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        The minimal height for a column or width for a bar. By default, 0 values
        are not shown. To visualize a 0 (or close to zero) point, set the minimal point length to a
        pixel value like 3. In stacked column charts, minPointLength might not be respected for tightly
        packed values.
      ',
      ),
      'pointPadding' => array(
        'name' => 'pointPadding',
        'type' => 'Number',
        'default' => '0.1',
        'desc' => '
        Padding between each column or bar, in x axis units.
      ',
      ),
      'pointWidth' => array(
        'name' => 'pointWidth',
        'type' => 'Number',
        'default' => 'null',
        'desc' => '
        A pixel value specifying a fixed width for each column or bar. When <code>null</code>,
        the width is calculated from the <code>pointPadding</code> and <code>groupPadding</code>.
      ',
      ),
      ),
    ),
    'plotOptions.column.dataLabels' => array(
    'title' => 'plotOptions.column.dataLabels',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.column.events' => array(
    'title' => 'plotOptions.column.events',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.column.point' => array(
    'title' => 'plotOptions.column.point',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.column.point.events' => array(
    'title' => 'plotOptions.column.point.events',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.column.states' => array(
    'title' => 'plotOptions.column.states',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.column.states.hover' => array(
    'title' => 'plotOptions.column.states.hover',
    'description' => '',
    'members' => array(
      'brightness' => array(
        'name' => 'brightness',
        'type' => 'Number',
        'default' => '0.1',
        'desc' => '
        How much to brighten the point on interaction. Requires the main color to be defined in hex or rgb(a) format.
      ',
      ),
      ),
    ),
    'plotOptions.column.states.hover.marker' => array(
    'title' => 'plotOptions.column.states.hover.marker',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.bar' => array(
    'title' => 'plotOptions.bar',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.bar.dataLabels' => array(
    'title' => 'plotOptions.bar.dataLabels',
    'description' => '',
    'members' => array(
      'align' => array(
        'name' => 'align',
        'type' => 'String',
        'default' => '"left"',
        'desc' => '
        Alignment of the data label relative to the data point.
      ',
      ),
      'x' => array(
        'name' => 'x',
        'type' => 'Number',
        'default' => '5',
        'desc' => '
        The x position of the data label relative to the data point.
      ',
      ),
      ),
    ),
    'plotOptions.bar.events' => array(
    'title' => 'plotOptions.bar.events',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.bar.point' => array(
    'title' => 'plotOptions.bar.point',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.bar.point.events' => array(
    'title' => 'plotOptions.bar.point.events',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.bar.states' => array(
    'title' => 'plotOptions.bar.states',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.bar.states.hover' => array(
    'title' => 'plotOptions.bar.states.hover',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.bar.states.hover.marker' => array(
    'title' => 'plotOptions.bar.states.hover.marker',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.pie' => array(
    'title' => 'plotOptions.pie',
    'description' => '',
    'members' => array(
      'borderColor' => array(
        'name' => 'borderColor',
        'type' => 'Color',
        'default' => '"#FFFFFF"',
        'desc' => '
        The color of the border surronding each slice.
      ',
      ),
      'borderWidth' => array(
        'name' => 'borderWidth',
        'type' => 'Number',
        'default' => '1',
        'desc' => '
        The width of the border surronding each slice.
      ',
      ),
      'center' => array(
        'name' => 'center',
        'type' => 'Array<String>',
        'default' => '[\'50%\', \'50%\']',
        'desc' => '
        The center of the pie chart relative to the plot area. Can be percentages
        or pixel values.
      ',
      ),
      'innerSize' => array(
        'name' => 'innerSize',
        'type' => 'String|Number',
        'default' => '0',
        'desc' => '
        The size of the inner diameter for the pie. A size greater than 0
        renders a donut chart. Can be a percentage
        or pixel value. Percentages are relative to the size of the plot area.
        Pixel values are given as integers.
      ',
      ),
      'showInLegend' => array(
        'name' => 'showInLegend',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => '
        Whether to display this particular series or series type in the legend. Since 2.1,
        pies are not shown in the legend by default.
      ',
      ),
      'size' => array(
        'name' => 'size',
        'type' => 'String|Number',
        'default' => '"75%"',
        'desc' => '
        The diameter of the pie relative to the plot area. Can be a percentage
        or pixel value. Pixel values are given as integers.
      ',
      ),
      'slicedOffset' => array(
        'name' => 'slicedOffset',
        'type' => 'Number',
        'default' => '10',
        'desc' => '
        If a point is sliced, moved out from the center, how many pixels should
        it be moved?
      ',
      ),
      ),
    ),
    'plotOptions.pie.dataLabels' => array(
    'title' => 'plotOptions.pie.dataLabels',
    'description' => '',
    'members' => array(
      'align' => array(
        'name' => 'align',
        'type' => 'String',
        'default' => 'null',
        'desc' => '
        N/A for pies.
      ',
      ),
      'connectorWidth' => array(
        'name' => 'connectorWidth',
        'type' => 'Number',
        'default' => '1',
        'desc' => '
        The width of the line connecting the data label to the pie slice
      ',
      ),
      'connectorColor' => array(
        'name' => 'connectorColor',
        'type' => 'String',
        'default' => '{point.color}',
        'desc' => '
        The color of the line connecting the data label to the pie slice. The default color
        is the same as the point\'s color.
      ',
      ),
      'connectorPadding' => array(
        'name' => 'connectorPadding',
        'type' => 'Number',
        'default' => '5',
        'desc' => '
        The distance from the data label to the connector
      ',
      ),
      'distance' => array(
        'name' => 'distance',
        'type' => 'Number',
        'default' => '30',
        'desc' => '
        The distance of the data label from the pie\'s edge. Negative numbers put the data
        label on top of the pie slices. Connectors are only shown for data labels outside
        the pie.
      ',
      ),
      'enabled' => array(
        'name' => 'enabled',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        Enable or disable the data labels.
      ',
      ),
      'softConnector' => array(
        'name' => 'softConnector',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        Whether to render the connector as a soft arc or a line with sharp break.
      ',
      ),
      ),
    ),
    'plotOptions.pie.events' => array(
    'title' => 'plotOptions.pie.events',
    'description' => '',
    'members' => array(
      'legendItemClick' => array(
        'name' => 'legendItemClick',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        Not applicable to pies, as the legend item is per point. See point.events.
      ',
      ),
      ),
    ),
    'plotOptions.pie.point' => array(
    'title' => 'plotOptions.pie.point',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.pie.point.events' => array(
    'title' => 'plotOptions.pie.point.events',
    'description' => '',
    'members' => array(
      'legendItemClick' => array(
        'name' => 'legendItemClick',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        Fires when the legend item belonging to the pie point (slice) is clicked.
        The <code>this</code> keyword refers to the
        point itself. One parameter, <code>event</code>, is passed to the function.
        This contains common event information based on jQuery or MooTools depending on
        which library is used as the base for Highcharts. The default action is to toggle
        the visibility of the point. This can be prevented by calling
        <code>event.preventDefault()</code>.
      ',
      ),
      ),
    ),
    'plotOptions.pie.states' => array(
    'title' => 'plotOptions.pie.states',
    'description' => '',
    'members' => array(
      ),
    ),
    'plotOptions.pie.states.hover' => array(
    'title' => 'plotOptions.pie.states.hover',
    'description' => '',
    'members' => array(
      'brightness' => array(
        'name' => 'brightness',
        'type' => 'Number',
        'default' => '.1',
        'desc' => '
        How much to brighten the point on interaction. Requires the main color to be defined in hex or rgb(a) format.
      ',
      ),
      ),
    ),
    'plotOptions.pie.states.hover.marker' => array(
    'title' => 'plotOptions.pie.states.hover.marker',
    'description' => '',
    'members' => array(
      ),
    ),
    'labels' => array(
    'title' => 'labels',
    'description' => '
    HTML labels that can be positioined anywhere in the chart area.
    ',
    'members' => array(
      'items' => array(
        'name' => 'items',
        'type' => 'Array<labels.item>',
        'default' => '',
        'desc' => '
        An array containing configuration for each label.
      ',
      ),
      'style' => array(
        'name' => 'style',
        'type' => 'CSSObject',
        'default' => '',
        'desc' => '
        Shared CSS styles for all labels. Defaults to:
  <pre>style: {
    color: \'#3E576F\'
  }</pre>
      ',
      ),
      ),
    ),
    'labels.items' => array(
    'title' => 'labels.items',
    'description' => '
    A HTML label that can be positioined anywhere in the chart area.
    ',
    'members' => array(
      'html' => array(
        'name' => 'html',
        'type' => 'String',
        'default' => '""',
        'desc' => '
        Inner HTML or text for the label.
      ',
      ),
      'style' => array(
        'name' => 'style',
        'type' => 'CSSObject',
        'default' => '',
        'desc' => '
        CSS styles for each label. To position the label, use left and top like this:
  <pre>style: {
    left: \'100px\',
    top: \'100px\'
  }</pre>
      ',
      ),
      ),
    ),
    'legend' => array(
    'title' => 'legend',
    'description' => '
    The legend is a box containing a symbol and name for each series item
    or point item in the chart.
    ',
    'members' => array(
      'align' => array(
        'name' => 'align',
        'type' => 'String',
        'default' => '"center"',
        'desc' => '
        The horizontal alignment of the legend box within the chart area.
      ',
      ),
      'backgroundColor' => array(
        'name' => 'backgroundColor',
        'type' => 'Color',
        'default' => 'null',
        'desc' => '
        The background color of the legend, filling the rounded corner border.
      ',
      ),
      'borderColor' => array(
        'name' => 'borderColor',
        'type' => 'Color',
        'default' => '#909090',
        'desc' => '
        The color of the drawn border around the legend.
      ',
      ),
      'borderRadius' => array(
        'name' => 'borderRadius',
        'type' => 'Number',
        'default' => '5',
        'desc' => '
        The border corner radius of the legend.
      ',
      ),
      'borderWidth' => array(
        'name' => 'borderWidth',
        'type' => 'Number',
        'default' => '1',
        'desc' => '
        The width of the drawn border around the legend.
      ',
      ),
      'enabled' => array(
        'name' => 'enabled',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        Enable or disable the legend.
      ',
      ),
      'floating' => array(
        'name' => 'floating',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => '
        When the legend is floating, the plot area ignores it and is allowed
        to be placed below it.
      ',
      ),
      'itemHiddenStyle' => array(
        'name' => 'itemHiddenStyle',
        'type' => 'CSSObject',
        'default' => '',
        'desc' => '
        CSS styles for each legend item when the corresponding series or point
        is hidden. Properties are inherited
        from <code>style</code> unless overridden here. Defaults to:
  <pre>itemHiddenStyle: {
    color: \'#CCC\'
  }</pre>
      ',
      ),
      'itemHoverStyle' => array(
        'name' => 'itemHoverStyle',
        'type' => 'CSSObject',
        'default' => '',
        'desc' => '
        CSS styles for each legend item in hover mode. Properties are inherited
        from <code>style</code> unless overridden here. Defaults to:
  <pre>itemHoverStyle: {
    color: \'#000\'
  }</pre>
      ',
      ),
      'itemStyle' => array(
        'name' => 'itemStyle',
        'type' => 'CSSObject',
        'default' => '',
        'desc' => '
        CSS styles for each legend item. Defaults to:
  <pre>itemStyle: {
    cursor: \'pointer\',
    color: \'#3E576F\'
  }</pre>
      ',
      ),
      'itemWidth' => array(
        'name' => 'itemWidth',
        'type' => 'Number',
        'default' => 'null',
        'desc' => '
        The width for each legend item. This is useful in a horizontal layout with
        many items when you want the items to align vertically.
      ',
      ),
      'layout' => array(
        'name' => 'layout',
        'type' => 'String',
        'default' => '"horizontal"',
        'desc' => '
        The layout of the legend items. Can be one of "horizontal" or "vertical".
      ',
      ),
      'labelFormatter' => array(
        'name' => 'labelFormatter',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        Callback function to format each of the series\' labels. The <em>this</em>
        keyword refers to the series object, or the point object in case of pie charts.
        Defaults to:
  <pre>labelFormatter: function() {
    return this.name
  }</pre>
      ',
      ),
      'lineHeight' => array(
        'name' => 'lineHeight',
        'type' => 'Number',
        'default' => '16',
        'desc' => '
        Line height for the legend items. Deprecated as of 2.1. Instead, the line height for each
        item can be set using itemStyle.lineHeight, and the padding between items using itemStyle.paddingBottom.
      ',
      ),
      'margin' => array(
        'name' => 'margin',
        'type' => 'Number',
        'default' => '15',
        'desc' => '
        If the plot area sized is calculated automatically and the legend
        is not floating, the legend margin is the
        space between the legend and the axis labels or plot area.
      ',
      ),
      'reversed' => array(
        'name' => 'reversed',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => '
        Whether to reverse the order of the legend items compared to the order
        of the series or points as defined in the configuration object.
      ',
      ),
      'rtl' => array(
        'name' => 'rtl',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => '
        Whether to show the symbol on the right side of the text rather than the left side.
        This is common in Arabic and Hebraic.
      ',
      ),
      'shadow' => array(
        'name' => 'shadow',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => '
        Whether to apply a drop shadow to the legend. A <code>backgroundColor</code>
        also needs to be applied for this to take effect.
      ',
      ),
      'style' => array(
        'name' => 'style',
        'type' => 'CSSObject',
        'default' => '',
        'desc' => '
        CSS styles for the legend area. In the 1.x versions the position of the legend area
        was determined by CSS. In 2.x, the position is determined by properties like
        <code>align</code>, <code>verticalAlign</code>, <code>x</code> and <code>y</code>,
        but the styles are still parsed for backwards compatibility.
      ',
      ),
      'symbolPadding' => array(
        'name' => 'symbolPadding',
        'type' => 'Number',
        'default' => '5',
        'desc' => '
        The pixel padding between the legend item symbol and the legend item text.
      ',
      ),
      'symbolWidth' => array(
        'name' => 'symbolWidth',
        'type' => 'Number',
        'default' => '30',
        'desc' => '
        The pixel width of the legend item symbol.
      ',
      ),
      'verticalAlign' => array(
        'name' => 'verticalAlign',
        'type' => 'String',
        'default' => '"bottom"',
        'desc' => '
        The vertical alignment of the legend box. Can be one of "top", "middle" or
        "bottom". Vertical position can be further determined by the <code>y</code>
        option.
      ',
      ),
      'width' => array(
        'name' => 'width',
        'type' => 'Number',
        'default' => 'null',
        'desc' => '
        The width of the legend box, not including style.padding.
      ',
      ),
      'x' => array(
        'name' => 'x',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        The x offset of the legend relative to it\'s horizontal alignment <code>align</code>
        within chart.spacingLeft and chart.spacingRight. Negative
        x moves it to the left, positive x moves it to the right. The default value of
        15 together with <code>align: "center"</code> puts it in the center of the
        plot area.
      ',
      ),
      'y' => array(
        'name' => 'y',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        The vertical offset of the legend relative to it\'s vertical alignment
        <code>verticalAlign</code> within chart.spacingTop and chart.spacingBottom. Negative
        y moves it up, positive y moves it down.
      ',
      ),
      ),
    ),
    'loading' => array(
    'title' => 'loading',
    'description' => '
    The loading options control the appearance of the loading screen that covers the
    plot area on chart operations. This screen only appears after an explicit call
    to <code>chart.showLoading()</code>. It is a utility for developers to communicate
    to the end user that something is going on, for example while retrieving new data
    via an XHR connection. The "Loading..." text itself is not part of this configuration
    object, but part of the <code>lang</code> object.
    ',
    'members' => array(
      'hideDuration' => array(
        'name' => 'hideDuration',
        'type' => 'Number',
        'default' => '100',
        'desc' => '
        The duration in milliseconds of the fade out effect.
      ',
      ),
      'labelStyle' => array(
        'name' => 'labelStyle',
        'type' => 'CSSObject',
        'default' => '',
        'desc' => '
        CSS styles for the loading label <code>span</code>. Defaults to:
  <pre>labelStyle: {
    fontWeight: \'bold\',
    position: \'relative\',
    top: \'1em\'
  }</pre>
      ',
      ),
      'showDuration' => array(
        'name' => 'showDuration',
        'type' => 'Number',
        'default' => '100',
        'desc' => '
        The duration in milliseconds of the fade in effect.
      ',
      ),
      'style' => array(
        'name' => 'style',
        'type' => 'CSSObject',
        'default' => '',
        'desc' => '
        CSS styles for the loading screen that covers the plot area. Defaults to:
  <pre>style: {
    position: \'absolute\',
    backgroundColor: \'white\',
    opacity: 0.5,
    textAlign: \'center\'
  }</pre>
      ',
      ),
      ),
    ),
    'tooltip' => array(
    'title' => 'tooltip',
    'description' => '
    Options for the tooltip that appears when the user hovers over a series or point.
    ',
    'members' => array(
      'backgroundColor' => array(
        'name' => 'backgroundColor',
        'type' => 'Color',
        'default' => '"rgba(255, 255, 255, .85)"',
        'desc' => '
        The background color or gradient for the tooltip.
      ',
      ),
      'borderColor' => array(
        'name' => 'borderColor',
        'type' => 'Color',
        'default' => '"auto"',
        'desc' => '
        The color of the tooltip border. When <code>null</code>, the border takes the color of the
        corresponding series or point.
      ',
      ),
      'borderRadius' => array(
        'name' => 'borderRadius',
        'type' => 'Number',
        'default' => '5',
        'desc' => '
        The radius of the rounded border corners.
      ',
      ),
      'borderWidth' => array(
        'name' => 'borderWidth',
        'type' => 'Number',
        'default' => '2',
        'desc' => '
        The pixel width of the tooltip border.
      ',
      ),
      'crosshairs' => array(
        'name' => 'crosshairs',
        'type' => 'Mixed',
        'default' => 'null',
        'desc' => '
        Display crosshairs to connect the points with their corresponding axis values. The crosshairs can
        be defined as a boolean, an array of booleans or an object.
        <dl>
          <dt>Boolean</dt>
          <dd>If the crosshairs option is true, a single crosshair relating to the
          x axis will be shown.</dd>

          <dt>Array of booleans</dt>
          <dd>In an array of booleans, the first value turns on the x axis crosshair and the second
          value to the y axis crosshair. Use <code>[true, true]</code> to show complete crosshairs.</dd>

          <dt>Array of objects</dt>
          <dd>In an array of objects, the first value applies to the x axis crosshair and the second
          value to the y axis crosshair. For each dimension, a <code>width</code>, <code>color</code>,
          <code><a href="http://jsfiddle.net/gh/get/jquery/1.7.1/highslide-software/highcharts.com/tree/master/samples/highcharts/plotoptions/series-dashstyle-all/">dashStyle</a></code> and <code>zIndex</code> can be given.</dd>
      </dl>',
      ),
      'enabled' => array(
        'name' => 'enabled',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        Enable or disable the tooltip.
      ',
      ),
      'footerFormat' => array(
        'name' => 'footerFormat',
        'type' => 'String',
        'default' => 'false',
        'desc' => '
        A string to append to the tooltip format.
      ',
      ),
      'formatter' => array(
        'name' => 'formatter',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        <p>Callback function to format the text of the tooltip. Return false to disable
        tooltip for a specific point on series.</p>
        <p>A subset of HTML is supported. The HTML of the tooltip is parsed and converted to SVG,
        therefore this isn\'t a complete HTML renderer. The following tabs are supported:
        <code><b></code>, <code><strong></code>,
        <code><i></code>, <code><em></code>, <code><br/></code>, <code><span></code>.
        Spans can be styled with a <code>style</code> attribute, but only text-related CSS that is
        shared with SVG is handled.
        </p>
        <p>Since version 2.1 the tooltip can be shared between multiple series through
        the <code>shared</code> option. The available data in the formatter differ a bit
        depending on whether the tooltip is shared or not. In a shared tooltip, all
        properties except <code>x</code>, which is common for all points, are kept in
        an array, <code>this.points</code>.</p>

        <p>Available data are:</p>
        <dl>
          <dt>this.percentage (not shared) / this.points[i].percentage (shared)</dt>
          <dd>Stacked series and pies only. The point\'s percentage of the total.</dd>

          <dt>this.point (not shared) / this.points[i].point (shared)</dt>
          <dd>The point object. The point name, if defined, is available
        through <code>this.point.name</code>.</dd>

          <dt>this.points</dt>
          <dd>In a shared tooltip, this is an array containing all other properties for each point.</dd>

          <dt>this.series (not shared) / this.points[i].series (shared)</dt>
          <dd>The series object. The series name is available
        through <code>this.series.name</code>.</dd>

          <dt>this.total (not shared) / this.points[i].total (shared)</dt>
          <dd>Stacked series only. The total value at this point\'s x value.</dd>

          <dt>this.x</dt>
          <dd>The x value. This property is the same regardless of the tooltip being shared or not.</dd>

          <dt>this.y (not shared) / this.points[i].y (shared)</dt>
          <dd>The y value.</dd>

        </dl>
      ',
      ),
      'pointFormat' => array(
        'name' => 'pointFormat',
        'type' => 'String',
        'default' => '',
        'desc' => '
        <p>The HTML of the point\'s line in the tooltip. Variables are enclosed by curly brackets. Available variables
        are point.x, point.y, series.name and series.color and other properties on the same form. Furthermore,
        point.y can be extended by the <code>tooltip.yPrefix</code>
        and <code>tooltip.ySuffix</code> variables. This can also be overridden for each series, which makes it a good hook for displaying
        units.</p>

        <p>Defaults to <code><span style="color:{series.color}">{series.name}</span>: <b>{point.y}</b><br/></code></p>
      ',
      ),
      'shadow' => array(
        'name' => 'shadow',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        Whether to apply a drop shadow to the tooltip.
      ',
      ),
      'shared' => array(
        'name' => 'shared',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => '
        When the tooltip is shared, the entire plot area will capture mouse movement, and tooltip texts
        for all series will be shown in a single bubble. This is recommended for single series charts
        and for iPad optimized sites.
      ',
      ),
      'snap' => array(
        'name' => 'snap',
        'type' => 'Number',
        'default' => '10/25',
        'desc' => '
        Proximity snap for graphs or single points. Does not apply to bars, columns
        and pie slices. It defaults to 10 for mouse-powered devices and 25 for touch
        devices.
      ',
      ),
      'style' => array(
        'name' => 'style',
        'type' => 'CSSObject',
        'default' => '',
        'desc' => '
        CSS styles for the tooltip. The tooltip can also be styled through the CSS
        class <code>.highcharts-tooltip</code>.
        Default value:
  <pre>style: {
    color: \'#333333\',
    fontSize: \'9pt\',
    padding: \'5px\'
  }</pre>
      ',
      ),
      'valueDecimals' => array(
        'name' => 'valueDecimals',
        'type' => 'Number',
        'default' => '',
        'desc' => '
        How many decimals to show in each series\' y value. This is overridable in each series\' tooltip options object.
        The default is to preserve all decimals.
      ',
      ),
      'valuePrefix' => array(
        'name' => 'valuePrefix',
        'type' => 'String',
        'default' => '""',
        'desc' => '
        A string to prepend to each series\' y value. Overridable in each series\' tooltip options object.
      ',
      ),
      'valueSuffix' => array(
        'name' => 'valueSuffix',
        'type' => 'String',
        'default' => '""',
        'desc' => '
        A string to append to each series\' y value. Overridable in each series\' tooltip options object.
      ',
      ),
      'xDateFormat' => array(
        'name' => 'xDateFormat',
        'type' => 'String',
        'default' => '',
        'desc' => '
        The format for the date in the tooltip header if the X axis is a datetime axis.
      ',
      ),
      'useHTML' => array(
        'name' => 'useHTML',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => '
        Use HTML to render the contents of the tooltip instead of SVG. Using HTML allows advanced formatting
        like tables and images in the tooltip. It is also recommended for rtl languages as it works around
        rtl bugs in early Firefox.
      ',
      ),
      ),
    ),
    'credits' => array(
    'title' => 'credits',
    'description' => '
      Highchart by default puts a credits label in the lower right corner of the chart.
      This can be changed using these options.
    ',
    'members' => array(
      'enabled' => array(
        'name' => 'enabled',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        Whether to show the credits text.
      ',
      ),
      'position' => array(
        'name' => 'position',
        'type' => 'Object',
        'default' => '',
        'desc' => '
        Position configuration for the credtis label. Supported properties are
        <code>align</code>, <code>verticalAlign</code>, <code>x</code> and <code>y</code>.
        Defaults to
  <pre>position: {
    align: \'right\',
    x: -10,
    verticalAlign: \'bottom\',
    y: -5
  }</pre>
      ',
      ),
      'href' => array(
        'name' => 'href',
        'type' => 'String',
        'default' => '"http://www.highcharts.com"',
        'desc' => '
        The URL for the credits label.
      ',
      ),
      'style' => array(
        'name' => 'style',
        'type' => 'CSSObject',
        'default' => '',
        'desc' => '
        CSS styles for the credits label. Defaults to:
  <pre>itemStyle: {
    cursor: \'pointer\',
    color: \'#909090\',
    fontSize: \'10px\'

  }</pre>
      ',
      ),
      'text' => array(
        'name' => 'text',
        'type' => 'String',
        'default' => '"Highcharts.com"',
        'desc' => '
        The text for the credits label.
      ',
      ),
      ),
    ),
    'global' => array(
    'title' => 'global',
    'description' => '
      Global options that don\'t apply to each chart. These options, like the <code>lang</code>
      options, must be set using the <code>Highcharts.setOptions</code> method.
  <pre>Highcharts.setOptions({
    global: {
      useUTC: false
    }
  });</pre>
    ',
    'members' => array(
      'canvasToolsURL' => array(
        'name' => 'canvasToolsURL',
        'type' => 'String',
        'default' => '"http://www.highcharts.com/js/canvas-tools.js"',
        'desc' => '
        The URL to the additional file to lazy load for Android 2.x devices. These devices don\'t
        support SVG, so we download a helper file that contains <a href="http://code.google.com/p/canvg/">canvg</a>,
        its dependecy rbcolor, and our own CanVG Renderer class. To avoid hotlinking to our site,
        you can install canvas-tools.js on your own server and change this option accordingly.
      ',
      ),
      'useUTC' => array(
        'name' => 'useUTC',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        Whether to use UTC time for axis scaling, tickmark placement and time display in
        <code>Highcharts.dateFormat</code>. Advantages of using UTC is that the time
        displays equally regardless of the user agent\'s time zone settings. Local time
        can be used when the data is loaded in real time or when correct Daylight Saving Time
        transitions are required.
      ',
      ),
      ),
    ),
    'colors' => array(
    'title' => 'colors',
    'description' => '
    An array containing the default colors for the chart\'s series. When all
    colors are used, new colors are pulled from the start again. Defaults to:
  <pre>colors: [
    \'#4572A7\',
    \'#AA4643\',
    \'#89A54E\',
    \'#80699B\',
    \'#3D96AE\',
    \'#DB843D\',
    \'#92A8CD\',
    \'#A47D7C\',
    \'#B5CA92\'
  ]</pre>
    ',
    'members' => array(
      ),
    ),
    'symbols' => array(
    'title' => 'symbols',
    'description' => '
    An array containing the default symbols for the series point markers. When all
    symbols are used, new symbols are pulled from the start again. Defaults to:
  <pre>symbols: [
    \'circle\',
    \'diamond\',
    \'square\',
    \'triangle\',
    \'triangle-down\'
  ]</pre>
    ',
    'members' => array(
      ),
    ),
    'lang' => array(
    'title' => 'lang',
    'description' => '
      Language object. The language object is global and it can\'t
      be set on each chart initiation. Instead, use <code>Highcharts.setOptions</code> to
      set it before any chart is initiated.
  <pre>Highcharts.setOptions({
    lang: {
      months: [\'Janvier\', \'Février\', \'Mars\', \'Avril\', \'Mai\', \'Juin\',
        \'Juillet\', \'Août\', \'Septembre\', \'Octobre\', \'Novembre\', \'Décembre\'],
      weekdays: [\'Dimanche\', \'Lundi\', \'Mardi\', \'Mercredi\', \'Jeudi\', \'Vendredi\', \'Samedi\']
    }
  });</pre>
    ',
    'members' => array(
      'decimalPoint' => array(
        'name' => 'decimalPoint',
        'type' => 'String',
        'default' => '"."',
        'desc' => '
        The default decimal point used in the <code>Highcharts.numberFormat</code> method
        unless otherwise specified in the function arguments.
      ',
      ),
      'downloadPNG' => array(
        'name' => 'downloadPNG',
        'type' => 'String',
        'default' => '"Download PNG image"',
        'desc' => '
        Exporting module only. The text for the PNG download menu item.
      ',
      ),
      'downloadJPEG' => array(
        'name' => 'downloadJPEG',
        'type' => 'String',
        'default' => '"Download JPEG image"',
        'desc' => '
        Exporting module only. The text for the JPEG download menu item.
      ',
      ),
      'downloadPDF' => array(
        'name' => 'downloadPDF',
        'type' => 'String',
        'default' => '"Download PDF document"',
        'desc' => '
        Exporting module only. The text for the PDF download menu item.
      ',
      ),
      'downloadSVG' => array(
        'name' => 'downloadSVG',
        'type' => 'String',
        'default' => '"Download SVG vector image"',
        'desc' => '
        Exporting module only. The text for the SVG download menu item.
      ',
      ),
      'exportButtonTitle' => array(
        'name' => 'exportButtonTitle',
        'type' => 'String',
        'default' => '"Export to raster or vector image"',
        'desc' => '
        Exporting module only. The tooltip text for the export button.
      ',
      ),
      'loading' => array(
        'name' => 'loading',
        'type' => 'String',
        'default' => 'Loading...',
        'desc' => '
        The loading text that appears when the chart is set into the loading state
        following a call to <code>chart.showLoading</code>.
      ',
      ),
      'months' => array(
        'name' => 'months',
        'type' => 'Array<String>',
        'default' => '[\'January\', \'February\', \'March\', \'April\', \'May\', \'June\', \'July\',
          \'August\', \'September\', \'October\', \'November\', \'December\']',
        'desc' => '
        An array containing the months names. Corresponds to the
        <code>%B</code> format in <code>Highcharts.dateFormat()</code>.
      ',
      ),
      'shortMonths' => array(
        'name' => 'shortMonths',
        'type' => 'Array<String>',
        'default' => '[\'Jan\', \'Feb\', \'Mar\', \'Apr\', \'May\', \'Jun\', \'Jul\',
          \'Aug\', \'Sep\', \'Oct\', \'Nov\', \'Dec\']',
        'desc' => '
        An array containing the months names in abbreviated form. Corresponds to the
        <code>%b</code> format in <code>Highcharts.dateFormat()</code>.
      ',
      ),
      'printButtonTitle' => array(
        'name' => 'printButtonTitle',
        'type' => 'String',
        'default' => '"Print the chart"',
        'desc' => '
        Exporting module only. The tooltip text for the print button.
      ',
      ),
      'resetZoom' => array(
        'name' => 'resetZoom',
        'type' => 'String',
        'default' => 'Reset zoom',
        'desc' => '
        The text for the label appearing when a chart is zoomed.
      ',
      ),
      'resetZoomTitle' => array(
        'name' => 'resetZoomTitle',
        'type' => 'String',
        'default' => 'Reset zoom level 1:1',
        'desc' => '
        The tooltip title for the label appearing when a chart is zoomed.
      ',
      ),
      'thousandsSep' => array(
        'name' => 'thousandsSep',
        'type' => 'String',
        'default' => '","',
        'desc' => '
        The default thousands separator used in the <code>Highcharts.numberFormat</code> method
        unless otherwise specified in the function arguments.
      ',
      ),
      'weekdays' => array(
        'name' => 'weekdays',
        'type' => 'Array<String>',
        'default' => '[\'Sunday\', \'Monday\', \'Tuesday\', \'Wednesday\', \'Thursday\', \'Friday\', \'Saturday\']',
        'desc' => '
        An array containing the weekday names.
      ',
      ),
      ),
    ),
    'series' => array(
    'title' => 'series',
    'description' => 'The actual series to append to the chart. In addition to
    the members listed below, any member of the <code>plotOptions</code> for that specific
    type of plot can be added to a series individually. For example, even though a general
    <code>lineWidth</code> is specified in <code>plotOptions.series</code>, an individual
    <code>lineWidth</code> can be specified for each series.',
    'members' => array(
      'data' => array(
        'name' => 'data',
        'type' => 'Array<Mixed>',
        'default' => '""',
        'desc' => '
        An array of data points for the series. The points can be given in three ways:
        <ol>
          <li>A list of numerical values. In this case, the numberical values will
          be interpreted and y values, and x values will be automatically calculated,
          either starting at 0 and incrementing by 1, or from <code>pointStart</code>
          and <code>pointInterval</code> given in the plotOptions. If the axis is
          has categories, these will be used. Example:
  <pre>data: [0, 5, 3, 5]</pre>
          </li>
          <li>A list of arrays with two values. In this case, the first value is the
          x value and the second is the y value. If the first value is a string, it is
          applied as the name of the point, and the x value is incremented following
          the above rules. Example:
  <pre>data: [[5, 2], [6, 3], [8, 2]]</pre></li>
          <li>A list of object with named values. In this case the objects are
          point configuration objects as seen under <a class="internal" href="#point">
          options.point</a>. Example:
  <pre>data: [{
    name: \'Point 1\',
    color: \'#00FF00\',
    y: 0
  }, {
    name: \'Point 2\',
    color: \'#FF00FF\',
    y: 5
  }]</pre></li>
        </ol>

      ',
      ),
      'dataParser' => array(
        'name' => 'dataParser',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        This method is deprecated as of version 2.0. Instead, use options preprocessing
        as described in <a href="http://highcharts.com/documentation/how-to-use">the
        how-to</a>, section #3.
      ',
      ),
      'dataURL' => array(
        'name' => 'dataURL',
        'type' => 'String',
        'default' => 'null',
        'desc' => '
        This method is deprecated as of version 2.0. Instead, load the data
        using jQuery.ajax and use options preprocessing
        as described in <a href="http://highcharts.com/documentation/how-to-use">the
        how-to</a>, section #3.
      ',
      ),
      'legendIndex' => array(
        'name' => 'legendIndex',
        'type' => 'Number',
        'default' => 'undefined',
        'desc' => '
        The sequential index of the series in the legend.

        <div class="demo">
          <a href="http://jsfiddle.net/gh/get/jquery/1.7.1/highslide-software/highcharts.com/tree/master/samples/highcharts/series/legendindex/">Legend in opposite order</a>
        </div>
      ',
      ),
      'name' => array(
        'name' => 'name',
        'type' => 'String',
        'default' => '""',
        'desc' => '
        The name of the series as shown in the legend, tooltip etc.
      ',
      ),
      'stack' => array(
        'name' => 'stack',
        'type' => 'Mixed',
        'default' => 'null',
        'desc' => '
        This option allows grouping series in a stacked chart. The stack option can be a string
        or a number or anything else, as long as the grouped series\' stack options match each other.
      ',
      ),
      'type' => array(
        'name' => 'type',
        'type' => 'String',
        'default' => '"line"',
        'desc' => '
        The type of series. Can be one of <code>area</code>, <code>areaspline</code>,
        <code>bar</code>, <code>column</code>, <code>line</code>, <code>pie</code>,
        <code>scatter</code> or <code>spline</code>.
      ',
      ),
      'xAxis' => array(
        'name' => 'xAxis',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        When using dual or multiple x axes, this number defines which xAxis the particular
        series is connected to. It refers to the index of the axis in the xAxis array, with
        0 being the first.
      ',
      ),
      'yAxis' => array(
        'name' => 'yAxis',
        'type' => 'Number',
        'default' => '0',
        'desc' => '
        When using dual or multiple y axes, this number defines which yAxis the particular
        series is connected to. It refers to the index of the axis in the yAxis array, with
        0 being the first.
      ',
      ),
      ),
    ),
    'point' => array(
    'title' => 'point',
    'description' => '
    Config options for the individual point as given in series.data.
    ',
    'members' => array(
      'color' => array(
        'name' => 'color',
        'type' => 'Color',
        'default' => 'null',
        'desc' => '
        Individual color for the point.
      ',
      ),
      'dataLabels' => array(
        'name' => 'dataLabels',
        'type' => 'Object',
        'default' => '',
        'desc' => '
        Individual data label for each point. The options are the same as the ones for
        <a class="internal" href="#plotOptions-series-dataLabels">plotOptions.series.dataLabels</a>
      ',
      ),
      'events' => array(
        'name' => 'events',
        'type' => 'plotOptions.series.point.events',
        'default' => 'null',
        'desc' => '
        Individual events for the point.
      ',
      ),
      'id' => array(
        'name' => 'id',
        'type' => 'String',
        'default' => 'null',
        'desc' => '
        An id for the point. This can be used after render time to get a pointer
        to the point object through <code>chart.get()</code>.
      ',
      ),
      'marker' => array(
        'name' => 'marker',
        'type' => 'plotOptions.series.marker',
        'default' => 'null',
        'desc' => '
        An individual point marker for the point.
      ',
      ),
      'legendIndex' => array(
        'name' => 'legendIndex',
        'type' => 'Number',
        'default' => 'undefined',
        'desc' => '
        Pies only. The sequential index of the pie slice in the legend.
      ',
      ),
      'name' => array(
        'name' => 'name',
        'type' => 'String',
        'default' => '""',
        'desc' => '
        The name of the point as shown in the legend, tooltip, dataLabel etc.
      ',
      ),
      'sliced' => array(
        'name' => 'sliced',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => '
        Pie series only. Whether to display a slice offset from the center.
      ',
      ),
      'x' => array(
        'name' => 'x',
        'type' => 'Number',
        'default' => 'null',
        'desc' => '
        The x value of the point
      ',
      ),
      'y' => array(
        'name' => 'y',
        'type' => 'Number',
        'default' => 'null',
        'desc' => '
        The y value of the point
      ',
      ),
      ),
    ),
    'point.events' => array(
    'title' => 'point.events',
    'description' => '',
    'members' => array(
      ),
    ),
    'point.marker' => array(
    'title' => 'point.marker',
    'description' => '',
    'members' => array(
      ),
    ),
    'navigation' => array(
    'title' => 'navigation',
    'description' => '
      A collection of options for buttons and menus appearing in the exporting module.
    ',
    'members' => array(
      'menuStyle' => array(
        'name' => 'menuStyle',
        'type' => 'CSSObject',
        'default' => '',
        'desc' => '
        CSS styles for the popup menu appearing by default when the export icon is
        clicked. This menu is rendered in HTML. Defaults to
  <pre>menuStyle: {
    border: \'1px solid #A0A0A0\',
    background: \'#FFFFFF\'
  }</pre>
      ',
      ),
      'menuItemStyle' => array(
        'name' => 'menuItemStyle',
        'type' => 'CSSObject',
        'default' => '',
        'desc' => '
        CSS styles for the individual items within the popup menu appearing by
        default when the export icon is
        clicked. The menu items are rendered in HTML. Defaults to
  <pre>menuItemStyle: {
    padding: \'0 5px\',
    background: NONE,
    color: \'#303030\'
  }</pre>
      ',
      ),
      'menuItemHoverStyle' => array(
        'name' => 'menuItemHoverStyle',
        'type' => 'CSSObject',
        'default' => '',
        'desc' => '
        CSS styles for the hover state of the individual items within the popup menu appearing by
        default when the export icon is
        clicked. The menu items are rendered in HTML. Defaults to
  <pre>menuItemHoverStyle: {
    background: \'#4572A5\',
    color: \'#FFFFFF\'
  }</pre>
      ',
      ),
      'buttonOptions' => array(
        'name' => 'buttonOptions',
        'type' => 'navigation.buttonOptions',
        'default' => '',
        'desc' => '
        General options for buttons like the Print and Export buttons.
      ',
      ),
      ),
    ),
    'navigation.buttonOptions' => array(
    'title' => 'navigation.buttonOptions',
    'description' => '
      A collection of options for buttons appearing in the exporting module.
    ',
    'members' => array(
      'align' => array(
        'name' => 'align',
        'type' => 'String',
        'default' => '"right"',
        'desc' => '
        Alignment for the buttons.
      ',
      ),
      'backgroundColor' => array(
        'name' => 'backgroundColor',
        'type' => 'Color',
        'default' => '',
        'desc' => '
        Background color or gradient for the buttons. Defaults to
  <pre>backgroundColor: {
    linearGradient: [0, 0, 0, 20],
    stops: [
      [0.4, \'#F7F7F7\'],
      [0.6, \'#E3E3E3\']
    ]
  }</pre>
      ',
      ),
      'borderColor' => array(
        'name' => 'borderColor',
        'type' => 'String',
        'default' => '"#B0B0B0"',
        'desc' => '
        The border color of the buttons
      ',
      ),
      'borderRadius' => array(
        'name' => 'borderRadius',
        'type' => 'Number',
        'default' => '3',
        'desc' => '
        The border corner radius of the buttons
      ',
      ),
      'borderWidth' => array(
        'name' => 'borderWidth',
        'type' => 'Number',
        'default' => '1',
        'desc' => '
        The border width of the buttons
      ',
      ),
      'enabled' => array(
        'name' => 'enabled',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        Whether to enable buttons
      ',
      ),
      'height' => array(
        'name' => 'height',
        'type' => 'Number',
        'default' => '20',
        'desc' => '
        Pixel height of the buttons
      ',
      ),
      'hoverBorderColor' => array(
        'name' => 'hoverBorderColor',
        'type' => 'String',
        'default' => '#909090',
        'desc' => '
        Color of the button border on hover
      ',
      ),
      'hoverSymbolFill' => array(
        'name' => 'hoverSymbolFill',
        'type' => 'String',
        'default' => '#81A7CF',
        'desc' => '
        Fill color for the symbol within the button on hover
      ',
      ),
      'hoverSymbolStroke' => array(
        'name' => 'hoverSymbolStroke',
        'type' => 'String',
        'default' => '#4572A5',
        'desc' => '
        Stroke (line) color for the symbol within the button on hover
      ',
      ),
      'symbolFill' => array(
        'name' => 'symbolFill',
        'type' => 'String',
        'default' => '#E0E0E0',
        'desc' => '
        Fill color for the symbol within the button
      ',
      ),
      'symbolSize' => array(
        'name' => 'symbolSize',
        'type' => 'Number',
        'default' => '12',
        'desc' => '
        The pixel size of the symbol on the button
      ',
      ),
      'symbolStroke' => array(
        'name' => 'symbolStroke',
        'type' => 'String',
        'default' => '"#A0A0A0"',
        'desc' => '
        The color of the symbol\'s stroke or line
      ',
      ),
      'symbolStrokeWidth' => array(
        'name' => 'symbolStrokeWidth',
        'type' => 'Number',
        'default' => '1',
        'desc' => '
        The pixel stroke width of the symbol on the button
      ',
      ),
      'symbolX' => array(
        'name' => 'symbolX',
        'type' => 'Number',
        'default' => '11.5',
        'desc' => '
        The x position of the center of the symbol inside the button.
      ',
      ),
      'symbolY' => array(
        'name' => 'symbolY',
        'type' => 'Number',
        'default' => '10.5',
        'desc' => '
        The y position of the center of the symbol inside the button.
      ',
      ),
      'verticalAlign' => array(
        'name' => 'verticalAlign',
        'type' => 'String',
        'default' => '"top"',
        'desc' => '
        The vertical alignment of the buttons. Can be one of "top", "middle" or
        "bottom".
      ',
      ),
      'width' => array(
        'name' => 'width',
        'type' => 'Number',
        'default' => '24',
        'desc' => '
        The pixel width of the button
      ',
      ),
      'y' => array(
        'name' => 'y',
        'type' => 'Number',
        'default' => '10',
        'desc' => '
        The vertical offset of the button\'s position relative to its <code>verticalAlign</code>.
      ',
      ),
      ),
    ),
    'exporting' => array(
    'title' => 'exporting',
    'description' => '
      Options for the exporting module.
    ',
    'members' => array(
      'buttons' => array(
        'name' => 'buttons',
        'type' => 'exporting.buttons',
        'default' => '',
        'desc' => '
        Configuration options for the buttons associated with the exporting module.
      ',
      ),
      'enabled' => array(
        'name' => 'enabled',
        'type' => 'Boolean',
        'default' => 'true',
        'desc' => '
        Whether to enable the exporting module.
      ',
      ),
      'enableImages' => array(
        'name' => 'enableImages',
        'type' => 'Boolean',
        'default' => 'false',
        'desc' => '
        Whether to enable images in the export. Including image point markers, background
        images etc.
      ',
      ),
      'filename' => array(
        'name' => 'filename',
        'type' => 'string',
        'default' => '"chart"',
        'desc' => '
        The filename, without extension, to use for the exported chart.
      ',
      ),
      'type' => array(
        'name' => 'type',
        'type' => 'String',
        'default' => '"image/png"',
        'desc' => '
        Default MIME type for exporting if <code>chart.exportChart()</code> is called
        without specifying a <code>type</code> option. Possible values are <code>image/png</code>,
        <code>image/jpeg</code>, <code>application/pdf</code> and <code>image/svg+xml</code>.
      ',
      ),
      'url' => array(
        'name' => 'url',
        'type' => 'String',
        'default' => 'http://export.highcharts.com',
        'desc' => '
        The URL for the server module converting the SVG string to an image format. By
        default this points to Highslide Software\'s free web service.
      ',
      ),
      'width' => array(
        'name' => 'width',
        'type' => 'Number',
        'default' => '800',
        'desc' => '
        The pixel width of charts exported to PNG or JPG.
      ',
      ),
      ),
    ),
    'exporting.buttons' => array(
    'title' => 'exporting.buttons',
    'description' => '
      Options for the export related buttons, print and export.
    ',
    'members' => array(
      'exportButton' => array(
        'name' => 'exportButton',
        'type' => 'exporting.buttons.exportButton',
        'default' => '',
        'desc' => '
        Options for the export button.
      ',
      ),
      'printButton' => array(
        'name' => 'printButton',
        'type' => 'exporting.buttons.printButton',
        'default' => '',
        'desc' => '
        Options for the print button.
      ',
      ),
      ),
    ),
    'exporting.buttons.exportButton' => array(
    'title' => 'exporting.buttons.exportButton',
    'description' => '
      Options for the export button.
    ',
    'members' => array(
      'hoverSymbolFill' => array(
        'name' => 'hoverSymbolFill',
        'type' => 'String',
        'default' => '#768F3E',
        'desc' => '
        See <a class="internal" href="#navigation-buttonOptions">navigation.buttonOptions</a>
        => hoverSymbolFill.
      ',
      ),
      'onclick' => array(
        'name' => 'onclick',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        A click handler callback to use on the button directly instead of the popup menu.
      ',
      ),
      'menuItems' => array(
        'name' => 'menuItems',
        'type' => 'Array<Object>',
        'default' => '',
        'desc' => '
        <p>A collection of config options for the menu items. Each options object consists
        of a <code>text</code> option which is a string to show in the menu item, as
        well as an <code>onclick</code> parameter which is a callback function to run
        on click.</p>
        <p>By default, there is one menu item for each of the available export types.
        Menu items can be customized by defining a new array of items and assigning
        <code>null</code> to unwanted positions (see override example below).</p>
      ',
      ),
      'symbol' => array(
        'name' => 'symbol',
        'type' => 'String',
        'default' => '"exportIcon"',
        'desc' => '
        The symbol for the button. Points to a definition function in the
        <code>Highcharts.Renderer.symbols</code> collection. The default
        <code>exportIcon</code> function is part of the exporting module.
      ',
      ),
      'symbolFill' => array(
        'name' => 'symbolFill',
        'type' => 'String',
        'default' => '#A8BF77',
        'desc' => '
        See <a class="internal" href="#navigation-buttonOptions">navigation.buttonOptions</a>
        => symbolFill.
      ',
      ),
      'x' => array(
        'name' => 'x',
        'type' => 'Number',
        'default' => '10',
        'desc' => '
        The horizontal positioin of the button relative to the <code>align</code>
        option.
      ',
      ),
      ),
    ),
    'exporting.buttons.printButton' => array(
    'title' => 'exporting.buttons.printButton',
    'description' => '
      Options for the print button.
    ',
    'members' => array(
      'hoverSymbolFill' => array(
        'name' => 'hoverSymbolFill',
        'type' => 'String',
        'default' => '#779ABF',
        'desc' => '
        See <a class="internal" href="#navigation-buttonOptions">navigation.buttonOptions</a>
        => hoverSymbolFill.
      ',
      ),
      'onclick' => array(
        'name' => 'onclick',
        'type' => 'Function',
        'default' => '',
        'desc' => '
        A click handler callback to use on the button directly. By default this onclick calls
        <code>chart.print()</code>, but it can be overridden to do other actions.
      ',
      ),
      'symbol' => array(
        'name' => 'symbol',
        'type' => 'String',
        'default' => '"printIcon"',
        'desc' => '
        The symbol for the button. Points to a definition function in the
        <code>Highcharts.Renderer.symbols</code> collection. The default
        <code>print</code> function is part of the exporting module.
      ',
      ),
      'symbolFill' => array(
        'name' => 'symbolFill',
        'type' => 'String',
        'default' => '#B5C9DF',
        'desc' => '
        See <a class="internal" href="#navigation-buttonOptions">navigation.buttonOptions</a>
        => symbolFill.
      ',
      ),
      'x' => array(
        'name' => 'x',
        'type' => 'Number',
        'default' => '-36',
        'desc' => '
        The horizontal positioin of the button relative to the <code>align</code>
        option.
      ',
      ),
      ),
    ),
  );
}
