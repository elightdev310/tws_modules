<?php
/**
 * Additional Claim Information
 *
 * @var
 *     $claim_id
 */
?>

<div class="claim-aci-section panel-box">
<form id='aci_addtional_claims_data_form' class="form-horizontal ncn-form-default" method='POST'>
    <input type='hidden' name='tfunction' value='additional_claims_data' />

    <ul class="nav nav-pills panel-box-content">
        <li class="active"><a data-toggle="pill" href="#aci_poi">Property Owner Info</a></li>
        <li><a data-toggle="pill" href="#aci_ci">Claim Information</a></li>
        <li><a data-toggle="pill" href="#aci_dcl" class="aci-dcl-link">Daily Claim Log</a></li>
        <li><a data-toggle="pill" href="#aci_icp">Insurance Policy Information</a></li>
        <li><a data-toggle="pill" href="#aci_comments">Comments</a></li>
    </ul>
    <div class="tab-content panel-box-content">
        <div id="aci_poi" class="tab-pane fade in active">
            <?php print ncn_edit_claim_aci_poi_panel($claim_id); ?>
        </div>
        <div id="aci_ci" class="tab-pane fade">
            <?php print ncn_edit_claim_aci_ci_panel($claim_id); ?>
        </div>
        <div id="aci_dcl" class="tab-pane fade">
            <?php print ncn_edit_claim_aci_dcl_panel($claim_id); ?>
        </div>
        <div id="aci_icp" class="tab-pane fade">
            <?php print ncn_edit_claim_aci_icp_panel($claim_id); ?>
        </div>
        <div id="aci_comments" class="tab-pane fade">
            <?php print ncn_edit_claim_aci_comments_panel($claim_id); ?>
        </div>
    </div>

    <div class='aci-actions text-center p20'><input type='submit' class="btn btn-primary pl30 pr30" value='Save'/></div>
</form>
</div>
