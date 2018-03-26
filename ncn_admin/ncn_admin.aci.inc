<?php
/*****************************************************************************/
/*  Additional Claim Information
/*****************************************************************************/

/**
 * ACI Render - "Property Owner Information" Section
 */ 
function ncn_admin_aci_render_property_owner_info($claim_id, $section) {
    $data = array();
    if (isset($_POST['poi'])) {
        $data = $_POST['poi'];
    } else {
        $data = ncn_admin_get_aci_property_owner_info($claim_id);
    }
    
    ob_start();
?>
<table>
    <tr>
        <td class="td-label"><label for="poi_insured_name">Owner/Insured Name:</label></td>
        <td><div class="field-item text-field-item"><input type="text" name="poi[insured_name]" id="poi_insured_name" value="<?php echo isset($data['insured_name'])?$data['insured_name']:''; ?>" /></div></td>
    </tr>
    <tr>
        <td class="td-label"><label for="poi_insured_address">Property Loss Address:</label></td>
        <td><div class="field-item text-field-item"><input type="text" name="poi[insured_address]" id="poi_insured_address" value="<?php echo isset($data['insured_address'])?$data['insured_address']:''; ?>" /></div></td>
    </tr>
    <tr>
        <td class="td-label"><label for="poi_apt_bldg">Apt/Suite/Bldg:</label></td>
        <td><div class="field-item text-field-item"><input type="text" name="poi[apt_bldg]" id="poi_apt_bldg" value="<?php echo isset($data['apt_bldg'])?$data['apt_bldg']:''; ?>" /></div></td>
    </tr>
    <tr>
        <td class="td-label"><label for="poi_insured_city">City:</label></td>
        <td><div class="field-item text-field-item"><input type="text" name="poi[insured_city]" id="poi_insured_city" value="<?php echo isset($data['insured_city'])?$data['insured_city']:''; ?>" /></div></td>
    </tr>
    <tr>
        <td class="td-label"><label for="poi_insured_state">State:</label></td>
        <td><div class="field-item text-field-item">
            <?php print ncn_loc_state(array(
                            'name'      => 'poi[insured_state]', 
                            'id'        => 'poi_insured_state', 
                            'class'     => '', 
                            'disabled'  => false, 
                            'required'  => false, 
                            'state'     => isset($data['insured_state'])?$data['insured_state']:'', 
                            'country'   => 'US', 
                        ));
            ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="td-label"><label for="poi_insured_zip">Zip Code:</label></td>
        <td><div class="field-item text-field-item"><input type="text" name="poi[insured_zip]" id="poi_insured_zip" value="<?php echo isset($data['insured_zip'])?$data['insured_zip']:''; ?>" /></div></td>
    </tr>

    <tr>
        <td class="td-label"><label for="poi_billing_address">Billing Address (if different):</label></td>
        <td><div class="field-item text-field-item"><input type="text" name="poi[billing_address]" id="poi_billing_address" value="<?php echo isset($data['billing_address'])?$data['billing_address']:''; ?>" /></div></td>
    </tr>
    <tr>
        <td class="td-label"><label for="poi_billing_apt_bldg">Apt/Suite/Bldg:</label></td>
        <td><div class="field-item text-field-item"><input type="text" name="poi[billing_apt_bldg]" id="poi_billing_apt_bldg" value="<?php echo isset($data['billing_apt_bldg'])?$data['billing_apt_bldg']:''; ?>" /></div></td>
    </tr>
    <tr>
        <td class="td-label"><label for="poi_billing_city">City:</label></td>
        <td><div class="field-item text-field-item"><input type="text" name="poi[billing_city]" id="poi_billing_city" value="<?php echo isset($data['billing_city'])?$data['billing_city']:''; ?>" /></div></td>
    </tr>
    <tr>
        <td class="td-label"><label for="poi_billing_state">State:</label></td>
        <td><div class="field-item text-field-item">
            <?php print ncn_loc_state(array(
                            'name'      => 'poi[billing_state]', 
                            'id'        => 'poi_billing_state', 
                            'class'     => '', 
                            'disabled'  => false, 
                            'required'  => false, 
                            'state'     => isset($data['billing_state'])?$data['billing_state']:'', 
                            'country'   => 'US', 
                        ));
            ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="td-label"><label for="poi_billing_zip">Zip Code:</label></td>
        <td><div class="field-item text-field-item"><input type="text" name="poi[billing_zip]" id="poi_billing_zip" value="<?php echo isset($data['billing_zip'])?$data['billing_zip']:''; ?>" /></div></td>
    </tr>

    <tr>
        <td class="td-label">
            <label for="poi_phone_0_number">Phone
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" id="poi_phone_0_type_home" name="poi[phone][0][type]" value="Home" <?php if(isset($data['phone'][0]['type'])&&$data['phone'][0]['type']=='Home') { echo "checked='checked'";} ?> ><label for="poi_phone_0_type_home">Home</label></li>
                <li class="inline-field-item"><input type="radio" id="poi_phone_0_type_work" name="poi[phone][0][type]" value="Work" <?php if(isset($data['phone'][0]['type'])&&$data['phone'][0]['type']=='Work') { echo "checked='checked'";} ?> ><label for="poi_phone_0_type_work">Work</label></li>
                <li class="inline-field-item"><input type="radio" id="poi_phone_0_type_cell" name="poi[phone][0][type]" value="Cell" <?php if(isset($data['phone'][0]['type'])&&$data['phone'][0]['type']=='Cell') { echo "checked='checked'";} ?> ><label for="poi_phone_0_type_cell">Cell:</label></li>
            </ul></label>
        </td>
        <td><div class="field-item text-field-item"><input type="text" name="poi[phone][0][number]" id="poi_phone_0_number" class="phone-style-field" onkeypress="return isNumberKey(event, false);" value="<?php echo isset($data['phone'][0]['number'])?$data['phone'][0]['number']:''; ?>" /></div></td>
    </tr>
    <tr>
        <td class="td-label">
            <label for="poi_phone_1_number">Phone
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" id="poi_phone_1_type_home" name="poi[phone][1][type]" value="Home" <?php if(isset($data['phone'][1]['type'])&&$data['phone'][1]['type']=='Home') { echo "checked='checked'";} ?> ><label for="poi_phone_1_type_home">Home</label></li>
                <li class="inline-field-item"><input type="radio" id="poi_phone_1_type_work" name="poi[phone][1][type]" value="Work" <?php if(isset($data['phone'][1]['type'])&&$data['phone'][1]['type']=='Work') { echo "checked='checked'";} ?> ><label for="poi_phone_1_type_work">Work</label></li>
                <li class="inline-field-item"><input type="radio" id="poi_phone_1_type_cell" name="poi[phone][1][type]" value="Cell" <?php if(isset($data['phone'][1]['type'])&&$data['phone'][1]['type']=='Cell') { echo "checked='checked'";} ?> ><label for="poi_phone_1_type_cell">Cell:</label></li>
            </ul></label>
        </td>
        <td><div class="field-item text-field-item"><input type="text" name="poi[phone][1][number]" id="poi_phone_1_number" class="phone-style-field" onkeypress="return isNumberKey(event, false);" value="<?php echo isset($data['phone'][1]['number'])?$data['phone'][1]['number']:''; ?>" /></div></td>
    </tr>
    <tr>
        <td class="td-label">
            <label for="poi_phone_2_number">Phone
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" id="poi_phone_2_type_home" name="poi[phone][2][type]" value="Home" <?php if(isset($data['phone'][2]['type'])&&$data['phone'][2]['type']=='Home') { echo "checked='checked'";} ?> ><label for="poi_phone_2_type_home">Home</label></li>
                <li class="inline-field-item"><input type="radio" id="poi_phone_2_type_work" name="poi[phone][2][type]" value="Work" <?php if(isset($data['phone'][2]['type'])&&$data['phone'][2]['type']=='Work') { echo "checked='checked'";} ?> ><label for="poi_phone_2_type_work">Work</label></li>
                <li class="inline-field-item"><input type="radio" id="poi_phone_2_type_cell" name="poi[phone][2][type]" value="Cell" <?php if(isset($data['phone'][2]['type'])&&$data['phone'][2]['type']=='Cell') { echo "checked='checked'";} ?> ><label for="poi_phone_2_type_cell">Cell:</label></li>
            </ul></label>
        </td>
        <td><div class="field-item text-field-item"><input type="text" name="poi[phone][2][number]" id="poi_phone_2_number" class="phone-style-field" onkeypress="return isNumberKey(event, false);" value="<?php echo isset($data['phone'][2]['number'])?$data['phone'][2]['number']:''; ?>" /></div></td>
    </tr>
    <tr>
        <td class="td-label"><label for="poi_email">Email:</label></td>
        <td><div class="field-item text-field-item"><input type="text" name="poi[property_owner_email]" id="poi_property_owner_email" value="<?php echo isset($data['property_owner_email'])?$data['property_owner_email']:''; ?>" /></div></td>
    </tr>
</table>
<?php   
    $output = ob_get_contents();
    ob_end_clean();
    
    return $output;
}

