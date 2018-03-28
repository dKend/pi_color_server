<?php
$timeStep = 0.01;
$wavelength = 1;
$start = 2;
$end = 255;
$amplitude = ($end-$start)/2;
$midpoint = ($start+$end)/2;
while(true){
	$timeR = 0;
	$timeO = 0;
	$timeY = 0;
	$timeG = 0;
	$timeB = 0;
	$timeI = 0;
	$timeV = 0;
	
	exec("pcs-client -setcolor 255 0 0");
	while($timeR<=$wavelength){
		$offset = $midpoint/$amplitude;
		$rad = ((6.28*$timeR)/$wavelength)+4.72;
		$final = $amplitude*($offset+sin($rad));
		exec("pcs-client -setbright $final");
		//print("$final\n");
		sleep($timeStep);
		$timeR = $timeR+$timeStep;
	}
	
	exec("pcs-client -setcolor 255 100 0");
	while($timeO<=$wavelength){
		$offset = $midpoint/$amplitude;
		$rad = ((6.28*$timeO)/$wavelength)+4.72;
		$final = $amplitude*($offset+sin($rad));
		exec("pcs-client -setbright $final");
		//print("$final\n");
		sleep($timeStep);
		$timeO = $timeO+$timeStep;
	}
	
	exec("pcs-client -setcolor 255 255 0");
	while($timeY<=$wavelength){
		$offset = $midpoint/$amplitude;
		$rad = ((6.28*$timeY)/$wavelength)+4.72;
		$final = $amplitude*($offset+sin($rad));
		exec("pcs-client -setbright $final");
		//print("$final\n");
		sleep($timeStep);
		$timeY = $timeY+$timeStep;
	}
	
	exec("pcs-client -setcolor 0 255 0");
	while($timeG<=$wavelength){
		$offset = $midpoint/$amplitude;
		$rad = ((6.28*$timeG)/$wavelength)+4.72;
		$final = $amplitude*($offset+sin($rad));
		exec("pcs-client -setbright $final");
		//print("$final\n");
		sleep($timeStep);
		$timeG = $timeG+$timeStep;
	}
	
	exec("pcs-client -setcolor 0 0 255");
	while($timeB<=$wavelength){
		$offset = $midpoint/$amplitude;
		$rad = ((6.28*$timeB)/$wavelength)+4.72;
		$final = $amplitude*($offset+sin($rad));
		exec("pcs-client -setbright $final");
		//print("$final\n");
		sleep($timeStep);
		$timeB = $timeB+$timeStep;
	}
	
	exec("pcs-client -setcolor 80 0 255");
	while($timeI<=$wavelength){
		$offset = $midpoint/$amplitude;
		$rad = ((6.28*$timeI)/$wavelength)+4.72;
		$final = $amplitude*($offset+sin($rad));
		exec("pcs-client -setbright $final");
		//print("$final\n");
		sleep($timeStep);
		$timeI = $timeI+$timeStep;
	}
	
	exec("pcs-client -setcolor 255 0 255");
	while($timeV<=$wavelength){
		$offset = $midpoint/$amplitude;
		$rad = ((6.28*$timeV)/$wavelength)+4.72;
		$final = $amplitude*($offset+sin($rad));
		exec("pcs-client -setbright $final");
		//print("$final\n");
		sleep($timeStep);
		$timeV = $timeV+$timeStep;
	}
	
	
}
exit(0);

?>