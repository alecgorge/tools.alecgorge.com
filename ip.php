<?php

if(array_key_exists('dbug', $_GET)) {
	var_dump($_SERVER);
}

if(array_key_exists('X-Forwarded-For', $_SERVER)) {
	echo $_SERVER['X-Forwarded-For'] . "\n";
}
else {
	echo $_SERVER['REMOTE_ADDR'] . "\n";	
}
