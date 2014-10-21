window.onload = (function($)
{
rand=Math.floor(Math.random()*1E16);
$('iframe').attr('src', $('iframe').attr('src')+'?'+rand);
//alert($('iframe').attr('src'));
}(jQuery));
