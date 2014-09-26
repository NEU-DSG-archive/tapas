(function ($) {
  Drupal.behaviors.authcacheContact = {
    attach: function (context, settings) {
      if (settings.authcacheContact) {
        $('#contact-site-form', context).once(function() {
          var $form = $(this);
          $form.find("input[name='name']").val(settings.authcacheContact.name);
          $form.find("input[name='mail']").val(settings.authcacheContact.mail);
        });
      }
    }
  };
}(jQuery));
