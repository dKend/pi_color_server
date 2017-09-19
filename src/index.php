<!DOCTYPE html>
<html>
<body>
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
	else
	{
		var_dump($colorData);
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
		$sock = connectToColorServer();
		if ($sock == false)
		{
			return(false);
		}
		else
		{
			socket_write($sock, pack("I", 4));
			$r = unpack(socket_read($sock, $INT_BYTES));
			$g = unpack(socket_read($sock, $INT_BYTES));
			$b = unpack(socket_read($sock, $INT_BYTES));
			socket_close($sock);
			$ret = [
				"r" => $r,
				"g" => $g,
				"b" => $b,
				
			];
			return($ret);
		}
		
		
	}
	
	function connectToColorServer()
	{
		if(chdir("/"))
		{
			echo "Directory changed Successfully.";
			$sock = socket_create(AF_UNIX, SOCK_STREAM);
			if($sock == false)
			{
				echo "Socket creation failed.";
			}else
			{
				socket_connect($sock, "colorserver", 0);
				return($sock);
			}
		}else
		{
			echo "Directory Change Failed.";
		}
		return(false);
	}
	
	function serverStatus()
	{
		//server status code
		return(false);
	}
	function getTarget()
	{
		return(false);
	}
	function isCycle()
	{
		return(false);
	}
	function startCycle()
	{
		return(false);
	}
	function stopCycle()
	{
		return(false);
	}
?>
	<form id = "rgb_sliders" method = "GET" action="handleColor.php">
		R: <input name = "red" type = "range" min = "0" max = "255"/><br>
		G: <input name = "green" type = "range" min = "0" max = "255"/><br>
		B: <input name = "blue" type = "range" min = "0" max = "255"/><br>
		<input name = "setTarget" type = "radio" defaultValue = "off"/>Set Cycle Color<br>
		<input name = "stclr" type = "submit"/><br>
	</form>
	<?php
	
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
		
	?>
</body>
</html>

