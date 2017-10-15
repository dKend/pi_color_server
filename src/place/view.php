<! DOCTYPE html>
<html>
<head>

</head>
<body>
	<div class="formbox">
<?php
	$username = "dhk";
	$password = "Harrison18";
	$servername = "localhost";
	$dbname = "dhkDB";
	$fileid = $_GET['FIL_id'];
	$result = "";
	try
	{
		$conn = new PDO("mysql:host=$servername;dbname=$dbname", $username, $password);
		$query = "SELECT * FROM datafiles WHERE FIL_id = $fileid;";
		
		$conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
		$prep = $conn->prepare($query);
		$prep->execute();
		
		$result = $prep->setFetchMode(PDO::FETCH_ASSOC);
	}catch(PDOException $e) 
	{
		echo "Error: " . $e->getMessage();
	}
	$conn = null;
	
?>
	</div>
</body>
</html>