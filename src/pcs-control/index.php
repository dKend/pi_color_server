<!DOCTYPE html>
<html>
<head>
<link rel="stylesheet" type="text/css" href="style2.css">
<meta charset="UTF-8">
</head>
<body>
<div id="formbox">
<a href="http://192.168.1.144/porn/" >Le Porn</a>
<?php
	//not the same for every system
	// 4 is integer byte length (32-bit unsigned integer) on raspberry pi model b using raspbian OS
	$INT_BYTES = 4;
	
	//strings
	
	
	//do debug stuff
	$colorData = getColor();
	
	
	//do server status/control stuff
	$status = serverStatus();
	if($status == false)
	{
		echo "<form id = \"pcs_control_panel\" method = \"GET\" action=\"pcs-start.php\">";
		echo "Server Offline<br>";
		echo "<input type = \"submit\" id = \"START\" value = \"Start Server\"><br>";
	}
	else
	{
		echo "<form id = \"pcs_control_panel\" method = \"GET\" action=\"pcs-stop.php\">";
		echo "Server Online<br>";
		echo "<input type = \"submit\" id = \"STOP\" value = \"Stop Server\"></br>";
	}
	echo "</form>";
	
	
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
	
	function serverStatus()
	{
		$ret = false;
		if(file_exists("/colorserver"))
			$ret=true;
		return($ret);
	}
	function println($string)
	{
		echo $string . "<br>";
	}
	
?>
	
		<form id = "rgb_sliders" method = "GET" action="handleColor.php">
			R: <input name = "red" type = "range" min = "0" max = "255" <?php echo "value = \"".$colorData["r"]."\""; ?>/><br>
			G: <input name = "green" type = "range" min = "0" max = "255" <?php echo "value = \"".$colorData["g"]."\""; ?>/><br>
			B: <input name = "blue" type = "range" min = "0" max = "255" <?php echo "value = \"".$colorData["b"]."\""; ?>/><br>
			<input name = "setTarget" type = "radio" defaultValue = "off"/>Set Cycle Color<br>
			<input name = "stclr" type = "submit"/><br>
		</form>
	</div>
</body>
</html>

