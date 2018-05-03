<?php /* Smarty version 2.6.2, created on 2005-03-14 15:57:28
         compiled from ./atk/themes/iisg/templates/editform.tpl */ ?>
<br>
<table width="100%">
  <?php if (( count ( $this->_tpl_vars['errors'] ) > 0 )): ?>
    <tr>
      <td colspan="2" class="error">
        <?php echo $this->_tpl_vars['errortitle']; ?>

        <?php if (count($_from = (array)$this->_tpl_vars['errors'])):
    foreach ($_from as $this->_tpl_vars['error']):
?>
          <br><?php echo $this->_tpl_vars['error']['label']; ?>
: <?php echo $this->_tpl_vars['error']['msg']; ?>
 <?php if ($this->_tpl_vars['error']['tab'] != ""): ?> (<?php echo $this->_tpl_vars['error']['tab']; ?>
)<?php endif; ?>          
        <?php endforeach; unset($_from); endif; ?>
      </td>
    </tr>
  <?php endif; ?>  
  <?php if (count($_from = (array)$this->_tpl_vars['fields'])):
    foreach ($_from as $this->_tpl_vars['field']):
?>
    <tr<?php if ($this->_tpl_vars['field']['rowid'] != ""): ?> id="<?php echo $this->_tpl_vars['field']['rowid']; ?>
"<?php endif;  if ($this->_tpl_vars['field']['initial_on_tab'] != 'yes'): ?> style="display: none"<?php endif; ?> class="<?php echo $this->_tpl_vars['field']['tab']; ?>
">
      <?php if ($this->_tpl_vars['field']['line'] != ""): ?>
        <td colspan="2" valign="top"><?php echo $this->_tpl_vars['field']['line']; ?>
</td>      
      <?php else: ?>
      <?php if ($this->_tpl_vars['field']['label'] !== 'AF_NO_LABEL'): ?><td valign="top" class="<?php if ($this->_tpl_vars['field']['error'] != ""): ?>errorlabel<?php else: ?>fieldlabel<?php endif; ?>"><?php if ($this->_tpl_vars['field']['label'] != ""):  echo $this->_tpl_vars['field']['label']; ?>
: <?php endif; ?></td><?php endif; ?>
        <td valign="top" class="field"><?php echo $this->_tpl_vars['field']['full']; ?>
</td>
      <?php endif; ?>
    </tr>
  <?php endforeach; unset($_from); endif; ?>
</table>