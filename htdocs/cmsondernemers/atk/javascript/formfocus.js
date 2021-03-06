  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be 
   * included in the distribution.
   *
   * @package atk
   * @subpackage javascript
   *
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 5.1 $
   * $Id: formfocus.js,v 5.1 2004/05/31 22:11:11 ivo Exp $
   */
   
function placeFocus()
{
  if (document.forms.length > 0) 
  {
    var field = document.forms[0];
    for (i = 0; i < field.length; i++) 
    {
      if ((field.elements[i].type == "text") || (field.elements[i].type =="textarea") || (field.elements[i].type.toString().charAt(0) == "s")) 
      {
        document.forms[0].elements[i].focus();
        break;
      }
    }
  }
}
