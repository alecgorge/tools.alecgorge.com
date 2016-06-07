<?php

if(array_key_exists('dbug', $_GET)) {
	var_dump($_SERVER);
}

if(array_key_exists('X-Forwarded-For', $_SERVER)) {
	$parts = explode(',', $_SERVER['X-Forwarded-For']);
	echo $parts[0] . "\n";
}
else {
	echo $_SERVER['REMOTE_ADDR'] . "\n";	
}
