<?php
/*******************************************************************************
 * Dashboard
 ******************************************************************************/

function ncn_claim_manager_homepage()
{
    ?>
    <div id="page_loading"></div>
    <div id="page_results" class="clearfix">
        <?php ncn_claim_manager_homepage_innerpage(); ?>
    </div>
    <?php
    // Splash Page
    if (module_exists('ncn_splash')) {
        ncn_splash_open_prompt_pages();
    }
}

// -----------------------------------------------
function ncn_claim_manager_homepage_innerpage()
{
    GLOBAL $user;
    global $base_url;
    if(in_array("leaduser",$user->roles)===true){
        $GLOBALS['account_role'] = "leaduser";
    } else if (in_array("associate",$user->roles)===true) {
        $GLOBALS['account_role'] = "associate";
    } else {
        $GLOBALS['account_role'] = "user";
    }
    ?>
    <div class="page-content clearfix">
        <script type="text/javascript">
            var reload_url = "<?php echo $base_url; ?>/account/update_claim_preview/account-home";
        </script>
        <div>

            <?php
            if ($GLOBALS['account_role'] != "leaduser") {
                ?>
                <div class="add-claim-section clearfix"><?php _ncn_claim_manager_homepage_add_claim(); ?></div>
            <?php } ?>



            <?php if ($GLOBALS['account_role'] == "leaduser") { ?>
                <div class="claim-list-lead clearfix">
                    <div class="my-active-claims list"><?php _ncn_claim_manager_homepage_my_active_claims(4); ?></div>
                    <div class="out-for-review list"><?php _ncn_claim_manager_homepage_out_for_reviews(4); ?></div>
                    <div class="receivables list"><?php _ncn_claim_manager_homepage_receivables(3) ?></div>
                </div>
            <?php } else { ?>
                <div class="claim-list clearfix">
                    <div class="my-active-claims list"><?php _ncn_claim_manager_homepage_my_active_claims(4); ?></div>
                    <div class="out-for-review list"><?php _ncn_claim_manager_homepage_out_for_reviews(4); ?></div>
                    <div class="receivables list"><?php _ncn_claim_manager_homepage_receivables(3) ?></div>
                </div>
            <?php } ?>


        </div>
    </div>
    <div class="sponsor-section">
        <div class="sponsor-section-content">
            <div class="title">OUR SPONSOR</div>
            <?php _ncn_claim_manager_homepage_sponsor() ?>
        </div>
    </div>

    <?php if (module_exists('ncn_chatter')): ?>
    <div class="chatter-section">
        <div class="chatter-section-content">
            <?php print ncn_chatter_user_news_feed_block() ?>
        </div>
    </div>
    <?php endif; ?>
<?php
}

//----------------------------------------------------------------------

function _ncn_claim_manager_homepage_sponsor()
{
    echo ncn_sponsor_section();
}


function _ncn_claim_manager_homepage_add_claim()
{
    GLOBAL $user;
    global $base_url;
    if (is_leaduser($user)) {
        $GLOBALS['account_role'] = "leaduser";
    } else if (isset($user->roles[7])) {
        $GLOBALS['account_role'] = "associate";
    } else {
        $GLOBALS['account_role'] = "user";
    }

    ?>
    <div class="ahome-section-wrapper">
        <!--        KT start        -->
        <div>
            <a href="<?php echo  $base_url ?>/account/create-new-claim.html" id="ahome-addclaim-top-btn"
               class="clearfix"><span>Add a Claim</span></a>

            <div id="bottom-text">To create a new claim click the button above</div>
        </div>
        <div>
            <div id="message-title">Messages and Alerts</div>
            <div id="message-body" style="<?php if ($GLOBALS['account_role'] == "leaduser") {
                echo 'height:365px;';
            } ?>">
                <h2><?= nl2br(variable_get('ncn_msg_alert_title', '')); ?></h2>

                <p><?= nl2br(variable_get('ncn_msg_alert_text', '')); ?></p>
            </div>
        </div>
        <div>
        </div>
    </div>
<?php
}

