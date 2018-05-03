<?php

class atkLinkAttributeXF extends atkAttribute
{
	var $m_linktarget;

	function atkLinkAttributeXF($name, $target="", $flags=0, $size=50) {
		$this->atkAttribute($name,$flags, $size);
		$this->m_linktarget = $target;
	}

	function display($record, $mode="") {
		// the next if-statement is a workaround for derived attributes which do
		// not override the display() method properly. This will not give them a 
		// working display() functionality but at least it will not give error messages.
		if (!is_array($record[$this->fieldName()])) 
		{
			// default behaviour is that we display a value 'as is'.
			return "<a href=\"" . $record[$this->fieldName()] . "\" target=\"" .  $this->m_linktarget . "\">" . nl2br(htmlspecialchars($record[$this->fieldName()])) . "</a>";
		}
	}
}
