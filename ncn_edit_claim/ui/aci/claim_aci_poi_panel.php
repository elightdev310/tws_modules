<?php
/**
 * ACI - Property Onwer Information Panel
 *
 * @var
 *     $claim_id
 *     $data
 */
?>
<div class="claim-aci-poi-panel">
    <div class="form-group">
        <div for="poi_insured_name" class="col-sm-5 control-label">Owner/Insured Name: </div>
        <div class="col-sm-7">
            <input type="text" class="form-control" name="poi[insured_name]" id="poi_insured_name" value="<?php echo isset($data['insured_name'])?$data['insured_name']:''; ?>" />
        </div>
    </div>
    <div class="form-group">
        <div for="poi_insured_address" class="col-sm-5 control-label">Property Loss Address: </div>
        <div class="col-sm-7">
            <input type="text" class="form-control" name="poi[insured_address]" id="poi_insured_address" value="<?php echo isset($data['insured_address'])?$data['insured_address']:''; ?>" />
        </div>
    </div>
    <div class="form-group">
        <div for="poi_apt_bldg" class="col-sm-5 control-label">Apt/Suite/Bldg: </div>
        <div class="col-sm-7">
            <input type="text" class="form-control" name="poi[apt_bldg]" id="poi_apt_bldg" value="<?php echo isset($data['apt_bldg'])?$data['apt_bldg']:''; ?>" />
        </div>
    </div>
    <div class="form-group">
        <div for="poi_insured_city" class="col-sm-5 control-label">City: </div>
        <div class="col-sm-7">
            <input type="text" class="form-control" name="poi[insured_city]" id="poi_insured_city" value="<?php echo isset($data['insured_city'])?$data['insured_city']:''; ?>" />
        </div>
    </div>
    <div class="form-group">
        <div for="poi_insured_state" class="col-sm-5 control-label">State: </div>
        <div class="col-sm-7">
            <?php print ncn_loc_state(array(
                            'name'      => 'poi[insured_state]', 
                            'id'        => 'poi_insured_state', 
                            'class'     => 'form-control', 
                            'disabled'  => false, 
                            'required'  => false, 
                            'state'     => isset($data['insured_state'])?$data['insured_state']:'', 
                            'country'   => 'US', 
                        )); ?>
        </div>
    </div>
    <div class="form-group">
        <div for="poi_insured_zipcode" class="col-sm-5 control-label">Zip Code: </div>
        <div class="col-sm-7">
            <input type="text" class="form-control" name="poi[insured_zip]" id="poi_insured_zip" value="<?php echo isset($data['insured_zip'])?$data['insured_zip']:''; ?>" />
        </div>
    </div>

    <div class="form-group">
        <div for="poi_billing_insured_address" class="col-sm-5 control-label">Billing Address (if different): </div>
        <div class="col-sm-7">
            <input type="text" class="form-control" name="poi[billing_address]" id="poi_billing_address" value="<?php echo isset($data['billing_address'])?$data['billing_address']:''; ?>" />
        </div>
    </div>
    <div class="form-group">
        <div for="poi_billing_apt_bldg" class="col-sm-5 control-label">Apt/Suite/Bldg: </div>
        <div class="col-sm-7">
            <input type="text" class="form-control" name="poi[billing_apt_bldg]" id="poi_billing_apt_bldg" value="<?php echo isset($data['billing_apt_bldg'])?$data['billing_apt_bldg']:''; ?>" />
        </div>
    </div>
    <div class="form-group">
        <div for="poi_billing_city" class="col-sm-5 control-label">City: </div>
        <div class="col-sm-7">
            <input type="text" class="form-control" name="poi[billing_city]" id="poi_billing_city" value="<?php echo isset($data['billing_city'])?$data['billing_city']:''; ?>" />
        </div>
    </div>
    <div class="form-group">
        <div for="poi_billing_state" class="col-sm-5 control-label">State: </div>
        <div class="col-sm-7">
            <?php print ncn_loc_state(array(
                            'name'      => 'poi[billing_state]', 
                            'id'        => 'poi_billing_state', 
                            'class'     => 'form-control', 
                            'disabled'  => false, 
                            'required'  => false, 
                            'state'     => isset($data['billing_state'])?$data['billing_state']:'', 
                            'country'   => 'US', 
                        )); ?>
        </div>
    </div>
    <div class="form-group">
        <div for="poi_billing_zipcode" class="col-sm-5 control-label">Zip Code: </div>
        <div class="col-sm-7">
            <input type="text" class="form-control" name="poi[billing_zip]" id="poi_billing_zipcode" value="<?php echo isset($data['billing_zip'])?$data['billing_zip']:''; ?>" />
        </div>
    </div>

    <div class="form-group">
        <div for="poi_phone_0_number" class="col-sm-5 control-label">
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" id="poi_phone_0_type_home" name="poi[phone][0][type]" value="Home" <?php if(isset($data['phone'][0]['type'])&&$data['phone'][0]['type']=='Home') { echo "checked='checked'";} ?> ><label for="poi_phone_0_type_home">Home</label></li>
                <li class="inline-field-item"><input type="radio" id="poi_phone_0_type_work" name="poi[phone][0][type]" value="Work" <?php if(isset($data['phone'][0]['type'])&&$data['phone'][0]['type']=='Work') { echo "checked='checked'";} ?> ><label for="poi_phone_0_type_work">Work</label></li>
                <li class="inline-field-item"><input type="radio" id="poi_phone_0_type_cell" name="poi[phone][0][type]" value="Cell" <?php if(isset($data['phone'][0]['type'])&&$data['phone'][0]['type']=='Cell') { echo "checked='checked'";} ?> ><label for="poi_phone_0_type_cell">Cell:</label></li>
            </ul>
        </div>
        <div class="col-sm-7">
            <input type="text" class="form-control" name="poi[phone][0][number]" id="poi_phone_0_number" class="phone-style-field" onkeypress="return isNumberKey(event, false);" value="<?php echo isset($data['phone'][0]['number'])?$data['phone'][0]['number']:''; ?>" />
        </div>
    </div>
    <div class="form-group">
        <div for="poi_phone_1_number" class="col-sm-5 control-label">
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" id="poi_phone_1_type_home" name="poi[phone][1][type]" value="Home" <?php if(isset($data['phone'][1]['type'])&&$data['phone'][1]['type']=='Home') { echo "checked='checked'";} ?> ><label for="poi_phone_1_type_home">Home</label></li>
                <li class="inline-field-item"><input type="radio" id="poi_phone_1_type_work" name="poi[phone][1][type]" value="Work" <?php if(isset($data['phone'][1]['type'])&&$data['phone'][1]['type']=='Work') { echo "checked='checked'";} ?> ><label for="poi_phone_1_type_work">Work</label></li>
                <li class="inline-field-item"><input type="radio" id="poi_phone_1_type_cell" name="poi[phone][1][type]" value="Cell" <?php if(isset($data['phone'][1]['type'])&&$data['phone'][1]['type']=='Cell') { echo "checked='checked'";} ?> ><label for="poi_phone_1_type_cell">Cell:</label></li>
            </ul>
        </div>
        <div class="col-sm-7">
            <input type="text" class="form-control" name="poi[phone][1][number]" id="poi_phone_1_number" class="phone-style-field" onkeypress="return isNumberKey(event, false);" value="<?php echo isset($data['phone'][1]['number'])?$data['phone'][1]['number']:''; ?>" />
        </div>
    </div>
    <div class="form-group">
        <div for="poi_phone_2_number" class="col-sm-5 control-label">
            <ul class="field-check-lists inline-group-fields clearfix">
                <li class="inline-field-item"><input type="radio" id="poi_phone_2_type_home" name="poi[phone][2][type]" value="Home" <?php if(isset($data['phone'][2]['type'])&&$data['phone'][2]['type']=='Home') { echo "checked='checked'";} ?> ><label for="poi_phone_2_type_home">Home</label></li>
                <li class="inline-field-item"><input type="radio" id="poi_phone_2_type_work" name="poi[phone][2][type]" value="Work" <?php if(isset($data['phone'][2]['type'])&&$data['phone'][2]['type']=='Work') { echo "checked='checked'";} ?> ><label for="poi_phone_2_type_work">Work</label></li>
                <li class="inline-field-item"><input type="radio" id="poi_phone_2_type_cell" name="poi[phone][2][type]" value="Cell" <?php if(isset($data['phone'][2]['type'])&&$data['phone'][2]['type']=='Cell') { echo "checked='checked'";} ?> ><label for="poi_phone_2_type_cell">Cell:</label></li>
            </ul>
        </div>
        <div class="col-sm-7">
            <input type="text" class="form-control" name="poi[phone][2][number]" id="poi_phone_2_number" class="phone-style-field" onkeypress="return isNumberKey(event, false);" value="<?php echo isset($data['phone'][2]['number'])?$data['phone'][2]['number']:''; ?>" />
        </div>
    </div>
    <div class="form-group">
        <div for="poi_email" class="col-sm-5 control-label">Email: </div>
        <div class="col-sm-7">
            <input type="text" class="form-control" name="poi[property_owner_email]" id="poi_property_owner_email" value="<?php echo isset($data['property_owner_email'])?$data['property_owner_email']:''; ?>" />
        </div>
    </div>
</div>
