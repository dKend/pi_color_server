<! DOCTYPE html>
<html>
<head>
	<link rel="stylesheet" type="text/css" href="style.css">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<!--<meta http-equiv="refresh" content="500">-->
</head>
<body>
	<!--<textarea disabled>-->
	<?php
		exec("make -C ..", $out);
		foreach($out as $str)
			echo $str."<br>";
	?>
	<!--</textarea>-->
	<form method="GET" action="">
		<input type="submit" value="Rerun Tests"><br/>
	</form>
</body>
</html>