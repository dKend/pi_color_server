<?php
exec("pcs-client -setbright 255");
$timeStep = 0.001;
$wavelength = 0.1;
function fade($start, $end, $time, $wlen){
	$m = 1;
	if($start > $end){
		$m = -1;
	}
	$diff = abs($start - $end);
	$ratio = $time/$wlen;
	return($start + ($m*$diff*$ratio));
}
while(true){
	$timeW = 0;
	$time0 = 0;
	$time1 = 0;
	$time2 = 0;
	
	
	while($time0<=$wavelength){
		$r = fade(0,	255, $time0, $wavelength);
		$g = fade(0,	0, $time0, $wavelength);
		$b = fade(255,	0, $time0, $wavelength);
		exec("pcs-client -setcolor $r $g $b");
		sleep($timeStep);
		$time0 = $time0+$timeStep;
	}
	
	while($timeW<=$wavelength){
		$r = fade(255,	255, $timeW, $wavelength);
		$g = fade(0,	255, $timeW, $wavelength);
		$b = fade(0,	0, $timeW, $wavelength);
		exec("pcs-client -setcolor $r $g $b");
		sleep($timeStep);
		$timeW = $timeW+$timeStep;
	}
	$timeW = 0;
	
	while($time1<=$wavelength){
		$r = fade(255,	0, $time1, $wavelength);
		$g = fade(255,	255, $time1, $wavelength);
		$b = fade(0,	0, $time1, $wavelength);
		exec("pcs-client -setcolor $r $g $b");
		sleep($timeStep);
		$time1 = $time1+$timeStep;
	}
	
	while($timeW<=$wavelength){
		$r = fade(0,	0, $timeW, $wavelength);
		$g = fade(255,	255, $timeW, $wavelength);
		$b = fade(0,	255, $timeW, $wavelength);
		exec("pcs-client -setcolor $r $g $b");
		sleep($timeStep);
		$timeW = $timeW+$timeStep;
	}
	$timeW = 0;
	
	while($time2<=$wavelength){
		$r = fade(0,	0, $time2, $wavelength);
		$g = fade(255,	0, $time2, $wavelength);
		$b = fade(255,	255, $time2, $wavelength);
		exec("pcs-client -setcolor $r $g $b");
		sleep($timeStep);
		$time2 = $time2+$timeStep;
	}
}
exit(0);

?>