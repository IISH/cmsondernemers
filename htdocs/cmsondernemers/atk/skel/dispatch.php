<?php

  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be 
   * included in the distribution.
   *
   * This file is the skeleton dispatcher file, which you can copy
   * to your application dir and modify if necessary. By default, it
   * checks the $atknodetype and $atkaction postvars and creates the
   * node and dispatches the action.
   *
   * @package atk
   * @subpackage skel
   *
   * @author Ivo Jansch <ivo@achievo.org>
   *
   * @copyright (c)2000-2004 Ivo Jansch
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 5.1 $
   * $Id: dispatch.php,v 5.1 2004/05/24 20:19:15 ivo Exp $
   */
   
  /**
   * @internal Setup the system
   */
  $config_atkroot = "./";
  include_once("atk.inc");

  atksession();
  
  $session = &atkSessionManager::getSession(); 
  
  if($ATK_VARS["atknodetype"]=="" || $session["login"]!=1)
  {
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
  }
  else
  {
    atksecure();

    atklock();

    // Create node
    $obj = &getNode($ATK_VARS["atknodetype"]);

    if (is_object($obj))
    {
      $obj->dispatch($ATK_VARS);
    }
    else
    {
      atkdebug("No object created!!?!");
    }
  }
  $output = &atkOutput::getInstance();
  $output->outputFlush();
?>
