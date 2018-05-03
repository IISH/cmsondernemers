<?php /* Smarty version 2.6.2, created on 2005-03-14 15:57:24
         compiled from ./atk/themes/default/templates/recordlist.tpl */ ?>
<table border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td align="left">      
        <table id="<?php echo $this->_tpl_vars['listid']; ?>
" class="recordlist" cellspacing=0>
            <!-- header -->
            <tr> 
              <?php if (count($_from = (array)$this->_tpl_vars['header'])):
    foreach ($_from as $this->_tpl_vars['col']):
?>
                <th valign="<?php echo $this->_tpl_vars['vorientation']; ?>
" <?php echo $this->_tpl_vars['col']['htmlattributes']; ?>
>
                  <?php if ($this->_tpl_vars['col']['content'] != ""):  echo $this->_tpl_vars['col']['content'];  else: ?>&nbsp;<?php endif; ?>                     
                </th>
              <?php endforeach; unset($_from); endif; ?>
            </tr>
                            
            <?php if (count ( $this->_tpl_vars['search'] )): ?>
              <!-- search row -->
              <tr>
              <?php echo $this->_tpl_vars['searchstart']; ?>

              <?php if (count($_from = (array)$this->_tpl_vars['search'])):
    foreach ($_from as $this->_tpl_vars['col']):
?>
                  <th valign="<?php echo $this->_tpl_vars['vorientation']; ?>
" <?php echo $this->_tpl_vars['col']['htmlattributes']; ?>
>
                    <?php if ($this->_tpl_vars['col']['content'] != ""):  echo $this->_tpl_vars['col']['content'];  else: ?>&nbsp;<?php endif; ?>                     
                  </th>
              <?php endforeach; unset($_from); endif; ?>
              <?php echo $this->_tpl_vars['searchend']; ?>

              </tr>
            <?php endif; ?>
              
            <!-- records -->
            <?php echo $this->_tpl_vars['liststart']; ?>

              
            <?php if (count($_from = (array)$this->_tpl_vars['rows'])):
    foreach ($_from as $this->_tpl_vars['row']):
?>
              <tr id="<?php echo $this->_tpl_vars['row']['id']; ?>
" class="row<?php if ($this->_tpl_vars['row']['rownum'] % 2 == 0): ?>1<?php else: ?>2<?php endif; ?>" <?php if ($this->_tpl_vars['row']['background'] != ""): ?>style="background-color:<?php echo $this->_tpl_vars['row']['background']; ?>
" <?php endif; ?>
                   onmouseover="highlightrow(this, '<?php echo $this->_tpl_vars['row']['highlight']; ?>
')"
                   onmouseout="resetrow(this)"
                   onclick="selectrow(this, '<?php echo $this->_tpl_vars['listid']; ?>
', <?php echo $this->_tpl_vars['row']['rownum']; ?>
)">
              <?php if (count($_from = (array)$this->_tpl_vars['row']['cols'])):
    foreach ($_from as $this->_tpl_vars['col']):
?>
                <td valign="<?php echo $this->_tpl_vars['vorientation']; ?>
" <?php echo $this->_tpl_vars['col']['htmlattributes']; ?>
>
                  <?php if ($this->_tpl_vars['col']['content'] != ""):  echo $this->_tpl_vars['col']['content'];  else: ?>&nbsp;<?php endif; ?>
                </td>
              <?php endforeach; unset($_from); endif; ?>
            </tr>
            <?php endforeach; unset($_from); endif; ?>
            
            <?php if (count ( $this->_tpl_vars['total'] )): ?>
            <!-- totals row -->
              <tr>              
              <?php if (count($_from = (array)$this->_tpl_vars['total'])):
    foreach ($_from as $this->_tpl_vars['col']):
?>
                  <th valign="<?php echo $this->_tpl_vars['vorientation']; ?>
" <?php echo $this->_tpl_vars['col']['htmlattributes']; ?>
>
                    <?php if ($this->_tpl_vars['col']['content'] != ""):  echo $this->_tpl_vars['col']['content'];  else: ?>&nbsp;<?php endif; ?>                     
                  </th>
              <?php endforeach; unset($_from); endif; ?>              
              </tr>
            <?php endif; ?>
                      
      </table>
    </td>
  </tr>
  <?php if ($this->_tpl_vars['mra'] != ""): ?>
  <!-- multirecord actions -->
   <tr>
     <td align="left">
       <table border="0" cellspacing="0" cellpadding="2">
         <tr>
           <td valign="top"><img src="<?php echo $this->_tpl_vars['atkroot']; ?>
atk/images/arrow.gif" border="0"></td>
           <td>
             <?php echo $this->_tpl_vars['mra']; ?>
 <?php echo $this->_tpl_vars['listend']; ?>

           </td>
         </tr>
       </table>
     </td>
   </tr>
  <?php endif; ?>
</table>