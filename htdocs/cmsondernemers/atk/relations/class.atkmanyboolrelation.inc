<?php
  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be 
   * included in the distribution.
   *
   * @package atk
   * @subpackage relations
   *
   * @copyright (c)2000-2004 Ivo Jansch
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 5.6 $
   * $Id: class.atkmanyboolrelation.inc,v 5.6 2004/10/21 13:00:02 mark Exp $
   */
  
  /** @internal include baseclass. */
  userelation("atkmanytomanyrelation");
   
  /**
   * Many-to-many relation.
   *
   * The relation shows a list of available records, and a set of checkboxes
   * to link the records with the current record on the source side.
   *   
   * @author Ivo Jansch <ivo@achievo.org>
   * @package atk
   * @subpackage relations
   *
   */  
  class atkManyBoolRelation extends atkManyToManyRelation
  {

    /**
     * Constructor.
     * @param String $name The name of the relation.
     * @param String $link The full name of the node that is used as
     *                     intermediairy node.
     * @param String $destination The full name of the node that is the
     *                            destination end of the relationship.
     * @param int $flags Attribute flags.
     */
    function atkManyBoolRelation($name, $link, $destination, $flags=0)
    {
      $this->atkManyToManyRelation($name, $link, $destination, $flags);
    }

    /**
     * Return a piece of html code to edit the attribute
     * @param $record Current record
     * @return String piece of html code
     */
    function edit($record="", $fieldprefix="")
    {
    //var_dump($record);
      $this->createDestination();
      $this->createLink();

      $selectedPk = array();

      // first the selected records..
      for ($i=0;$i<count($record[$this->m_name]);$i++)
      {

        if(is_array($record[$this->fieldName()][$i][$this->remoteKey()]))
          $newselected = $this->m_destInstance->primaryKey($record[$this->m_name][$i][$this->remoteKey()]);
        else{
          $newselected = $this->m_destInstance->primaryKey(array($this->m_destInstance->primaryKeyField()=>$record[$this->m_name][$i][$this->remoteKey()]));
        }
        $selectedPk[] = $newselected;
       }
      
      // now select all records
      $recordset = $this->m_destInstance->selectDb();

      $result.= '<table border="0">';
      for ($i=0;$i<count($recordset);$i++)
      {
        $detaillink = "&nbsp;";
        $selector="";
        if (in_array($this->m_destInstance->primaryKey($recordset[$i]),$selectedPk))
        {
          $sel = "checked";
          if (!$this->m_linkInstance->hasFlag(NF_NO_EDIT)&&$this->m_linkInstance->allowed("edit"))
          {
            $selector = $this->localKey()."='".$record[$this->m_ownerInstance->primaryKeyField()]."'".
              ' AND '.$this->remoteKey().
              "='".$recordset[$i][$this->m_destInstance->primaryKeyField()]."'";

            // Create link to details.
            //var_dump($recordset[$this->fieldName()]);
            $detaillink = href(dispatch_url($this->m_link, "edit", array("atkselector"=>$selector)),
              "[".text("details")."]",
              SESSION_NESTED,
              true);
          }
        }
        else
        {
          $sel = "";
        }
        $result.= '<tr><td class="table"><input type="checkbox" name="'.
                       $fieldprefix.$this->fieldName().
                       '_AMDAE_'.$this->remoteKey().
                       '[]" value="'.
                       $recordset[$i][$this->m_destInstance->primaryKeyField()].
                       '" '.$sel.'></td><td class="table">'.
                       $this->m_destInstance->descriptor($recordset[$i]).
                       '</td><td class="table">'.$detaillink.'</td></tr>'."\n";
      }
      $result .= '</table>';
      return $result;
    }

    /**
     * Stores the values in the database
     * @param $notused Not used
     * @param $record Current record
     */
    function store($notused, $record, $mode)
    {
      //atkerror(arrayToString($record));
    
      $this->createLink();
      $this->createDestination();
      $rel = &$this->m_linkInstance;
      
      // Find items that are checked.
      $checked = array();
      for ($i=0;$i<count($record[$this->fieldName()]);$i++)
      {
        if(is_array($record[$this->fieldName()][$i][$this->remoteKey()]))
          $newchecked = $record[$this->fieldName()][$i][$this->remoteKey()][$this->m_destInstance->primaryKeyField()];
        else
          $newchecked = $record[$this->fieldName()][$i][$this->remoteKey()];

      }
      
      if(count($checked) > 0)
        $filter = "NOT IN (".implode(",",$checked).",0)";

      if (is_object($rel) && $rel->deleteDb($this->localKey()."=".$record[$this->m_ownerInstance->primaryKeyField()]." AND ".$this->remoteKey()." $filter"))
      {
        for ($i=0;$i<count($record[$this->fieldName()]);$i++)
        {
          // magical stuff..
          $newrecord = $this->m_destInstance->initial_values(); 
          $newrecord[$this->remoteKey()][$this->m_destInstance->primaryKeyField()] = $record[$this->fieldName()][$i][$this->remoteKey()];
  
          $newrecord[$this->localKey()][$this->m_ownerInstance->primaryKeyField()] = $record[$this->m_ownerInstance->primaryKeyField()];
  
          // First check if the record does not exist yet.
          /* @var $rel atkNode */
          $existing = $rel->selectDb($rel->primaryKey($newrecord), "", "", "", $rel->m_primaryKey);
          if (!count($existing))
          {
            if (!$rel->addDb($newrecord, true, $mode))
            {
              return false;
            }
          }
        }        
        return true;
      }
      return false;
    }
    
    /**
     * Check if the attribute is empty
     * @return true if it's empty
     */
    function isEmpty($postvars)
    {      
      return (!is_array($postvars[$this->fieldName()]) || count($postvars[$this->fieldName()])==0);
    }

  }

?>
