<?php
/**
 * ARS List
 */
$ars_file_status_info = ncn_ars_file_status();
?>
<div id="page_loading"></div>
<div id="page_results" class="claims-list-page $class_name;">
    <div class="page-title-section clearfix">
        <div class="title page-title pull-left mr20">ARS</div>
        <!-- <div class="pull-left">
            <?php print theme_select(array(
                'element' => array(
                    '#options' => array(
                            'all'           => 'All Claims', 
                            'incomplete'    => 'Active Claims', 
                            'out for review'=> 'Out for Review', 
                            'returned'      => 'Returned', 
                            'approved'      => 'Approved', 
                            'receivables'   => 'Receivables', 
                            'archived'      => 'Archived'
                        ), 
                    '#value' => $filter, 
                    '#attributes' => array('class'=>array('claim-list-filter')), 
                ))); ?>
        </div> -->
    </div>
    <div class="panel-box">
        <div class="panel-box-content">
            <div class="ncn-data-table table-responsive">
            <table class="table">
            <thead>
                <tr>
                    <th class="td-id">ID</th>
                    <th class="td-created-at">Created Date</th>
                    <th class="td-claim-status">Status</th>
                    <th class="td-insured-name">Insured Name</th>
                    <th class="td-property-loss-address">Property Loss Address</th>
                    <th class="td-ars-file-status">ARS File Status</th>
                    <th class="td-ars-info">ARS Info</th>
                    <th class="td-approve-invoice">Approve Invoice</th>
                </tr>
            </thead>
            <tbody>
            <?php if (empty($claims)): ?>
                <tr>
                    <td colspan="7">No claim</td>
                </tr>
            <?php else: ?>
                <?php foreach($claims as $row): ?>
                <?php 
                    $row = (array)$row;
                    $editable = true;
                    $send_to_archive = false;
                    $send_to_admin = false;
                    $approve_deny = false;
                    switch ($row['claim_status']) {
                        case 'receivables':
                            $editable = false;
                            $send_to_archive = true;
                            break;
                    }
                ?>
                    <tr>
                        <td class="td-id">
                            <a href="<?php print url('account/ar/'.$row['claim_id']); ?>">
                                <?php echo $row['claim_id'] ?>
                            </a>
                        </td>
                        <td class="td-created-at">
                            <?php echo strTime($row['created']) ?>
                        </td>
                        <td class="td-claim-status">
                            <?php echo ncn_claim_manager_claim_status_style($row['claim_status']) ?>
                        </td>
                        <td class="td-insured-name">
                            <?php echo ncn_cd($row['claim_id'], 'customer_name') ?>
                        </td>
                        <td class="td-property-loss-address">
                            <?php echo strClaimAddress($row['claim_id']) ?>
                        </td>
                        <td class="td-ars-file-status">
                            <?php echo $ars_file_status_info[$row['ars_file_status']]; ?>
                        </td>
                        <td class="td-ars-info">
                            <a href="<?php print url('account/ar/'.$row['claim_id']); ?>" class="btn btn-primary">View</a>
                        </td>
                        <td class="td-approve-invoice">
                            <?php if ($row['ars_file_status'] == ARS_OUT_FOR_APPROVAL): ?>
                                <a href="#" class="btn btn-primary approve-btn ars-approve-invoice-btn" data-claim="<?php echo $row['claim_id']; ?>">Approve Invoice</a>
                            <?php endif; ?>
                        </td>
                    </tr>
                <?php endforeach; ?>
            <?php endif; ?>
            </tbody>
            </table>
            </div>

            <div class="ncn-table-pager">
                <?php print theme('pager'); ?>
            </div>
        </div>
    </div>
</div>


<script>
jQuery(function($) {
    $(document).ready(function() {
        $('.claim-list-filter').on('change', function() {
            var filter = $(this).val();
            window.location.href = Drupal.settings.basePath + "account/ars.html?filter="+filter;
        });
    });
});
</script>

<?php print ncn_edit_claim_ars_approve_invoice_js('.claims-list-page .panel-box'); ?>
