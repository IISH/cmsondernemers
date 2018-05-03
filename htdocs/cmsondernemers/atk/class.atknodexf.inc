<?php
include_once("class.atknode.inc");

	class atkNodeXF extends atkNode
	{
		var $m_available_letters;

		function atkNodeXF($name, $flags=0) {
			$this->atkNode($name,$flags);
		}

	 	function setAvailableLetters($attribname) {
			$this->m_available_letters = $attribname;
		}

	function getActiveTabOLDOLDOLD() {
		global $ATK_VARS, $_COOKIE;
		$tablist = $this->getTabs($ATK_VARS["atkaction"]);

      // Note: we may not read atktab from $this->m_postvars, because $this->m_postvars is not filled if this is
      // a nested node (in a relation for example).

		// originele code
      if (!empty($ATK_VARS["atktab"]) && in_array($ATK_VARS["atktab"], $tablist)) $tab = $ATK_VARS["atktab"];
      elseif (!empty($this->m_default_tab) && in_array($this->m_default_tab, $tablist)) $tab = $this->m_default_tab;
      else $tab = $tablist[0];

      return $tab;
    }

	}
?>