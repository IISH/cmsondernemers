<?php /* Smarty version 2.6.2, created on 2008-10-22 14:32:18
         compiled from ./atk/themes/default/templates/action.tpl */ ?>
<?php echo $this->_tpl_vars['formstart']; ?>

<table border="0" cellspacing="0" cellpadding="2" width="100%">
  <?php if ($this->_tpl_vars['helplink'] != ""): ?><tr><td align="right" class="helplink"><?php echo $this->_tpl_vars['helplink']; ?>
</td></tr><?php endif; ?>
  <tr>
    <td valign="top" align="center"><?php echo $this->_tpl_vars['content']; ?>
<br></td>
  </tr>
  <tr>
    <td align="center" valign="top">
      <?php if (count($_from = (array)$this->_tpl_vars['buttons'])):
    foreach ($_from as $this->_tpl_vars['button']):
?>
        &nbsp;<?php echo $this->_tpl_vars['button']; ?>
&nbsp;        
      <?php endforeach; unset($_from); endif; ?><br><br></td>
  </tr>
</table>
<?php echo $this->_tpl_vars['formend']; ?>
