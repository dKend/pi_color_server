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
			if(file_exists($filepath))
			{
				$index = $this->getNextIndex();
				$ret = $this->createIndexFile($filepath, $index);
			}
			
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
			}else
			{
				$ret = $this->recycledIndicies->deQueue();
			}
			return($ret);
		}
		public function indexExists($index)
		{
			$ret = false;
			if($index >= 0)
			{
				$path = $this->constructIndexString($index, 16);
				$path = $this->outPath."/".$path.".ind";
				if(file_exists($path));
				{
					$ret = true;
				}
			}
				
			return($ret);
		}
		public function recycleIndex($index)
		{
			$ret = 0;
			if( $this->indexExists($index) ) //the index needs to be occupied for it to be able to be reused. allowing indicies that have never been used could cause some data to be lost later on when the next index variable is the same as a value in the recycled reQueue
			{
				$this->recycledIndicies->reQueue($index);
				$ret = 1;
			}
			
			return($ret);
		}
		public function createIndexFile($filepath, $index)
		{
			$ret = 0;
			
			if($this->indexExists($index)===false)
			{
				$name = $this->constructIndexString($index, 16);
				$name = $this->outPath."/".$name."ind";
				if(file_exists($this->outPath) === false)
					mkdir($this->outPath);
				file_put_contents($name, $filepath);
				$ret = 1;
			}
			
			return($ret);
		}
		public function loadIndexFile($index)
		{
			$ret = 0;
			if($this->indexExists($index))
			{
				$path = $this->constructIndexString($index, 16);
				$path = $this->outPath."/".$path.".ind";
				$ret = file_get_contents($path);
			}
			return($ret);
		}
		public function createTagFile($index, $tag)
		{
			$ret = 0;
			if($this->tagExists($tag))
			{
				$tmp = $this->constructIndexString($index, 16);
				$tmp = $this->outPath."/tags/".$tmp.".".$tag;
				
				if(file_exists($this->outPath."/tags") === false)
					mkdir($this->outPath."/tags");
				if(file_exists($tmp) === false)
					file_put_contents($tmp, "1");
				
				$ret = 1;
			}
			return($ret);
		}
		public function loadTagFile($filepath)
		{
			$ret = 0;
			if(file_exists($filepath))
				$ret = file_get_contents($filepath);
			return($ret);
		}
		public function tagExists($tag)
		{
			$ret = false;
			if($tag !== NULL)
			{
				$tmp = $this->outPath."/tags/MASTER.".$tag;
				$ret = file_exists($tmp);
			}
			return($ret);
		}
		public function createTag($string)
		{
			$ret = 0;
			if($this->tagExists($string) === false)
			{
				$tmp = $this->outPath."/tags/MASTER.".$string;
				file_put_contents($tmp);
				$ret = 1;
			}
			return($ret);
		}
		public function addTag($index, $tag)
		{
			$ret = 0;
			if($this->indexExists($index))
			{
				$ret = $this->createTagFile($index, $tag);
			}
			return($ret);
		}
		public function removeTag($index, $tag)
		{
			$ret = 0;
			if($this->indexExists($index) && $this->tagExists($tag))
			{
				if($this->indexHasTag($index, $tag))
				{
					
				}
			}
			return($ret);
		}
		public function deleteTag($tag)
		{
			$ret = 0;
			
			return($ret);
		}
		public function constructIndexString($index, $len)
		{
			$ret = 0;
			if($len > 0)
			{
				$ret = sprintf("%d", $index);
				$ret = str_pad($ret, "0", $len);
			}
			return($ret);
		}
		public function indexHasTag($index, $tag)
		{
			$ret = false;
			if($this->indexExists($index) && $this->tagExists($tag))
			{
				$ret = file_exists($this->outPath."/tags/".$this->constructIndexString($index).".".$tag);
			}
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