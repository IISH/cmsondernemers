<?php

class atkLinkAttributeNoLinkShortNameXF extends atkAttribute
{
	var $m_linktarget;
	var $m_href;

	function atkLinkAttributeNoLinkShortNameXF($name, $href=0, $target="", $flags=0) {
		$this->atkAttribute($name,$flags);
		$this->m_linktarget = $target;
		$this->m_href = $href;
	}

	function display($record, $mode="") {
		$retURL = $record[$this->fieldName()];
		$retLabel = $record[$this->fieldName()];

		// zoek naar laatste slash
		// indien aanwezig, gebruik alleen laatste stuk als (href) label
		$pos = strrpos($retLabel, "/");
		if (!($pos === false)) {
			$retLabel = "..." . substr($retLabel, -(strlen($retLabel)-$pos));
		}

		if ( $this->m_href == 1 ) {
			// create url
			$retVal = "<a href=\"" . $retURL . "\"";
			if ( $this->m_linktarget != "" ) {
				$retVal .= " target=\"" . $this->m_linktarget . "\"";
			}
			$retVal .= ">" . $retLabel . "</a>";
		} else {
			// show only label
			$retVal = $retLabel;
		}

		return $retVal;
	}

}
?>