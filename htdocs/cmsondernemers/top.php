<?php
/*
  $config_atkroot = "./";
  include_once("atk.inc"); 

  atksession();
  atksecure();   
  
  $page = &atknew("atk.ui.atkpagexf");  
  $ui = &atknew("atk.ui.atkui");  
  $theme = &atkTheme::getInstance();
  $output = &atkOutput::getInstance();
  
  $page->register_style($theme->stylePath("style.css"));
  
  $loggedin = text("loggedinuser", "", "atk").": <b>".$g_user["name"]."</b>";  
  $content = '<br>'.$loggedin.' &nbsp; <a href="app.php?atklogout=1" target="_top">'.ucfirst(text("logout", "", "atk")).'.</a><br>&nbsp;';
  
  $box = $ui->renderBox(array("title"=>text("topframe"),
                                            "content"=>$content));
 
  $page->addContent($box);

  $output->output($page->render(atktext('app_title'), true));
  
  $output->outputFlush();
*/
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
        "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
  <title>IISG - Content Beheer Systeem</title>
  <META NAME="atkversion" CONTENT="5.0.7 (: atk_5_0_7 $)">
  <META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=iso-8859-1">
  <script type="text/javascript">
  function globalSubmit(form)
  {
     var retval = true;
    return retval;
   }
  </script>

  <link href="./atk/style.css" rel="stylesheet" type="text/css">
  <link href="./atk/themes/oneframe/styles/style.css" rel="stylesheet" type="text/css">
</head>
<body >
<!--aangepaste top.php in root directory -->
<center>
<table cellspacing=0 cellpadding=0 width="100%">
	<tr>
		<td colspan=2>

			<table bgcolor="#cccccc" width="100%" border="0" cellspacing="0" cellpadding="0">
				<tr>
					<td width="140"><nobr><img src="http://www.iisg.nl/iisg-logo-rood.gif" alt="" width="49" height="59" border="0"><a href="http://www.iisg.nl/" target="_blank"><img src="http://www.iisg.nl/iisg-gebouw.jpg" alt="" width="91" height="59" border="0"></a></nobr></td>
					<td bgcolor="#ffffff" valign="center" align=left><font size="+2">&nbsp;International Institute of Social History</font></td>
					<td bgcolor="#ffffff" valign="bottom"><br>&nbsp;<br><a href="login/logout.php">Uitloggen</a><br>&nbsp;</td>
				</tr>
				<tr>
					<td bgcolor="#000000" colspan="3" align="center">&nbsp;</td>
				</tr>
			</table>
		</td>
	</tr>
</table>
</center>
</body>
</html>
