<?php /* Smarty version 2.6.2, created on 2005-03-14 15:57:28
         compiled from ./atk/themes/iisg/templates/tabs.tpl */ ?>
<script language="JavaScript" type="text/javascript">
var tabs = new Array();
<?php if (isset($this->_sections['i'])) unset($this->_sections['i']);
$this->_sections['i']['name'] = 'i';
$this->_sections['i']['loop'] = is_array($_loop=$this->_tpl_vars['tabs']) ? count($_loop) : max(0, (int)$_loop); unset($_loop);
$this->_sections['i']['show'] = true;
$this->_sections['i']['max'] = $this->_sections['i']['loop'];
$this->_sections['i']['step'] = 1;
$this->_sections['i']['start'] = $this->_sections['i']['step'] > 0 ? 0 : $this->_sections['i']['loop']-1;
if ($this->_sections['i']['show']) {
    $this->_sections['i']['total'] = $this->_sections['i']['loop'];
    if ($this->_sections['i']['total'] == 0)
        $this->_sections['i']['show'] = false;
} else
    $this->_sections['i']['total'] = 0;
if ($this->_sections['i']['show']):

            for ($this->_sections['i']['index'] = $this->_sections['i']['start'], $this->_sections['i']['iteration'] = 1;
                 $this->_sections['i']['iteration'] <= $this->_sections['i']['total'];
                 $this->_sections['i']['index'] += $this->_sections['i']['step'], $this->_sections['i']['iteration']++):
$this->_sections['i']['rownum'] = $this->_sections['i']['iteration'];
$this->_sections['i']['index_prev'] = $this->_sections['i']['index'] - $this->_sections['i']['step'];
$this->_sections['i']['index_next'] = $this->_sections['i']['index'] + $this->_sections['i']['step'];
$this->_sections['i']['first']      = ($this->_sections['i']['iteration'] == 1);
$this->_sections['i']['last']       = ($this->_sections['i']['iteration'] == $this->_sections['i']['total']);
?>tabs[tabs.length] = "<?php echo $this->_tpl_vars['tabs'][$this->_sections['i']['index']]['tab']; ?>
"; <?php endfor; endif; ?>

var tabLeftImage = "<?php echo $this->_tpl_vars['themedir']; ?>
images/tab_left.gif";
var tabRightImage = "<?php echo $this->_tpl_vars['themedir']; ?>
images/tab_right.gif";
var tabBackgroundImage = "<?php echo $this->_tpl_vars['themedir']; ?>
images/tab_back.gif";
var tabSelectedLeftImage = "<?php echo $this->_tpl_vars['themedir']; ?>
images/tab_left_s.gif";
var tabSelectedRightImage = "<?php echo $this->_tpl_vars['themedir']; ?>
images/tab_right_s.gif";
var tabSelectedBackgroundImage = "<?php echo $this->_tpl_vars['themedir']; ?>
images/tab_back_s.gif";

var tabColor = "#FFFFFF";
var tabSelectedColor = "#000000";
</script>

<table border="0" cellpadding="0" cellspacing="0" bgcolor="#606060" width="98%" align="center" valign="top">
  <tr>
    <td width="100%" bgcolor="#EEEEE0">
      <table border="0" cellpadding="0" cellspacing="0">
        <tr>
          <?php if (isset($this->_sections['i'])) unset($this->_sections['i']);
$this->_sections['i']['name'] = 'i';
$this->_sections['i']['loop'] = is_array($_loop=$this->_tpl_vars['tabs']) ? count($_loop) : max(0, (int)$_loop); unset($_loop);
$this->_sections['i']['show'] = true;
$this->_sections['i']['max'] = $this->_sections['i']['loop'];
$this->_sections['i']['step'] = 1;
$this->_sections['i']['start'] = $this->_sections['i']['step'] > 0 ? 0 : $this->_sections['i']['loop']-1;
if ($this->_sections['i']['show']) {
    $this->_sections['i']['total'] = $this->_sections['i']['loop'];
    if ($this->_sections['i']['total'] == 0)
        $this->_sections['i']['show'] = false;
} else
    $this->_sections['i']['total'] = 0;
