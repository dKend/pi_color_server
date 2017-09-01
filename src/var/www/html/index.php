<!DOCTYPE html>
<html>
<body>
<?php echo "hello world"; ?>
	<form id = "rgb_sliders" method = "GET" action="handleColor.php">
		R: <input name = "red" type = "range" min = "0" max = "255"/><br>
		G: <input name = "green" type = "range" min = "0" max = "255"/><br>
		B: <input name = "blue" type = "range" min = "0" max = "255"/><br>
		<input type = "submit"/>
	</form>
</body>
</html>

