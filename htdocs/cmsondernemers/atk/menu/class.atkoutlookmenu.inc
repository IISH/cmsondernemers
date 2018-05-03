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
   * $Id: class.atkoutlookmenu.inc,v 5.1 2004/05/31 22:11:11 ivo Exp $
   */
   
  /**
   * Wrapperclass for the outplook menu.
   *
   * @author Ber Dohmen <ber@ibuildings.nl>
   * @package atk
   * @subpackage menu
   */
  class atkOutlookMenu extends atkmenuinterface
  {
    function render() {
      include_once(atkconfig("menu_layout").".inc");
    }

    function getPosition() {
      return MENU_LEFT;
    }

    function getScrollable() {
      return MENU_UNSCROLLABLE;
    }
		
		function getMultilevel() {
			return MENU_NOMULTILEVEL;
		}
  }

?>