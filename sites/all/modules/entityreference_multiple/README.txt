This module provides a new field formatter for Entity Reference that let
you configure different view modes for each entities referenced. You
can split referenced entities in groups and choose a specific view mode
for each groups.

CONTENTS OF THIS FILE
---------------------

 * Requirements
 * Installation
 * Configure and use

************************************************************************

REQUIREMENTS
------------

This module require at least version 7.x of Drupal
and entityreference 7.x-1.0+

INSTALLATION
------------

See http://drupal.org/documentation/install/modules-themes/modules-7

CONFIGURE AND USE
----------------- 

You need to add an entity reference field (multiple!) to any bundle that
support fields. On "MANAGE DISPLAY" of your bundle you will have a new
option:
  Rendered entity with different view modes

When selecting this option you will have some specific settings.
First you need to select how many groups you will create with different
view mode for refenrenced entities. For exemple the first one, and next
three and all the rest: you need 3 groups.

For each group, you will have the possibility to select a view mode and
the number of elements. Fill "#" on the last group to display all last
entities.

************************************************************************

This module has been developed by Jean Valverde
http://www.developpeur-drupal.com

I provide paid development service, visit my website to get more info.

Jean Valverde.
contact@developpeur-drupal.com

