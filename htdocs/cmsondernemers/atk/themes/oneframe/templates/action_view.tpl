<!-- START oneframe action_view.tpl -->
<table border="0" cellspacing="0" cellpadding="2" xxxwidth="100%">
  {if $helplink!=""}<tr><td align="right" class="helplink">{$helplink}</td></tr>{/if}
  <tr>
    <td valign="top" align="center">{$content}<br></td>
  </tr>
  <tr>
    <td align="center" valign="top">
      {$formstart}
      {foreach from=$buttons item=button}
        &nbsp;{$button}&nbsp;        
      {/foreach}<br><br></td>
      {$formend}
  </tr>
</table>
<!-- END oneframe action_view.tpl -->

