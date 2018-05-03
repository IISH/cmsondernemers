<?php
require_once "config.inc.php";

# database
$verbinding = connect2server($config_databasehost, $config_databaseuser, $config_databasepassword,"582421");
mysql_select_db($config_databasename, $verbinding) or die ("SQL-Error 4578: ". mysql_error());

// controle of rechten op huidige onderdeel
if ( check_rights($_COOKIE['atkuser']) == 0 ) {
	Header("Location: login/login.php?ref=" . get_currenturl_encoded());
}

// Setup the system
$config_atkroot = "./";
include_once("atk.inc");

atksession();

$session = &atkSessionManager::getSession();

if($ATK_VARS["atknodetype"]=="" || $session["login"]!=1) {
	// no nodetype passed, or session expired

	$page = &atknew("atk.ui.atkpagexf");
	$ui = &atknew("atk.ui.atkui");
	$theme = &atkTheme::getInstance();
	$output = &atkOutput::getInstance();

	$page->register_style($theme->stylePath("style.css"));

	$box = $ui->renderBox(array("title"=>text("title_session_expired"),
	                            "content"=>"<br><br>".text("explain_session_expired")."<br><br><br><br>"));

	$page->addContent($box);

	$output->output($page->render(text("title_session_expired"), true));
} else {
	atksecure();

	atklock();

	// Create node
	$obj = &getNode($ATK_VARS["atknodetype"]);

	if (is_object($obj)) {
	  $obj->dispatch($ATK_VARS);
	}
	else
	{
	  atkdebug("No object created!!?!");
	}
}
$output = &atkOutput::getInstance();
$output->outputFlush();
