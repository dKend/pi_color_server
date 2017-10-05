<?php
	$savepath = "index/Indexer.sav";
	class Node {
		private $next = NULL;
		private $data;
		
		function __construct($data)
		{
			$this->data = $data;
		}
		
		function getData()
		{
			return($this->data);
		}
		
		function getNext()
		{
			return($this->next);
		}
		
		function setNext($new)
		{
			$this->next = $new;
		}
	}
	
	class Queue {
		private $head = NULL;
		private $tail = NULL;
		
		function reQueue($data)
		{
			$new = new Node($data);
			if($this->head == NULL)
			{
				$this->head = $new;
				$this->tail = $new;
			}else
			{
				$this->tail->setNext($new);
				$this->tail=$new;
			}
		}
		
		function deQueue()
		{
			$ret = -1;
			if($this->head != NULL)
			{
				$ret = $this->head->getData();
				$tmp = $this->head->getNext();
				if($tmp == NULL) //if head is tail then next is null
				{
					$this->head = NULL;
					$this->tail = NULL;
				}else
				{
					$this->head = $tmp;
				}
			}
			return($ret);
		}
		
		function isEmpty()
		{
			$ret = false;
			if($this->head == NULL)
				$ret = true;
			return($ret);
		}
	}
	
	class FileIndexData {
		public $filename;
		public $index = -1;
		
	}
	class Indexer {
		
		private $nextIndex = 0;
		private $indexMap;
		private $inPath;
		private $outPath;
		private $recycledIndicies;
		private $hasGeneratedAllIndices = 0;
		private $filetypes;
		private $indices;
		
		function __construct($inPath, $outPath, $filetypes)
		{
			$this->inPath = $inPath;
			$this->outPath = $outPath;
			$this->filetypes = $filetypes;
			$this->recycledIndicies = new Queue;
		}
		
		function setInPath($inPath)
		{
			$this->inPath = $inPath;
		}
		
		function setOutPath($outPath)
		{
			$this->outPath = $outPath;
		}
		
		function generateAllIndicies()
		{
			if( $this->hasGeneratedAllIndices == 0)
			{
				//mkdir($this->outPath);
				if( is_dir($this->inPath) == true )
				{
					$dir = opendir($this->inPath);
					if($dir)
					{
						while(($file = readdir($dir)) !== false)
						{
							//printf("%s\n", $file);
							if(validateString($file, $this->filetypes))
								$this->addNewIndex($file);
							
						}
						closedir($dir);
					}
					
				}
				$this->hasGeneratedAllIndices = 1;
			}
			
			return($this->nextIndex);
		}
		
		function addNewIndex($filename)
		{
			$ret = 0;
			$index = $this->getNextIndex();
			$md = new FileIndexData;
			$md->filename = $filename;
			$md->index = $index;
			$this->indices[$index] = $md;
			//printf("%d\n", $index);
			
			$ret = $index;
			
			return($ret);
		}
		
		function deleteIndex($index)
		{
			$ret = 0;
			if( $this->indexExists($index) )
			{
				addIndexToRecycle($index);
			}
			
			return($ret);
		}
		
		function getNextIndex()
		{
			$ret = -1;
			if( $this->recycledIndicies->isEmpty() == true )
			{
				$ret = $this->nextIndex;
				$this->nextIndex = $this->nextIndex + 1;
			}
			return($ret);
		}
		
		function indexExists($index)
		{
			$ret = false;
			if( $this->indices[$index] == NULL )
			{
				$ret = true;
			}
			return($ret);
		}
		
		function outputIndiciesToFile()
		{
			$i = 0;
			$fileContents = "";
			while($i < $this->nextIndex)
			{
				//printf("%d\n", $i);
				//var_dump($this->indices[$i]);
				$fileContents = sprintf("%s%d\t%s\n", $fileContents, $i, $this->indices[$i]->filename);
				$i = $i + 1;
			}
			file_put_contents(sprintf("%s/debug.txt", $this->outPath), $fileContents);
		}
		
		private function addIndexToRecycle($index)
		{
			$ret = 0;
			if( indexExists($index) ) //the index needs to be occupied for it to be able to be reused. allowing indicies that have never been used could cause some data to be lost later on when the next index variable is the same as a value in the recycled reQueue
			{
				$this->recycledIndicies->reQueue($index);
				$this->indices[$index] = NULL;
				$ret = 1;
			}
			
			return($ret);
		}
		
		function createIndexFiles()
		{
			$i = 0;
			while($i<$this->nextIndex)
			{
				if($this->indices[$i] !== NULL)
				{
					$fname = sprintf("%d", $this->indices[$i]->index);
					str_pad($fname, 32, "0", STR_PAD_LEFT);
					$name = sprintf("index/%s.ind", $fname);
					file_put_contents($name, $this->indices[$i]->filename);
					chmod($name, 0644);
				}
				$i = $i + 1;
				
			}
		}
		
	}
	
	function loadData($path)
	{
		$t = file_get_contents($path);
		$ret = unserialize($t);
		return($ret);
	}
	
	function validateString($string, $patterns)
	{
		$ret = false;
		$i = 0;
		$count = count($patterns);
		//printf("%d\n", $count);
		while($i < $count)
		{
			
			if( strpos($string,$patterns[$i]) !== false )
			{
				$ret = true;
				break;
			}
			$i = $i + 1;
		}
		return($ret);
	}
?>