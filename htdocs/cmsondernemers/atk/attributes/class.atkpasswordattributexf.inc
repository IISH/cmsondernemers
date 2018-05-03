<?php

class atkPasswordAttributeXF extends atkAttribute
{
	function atkPasswordAttributeXF($name, $flags=0) {
		$this->atkAttribute($name,$flags);
	}

	function edit($record="", $fieldprefix="", $mode="") {
		$id = $this->getHtmlId($fieldprefix);
		$this->registerKeyListener($id, KB_CTRLCURSOR|KB_UPDOWN);

		$result = '<input type="password" id="'.$id.'" name="'.$id.
					'" value="'.htmlspecialchars($record[$this->fieldName()]).'"'.
					($this->m_size > 0 ? ' size="'.$this->m_size.'"' : '').
					($this->m_maxsize > 0 ? ' maxlength="'.$this->m_maxsize.'"' : '').'>';

		return $result;
	}

	function display($record, $mode="") {
		return text("password_hidden");
	}

}
