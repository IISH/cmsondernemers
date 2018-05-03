<?php
include_once("class.atkmanytoonerelation.inc");

class atkManyToOneRelationXF  extends atkManyToOneRelation
{

	function edit(&$record, $fieldprefix="") {
		global $ATK_VARS;
      atkimport("atk.utils.atkstringparser");

      // Two variances.. one with a dropdownbox containing all records,
      // and one with a dropdownbox that contains the last 10 used records,
      // with a link to a selector with all records. This second one is for
      // relations with large tables, so is only used when AF_LARGE is set.

      if ($this->createDestination())
      {
        if (!$this->hasFlag(AF_LARGE))
        {
          // normal dropdown..
          $currentPk = $this->m_destInstance->primaryKey($record[$this->fieldName()]);
          $currentPkValue = $this->m_destInstance->descriptor($record[$this->fieldName()]);

          if ($this->m_destinationFilter!="")
          {
            $parser = new atkStringParser($this->m_destinationFilter);
            $filter = $parser->parse($record);
            $this->m_destInstance->addFilter($filter);
          }
          $recordset = $this->m_destInstance->selectDb("","","","",atk_array_merge($this->m_destInstance->descriptorFields(),$this->m_destInstance->m_primaryKey));

          $id = $fieldprefix.$this->fieldName();
          $autolink = "";
          if ($this->hasFlag(AF_RELATION_AUTOLINK)) // auto edit/view link
          {
            $page = &atkPage::getInstance();
            $page->register_script(atkconfig("atkroot")."atk/javascript/class.atkmanytoonerelation.js");
            if ($this->m_destInstance->allowed("edit"))
            {
              $editlink = session_url("dispatch.php?atknodetype=".$this->m_destination."&atkaction=edit&atkselector=REPLACEME",SESSION_NESTED);
              $autolink = "<a href='javascript:atkSubmit(mto_parse(\"".atkurlencode($editlink)."\", document.entryform.".$fieldprefix.$this->fieldName().".value))'>".text('edit')."</a>";
            }
            if ($this->m_destInstance->allowed("add"))
            {
              $autolink.= "&nbsp;".href("dispatch.php?atknodetype=".$this->m_destination."&atkaction=add&atkpkret=".$id."_ret".($filter!=""?"&atkfilter=".rawurlencode($filter):""),text("new"),SESSION_NESTED,true);
            }
          }          

          // autoselect if there is only one record (if obligatory is not set,
          // we don't autoselect, since user may wist to select 'none' instead
          // of the 1 record.
          if (count($recordset)==1&&$this->hasFlag(AF_OBLIGATORY))
          {
            $result = $this->m_destInstance->descriptor($recordset[0])."&nbsp;&nbsp;";
            $result.= $this->hide(array($this->fieldName()=>$recordset[0]), $fieldprefix);
          }
          else
          {
            $this->registerKeyListener($id, KB_CTRLCURSOR|KB_LEFTRIGHT);
            $result.= '<select id="'.$id.'" name="'.$id.'">';
//            if ($this->hasFlag(AF_OBLIGATORY)==false)
            if (!($this->hasFlag(AF_OBLIGATORY)))
            {
              //get the select_none string
              $nodename = $this->m_destInstance->m_type;
              $modulename = $this->m_destInstance->m_module;;
              $nonetext = atktext('select_none', $modulename, $nodename);

              // Relation may be empty, so we must provide an empty selectable..
//              $result.= '<option value="">'.$nonetext;
              $result.= '<option value="">'.$nonetext . '</option>' . "\n";
            }
          
            // Newly added check
            $newlyadded = $this->m_ownerInstance->m_postvars[$id."_ret"];
            for ($i=0;$i<count($recordset);$i++)
            {
//              $pk = $this->m_destInstance->primaryKey($recordset[$i]);
				$pk = $this->m_destInstance->descriptor($recordset[$i]);

//              if (($newlyadded!="" && $newlyadded==$pk) || ($newlyadded=="" && $pk==$currentPk)) $sel = "selected"; else $sel = "";
              if (($newlyadded!="" && $newlyadded==$pk) || ($newlyadded=="" && $pk==$currentPkValue)) $sel = "selected"; else $sel = "";

//              $result.= '<option value="'.$pk.'" '.$sel.'>'.$this->m_destInstance->descriptor($recordset[$i]);
				$result.= '<option value="'.$pk.'" '.$sel.'>'.$this->m_destInstance->descriptor($recordset[$i]) . '</option>' . "\n";
            }
            $result.='</select>';

          }
          $result.=$autolink;
        }
        else
        {
          // Large mode
          // TODO: Fill a dropdown box with recent records..
          $selname = $fieldprefix.$this->fieldName()."_newsel";        

          if ($ATK_VARS[$selname]!="") 
          // new record selected
          {          
            // $ATK_VARS[$selname] contains full primary key of selected record.
            $recset = $this->m_destInstance->selectDb($ATK_VARS[$selname],"","","",atk_array_merge($this->m_destInstance->descriptorFields(),$this->m_destInstance->m_primaryKey));
            $destrecord = $recset[0];          
            $record[$this->fieldName()] = $destrecord; // put destrecord inside original record.
          }
          elseif (is_array($record[$this->fieldName()]) && count($record[$this->fieldName()]) == count($this->m_destInstance->m_primaryKey)) 
          // old record present, but probably only primary key fields are loaded (this happens after pressing 'save' on a record)                                                                
          {
            $currentPk = $this->m_destInstance->primaryKey($record[$this->fieldName()]);
            $recset = $this->m_destInstance->selectDb($currentPk,"","","",atk_array_merge($this->m_destInstance->descriptorFields(),$this->m_destInstance->m_primaryKey));
            $destrecord = $recset[0];          
            $record[$this->fieldName()] = $destrecord; // put destrecord inside original record.
          }
          elseif (is_array($record[$this->fieldName()]) && !$this->isEmpty($record)) 
          // complete old record present
          {        
            $destrecord = $record[$this->fieldName()];        
          }
          else 
          // no record yet, empty value.
          { 
            $destrecord = array();          
          }
        
          if (count($destrecord))
          {          
            $result.= $this->m_destInstance->descriptor($destrecord)."&nbsp;&nbsp;";            
          }                
        
          $result.=$this->hide($record, $fieldprefix);
          
          // we use the current level to automatically return to this page
          // when we come from the select..
          $atktarget = rawurlencode("dispatch.php?atklevel=".atkLevel()."&".$selname."=[pk]");
          if ($this->m_destinationFilter!="")
          {
            $parser = new atkStringParser($this->m_destinationFilter);
            $filter = $parser->parse($record);
            $result.= href("dispatch.php?atknodetype=".$this->m_destination.
                           "&atkaction=select&atkfilter=".$filter."&atktarget=".$atktarget,
                           text("link_select_".getNodeType($this->m_destination)),
                           SESSION_NESTED,
                           true);
          }
          else
          {
            $result.= href("dispatch.php?atknodetype=".$this->m_destination.
                           "&atkaction=select&atktarget=".$atktarget,
                           text("link_select_".getNodeType($this->m_destination)),
                           SESSION_NESTED,
                           true);
          }
        }
        return $result;
      }
      return atkerror("Could not create destination for $this->m_destination!");
    }




}
?>