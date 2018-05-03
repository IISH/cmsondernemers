<?php
useattrib("atktextattribute");
useattrib("atknumberattribute");
useattrib("atkboolattribute");

userelation("atkmanytoonerelationxf");
userelation("atkonetomanyrelation");

class ondernemers extends atkNodeXF
{
	function ondernemers() {
		$this->atkNodeXF("ondernemers", NF_ADD_LINK|NF_NO_IMPORT);

		$this->add(new atkNumberAttribute("ondernemersid", AF_AUTOKEY));

		$this->add(new atkAttribute("titulatuur", AF_HIDE_LIST));
		$this->add(new atkAttribute("achternamen", AF_SEARCHABLE));
		$this->add(new atkAttribute("tussenvoegsel", AF_SEARCHABLE));
		$this->add(new atkAttribute("voornamen", AF_SEARCHABLE));
		$this->add(new atkAttribute("geboortejaar", AF_SEARCHABLE));
		$this->add(new atkAttribute("sterftejaar", AF_SEARCHABLE));
		$this->add(new atkAttribute("plaats", AF_HIDE_LIST));
		$this->add(new atkTextAttribute("functie", AF_HIDE_LIST));

		# extra tab: sbi
		$this->add(new atkOneToManyRelation("ondernemers_sbi", "ondernemers.ondernemers_sbi", "ondernemersid", AF_HIDE_LIST), "ondernemers_sbi");

		# extra tab: bronnen
		$this->add(new atkOneToManyRelation("ondernemers_bron", "ondernemers.ondernemers_bron", "ondernemersid", AF_HIDE_LIST), "ondernemers_bron");

		# extra tab: externedataset link
		$this->add(new atkOneToManyRelation("ondernemers_externedataset_link", "ondernemers.ondernemers_externedataset_link", "ondernemersid", AF_HIDE_LIST), "ondernemers_externedataset_link");

		# extra tab: dataset bestandsnaam
		$this->add(new atkOneToManyRelation("ondernemers_dataset_bestandsnaam", "ondernemers.ondernemers_dataset_bestandsnaam", "ondernemersid", AF_HIDE_LIST), "ondernemers_dataset_bestandsnaam");

		$this->setOrder("achternamen");
		$this->setIndex("achternamen");
		$this->setTable("tbl_ondernemers");
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
		return "[fld_fullname]";
	}

	function adminFooter() {
		return "";
	}
}
