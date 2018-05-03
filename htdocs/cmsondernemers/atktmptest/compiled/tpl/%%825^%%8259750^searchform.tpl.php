<?php /* Smarty version 2.6.2, created on 2005-03-24 08:43:59
         compiled from ./atk/themes/default/templates/searchform.tpl */ ?>
<table width="100%">
  <tr>
    <td class="fieldlabel">
      <?php echo $this->_tpl_vars['searchmode_title']; ?>

    </td>
    <td colspan="2" class="field">
      <?php echo $this->_tpl_vars['searchmode_and']; ?>
 &nbsp;&nbsp; <?php echo $this->_tpl_vars['searchmode_or']; ?>

    </td>
  </tr>    
  <tr>
    <td colspan="3"><hr></td>
  </tr>
  <?php if (count($_from = (array)$this->_tpl_vars['fields'])):
    foreach ($_from as $this->_tpl_vars['field']):
?>        
    <tr>
      <?php if ($this->_tpl_vars['field']['line'] != ""): ?>
        <td colspan="2" valign="top" class="field"><?php echo $this->_tpl_vars['field']['line']; ?>
</td>      
      <?php else: ?>
        <td valign="top" class="fieldlabel"><?php if ($this->_tpl_vars['field']['label'] != ""):  echo $this->_tpl_vars['field']['label']; ?>
: <?php endif; ?></td>
        <td valign="top" class="field"><?php echo $this->_tpl_vars['field']['full']; ?>
</td>
      <?php endif; ?>
      <td class="field">
        <?php echo $this->_tpl_vars['field']['searchmode']; ?>

      </td>
    </tr>
  <?php endforeach; unset($_from); endif; ?>
</table>