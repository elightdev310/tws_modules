<?php
/**
 * Render Available Claims Page ( ce_portal/available_claims )
 */
function ncn_ceportal_available_claims_page() {
ncn_ceportal_check_expired_all_timer_trigger();
drupal_add_css(drupal_get_path('module', 'ncn_message').'/ncn_message.css', 'module', 'all', FALSE);

global $user;
global $base_url;

    $ava_claim_filter = isset($_REQUEST['filter'])? $_REQUEST['filter']:'';
    $_url = $base_url.base_path().'ce_portal/available_claims?'. (($ava_claim_filter!='')? "filter=$ava_claim_filter&":"");
    
    $page_records = 10;
    $page_number = 0;
    if (isset($_REQUEST['page'])) {
        $page_number = intval($_REQUEST['page']);
    }
    
    $from = $page_number * $page_records;
    $params = array(
        'pager' => array('from'=>$from, 'count'=>$page_records),
        'filter'=> $ava_claim_filter
    );
    $total_count = ncn_ce_portal_get_ava_claims_total_count($user->uid, $params);
    $ava_claims = ncn_ceportal_get_available_claims_list($user->uid, $params);
    
    ob_start();
?>
<div id="page_results">
    <div id="ce_available_claims_page" class="claims-detail"> 
        <div class="claims-section-wrapper">
            <div class="clearfix">
                <div class="title">Available Claims</div>
                <div id="ava-claim-filter">Filtered by Assigned Status&nbsp;&nbsp;<?php print ncn_ceportal_render_ava_claim_filter('ce_ava_claim_filter', 'ce_ava_claim_filter', $ava_claim_filter); ?></div>
                <!-- <div class="help"><a href="#">Help</a></div> -->
            </div>
            <div class="ncn-pager-ranger"><?php print ncn_ceportal_render_page_ranger($page_number, $page_records, $total_count); ?></div>
            <div id="ce_available_claims_table_wrapper" class="table clearfix">
                <?php print ncn_ceportal_render_ava_claims_table($ava_claims, $user->uid, $page_records); ?>
            </div>
            <?php print ncn_ceportal_render_pager_links($page_number, $page_records, $total_count, $_url); ?>
        </div>
    </div>
</div>
<?php
    $content = ob_get_contents();
    ob_end_clean();
    
    return $content;
}

function ncn_ceportal_render_ava_claims_table($ava_claims, $ce_uid, $max_row=10) {
global $base_url;
    $as_data = ncn_ceportal_get_assigned_status_data();
    ob_start();
    $ce_auto_assign = variable_get('ce_auto_assign', FALSE);
?>
    <div class="table-header">
    <table><tbody>
        <tr>
            <td class="td-id">ID</td>
            <td class="td-assigned">Assigned Status</td>
            <td class="td-accept">Accept Claim</td>
        </tr>
    </tbody></table>
    </div>
    
    <div class="table-body">
    <table><tbody>
        <?php for ($index=0; $index<count($ava_claims); $index++): ?>
        <?php $ce_claim = $ava_claims[$index]; ?>
        <tr>
            <td class="td-id">
                <?php 
                    $time_2nd="";
                    if ($ce_auto_assign && $ce_claim['assigned']==0 && ncn_ceportal_is_available_when_auto_assign($ce_claim['claim_id'], $ce_uid)==='special-auto-assign') {
                        $time_2nd = "<span class='ce-time-2nd-claim'> [Special For You in 1 Hour] </span>";
                    } else if (ncn_ceportal_get_time_assigned($ce_claim['claim_id'], $ce_uid, 'missed') > 0) {
                        $time_2nd = "<span class='ce-time-2nd-claim'> * </span>";
                    }
                ?>
                <?php echo $ce_claim['claim_id'].$time_2nd; ?>
            </td>
            <td class="td-assigned">
                <?php 
                    $assigned_status = $as_data[$ce_claim['assigned']];
                ?>
                <?php echo "<span class='".$assigned_status['value']."'>".$assigned_status['text']."</span>"; ?>
            </td>
            <td class="td-accept">
                <div class="clearfix">
                    <?php print ncn_ceportal_render_accept_button($ce_claim, $ce_uid); ?>
                </div>
            </td>
        </tr>
        <?php endfor; ?>
        <?php for (; $index<$max_row; $index++): ?>
        <tr><td colspan="3">&nbsp;</td></tr>        
        <?php endfor; ?>
    </tbody></table>
    </div>
<?php
    $output = ob_get_contents();
    ob_end_clean();
    
    return $output;
}

