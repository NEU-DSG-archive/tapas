// JavaScript Document
//ADDED from https://drupal.org/files/1666802-46-drag-compatible-containers.patch in regard to https://drupal.org/node/1666802

/**
 * @file
 * Extends tabledrag.js to restrict element containers in elements tables.
 */

// Registers the rules namespace.
Drupal.rules = Drupal.rules || {};

(function ($) {
  Drupal.behaviors.rules_tabledrag = {
    attach: function (context, settings) {
      for (var base in settings.rulesTableDrag) {
        if (Drupal.tableDrag[base]) {
          // Create rulesTableDrag wrapper.
          var tableDrag = Drupal.tableDrag[base];
          $('#' + base, context).once('rules-tabledrag', function () {
            Drupal.rulesTableDrag[base] = new Drupal.rulesTableDrag(tableDrag, settings.rulesTableDrag[base]);
          });
        }
      }
    }
  };

  /**
   * @constructor
   * Constructor for the rulesTableDrag wrapper.
   *
   * @param tableDrag
   *   An initialized tableDrag object.
   * @param constraints
   *   An array of constraints to restrict elements.
   */
  Drupal.rulesTableDrag = function (tableDrag, constraints) {
    var self = this;

    this.tableDrag = tableDrag;
    this.constraints = constraints;

    // Override methods.
    this.override(tableDrag, 'findDropTargetRow');

    // Override the row object.
    tableDrag.onDrag = function () {
      if (this.rowObject) {
        self.rowWrapper = new self.row(self, this.rowObject);
      }
      self.markEmbeddable(this.rowObject);
    };
    tableDrag.onDrop = function () {
      if (self.rowWrapper) {
        self.rowWrapper.dispose();
        self.rowWrapper = null;
      }
      self.unmarkEmbeddable();
    };
  };

  /**
   * Overrides tableDrag.findDropTargetRow() to track row change.
   */
  Drupal.rulesTableDrag.prototype.findDropTargetRow = function (x, y) {
    var row = this.invokeOriginal();
    this.rowChanged = this.previousFoundRow != row;
    this.previousFoundRow = row;
    return row;
  };

  /**
   * Finds the parent element of a row, optionally given a depth.
   */
  Drupal.rulesTableDrag.prototype.lookupParent = function (element, parentDepth) {
    // Fill in default arguments.
    element = $(element);
    if (parentDepth === undefined) {
      parentDepth = $('.indentation', element).length - 1;
    }
    else if (parentDepth == $('.indentation', element).length) {
      // Return row if it matches the requested parent depth.
      return element;
    }

    // Look up parent.
    var parent;
    if (parentDepth >= 0) {
      parent = element.prev('tr');
      while (parent.length && $('.indentation', parent).length != parentDepth) {
        parent = parent.prev('tr');
      }
    }
    // Return parent row or table if none was found.
    return parent && parent.length ? parent : element.closest('table');
  };

  /**
   * Matches an element for compatible container classes.
   */
  Drupal.rulesTableDrag.prototype.matchEmbeddable = function (element) {
    // Normalize elements into jQuery objects.
    element = $(element);

    var constraints = this.constraints;
    for (var i = 0; i < constraints.length; i ++) {
      // Match any element class.
      var match = false;
      for (var j = 0; j < constraints[i].element.length; j ++) {
        if (element.hasClass(constraints[i].element[j])) {
          // Return classes for matched rule.
          return constraints[i].embeddable;
        }
      }
    }

    // Return empty classes otherwise.
    return [];
  };

  /**
   * Matches an element against a container according to constraints.
   */
  Drupal.rulesTableDrag.prototype.containerIsCompatible = function (element, container) {
    // Normalize elements into jQuery objects.
    container = $(container);

    var embeddable = this.matchEmbeddable(element);
    if (embeddable.length) {
      // Match any container class.
      for (var i = 0; i < embeddable.length; i ++) {
        if (container.hasClass(embeddable[i])) {
          return true;
        }
      }
      return false;
    }
    // Assume container is compatible otherwise.
    return true;
  };

  /**
   * Marks embeddable containers.
   */
  Drupal.rulesTableDrag.prototype.markEmbeddable = function (row) {
    var embeddable = this.matchEmbeddable(row.element);
    // Add compatible container class.
    var context = $(this.tableDrag.table).closest('.rules-elements-table');
    var elements = [];
    for (var i = 0; i < embeddable.length; i ++) {
      elements.push($('.' + embeddable[i], context).get(0));
    }
    $(elements).not(row.element).addClass('rules-elements-embeddable');
    $(row.group).not(row.element).addClass('rules-elements-group');
  };

  /**
   * Unmarks embeddable containers.
   */
  Drupal.rulesTableDrag.prototype.unmarkEmbeddable = function () {
    $(this.tableDrag.table).closest('.rules-elements-table')
      .find('.rules-elements-embeddable').removeClass('rules-elements-embeddable').end()
      .find('.rules-elements-group').removeClass('rules-elements-group').end();
  };

  /**
   * @constructor
   * Constructor for the row DOM object.
   */
  Drupal.rulesTableDrag.prototype.row = function (rulesTableDrag, row) {
    var self = this;

    this.rulesTableDrag = rulesTableDrag;
    this.rowObject = row;

    // Override methods.
    this.override(row, 'isValidSwap');
    this.override(row, 'validIndentInterval');
    this.override(row, 'indent');
  };

  /**
   * Detaches the wrapper from the wrapped row.
   */
  Drupal.rulesTableDrag.prototype.row.prototype.dispose = function () {
    // Restore overridden methods.
    this.restore();
  };

  /**
   * Overrides tableDrag.row.isValidSwap() to determine if a move is possible.
   */
  Drupal.rulesTableDrag.prototype.row.prototype.isValidSwap = function (row) {
    // Check swap using default method.
    if (!this.invokeOriginal()) {
      return false;
    }
    // Check if row contains operations.
    else if ($(row).hasClass('rules-elements-add')) {
      return false;
    }

    return true;
  };

  /**
   * Overrides tableDrag.row.validIndentInterval() to filter containers.
   */
  Drupal.rulesTableDrag.prototype.row.prototype.validIndentInterval = function (prevRow, nextRow) {
    var interval = this.invokeOriginal();

    // Check if rows have changed.
    this.interval = this.interval || {};
    if (prevRow !== this.prevRowOld || nextRow !== this.nextRowOld) {
      this.prevRowOld = prevRow;
      this.nextRowOld = nextRow;

      this.interval.min = interval.min;
      this.interval.max = interval.max;

      // Trim incompatible containers.
      // In each iteration, trim both left and right such that if the boundaries
      // cross tableDrag.row.isValidSwap() will consider a swap invalid.
      while (this.interval.max >= this.interval.min) {
        var parent, minDiff = 0, maxDiff = 0;

        // Left trim.
        parent = prevRow ? this.rulesTableDrag.lookupParent(prevRow, this.interval.min - 1) : this.original.table;
        if (!this.rulesTableDrag.containerIsCompatible(this.rowObject.element, parent)) {
          minDiff ++;
        }

        // Right trim.
        parent = prevRow ? this.rulesTableDrag.lookupParent(prevRow, this.interval.max - 1) : this.original.table;
        if (!this.rulesTableDrag.containerIsCompatible(this.rowObject.element, parent)) {
          maxDiff --;
        }

        // Update interval.
        if (minDiff || maxDiff) {
          this.interval.min += minDiff;
          this.interval.max += maxDiff;
        }
        // Stop if interval is empty.
        else {
          break;
        }
      }
    }

    // Update interval.
    interval.min = this.interval.min;
    interval.max = this.interval.max;
    this.invalidRow = interval.min > interval.max;

    return interval;
  };

  /**
   * Overrides tableDrag.row.indent() to only indent if container is valid.
   */
  Drupal.rulesTableDrag.prototype.row.prototype.indent = function (indentDiff) {
    var self = this;

    // Abort if row is invalid.
    if (this.invalidRow) {
      return 0;
    }

    // Align indentation.
    var indentDiffOffset = this.invokeOriginal(0);
    indentDiff -= indentDiffOffset;

    // Check if indentation has changed.
    var indent = this.original.indents + indentDiff;
    if (indent !== this.indentOld || indentDiffOffset || this.rulesTableDrag.rowChanged) {
      this.indentOld = indent;

      // Adjust indent for compatible container.
      if (indent >= this.original.interval.min && indent <= this.original.interval.max) {
        var compatible = function (parentDepth) {
          // Find parent element with a given depth using previous row.
          var prevRow = $(self.rowObject.element).prev('tr');
          var parent = self.rulesTableDrag.lookupParent(prevRow.length ? prevRow : self.rowObject.table, parentDepth);
          return self.rulesTableDrag.containerIsCompatible(self.rowObject.element, parent);
        };
        if (!compatible(indent - 1)) {
          // Look up nearest depths with a compatible container.
          var left, right;
          for (left = indent; left > this.original.interval.min && !compatible(left - 1); left --);
          for (right = indent; right < this.original.interval.max && !compatible(right - 1); right ++);

          // Shift in the drag direction if halfway.
          if (indentDiff && 2 * indent == left + right) {
            indent = indentDiff > 0 ? right : left;
          }
          // Move to the left if less than halfway.
          else if (2 * indent < left + right) {
            indent = left;
          }
          // Move right otherwise.
          else {
            indent = right;
          }
        }
      }

      indentDiff = indent - this.original.indents;
      return this.invokeOriginal(indentDiff) + indentDiffOffset;
    }

    return indentDiffOffset;
  };

  // Utility functions:

  /**
   * Overrides a named method on an object.
   */
  Drupal.rulesTableDrag.prototype.override = function (object, name) {
    if (!this.overridden) {
      this.overridden = {};
    }

    if (this[name] && object[name]) {
      // Track this override.
      var original = object[name];
      this.overridden[name] = {
        'object': object,
        'method': original
      };

      // Override the method.
      var self = this;
      var substitute = this[name];
      object[name] = function () {
        // Stash old variables.
        var old = {original: null, invokeOriginal: null};
        for (var i in old) {
          old[i] = self[i];
        }

        // Set up object.
        var invokedArguments = Array.prototype.slice.call(arguments);
        self.original = object;
        self.invokeOriginal = function () {
          if (arguments.length > 0) {
            return original.apply(object, Array.prototype.slice.call(arguments));
          }
          else {
            return original.apply(object, invokedArguments);
          }
        };

        var result = substitute.apply(self, invokedArguments);

        // Unstash variables.
        for (var i in old) {
          self[i] = old[i];
        }
        return result;
      };
    }
  };

  /**
   * Restore overridden methods.
   */
  Drupal.rulesTableDrag.prototype.restore = function () {
    if (this.overridden) {
      // Get overridden methods in reverse.
      var reverse = [];
      for (var name in this.overridden) {
        reverse.unshift(name);
      }
      // Restore.
      for (var i = 0; i < reverse.length; i ++) {
        var override = this.overridden[reverse[i]];
        override.object[name] = override.method;
      }
    }
  };

  Drupal.rulesTableDrag.prototype.row.prototype.override = Drupal.rulesTableDrag.prototype.override;
  Drupal.rulesTableDrag.prototype.row.prototype.restore = Drupal.rulesTableDrag.prototype.restore;

})(jQuery);
//END