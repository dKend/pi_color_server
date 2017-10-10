<?php
	exec("./home/dhk/pcs_test_4/pcs-client -halt");
	header("Location: http://192.168.1.144/pcs-control");
	exit();
?>