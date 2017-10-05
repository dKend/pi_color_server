<html>
<head>
<link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>
	<h1>Le Porn</h1>
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
		$FILES = glob("porn/*.{jpg,png,gif}", GLOB_BRACE);
		$files_count = count($FILES);
		$MAX_PAGES = number_format(($files_count/$MAX_PER_PAGE) + 1, 0);

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
			$count1 = 0;
			$count1Max=5;
			echo "<tr>";
			while($count1 < $count1Max && $index < $MAX_INDEX)
			{
				$entry = $FILES[$index];
				$tmp = "http://$_SERVER[HTTP_HOST]/$entry";
				echo "<td><a href=\"$tmp\">$index<img class=\"preview\" src=\"$entry\" ></a></td>";
				$count1 = $count1 + 1;
				$index = $index + 1;
			}
			echo "</tr>";
			
			
		}
		
		//navigation
		$wSize = 10;
		echo "</table>";
		echo "<div class=\"center\">";
		if($PAGE_NUM > 1)
		{
			$prevPage=$PAGE_NUM-1;
			echo "<a href=\"http://$_SERVER[HTTP_HOST]/porndex.php?page=$prevPage\"><<</a>";
		}
		echo "<a href=\"http://$_SERVER[HTTP_HOST]/porndex.php?page=1\"> First</a>";
		if($PAGE_NUM <= ($wSize/2)) 
		{
			$count = 1;
			while($count < ($wSize) && $count < $MAX_PAGES) 
			{
				if($count != $PAGE_NUM)
				{
					echo "<a href=\"http://$_SERVER[HTTP_HOST]/porndex.php?page=$count\"> $count</a>";
				}else
				{
					echo " $count";
				}
				$count = $count + 1;
			}
		}else if($PAGE_NUM >= ($wSize/2))
		{
			$count = $PAGE_NUM - ($wSize/2);
			$tmp = $PAGE_NUM + ($wSize/2);
			while($count < ($tmp) && $count < $MAX_PAGES) 
			{
				if($count != $PAGE_NUM)
				{
					echo "<a href=\"http://$_SERVER[HTTP_HOST]/porndex.php?page=$count\"> $count</a>";
				}else
				{
					echo " $count";
				}
				$count = $count + 1;
			}
		}
		echo "<a href=\"http://$_SERVER[HTTP_HOST]/porndex.php?page=$MAX_PAGES\"> Last</a>";
		
		if($PAGE_NUM < $MAX_PAGES)
		{
			$nextPage=$PAGE_NUM+1;
			echo "<a href=\"http://$_SERVER[HTTP_HOST]/porndex.php?page=$nextPage\"> >></a>";
		}
		echo "</div>";
		
	?>
	
</body>
</html>
