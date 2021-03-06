<?php
  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be 
   * included in the distribution.
   *
   * This file is the skeleton main include file, which you can copy
   * to your application dir and modify if necessary. By default, it 
   * includes the atknode and sets the $config_atkroot. Any other
   * application global includes should be added to this file.
   *
   * @package atk
   * @subpackage skel
   *
   * @author Ivo Jansch <ivo@achievo.org>
   *
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 5.3 $
   * $Id: atk.inc,v 5.3 2005/01/12 08:19:10 peter Exp $
   */
   
  /**
   * @internal This variable indicates where to get all the includes. Usually,
   *           this is the current pc.
   */
  if ($config_atkroot =="")  $config_atkroot = "./";      
  
  // Atknode is required. Don't remove it.
  include_once($config_atkroot."atk/class.atknode.inc");
  
  // This one may be removed, but why would you wanna do that? :)
  include_once($config_atkroot."atk/class.atkmetanode.inc");  
?>