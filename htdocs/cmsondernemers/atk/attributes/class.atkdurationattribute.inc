<?php
  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be 
   * included in the distribution.
   *
   * @package atk
   * @subpackage attributes
   *
   * @copyright (c)2000-2004 Ivo Jansch
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 5.1 $
   * $Id: class.atkdurationattribute.inc,v 5.1 2004/05/31 22:11:10 ivo Exp $
   */
   
  /**
   * The atkDurationAttribute is an attribute for entering a length of time.
   *
   * @author Ivo Jansch <ivo@achievo.org>
   * @package atk
   * @subpackage attributes
   *
   */
  class atkDurationAttribute extends atkAttribute
  {    
    /** member vars **/
    var $m_resolution_min;
    var $m_maxtime_min;
    
  
    /**
     * Default Constructor, sets up atkAttribute 
     *
     * @param $name
     * @param $resolution
     * @param $maxtime
     * @param $flags
     * @see atkAttribute
     */
    function atkDurationAttribute($name, $resolution="1m", $maxtime="10h", $flags=0, $size=0)
    {
      $this->atkAttribute($name,$flags, $size); // base class constructor
      $hms = substr($resolution,-1);
      $resolution = substr($resolution,0,-1);

      if (strtoupper($hms)=="H") $factor = 60;
      else $factor=1;      
     
      $this->m_resolution_min = $resolution*$factor;
      
      $hms = substr($maxtime,-1);
      $maxtime = substr($maxtime,0,-1);
      if (strtoupper($hms)=="H") $factor = 60;
      else $factor=1;      
      
      $this->m_maxtime_min = $maxtime*$factor;
    }

    /**   
     * Returns a piece of html code that can be used in a form to edit this
     * attribute's value. (hours, minutes and seconds will be a dropdownbox)
     * @param array $record 
     * @return String Piece a of HTML Code
     */
    function edit($record="", $fieldprefix="")
    {  
      $id = $fieldprefix.$this->formName();
      if ($this->m_maxtime_min>=60)
      {      
        $this->registerKeyListener($id.'_hours', KB_CTRLCURSOR|KB_LEFTRIGHT);  
        $result.= '<select id="'.$id.'_hours" name="'.$id.'[hours]">';
        for ($h=0;$h<=$this->m_maxtime_min/60;)
        { 
          $result .= '<option value="'.$h.'" ';         
          if ($record[$this->fieldName()]['hours']==$h) $result.= "selected";
          $result .='>'.$h.' '.text('hours');
          if ($this->m_resolution_min<=60)
          {
            $h++;
          }
          else
          {
            $h = floor($h+$this->m_resolution_min/60);
          }
        }
      }
      $result.='</select>';
      
      if ($this->m_maxtime_min>=1&&$this->m_resolution_min<60)
      {
        $this->registerKeyListener($id.'_minutes', KB_CTRLCURSOR|KB_LEFTRIGHT);  
        $result.= '<select id="'.$id.'_minutes" name="'.$id.'[minutes]">';
        for ($m=0;$m<60||($this->m_maxtime_min<60 && $m<$this->m_maxtime_min);)
        { 
          $result .= '<option value="'.$m.'" ';
          if ($record[$this->fieldName()]['minutes']==$m) $result.= "selected";
          $result.='>'.$m.' '.text('minutes');
          if ($this->m_resolution_min<=1)
          {
            $m++;
          }
          else
          {
            $m = $m+$this->m_resolution_min;
          }
        }
      }
      $result.='</select>';   
      
      return $result;
    }
    
    function hide($record="", $fieldprefix="")
    {
      $result = '<input type="hidden" name="'.$fieldprefix.$this->formName().'[hours]" value="'.$record[$this->fieldName()]["hours"].'">';
      $result.= '<input type="hidden" name="'.$fieldprefix.$this->formName().'[minutes]" value="'.$record[$this->fieldName()]["minutes"].'">';
      return $result;
    }
    
    /**   
     * Returns a piece of html code that can be used in a form to search values.    
     * @param array $record Array of records, with for each record 2 fields (hours, minutes)
     * @return String Piece a of HTML Code
     */
    function search($record="")
    {
      return "&nbsp;"; // currently not searchable.
    }
    
    

    /**
     * Convert Time Array to minutes
     * @param $rec Array with 2 fields (hours, minutes)
     * @return integer indicating the number of minutes
     */
    function value2db($rec)
    {
        return $rec[$this->fieldName()]["hours"]*60+$rec[$this->fieldName()]["minutes"];
    }

    /**
     * Convert time to Array
     * @param $rec contains the time in minute format
     * @return Array with 2 fields (hours, minutes)
     */
    function db2value($rec)
    {
      $hours = floor($rec[$this->fieldName()]/60);
      return Array("hours"=>$hours,
                   "minutes"=>$rec[$this->fieldName()]-($hours*60)
                   );
    }

    /**
     * Function display's the time
     * @param $record Array with time
     * @return String with YYYY-MM-DD
     */
    function display($record)
    {
      $value=$record[$this->fieldName()];

      return sprintf("%02d",$value["hours"]).":".sprintf("%02d",$value["minutes"]);
    }
    
    /**
     * Check if the attribute is empty
     * @return true if it's empty
     */
    function isEmpty($postvars)
    {
      return ($postvars[$this->fieldName()]["hours"]==0&&$postvars[$this->fieldName()]["minutes"]==0);
    }
    
    function sum($rec1, $rec2)
    {                  
      if (!is_array($rec1))
      {   
        $a=Array("hours"=>0,"minutes"=>0);       
      }
      else
      {        
        $a = $rec1[$this->fieldName()];
      }      
      
      $b = $rec2[$this->fieldName()];              
      
      $total = ($a["hours"]*60)+$a["minutes"]+($b["hours"]*60)+$b["minutes"];      
         
      return Array($this->fieldName()=>Array("hours"=>(floor($total/60)),"minutes"=>$total%60));
    }
    
    function getSearchModes()
    {
      // exact match and substring search should be supported by any database.
      // (the LIKE function is ANSI standard SQL, and both substring and wildcard
      // searches can be implemented using LIKE)
      // Possible values
      //"regexp","exact","substring", "wildcard","greaterthan","greaterthanequal","lessthan","lessthanequal"
      return array("exact");
    }
    
    function dbFieldType()
    {
      return "number";
    }
  }
?>