<?php
/**
 * Render View Addtional Claims Data Page
 * Page Handler (admin/config/ncn_admin/edit-additional-claim-data)
 */
function ncn_admin_view_additional_claim_data($claim_id) {
  drupal_set_title("View Claims Processing: ".$claim_id." (".ncn_cd($claim_id, 'customer_name').")");
?>
<div id="page_loading"></div>
<div id="page_results">
  <div class="claims-detail additional-claim-info"> 
    <div class="claims-section-wrapper">
      <div class="aci-section-content"><?php print ncn_admin_view_aci_render_sections($claim_id); ?></div>
    </div>
  </div>
</div>
<?php 
}

function ncn_admin_view_aci_render_sections($claim_id) {  
  ob_start();
?>
<div id='view_aci_property_owner_info_section' method='POST'>
<div class="clearfix">
  <?php if (user_access('ncn_admin edit addtional claim data')): ?>
  <div class="room-ss-room-edit-view-link">
    <a href="<?php print base_path()."admin/config/ncn_admin/edit-additional-claim-data/$claim_id"; ?>">Edit Claims Processing</a>
  </div>
  <?php endif; ?>
</div>
  <?php 
    $sections = ncn_admin_get_view_aci_sections();
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
</div>
<?php 
  $output = ob_get_contents();
  ob_end_clean();
  return $output;
}

function ncn_admin_get_view_aci_sections() {
  return array(
    'property_owner_info'   => array('title'=>'Property Owner Information', 'render_func'=>'ncn_admin_view_aci_render_property_owner_info' ), 
    'claim_information'     => array('title'=>'Claim Information', 'render_func'=>'ncn_admin_view_aci_render_claim_information'), 
    'daily_claim_log'       => array('title'=>'Daily Claim Log', 'render_func'=>'ncn_admin_view_aci_render_daily_claim_log'), 
    'insurance_claim_policy'=> array('title'=>'Insurance Policy Information', 'render_func'=>'ncn_admin_view_aci_render_insurance_claim_policy'), 
    'comments'               => array('title'=>'Comments', 'render_func'=>'ncn_admin_view_aci_render_comments'), 
  );
}

