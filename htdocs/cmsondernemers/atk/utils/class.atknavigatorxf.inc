<?php
include_once("class.atknavigator.inc");

class atkNavigatorXF extends atkNavigator
{
	function buildIndex(&$node, $selected="") {
		$nav = '';
// You decide which 'letters' should be 'available'
if ( $node->m_available_letters != "" ) {
     $available_letters = split(";", $node->m_available_letters);
} else {
      $available_letters = array('A','B','C','D','E','F','G','H','I','J','K','L','M',
                                 'N','O','P','Q','R','S','T','U','V','W','X','Y','Z');
}

      $count = count($available_letters);
      for ($i=0;$i<$count;$i++)
      {
        $char = $available_letters[$i];
        if (strtoupper($selected[0])==$char) $label="<b>$char</b>";
        else $label=$char;

        // We use wildcard search for the index. Should be supported by any database.

        $nav.=href($_SERVER["PHP_SELF"]."?atknodetype=".$node->atkNodeType()."&atkaction=".$node->m_action."&atksearch[".$node->m_index."]=".$char."*&atksearchmode[".$node->m_index."]=wildcard&atkstartat=0",$label);

        // uncomment the following line if 26 letters seems to become too wide.
        //if ($count>13 && ($i+1)==floor($count/2)) $nav.='<br>'; else
        if ($i!=$count-1) $nav.=" ";
      }

		return $nav;
	}
}
?>