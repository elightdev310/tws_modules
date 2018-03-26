<?php
// Render Claim Examiners in Claim View Page
// refered in ncn_admin
function ncn_ceportal_claimview_claim_exaimners($claim_id, $pvc_ce_management, $pvc_ce_management_disabled) {
    ncn_ceportal_check_expired_active_claim_timer_trigger($claim_id);
    
    $tfunction_router = isset($_POST['tfunction_router'])? $_POST['tfunction_router']: '';
    if ($tfunction_router == 'ncn_ceportal_claimview_claim_exaimners') {
        $tfunction = isset($_POST['tfunction'])? $_POST['tfunction']: '' ;
        if ($tfunction) {
            ncn_submit_handler_reflection($tfunction, $claim_id);
        }
    }
    
    if ( !($pvc_ce_management&&01) ) { return ''; }
    
    $ce_claim = ncn_ceportal_get_ncn_ce_claim($claim_id, $pvc_ce_management, $pvc_ce_management_disabled);
    
    ob_start();
?>
    <fieldset>
    <legend>Claim Examiners Management</legend>
        <div id="ce_available_claim_setting_wrapper">
        <form id="ce_available_claim_setting_form" method="POST">
            <input type="hidden" name="tfunction_router" value="ncn_ceportal_claimview_claim_exaimners" />
            <input type="hidden" name="tfunction" value="ncn_ceportal_set_available_claim_handler" />
            <label><strong>Available Claim for Claim Examiner: </strong></label>&nbsp;&nbsp;&nbsp;
            <?php print ncn_ceportal_render_select_available_claim_setting('ce_is_available_claim', $ce_claim['is_available'] ); ?>&nbsp;&nbsp;&nbsp;
            <?php if ($pvc_ce_management_disabled !='disabled'): ?>
                <input type="submit" value=" Set " />
            <?php endif; ?>
        </form>
        </div>
        
        <?php print ncn_ceportal_claim_info_reviewable_section($claim_id, $pvc_ce_management, $pvc_ce_management_disabled); ?>
        <?php print ncn_ceportal_assign_ce_section($claim_id, $pvc_ce_management, $pvc_ce_management_disabled); ?>
        
        <div id="ce_history_wrapper">
            <?php print ncn_ceportal_ce_history_section($claim_id, $pvc_ce_management, $pvc_ce_management_disabled); ?>
        </div>
        
    </fieldset>
<?php 
    $output = ob_get_contents();
    ob_end_clean();
    
    return $output;
}


function ncn_ceportal_render_select_available_claim_setting($name, $select_val, $attributes='') {
    $content = "<select name=\"$name\" $attributes>";
    $options = array(0=>'No', 1=>'Yes');
    
    foreach ($options as $key=>$val) {
        $selected = '';
        if ($key == $select_val) {$selected = 'selected'; };
        $content .= '<option value="'.$key.'" '.$selected.' >'.$val.'</option>';
    }
    $content .= "</select>";
    return $content;


}

/**
 * Render Accepted User List ( in backend )
 */
function ncn_ceportal_assign_ce_section($claim_id, $pvc_ce_management, $pvc_ce_management_disabled) {
    if (!ncn_ceportal_is_not_assigned_claim($claim_id)) { return ""; }
    
    $ce_claim = ncn_ceportal_get_ncn_ce_claim($claim_id);
    if (empty($ce_claim)) {
        return "";
    }
    
    $output = "<div id='ce_accepted_claim_examiners_wrapper'>";
    $output.= '<form id="ce_assign_claim_to_ce_form" method="POST">';
    $output.= '<input type="hidden" name="tfunction_router" value="ncn_ceportal_claimview_claim_exaimners" />';
    $output.= '<input type="hidden" name="tfunction" value="ncn_ceportal_choose_ce_handler" />';
    $output.= "<label><strong>Assigned claim to: </strong></label>&nbsp;&nbsp;&nbsp;";
    $output.= ncn_ceportal_render_select_accepted_ce_list('ce_claim_ce_uid', $ce_claim['ce_uid'], '', array('claim_id'=>$claim_id) );
    if ($pvc_ce_management_disabled != 'disabled') {
        $output.="&nbsp;&nbsp;&nbsp;";
        $output.= '<input type="submit" value="Choose" />';
    }
    $output.= '</form>';
    $output.= "</div>";
    
    return $output;
}

