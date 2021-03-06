<?php
  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be
   * included in the distribution.
   *
   * This file is the skeleton main frameset file, which you can copy
   * to your application dir and modify if necessary. By default, it checks
   * the settings $config_top_frame to determine how many frames to show,
   * and reads the menu config to display the proper menu.
   *
   * @package atk
   * @subpackage skel
   *
   * @author Ivo Jansch <ivo@achievo.org>
   *
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 5.3 $
   * $Id: app.php,v 5.3 2004/06/29 20:39:42 boy Exp $
   */

  /**
   * @internal includes..
   */

  $config_atkroot = "./";  
  include_once("atk.inc");
  atksession();
  atksecure();
  $output='<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Frameset//EN" "http://www.w3.org/TR/html4/frameset.dtd">';
  $output.="\n<html>\n <head>\n";
  $output.='  <META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset='.text("charset","","atk").'">';
  $output.="\n  <title>".text('app_title')."</title>\n </head>\n";
  
  atkimport("atk.menu.atkmenu");
  atkimport("atk.utils.atkframeset");
  $menu = &atkMenu::getMenu();
  
  /* @var $menu atkmenuinterface */    
  
  $position = $menu->getPosition();
  $scrolling = ($menu->getScrollable()==MENU_SCROLLABLE?FRAME_SCROLL_AUTO:FRAME_SCROLL_NO);
  
  $topframe = &new atkFrame("70", "top", "top.php", FRAME_SCROLL_NO, true);
  $mainframe = &new atkFrame("*", "main", "welcome.php", FRAME_SCROLL_AUTO, true);
  $menuframe = &new atkFrame(($position==MENU_LEFT||$position==MENU_RIGHT?190:$menu->getHeight()), "menu", "menu.php", $scrolling);
  $noframes = '<p>Your browser doesnt support frames, but this is required to run '.text('app_title')."</p>\n";
  
  $root = &new atkRootFrameset();
  if (atkconfig("top_frame"))
  {
    $outer = &new atkFrameSet("*", FRAMESET_VERTICAL, 0, $noframes);
    $outer->addChild($topframe);
    $root->addChild($outer);           
  }
  else
  {
    $outer = &$root;
    $outer->m_noframes = $noframes;
  }    
  
  $orientation = ($position==MENU_TOP||$position==MENU_BOTTOM?FRAMESET_VERTICAL:FRAMESET_HORIZONTAL);
  
  $wrapper = &new atkFrameSet("*", $orientation);  
  
  if($position==MENU_TOP||$position==MENU_LEFT)
  {    
    $wrapper->addChild($menuframe);
    $wrapper->addChild($mainframe);    
  }
  else
  {    
    $wrapper->addChild($mainframe);    
    $wrapper->addChild($menuframe);
  }      
    
  $outer->addChild($wrapper);
  
  $output.= $root->render();
  $output.= "</html>";
  echo $output;
?>
