<?php 
useattrib("atktextattribute");
useattrib("atknumberattribute");

class ondernemers_externedataset_link extends atkNodeXF
{
	function ondernemers_externedataset_link() {
		$this->atkNodeXF("ondernemers_externedataset_link", NF_ADD_LINK|NF_NO_IMPORT);

		$this->add(new atkAttribute("id", AF_AUTOKEY));
		$this->add(new atkNumberAttribute("ondernemersid", AF_HIDE));
		$this->add(new atkAttribute("externedataset"));
		$this->add(new atkAttribute("link"));

		$this->setOrder("externedataset");
		$this->setTable("tbl_ondernemers_externedataset_link");
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
		return "[externedataset]";
	}

	function adminFooter() {
		return "";
	}
}
?>
