Welcome to the future! Well, rather, to Droptor!
================================================
Droptor is a tool to monitor one or more Drupal sites in a single place. Droptor
is made up of this module and the droptor.com service. For each site you want to
monitor in Droptor you need to install this module.

This module feeds data to your Droptor.com account that our service analyzes and
displays for you in a nifty, central dashboard. Droptor will help make you site
faster and more secure by pointing out configuration problems with your site and
proactively warn you about odd behavior as it occurs on your site.

Installation
============
1. Enable the module
2. Go to droptor.com and sign up for a free account
3. In droptor.com, add this site. You will get a hash key. Copy it.
4. In your Drupal site go to admin/config/system/droptor and enter in that hash
5. Go back to droptor.com and hit the sync button.
6. Drink a tasty craft beer. (We recommend any beer from Rogue, Dogfish 
   Head, Lagunitas, Stone, Avery, or Drakes breweries.)

Troubleshooting
===============
Are you running Drupal behind a reverse prozy (like Varnish) or on a cloud
platform like Pantheon or Rackspace Cloud? If so you probably need to make
Drupal aware of the IP address of the proxy with something like this in your
settings.php:

	$conf['reverse_proxy'] = TRUE;
	$conf['reverse_proxy_addresses'] = array('IP ADDRESS OF PROXY');

Check the Support FAQ!
http://www.droptor.com/support

Contact support: Just email support@droptor.com or reach out in other ways:
http://www.droptor.com/support