function ncn_ceportal_render_accept_button($ce_claim, $ce_uid) {
global $base_url;

    $b_anchor = true;
    $ce_disabled = '';
    if ($ce_claim['assigned']!=0) {
        if ($ce_claim['ce_uid'] == $ce_uid) {
            $span_text = 'Me';
            $span_class = 'me';
            $b_anchor = false;          
        } else {
            $b_anchor = false;
            $span_text = '';
            $span_class = 'other';
        }
    } else {
        $ce_disabled_status = ncn_ceportal_is_disabled_to_accept_claim($ce_claim['claim_id'], $ce_uid);
        if ($ce_disabled_status === FALSE) {
            $b_anchor = true;
        } else if ($ce_disabled_status === 'accepted') {
            $span_class = 'accepted';
            $span_text = 'Accepted';
            $b_anchor = false;
        } else if ($ce_disabled_status === 'assigned') {
            $span_class .= 'me';
            $span_text = 'Me';
            $b_anchor = false;
        } else if ($ce_disabled_status === 'missed') {
            $span_class = 'missed';
            $span_text = 'Missed';
            $b_anchor = false;
        } else if ($ce_disabled_status === 'special-auto-assign') {
            $span_class = 'special-auto-assign';
            $span_text = 'Special';
            $b_anchor = false;
        } else {    // overload
            $ce_disabled_status = 'overloaded';
            $b_anchor = true;
            $ce_disabled = 'disabled';
        }
    }
    
    if ($b_anchor) {
        if ($ce_disabled == 'disabled') {
            $_url_accept = '#';
        } else {
            $_url_accept = $base_url.t('/ce_portal/ava_claim/!ce_claim_id/accept', array('!ce_claim_id'=>$ce_claim['claim_id'])); 
            $_url_dest = urlencode(request_uri());
            $_url_accept.= "?".htmlentities("destination=".$_url_dest);
        }
        $output = "<a href='$_url_accept' class='ce-accept-btn link-btn $ce_disabled' $ce_disabled >Accept</a>";
    } else {
        $output = "<span class='$span_class span-disabled'>$span_text</span>";
    }
    
    return $output;
}

function ncn_ceportal_render_ava_claim_filter($id, $name, $val, $attr='') {
global $base_url;
    if($val==='') {
        $val = '101';
    }
    
    $data = array(
        '101'=>t('Show All'),
        '0'  =>t('Not Assigned'),
        '1'  =>t('Assigned'),
        '2'  =>t('Re-Assigned'),
    );
    $output = "<select id='$id' name='$name' $attr>";
    foreach ($data as $k=>$item) {
        $selected = '';
        if ($k == $val) { $selected='selected'; }
        $output.="<option value='$k' $selected>$item</option>";
    }
    $output.= "</select>";
    
    $_url = $base_url.base_path().'ce_portal/available_claims';
    drupal_add_js("
        jQuery(document).ready(function() {
            jQuery('#$id').bind('change', function() {
                var new_val = jQuery(this).find('option:selected').val();
                if (new_val == '101') {
                    location.href = '$_url';
                } else {
                    location.href = '$_url?filter='+new_val;
                }
            });
        });
    ", 'inline'
    );
    return $output;
}






/**
 * My Active Claims Page (ce_portal/active_claims)
 */
