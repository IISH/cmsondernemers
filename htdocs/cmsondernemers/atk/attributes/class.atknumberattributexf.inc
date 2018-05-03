<?php
useattrib("atknumberattribute");

class atkNumberAttributeXF extends atkNumberAttribute {
	function atkNumberAttributeXF($name, $flags=0, $size=10, $decimals=0) {
		$this->atkNumberAttribute($name,$flags|AF_NO_QUOTES, $size, $decimals);
	}

	// aangepaste display
	// als waarde=0
	// dan moet 'alle' getoond worden
	function display($record, $mode="") {
		$retVal = $record[$this->fieldName()];

		if ( $retVal . "" == "0" ) {
			$retVal = "alle";
		}

		return $retVal;
	}
}
?>