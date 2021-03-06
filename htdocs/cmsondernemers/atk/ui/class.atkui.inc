<?php
  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be 
   * included in the distribution.
   *
   * @package atk
   * @subpackage ui
   *
   * @copyright (c)2000-2004 Ivo Jansch
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 5.5 $
   * $Id: class.atkui.inc,v 5.5 2004/10/31 15:48:58 ivo Exp $
   */
   
  /**
   * Utility class for rendering boxes, lists, tabs or other templates. 
   *
   * @author Ivo Jansch <ivo@achievo.org>
   * @package atk
   * @subpackage ui
   */
  class atkUI
  {
    /**
     * Smarty instance, initialised by constructor
     * @access private
     * @var Smarty
     */ 
    var $m_smarty = NULL;

    /**
     * atkTheme instance, initialised by constructor
     * @access private
     * @var atkTheme
     */  
    var $m_theme = NULL;

    /**
     * Node instance
     * @access private
     * @var atkNode
     */
    var $m_node = NULL;
    
    /**
     * atkUi constructor, initialises Smarty and atkTheme instance
     */
    function atkUI()
    {
      atkimport("atk.ui.atktheme");      
      $this->m_theme = &atkTheme::getInstance();
      
      atkimport("atk.ui.atksmarty");
      $this->m_smarty = &atkSmarty::getInstance();          
            
      atkdebug("Created a new atkUI");            
    }        
    
    /**
     * Used to set a node for atkUi to perform actions on (?)
     * @param atkNode &$node an instance of an atkNode
     */
    function setNode(&$node)
    {
      $this->m_node = &$node;
    }
    
    /**
     * Renders action templates
     * Currently only the view action is implemented
     * @param String $action the action for which to render the template
     * @param array  $vars   the template variables
     * @return String the rendered template
     */
    function renderAction($action, $vars)
    {
      // todo.. action specific templates
      switch($action)
      {
        case "view":
          $tpl = "action_view.tpl";
          break;
        default:
          $tpl = "action.tpl";
      }
      return $this->render($tpl, $vars);
    }      
    
    /**
     * Renders a list template
     * @param String $action not used (deprecated?)
     * @param array  $vars   the variables with which to parse the list template
     */
    function renderList($action, $vars)
    {
      return $this->render("list.tpl", $vars);
    }

    /**
     * Renders top template
     * Deprecated, use renderBox($vars, "top")
     * @param array $vars the variables for the template
     */
    function renderTop($vars)
    {
      atkdebug("Using deprecated renderTop function, please install newer top.php from the atk/skel");
      return $this->renderBox($vars, "top");
    }
    
    /**
     * Renders a box with Smarty template. 
     * Call with a $name variable to provide a
     * better default than "box.tpl".
     *
     * For instance, calling renderBox($smartyvars, "menu")
     * will make it search for a menu.tpl first and use that
     * if it's available, otherwise it will just use box.tpl
     */
    function renderBox($vars, $name="")
    {
      if (file_exists($this->m_theme->tplPath($name.".tpl")))
      {
        return $this->render($name.".tpl", $vars);
      }
      return $this->render("box.tpl", $vars);
    }

    /**
     * Renders a tabulated template
     * Registers some scriptcode too when the tabtype is set to dhtml
     * @param array $vars the variables with which to render the template
     * @param String the rendered template
     */
    function renderTabs($vars)
    {      
      if ($this->m_theme->getAttribute("tabtype")=="dhtml")
      {
        $page = &atkPage::getInstance();
        $page->register_script(atkconfig("atkroot")."atk/javascript/dhtml_tabs.js.php");
        
        $tabscript = "var tabs = new Array();\n";
        foreach($vars["tabs"] as $tab)
        {
          $tabscript.="tabs[tabs.length] = '".$tab['tab']."';\n";
        }
        $page->register_scriptcode($tabscript);                 
      }
      return $this->render("tabs.tpl", $vars);
    }
    
    /**
     * Handler for Smarty rendering.
     * Uses smarty to render a template.
     * @param String $tplname the name of the template to render
     * @param array $vars     the variables with which to render the template
     */
    function render($tplname, $vars=array())
    { 
      // Backwardscompatibility hack.. g_layout must be initGui'd
      global $g_layout;
      if (is_object($g_layout))
      {
        $g_layout->initGui();
      }

      // First clear any existing smarty var.
      $this->m_smarty->clear_all_assign();
      
      // Then set some defaults that we need in all templates.
      $this->m_smarty->assign("themedir", $this->m_theme->themeDir());

      $this->m_smarty->assign("atkroot", atkconfig("atkroot"));

      $this->m_smarty->assign($vars);              
      
      // if $tplname contains a full pathname, the template
      // is rendered as is. 
      // If it's just a filename, we lookup the template in
      // the theme directory.
      if (strpos($tplname, "/")===false)
      {
        // lookup template in theme.
        $tplname = $this->m_theme->tplPath($tplname);    
      }
      
      // Smarty fetches templates relative from the template_dir setting.
      // Since that is an application directory, and themes reside in
      // a different directory, we have to hack the template_dir 
      // setting.
      $old = $this->m_smarty->template_dir;
      $this->m_smarty->template_dir = "./"; // current dir, becaue tplname already contains full relative path.
      $res = &$this->m_smarty->fetch($tplname);
      $this->m_smarty->template_dir = $old;
                       
      return $res;
    }        
    
    /**
     * This function returns a complete themed path for a given stylesheet.
     * This is a convenience method, which calls the stylePath method on
     * the theme instance. 
     *
     * @param String $style The filename (without path) of the stylesheet for
     *                      which you want to complete the path.
     * @return String the path of the style
     */
    function stylePath($style)
    {
      return $this->m_theme->stylePath($style);
    }
    
    /**
     * This function returns a suitable title text for an action.
     * Example: echo $g_layout->title("employee", "edit"); might return:
     *          'Edit an existing employee'
     * @param String $module   the module in which to look
     * @param String $nodetype the nodetype of the action
     * @param String $action   the action that we are trying to find a title for
     * @return String the title for the action
     */
    function title($module, $nodetype, $action)
    {
      // first look for specific text.
      $key = 'title_'.$module.'_'.$nodetype.'_'.$action;
      if (atktext($key,$module,"","","",true))
      {
        return atktext($key,$module);
      }
      else
      {
        $key = $module."_".$nodetype;
        if (atktext($key,"","","","","",true))
        {
          return atktext($key)." - ".atktext($action, "",$nodetype);
        }
        else
        {
          $key = 'title_'.$nodetype.'_'.$action;
          if (atktext($key,"","","","",true))
          {
            return atktext($key);
          }
          else
          {
            // if no specific text available for this node,
            // generate a generic title
            return atktext($nodetype, $module)." - ".atktext($action, $nodetype);
          }
        }
      }
    }   
  }
?>