function ncn_ceportal_active_claims_page() {
ncn_ceportal_check_expired_all_timer_trigger();
drupal_add_css(drupal_get_path('module', 'ncn_message').'/ncn_message.css', 'module', 'all', FALSE);

global $user;
global $base_url;
    
    $_url = $base_url.base_path().'ce_portal/active_claims?';
    
    $page_records = 10;
    $page_number = 0;
    if (isset($_REQUEST['page'])) {
        $page_number = intval($_REQUEST['page']);
    }
    
    $from = $page_number * $page_records;
    $params = array(
        'pager' => array('from'=>$from, 'count'=>$page_records),
    );
    $total_count = ncn_ceportal_get_active_claims_total_count($user->uid, $params);
    $active_claims = ncn_ceportal_get_active_claims_list($user->uid, $params);
    
    ob_start();
?>
<div id="page_results">
    <div id="ce_active_claims_page" class="claims-detail"> 
        <div class="claims-section-wrapper">
            <div class="clearfix">
                <div class="title">My Active Claims</div>
                <!-- <div class="help"><a href="#">Help</a></div> -->
                <div class="ncn-pager-ranger"><?php print ncn_ceportal_render_page_ranger($page_number, $page_records, $total_count); ?></div>
            </div>
            <div id="ce_active_claims_table_wrapper" class="table clearfix">
                <?php print ncn_ceportal_render_active_claims_table($active_claims, $user->uid, $page_records); ?>
            </div>
            <?php print ncn_ceportal_render_pager_links($page_number, $page_records, $total_count, $_url); ?>
        </div>
    </div>
</div>
<?php
    $content = ob_get_contents();
    ob_end_clean();
    
    return $content;
}

