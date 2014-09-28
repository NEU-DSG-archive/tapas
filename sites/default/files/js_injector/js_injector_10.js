//editablefields-form-node-2356-16535-field-formatter-field

pathArray = window.location.pathname.split( '/' );
//alert(pathArray[3]);
//alert(jQuery("#page-vid .block-inner .block-content").html());
nid=pathArray[3];
vid=jQuery("#page-vid .block-inner .block-content").html();
vid=jQuery.trim(vid);
formname="#editablefields-form-node-"+nid+"-"+vid+"-field-formatter-field";


jQuery(document).ready(function() {
//alert(formname);
    jQuery(formname).change(function() {
//alert(formname);
        var data = jQuery(this).serialize();
//alert(data);
 jQuery(formname).submit();
    })
})



window.onload=function(){
/*
setDefaults(CodeMirrorConfig, {
    stylesheet: [],
    path: "",
    parserfile: [],
    basefiles: ["util.js", "stringstream.js", "select.js", "undo.js", "editor.js", "tokenize.js"],
    iframeClass: null,
    passDelay: 200,
    passTime: 50,
    lineNumberDelay: 200,
    lineNumberTime: 50,
    continuousScanning: false,
    saveFunction: null,
    onChange: null,
    undoDepth: 50,
    undoDelay: 800,
    disableSpellcheck: true,
    textWrapping: true,
    readOnly: true,
    width: "",
    height: "300px",
    minHeight: 100,
    autoMatchParens: false,
    parserConfig: null,
    tabMode: "indent", // or "spaces", "default", "shift"
    enterMode: "indent", // or "keep", "flat"
    electricChars: true,
    reindentOnLoad: false,
    activeTokens: null,
    cursorActivity: null,
    lineNumbers: false,
    firstLineNumber: 1,
    indentUnit: 2,
    domain: null,
    noScriptCaching: false
  });

var test = new CodeMirror(CodeMirror.replace("edit-field-code-mirror-und-0-value"), {
  parserfile: ["tokenizejavascript.js", "parsejavascript.js"],
  path: "/sites/all/libraries/codemirror/lib/codemirror/js/",
  stylesheet: "/sites/all/libraries/codemirror/lib/codemirror/css/jscolors.css",
  content: document.getElementById("edit-field-code-mirror-und-0-value").value
});
*/
}
