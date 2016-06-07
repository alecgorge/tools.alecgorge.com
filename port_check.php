<?php

$host = $_GET['host'];
$port = intval($_GET['port']);

if(empty($host) || $port < 1 || $port > 65535) {
	header('HTTP/1.1 503 Service Unavailable');
	echo 0;
	exit();
}

$errno = 0;
$errmsg = "";
$connection = @fsockopen($host, $port, $errno, $errmsg, 3);

if (is_resource($connection)) {
	echo 1;
	fclose($connection);

	exit();
}

header('HTTP/1.1 503 Service Unavailable');
echo 0;
