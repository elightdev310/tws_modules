<h1 class="submit_title">Claims Checklist: Check each task completed</h1>
<div>
  <p>Did you send complete Claim Documentation? Check off each claim documentation you have sent to have Net Claims Now start working on your invoice.</p>
</div>
<div style="margin-left:50px;">
  <table>
    <tr>
      <td><input type="checkbox" id="checked_1" onclick="check_options_for_submit();"></td>
      <td><label for="checked_1">Fax Cover Sheet</label></td>
    </tr>
    <tr>
      <td><input type="checkbox" id="checked_2" onclick="check_options_for_submit();"></td>
      <td><label for="checked_2">Claims Processing Form</label></td>
    </tr>
    <tr>
      <td><input type="checkbox" id="checked_3" onclick="check_options_for_submit();"></td>
      <td><label for="checked_3">1 Scope sheet per room</label></td>
    </tr>
    <tr>
      <td><input type="checkbox" id="checked_4" onclick=""></td>
      <td><label for="checked_4">Service Contract that includes Assignment of Benefits*</label></td>
    </tr>
    <tr>
      <td><input type="checkbox" id="checked_5" onclick=""></td>
      <td><label for="checked_5">Certificate of Satisfaction and Completion*</label></td>
    </tr>
    <tr>
      <td><input type="checkbox" id="checked_6" onclick=""></td>
      <td><label for="checked_6">Photos uploaded to Photo Album in Virtual Office**</label></td>
    </tr>
    <tr>
      <td colspan=2>&nbsp;</td>
    </tr>
    <tr>
      <td colspan="2" align="center">
<!--          <input type="button" value="Cancel" onclick="parent.$.colorbox.close();" />-->
          <input type="button" value="Cancel" onclick="jQuery('#cboxClose').trigger('click');" />
        &nbsp;
        <input type="button" value="Continue >" id="continue_button" disabled="disabled" onclick="parent.window.location='<?= url('account/submit_claim/'.$claim_id); ?>';" />
      </td>
    </tr>
    <tr>
      <td colspan=2>&nbsp;</td>
    </tr>
  </table>
</div>
<div>
  <p>* Not required but highly recommended. May have used your company's own service contract and/or certificate of completion.</p>
  <p>** Not required but highly recommended. You will not be able to add more photos after pressing "Continue".</p>
  <p>By pressing "Continue" you are stating that you have submitted all completed claim documents to Net Claims Now and uploaded all available photos for this claim. You may click "cancel" if you do not want to continue or have not sent all of your documents.</p>
  <p>If your claim documents are incomplete and/or documented poorly, your documents will be required to be clarified and/or rewritten for further processing.</p>
  <p>We will send you an email with what information is needed. Please send your clarifications/answers to fax@netclaimsnow.com to add information to your claim documents.</p>
</div>
