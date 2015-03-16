var form;
form = document.getElementById('tei-content-node-form');
var button;
button = document.getElementById('edit-next');

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