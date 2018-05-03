<?php
useattrib("atktextattribute");

class sbicodes extends atkNode
{
	function sbicodes() {
		$this->atkNode("sbicodes", NF_ADD_LINK|NF_NO_IMPORT);

		$this->add(new atkAttribute("sbiid", AF_AUTOKEY));
		$this->add(new atkAttribute("code"));
		$this->add(new atkTextAttribute("description"));

		$this->setOrder("code");

		$this->setTable("tbl_sbicodes");
	}

	# database last modified
	function UpdateLastModifiedFieldInSettingsTable($rec) {
		$queryUpdate = 'UPDATE tbl_settings SET setting_value="' . date("Y-m-d H:i:s") . '" WHERE setting="ondernemers_last_modified" ';
		$resultUpdate = mysql_query($queryUpdate) or die ('Query failed: ' . mysql_error());

		return true;
	}

	function postUpdate($rec) {
		$this->UpdateLastModifiedFieldInSettingsTable($rec);
		return true;
	}

	function postAdd($rec) {
		$this->UpdateLastModifiedFieldInSettingsTable($rec);
		return true;
	}

	function postDel($rec) {
		$this->UpdateLastModifiedFieldInSettingsTable($rec);
		return true;
	}

	function descriptor_def() {
		return "[code]";
	}

	function adminFooter() {
		return "";
	}
}
