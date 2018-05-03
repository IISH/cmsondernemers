<?php
include_once(atkconfig("atkroot")."atk/attributes/class.atkdummyattribute.inc");

class atkSpecialURLXML2webXF extends atkDummyAttribute
{
	var $m_linktarget;

	function atkSpecialURLXML2webXF($name="", $target="", $flags=0) {
		$this->atkDummyAttribute($name,"",$flags);
		$this->m_linktarget = $target;
	}

	function display($record, $mode="") {
		$retVal = "";

		$id = $record['id'];

		// html
		$retVal .= "<nobr>" . text("phpcode") . " ";
		$type = "html";
		$length = "lang";
		$retVal .= "<a href=\"../_dynamic/xml2web/show_code.php?id=" . $id . "&length=" . $length . "&type=" . $type . "\" target=\"_blank\">toon code</a>";

		$retVal .= "<br>";

		// javascript
		$retVal .= "<nobr>" . text("javascriptcode") . " ";
		$type = "javascript";
		$length = "lang";
		$retVal .= "<a href=\"../_dynamic/xml2web/show_code.php?id=" . $id . "&length=" . $length . "&type=" . $type . "\" target=\"_blank\">toon code</a>";

//		$retVal .= " / ";
//		$length = "short";

//		$retVal .= "<a href=\"../_dynamic/xml2web/show_code.php?id=" . $id . "&length=" . $length . "&type=" . $type . "\" target=\"_blank\">" . text("kort") . "</a>";
		$retVal .= "<br>";

		return $retVal;
	}
}
?>