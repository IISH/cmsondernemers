<?php
include_once("class.atkpage.inc");

class atkPageXF extends atkPage
{

	function atkPageXF() {
		$this->atkPage();
	}

	// aangepaste versie van body van atkpage
    function bodyOLDOLDOLD($extraprops="")
    {
      global $ATK_VARS;

      $res = '<body ';
      if (count($this->m_loadscripts)) 
      {
        $res.='onLoad="globalLoad(); if ( window.showTab ) {showTab(\'';

// originele regel
		if ($ATK_VARS['atktab']) $res.=$ATK_VARS['atktab'];

        $res.='\');}" ';
      }
      $res.= $extraprops.">\n";
      return $res;
    }

	function head($title) {
      $res = "<head>\n  <title>$title</title>\n";
      
      // $Name: atk_5_1_0 $ is the CVS tag.
      $version = atkversion();
      if ("\$Name: atk_5_1_0 $"!="\$"."Name:  $") $version.=" ($Name: atk_5_1_0 $)";
      $res.= '  <META NAME="atkversion" CONTENT="'.$version.'">'."\n";
      $res.= '  <META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset='.text("charset","","atk").'">'."\n";         
      $this->addScripts($res);

      for ($i = 0; $i < count($this->m_stylesheets); $i++)
        $res.='  <link href="'.$this->m_stylesheets[$i].'" rel="stylesheet" type="text/css">'."\n";
				
      for ($i = 0; $i < count($this->m_stylecode); $i++)
	$res.= '<style> '.$this->m_stylecode[$i].' </style>'."\n";

// begin toegevoegde deel
		// add html area javascript
		$res.= '<script language="Javascript1.2">' . "\n";
		$res.= '<!-- // load htmlarea' . "\n";
		$res.= '_editor_url = "htmlarea/";                     // URL to htmlarea files' . "\n";
		$res.= 'var win_ie_ver = parseFloat(navigator.appVersion.split("MSIE")[1]);' . "\n";
		$res.= 'if (navigator.userAgent.indexOf(\'Mac\')        >= 0) { win_ie_ver = 0; }' . "\n";
		$res.= 'if (navigator.userAgent.indexOf(\'Windows CE\') >= 0) { win_ie_ver = 0; }' . "\n";
		$res.= 'if (navigator.userAgent.indexOf(\'Opera\')      >= 0) { win_ie_ver = 0; }' . "\n";
		$res.= 'if (win_ie_ver >= 5.5) {' . "\n";
		$res.= '	document.write(\'<scr\' + \'ipt src="\' +_editor_url+ \'editor.js"\');' . "\n";
		$res.= '	document.write(\' language="Javascript1.2"></scr\' + \'ipt>\');  ' . "\n";
		$res.= '} else {' . "\n";
		$res.= '	document.write(\'<scr\'+\'ipt>function editor_generate() { return false; }</scr\'+\'ipt>\');' . "\n";
		$res.= '}' . "\n";
		$res.= '// -->' . "\n";
		$res.= '</script> ' . "\n";
// einde toegevoegde deel

      $res.="</head>\n";
      return $res;

	}

}
?>