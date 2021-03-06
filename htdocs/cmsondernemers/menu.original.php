<?php
  /**
   * Builds a navigation menu for the ATK application. 
   * Items for the main application can be added within the
   * config.menu.inc file with the menuitem() method. Modules
   * can register menuitems in their constructor. The menu
   * has support for enabling/disabling items on a user profile base.
   *
   * For more information check the atkmoduletools.inc file!
   *
   * @author Peter Verhage <peter@ibuildings.nl>
   * @version $Revision: 1.1.1.1 $
   *
   * $Id: menu.php,v 1.1.1.1 2004/04/27 22:23:43 ivo Exp $   
   * $Log: menu.php,v $
   * Revision 1.1.1.1  2004/04/27 22:23:43  ivo
   * Initial import of the ATK5 demo/showcase/crashcourse.
   *
   * Revision 4.24  2003/08/19 15:48:30  martin
   * [Ivo] Added ui instance to menu.php
   *
   * Revision 4.23  2003/08/08 08:37:27  ivo
   * + Merge of Cendris features:
   *   + Cursor navigation for editforms (by Ludo)
   *   + Fullscreen support (by Ludo)
   *   + Multi-foreign key relations
   *   + Commit/Rollback support in DB abstraction layer (by Sandy)
   *   + Better db error handling (by Sandy and me)
   * + Merge of Rockhopper features:
   *   + Smarty 2 template engine
   *   + New output and theme engines
   *   + New atkimport() and atknew() api for creating objects
   *
   * Revision 4.22  2003/02/16 09:01:42  ivo
   * The atk skel was out of date. I made it up 2 par with latest state of
   * atk.
   *
   * Revision 4.21  2002/08/22 16:22:25  ivo
   * Atk skel was exploitable by the ?config_atkroot= exploit.
   *
   * Revision 4.20  2001/11/19 14:51:52  peter
   * Fixed bug in menu.php (did not include the general menu stuff).
   * $config_menu_layout can now be anything and defaults to... "default".
   *
   */  
  $config_atkroot = "./";
  include_once("atk.inc");    

  atksession();
  atksecure();
  
  $output = &atkOutput::getInstance();
  $page = &atknew("atk.ui.atkpagexf");
  $theme = &atkTheme::getInstance();  
  $ui = &atknew("atk.ui.atkui");


  /* general menu stuff */
  include_once($config_atkroot."atk/menu/general.inc");  

  /* load menu layout */
  atkimport("atk.menu.atkmenu");
  $menu = &atkMenu::getMenu();
  
  $menu->render();

?>
