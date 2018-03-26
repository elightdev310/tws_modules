<?php
function ncn_ceportal_check_claim_reviewable($claim_id) {
    $reviewable = get_ncn_data_extra($claim_id, 'ce_reviewable_claim_info');
    if (user_access('ncn ce-portal') && $reviewable == 'reviewable') {
        return TRUE;
    }
    return FALSE;
}

/**
 * Page Handler (ce_portal/review/claim/%/additional_data)
 */
function ncn_ceportal_review_claim_addtional_data($claim_id) {
    $title = t("Additional Data (claim #!claim_id)", array('!claim_id'=>$claim_id));
    drupal_set_title($title);
    ob_start();
?>
<div id="page_results">
    <div id="ce_review_additional_data" class="claims-detail ce-review-page"> 
        <div class="claims-section-wrapper">
            <div class="clearfix">
                <div class="title raw-title"><?php echo $title; ?></div>
                <!-- <div class="help"><a href="#">Help</a></div> -->
                <div class="sub-page-tabs clearfix"><?php print ncn_ceportal_review_render_sub_page_links($claim_id, 'additional_data'); ?></div>
                <div class="sub-page-wrapper">
                    <div class="sub-page-content">
                        <div class="aci-section-links"><?php print ncn_ceportal_aci_render_section_links($claim_id); ?></div>
                        <div class="aci-section-content-wrapper"><?php print ncn_ceportal_review_render_claim_additional_data($claim_id); ?></div>
                    </div>
                </div>
            </div>          
        </div>
    </div>
</div>
<?php
    $output = ob_get_contents();
    ob_end_clean();
    return $output;
}

/**
 * Render Top Section Links (under Additional Claim Info)
 */ 
function ncn_ceportal_aci_render_section_links($claim_id) {
    $output = '<ul class="clearfix">';
    $sections = ncn_ceportal_get_aci_sections();
    foreach ($sections as $key=>$section) {
        $class = 'aci-si-'.$key;
        
        $div_wrapper = "aci_".$key."_section";
        $link_id = "aci_link_$key";
        $output.= "<li class='aci-section-link-item $class'>";
        $output.= "<a href='$div_wrapper' id='$link_id' onclick='show_aci_section(\"$link_id\"); return false;'><span>". ($section['short_title']? $section['short_title'] : $section['title']) ."</span></a>";
        $output.= '</li>';
    }
    $output.= '</ul>';
    
    drupal_add_js("
            jQuery(document).ready(function() {
                show_aci_section('aci_link_property_owner_info');
            }); ", 'inline'
    );
    
    return $output;
    
}


function ncn_ceportal_review_render_sub_page_links($claim_id, $type, $room_name='') {
    $output = "<ul class='review-sub-page-links clearfix'>";
    $active = ''; if ($type == 'additional_data') { $active='active'; }
    $output.= "<li class='review-sp-link $active'>".l('Additional Data', "ce_portal/review/claim/$claim_id/additional_data")."</li>";
    
    $rooms = ncn_admin_get_claim_roomname_data($claim_id, TRUE);
    foreach ($rooms as $r_data) {
        $active = '';
        if ($r_data['room_name'] == $room_name) {
            $active = 'active';
        }
        $output.= "<li class='review-sp-link $active'>".l($r_data['room_name'], "ce_portal/review/claim/$claim_id/room/".$r_data['room_name']."/scopesheet")."</li>";
    }
    $output.= "</ul>";
    
    return $output;
}

function ncn_ceportal_review_render_claim_additional_data($claim_id) {
    ob_start();
?>
<div id='aci_addtional_claims_data_view'>
    <div class="aci-section-content">
    <?php 
        $sections = ncn_ceportal_get_aci_sections();
        foreach ($sections as $section=>$s_info) {
            if ($s_info['render_func'] && function_exists($s_info['render_func'])) {
                $div_wrapper = "aci_".$section."_section";
                echo "<div id='$div_wrapper' class='aci-section' >";
                print call_user_func($s_info['render_func'], $claim_id, $section);
                echo "</div>";
            }
        }
    ?>
    </div>
</div>

<?php 
    $output = ob_get_contents();
    ob_end_clean();
    return $output;
}

