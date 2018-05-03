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
   * @version $Revision: 5.1 $
   * $Id: class.atkplainmenu.inc,v 5.1 2004/05/31 22:11:11 ivo Exp $
   */
   
  /**
   * Implementation of the plaintext menu.
   *
   * @author Ber Dohmen <ber@ibuildings.nl>
   * @package atk
   * @subpackage menu
   */
  class atkIisgMenu extends atkmenuinterface
  {
    var $height;

    function atkIisgMenu() 
    {
      $this->height = "50";
    }

    function render()
    {
     include_once(atkconfig("menu_layout").".inc");
    }

    function getHeight()
    {
      return $this->height;
    }	

    function getPosition()
    {
      switch(atkconfig("menu_pos", "left"))
      {
        case "left": return MENU_LEFT;
        case "right": return MENU_RIGHT;
        case "top": return MENU_TOP;
        case "bottom": return MENU_BOTTOM;
      }
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