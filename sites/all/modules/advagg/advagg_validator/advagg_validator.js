/* global jQuery:false */
/* global Drupal:false */
/* global JSHINT:false */

/**
 * @file
 * Run JSHINT in the browser against the servers JS.
 */

/**
 * Have clicks to advagg_validator_js classes run JSHINT on the client.
 */
(function ($) {
  "use strict";
  Drupal.behaviors.advagg_validator = {
    attach: function (context, settings) {
      $('.advagg_validator_js', context).click(function (context) {
        // Get Results Div.
        var results = $(this).siblings('.results');
        // Clear out the results.
        $(results).html('');
        // Loop over each filename.
        $.each($(this).siblings('.filenames'), function() {
          var filename = $(this).val();
          try {
            var t = new Date().getTime();
            var x = jQuery.ajax({
              url: settings.basePath + filename + '?t=' + t,
              dataType: 'text',
              async: false
            });
            if (JSHINT(x.responseText, Drupal.settings.jshint, Drupal.settings.jshint.predef)) {
              $(results).append('<h4>' + filename + ' Passed!</h4>');
            } else {
              $(results).append('<p><h4>' + filename + ' Failed!</h4>');
              $(results).append('<ul>');
              for (var i = 0; i < JSHINT.errors.length; i++) {
                $(results).append('<li><b>' + JSHINT.errors[i].line + ':</b> ' + JSHINT.errors[i].reason + '</li>');
              }
              $(results).append('</ul></p>');
            }
          }
          catch (err) {
            $(results).append(err);
          }
        });

        return false;
      });
    }
  };
}(jQuery));
