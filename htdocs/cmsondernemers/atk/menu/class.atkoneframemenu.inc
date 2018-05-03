<?php

	class atkOneframeMenu extends atkmenuinterface
	{
		function render() {
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
					$tblmenu .= "<table width=\"100%\" border=0 cellspacing=0 cellpadding=4>\n";
				}

				$atkmenutop=$name;

				// toon niet de menuonderdeel 'main'
				if ( $atkmenutop != "main" ) {

						// ja, toon menu

						$tblmenu .= "<tr><th bgcolor=\"red\" nowrap>";
						$tblmenu .= "<B><small style=\"color:#ffffff;font-size:9pt\">" . text("menu_" . $atkmenutop) . "</small></B>";
						$tblmenu .= "</th></tr>\n";

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

								$tblmenu .= "<tr><td><table border=0 width=\"100%\" cellspacing=0 cellpadding=0><tr><td>";
								$tblmenu .= "<nobr><a href=\"" . $url . "\" target=\"_top\">";
								$tblmenu .= text("menu_".$name);
								$tblmenu .= "</a></nobr>";
								$tblmenu .= "</td></tr></table></td></tr>\n";

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

		function getPosition() {
			return MENU_LEFT;
		}

		function getScrollable() {
			return MENU_SCROLLABLE;
		}

		function getMultilevel() {
			return MENU_NOMULTILEVEL;
		}

	}
?>