if ($this->_sections['i']['show']):

            for ($this->_sections['i']['index'] = $this->_sections['i']['start'], $this->_sections['i']['iteration'] = 1;
                 $this->_sections['i']['iteration'] <= $this->_sections['i']['total'];
                 $this->_sections['i']['index'] += $this->_sections['i']['step'], $this->_sections['i']['iteration']++):
$this->_sections['i']['rownum'] = $this->_sections['i']['iteration'];
$this->_sections['i']['index_prev'] = $this->_sections['i']['index'] - $this->_sections['i']['step'];
$this->_sections['i']['index_next'] = $this->_sections['i']['index'] + $this->_sections['i']['step'];
$this->_sections['i']['first']      = ($this->_sections['i']['iteration'] == 1);
$this->_sections['i']['last']       = ($this->_sections['i']['iteration'] == $this->_sections['i']['total']);
?>
          
          <td valign="bottom">
          <div style="position: relative;">
            <div id="tab_<?php echo $this->_tpl_vars['tabs'][$this->_sections['i']['index']]['tab']; ?>
" style="position: absolute;">
            <table border="0" cellspacing="0" cellpadding="0">
                <tr>
                  <td><img  src="<?php echo $this->_tpl_vars['themedir']; ?>
images/tab_left_s.gif"></td>
                  <td onclick="showTab('<?php echo $this->_tpl_vars['tabs'][$this->_sections['i']['index']]['tab']; ?>
')" height="22" valign="middle" background="<?php echo $this->_tpl_vars['themedir']; ?>
images/tab_back_s.gif" align="center" nowrap>
                    <b><?php echo $this->_tpl_vars['tabs'][$this->_sections['i']['index']]['title']; ?>
</b>
                  </td>
                  <td><img src="<?php echo $this->_tpl_vars['themedir']; ?>
images/tab_right_s.gif"></td>
                </tr>
              </table>
            </div>
            <table border="0" cellspacing="0" cellpadding="0" style="cursor: pointer;">
              <tr>
                <td><img src="<?php echo $this->_tpl_vars['themedir']; ?>
images/tab_left.gif"></td>
                <td onclick="showTab('<?php echo $this->_tpl_vars['tabs'][$this->_sections['i']['index']]['tab']; ?>
')" height="22" valign="middle" background="<?php echo $this->_tpl_vars['themedir']; ?>
images/tab_back.gif" align="center" nowrap>
                    <b><?php echo $this->_tpl_vars['tabs'][$this->_sections['i']['index']]['title']; ?>
</b>
                </td>
                <td><img src="<?php echo $this->_tpl_vars['themedir']; ?>
images/tab_right.gif"></td>
              </tr>
            </table>  
          </div>
          </td>
          <?php endfor; endif; ?>
          <td valign="bottom" width="100%">
            <table cellspacing="0" cellpadding="0" border="0" width="100%">
              <tr><td bgcolor="#00309c" height="2"><img src="<?php echo $this->_tpl_vars['themedir']; ?>
images/blank.gif" height="2" width="2"></td></tr>
            </table>
          </td>
        </tr>
      </table>
      <table width="100%" border="0" cellspacing="0" cellpadding="0">
        <tr>
          <td bgcolor="#00309c" width="2"><img src="<?php echo $this->_tpl_vars['themedir']; ?>
images/blank.gif" width="2"></td>
          <td bgcolor="#EEEEE0" align="left" class="block">
            <table border="0" cellspacing="5" cellpadding="5">
              <tr>
                <td>
                  <?php echo $this->_tpl_vars['content']; ?>

                </td>
              </tr>
            </table>
          </td>
          <td bgcolor="#00309c" width="2"><img src="<?php echo $this->_tpl_vars['themedir']; ?>
images/blank.gif" width="2"></td>
        </tr>
        <tr>
          <td bgcolor="#00309c" colspan="3" valign="top"><img src="<?php echo $this->_tpl_vars['themedir']; ?>
images/blank.gif" height="2" width="1"></td>
        </tr>
      </table>
    </td>
  </tr>
</table>

<script language="JavaScript" type="text/javascript">
showTab();
</script>