function ncn_admin_view_aci_render_property_owner_info($claim_id, $section) {
  $data = array();
  $data = ncn_admin_get_aci_property_owner_info($claim_id);
  
  ob_start();
?>
<table>
  <tr>
    <td class="td-label"><label for="poi_insured_name">Owner/Insured Name:</label></td>
    <td><div class="field-item text-field-item"><div id="poi_insured_name"><?php echo $data['insured_name']; ?></div></div></td>
  </tr>
  <tr>
    <td class="td-label"><label for="poi_insured_address">Property Loss Address:</label></td>
    <td><div class="field-item text-field-item"><div id="poi_insured_address"><?php echo $data['insured_address']; ?></div></div></td>
  </tr>
  <tr>
    <td class="td-label"><label for="poi_apt_bldg">Apt/Suite/Bldg:</label></td>
    <td><div class="field-item text-field-item"><div id="poi_apt_bldg"><?php echo $data['apt_bldg']; ?></div></div></td>
  </tr>
  <tr>
    <td class="td-label"><label for="poi_insured_city">City:</label></td>
    <td><div class="field-item text-field-item"><div id="poi_insured_city"><?php echo $data['insured_city']; ?></div></div></td>
  </tr>
  <tr>
    <td class="td-label"><label for="poi_insured_state">State:</label></td>
    <td><div class="field-item text-field-item"><div id="poi_insured_state"><?php echo $data['insured_state']; ?></div></div></td>
  </tr>
  <tr>
    <td class="td-label"><label for="poi_insured_zip">Zip Code:</label></td>
    <td><div class="field-item text-field-item"><div id="poi_insured_zip"><?php echo $data['insured_zip']; ?></div></div></td>
  </tr>

  <tr>
    <td class="td-label"><label for="poi_billing_address">Billing Address:</label></td>
    <td><div class="field-item text-field-item"><div id="poi_billing_address"><?php echo $data['billing_address']; ?></div></div></td>
  </tr>
  <tr>
    <td class="td-label"><label for="poi_billing_apt_bldg">Apt/Suite/Bldg:</label></td>
    <td><div class="field-item text-field-item"><div id="poi_billing_apt_bldg"><?php echo $data['billing_apt_bldg']; ?></div></div></td>
  </tr>
  <tr>
    <td class="td-label"><label for="poi_billing_city">City:</label></td>
    <td><div class="field-item text-field-item"><div id="poi_billing_city"><?php echo $data['billing_city']; ?></div></div></td>
  </tr>
  <tr>
    <td class="td-label"><label for="poi_billing_state">State:</label></td>
    <td><div class="field-item text-field-item"><div id="poi_billing_state"><?php echo $data['billing_state']; ?></div></div></td>
  </tr>
  <tr>
    <td class="td-label"><label for="poi_billing_zip">Zip Code:</label></td>
    <td><div class="field-item text-field-item"><div id="poi_billing_zip"><?php echo $data['billing_zip']; ?></div></div></td>
  </tr>

  <tr>
    <td class="td-label">
      <label for="poi_phone_0_number">Phone
      <ul class="field-check-lists inline-group-fields clearfix">
        <li class="inline-field-item"><span id="poi_phone_0_type_home"><?php if($data['phone'][0]['type']=='Home') { echo "&#9745;";} else { echo "&#9744"; } ?></span> <label for="poi_phone_0_type_home">Home</label></li>
        <li class="inline-field-item"><span id="poi_phone_0_type_work"><?php if($data['phone'][0]['type']=='Work') { echo "&#9745;";} else { echo "&#9744"; } ?></span> <label for="poi_phone_0_type_work">Work</label></li>
        <li class="inline-field-item"><span id="poi_phone_0_type_cell"><?php if($data['phone'][0]['type']=='Cell') { echo "&#9745;";} else { echo "&#9744"; } ?></span> <label for="poi_phone_0_type_cell">Cell:</label></li>
      </ul></label>
    </td>
    <td><div class="field-item text-field-item"><span id="poi_phone_0_number" class="phone-style-field"><?php echo $data['phone'][0]['number']; ?></span></div></td>
  </tr>
  <tr>
    <td class="td-label">
      <label for="poi_phone_1_number">Phone
      <ul class="field-check-lists inline-group-fields clearfix">
        <li class="inline-field-item"><span id="poi_phone_1_type_home"><?php if($data['phone'][1]['type']=='Home') { echo "&#9745;";} else { echo "&#9744"; } ?></span> <label for="poi_phone_1_type_home">Home</label></li>
        <li class="inline-field-item"><span id="poi_phone_1_type_work"><?php if($data['phone'][1]['type']=='Work') { echo "&#9745;";} else { echo "&#9744"; } ?></span> <label for="poi_phone_1_type_work">Work</label></li>
        <li class="inline-field-item"><span id="poi_phone_1_type_cell"><?php if($data['phone'][1]['type']=='Cell') { echo "&#9745;";} else { echo "&#9744"; } ?></span> <label for="poi_phone_1_type_cell">Cell:</label></li>
      </ul></label>
    </td>
    <td><div class="field-item text-field-item"><span id="poi_phone_1_number" class="phone-style-field"><?php echo $data['phone'][1]['number']; ?></span></div></td>
  </tr>
  <tr>
    <td class="td-label">
      <label for="poi_phone_2_number">Phone
      <ul class="field-check-lists inline-group-fields clearfix">
        <li class="inline-field-item"><span id="poi_phone_2_type_home"><?php if($data['phone'][2]['type']=='Home') { echo "&#9745;";} else { echo "&#9744"; } ?></span> <label for="poi_phone_2_type_home">Home</label></li>
        <li class="inline-field-item"><span id="poi_phone_2_type_work"><?php if($data['phone'][2]['type']=='Work') { echo "&#9745;";} else { echo "&#9744"; } ?></span> <label for="poi_phone_2_type_work">Work</label></li>
        <li class="inline-field-item"><span id="poi_phone_2_type_cell"><?php if($data['phone'][2]['type']=='Cell') { echo "&#9745;";} else { echo "&#9744"; } ?></span> <label for="poi_phone_2_type_cell">Cell:</label></li>
      </ul></label>
    </td>
    <td><div class="field-item text-field-item"><span id="poi_phone_2_number" class="phone-style-field"><?php echo $data['phone'][2]['number']; ?></span></div></td>
  </tr>
  <tr>
    <td class="td-label"><label for="poi_email">Email:</label></td>
    <td><div class="field-item text-field-item"><div id="poi_property_owner_email"><?php echo $data['property_owner_email']; ?></div></div></td>
  </tr>
</table>
<?php  
  $output = ob_get_contents();
  ob_end_clean();
  
  return $output;
}

/**
 * ACI Render - "Information" Section
 */ 
