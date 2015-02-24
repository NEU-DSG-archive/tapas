CONTENTS OF THIS FILE
----------------------

  * Introduction
  * Installation
  * Where to get apc.php file from?


INTRODUCTION
------------
Maintainer: Tim Kamanin (http://drupal.org/user/49393)

Manage APC from Drupal administration interface. This module embeds default APC administrative interface (apc.php file) into Drupal admin.

INSTALLATION
------------
1. Copy apc_admin folder to modules directory.
2. At admin/build/modules enable the Menu position vocabulary module.
3. Go to admin/config/development/performance/apc_admin and generate a secret_key.
4. Copy apc.php into one of the suggested paths.

WHERE TO GET APC.PHP FILE FROM?
------------
APC.PHP ships with every APC installation, but the easiest way to get it
is to go to http://pecl.php.net/package/APC, download latest package and
extract apc.php from downloaded archive.