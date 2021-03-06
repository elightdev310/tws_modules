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
        <td class="td-label"><label for="ci_vehicle_fuel">Vehicle Fuel:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" name="ci[vehicle_fuel]" id="ci_vf_yes_45" value="Yes($45)" <?php if(isset($data['vehicle_fuel'])&&$data['vehicle_fuel']=='Yes($45)') { echo "checked='checked'";} ?> /><label for="ci_vf_yes_45">Yes($45)</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[vehicle_fuel]" id="ci_vf_yes_other" value="Yes(Other @d)" <?php if(isset($data['vehicle_fuel'])&&$data['vehicle_fuel']=='Yes(Other @d)') { echo "checked='checked'";} ?> class="cboxradio" />
                    <label class="cboxlabel" for="ci_vehicle_fuel_other">Yes(Other $<span class="field-item text-field-item small-size"><input type="text" name="ci[vehicle_fuel_other]" id="ci_vehicle_fuel_other" value="<?php echo isset($data['vehicle_fuel_other'])?$data['vehicle_fuel_other']:''; ?>" /></span>)</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[vehicle_fuel]" id="ci_vf_no" value="No" <?php if(isset($data['vehicle_fuel'])&&$data['vehicle_fuel']=='No') { echo "checked='checked'";} ?> /><label for="ci_vf_no">No</label></li>
            </ul>
        </td>
    <td class="reset-radio"><input type="button" class="btn  radio-reset-button" value="Reset" /></td>
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
    <td class="reset-radio"><input type="button" class="btn  radio-reset-button" value="Reset" /></td>
    </tr>
    <tr>
        <td class="td-label"><label for="ci_equipment_fuel">Equipment Fuel:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" name="ci[equipment_fuel]" id="ci_ef_yes" value="Yes(@d)" <?php if(isset($data['equipment_fuel'])&&$data['equipment_fuel']=="Yes(@d)") { echo "checked='checked'";} ?> class="cboxradio" />
                    <label class="cboxlabel" for="ci_haul_debris_yes_price">Yes (
                        <span class="field-item text-field-item small-size">Type: <input type="text" name="ci[equipment_fuel_yes_type]" id="ci_haul_debris_yes_type" value="<?php echo isset($data['equipment_fuel_yes_type'])?$data['equipment_fuel_yes_type']:''; ?>" /></span>&nbsp;
                        <span class="field-item text-field-item small-size">Gallons: <input type="text" name="ci[equipment_fuel_yes_price]" id="ci_haul_debris_yes_price" value="<?php echo isset($data['equipment_fuel_yes_price'])?$data['equipment_fuel_yes_price']:''; ?>" /></span>&nbsp;
                        <span class="field-item text-field-item small-size">$Fee: <input type="text" name="ci[equipment_fuel_yes_fee]" id="ci_haul_debris_yes_fee" value="<?php echo isset($data['equipment_fuel_yes_fee'])?$data['equipment_fuel_yes_fee']:''; ?>" /></span>
                        )
                    </label>
                </li>
                <li class="inline-field-item"><input type="radio" name="ci[equipment_fuel]" id="ci_ef_no" value="No" <?php if(isset($data['equipment_fuel'])&&$data['equipment_fuel']=='No') { echo "checked='checked'";} ?> /><label for="ci_ef_no">No</label></li>
            </ul>
        </td>
    <td class="reset-radio"><input type="button" class="btn  radio-reset-button" value="Reset" /></td>
    </tr>
    <tr style="display:none;">
        <td class="td-label"><label for="ci_base_services_charges">Base Services Charges:</label></td>
        <td>
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" name="ci[base_services_charges]" id="ci_bsc_yes" value="Yes" <?php if(isset($data['base_services_charges'])&&$data['base_services_charges']=='Yes') { echo "checked='checked'";} ?> /><label for="ci_bsc_yes">Yes</label></li>
                <li class="inline-field-item"><input type="radio" name="ci[base_services_charges]" id="ci_bsc_no" value="No" <?php if(isset($data['base_services_charges'])&&$data['base_services_charges']=='No') { echo "checked='checked'";} ?> /><label for="ci_bsc_no">No</label></li>
            </ul>
        </td>
    <td class="reset-radio"><input type="button" class="btn  radio-reset-button" value="Reset" /></td>
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
    <td class="reset-radio"><input type="button" class="btn  radio-reset-button" value="Reset" /></td>
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
    <td class="reset-radio"><input type="button" class="btn  radio-reset-button" value="Reset" /></td>
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
    <td class="reset-radio"><input type="button" class="btn  radio-reset-button" value="Reset" /></td>
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
    <td class="reset-radio"><input type="button" class="btn  radio-reset-button" value="Reset" /></td>
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
    <td class="reset-radio"><input type="button" class="btn  radio-reset-button" value="Reset" /></td>
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
    <td class="reset-radio"><input type="button" class="btn  radio-reset-button" value="Reset" /></td>
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
    <td class="reset-radio"><input type="button" class="btn  radio-reset-button" value="Reset" /></td>
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
    <td class="reset-radio"><input type="button" class="btn radio-reset-button" value="Reset" /></td>
    </tr>
</tbody>
</table>
</div>

<script>
jQuery(document).ready(function() { 
    bind_ncn_admin_aci_ci_radio_reset_buttons(); 
    bind_ncn_admin_aci_ci_cboxlabel_buttons();
});
</script>
