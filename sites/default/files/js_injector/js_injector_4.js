function show_hide_collection_content(id){
	item12=window.document.getElementById('collection-'+id);
	button=window.document.getElementById('show-content-button-'+id);
	if(item12.style.display=="none" || item12.style.display==""){
		item12.style.display="block";
		button.innerHTML='<a title="hide"> Hide Content</a>';
	}else{
		item12.style.display="none";
		button.innerHTML='<a title="show">Show Content</a>';
	}
}