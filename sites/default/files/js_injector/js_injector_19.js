function swap_search_forms(style){
	item12=window.document.getElementById('block-custom-search-blocks-1');
	item23=window.document.getElementById('block-views-exp-solr-facets-page-2');
sortblock=window.document.getElementById('block-apachesolr-search-sort');
//	button=window.document.getElementById('search-swap-button');
if(style=='faceted' && item23.style.display=="" && item12.style.display==""){
item12.style.display="inline-block";
item23.style.display="none";
if (sortblock!= null){
sortblock.style.display="block";
}
//button.innerHTML='Faceted Search';
document.getElementById('basic_radio').checked = true;
document.getElementById('facet_radio').checked = false;
}else if(style=='basic' && item23.style.display=="" && item12.style.display==""){
item23.style.display="inline-block";
item12.style.display="none";
if (sortblock!= null){
sortblock.style.display="none";
}
//button.innerHTML='Basic Search';
document.getElementById('basic_radio').checked = false;
document.getElementById('facet_radio').checked = true;
}else if(item23.style.display=="none"){
item23.style.display="inline-block";
item12.style.display="none";
if (sortblock!= null){
sortblock.style.display="none";
}
//button.innerHTML='Basic Search';
document.getElementById('basic_radio').checked = false;
document.getElementById('facet_radio').checked = true;
}else if(item12.style.display=="none"){
item12.style.display="inline-block";
item23.style.display="none";
if (sortblock!= null){
sortblock.style.display="block";
}
//button.innerHTML='Faceted Search';
document.getElementById('basic_radio').checked = true;
document.getElementById('facet_radio').checked = false;
}
}