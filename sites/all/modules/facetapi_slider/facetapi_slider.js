(function ($) {

Drupal.behaviors.facetapi_slider = {
  attach: function(context, settings) {

    // Iterates over facets, applies slider widgets for block realm facets.
    for (var index in settings.facetapi.facets) {
      if (settings.facetapi.facets[index].makeSlider != null) {
        // Find the slider form with the matching Id.
        var $form = $('#' + settings.facetapi.facets[index].id, context);
        // Hide the form itself and create a slider.
        Drupal.facetapi_slider.makeSlider($form, settings.facetapi.facets[index]);
      }
    }
  }
}

/**
 * Class containing functionality for Facet API.
 */
Drupal.facetapi_slider = {};

/**
 * Applies the slider to a form.
 */
Drupal.facetapi_slider.makeSlider = function($form, settings) {
  var $wrapper = $( '<div id="slider-' + settings.id + '">\n\
    <span class="facetapi-slider-min"></span>\n\
    <span class="facetapi-slider-max" style="float: right;"></span>\n\
    <div class="facetapi-slider"></div></div>' ).insertAfter( $form );

  $wrapper.children('.facetapi-slider').slider({
    range: true,
    min: parseFloat(settings.sliderMin),
    max: parseFloat(settings.sliderMax),
    step: settings.sliderStep,
    values: [settings.sliderMinHandle, settings.sliderMaxHandle],
    slide: function (event, ui) {
      $form.find('input.facetapi-slider-min').val(ui.values[0]);
      $form.find('input.facetapi-slider-max').val(ui.values[1]);

      // Calculate the position of the slider handles
      var $real_width = $wrapper.children('.facetapi-slider').width();
      var $range = settings.sliderMax - settings.sliderMin;
      var $real_left = ui.values[0] - settings.sliderMin;
      var $real_right = settings.sliderMax - ui.values[1];
      var $real_left_position = ($real_width/$range)*$real_left-3;
      var $real_right_position = ($real_width/$range)*$real_right-3;
      // Set the handles with text and position
      $wrapper.children('.facetapi-slider-min').text(ui.values[0]);
      $wrapper.children('.facetapi-slider-min').css("margin-left", $real_left_position+"px");
      $wrapper.children('.facetapi-slider-max').text(ui.values[1]);
      $wrapper.children('.facetapi-slider-max').css("margin-right", $real_right_position+"px");
    },
    stop: function(event, ui) {
      $form.submit();
    },
    create: function() {
      // add classes to slider handles, handy for styling
      $('.facetapi-slider > a:eq(0)').addClass('handle-min');
      $('.facetapi-slider > a:eq(1)').addClass('handle-max');
    }
  });

  $form.find('input.facetapi-slider-min').val(settings.sliderMin);
  $form.find('input.facetapi-slider-max').val(settings.sliderMax);
  $wrapper.children('.facetapi-slider-min').text(settings.sliderMin);
  $wrapper.children('.facetapi-slider-max').text(settings.sliderMax);

  $form.hide();
}

})(jQuery);
