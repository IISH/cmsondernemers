<?php 
class atkAttributeGetFromDBXF extends atkAttribute {

	function atkAttributeGetFromDBXF($name, $flags = 0, $size = 0) {
		$this->atkAttribute($name, $flags, $size);
	}

    function hide($record="", $fieldprefix="") {
        return "";
    }

	function display($record) {
		$value = $this->Get_ValueFromDb($record);
		return $value;
	}

	function Get_ValueFromDb($record) {
		global $link;
		$closeLink = 0;
		if (!isset($link)) {
			$link = mysql_connect(atkconfig("databasehost"), atkconfig("databaseuser"), atkconfig("databasepassword")) or die('Could not connect: ' . mysql_error());
			mysql_select_db(atkconfig("databasename")) or die ('Could not select database');
		}

		$tabel = $this->m_ownerInstance->m_table;
		$veldnaam = $this->m_ownerInstance->primaryKeyField($record);
		if ( $tabel != "" && $record[$veldnaam] != "" ) {
			$query = "SELECT * FROM " . $tabel . " WHERE " . $veldnaam . "='" . $record[$veldnaam] . "'";
			$result = mysql_query($query) or die("456fx Fout bij uitvoeren query");

			if (($rsresult = mysql_fetch_array($result, MYSQL_ASSOC))) {
				$result = $rsresult[$this->fieldName()];
				$result = str_replace(">", "&gt;", $result);
				$result = str_replace("<", "&lt;", $result);
				$result = str_replace("\n\n", "<br>\n", $result);
			} else {
				$result = "";
			}
		} else {
			$result = "";
		}

//		if ( $closeLink == 1 ) {
//			mysql_close($link);
//		}

		return $result;
	}
}
?>