function ncn_ceportal_get_aci_sections() {
    return array(
        'property_owner_info'   => array('title'=>'Property Owner Information', 'render_func'=>'ncn_ceportal_aci_render_property_owner_info' ), 
        'claim_information'         => array('title'=>'Claim Information', 'render_func'=>'ncn_ceportal_aci_render_claim_information'), 
        'daily_claim_log'           => array('title'=>'Daily Claim Log', 'render_func'=>'ncn_ceportal_aci_render_daily_claim_log'), 
        'comments'                          => array('title'=>'Comments', 'render_func'=>'ncn_ceportal_aci_render_comments'), 
    );
}

/**
 * ACI Render - "Property Owner Information" Section
 */ 
function ncn_ceportal_aci_render_property_owner_info($claim_id, $section) {
    $data = ncn_admin_get_aci_property_owner_info($claim_id);
    if (empty($data)) return '';
    
    ob_start();
?>
<table>
    <?php if ($data['insured_zip']): ?>
    <tr>
        <td class="td-label"><label for="poi_insured_zip">Zip Code:</label></td>
        <td><div class="field-item text-field-item"><?php echo $data['insured_zip']; ?></div></td>
    </tr><?php endif; ?>
</table>
<?php   
    $output = ob_get_contents();
    ob_end_clean();
    
    return $output;
}

/**
 * ACI Render - "Information" Section
 */ 
function ncn_ceportal_aci_render_claim_information($claim_id, $section) {
    $data = ncn_admin_get_aci_claim_information($claim_id);
    if (empty($data)) return '';
    
    ob_start();
?>
<table>
    <?php if ($data['us_to_build']): ?>
    <tr>
        <td class="td-label"><label for="ci_us_to_build">You would like us to build:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><?php if($data['us_to_build']=='Invoice for Services Already Provided') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_utb_invoice_for_already_provided">Invoice for Services Already Provided</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['us_to_build']=='Estimate') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_utb_estimate">Estimate</label></li>
            </ul>
        </td>
    </tr><?php endif; ?>
    <?php if ($data['service_call']): ?>
    <tr>
        <td class="td-label"><label for="ci_service_call">Service Call:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><?php if($data['service_call']=='Normal Business Hrs') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_sc_normal_business">Normal Business Hrs</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['service_call']=='After Hours') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_sc_after">After Hours</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['service_call']=='None') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_sc_none">None</label></li>
            </ul>
        </td>
    </tr><?php endif; ?>
    <?php if ($data['date_of_service']): ?>
    <tr>
        <td class="td-label"><label for="ci_date_of_service">Date of Service:</label></td>
        <td><div class="field-item text-field-item small-size"><?php echo $data['date_of_service']; ?></div></td>
    </tr><?php endif; ?>
    <?php if ($data['date_of_completion']): ?>
    <tr>
        <td class="td-label"><label for="ci_date_of_completion">Date of Completion:</label></td>
        <td><div class="field-item text-field-item small-size"><?php echo $data['date_of_completion']; ?></div></td>
    </tr><?php endif; ?>
    <?php if ($data['type_of_claim']): ?>
