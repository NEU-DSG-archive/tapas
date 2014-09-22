/**
 * @file
 * Attaches behavior for the Edit module.
 */
(function ($, _, Backbone, Drupal, drupalSettings) {

"use strict";

/**
 * The edit ARIA live message area.
 *
 * @todo Eventually the messages area should be converted into a Backbone View
 * that will respond to changes in the application's model. For the initial
 * implementation, we will call the Drupal.edit.setMessage method when an aural
 * message should be read by the user agent.
 */
var $messages;

Drupal.edit = Drupal.edit || {};
Drupal.edit.metadataCache = Drupal.edit.metadataCache || {};

/**
 * Attach toggling behavior and in-place editing.
 */
Drupal.behaviors.edit = {
  attach: function(context) {
    var $context = $(context);
    var $fields = $context.find('[data-edit-id]');

    // Initialize the Edit app.
    $context.find('#edit-trigger-link').once('edit-init', Drupal.edit.init);

    var annotateField = function(field) {
      if (_.has(Drupal.edit.metadataCache, field.editID)) {
        var meta = Drupal.edit.metadataCache[field.editID];

        field.$el.addClass((meta.access) ? 'edit-allowed' : 'edit-disallowed');
        if (meta.access) {
          field.$el
            .attr('data-edit-field-label', meta.label)
            .attr('aria-label', meta.aria)
            .addClass('edit-field edit-type-' + ((meta.editor === 'form') ? 'form' : 'direct'));
        }

        return true;
      }
      return false;
    };

    // Find all fields in the context without metadata.
    var fieldsToAnnotate = _.map($fields.not('.edit-allowed, .edit-disallowed'), function(el) {
      var $el = $(el);
      return { $el: $el, editID: $el.attr('data-edit-id') };
    });

    // Fields whose metadata is known (typically when they were just modified)
    // can be annotated immediately, those remaining must be requested.
    var remainingFieldsToAnnotate = _.reduce(fieldsToAnnotate, function(result, field) {
      if (!annotateField(field)) {
        result.push(field);
      }
      return result;
    }, []);

    // Make fields that could be annotated immediately available for editing.
    Drupal.edit.app.findEditableProperties($context);

    if (remainingFieldsToAnnotate.length) {
      // Create a Drupal.ajax instance to load the metadata. We use Drupal.ajax
      // plus AJAX commands because then the server side can load Create.js
      // PropertyEditor widgets (plus dependencies) when they are really needed,
      // instead of loading them on the initial page load.
      var toolbarAjax = Drupal.ajax['edit-trigger-link'];
      $.extend(toolbarAjax.options.data, {
        fields: _.pluck(remainingFieldsToAnnotate, 'editID')
      });

      // If necessary, the server side will generate AJAX commands to load the
      // Create.js PropertyEditor widgets, we don't need to handle that. We only
      // need to handle the editMetadata command, which is guaranteed to occur.
      toolbarAjax.commands.editMetadata = function (ajax, response, status) {
        // Update the metadata cache.
        _.each(response.results, function(metadata, editID) {
          Drupal.edit.metadataCache[editID] = metadata;
        });

        // Annotate the remaining fields based on the updated access cache.
        _.each(remainingFieldsToAnnotate, annotateField);

        // Find editable fields, make them editable.
        Drupal.edit.app.findEditableProperties($context);
      };

      // Trigger event, which will trigger the AJAX request.
      // @see edit_trigger_link()
      $('#edit-trigger-link').trigger('metadatafetch');
    }
  }
};


Drupal.edit.init = function() {
  // Append a messages element for appending interaction updates for screen
  // readers.
  $messages = $(Drupal.theme('editMessageBox')).appendTo($(this).parent());
  // Instantiate EditAppView, which is the controller of it all. EditAppModel
  // instance tracks global state (viewing/editing in-place).
  var appModel = new Drupal.edit.models.EditAppModel();
  var app = new Drupal.edit.EditAppView({
    el: $('body'),
    model: appModel
  });

  // Instantiate EditRouter.
  var editRouter = new Drupal.edit.routers.EditRouter({
    appModel: appModel
  });

  // Start Backbone's history/route handling.
  Backbone.history.start();

  // For now, we work with a singleton app, because for Drupal.behaviors to be
  // able to discover new editable properties that get AJAXed in, it must know
  // with which app instance they should be associated.
  Drupal.edit.app = app;
};

/**
 * Places the message in the edit ARIA live message area.
 *
 * The message will be read by speaking User Agents.
 *
 * @param {String} message
 *   A string to be inserted into the message area.
 */
Drupal.edit.setMessage = function(message) {
  var args = Array.prototype.slice.call(arguments);
  args.unshift('editMessage');
  $messages.html(Drupal.theme.apply(this, args));
};

})(jQuery, _, Backbone, Drupal, Drupal.settings);
