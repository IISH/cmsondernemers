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
   * @version $Revision: 5.14 $
   * $Id: class.atkedithandler.inc,v 5.14 2005/02/05 18:37:27 boy Exp $
   */
   
  /**
   * Handler class for the edit action of a node. The handler draws a
   * generic edit form for the given node.
   *
   * @author Ivo Jansch <ivo@achievo.org>
   * @author Peter C. Verhage <peter@ibuildings.nl>
   * @package atk
   * @subpackage handlers
   *
   */
  class atkEditHandler extends atkActionHandler
  {
    /**
     * The action handler method.
     */
    function action_edit()
    {
      $page = &$this->getPage();    
      
      $recordset = $this->m_node->selectDb($this->m_postvars['atkselector'],"","","","","edit");
      if ($this->m_node->hasFlag(NF_LOCK))
      {
        if ($this->m_node->m_lock->lock($this->m_node->primaryKey($recordset[0]), $this->m_node->m_table)) 
        {
          $res = $this->invoke("editPage", $recordset[0], true);
        }
        else
        {
          $res = $this->m_node->lockPage();
        }
      }  
      else
      {                
        $res = $this->invoke("editPage", $recordset[0], false);       
        echo $this->m_node->test;
      }
            
      $page->addContent($this->m_node->renderActionPage("edit", $res));
    }
    
    /**
     * This method draws a generic edit-page for a given record.
     *
     * @param array $record The record to edit.
     * @param boolean $locked Indicates whether the record is locked by the
     *                        current user.
     * @return String The rendered page as a string.
     */
    function editPage($record, $locked=FALSE)
    {        
      $node = &$this->m_node;            
      $page = &$this->getPage();
      $page->register_script(atkconfig("atkroot")."atk/javascript/formfocus.js");            
      $page->register_script(atkconfig("atkroot")."atk/javascript/dhtml_formtools.js");            
      $node->addStyle("style.css");
      
      $params = $node->getDefaultActionParams($locked);

      $params["formstart"] = '<form name="entryform" enctype="multipart/form-data" action="'.$_SERVER["PHP_SELF"].'?'.SID.'"'.
                                   ' method="post" onsubmit="return globalSubmit(this)">'.
                                   session_form();
      
      $ui = &$node->getUi();
      
      if (is_object($ui))
      {      
        $forceList = decodeKeyValueSet($node->m_postvars['atkfilter']);
        $form =$this->editForm("edit",$record,$forceList,$node->m_postvars['atksuppress']);

        $params["content"] = $node->tabulate("edit", $form);
        
        $params["buttons"] = $node->getFormButtons("edit", $record);
        
        $output = $ui->renderAction("edit", $params);
              
        return $ui->renderBox(array("title"=>$node->actionTitle('edit', $record),
                                      "content"=>$output));        
      }
      else
      {
        atkerror("Failed to create ui object");
      }      
    }   
    
    /**
     * Function returns a generic html form for editing a record.
     *
     * @param String $mode         The edit mode ("add" or "edit").
     * @param array $record        The record to edit.
     * @param array $forcelist     A key-value array used to preset certain 
     *                             fields to a certain value.
     * @param array $suppresslist  An array of fields that will be hidden.
     * @param String $fieldprefix  If set, each form element is prefixed with
     *                             the specified prefix (used in embedded
     *                             forms)
     *
     * @return String the edit form as a string
     */
    function editForm($mode="add", $record = NULL, $forceList="", $suppressList="", $fieldprefix="")
    {      
      $node = &$this->m_node;            
      
      /* get data, transform into form, return */            
      $data = $node->editArray($mode, $record, $forceList, $suppressList, $fieldprefix);                 
      // Format some things for use in tpl.
      /* check for errors and display them */
      $tab = $node->getActiveTab();
      $error_title = "";
      $pk_err_attrib = array();

      // Handle errors
      $errors = array();
      if (count($data['error']) > 0)
      {      
        $error_title = '<b>'.text('error_formdataerror').'</b>';        

        foreach ($data["error"] as $error)
        {
          if ($error['err'] == "error_primarykey_exists")
          {
            $pk_err_attrib[] = $error['attrib_name'];
          }
          else
          {
            $type = (empty($error["node"]) ? $node->m_type : $error["node"]);
            
            if (count($node->getTabs($node->m_action)) > 1
                 && $error["tab"])
              $error_tab = ' ('.text("error_tab").' '.'<a href="javascript:showTab(\''.$error["tab"].'\');">'.text("tab_".$error["tab"]).'</a> )';
            else $error_tab = "";
            
            if(!is_array($error['attrib_name']))
            {	
              $label = text($error['attrib_name'], $type);
            }
            else
            {
              $label = array();
              foreach($error['attrib_name'] as $attrib)
                $label[] = text($attrib, $type);

                $label= implode(", ", $label);
            }
                
            $errors[] = array("msg"=>$error['msg'].$error_tab, "label"=>$label);
          }
        }
        if (count($pk_err_attrib)>0) // Make primary key error message
        {                  
          for($i=0;$i<count($pk_err_attrib); $i++)
          {
            $pk_err_msg .= text($pk_err_attrib[$i]);
            if (($i+1) < count($pk_err_attrib)) $pk_err_msg .= ", ";
          }
          $errors[] = array("label"=>text("error_primarykey_exists"),
                            "msg"=>$pk_err_msg);
        }        
        
      }
      
      // Handle fields
      // load images
      atkimport("atk.ui.atktheme");
      $theme = &atkTheme::getInstance();
      $tipimg = $theme->imgPath("help.gif");
      $reqimg = '<img align="top" onMouseOver="javascript:window.status=\''.
                   addslashes(text("field_obligatory")).'\';" src="'.$theme->imgPath("required_field.gif").'" border="0" alt="'.
                   text("field_obligatory").'">';            

      /* display the edit fields */
      $fields = array();
      for ($i = 0, $_i= count($data["fields"]); $i<$_i; $i++)
      {
        $field = &$data["fields"][$i];
				
        $tplfield = array();
				        
		    $tplfield["tab"] = ($field["tabs"][0]=="*"?"alltabs":$field["tabs"][0]); // $this->getTab($field_name);
		
		    // Todo fixme: initial_on_tab kan er uit, als er gewoon bij het opstarten al 1 keer showTab aangeroepen wordt (is netter dan aparte initial_on_tab check)
		    // maar, let op, die showTab kan pas worden aangeroepen aan het begin.
		    $tplfield["initial_on_tab"] = ($tplfield["tab"]==$tab || $tplfield["tab"]=="alltabs");
								
		    // ar_ stands voor 'attribrow'.
		    $tplfield["rowid"] = "ar_".($field['id']!=''?$field['id']:getUniqueID("anonymousattribrows")); // The id of the containing row

		    /* check for seperator */
        if ($field["html"] == "-" && $i > 0 && $data["fields"][$i-1]["html"] != "-")
        {
          $tplfield["line"] = "<hr>"; 
        }
        /* double seperator, ignore */
        elseif ($field["html"] == "-")
        {
        }        
        /* only full HTML */
        elseif ($field["line"]!="")
        {
          $tplfield["line"] = $field["line"];
        }
        /* edit field */
        else
        {                           
          if ($field["attribute"]->m_ownerInstance->getNumbering())
          {
            $this->_addNumbering($field, $tplfield, $i);
          }
          
          /* does the field have a label? */
          if ($field["label"]!=="AF_NO_LABEL")
          {
            if ($field["label"] == "") 
            {
              $tplfield["label"] = "";
            }
            else
            {
              $tplfield["label"] .=  $field["label"];
              if ($field["error"]) // TODO KEES
              {
                $tplfield["error"] = $field["error"];
              }
            }            
          }
          else 
          {
          	$tplfield["label"]="AF_NO_LABEL";
          }
          
          /* html source */
          $tplfield["widget"] = $field["html"];
          $editsrc = $field["html"];

          /* obligatory indicator */
          if ($field["obligatory"]) 
          {
            $editsrc.= $reqimg."&nbsp;";
            $tplfield["obligatory"] = $reqimg;
          }

          /* tooltip */
          $ttip = atktext("txt_".$node->m_type."_".$field["name"]."_tooltip", "atk", "", "", "", true);
          
          if ($ttip)
          {
            $tooltip = '<img align="top" src="'.$tipimg.'" border="0" alt="'.$ttip.'" onClick="javascript:alert(\''.preg_replace('/([\r\n])/e',"",addslashes($ttip)).'\')" onMouseOver="javascript:window.status=\''. preg_replace('/([\r\n])/e',"",addslashes($ttip)).'\';">';
            $tplfield["tooltip"] = $tooltip;
            $editsrc.=$tooltip."&nbsp;";
          }

          $tplfield["full"] = $editsrc;
        }
        $fields[] = $tplfield; // make field available in numeric array
        $params[$field["name"]] = $tplfield; // make field available in associative array
      }
      
      $ui = &$this->getUi();
      $page = &$this->getPage();
      $page->register_script(atkconfig("atkroot")."atk/javascript/formsubmit.js");      
      foreach ($data["hide"] as $hidden)
      {
        $result.= $hidden;
      }
			
      $params["activeTab"] = $tab;
      $params["fields"] = $fields; // add all fields as an numeric array.
      $params["errortitle"] = $error_title;
      $params["errors"] = $errors; // Add the list of errors.
      
      $result.= $ui->render($node->getTemplate($mode, $record, $tab), $params);            
      
	  return $result;       
    } 
    
    /**
     * Adds numbering to the label of a field
     * @param array $field    the currently handled attribute
     * @param array $tplfield the template data for the current attribute
     * @param int $i          the counter being used to loop the node for each attribute
     */
    function _addNumbering(&$field, &$tplfield, &$i)
    {
      static $number, $subnumber;
      
      if (!$number && !$subnumber) $number = $field["attribute"]->m_ownerInstance->getNumbering();
      if (!$subnumber)
      {
        if (strlen($number) == 1 || (floor($number) <= 9 && floor($number) >= -9 && floor($number) == $number)) 
        {
          $subnumber = $number;
          $number = null;
        }
        else 
        {
          $subnumber = substr($number, strrpos($number, ".")+1);
          $number = substr($number, 0, strrpos($number, "."));
        }
      }
      
      if ($field["label"]) 
      {
        if ($number) $tplfield["label"]="$number.$subnumber. "; 
        else $tplfield["label"]="$subnumber. "; 
        $subnumber++;
      }
    }
  }
?>
