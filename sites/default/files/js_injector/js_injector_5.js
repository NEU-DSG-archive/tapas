if(window.location.href.indexOf("?")!=-1){
item12=window.document.getElementById('views-exposed-form-collection-view-panel-pane-1');
button=window.document.getElementById('show-content-button-views-exposed-form-collection-view-panel-pane-1');
if(item12!='' && button!=''){
     item12.style.display="block";
     button.innerHTML='Hide Filter';
}
filtered=1;
}else{
filtered=0;
}


function show_hide_content(id){
	item12=window.document.getElementById(id);
	button=window.document.getElementById('show-content-button-'+id);
	if(item12.style.display=="none" || item12.style.display==""){
		item12.style.display="block";
		button.innerHTML='Hide Filter';
	}else{
		if(filtered){
                    button.innerHTML='Show Engaged FIlters';
                }else{
                     button.innerHTML='Filter Results';
                }
		item12.style.display="none";
	}
}