<?php
	$red=$_GET["red"];
	$green=$_GET["green"];
	$blue=$_GET["blue"];
	
	chdir("/home/dhk/pcs_test_4");
	exec("./pcs-client -stclr ".$red." ".$green." ".$blue);
	
	header("Location: http://192.168.1.144/pcs-control");
	exit();
?>