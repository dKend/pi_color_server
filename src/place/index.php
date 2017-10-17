<html>
<head>
<link rel="stylesheet" type="text/css" href="style2.css">
</head>
<body>
	<h1>Le Porn</h1>
	
	<?php
		include("../connect.php");
		function buildGlobBrace($searchParams)
		{
			$ret = "";
			$count = count($searchParams);
			$i = 0;
			while($i < $count)
			{
				$ret = sprintf("%s,%s", $ret, $searchParams[$i]);
				$i++;
			}
			$ret = sprintf("{%s}", $ret);
			return($ret);
		}
		function getFilenames($searchParams)
		{
			$ret = -1;
			if($searchParams == NULL)
			{
				return(glob($DFT_PATH."*.{jpg,png,gif,mp4}", GLOB_BRACE));
			}else
			{
				$indices = glob("tags/*.".buildGlobBrace($searchParams), GLOB_BRACE);
				$i = 0;
				$count = count($indices);
				while($i < $count)
				{
					$ret[$i] = file_get_contents($indices[$i]);
				}
			}
		}
		
		$PAGE_NUM = $_GET["page"];
		$QUERY = $_GET["search"];
		$MAX_PER_PAGE = 25;
		$DFT_PATH = "";
		echo "<div class=\"formbox\">";
		echo "<form method=\"get\" action=\"\"><input type=\"text\" name=\"search\" placeholder=\"Search...\"><input type=\"submit\"></form>";
		echo "<table class=\"layout\">";
		if($QUERY !== NULL)
		{
			
			include("classIndexer.php");
			if( $PAGE_NUM == NULL){
				$PAGE_NUM = 1;
			}
			
			$MAX_INDEX = ($PAGE_NUM * $MAX_PER_PAGE);
			$MIN_INDEX = (( $PAGE_NUM - 1 ) * $MAX_PER_PAGE)+1;
			
			
			$conn = NULL;
			$FILES = NULL;
			

			
			
			try
			{
				
				$conn = connectToDB();
				$prep1 = $conn->query("SELECT COUNT(FIL_id) FROM datafiles;");
				
				$files_count =  $prep1->fetchColumn();
				$MAX_PAGES = number_format(($files_count/$MAX_PER_PAGE) + 1, 0);
				if($MAX_INDEX > $files_count){
					$MAX_INDEX = $files_count;
				}
				
				if($files_count > 0 && $MIN_INDEX < 0)
				{
					$MIN_INDEX = 1;
				}
				$query = "SELECT * FROM datafiles WHERE FIL_id BETWEEN ".$MIN_INDEX." AND ".$MAX_INDEX.";";
				
				$prep = $conn->prepare($query);
				$prep->execute();
				
				$result = $prep->fetchAll(PDO::FETCH_ASSOC);
				
			}catch(PDOException $e)
			{
				echo "Error: " . $e->getMessage();
			}
			$conn = null;
			
			$count = 1;
			foreach($result as $v)
			{
				echo "<td><a href=\"http://$_SERVER[HTTP_HOST]/porn/view.php?FIL_id=$v[FIL_id]\"><img class = \"preview\" src=\"http://$_SERVER[HTTP_HOST]$v[FIL_path]\"></a></td>";
				if($count%5 === 0)
					echo "</tr><tr>";
				$count = $count + 1;
				
			}
			
			//navigation
			$wSize = 10;
			echo "</table>";
			echo "<div class=\"center\">";
			if($PAGE_NUM > 1)
			{
				$prevPage=$PAGE_NUM-1;
				echo "<a href=\"http://$_SERVER[HTTP_HOST]/porn/?search=$QUERY&page=$prevPage\"><<</a>";
			}
			echo "<a href=\"http://$_SERVER[HTTP_HOST]/porn/?search=$QUERY&page=1\"> First</a>";
			if($PAGE_NUM <= ($wSize/2)) 
			{
				$count = 1;
				while($count < ($wSize) && $count < $MAX_PAGES) 
				{
					if($count != $PAGE_NUM)
					{
						echo "<a href=\"http://$_SERVER[HTTP_HOST]/porn/?search=$QUERY&page=$count\"> $count</a>";
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
						echo "<a href=\"http://$_SERVER[HTTP_HOST]/porn/?search=$QUERY&page=$count\"> $count</a>";
					}else
					{
						echo " $count";
					}
					$count = $count + 1;
				}
			}
			echo "<a href=\"http://$_SERVER[HTTP_HOST]/porn/?search=$QUERY&page=$MAX_PAGES\"> Last</a>";
			
			if($PAGE_NUM < $MAX_PAGES)
			{
				$nextPage=$PAGE_NUM+1;
				echo "<a href=\"http://$_SERVER[HTTP_HOST]/porn/?search=$QUERY&page=$nextPage\"> >></a>";
			}
			echo "</div>";
		}
		
	?>
	</table>
	</div>
</body>
</html>