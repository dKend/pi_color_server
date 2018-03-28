<?php

$time = 0;
$timeStep = 0.01;
$wavelength = 1;
$start = 2;
$end = 255;
$amplitude = ($end-$start)/2;
$midpoint = ($start+$end)/2;
while(true){
	$offset = $midpoint/$amplitude;
	$rad = ((6.28*$time)/$wavelength)+4.72;
	$final = $amplitude*($offset+sin($rad));
	exec("pcs-client -setbright $final");
	//print("$final\n");
	sleep($timeStep);
	$time = $time+$timeStep;
}
exit(0);

?>