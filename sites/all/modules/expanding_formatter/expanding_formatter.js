
(function ($) {

/**
 * Auto-hide summary textarea if empty and show hide and unhide links.
 */
Drupal.behaviors.expandingFormatter = {
  attach: function (context, settings) {
    var expandingFormatters = $('.expanding-formatter', context);
    expandingFormatters.each(function(){
      var formatter = $(this);
      var trigger = $('a.trigger', formatter);
      if (trigger.length) {
        trigger.data('expandingFormatter', formatter);
        trigger.click(function(){
          var trigger = $(this);
          var formatter = trigger.data('expandingFormatter');
          trigger.fadeOut().remove();
          var content = $('.content', formatter);
          var ellipsis = $('.trim-ellipsis', formatter);
          var effect = 'slide';
          if (formatter.hasClass('fade')) {
            effect = 'fade';
          }
          switch(effect) {
            case 'slide': content.slideDown('normal', function(){
              ellipsis.fadeOut().remove();
              formatter.append(content.html());
              content.remove();
            }); break;
            case 'fade': content.fadeIn('normal', function(){
              ellipsis.fadeOut().remove();
              formatter.append(content.html());
              content.remove();
            }); break;
          }
        });
      }
    });
  }
};

})(jQuery);