function ncn_ceportal_render_select_accepted_ce_list($name, $select_val, $attributes='', $data=array()) {
    $options = ncn_ceportal_get_accepted_ce_list($data['claim_id']);
    
    $content = "<select name=\"$name\" $attributes>";   
    $content .= '<option value=""> - Please Select - </option>';
    foreach ($options as $key=>$_user) {
        $selected = '';
        if ($key == $select_val) {$selected = 'selected'; };
        $content .= '<option value="'.$key.'" '.$selected.' >'.$_user->profile_firstname.' '.$_user->profile_lastname.'</option>';
    }
    $content .= "</select>";
    return $content;
}

function ncn_ceportal_ce_history_section($claim_id, $pvc_ce_management, $pvc_ce_management_disabled) {
    $header = array(
        array('data' => t('Claim Examiner')),
        array('data' => t('Start')),
        array('data' => t('End')), 
        array('data' => t('Status')),
    );
    
    $result = db_query("SELECT * FROM {ncn_ce_claim_trigger} WHERE claim_id=:d",array(":d"=>$claim_id));
    $rows = array();
//  while ($row = db_fetch_array($result))

    foreach($result as $row)
    {
        $row = (array)$row;
        $ce_user = user_load($row['ce_uid']);
        $ce_name = $ce_user->profile_firstname.' '.$ce_user->profile_lastname;
        $ce_ct_start = date('m/d/Y H:i:s', intval($row['ts_start']));
        if ($row['status'] == 0) { // miss
            $ce_ct_end = date('m/d/Y H:i:s', intval($row['ts_end']));
            $ce_ct_status = "Missed";
        } else if ($row['status'] == 1) {
            $ce_ct_end = '';
            $ce_ct_remained = $row['period']-(date('U')-intval($row['ts_start']));

            $cect_remained_class = "ce-remained-time";
            if ($ce_ct_remained <= 2*3600) { $cect_remained_class.=" half-pass"; }
            $sec = $ce_ct_remained % 60;    $ce_ct_remained = (int)($ce_ct_remained / 60);
            $min = $ce_ct_remained % 60;  $hour = (int)($ce_ct_remained / 60);

            $ce_ct_status = "<span class='$cect_remained_class'>".sprintf('%02d:%02d:%02d', $hour, $min, $sec)."</span>";
        } else if ($row['status'] == 2) {
            $ce_ct_end = date('m/d/Y H:i:s', intval($row['ts_end']));
            $ce_ct_status = "Completed";
            if ($pvc_ce_management_disabled != 'disabled') {
                $ce_ct_status .="<br/>";
                $_url = base_path()."ce_portal/re-available-to-accept/".$claim_id;
                $ce_ct_status .='<a href="'.$_url.'" onclick="return confirm(\'Are you sure you want to make this claim re-available?\');">Re-available to accept</a>';
            }
        }

        $rows[] = array('data' => array(
            $ce_name, $ce_ct_start, $ce_ct_end, $ce_ct_status
        ));
    }

    $output = theme('table',array("header"=>$header,"rows"=>$rows, 'attributes'=>array('id' => 'ce_history_table')));
    return $output;
}