function ncn_ceportal_render_active_claims_table($active_claims, $ce_uid, $max_row=10) {
global $base_url;
    ob_start();
?>
    <div class="table-header">
    <table><tbody>
        <tr>
            <td class="td-id">ID</td>
            <td class="td-ce-claim-file">CE Claim File</td>
            <td class="td-ce-claim-zip-code">Zip Code</td>
            <td class="td-esx-file">ESX File</td>
            <td class="td-invoice-file">Invoice PDF File</td>
            <td class="td-review">Claim Info</td>
            <td class="td-inbox">Inbox</td>
            <td class="td-remaining">Time Remaining</td>
            <td class="td-completed">Invoice Completed</td>
        </tr>
    </tbody></table>
    </div>
    
    <div class="table-body">
    <table><tbody>
        <?php for ($index=0; $index<count($active_claims); $index++): ?>
        <?php 
            $ce_claim = $active_claims[$index]; 
            $ce_claim_trigger = ncn_ceportal_get_active_ce_claim_trigger($ce_claim['claim_id'], $ce_uid);
            
            $ce_ct_remained = $ce_claim_trigger['period']-(date('U')-intval($ce_claim_trigger['ts_start']));
            
            $cect_remained_class = "ce-remained-time";
            if ($ce_ct_remained <= 2*3600) { $cect_remained_class.=" half-pass"; }
            $sec = $ce_ct_remained % 60;    $ce_ct_remained = $ce_ct_remained / 60;
            $min = $ce_ct_remained % 60;  $hour = $ce_ct_remained / 60;
            
            $ce_ct_status = "<span class='$cect_remained_class'>".sprintf('%02d:%02d:%02d', $hour, $min, $sec)."</span>";
            
            $_url_complete = $base_url.base_path()."ce_portal/ava_claim/".$ce_claim['claim_id']."/complete";
            $_url_dest = urlencode("ce_portal/archive_claims");
            $_url_complete.= "?".htmlentities("destination=".$_url_dest);
            
            if (ncn_ceportal_validate_complete_active_claim($ce_claim['claim_id'])) {
                $complete_disabled = '';
            } else {
              $complete_disabled = 'disabled';
                $_url_complete = '#';
            }
        ?>
        <?php 
            $time_2nd="";
            if (ncn_ceportal_get_time_assigned($ce_claim['claim_id'], $ce_uid, 'missed') > 0) {
                $time_2nd = "<span class='ce-time-2nd-claim'> * </span>";
            }
        ?>
                
        <tr>
            <td class="td-id"><?php echo $ce_claim['claim_id'].$time_2nd; ?></td>
            <td class="td-ce-claim-file"><?php print ncn_ceportal_render_upload_item('ceportal_upload_ce_claim_file_'.$ce_claim['claim_id'], 'ce_claim_file_'.$ce_claim['claim_id'], $ce_claim['claim_id'], 10, 'ce-claim-file', false); ?></td>
            <td class="td-ce-claim-zip-code"><?php print ncn_cd($ce_claim['claim_id'], 'insured_zip'); ?></td>
            <td class="td-esx-file"><?php print ncn_ceportal_render_upload_item('ceportal_upload_ce_esx_file_'.$ce_claim['claim_id'], 'ce_claim_file_'.$ce_claim['claim_id'], $ce_claim['claim_id'], 11, 'ce-esx-file'); ?></td>
            <td class="td-invoice-file"><?php print ncn_ceportal_render_upload_item('ceportal_upload_ce_invoice_file_'.$ce_claim['claim_id'], 'ce_claim_file_'.$ce_claim['claim_id'], $ce_claim['claim_id'], 12, 'ce-invoice-file'); ?></td>
            <td class="td-review"><?php if (ncn_ceportal_check_claim_reviewable($ce_claim['claim_id'])) { print l('View', "ce_portal/review/claim/".$ce_claim['claim_id']."/additional_data", array('attributes'=>array('class'=>'ce-view-btn link-btn'))); } ?></td>
            <td class="td-inbox"><?php print ncn_ceportal_render_inbox($ce_uid, $ce_claim['claim_id']); ?></td>
            <td class="td-remaining"><?php echo $ce_ct_status; ?></td>
            <td class="td-completed"><a href="<?php echo $_url_complete; ?>" class="ce-completed-btn link-btn <?php echo $complete_disabled; ?>">Completed</a></td>
        </tr>
        <?php endfor; ?>
        <?php for (; $index<$max_row; $index++): ?>
        <tr><td colspan="9">&nbsp;</td></tr>        
        <?php endfor; ?>
    </tbody></table>
    </div>
<?php
    $output = ob_get_contents();
    ob_end_clean();
    
    return $output;
}

function ncn_ceportal_render_inbox($user_id, $claim_id) {
global $base_url;

    // get number of read messages in thread
    $read = db_query("SELECT * FROM {pm_message} WHERE claim_id=:claim_id AND `read`=1 AND `to`=:to",
        array(":claim_id"=>$claim_id,":to"=>$user_id));
    $read = $read->rowCount();
    if ($read > 0)
    {   $class = "message-icon-link-read";  }
    
    // get number of read messages in thread
    $unread = db_query("SELECT * FROM {pm_message} WHERE claim_id=:claim_id AND `read`=0 AND `to`=:to",
        array(":claim_id"=>$claim_id,":to"=>$user_id));
    $unread = $unread->rowCount();
    if ($unread > 0)
    {   $class = "message-icon-link-unread";    }
    


    // display the inbox button             
    if (($unread > 0) || ($read > 0))
    {
        $output = '<a href="'.$base_url.'/account/message-center/viewthread/'.$claim_id.'" class="'.$class.'"></a>';
    }
    
    return $output;
}