function ncn_admin_view_aci_render_claim_information($claim_id, $section) {
  $data = array();
  $data = ncn_admin_get_aci_claim_information($claim_id);
  
  ob_start();
?>
<table id="aci_ci_content" border="1">
  <tr>
    <td class="td-label"><label for="ci_us_to_build">You would like us to build:</label></td>
    <td>
      <ul class="field-check-lists inline-group-fields clearfix">
        <li class="inline-field-item"><span id="ci_utb_invoice_for_already_provided"><?php if($data['us_to_build']=='Invoice for Services Already Provided') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_utb_invoice_for_already_provided">Invoice for Services Already Provided</label></li>
        <li class="inline-field-item"><span id="ci_utb_estimate"><?php if($data['us_to_build']=='Estimate') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_utb_estimate">Estimate</label></li>
      </ul>
    </td>
  </tr>
  <tr>
    <td class="td-label"><label for="ci_service_call">Service Call:</label></td>
    <td>
      <ul class="field-check-lists inline-group-fields clearfix">
        <li class="inline-field-item"><span id="ci_sc_normal_business"><?php if($data['service_call']=='Normal Business Hrs') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_sc_normal_business">Normal Business Hrs</label></li>
        <li class="inline-field-item"><span id="ci_sc_after"><?php if($data['service_call']=='After Hours') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_sc_after">After Hours</label></li>
        <li class="inline-field-item"><span id="ci_sc_none"><?php if($data['service_call']=='None') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_sc_none">None</label></li>
      </ul>
    </td>
  </tr>
  <tr>
    <td class="td-label"><label for="ci_date_of_service">Date of Service:</label></td>
    <td><div class="field-item text-field-item small-size"><div><?php echo $data['date_of_service']; ?></div></div></td>
  </tr>
  <tr>
    <td class="td-label"><label for="ci_date_of_completion">Date of Completion:</label></td>
    <td><div class="field-item text-field-item small-size"><div><?php echo $data['date_of_completion']; ?></div></div></td>
  </tr>
  <tr>
    <td class="td-label"><label for="ci_cause_origin_of_loss">Cause and Origin of Loss:</label></td>
    <td><div class="field-item text-field-item"><div id="ci_cause_origin_of_loss"><?php echo $data['cause_origin_of_loss']; ?></div></div></td>
  </tr>
  <tr>
    <td class="td-label"><label for="ci_category_of_water">Category of Water:</label></td>
    <td>
      <ul class="field-check-lists inline-group-fields clearfix">
        <li class="inline-field-item"><span id="ci_cow_1"><?php if($data['category_of_water']=='1') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_cow_1">1</label></li>
        <li class="inline-field-item"><span id="ci_cow_2"><?php if($data['category_of_water']=='2') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_cow_2">2</label></li>
        <li class="inline-field-item"><span id="ci_cow_3"><?php if($data['category_of_water']=='3') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_cow_3">3</label></li>
        <li class="inline-field-item"><span id="ci_cow_no_water"><?php if($data['category_of_water']=='No Water') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_cow_no_water">No Water</label></li>
      </ul>
    </td>
  </tr>
  <tr>
    <td class="td-label"><label for="ci_class_of_loss">Class of Loss:</label></td>
    <td>
      <ul class="field-check-lists inline-group-fields clearfix">
        <li class="inline-field-item"><span id="ci_col_1"><?php if($data['class_of_loss']=='1') { echo "&#9745;";} else { echo "&#9744"; } ?></span> <label for="ci_col_1">1</label></li>
        <li class="inline-field-item"><span id="ci_col_2"><?php if($data['class_of_loss']=='2') { echo "&#9745;";} else { echo "&#9744"; } ?></span> <label for="ci_col_2">2</label></li>
        <li class="inline-field-item"><span id="ci_col_3"><?php if($data['class_of_loss']=='3') { echo "&#9745;";} else { echo "&#9744"; } ?></span> <label for="ci_col_3">3</label></li>
        <li class="inline-field-item"><span id="ci_col_4"><?php if($data['class_of_loss']=='4') { echo "&#9745;";} else { echo "&#9744"; } ?></span> <label for="ci_col_3">4</label></li>
        <li class="inline-field-item"><span id="ci_col_no_water"><?php if($data['class_of_loss']=='No Water') { echo "&#9745;";} else { echo "&#9744"; } ?> <label for="ci_col_no_water">No Water</label></li>
      </ul>
    </td>
  </tr>
  <tr>
    <td class="td-label"><label for="ci_number_of_stories_affected">Number of Stories Affected:</label></td>
    <td><div class="field-item text-field-item small-size"><div id="ci_number_of_stories_affected"><?php echo $data['number_of_stories_affected']; ?></div></div></td>
  </tr>
  <tr>
    <td class="td-label"><label for="ci_deductible">Deductible:</label></td>
    <td><div class="field-item text-field-item small-size"><?php if ($data['deductible']): ?><span id="ci_deductible">$<?php echo $data['deductible']; ?></span><?php endif; ?></div></td>
  </tr>
  <tr>
    <td class="td-label"><label for="ci_deductible_collected">Deductible Collected:</label></td>
    <td><div class="field-item text-field-item small-size"><?php if ($data['deductible_collected']): ?><span id="ci_deductible_collected">$<?php echo $data['deductible_collected']; ?>"</span><?php endif; ?></div></td>
  </tr>
  <tr>
    <td class="td-label"><label for="ci_vehicle_fuel">Vehicle Fuel:</label></td>
    <td>
      <ul class="field-check-lists inline-group-fields clearfix">
        <li class="inline-field-item"><span id="ci_vf_yes_45"><?php if($data['vehicle_fuel']=='Yes($45)') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_vf_yes_45">Yes($45)</label></li>
        <li class="inline-field-item"><span id="ci_vf_yes_other"><?php if($data['vehicle_fuel']=='Yes(Other @d)') { echo "&#9745;";} else { echo "&#9744"; } ?></span>
          <label for="ci_vf_yes_other">Yes(Other <?php if ($data['vehicle_fuel_other']): ?><span class="field-item text-field-item small-size"><span id="ci_vehicle_fuel_other" >$<?php echo $data['vehicle_fuel_other']; ?></span><?php endif; ?></span>)</label></li>
        <li class="inline-field-item"><span id="ci_vf_no"><?php if($data['vehicle_fuel']=='No') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_vf_no">No</label></li>
      </ul>
    </td>
  </tr>
  <tr>
    <td class="td-label"><label for="ci_haul_debris">Haul Debris:</label></td>
    <td>
      <ul class="field-check-lists inline-group-fields clearfix">
        <li class="inline-field-item"><span id="ci_hd_yes_load"><?php if($data['haul_debris']=="Yes(@d Loads)") { echo "&#9745;";} else { echo "&#9744"; } ?></span>
          <label for="ci_hd_yes_load">Yes(# <span class="field-item text-field-item small-size"><span id="ci_haul_debris_yes_load" ><?php echo $data['haul_debris_yes_load']; ?> </span> Loads</span>)</label></li>
        <li class="inline-field-item"><span id="ci_hd_yes_yd"><?php if($data['haul_debris']=="Yes(@d YD Dumpster)") { echo "&#9745;";} else { echo "&#9744"; } ?></span>
          <label for="ci_hd_yes_yd">Yes(# <span class="field-item text-field-item small-size"><span id="ci_haul_debris_yes_yd" ><?php echo $data['haul_debris_yes_yd']; ?> </span> YD Dumpster</span>)</label></li>
        <li class="inline-field-item"><span id="ci_hd_no"><?php if($data['haul_debris']=='No') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_hd_no">No</label></li>
      </ul>
    </td>
  </tr>
  <tr>
    <td class="td-label"><label for="ci_equipment_fuel">Equipment Fuel:</label></td>
    <td>
      <ul class="field-check-lists inline-group-fields clearfix">
        <li class="inline-field-item"><span id="ci_ef_yes" ><?php if($data['equipment_fuel']=="Yes(@d)") { echo "&#9745;";} else { echo "&#9744"; } ?></span>
          <label for="ci_ef_yes">Yes(
            <span class="field-item text-field-item small-size">Type: <span id="ci_haul_debris_yes_type"><?php echo $data['equipment_fuel_yes_type']; ?></span></span>
            <span class="field-item text-field-item small-size">Gallons: <span id="ci_haul_debris_yes_price"><?php echo $data['equipment_fuel_yes_price']; ?></span></span>
            <span class="field-item text-field-item small-size">$Fee: <span id="ci_haul_debris_yes_fee"><?php echo $data['equipment_fuel_yes_fee']; ?></span></span>
            )
          </label></li>
        <li class="inline-field-item"><span id="ci_ef_no" ><?php if($data['equipment_fuel']=='No') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_ef_no">No</label></li>
      </ul>
    </td>
  </tr>
  <tr>
    <td class="td-label"><label for="ci_base_services_charges">Base Services Charges:</label></td>
    <td>
      <ul class="field-check-lists inline-group-fields clearfix">
        <li class="inline-field-item"><span id="ci_bsc_yes"><?php if($data['base_services_charges']=='Yes') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_bsc_yes">Yes</label></li>
        <li class="inline-field-item"><span ="ci_bsc_no"><?php if($data['base_services_charges']=='No') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_bsc_no">No</label></li>
      </ul>
    </td>
  </tr>
  <tr>
    <td class="td-label"><label for="ci_overhead_profit">Overhead & Profit:</label></td>
    <td>
      <ul class="field-check-lists inline-group-fields clearfix">
        <li class="inline-field-item"><span id="ci_op_yes"><?php if($data['overhead_profit']=='Yes') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_op_yes">Yes</label></li>
        <li class="inline-field-item"><span id="ci_op_yes_cumulative"><?php if($data['overhead_profit']=='Yes(Cumulative)') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_op_yes_cumulative">Yes(Cumulative)</label></li>
        <li class="inline-field-item"><span id="ci_op_no"><?php if($data['overhead_profit']=='No') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_op_no">No</label></li>
      </ul>
    </td>
  </tr>
  <tr>
    <td class="td-label"><label for="ci_project_management">Project Management Time:</label></td>
    <td>
      <ul class="field-check-lists inline-group-fields clearfix">
        <li class="inline-field-item"><span id="ci_pm_yes"><?php if($data['project_management']=='Yes') { echo "&#9745;";} else { echo "&#9744"; } ?></span>
          <label for="ci_pm_yes">Yes<?php if ($data['project_management_time']): ?>(<span class="field-item text-field-item small-size"><span id="ci_project_management_time"><?php echo $data['project_management_time']; ?></span> Hours</span>)<?php endif; ?></label>
        </li>
        <li class="inline-field-item"><span id="ci_pm_no"><?php if($data['project_management']=='No') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_pm_no">No</label></li>
      </ul>
    </td>
  </tr>
  <tr>
    <td class="td-label"><label for="ci_thermal_image">Thermal Image:</label></td>
    <td>
      <ul class="field-check-lists inline-group-fields clearfix">
        <li class="inline-field-item"><span id="ci_ti_yes_150"><?php if($data['thermal_image']=='Yes($150)') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_ti_yes_150">Yes($150)</label></li>
        <li class="inline-field-item"><span id="ci_ti_other"><?php if($data['thermal_image']=='Other(@d)') { echo "&#9745;";} else { echo "&#9744"; } ?></span>
          <label for="ci_ti_other">Other<?php if ($data['thermal_image_other_price']): ?>(<span>$ </span><span class="field-item text-field-item small-size"><span id="ci_thermal_image_other_price"><?php echo $data['thermal_image_other_price']; ?></span></span>)<?php endif; ?></label></li>
        <li class="inline-field-item"><span id="ci_ti_no"><?php if($data['thermal_image']=='No') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_ti_no">No</label></li>
      </ul>
    </td>
  </tr>
  <tr>
    <td class="td-label"><label for="ci_ppe">PPE (coverall, tape, gloves, masks):</label></td>
    <td>
      <ul class="field-check-lists inline-group-fields clearfix">
        <li class="inline-field-item"><span id="ci_ppe_standard_65"><?php if($data['ppe']=='Standard $65') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_ppe_standard_65">Standard $65</label></li>
        <li class="inline-field-item"><span id="ci_ppe_other"><?php if($data['ppe']=='Other(@d)') { echo "&#9745;";} else { echo "&#9744"; } ?></span>
          <label for="ci_ppe_other">Other<?php if ($data['ppe_other_price']): ?>(<span>$ </span><span class="field-item text-field-item small-size"><span id="ci_ppe_other_price"><?php echo $data['ppe_other_price']; ?></span></span>)<?php endif; ?></label></li>
        <li class="inline-field-item"><span id="ci_ppe_no"><?php if($data['ppe']=='No') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_ppe_no">No</label></li>
      </ul>
    </td>
  </tr>
  <tr>
    <td class="td-label"><label for="ci_respirators">Respirators:</label></td>
    <td>
      <ul class="field-check-lists inline-group-fields clearfix">
        <li class="inline-field-item"><span id="ci_r_full"><?php if($data['respirators']=='Full') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_r_full">Full</label></li>
        <li class="inline-field-item"><span id="ci_r_half"><?php if($data['respirators']=='Half') { echo "&#9745;";} else { echo "&#9744"; } ?></span>
          <label for="ci_r_half">Half  
            <span class="field-item text-field-item small-size"><span id="ci_respirators_half_ea"><?php echo $data['respirators_half_ea']; ?></span></span><span>EA x</span>
            <span class="field-item text-field-item small-size"><span id="ci_respirators_half_days"><?php echo $data['respirators_half_days']; ?></span></span><span>Days</span>
          </label>
        </li>
      </ul>
    </td>
  </tr>
  <tr>
    <td class="td-label"><label for="ci_respirator_cartridges">Respirator Cartridges:</label></td>
    <td><div class="field-item text-field-item small-size"><?php if ($data['respirator_cartridges']): ?><span id="ci_respirator_cartridges"><?php echo $data['respirator_cartridges']; ?></span><span>EA</span><?php endif; ?></div></td>
  </tr>
  <tr>
    <td class="td-label"><label for="ci_air_testing">Air Testing:</label></td>
    <td>
      <ul class="field-check-lists inline-group-fields clearfix">
        <li class="inline-field-item"><span id="ci_at_yes"><?php if($data['air_testing']=="Yes(@d)") { echo "&#9745;";} else { echo "&#9744"; } ?></span>
          <label for="ci_at_yes">Yes <?php if ($data['air_testing_yes_price']): ?>(<span class="field-item text-field-item small-size"><span>$ </span><span id="ci_air_testing_yes_price"><?php echo $data['air_testing_yes_price']; ?></span></span>)<?php endif; ?></label></li>
        <li class="inline-field-item"><span id="ci_at_no"><?php if($data['air_testing']=='No') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_at_no">No</label></li>
      </ul>
    </td>
  </tr>
  <tr>
    <td class="td-label"><label for="ci_hvac_duct_cleaning">HVAC/ Duct Cleaning:</label></td>
    <td>
      <ul class="field-check-lists inline-group-fields clearfix">
        <li class="inline-field-item"><span id="ci_hadc_reg"><?php if($data['hvac_duct_cleaning']['type']=='Reg') { echo "&#9745;";} else { echo "&#9744"; }  ?></span><label for="ci_hadc_reg">Reg</label></li>
        <li class="inline-field-item"><span id="ci_hadc_heavy"><?php if($data['hvac_duct_cleaning']['type']=='Heavy') { echo "&#9745;";} else { echo "&#9744"; }  ?></span><label for="ci_hadc_heavy">Heavy</label></li>
        <li class="inline-field-item">
          <ul class="field-check-lists inline-group-fields clearfix">
            <li class="inline-field-item"><span id="ci_hadc_rh_type_per_register"><?php if($data['hvac_duct_cleaning']['rh_type']=="Per Register") { echo "&#9745;";} else { echo "&#9744"; }  ?></span>
              <label for="ci_hadc_rh_type_per_register">Per Register<?php if ($data['hvac_duct_cleaning']['rh_per_register']): ?>(# <span class="field-item text-field-item small-size"><span type="text" id="ci_hadc_rh_per_register"><?php echo $data['hvac_duct_cleaning']['rh_per_register']; ?></span></span>)<?php endif; ?></label>
            </li>
            <li class="inline-field-item"><span id="ci_hadc_rh_type_bid_price"><?php if($data['hvac_duct_cleaning']['rh_type']=="Bid Price") { echo "checked='checked'";} ?></span>
              <label for="ci_hadc_rh_type_bid_price">Bid Price<?php if ($data['hvac_duct_cleaning']['rh_bid_price']): ?>($ <span class="field-item text-field-item small-size"><span type="text" id="ci_hadc_rh_per_register"><?php echo $data['hvac_duct_cleaning']['rh_bid_price']; ?></span></span>)<?php endif; ?></label>
            </li>
          </ul>
        </li>
        <li class="inline-field-item"><span id="ci_hadc_no"><?php if($data['hvac_duct_cleaning']['type']=='No') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_hadc_no">No</label></li>
      </ul>
    </td>
  </tr>
  <!-- One row more -->
  <tr>
    <td class="td-label"><label for="ci_clean_air_handler">Clean Air Handler:</label></td>
    <td>
      <ul class="field-check-lists inline-group-fields clearfix">
        <li class="inline-field-item"><span id="ci_cah_yes_150"><?php if($data['clean_air_handler']=='Yes($150)') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_cah_yes_150">Yes($150)</label></li>
        <li class="inline-field-item"><span id="ci_cah_yes_other"><?php if($data['clean_air_handler']=='Yes(Other @d)') { echo "&#9745;";} else { echo "&#9744"; } ?></span>
          <label for="ci_cah_yes_other">Yes(Other <?php if ($data['clean_air_handler_other']): ?>$<span class="field-item text-field-item small-size"><span id="clean_air_handler_other"><?php echo $data['clean_air_handler_other']; ?></span></span><?php endif; ?>)</label></li>
        <li class="inline-field-item"><span id="ci_cah_no"><?php if($data['clean_air_handler']=='No') { echo "&#9745;";} else { echo "&#9744"; } ?></span><label for="ci_cah_no">No</label></li>
      </ul>
    </td>
  </tr>
</table>
<?php
  $output = ob_get_contents();
  ob_end_clean();
  
  return $output;
}

/**
 * ACI Render - "Insurance Claim Policy" Section
 */ 
function ncn_admin_view_aci_render_insurance_claim_policy($claim_id, $section) {
  $data = ncn_admin_get_aci_insurance_claim_policy($claim_id);
  
  ob_start();
?>
<table>
  <tr>
    <td class="td-label"><label for="icp_insured_name">Insured's Name:</label></td>
    <td><div class="field-item text-field-item"><span id="icp_insured_name"><?php echo $data['insured_name']; ?></span></div></td>
  </tr>
  <tr>
    <td class="td-label"><label for="icp_insurance_company">Insurance Company:</label></td>
    <td><div class="field-item text-field-item"><span id="icp_insurance_company"><?php echo $data['insurance_company']; ?></span></div></td>
  </tr>
  <tr>
    <td class="td-label"><label for="icp_insurance_company_phone">Insurance Company Phone:</label></td>
    <td>
      <div class="field-item text-field-item inline-field field-phone-number"><span id="icp_insurance_company_phone_number"><?php echo $data['insurance_company_phone']['number']; ?></span></div>
      <?php if ($data['insurance_company_phone']['ext']): ?>
      <div class="field-item text-field-item inline-field field-phone-ext"><label for="icp_insurance_company_phone_ext">ext</label><span id="icp_insurance_company_phone_ext"><?php echo $data['insurance_company_phone']['ext']; ?></span></div>
      <?php endif; ?>
    </td>
  </tr>
  <tr>
    <td class="td-label"><label for="icp_insurance_agent">Insurance Agent:</label></td>
    <td><div class="field-item text-field-item"><span id="icp_insurance_agent"><?php echo $data['insurance_agent']; ?></span></div></td>
  </tr>  
  <tr>
    <td class="td-label"><label for="icp_insurance_agent_phone">Insurance Agent Phone:</label></td>
    <td>
      <div class="field-item text-field-item inline-field field-phone-number"><span id="icp_insurance_agent_phone_number"><?php echo $data['insurance_agent_phone']['number']; ?></span></div>
      <?php if($data['insurance_agent_phone']['ext']): ?>
      <div class="field-item text-field-item inline-field field-phone-ext"><label for="icp_insurance_agent_phone_ext">ext</label><span id="icp_insurance_agent_phone_ext"><?php echo $data['insurance_agent_phone']['ext']; ?></span></div>
      <?php endif; ?>
    </td>
  </tr>
  <tr>
    <td class="td-label"><label for="icp_date_of_loss">Date of Loss:</label></td>
    <td><div class="field-item text-field-item"><span id="icp_date_of_loss"><?php echo $data['date_of_loss']; ?></span></div></td>
  </tr>

  <tr>
    <td class="td-label"><label for="icp_field_adjuster">Field Adjuster:</label></td>
    <td><div class="field-item text-field-item"><span id="icp_field_adjuster"><?php echo $data['field_adjuster']; ?></span></div></td>
  </tr>
  <tr>
    <td class="td-label"><label for="icp_field_adjuster_email">Field Adjuster Email:</label></td>
    <td><div class="field-item text-field-item"><span id="icp_field_adjuster_email"><?php echo $data['field_adjuster_email']; ?></span></div></td>
  </tr>
  <tr>
    <td class="td-label"><label for="icp_field_adjuster_phone">Phone:</label></td>
    <td>
      <div class="field-item text-field-item inline-field field-phone-number"><span id="icp_field_adjuster_phone"><?php echo $data['field_adjuster_phone']['number']; ?></span></div>
      <?php if ($data['field_adjuster_phone']['ext']): ?>
      <div class="field-item text-field-item inline-field field-phone-ext"><label for="icp_field_adjuster_phone_ext">ext</label><span id="icp_field_adjuster_phone_ext"><?php echo $data['field_adjuster_phone']['ext']; ?></span></div>
      <?php endif; ?>
    </td>
  </tr>
  <tr>
    <td class="td-label"><label for="icp_field_adjuster_alternate_phone">Alternate Phone:</label></td>
    <td>
      <div class="field-item text-field-item inline-field field-phone-number"><span id="icp_field_adjuster_alternate_phone" class="phone-style-field"><?php echo $data['field_adjuster_alternate_phone']['number']; ?></span></div>
      <?php if ($data['field_adjuster_alternate_phone']['ext']): ?>
      <div class="field-item text-field-item inline-field field-phone-ext"><label for="icp_field_adjuster_alternate_phone_ext">ext</label><span id="icp_field_adjuster_alternate_phone_ext"><?php echo $data['field_adjuster_alternate_phone']['ext']; ?></span></div>
      <?php endif; ?>
    </td>
  </tr>

  <tr>
    <td class="td-label"><label for="icp_claim_adjuster">Claim Adjuster:</label></td>
    <td><div class="field-item text-field-item"><span id="icp_claim_adjuster"><?php echo $data['claim_adjuster']; ?></span></div></td>
  </tr>
  <tr>
    <td class="td-label"><label for="icp_adjuster_email">Adjuster Email:</label></td>
    <td><div class="field-item text-field-item"><span id="icp_adjuster_email"><?php echo $data['adjuster_email']; ?></span></div></td>
  </tr>
  <tr>
    <td class="td-label"><label for="icp_insurance_agent_phone">Adjuster Phone:</label></td>
    <td>
      <div class="field-item text-field-item inline-field field-phone-number"><span id="icp_adjuster_phone_number"><?php echo $data['adjuster_phone']['number']; ?></span></div>
      <?php if ($data['adjuster_phone']['ext']): ?>
      <div class="field-item text-field-item inline-field field-phone-ext"><label for="icp_adjuster_phone_ext">ext</label><span id="icp_adjuster_phone_ext"><?php echo $data['adjuster_phone']['ext']; ?></span></div>
      <?php endif; ?>
    </td>
  </tr>
  <tr>
    <td class="td-label"><label for="icp_adjuster_alternate_phone_number">Adjuster Alternate Phone:</label></td>
    <td>
      <div class="field-item text-field-item inline-field field-phone-number"><span id="icp_adjuster_alternate_phone_number" class="phone-style-field"><?php echo $data['adjuster_alternate_phone_number']['number']; ?></span></div>
      <?php if ($data['adjuster_alternate_phone']['ext']): ?>
      <div class="field-item text-field-item inline-field field-phone-ext"><label for="icp_adjuster_alternate_phone_ext">ext</label><span id="icp_adjuster_alternate_phone_ext"><?php echo $data['adjuster_alternate_phone']['ext']; ?></span></div>
      <?php endif; ?>
    </td>
  </tr>

  <tr>
    <td class="td-label"><label for="icp_insurance_fax">Insurance Fax:</label></td>
    <td><div class="field-item text-field-item"><span id="icp_insurance_fax" class="phone-style-field"><?php echo $data['insurance_fax']; ?></span></div></td>
  </tr>
  <tr>
    <td class="td-label"><label for="icp_effective_policy_date">Effective Policy Date:</label></td>
    <td>
      <div class="field-item text-field-item inline-field field-date-from"><span id="icp_effective_policy_date_from"><?php echo $data['effective_policy_date']['from']; ?></span></div>
      <div class="field-item text-field-item inline-field field-date-to"><span> - </span><span id="icp_effective_policy_date_to"><?php echo $data['effective_policy_date']['to']; ?></span></div>
    </td>
  </tr>
  <tr>
    <td class="td-label"><label for="icp_policy_number">Policy Number:</label></td>
    <td><div class="field-item text-field-item"><span id="icp_policy_number"><?php echo $data['policy_number']; ?></span></div></td>
  </tr>
  <tr>
    <td class="td-label"><label for="icp_insurance_claim_number">Insurance Claim Number:</label></td>
    <td><div class="field-item text-field-item"><span id="icp_insurance_claim_number"><?php echo $data['insurance_claim_number']; ?></span></div></td>
  </tr>
</table>
<?php  
  $output = ob_get_contents();
  ob_end_clean();
  
  return $output;
}

/**
 * ACI Render - "Comments" Section
 */ 
function ncn_admin_view_aci_render_comments($claim_id, $section) {  
  $data = ncn_admin_get_aci_comments($claim_id);
  
  ob_start();
?>
<table>
  <tr>
    <td class="td-label"><label for="comments_comments">Comments:</label></td>
    <td><div class="field-item text-field-item"><div id="comments_content"><?php echo $data['content']; ?></div></div></td>
  </tr>
</table>
<?php
  $output = ob_get_contents();
  ob_end_clean();
  
  return $output;
}
?>
