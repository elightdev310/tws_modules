<div id="page_loading"></div>
<div id="page_results" class="claims-list-page">
    <div class="page-title-section clearfix">
        <div class="title page-title pull-left mr20">Claims</div>
        <div class="pull-left">
            <?php print theme_select(array(
                'element' => array(
                    '#options' => array_merge(array('all' => 'All Claims'), ncn_get_claim_status()),
                    '#value' => $filter, 
                    '#attributes' => array('class'=>array('claim-list-filter')), 
                ))); ?>
        </div>
    </div>
    <div class="panel-box">
        <div class="panel-box-content">
            <div class="ncn-data-table table-responsive">
            <table class="table">
            <thead>
                <tr>
                    <th class="td-created-at">Created Date</th>
                    <th class="td-insured-name">Insured Name</th>
                    <th class="td-property-loss-address">Property Loss Address</th>
                    <th class="td-claim-processing">Claim Processing</th>
                    <th class="td-rooms">Rooms</th>
                    <th class="td-photo-album">Photo Album</th>
                    <th class="td-claim-status">Status</th>
                    <th class="td-create-my-invoice">Buy My Claim</th>
                </tr>
            </thead>
            <tbody>
            <?php if (empty($claims)): ?>
                <tr>
                    <td colspan="10">No claim</td>
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
                        case 'incomplete':
                            $editable = true;
                            $send_to_archive = false;
                            $send_to_admin = true;
                            break;
                        case 'out for review':
                        case 'returned':
                        case 'approved':
                            $editable = true;
                            $send_to_archive = false;
                            $send_to_admin = false;
                            $approve_deny = true;
                            break;
                        case 'archived': 
                            $editable = false;
                            break;
                    }

                    $result2 = db_query('SELECT * FROM {claims_invoices} WHERE claim_id=:a AND live=1',
                        array(':a'=>$row['claim_id']));
                        $live_claim_count = $result2->rowCount();

                    $claim_locked = false;
                    if (ncn_admin_get_claim_first_free_locked($row['claim_id']) == 'LOCKED') {
                        $claim_locked = true;
                    }
                ?>
                    <tr>
                        <td class="td-created-at">
                            <a href="<?php print url("account/edit_claim/".$row['claim_id']) ?>" class="">
                                <?php echo strTime($row['created']) ?>
                            </a>
                        </td>
                        <td class="td-insured-name">
                            <a href="<?php print url("account/edit_claim/".$row['claim_id']) ?>" class="">
                                <?php echo ncn_cd($row['claim_id'], 'customer_name') ?>
                            </a>
                        </td>
                        <td class="td-property-loss-address">
                            <?php echo strClaimAddress($row['claim_id']) ?>
                        </td>
                        <td class="td-claim-processing">
                            <?php if ($send_to_admin == true && !is_leaduser($user)): ?>
                                <a href="<?php print url("account/edit_claim/".$row['claim_id']) ?>" class="btn btn-primary">Edit</a>
                            <?php endif; ?>
                        </td>
                        <td class="td-rooms">
                            <?php if ($editable == true): ?>
                            <a class="btn btn-primary" onclick="open_scopesheet_edit_box(<?php echo $row['claim_id']; ?>)">Edit</a>
                            <?php endif; ?>
                        </td>
                        <td class="td-photo-album">
                            <?php if ($editable == true): ?>
                                <a class="enabled btn btn-primary" onclick='open_edit_box(<?php echo $row['claim_id']; ?>)'>
                                    <span class="btn-camera-icon">Edit Album</span>
                                </a>
                            <?php endif; ?>
                        </td>
                        <td class="td-claim-status">
                            <?php echo ncn_claim_manager_claim_status_style($row['claim_status']) ?>
                        </td>
                        <td class="td-create-my-invoice">
                            <?php if ($send_to_admin == true && !is_leaduser($user)): ?>
                            <a class="create-invoice-btn enabled colorbox-node btn btn-primary" href="<?php echo $base_url; ?>/account/confirm_submit_claim/<?php echo $row['claim_id']; ?>?width=700&height=540">Buy My Claim</a>
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
            window.location.href = Drupal.settings.basePath + "account/claims.html?filter="+filter;
        });
    });
});
</script>
