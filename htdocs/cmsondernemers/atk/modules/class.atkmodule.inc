<?php

  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be 
   * included in the distribution.
   *
   * @package atk
   * @subpackage modules
   *
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 5.1 $
   * $Id: class.atkmodule.inc,v 5.1 2004/05/31 22:11:11 ivo Exp $
   */
   
  /**
   * The atkModule abstract base class.
   *
   * All modules in an ATK application should derive from this class, and
   * override the methods of this abstract class as they see fit.
   *
   * @author Peter C. Verhage <peter@ibuildings.nl>
   * @package atk
   * @subpackage modules
   * @abstract
   */
  class atkModule
  {
    var $m_name;

    /**
     * Constructor. The module needs to register it's nodes
     * overhere, create its menuitems etc.
     * @param String $name The name of the module.
     */
    function atkModule($name="")
    {
      $this->m_name = $name;
    }

    /**
     * Register nodes with their supported actions. Can be used
     * for security etc.
     */
    function getNodes()
    {
    }

    /**
     * Returns an array with filenames of attributes that need to be included
     * in order to let this module work properly.
     * @return array with attribute filenames
     */
    function getAttributes()
    {
    }

    /**
     * This method returns an array with menu items that need to be available
     * in the main ATK menu. This function returns the array created with
     * the menuitem() method, and does not have to be extended!
     * @return array with menu items for this module
     */
    function getMenuItems()
    {
    }

    /**
     * Function that adds a menuitem to the menu. This function can be called
     * from inside the getMenuItems() function, to register a menuitem.
     */
    function menuitem($name="", $url="", $parent="main", $enable=1, $order=0)
    {
      /* call basic menuitem */
      menuitem($name, $url, $parent, $enable, $order, $this->m_name);
    }

    /**
     * This method can be used to return an array similar to the menu array
     * but with links to (a) preference(s) page(s) for this module. The items
     * that will be returned have to be added with the preferencelink() method.
     * @return array with preference links for this module
     */
    function getPreferenceLinks()
    {
    }

    /**
     * This method is similar to the getPreferenceLinks() method but instead
     * will return links to (an) admin page(s) for this module. The array which
     * will be returned have to be created with the adminlink() method.
     * @return array with admin links for thid module
     */
    function getAdminLinks()
    {
    }

    /**
     * Returns the node overloader if it exists. Else it
     * will just return the module/node name of the given node.
     * @param $node module/node string
     * @return (overloader) module/node string
     */
    function &nodeOverloader($node)
    {
      global $g_modules, $config_atkroot, $g_overloaders;

      /* overloader check */
      if (!empty($g_overloaders[$node]))
      {
        atkdebug("Using overloader '".$g_overloaders[$node]."' for class '".$node."'");
        return newNode($g_overloaders[$node], FALSE);
      }

      /* no overloader */
      else return NULL;
    }

    /**
     * Construct a new node. A module can override this method for it's own nodes.
     * @param $node the node type
     * @return new node object
     */
    function &newNode($node)
    {
      global $g_modules, $config_atkroot, $g_overloaders;

      /* check for file */
      $file = nodeFile($node);
      if (!file_exists($file))
      {        
        atkimport("atk.utils.atkclassloader");
        $res = atkClassLoader::invokeFromString(atkconfig("missing_class_handler"), array("node"=>$node));
        if ($res!==false)
        {
          return $res;
        }
        else
        {
          atkerror("Cannot create node, because a required file ($file) does not exist!", "critical");
          return NULL;
        }
      }

      /* include file */
      include_once($file);

      /* module */
      $module = getNodeModule($node);

      /* now that we have included the node source file, we check
       * for overloaders (because overloaders might need the included file!)
       */
      $overloader = &$this->nodeOverloader($node);
      if ($overloader != NULL)
      {
        $overloader->m_module = $module;
        return $overloader;
      }

      /* initialize node and return */
      $type = getNodeType($node);
      $node = new $type();
      $node->m_module = $module;
      return $node;
    }
  }
?>