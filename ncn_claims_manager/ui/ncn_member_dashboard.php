<div id="page_loading"></div>
<div id="page_results" class="clearfix">
    <?php print ncn_member_logo_legalname_block($user->uid); ?>
    <!-- 3 Box -->
    <div class="row dashboard-claim-summary">
        <div class="col-sm-4">
            <div class="panel-box">
                <div class="panel-box-content">
                    <div class="box-title">Average Invoice Amount</div>
                    <div class="box-value"><?php print '$' . number_format($average_claim_value, 2); ?></div>
                </div>
            </div>
        </div>
        <div class="col-sm-4">
            <div class="panel-box">
                <div class="panel-box-content">
                    <div class="box-title">Number of MYNCN Claims Lifetime</div>
                    <div class="box-value"><?php print $total_claims; ?></div>
                </div>
            </div>
        </div>
        <div class="col-sm-4">
            <div class="panel-box">
                <div class="panel-box-content">
                    <div class="box-title">Total MYNCN Claim Value</div>
                    <div class="box-value"><?php print '$' . number_format($total_claim_value, 2); ?></div>
                </div>
            </div>
        </div>
    </div>

    <!--  -->
    <div class="dashboard-claim-section">
        <ul class="nav nav-pills">
            <li class="active"><a data-toggle="pill" href="#d_claims">Claims</a></li>
            <li><a data-toggle="pill" href="#d_invoices">Invoices</a></li>
            <li><a data-toggle="pill" href="#d_more_info">More Info Needed</a></li>
            <li><a data-toggle="pill" href="#d_invoices_completed">Invoices Completed</a></li>
            <li><a data-toggle="pill" href="#d_out_approval">Out for Approval</a></li>
            <li><a data-toggle="pill" href="#d_ars">ARS in Progress</a></li>
        </ul>

        <div class="tab-content panel-box">
            <div id="d_claims" class="tab-pane fade in active panel-box-content">
                <?php print ncn_claims_manager_dashboard_claims_section(); ?>
            </div>
            <div id="d_invoices" class="tab-pane fade panel-box-content">
              <?php print ncn_claims_manager_dashboard_invoices_section(); ?>
            </div>
            <div id="d_more_info" class="tab-pane fade panel-box-content">
              <?php print ncn_claims_manager_dashboard_more_info_needed_section(); ?>
            </div>
            <div id="d_invoices_completed" class="tab-pane fade panel-box-content">
              <?php print ncn_claims_manager_dashboard_invoices_completed_section(); ?>
            </div>
            <div id="d_out_approval" class="tab-pane fade panel-box-content">
              <?php print ncn_claims_manager_dashboard_out_for_approval_section(); ?>
            </div>
            <div id="d_ars" class="tab-pane fade panel-box-content">
              <?php print ncn_claims_manager_dashboard_ars_in_progress_section(); ?>
            </div>
        </div>
    </div>

    <!-- Chatter -->
    <?php if (module_exists('ncn_chatter')): ?>
    <div class="chatter-section">
        <div class="chatter-section-content">
            <?php print ncn_chatter_user_news_feed_block() ?>
        </div>
    </div>
    <?php endif; ?>
</div>
