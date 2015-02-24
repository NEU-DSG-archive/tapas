DESCRIPTION
The module extends Views with an option of using hierarchical taxonomy select in a capacity of the exposed filter. 

The module adds to Views a special filter that enables selecting taxonomy terms of the specified vocabulary consecutively. 
The selection considers terms hierarchy and uses AJAX for set of “select” type fields, in which a value of each next depends on the value of the previous. 
It works in combination with other taxonomy filters, including the same filter, used for the other vocabulary.


INSTALLATION
- Unpack the Views Hierarchical Taxonomy Filter module into your modules directory.
- Enable the module.
	Visit your site’s Administration > Modules page.
	Enable Views Hierarchical Taxonomy Filter.


APPLICATION
It is necessary to add a filer Content: Taxonomy hierarchical filter to Views.

In the settings of the filter it is enough to indicate an optional label for the field (it will be used for entire “select” field block) and a vocabulary, whose terms are used for the filtration.

It is not necessary to inform that the filter is exposed for users, if it would not be exposed, there would be no point in it. )

ADDITIONAL FEATURES

- Counter of connected nodes for each term.
- Exclude empty terms;


LIMITATIONS:
Filter will not work when autosubmit in the settings of the Exposed form style is enabled.