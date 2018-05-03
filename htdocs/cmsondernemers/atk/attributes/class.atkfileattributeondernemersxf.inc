<?php
include_once($config_atkroot."atk/attributes/class.atkfileattribute.inc");

class atkFileAttributeOndernemersXF extends atkFileAttribute
{

    function atkFileAttributeOndernemersXF($name, $dir, $flags=0, $size=0) {
		$this->atkFileAttribute($name, $dir, $flags, $size);
	}

    /**
     * Display values
     * @param array $record Array with fields
     * @return string Filename or Nothing
     */
    function display($record)
    {      
            
      $filename    = $record[$this->fieldName()]["orgfilename"];
      atkdebug($this->fieldname()." - File: $filename");
      $prev_type     = Array("jpg","jpeg","gif","tif","png","bmp","htm","html","txt");  // file types for preview
      $imgtype_prev = Array("jpg","jpeg","gif","png");  // types whitch are supported by GetImageSize
      if($filename != "")
      {
        if(is_file($this->m_dir.$filename))
        {
          $ext = strtolower(substr($filename, strrpos($filename,'.')+1,strlen($filename)));
          if((in_array($ext, $prev_type) && $this->hasFlag(AF_FILE_NO_AUTOPREVIEW))||(!in_array($ext, $prev_type)))
          { 
            return "<a href=\"".$this->OndernemersUrl($this->m_url.$filename)."\" target=\"_blank\">$filename</a>";
          }
          elseif(in_array($ext, $prev_type) && $this->hasFlag(AF_FILE_POPUP))
          {
            if(in_array($ext, $imgtype_prev))
            { 
              $imagehw = GetImageSize($this->m_dir.$filename);
            }
            else
            {
              $imagehw=Array("0"=>"640","1"=>"480");
            }
            $page = &atkPage::getInstance();
            $page->register_script(atkconfig("atkroot")."atk/javascript/newwindow.js");
            return '<a href="'.$this->OndernemersUrl($this->m_url.$filename).'" alt="'.$filename.'" onclick="NewWindow(this.href,\'name\',\''.($imagehw[0]+50).'\',\''.($imagehw[1]+50).'\',\'yes\');return false;">'.$filename.'</a>';
          }
          return '<img src="'.$this->OndernemersUrl($this->m_url.$filename).'" alt="'.$filename.'">';
        }
        else
        {
          return $filename."(<font color=\"#ff0000\">".text("file_not_exist")."</font>)";
        }
      }
    }

	function OndernemersUrl($url) {
		$url = str_replace("/www/cmsondernemers/public_html", "", $url);
		$url = "http://www.iisg.nl" . $url;

		return $url;
	}
}
?>