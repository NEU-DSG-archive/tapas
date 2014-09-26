(function ($) {
  // Simple Ajax fragment
  Drupal.behaviors.authcacheP13nAjaxFragments = {
    attach: function (context, settings) {
      $('span.authcache-ajax-frag', context).once('authcache-ajax-frag', function() {
        var $target = $(this);
        $.ajax({
          url: $target.data('authcache-ajax-src'),
          data: {v: $.cookie('aucp13n')},
          // Custom header to help prevent cross-site forgery requests
          // and to flag caching bootstrap that Ajax request is being made
          beforeSend: function(xhr) {
            xhr.setRequestHeader('X-Authcache','1');
          },
          success: function(data, status, xhr) {
            $target.authcacheP13nReplaceWith(data);
          }
        });
      });
    }
  };

  // Ajax settings
  Drupal.behaviors.authcacheP13nAjaxSettings = {
    attach: function (context, settings) {
      if (settings.authcacheP13nAjaxSettings) {
        $.each(settings.authcacheP13nAjaxSettings, function() {
          var url = this;

          $.ajax({
            url: url,
            data: {v: $.cookie('aucp13n')},
            dataType: 'json',
            // Custom header to help prevent cross-site forgery requests
            // and to flag caching bootstrap that Ajax request is being made
            beforeSend: function(xhr) {
              xhr.setRequestHeader('X-Authcache','1');
            },
            success: function(data, status, xhr) {
              $.authcacheP13nMergeSetting(data);
            }
          });
        });

        // Remove the urls we processed
        settings.authcacheP13nAjaxSettings = [];
      }
    }
  };

  // Ajax fragment assembly
  Drupal.behaviors.authcacheP13nAjaxAssemblies = {
    attach: function (context, settings) {
      if (settings.authcacheP13nAjaxAssemblies) {
        $.each(settings.authcacheP13nAjaxAssemblies, function(selector) {
          var targets = $(selector, context);
          var url = this;

          if (targets.length) {
            $.ajax({
              url: url,
              data: {v: $.cookie('aucp13n')},
              dataType: 'json',
              // Custom header to help prevent cross-site forgery requests
              // and to flag caching bootstrap that Ajax request is being made
              beforeSend: function(xhr) {
                xhr.setRequestHeader('X-Authcache','1');
              },
              success: function(data, status, xhr) {
                var response = {};
                response[selector] = data;

                $(context).authcacheP13nEachElementInSettings(response, function(markup) {
                  $(this).authcacheP13nReplaceWith(markup);
                });
              }
            });
          }
        });
      }
    }
  };
}(jQuery));
