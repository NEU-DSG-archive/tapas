/**
 * @file
 * JavaScript integration between Highcharts and Drupal.
 * MODIFIED for use with TAPAS timeline - consolidates categories with same name
 */
 
(function ($) {

Drupal.behaviors.chartsHighcharts = {};
Drupal.behaviors.chartsHighcharts.attach = function(context, settings) {
  $('.charts-highchart').once('charts-highchart', function() {
    if ($(this).attr('data-chart')) {
    
      var config = $.parseJSON($(this).attr('data-chart'));
      
      // START - MODIFY FOR TAPAS
      
      var newCategoryData = {}; // New data structure is a hash table
      var categoryNameList = new Array(); // For keeping a (unique) list of category names 
      
      // Go through each category name
      
      $.each(config.xAxis[0].categories, function (index, category) {
      
        // If never seen this category before, initialize in new data structure
        //  and add it to the categoryNameList
      
        if (newCategoryData[category] == undefined) {
          newCategoryData[category] = 0;
          categoryNameList.push(category);
        }
        
        // Add this value to data structure
        
        newCategoryData[category] += config.series[0].data[index];
      });
      
      // Clear existing data
      
      config.xAxis[0].categories = new Array();
      config.series[0].data = new Array();
      
      // Reload data into config data
      
      console.log(categoryNameList);
      
      $.each(categoryNameList.sort(), function (index, categoryName) { 
        config.xAxis[0].categories.push(categoryName);
        config.series[0].data.push(newCategoryData[categoryName]);
      });

      // END - MODIFY FOR TAPAS
      
      $(this).highcharts(config);
    }
  });
};

})(jQuery);
