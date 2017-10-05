<?php
	include("classIndexer.php");
	$ftypes[0] = ".jpg";
	$ftypes[1] = ".gif";
	$ftypes[2] = ".png";
	$i = New Indexer("porn", "index", $ftypes);
	$i->generateAllIndicies();
	$i->createIndexFiles();
	echo "Done.";
?>