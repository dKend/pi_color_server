<?php
	exec("sudo ./home/dhk/pcs_test_4/pcs-server -d");
	header("Location: http://192.168.1.144/pcs-control");
	exit();
?>