function replaceContentInContainer(matchClass,style) {
    var elems = document.getElementsByTagName('*'), i;
    for (i in elems) {
        if((' ' + elems[i].className + ' ').indexOf(' ' + matchClass + ' ')
                > -1) {
            elems[i].style.display = style;
        }
    }
}

document.getElementById('edit-next-directions').onclick=function(){replaceContentInContainer('group-project-directions','none');replaceContentInContainer('group-project-title-and-url','block');document.getElementById('edit-next').style.display='inline-block';};