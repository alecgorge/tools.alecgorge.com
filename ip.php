<?php

if(array_key_exists('dbug', $_GET)) {
	var_dump($_SERVER);
}

if(array_key_exists('HTTP_X_FORWARDED_FOR', $_SERVER)) {
	$parts = explode(',', $_SERVER['HTTP_X_FORWARDED_FOR']);
	echo $parts[0] . "\n";
}
else {
	echo $_SERVER['REMOTE_ADDR'] . "\n";	
}
