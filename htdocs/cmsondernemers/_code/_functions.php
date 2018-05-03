<?php
// connect2server
function connect2server($server, $user, $pass, $messagecode = "") {
	$serverconnection = mysql_connect($server, $user, $pass);

	// als geen server connection, stop dan uitvoer van programma
	if ( !$serverconnection ) {
		die ('Error: ' . $messagecode . ' Cannot connect to database server.');
	}

	return $serverconnection;
}

function check_rights($usernaam) {
	global $allowedUsers;

	$retval = 0;

	$usernaam = trim($usernaam);

	//
	if ( in_array($usernaam, $allowedUsers) ) {
		$retval = 1;
	}

	return $retval;
}

function get_currenturl_encoded() {
	//
	$retval = "http://" . $_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI'];
	if ( $_SERVER['QUERY_STRING'] <> "" ) {
		$retval .= "?" . $_SERVER['QUERY_STRING'];
	}
	$retval = urlencode($retval);

	return $retval;
}
