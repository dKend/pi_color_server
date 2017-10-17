<! DOCTYPE html>
<html>
<head>
<link rel="stylesheet" type="text/css" href="style2.css">
</head>
<body>
	<div class="formbox">
		<?php
			include("../connect.php");
			$fileid = $_GET['FIL_id'];
			try
			{
				$conn = connectToDB();
				$query = "SELECT * FROM datafiles WHERE FIL_id = ".$fileid.";";
				
				$prep = $conn->prepare($query);
				$prep->execute();
				
				$result = $prep->fetch(PDO::FETCH_ASSOC);
				$next_id = $fileid + 1;
				$viewspace = sprintf("<a href=\"%s\"><img class=\"preview-large\" src=\"%s\" ></a>", "http://".$_SERVER['HTTP_HOST']."/porn/view.php?FIL_id=".$next_id, "http://".$_SERVER['HTTP_HOST'].$result['FIL_path']);
				echo $viewspace;
				if($fileid > 1)
				{
					$prev_id = $fileid - 1;
					echo "<a href=\"http://$_SERVER[HTTP_HOST]/porn/view.php?FIL_id=$prev_id\"> << Previous</a>";
				}
				echo "<a href=\"http://$_SERVER[HTTP_HOST]/porn/view.php?FIL_id=$next_id\"> Next >> </a><br>";
				echo "<a href=\"http://$_SERVER[HTTP_HOST]$result[FIL_path]\">Download</a><br>";
				
			}catch(PDOException $e)
			{
				echo "Error: " . $e->getMessage();
			}
		
			try
			{
				$prep1 = $conn->prepare("SELECT tag_name FROM tag_to_files ttf , tags t WHERE ttf.FIL_id=$fileid AND t.tag_id=ttf.tag_id;");
				$prep1->execute();
				
				$tags = $prep1->fetchAll(PDO::FETCH_ASSOC);
				foreach($tags as $v)
				{
					echo "$v[tag_name]<br>";
				}
			}
			catch(PDOException $e)
			{
				echo "Error: " . $e->getMessage();
			}
		?>
		<form id="addtags" method="POST" action="addTag.php">
			<input type="text" name="tags">
			<input type="submit">
		</form>
	</div>
</body>
</html>