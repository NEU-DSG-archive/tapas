/*
 * Drupal.behaviors.ajaxHijackErrors
 * HiJack the ajax error reporting in D7. Avoiding the alert box for users
 * http://drupal.org/node/1232416 - comments: #93, #97, #98
 */
Drupal.behaviors.ajaxHijackErrors = {
  attach:function(context, settings){
    if (typeof context !== 'undefined') { //run only if there is a context var
      window.console = window.console || {};
      var methods = ['log', 'warn', 'error'];
      for (var i = 0; i < methods.length; i++) {
        window.console[methods[i]] = window.console[methods[i]] || function() {};
      } //end for
      
      $.ajaxSetup({
        beforeSend: function(jqXHR, settings) {
          settings.error = function(jqXHR, textStatus, errorThrown) { 
            //end user doesn't need to see debugging info
            {console.log('ajax error: ' + textStatus);};
          }; //end settings.error
        } //end beforeSend
      }); //end $.ajaxSetup
    } // end if (typeof context !== 'undefined')
  } // end attach:function(context, settings)
} //end Drupal.behaviors.ajaxHijackErrors
})(jQuery);