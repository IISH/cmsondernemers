<?php

  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be 
   * included in the distribution.
   *
   * @package atk
   * @subpackage security
   *
   * @copyright (c)2000-2004 Ivo Jansch
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 5.4 $
   * $Id: class.atksecuritymanager.inc,v 5.4 2004/10/14 13:13:00 mark Exp $
   */
   
  /** 
   * @internal includes and definitions
   */
  require_once($config_atkroot."atk/security/class.auth_interface.inc");
  
  // initialise g_ array.
  $g_user = array();
  
  if ($config_authentication == "") $config_authentication = "none";
  if ($config_authorization  == "") $config_authorization = $config_authentication;
  if ($config_securityscheme == "") $config_securityscheme = "none";
  
  // Some result defines
  define ("AUTH_UNVERIFIED", 0); // initial value.
  define ("AUTH_SUCCESS", 1);
  define ("AUTH_LOCKED", 2);
  define ("AUTH_MISMATCH", 3);
  define ("AUTH_ERROR",-1);
   
  $authentication = explode(",",trim($config_authentication));
  if(is_array($authentication))
  {
    foreach($authentication as $value)
    {
      $value = trim($value);
      if(is_file($config_atkroot."atk/security/class.auth_".$value.".inc"))
      {
        include_once($config_atkroot."atk/security/class.auth_".$value.".inc");
      }
    }
  }
  else
  {
    if(is_file($config_atkroot."atk/security/class.auth_".$authentication.".inc"))
    {
      include_once($config_atkroot."atk/security/class.auth_".$authentication.".inc");
    }
  }

  include_once($config_atkroot."atk/security/class.auth_".$config_authorization.".inc");
   
  /**
   * The security manager for ATK applications.
   * 
   * Don't instantiate this class yourself, use the global instance 
   * that is returned by atkGetSecurityManager() instead.
   *
   * @todo Make a real singleton with a getInstance class.
   *
   * @author Ivo Jansch <ivo@achievo.org>
   * @package atk
   * @subpackage security
   */
  class atkSecurityManager
  {
    var $m_authentication = "";    
    var $m_authorization = 0; 
    var $m_scheme = "none";
    var $m_user = array();
    
    // If login really fails (no relogin box, but an errormessage), the 
    // error message that caused the fatal error is put in this variable.
    var $m_fatalError = "";
             
    /**
     * Constructor
     * @access private
     */
    function atkSecurityManager($authentication_type="none", $authorization_type="none", $securityscheme="none")
    {       
     
      atkdebug("creating securityManager (authenticationtype: $authentication_type, authorizationtype: $authorization_type, scheme: $securityscheme)");

      /* authentication class */       
      $authentication = explode(",",trim($authentication_type));
      if(is_array($authentication))
      {
        foreach($authentication as $class)
        {
          $class = trim($class);
          if($class!="")
          {
            $classname = "auth_".$class;
            if(class_exists($classname))
            {
              atkdebug("atkSecurityManager() constructing class $classname");
              $this->m_authentication[$class] = new $classname;
            }
          }
        }
      }
      else
      {
        $classname = "auth_".$authentication;
        if(class_exists($classname))
        {
          atkdebug("atkSecurityManager() constructing class $classname");
          $this->m_authentication[$authentication] = new $classname;
        }
      }          
      
      /* authorization class */    
      $classname = "auth_".$authorization_type; 
      $this->m_authorization = new $classname;
       
      /* security scheme */
      $this->m_scheme = $securityscheme;
    }
     
    /**
     * Perform user authentication. 
     *
     * Called by the framework, it should not be necessary to call this method
     * directly.
     */
    function authenticate()
    {    
      global $g_sessionManager, $ATK_VARS;      

      $session = &atkSessionManager::getSession();

      $response=AUTH_UNVERIFIED;

// dirty manier om de atk login te omzeilen
// de login controle doen we via dispatch.php (check login)
if (atkconfig("authentication")=="none") {
	$response=AUTH_SUCCESS;
	$auth_user = "none";
	$auth_pw = "none";
	$session["login"] = "1";
	$g_sessionManager->globalVar("authentication",array("authenticated"=>1, "user"=>"none"), true);
} else {

      if (atkconfig("auth_loginform")==true) // form login
      {
        $auth_user = $ATK_VARS["auth_user"];
        $auth_pw = $ATK_VARS["auth_pw"];                
      }
      else // HTTP login
      {
        $auth_user = $_SERVER["PHP_AUTH_USER"];
        $auth_pw = $_SERVER["PHP_AUTH_PW"];
      }      

}

      $md5 = false; // PHP_AUTH_PW is plain text..            
      // first check if we want to logout
      if (isset($ATK_VARS["atklogout"]) && $session["relogin"]!=1)
      {        
        session_destroy();
        $cookie_params = session_get_cookie_params();
        session_set_cookie_params($cookie_params["lifetime"], dirname($_SERVER["SCRIPT_NAME"]));
        session_start();
        $session["relogin"]=1;

        // destroy cookie
        if (atkconfig("authentication_cookie") && $auth_user!="administrator")
        {
          $cookiename = "atkauth_".str_replace(" ","_",text("app_title"));
          if (!empty($_COOKIE[$cookiename])) setcookie($cookiename, "", 0);
        }
        if($ATK_VARS["atklogout"]>1)
        {
          header("Location: logout.php");
          exit;
        }
      } 
 
      // do we need to login?
      else if ($session["login"]!=1)
      {         
        // authenticated?
        $authenticated = false;

        // sometimes we manually have to set the PHP_AUTH vars 
        if (empty($auth_user) && empty($auth_pw) && ereg("^Basic ", $_SERVER["HTTP_AUTHORIZATION"]))
        list($auth_user, $auth_pw) = explode(":", base64_decode(substr($_SERVER["HTTP_AUTHORIZATION"], 6)));

        // check previous sessions.. 
        if (atkconfig("authentication_cookie"))
        {          
          // Cookiename is based on the app_title, for there may be more than 1 atk app running,
          // each with their own cookie..
          $cookiename = "atkauth_".str_replace(" ","_",text("app_title"));          
          list($enc, $user, $passwd) = split("\.",base64_decode($_COOKIE[$cookiename]));          
                     
          // for security reasons administrator will never be cookied..
          if ($auth_user=="" && $user!="" && $user!="administrator")
          {        
            atkdebug("Using cookie to retrieve previously used userid/password");          
            $auth_user = $user;
            $auth_pw = $passwd;
            $md5 = ($enc=="MD5"); // cookie may already be md5;
          }
        }              
        
        if ($auth_user=="")
        {
          // Can't verify a login, because we don't have a username yet.
          $authenticated = false;
          $response = AUTH_UNVERIFIED;
        }
        else
        {
          // We have a username, which we must now validate against several 
          // checks. If all of these fail, we have a status of AUTH_MISMATCH.
          $response = AUTH_MISMATCH;
          $authenticated = false;
          
          // check superuser       
          if (atkconfig("administratorpassword")!=""&&$auth_user=="administrator"&&$auth_pw==atkconfig("administratorpassword"))
          {
            $this->m_user = array("name"=>"administrator","level"=>-1, "access_level"=>9999999);                    
            // for convenience, we also store the user as a global variable.                     
            atkdebug("Administrator (configfile) logged in");
            $authenticated = true;          
          } 
          // check guest user
          else if (atkconfig("guestpassword")!=""&&$auth_user=="guest"&&$auth_pw==atkconfig("guestpassword"))
          {
            $this->m_user = Array("name"=>"guest","level"=>-2, "access_level"=>0);
            // for convenience, we also store the user as a global variable.                     
            atkdebug("Guest (configfile) logged in");
            $authenticated = true;
          }
          // other users 
          // we must first explicitly check that we are not trying to login as administrator or guest.
          // these accounts have been validated above. If we don't check this, an account could be
          // created in the database that provides administrator access.        
          else if ($auth_user!=""&&$auth_user!="administrator"&&$auth_user!="guest") 
          {               
            if(is_array($this->m_authentication))
            {
              foreach($this->m_authentication as $name => $obj)
              {
                $obj->canMd5()&&!$md5 ? $tmp_pw=md5($auth_pw) : $tmp_pw=$auth_pw;
                $response = $obj->validateUser($auth_user,$tmp_pw);
                if($response==AUTH_SUCCESS)
                {
                  atkdebug("atkSecurityManager::authenticate() using $name authentication");
                  $authname=$name;
                  break;
                }
              }
            }          
            if($response==AUTH_SUCCESS) // succesful login
            {   
              // We store the username + securitylevel of the logged in user. 
              $this->m_user = $this->m_authorization->getUser($auth_user);
              $this->m_user['AUTH']=$authname; // something to see wich auth scheme is used
              $this->m_user['PASS']=$auth_pw; // used by aktsecurerelation to decrypt an linkpass
              // for convenience, we also store the user as a global variable.
              (is_array($this->m_user['level'])) ? $dbg = implode(",",$this->m_user['level']) : $dbg = $this->m_user['level'];
              atkdebug("Logged in user: ".$this->m_user["name"]." (level: ".$dbg.")");
              $authenticated = true;
  
              // Remember that we are logged in..
              //$g_sessionManager->globalVar("authentication",array("authenticated"=>1, "user"=>$this->m_user), true);
  
              // write cookie            
              if (atkconfig("authentication_cookie")&&$auth_user!="administrator")
              {                           
                // if the authentication scheme supports md5 passwords, we can safely store
                // the password as md5 in the cookie.
                if ($md5) // Password is already md5 encoded
                {
                  $tmppw = $auth_pw;
                  $enc = "MD5";
                }
                else // password is not md5 encoded
                {
                  if ($this->m_authentication[$authname]->canMd5()) // we can encode it
                  {
                    $tmppw = md5($auth_pw);
                    $enc = "MD5";
                  }
                  else // authentication scheme does not support md5 encoding.
                       // our only choice is to store the password plain text 
                       // :-(
                       // NOTE: If you use a non-md5 enabled authentication 
                       // scheme then, for security reasons, you shouldn't use 
                       // $config_authentication_cookie at all.
                  {
                    $tmppw = $auth_pw;
                    $enc = "PLAIN";
                  }
                }
                setcookie($cookiename,base64_encode($enc.".".$auth_user.".".$tmppw), time()+60*(atkconfig("authentication_cookie_expire")));
              }
            }
            else
            {
              // login was incorrect. Either the supplied username/password combination is 
              // incorrect (we just try again) or there was an error (we display an error 
              // message)
              if ($response==AUTH_ERROR)
              {
                $this->m_fatalError = $this->m_authentication->m_fatalError;
              }
              $authenticated = false;            
            }
          }
          // we are logged in 
          if ($authenticated) $session["login"]=1;
        }        
      }      
      else
      {
        // using session for authentication, because "login" was registered.    
        // but we double check with some more data from the session to see
        // if the login is really valid.
        $session_auth = $g_sessionManager->getValue("authentication", "globals");          
                
        if (atkconfig("authentication_session") && 
            $session["login"]==1 && 
            $session_auth["authenticated"]==1 && 
            !empty($session_auth["user"]))
        {
          $this->m_user = $session_auth["user"];
          atkdebug("Using session for authentication / user = ".$this->m_user["name"]);
          //atk_var_dump($this->m_user);
        }
        else
        {
          // Invalid session
          $authenticated = false;          
        }
      }

      // if there was an error, drop out.
      if ($this->m_fatalError!="") 
      {        
        return false;
      }      
      // still not logged in?! 
      if ($session["login"]!=1)
      {
        if(atkconfig("auth_loginform"))
        {
          $this->loginForm($auth_user, $response);
          $output = &atkOutput::getInstance();
          $output->outputFlush();
          exit();
        }
        else
        {
          header('WWW-Authenticate: Basic realm="'.text("app_title").' - '.strftime("%c",time()).'"');
          if (ereg("Microsoft", $SERVER_SOFTWARE)) header("Status: 401 Unauthorized");
          else header("HTTP/1.0 401 Unauthorized");
          return false;
        }
      }

      // we are authenticated, but atklogout is still active, let's get rid of it!
      else if (isset($ATK_VARS["atklogout"])) header("Location: ".$_SERVER["PHP_SELF"]."?");

      // we keep the relogin state until the atklogout variable isn't set anymore
      else if (!isset($ATK_VARS["atklogout"]) && $session["relogin"]==1) $session["relogin"]=0;

      // return
      // g_user always lowercase                  
      $this->m_user["name"] =  strtolower($this->m_user["name"]);
      //Send the username with the header
      //This way we can always retrieve the user from apache logs
      header('user: '.$this->m_user["name"]);            
      $GLOBALS["g_user"] = $this->m_user;
      $g_sessionManager->globalVar("authentication",array("authenticated"=>1, "user"=>$this->m_user), true);
      sessionStore('loginattempts',''); //reset maxloginattempts
      return true;
    }     
    
    /**
     * Reload the current user data.
     * This method should be called if userdata, for example name or other 
     * fields, have been updated for the currently logged in user.
     * 
     * The method will make sure that $g_securityManager->m_user, $g_user and 
     * the authenticated user in the session are refreshed.
     */
    function reloadUser()
    {
      global $g_user, $g_sessionManager;
      $user = getUser();
      $this->m_user = $this->m_authorization->getUser($user["userid"]);            
      $g_user = &$this->m_user;      
      $old_auth = $g_sessionManager->getValue("authentication", "globals");          
      $old_auth["user"] = $g_user;
      $g_sessionManager->globalVar("authentication",$old_auth, true);
    }
          
    /**
     * Display a login form.
     * @access private
     */
    function loginForm($defaultname, $lastresponse)
    {      

      $loginattempts = sessionLoad('loginattempts');            
      if($loginattempts=="") $loginattempts = 0;
      
      if($loginattempts=="") 
      {        
        $loginattempts = 1;
      }
      else
      {     
        $loginattempts++;      
      }

      sessionStore('loginattempts',$loginattempts);      

      atkdebug('LoginAttempts: '.$loginattempts);
      
      $page = &atknew("atk.ui.atkpagexf");            
      $ui = &atknew("atk.ui.atkui");

      atkimport("atk.ui.atktheme");
      $theme = &atkTheme::getInstance();
      $page->register_style($theme->stylePath("style.css"));
      $page->register_style($theme->stylePath("login.css"));

      $page->register_script(atkConfig('atkroot')."atk/javascript/formfocus.js");      
            
      $tplvars = Array();      
      $output.='<form action="'.$_SERVER["PHP_SELF"].'" method="post">';
      $output.=makeHiddenPostVars(array("atklogout", "loginattempts"));       
      $output.='<br><br><table border=0 cellspacing=2 cellpadding=0 align=center>';                     

      $tplvars["atksessionformvars"] = makeHiddenPostVars(array("atklogout", "loginattempts"));
      $tplvars["formurl"] = $_SERVER["PHP_SELF"];

      // max_loginattempts of 0 means no maximum.
      if(atkconfig('max_loginattempts')>0 && $loginattempts>atkconfig('max_loginattempts'))
      {        
        $output.="<tr><td class=table>".text('auth_max_loginattempts_exceeded')."<br><br></td></tr>";
        $tplvars["auth_max_loginattempts_exceeded"] = text('auth_max_loginattempts_exceeded');
      }      
      else
      {
        // generate the username input field
        // based upon the config_authdropdown and auth. method
        $userField = $this->auth_userField($defaultname);
        $tplvars["username"] = text("username");
        $tplvars["password"] = text("password");
        $tplvars["userfield"] = $userField;
        $tplvars["passwordfield"] = '<input class="loginform" type="password" size="15" name="auth_pw" value="">';
        $tplvars["submitbutton"] = '<input class="loginform" type="submit" value="'.text('login').'">';
        $tplvars["title"] = text('login_form');
        
        if ($lastresponse == AUTH_LOCKED)
        {        
          $output.="<tr><td colspan=2 class=error>".text('auth_account_locked')."<br><br></td></tr>";
          $tplvars["auth_account_locked"] = text('auth_account_locked');
        }
        else if($lastresponse == AUTH_MISMATCH)
        {
          $output.='<tr><td colspan=2 class=error>'.text('auth_mismatch').'<br><br></td></tr>';
          $tplvars["auth_mismatch"] = text('auth_mismatch');
        }

        // generate the form
        $output.="<tr><td valign=top>".text('username')."</td><td>:</td><td>".$userField."</td></tr>";   
        $output.="<tr><td colspan=2 height=6></td></tr>";
        $output.="<tr><td valign=top>".text('password')."</td><td>:</td><td><input type=password size=15 name=auth_pw value=''></td></tr>";
        $output.="<tr><td colspan=2 align=center height=50 valign=middle><input type=submit value=".text('login')."></td></tr>";      
                       
      }
      
      $output.='</table></form>';

      $output.='<SCRIPT type="text/javascript">placeFocus()</SCRIPT>';
      
      $tplvars["content"]=$output;
      $page->addContent($ui->render("login.tpl", $tplvars));       
      $o = &atkOutput::getInstance();
      $o->output($page->render(text("login_form")));
    }

    /**
     * Generate field for entering the username (dropdown or input box,
     * depending on settings.
     * @access private
     */
    function auth_userField($defaultname)
    {            
      if (atkconfig("auth_dropdown") == true)
      {
        $auth_types = $this->m_authentication;
        $userlist = array();
        
        // Administrator and guest user may be present.
        if (atkconfig("administratorpassword")!="")
        {
          $userlist[] = array("userid"=>"administrator", "username"=>"Administrator");
        }
        if (atkconfig("guestpassword")!="")
        {
          $userlist[] = array("userid"=>"guest", "username"=>"Guest");
        }
        
        foreach ($auth_types as $type => $obj)
        {
          $userlist = array_merge($userlist, $obj->getUserList());
        }
                
        $userField  = '<select name="auth_user">'."\n";
              
        for ($i=0,$_i=count($userlist);$i<$_i;$i++)
        {
          $selected = "";
          if (trim(strtolower($defaultname)) == strtolower(trim($userlist[$i]["userid"])))
          {
            $selected = " selected";
          }
          $userField .= "<option value='".$userlist[$i]["userid"]."'".$selected.">".$userlist[$i]["username"]."</option>\n";
        }
        $userField .= "</SELECT>\n";
     
        return $userField;
      }
      else
      {
        return '<input type="text" size="15" name="auth_user" value="'.$defaultname.'">';
      }
    }

    /**
     * Check if the currently logged-in user has a certain privilege on a
     * node.
     * @param String $node The full nodename of the node for which to check
     *                     access privileges. (modulename.nodename notation).
     * @param String $action The privilege to check. 
     * @return boolean True if the user has the privilege, false if not.
     */
    function allowed($node, $privilege)
    {
      static $_cache = array();
      
      if (!isset($_cache[$node][$privilege]))
      {
        // security disabled or user is superuser? (may do anything)
        if (($this->m_scheme=="none") || ($this->hasLevel(-1)) || (strtolower($this->m_user["name"])=="administrator")) 
        {        
          $allowed = true;
        }
        // user is guest? (guests may do nothing)
        else if (($this->hasLevel(-2)) || (strtolower($this->m_user["name"])=="guest"))
        {
          $allowed = false;
        }
        // all other situations
        else
        {
         
          $required = $this->m_authorization->getEntity($node, $privilege);       
         
          if (count($required)==0)
          {
            // No access restrictions found.. 
            // so either nobody or anybody can perform this
            // operation, depending on the configuration.
            $allowed = !atkconfig("restrictive");
          }
          else
          {
            if ($this->m_scheme=="level")
            {
              // in level based security, only one level is specified for each node/action combination.
              $allowed = ($this->m_user["level"] >= $required[0]);
            }
            else if ($this->m_scheme=="group")
            {           
              // user may have more then one level
              if (is_array($this->m_user["level"]))
              {
                $allowed = (count(array_intersect($this->m_user["level"], $required))>0);
              }
              else
              {
                // user has only one level
                $allowed = in_array($this->m_user["level"], $required);          
              }
              
            }
            else // unknown scheme??
            {
              $allowed = false;
            }
          }
        }  
        
        $_cache[$node][$privilege] = $allowed;
      }        
      
      return $_cache[$node][$privilege];
    }
        
    /**
     * Check if the currently logged-in user has the right to view or edit
     * an attribute of a node.
     * @param String $node The full nodename of the node for which to check
     *                     access.
     * @param String $attribute The name of the attribute to check.
     * @param String $mode "edit" or "view"
     * @return boolean True if access is granted, false if not.
     */
    function attribAllowed($node, $attribute, $mode)
    {      
      // No securityscheme, or user is superuser.
      if (($this->m_scheme=="none") || (!atkconfig("security_attributes")) || ($this->hasLevel(-1)) || (strtolower($this->m_user["name"])=="administrator")) 
      {        
        return true;
      }
      
      // User is guest (may do nothing)
      if (($this->hasLevel(-2)) || (strtolower($this->m_user["name"])=="administrator")) 
      {        
        return false;
      }
      
      // all other situations
      $required = $this->m_authorization->getAttribEntity($node, $attribute, $mode);
       
      if ($required==-1)
      {
        // No access restrictions found..          
        $allowed = true;
      }
      else
      {
        if ($this->m_scheme=="level")
        {
          $allowed = ($this->m_user["level"] >= $required);
        }
        else if ($this->m_scheme=="group")
        {
          $allowed = in_array($this->m_user["level"],$required);
        }
        else // unknown scheme??
        {
          $allowed = false;
        }
      }
     
      return $allowed;
     
    }
    
    /**
     * Check if the currently logged in user has the requested level.
     * @param int $level The level to check.
     * @return boolean True if the user has the required level, false if not.
     */
    function hasLevel($level)
    {
      if (is_array($level))
      {
        if (is_array($this->m_user["level"]))
        {
          return (count(array_intersect($this->m_user["level"], $level)) >= 1);
        }
        else
        {
          return in_array($this->m_user["level"], $level);
        }     
      }
      else
      {
        if (is_array($this->m_user["level"]))
        {
          return in_array($level, $this->m_user["level"]);
        }
        else
        {
          return $this->m_user["level"]==$level;
        }
      }
    }
     
    /**
     * Write an access entry in the logfile.
     * @param String $node The full name of the node that is being accessed.
     * @param String $action The action that has been performed.
     */
    function logAction($node, $action)
    {
      $this->log(2,"Performing $node.$action");
    }
     
    /**
     * Write a logentry in the logfile.
     * The entry is only written to the file, if the level of the message is
     * equal or higher than the setting of $config_logging.
     *
     * @todo Logging should be moved to a separate atkLogger class.
     * @param int $level The loglevel.
     * @param String $message The message to log.
     */
    function log($level, $message)
    {      
    
      if (atkconfig("logging")>0 && atkconfig("logging")>=$level)
      {
        $fp = @fopen(atkconfig("logfile"),"a");
        if ($fp)
        {
          $logstamp = "[".date("d-m-Y H:i:s")."] [".$_SERVER["REMOTE_ADDR"]."] ".$this->m_user["name"]." | ";
          @fwrite($fp, $logstamp.$message."\n");
          @fclose($fp);
        }
        else
        {
          atkdebug("error opening logfile");
        }
      }
    }     
  }
  
  /**
   * Calling this function will invoke the login process. Call this function in 
   * every file that you want to have secured.
   * (This is actually a small wrapper for $securityManager->authenticate(), 
   * so you can quickly secure an application.
   */
  function atksecure()
  {
    $securityMgr = &atkGetSecurityManager();
    
    if (!$securityMgr->authenticate())
    {
      echo '<b>'.text("login_failed").'</b>';      
      echo '<br><br>'.$securityMgr->m_fatalError;
      exit;
    }
  }      
  
  /**
   * Retrieve all known information about the currently logged-in user.
   * @return array Array with userinfo, or "" if no user is logged in.
   */
  function getUser()
  {
    global $g_sessionManager;
    $session = &atkSessionManager::getSession();
    $user = "";    
    $session_auth = $g_sessionManager->getValue("authentication", "globals");          
    if (atkconfig("authentication_session") && 
        $session["login"]==1 && 
        $session_auth["authenticated"]==1 && 
        !empty($session_auth["user"]))
    {
      $user = $session_auth["user"];
      if($user["access_level"]=="") $user["access_level"]=0;
    }

    return $user;
  }
    
  /**
   * Wrapper method to access the security manager.
   * @todo Rewrite to a getInstance method in atkSecurityManager.
   */
  function &atkGetSecurityManager()
  {
    global $g_securityManager; // WORKAROUND: we use a global var since many 
                               // atk apps rely on a global variable being present.
                               // In ATK5 we must create a clean singleton with
                               // a static instance.
    if (!is_object($g_securityManager))
    {
      // The one and only security manager.   
      $g_securityManager = new atkSecurityManager(atkconfig("authentication"), atkconfig("authorization"), atkconfig("securityscheme"));   
    }
    return $g_securityManager;
  }
  
  /**
   * Check if the currently logged-in user has a certain privilege.
   *
   * @deprecated Use the allowed method of the security manager instead.
   * @param String $node The full name of the node for which to check access.
   * @param String $privilege The privilege to check.
   * @return boolean True if access is granted, false if not.
   */
  function is_allowed($node, $privilege)
  {
    $secMgr = &atkGetSecurityManager();
    if (!is_object($secMgr)) return 1;
    else return $secMgr->allowed($node, $privilege);
  }
     
?>
