
-- SUMMARY --

The Views Dynamic Fields module provides a filter for use with Views module.
This filter allows the user to pick and choose which fields to display for a rendered 
instance of a view for that user. This provides a customized view instance for each user.
 
This module provides additional value when used with the Views Excel Export module 
(http://drupal.org/project/views_export_xls) to generate an xls file from a view. 
The xls file will only display the fields you have chosen on the rendered view instance in browser. 


-- REQUIREMENTS --

This module depends on the Views (http://drupal.org/project/views) module.


-- INSTALLATION --

 1. Place the entirety of this directory in sites/all/modules/views_dynamic_fields
 2. Navigate to administer >> build >> modules. Enable views_dynamic_fields.
 
 
-- CONFIGURATION --

None.


-- USAGE --

  1.  Create (or edit) a View by navigating to site building >> views >> add
  2.  Click on '+' near Filters to add a filter
  3.  Under 'Node', choose 'Node: Dynamic Fields'
  4.  Choose 'Provide exclusion mode' if you want the user to use the selected list of fields 
      to be 'excluded' from the view output. If this is not checked, then the selection list 
      will be included in the view and the unchecked fields will be excluded.
  5.  Click Update
  6.  Choose 'Expose' (this filter is to be used as an exposed filter)
  7.  More default filter options
  8.  Click Update
  9.  Add fields and other items to your view
  10. Make sure you have a 'path' set for a 'page' display of the view
  11. Click Save to save your view to the database.
  12. Navigate to the url set for the view. You should now see a list of fields of that view 
      with a checkbox next to each.
  13. Select the list of fields that you want to include/exclude (see step 4) in the current displayed instance
  14. Click Apply to see the view displayed with only the fields chosen
  
  
-- TROUBLESHOOTING --

* If you are not seeing the filter with the list of fields with checkboxes, 
  make sure you have 'exposed' the 'Node: Dynamic Fields' filter
  
-- CONTACT --

* Girish Nair (girishmuraly) - http://drupal.org/user/61249