(function ($) {
  function displayContainerIfNotEmpty(container) {
    if (!/\S/.test(container.text())) {
      container.hide();
    }
    else {
      container.show();
    }
  }

  var tabs_container, actions_container;
  Drupal.behaviors.authcacheMenuFragments = {
    attach: function (context, settings) {
      if (tabs_container === undefined) {
        tabs_container = $('.tabs');
      }
      displayContainerIfNotEmpty(tabs_container);

      if (actions_container === undefined) {
        actions_container = $('.action-links');
      }
      displayContainerIfNotEmpty(actions_container);
    }
  };
}(jQuery));
