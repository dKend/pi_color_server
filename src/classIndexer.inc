<?php
	class Node {
		private $next = NULL;
		private $data;
		
		function __construct($data)
		{
			$this->$data = $data;
		}
		
		function getData()
		{
			return($this->$data);
		}
		
		function getNext()
		{
			return($this->$next);
		}
		
		function setNext($new)
		{
			$this->$next = $new;
		}
	}
	
	class Queue {
		private $head = NULL;
		private $tail = NULL;
		
		function queue($data)
		{
			$new = New Node($data);
			if($this->$head == NULL)
			{
				$this->$head = $new;
				$this->$tail = $new;
			}else
			{
				$this->$tail->setNext($new);
				$this->$tail=$new;
			}
		}
		
		function deQueue()
		{
			$ret = -1;
			if($this->$head != NULL)
			{
				$ret = $this->$head->getData();
				$tmp = $this->$head->getNext();
				if($tmp == NULL) //if head is tail then next is null
				{
					$this->$head = NULL;
					$this->$tail = NULL;
				}else
				{
					$this->$head = $tmp;
				}
			}
			return($ret);
		}
		
		function isEmpty()
		{
			$ret = false;
			if($this->$head == NULL)
				$ret = true;
			return($ret);
		}
	}
	
	class FileMetaData {
		public $filename;
		public $tags;
		public $index = -1;
		
	}
	class Indexer {
		private $nextIndex = 0;
		private $indexMap;
		private $inPath;
		private $outPath;
		private $recycledIndicies= New Queue;
		private $hasGeneratedAllIndices = 0;
		private $filetypes;
		private $indices;
		
		function __construct($inPath, $outPath, $filetypes)
		{
			$this->$inPath = $inPath;
			$this->$outPath = $outPath;
		}
		
		function setInPath($inPath)
		{
			$this->$inPath = $inPath;
		}
		
		function setOutPath($outPath)
		{
			$this->$outPath = $outPath;
		}
		
		function generateAllIndicies()
		{
			if( $this->$hasGeneratedAllIndices == 0)
			{
				//mkdir($this->$outPath);
				if( is_dir($this->$inPath) == true )
				{
					$dir = opendir($this->$inPath);
					if($dir)
					{
						while(($file = readdir($dir)) !== false)
						{
							
							if(validateString($file, $this->$filetypes))
								$this->addNewIndex($file);
							
							$this->$nextIndex = $this->$nextIndex + 1;
							
						}
						closedir($dir);
					}
					
				}
				$this->$hasGeneratedAllIndices = 1;
			}
			
			return($this->$nextIndex);
		}
		
		function addNewIndex($filename)
		{
			$ret = 0;
			$index = getNextIndex();
			$md = New FileMetaData;
			$md->$filename = $filename;
			$md->$index = $index;
			$this->$indices[$index] = $md;
			
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
			if( $this->$recycledIndicies->isEmpty() == true )
			{
				$ret = $this->$nextIndex
				$nextIndex = $nextIndex + 1;
			}
			
			return($ret);
		}
		
		function indexExists($index)
		{
			$ret = false;
			if( $this->$indices[$index] == NULL )
			{
				$ret = true;
			}
			return($ret);
		}
		
		private function addIndexToRecycle($index)
		{
			$ret = 0;
			if( indexExists($index) ) //the index needs to be occupied for it to be able to be reused. allowing indicies that have never been used could cause some data to be lost later on when the next index variable is the same as a value in the recycled queue
			{
				$this->$recycledIndicies->queue($index);
				$this->$indices[$index] = NULL;
				$ret = 1;
			}
			
			return($ret);
		}
	}
	
	function validateString($string, $patterns)
	{
		$ret = false;
		$i = 0;
		$count = count($patterns);
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