function ncn_ceportal_render_upload_item($id, $name, $claim_id, $claim_file_type, $class='', $uploadable=true) {
global $base_url;

    $result = db_query("SELECT * FROM {claims_invoices} WHERE claim_id=:claim_id AND `live`=:live ORDER BY revision DESC",
        array(":claim_id"=>$claim_id,":live"=>$claim_file_type));
    $output = '';
    $b_reload = false;
    if ($result->rowCount()>0) {
        $b_reload = true;
        $donwload_class = $class." downloadable";
        $ci = $result->fetchAssoc();
        $_url_to_file = $base_url.base_path()."account/serve_invoice_file/$claim_id/$claim_file_type/0/".$ci['invoice_id'];
        $output = "<a href='$_url_to_file' class='$donwload_class ce-file-download link-btn'>Download</a>";
    } else {
        $donwload_class = $class;
    }

    
    if ($uploadable) {
        if ($b_reload) {
            $upload_class = $class." re-upload";
        } else {
            $upload_class = $class;
        }
        $output .= "<div href='#' id='$id' name='files[$name]' class='$upload_class ce-file-upload link-btn'>Upload</div>";
        drupal_add_js("jQuery(document).ready( function(){
            ncn_ceportal_bind_ajax_upload('#$id', 'files[$name]', '$name', $claim_id, $claim_file_type);
        }); ", 'inline'
        );
    }
    return $output;
}

/**
 * Render Archive Claims Page   ( ce_portal/archive_claims )
 */
function ncn_ceportal_archive_claims_page() {
//ncn_ceportal_check_expired_all_timer_trigger();
drupal_add_css(drupal_get_path('module', 'ncn_message').'/ncn_message.css', 'module', 'all', FALSE);

global $user;
global $base_url;

    $_url = $base_url.base_path().'ce_portal/archive_claims?';
    
    $page_records = 10;
    $page_number = 0;
    if (isset($_REQUEST['page'])) {
        $page_number = intval($_REQUEST['page']);
    }
    
    $from = $page_number * $page_records;
    $params = array(
        'pager' => array('from'=>$from, 'count'=>$page_records),
    );
    $total_count = ncn_ceportal_get_archive_claims_total_count($user->uid, $params);
    $archive_claims = ncn_ceportal_get_archive_claims_list($user->uid, $params);
    
    ob_start();
?>
<div id="page_results">
    <div id="ce_archive_claims_page" class="claims-detail"> 
        <div class="claims-section-wrapper">
            <div class="clearfix">
                <div class="title">Archive Claims</div>
                <!-- <div class="help"><a href="#">Help</a></div> -->
                <div class="ncn-pager-ranger"><?php print ncn_ceportal_render_page_ranger($page_number, $page_records, $total_count); ?></div>
            </div>
            <div id="ce_archive_claims_table_wrapper" class="table clearfix">
                <?php print ncn_ceportal_render_archive_claims_table($archive_claims, $user->uid, $page_records); ?>
            </div>
            <?php print ncn_ceportal_render_pager_links($page_number, $page_records, $total_count, $_url); ?>
        </div>
    </div>
</div>
<?php
    $content = ob_get_contents();
    ob_end_clean();
    
    return $content;
}

function ncn_ceportal_render_archive_claims_table($archive_claims, $ce_uid, $max_row=10) {
global $base_url;
    ob_start();
?>
    <div class="table-header">
    <table><tbody>
        <tr>
            <td class="td-id">ID</td>
            <td class="td-ce-claim-file">CE Claim File</td>
            <td class="td-esx-file">ESX File</td>
            <td class="td-invoice-file">Invoice PDF File</td>
            <td class="td-inbox">Inbox</td>
        </tr>
    </tbody></table>
    </div>
    
    <div class="table-body">
    <table><tbody>
        <?php for ($index=0; $index<count($archive_claims); $index++): ?>
        <?php 
            $ce_claim = $archive_claims[$index]; 
        ?>
        <tr>
            <td class="td-id"><?php echo $ce_claim['claim_id']; ?></td>
            <td class="td-ce-claim-file"><?php print ncn_ceportal_render_upload_item('ceportal_upload_ce_claim_file_'.$ce_claim['claim_id'], 'ce_claim_file_'.$ce_claim['claim_id'], $ce_claim['claim_id'], 10, 'ce-claim-file', false); ?></td>
            <td class="td-esx-file"><?php print ncn_ceportal_render_upload_item('ceportal_upload_ce_esx_file_'.$ce_claim['claim_id'], 'ce_claim_file_'.$ce_claim['claim_id'], $ce_claim['claim_id'], 11, 'ce-esx-file', false); ?></td>
            <td class="td-invoice-file"><?php print ncn_ceportal_render_upload_item('ceportal_upload_ce_invoice_file_'.$ce_claim['claim_id'], 'ce_claim_file_'.$ce_claim['claim_id'], $ce_claim['claim_id'], 12, 'ce-invoice-file', false); ?></td>
            <td class="td-inbox"><?php print ncn_ceportal_render_inbox($ce_uid, $ce_claim['claim_id']); ?></td>
        </tr>
        <?php endfor; ?>
        <?php for (; $index<$max_row; $index++): ?>
        <tr><td colspan="5">&nbsp;</td></tr>        
        <?php endfor; ?>
    </tbody></table>
    </div>
<?php
    $output = ob_get_contents();
    ob_end_clean();
    
    return $output;
}

