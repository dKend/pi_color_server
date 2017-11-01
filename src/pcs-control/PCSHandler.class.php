<?php
	//call this func on the pcs-control page to do everything
	function pageHandler(){
		$method = filter_input(INPUT_SERVER, "REQUEST_METHOD");
		if($method === "POST")
			$args = $_POST;
		else if($method === "GET")
			$args = $_GET;
		
		$handler = new PCSHandler($args, null, null);
		$str = $handler->handleRequestWrapper($method);
		
		//echo output to user
		foreach($str as $s)
			echo $s;
		
	}
	
	
	class PCSSocketInterface {
		public function getColor(){
			$ret = false;
			if($this->getServerStatus()){
				exec("pcs-client -getred", $dummy, $r);
				exec("pcs-client -getgreen", $dummy, $g);
				exec("pcs-client -getblue", $dummy, $b);
				$ret = [
					"r"=>$r,
					"g"=>$g,
					"b"=>$b
				];
			}
			return($ret);
		}
		
		public function getServerStatus(){
			return(file_exists("/tmp/pcs/colorserver"));
		}
		
		public function setColor($colorArray){
			$ret = false;
			if($this->getServerStatus()){
				if(is_array($colorArray)){
					if(isset($colorArray["r"]) && isset($colorArray["g"]) && isset($colorArray["b"])){
						$colorbad = 0;
						foreach($colorArray as $v){
							if($v > 255 || $v < 0){
								$colorbad = 1;
								break;
							}
						}
						
						if($colorbad === 0){
							exec("pcs-client -setcolor ".$colorArray["r"]." ".$colorArray["g"]." ".$colorArray["b"], $dummy, $tmp);
							if($tmp===0)
								$ret = true;
						}
					}
				}
			}
			return($ret);
		}
		
		public function getBrightness(){
			$ret = false;
			if($this->getServerStatus())
				exec("pcs-client -getbright", $dummy, $ret);
			return($ret);
		}
		public function setBrightness($value){
			$ret = false;
			if($this->getServerStatus() && $value >= 0 && $value < 256 && $value !== null){
				exec("pcs-client -setbright ".$value, $dummy, $ret);
				if($ret === 0)
					$ret = true;
			}
			return($ret);
		}
	}
	
	
	abstract class Handler {
		public abstract function handleGET();
		public abstract function handlePOST();
		public abstract function getHasHandled();
		public abstract function setHasHandled();
		public function handleRequest($method) {
			$ret = false;
			if($this->getHasHandled() === false)
			{
				if($method === "POST")
					$ret = $this->handlePOST();
				else if($method === "GET")
					$ret = $this->handleGET();
				else
					$ret = "ERROR 303<br>";
				$this->setHasHandled();
			}
			return($ret);
		}
	}
	
	class PCSHandler extends Handler {
		private $args;
		private $session;
		private $cookies;
		private $hasHandled;
		private $serverInterface;
		
		//getters
		public function getHasHandled(){
			return($this->hasHandled);
		}
		
		public function __construct($args, $session, $cookies){
			$this->args = $args;
			$this->session = $session;
			$this->cookies = $cookies;
			$this->hasHandled = false;
			$this->pcsInterface = new PCSSocketInterface();
		}
		
		//setter(s)
		public function setHasHandled(){
			$this->hasHandled = true;
		}
		
		public function handleGET(){
			$ret = false;
			
			if($this->args === null){
				$color = $this->pcsInterface->getColor();
				//check color values
				$colorbad = 0;
				foreach($color as $v){
					if($v > 255 || $v < 0){
						$colorbad = 1;
						break;
					}
				}
			}else{
				$color = $this->args;
			
				//check color values
				$colorbad = 0;
				foreach($color as $v){
					if($v > 255 || $v < 0){
						$colorbad = 1;
						break;
					}
				}
				
				//bad user-supplied arguments
				if($colorbad === 1){
					$color = [
						"r"=>0,
						"g"=>0,
						"b"=>0
					];
					$colorbad = 0;
				}
			}
			
			if($colorbad !== 1)
			{
				//construct the return array
				$ret = [
					0 => "<form method='POST' action='' >",
					1 => "R: <input type='range' min='0' max='255' value='".$color["r"]."' name='r'><br>",
					2 => "G: <input type='range' min='0' max='255' value='".$color["g"]."' name='g'><br>",
					3 => "B: <input type='range' min='0' max='255' value='".$color["b"]."' name='b'><br>",
					4 => "<input type='submit' value='Set Color'><br>",
					5 => "</form>"
				];
			}
			
			return($ret);
		}
		
		public function handlePOST(){
			$ret = false;
			
			if($this->args !== null){
				if(isset($this->args["r"]) && isset($this->args["g"]) && isset($this->args["b"])){
					$colorbad = 0;
					$color = [
						"r"=>$this->args["r"],
						"g"=>$this->args["g"],
						"b"=>$this->args["b"]
					];
					
					//check that colors are good
					foreach($color as $v){
						if($v > 255 || $v < 0){
							$colorbad = 1;
							break;
						}
					}
					
					if($colorbad !== 1){
						$this->pcsInterface->setColor($color);
						$ret = 0;
					}
					
				}
			}
			
			return($ret);
		}
		
		public function handleRequestWrapper($method){
			$ret = $this->handleRequest($method);
			if($ret === 0){
				header("Location: http://$_SERVER[HTTP_HOST]");
				die();
			}
			return($ret);
		}
	}
?>
