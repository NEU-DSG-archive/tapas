(function($) {
Drupal.behaviors.entityreferenceViewWidget = {
  attach: function(context, settings) {
    var checkboxes = '#modal-content input[name="entity_ids[]"]';
    $('#entityreference_view_widget_select_all').unbind('click').text('Select all').data('unselect', 0).click(function(){
      if ($(this).data('unselect')) {
        $(checkboxes).removeAttr('checked');
        $(this).data('unselect', 0).text('Select all');
      }
      else {
        $(checkboxes).attr('checked', 'checked');
        $(this).data('unselect', 1).text('Unselect all');
      }
      return false;
    });
    $('#entityreference-view-widget-modal-submit .button').click(function(){
      $('#modal-content .error').remove();

      var button = $(this);
      var selected_amount = $(checkboxes + ':checked').length;
      var field_name = $('#entityreference-view-widget-field-name').val();
      var field_frontend_name = field_name.replace(/\_/g, '-');
      var widget_settings = JSON.parse($('#entityreference-view-widget-' + field_frontend_name + '-settings').val());
      var offset = $('#' + widget_settings.table_id + ' tbody tr').length;
      var entity_ids = $(checkboxes).serialize();
      var query_string = entity_ids + '&field_name=' + field_name + '&langcode=' + widget_settings.langcode + '&target_type=' + widget_settings.target_type + '&cardinality=' + widget_settings.cardinality;

      $('#' + widget_settings.table_id + ' input[type=checkbox]:checked').each(function(){
        query_string += '&default_entity_ids[' + $(this).data('delta') + ']=' + $(this).val();
        selected_amount++;
      });

      if (widget_settings.cardinality > 0 && widget_settings.cardinality < selected_amount) {
        $('#modal-content').prepend('<div class="messages error">Please select no more than ' + widget_settings.cardinality + ' values.</div>')
      }
      else {
        $.ajax({
          url: '/?q=entityreference_view_widget/ajax',
          type: 'POST',
          dataType: 'html',
          data: query_string,
          success: function(data) {
            data && $('#' + widget_settings.table_id + ' tbody').html(data);
            $('#' + widget_settings.table_id + ' tbody tr').each(function(){
              var el = $(this);
              if (!el.find('.tabledrag-handle').length) {
                Drupal.tableDrag[widget_settings.table_id].makeDraggable(el.get(0));
                el.find('td:last').addClass('tabledrag-hide');
                if ($.cookie('Drupal.tableDrag.showWeight') == 1) {
                  el.find('.tabledrag-handle').hide();
                }
                else {
                  el.find('td:last').hide();
                }
              }
            });
            button.hasClass('modal-close') && Drupal.CTools.Modal.dismiss();
          }
        });
      }
    });
  }
}
})(jQuery);