function ncn_ceportal_choose_ce_handler($claim_id) {
    $ce_uid = $_REQUEST['ce_claim_ce_uid'];
    if ($ce_uid == '') {
        drupal_set_message(t("Please select one of claim examiners"), 'error');
        return FALSE;
    }
    
    if (ncn_admin_claim_file_exist($claim_id, 10) || get_ncn_data_extra($claim_id, 'ce_reviewable_claim_info')=='reviewable') { // CE Claim File
    } else {
        drupal_set_message(t("Before assigning, CE Claim File need to be uploaded."), 'error');
        return FALSE;
    }
    
    if (!ncn_ceportal_ava_claim($claim_id, 'assign', $ce_uid)) {
        drupal_set_message(t("Failed to assign claim to claim examiner"), 'error');
        return FALSE;
    }
    
    return TRUE;
}

/**
 * Menu Handler (admin/config/ncn_ceportal/settings)
 */
function ncn_ceportal_admin_settings() {
    $output = drupal_get_form('ncn_ceportal_admin_settings_alert');
    return $output;
}


/**
 * "Messages & Alerts" setting form
 */ 
function ncn_ceportal_admin_settings_alert() {
    $form['ce_alert'] = array(
        '#type' => 'fieldset', 
        '#title' => t('Messages & Alerts'), 
        '#weight' => -50, 
        '#collapsible' => TRUE, 
        '#collapsed' => FALSE,
    );
    $form['ce_alert']['ce_dashboard_message'] = array(
        '#type' => 'textarea', 
        '#title' => t('Body'),
        '#default_value' => variable_get('ce_dashboard_message', ''),
        '#required' => 1,
    );
    
    $form['ce_alert']['submit'] = array('#type' => 'submit', '#weight' => 101, '#value' => t('Save'));
    
    return $form;
}

function ncn_ceportal_admin_settings_alert_submit($form, &$form_state) {
    variable_set('ce_dashboard_message', t($form_state['values']['ce_dashboard_message']));
    drupal_set_message("Messages & Alerts saved, successfully");
}

/**
 * Menu Handler (admin/config/ncn_ceportal/mail_settings)
 */
function ncn_ceportal_admin_mail_settings() {
    $output = drupal_get_form('ncn_ceportal_admin_mail_settings_form');
    return $output;
}

