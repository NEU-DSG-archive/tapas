//FOR BULK UPLOAD
waitUntilExists("edit-field-upload-files-und--2-table",function(){
  // Remove any element-specific value, falling back to stylesheets
  document.getElementById('node_tei_bulk_upload_form_group_collection_and_rights').style.display='block';
  document.getElementById('edit-actions').style.display='block';
});
waitUntilExists("edit-field-upload-files-und-table",function(){
  // Remove any element-specific value, falling back to stylesheets
  document.getElementById('node_tei_bulk_upload_form_group_collection_and_rights').style.display='block';
  document.getElementById('edit-actions').style.display='block';
});
//FOR SUPPORT FILES
waitUntilExists("edit-field-support-file-und--2-table",function(){
  // Remove any element-specific value, falling back to stylesheets
  document.getElementById('edit-actions').style.display='block';
});
waitUntilExists("edit-field-support-file-und-table",function(){
  // Remove any element-specific value, falling back to stylesheets
  document.getElementById('edit-actions').style.display='block';
});
/*
jQuery(document).ready(function($){

if (getElementById("edit-field-upload-files-und--2-table")){
  document.getElementById('node_tei_bulk_upload_form_group_collection_and_rights').style.display='block';
  document.getElementById('edit-actions').style.display='block';
}

//document.getElementById('edit-field-upload-files-und-0-filefield-plupload-upload-button').onclick=function(){
$('*[id^="edit-field-upload-files-und-"]').onclick=function(){
waitUntilExists("edit-field-upload-files-und--2-table",function(){
  // Remove any element-specific value, falling back to stylesheets
  document.getElementById('node_tei_bulk_upload_form_group_collection_and_rights').style.display='block';
  document.getElementById('edit-actions').style.display='block';
});
};

});
*/