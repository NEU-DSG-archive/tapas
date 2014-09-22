<?php 
    header('Content-Type: text/plain');
	$filecontents = file_get_contents($_GET["file"]);
	print $filecontents;



