<?php

  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be 
   * included in the distribution.
   *
   * @package atk
   * @subpackage menu
   *
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 5.2 $
   * $Id: class.atkdropdownmenu.inc,v 5.2 2004/06/11 14:22:16 boy Exp $
   */
   
  /**
   * Implementation of the dhtml dropdown menu.
   *
   * @author Ber Dohmen <ber@ibuildings.nl>
   * @package atk
   * @subpackage menu
   */
  class atkDropdownMenu extends atkmenuinterface
  {
    var $height;

    function atkDropdownMenu() 
    {
      $this->height = "106";
    }

    function render()
    {
      atkimport("atk.ui.atkpagexf");
      $page = &atkPage::getInstance();
      $ui =   &atknew("atk.ui.atkui");
      $theme = &atkTheme::getInstance();
      $smarty = &atkSmarty::getInstance();
      
      $page->register_style($ui->stylePath("dhtml_dropdown.css"));
      
      $user = getUser();
      
      $tplvars = array(
      "jscode"=>atkconfig("atkroot").'atk/javascript/dhtml_dropdown_events.js',
      "name"=>$user["name"],
      "menubanner"=>$theme->imgPath("menubanner.jpg"),
      "menulogo"=>$theme->imgPath("menulogo.gif"));
      
      $dhtml_dropdown = $ui->render("dhtml_menu.tpl", $tplvars);
      $page->addContent($dhtml_dropdown);
      
      $string = $page->render("Menu", true);
      $output = &atkOutput::getInstance();
      $output->output($string);
      $output->outputFlush();
    }

    function getHeight()
    {
      return $this->height;
    }	

    function getPosition()
    {
      return MENU_TOP;
    }

    function getScrollable()
    {
      return MENU_UNSCROLLABLE;
    }

		function getMultilevel() {
			return MENU_MULTILEVEL;
		}
  }

?>