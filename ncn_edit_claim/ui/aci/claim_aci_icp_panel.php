<?php
/**
 * ACI - Insurance Claim Policy Panel
 *
 * @var
 *     $claim_id
 *     $data
 */
global $base_url;
?>
<div class="aci-icp-panel">
    <div class="form-group">
        <div for="icp_insured_name" class="col-sm-4 control-label">Insured's Name:</div>
        <div class="col-sm-8">
            <input type="text" class="form-control" name="icp[insured_name]" id="icp_insured_name" value="<?php echo isset($data['insured_name'])?$data['insured_name']:''; ?>" />
        </div>
    </div>
    <div class="form-group">
        <div for="icp_insured_name" class="col-sm-4 control-label">Insurance Company:</div>
        <div class="col-sm-8">
            <input type="text" class="form-control" name="icp[insurance_company]" id="icp_insurance_company" value="<?php echo isset($data['insurance_company'])?$data['insurance_company']:''; ?>" />
        </div>
    </div>
    <div class="form-group">
        <div for="icp_insured_name" class="col-sm-4 control-label">Insurance Company Phone:</div>

        <div class="field-item text-field-item inline-field field-phone-number col-sm-5 col-xs-7"><input type="text" class="form-control" name="icp[insurance_company_phone][number]" id="icp_insurance_company_phone_number" onkeypress="return isNumberKey(event, false);" value="<?php echo isset($data['insurance_company_phone']['number'])?$data['insurance_company_phone']['number']:''; ?>" /></div>
        <div class="field-item text-field-item inline-field field-phone-ext col-sm-3 col-xs-5"><input type="text" class="form-control" name="icp[insurance_company_phone][ext]" id="icp_insurance_company_phone_ext" value="<?php echo isset($data['insurance_company_phone']['ext'])?$data['insurance_company_phone']['ext']:''; ?>" /></div>
    </div>
    <div class="form-group">
        <div for="icp_insurance_agent" class="col-sm-4 control-label">Insurance Agent:</div>
        <div class="col-sm-8">
            <input type="text" class="form-control" name="icp[insurance_agent]" id="icp_insurance_agent" value="<?php echo isset($data['insurance_agent'])?$data['insurance_agent']:''; ?>" />
        </div>
    </div>
    <div class="form-group">
        <div for="icp_insurance_agent_phone" class="col-sm-4 control-label">Insurance Agent Phone:</div>

        <div class="field-item text-field-item inline-field field-phone-number col-sm-5 col-xs-7"><input type="text" class="form-control" name="icp[insurance_agent_phone][number]" id="icp_insurance_agent_phone_number" onkeypress="return isNumberKey(event, false);" value="<?php echo isset($data['insurance_agent_phone']['number'])?$data['insurance_agent_phone']['number']:''; ?>" /></div>
        <div class="field-item text-field-item inline-field field-phone-ext col-sm-3 col-xs-5"><input type="text" class="form-control" name="icp[insurance_agent_phone][ext]" id="icp_insurance_agent_phone_ext" value="<?php echo isset($data['insurance_agent_phone']['ext'])?$data['insurance_agent_phone']['ext']:''; ?>" /></div>
    </div>
    <div class="form-group">
        <div for="icp_date_of_loss" class="col-sm-4 control-label">Date of Loss:</div>
        <div class="col-sm-8 date-control">
            <input type="text" class="form-control" name="icp[date_of_loss]" id="icp_date_of_loss" value="<?php echo isset($data['date_of_loss'])?$data['date_of_loss']:''; ?>" onkeypress="return isDateKey(event);" />
        </div>
    </div>

    <div class="form-group">
        <div for="icp_field_adjuster" class="col-sm-4 control-label">Field Adjuster:</div>
        <div class="col-sm-8">
            <input type="text" class="form-control" name="icp[field_adjuster]" id="icp_field_adjuster" value="<?php echo isset($data['field_adjuster'])?$data['field_adjuster']:''; ?>" />
        </div>
    </div>
    <div class="form-group">
        <div for="icp_field_adjuster_email" class="col-sm-4 control-label">Field Adjuster Email:</div>
        <div class="col-sm-8">
            <input type="text" class="form-control" name="icp[field_adjuster_email]" id="icp_field_adjuster_email" value="<?php echo isset($data['field_adjuster_email'])?$data['field_adjuster_email']:''; ?>" />
        </div>
    </div>
    <div class="form-group">
        <div for="icp_field_adjuster_phone" class="col-sm-4 control-label">Phone:</div>

        <div class="field-item text-field-item inline-field field-phone-number col-sm-5 col-xs-7"><input type="text" class="form-control" name="icp[field_adjuster_phone][number]" id="icp_field_adjuster_phone_number" onkeypress="return isNumberKey(event, false);" value="<?php echo isset($data['field_adjuster_phone']['number'])?$data['field_adjuster_phone']['number']:''; ?>" /></div>
        <div class="field-item text-field-item inline-field field-phone-ext col-sm-3 col-xs-5"><input type="text" class="form-control" name="icp[field_adjuster_phone][ext]" id="icp_field_adjuster_phone_ext" value="<?php echo isset($data['field_adjuster_phone']['ext'])?$data['field_adjuster_phone']['ext']:''; ?>" /></div>
    </div>
    <div class="form-group">
        <div for="icp_field_adjuster_alternate_phone" class="col-sm-4 control-label">Alternate Phone:</div>

        <div class="field-item text-field-item inline-field field-phone-number col-sm-5 col-xs-7"><input type="text" class="form-control" name="icp[field_adjuster_alternate_phone][number]" id="icp_field_adjuster_alternate_phone" class="phone-style-field" onkeypress="return isNumberKey(event, false);" value="<?php echo isset($data['field_adjuster_alternate_phone']['number'])?$data['field_adjuster_alternate_phone']['number']:''; ?>" /></div>
        <div class="field-item text-field-item inline-field field-phone-ext col-sm-3 col-xs-5"><input type="text" class="form-control" name="icp[field_adjuster_alternate_phone][ext]" id="icp_field_adjuster_alternate_phone_ext" value="<?php echo isset($data['field_adjuster_alternate_phone']['ext'])?$data['field_adjuster_alternate_phone']['ext']:''; ?>" /></div>
    </div>


    <div class="form-group">
        <div for="icp_claim_adjuster" class="col-sm-4 control-label">Claim Adjuster:</div>
        <div class="col-sm-8">
            <input type="text" class="form-control" name="icp[claim_adjuster]" id="icp_claim_adjuster" value="<?php echo isset($data['claim_adjuster'])?$data['claim_adjuster']:''; ?>" />
        </div>
    </div>
    <div class="form-group">
        <div for="icp_adjuster_email" class="col-sm-4 control-label">Claim Adjuster Email:</div>
        <div class="col-sm-8">
            <input type="text" class="form-control" name="icp[adjuster_email]" id="icp_adjuster_email" value="<?php echo isset($data['adjuster_email'])?$data['adjuster_email']:''; ?>" />
        </div>
    </div>
    <div class="form-group">
        <div for="icp_insurance_adjuster_phone" class="col-sm-4 control-label">Phone:</div>

        <div class="field-item text-field-item inline-field field-phone-number col-sm-5 col-xs-7"><input type="text" class="form-control" name="icp[adjuster_phone][number]" id="icp_adjuster_phone_number" onkeypress="return isNumberKey(event, false);" value="<?php echo isset($data['adjuster_phone']['number'])?$data['adjuster_phone']['number']:''; ?>" /></div>
        <div class="field-item text-field-item inline-field field-phone-ext col-sm-3 col-xs-5"><input type="text" class="form-control" name="icp[adjuster_phone][ext]" id="icp_adjuster_phone_ext" value="<?php echo isset($data['adjuster_phone']['ext'])?$data['adjuster_phone']['ext']:''; ?>" /></div>
    </div>
    <div class="form-group">
        <div for="icp_adjuster_alternate_phone_number" class="col-sm-4 control-label">Alternate Phone:</div>

        <div class="field-item text-field-item inline-field field-phone-number col-sm-5 col-xs-7"><input type="text" class="form-control" name="icp[adjuster_alternate_phone_number][number]" id="icp_adjuster_alternate_phone_number" class="phone-style-field" onkeypress="return isNumberKey(event, false);" value="<?php echo isset($data['adjuster_alternate_phone_number']['number'])?$data['adjuster_alternate_phone_number']['number']:''; ?>" /></div>
        <div class="field-item text-field-item inline-field field-phone-ext col-sm-3 col-xs-5"><input type="text" class="form-control" name="icp[adjuster_alternate_phone][ext]" id="icp_adjuster_alternate_phone_ext" value="<?php echo isset($data['adjuster_alternate_phone']['ext'])?$data['adjuster_alternate_phone']['ext']:''; ?>" /></div>
    </div>
    <div class="form-group">
        <div for="icp_insurance_fax" class="col-sm-4 control-label">Insurance Fax:</div>
        <div class="col-sm-8">
            <input type="text" class="form-control" name="icp[insurance_fax]" id="icp_insurance_fax" class="phone-style-field" onkeypress="return isNumberKey(event, false);" value="<?php echo isset($data['insurance_fax'])?$data['insurance_fax']:''; ?>" />
        </div>
    </div>

    <div class="form-group">
        <div for="icp_effective_policy_date" class="col-sm-4 control-label">Effective Policy Date:</div>
        <div class="field-item text-field-item inline-field field-date-from col-sm-4 date-control"><input type="text" class="form-control" name="icp[effective_policy_date][from]" id="icp_effective_policy_date_from" value="<?php echo isset($data['effective_policy_date']['from'])?$data['effective_policy_date']['from']:''; ?>" onkeypress="return isDateKey(event);" /></div>
        <div class="field-item text-field-item inline-field field-date-to col-sm-4 date-control"><input type="text" class="form-control" name="icp[effective_policy_date][to]" id="icp_effective_policy_date_to" value="<?php echo isset($data['effective_policy_date']['to'])?$data['effective_policy_date']['to']:''; ?>" onkeypress="return isDateKey(event);" /></div>
    </div>

    <div class="form-group">
        <div for="icp_policy_number" class="col-sm-4 control-label">Policy Number:</div>
        <div class="col-sm-8">
            <input type="text" class="form-control" name="icp[policy_number]" id="icp_policy_number" value="<?php echo isset($data['policy_number'])?$data['policy_number']:''; ?>" />
        </div>
    </div>
    <div class="form-group">
        <div for="icp_insurance_claim_number" class="col-sm-4 control-label">Insurance Claim Number:</div>
        <div class="col-sm-8">
            <input type="text" class="form-control" name="icp[insurance_claim_number]" id="icp_insurance_claim_number" value="<?php echo isset($data['insurance_claim_number'])?$data['insurance_claim_number']:''; ?>" />
        </div>
    </div>

</div>

<script>
jQuery(function() {
  ncn_admin_aci_icp_datepicker('#icp_date_of_loss');
  ncn_admin_aci_icp_datepicker('#icp_effective_policy_date_from');
  ncn_admin_aci_icp_datepicker('#icp_effective_policy_date_to');
});
function ncn_admin_aci_icp_datepicker(objStr) {
  jQuery( objStr ).datepicker({
    showOn: "button",
    buttonImage: "<?php echo $base_url."/".drupal_get_path('module', 'ncn_report')."/images/calendar.gif"; ?>",
    buttonImageOnly: true,
    dateFormat: 'mm/dd/yy',
  });
}

</script>
