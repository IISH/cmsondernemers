<?php

  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be 
   * included in the distribution.
   *
   * @package atk
   * @subpackage menu
   * @author Paul Verhoef <paul@ibuildings.nl>
   *
   * @todo Move this to the menu class that wraps this code.
   *
   * Builds an outlook-ish navigation menu for the ATK application.    
   *
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 5.3 $
   * $Id: outlook.inc,v 5.3 2004/10/08 20:21:20 sandy Exp $
   */  

  /** @internal includes and defines etc. */
      global $g_menu, $atkmenutop, $g_menu_parent;
	//$config_platformroot = "../";
	//$config_atkroot = "../";
	include_once(atkconfig("atkroot")."atk.inc");
	include_once(atkconfig("atkroot")."atk/menu/general.inc");
	
	atkimport("atk.ui.atkpagexf");
	$page = &atkPage::getInstance();
	$ui =   &atknew("atk.ui.atkui");
		
	global $g_menu;
	global $g_theme;
	
	/**
	 * @ignore
	 */
	function menu_cmp($a,$b) {
		if ($a["order"] == $b["order"]) return 0;
		return ($a["order"] < $b["order"]) ? -1 : 1;
	}
	
	//$atkmenutop = $_REQUEST["atkmenutop"];
	//var_dump($atkmenutop);
	//var_dump($g_theme);
	usort($g_menu[$atkmenutop],"menu_cmp");
	
	function menu_href($url,$name="",$sessionstatus=SESSION_DEFAULT, $extraprops="") {    
			$str = session_url($url,$sessionstatus);
			// note, we use single quotes instead of doublequotes in the href='', 
			// because we otherwise have a problem with internet explorers buggy
			// javascript parser.. 
		 //echo $str;
		 return "alert(\"".addslashes($name)."\")";
	}
	
	// you need this style or you will get an error in ns4 on first page load!
	$page->register_stylecode('
		div {
			position:absolute;
		}
	');
	
	$theme = &atkTheme::getInstance();
	atk_var_dump($theme);
	// this vars are here for theme support
	$page->register_scriptcode("
		this.ol_themename       = '".$theme->m_curtheme."';
		this.ol_pimage     = '".$theme->imgPath("dark_black.gif")."';
		this.ol_menucolor  = '".$theme->getAttribute('ol_menucolor')."';
		this.ol_wincolor   = '".$theme->getAttribute('ol_wincolor')."'; 
		this.ol_background = '".$theme->getAttribute('ol_background')."';
		this.ol_arrowup    = '".$theme->imgPath("arrowup.gif")."';
		this.ol_arrowdown  = '".$theme->imgPath("arrowdown.gif")."';
	");
	
	$page->register_script($config_atkroot."atk/javascript/crossbrowser.js");
	$page->register_script($config_atkroot."atk/javascript/outlook.js");
	
	$menucode = "
		//create OutlookBar
		var o = new createOutlookBar('Bar',0,0,screenSize.width,screenSize.height,ol_background,'white') // OutlookBar
		var p
	";
	
	while (list ($name) = each ($g_menu)) {
		$atkmenutop=$name;
		$panelbuttons="";
		
		for ($i = 0; $i < count($g_menu[$atkmenutop]); $i++) {
			$menu="";
			$name = $g_menu[$atkmenutop][$i]["name"];
			$url = session_url($g_menu[$atkmenutop][$i]["url"],SESSION_NEW);
			$enable = $g_menu[$atkmenutop][$i]["enable"];
	
			if (is_array($enable)) {
				$enabled = false;
				for ($j=0;$j<(count($enable)/2);$j++)
				{
					$enabled |= is_allowed($enable[(2*$j)],$enable[(2*$j)+1]);
				}
				$enable = $enabled;
			}
	
			/* delimiter ? */
			if ($g_menu[$atkmenutop][$i]["name"] == "-") $menu .= "<br>";
			
			/* submenu ? */
			else if (empty($url) && $enable) {
				$menu .= href('menu.php?atkmenutop='.$name,text("menu_$name"),SESSION_DEFAULT).$config_menu_delimiter;
			}
			else if (empty($url) && !$enable) {
				//$menu .=text("menu_$name").$config_menu_delimiter;
			}
				
			/* normal menu item */
			else if ($enable) 
			{            
				if($g_menu[$atkmenutop][$i]["url"]!="")
				{    
					if ($g_menu[$atkmenutop][$i]["module"]!="")
					{              
						$menu_icon = $g_modules[$g_menu[$atkmenutop][$i]["module"]].'icons/'.$atkmenutop.'_'.$name.'.gif';
					}
					else
					{					  
						$menu_icon = $theme->iconPath($atkmenutop.'_'.$name, "outlook");
					}
					if(file_exists($menu_icon))
					{
						$panelbuttons .= "p.addButton('".$menu_icon."','".addslashes(text("menu_$name"))."','parent.main.location.href = \""."".$url."\"');\n";
					}
					else
					{
						$panelbuttons .= "p.addButton('".$theme->imgPath('empty.gif')."','".addslashes(text("menu_$name"))."','parent.main.location.href = \""."".$url."\"');\n";          
					}
				}
			}
			else 
			{
				// do nothing
			}
	
		}
		//    echo "alert('$panelbuttons')";
		// When there no panelbuttons, don't show the panel
		if ($panelbuttons!="") {
			$menucode .= "p = new createPanel('".addslashes($atkmenutop)."','".addslashes(text("menu_".$atkmenutop))."');\n";  
			$menucode .= $panelbuttons;
			$menucode .= "o.addPanel(p);\n";	
		}
	}
		
	$menucode .= '
		o.draw();         //draw the OutlookBar
	 
	//-----------------------------------------------------------------------------
	//functions to manage window resize
	//-----------------------------------------------------------------------------
	//resize OP5 (test screenSize every 100ms)
	function resize_op5() {
		if (bt.op5) {
			o.showPanel(o.aktPanel);
			var s = new createPageSize();
			if ((screenSize.width!=s.width) || (screenSize.height!=s.height)) {
				screenSize=new createPageSize();
				//need setTimeout or resize on window-maximize will not work correct!
				setTimeout("o.resize(0,0,screenSize.width,screenSize.height)",100);
			}
			setTimeout("resize_op5()",100);
		}
	}
	
	//resize IE & NS (onResize event!)
	function myOnResize() {
		if (bt.ie4 || bt.ie5 || bt.ns5) {
			var s=new createPageSize();
			o.resize(0,0,s.width,s.height);
		}
		else
			if (bt.ns4) location.reload();
	}
';

$page->register_scriptcode($menucode);

// need an onResize event to redraw outlookbar after pagesize changes!
// OP5 does not support onResize event! use setTimeout every 100ms

$string = $page->render("Menu",HTML_ALL,'onLoad="resize_op5();" onResize="myOnResize();"');
$output = &atkOutput::getInstance();
$output->output($string);
$output->outputFlush();

?>