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
	 * @license http://www.achievo.org/atk/licensing ATK Open Source License
	 *
	 * @version $Revision: 5.1 $
	 * $Id: class.auth_iisg.inc,v 5.1 2004/05/25 06:32:27 ivo Exp $
	 */
	 
	/**
	 * Dummy driver for non-authentication. When using 'iisg' as authentication
	 * method, any loginname and any password will be accepted.
	 *
	 * @author Ivo Jansch <ivo@achievo.org>
	 * @package atk
	 * @subpackage security
	 *
	 */	
	 class auth_iisg extends auth_interface
	 {
		 function validateUser($user, $passwd)
		 {
			 return AUTH_SUCCESS;
		 }					
		 
		 function canMd5()
		 {
			 return false;
		 }
	 }

?>