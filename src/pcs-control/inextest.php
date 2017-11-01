<?php
	include("index.php");
	use PHPUnit\Framework\TestCase;
	
	class PCSHandlerTest extends TestCase{
		protected $handler;
		protected $goodArgs;
		protected $badArgs;
		protected $socketInterface;
		
		protected function SetUp(){
			$this->socketInterface = new PCSSocketInterface();
			$this->goodArgs = [
				"r" => 255,
				"g" => 20,
				"b" => 150
			];
			$this->badArgs = [
				"r" => 256,
				"g" => -1,
				"b" => -256
			];
			
			//exec("pcs-server -h");
			//exec("pcs-client -setcolor 0 0 0");
		}
		
		protected function TearDown(){
			$this->handler = null;
			$this->goodArgs = null;
			$this->badArgs = null;
			
			//exec("pcs-client -halt");
		}
		
		public function test_HandleRequest_OnValidMethod(){
			$this->handler = new PCSHandler($this->goodArgs, null, null);
			
			//test behavior on valid method
			$this->assertTrue($this->handler->handleRequest("GET") !== "ERROR 303<br>");
			$this->assertTrue($this->handler->handleRequest("POST") !== "ERROR 303<br>");
		}
		
		public function test_HandleRequest_OnInvalidMethod(){
			$this->handler = new PCSHandler($this->goodArgs, null, null);
			
			//test behavior on invalid method with goodArgs
			$this->assertTrue($this->handler->handleRequest("AAAAAAAA") === "ERROR 303<br>");
			
			$this->handler = new PCSHandler($this->goodArgs, null, null);
			$this->assertTrue($this->handler->handleRequest(null) === "ERROR 303<br>");
			
		}
		
		public function test_HandleRequest_ChangesHasHandled(){
			$this->handler = new PCSHandler($this->goodArgs, null, null);
			
			
			//test behavior on valid requests
			$this->assertFalse($this->handler->getHasHandled());
			$this->handler->handleRequest("GET");
			$this->assertTrue($this->handler->getHasHandled());
			
			$this->handler = new PCSHandler($this->goodArgs, null, null);
			$this->assertFalse($this->handler->getHasHandled());
			$this->handler->handleRequest("POST");
			$this->assertTrue($this->handler->getHasHandled());
			
			//test behavior on invalid requests
			$this->handler = new PCSHandler($this->goodArgs, null, null);
			$this->assertFalse($this->handler->getHasHandled());
			$this->handler->handleRequest("AAAAAAAAAAAAA");
			$this->assertTrue($this->handler->getHasHandled());
			$this->handler->handleRequest(null);
			$this->assertTrue($this->handler->getHasHandled());
			
		}
		
		public function test_HandleRequest_CalledMultipleTimesFailsAfterFirst(){
			$this->handler = new PCSHandler($this->goodArgs, null, null);
			
			//test behavior
			$this->assertTrue($this->handler->handleRequest("GET") !== false);
			$this->assertFalse($this->handler->handleRequest("GET"));
			$this->assertFalse($this->handler->handleRequest("POST"));
			$this->assertFalse($this->handler->handleRequest("AAAAAAAAAAAA"));
			$this->assertFalse($this->handler->handleRequest(null));
		}
		
		public function test_SetHasHandled(){
			$this->handler = new PCSHandler($this->goodArgs, null, null);
			
			$this->assertFalse($this->handler->getHasHandled());
			
			//test behavior
			$this->handler->setHasHandled();
			$this->assertTrue($this->handler->getHasHandled());
			
			//test behavior after it has been called before on same object
			$this->handler->setHasHandled();
			$this->assertTrue($this->handler->getHasHandled());
		}
		
		public function test_HandleGET_WithGoodArgumentsReturnsArray(){
			$this->handler = new PCSHandler($this->goodArgs, null, null);
			
			//test behavior
			$this->assertTrue(($tmp = $this->handler->handleGET()) !== false);
			$this->assertTrue(is_array($tmp));
			$this->assertTrue($tmp[0] === "<form method='POST' action='' >");
			$this->assertTrue($tmp[1] === "R: <input type='range' min='0' max='255' value='255' name='r'><br>");
			$this->assertTrue($tmp[2] === "G: <input type='range' min='0' max='255' value='20' name='g'><br>");
			$this->assertTrue($tmp[3] === "B: <input type='range' min='0' max='255' value='150' name='b'><br>");
			$this->assertTrue($tmp[4] === "<input type='submit' value='Set Color'><br>");
			$this->assertTrue($tmp[5] === "</form>");
		}
		
		public function test_HandleGET_WithBadArguments(){
			$this->handler = new PCSHandler($this->badArgs, null, null);
			
			//test behavior 
			$this->assertTrue(($tmp = $this->handler->handleGET()) !== false);
			$this->assertTrue(is_array($tmp));
			$this->assertTrue($tmp[0] === "<form method='POST' action='' >");
			$this->assertTrue($tmp[1] === "R: <input type='range' min='0' max='255' value='0' name='r'><br>");
			$this->assertTrue($tmp[2] === "G: <input type='range' min='0' max='255' value='0' name='g'><br>");
			$this->assertTrue($tmp[3] === "B: <input type='range' min='0' max='255' value='0' name='b'><br>");
			$this->assertTrue($tmp[4] === "</form>");
		}
		
		public function test_HandlePOST_WithGoodArguments(){
			$this->handler = new PCSHandler($this->goodArgs, null, null);
			
			$this->assertTrue(($tmp = $this->handler->handlePOST()) !== false);
			$this->assertTrue($tmp === 0);
		}
		
		public function test_HandlePOST_WithBadArguments(){
			$this->handler = new PCSHandler($this->badArgs, null, null);
			$this->assertFalse($this->handler->handlePOST());
		}
		
		public function test_GetColor_ServerOffline(){
			$this->assertFalse($this->socketInterface->getColor());
		}
		
		public function test_GetColor_ServerOnline(){
			$this->assertEquals(0, exec("pcs-server -d"));
			$this->assertTrue($this->socketInterface->getColor()!==false);
		}
		
		public function test_SetColor_ServerOffline(){
			$this->assertFalse($this->socketInterface->setColor($this->goodArgs));
			$this->assertFalse($this->socketInterface->setColor($this->badArgs));
			$this->assertFalse($this->socketInterface->setColor(null));
		}
		
		public function test_SetColor_ServerOnlineWithGoodArgs(){
			$this->assertEquals(0, exec("pcs-server -d"));
			$this->assertTrue($this->socketInterface->setColor($this->goodArgs));
		}
		
		public function test_SetColor_ServerOnlineWithBadArgs(){
			$this->assertEquals(0, exec("pcs-server -d"));
			$this->assertFalse($this->socketInterface->setColor($this->badArgs));
		}
		
		public function test_GetServerStatus_ServerOffline(){
			$this->assertFalse($this->socketInterface->getServerStatus());
		}
		
		public function test_GetServerStatus_ServerOnline(){
			$this->assertEquals(0, exec("pcs-server -d"));
			$this->assertTrue($this->socketInterface->getServerStatus());
		}
		
		public function test_GetBrightness_ServerOffline(){
			$this->assertFalse($this->socketInterface->getBrightness());
		}
		
		public function test_GetBrightness_ServerOnline(){
			$this->assertEquals(0, exec("pcs-server -d"));
			$this->assertTrue($this->socketInterface->getBrightness()!==false);
		}
		
		public function test_SetBrightness_ServerOffline(){
			$this->assertFalse($this->socketInterface->setBrightness(255));
			$this->assertFalse($this->socketInterface->setBrightness(0));
			$this->assertFalse($this->socketInterface->setBrightness(-1));
			$this->assertFalse($this->socketInterface->setBrightness(-256));
		}
		
		public function test_SetBrightness_ServerOnlineWithGoodArgs(){
			$this->assertEquals(0, exec("pcs-server -d"));
			$this->assertTrue($this->socketInterface->setBrightness(255));
			$this->assertTrue($this->socketInterface->setBrightness(0));
		}
		
		public function test_SetBrightness_ServerOnlineWithBadArgs(){
			$this->assertEquals(0, exec("pcs-server -d"));
			$this->assertFalse($this->socketInterface->setBrightness(-1));
			$this->assertFalse($this->socketInterface->setBrightness(-256));
			$this->assertFalse($this->socketInterface->setBrightness(256));
			$this->assertFalse($this->socketInterface->setBrightness(null));
		}
	}
?>
