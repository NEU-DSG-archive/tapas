
README.txt for the Views Slideshow Xtra Example Module

This module creates working example Slideshow having multiple overlays. 

When the module is enabled, it creates an example Slide Content Type and default Slideshow View.

To generate example Slide nodes, go to:
Toolbar >> Configuration >> Media >> Views Slideshow Xtra >> Generate Example Nodes.
    
Removal of the Slide Content Type was omitted from this module's uninstall process,
with the idea that users could enable this module and then use the Slide Content Type
and Slideshow View it creates for their own slideshow, after deleting the example nodes.
Thus, uninstalling the this module does not delete the example nodes or the example content type.
They may be manually deleted from Content >> Content and Toolbar >> Structure >> Content types.  
The default Slideshow View is not deleted if it was enabled.

If this module is disabled, and you want to continue using the Slide Content Type and/or 
Slideshow View it created, then be sure to copy the CSS in views_slideshow_xtra_example.css to
one of your site's CSS files, as this CSS file will no longer be available, once this module
is disabled.