function ncn_ceportal_admin_mail_settings_form() {
    // Notification of New Available Claim 
    $form['ava_claim_notification'] = array(
        '#type' => 'fieldset', 
        '#title' => t('Notification of Available Claim for CE'), 
        '#weight' => -50, 
        '#collapsible' => TRUE, 
        '#collapsed' => TRUE,
    );
    $form['ava_claim_notification']['ce_ava_claim_note_mail_to'] = array(
        '#type' => 'textfield', 
        '#title' => t('mail To'),
        '#default_value' => variable_get('ce_ava_claim_note_mail_to', 'claimsexaminer@netclaimsnow.com'),
        '#required' => 1,
    );
    $form['ava_claim_notification']['ce_ava_claim_note_mail_subject'] = array(
        '#type' => 'textfield', 
        '#title' => t('Subject'),
        '#default_value' => variable_get('ce_ava_claim_note_mail_subject', ''),
        '#required' => 1,
    );
    $form['ava_claim_notification']['ce_ava_claim_note_mail_body'] = array(
        '#type' => 'textarea', 
        '#title' => t('body'),
        '#default_value' => variable_get('ce_ava_claim_note_mail_body', ''),
    );
    
    // Notification of Accepting Available Claim from CE
    $form['accept_ava_claim'] = array(
        '#type' => 'fieldset', 
        '#title' => t('CE accept claim'), 
        '#weight' => -45, 
        '#collapsible' => TRUE, 
        '#collapsed' => TRUE,
    );
    $form['accept_ava_claim']['ce_accept_ava_claim_mail_subject'] = array(
        '#type' => 'textfield', 
        '#title' => t('Subject'),
        '#default_value' => variable_get('ce_accept_ava_claim_mail_subject', ''),
        '#required' => 1,
    );
    $form['accept_ava_claim']['ce_accept_ava_claim_mail_body'] = array(
        '#type' => 'textarea', 
        '#title' => t('body'),
        '#default_value' => variable_get('ce_accept_ava_claim_mail_body', ''),
    );
    
    // Notification of Assigning Available Claim to CE
    $form['assign_ava_claim'] = array(
        '#type' => 'fieldset', 
        '#title' => t('Assigning claim to CE'), 
        '#weight' => -42, 
        '#collapsible' => TRUE, 
        '#collapsed' => TRUE,
    );
    $form['assign_ava_claim']['ce_assign_ava_claim_mail_subject'] = array(
        '#type' => 'textfield', 
        '#title' => t('Subject'),
        '#default_value' => variable_get('ce_assign_ava_claim_mail_subject', ''),
        '#required' => 1,
    );
    $form['assign_ava_claim']['ce_assign_ava_claim_mail_body'] = array(
        '#type' => 'textarea', 
        '#title' => t('body'),
        '#default_value' => variable_get('ce_assign_ava_claim_mail_body', ''),
    );
    // Notification of Missing Available Claim from CE
    $form['miss_ava_claim'] = array(
        '#type' => 'fieldset', 
        '#title' => t('Missed claim from CE'), 
        '#weight' => -40, 
        '#collapsible' => TRUE, 
        '#collapsed' => TRUE,
    );
    $form['miss_ava_claim']['ce_miss_ava_claim_mail_subject'] = array(
        '#type' => 'textfield', 
        '#title' => t('Subject'),
        '#default_value' => variable_get('ce_miss_ava_claim_mail_subject', ''),
        '#required' => 1,
    );
    $form['miss_ava_claim']['ce_miss_ava_claim_mail_body'] = array(
        '#type' => 'textarea', 
        '#title' => t('body'),
        '#default_value' => variable_get('ce_miss_ava_claim_mail_body', ''),
    );
    
    // Notification of CE Completing
    $form['ce_complete'] = array(
        '#type' => 'fieldset', 
        '#title' => t('CE complete claim'), 
        '#weight' => -36, 
        '#collapsible' => TRUE, 
        '#collapsed' => TRUE,
    );
    $form['ce_complete']['ce_complete_mail_subject'] = array(
        '#type' => 'textfield', 
        '#title' => t('Subject'),
        '#default_value' => variable_get('ce_complete_mail_subject', ''),
        '#required' => 1,
    );
    $form['ce_complete']['ce_complete_mail_body'] = array(
        '#type' => 'textarea', 
        '#title' => t('body'),
        '#default_value' => variable_get('ce_complete_mail_body', ''),
    );
    
    // Notification of Checked Assign to CE
    $form['checked_assign_to_ce'] = array(
        '#type' => 'fieldset', 
        '#title' => t('When checked "Assigned to CE" checkbox'), 
        '#weight' => -35, 
        '#collapsible' => TRUE, 
        '#collapsed' => TRUE,
    );
    $form['checked_assign_to_ce']['checked_assign_to_ce_mail_subject'] = array(
        '#type' => 'textfield', 
        '#title' => t('Subject'),
        '#default_value' => variable_get('checked_assign_to_ce_mail_subject', ''),
        '#required' => 1,
    );
    $form['checked_assign_to_ce']['checked_assign_to_ce_mail_body'] = array(
        '#type' => 'textarea', 
        '#title' => t('body'),
        '#default_value' => variable_get('checked_assign_to_ce_mail_body', ''),
    );
    
    
    // CE Mail Tokens
    $form['ce_mail_token'] = array(
        '#type' => 'fieldset', 
        '#title' => t('CE Mail Tokens'), 
        '#weight' => 50, 
        '#collapsible' => TRUE, 
        '#collapsed' => FALSE,
    );
    $form['ce_mail_token']['ce_mail_token_table'] = array(
        '#markup' => ncn_ceportal_admin_mail_settings_token_table(),
    );
    
    $form['submit'] = array('#type' => 'submit', '#weight' => 101, '#value' => t('Save'));
    return $form;
}

