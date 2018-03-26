<form method="POST" id="create-claim-form">
  <input type="hidden" name="tfunction" value="process">
  <input type="hidden" name="page" value="1">
  <table class="create_claim_table">
    <?php fm_draw_leaduser("Internal Use Only", "lead_user", false); ?>
    <?php fm_draw_date("Date of Loss", "date_of_loss", true); ?>
    <?php fm_draw_textfield("Insured's Name", 'customer_name', false, true); ?>
    <?php fm_draw_countryfield('Country', 'insured_country', false, true); ?>
    <?php fm_draw_textfield('Address', 'insured_address', false, true); ?>
    <?php fm_draw_textfield('City', 'insured_city', false, true); ?>
    <?php fm_draw_statefield('State', 'insured_state', false, true); ?>
    <?php fm_draw_textfield('Zip Code', 'insured_zip', false, true); ?>
    <?php fm_draw_textfield("Insured's Phone Number", 'insured_phone_number', false, true); ?>
    <?php fm_draw_textfield('Technician Service Name', 'technician_service_name', false, true); ?>
    <script>
		jQuery(document).ready(function () {
			jQuery("#fm_insured_phone_number").mask("999-999-9999");
		});
	</script>
    <tr>
      <td class="tlabel"><label>Claim Type:<font color="red">*</font> </label></td>
      <td><select id="claim_type" name="claim_type">
          <option value="">-- Choose --</option>
          <option value="residential">Residential</option>
          <option value="commercial">Commercial</option>
        </select>
        &nbsp;
        <select id="claim_product" name="claim_product">
          <option value="">-- Choose --</option>
          <option value="Water">Water</option>
          <option value="Water With Sketch">Water With Sketch</option>
          <!--		<option value="Fire">Fire</option>
											<option value="Fire With Sketch">Fire With Sketch</option>	-->
          <option value="Fire/Smoke Contents Cleaning">Contents Cleaning</option>
          <option value="Fire/Smoke Structure Cleaning">Structure Cleaning</option>
          <option value="Mold">Mold</option>
          <option value="Mold With Sketch">Mold With Sketch</option>
          <option value="Reconstruction">Reconstruction</option>
          <option value="Reconstruction With Sketch">Reconstruction With Sketch</option>
        </select>
        &nbsp; <span id="claim_price"></span>
        <script type="text/javascript">
			(function ($) {
				$(document).ready(function () {
					$('#claim_type').bind('change', function(event){get_claim_invoice_price();});
					$('#claim_product').bind('change', function(event){get_claim_invoice_price();});
					$('#fm_expedite').bind('click', function(event){get_claim_invoice_expedite_price();});
				});
			})(jQuery);
		</script>
      </td>
    </tr>
    <?php fm_draw_checkbox('Check this box if you would like to <b>EXPEDITE</b> your claim.<br/> Additional fee to expedite is: <span id="expedite_price"></span>', 'expedite', false, false); ?>
    <tr>
      <td>&nbsp;</td>
      <td style="padding: 10px 0px; line-height: 25px; vertical-align:middle;"><a class="create-claim-btn" href="javascript:void();" onClick="create_claim_page1()">Create a Claim</a> <span style="margin-top: 2px;">&nbsp; or &nbsp;<a href="<?php echo $GLOBALS['base_url']; ?>/account">cancel </td>
      </span></tr>
    <tr>
      <td colspan=2><div class="note"> <strong>Please Note:</strong> By clicking the "Create a Claim" button, a Terms and Conditions Agreement will appear.<br/>
          To save this file you must first read and then click the "I Agree" button. At that time your
          credit card will be charged and processed for the agreed fee schedule for this claim file. If
          you select the "cancel" button, the claim will be deleted. </div></td>
    </tr>
  </table>
</form>
