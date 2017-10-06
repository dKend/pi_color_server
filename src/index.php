<!DOCTYPE html>
<html>
<head>
<link rel="stylesheet" type="text/css" href="style.css">
<meta charset="UTF-8">
</head>
<body>
<a href="http://192.168.1.144/browse.php" >Le Porn</a>
<?php
	//not the same for every system
	// 4 is integer byte length (32-bit unsigned integer) on raspberry pi model b using raspbian OS
	$INT_BYTES = 4;
	
	//strings
	$TABLE_OPEN = "<table class=\"form_table\">";
	$DEBUG_STRING = "<div class = \"debug_area\">";
	$PCS_CONTROL_STRING = "<form id = \"pcs_control_panel\" method = \"GET\" action=\"pcsControl.php\">";
	
	//do debug stuff
	$colorData = getColor();
	echo $DEBUG_STRING;
	if($colorData == false)
	{
		echo "Failed to load color data from pcs.<br>";
	}
	echo "</div>";
	
	//do server status/control stuff
	echo  $PCS_CONTROL_STRING.$TABLE_OPEN;
	$status = serverStatus();
	if($status == false)
	{
		echo "<tr><td>Server Offline</td></tr>";
		echo "<tr><td><input type = \"submit\" id = \"START\" value = \"Start Server\"></td></tr>";
	}
	else
	{
		echo "<tr><td>Server Online</td></tr>";
		echo "<tr><td><input type = \"submit\" id = \"STOP\" value = \"Stop Server\"></td></tr>";
	}
	echo "</table></form>";
	
	
	function getColor()
	{
		$rv = 0;
		$dummy = "";
		chdir("/home/dhk/pcs_test_4");
		
		$r = 0;
		$g = 0;
		$b = 0;
		
		exec("./pcs-client -getred", $dummy, $r);
		exec("./pcs-client -getgreen", $dummy, $g);
		exec("./pcs-client -getblue", $dummy, $b);
		
		
		$ret = [
			"r" => $r,
			"g" => $g,
			"b" => $b,
			
		];
		
		return($ret);
	}
	
	function connectToColorServer()
	{
		
		$timeOut = 10;
		$sock = stream_socket_client("unix:///home/dhk/pcs_test_4/colorserver", $errorno,$errorstr,$timeOut);
		stream_set_timeout($sock, $timeOut);
		return($sock);
	}
	
	function serverStatus()
	{
		$ret = false;
		if(file_exists("/home/dhk/pcs_test_4/colorserver"))
			$ret=true;
		return($ret);
	}
	function println($string)
	{
		echo $string . "<br>";
	}
	
?>
	<form id = "rgb_sliders" method = "GET" action="handleColor.php">
		R: <input name = "red" type = "range" min = "0" max = "255"/><br>
		G: <input name = "green" type = "range" min = "0" max = "255"/><br>
		B: <input name = "blue" type = "range" min = "0" max = "255"/><br>
		<input name = "setTarget" type = "radio" defaultValue = "off"/>Set Cycle Color<br>
		<input name = "stclr" type = "submit"/><br>
	</form>
</body>
</html>