function ncn_ceportal_admin_mail_settings_token_table() {
    $header = array(
        array('data' => t('Token')),
        array('data' => t('Description')),
    );
    $tokens = ncn_ceportal_get_ce_mail_tokens();
    foreach ($tokens as $token=>$desc) {
        $rows[] = array(
            'data'=>array($token, $desc), 
        );
    }
    $output = theme('table',array('header'=>$header,'rows'=>$rows, 'attributes'=>array('id' => 'ce_mail_settings_token_table')));
    
    return $output;
}

function ncn_ceportal_admin_mail_settings_form_submit($form, &$form_state) {
    variable_set('ce_ava_claim_note_mail_to', t($form_state['values']['ce_ava_claim_note_mail_to']));
    variable_set('ce_ava_claim_note_mail_subject', t($form_state['values']['ce_ava_claim_note_mail_subject']));
    variable_set('ce_ava_claim_note_mail_body', t($form_state['values']['ce_ava_claim_note_mail_body']));
    
    variable_set('ce_accept_ava_claim_mail_subject', t($form_state['values']['ce_accept_ava_claim_mail_subject']));
    variable_set('ce_accept_ava_claim_mail_body', t($form_state['values']['ce_accept_ava_claim_mail_body']));
    
    variable_set('ce_assign_ava_claim_mail_subject', t($form_state['values']['ce_assign_ava_claim_mail_subject']));
    variable_set('ce_assign_ava_claim_mail_body', t($form_state['values']['ce_assign_ava_claim_mail_body']));
    
    variable_set('ce_miss_ava_claim_mail_subject', t($form_state['values']['ce_miss_ava_claim_mail_subject']));
    variable_set('ce_miss_ava_claim_mail_body', t($form_state['values']['ce_miss_ava_claim_mail_body']));
    
    variable_set('ce_complete_mail_subject', t($form_state['values']['ce_complete_mail_subject']));
    variable_set('ce_complete_mail_body', t($form_state['values']['ce_complete_mail_body']));
    
    variable_set('checked_assign_to_ce_mail_subject', t($form_state['values']['checked_assign_to_ce_mail_subject']));
    variable_set('checked_assign_to_ce_mail_body', t($form_state['values']['checked_assign_to_ce_mail_body']));
}

