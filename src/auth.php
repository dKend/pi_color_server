<! DOCTYPE html>
<html>
<head>
<link rel="stylesheet" type="text/css" href="pcs-control/style.css">
</head>
<body>
<div class="formbox">
<?php
	include("connect.php");
	include("Member.class.php");
	$target_url = $_POST['target_url'];
	$redirect_url = $_POST['source_url'];
	$member_username = $_POST['password'];
	$member_password = $_POST['username'];
	$cookie_name = "login_credentials";
	if(!isset($_COOKIE[$cookie_name]))
	{
		if( $target_url !== null || $redirect_url !== null || $member_username !== null || $member_password !== null )
		{
			$member = new Member($member_username, $member_password);
			$isAuthed = false;
			try 
			{
				$connection = connectToDB();
				$isAuthed = $member->authLogin($connection);
			}
			catch(PDOException $e)
			{
				echo "ERROR: ".$e->getMessage();
			}
			
			$connection = null;
			if( $isAuthed === true )
			{
				setcookie($cookie_name, time() + 3600, "/");
				echo "<meta http-equiv=\"refresh\" content=\"3;$target_url\" />";
				echo "<h2 class=\"redirectMessage\"> Login Successful!<br> Redirecting in 3 seconds...</h2>";
			}else
			{
				header($redirect_url);
				exit(0);
			}

		}else
		{
			header($redirect_url);
			exit(0);
		}
	}else
	{
		$memb = unserialize($_COOKIE[$cookie_name]);
		if( $memb->getIsValidated() === true )
		{
			header($target_url);
			exit(0);
		}else
		{
			header($redirect_url);
			exit(0);
		}
	}
?>
</div>
</body>
</html>
