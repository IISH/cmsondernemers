<?php
include_once($config_atkroot."atk/attributes/class.atktextattribute.inc");

class atkTextAttributeXF extends atkTextAttribute
{

	function atkTextAttributeXF($name,$size=0,$flags=0) {
		$this->atkTextAttribute($name,$size,$flags);
	}

	function edit($record="", $fieldprefix="") {
		if ($this->m_cols != 0) {
			$cols = $this->m_cols;
		} else {
			$cols = $this->maxInputSize();
		}
		$rows = $this->m_rows;
		$id = $fieldprefix.$this->fieldName();
		$this->registerKeyListener($id, KB_CTRLCURSOR);	
		if ($rows =="" || $rows == 0) {
			$rows = 10;
		}

		if ($this->m_autoadjust) {
			$this->doAutoAdjust(htmlspecialchars($record[$this->fieldName()]), $rows, $cols);
		}

		$result = '<textarea id="'.$id.'" name="'.$id.'" rows="'.$rows.'" cols="'.$cols.'" wrap="soft"'.
						 '>'.htmlspecialchars($record[$this->fieldName()]).'</textarea>' . "\n";
		$result .= '<script language="JavaScript1.2">' . "\n";
		$result .= 'editor_generate(\'' . $id . '\');' . "\n";
		$result .= '</SCRIPT>' . "\n";

		return $result;
	}

}
?>