//----------------------------------------------------------------------
function _ncn_claim_manager_homepage_my_active_claims($min_row) {

// param
    $order_by = "last_modified";
    //$additional_sql = ' AND `claim_status`!="out for review" AND `claim_status`!="returned" AND `claim_status`!="approved" AND `claim_status`!="archived" ';
    $additional_sql = " AND claim_status= 'incomplete' ";
    $editable = true;
    $send_to_archive = false;
    $send_to_admin = true;
    $approve_deny = false;

    ?>

<?php if ($GLOBALS['account_role'] == "leaduser"){ ?>
<div class="ahome-section-wrapper-lead">
<?php }else{ ?>
    <div class="ahome-section-wrapper">
        <?php } ?>
        <div class="clearfix">
            <div class="title">My Active Claims</div>
            <div class="help"><a href="#">Help</a></div>
        </div>
        <div class="table clearfix">
            <div class="table-header">
                <table>
                    <tr>
                        <td width="120px">Customer Name</td>
                        <td width="100px">Photo Album</td>
                        <td width="100px">Claims Processing</td>
                        <td>Create My Invoice</td>
                    </tr>
                </table>
            </div>
            <div class="table-body">
                <table>
                    <?php
                    GLOBAL $user;
                    GLOBAL $base_url;

                    if ($editable == true) {
                        $_editable = 1;
                    } else {
                        $_editable = 0;
                    }

                    if ($send_to_admin == true) {
                        $_send_to_admin = 1;
                    } else {
                        $_send_to_admin = 0;
                    }

                    if ($approve_deny == true) {
                        $_approve_deny = 1;
                    } else {
                        $_approve_deny = 0;
                    }

                    if (is_leaduser($user)) {
                        $query = "SELECT * FROM claims WHERE leaduser=" . $user->uid . " AND deleted=0 AND claim_status!='unpurchased' $additional_sql ORDER BY `$order_by` DESC limit 0, 4;";
                    } else {
                        $query = "SELECT * FROM claims WHERE user_id=" . $user->uid . " AND deleted=0 AND (claim_status!='unpurchased' ".$additional_sql.") ORDER BY `$order_by` DESC limit 0, 4;";
                    }
                    //echo $query;exit;
                    $result = db_query($query);                 
                    $row_count = $result->rowCount();
                    
                    $orders_total = 0;
                    $payment_received_total = 0;

                    $result_rec = $result->fetchAll();
                    foreach($result_rec as $row )
                    {
                        // get data from sql
                        $row = (array)$row;                     
                        $claim_id = $row['claim_id'];

                        // does the file note exists/not blank
                        if ($row['file_note'] == '') // give the file notes some default value
                        {
                            $row['file_note'] = base64_encode('[no note]');
                        }


                        // does an invoice exist?
                     //   $query2 = "SELECT * FROM claims_invoices WHERE claim_id=" . $claim_id . " AND `live`=2;";
                        $result2 = db_query('SELECT * FROM {claims_invoices} WHERE claim_id=:a AND live=2',
                            array(':a'=>$claim_id));
                        $live_invoice_count = $result2->rowCount();

                       // $query2 = "SELECT * FROM claims_invoices WHERE claim_id=" . $claim_id . " AND `live`=1;";
                        $result2 = db_query('SELECT * FROM {claims_invoices} WHERE claim_id=:a AND live=1',
                        array(':a'=>$claim_id));
                        $live_claim_count = $result2->rowCount();

                        // draw the row
                        ?>
                        <tr>
                            <td width="120px"><?php echo ncn_cd($claim_id, 'customer_name'); ?></td>

                            <?php // edit controls
                            if ($editable == true && !is_leaduser($user)) {
                                ?>
                                <td width="100px">
                                    <?php    if (($row['claim_status'] != "receivables") && ($row['claim_status'] != "paid in full")) {
                                        /*if ( ($row['claim_status'] == 'out for review') || ($row['claim_status'] == 'approved'))  {*/
                                        $disabled_string = 'enabled';
                                    } else {
                                        $disabled_string = 'disabled';
                                    }
                                    ?>
                                    <div class="edit-album-btn clearfix">
                                        <a href="#"
                                           class="<?php echo $disabled_string; ?>" <?php if ($row['claim_status'] == 'out for review') print 'disabled'; ?> <?php if ($disabled_string != 'disabled' && $row['claim_status'] != 'out for review') { ?> onclick='open_edit_box(<?php echo $claim_id; ?>, true)' <?php } ?>>
                                            Edit Album
                                        </a>
                                    </div>
                                    <?php //} ?>
                                </td>
                            <?php
                            } //editable==true;
                            else {
                                ?>
                                <td width="100px">
                                    <div class="edit-album-btn clearfix">
                                        <a href="#"
                                           class="" <?php if ($row['claim_status'] == 'out for review') print 'disabled'; ?> >
                                            Edit Album
                                        </a>
                                    </div>
                                </td>
                            <?php } ?>
                            <?php
                            if (!is_leaduser($user)) {
                                ?>
                                <td width="100px">
                                    <a href="<?php echo base_path() . "account/additional_claim_info/$claim_id"; ?>"
                                       class="edit-additional-data-btn">Edit</a>
                                </td>
                            <?php
                            } else {
                                ?>
                                <td width="100px">
                                    <a href="#"
                                       class="edit-additional-data-btn">Edit</a>
                                </td>
                            <?php
                            }?>
                            <?php
                            if (!is_leaduser($user)) {
                                ?>
                                <td>
                                    <!--<a href="#" class="create-invoice-btn enabled" onclick="open_submit_box(<?= $claim_id; ?>);return false;">Create My Invoice</a>-->
                                    <a class="create-invoice-btn enabled colorbox-node" href="<?php echo $base_url; ?>/account/confirm_submit_claim/<?= $claim_id; ?>?width=700&height=540">Create My Invoice</a>
                                </td>
                            <?php
                            } else {
                                ?>
                                <td><a href="#" class="create-invoice-btn enabled">Create My Invoice</a>
                                </td>
                            <?php
                            }?>
                        </tr>

                    <?php
                    } //end of for
                    for ($i = 0; $i < ($min_row - $row_count); $i++) {
                        print ('<tr>');
                        print ('<td colspan="4">&nbsp</td>');
                        print ('</tr>');
                    }
                    ?>
                </table>
            </div>
        </div>
        <div class="view-details clearfix">
            <a href="<?php echo $base_url ?>/account/my-active-claims.html">View Details</a>
        </div>
    </div>
<?php
}
//----------------------------------------------------------------------
function _ncn_claim_manager_homepage_out_for_reviews($min_row)
{

// param
    $order_by = "last_modified";
    $additional_sql = " AND (`claim_status`='out for review' OR `claim_status`='returned' OR `claim_status`='approved' ) ";
    $editable = true;
    $send_to_archive = false;
    $send_to_admin = false;
    $approve_deny = true;

    ?>

    <div class="ahome-section-wrapper">
        <div class="clearfix">
            <div class="title">Out for Review</div>
            <div class="help"><a href="#">Help</a></div>
        </div>
        <div class="table clearfix">
            <div class="table-header">
                <table>
                    <tr>
                        <td width="120px">Customer Name</td>
                        <td>Claim Status</td>
                    </tr>
                </table>
            </div>
            <div class="table-body">
                <table>
                    <?php
                    GLOBAL $user;
                    GLOBAL $base_url;

                    if ($editable == true) {
                        $_editable = 1;
                    } else {
                        $_editable = 0;
                    }

                    if ($send_to_admin == true) {
                        $_send_to_admin = 1;
                    } else {
                        $_send_to_admin = 0;
                    }

                    if ($approve_deny == true) {
                        $_approve_deny = 1;
                    } else {
                        $_approve_deny = 0;
                    }

                    if (is_leaduser($user)) {
                        $query = "SELECT * FROM claims WHERE leaduser=" . $user->uid . " AND deleted=0 AND claim_status!='unpurchased' $additional_sql ORDER BY `$order_by` DESC limit 0, 4;;";
                    } else {
                        $query = "SELECT * FROM claims WHERE user_id=" . $user->uid . " AND deleted=0 AND claim_status!='unpurchased' $additional_sql ORDER BY `$order_by` DESC limit 0, 4;;";
                    }
                    $result = db_query($query);
                    $row_count = $result->rowCount();
                    
                    $orders_total = 0;
                    $payment_received_total = 0;

                    $result_rec = $result->fetchAll();
                    foreach($result_rec as $row)
                    {
                        // get data from sql
                        $row = (array)$row;
                        $claim_id = $row['claim_id'];

                        // does the file note exists/not blank
                        if ($row['file_note'] == '') // give the file notes some default value
                        {
                            $row['file_note'] = base64_encode('[no note]');
                        }


                        // does an invoice exist?
                      //  $query2 = "SELECT * FROM claims_invoices WHERE claim_id=" . $claim_id . " AND `live`=2;";
                        $result2 = db_query('SELECT * FROM {claims_invoices} WHERE claim_id=:a AND live=2',
                        array(':a'=>$claim_id));
                        $live_invoice_count = $result2->rowCount();

                      //  $query2 = "SELECT * FROM claims_invoices WHERE claim_id=" . $claim_id . " AND `live`=1;";
                        $result2 = db_query('SELECT * FROM {claims_invoices} WHERE claim_id=:a AND live=1',
                        array(':a'=>$claim_id));
                        $live_claim_count = $result2->rowCount();

                        // draw the row
                        ?>
                        <tr>
                            <td width="120px"><?php echo ncn_cd($claim_id, 'customer_name'); ?></td>
                            <td><?= ncn_claim_manager_claim_status_style($row['claim_status']); ?></td>
                        </tr>

                    <?php
                    } //end of for
                    for ($i = 0; $i < ($min_row - $row_count); $i++) {
                        print ('<tr>');
                        print ('<td colspan="2">&nbsp</td>');
                        print ('</tr>');
                    }
                    ?>
                </table>
            </div>
        </div>
        <div class="view-details clearfix">
            <a href="<?php echo $base_url ?>/account/out-for-review.html">View Details</a>
        </div>
    </div>
<?php
}