<!--    <tr>
        <td class="td-label"><label for="ci_type_of_claim">Type of Claim:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><?php if($data['type_of_claim']=='Residential') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_toc_residential">Residential</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['type_of_claim']=='Commercial') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_toc_commercial">Commercial</label></li>
            </ul>
        </td>
    </tr><?php endif; ?>
    <?php if ($data['type_of_loss']): ?>
    <tr>
        <td class="td-label"><label for="ci_type_of_loss">Type of Loss:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><?php if($data['type_of_loss']=='Water') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_tol_water">Water</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['type_of_loss']=='Mold') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_tol_mold">Mold</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['type_of_loss']=='Reconstruction') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_tol_reconstruction">Reconstruction</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['type_of_loss']=='Structure Cleaning') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_tol_structure_cleaning">Structure Cleaning</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['type_of_loss']=='Contents Cleaning') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_tol_contents_cleaning">Contents Cleaning</label></li>
            </ul>
        </td>
    </tr>--><?php endif; ?>
    <?php if ($data['cause_origin_of_loss']): ?>
    <tr>
        <td class="td-label"><label for="ci_cause_origin_of_loss">Cause and Origin of Loss:</label></td>
        <td><div class="field-item text-field-item"><?php echo $data['cause_origin_of_loss']; ?></div></td>
    </tr><?php endif; ?>
    <?php if ($data['sketch_purchased']): ?>
    <!-- <tr>
        <td class="td-label"><label for="ci_sketch_purchased">Sketch Purchased:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><?php if($data['sketch_purchased']=='Yes') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_sp_yes">Yes</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['sketch_purchased']=='No') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_sp_no">No</label></li>
            </ul>
        </td>
    </tr> --><?php endif; ?>
    <?php if ($data['category_of_water']): ?>
    <tr>
        <td class="td-label"><label for="ci_category_of_water">Category of Water:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><?php if($data['category_of_water']=='1') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_cow_1">1</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['category_of_water']=='2') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_cow_2">2</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['category_of_water']=='3') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_cow_3">3</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['category_of_water']=='No Water') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_cow_no_water">No Water</label></li>
            </ul>
        </td>
    </tr><?php endif; ?>
    <?php if ($data['class_of_loss']): ?>
    <tr>
        <td class="td-label"><label for="ci_class_of_loss">Class of Loss:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><?php if($data['class_of_loss']=='1') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_col_1">1</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['class_of_loss']=='2') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_col_2">2</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['class_of_loss']=='3') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_col_3">3</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['class_of_loss']=='4') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_col_3">4</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['class_of_loss']=='No Water') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_col_no_water">No Water</label></li>
            </ul>
        </td>
    </tr><?php endif; ?>
    <?php if ($data['number_of_stories_affected']): ?>
    <tr>
        <td class="td-label"><label for="ci_number_of_stories_affected">Number of Stories Affected:</label></td>
        <td><div class="field-item text-field-item small-size"><?php echo $data['number_of_stories_affected']; ?></div></td>
    </tr><?php endif; ?>
    <?php if ($data['deductible']): ?>
    <tr>
        <td class="td-label"><label for="ci_deductible">Deductible:</label></td>
        <td><div class="field-item text-field-item small-size"><span> $ </span><?php echo $data['deductible']; ?></div></td>
    </tr><?php endif; ?>
    <?php if ($data['deductible_collected']): ?>
    <tr>
        <td class="td-label"><label for="ci_deductible_collected">Deductible Collected:</label></td>
        <td><div class="field-item text-field-item small-size"><span> $ </span><?php echo $data['deductible_collected']; ?></div></td>
    </tr><?php endif; ?>
    <?php if ($data['vehicle_fuel']): ?>
    <tr>
        <td class="td-label"><label for="ci_vehicle_fuel">Vehicle Fuel:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><?php if($data['vehicle_fuel']=='Yes($45)') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_vf_yes_45">Yes($45)</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['vehicle_fuel']=='Yes(Other @d)') { echo "&#9745;"; } else { echo "&#9744;"; } ?> 
                    <label for="ci_vf_yes_other">Yes(Other <?php if($data['vehicle_fuel_other']): ?>$<span class="field-item text-field-item small-size"><?php echo $data['vehicle_fuel_other']; ?></span><?php endif; ?>)</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['vehicle_fuel']=='No') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_vf_no">No</label></li>
            </ul>
        </td>
    </tr><?php endif; ?>
    <?php if ($data['haul_debris']): ?>
    <tr>
        <td class="td-label"><label for="ci_haul_debris">Haul Debris:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><?php if($data['haul_debris']=="Yes(@d Loads)") { echo "&#9745;"; } else { echo "&#9744;"; } ?> 
                    <label for="ci_hd_yes_load">Yes(# <span class="field-item text-field-item small-size"><?php echo $data['haul_debris_yes_load']; ?> Loads</span>)</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['haul_debris']=="Yes(@d YD Dumpster)") { echo "&#9745;"; } else { echo "&#9744;"; } ?> 
                    <label for="ci_hd_yes_yd">Yes(# <span class="field-item text-field-item small-size"><?php echo $data['haul_debris_yes_yd']; ?> YD Dumpster</span>)</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['haul_debris']=='No') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_hd_no">No</label></li>
            </ul>
        </td>
    </tr><?php endif; ?>
    <?php if ($data['equipment_fuel']): ?>
    <tr>
        <td class="td-label"><label for="ci_equipment_fuel">Equipment Fuel:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><?php if($data['equipment_fuel']=="Yes(@d)") { echo "&#9745;"; } else { echo "&#9744;"; } ?> 
                    <label for="ci_ef_yes">Yes(# <span class="field-item text-field-item small-size"><?php echo $data['equipment_fuel_yes_price']; ?> Gallons</span>)</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['equipment_fuel']=='No') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_ef_no">No</label></li>
            </ul>
        </td>
    </tr><?php endif; ?>
    <?php if ($data['base_services_charges']): ?>
    <tr>
        <td class="td-label"><label for="ci_base_services_charges">Base Services Charges:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><?php if($data['base_services_charges']=='Yes') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_bsc_yes">Yes</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['base_services_charges']=='No') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_bsc_no">No</label></li>
            </ul>
        </td>
    </tr><?php endif; ?>
    <?php if ($data['overhead_profit']): ?>
    <tr>
        <td class="td-label"><label for="ci_overhead_profit">Overhead & Profit:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><?php if($data['overhead_profit']=='Yes') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_op_yes">Yes</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['overhead_profit']=='Yes(Cumulative)') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_op_yes_cumulative">Yes(Cumulative)</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['overhead_profit']=='No') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_op_no">No</label></li>
            </ul>
        </td>
    </tr><?php endif; ?>
    
    <!--<tr>
        <td class="td-label"><label for="ci_admin_sup_tim_on_loss">Admin/Supervisory Time on Loss:</label></td>
        <td><div class="field-item text-field-item small-size"><input type="text" name="ci[admin_sup_tim_on_loss]" id="ci_admin_sup_tim_on_loss" value="<?php echo $data['admin_sup_tim_on_loss']; ?>" /><span> Hrs (+1.75 Hrs) </span></div></td>
    </tr> -->   
    <?php if ($data['project_management']): ?>
    <tr>
        <td class="td-label"><label for="ci_project_management">Project Management Time:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><?php if($data['project_management']=='Yes') { echo "&#9745;"; } else { echo "&#9744;"; } ?>
                    <label for="ci_pm_yes">Yes<?php if($data['project_management_time']): ?>(<span class="field-item text-field-item small-size"><?php echo $data['project_management_time']; ?> Hours</span>)<?php endif; ?></label>&nbsp;&nbsp;&nbsp;
                </li>
                <li class="inline-field-item"><?php if($data['project_management']=='No') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_pm_no">No</label></li>
            </ul>
        </td>
    </tr><?php endif; ?>
    <?php if ($data['thermal_image']): ?>
    <tr>
        <td class="td-label"><label for="ci_thermal_image">Thermal Image:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><?php if($data['thermal_image']=='Yes($150)') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_ti_yes_150">Yes($150)</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['thermal_image']=='Other(@d)') { echo "&#9745;"; } else { echo "&#9744;"; } ?> 
                    <label for="ci_ti_other">Other<?php if($data['thermal_image_other_price']): ?>(<span>$ </span><span class="field-item text-field-item small-size"><?php echo $data['thermal_image_other_price']; ?></span>)<?php endif; ?></label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['thermal_image']=='No') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_ti_no">No</label></li>
            </ul>
        </td>
    </tr><?php endif; ?>
    <?php if ($data['ppe']): ?>
    <tr>
        <td class="td-label"><label for="ci_ppe">PPE (coverall, tape, gloves, masks):</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><?php if($data['ppe']=='Standard $65') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_ppe_standard_65">Standard $65</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['ppe']=='Other(@d)') { echo "&#9745;"; } else { echo "&#9744;"; } ?> 
                    <label for="ci_ppe_other">Other<?php if($data['ppe_other_price']): ?>(<span>$ </span><span class="field-item text-field-item small-size"><?php echo $data['ppe_other_price']; ?></span>)<?php endif; ?></label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['ppe']=='No') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_ppe_no">No</label></li>
            </ul>
        </td>
    </tr><?php endif; ?>
    <?php if ($data['respirators']): ?>
    <tr>
        <td class="td-label"><label for="ci_respirators">Respirators:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><?php if($data['respirators']=='Full') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_r_full">Full</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['respirators']=='Half') { echo "&#9745;"; } else { echo "&#9744;"; } ?> 
                    <label for="ci_r_half">Half <?php if($data['respirators_half_ea'] || $data['respirators_half_days']): ?>
                        <span class="field-item text-field-item small-size"><?php echo $data['respirators_half_ea']; ?></span><span>EA x</span>
                        <span class="field-item text-field-item small-size"><?php echo $data['respirators_half_days']; ?></span><span>Days</span><?php endif; ?>
                    </label>
                </li>
            </ul>
        </td>
    </tr><?php endif; ?>
    <?php if ($data['respirator_cartridges']): ?>
    <tr>
        <td class="td-label"><label for="ci_respirator_cartridges">Respirator Cartridges:</label></td>
        <td><div class="field-item text-field-item small-size"><?php echo $data['respirator_cartridges']; ?><span>EA</span></div></td>
    </tr><?php endif; ?>
    <?php if ($data['air_testing']): ?>
    <tr>
        <td class="td-label"><label for="ci_air_testing">Air Testing:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><?php if($data['air_testing']=="Yes(@d)") { echo "&#9745;"; } else { echo "&#9744;"; } ?> 
                    <label for="ci_at_yes">Yes <?php if ($data['air_testing_yes_price']): ?>(<span class="field-item text-field-item small-size"><span>$ </span><?php echo $data['air_testing_yes_price']; ?></span>)<?php endif; ?></label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['air_testing']=='No') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_at_no">No</label></li>
            </ul>
        </td>
    </tr><?php endif; ?>
    <?php if ($data['hvac_duct_cleaning']['type'] || $data['hvac_duct_cleaning']['rh_type']): ?>
    <tr>
        <td class="td-label"><label for="ci_hvac_duct_cleaning">HVAC/ Duct Cleaning:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><?php if($data['hvac_duct_cleaning']['type']=='Reg') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_hadc_reg">Reg</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['hvac_duct_cleaning']['type']=='Heavy') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_hadc_heavy">Heavy</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item">
                    <ul class="field-check-lists inline-group-fields clearfix">
                        <li class="inline-field-item"><?php if($data['hvac_duct_cleaning']['rh_type']=="Per Register") { echo "&#9745;"; } else { echo "&#9744;"; } ?> 
                            <label for="ci_hadc_rh_type_per_register">Per Register<?php if ($data['hvac_duct_cleaning']['rh_per_register']): ?>(# <span class="field-item text-field-item small-size"><?php echo $data['hvac_duct_cleaning']['rh_per_register']; ?></span>)<?php endif; ?></label>&nbsp;
                        </li>
                        <li class="inline-field-item"><?php if($data['hvac_duct_cleaning']['rh_type']=="Bid Price") { echo "&#9745;"; } else { echo "&#9744;"; } ?> 
                            <label for="ci_hadc_rh_type_bid_price">Bid Price<?php if ($data['hvac_duct_cleaning']['rh_bid_price']): ?>($ <span class="field-item text-field-item small-size"><?php echo $data['hvac_duct_cleaning']['rh_bid_price']; ?></span>)<?php endif; ?></label>
                        </li>
                    </ul>&nbsp;&nbsp;&nbsp;
                </li>
                <li class="inline-field-item"><?php if($data['hvac_duct_cleaning']['type']=='No') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_hadc_no">No</label></li>
            </ul>
        </td>
    </tr><?php endif; ?>
    <?php if ($data['clean_air_handler']): ?>
    <!-- One row more -->
    <tr>
        <td class="td-label"><label for="ci_clean_air_handler">Clean Air Handler:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><?php if($data['clean_air_handler']=='Yes($150)') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_cah_yes_150">Yes($150)</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['clean_air_handler']=='Yes(Other @d)') { echo "&#9745;"; } else { echo "&#9744;"; } ?> 
                    <label for="ci_cah_yes_other">Yes(Other $<span class="field-item text-field-item small-size"><?php echo $data['clean_air_handler_other']; ?></span>)</label>&nbsp;&nbsp;&nbsp;</li>
                <li class="inline-field-item"><?php if($data['clean_air_handler']=='No') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ci_cah_no">No</label></li>
            </ul>
        </td>
    </tr><?php endif; ?>
</table>
<?php
    $output = ob_get_contents();
    ob_end_clean();
    
    return $output;
}