function ncn_ceportal_admin_weekly_sales_report() {
GLOBAL $base_url;
    $report_date = 0;
    if ( arg(4) == "" ) {
        $report_date = time();
    } else {
        $report_date = arg(4);
    }
    
    $ce_uid = intval(arg(5));
    $ce_list = get_user_list(14);
    
    foreach($ce_list as $uid=>$ce_user) {
        $_user = user_load($uid);
        if ($_user) {
            $ce_list[$uid]['name'] = $_user->profile_firstname.' '.$_user->profile_lastname;
        }
    }
    
    /*if (!empty($ce_list)) {
        if ($ce_uid == 0 || !array_key_exists($ce_uid, $ce_list)) {
            $keys = array_keys($ce_list);
            $ce_uid = $keys[0];
        }
    }*/
    if (array_key_exists($ce_uid, $ce_list)) {
        $ce_list[$ce_uid]['selected'] = true;
    }
    
    
    $next_week = strtotime("+1 week", $report_date);
    $last_week = strtotime("-1 week", $report_date);
    
    $url = $base_url."/admin/config/ncn_ceportal/weekly_sales_report/";
    if (isset($_REQUEST['rsd'])) {
        $report_date = strtotime($_REQUEST['rsd']);
        $timezone = variable_get('date_default_timezone', 0);
        $report_date -= $timezone;
        drupal_goto($url.$report_date);
        exit;
    }
    
    //$download_url = $base_url."/download/ncn_report/weekly?date=$report_date";
    
    $to = strtotime("next monday", $report_date);
    $from = strtotime("last monday", $to);
    
    $data = array();
    $total = 0;
    
    $url_query = "?date=$report_date&ce_uid=$ce_uid&dest=".base64_encode("admin/config/ncn_ceportal/weekly_sales_report/$report_date/$ce_uid");
    $create_url = $base_url.base_path()."ce_statement/create";
    $create_text = "Create Statement";
    if ($ce_uid == 0) {
        $create_text = "Create All Statements";
        $create_url = $base_url.base_path()."ce_statement/all_create";
    }
    $create_url .= $url_query;
    
    ob_start();
?>

<div class="sales-report-header clear-block">
    <div class="report-date">
        <span><a href="<?php echo $url; ?>">[This Week]</a></span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <span><a href="<?php echo $url.$last_week; ?>" title="<?php echo date("Y/m/d", strtotime("-1 week", $from)); ?> to <?php echo date("Y/m/d", strtotime("-1 day", $from)); ?>">[Last]</a></span>&nbsp;&nbsp;
        <span><input type="text" id="datepicker" value="<?php echo date("Y/m/d", $from); ?> ~ <?php echo date("Y/m/d", strtotime("-1 day", $to)); ?>" size="25" readonly></span>&nbsp;
        <span><a href="<?php echo $url.$next_week; ?>" title="<?php echo date("Y/m/d", $to); ?> to <?php echo date("Y/m/d", strtotime("+6 day", $to)); ?>">[Next]</a></span>&nbsp;&nbsp;
    </div>
    <input type="hidden" id="report_url" value="<?php echo $url; ?>" />
    <?php echo ncn_report_insert_date_script(); ?>
    <div class="download-report">
        <a href="<?php echo $create_url; ?>"><?php echo $create_text; ?></a>
    </div>
</div>

<script type="text/javascript">
jQuerydocument).ready(function() {
    jQuery"#ce_name").bind('change', function() {
        var url = jQuery"#ce_name").val();
        window.location = url;
    });
});
</script>

<div class="sales-report-header-name">
    <label for="ce_name">Claim Examiner: </label>
    <select id="ce_name">
        <option value="<?php echo "$url/$report_date"; ?>" <?php if($ce_uid==0) { echo "selected"; } ?> >All</option>
        <?php foreach ($ce_list as $uid=>$ce_user) :?>
        <option value="<?php echo "$url/$report_date/$uid"; ?>" <?php if(isset($ce_user['selected'])) { echo "selected"; } ?> >
            <?php echo $ce_user['name']; ?>
        </option>
        <?php endforeach; ?>
    </select>
</div>
<?php
    // Nikhil: Need to check here
    _ncn_ceportal_render_sales_table($report_date, $ce_uid);
    echo "<div style='margin-top: 25px;'><h3><strong>Statements</strong></h3></div>";
    build_ce_statement_block($report_date, $ce_uid);
    $output = ob_get_contents();
    ob_end_clean();
    return $output;
}

