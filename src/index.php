<!DOCTYPE html>
<html>
<head>
<link rel="stylesheet" type="text/css" href="pcs-control/style2.css">
</head>
<body>
	<?php
		$baseLink = "http://$_SERVER[HTTP_HOST]/%s";
		$link1 = sprintf($baseLink, "porn");
		$link2 = sprintf($baseLink, "pcs-control");
	?>
	<div class = "formbox" >
		
		<form id="pornlink" action=<?php echo "\"".$link1."\""; ?> method="GET">
			<input class="topbar" type="submit" value = "Le Porn">
		</form>
		
		<form id = "pcslink" action=<?php echo "\"".$link2."\""; ?> method="GET">
			<input class="topbar" type="submit" value = "pcs-control">
		</form>
	</div>

	
</body>
</html>