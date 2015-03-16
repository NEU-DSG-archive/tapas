var form; 
form = document.getElementById('group-node-form');
var button;
button = document.getElementById('edit-next');

window.onload = function () {
button.value="Go to collection >";
 form.onsubmit = function(e) {
   button.value="Next >";
 };
};