function _ncn_ceportal_render_sales_table($report_date, $user_id=0) {
    $to = strtotime("next monday", $report_date);
    $from = strtotime("last monday", $to);
    
    //$count_query = "SELECT COUNT(*)    FROM {ncn_ce_sales_report} WHERE status=1 AND timestamp>=$from AND timestamp<$to ";
    // $count_query = db_select('ncn_ce_sales_report', 'n')
 //        ->fields('n')
 //        ->condition('status', '1','=')
 //        ->condition('timestamp',$from ,'>=')
 //        ->condition('timestamp',$to,'<')
 //        ->countquery()
 //        ->extend('PagerDefault')
 //        ->limit(10)
 //        ->execute();
   // $count_query = $count_query->
    $sql         = "SELECT * FROM {ncn_ce_sales_report} WHERE status=1 AND timestamp>=$from AND timestamp<$to";
    $total_query = "SELECT SUM(amount) as amt FROM {ncn_ce_sales_report} WHERE status=1 AND timestamp>=$from AND timestamp<$to ";
    if($user_id)
    {
        //$count_query->condition('ce_uid',$user_id,'=');
        $sql         .= " AND ce_uid=".$user_id;
        $total_query .= " AND ce_uid=".$user_id;
    }
    $sql .= " ORDER BY timestamp DESC";
    //$result = pager_query($sql, 10, 0, $count_query);
    //$result = $count_query;
    
    $headers  = array();
    $rows = array();
    $total_result = db_query($sql);
    $total_amount = db_query($total_query)->fetchField();

    foreach($total_result as $row)
    {
        $row = (array)$row;
        $_user = user_load($row['ce_uid']);

        $claim_q = db_query("SELECT claim_id FROM {ncn_ce_claim_trigger} WHERE id=:d", array(":d"=>$row['ce_claim_trigger_id']));
        $claim_id = $claim_q->fetchField();

        $cur_path = $_GET['q'];
        $op_edit_url = base_path()."admin/config/ncn_ceportal/sales/".$row['id']."/edit?dest=".base64_encode($cur_path);

        $rows[] = array(
            array('data' => $row['id']),
            array('data' => $_user->profile_firstname.' '.$_user->profile_lastname),
            array('data' => $claim_id ),
            array('data' => render_payment_cost($row['amount'])),
            array('data' => date("m/d/Y H:i", $row['timestamp'])),
            array('data' => "<a href='$op_edit_url'>Edit</a>"),
        );

    }

    $headers = array("ID", "Full Name", "Claim ID", "Cost", "Timestamp", "Operation");

    $output = theme('table',array("headers"=>$headers,"rows"=>$rows));
    $output .= theme('pager');
    $output .= "<div class='clearfix'><div style='float: right; font-weight: bold;'>Total: ".render_payment_cost($total_amount)."</div></div>";

    print $output;
}

function ncn_ceportal_get_sales_data($report_date, $user_id, &$data) {
    $to = strtotime("next monday", $report_date);
    $from = strtotime("last monday", $to);
    
    $sql = "SELECT * FROM {ncn_ce_sales_report} WHERE status=1 AND timestamp>=$from AND timestamp<$to ";
    
    if ($user_id) {
        $sql .= " AND ce_uid=".$user_id;
    }
    $sql .= " ORDER BY timestamp DESC";
    $result = db_query($sql);
//  while ($row = db_fetch_array($result))
    foreach($result as $row)
    {
        $row = (array)$row;
        $_user = user_load($row['ce_uid']);
        $claim_id = db_query("SELECT claim_id FROM {ncn_ce_claim_trigger} WHERE id=:d",array(":d"=>$row['ce_claim_trigger_id']));
        $row['claim_id'] = $claim_id;
        $row['ce_name'] = $_user->profile_firstname.' '.$_user->profile_lastname;
        
        $data[] = $row;
    }
}

function ncn_ceportal_admin_edit_sale($id) {
    drupal_set_title( t("Payment #@id", array('@id'=>$id)) );
    $output = '';
    if (isset($_POST['tfunction']) && $_POST['tfunction'] == "ncn_ceportal_sale_edit") {
        ncn_ceportal_sale_edit_action($id);
    }
    
    $sale_info = ncn_ce_portal_get_sales_info($id);
    if (empty($sale_info)) {
        $dest = base64_decode($_REQUEST['dest']);
        drupal_goto($dest);
    }
    
    $_user = user_load($sale_info['ce_uid']);
    $ce_name = $_user->profile_firstname.' '.$_user->profile_lastname;
    ob_start();
?>
    <div class="ncn-ceportal-admin-edit-sale">
        <form method="POST">
        <input type="hidden" name="tfunction" value="ncn_ceportal_sale_edit" />
        <table>
            <tr><td class="td-label" style="width: 150px"><label>Claim Examiner: </label></td> <td><?php echo $ce_name; ?></td></tr>
            <tr><td class="td-label"><label>Amount: </label></td> <td>$<input type="text" name="amount" value="<?php echo $sale_info['amount']; ?>" /></td></tr>
            <tr><td class="td-label"><label>Timestamp: </label></td> <td><?php echo date("m/d/Y H:i", $sale_info['timestamp']); ?></td></tr>
            <tr><td colspan="2"><input type="submit" value="Save" /></td></tr>
        </table>
        </form>
    </div>
<?php
    $output = ob_get_contents();
    ob_end_clean();
    return $output;
}