/**
 * Sales Report ( ce_portal/sales_report )
 */
function ncn_ceportal_sales_report_page() {
drupal_add_css(drupal_get_path('module', 'ncn_message').'/ncn_message.css', 'module', 'all', FALSE);

global $user;
global $base_url;

    $url = $base_url.base_path()."ce_portal/sales_report";
    
    $report_date = 0;
    if ( isset($_REQUEST['report_ts']) ) {
        $report_date = intval($_REQUEST['report_ts']);
    } else {
        $report_date = date('U');
    }
        
    $next_month = strtotime("+1 month", $report_date);
    $last_month = strtotime("-1 month", $report_date);
    
    if (isset($_REQUEST['rsd'])) {
        $report_date = strtotime($_REQUEST['rsd']);
        $timezone = variable_get('date_default_timezone', 0);
        $report_date -= $timezone;
        drupal_goto($url."?report_ts=$report_date");
        exit;
    }
    
    $_url = $base_url.base_path().'ce_portal/sales_report?'. (($report_date!=0)? "report_ts=$report_date&":"");
    
    $page_records = 10;
    $page_number = 0;
    if (isset($_REQUEST['page'])) {
        $page_number = intval($_REQUEST['page']);
    }
    
    $from = $page_number * $page_records;
    $params = array(
        'pager' => array('from'=>$from, 'count'=>$page_records),
        'report_date' => $report_date, 
    );
    $total_count = ncn_ceportal_get_sales_report_total_count($user->uid, $params);
    $sales = ncn_ceportal_get_sales_report_list($user->uid, $params);
    $total_earning = ncn_ceportal_get_total_monthly_earnings($user->uid, $report_date);
    ob_start();
?>
<div id="page_results">
    <div id="ce_sales_report_page" class="claims-detail"> 
        <div class="claims-section-wrapper">
            <div class="clearfix">
                <div class="title">Sales Report</div>
                <div class="report-date">
                    <span><a href="<?php echo $url; ?>">[This Month]</a></span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                    <span><a href="<?php echo $url."?report_ts=$last_month"; ?>" title="<?php echo date("F Y", $last_month); ?>">[Last]</a></span>&nbsp;&nbsp;
                    <span><input type="text" id="datepicker" value="<?php echo date("F Y", $report_date); ?>" size="15" readonly></span>&nbsp;
                    <span><a href="<?php echo $url."?report_ts=$next_month"; ?>" title="<?php echo date("F Y", $next_month); ?>">[Next]</a></span>&nbsp;&nbsp;
                    <input type="hidden" id="report_url" value="<?php echo $url; ?>" />
                    <?php echo ncn_report_insert_date_script(); ?>
                </div>
                <div class="total-earning">
                    <span class="total-earning-title">Total Monthly Earnings:</span> <span class="total-amount"><?php echo '$'.number_format(floatval($total_earning), 2); ?></span>
                </div>
                <!-- <div class="help"><a href="#">Help</a></div> -->
            </div>
            <div class="ncn-pager-ranger"><?php print ncn_ceportal_render_page_ranger($page_number, $page_records, $total_count); ?></div>
            <div id="ce_sales_report_table_wrapper" class="table clearfix">
                <?php print ncn_ceportal_render_sale_report_table($sales, $user->uid, $page_records); ?>
            </div>
            <?php print ncn_ceportal_render_pager_links($page_number, $page_records, $total_count, $_url); ?>
        </div>
    </div>
</div>
<?php
    $content = ob_get_contents();
    ob_end_clean();
    
    return $content;
}


