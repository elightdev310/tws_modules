<div class="buy-claim-section">
    <h1 class="submit_title">True Water Claims Checklist</h1>
    <p><b>Did you send complete claim documentation? Check off each box marked for required documents and information to submit your claim to True Water Claims.</b></p>
    <p><b>Required Documents Signed by the Policyholder:</b></p>
    <p>The following documents require the policyholder’s signature before submitting a claims purchase request.</p>
    <div style="margin:10px 0px 10px 50px;">
        <table>
    <!--    <tr>-->
    <!--      <td><input type="checkbox" id="checked_1" onclick="check_options_for_submit();"></td>-->
    <!--      <td><label for="checked_1">Fax Cover Sheet</label></td>-->
    <!--    </tr>-->
            <tr>
                <td><input type="checkbox" id="checked_11" class="buy-checkbox" onclick="check_all_checkboxes();"></td>
                <td><label for="checked_11">An executed True Water Claims Water Mitigation Service Contract.</label></td>
            </tr>
            <tr>
                <td><input type="checkbox" id="checked_12" class="buy-checkbox" onclick="check_all_checkboxes();"></td>
                <td><label for="checked_12">An executed True Water Claims Certificate of Satisfaction/Direction To Pay.</label></td>
            </tr>
        </table>
    </div>

    <p><b>Required Project Information:</b></p>
    <div style="margin:10px 0px 10px 50px;">
        <table>
            <tr>
                <td><input type="checkbox" id="checked_21" class="buy-checkbox" onclick="check_all_checkboxes();"></td>
                <td><label for="checked_21">A completed True Water Claims Purchase Request and User Agreement.</label></td>
            </tr>
            <tr>
                <td><input type="checkbox" id="checked_22" class="buy-checkbox" onclick="check_all_checkboxes();"></td>
                <td><label for="checked_22">A completed True Water Claims Release and Reassignment of Claim Rights document</label></td>
            </tr>
            <tr>
                <td><input type="checkbox" id="checked_23" class="buy-checkbox" onclick="check_all_checkboxes();"></td>
                <td><label for="checked_23">A completed True Water Claims Claims Processing Form.</label></td>
            </tr>
            <tr>
                <td><input type="checkbox" id="checked_24" class="buy-checkbox" onclick="check_all_checkboxes();"></td>
                <td><label for="checked_24">A completed True Water Claims Water Damage Scope Sheet (per room), including moisture readings (per room).</label></td>
            </tr>
            <tr>
                <td><input type="checkbox" id="checked_25" class="buy-checkbox" onclick="check_all_checkboxes();"></td>
                <td><label for="checked_25">Photo of the front of the home to identify the property.</label></td>
            </tr>

            <tr>
                <td><input type="checkbox" id="checked_26" class="buy-checkbox" onclick="check_all_checkboxes();"></td>
                <td><label for="checked_26">Photo of the cause and origin of the loss.</label></td>
            </tr>
            <tr>
                <td><input type="checkbox" id="checked_27" class="buy-checkbox" onclick="check_all_checkboxes();"></td>
                <td><label for="checked_27">Photo of the policyholder’s driver’s license</label></td>
            </tr>
            <tr>
                <td><input type="checkbox" id="checked_28" class="buy-checkbox" onclick="check_all_checkboxes();"></td>
                <td><label for="checked_28">Photo of the insurance policy declaration page.</label></td>
            </tr>
            <tr>
                <td><input type="checkbox" id="checked_29" class="buy-checkbox" onclick="check_all_checkboxes();"></td>
                <td><label for="checked_29">Pre-mitigation photos of all damages by room, dry standard from an unaffected room and initial moisture readings in affected rooms.</label></td>
            </tr>
            <tr>
                <td><input type="checkbox" id="checked_210" class="buy-checkbox" onclick="check_all_checkboxes();"></td>
                <td><label for="checked_210">During mitigation photos of equipment placed and services provided.</label></td>
            </tr>
            <tr>
                <td><input type="checkbox" id="checked_211" class="buy-checkbox" onclick="check_all_checkboxes();"></td>
                <td><label for="checked_211">Post mitigation photos showing final moisture readings and property conditions on the final day of service.</label></td>
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

    <p>By pressing "Continue", you are stating that you have submitted all completed claim documents and information to True Water Claims. You may click "Cancel" if you do not want to continue or still need to send more documents or information</p>
    <p>If your documents are incomplete or information is missing, your claim cannot be processed and will be returned.</p>
    <p>Any questions, email <a href="mailto:​inquiries@truewaterclaims.com" >​inquiries@truewaterclaims.com</a>​ or call us at 407-720-4003.</p>
</div>