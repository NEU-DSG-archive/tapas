(function ($) {
  Drupal.behaviors.geofieldMap = {
    attach: function(context, settings) {
      Drupal.geoField = Drupal.geoField || {};
      Drupal.geoField.maps = Drupal.geoField.maps || {};

      $('.geofieldMap', context).once('geofield-processed', function(index, element) {
        var data = undefined;
        var map_settings = [];
        var pointCount = 0;
        var resetZoom = true;
        var elemID = $(element).attr('id');

        if(settings.geofieldMap[elemID]) {
            data = settings.geofieldMap[elemID].data;
            map_settings = settings.geofieldMap[elemID].map_settings;
        }

        // Checking to see if google variable exists. We need this b/c views breaks this sometimes. Probably
        // an AJAX/external javascript bug in core or something.
        if (typeof google != 'undefined' && typeof google.maps.ZoomControlStyle != 'undefined' && data != undefined) {
          var features = GeoJSON(data);
          // controltype
          var controltype = map_settings.controltype;
          if (controltype == 'default') { controltype = google.maps.ZoomControlStyle.DEFAULT; }
          else if (controltype == 'small') { controltype = google.maps.ZoomControlStyle.SMALL; }
          else if (controltype == 'large') { controltype = google.maps.ZoomControlStyle.LARGE; }
          else { controltype = false }

          // map type
          var maptype = map_settings.maptype;
          if (maptype) {
            if (maptype == 'map' && map_settings.baselayers_map) { maptype = google.maps.MapTypeId.ROADMAP; }
            if (maptype == 'satellite' && map_settings.baselayers_satellite) { maptype = google.maps.MapTypeId.SATELLITE; }
            if (maptype == 'hybrid' && map_settings.baselayers_hybrid) { maptype = google.maps.MapTypeId.HYBRID; }
            if (maptype == 'physical' && map_settings.baselayers_physical) { maptype = google.maps.MapTypeId.TERRAIN; }
          }
          else { maptype = google.maps.MapTypeId.ROADMAP; }

          // menu type
          var mtc = map_settings.mtc;
          if (mtc == 'standard') { mtc = google.maps.MapTypeControlStyle.HORIZONTAL_BAR; }
          else if (mtc == 'menu' ) { mtc = google.maps.MapTypeControlStyle.DROPDOWN_MENU; }
          else { mtc = false; }
//ADDED from https://drupal.org/files/geofield_custom_marker-1856896-15.patch in regard to https://drupal.org/node/1856896
          //custom marker
          var iconPath = map_settings.custom_marker;
//END
          var myOptions = {
            zoom: parseInt(map_settings.zoom),
//ADDED from https://drupal.org/files/geofield-map_zoom_levels.patch in regards to https://drupal.org/node/1870130
            minZoom: parseInt(map_settings.min_zoom),
            maxZoom: parseInt(map_settings.max_zoom),
//END
            mapTypeId: maptype,
            mapTypeControl: (mtc ? true : false),
            mapTypeControlOptions: {style: mtc},
            zoomControl: ((controltype !== false) ? true : false),
            zoomControlOptions: {style: controltype},
            panControl: (map_settings.pancontrol ? true : false),
            scrollwheel: (map_settings.scrollwheel ? true : false),
            draggable: (map_settings.draggable ? true : false),
            overviewMapControl: (map_settings.overview ? true : false),
            overviewMapControlOptions: {opened: (map_settings.overview_opened ? true : false)},
            streetViewControl: (map_settings.streetview_show ? true : false),
            scaleControl: (map_settings.scale ? true : false),
            scaleControlOptions: {style: google.maps.ScaleControlStyle.DEFAULT}
          };

          var map = new google.maps.Map($(element).get(0), myOptions);
          // Store a reference to the map object so other code can interact
          // with it.
          Drupal.geoField.maps[elemID] = map;

          var range = new google.maps.LatLngBounds();

          var infowindow = new google.maps.InfoWindow({
            content: ''
          });

          if (features.setMap) {
            placeFeature(features, map, range);
            // Don't move the default zoom if we're only displaying one point.
            if (features.getPosition) {
              resetZoom = false;
            }
          } else {
            for (var i in features) {
              if (features[i].setMap) {
                placeFeature(features[i], map, range);
              } else {
                for (var j in features[i]) {
                  if (features[i][j].setMap) {
                    placeFeature(features[i][j], map, range);
                  }
                }
              }
            }
          }

//ADDED from https://drupal.org/files/geofield_map_center-6.patch in regards to https://drupal.org/node/1733864
//          if (resetZoom) {
//            map.fitBounds(range);
//This was added in patch 7, see https://drupal.org/node/1733864#comment-7687689 for an explanation
          for (first in features) break;
//        if (features.length >= 1 || map_settings.center=='') {
          if (first!='type') {
//end patch 7
            if (resetZoom) {
              map.fitBounds(range);
            } else {
              map.setCenter(range.getCenter());
            }
//END
          } else {
//ADDED from https://drupal.org/files/geofield_map_center-6.patch in regards to https://drupal.org/node/1733864
//            map.setCenter(range.getCenter());
            var center = map_settings.center.split(',');
            map.setCenter(new google.maps.LatLng(center[0], center[1]));
//END
          }
        }
        
        function placeFeature(feature, map, range) {
          var properties = feature.get('geojsonProperties');
          if (feature.setTitle && properties && properties.title) {
            feature.setTitle(properties.title);
          }
          feature.setMap(map);
//ADDED from https://drupal.org/files/geofield_custom_marker-1856896-15.patch in regard to https://drupal.org/node/1856896
          if (iconPath) {
            feature.setIcon(iconPath);
          }
//END
          if (feature.getPosition) {
            range.extend(feature.getPosition());
          } else {
            var path = feature.getPath();
            path.forEach(function(element) {
              range.extend(element);
            });
          }

          if (properties && properties.description) {
            var bounds = feature.get('bounds');
            google.maps.event.addListener(feature, 'click', function() {
              infowindow.setPosition(bounds.getCenter());
              infowindow.setContent(properties.description);
              infowindow.open(map);
            });
          }
        }
      });
    }
  }
})(jQuery);