function ncn_ceportal_render_sale_report_table($sales, $ce_uid, $max_row=10) {
global $base_url;
    $as_data = ncn_ceportal_get_assigned_status_data();
    ob_start();
?>
    <div class="table-header">
    <table><tbody>
        <tr>
            <td class="td-id">ID</td>
            <td class="td-assigned-ts">Date & Time Assigned</td>
            <td class="td-completed-ts">Date & Time Completed</td>
            <td class="td-amount">Total Earned</td>
        </tr>
    </tbody></table>
    </div>
    
    <div class="table-body">
    <table><tbody>
        <?php for ($index=0; $index<count($sales); $index++): ?>
        <tr>
            <?php 
                $sale_info = $sales[$index];
                $trigger = ncn_ceportal_get_completed_ce_claim_trigger_from_id($sale_info['ce_claim_trigger_id']);
            ?>
            <td class="td-id"><?php echo $trigger['claim_id']; ?></td>
            <td class="td-assigned-ts"><?php echo date('F j, Y H:i:s', $trigger['ts_start']); ?></td>
            <td class="td-completed-ts"><?php echo date('F j, Y H:i:s', $trigger['ts_end']); ?></td>
            <td class="td-amount"><?php echo '$'.number_format(floatval($sale_info['amount']), 2); ?></td>
        </tr>
        <?php endfor; ?>
        <?php for (; $index<$max_row; $index++): ?>
        <tr><td colspan="4">&nbsp;</td></tr>        
        <?php endfor; ?>
    </tbody></table>
    </div>
<?php
    $output = ob_get_contents();
    ob_end_clean();
    
    return $output;
}



function ncn_ceportal_dashboard_page() {
    $output = '<div id="page_loading"> </div>
    <div id="page_results" class="clearfix">';
    $output.=ncn_ceportal_homepage_innerpage();
    $output.='</div>';
    
    return $output;
}

function ncn_ceportal_homepage_innerpage() {
ncn_ceportal_check_expired_all_timer_trigger();
    ob_start();
?>
    <div id="ceportal_homepage" class="page-content clearfix">

    <div>
    <div class="add-claim-section clearfix"><?php print _ncn_ceportal_homepage_alert(); ?></div>
    <div class="claim-list clearfix">
        <div id="ce-available-claims-homewrapper"><?php print _ncn_ceportal_homepage_available_claims(4); ?></div>
        <div id="ce-my-active-claims-homewrapper"><?php print _ncn_ceportal_homepage_my_active_claims(3); ?></div>
    </div>
    </div>
    </div>
<?php
    $output = ob_get_contents();
    ob_end_clean();
    
    return $output;
}

function _ncn_ceportal_homepage_alert() {
    ob_start();
?>
    <div class="ahome-section-wrapper">
        <div>
            <div id="message-title">Messages and Alerts</div>
            <div id="message-body">
                <h2><?= nl2br(variable_get('ncn_msg_alert_title', '')); ?></h2>
                <p><?= nl2br(variable_get('ce_dashboard_message', '')); ?></p>
            </div>
        </div>
        <div>
        </div>
    </div>
<?php
    $content = ob_get_contents();
    ob_end_clean();
    return $content;
}

