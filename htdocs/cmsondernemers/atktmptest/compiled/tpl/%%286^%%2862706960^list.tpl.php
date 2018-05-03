<?php /* Smarty version 2.6.2, created on 2008-10-22 11:34:37
         compiled from ./atk/themes/default/templates/list.tpl */ ?>
<?php echo $this->_tpl_vars['formstart']; ?>

<table border="0" cellspacing="0" cellpadding="2" width="100%">
  <?php if ($this->_tpl_vars['header'] != ""): ?>
  <tr>
    <td valign="top" align="left"><?php echo $this->_tpl_vars['header']; ?>
<br><br></td>
  </tr>
  <?php endif; ?>
  <?php if ($this->_tpl_vars['index'] != ""): ?>
  <tr>
    <td valign="top" align="left"><?php echo $this->_tpl_vars['index']; ?>
<br><br></td>
  </tr>
  <?php endif; ?>
  <?php if ($this->_tpl_vars['navbar'] != ""): ?>
  <tr>
    <td valign="top" align="left"><?php echo $this->_tpl_vars['navbar']; ?>
<br></td>
  </tr>
  <?php endif; ?>
  <tr>
    <td valign="top" align="left"><?php echo $this->_tpl_vars['list']; ?>
<br></td>
  </tr>
  <?php if ($this->_tpl_vars['navbar'] != ""): ?>
  <tr>
    <td valign="top" align="left"><?php echo $this->_tpl_vars['navbar']; ?>
<br></td>
  </tr>
  <?php endif; ?>
  <?php if ($this->_tpl_vars['footer'] != ""): ?>
  <tr>
    <td valign="top" align="left"><?php echo $this->_tpl_vars['footer']; ?>
<br><br></td>
  </tr>
  <?php endif; ?>
</table>
<?php echo $this->_tpl_vars['formend']; ?>