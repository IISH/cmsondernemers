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
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 5.1 $
   * $Id: class.atkoci8query.inc,v 5.1 2004/05/31 22:11:10 ivo Exp $
   */
   
  /**
   * Query build for Oracle 8i databases.
   *
   * @author Sandy Pleyte <sandy@ibuildings.nl>
   * @package atk
   * @subpackage db
   *
   */   
  class atkOci8Query extends atkQuery
  {

    /**
    * Makes a Join SQL query for Oracle 
    * 
    * @param $table Table name
    * @param $alias Alias for the table
    * @param $condition Condition for the join
    * @since ATK 0.5.2 
    */ 
    
    function addJoin($table, $alias, $condition, $outer)
    {      
      if ($outer) $condition.=" (+)";
      $this->addtable($table,$alias);
      $this->addcondition($condition);
    }
    
    function _addLimiter(&$query)
    {
      /* limit? */
      if ($this->m_offset >= 0 && $this->m_limit > 0)
      {
        /* row id's start at 1! */
	      $query="SELECT * FROM (SELECT rownum AS rid, XX.* FROM (".
            $query.") XX) YY  WHERE YY.rid >= ".($this->m_offset+1)." AND YY.rid <= ".($this->m_offset + $this->m_limit);
      }	      
    }
  }

?>