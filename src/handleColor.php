<?php 
	$red = $_GET["red"];
	$green = $_GET["green"];
	$blue = $_GET["blue"];
	redirect("/");
	function redirect($url, $statusCode = 303)
	{
   		header('Location: ' . $url, true, $statusCode);
   		die();
	}
?>

