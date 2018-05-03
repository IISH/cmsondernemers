<?php 
useattrib("atktextattribute");
useattrib("atknumberattribute");
useattrib("atkfileattributeondernemersxf");

class ondernemers_dataset_bestandsnaam extends atkNodeXF
{
	function ondernemers_dataset_bestandsnaam() {
		global $ondernemersPublicHtmlDir;

		$this->atkNodeXF("ondernemers_dataset_bestandsnaam", NF_ADD_LINK|NF_NO_IMPORT);

		$this->add(new atkAttribute("id", AF_AUTOKEY));
		$this->add(new atkNumberAttribute("ondernemersid", AF_HIDE));
		$this->add(new atkAttribute("dataset"));
		$this->add(new atkFileAttributeOndernemersXF("bestandsnaam_pdf", $ondernemersPublicHtmlDir . "/pdf/", AF_FILE_POPUP|AF_FILE_NO_SELECT));
		$this->add(new atkFileAttributeOndernemersXF("bestandsnaam_text", $ondernemersPublicHtmlDir . "/txt/", AF_FILE_POPUP|AF_FILE_NO_SELECT));

		$this->setOrder("dataset");
		$this->setTable("tbl_ondernemers_dataset_bestandsnaam");
	}

	# database last modified
	function UpdateLastModifiedFieldInSettingsTable($rec) {
		$queryUpdate = 'UPDATE tbl_settings SET setting_value="' . date("Y-m-d H:i:s") . '" WHERE setting="ondernemers_last_modified" ';
		$resultUpdate = mysql_query($queryUpdate) or die ('Query failed: ' . mysql_error());

		return true;
	}

	function SaveFileSource($rec) {
		global $ondernemersPublicHtmlDir;

		$source = "";
		$sourcedir = $ondernemersPublicHtmlDir . "/txt/";
		$updatequery = "UPDATE tbl_ondernemers_dataset_bestandsnaam SET source='::source::' WHERE id=::id::";
		$updatequery = str_replace("::id::", $rec["id"], $updatequery);

		if ( $rec["bestandsnaam_text"]["filename"] <> "" ) {

			if ( file_exists($sourcedir . $rec["bestandsnaam_text"]["filename"]) ) {
				// read file source
				$fp = @fopen($sourcedir . $rec["bestandsnaam_text"]["filename"], "r") or die("Couldn't open file"); 
				while(!feof($fp)) {
					$source .= fgets($fp, 1024); 
				}
				fclose($fp); 
			}
			$updatequery = str_replace("::source::", addslashes($source), $updatequery);
			$resultUpdateQuery = mysql_query($updatequery) or die ('Query failed: ' . mysql_error());
		} else {
			if ( $rec["bestandsnaam_text"]["orgfilename"] <> "" ) {
				$updatequery = str_replace("::source::", addslashes($source), $updatequery);
				$resultUpdateQuery = mysql_query($updatequery) or die ('Query failed: ' . mysql_error());
			}
		}
		return true;
	}

	function postUpdate($rec) {
		$this->SaveFileSource($rec);
		$this->UpdateLastModifiedFieldInSettingsTable($rec);
		return true;
	}

	function postAdd($rec) {
		$this->SaveFileSource($rec);
		$this->UpdateLastModifiedFieldInSettingsTable($rec);
		return true;
	}

	function postDel($rec) {
		$this->UpdateLastModifiedFieldInSettingsTable($rec);
		return true;
	}

	function descriptor_def() {
		return "[externedataset]";
	}

	function adminFooter() {
		return "";
	}
}
?>