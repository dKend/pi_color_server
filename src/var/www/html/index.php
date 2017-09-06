<!DOCTYPE html>
<html>
<body>
<?php
	//not the same for every system
	// 4 is integer byte length (32-bit unsigned integer) on raspberry pi model b using raspbian OS
	$INT_BYTES = 4;
	
	function getColor(resource $sock)
	{
		socket_write($sock,pack("I", $INT_BYTES));
		$r = unpack(socket_read($sock, $INT_BYTES));
		$g = unpack(socket_read($sock, $INT_BYTES));
		$b = unpack(socket_read($sock, $INT_BYTES));
		
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
			echo "Directory Change Failed."
		}
		return(false);
	}
?>
	<form id = "rgb_sliders" method = "GET" action="handleColor.php">
		R: <input name = "red" type = "range" min = "0" max = "255"/><br>
		G: <input name = "green" type = "range" min = "0" max = "255"/><br>
		B: <input name = "blue" type = "range" min = "0" max = "255"/><br>
		<input name = "stclr" type = "submit"/>
	</form>
</body>
</html>

