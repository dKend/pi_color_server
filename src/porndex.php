<html>
<head>
<link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>
	<table class="layout">
	<?php
		$PAGE_NUM = $_GET["page"];
		$MAX_PER_PAGE = 25;
		$QUERY = $_GET["query"];
		
		if( $PAGE_NUM == NULL){
			$PAGE_NUM = 1;
		}
		
		$MAX_INDEX = $PAGE_NUM * $MAX_PER_PAGE;
		$MIN_INDEX = ( $PAGE_NUM - 1 ) * $MAX_PER_PAGE;
		$FILES = glob("*.*");
		$files_count = count($FILES);
		$MAX_PAGES = ($files_count/$MAX_PER_PAGE) + 1;

		if($MAX_INDEX > $files_count){
			$MAX_INDEX = $files_count;
		}
		
		if($files_count > 0 && $MIN_INDEX < 0)
		{
			$MIN_INDEX = 0;
		}
		
		$index = $MIN_INDEX;
		//echo "$index <br> $MIN_INDEX <br> $MAX_INDEX";
		
		while($index < $MAX_INDEX) {
			
			$entry = $FILES[$index];
			$tmp = "http://$_SERVER[HTTP_HOST]/porn/$entry";
			echo "<tr><td><a href=\"$tmp\">$index<img class=\"preview\" src=\"$entry\" ></a></td></tr>";
			$index = $index + 1;
			
		}
		
		//navigation
		echo "<tr>";
		if($PAGE_NUM > 1)
		{
			$prevPage=$PAGE_NUM-1;
			echo "<td><a href=\"http://$_SERVER[HTTP_HOST]/porn/porndex.php?page=$prevPage\">Prev</a></td>";
		}
		echo "<td><a href=\"http://$_SERVER[HTTP_HOST]/porn/porndex.php?page=1\">First</a></td>";
		if($PAGE_NUM <= ($wSize/2)) 
		{
			$count = 1;
			while($count < ($wSize) && $count < $MAX_PAGES) 
			{
				if($count != $PAGE_NUM)
				{
					echo "<td><a href=\"http://$_SERVER[HTTP_HOST]/porn/porndex.php?page=$count\">$count</a></td>";
				}else
				{
					echo "<td>$count</td>";
				}
				$count = $count + 1;
			}
		}else if($PAGE_NUM >= ($wSize/2))
		{
			$count = $PAGE_NUM - ($wSize/2);
			while($count < ($wSize) && $count < $MAX_PAGES) 
			{
				if($count != $PAGE_NUM)
				{
					echo "<td><a href=\"http://$_SERVER[HTTP_HOST]/porn/porndex.php?page=$count\">$count</a></td>";
				}else
				{
					echo "<td>$count</td>";
				}
				$count = $count + 1;
			}
		}
		echo "<td>><a href=\"http://$_SERVER[HTTP_HOST]/porn/porndex.php?page=$MAX_PAGES\">Last</a></td>";
		echo "<td>";
		if($PAGE_NUM < $MAX_PAGES)
		{
			$nextPage=$PAGE_NUM+1;
			echo "<a href=\"http://$_SERVER[HTTP_HOST]/porn/porndex.php?page=$nextPage\">Next</a>";
		}
		echo "</td></tr>";
		
	?>
	</table>
</body>
</html>