function _ncn_ceportal_homepage_available_claims($min_row) {
global $user;
    $params = array(
        'pager' => array('from'=>0, 'count'=>$min_row),
    );
    $ava_claims = ncn_ceportal_get_available_claims_list($user->uid, $params);
    
    ob_start();
?>
    <div id="ce_available_claims_homepage" class="claims-detail"> 
        <div class="ahome-section-wrapper">
        <div class="claims-section-wrapper">
            <div class="clearfix">
                <div class="title">Available Claims</div>
                <!-- <div class="help"><a href="#">Help</a></div> -->
            </div>
            <div id="ce_available_claims_table_wrapper" class="table clearfix">
                <?php print ncn_ceportal_render_ava_claims_table($ava_claims, $user->uid, $min_row); ?>
            </div>
            <div class="view-details clearfix">
                <a href="<?php $base_url?>/ce_portal/available_claims">View Details</a>
            </div>
        </div>
        </div>
    </div>
<?php
    $content = ob_get_contents();
    ob_end_clean();
    
    return $content;
}

function _ncn_ceportal_homepage_my_active_claims($min_row) {
    global $user;
    $params = array(
        'pager' => array('from'=>0, 'count'=>$min_row),
    );
    $active_claims = ncn_ceportal_get_active_claims_list($user->uid, $params);
    
    ob_start();
?>
    <div id="ce_active_claims_homepage" class="claims-detail"> 
    <div class="ahome-section-wrapper">
        <div class="claims-section-wrapper">
            <div class="clearfix">
                <div class="title">My Active Claims</div>
                <!-- <div class="help"><a href="#">Help</a></div> -->
            </div>
            <div id="ce_active_claims_table_wrapper" class="table clearfix">
                <?php print ncn_ceportal_render_active_claims_table($active_claims, $user->uid, $min_row); ?>
            </div>
            <div class="view-details clearfix">
                <a href="<?php $base_url?>/ce_portal/active_claims">View Details</a>
            </div>
        </div>
    </div>
    </div>
<?php
    $content = ob_get_contents();
    ob_end_clean();
    
    return $content;
}



function ncn_ceportal_render_page_ranger($page_number, $page_records, $total_count) {
    if ($page_number*$page_records>=$total_count) {
        $page_number = ceil((double)$total_count/$page_records) - 1;
    }
    if ($page_number < 0) {
        $page_number = 0;
    }
    
    $from = $page_number*$page_records+1;
    $to =  ($page_number+1)*$page_records ;
    if ($to>$total_count) { $to = $total_count; }
    $str_ranger = $from . " - " . $to . " of " . $total_count;

    if ($total_count == 0) {
        $str_ranger = '';
    }
    return $str_ranger;
}

function ncn_ceportal_render_pager_links($page_number, $page_records, $total_count, $_url) {
    $output = '';
    if( $total_count > $page_records ) {
        $first_url = $_url.'page=0';
        $prev_url = $_url.'page='.($page_number-1);
        $next_url = $_url.'page='.($page_number+1);
        $last_url = $_url.'page='.(int)($total_count/$page_records);
        if ($page_number == 0) {
            $first_url = $prev_url = '#';
        } else if(($page_number+1)*$page_records>=$total_count) {
            $last_url = $next_url = '#';
        }
        
        ob_start();
?>
        <div class="ncn-pager-control clearfix">    <!-- pager_control -->
            <div style="float: right">
            <a class="first <?php if ($first_url=='#') { echo 'disabled'; } ?>" href="<?php echo $first_url; ?>" >first</a>
            <a class="prev <?php if ($prev_url=='#') { echo 'disabled'; } ?>" href="<?php echo $prev_url; ?>" >prev</a>
            <a class="next <?php if ($next_url=='#') { echo 'disabled'; } ?>" href="<?php echo $next_url; ?>" >next</a>
            <a class="last <?php if ($last_url=='#') { echo 'disabled'; } ?>" href="<?php echo $last_url; ?>" >last</a>
            </div>
        </div>
<?php
        $output = ob_get_contents();
        ob_end_clean();
    }
    return $output;
}
?>
