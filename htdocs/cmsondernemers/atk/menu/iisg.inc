<?php
  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be 
   * included in the distribution.
   *
   * @package atk
   * @subpackage menu
   * @author Peter Verhage <peter@ibuildings.nl>
   *
   * @todo Move this to the menu class that wraps this code.
   *
   * Builds a plain text navigation menu for the ATK application. 
   * Items for the main application can be added within the
   * config.menu.inc file with the menuitem() method. Modules
   * can register menuitems in their constructor. The menu
   * has support for enabling/disabling items on a user profile base.
   *
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 5.5 $
   * $Id: plain.inc,v 5.5 2004/10/26 21:32:24 boy Exp $
   */  
	
  /** @internal defines etc. */
  global $g_menu, $atkmenutop, $g_menu_parent;
	
  atkimport("atk.ui.atkpagexf");
  $page = &atkPage::getInstance();
  $ui =   &atknew("atk.ui.atkui");
  $theme = &atkTheme::getInstance();

//  $menu = "<div align='".atkconfig("menu_align", "center")."'>"; 

//  function menu_cmp($a,$b)
//  {
//    if ($a["order"] == $b["order"]) return 0;
//    return ($a["order"] < $b["order"]) ? -1 : 1;
//  }

//  if (is_array($g_menu[$atkmenutop]))
//  {
//   usort($g_menu[$atkmenutop],"menu_cmp");   
//    $menuitems = array();
//    for ($i = 0; $i < count($g_menu[$atkmenutop]); $i++)
//    {
//      if ($i==count($g_menu[$atkmenutop])-1)
//      {
//        $delimiter = "";
//      }
//      else
//      {
//        $delimiter = atkconfig("menu_delimiter");
//      }
//      $name = $g_menu[$atkmenutop][$i]["name"];
//      $menuitems[$i]["name"]=$name;
//      $url = $g_menu[$atkmenutop][$i]["url"];
//      $enable = $g_menu[$atkmenutop][$i]["enable"];

//      if (is_array($enable))
//      {
//        $enabled = false;
//        for ($j=0;$j<(count($enable)/2);$j++)
//        {
//          $enabled |= is_allowed($enable[(2*$j)],$enable[(2*$j)+1]);
//        }
//        $enable = $enabled;
//      }

//      $menuitems[$i]["enable"]=$enable;

//      /* delimiter ? */
//      if ($name == "-") $menu .= "<br>";
      
//      /* submenu ? */
//      else if (empty($url) && $enable)
//      {
//        $url = 'menu.php?atkmenutop='.$name;
//        $menu .= href($url,text("menu_$name"),SESSION_DEFAULT).$delimiter;
//      }
//      else if (empty($url) && !$enable)
//      {
//        //$menu .=text("menu_$name").$config_menu_delimiter;
//      }
//      /* normal menu item */
//      else if ($enable) $menu .= href($url,text("menu_$name"),SESSION_NEW,false,'target="main"').$delimiter;
//      else 
//      {
//        //$menu .= text("menu_$name").$config_menu_delimiter;    
//      }
//      $menuitems[$i]["url"]=session_url($url);
//    }
//  }
//  /* previous */  
//  if ($atkmenutop != "main")
//  {
//    $parent = $g_menu_parent[$atkmenutop];
//    $menu .= atkconfig("menu_delimiter");
//    $menu .= href('menu.php?atkmenutop='.$parent,text("back_to").' '.text("menu_$parent"),SESSION_DEFAULT).'<br>';  
//  }      

  $page->register_style($theme->stylePath("style.css"));
  $page->register_style($theme->stylePath("menu.css"));

//  $box = $ui->renderBox(array("title"=>text("menu_".$atkmenutop),
//                                         "content"=>$menu,
//                                         "menuitems"=>$menuitems),"menu");

	$box = $ui->renderBox(
		array(
			"title"=>text("menu_".$atkmenutop),
			"content"=>renderIISG(),
			"menuitems"=>""
		)
		 , "menu"
	);

	$page->addContent($box);

  $string = $page->render("Menu", true);
  $output = &atkOutput::getInstance();
  $output->output($string);
  $output->outputFlush();

  		function renderIISG() {
			global $g_user, $g_menu, $atkmenutop, $g_menu_parent;
			global $config_auth_accesstable;
			global $config_databasehost, $config_databasename, $config_databaseuser, $config_databasepassword;
			global $g_theme;

			include_once(atkconfig("atkroot")."atk.inc");
			include_once(atkconfig("atkroot")."atk/menu/general.inc");

			atkimport("atk.ui.atkpagexf");
			$page = &atkPage::getInstance();
			$ui = &atknew("atk.ui.atkui");
			$theme = &atkTheme::getInstance();
			$output = &atkOutput::getInstance();

			$tblmenu = "";

			while (list ($name) = each ($g_menu)) {
				if ( $tblmenu == "" ) {
					$tblmenu .= "<table width=\"100%\" border=0 cellspacing=0 cellpadding=2>\n";
				}

				$atkmenutop=$name;

				// toon niet de menuonderdeel 'main'
				if ( $atkmenutop != "main" ) {

						// ja, toon menu

						$tblmenu .= "<tr><td>";

						$tblmenu .= "<table bgcolor=\"#708090\" border=0 cellspacing=1 cellpadding=1 width=\"100%\"><tr><td align=\"center\" bgcolor=\"red\">";
						$tblmenu .= "<B><font style=\"color:#ffffff;\">" . text("menu_" . $atkmenutop) . "</font></B>";
						$tblmenu .= "</td></tr></table>";

						$tblmenu .= "</td></tr>\n";

						// doorloop alle submenus van de huidige menu
						for ($i = 0; $i < count($g_menu[$atkmenutop]); $i++)
						{

							$name = $g_menu[$atkmenutop][$i]["name"];
							$url = session_url($g_menu[$atkmenutop][$i]["url"],SESSION_NEW);

							if ( $name != "-" ) { // is het een menu separator
								// nee, het is geen menu separator

								// wat is de nodetype
								$tmpNodeType = $g_menu[$atkmenutop][$i]["url"];
								$tmpNodeType = stristr($tmpNodeType, "atknodetype="); // neem het gedeelte vanaf atknodetype= tot einde string
								$tmpNodeType = substr($tmpNodeType, strlen("atknodetype=")); // verwijder het gedeelte atknodetype=
								$pos = strpos($tmpNodeType, "&"); // zoek naar positie &
								if (!($pos === false)) { // gevonden
									$tmpNodeType = substr($tmpNodeType, 0, $pos); // neem alleen het gedeelte tot 'gevonden positie'
								}

								$tblmenu .= "<tr><td align=\"center\">";
								$tblmenu .= "<nobr><a href=\"" . $url . "\" target=\"main\">"  .  text("menu_".$name) .  "</a></nobr>";
								$tblmenu .= "</td></tr>\n";

							} else {
								// ja, het is een menu separator
								// toon lege regel
								$tblmenu .= "<tr><td>" . "&nbsp;" . "</td></tr>\n";
							}
						}
				}
			}

			if ( $tblmenu != "" ) {
				$tblmenu .= "</table>";
			}

			return $tblmenu;
		}
?>
