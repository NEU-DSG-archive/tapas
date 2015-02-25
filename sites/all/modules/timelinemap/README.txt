
-- SUMMARY --

Timeline Map module uses Timemap Google API. This is Views style plugin.
It allows you to load one or more datasets onto both a map and a timeline simultaneously. 
Only items in the visible range of the timeline are displayed on the map.


-- REQUIREMENTS --

* Drupal 7.x
* Views

-- INSTALLATION --

1. Download and unzip the Timeline Map module into your modules directory.

2. Goto http://timemap.googlecode.com/files/timemap.1.5.zip (on http://code.google.com/p/timemap/) download and unzip timemap.1.5.zip and copy timemap.js
   file in timelinemap/js directory.

3. Goto Administer > Modules and enable Timeline Map.

4. Goto Administer > Structure > Views and edit or add a view.

5. In the style options choose Timeline Map.

-- CONFIGURATION --

You will need a Google Maps API key for your website. You can set it on timelinemap settings page.
If you already have GMap module enabled, Timeline Map module will inherit GMap setting for Google Maps API key.