function ncn_admin_get_aci_property_owner_info($claim_id) {
    $data = get_ncn_data_extra($claim_id, 'aci_property_owner_info');
    if ($data) {
        $data = unserialize($data);
    } else {
        $data = array();
    }
    
    // Populate
    if (!isset($data['insured_name']))      {  $data['insured_name']    = ncn_cd($claim_id, 'customer_name'); }
    if (!isset($data['insured_address']))   { $data['insured_address']  = ncn_cd($claim_id, 'insured_address'); }
    if (!isset($data['insured_city']))      { $data['insured_city']     = ncn_cd($claim_id, 'insured_city'); }
    if (!isset($data['insured_state']))     { $data['insured_state']    = ncn_cd($claim_id, 'insured_state'); }
    if (!isset($data['insured_zip']))       { $data['insured_zip']      = ncn_cd($claim_id, 'insured_zip'); }
    if (!isset($data['phone'][0]['type']))  { $data['phone'][0]['type'] = 'Home'; }
    if (!isset($data['phone'][0]['number'])){ $data['phone'][0]['number']= ncn_cd($claim_id, 'insured_phone_number'); }
    
    return $data;
}

function ncn_admin_sync_claim_data($claim_id) {
  $data = get_ncn_data_extra($claim_id, 'aci_property_owner_info');
  if ($data) {
    $data = unserialize($data);
    if (!empty($data['insured_name']) && ncn_cd($claim_id, 'customer_name') != $data['insured_name']) {
      $data['insured_name'] = ncn_admin_remove_special_char_from_customer_name($data['insured_name']);
      
      if (ncn_admin_change_first_room_name($claim_id, $data['insured_name'])) {
        set_ncn_cd($claim_id, 'customer_name', $data['insured_name']);
      }
    }
    
    if (!empty($data['insured_address'])) { set_ncn_cd($claim_id, 'insured_address', $data['insured_address']); }
    if (!empty($data['insured_city'])) { set_ncn_cd($claim_id, 'insured_city', $data['insured_city']); }
    if (!empty($data['insured_state'])) { set_ncn_cd($claim_id, 'insured_state', $data['insured_state']); }
    if (!empty($data['insured_zip'])) { set_ncn_cd($claim_id, 'insured_zip', $data['insured_zip']); }
    if (!empty($data['phone'][0]['number'])) { set_ncn_cd($claim_id, 'insured_phone_number', $data['phone'][0]['number']); }
  }
  
  $data = get_ncn_data_extra($claim_id, 'aci_insurance_claim_policy');
  if ($data) {
    $data = unserialize($data);
    if (!empty($data['date_of_loss']) && $data['date_of_loss']!=0) {
      $date_of_loss = strtotime($data['date_of_loss']);
      if ($date_of_loss) {
        $d_array = array( date("F", $date_of_loss), date("j", $date_of_loss), date("Y", $date_of_loss) );
        set_ncn_cd($claim_id, 'date_of_loss', $d_array);
      }
    }
  }
}

function ncn_admin_sync_additional_data($claim_id, $options = array()) {
  $data = get_ncn_data_extra($claim_id, 'aci_property_owner_info');
  if ($data) {
    $data = unserialize($data);
    
    $cd_data = array();
    if (isset($options['insured_name'])) {
      $cd_data['insured_name']   = ncn_cd($claim_id, 'customer_name');
    }
    $cd_data['insured_address']   = ncn_cd($claim_id, 'insured_address');
    $cd_data['insured_city'] = ncn_cd($claim_id, 'insured_city'); 
    $cd_data['insured_state'] = ncn_cd($claim_id, 'insured_state'); 
    $cd_data['insured_zip'] = ncn_cd($claim_id, 'insured_zip'); 
    $cd_data['insured_phone_number'] = ncn_cd($claim_id, 'insured_phone_number');
    
    
    if (!empty($cd_data['insured_name'])) { $data['insured_name'] = $cd_data['insured_name']; }
    if (!empty($cd_data['insured_address'])) { $data['insured_address'] = $cd_data['insured_address']; }
    if (!empty($cd_data['insured_city'])) { $data['insured_city'] = $cd_data['insured_city']; }
    if (!empty($cd_data['insured_state'])) { $data['insured_state'] = $cd_data['insured_state']; }
    if (!empty($cd_data['insured_zip'])) { $data['insured_zip'] = $cd_data['insured_zip']; }
    if (!empty($cd_data['insured_phone_number'])) { $data['phone'][0]['number'] = $cd_data['insured_phone_number']; }
    
    set_ncn_data_extra($claim_id, 'aci_property_owner_info', serialize($data));
  }
  
  $data = get_ncn_data_extra($claim_id, 'aci_insurance_claim_policy');
  if ($data) {
    $data = unserialize($data);
    
    $icp_data = array();
    $date_of_loss = ncn_cd($claim_id, 'date_of_loss');
    if ($date_of_loss) {
      if (is_array($date_of_loss)) {
        $date_of_loss = implode(" ", $date_of_loss);
      }
      $icp_data['date_of_loss'] = date("m/d/Y", strtotime($date_of_loss));
    }
    
    if (!empty($icp_data['date_of_loss'])) { $data['date_of_loss'] = $icp_data['date_of_loss']; }
    set_ncn_data_extra($claim_id, 'aci_insurance_claim_policy', serialize($data));
  }
}
/**
 * ACI Render - "Information" Section
 */ 
