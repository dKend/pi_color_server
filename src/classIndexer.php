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
	
	class Indexer {
		
		private $nextIndex = 0;
		private $inPath;
		private $outPath;
		private $uploadEnabled = false;
		private $uploadPath;
		private $recycledIndicies;
		
		public function __construct($inPath, $outPath)
		{
			$this->inPath = $inPath;
			$this->outPath = $outPath;
			$this->recycledIndicies = new Queue;
		}
		public function setInPath($inPath)
		{
			$this->inPath = $inPath;
		}
		public function setOutPath($outPath)
		{
			$this->outPath = $outPath;
		}
		public function setUploadPath($uploadPath)
		{
			$this->uploadPath = $uploadPath;
		}
		public function enableUpload()
		{
			$this->uploadEnabled = true;
		}
		public function disableUpload()
		{
			$this->uploadEnabled = false;
		}
		public function isUploadEnabled()
		{
			return($this->uploadEnabled);
		}
		public function getData($index)
		{
			
		}
		public function addIndex($filepath)
		{
			$ret = 0;
			
			
			return($ret);
		}
		public function deleteIndex($index)
		{
			$ret = false;
			
			
			return($ret);
		}
		public function getNextIndex()
		{
			$ret = -1;
			if( $this->recycledIndicies->isEmpty() == true )
			{
				$ret = $this->nextIndex;
				$this->nextIndex = $this->nextIndex + 1;
			}
			return($ret);
		}
		public function indexExists($index)
		{
			$ret = false;
			
			return($ret);
		}
		private function recycleIndex($index)
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
		public function createIndexFile($filepath, $index)
		{
			$ret = 0;
			if($index>=0 && $index < $this->nextIndex)
			{
				$fname = sprintf("%d", $index);
				str_pad($fname, 16, "0", STR_PAD_LEFT);
				$name = sprintf("index/%s.ind", $fname);
				file_put_contents($name, $filepath);
				chmod($name, 0644);
				$ret = 1;
			}
			return($ret);
		}
		public function loadIndexFile($index)
		{
			$ret = 0;
			
			return($ret);
		}
		public function createTagFile($index, $tag)
		{
			$ret = 0;
			
			return($ret);
		}
		public function loadTagFile($index)
		{
			$ret = 0;
			
			return($ret);
		}
		public function tagExists($tag)
		{
			$ret = false;
			
			return($ret);
		}
		public function createTag($string)
		{
			$ret = 0;
			
			return($ret);
		}
		public function addTag($index, $tag)
		{
			$ret = 0;
			
			return($ret);
		}
		public function removeTag($index, $tag)
		{
			$ret = 0;
			
			return($ret);
		}
		public function deleteTag($tag)
		{
			$ret = 0;
			
			return($ret);
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