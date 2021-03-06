<?php

  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be 
   * included in the distribution.
   *
   * @package atk
   * @subpackage handlers
   *
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @copyright (c)2000-2004 Ivo Jansch
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 5.3 $
   * $Id: class.atksearchhandler.inc,v 5.3 2004/11/15 13:59:07 boy Exp $
   */
   
  /**
   * Handler class for the search action of a node. The handler draws a
   * generic search form for the given node.
   *
   * The actual search is not performed by this handler. The search values are
   * stored in the default atksearch variables, which the admin page uses to
   * perform the actual search. The search form by default redirects to 
   * the adminpage to display searchresults.  
   *
   * @author Ivo Jansch <ivo@achievo.org>
   * @author Sandy Pleyte <sandy@achievo.org>
   * @package atk
   * @subpackage handlers
   * @todo The admin action handler is called when a search is performed. This
   *       should be customizable in the future.
   *
   */
  class atkSearchHandler extends atkActionHandler
  {
    /**
     * The action handler method. 
     */
    function action_search()
    {
      $page = &$this->getPage();
      $page->addcontent($this->m_node->renderActionPage("search", $this->invoke("searchPage", $this->m_postvars["atksearch"])));
    }
    
    /**
     * This method returns an html page that can be used as a search form.     
     * @param array $record A record containing default values that will be
     *                      entered in the searchform.
     * @return String The html search page.
     */
    function searchPage($record=NULL)
    {         
      $node = &$this->m_node;
      
      $node->addStyle("style.css");      
      
      $page = &$this->getPage();
      $page->register_script(atkconfig("atkroot")."atk/javascript/formfocus.js");      
      $ui = &$this->getUi();
      if (is_object($ui))
      {        
        $params = array();
        $params["formstart"] = '<form name="entryform" action="'.$_SERVER["PHP_SELF"].'?'.SID.'" method="post">';

        // usually, extended search is called from a page, and we go back to that page
        // when the user presses search.
        if (atkLevel()>0)
        {
          $params["formstart"].=session_form(SESSION_BACK);
        }
        // but sometimes we directly call the searchpage. if that happens,
        // we reload the adminpage with the searched items.
        else
        {
          $params["formstart"].=session_form(SESSION_DEFAULT);
          $params["formstart"].='<input type="hidden" name="atkaction" value="admin">';
        }
        $params["formstart"].='<input type="hidden" name="atknodetype" value="'.$node->atknodetype().'">';        
        $params["formstart"].='<input type="hidden" name="atkstartat" value="0">'; // start at first page after new search

        $params["content"] = $this->invoke("searchForm", $record);
        $params["buttons"][] = '<input type="submit" class="btn_search" value="'.text('search').'">';
        $params["formend"] = '</form><SCRIPT type="text/javascript" LANGUAGE="JavaScript">placeFocus()</SCRIPT>';      
      
        $output = $ui->renderAction("search", $params);
              
        $total = $ui->renderBox(array("title"=>$node->actionTitle('search'),
                                      "content"=>$output));
        
        return $total;               
      }      
      else
      {
        atkerror("ui object failure");
      }
    }
    
    /**
     * This method returns a form that the user can use to search records.
     * 
     * @param array $record A record containing default values to put into 
     *                      the search fields.
     * @return String The searchform in html form.
     */
    function searchForm($record = NULL)
    {
      $node = &$this->m_node;
      $ui = &$this->getUi();
      
      if (is_object($ui))
      {
        $node->setAttribSizes();
        
        $params = array();
        $params['searchmode_title'] = text("search_mode");
        $params['searchmode_and'] = '<input type="radio" name="atksearchmethod" value="AND" checked>'.text("search_and");
        $params['searchmode_or'] = '<input type="radio" name="atksearchmethod" value="OR">'.text("search_or");      
        $params["fields"] = array();                
  
        foreach (array_keys($node->m_attribList) as $attribname)
        {
          $p_attrib = &$node->m_attribList[$attribname];
           
          if (!$p_attrib->hasFlag(AF_HIDE_SEARCH))
            $p_attrib->addToSearchformFields($params["fields"], $node, $record);
        }
        return $ui->render($node->getTemplate("search", $record), $params);
      }    
      else atkerror("ui object error");
    }
  }
?>
