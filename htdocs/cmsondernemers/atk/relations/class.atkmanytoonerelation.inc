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
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @copyright (c)2000-2004 Ivo Jansch
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 5.19 $
   * $Id: class.atkmanytoonerelation.inc,v 5.19 2005/02/09 08:08:18 boy Exp $
   */   

  /**
   * flag(s) specific for atkManyToOneRelation 
   */
  define("AF_RELATION_AUTOLINK", AF_SPECIFIC_1); // create edit/view links for the items in a manytoonerelation dropdown.
  
  /**
   * @internal include base class
   */
  userelation("atkrelation");

  /**
   * @internal include used utility class(es)
   */
  atkimport("atk.utils.atkstringparser");
  
  /**
   * A N:1 relation between two classes.
   *
   * For example, projects all have one coordinator, but one
   * coordinator can have multiple projects. So in the project
   * class, there's a ManyToOneRelation to a coordinator.
   *
   * This relation essentially creates a dropdown box, from which
   * you can select from a set of records.
   *
   * @author Ivo Jansch <ivo@achievo.org>
   * @package atk
   * @subpackage relations
   *
   */         
  class atkManyToOneRelation extends atkRelation
  {  
    /**
     * By default, we do a left join. this means that records that don't have
     * a record in this relation, will be displayed anyway. NOTE: set  this to
     * false only if you know what you're doing. When in doubt, 'true' is
     * usually the best option.
     * @var boolean
     */
    var $m_leftjoin = true; 
                            
    /**
     * The array of referential key fields.
     * @access private
     * @var array
     */
    var $m_refKey = array(); 
               
    /**
     * SQL statement with extra filter for the join that retrieves the
     * selected record.
     * @var String
     */
    var $m_joinFilter = ""; 
    
    
    /**
     * Constructor.
     * @param String $name The name of the attribute. This is the name of the
     *                     field that is the referential key to the
     *                     destination. 
     *                     For relations with more than one field in the
     *                     foreign key, you should pass an array of
     *                     referential key fields. The order of the fields
     *                     must match the order of the primary key attributes
     *                     in the destination node.
     * @param String $destination The node we have a relationship with.
     * @param int $flags Flags for the relation
     */
    function atkManyToOneRelation($name, $destination, $flags=0)
    {
      if (is_array($name))
      {
        $this->m_refKey = $name;
        
        // ATK can't handle an array as name, so we initialize the
        // underlying attribute with the first name of the referential
        // keys.
        // Languagefiles, overrides, etc should use this first name to 
        // override the relation.
        $this->atkRelation($name[0], $destination, $flags);
      }
      else
      {
        $this->m_refKey[] = $name;
        $this->atkRelation($name, $destination, $flags);
      }
    }

    /**
     * Convert value to DataBase value     
     * @param $rec
     * @return
     */
    function value2db($rec)
    {
      if ($this->isEmpty($rec))
      {
        atkdebug($this->fieldName()." IS EMPTY!");
        return NULL;
      }
      else
      {
        if ($this->createDestination())
        {
          if (is_array($rec[$this->fieldName()]))
          {                 
            $pkfield = $this->m_destInstance->m_primaryKey[0];
            //return $rec[$this->fieldName()][$pkfield];           
            $pkattr = &$this->m_destInstance->getAttribute($pkfield);
            return $pkattr->value2db($rec[$this->fieldName()]);
          }
          else
          {
            return $rec[$this->fieldName()];           
          }
        }
      }
      // This never happens, does it?
      return "";
    }

    /**
     * Fetch value out of record
     * @param $rec Record
     * @return decoded value
     */
    function fetchValue($rec)
    {      
      if ($rec[$this->fieldName()]!="")
      {      
        // Split the primary key of the selected record into its 
        // referential key elements.
        $keyelements = decodeKeyValueSet($rec[$this->fieldName()]);
      
        $result = array();
      
        foreach ($keyelements as $key=>$value)
        {
          // Tablename must be stripped out because it is in the way..
          if (strpos($key,'.')>0)
          {
            $field = substr($key,strpos($key,'.')+1);
          }
          else
          {
            $field = $key;
          }
          $result[$field] = $value;
        }        
        return $result;
      }            
      return NULL;
    }


    /**
     * Converts DataBase value to normal value
     * @param $rec Record
     * @return decoded value
     */
    function db2value($rec)
    {    
      $myrec = $rec[$this->fieldName()];
            
      if (is_array($myrec)) return $myrec;
      else
      {
        // if the record is not an array, probably only the value of the primary key was loaded.
        // This workaround only works for single-field primary keys.
        if ($this->createDestination())
          return array($this->m_destInstance->primaryKeyField() => $myrec);
      }      
    }

    /**
     * Returns a displayable string for this value.
     * @param $record Record
     * @return a displayable string
     */
    function display($record)
    {      
      if ($this->createDestination())      
      {        
      
        if (count($record[$this->fieldName()])==1)
        {
          // The record consist of only one field, which is the primary key.
          // This means we have to load the rest of the data, in order to 
          // load a descriptor. 
          atkdebug("Delayed loading of descriptor fields for ".$this->m_name);
          $recs = $this->m_destInstance->selectDb($this->m_destInstance->primaryKey($record[$this->fieldName()]), "", "", "", $this->m_destInstance->descriptorFields());
          $title = $this->m_destInstance->descriptor($recs[0]);
        }
        else
        {        
          if(!$this->isEmpty($record))
          {
            $title = $this->m_destInstance->descriptor($record[$this->fieldName()]);
          }
          else
          {
            $title = ""; // no record
          }
        }

        $result = $title; // default result is just the title.
 
        if ($this->hasFlag(AF_RELATION_AUTOLINK)) // create link to edit/view screen
        {                                      
          if (($this->m_destInstance->allowed("view"))&&!$this->m_destInstance->hasFlag(NF_NO_VIEW)&&$title!="")
          {
            $result = href("dispatch.php?atknodetype=".$this->m_destination."&atkaction=view&atkselector=".rawurlencode($this->m_destInstance->primaryKey($record[$this->fieldName()])), $title, SESSION_NESTED);
          }
        }

        return $result;
      }
      return "";
    }

    /**
     * Returns a piece of html code that can be used in a form to edit this
     * attribute's value.
     * @param array  $record       The edit record
     * @param String $fieldprefix  Prefix for the fieldname
     * @return Piece of html code that can  be used in a form to edit this
     */
    function edit(&$record, $fieldprefix="")
    {
      global $ATK_VARS;
      atkimport("atk.utils.atkstringparser");

      // Two variances.. one with a dropdownbox containing all records,
      // and one with a dropdownbox that contains the last 10 used records,
      // with a link to a selector with all records. This second one is for
      // relations with large tables, so is only used when AF_LARGE is set.

      if ($this->createDestination())
      {
        $id = $fieldprefix.$this->fieldName();
        $autolink = $this->getRelationAutolink($id, $filter);
        
        if (!$this->hasFlag(AF_LARGE))
        {
          // normal dropdown..
          $currentPk = $this->m_destInstance->primaryKey($record[$this->fieldName()]);
          if ($this->m_destinationFilter!="")
          {
            $parser = new atkStringParser($this->m_destinationFilter);
            $filter = $parser->parse($record);
            $this->m_destInstance->addFilter($filter);
          }
          $recordset = $this->m_destInstance->selectDb("","","","",atk_array_merge($this->m_destInstance->descriptorFields(),$this->m_destInstance->m_primaryKey));

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
            if ($this->hasFlag(AF_OBLIGATORY)==false)
            {
              //get the select_none string
              $nodename = $this->m_destInstance->m_type;
              $modulename = $this->m_destInstance->m_module;;
              $nonetext = atktext('select_none', $modulename, $nodename);

              // Relation may be empty, so we must provide an empty selectable..
              $result.= '<option value="">'.$nonetext;
            }
          
            // Newly added check
            $newlyadded = $this->m_ownerInstance->m_postvars[$id."_ret"];
            for ($i=0;$i<count($recordset);$i++)
            {
              $pk = $this->m_destInstance->primaryKey($recordset[$i]);
              if (($newlyadded!="" && $newlyadded==$pk) || ($newlyadded=="" && $pk==$currentPk)) $sel = "selected"; else $sel = "";
              $result.= '<option value="'.$pk.'" '.$sel.'>'.$this->m_destInstance->descriptor($recordset[$i]);
            }
            $result.='</select>';

          }
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
        $result.=$autolink;
        return $result;
      }
      return atkerror("Could not create destination for $this->m_destination!");
    }

    /**
     * Creates and returns the auto edit/view links
     * @param String $id          The field id
     * @param String $filter      Filter that we want to apply on the destination node
     * @return String The HTML code for the autolink links
     */
    function getRelationAutolink($id, $filter)
    {
      $autolink = "";
      if ($this->hasFlag(AF_RELATION_AUTOLINK)) // auto edit/view link
      {
        $page = &atkPage::getInstance();
        $page->register_script(atkconfig("atkroot")."atk/javascript/class.atkmanytoonerelation.js");
        if ($this->m_destInstance->allowed("edit"))
        {
          $editlink = session_url("dispatch.php?atknodetype=".$this->m_destination."&atkaction=edit&atkselector=REPLACEME",SESSION_NESTED);
          $autolink = "&nbsp;<a href='javascript:atkSubmit(mto_parse(\"".atkurlencode($editlink)."\", document.entryform.".$id.".value))'>".text('edit')."</a>";
        }
        if ($this->m_destInstance->allowed("add"))
        {
          $autolink.= "&nbsp;".href("dispatch.php?atknodetype=".$this->m_destination."&atkaction=add&atkpkret=".$id."_ret".($filter!=""?"&atkfilter=".rawurlencode($filter):""),text("new"),SESSION_NESTED,true);
        }
      }
      return $autolink;
    }

    /**
     * Returns a piece of html code that can be used in a form to display
     * hidden values for this attribute.
     * @param $record Array with values
     * @return Piece of htmlcode
     */
    function hide($record="", $fieldprefix="")
    {
      if ($this->createDestination())
      {              
        $currentPk = "";
        if (count($record[$this->fieldName()]))
        {
          $currentPk = $this->m_destInstance->primaryKey($record[$this->fieldName()]);
        }
        $result = '<input type="hidden" name="'.$fieldprefix.$this->formName().
                  '" value="'.$currentPk.'">';
        return $result;
      }
      return "";
    }


    /**
     * Returns a piece of html code that can be used in a form to search
     * @param $record Record
     * @param $size Size of the editbox
     * @param $maxsize Maxsize of the editbox
     * @return Piece of html code that can  be used in a form to edit this
     */
    function search($record="", $extended=false, $fieldprefix="")
    {
      if (!$this->hasFlag(AF_LARGE))
      {
        if ($this->createDestination())
        {
          if ($this->m_destinationFilter!="")
          {
            $parser = new atkStringParser($this->m_destinationFilter);
            $filter = $parser->parse($record);
            $this->m_destInstance->addFilter($filter);
          }
          $recordset = $this->m_destInstance->selectDb("","","","",atk_array_merge($this->m_destInstance->descriptorFields(),$this->m_destInstance->m_primaryKey));
          
          $result = '<select ';
          if ($extended)
          {
            $result.='multiple size="'.min(5,count($recordset)+1).'"';
          }
          $result.='name="atksearch['. $this->fieldName().'][]">';
  
          $pkfield = $this->m_destInstance->primaryKeyField();
  
          $result.= '<option value="">'.atktext('search_all');
  
          for ($i=0;$i<count($recordset);$i++)
          {
            $pk = $recordset[$i][$pkfield];
            if (atk_in_array($pk, $record[$this->fieldName()])) $sel = "selected"; else $sel = "";
            $result.= '<option value="'.$pk.'" '.$sel.'>'.$this->m_destInstance->descriptor($recordset[$i]);
          }
          $result.='</select>';
          return $result;
        }
        return "";
      }
      else
      {
        $id = "atksearch[".$fieldprefix.$this->fieldName()."]";
        $this->registerKeyListener($id, KB_CTRLCURSOR|KB_UPDOWN);
        $result = '<input type="text" id="'.$id.'" name="'.$id.'" value="'.$record[$this->fieldName()].'"'.
          ($this->m_searchsize > 0 ? ' size="'.$this->m_searchsize.'"' : '').
          ($this->m_maxsize > 0 ? ' maxlength="'.$this->m_maxsize.'"' : '').'>';
        return $result;
      }
    }

    /**
     * Creates an search condition for a given search value
     * @param  atkQuery $query query to hook ourselves in to
     * @param  string   $table the table name
     * @param  string   $value the search value
     * @return string          a piece of the SQL where clause
     */
    function searchCondition(&$query, $table, $value, $searchmode)
    {
      if (is_array($value))
      {
        foreach($value as $val)
        {
          if ($val!=="")
          {
            $searchcondition = $this->getSearchCondition($query, $table, $val, $searchmode);
            $query->addSearchCondition($searchcondition);
          }
        }
      }
      else
      {
        $searchcondition = $this->getSearchCondition($query, $table, $value, $searchmode);
        $query->addSearchCondition($searchcondition);
      }
    }
    
    function getSearchCondition(&$query, $table, $value, $searchmode)
    {      
      if (!$this->hasFlag(AF_LARGE) && !$this->createDestination())
      {
        // We only support 'exact' matches. 
        // But you can select more than one value, which we search using the IN() statement,
        // which should work in any ansi compatible database.
        if (is_array($value) && count($value)>0 && $value[0]!="") // This last condition is for when the user selected the 'search all' option, in which case, we don't add conditions at all.
        {
          if (count($value)==1) // exactly one value
            return $query->exactCondition($table.".".$this->fieldName(),escapeSQL($value[0]));
          else // search for more values using IN()
            return $table.".".$this->fieldName()." IN ('".implode("','",$value)."')";
        }
      }
      else
      {
        if ($this->createDestination())
        {
          $searchConditions = array();
          $descfields = array();
          $descfields = $this->m_destInstance->descriptorFields();
          $descfields[] = $this->m_destInstance->primaryKeyField();
          
          foreach($descfields as $descField)
          {
            if ($descField !== $prevdescfield)
            {
              $p_attrib = &$this->m_destInstance->getAttribute($descField);
              if (is_object($p_attrib))
              {
                if ($query->m_joinaliases[$this->m_destInstance->m_table."*".$this->m_destInstance->primaryKeyField()])
                {
                  $itsTable = $query->m_joinaliases[$this->m_destInstance->m_table."*".$this->m_destInstance->primaryKeyField()];
                }
                else if ($p_attrib->m_destInstance)
                {
                  $itsTable = $p_attrib->m_destInstance->m_table;
                }
                else
                {
                  $itsTable = $p_attrib->m_ownerInstance->m_table;
                }

                // checking for the getSearchCondition
                // for backwards compatibility
                if (method_exists($p_attrib,"getSearchCondition"))
                {
                  $searchcondition = $p_attrib->getSearchCondition($query,$itsTable,$value,$searchmode);
                  if ($searchcondition)
                  {
                    $searchConditions[] = $searchcondition;
                  }
                }
                else
                {
                  $p_attrib->searchCondition($query,$itsTable,$value,$searchmode);
                }
                
              }
              $prevdescfield = $descField;
            }
          }
          return implode(" OR ",$searchConditions);
        }
      }
      return;
    }

    /**
     * Function to add values to query
     * @param $query
     * @param $tablename Table name
     * @param $fieldaliasprefix Field alias prefix
     * @param $level Level
     */
    function addToQuery(&$query, $tablename="", $fieldaliasprefix="", $rec="", $level=0, $mode="")
    {
      if ($this->createDestination())
      {
        if ($mode != "update" && $mode != "add")
        {              
          $alias = $fieldaliasprefix . $this->fieldName(); 
          $query->addJoin($this->m_destInstance->m_table, 
                          $alias,
                          $this->getJoinCondition($query, $tablename, $alias),
                          $this->m_leftjoin);
          $this->m_destInstance->addToQuery($query, $alias, $level+1);
        }
        else
        {
          for ($i=0, $_i=count($this->m_refKey); $i<$_i; $i++)
          {
            if ($rec[$this->fieldName()]==NULL)
            {            
              $query->addField($this->m_refKey[$i],"NULL","","",false); 
            }
            else
            {         
              $fk = &$this->m_destInstance->getAttribute($this->m_destInstance->m_primaryKey[$i]);
              $query->addField($this->m_refKey[$i],$fk->value2db($rec[$this->fieldName()]),"","",!$this->hasFlag(AF_NO_QUOTES));
            }
          }    
        } 
      }
    }

    /**
     * Dummy function
     */
    function validate(&$record, $mode)
    {
    }

    function equal($recA, $recB)
    {
      if ($this->createDestination())
      {
        return (($recA[$this->fieldName()][$this->m_destInstance->primaryKeyField()]
                 ==
                 $recB[$this->fieldName()][$this->m_destInstance->primaryKeyField()])
               ||
                ($this->isEmpty($recA)&&$this->isEmpty($recB)));
             // we must also check empty values, because empty values need not necessarily
             // be equal (can be "", NULL or 0.
      }
      return false;
    }
    
    function dbFieldType()
    {
      // The type of field that we need to store the foreign key, is equal to 
      // the type of field of the primary key of the node we have a 
      // relationship with.
      if ($this->createDestination())
      {
        return $this->m_destInstance->m_attribList[$this->m_destInstance->primaryKeyField()]->dbFieldType();
      }
      return "";
    }
    
    function dbFieldSize()
    {
      // The size of the field we need to store the foreign key, is equal to 
      // the size of the field of the primary key of the node we have a 
      // relationship with.
      if ($this->createDestination())
      {
        return $this->m_destInstance->m_attribList[$this->m_destInstance->primaryKeyField()]->dbFieldSize();
      }
      return 0;
    }
    
      
   /**
    * Returns the condition (SQL) that should be used when we want to join a relation's
    * owner node with the parent node.
    * @param String $tablename=""  the tablename on which
    * @param String $fieldalias="" 
    * @return String SQL string for joining the owner with the destination.
    *                Returns false when impossible (f.e. attrib is not a relation).
    */
    function getJoinCondition(&$query, $tablename="",$fieldalias="")
    {
      if (!$this->createDestination()) return false;

      if ($tablename!="") $realtablename=$tablename;
      else $realtablename = $this->m_ownerInstance->m_table;
      $joinconditions = array();
      
      if ($fieldalias) $query->m_joinaliases[$this->m_destInstance->m_table."*".$this->m_destInstance->primaryKeyField()] = $fieldalias;
      else $query->m_joinaliases[$this->m_destInstance->m_table."*".$this->m_destInstance->primaryKeyField()] = $this->m_destInstance->m_table;
      
      $alias = $query->m_joinaliases[$this->m_destInstance->m_table."*".$this->m_destInstance->primaryKeyField()];
      
      for ($i=0, $_i=count($this->m_refKey); $i<$_i; $i++)
      {
        $joinconditions[] = $realtablename.".".$this->m_refKey[$i].
        "=".
        $alias.".".$this->m_destInstance->m_primaryKey[$i];
      }

      if ($this->m_joinFilter!="")
      {
        $joinconditions[] = $this->m_joinFilter;
      }
      return implode(" AND ",$joinconditions);
    }
  }

?>
