<?php /* Smarty version 2.6.2, created on 2005-03-17 11:41:11
         compiled from ./atk/themes/default/templates/viewform.tpl */ ?>
<table width="100%">
  <?php if (count($_from = (array)$this->_tpl_vars['fields'])):
    foreach ($_from as $this->_tpl_vars['field']):
?>
    <tr<?php if ($this->_tpl_vars['field']['rowid'] != ""): ?> id="<?php echo $this->_tpl_vars['field']['rowid']; ?>
"<?php endif;  if ($this->_tpl_vars['field']['initial_on_tab'] != 'yes'): ?> style="display: none"<?php endif; ?> class="<?php echo $this->_tpl_vars['field']['tab']; ?>
">
      <?php if ($this->_tpl_vars['field']['line'] != ""): ?>
        <td colspan="2" valign="top" class="field"><?php echo $this->_tpl_vars['field']['line']; ?>
</td>      
      <?php else: ?>
        <td valign="top" class="fieldlabel"><?php if ($this->_tpl_vars['field']['label'] != ""):  echo $this->_tpl_vars['field']['label']; ?>
: <?php endif; ?></td>
        <td valign="top" class="field"><?php echo $this->_tpl_vars['field']['full']; ?>
</td>
      <?php endif; ?>
    </tr>
  <?php endforeach; unset($_from); endif; ?>
</table>