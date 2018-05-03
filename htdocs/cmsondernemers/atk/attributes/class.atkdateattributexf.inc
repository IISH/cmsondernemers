<?php
class atkDateAttributeXF extends atkAttribute
{
    var $m_date_format_view;

	function atkDateAttributeXF($name, $format_edit="", $format_view="", $min=0, $max=0, $flags = 0, $size = 0) {
		if ( $format_view == "" ) {
			$format_view = "j F Y";
		}

		$this->m_date_format_view = $format_view;
		$this->atkAttribute($name,$flags);
	}

	/* aangepaste DATE display functie */
	function display($record) {
		$value = $record[$this->fieldName()];

		if ( $value == "" ) return "&nbsp;";

		$year = intval(substr($value, 0, 4));
		$month = intval(substr($value, 5, 2));
		$day = intval(substr($value, 8, 2));

		$tmp_date = adodb_getdate(adodb_mktime(0, 0, 0, $month, $day, $year));
		if (!empty($tmp_date)) {
			if ($this->hasFlag(AF_DATE_DISPLAY_DAY)) {
				return $this->formatDate($tmp_date, $this->m_date_format_view, 1);
			} else {
				return $this->formatDate($tmp_date, $this->m_date_format_view, 0);
			}
		}
		else return "&nbsp;";
	}

	/* kopie van formatDate functie uit DATE */
	function formatDate($date, $format, $weekday=1) {
		static $langcache = array();

		/* format month */
		$format = str_replace("M", "%-%",   $format);
		$format = str_replace("F", "%=%",   $format);

		/* format day */
		if ($weekday) {
			$format = str_replace("d", "%*% d", $format);
			$format = str_replace("j", "%*% j", $format);
		}

		/* get date string */
		$str_date = adodb_date($format, $date[0]);

		$month = $date['month'];
		$shortmonth = substr(strtolower($date["month"]), 0, 3);

		/* store the text calls */
		if(!$langcache[$month]) {
			$langcache[$month]= text(strtolower($month),"","atk");
		}

		if(!$langcache[$shortmonth]) {
			$langcache[$shortmonth] = text($shortmonth,"","atk");
		}

		/* replace month/week name */
		$str_date = str_replace("%-%", $langcache[$shortmonth], $str_date);
		$str_date = str_replace("%=%", $langcache[$month], $str_date);
		if ($weekday) $str_date = str_replace("%*%", text(strtolower($date["weekday"]),"","atk"), $str_date);

		/* return string */
		return $str_date;
	}

}
?>