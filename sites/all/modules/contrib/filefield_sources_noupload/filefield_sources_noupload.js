(function ($) {
/**
 * Behavior to add source options to configured fields.
 */
Drupal.behaviors.fileFieldSourcesNoUpload = function(context) {
  $fileFieldSourcesLinks = $('div.filefield-sources-list a', context);
  $fileFieldElement = $fileFieldSourcesLinks.parents('div.form-item').find('div.filefield-element');
  //$fileFieldElement.find('div.filefield-upload').parent().css('display', 'none');
  // Get 'class' attribute of first element
  var fileFieldSourceClass = $fileFieldSourcesLinks.attr('class');
  if (fileFieldSourceClass) {
    // Get the class that matches the correct pattern
    var fileFieldSourceClassMatches = fileFieldSourceClass.match(/filefield-source-[0-9a-z]+/i);
    if (fileFieldSourceClassMatches) {
      $fileFieldElement.find('div.' + fileFieldSourceClassMatches[0]).css('display', '');
    }
  }
};

})(jQuery);