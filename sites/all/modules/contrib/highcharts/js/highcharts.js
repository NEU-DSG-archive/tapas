(function($) {

Drupal.highcharts = Drupal.highcharts || {};
Drupal.highcharts.charts = Drupal.highcharts.charts || {};

/**
 * Core behavior for Highcharts.
 *
 * Check for and loop through any highchart containers in the output, and create
 * new chart objects to be rendered to their containers.
 */
Drupal.behaviors.highcharts = {
  charts: [],
	attach: function(context, settings) {
    $.each($(".highcharts-chart", context).not(".highcharts-processed"), function(idx, value) {
      chart_id = $(value).attr("id");
      var options = settings.highcharts[chart_id];
      if (options !== undefined) {
        // Create the chart.
        Drupal.highcharts.charts[chart_id] = new Highcharts.Chart(options);
        $(value).addClass("highcharts-processed");
      }
    })
  },
	detach: function(context) {
	}
};

})(jQuery);
