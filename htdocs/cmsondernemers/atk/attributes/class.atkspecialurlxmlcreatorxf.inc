<?php
include_once(atkconfig("atkroot")."atk/attributes/class.atkdummyattribute.inc");

class atkSpecialURLXMLcreatorXF extends atkDummyAttribute
{
	var $m_linktarget;

	function atkSpecialURLXMLcreatorXF($name="", $target="", $flags=0) {
		$this->atkDummyAttribute($name,"",$flags);
		$this->m_linktarget = $target;
	}

	function display($record, $mode="") {
		// the next if-statement is a workaround for derived attributes which do
		// not override the display() method properly. This will not give them a 
		// working display() functionality but at least it will not give error messages.
		if (!is_array($record[$this->fieldName()])) 
		{
			// default behaviour is that we display a value 'as is'.
			$xmlurl_webpath = "";
			$xmlurl_serverpath = "";

			// achterhaal xml filenaam
			$gcresult = mysql_query("SELECT id, xml_filename, externintern FROM xmlcreator WHERE id=" . $record[id]);
			if (!$gcresult) {
			   echo 'Could not run query: ' . mysql_error();
			   exit;
			}
			$gcrow = mysql_fetch_row($gcresult);
			$xmlfilenaam = $gcrow[1]; // xml_filename
			$externintern = $gcrow[2]; // externintern

			if ( $xmlfilenaam != "" ) {
				// achterhaal webpath
				$gcresult2 = mysql_query("SELECT filepath_webpath, filepath_serverpath FROM xmlcreator_filepaths WHERE filepath_nickname='" . $externintern . "'");
				$gcrow2 = mysql_fetch_row($gcresult2);
				$xmlurl_webpath = $gcrow2[0];
				$xmlurl_serverpath = $gcrow2[1];
			}

			if ( $xmlurl_webpath != "" ) {
				$xmlurl_webpath .= "/" . $xmlfilenaam;
				$xmlurl_serverpath .= "/" . $xmlfilenaam;
			}

			// toon url voor het 'genereren van de xml file'
			$retValue = "<nobr><a href=\"xml_create.php?atk_id=" . $record[id] . "&atk_backurl=" . $this->Get_CurrentPageURL() . "\">" . text("create_xml_file") . "</a></nobr><br>";

			// toon url voor het tonen van de xml file
			// deze url wordt alleen getoond als er 
			if ( $xmlurl_webpath != "" ) {

				if (file_exists($xmlurl_serverpath)) {
					$retValue .= "<a href=\"#\" onClick=\"javascript:window.open('" . $xmlurl_webpath . "', '_xmlfile" . $record[id] . "');return false;\">" . text("view_xml_file") . "</a><br>";
				}

			}
			return $retValue;
		}
	}

	function Get_CurrentPageURL() {
		$retValue = $_SERVER['PHP_SELF'] . "?" . $_SERVER['QUERY_STRING'];
		$retValue = urlencode($retValue);
		return $retValue;
	}

}
?>