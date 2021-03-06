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
   * @version $Revision: 5.5 $
   * $Id: class.atkdb.inc,v 5.5 2005/01/13 20:40:36 peter Exp $
   */

  /**
   * Some defines used for connection statusses, generic error messages, etc.
   */
  define("DB_SUCCESS"           , 0);
  define("DB_UNKNOWNERROR"      , 1);
  define("DB_UNKNOWNHOST"       , 2);
  define("DB_UNKNOWNDATABASE"   , 3);  
  define("DB_ACCESSDENIED_USER" , 4);
  define("DB_ACCESSDENIED_DB"   , 5);
  
  /**
   * Meta flags.
   */
  define('MF_PRIMARY',        1);
  define('MF_UNIQUE',         2);
  define('MF_NOT_NULL',       4);
  define('MF_AUTO_INCREMENT', 8);  

  /**
   * Abstract baseclass for ATK database drivers. 
   *
   * Implements some custom functionality and defines some methods that
   * derived classes should override.
   *
   * @author Peter Verhage <peter@achievo.org>
   * @author Ivo Jansch <ivo@achievo.org>
   * @package atk
   * @subpackage db
   *
   */
  class atkDb
  {
    /**
     * The hostname/ip to connect to.
     * @access private
     * @var String
     */
    var $m_host     = "";
    
    /** 
     * The name of the database/schema to use.
     * @access private
     * @var String
     */
    var $m_database = "";
    
    /** 
     * The username for the connection.
     * @access private 
     * @var String
     */
    var $m_user     = "";
    
    /**
     * The password for the connection.
     * @access private
     * @var String
     */
    var $m_password = "";

    /**
     * Contains the current record from the result set.
     * @access private 
     * @var array
     */
    var $m_record = array();
    
    /**
     * Current row number
     * @access private 
     * @var int
     */
    var $m_row = 0;

    /**
     * Contains error number, in case an error occurred.
     * @access private
     * @var int
     */
    var $m_errno = 0;
    
    /** 
     * Contains textual error message, in case an error occurred.
     * @access private 
     * @var String
     */
    var $m_error = "";
    
    /**
     * If true, an atkerror is raised when an error occurred.
     *
     * The calling script can use this to stop execution and rollback.
     * If false, the error will be ignored and script execution 
     * continues. Use this only for queries that may fail but still
     * be valid.
     * @access private
     * @var boolean
     */
    var $m_haltonerror = true;
    
    /**
     * Driver name.
     * 
     * Derived classes should add their own m_type var to the class
     * definition and put the correct name in it. (e.g. "mysql" etc.)
     * @abstract
     * @access private 
     * @var String
     */
    var $m_type = "";
    
    /** 
     * number of affected rows after an update/delete/insert query 
     * @access private
     * @var int
     */
    var $m_affected_rows = 0;      
    
    /**
     * array to cache meta-information about tables.
     * @access private
     * @var array
     */
    var $m_tableMeta = array();

    /**
     * The connection is stored in this variable.
     * @access private
     * @var Resource
     */
    var $m_link_id  = 0;
    
    /**
     * The query statement is stored in this variable.
     * @access private
     * @var Resource
     */
    var $m_query_id = 0;

    /**
     * Auto free result upon next query.
     *
     * When set to true, the previous results are cleared when a new query is
     * executed. It should generally not be necessary to put this to false.
     * @access private
     * @var boolean
     */    
    var $m_auto_free = true;
    
    /**
     * List of error codes that could be caused by an end-user.
     *
     * This type of errors is 'recoverable'. An example is a violation of a
     * unique constraint.
     * @access private
     * @var array
     */    
    var $m_user_error = array();
    
    /**
     * List of unrecoverable system errors.
     *
     * This type of errors is non-recoverable. An example would be a
     * connection failure or an internal server error.     
     * @access private
     * @var array
     */    
    var $m_critical_error = array();

    /**
     * Get the current connection.
     * @return Connection resource id
     */
    function link_id()
    {
      return $this->m_link_id;
    }
    
    /**
     * Determine whether an error that occurred is a recoverable (user) error
     * or a system error.
     * @return String "user" or "system"
     */
    function getErrorType()
    {
      if(in_array($this->m_errno,$this->m_user_error))
      {
        return "user";   
      }
      return "system";
    }
    
    /*
     * Get generic atk errorccode
     * @return int One of the ATK DB_* codes.
     */
    function getAtkDbErrno()
    {
      return $this->_translateError();
    }
    
    /**
     * Get vendor-dependent database error number.
     *
     * Applications should not rely on this method, if they want to be
     * database independent.
     * @return mixed Database dependent error code.
     */
    function getDbErrno()
    {
      return $this->m_errno;
    }

    /**
     * Get vendor-dependent database error message.
     *
     * Applications should not rely on this method, if they want to be
     * database independent.
     * @return String Database dependent error message.
     */
    function getDbError()
    {
      return $this->m_error;
    }
    
    /**
     * Define custom user error codes. 
     *
     * Error codes passed to this method will be treated as recoverable user
     * errors.
     * @param mixed Vendor-dependent database error code
     */
    function setUserError($errno)
    {
      atkdebug("atkdb::setUserError() -> ".$errno);
      $this->m_user_error[]=$errno;
    }
    
    /**
     * Get localized error message (for display in the application)
     * @return String Error message
     */
    function getErrorMsg()
    {
      $errno = $this->getAtkDbErrno(); 
      if($errno==DB_UNKNOWNERROR)
      {
        $errstr = 'db_'.$this->getDbErrno();
        if($GLOBALS[$errstr]=="") { $this->m_error = text('unknown_error').": ".$this->getDbErrno()." (".$this->getDbError().")"; } else { $this->m_error=text($errstr); }
      //  atkerror($this->m_error);

        return $this->m_error;
      }
      else
      {
        $tmp_error='';
        switch ($errno)
        {
          case DB_ACCESSDENIED_DB: $tmp_error = sprintf(text("db_access_denied_database"),$this->m_user,$this->m_database);
                                break;
          case DB_ACCESSDENIED_USER: $tmp_error = sprintf(text("db_access_denied_user"),$this->m_user,$this->m_database);
                                     break;
          case DB_UNKNOWNDATABASE: $tmp_error = sprintf(text("db_unknown_database"),$this->m_database);
                                   break;
          case DB_UNKNOWNHOST: $tmp_error = sprintf(text("db_unknown_host"),$this->m_host);
                               break;
        }
        $this->m_error = $tmp_error;
        return $this->m_error;
      }
    }
    
    /**
     * If haltonerror is set, this will raise an atkerror. If not, it will
     * place the error in atkdebug and continue.
     * @access protected     
     */
    function halt()
    {
      if ($this->m_haltonerror)
      {
        if($this->getErrorType()=="system")
        {
          atkdebug("atkdb::halt() on system error");
          if(in_array($this->m_errno,$this->m_critical_error)) $level='critical';
          halt($this->getErrorMsg(),$level);
        }
        else
        {
          atkdebug("atkdb::halt() on user error (not halting)");
        }
      }
    }

    /**
     * Get the current query statement resource id.
     * @return Resource Query statement resource id.
     */
    function query_id()
    {
      return $this->m_query_id;
    }
    
    /**
     * Connect to the database.
     * @return int Connection status
     * @abstract
     */
    function connect()
    {
      return DB_UNKNOWNERROR;
    }

    
    /**
     * Translate database-vendor dependent error messages into an ATK generic
     * error code.
     *
     * Derived classes should implement this method and translate their error
     * codes.
     * @access private
     * @param mixed Vendor-dependent error code.
     * @return int ATK error code
     */
    function _translateError($errno)
    {      
      return DB_UNKNOWNERROR;     
    }

    /**
     * Disconnect from database
     * @abstract
     */
    function disconnect()
    {
    }

    /**
     * Commit the current transaction.
     * @abstract
     */
    function commit()
    {
    }

    /**
     * Rollback the current transaction.
     * @abstract
     */
    function rollback()
    {
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
     * @abstract
     */
    function query($query, $offset=-1, $limit=-1)
    {
echo $query;
echo "++<P>";
      return true;    
    }

    /**
     * Retrieve the next record in the resultset.
     * @return mixed An array containing the record, or 0 if there are no more
     *               records to retrieve.
     * @abstract
     */
    function next_record()
    {
      return 0;
    }
 
    /**
     * Lock a table in the database.
     *
     * @param String $table The name of the table to lock.
     * @param String $mode The lock type. 
     * @return boolean True if succesful, false if not.
     * @abstract
     */
    function lock($table, $mode="write")
    {
      return 0;
    }
  
    /**
     * Relieve all locks. 
     *
     * @return boolean True if succesful, false if not.
     * @abstract
     */
    function unlock()
    {
      return 0;
    }

    /**
     * Retrieve the number of rows affected by the last query.
     *
     * After calling query() to perform an update statement, this method will
     * return the number of rows that was updated.
     *
     * @return int The number of affected rows
     * @abstract
     */
    function affected_rows()
    {
      return array();
    }    

    /**
     * Get the next sequence number of a certain sequence.
     *
     * If the sequence does not exist, it is created automatically. 
     *
     * @param $sequence The sequence name
     * @return int The next sequence value
     * @abstract
     */
    function nextid($sequence)
    {
    }

    /**
     * Return the meta data of a certain table HIE GEBLEVEN
     *
     * depending on $full, metadata returns the following values:
     *  -full is false (default):
     *   $result[]:
     *     [0]["table"]  table name
     *     [0]["name"]   field name
     *     [0]["type"]   field type
     *     [0]["len"]    field length
     *     [0]["flags"]  field flags
     * 
     *  -full is true:
     *   $result[]:
     *     ["num_fields"] number of metadata records
     *     [0]["table"]  table name
     *     [0]["name"]   field name
     *     [0]["type"]   field type
     *     [0]["len"]    field length
     *     [0]["flags"]  field flags
     *     ["meta"][field name] index of field named "field name"
     *     The last one is used, if you have a field name, but no index.
     *
     * @param $table the table name
     * @param $full all meta data or not
     * @return array with meta data    
     */
    function metadata($table, $full=false)
    {            
    
      return array();
    }   

    /**
     * Return the available table names
     * @return array with table names etc.
     */
    function table_names()
    {
      return array();
    }
  
   /**
     * This function checks the database for a table with 
     * the provide name
     *
     * @param String $tableName the table to find
     * @return boolean true if found, false if not found
     */  
    function tableExists($tableName)
    {
      return false;
    } 
  
    /**
     * Get all rows that are the result
     * of a certain specified query
     *
     * Note: This is not an efficient way to retrieve
     * records, as this will load all records into one
     * array into memory. If you retrieve a lot of records,
     * you might hit the memory_limit and your script will die.
     *
     * @param $query the query
     * @return array with rows
     */
    function getrows($query, $offset=-1, $limit=-1)
    {
      $result = array();

      $this->query($query, $offset, $limit);    
      while ($this->next_record())
        $result[] = $this->m_record;
      
      return $result;
    }
    
    /**
     * This function indicates what searchmodes the database supports.
     * @return array with search modes
     */
    function getSearchModes()
    {
      // exact match and substring search should be supported by any database.
      // (the LIKE function is ANSI standard SQL, and both substring and wildcard
      // searches can be implemented using LIKE)
      return array("exact","substring", "wildcard","greaterthan","greaterthanequal","lessthan","lessthanequal","between");
    }
    
    function tableMeta($table)
    {
      if (!is_array($this->m_tableMeta[$table]))
      {
        $meta = $this->metadata($table);
        $this->m_tableMeta[$table] = array();
        for ($i = 0, $_i = count($meta); $i < $_i; $i++)
        {
          $meta[$i]['num'] = $i;
          $this->m_tableMeta[$table][$meta[$i]['name']] = &$meta[$i];
        }
      }
      
      return $this->m_tableMeta[$table];
    }
    
    /**
      * get NOW() or SYSDATE() equivalent for the current database
      *
      * Every database has it's own implementation to get the current date
      *
      */
    function func_now()
    {
      return "NOW()";
    }
    
    /**
      * get SUBSTRING() equivalent for the current database.
      */
    function func_substring($fieldname, $startat=0, $length=0)
    {
      return "SUBSTRING($fieldname, $startat".($length!=0?", $length":"").")";
    }
    
    /** 
     * Get TO_CHAR() equivalent for the current database.
     *
     * TODO/FIXME: add format paramater. Current format is always yyyy-mm-dd.
     */
    function func_datetochar($fieldname)
    {
      return "TO_CHAR($fieldname, 'YYYY-MM-DD')";
    }
      
      
    /** 
     * Get TO_CHAR() equivalent for the current database.
     *
     * TODO/FIXME: add format paramater. Current format is always yyyy-mm-dd hh:mi.
     */
    function func_datetimetochar($fieldname)
    {
      return "TO_CHAR($fieldname, 'YYYY-MM-DD hh:mi')";
    }      
    
    /**
      * Returns the maximum length an identifier (tablename, columnname, etc) may have
      *      
      */
    function maxIdentifierLength()
    {
      return 64;
    }
       
    /**
     * escapes quotes for use in SQL: ' -> '' (and sometimes % -> %%)
     */
    function escapeSQL($string, $wildcard=false)
    {    
      $result = str_replace("'","''",$string);
      $result = str_replace("\\","\\\\",$result);
      if ($wildcard == true) $result = str_replace("%","%%",$result);
      return $result;
    }

    function &createQuery()
    {      
      $query = &atknew("atk.db.atk".$this->m_type."query");
      return $query;
    }

  }  

?>
