(function ($) {
  Drupal.behaviors.authcacheViewsAdmin = {
    attach: function (context, settings) {
      if (Drupal.viewsUi) {
        $('#edit-authcache-status').change(function() {
          Drupal.viewsUi.resizeModal();
        });
      }
    }
  };
}(jQuery));
