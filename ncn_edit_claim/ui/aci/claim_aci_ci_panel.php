<?php
/**
 * ACI - Claim Information Panel
 *
 * @var
 *     $claim_id
 *     $data
 */
global $base_url;
?>
<div class="table-responsive">
<table id="aci_ci_content" class="table">
<tbody>
    <tr>
        <td class="td-label"><label for="ci_us_to_build">You would like us to build:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" name="ci[us_to_build]" id="ci_utb_invoice_for_already_provided" value="Invoice for Services Already Provided" <?php if(isset($data['us_to_build'])&&$data['us_to_build']=='Invoice for Services Already Provided') { echo "checked='checked'";} ?> /><label for="ci_utb_invoice_for_already_provided">Invoice for Services Already Provided</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[us_to_build]" id="ci_utb_estimate" value="Estimate" <?php if(isset($data['us_to_build'])&&$data['us_to_build']=='Estimate') { echo "checked='checked'";} ?> /><label for="ci_utb_estimate">Estimate</label></li>
            </ul>
        </td>
    <td class="reset-radio"><input type="button" class="btn radio-reset-button" value="Reset" /></td>
    </tr>
    <tr>
        <td class="td-label"><label for="ci_service_call">Service Call:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" name="ci[service_call]" id="ci_sc_normal_business" value="Normal Business Hrs" <?php if(isset($data['service_call'])&&$data['service_call']=='Normal Business Hrs') { echo "checked='checked'";} ?> /><label for="ci_sc_normal_business">Normal Business Hrs</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[service_call]" id="ci_sc_after" value="After Hours" <?php if(isset($data['service_call'])&&$data['service_call']=='After Hours') { echo "checked='checked'";} ?> /><label for="ci_sc_after">After Hours</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[service_call]" id="ci_sc_none" value="None" <?php if(isset($data['service_call'])&&$data['service_call']=='None') { echo "checked='checked'";} ?> /><label for="ci_sc_none">None</label></li>
            </ul>
        </td>
    <td class="reset-radio"><input type="button" class="btn radio-reset-button" value="Reset" /></td>
    </tr>
    <tr>
        <td class="td-label"><label for="ci_date_of_service">Date of Service:</label></td>
        <td><div class="field-item text-field-item small-size"><input type="text" name="ci[date_of_service]" id="ci_date_of_service" value="<?php echo isset($data['date_of_service'])?$data['date_of_service']:''; ?>" onkeypress="return isDateKey(event);" /></div></td>
    <td class="reset-radio"><input type="button" class="btn  radio-reset-button" value="Reset" /></td>
    </tr>
    <tr>
        <td class="td-label"><label for="ci_date_of_completion">Date of Completion:</label></td>
        <td><div class="field-item text-field-item small-size"><input type="text" name="ci[date_of_completion]" id="ci_date_of_completion" value="<?php echo isset($data['date_of_completion'])?$data['date_of_completion']:''; ?>" onkeypress="return isDateKey(event);" /></div></td>
    <td class="reset-radio"><input type="button" class="btn  radio-reset-button" value="Reset" /></td>
    </tr>
<!--    <tr>
        <td class="td-label"><label for="ci_type_of_claim">Type of Claim:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" name="ci[type_of_claim]" id="ci_toc_residential" value="Residential" <?php if(isset($data['type_of_claim'])&&$data['type_of_claim']=='Residential') { echo "checked='checked'";} ?> /><label for="ci_toc_residential">Residential</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[type_of_claim]" id="ci_toc_commercial" value="Commercial" <?php if(isset($data['type_of_claim'])&&$data['type_of_claim']=='Commercial') { echo "checked='checked'";} ?> /><label for="ci_toc_commercial">Commercial</label></li>
            </ul>
        </td>
    <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
    </tr>
    <tr>
        <td class="td-label"><label for="ci_type_of_loss">Type of Loss:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" name="ci[type_of_loss]" id="ci_tol_water" value="Water" <?php if(isset($data['type_of_loss'])&&$data['type_of_loss']=='Water') { echo "checked='checked'";} ?> /><label for="ci_tol_water">Water</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[type_of_loss]" id="ci_tol_mold" value="Mold" <?php if(isset($data['type_of_loss'])&&$data['type_of_loss']=='Mold') { echo "checked='checked'";} ?> /><label for="ci_tol_mold">Mold</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[type_of_loss]" id="ci_tol_reconstruction" value="Reconstruction" <?php if(isset($data['type_of_loss'])&&$data['type_of_loss']=='Reconstruction') { echo "checked='checked'";} ?> /><label for="ci_tol_reconstruction">Reconstruction</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[type_of_loss]" id="ci_tol_structure_cleaning" value="Structure Cleaning" <?php if(isset($data['type_of_loss'])&&$data['type_of_loss']=='Structure Cleaning') { echo "checked='checked'";} ?> /><label for="ci_tol_structure_cleaning">Structure Cleaning</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[type_of_loss]" id="ci_tol_contents_cleaning" value="Contents Cleaning" <?php if(isset($data['type_of_loss'])&&$data['type_of_loss']=='Contents Cleaning') { echo "checked='checked'";} ?> /><label for="ci_tol_contents_cleaning">Contents Cleaning</label></li>
            </ul>
        </td>
    <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
    </tr>  -->
    <tr>
        <td class="td-label"><label for="ci_cause_origin_of_loss">Cause and Origin of Loss:</label></td>
        <td><div class="field-item text-field-item"><input type="text" name="ci[cause_origin_of_loss]" id="ci_cause_origin_of_loss" value="<?php echo isset($data['cause_origin_of_loss'])?$data['cause_origin_of_loss']:''; ?>" /></div></td>
    <td class="reset-radio"><input type="button" class="btn  radio-reset-button" value="Reset" /></td>
    </tr>
