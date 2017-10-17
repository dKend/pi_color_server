<?php
	function connectToDB()
	{
		$config = parse_ini_file("conf.ini");
		$conn = new PDO("mysql:host=".$config['servername'].";dbname=".$config['dbname'], $config['username'], $config['password']);
		$conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
		return($conn);
	
	}
?>