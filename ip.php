<?php

if(array_key_exists('X-Forwarded-For', $_SERVER)) {
	echo $_SERVER['X-Forwarded-For'] . "\n";
}
else {
	echo $_SERVER['REMOTE_ADDR'] . "\n";	
}
