<?php
/**
 * 
 */
?>

<div id="page_loading"></div>
<div id="page_results" class="claim-processing-page">
    <div class="clearfix">
    <div class="claim-top-links-section">
        <div class="panel-box claim-link-list">
            <div class="panel-box-content">
                <?php if ($editable == true): ?>
                    <a class="btn" onclick="open_scopesheet_edit_box(<?php echo $claim_id; ?>)">EDIT ROOM(s)</a> 
                <?php else: ?>
                    <a class="btn disabled">EDIT ROOM(s)</a> 
                <?php endif; ?>
                <?php if ($editable == true): ?>
                    <a class="btn" onclick='open_edit_box(<?php echo $claim_id; ?>)'>EDIT PHOTO ALBUM</a> 
                <?php else: ?>
                    <a class="btn disabled">EDIT PHOTO ALBUM</a> 
                <?php endif; ?>
                <!-- <a href="#" class="btn">EDIT CLAIM</a>  -->
                <a href="#" class="btn new-monitoring-hours">NEW MONITORING HOURS</a> 
            </div>
        </div>
        <?php if ($send_to_admin == true && !is_leaduser($user)): ?>
            <a class="btn btn-primary claim-right-link create-invoice-btn enabled colorbox-node" href="<?php echo $base_url; ?>/account/confirm_submit_claim/<?= $claim_id; ?>?width=700&height=540">Create My Invoice</a>
        <?php else: ?>
            <a class="btn btn-primary disabled" >Create My Invoice</a>
        <?php endif; ?>
    </div>
    </div>

    <div class="claim-icon-section">
        <div>Claim</div>
        <div class="name-info"><?php echo ncn_cd($claim_id, 'customer_name') ?></div>
    </div>

    <div class="panel-box">
        <div class="panel-box-content">
            <div class="ncn-data-table table-responsive">
            <table class="table">
            <thead>
                <tr>
                    <th class="td-id">NCN Number</th>
                    <th class="td-claim-status">Claim Status</th>
                    <th class="td-property-loss-address">Property Loss Address</th>
                    <th class="td-claim-type">Claim Type</th>
                </tr>
            </thead>
            <tbody>
                <tr>
                    <td class="td-id">
                        <?php echo $claim_id ?>
                    </td>
                    <td class="td-claim-status">
                        <?php echo ncn_claim_manager_claim_status_style($claim['claim_status']) ?>
                    </td>
                    <td class="td-property-loss-address">
                        <?php echo strClaimAddress($claim_id) ?>
                    </td>
                    <td class="td-claim-type">
                        <?php print ucwords($claim['claim_type']) ?>
                    </td>
                </tr>
            </tbody>
            </table>
            </div>
        </div>
    </div>

    <div class="row">
        <div class="claims-detail col-sm-8 col-md-7"> 
            <h3>DETAILS</h3>
            <?php print ncn_edit_claim_aci_panel($claim_id); ?>
        </div>
        <div class="col-sm-4 col-md-5"> 
            <?php print ncn_chatter_claim_feed_block($claim_id); ?>
        </div>
    </div>

</div>

<script>
jQuery(function($) {
    $(document).ready(function() {
        $('.claim-top-links-section .new-monitoring-hours').on('click', function() {
            $('.claim-aci-section .aci-dcl-link').trigger('click');
        });
    });
});
</script>
