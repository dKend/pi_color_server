<?php
	/*
	This module needs to do a few things:
		1 - 	it needs to parse a comma separated string into individual strings ignoring whitespace
		2 - 	be able to drop any strings from the list which are invalid or a tag_to_file reference already exists for it
			between given file and the tag string
		3 -		escape strings so that they are not able to contain more sql code.
		4 -		create new tag_to_file references where valid
	On Post, this module needs to recieve:
		1 -		the source url in src_url
		2 -		the csv string in tags
		3 -		the image id in FIL_id
	*/
?>