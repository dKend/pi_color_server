<?php
	
	function generateAllIndicies($in_path, $out_path, $filetypes)
	{
		mkdir($out_path);
		$nextIndex = 0;
		if( is_dir($in_path) == true )
		{
			$dir = opendir($in_path);
			if($dir)
			{
				while(($file = readdir($dir)) !== false)
				{
					
					if(validateString($file, $filetypes))
						file_put_contents(sprintf("%s/%d.ind", $out_path, $nextIndex), $file);
					
					$nextIndex = $nextIndex + 1;
					
				}
				closedir($dir);
				file_put_contents(sprintf("%s/next.ind", $out_path), $nextIndex);
			}
			
		}
		
		return($nextIndex);
	}
	
	
	
	
	function addIndex($key, $value)
	{
		$ret = 0;
		
		return($ret);
	}
	
	function removeIndex($key)
	{
		$ret = -1;
		
		return($ret);
	}
	
	function getNextRecycledIndex()
	{
		$ret = -1;
		
		return($ret);
	}
	
	function getNextIndex()
	{
		$ret = -1;
		
		return($ret);
	}
	
	function isIndexFree($index)
	{
		$ret = false;
		
		return($ret);
	}
?>