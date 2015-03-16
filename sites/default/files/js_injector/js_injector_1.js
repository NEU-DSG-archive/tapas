window.onload = function () {

 var first = document.getElementById('edit-field-title-und-0-value');

var element =  document.getElementById('edit-field-collection-url-und-0-value');
if (typeof(element) != 'undefined' && element != null)
{
  var second = document.getElementById('edit-field-collection-url-und-0-value');
  var strlen = 128;
}
var element1 =  document.getElementById('edit-field-url-und-0-value');
if (typeof(element1) != 'undefined' && element1 != null)
{
  var second = document.getElementById('edit-field-url-und-0-value');
  var strlen = 40;
}

  first.onkeyup = function () { // or first.onchange
    second.value = first.value.toLowerCase().replace(/[^a-z0-9-]/g, '').replace(/[^a-z]+/, "").substring(0,strlen);
  };
//var checkbox = document.getElementById('edit-field-email-yet-und');
//checkbox.checked="checked";

};