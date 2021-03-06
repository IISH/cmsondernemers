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
   * @version $Revision: 5.2 $
   * $Id: class.atksavehandler.inc,v 5.2 2004/05/19 09:17:14 lineke Exp $
   */
   
  /**
   * Handler class for the save action of a node. The action saves a 
   * new record to the database. The data is retrieved from the postvars.
   * This is the action that follows an 'add' action. The 'add' action
   * draws the add form, the 'save' action saves the data to the database.
   * Validation of the record is performed before storage. If validation
   * fails, the add handler is invoked again.
   *
   * @author Ivo Jansch <ivo@achievo.org>   
   * @package atk
   * @subpackage handlers
   *
   */
  class atkSaveHandler extends atkActionHandler
  {
    /**
     * The action handler method
     */
    function action_save()
    {      
      $record = $this->m_node->updateRecord();   
      
      if ($this->m_postvars['atkcancel']=="")
      {      

        // just before we validate the record we call the preAdd() to check if the record needs to be modified
        $this->m_node->preAdd($record);

        $this->m_node->validate($record, "add");


        if(!is_array($record['atkerror']))
          $record['atkerror'] = array();

        $error = count($record['atkerror']) > 0;

        foreach (array_keys($record) as $key)
        {
          $error = $error || (is_array($record[$key]) && array_key_exists('atkerror', $record[$key]) && count($record[$key]['atkerror']) > 0);
        }

        if ($error)
        {
          $this->m_action="add";
          $this->m_node->m_action = "add";
          $page = &$this->getPage();
          $addhandler = &$this->m_node->getHandler("add");
          $page->addContent($addhandler->invoke("addPage", $record));
        }
        else
        {
          if(!$this->m_node->addDb($record, true, "add"))
          {
            $this->m_node->m_db->rollback();
            if($this->m_node->m_db->getErrorType()=="user")
            {
              triggerError($record, 'Error', $this->m_node->m_db->getErrorMsg(), '', '');
              $this->m_action="add";
              $this->m_node->m_action = "add";
              $this->m_node->addPage($record);
            }
            else
            {
              $location = $this->m_node->feedbackUrl("save", ACTION_FAILED, $record, $this->m_node->m_db->getErrorMsg());
              $this->m_node->redirect($location);
            }
          }
          else
          {
            $this->m_node->m_db->commit();
            
            $location="";
            if ($this->m_node->hasFlag(NF_EDITAFTERADD))
            {
              $location = session_url($_SERVER["PHP_SELF"].'?atknodetype='.$this->m_node->atknodetype().'&atkaction=edit&atkselector='.rawurlencode($this->m_node->primaryKey($record)), SESSION_REPLACE);
            }
            else
            {
              $location = $this->m_node->feedbackUrl("save", ACTION_SUCCESS, $record);
            }
            $this->m_node->redirect($location, $record);
          }
        }
      }
      else
      {
        // Cancel was pressed
        $location = $this->m_node->feedbackUrl("save", ACTION_CANCELLED, $record);
        $this->m_node->redirect($location);
      }
    }
  }

?>