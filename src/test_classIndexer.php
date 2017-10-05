<?php
	use PHPUnit\Framework\TestCase;
	function rrmdir($dir) 
	{
		if (is_dir($dir)) 
		{
			$objects = scandir($dir);
			foreach ($objects as $object) 
			{
				if ($object != "." && $object != "..") 
				{
					if (filetype($dir."/".$object) == "dir") 
					rrmdir($dir."/".$object); 
					else unlink   ($dir."/".$object);
				}
			}
			reset($objects);
			rmdir($dir);
		}
	}
	include("classIndexer.php");
	class TestIndexer extends TestCase
	{
		protected $indexer;
		protected $inPath = "test/data";
		protected $outPath = "test/index";
		protected function setUp()
		{
			mkdir("test");
			mkdir("test/data");
			file_put_contents("test/data/test0.data", "(TEST)");
			file_put_contents("test/data/test1.data", "(TEST)");
			file_put_contents("test/data/test2.data", "(TEST)");
			file_put_contents("test/data/test3.data", "(TEST)");
			file_put_contents("test/data/test4.data", "(TEST)");
			
			$this->indexer = New Indexer($this->inPath, $this->outPath);
		}
		protected function tearDown()
		{
			rrmdir("test");
		}
		
		/*
			@depends test_addIndex
			@depends test_getNextIndex_basic
			@depends test_createIndexFile
			@depends test_loadIndexFile
			@depends test_indexExists
		*/
		public function test_getData()
		{
			
		}
		
		/*
			@depends test_getNextIndex_basic
			@depends test_createIndexFile
		*/
		public function test_addIndex()
		{
			
		}
		
		/*
			@depends test_addIndex
		*/
		public function test_deleteIndex()
		{
			
		}
		
		/*
			@depends test_indexExists
			@depends test_createIndexFile
		*/
		public function test_recycleIndex()
		{
			
		}
		
		public function test_getNextIndex_basic()
		{
			$first = $this->indexer->getNextIndex();
			$second = $this->indexer->getNextIndex();
			$third = $this->indexer->getNextIndex();
			$this->assertTrue($first >= 0);
			$this->assertTrue($second >= 0);
			$this->assertTrue($third >= 0);
			$this->assertTrue($second > $first);
			$this->assertTrue($third > $second);
		}
		
		/*
			@depends test_loadIndexFile
			@depends test_addIndex
			@depends test_deleteIndex
		*/
		public function test_indexExists()
		{
			$this->assertTrue($this->indexer->indexExists(0) == false);
			$this->assertTrue($this->indexer->indexExists(1) == false);
			$this->assertTrue($this->indexer->indexExists(-1) == false);
			$this->indexer->createIndexFile("test/data/test0.data", $this->indexer->getNextIndex());
			$this->assertTrue($this->indexer->indexExists(0) == true);
			$this->assertTrue($this->indexer->indexExists(1) == false);
			$this->assertTrue($this->indexer->indexExists(-1) == false);
			$this->indexer->createIndexFile("test/data/test1.data", $this->indexer->getNextIndex());
			$this->assertTrue($this->indexer->indexExists(0) == true);
			$this->assertTrue($this->indexer->indexExists(1) == true);
			$this->assertTrue($this->indexer->indexExists(-1) == false);
			
		}
		
		/*
			@depends test_getNextIndex_basic
		*/
		public function test_createIndexFile()
		{
			$this->assertTrue($this->indexer->createIndexFile("test/data/test0.data", $this->indexer->getNextIndex())==1);
			$tmp = str_pad("0", 16, "0", STR_PAD_LEFT);
			$tmp = sprintf("test/index/%s.ind", $tmp);
			$this->assertTrue(file_exists($tmp));
			
			$fileContents = file_get_contents($tmp);
			$this->assertTrue($fileContents !== false);
			$this->assertTrue($fileContents === "test/data/test0.data");
			
			
		}
		
		/*
			@depends test_createIndexFile
		*/
		public function test_loadIndexFile()
		{
			$this->indexer->createIndexFile("test/data/test0.data", $this->indexer->getNextIndex());
			$fileContents = $this->indexer->loadIndexFile(0);
			$this->assertTrue($fileContents !== 0);
			$this->assertTrue($fileContents === "test/data/test0.data");
			$this->assertTrue($this->indexer->loadIndexFile(-1)===0);
			$this->assertTrue($this->indexer->loadIndexFile(1)===0);
			$this->assertTrue($this->indexer->loadIndexFile(10000000000000000)===-1);
			
		}
		public function test_createTagFile()
		{
			
		}
		public function test_loadTagFile()
		{
			
		}
		public function test_tagExists()
		{
			
		}
		public function test_tagExists()
		{
			
		}
		public function test_createTag()
		{
			
		}
		public function test_addTag()
		{
			
		}
		public function test_removeTag()
		{
			
		}
		public function test_deleteTag()
		{
			
		}
	}
	
?>