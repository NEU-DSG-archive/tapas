if(document.getElementById("edit-submit")!=null){
Image1= new Image();
Image1.src = "/sites/default/files/pictures/wait-gif.gif";
document.getElementById("edit-submit").onclick = function() {
this.style.backgroundImage="url('/sites/default/files/pictures/wait-gif.gif')";
this.style.width="250px";
this.style.height="47px";
this.style.borderStyle="none";
this.style.backgroundColor="transparent";
this.style.textIndent="-1000px";
this.style.cursor="pointer";
var t=setTimeout(function(){document.getElementById("edit-submit").disabled = true;},1)
}
}