/**
 * ACI Render - "Daily Claim Log" Section
 */ 
function ncn_ceportal_aci_render_daily_claim_log($claim_id, $section) { 
    $data = ncn_admin_get_aci_daily_claim_log($claim_id);
    if (empty($data)) return '';
    
    ob_start();
?>
<div id="dcl_log_list">
<table>
<thead>
    <th class='dcl-day-week'>Day of Week</th>
    <th class='dcl-date'>Date</th>
    <th class='dcl-men'># Men</th>
    <th class='dcl-hours-day'>Hours Per Day</th>
    <th class='dcl-total-hours'>Totoal Hours</th>
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
        <td class='dcl-operation'></td>
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
 * ACI Render - "Insurance Claim Policy" Section
 */ 
function ncn_ceportal_aci_render_insurance_claim_policy($claim_id, $section) {
    $data = ncn_admin_get_aci_insurance_claim_policy($claim_id);
    if (empty($data)) return '';
    
    ob_start();
?>
<table>
    <?php if($data['insured_name']): ?>
    <tr>
        <td class="td-label"><label for="icp_insured_name">Insured's Name:</label></td>
        <td><div class="field-item text-field-item"><?php echo $data['insured_name']; ?></div></td>
    </tr><?php endif; ?>
    <?php if($data['insurance_company']): ?>
    <tr>
        <td class="td-label"><label for="icp_insurance_company">Insurance Company:</label></td>
        <td><div class="field-item text-field-item"><?php echo $data['insurance_company']; ?></div></td>
    </tr><?php endif; ?>
    <?php if($data['insurance_company_phone']): ?>
    <tr>
        <td class="td-label"><label for="icp_insurance_company_phone">Insurance Company Phone:</label></td>
        <td>
            <div class="field-item text-field-item inline-field field-phone-number"><?php echo $data['insurance_company_phone']['number']; ?></div>&nbsp;&nbsp;&nbsp;
            <div class="field-item text-field-item inline-field field-phone-ext"><label for="icp_insurance_company_phone_ext">ext</label><?php echo $data['insurance_company_phone']['ext']; ?></div>
        </td>
    </tr><?php endif; ?>
    <?php if($data['insurance_agent']): ?>
    <tr>
        <td class="td-label"><label for="icp_insurance_agent">Insurance Agent:</label></td>
        <td><div class="field-item text-field-item"><?php echo $data['insurance_agent']; ?></div></td>
    </tr><?php endif; ?>
    <?php if($data['insurance_agent_phone']): ?>
    <tr>
        <td class="td-label"><label for="icp_insurance_agent_phone">Insurance Agent Phone:</label></td>
        <td>
            <div class="field-item text-field-item inline-field field-phone-number"><?php echo $data['insurance_agent_phone']['number']; ?></div>&nbsp;&nbsp;&nbsp;
            <div class="field-item text-field-item inline-field field-phone-ext"><label for="icp_insurance_agent_phone_ext">ext</label><?php echo $data['insurance_agent_phone']['ext']; ?></div>
        </td>
    </tr><?php endif; ?>
    <?php if($data['date_of_loss']): ?>
    <tr>
        <td class="td-label"><label for="icp_date_of_loss">Date of Loss:</label></td>
        <td><div class="field-item text-field-item"><?php echo $data['date_of_loss']; ?></div></td>
    </tr><?php endif; ?>
    <?php if($data['claim_adjuster']): ?>
    <tr>
        <td class="td-label"><label for="icp_claim_adjuster">Claim Adjuster:</label></td>
        <td><div class="field-item text-field-item"><?php echo $data['claim_adjuster']; ?></div></td>
    </tr><?php endif; ?>
    <?php if($data['adjuster_email']): ?>
    <tr>
        <td class="td-label"><label for="icp_adjuster_email">Adjuster Email:</label></td>
        <td><div class="field-item text-field-item"><?php echo $data['adjuster_email']; ?></div></td>
    </tr><?php endif; ?>
    <?php if($data['adjuster_phone']): ?>
    <tr>
        <td class="td-label"><label for="icp_insurance_agent_phone">Adjuster Phone:</label></td>
        <td>
            <div class="field-item text-field-item inline-field field-phone-number"><?php echo $data['adjuster_phone']['number']; ?></div>&nbsp;&nbsp;&nbsp;
            <div class="field-item text-field-item inline-field field-phone-ext"><label for="icp_adjuster_phone_ext">ext</label><?php echo $data['adjuster_phone']['ext']; ?></div>
        </td>
    </tr><?php endif; ?>
    <?php if($data['adjuster_alternate_phone_number']): ?>
    <tr>
        <td class="td-label"><label for="icp_adjuster_alternate_phone_number">Adjuster Alternate Phone:</label></td>
        <td>
            <div class="field-item text-field-item inline-field field-phone-number"><?php echo $data['adjuster_alternate_phone_number']['number']; ?></div>&nbsp;&nbsp;&nbsp;
            <div class="field-item text-field-item inline-field field-phone-ext"><label for="icp_adjuster_alternate_phone_ext">ext</label><?php echo $data['adjuster_alternate_phone']['ext']; ?></div>
        </td>
    </tr><?php endif; ?>
    <?php if($data['insurance_fax']): ?>
    <tr>
        <td class="td-label"><label for="icp_insurance_fax">Insurance Fax:</label></td>
        <td><div class="field-item text-field-item"><?php echo $data['insurance_fax']; ?></div></td>
    </tr><?php endif; ?>
    <?php if($data['effective_policy_date']): ?>
    <tr>
        <td class="td-label"><label for="icp_effective_policy_date">Effective Policy Date:</label></td>
        <td>
            <div class="field-item text-field-item inline-field field-date-from"><?php echo $data['effective_policy_date']['from']; ?></div>
            <div class="field-item text-field-item inline-field field-date-to"><span> - </span><?php echo $data['effective_policy_date']['to']; ?></div>
        </td>
    </tr><?php endif; ?>
    <?php if($data['policy_number']): ?>
    <tr>
        <td class="td-label"><label for="icp_policy_number">Policy Number:</label></td>
        <td><div class="field-item text-field-item"><?php echo $data['policy_number']; ?></div></td>
    </tr><?php endif; ?>
    <?php if($data['insurance_claim_number']): ?>
    <tr>
        <td class="td-label"><label for="icp_insurance_claim_number">Insurance Claim Number:</label></td>
        <td><div class="field-item text-field-item"><?php echo $data['insurance_claim_number']; ?></div></td>
    </tr><?php endif; ?>
</table>
<?php   
    $output = ob_get_contents();
    ob_end_clean();
    
    return $output;
}

/**
 * ACI Render - "Comments" Section
 */ 
function ncn_ceportal_aci_render_comments($claim_id, $section) {    
    $data = ncn_admin_get_aci_comments($claim_id);
    if (empty($data)) return '';
    ob_start();
?>
<table>
    <tr>
        <td class="td-label"><label for="comments_comments">Comments:</label></td>
        <td><div class="field-item text-field-item"><?php echo $data['content']; ?></div></td>
    </tr>
</table>
<?php
    $output = ob_get_contents();
    ob_end_clean();
    
    return $output;
}
?>
