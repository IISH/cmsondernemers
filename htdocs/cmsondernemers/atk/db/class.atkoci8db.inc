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
   * @version $Revision: 5.3 $
   * $Id: class.atkoci8db.inc,v 5.3 2004/05/31 22:11:10 ivo Exp $
   */

  /**   
   * @internal Include baseclass
   */
  require_once(atkconfig("atkroot")."atk/db/class.atkdb.inc");
  
  /** 
   * @internal Include error translations
   */
  include_once(atkconfig("atkroot")."atk/db/languages/oracle_".atkconfig('language').'.lng');

  /**
   * Driver for Oracle 8i databases.
   *
   * @author Peter C. Verhage <peter@ibuildings.nl>
   * @package atk
   * @subpackage db
   *
   */
  class atkOci8Db extends atkDb
  {    
    /**
     * @var String
     * @access private
     */
    var $m_type = "oci8";
    
    /**
     * Default constructor
     */
    function atkOci8Db()
    {
      // set the user error's
      $this->m_user_error=array(1,1400,1438,2290,2291,2292,2293);   
      // set critical error's      
      $this->m_critical_error = array(600,904,1017,12154);
    }

    /**
     * Connect to the database.
     * @return int One of the generic DB_ connection statusses.
     */
    function connect()
    {      
      /* establish connection to database */
      if (empty($this->m_link_id))
      {
        $connectfunc = (atkconfig("databasepersistent")?"OCIPlogon":"OCILogon");
        atkdebug("Connectiontype: ".$connectfunc);
        $this->m_link_id = $connectfunc($this->m_user, $this->m_password, $this->m_database);
        if (!$this->m_link_id)
        {
          $error = OCIError(); // can't pass link_id because it is null if connect failed.
          atkdebug("CONNECT ERROR");
          $this->halt($this->_translateError($error));
        }

        /* set default date format for Oracle */ 
        $nls = OCIParse($this->m_link_id, "alter session set nls_date_format='YYYY-MM-DD'");
        $myresult = OCIExecute($nls);
        $error = OCIError($this->m_link_id);
        if ($error["code"]!="")
        {
          $this->halt("Invalid SQL: $query"); 
          return $this->_translateError($error);
        }

      }
      return DB_SUCCESS;
    }
    
    /**
     * Get last error code.
     *
     * If an error occurred, this method can be called to retrieve
     * the generic error code. 
     * @return int One of the generic DB_* statusses.
     */
    function getAtkDbErrno()
    {
      atkdebug("atkoci8db::getAtkDbErrno() -> ".$this->getDbErrno());
      return $this->_translateError(array('code'=>$this->getDbErrno(),'error'=>$this->getDbError()));
    }
  
    /**
     * Translate a driver specific error to a generic ATK db error code.
     * @access private
     * @return int One of the generic DB_* constants
     */
    function _translateError($err)
    {
      $this->m_errno = $err['code'];
      $this->m_error = $err['message'];
      atkdebug('atkoci8db::_translateError() -> DB Error code: '.$err['code']);
      switch ($this->m_errno)
      {
        case     0: return DB_SUCCESS;
        case  1017: return DB_ACCESSDENIED_USER;
        case 12154: return DB_UNKNOWNDATABASE;
        default: 
                    return DB_UNKNOWNERROR;
      }
    }

    /**
     * Disconnect from database.
     */
    function disconnect()
    {
      if($this->m_link_id)
      {
        atkdebug("Disconnecting from database...");
        OCILogoff($this->m_link_id);
      }
    }

    /**
     * Commit the current transaction.
     */
    function commit()
    {
      if($this->m_link_id)
      {
        atkdebug("OCICommit");
        OCICommit($this->m_link_id);
      }    
      return true;
    }

    /*
     * Rollback the the current transaction.
     */
    function rollback()
    {
      if($this->m_link_id)
      {
        atkdebug("OCIRollback");
        OCIRollback($this->m_link_id);
      }
      return true;
    }

    /**
     * Parse and execute a query.
     *
     * If the query is a select query, the rows can be retrieved using the
     * next_record() method. 
     *
     * @param String $query The SQL query to execute
     * @param int $offset Retrieve the results starting at the specified
     *                    record number. Pass -1 or 0 to start at the first
     *                    record.
     * @param int $limit Indicates how many rows to retrieve. Pass -1 to 
     *                   retrieve all rows.
     */
    function query($query, $offset=-1, $limit=-1)
    {
      /* limit? */
      if ($offset >= 0 && $limit > 0)
      {
        /* row id's start at 1! */
	      $query="SELECT * FROM (SELECT rownum AS rid, XX.* FROM (". 
        $query.") XX) YY  WHERE YY.rid >= ".($offset+1)." AND YY.rid <= ".($offset + $limit);
      }	
      
      /* query */
      atkdebug("atkoci8db.query(): ".$query);

      /* connect to database */
      if ($this->connect()==DB_SUCCESS)
      {
        /* free old results */
        if ($this->m_query_id)
        {
          @OCIFreeStatement($this->m_query_id);
          $this->m_query_id = 0;
        }                    

        /* query database */
        $execresult = false;
        $this->m_query_id = @OCIParse($this->m_link_id, $query);              
        
        if ($this->m_query_id) 
        {                
          if(!@OCIExecute($this->m_query_id,OCI_DEFAULT))
          {         
            $error = OCIError($this->m_query_id);
            atkerror("Invalid SQL: $query");    
            $this->halt($this->_translateError($error));
            return false;
          }          
        }
        else
        {        
          /* error in query */          
          $error = OCIError($this->m_link_id);
          atkerror("Invalid SQL: $query");    
          $this->halt($this->_translateError($error));          
          return false;
        }
        $this->m_row = 0;        
        /* return query id */                        
        return true;
      }
      return false;
    }

    /**
     * Retrieve the next record in the resultset.
     * @return mixed An array containing the record, or 0 if there are no more
     *               records to retrieve.
     */
    function next_record()
    {
      /* goto next record */
      $this->m_record = array();

      /* get record data */
      if (@OCIFetchInto($this->m_query_id, $result, OCI_ASSOC|OCI_RETURN_NULLS|OCI_RETURN_LOBS))
      {
        for ($i = 1; $i <= OCINumcols($this->m_query_id); $i++)
        {
          $column = strtoupper(OCIColumnname($this->m_query_id, $i));
          $column_return = strtolower($column);
          $this->m_record[$column_return] = $result[$column];
        }
        $m_row++;

        return 1;
      }

      /* error */
      else
      {
        $error = OCIError($this->m_link_id);
        $this->m_errno = $error["code"];
        $this->m_error = $error["message"];

        /* no more records, no problem */
        if(1403 == $this->m_errno) 
        { 
          $this->m_errno = 0;
          $this->m_error = "";
          $this->disconnect();
        }

        return 0;
      }
    }

    /**
     * Skip to a certain position in the result set for subsequent calls to
     * next_record().     
     *
     * Calling seek() with no parameters places the pointer to the first
     * record in the resultset.
     * 
     * @param int $position The number of the record to skip to. 
     */
    function seek($position=0)
    {
      if ($this->num_rows() > $position) $this->m_row = $position;
      else $this->halt("seek($position) failed: result has ".$this->num_rows()." rows");
    }

    /**
     * Lock a table in the database.
     *
     * @param String $table The name of the table to lock.
     * @param String $mode The lock type. Note that this driver only supports 
     *                     "write".
     *
     * @return boolean True if succesful, false if not.
     */
    function lock($table, $mode="write")
    {
      /* connect first */
      if ($this->connect()==DB_SUCCESS)
      {

        /* lock */
        if ($mode == "write")
        {
          $m_query_id = OCIParse($this->m_link_id, "lock table $table in row exclusive mode");
          $result = @OCIExecute($m_query_id);
          if (!$result) 
          {
            $this->halt("$mode lock on $table failed.");
            return 0;
          }
        }

        /* return result */
        return 1;
      }
      return 0;
    }

    /**
     * Relieve all locks. 
     *
     * The oracle 8i driver has no way to unlock other than committing the
     * current transaction. So be careful with calling this function; any
     * outstanding transaction will be committed.
     *
     * @return boolean True if succesful, false if not.
     */
    function unlock()
    {
      return $this->query("commit");
    }

    /**
     * Retrieve the number of rows affected by the last query.
     *
     * After calling query() to perform an update statement, this method will
     * return the number of rows that was updated.
     *
     * @return int The number of affected rows
     */
    function affected_rows()
    {
      return $this->num_rows();
    }

    /**
     * Retrieve the number of rows in the resultset.
     * 
     * After calling query() to perform a select statement, this method will 
     * return the number of rows in the resultset.
     *
     * @return int The number of rows in the resultset.
     */
    function num_rows()
    {
      return @OCIRowCount($this->m_query_id);
    }

    /**
     * Evaluatie the result; how many fields
     * where affected by the query.
     * @return number of affected fields
     */
    function num_fields()
    {
      return @OCINumCols($this->m_query_idD);
    }


    /**
     * Get the next sequence number of a certain sequence.
     *
     * If the sequence does not exist, it is created automatically. For this
     * to work, the database user should have the 'create sequence' privilege.
     * The name of the sequence in the database is prefixed with "seq_".
     *
     * <b>Example</b>
     * If the database contains a sequence named "seq_employees", the following
     * call will retrieve the next value from the sequence:
     * <code> 
     *   $id = $g_db->nextid("employees");
     * </code>
     *
     * @param $sequence The sequence name
     * @return int The next sequence value
     */
    function nextid($sequence)
    {
      /* connect first */
      if ($this->connect()==DB_SUCCESS)
      {

        /* get sequence number and increment */
        $query = "SELECT seq_".$sequence.".nextval AS nextid FROM dual";
  
        /* execute query */
        $m_query_id = OCIParse($this->m_link_id, $query);
        $id = @OCIExecute($m_query_id,OCI_COMMIT_ON_SUCCESS);
  
        /* error? */
        if (empty($id))
        {
          /* create sequence */
          $query = "CREATE SEQUENCE seq_".$sequence." INCREMENT BY 1 START WITH 1 NOCYCLE NOORDER";
          $m_query_id = OCIParse($this->m_link_id, $query);
          $id = @OCIExecute($m_query_id,OCI_COMMIT_ON_SUCCESS) or $this->halt("cannot create 'seq_$sequence'");
          
          /* try again */
          $query = "SELECT seq_".$sequence.".nextval AS nextid FROM dual";
          $m_query_id = OCIParse($this->m_link_id, $query);
          $id = @OCIExecute($m_query_id) or $this->halt("cannot get nextval from sequence 'seq_$sequence'");
          
          /* empty? */
          if (empty($id)) return 0;
        }
  
        /* get nextid */
        OCIFetchInto($m_query_id, $result);
  
        /* return id */
        return $result[0];
      }
      return 0;
    }

    /**
     * Return the meta data of a certain table
     *
     * @param String $table The name of the table
     * @param boolean $full If set to true, more metadata is retrieved than
     *                      when set to false.
     * @return array The retrieved metadata. See documentation for
     *               atkDb::metadata() for information about the structure
     *               of the returned array.
     */
    function metadata($table, $full=false)
    {
      /* query for data */
      $this->query("SELECT 
                      T.table_name,T.column_name,T.data_type,
                      T.data_length,T.data_precision,T.data_scale,T.nullable,
                      T.char_col_decl_length,I.index_name
                   FROM USER_TAB_COLUMNS T,USER_IND_COLUMNS I
                   WHERE 
                     T.column_name=I.column_name (+)
                     AND T.table_name=I.table_name (+)
                     AND T.table_name=UPPER('$table') 
                   ORDER BY T.column_id");

      atkimport("atk.db.atkddl");
      $ddl = atkDDL::create();
      
      /* result */
      $result = array();
      for ($i = 0; $this->next_record(); $i++)
      {
        $result[$i]["table"]  = $this->m_record["table_name"];
        $result[$i]["name"]   = strtolower($this->m_record["column_name"]);
        $result[$i]["type"]   = $ddl->getGenericType($this->m_record["data_type"]);
        $result[$i]["len"]    = $this->m_record["data_length"];

        if ($this->m_record["index_name"]) $result[$i]["flags"] = "INDEX ";
        if ($this->m_record["nullable"] == "N") $result[$i]["flags"] .= "NOT NULL";
        $result[$i]["format"] = (int)$this->m_record["data_precision"].",".
                                (int)$this->m_record["data_scale"];
        if ($result[$i]["format"] == "0,0") $result[$i]["format"]="";
        $result[$i]["index"]  = $this->m_record["index_name"];
        $result[$i]["chars"]  = $this->m_record["char_col_decl_length"];
        if ($full)
        {
          $result["meta"][$result[$i]["name"]] = $i;
          $result["num_fields"] = $i;
        } 
      }

      /* return */
      return $result;
    }

    /**
     * Return a list of all table names in the current connection.
     *
     * @return array The array containing the names of the tables.
     */
    function table_names()
    {
      /* query */
      $this->query("SELECT table_name, tablespace_name FROM user_tables");

      /* get table names */
      $result = array();
      for ($i=0; $this->next_record(); $i++)
      {
        $result[$i]["table_name"]      = $this->m_record["table_name"];
        $result[$i]["tablespace_name"] = $this->m_record["tablespace_name"];
        $result[$i]["database"]        = $this->m_database;
      }

      /* return result */
      return $result;
    }
    
    /**
     * This function checks in the database if a table exists.     
     *
     * @param String $tableName The name of the table to check.
     * @return boolean True if the table exists, false if not.
     */  
    function tableExists($table)
    {      
      global $g_db;
      
      $found = $g_db->getrows("SELECT count(*) as found FROM cat WHERE table_name='".strtoupper($table)."'");      
      return ($found[0]['found']==0 ? false : true);
    }    
    
    /** 
     * Retrieve this drivers' specific SQL implementation of a now() function.
     * @return String
     */
    function func_now()
    {
      return "SYSDATE";
    }  
    
    /**
      * Retrieve this drivers' specific SQL implementation of a substring
      * function.
      *
      * Note that this function does not actually perform a substring
      * operation. It just returns a piece of valid SQL code that can be used
      * in a query.
      *
      * @param String $fieldname The name of the field to perform a substring
      *                          operation on.
      * @param int $startat The character at which to start.
      * @param int $length The number of charachters to retrieve.
      */
    function func_substring($fieldname, $startat=0, $length=0)
    {
      return "SUBSTR($fieldname, $startat".($length!=0?", $length":"").")";
    }
    
    /**
      * Returns the maximum length an identifier (tablename, columnname, etc)
      * for this driver may have.
      * @return int
      */
    function maxIdentifierLength()
    {
      return 30;
    }
    
    /**
     * Escape a string for use in SQL queries.
     *
     * This driver's implementation escapes single quotes and optionally,
     * wildcards. Wildcards should be escaped if they appear as regular 
     * characters inside a string. When actually searching for wildcards, 
     * those wildcards should not be escaped.
     * 
     * @param String $string The string to escape.
     * @param boolean $wildcard Set to true to convert wildcard chars ('%'). 
     *                          False will leave them unescaped.
     * @return String A SQL compatible version of the input string.
     
     */
    function escapeSQL($string, $wildcard=false)
    {    
      $result = str_replace("'","''",$string);
      if ($wildcard == true) $result = str_replace("%","%%",$result);
      return $result;
    }
    
  }

?>