<!--<tr>
        <td class="td-label"><label for="ci_sketch_purchased">Sketch Purchased:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" name="ci[sketch_purchased]" id="ci_sp_yes" value="Yes" <?php if(isset($data['sketch_purchased'])&&$data['sketch_purchased']=='Yes') { echo "checked='checked'";} ?> /><label for="ci_sp_yes">Yes</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[sketch_purchased]" id="ci_sp_no" value="No" <?php if(isset($data['sketch_purchased'])&&$data['sketch_purchased']=='No') { echo "checked='checked'";} ?> /><label for="ci_sp_no">No</label></li>
            </ul>
        </td>
    <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
    </tr> -->
    <tr>
        <td class="td-label"><label for="ci_category_of_water">Category of Water:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" name="ci[category_of_water]" id="ci_cow_1" value="1" <?php if(isset($data['category_of_water'])&&$data['category_of_water']=='1') { echo "checked='checked'";} ?> /><label for="ci_cow_1">1</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[category_of_water]" id="ci_cow_2" value="2" <?php if(isset($data['category_of_water'])&&$data['category_of_water']=='2') { echo "checked='checked'";} ?> /><label for="ci_cow_2">2</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[category_of_water]" id="ci_cow_3" value="3" <?php if(isset($data['category_of_water'])&&$data['category_of_water']=='3') { echo "checked='checked'";} ?> /><label for="ci_cow_3">3</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[category_of_water]" id="ci_cow_no_water" value="No Water" <?php if(isset($data['category_of_water'])&&$data['category_of_water']=='No Water') { echo "checked='checked'";} ?> /><label for="ci_cow_no_water">No Water</label></li>
            </ul>
        </td>
    <td class="reset-radio"><input type="button" class="btn  radio-reset-button" value="Reset" /></td>
    </tr>
    <tr>
        <td class="td-label"><label for="ci_class_of_loss">Class of Loss:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" name="ci[class_of_loss]" id="ci_col_1" value="1" <?php if(isset($data['class_of_loss'])&&$data['class_of_loss']=='1') { echo "checked='checked'";} ?> /><label for="ci_col_1">1</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[class_of_loss]" id="ci_col_2" value="2" <?php if(isset($data['class_of_loss'])&&$data['class_of_loss']=='2') { echo "checked='checked'";} ?> /><label for="ci_col_2">2</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[class_of_loss]" id="ci_col_3" value="3" <?php if(isset($data['class_of_loss'])&&$data['class_of_loss']=='3') { echo "checked='checked'";} ?> /><label for="ci_col_3">3</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[class_of_loss]" id="ci_col_4" value="4" <?php if(isset($data['class_of_loss'])&&$data['class_of_loss']=='4') { echo "checked='checked'";} ?> /><label for="ci_col_3">4</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[class_of_loss]" id="ci_col_no_water" value="No Water" <?php if(isset($data['class_of_loss'])&&$data['class_of_loss']=='No Water') { echo "checked='checked'";} ?> /><label for="ci_col_no_water">No Water</label></li>
            </ul>
        </td>
    <td class="reset-radio"><input type="button" class="btn  radio-reset-button" value="Reset" /></td>
    </tr>
    <tr>
        <td class="td-label"><label for="ci_number_of_stories_affected">Number of Stories Affected:</label></td>
        <td><div class="field-item text-field-item small-size"><input type="text" name="ci[number_of_stories_affected]" id="ci_number_of_stories_affected" value="<?php echo isset($data['number_of_stories_affected'])?$data['number_of_stories_affected']:''; ?>"  onkeypress="return isNumberKey(event, false);" /></div></td>
    <td class="reset-radio"><input type="button" class="btn  radio-reset-button" value="Reset" /></td>
    </tr>
    <tr>
        <td class="td-label"><label for="ci_deductible">Deductible:</label></td>
        <td><div class="field-item text-field-item small-size"><span> $ </span><input type="text" name="ci[deductible]" id="ci_deductible" value="<?php echo isset($data['deductible'])?$data['deductible']:''; ?>" onkeypress="return isNumberKey(event, true);" /></div></td>
    <td class="reset-radio"><input type="button" class="btn  radio-reset-button" value="Reset" /></td>
    </tr>
    <tr>
        <td class="td-label"><label for="ci_deductible_collected">Deductible Collected:</label></td>
        <td><div class="field-item text-field-item small-size"><span> $ </span><input type="text" name="ci[deductible_collected]" id="ci_deductible_collected" value="<?php echo isset($data['deductible_collected'])?$data['deductible_collected']:''; ?>" /></div></td>
    <td class="reset-radio"><input type="button" class="btn  radio-reset-button" value="Reset" /></td>
    </tr>
</tbody>
</table>
</div>

<script>
jQuery(document).ready(function() { 
    bind_ncn_admin_aci_ci_radio_reset_buttons(); 
    bind_ncn_admin_aci_ci_cboxlabel_buttons(); 

    ncn_admin_aci_claim_information_datepicker('#ci_date_of_service');
    ncn_admin_aci_claim_information_datepicker('#ci_date_of_completion');
});

function ncn_admin_aci_claim_information_datepicker(objStr) {
    jQuery( objStr ).datepicker({
        showOn: "button",
        buttonImage: "<?php echo $base_url."/".drupal_get_path('module', 'ncn_report')."/images/calendar.gif"; ?>",
        buttonImageOnly: true,
        dateFormat: 'mm/dd/yy',
    });
}
</script>