function ncn_admin_aci_render_claim_information($claim_id, $section) {
global $base_url;
    $data = array();
    if (isset($_POST['ci'])) {
        $data = $_POST['ci'];
    } else {
        $data = ncn_admin_get_aci_claim_information($claim_id);
    }   
    ob_start();
?>
<script>
  jQuery(document).ready(function() { bind_ncn_admin_aci_ci_radio_reset_buttons(); bind_ncn_admin_aci_ci_cboxlabel_buttons(); });
</script>
<table id="aci_ci_content" border="1">
    <tr>
        <td class="td-label"><label for="ci_us_to_build">You would like us to build:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" name="ci[us_to_build]" id="ci_utb_invoice_for_already_provided" value="Invoice for Services Already Provided" <?php if(isset($data['us_to_build'])&&$data['us_to_build']=='Invoice for Services Already Provided') { echo "checked='checked'";} ?> /><label for="ci_utb_invoice_for_already_provided">Invoice for Services Already Provided</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[us_to_build]" id="ci_utb_estimate" value="Estimate" <?php if(isset($data['us_to_build'])&&$data['us_to_build']=='Estimate') { echo "checked='checked'";} ?> /><label for="ci_utb_estimate">Estimate</label></li>
            </ul>
        </td>
    <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
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
    <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
    </tr>
    <tr>
        <td class="td-label"><label for="ci_date_of_service">Date of Service:</label></td>
        <td><div class="field-item text-field-item small-size"><input type="text" name="ci[date_of_service]" id="ci_date_of_service" value="<?php echo isset($data['date_of_service'])?$data['date_of_service']:''; ?>" onkeypress="return isDateKey(event);" /></div></td>
    <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
    </tr>
    <tr>
        <td class="td-label"><label for="ci_date_of_completion">Date of Completion:</label></td>
        <td><div class="field-item text-field-item small-size"><input type="text" name="ci[date_of_completion]" id="ci_date_of_completion" value="<?php echo isset($data['date_of_completion'])?$data['date_of_completion']:''; ?>" onkeypress="return isDateKey(event);" /></div></td>
    <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
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
    <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
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
    <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
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
    <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
    </tr>
    <tr>
        <td class="td-label"><label for="ci_number_of_stories_affected">Number of Stories Affected:</label></td>
        <td><div class="field-item text-field-item small-size"><input type="text" name="ci[number_of_stories_affected]" id="ci_number_of_stories_affected" value="<?php echo isset($data['number_of_stories_affected'])?$data['number_of_stories_affected']:''; ?>"  onkeypress="return isNumberKey(event, false);" /></div></td>
    <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
    </tr>
    <tr>
        <td class="td-label"><label for="ci_deductible">Deductible:</label></td>
        <td><div class="field-item text-field-item small-size"><span> $ </span><input type="text" name="ci[deductible]" id="ci_deductible" value="<?php echo isset($data['deductible'])?$data['deductible']:''; ?>" onkeypress="return isNumberKey(event, true);" /></div></td>
    <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
    </tr>
    <tr>
        <td class="td-label"><label for="ci_deductible_collected">Deductible Collected:</label></td>
        <td><div class="field-item text-field-item small-size"><span> $ </span><input type="text" name="ci[deductible_collected]" id="ci_deductible_collected" value="<?php echo isset($data['deductible_collected'])?$data['deductible_collected']:''; ?>" /></div></td>
    <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
    </tr>
    <tr>
        <td class="td-label"><label for="ci_vehicle_fuel">Vehicle Fuel:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" name="ci[vehicle_fuel]" id="ci_vf_yes_45" value="Yes($45)" <?php if(isset($data['vehicle_fuel'])&&$data['vehicle_fuel']=='Yes($45)') { echo "checked='checked'";} ?> /><label for="ci_vf_yes_45">Yes($45)</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[vehicle_fuel]" id="ci_vf_yes_other" value="Yes(Other @d)" <?php if(isset($data['vehicle_fuel'])&&$data['vehicle_fuel']=='Yes(Other @d)') { echo "checked='checked'";} ?> class="cboxradio" />
                    <label class="cboxlabel" for="ci_vehicle_fuel_other">Yes(Other $<span class="field-item text-field-item small-size"><input type="text" name="ci[vehicle_fuel_other]" id="ci_vehicle_fuel_other" value="<?php echo isset($data['vehicle_fuel_other'])?$data['vehicle_fuel_other']:''; ?>" /></span>)</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[vehicle_fuel]" id="ci_vf_no" value="No" <?php if(isset($data['vehicle_fuel'])&&$data['vehicle_fuel']=='No') { echo "checked='checked'";} ?> /><label for="ci_vf_no">No</label></li>
            </ul>
        </td>
    <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
    </tr>
    <tr>
        <td class="td-label"><label for="ci_haul_debris">Haul Debris:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" name="ci[haul_debris]" id="ci_hd_yes_load" value="Yes(@d Loads)" <?php if(isset($data['haul_debris'])&&$data['haul_debris']=="Yes(@d Loads)") { echo "checked='checked'";} ?> class="cboxradio"  />
                    <label class="cboxlabel" for="ci_haul_debris_yes_load">Yes(# <span class="field-item text-field-item small-size"><input type="text" name="ci[haul_debris_yes_load]" id="ci_haul_debris_yes_load" value="<?php echo isset($data['haul_debris_yes_load'])?$data['haul_debris_yes_load']:''; ?>" /> Loads</span>)</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[haul_debris]" id="ci_hd_yes_yd" value="Yes(@d YD Dumpster)" <?php if(isset($data['haul_debris'])&&$data['haul_debris']=="Yes(@d YD Dumpster)") { echo "checked='checked'";} ?> class="cboxradio" />
                    <label class="cboxlabel" for="ci_haul_debris_yes_yd">Yes(# <span class="field-item text-field-item small-size"><input type="text" name="ci[haul_debris_yes_yd]" id="ci_haul_debris_yes_yd" value="<?php echo isset($data['haul_debris_yes_yd'])?$data['haul_debris_yes_yd']:''; ?>" /> YD Dumpster</span>)</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[haul_debris]" id="ci_hd_no" value="No" <?php if(isset($data['haul_debris'])&&$data['haul_debris']=='No') { echo "checked='checked'";} ?> /><label for="ci_hd_no">No</label></li>
            </ul>
        </td>
    <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
    </tr>
    <tr>
        <td class="td-label"><label for="ci_equipment_fuel">Equipment Fuel:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" name="ci[equipment_fuel]" id="ci_ef_yes" value="Yes(@d)" <?php if(isset($data['equipment_fuel'])&&$data['equipment_fuel']=="Yes(@d)") { echo "checked='checked'";} ?> class="cboxradio" />
                    <label class="cboxlabel" for="ci_haul_debris_yes_price">Yes(
                        <span class="field-item text-field-item small-size">Type: <input type="text" name="ci[equipment_fuel_yes_type]" id="ci_haul_debris_yes_type" value="<?php echo isset($data['equipment_fuel_yes_type'])?$data['equipment_fuel_yes_type']:''; ?>" /></span>&nbsp;
                        <span class="field-item text-field-item small-size">Gallons: <input type="text" name="ci[equipment_fuel_yes_price]" id="ci_haul_debris_yes_price" value="<?php echo isset($data['equipment_fuel_yes_price'])?$data['equipment_fuel_yes_price']:''; ?>" /></span>&nbsp;
                        <span class="field-item text-field-item small-size">$Fee: <input type="text" name="ci[equipment_fuel_yes_fee]" id="ci_haul_debris_yes_fee" value="<?php echo isset($data['equipment_fuel_yes_fee'])?$data['equipment_fuel_yes_fee']:''; ?>" /></span>
                        )
                    </label></li>
                <li class="inline-field-item"><input type="radio" name="ci[equipment_fuel]" id="ci_ef_no" value="No" <?php if(isset($data['equipment_fuel'])&&$data['equipment_fuel']=='No') { echo "checked='checked'";} ?> /><label for="ci_ef_no">No</label></li>
            </ul>
        </td>
    <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
    </tr>
    <tr style="display:none;">
        <td class="td-label"><label for="ci_base_services_charges">Base Services Charges:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" name="ci[base_services_charges]" id="ci_bsc_yes" value="Yes" <?php if(isset($data['base_services_charges'])&&$data['base_services_charges']=='Yes') { echo "checked='checked'";} ?> /><label for="ci_bsc_yes">Yes</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[base_services_charges]" id="ci_bsc_no" value="No" <?php if(isset($data['base_services_charges'])&&$data['base_services_charges']=='No') { echo "checked='checked'";} ?> /><label for="ci_bsc_no">No</label></li>
            </ul>
        </td>
    <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
    </tr>
    <tr>
        <td class="td-label"><label for="ci_overhead_profit">Overhead & Profit:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" name="ci[overhead_profit]" id="ci_op_yes" value="Yes" <?php if(isset($data['overhead_profit'])&&$data['overhead_profit']=='Yes') { echo "checked='checked'";} ?> /><label for="ci_op_yes">Yes</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[overhead_profit]" id="ci_op_yes_cumulative" value="Yes(Cumulative)" <?php if(isset($data['overhead_profit'])&&$data['overhead_profit']=='Yes(Cumulative)') { echo "checked='checked'";} ?> /><label for="ci_op_yes_cumulative">Yes(Cumulative)</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[overhead_profit]" id="ci_op_no" value="No" <?php if(isset($data['overhead_profit'])&&$data['overhead_profit']=='No') { echo "checked='checked'";} ?> /><label for="ci_op_no">No</label></li>
            </ul>
        </td>
    <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
    </tr>
    <!--<tr>
        <td class="td-label"><label for="ci_admin_sup_tim_on_loss">Admin/Supervisory Time on Loss:</label></td>
        <td><div class="field-item text-field-item small-size"><input type="text" name="ci[admin_sup_tim_on_loss]" id="ci_admin_sup_tim_on_loss" value="<?php echo isset($data['admin_sup_tim_on_loss'])?$data['admin_sup_tim_on_loss']:''; ?>" /><span> Hrs (+1.75 Hrs) </span></div></td>
    </tr> -->   
    <tr>
        <td class="td-label"><label for="ci_project_management">Project Management Time:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" name="ci[project_management]" id="ci_pm_yes" value="Yes" <?php if(isset($data['project_management'])&&$data['project_management']=='Yes') { echo "checked='checked'";} ?> class="cboxradio" />
                    <label class="cboxlabel" for="ci_project_management_time">Yes(<span class="field-item text-field-item small-size"><input type="text" name="ci[project_management_time]" id="ci_project_management_time" onkeypress="return isNumberKey(event, false);" value="<?php echo isset($data['project_management_time'])?$data['project_management_time']:''; ?>" /> Hours</span>)</label>
                </li>
                <li class="inline-field-item"><input type="radio" name="ci[project_management]" id="ci_pm_no" value="No" <?php if(isset($data['project_management'])&&$data['project_management']=='No') { echo "checked='checked'";} ?> /><label for="ci_pm_no">No</label></li>
            </ul>
        </td>
    <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
    </tr>
    <tr>
        <td class="td-label"><label for="ci_thermal_image">Thermal Image:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" name="ci[thermal_image]" id="ci_ti_yes_150" value="Yes($150)" <?php if(isset($data['thermal_image'])&&$data['thermal_image']=='Yes($150)') { echo "checked='checked'";} ?> /><label for="ci_ti_yes_150">Yes($150)</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[thermal_image]" id="ci_ti_other" value="Other(@d)" <?php if(isset($data['thermal_image'])&&$data['thermal_image']=='Other(@d)') { echo "checked='checked'";} ?> class="cboxradio" />
                    <label class="cboxlabel" for="ci_thermal_image_other_price">Other(<span>$ </span><span class="field-item text-field-item small-size"><input type="text" name="ci[thermal_image_other_price]" id="ci_thermal_image_other_price" value="<?php echo isset($data['thermal_image_other_price'])?$data['thermal_image_other_price']:''; ?>" /></span>)</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[thermal_image]" id="ci_ti_no" value="No" <?php if(isset($data['thermal_image'])&&$data['thermal_image']=='No') { echo "checked='checked'";} ?> /><label for="ci_ti_no">No</label></li>
            </ul>
        </td>
    <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
    </tr>
    <tr>
        <td class="td-label"><label for="ci_ppe">PPE (coverall, tape, gloves, masks):</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" name="ci[ppe]" id="ci_ppe_standard_65" value="Standard $65" <?php if(isset($data['ppe'])&&$data['ppe']=='Standard $65') { echo "checked='checked'";} ?> /><label for="ci_ppe_standard_65">Standard $65</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[ppe]" id="ci_ppe_other" value="Other(@d)" <?php if(isset($data['ppe'])&&$data['ppe']=='Other(@d)') { echo "checked='checked'";} ?> class="cboxradio" />
                    <label class="cboxlabel" for="ci_ppe_other_price">Other(<span>$ </span><span class="field-item text-field-item small-size"><input type="text" name="ci[ppe_other_price]" id="ci_ppe_other_price" value="<?php echo isset($data['ppe_other_price'])?$data['ppe_other_price']:''; ?>" /></span>)</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[ppe]" id="ci_ppe_no" value="No" <?php if(isset($data['ppe'])&&$data['ppe']=='No') { echo "checked='checked'";} ?> /><label for="ci_ppe_no">No</label></li>
            </ul>
        </td>
    <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
    </tr>
    <tr>
        <td class="td-label"><label for="ci_respirators">Respirators:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" name="ci[respirators]" id="ci_r_full" value="Full" <?php if(isset($data['respirators'])&&$data['respirators']=='Full') { echo "checked='checked'";} ?> /><label for="ci_r_full">Full</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[respirators]" id="ci_r_half" value="Half" <?php if(isset($data['respirators'])&&$data['respirators']=='Half') { echo "checked='checked'";} ?> class="cboxradio" />
                    <label class="cboxlabel" for="ci_respirators_half_ea">Half
                        <span class="field-item text-field-item small-size"><input type="text" name="ci[respirators_half_ea]" id="ci_respirators_half_ea" value="<?php echo isset($data['respirators_half_ea'])?$data['respirators_half_ea']:''; ?>" /></span><span>EA x</span>
                   </label>
                   <label class="cboxlabel" for="ci_respirators_half_days">
                        <span class="field-item text-field-item small-size"><input type="text" name="ci[respirators_half_days]" id="ci_respirators_half_days" value="<?php echo isset($data['respirators_half_days'])?$data['respirators_half_days']:''; ?>" /></span><span>Days</span>
                    </label>
                </li>
            </ul>
        </td>
    <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
    </tr>
    <tr>
        <td class="td-label"><label for="ci_respirator_cartridges">Respirator Cartridges:</label></td>
        <td><div class="field-item text-field-item small-size"><input type="text" name="ci[respirator_cartridges]" id="ci_respirator_cartridges" value="<?php echo isset($data['respirator_cartridges'])?$data['respirator_cartridges']:''; ?>" /><span>EA</span></div></td>
    <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
    </tr>
    <tr>
        <td class="td-label"><label for="ci_air_testing">Air Testing:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" name="ci[air_testing]" id="ci_at_yes" value="Yes(@d)" <?php if(isset($data['air_testing'])&&$data['air_testing']=="Yes(@d)") { echo "checked='checked'";} ?> class="cboxradio" />
                    <label class="cboxlabel" for="ci_air_testing_yes_price">Yes (<span class="field-item text-field-item small-size"><span>$ </span><input type="text" name="ci[air_testing_yes_price]" id="ci_air_testing_yes_price" value="<?php echo isset($data['air_testing_yes_price'])?$data['air_testing_yes_price']:''; ?>" /></span>)</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[air_testing]" id="ci_at_no" value="No" <?php if(isset($data['air_testing'])&&$data['air_testing']=='No') { echo "checked='checked'";} ?> /><label for="ci_at_no">No</label></li>
            </ul>
        </td>
    <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
    </tr>
    <tr>
        <td class="td-label"><label for="ci_hvac_duct_cleaning">HVAC/ Duct Cleaning:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" name="ci[hvac_duct_cleaning][type]" id="ci_hadc_reg" value="Reg" <?php if(isset($data['hvac_duct_cleaning']['type'])&&$data['hvac_duct_cleaning']['type']=='Reg') { echo "checked='checked'";} ?> /><label for="ci_hadc_reg">Reg</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[hvac_duct_cleaning][type]" id="ci_hadc_heavy" value="Heavy" <?php if(isset($data['hvac_duct_cleaning']['type'])&&$data['hvac_duct_cleaning']['type']=='Heavy') { echo "checked='checked'";} ?> /><label for="ci_hadc_heavy">Heavy</label></li>
                <li class="inline-field-item">
                    <ul class="field-check-lists inline-group-fields clearfix">
                        <li class="inline-field-item"><input type="radio" name="ci[hvac_duct_cleaning][rh_type]" id="ci_hadc_rh_type_per_register" value="Per Register" <?php if(isset($data['hvac_duct_cleaning']['rh_type'])&&$data['hvac_duct_cleaning']['rh_type']=="Per Register") { echo "checked='checked'";} ?> class="cboxradio" />
                            <label class="cboxlabel" for="ci_hadc_rh_per_register">Per Register(# <span class="field-item text-field-item small-size"><input type="text" name="ci[hvac_duct_cleaning][rh_per_register]" id="ci_hadc_rh_per_register" value="<?php echo isset($data['hvac_duct_cleaning']['rh_per_register'])?$data['hvac_duct_cleaning']['rh_per_register']:''; ?>" /></span>)</label>
                        </li>
                        <li class="inline-field-item"><input type="radio" name="ci[hvac_duct_cleaning][rh_type]" id="ci_hadc_rh_type_bid_price" value="Bid Price" <?php if(isset($data['hvac_duct_cleaning']['rh_type'])&&$data['hvac_duct_cleaning']['rh_type']=="Bid Price") { echo "checked='checked'";} ?> class="cboxradio" />
                            <label class="cboxlabel" for="ci_hadc_rh_per_bid_price">Bid Price($ <span class="field-item text-field-item small-size"><input type="text" name="ci[hvac_duct_cleaning][rh_bid_price]" id="ci_hadc_rh_per_bid_price" onkeypress="return isNumberKey(event, true);" value="<?php echo isset($data['hvac_duct_cleaning']['rh_bid_price'])?$data['hvac_duct_cleaning']['rh_bid_price']:''; ?>" /></span>)</label>
                        </li>
                    </ul>
                </li>
                <li class="inline-field-item"><input type="radio" name="ci[hvac_duct_cleaning][type]" id="ci_hadc_no" value="No" <?php if(isset($data['hvac_duct_cleaning']['type'])&&$data['hvac_duct_cleaning']['type']=='No') { echo "checked='checked'";} ?> /><label for="ci_hadc_no">No</label></li>
            </ul>
        </td>
    <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
    </tr>
    <!-- One row more -->
    <tr>
        <td class="td-label"><label for="ci_clean_air_handler">Clean Air Handler:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" name="ci[clean_air_handler]" id="ci_cah_yes_150" value="Yes($150)" <?php if(isset($data['clean_air_handler'])&&$data['clean_air_handler']=='Yes($150)') { echo "checked='checked'";} ?> /><label for="ci_cah_yes_150">Yes($150)</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[clean_air_handler]" id="ci_cah_yes_other" value="Yes(Other @d)" <?php if(isset($data['clean_air_handler'])&&$data['clean_air_handler']=='Yes(Other @d)') { echo "checked='checked'";} ?> class="cboxradio" />
                    <label class="cboxlabel" for="clean_air_handler_other">Yes(Other $<span class="field-item text-field-item small-size"><input type="text" name="ci[clean_air_handler_other]" id="clean_air_handler_other" value="<?php echo isset($data['clean_air_handler_other'])?$data['clean_air_handler_other']:''; ?>" /></span>)</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[clean_air_handler]" id="ci_cah_no" value="No" <?php if(isset($data['clean_air_handler'])&&$data['clean_air_handler']=='No') { echo "checked='checked'";} ?> /><label for="ci_cah_no">No</label></li>
            </ul>
        </td>
    <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
    </tr>
</table>

<script>
jQuery(function() {
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

<?php
    $output = ob_get_contents();
    ob_end_clean();
    
    return $output;
}
function ncn_admin_get_aci_claim_information($claim_id) {
    $data = get_ncn_data_extra($claim_id, 'aci_claim_information');
    if ($data) {
        $data = unserialize($data);
    } else {
        $data = array();
    }
    
    // Populate
    $invoice_data = get_extended_data($claim_id);

    if (!isset($data['category_of_water']))     { $data['category_of_water']    = isset($invoice_data['category'])?$invoice_data['category']:''; }
    if (!isset($data['date_of_service']))       { $data['date_of_service']      = isset($invoice_data['start_date'])?$invoice_data['start_date']:''; }
    if (!isset($data['date_of_completion']))    { $data['date_of_completion']   = isset($invoice_data['completion_date'])?$invoice_data['completion_date']:''; }
    if (!isset($data['cause_origin_of_loss']))  { $data['cause_origin_of_loss'] = isset($invoice_data['cause'])?$invoice_data['cause']:''; }
    
    return $data;
}



/**
 * ACI Render - "Daily Claim Log" Section
 */ 
function ncn_admin_aci_render_daily_claim_log($claim_id, $section) {    
    ob_start();
?>
<div id="dcl_log_list">
    <?php print ncn_admin_aci_render_dcl_log_list($claim_id); ?>
</div>
<div id="dcl_action_panel_content" class="dcl-action-panel">
    <div id="dcl_action_panel_descrption">
    <p style="color: red; margin: 5px 10px;">
        *ONLY record hours for travel to and from jobsite, equipment setup, inspection, monitoring, taking photos, moisture readings, and take down of equipment
    </p>
    </div>
    <?php print ncn_admin_aci_render_dcl_action_panel($claim_id, 'new'); ?>
</div>
<?php
    $output = ob_get_contents();
    ob_end_clean();
    
    return $output;
}
function ncn_admin_get_aci_daily_claim_log($claim_id) {
    $data = get_ncn_data_extra($claim_id, 'aci_daily_claim_log');
    if ($data) {
        $data = unserialize($data);
    if (is_array($data)) {
      $data = array_slice($data, 0);
    }
    } else {
        $data = array();
    }
    
    // Populate
    
    return $data;
}
function ncn_admin_aci_render_dcl_log_list($claim_id) {
    $data = array();
    if (isset($_POST['dcl'])) {
        $data = $_POST['dcl'];
    } else {
        $data = ncn_admin_get_aci_daily_claim_log($claim_id);
    }
    
    ob_start();
?>
<div class="table-responsive">
<table class="table">
<thead>
    <th class='dcl-day-week'>Day of Week</th>
    <th class='dcl-date'>Date</th>
    <th class='dcl-men'># Men</th>
    <th class='dcl-hours-day'>Hours Per Day</th>
    <th class='dcl-total-hours'>Total Hours</th>
    <th class='dcl-after-hours'>After Hours</th>
    <th class='dcl-operation'></th>
</thead>
<tbody>
<?php foreach ($data as $key=>$item): ?>
    <tr>
        <td class='dcl-day-week'><?php echo $item['day_week']; ?></td>
        <td class='dcl-date'><?php echo $item['date']; ?></td>
        <td class='dcl-men'><?php echo $item['men']; ?> </td>
        <td class='dcl-hours-day'><?php echo $item['hours_day']; ?> Hours</td>
        <td class='dcl-total-hours'><?php $t_hours = $item['men'] * $item['hours_day']; if ($t_hours) { echo $t_hours." Hours"; } ?></td>
        <td class='dcl-after-hours'><?php echo $item['after_hours']; ?></td>
        <td class='dcl-operation'>
            <a href="" onclick="render_aci_dcl_log_action_panel(<?php echo $claim_id.", "."'edit', $key"; ?>); return false;">Edit</a>
            <a href="" onclick="on_click_aci_dcl_delete_log(<?php echo $claim_id.", $key"; ?>); return false;">Delete</a>
        </td>
    </tr>
<?php endforeach; ?>
</tbody>
</table>
</div>
<?php
    $output = ob_get_contents();
    ob_end_clean();
    
    return $output;
}



/**
 * Render ACI - DCL Action Panel
 */
function ncn_admin_aci_render_dcl_action_panel($claim_id, $action, $index=0) {
    global $base_url;
    $log = array();
    if ($action == 'edit') {
        $data = ncn_admin_get_aci_daily_claim_log($claim_id);
        if (isset($data[$index])) { $log = $data[$index]; }
    } else if ($action == 'new') {
        $log['date'] = date("m/d/Y"); 
    }
    ob_start();
?>
<div class="dcl-form-section">
    <div class="form-group">
        <div for="username" class="col-sm-4 control-label">Date (MM/DD/YY):</div>
        <div class="col-sm-8 date-control">
            <input type="text" class="form-control" name="dc_log[date]" id="dcl_date" value="<?php echo isset($log['date'])?$log['date']:''; ?>" onkeypress="return isDateKey(event);" />
        </div>
    </div>
    <div class="form-group">
        <div for="dcl_men" class="col-sm-4 control-label"># Men:</div>
        <div class="col-sm-8">
            <input type="text" class="form-control" name="dc_log[men]" id="dcl_men" onkeypress="return isNumberKey(event, false);" onkeyup="dcl_calc_total_hours();" value="<?php echo isset($log['men'])?$log['men']:''; ?>" />
        </div>
    </div>
    <div class="form-group">
        <div for="dcl_hours_day" class="col-sm-4 control-label">Hours Per Day:</div>
        <div class="col-sm-8">
            <input type="text" class="form-control" name="dc_log[hours_day]" id="dcl_hours_day" onkeypress="return isNumberKey(event, true);" onkeyup="dcl_calc_total_hours();" value="<?php echo isset($log['hours_day'])?$log['hours_day']:''; ?>" />
        </div>
    </div>
    <div class="form-group">
        <div for="dcl_hours_day" class="col-sm-4 control-label">Total Hours:</div>
        <div class="col-sm-8">
            <input type="text" class="form-control" name="dc_log[total_hours]" id="dcl_total_hours" value="<?php echo isset($log['men'])?$log['men']:0 * isset($log['hours_day'])?$log['hours_day']:0; ?>" readonly /><span>&nbsp;(Calculated automatically)</span>
        </div>
    </div>
    <div class="form-group">
        <div for="dcl_hourdcl_after_hourss_day" class="col-sm-4 control-label">After Hours:</div>
        <div class="col-sm-8">
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" name="dc_log[after_hours]" id="dcl_after_hours_yes" value="Yes" <?php if(isset($log['after_hours'])&&$log['after_hours']=='Yes') { echo "checked='checked'";} ?> /><label for="dcl_after_hours_yes">Yes</label></li>
                <li class="inline-field-item"><input type="radio" name="dc_log[after_hours]" id="dcl_after_hours_no" value="No" <?php if(isset($log['after_hours'])&&$log['after_hours']=='No') { echo "checked='checked'";} ?> /><label for="dcl_after_hours_no">No</label></li>
            </ul>
        </div>
    </div>
    <div class="form-group">
        <div class="col-sm-4 control-label"></div>
        <div class="col-sm-8">
            <div class='aci-actions'>
                <input type="hidden" id="dcl_action_type" name="dcl_action_type" value="<?php echo $action; ?>" />
                <input type="hidden" id="dcl_log_index" name="dcl_log_index" value="<?php echo $index; ?>" />
                <?php if ($action == 'new'): ?>
                    <input type='button' class="btn btn-primary" value='  Add  ' onclick="on_click_aci_dcl_log_action(<?php echo $claim_id; ?>)"/>
                <?php else: ?>
                    <input type='button' class="btn btn-primary" value='  OK  ' onclick="on_click_aci_dcl_log_action(<?php echo $claim_id; ?>)" />&nbsp;&nbsp;&nbsp;
                    <input type='button' class="btn"value='  Cancel  ' onclick="render_aci_dcl_log_action_panel(<?php echo $claim_id; ?>, 'new', 0)" />
                <?php endif; ?>
            </div>
        </div>
    </div>

</div>

<script>
jQuery(function() {
    ncn_admin_aci_dcl_datepicker();
});
function ncn_admin_aci_dcl_datepicker() {
    jQuery( '#dcl_date' ).datepicker({
        showOn: "button",
        buttonImage: "<?php echo $base_url."/".drupal_get_path('module', 'ncn_report')."/images/calendar.gif"; ?>",
        buttonImageOnly: true,
        dateFormat: 'mm/dd/yy',
    });
}

</script>
<?php 
    $output = ob_get_contents();
    ob_end_clean();
    return $output;
}



/**
 * ACI Render - "Insurance Claim Policy" Section
 */ 
function ncn_admin_aci_render_insurance_claim_policy($claim_id, $section) {
    global $base_url;
    $data = array();
    if (isset($_POST['icp'])) {
        $data = $_POST['icp'];
    } else {
        $data = ncn_admin_get_aci_insurance_claim_policy($claim_id);
    }
    
    ob_start();
?>
<table>
    <tr>
        <td class="td-label"><label for="icp_insured_name">Insured's Name:</label></td>
        <td><div class="field-item text-field-item"><input type="text" name="icp[insured_name]" id="icp_insured_name" value="<?php echo isset($data['insured_name'])?$data['insured_name']:''; ?>" /></div></td>
    </tr>
    <tr>
        <td class="td-label"><label for="icp_insurance_company">Insurance Company:</label></td>
        <td><div class="field-item text-field-item"><input type="text" name="icp[insurance_company]" id="icp_insurance_company" value="<?php echo isset($data['insurance_company'])?$data['insurance_company']:''; ?>" /></div></td>
    </tr>
    <tr>
        <td class="td-label"><label for="icp_insurance_company_phone">Insurance Company Phone:</label></td>
        <td>
            <div class="field-item text-field-item inline-field field-phone-number"><input type="text" name="icp[insurance_company_phone][number]" id="icp_insurance_company_phone_number" onkeypress="return isNumberKey(event, false);" value="<?php echo isset($data['insurance_company_phone']['number'])?$data['insurance_company_phone']['number']:''; ?>" /></div>
            <div class="field-item text-field-item inline-field field-phone-ext"><label for="icp_insurance_company_phone_ext">ext</label><input type="text" name="icp[insurance_company_phone][ext]" id="icp_insurance_company_phone_ext" value="<?php echo isset($data['insurance_company_phone']['ext'])?$data['insurance_company_phone']['ext']:''; ?>" /></div>
        </td>
    </tr>
    <tr>
        <td class="td-label"><label for="icp_insurance_agent">Insurance Agent:</label></td>
        <td><div class="field-item text-field-item"><input type="text" name="icp[insurance_agent]" id="icp_insurance_agent" value="<?php echo isset($data['insurance_agent'])?$data['insurance_agent']:''; ?>" /></div></td>
    </tr>   
    <tr>
        <td class="td-label"><label for="icp_insurance_agent_phone">Insurance Agent Phone:</label></td>
        <td>
            <div class="field-item text-field-item inline-field field-phone-number"><input type="text" name="icp[insurance_agent_phone][number]" id="icp_insurance_agent_phone_number" onkeypress="return isNumberKey(event, false);" value="<?php echo isset($data['insurance_agent_phone']['number'])?$data['insurance_agent_phone']['number']:''; ?>" /></div>
            <div class="field-item text-field-item inline-field field-phone-ext"><label for="icp_insurance_agent_phone_ext">ext</label><input type="text" name="icp[insurance_agent_phone][ext]" id="icp_insurance_agent_phone_ext" value="<?php echo isset($data['insurance_agent_phone']['ext'])?$data['insurance_agent_phone']['ext']:''; ?>" /></div>
        </td>
    </tr>
    <tr>
        <td class="td-label"><label for="icp_date_of_loss">Date of Loss:</label></td>
        <td><div class="field-item text-field-item"><input type="text" name="icp[date_of_loss]" id="icp_date_of_loss" value="<?php echo isset($data['date_of_loss'])?$data['date_of_loss']:''; ?>" onkeypress="return isDateKey(event);" /></div></td>
    </tr>

    <tr>
        <td class="td-label"><label for="icp_field_adjuster">Field Adjuster:</label></td>
        <td><div class="field-item text-field-item"><input type="text" name="icp[field_adjuster]" id="icp_field_adjuster" value="<?php echo isset($data['field_adjuster'])?$data['field_adjuster']:''; ?>" /></div></td>
    </tr>
    <tr>
        <td class="td-label"><label for="icp_field_adjuster_email">Field Adjuster Email:</label></td>
        <td><div class="field-item text-field-item"><input type="text" name="icp[field_adjuster_email]" id="icp_field_adjuster_email" value="<?php echo isset($data['field_adjuster_email'])?$data['field_adjuster_email']:''; ?>" /></div></td>
    </tr>
    <tr>
        <td class="td-label"><label for="icp_field_adjuster_phone">Phone:</label></td>
        <td>
            <div class="field-item text-field-item inline-field field-phone-number"><input type="text" name="icp[field_adjuster_phone][number]" id="icp_field_adjuster_phone" onkeypress="return isNumberKey(event, false);" value="<?php echo isset($data['field_adjuster_phone']['number'])?$data['field_adjuster_phone']['number']:''; ?>" /></div>
            <div class="field-item text-field-item inline-field field-phone-ext"><label for="icp_field_adjuster_phone_ext">ext</label><input type="text" name="icp[field_adjuster_phone][ext]" id="icp_field_adjuster_phone_ext" value="<?php echo isset($data['field_adjuster_phone']['ext'])?$data['field_adjuster_phone']['ext']:''; ?>" /></div>
        </td>
    </tr>
    <tr>
        <td class="td-label"><label for="icp_field_adjuster_alternate_phone">Alternate Phone:</label></td>
        <td>
            <div class="field-item text-field-item inline-field field-phone-number"><input type="text" name="icp[field_adjuster_alternate_phone][number]" id="icp_field_adjuster_alternate_phone" class="phone-style-field" onkeypress="return isNumberKey(event, false);" value="<?php echo isset($data['field_adjuster_alternate_phone']['number'])?$data['field_adjuster_alternate_phone']['number']:''; ?>" /></div>
            <div class="field-item text-field-item inline-field field-phone-ext"><label for="icp_adjuster_alternate_phone_ext">ext</label><input type="text" name="icp[field_adjuster_alternate_phone][ext]" id="icp_field_adjuster_alternate_phone_ext" value="<?php echo isset($data['field_adjuster_alternate_phone']['ext'])?$data['field_adjuster_alternate_phone']['ext']:''; ?>" /></div>
        </td>
    </tr>

    <tr>
        <td class="td-label"><label for="icp_claim_adjuster">Claim Adjuster:</label></td>
        <td><div class="field-item text-field-item"><input type="text" name="icp[claim_adjuster]" id="icp_claim_adjuster" value="<?php echo isset($data['claim_adjuster'])?$data['claim_adjuster']:''; ?>" /></div></td>
    </tr>
    <tr>
        <td class="td-label"><label for="icp_adjuster_email">Adjuster Email:</label></td>
        <td><div class="field-item text-field-item"><input type="text" name="icp[adjuster_email]" id="icp_adjuster_email" value="<?php echo isset($data['adjuster_email'])?$data['adjuster_email']:''; ?>" /></div></td>
    </tr>
    <tr>
        <td class="td-label"><label for="icp_insurance_agent_phone">Adjuster Phone:</label></td>
        <td>
            <div class="field-item text-field-item inline-field field-phone-number"><input type="text" name="icp[adjuster_phone][number]" id="icp_adjuster_phone_number" onkeypress="return isNumberKey(event, false);" value="<?php echo isset($data['adjuster_phone']['number'])?$data['adjuster_phone']['number']:''; ?>" /></div>
            <div class="field-item text-field-item inline-field field-phone-ext"><label for="icp_adjuster_phone_ext">ext</label><input type="text" name="icp[adjuster_phone][ext]" id="icp_adjuster_phone_ext" value="<?php echo isset($data['adjuster_phone']['ext'])?$data['adjuster_phone']['ext']:''; ?>" /></div>
        </td>
    </tr>
    <tr>
        <td class="td-label"><label for="icp_adjuster_alternate_phone_number">Adjuster Alternate Phone:</label></td>
        <td>
            <div class="field-item text-field-item inline-field field-phone-number"><input type="text" name="icp[adjuster_alternate_phone_number][number]" id="icp_adjuster_alternate_phone_number" class="phone-style-field" onkeypress="return isNumberKey(event, false);" value="<?php echo isset($data['adjuster_alternate_phone_number']['number'])?$data['adjuster_alternate_phone_number']['number']:''; ?>" /></div>
            <div class="field-item text-field-item inline-field field-phone-ext"><label for="icp_adjuster_alternate_phone_ext">ext</label><input type="text" name="icp[adjuster_alternate_phone][ext]" id="icp_adjuster_alternate_phone_ext" value="<?php echo isset($data['adjuster_alternate_phone']['ext'])?$data['adjuster_alternate_phone']['ext']:''; ?>" /></div>
        </td>
    </tr>

    <tr>
        <td class="td-label"><label for="icp_insurance_fax">Insurance Fax:</label></td>
        <td><div class="field-item text-field-item"><input type="text" name="icp[insurance_fax]" id="icp_insurance_fax" class="phone-style-field" onkeypress="return isNumberKey(event, false);" value="<?php echo isset($data['insurance_fax'])?$data['insurance_fax']:''; ?>" /></div></td>
    </tr>
    <tr>
        <td class="td-label"><label for="icp_effective_policy_date">Effective Policy Date:</label></td>
        <td>
            <div class="field-item text-field-item inline-field field-date-from"><input type="text" name="icp[effective_policy_date][from]" id="icp_effective_policy_date_from" value="<?php echo isset($data['effective_policy_date']['from'])?$data['effective_policy_date']['from']:''; ?>" onkeypress="return isDateKey(event);" /></div>
            <div class="field-item text-field-item inline-field field-date-to"><span> - </span><input type="text" name="icp[effective_policy_date][to]" id="icp_effective_policy_date_to" value="<?php echo isset($data['effective_policy_date']['to'])?$data['effective_policy_date']['to']:''; ?>" onkeypress="return isDateKey(event);" /></div>
        </td>
    </tr>
    <tr>
        <td class="td-label"><label for="icp_policy_number">Policy Number:</label></td>
        <td><div class="field-item text-field-item"><input type="text" name="icp[policy_number]" id="icp_policy_number" value="<?php echo isset($data['policy_number'])?$data['policy_number']:''; ?>" /></div></td>
    </tr>
    <tr>
        <td class="td-label"><label for="icp_insurance_claim_number">Insurance Claim Number:</label></td>
        <td><div class="field-item text-field-item"><input type="text" name="icp[insurance_claim_number]" id="icp_insurance_claim_number" value="<?php echo isset($data['insurance_claim_number'])?$data['insurance_claim_number']:''; ?>" /></div></td>
    </tr>
</table>

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
<?php   
    $output = ob_get_contents();
    ob_end_clean();
    
    return $output;
}

function ncn_admin_get_aci_insurance_claim_policy($claim_id) {
    $data = get_ncn_data_extra($claim_id, 'aci_insurance_claim_policy');
    if ($data) {
        $data = unserialize($data);
    } else {
        $data = array();
    }
    
    $invoice_data = get_extended_data($claim_id);
    
    // Populate
    if (!isset($data['insurance_company']))    { $data['insurance_company'] = isset($invoice_data['insurance_company'])?$invoice_data['insurance_company']:''; }
    if (!isset($data['claim_adjuster']))       { $data['claim_adjuster']    = isset($invoice_data['claim_adjuster'])?$invoice_data['claim_adjuster']:''; }
    if (!isset($data['adjuster_alternate_phone_number']['number']))    { $data['adjuster_alternate_phone_number']['number'] = isset($invoice_data['claim_adjuster_phone'])?$invoice_data['claim_adjuster_phone']:''; }
    if (!isset($data['insurance_fax']))        { $data['insurance_fax'] = isset($invoice_data['claim_adjuster_fax'])?$invoice_data['claim_adjuster_fax']:''; }
    if (!isset($data['insurance_claim_number'])){$data['insurance_claim_number'] = isset($invoice_data['claim_number'])?$invoice_data['claim_number']:''; }
  
  if (!isset($data['date_of_loss'])) { 
    $date_of_loss = ncn_cd($claim_id, 'date_of_loss');
    if ($date_of_loss) {
      if (is_array($date_of_loss)) {
        $date_of_loss = implode(" ", $date_of_loss);
      }
      $data['date_of_loss'] = date("m/d/Y", strtotime($date_of_loss));
    }
  }
    
    return $data;
}


/**
 * Menu Handler (ajax/ncn_admin/aci/dcl/%/render_daily_log_list)
 */
function ncn_admin_ajax_aci_dcl_render_daily_log_list($claim_id) {
    $json = array();
    $json['code'] = 'success';
    $json['html']   = ncn_admin_aci_render_dcl_log_list($claim_id);
    
    echo json_encode($json);
    exit;
}

/**
 * Menu Handler (ajax/ncn_admin/aci/dcl/%/render_log_action_panel)
 */
function ncn_admin_ajax_aci_dcl_render_log_action_panel($claim_id) {
    $action = $_POST['action'];
    $index = $_POST['index'];
    
    $json = array();
    $json['code'] = 'success';
    $json['html']   = ncn_admin_aci_render_dcl_action_panel($claim_id, $action, $index);
    
    echo json_encode($json);
    exit;
}

/**
 * Menu Handler (ajax/ncn_admin/aci/dcl/%/daily_log_submit)
 */
function ncn_admin_ajax_aci_dcl_daily_log_submit($claim_id) {
    $action_type= $_POST['action_type'];
    $log_index  = $_POST['log_index'];
    
    $dcl_date           = $_POST['dcl_date'];
    $dcl_men                = $_POST['dcl_men'];
    $dcl_hours_day  = $_POST['dcl_hours_day'];
    $dcl_after_hours= $_POST['dcl_after_hours'];
    
    $dcl_data = array(
        'date' => $dcl_date,
        'men' => $dcl_men, 
        'hours_day' => $dcl_hours_day,
        'after_hours' => $dcl_after_hours,
    );
    
    $json = array();
    $json['code'] = 'success';
    if ($action_type == 'new') {
        $json['msg'] = "Successfully added the new daily claim log.";
    } else {    // edit
        $json['msg'] = "Successfully updated daily claim log.";
    }
    
    if (!ncn_admin_aci_dcl_submit_daily_log($claim_id, $action_type, $log_index, $dcl_data)) {
        $json['code'] = 'error-101';
        $json['msg'] = "Failed to save daily claim log data.";
    }
    
    echo json_encode($json);
    exit;
}

/**
 * Menu Handler (ajax/ncn_admin/aci/dcl/%/delete_daily_log)
 */
function ncn_admin_ajax_aci_dcl_delete_daily_log($claim_id) {
    $log_index  = $_POST['log_index'];
    
    $json = array();
    $json['code'] = 'success';
    $json['msg'] = "Successfully Deleted daily claim log.";
    
    $action_type = 'delete';
    if (!ncn_admin_aci_dcl_submit_daily_log($claim_id, $action_type, $log_index)) {
        $json['code'] = 'error-101';
        $json['msg'] = "Failed to delete daily claim log.";
    }
    
    echo json_encode($json);
    exit;
}

function ncn_admin_aci_dcl_submit_daily_log($claim_id, $action, $index=0, $data=array()) {

    if (!empty($data)) {
        $dcl_day_week = date( 'D', strtotime($data['date']) );
        $data['day_week'] = $dcl_day_week;
    }
    $logs = ncn_admin_get_aci_daily_claim_log($claim_id);
    // Add & Update
    if ($action == 'new') {
        if (!empty($data)) {
            $logs[] = $data;
        } else {
            return FALSE;
        }
    } 
    
    if ($action == 'edit') {
        if (isset($logs[$index]) && !empty($data)) {
            $logs[$index] = $data;
        } else {
            return FALSE;
        }
    } else if ($action == 'delete') {
        if (isset($logs[$index])) {
            unset($logs[$index]);
        } else {
            return FALSE;
        }
    }
    // Re-Sorting
    usort($logs, 'ncn_admin_aci_dcl_log_list_sort_by_date_cmp');
    // Save
    if ( !set_ncn_data_extra($claim_id, 'aci_daily_claim_log', serialize($logs)) ) {
        return FALSE;
    }
    
    return TRUE;
}

function ncn_admin_aci_dcl_log_list_sort_by_date_cmp($a, $b) {
    $a_time = strtotime($a['date']);
    $b_time = strtotime($b['date']);
    if ($a_time == $b_time) {
            return 0;
    }
    return ($a_time < $b_time) ? -1 : 1;
}



/**
 * Render Edit Addtional Claims Data Page
 * Page Handler (admin/config/ncn_admin/edit-additional-claim-data)
 */
function ncn_admin_edit_additional_claim_data($claim_id) {
    drupal_set_title("Edit Claims Processing: "/*"Additional Claim Data: "*/.$claim_id." (".ncn_cd($claim_id, 'customer_name').")");
?>
<div id="page_loading"></div>
<div id="page_results">
    <div class="claims-detail additional-claim-info"> 
        <div class="claims-section-wrapper">
            <div class="aci-section-content"><?php print ncn_admin_aci_render_sections($claim_id); ?></div>
        </div>
    </div>
</div>
<?php 
}

function ncn_admin_aci_render_sections($claim_id) {
  global $base_url;
  $old_a_data = array(
    'poi' =>  unserialize(get_ncn_data_extra($claim_id, 'aci_property_owner_info')), 
    'ci' =>   unserialize(get_ncn_data_extra($claim_id, 'aci_claim_information')), 
    'icp' =>  unserialize(get_ncn_data_extra($claim_id, 'aci_insurance_claim_policy')), 
    'comments' => unserialize(get_ncn_data_extra($claim_id, 'aci_comments')), 
  );
  
    if (isset($_POST['tfunction']) && $_POST['tfunction']=='additional_claims_data') {
        $b_save_log = FALSE;
        if (isset($_POST['poi'])) {
      $_POST['poi']['insured_name'] = ncn_admin_remove_special_char_from_customer_name($_POST['poi']['insured_name']);
            set_ncn_data_extra($claim_id, 'aci_property_owner_info', serialize($_POST['poi']));
            $b_save_log = TRUE;
        }
        if (isset($_POST['ci'])) {
            $_POST['ci']['cause_origin_of_loss'] = rtrim($_POST['ci']['cause_origin_of_loss'], " .");
            set_ncn_data_extra($claim_id, 'aci_claim_information', serialize($_POST['ci']));
            $b_save_log = TRUE;
        }
        if (isset($_POST['icp'])) {
            set_ncn_data_extra($claim_id, 'aci_insurance_claim_policy', serialize($_POST['icp']));
            $b_save_log = TRUE;
        }
        if (isset($_POST['comments'])) {
            set_ncn_data_extra($claim_id, 'aci_comments', serialize($_POST['comments']));
            $b_save_log = TRUE;
        }
    
        if ($b_save_log) {
      ncn_admin_sync_claim_data($claim_id);
      $additional_claim_log = ncn_admin_get_claim_log_additional_data($old_a_data, $_POST);
            $log_message = t("!user_name added additional data.", array('!user_name'=>ncn_amin_get_user_role_and_name()));
      $log_message .="\r\n".$additional_claim_log;
            ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
        }
    }
    
    ob_start();
?>
<form id='aci_property_owner_info_form' method='POST'>
    <input type='hidden' name='tfunction' value='additional_claims_data' />
  <div class="clearfix">
    <div class="room-ss-room-edit-view-link">
      <a href="<?php $base_url."/admin/config/ncn_admin/view-additional-claim-data/$claim_id"; ?>">View Claims Processing</a>
    </div>
  </div>
    <?php 
        $sections = ncn_admin_get_aci_sections();
        $s_info = $sections[$section];
        foreach ($sections as $section=>$s_info) {
            if ($s_info['render_func'] && function_exists($s_info['render_func'])) {
                echo "<div class='$section'>";
                echo "<fieldset class='aci-section-content'>";
                echo "<legend>".$s_info['title']."</legend>";
                print call_user_func($s_info['render_func'], $claim_id, $section);
                echo "</fieldset>";
                echo "</div>";
            }
        }
    ?>
    <div class='aci-actions'><input type='submit' value='  Save  '/></div>
</form>
<?php 
    $output = ob_get_contents();
    ob_end_clean();
    return $output;
}

/**
 * ACI Render - "Comments" Section
 */ 
function ncn_admin_aci_render_comments($claim_id, $section) {   
    $data = array();
    if (isset($_POST['comments'])) {
        $data = $_POST['comments'];
    } else {
        $data = ncn_admin_get_aci_comments($claim_id);
    }
    ob_start();
?>
<table>
    <tr>
        <td class="td-label"><label for="comments_comments">Comments:</label></td>
        <td><div class="field-item text-field-item"><textarea name="comments[content]" id="comments_content" cols="60" rows="10"><?php echo isset($data['content'])?$data['content']:''; ?></textarea></div></td>
    </tr>
</table>
<?php
    $output = ob_get_contents();
    ob_end_clean();
    
    return $output;
}
function ncn_admin_get_aci_comments($claim_id) {
    
    
    $data = get_ncn_data_extra($claim_id, 'aci_comments');
    if ($data) {
        $data = unserialize($data);
        if (isset($data['comments']) && !isset($data['content'])) {
            /** change comments->content **/    
            $data['content'] = $data['comments'];
            unset($data['comments']);
            set_ncn_data_extra($claim_id, 'aci_comments', serialize($data));
        }
        
    } else {
        $data = array();
    }
    
    // Populate
    
    return $data;
}

function ncn_admin_get_aci_sections() {
    return array(
        'property_owner_info'   => array('title'=>'Property Owner Information', 'render_func'=>'ncn_admin_aci_render_property_owner_info' ), 
        'claim_information'         => array('title'=>'Claim Information', 'render_func'=>'ncn_admin_aci_render_claim_information'), 
        'daily_claim_log'           => array('title'=>'Daily Claim Log', 'render_func'=>'ncn_admin_aci_render_daily_claim_log'), 
        'insurance_claim_policy'=> array('title'=>'Insurance Policy Information', 'render_func'=>'ncn_admin_aci_render_insurance_claim_policy'), 
        'comments'                          => array('title'=>'Comments', 'render_func'=>'ncn_admin_aci_render_comments'), 
    );
}

?>
