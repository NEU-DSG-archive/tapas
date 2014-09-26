/**
 * @file
 * Provides utility functions for Edit.
 */
(function($, _, Drupal, drupalSettings) {

"use strict";

Drupal.edit = Drupal.edit || {};
Drupal.edit.util = Drupal.edit.util || {};

Drupal.edit.util.constants = {};
Drupal.edit.util.constants.transitionEnd = "transitionEnd.edit webkitTransitionEnd.edit transitionend.edit msTransitionEnd.edit oTransitionEnd.edit";

Drupal.edit.util.calcPropertyID = function(entity, predicate) {
  return entity.getSubjectUri() + '/' + predicate;
};

/**
 * Retrieves a setting of the editor-specific Edit UI integration.
 *
 * If the editor does not implement the optional getEditUISettings() method, or
 * if it doesn't set a value for a certain setting, then the default value will
 * be used.
 *
 * @param editor
 *   A Create.js PropertyEditor widget instance.
 * @param setting
 *   Name of the Edit UI integration setting.
 *
 * @return {*}
 */
Drupal.edit.util.getEditUISetting = function(editor, setting) {
  var settings = {};
  var defaultSettings = {
    padding: false,
    unifiedToolbar: false,
    fullWidthToolbar: false
  };
  if (typeof editor.getEditUISettings === 'function') {
    settings = editor.getEditUISettings();
  }
  return _.extend(defaultSettings, settings)[setting];
};

Drupal.edit.util.buildUrl = function(id, urlFormat) {
  var parts = id.split('/');
  return Drupal.formatString(decodeURIComponent(urlFormat), {
    '!entity_type': parts[0],
    '!id'         : parts[1],
    '!field_name' : parts[2],
    '!langcode'   : parts[3],
    '!view_mode'  : parts[4]
  });
};

Drupal.edit.util.form = {
  /**
   * Loads a form, calls a callback to inserts.
   *
   * Leverages Drupal.ajax' ability to have scoped (per-instance) command
   * implementations to be able to call a callback.
   *
   * @param options
   *   An object with the following keys:
   *    - $editorElement (required): the PredicateEditor DOM element.
   *    - propertyID (required): the property ID that uniquely identifies the
   *      property for which this form will be loaded.
   *    - nocssjs (required): boolean indicating whether no CSS and JS should be
   *      returned (necessary when the form is invisible to the user).
   * @param callback
   *   A callback function that will receive the form to be inserted, as well as
   *   the ajax object, necessary if the callback wants to perform other AJAX
   *   commands.
   */
  load: function(options, callback) {
    // Create a Drupal.ajax instance to load the form.
    Drupal.ajax[options.propertyID] = new Drupal.ajax(options.propertyID, options.$editorElement, {
      url: Drupal.edit.util.buildUrl(options.propertyID, drupalSettings.edit.fieldFormURL),
      event: 'edit-internal.edit',
      submit: { nocssjs : options.nocssjs },
      progress: { type : null } // No progress indicator.
    });
    // Implement a scoped editFieldForm AJAX command: calls the callback.
    Drupal.ajax[options.propertyID].commands.editFieldForm = function(ajax, response, status) {
      callback(response.data, ajax);
      // Delete the Drupal.ajax instance that called this very function.
      delete Drupal.ajax[options.propertyID];
      options.$editorElement.off('edit-internal.edit');
    };
    // This will ensure our scoped editFieldForm AJAX command gets called.
    options.$editorElement.trigger('edit-internal.edit');
  },

  /**
   * Creates a Drupal.ajax instance that is used to save a form.
   *
   * @param options
   *   An object with the following keys:
   *    - nocssjs (required): boolean indicating whether no CSS and JS should be
   *      returned (necessary when the form is invisible to the user).
   *
   * @return
   *   The key of the Drupal.ajax instance.
   */
  ajaxifySaving: function(options, $submit) {
    // Re-wire the form to handle submit.
    var element_settings = {
      url: $submit.closest('form').attr('action'),
      setClick: true,
      event: 'click.edit',
      progress: { type: null },
      submit: { nocssjs : options.nocssjs }
    };
    var base = $submit.attr('id');

    Drupal.ajax[base] = new Drupal.ajax(base, $submit[0], element_settings);
    // Reimplement the success handler to ensure Drupal.attachBehaviors() does
    // not get called on the form.
    Drupal.ajax[base].success = function (response, status) {
      for (var i in response) {
        if (response.hasOwnProperty(i) && response[i].command && this.commands[response[i].command]) {
          this.commands[response[i].command](this, response[i], status);
        }
      }
    };

    return base;
  },

  /**
   * Cleans up the Drupal.ajax instance that is used to save the form.
   *
   * @param $submit
   *   The jQuery-wrapped submit DOM element that should be unajaxified.
   */
  unajaxifySaving: function($submit) {
    delete Drupal.ajax[$submit.attr('id')];
    $submit.off('click.edit');
  }
};

})(jQuery, _, Drupal, Drupal.settings);
