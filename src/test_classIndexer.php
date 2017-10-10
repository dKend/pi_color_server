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
			
		*/
		public function test_getData()
		{
			$this->assertFalse($this->indexer->getData(0));
			$this->indexer->addIndex("test/data/test0.data");
			$this->assertTrue($this->indexer->getData(0)==="test/data/test0.data");
		}
		
		/*
			@depends test_getNextIndex_basic
			@depends test_createIndexFile
		*/
		public function test_addIndex()
		{
			$this->indexer->addIndex("test/data/test0.data");
			$this->assertTrue($this->indexer->getNextIndex()==1);
			$this->assertTrue(file_exists($this->outPath."/0.ind"));
			$this->assertTrue(file_get_contents($this->outPath."/0.ind")===$this->inPath."/test0.data");
		}
		
		/*
			@depends test_addIndex
			@depends test_recycleIndex
		*/
		public function test_deleteIndexFilesAreRemoved()
		{
			$this->indexer->addIndex("test/data/test0.data");
			$this->indexer->addIndex("test/data/test1.data");
			$this->indexer->deleteIndex(0);
			$this->assertFalse($this->indexer->indexExists(0));
			$this->assertTrue($this->indexer->indexExists(0));
			$tmp = str_pad("0",16,"0",STR_PAD_LEFT);
			$tmp2 = $tmp;
			$tmp = sprintf("%s.ind", $tmp);
			$this->assertFalse(file_exists($this->outPath."/".$tmp));
			$this->assertTrue(empty(glob($this->outPath."/tags/".$tmp2.".*")));
			$this->assertTrue($this->indexer->getNextIndex() === 0);
			$this->assertTrue($this->indexer->getNextIndex() === 2);
			$this->assertTrue($this->indexer->getNextIndex() === 3);
			
		}
		
		/*
			@depends test_indexExists_basic
			@depends test_addIndex
			@depends test_getNextIndex_basic
		*/
		public function test_recycleIndex()
		{
			//removes actual index and adds the index to a queue to be reused on next getNextIndex call
			//attempt to remove non existant indices
			$this->assertFalse($this->indexer->recycleIndex(-1)!==0);
			$this->assertFalse($this->indexer->recycleIndex(0)!==0);
			$this->assertFalse($this->indexer->recycleIndex(100)!==0);
			//create indices for 3 test files
			$this->indexer->addIndex("test/data/test0.data");
			$this->indexer->addIndex("test/data/test1.data");
			$this->indexer->addIndex("test/data/test2.data");
			//remove and test valid indices can be removed
			$this->assertTrue($this->indexer->recycleIndex(0)!==0);
			$this->assertTrue($this->indexer->recycleIndex(1)!==0);
			$this->assertTrue($this->indexer->recycleIndex(2)!==0);
			//check that indices were removed properly
			$this->assertFalse($this->indexer->indexExists(0)!==0);
			$this->assertFalse($this->indexer->indexExists(1)!==0);
			$this->assertFalse($this->indexer->indexExists(2)!==0);
			
			$this->assertTrue($this->indexer->getNextIndex() === 0);
			$this->assertTrue($this->indexer->getNextIndex() === 1);
			$this->assertTrue($this->indexer->getNextIndex() === 2);
			$this->assertTrue($this->indexer->getNextIndex() === 3);
			
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
			@depends test_createIndexFile
		*/
		public function test_indexExists_basic()
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
			$this->assertFalse($this->indexer->createTagFile(0, "TEST0")!==0);
			$this->assertFalse(file_exists($this->outPath."/tags/".str_pad("0",16,"0",STR_PAD_LEFT).".TEST0"));
			$this->indexer->addIndex("test/data/test0.data");
			$this->assertTrue($this->indexer->createTagFile(0, "TEST0")!==0);
			$this->assertTrue(file_exists($this->outPath."/tags/".str_pad("0",16,"0",STR_PAD_LEFT).".TEST0"));
		}
		public function test_loadTagFile()
		{
			$this->indexer->addIndex("test/data/test0.data");
			$this->indexer->createTagFile(0, "TEST0");
			$this->assertTrue($this->indexer->loadTagFile($this->outPath."/tags/".str_pad("0",16,"0",STR_PAD_LEFT).".TEST0") === "1");
		}
		
		/*
			@depends test_createTag
		*/
		public function test_tagExists()
		{
			$this->assertFalse($this->indexer->tagExists("TEST0"));
			$this->indexer->createTag("TEST0");
			$this->assertTrue($this->indexer->tagExists("TEST0"));
			
		}
		
		public function test_createTag()
		{
			$this->assertTrue($this->indexer->createTag("TEST0")===1);
			$this->assertTrue(file_exists($this->outPath."/tags/TEST0.MASTER"));
			$this->assertFalse($this->indexer->createTag("TEST0")===0);
		}
		
		/*
			@depends test_createTag
			@depends test_addIndex
		*/
		public function test_addTag()
		{
			$this->assertFalse($this->indexer->addTag(0, "TEST0")!==0);
			$this->indexer->addIndex("test/data/test0.data");
			$this->assertFalse($this->indexer->addTag(0, "TEST0")!==0);
			$this->indexer->createTag("TEST0");
			$this->assertTrue($this->indexer->addTag(0, "TEST0")===1);
		}
		
		/*
			@depends test_addTag
		*/
		public function test_removeTag()
		{
			$this->assertFalse($this->indexer->removeTag(0, "TEST0")!==0);
			$this->indexer->addIndex("test/data/test0.data");
			$this->assertFalse($this->indexer->removeTag(0, "TEST0")!==0);
			$this->indexer->createTag("TEST0");
			$this->assertFalse($this->indexer->removeTag(0, "TEST0")!==0);
			$this->indexer->addTag(0, "TEST0");
			$this->assertTrue($this->indexer->removeTag(0, "TEST0")!==0);
			$this->assertFalse($this->indexer->removeTag(0, "TEST0")!==0);
		}
		
		/*
			@depends test_tagExists
		*/
		public function test_deleteTag()
		{
			$this->assertFalse($this->indexer->deleteTag("TEST0")!==0);
			$this->indexer->createTag("TEST0");
			$this->assertTrue($this->indexer->deleteTag("TEST0")!==0);
			$this->assertFalse($this->indexer->tagExists("TEST0")!==0);
		}
	}
	
?>