//----------------------------------------------------------------------
function _ncn_claim_manager_homepage_receivables($min_row) {

// param
    $order_by = "last_modified";
    $additional_sql = " AND claim_status= 'receivables' ";
    $editable = false;
    $send_to_archive = true;
    $send_to_admin = false;
    $approve_deny = false;

    ?>

<?php if ($GLOBALS['account_role'] == "leaduser"){ ?>
<div class="ahome-section-wrapper-lead">
<?php }else{ ?>
    <div class="ahome-section-wrapper">
        <?php } ?>
        <div class="clearfix">
            <div class="title">Receivables</div>
            <div class="help"><a href="#">Help</a></div>
        </div>
        <div class="table clearfix">
            <div class="table-header">
                <table>
                    <tr>
                        <td width="140px">Customer Name</td>
                        <td width="150px">Functions</td>
                        <td width="150px">Claim Amounts</td>
                        <td>File Notes</td>
                    </tr>
                </table>
            </div>
            <div class="table-body">
                <table>
                    <?php
                    GLOBAL $user;
                    GLOBAL $base_url;

                    if ($editable == true) {
                        $_editable = 1;
                    } else {
                        $_editable = 0;
                    }

                    if ($send_to_admin == true) {
                        $_send_to_admin = 1;
                    } else {
                        $_send_to_admin = 0;
                    }

                    if ($approve_deny == true) {
                        $_approve_deny = 1;
                    } else {
                        $_approve_deny = 0;
                    }

                    if (is_leaduser($user)) {
                        $query = "SELECT * FROM claims WHERE leaduser=" . $user->uid . " AND deleted=0 AND claim_status!='unpurchased' $additional_sql ORDER BY `$order_by` DESC limit 0, 3;";
                    } else {
                        $query = "SELECT * FROM claims WHERE user_id=" . $user->uid . " AND deleted=0 AND claim_status!='unpurchased' $additional_sql ORDER BY `$order_by` DESC limit 0, 3;";
                    }
                    $result = db_query($query);
                    $row_count = $result->rowCount();

                    $orders_total = 0;
                    $payment_received_total = 0;

                    $result_rec = $result->fetchAll();                  
                    foreach($result_rec as $row)
                    {
                        // get data from sql
                        $row = (array)$row;
                        $claim_id = $row['claim_id'];

                        // does the file note exists/not blank
                        if ($row['file_note'] == '') // give the file notes some default value
                        {
                            $row['file_note'] = base64_encode('[no note]');
                        }


                        // does an invoice exist?
                      //  $query2 = "SELECT * FROM claims_invoices WHERE claim_id=" . $claim_id . " AND `live`=2;";
                        $result2 = db_query('SELECT * FROM {claims_invoices} WHERE claim_id=:a AND live=2',
                        array(':a'=>$claim_id));
                        $live_invoice_count = $result2->rowCount();

                      //  $query2 = "SELECT * FROM claims_invoices WHERE claim_id=" . $claim_id . " AND `live`=1;";
                        $result2 = db_query('SELECT * FROM {claims_invoices} WHERE claim_id=:a AND live=1',
                        array(':a'=>$claim_id));
                        $live_claim_count = $result2->rowCount();

                        // draw the row
                        ?>
                        <tr>
                            <td width="120px"><?php echo ncn_cd($claim_id, 'customer_name'); ?></td>
                            <td width="150px">
                                <?php    if ($send_to_archive == true && !is_leaduser($user)) // payment received functions
                                {
                                    ?>
                                    <a href="#" class="pay-full"
                                       onclick="archive_claim('<?= $base_url; ?>', <?= $claim_id; ?>, '<?= $row['payment_received']; ?>');return false;">Pay
                                        in Full</a>
                                    <a href="#" class="pay-partial"
                                       onclick="partial_payment('<?= $base_url; ?>', <?= $claim_id; ?>);return false;">Partial
                                        Payment Received</a>
                                <?php }else if ($send_to_archive == true){
        ?>

                                <a href="#" class="pay-full">Paid In Full</a>
                                <a href="#" class="pay-partial">Partial Payment Received</a>

                            <?php } ?>
                            </td>
                            <td width="150px">
                                <?php    if ($row['claim_amount'] > 0) // only show claim amount if > 0
                                {
                                    //$orders_total += $row['claim_amount'];
                                    print '$' . number_format($row['claim_amount'], 2);
                                }
                                ?>
                            </td>
                            <td>
                                <!-- <a class="file-notes" href="#" onclick="set_file_note('<?= $base_url; ?>', <?= $claim_id; ?>, true);return false;">Set File Notes</a> -->
                                <!--<span class="file-notes">file-note</span>-->
                                <?php    if (!is_leaduser($user)){ ?>
                                    <a class="file-notes" href="#" onclick="set_file_note('<?= $base_url; ?>', <?= $claim_id; ?>, true);return false;">Set File Notes</a>
                                <?php }else{ ?>
                                    <a class="file-notes" href="#">Set File Notes</a>
                                <?php } ?>
                            </td>
                        </tr>
                    <?php
                    } //end of for
                    for ($i = 0; $i < ($min_row - $row_count); $i++) {
                        print ('<tr>');
                        print ('    <td colspan="4">&nbsp</td>');
                        print ('</tr>');
                    }
                    ?>
                </table>
            </div>
        </div>
        <div class="view-details clearfix">
            <a href="<?php echo $base_url ?>/account/receivables.html">View Details</a>
        </div>
    </div>
<?php
}