function ncn_ceportal_sale_edit_action($id) {
    $amount = $_POST['amount'];
    db_query("UPDATE {ncn_ce_sales_report} SET amount=:f WHERE id=:d",array(":f"=>$amount,":d"=>$id));
    
    $dest = base64_decode($_REQUEST['dest']);
    drupal_goto($dest);
}

function ncn_ceportal_reavailable_for_completed_claim($claim_id) {
    $ce_claim = ncn_ceportal_get_ncn_ce_claim($claim_id);
    if (!empty($ce_claim) && $ce_claim['status']=='archive') {
        $ce_claim_trigger = ncn_ceportal_get_completed_ce_claim_trigger($claim_id, $ce_claim['ce_uid']);
        if (!empty($ce_claim_trigger)) {
            db_query("DELETE FROM {ncn_ce_claim_trigger} WHERE id=:d",array(":d"=>$ce_claim_trigger['id']));                                         // CE Claim Trigger
            db_query("DELETE FROM {ncn_ce_sales_report} WHERE ce_claim_trigger_id=:d",array(":d"=>$ce_claim_trigger['id']));         // CE Sales Report
            
            $result = db_query("UPDATE {ncn_ce_claims} SET assigned=0, ce_uid=0, status='' WHERE claim_id=:d",array(":d"=>$claim_id));       // CE Claim
            if ($result) {
                drupal_set_message( t("Claim (#!claim_id) is reavailable for Claim Examiner", array('!claim_id'=>$claim_id)) );
            }
        }
    }
    
    drupal_goto("admin/config/ncn_admin/viewclaim/".$claim_id);
}

/**
 * Make Claim Info Reviewable( in backend )
 */
function ncn_ceportal_claim_info_reviewable_section($claim_id, $pvc_ce_management, $pvc_ce_management_disabled) {
    //if (!ncn_ceportal_is_not_assigned_claim($claim_id)) { return ""; }
    
    $ce_claim = ncn_ceportal_get_ncn_ce_claim($claim_id);
    if (empty($ce_claim)) {
        return "";
    }
    
    $reviewable = get_ncn_data_extra($claim_id, 'ce_reviewable_claim_info');
    
    $output = "<div id='ce_reviewable_claim_info'>";
    $output.= '<form id="ce_assign_claim_to_ce_form" method="POST">';
    $output.= '<input type="hidden" name="tfunction_router" value="ncn_ceportal_claimview_claim_exaimners" />';
    $output.= '<input type="hidden" name="tfunction" value="ncn_ceportal_set_reviewable_claim_info" />';
    
    $output.= "<input type='checkbox' id='ncn_ceportal_reviewable_mark' name='reviewable' value='reviewable' ".(($reviewable=='reviewable')? 'CHECKED':'')." />&nbsp;<label for='ncn_ceportal_reviewable_mark'><strong>Reviewable Claim Info </strong></label>";
    if ($pvc_ce_management_disabled != 'disabled') {
        $output.="&nbsp;&nbsp;&nbsp;";
        $output.= '<input type="submit" value=" Set " />';
    }
    $output.= '</form>';
    $output.= "</div>";
    
    return $output;
}

function ncn_ceportal_set_reviewable_claim_info($claim_id) {
    $reviewable = '';
    if (isset($_REQUEST['reviewable'])) {
        $reviewable = $_REQUEST['reviewable'];
    }
    set_ncn_data_extra($claim_id, 'ce_reviewable_claim_info', $reviewable);
}
?>
