<! DOCTYPE html >
<html>
<head>
</head>
<body>
	<?php
		abstract class Handler {
			public abstract function handleGET($get, $session, $cookies);
			public abstract function handlePOST($post, $session, $cookies);
			public handleRequest($method) {
				
			}
		}
		
		class PCSHandler implements Handler {
			public function handleGET($get, $session, $cookies){
				
			}
			
			public function handlerPOST($post, $session, $cookies){
				
			}
		}
	?>
</body>
</html>