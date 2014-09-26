<?php 
    $file = $_GET["file"];
    header('Content-Type: application/octet-stream .xml');
    header("Content-Transfer-Encoding: Binary"); 
    header("Content-disposition: attachment; filename=\"".$file."\""); 
    readfile($file);
	//REMOVED because was added to file object invalidating xml
	//echo "<a href="javascript:window.close()">CLOSE WINDOW</a>";
	exit();