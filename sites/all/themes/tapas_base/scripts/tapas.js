(function($) {
    $('.form-radio[value=_none]').parent().hide(); 
})(jQuery);


(function ($) {

Drupal.behaviors.fixAjax = {};
Drupal.behaviors.fixAjax.attach = function (context, settings) {
  Drupal.ajax.prototype.eventResponse = function (element, event) {
    // Create a synonym for this to reduce code confusion.
    var ajax = this;

    // Do not perform another ajax command if one is already in progress.
    if (ajax.ajaxing) {
      return false;
    }

    try {
      if (ajax.form) {
        // If setClick is set, we must set this to ensure that the button's
        // value is passed.
        if (ajax.setClick) {
          // Mark the clicked button. 'form.clk' is a special variable for
          // ajaxSubmit that tells the system which element got clicked to
          // trigger the submit. Without it there would be no 'op' or
          // equivalent.
          element.form.clk = element;
        }

        ajax.form.ajaxSubmit(ajax.options);
      }
      else {
        ajax.beforeSerialize(ajax.element, ajax.options);
        $.ajax(ajax.options);
      }
    }
    catch (e) {
      // Unset the ajax.ajaxing flag here because it won't be unset during
      // the complete response.
      ajax.ajaxing = false;

      window.console = window.console || {};
      var methods = ['log', 'warn', 'error'];
      for (var i = 0; i < methods.length; i++) {
        window.console[methods[i]] = window.console[methods[i]] || function() {};
      }
      console.log("An error occurred while attempting to process " + ajax.options.url + ": " + e.message);
    }

    // For radio/checkbox, allow the default event. On IE, this means letting
    // it actually check the box.
    if (typeof element.type != 'undefined' && (element.type == 'checkbox' || element.type == 'radio')) {
      return true;
    }
    else {
      return false;
    }

  };
};

}(jQuery));
