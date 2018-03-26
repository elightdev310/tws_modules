<div class="panel-box">
<div class="panel-box-content">
<form method="POST" id="create-claim-form" class="form-horizontal ncn-form-default">
    <input type="hidden" name="tfunction" value="process">
    <input type="hidden" name="page" value="1">

    <?php fm_draw_leaduser("Internal Use Only", "lead_user", false); ?>
    <div class="form-group">
        <div for="fm_date_of_loss" class="col-sm-3 control-label">Date of Loss:<font color=red>*</font> </div>
        <div class="col-sm-6 date-control">
            <input type="text" name="date_of_loss" id="fm_date_of_loss" class="form-control" value="<?= isset($_POST['date_of_loss'])?$_POST['date_of_loss']:''; ?>" >
        </div>
    </div>
    <div class="form-group">
        <div for="fm_customer_name" class="col-sm-3 control-label">Insured's Name:<font color=red>*</font> </div>
        <div class="col-sm-6">
            <input type="text" name="customer_name" id="fm_customer_name" class="form-control" value="<?= isset($_POST['customer_name'])?$_POST['customer_name']:''; ?>" autocomplete="off" >
        </div>
    </div>
    <div class="form-group">
        <div for="fm_insured_country" class="col-sm-3 control-label">Country:<font color=red>*</font> </div>
        <div class="col-sm-6">
            <?php draw_countryform_part(array(
                            'name'=>'insured_country', 
                            'class'=>'form-control', 
                            'disabled'=>false, 
                            'required'=>true)); ?>
        </div>
    </div>
    <div class="form-group">
        <div for="fm_insured_address" class="col-sm-3 control-label">Address:<font color=red>*</font> </div>
        <div class="col-sm-6">
            <input type="text" name="insured_address" id="fm_insured_address" class="form-control" value="<?= isset($_POST['insured_address'])?$_POST['insured_address']:''; ?>" autocomplete="off" >
        </div>
    </div>
    <div class="form-group">
        <div for="fm_insured_city" class="col-sm-3 control-label">City:<font color=red>*</font> </div>
        <div class="col-sm-6">
            <input type="text" name="insured_city" id="fm_insured_city" class="form-control" value="<?= isset($_POST['insured_city'])?$_POST['insured_city']:''; ?>" autocomplete="off" >
        </div>
    </div>
    <div class="form-group">
        <div for="fm_insured_state" class="col-sm-3 control-label">State:<font color=red>*</font> </div>
        <div class="col-sm-6">
            <?php draw_stateform_part(array(
                            'name'  => 'insured_state', 
                            'class' => 'form-control', 
                            'disabled' => false, 
                            'required' => true )); ?>
        </div>
    </div>
    <div class="form-group">
        <div for="fm_insured_zip" class="col-sm-3 control-label">Zip Code:<font color=red>*</font> </div>
        <div class="col-sm-6">
            <input type="text" name="insured_zip" id="fm_insured_zip" class="form-control" value="<?= isset($_POST['insured_zip'])?$_POST['insured_zip']:''; ?>" autocomplete="off" >
        </div>
    </div>
    <div class="form-group">
        <div for="fm_insured_phone_number" class="col-sm-3 control-label">Insured's Phone Number:<font color=red>*</font> </div>
        <div class="col-sm-6">
            <input type="text" name="insured_phone_number" id="fm_insured_phone_number" class="form-control" value="<?= isset($_POST['insured_phone_number'])?$_POST['insured_phone_number']:''; ?>" autocomplete="off" >
        </div>
    </div>
    <div class="form-group">
        <div for="fm_technician_service_name" class="col-sm-3 control-label">Technician Service Name:<font color=red>*</font> </div>
        <div class="col-sm-6">
            <input type="text" name="technician_service_name" id="fm_technician_service_name" class="form-control" value="<?= isset($_POST['technician_service_name'])?$_POST['technician_service_name']:''; ?>" autocomplete="off" >
        </div>
    </div>

    <div class="form-group">
        <div for="claim_type" class="col-sm-3 control-label">Claim Type:<font color=red>*</font> </div>
        <div class="col-sm-9">
            <div class="row">
                <div class="col-sm-4 col-xs-6">
                    <select id="claim_type" name="claim_type" class="form-control">
                        <option value="">-- Choose --</option>
                        <option value="residential" <?php echo ((isset($_POST['claim_type']) && $_POST['claim_type']=='residential') ? 'selected' : '');?>>Residential</option>
                        <option value="commercial" <?php echo ((isset($_POST['claim_type']) && $_POST['claim_type']=='commercial') ? 'selected' : '');?>>Commercial</option>
                    </select>
                </div>
                <div class="col-sm-4 col-xs-6 pl0">
                    <select id="claim_product" name="claim_product" class="form-control">
                        <option value="">-- Choose --</option>
                        <option value="Water" <?php echo ((isset($_POST['claim_product']) && $_POST['claim_product']=='Water') ? 'selected' : '');?>>Water</option>
                        <option value="Water With Sketch" <?php echo ((isset($_POST['claim_product']) && $_POST['claim_product']=='Water With Sketch') ? 'selected' : '');?>>Water With Sketch</option>
                   <!-- <option value="Fire" <?php echo ((isset($_POST['claim_product']) && $_POST['claim_product']=='Fire') ? 'selected' : '');?>>Fire</option>
                        <option value="Fire With Sketch" <?php echo ((isset($_POST['claim_product']) && $_POST['claim_product']=='Fire With Sketch') ? 'selected' : '');?>>Fire With Sketch</option>    -->
                        <option value="Fire/Smoke Contents Cleaning" <?php echo ((isset($_POST['claim_product']) && $_POST['claim_product']=='Fire/Smoke Contents Cleaning') ? 'selected' : '');?>>Contents Cleaning</option>
                        <option value="Fire/Smoke Structure Cleaning" <?php echo ((isset($_POST['claim_product']) && $_POST['claim_product']=='Fire/Smoke Structure Cleaning') ? 'selected' : '');?>>Structure Cleaning</option>
                        <option value="Mold" <?php echo ((isset($_POST['claim_product']) && $_POST['claim_product']=='Mold') ? 'selected' : '');?>>Mold</option>
                        <option value="Mold With Sketch" <?php echo ((isset($_POST['claim_product']) && $_POST['claim_product']=='Mold With Sketch') ? 'selected' : '');?>>Mold With Sketch</option>
                        <option value="Reconstruction" <?php echo ((isset($_POST['claim_product']) && $_POST['claim_product']=='Reconstruction') ? 'selected' : '');?>>Reconstruction</option>
                        <option value="Reconstruction With Sketch" <?php echo ((isset($_POST['claim_product']) && $_POST['claim_product']=='Reconstruction With Sketch') ? 'selected' : '');?>>Reconstruction With Sketch</option>
                    </select>
                </div>
                <div class="col-sm-4">
                    <div id="claim_price"></div>
                </div>
            </div>
        </div>
    </div>

    <div class="form-group">
        <div class="col-sm-3 col-xs-1 control-label"><input type="checkbox" name="expedite" id="fm_expedite" autocomplete="off"></div>
        <div class="col-sm-6 col-xs-10">
            <label for="fm_expedite" class="checkbox-label text-label">Check this box if you would like to <b>EXPEDITE</b> your claim. Additional fee to expedite is: <span id="expedite_price"></span></label>
        </div>
    </div>

    <div class="form-group">
        <div class="col-sm-6 col-sm-offset-3">
            <a class="create-claim-btn btn btn-primary btn-flat" href="javascript:void();" onclick="create_claim_page1()">Create a Claim</a>
                <span style="margin-top: 2px;">&nbsp; or &nbsp;<a href="<?= url('account'); ?>">cancel</a>
        </div>
    </div>
    <div class="text-label note mt30">
        <strong>Please Note:</strong> 
        By clicking the "Create a Claim" button, a Terms and Conditions Agreement will appear.<br/>
        To save this file you must first read and then click the "I Agree" button. At that time your
        credit card will be charged and processed for the agreed fee schedule for this claim file. If
        you select the "cancel" button, the claim will be deleted.
    </div>
</form>
</div>
</div>


<script type="text/javascript">
(function ($) {
    $(document).ready(function () {
        jQuery("#fm_insured_phone_number").mask("999-999-9999");

        $('#claim_type').bind('change', function(event){get_claim_invoice_price();});
        $('#claim_product').bind('change', function(event){get_claim_invoice_price();});
        $('#fm_expedite').bind('click', function(event){get_claim_invoice_expedite_price();});

        $( '#fm_date_of_loss' ).datepicker({
            showOn: "both",
            buttonImage: "<?php print url(drupal_get_path('module', 'ncn_report')."/images/calendar.gif"); ?>",
            buttonImageOnly: true,
        });
    });
})(jQuery);
</script>
