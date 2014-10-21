pathArray = window.location.pathname.split( '/' );
pathArray1 = window.location.href.split('&url=');
pathArray2=window.location.href;
if(pathArray[3]=='group' || pathArray[2]=='edit' || pathArray2.indexOf('step=group_set_title') === 1){
jQuery("#edit-field-url-und-0-value").before("<div id='url-prefix'>Your project’s URL: http://tapasproject.org/</div>");
jQuery("#edit-field-title-und-0-value").before("<div id='url-prefix'>Your project’s name: </div>");
}else if(pathArray[1]=='node' && pathArray[3]!='group'){
jQuery("#edit-field-collection-url-und-0-value").before("<div id='url-prefix'>Your collection's URL: http://tapasproject.org/</div>"+pathArray1[1]+"/");
jQuery("#edit-field-title-und-0-value").before("<div id='url-prefix'>Your collection's name: </div>");
}else if(pathArray2.indexOf('step=group_set_title') === -1){
jQuery("#edit-field-collection-url-und-0-value").before("<div id='url-prefix'>Your collection's URL: http://tapasproject.org/</div>"+pathArray[1]+"/");
jQuery("#edit-field-title-und-0-value").before("<div id='url-prefix'>Your collection's name: </div>");
}

/*
jQuery("#edit-field-url-und-0-value").before("http://www.ptapascit.services.brown.edu/");
pathArray = window.location.pathname.split( '/' );
//alert(pathArray[1]);
if(pathArray[1]=='node'){
pathArray1 = window.location.href.split('&url=');
jQuery("#edit-field-collection-url-und-0-value").before("http://www.ptapascit.services.brown.edu"+pathArray1[1]+"/");
}else{
jQuery("#edit-field-collection-url-und-0-value").before("http://www.ptapascit.services.brown.edu/"+pathArray[1]+"/");
}
*/
