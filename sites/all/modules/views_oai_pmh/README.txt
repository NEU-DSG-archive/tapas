Views OAI-PMH
=============

This is a Views plug-in module which creates a OAI-PMH data provider using any 
fields which the Views module has access to.

It is configured to expose metadata in the following formats:
  Dublin Core (oai_dc)
  Learning Objects Metadata (oai_lom)
  Learning Resource Exchange (oai_lre)
  IMS Learning Objects Exchange (oai_ilox)

It can be extended to work with other OAI formats by modifying the file
includes/views_oai_pmh_metadata_type_definitions.inc. 

 
Prerequisite software
=====================

This module requires Views 3.x for both Drupal 6 and Drupal 7.  
It will not work with Views 2.x.


Using the "Views OAI-PMH" module
====================================

  1. Add a new "OAI-PMH" display to your view.
  2. Change its "Style/Format" to "OAI-PMH (auto)".
  3. Select your preferred "Row style" from the available options. To allow your
     OAI-PMH data provider to respond with any of the requested data types,
     choose the "Auto fields" option.
  4. Configure the Drupal-"field"-to-OAI-"element" mapping using the row style
     settings control - match up the Drupal fields in the left-hand column with
     the OAI elements in the right-hand column to create the mapping.
  5. Some metadata types create XML elements in the output that require certain
     attributes, such as language, to be specified. You can achieve this by
     mapping a Drupal field to an element labeled "(Attribute)". Sometimes you
     won't have this information available in your Drupal setup, in which case
     you can create a field of "Global > Custom Text", which you can map to the
     attribute as required.
  6. The module supports templating of OAI elements. Simply add your field to the
     Views fields list and create your template containing the XML code in the
     usual fashion. Find the appropriate template filename to use by clicking on
     Other > Theme > Information and locating your field in the list.
  7. Give the view a path such as "oai" and save it.
  8. Optionally, you can test your repository by going to http://re.cs.uct.ac.za/ 
     and entering the full url and path (http://example.com/oai) in the 
     "Enter the OAI baseURL :" box and then clicking on 
     "Test the specified/selected baseURL" (on the right side of the page).


Default Views
=============

A default view called "Biblio OAI-PMH" is provided as an example.  This view emulates the oai2 module.
