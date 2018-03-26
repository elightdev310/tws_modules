<?php
/**
 * AR Page
 *
 * @var: $claim_id, $claim, $member_user, $aci_icp
 */
$ars_file_status_info = ncn_ars_file_status();
?>

<div id="page_loading"></div>
<div id="page_results" class="ar-page">
    <div class="clearfix">
    <div class="claim-top-links-section">
        <!-- <div class="panel-box claim-link-list">
            <div class="panel-box-content">
                <a href="#" class="btn">New Note</a> 
                <a href="#" class="btn">Edit</a> 
            </div>
        </div> -->
        <?php if ($claim['ars_file_status'] == ARS_OUT_FOR_APPROVAL): ?>
            <a href="#" class="btn btn-primary claim-right-link ars-approve-invoice-btn" data-claim="<?php echo $claim_id; ?>">Approve Invoice</a>
        <?php endif; ?>
    </div>
    </div>

    <div class="ar-icon-section">
        <div>Account Receivables</div>
        <div class="name-info"><?php echo $claim_id; ?></div>
    </div>

    <div class="panel-box">
        <div class="panel-box-content">
            <div class="ncn-data-table table-responsive">
            <table class="table">
            <thead>
                <tr>
                    <th class="td-member-name">Member Name</th>
                    <th class="td-carrier-name">Carrier Name</th>
                    <th class="td-amount">Amount Agreed Upon by Member</th>
                    <th class="td-file-status">File Status</th>
                </tr>
            </thead>
            <tbody>
                <tr>
                    <td class="td-member-name">
                        <?php echo $member_user->profile_legalname; ?>
                    </td>
                    <td class="td-carrier-name">
                        <?php echo isset($aci_icp['insured_name'])?$aci_icp['insured_name']:''; ?>
                    </td>
                    <td class="td-amount">
                        <?php echo '$' . number_format($claim['payment_received'], 2); ?>
                    </td>
                    <td class="td-file-status">
                        <?php echo $ars_file_status_info[$claim['ars_file_status']]; ?>
                    </td>
                </tr>
            </tbody>
            </table>
            </div>
        </div>
    </div>

    <div class="row">
        <div class="ar-detail col-sm-7"> 
            <h3>DETAILS</h3>
            <div class="panel-box">
            <div class="panel-box-content">
                <?php print ncn_edit_claim_ar_details_panel($claim_id); ?>
            </div>
            </div>
        </div>
        <div class="col-sm-5"> 
            <?php print ncn_chatter_claim_feed_block($claim_id, 'ar'); ?>
        </div>
    </div>

</div>

<?php print ncn_edit_claim_ars_approve_invoice_js('.right-side'); ?>
