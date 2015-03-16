if(document.getElementById("edit-next")!=null){
Image1= new Image();
Image1.src = "/sites/default/files/pictures/wait-gif.gif";
document.getElementById("edit-next").onclick = function() {
this.style.backgroundImage="url('/sites/default/files/pictures/wait-gif.gif')";

//alert(document.forms[1].id);
//this.disabled = true;
//document.forms[1].submit();
var t=setTimeout(function(){document.getElementById("edit-next").disabled = true;},1)
}
}