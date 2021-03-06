<?php

  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be 
   * included in the distribution.
   *
   * @package atk
   * @subpackage db
   *
   * @copyright (c)2000-2004 Ivo Jansch
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 5.2 $
   * $Id: class.atkmysqlquery.inc,v 5.2 2004/05/31 22:11:10 ivo Exp $
   */

  /**
   * SQL Builder for MySQL databases. 
   *
   * @author Ivo Jansch <ivo@achievo.org>
   * @package atk
   * @subpackage db
   *
   */
  class atkMysqlQuery extends atkQuery
  {
    /**
     * Generate an SQL searchcondition for a regular expression match.
     *
     * @param String $field The fieldname on which the regular expression
     *                      match will be performed.
     * @param String $value The regular expression to search for.
     * @param boolean $inverse Set to false (default) to perform a normal 
     *                         match. Set to true to generate a SQL string 
     *                         that searches for values dat do not match.
     * @return String A SQL regexp expression.
     */ 
    function regexpCondition($field, $value, $inverse=false)
    {
      if($value[0]=='!') 
      {
        return $field." NOT REGEXP '".substr($value,1,strlen($value))."'";
      }
      else
      {
        return $field." REGEXP '$value'";
      }
    }
    
    /**
     * Generate an SQL searchcondition for a soundex match.
     *
     * @param String $field The fieldname on which the soundex match will 
     *                      be performed.
     * @param String $value The value to search for.
     * @param boolean $inverse Set to false (default) to perform a normal 
     *                         match. Set to true to generate a SQL string 
     *                         that searches for values dat do not match.
     * @return String A SQL soundex expression.
     */ 
    function soundexCondition($field, $value, $inverse=false)
    {
      if($value[0]=='!') 
      {
        return "soundex($field) NOT like concat('%',substring(soundex('".substr($value,1,strlen($value))."') from 2),'%')";
      }
      else
      {
        return "soundex($field) like concat('%',substring(soundex('$value') from 2),'%')";
      }
    }
    
    /**
     * Prepare the query for a limit. 
     * @access private
     * @param String $query The SQL query that is being constructed.
     */
    function _addLimiter(&$query)
    {
      if ($this->m_offset >= 0 && $this->m_limit > 0)
      {
        $query .= " LIMIT ".$this->m_offset.", ".$this->m_limit;  
      }      
    }
  }

?>