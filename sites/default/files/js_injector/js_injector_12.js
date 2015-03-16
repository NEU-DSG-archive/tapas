var form;
form = document.getElementById('views-form-og-members-admin-default');
var button;
button = document.getElementById('edit-submit');

window.onload = function () {
button.value="Done";
 form.onsubmit = function(e) {
//alert(e);
//e.preventDefault();
   //return false;
   button.value="Next >";
//this.submit();
 };
};