// overview/list page(s)
function ncn_claim_manager_page($page_title, $class_name, $order_by = "created", $additional_sql = "", $editable = true, $send_to_archive = false, $send_to_admin = false, $approve_deny = false)
{
    ?>
    <div id="page_loading"></div>
    <div id="page_results">
    <div class="claims-detail <?php echo $class_name; ?>">
        <?php           
            ncn_claim_manager_page_internal($page_title, $class_name, $order_by, $additional_sql, $editable, $send_to_archive, $send_to_admin, $approve_deny);
        ?></div></div><?php
}

//------------------------------------------------------------------------------
// overview/list page(s)
function ncn_claim_manager_page_internal($page_title, $class_name, $order_by = "created", $additional_sql = "", $editable = true, $send_to_archive = false, $send_to_admin = false, $approve_deny = false)
{
    GLOBAL $user;
    GLOBAL $base_url;

    $colcount = 10;
    if ($class_name == 'my-active-claims-detail') {
        $colcount = 9;
    } else if ($class_name == 'out-for-review-detail') {
        $colcount = 10;
    } elseif ($class_name == 'archived-claims-detail') {
        $colcount = 10;
    } elseif ($class_name == 'recievables-detail') {
        $colcount = 11;
    }


    if ($editable == true) {
        $_editable = 1;
    } else {
        $_editable = 0;
    }

    if ($send_to_admin == true) {
        $_send_to_admin = 1;
    } else {
        $_send_to_admin = 0;
    }

    if ($approve_deny == true) {
        $_approve_deny = 1;
    } else {
        $_approve_deny = 0;
    }
    drupal_add_css(drupal_get_path('module', 'ncn_claims_manager') . '/ncn_claims_manager.css', array('group' => CSS_DEFAULT, 'type' => 'file'));
    ?>

    <script type="text/javascript">
        var reload_url = "/account/update_claim_preview/<?= base64_encode($order_by); ?>/<?= base64_encode($_editable); ?>/<?= base64_encode($_send_to_admin); ?>/<?= base64_encode($_approve_deny); ?>/<?= base64_encode($additional_sql); ?>";
    </script>
    <div class="claims-section-wrapper <?php echo $class_name; ?>">
    <div class="clearfix">
        <div class="title"><?php echo $page_title; ?></div>
        <div class="help"><a href="#">Help</a></div>
    </div>
    <div class="table clearfix">
    <div class="table-header">
        <table>
            <tr>
                <td class="td-id">ID</td>
                <td class="td-date">Date of Loss</td>
                <td class="td-customer-name">Customer Name</td>
                <td class="td-claim-file">Claim File</td>

                <?php if ($editable == true): ?>
                    <td class="td-photo-album">Photo Album</td>
                <?php endif; ?>
                <td class="td-claim-status">Claim Status</td>

                <?php if ($send_to_archive == true): ?>
                    <td class="td-payment-function">Functions</td>
                <?php endif; ?>

                <?php if ($class_name != 'my-active-claims-detail'): ?>
                    <td class="td-claim-amount">Claim Amount</td>
                <?php endif; ?>

                <?php if (($send_to_archive == true) || ($editable == false)): ?>
                    <td class="td-payment-received">Payment Received</td>
                    <td class="td-file-notes">File Notes</td>
                <?php endif; ?>

                <td class="td-inbox">Inbox</td>
                <?php if ($class_name != 'my-active-claims-detail'): ?>
                    <td class="td-invoice">Service Invoice</td>
                <?php endif; ?>

                <?php if ($approve_deny == true): ?>
                    <td class="td-final-approval">Final Approval</td>
                <?php endif; ?>

                <?php if ($send_to_admin == true): ?>
                    <td class="td-create-my-invoice">Create My Invoice</td>
                    <td class="td-edit-additional-data">Claims Processing</td>
                <?php endif; ?>
            </tr>
        </table>
    </div>
    <div class="table-body">
    <table class="claims_manager">
    <?php
    $user_id = $user->uid;
    
    if (is_subuser($user)) {    
        $parent_mid = ncn_subuser_get_memberid_from_subuid($user_id);
        $user_id = get_uid_from_memberid($parent_mid);
    }
    
    if (is_leaduser($user)) {
        $query = "SELECT * FROM claims WHERE leaduser=" . $user->uid . " AND deleted=0 AND claim_status!='unpurchased' ". $additional_sql." ORDER BY `$order_by` DESC";
    } else {
        $query = "SELECT * FROM claims WHERE user_id=" . $user_id . " AND deleted=0 AND claim_status!='unpurchased' ". $additional_sql." ORDER BY `$order_by` DESC";
    }
    
    $result = db_query($query);
    $row_count = $result->rowCount();

    $orders_total = 0;
    $payment_received_total = 0;

    $result_rec = $result->fetchAll();
    $i = 0;
    foreach($result_rec as $row)
    {
        // get data from sql
        $row = (array)$row;
        $claim_id = $row['claim_id'];

        // does the file note exists/not blank
        if ($row['file_note'] == '') // give the file notes some default value
        {
            $row['file_note'] = base64_encode('[no note]');
        }


        $live_invoice_count = get_claim_file_count($claim_id, 2);
        $live_claim_count = get_claim_file_count($claim_id, 1);

        $claim_locked = false;
        if (ncn_admin_get_claim_first_free_locked($claim_id) == 'LOCKED') {
            $claim_locked = true;
        }
        // draw the row
        ?>

        <tr>
            <td class="td-id">
                <a href="#" onclick="toggle_slide('basic_<?= $claim_id; ?>', 1);return false;"><?= $claim_id; ?></a>
            </td>
            <td class="td-date"><?= render_claim_date(ncn_cd($claim_id, 'date_of_loss')) ?></td>
            <td class="td-customer-name">
                <a href="#" onclick="toggle_slide('basic_<?= $claim_id; ?>', 1);return false;"><?= ncn_cd($claim_id, 'customer_name'); ?></a>
            </td>
            <td class="td-claim-file">
                <?php
                //if ( (($live_invoice_count > 0) && ($row['claim_status']=='approved')) || ( ($live_invoice_count > 0) && (($row['claim_status']=='receivables') || ($row['claim_status']=='paid in full')) ) )
                if ($live_invoice_count > 0): ?>
                    <a href="serve_invoice_file/<?= $claim_id; ?>/2" class="pdf-icon-link"></a>
                <?php else: ?>
                    <span>Not Received<span>
                <?php endif; ?>
            </td>

        <?php // edit controls
            if ($editable == true && !is_leaduser($user)): ?>
            <!-- updated by *** -->
            <td class="td-photo-album">
                <?php    
                if (($row['claim_status'] != "receivables") && ($row['claim_status'] != "paid in full")) {
                    if ( ($row['claim_status'] == 'out for review') || ($row['claim_status'] == 'approved')){ // Uncommented by KEYiDEAS on 28.10.2014
                    /*if ($row['claim_status'] == 'approved') {*/  // Commented by KEYiDEAS on 28.10.2014
                        $disabled_string = 'disabled';
                    } else {
                        $disabled_string = 'enabled';
                    }
                } else {
                    $disabled_string = 'disabled';
                }
                ?>
                <div class="edit-album-btn clearfix">
                    <!-- <a href="#" class="<?php echo $disabled_string; ?>" <?php if ($row['claim_status'] == 'out for review') print 'disabled'; ?> <?php if($disabled_string!='disabled' && $row['claim_status'] != 'out for review') { ?> onclick='open_edit_box(<?php echo $claim_id; ?>)' <?php } ?>> -->
                    <a href="#" class="<?php echo $disabled_string; ?>" <?php if ($disabled_string == 'disabled') print 'disabled'; ?> <?php if ($disabled_string != 'disabled') { ?> onclick='open_edit_box(<?php echo $claim_id; ?>)' <?php } ?>> Edit Album </a>
                </div>
                <?php //} ?>
            </td>
        <?php // end editable == true;
            elseif ($editable == true): ?>
            <td class="td-photo-album">
                <div class="edit-album-btn clearfix">
                    <a href="#" class="<?php echo $disabled_string; ?>">
                        Edit Album
                    </a>
                </div>
            </td>
        <?php endif; ?>

            <!-- <td><div class='status_<?= str_replace(' ', '_', $row['claim_status']); ?>'></div></td> -->
            <td class="td-claim-status"><?= ncn_claim_manager_claim_status_style($row['claim_status']); ?></td>

        <?php if ($send_to_archive == true && !is_leaduser($user)): // payment received functions ?>
            <td class="td-payment-function">
                <a href="#" class="pay-full"
                   onclick="archive_claim('<?= $base_url; ?>', <?= $claim_id; ?>, '<?= $row['payment_received']; ?>');return false;">Paid
                    In Full</a><br/>
                <a href="#" class="pay-partial"
                   onclick="partial_payment('<?= $base_url; ?>', <?= $claim_id; ?>);return false;">Partial Payment
                    Received</a>
            </td>
        <?php elseif ($send_to_archive == true): ?>
            <td class="td-payment-function">
                <a href="#" class="pay-full">Paid In Full</a><br/>
                <a href="#" class="pay-partial">Partial Payment Received</a>
            </td>
        <?php endif; ?>
        
        <?php if ($class_name != 'my-active-claims-detail'): ?>
            <td class="td-claim-amount">
                <?php
                if ($row['claim_amount'] > 0) // only show claim amount if > 0
                {
                    $orders_total += $row['claim_amount'];
                    print '$' . number_format($row['claim_amount'], 2);
                }
                ?>
            </td>
        <?php endif; ?>

        <?php if (($send_to_archive == true) || ($editable == false)): // payment received ?>
            <td class="td-payment-received">
                <?php if ($row['payment_received'] > 0) // only show claim amount if > 0
                {
                    $payment_received_total += $row['payment_received'];
                    print '$' . number_format($row['payment_received'], 2);
                }
                ?>
            </td>
            <td class="td-file-notes">
                <?php if (!is_leaduser($user)): ?>
                    <a class="file-notes" href="#" onclick="set_file_note('<?= $base_url; ?>', <?= $claim_id; ?>);return false;">Set File Notes</a>
                <?php else: ?>
                    <a class="file-notes" href="#">Set File Notes</a>
                <?php endif; ?>
            </td>
        <?php endif; ?>

            <td class="td-inbox">    <!-- Inbox -->
            <?php
            // get number of read messages in thread
            // $query2 = "SELECT * FROM pm_message WHERE `claim_id`=" . $claim_id . " AND `read`=1 AND `to`=" . $user->uid . ";";
            $result2 = db_query('SELECT * FROM {pm_message} WHERE `claim_id`=:a AND `read`=1 AND `to`=:b',array(':a'=>$claim_id,':b'=>$user->uid));
            $read = $result2->rowCount();

            if ($read > 0) {
                $class = "message-icon-link-read";
            }

            // get number of read messages in thread
            // $query2 = "SELECT * FROM pm_message WHERE `claim_id`=" . $claim_id . " AND `read`=0 AND `to`=" . $user->uid . ";";
            $result2 = db_query('SELECT * FROM {pm_message} WHERE `claim_id`=:a AND `read`=0 AND `to`=:b',
                array(':a'=>$claim_id,':b'=>$user->uid));
            $unread = $result2->rowCount();

            if ($unread > 0) {
                $class = "message-icon-link-unread";
            }

            // display the inbox button
            if (($unread > 0) || ($read > 0)): ?>
                <a href="<?= $base_url; ?>/account/message-center/viewthread/<?= $claim_id; ?>" class="<?= $class; ?>"></a>
            <?php endif; ?>
            </td>

        <?php if ($class_name != 'my-active-claims-detail'): ?>
            <td class="td-invoice">
                <?php // only show invoice download if an invoice exists
                    if ($live_claim_count > 0): ?>
                    <a href="<?= $base_url ?>/account/serve_invoice_file/<?= $claim_id; ?>/1" class="pdf-icon-link-32"></a>
                <?php endif; ?>
            </td>
        <?php endif; ?>

        <?php
        if ($approve_deny == true && $row['claim_status'] != 'returned') {
            print '<td class="td-final-approval">';
            if ($live_claim_count > 0) {
                if ($claim_locked == true) {
                    echo "<a href='#' class='purchase-btn' onclick=\"window.location = '".$base_url."/account/purchase_invoice/$claim_id';\">Purchase</a>";
                } else {
                    echo "<a href='#' class='approve-btn' onclick=\"window.location = '".$base_url."/account/approve_invoice/$claim_id';\">Approve</a>";
                }
                ?>
                <?php $_change_order_request_url = $base_url . "/account/ncn_change_order_request/" . $claim_id; ?>
                <a href="<?php echo $_change_order_request_url; ?>" class="reject-btn">Change Order</a>
                <!-- <a href="#" class="reject-btn" onclick="if (confirm('Please confirm you wish to change this invoice and notify the administrator.')) { window.location = '/account/reject_invoice/<?= $claim_id; ?>'; }">Reject</a> -->
            <?php
            }
            print "</td>";
        }
        ?>

        <?php if ($row['claim_status'] == "incomplete" && !is_leaduser($user) /*&& $live_invoice_count > 0*/): ?>
            <td class="td-create-my-invoice">
                <!--<a href="#" class="create-invoice-btn enabled" onclick="open_submit_box(<?= $claim_id; ?>);return false;">Create My Invoice</a>-->
                <a class="create-invoice-btn enabled colorbox-node" href="<?php echo $base_url; ?>/account/confirm_submit_claim/<?= $claim_id; ?>?width=700&height=540">Create My Invoice</a>
            </td>
        <?php elseif ($send_to_admin == true): ?>
            <td class="td-create-my-invoice">
                <a href="#" class="create-invoice-btn disabled">Create My Invoice</a>
            </td>
        <?php endif; ?>

        <?php if ($send_to_admin == true && !is_leaduser($user)): ?>
            <td class="td-edit-additional-data">
                <a href="<?php echo base_path() . "account/additional_claim_info/$claim_id"; ?>"
                   class="edit-additional-data-btn">Edit</a>
            </td>
        <?php elseif ($send_to_admin == true): ?>
            <td class="td-edit-additional-data">
                <a href="#" class="edit-additional-data-btn">Edit</a>
            </td>
        <?php endif; ?>
        </tr>

        <tr class="hidden-row">
            <td colspan="2" style="padding:0;"></td>
            <td colspan="<?php echo($colcount - 2); ?>" style="padding:0;">
                <div class="basic_info" id="basic_<?= $claim_id; ?>">
                    <table cellpadding="0" cellspacing="0" style="padding:0;margin:0;">
                        <tr>
                            <td width="150px"><span
                                    style="font-weight: bold;">Date of Loss:&nbsp;</span><?= ncn_cd($claim_id, 'date_of_loss'); ?>
                            </td>
                            <td width="150px"><span
                                    style="font-weight: bold;">Address:&nbsp;</span><?= ncn_cd($claim_id, 'insured_address'); ?>
                            </td>
                            <td width="100px"><span
                                    style="font-weight: bold;">State:&nbsp;</span><?= ncn_cd($claim_id, 'insured_state'); ?>
                            </td>
                            <td width="40px" rowspan="2" style="text-align:right;"><span style="font-weight: bold;">Notes:&nbsp;</span>
                            </td>
                            <td rowspan="2">
                                <?php
                                if (ncn_admin_note_check_mode($claim_id) == 'new') {
                                    print ncn_claims_manager_note_render_claim_notes_table($claim_id, $GLOBALS['user']->uid);
                                } else {
                                    $_file_note = str_replace("<br/>", "\n", base64_decode($row['file_note']));
                                    echo nl2br(htmlspecialchars($_file_note));
                                } ?>
                            </td>
                        </tr>
                        <tr>
                            <td><span
                                    style="font-weight: bold;">Insured's Name:&nbsp;</span><?= ncn_cd($claim_id, 'customer_name'); ?>
                            </td>
                            <td><span
                                    style="font-weight: bold;">City:&nbsp;</span><?= ncn_cd($claim_id, 'insured_city'); ?>
                            </td>
                            <td><span
                                    style="font-weight: bold;">Zip Code:&nbsp;</span><?= ncn_cd($claim_id, 'insured_zip'); ?>
                            </td>
                        </tr>
                    </table>
                </div>
            </td>
        </tr>

        <?php
            $min_row = 9; //insert empty row
            if ($i == $row_count - 1) {
                for ($index = 0; $index < ($min_row - $row_count); $index++) {
                    print ('<tr>');
                    print ("    <td colspan=\"$colcount\">&nbsp</td>");
                    print ('</tr>');
                }
            } 
        ?>
        <?php if ($i == $row_count - 1 && $orders_total > 0) {
            $colspan = 0;
            $colskip = 0;
            if ($class_name == 'out-for-review-detail') {
                $colspan = 5;
                $colskip = 1;
            } elseif ($class_name == 'archived-claims-detail') {
                $colspan = 4;
                $colskip = 2;
            } elseif ($class_name == 'recievables-detail') {
                $colspan = 5;
                $colskip = 2;
            }
            ?>
            <tr class="header">
                <td>Total:</td>
                <td colspan="<?php echo $colspan; ?>"></td>
                <td><?= '$' . number_format($orders_total, 2); ?></td>

                <?php
                if (($send_to_archive == true) || ($editable == false)) {
                    ?>
                    <td><?= '$' . number_format($payment_received_total, 2); ?></td>
                <?php
                }
                ?>

                <td colspan="<?php echo($colcount - $colspan - $colskip - 1); ?>"></td>
            </tr>
        <?php
        }
        $i++;
    } // end foreach
    ?>

    <?php
    $min_row = 9; // insert empty row
    if ($row_count == 0) {
        for ($index = 0; $index < ($min_row - $row_count); $index++) {
            print ('<tr>');
            print ("    <td colspan=\"$colcount\">&nbsp</td>");
            print ('</tr>');
        }
    }
    ?>

    </table>
    </div>
    </div>
<?php
}

