<! DOCTYPE HTML>
<html>
<head>
</head>
<body>
	<?php
		if($_SERVER['REQUEST_METHOD'] == 'POST')
		{
			$tmp = sprintf( "<a href=\"%s\" download=\"%s\">Download?</a><br>",$_POST['url_target'],$_POST['url_target']);
			echo $tmp;
		}
	?>
	<form method="POST" action="download.php">
	URL: <input type="text" name="url_target"><br>
	<input type="submit" value="Download">
	</form>

</body>
</html>