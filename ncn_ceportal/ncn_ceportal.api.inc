<?php

/*****************************************************************************/
/*      CE Portal API
/*****************************************************************************/
function ncn_submit_handler_reflection($function) {
    if ($function && function_exists($function)) {
        $params = func_get_args();
        array_shift($params);
        
        if (!is_array($params)) {
            $params = array($params);
        }
        return call_user_func_array($function, $params);
    } else {
        drupal_set_message(t('No function: !func_name', array('!func_name'=>$function)), 'error');
    }
    return FALSE;
}

function ncn_ceportal_get_ncn_ce_claim($claim_id) {
    $result = db_query("SELECT * FROM {ncn_ce_claims} WHERE claim_id=:d", array(':d'=>$claim_id));
    if($result->rowCount()>0)
    {
        foreach($result as $ce_claim)
        {
            $ce_claim = (array)$ce_claim;
            return $ce_claim;
        }
        return array();

    }

}

//    if ($result && $ce_claim = db_fetch_array($result)) {
//      return $ce_claim;
//  }
//  return array();
//}

function ncn_ceportal_is_available_claim($claim_id) {
    $ce_claim = ncn_ceportal_get_ncn_ce_claim($claim_id);
    if ($ce_claim['is_available'] && $ce_claim['status'] != 'archive') {
        return TRUE;
    }
    return FALSE;
}

function ncn_ceportal_is_not_assigned_claim($claim_id) {
    if (!ncn_ceportal_is_available_claim($claim_id)) { return FALSE; }
    $ce_claim = ncn_ceportal_get_ncn_ce_claim($claim_id);
    if ($ce_claim['assigned'] == 0) {
        return TRUE;
    }
    
    return FALSE;
}

/**
 * Get Claim Examiner List
 */
function ncn_ceportal_get_accepted_ce_list($claim_id) {
    $result = db_query("SELECT * FROM {ncn_ce_claim_accept} WHERE claim_id=:d", array(':d'=>$claim_id));
    $data = array();
//  while($row = db_fetch_array($result))
    foreach($result as $row)
    {
        $row = (array)$row;
        $data[$row['ce_uid']] = user_load($row['ce_uid']);
    }
    return $data;

//    {
//      $data[$row['ce_uid']] = user_load($row['ce_uid']);
//  }
//  return $data;
}

function ncn_ceportal_delete_all_accepted_ce_list($claim_id) {
    $result = db_query("DELETE FROM {ncn_ce_claim_accept} WHERE claim_id=:d",array(":d"=>$claim_id) );
}

/**
 * Get Available Claims
 */
function ncn_ceportal_get_available_claims_list($user_id, $params) {
    if (!isset($params['filter']) || $params['filter'] == '') {
        $addition_where = '';
    } else {
        $addition_where = "AND assigned=".$params['filter'];
    }
    
    $result = db_query("SELECT * FROM {ncn_ce_claims} WHERE is_available=1 AND status!='archive' $addition_where ORDER BY claim_id DESC LIMIT ".$params['pager']['from'].", ".$params['pager']['count']);

    $data = array();
//  while ($ce_claim = db_fetch_array($result))

    foreach($result as $ce_claim)
    {
        $ce_claim = (array)$ce_claim;
        $data[] = $ce_claim;
    }
    return $data;
}

function ncn_ce_portal_get_ava_claims_total_count($user_id, $params) {
    if ($params['filter'] == '') {
        $addition_where = '';
    } else {
        $addition_where = "AND assigned=".$params['filter'];
    }
    
    $result = db_query("SELECT * FROM {ncn_ce_claims} WHERE is_available=1 AND status!='archive' $addition_where");
    $result = $result->rowCount();
    return $result;
}
    
function ncn_ceportal_get_assigned_status_data() {
    return array(
        0=>array('value'=>'not_assigned', 'text'=>'Not Assigned'), 
        1=>array('value'=>'assigned',       'text'=>'Assigned'), 
        2=>array('value'=>'re_assigned',    'text'=>'Re-Assigned'), 
    );
}

function ncn_ceportal_is_disabled_to_accept_claim($claim_id, $ce_uid) {
    $result = db_query("SELECT * FROM {ncn_ce_claim_accept} WHERE claim_id=:claim_id AND ce_uid=:ce_uid",
        array(":claim_id"=>$claim_id,":ce_uid"=>$ce_uid));
    if ($result->rowCount()>0) {
        return 'accepted';
    }
    
    $result = db_query("SELECT * FROM {ncn_ce_claim_trigger} WHERE claim_id=:claim_id AND ce_uid=:ce_uid AND status=1",
        array(":claim_id"=>$claim_id,":ce_uid"=>$ce_uid));
    if ($result->rowCount()>0) {
        return 'assigned';
    }
    
    $result = db_query("SELECT * FROM {ncn_ce_claim_trigger} WHERE claim_id=:claim_id AND ce_uid=:ce_uid AND status=0",
        array(":claim_id"=>$claim_id,":ce_uid"=>$ce_uid));
    if ($result->rowCount()>1) {
        return 'missed';
    } else {
        $ce_auto_assign = variable_get('ce_auto_assign', FALSE);
        if ($ce_auto_assign) {
            $re_aa = ncn_ceportal_is_available_when_auto_assign($claim_id, $ce_uid);
            if ($re_aa === 'special-auto-assign') {
                //return FALSE;
            } else if(!$re_aa) {
                return 'special-auto-assign';
            }
        }
    }
    
    $result = ncn_ceportal_get_active_claims_total_count($ce_uid);
    if ($result>=2) {
        return 'overload';
    }
    
    return FALSE;
    
}

function ncn_ceportal_is_available_when_auto_assign($claim_id, $ce_uid) {
    $result = db_query("SELECT * FROM {ncn_ce_claim_trigger} WHERE claim_id=:d ORDER BY id DESC",array(":d"=>$claim_id));

    if($result)
    {
        foreach($result as $cec_trigger)
        {
            $cec_trigger = (array)$cec_trigger;
            if($cec_trigger)
            {
                if ($cec_trigger['status'] == 0 && (date('U')-$cec_trigger['ts_end'])<3600 ) {
                    if ($cec_trigger['ce_uid'] != $ce_uid) {
                        if (ncn_ceportal_get_time_assigned($claim_id, $cec_trigger['ce_uid'], 'missed')>1) {
                            return TRUE;
                        }
                        return FALSE;
                    } else {
                        return 'special-auto-assign';
                    }
                }
            }
        }
    }
    return TRUE;





//    if ($result && $cec_trigger=db_fetch_array($result)) {
//      if ($cec_trigger['status'] == 0 && (date('U')-$cec_trigger['ts_end'])<3600 ) {
//          if ($cec_trigger['ce_uid'] != $ce_uid) {
//              if (ncn_ceportal_get_time_assigned($claim_id, $cec_trigger['ce_uid'], 'missed')>1) {
//                  return TRUE;
//              }
//              return FALSE;
//          } else {
//              return 'special-auto-assign';
//          }
//      }
//  }
//  return TRUE;
}

function ncn_ceportal_get_time_assigned($claim_id, $ce_uid, $mode="all") {
    $where = '';
    if ($mode=='missed') {
        $where.= " AND status=0";
    }
    $result = db_query("SELECT * FROM {ncn_ce_claim_trigger} WHERE claim_id=:claim_id AND ce_uid=:ce_uid".$where,
        array(":claim_id"=>$claim_id,":ce_uid"=>$ce_uid));
    return $result->rowCount();
}

function ncn_ceportal_ce_claim_timer_action($ce_claim_id, $action, $params=array()) {
    $ce_claim = ncn_ceportal_get_ncn_ce_claim($ce_claim_id);
    switch ($action) {
    case 'start':
        $result = db_query("INSERT INTO {ncn_ce_claim_trigger}(claim_id, ce_uid, ts_start, period, ts_end, status) VALUES(:a, :b, :c, :d, :e, :f)",
                           array(':a' => $ce_claim_id, 
                                 ':b' => $params['ce_uid'], 
                                 ':c' => $params['ts_start'], 
                                 ':d' => $params['period'], 
                                 ':e' => 0, 
                                 ':f' => 1))
        ;
        if ($result) { 
            $ret_val = TRUE;
            // claim log
        }
        break;
    case 'stop':
        $result = db_query("UPDATE {ncn_ce_claim_trigger} SET ts_end=:ts_end, status=:status WHERE claim_id=:claim_id AND status=1", 
                            array(':ts_end' => $params['ts_end'], 
                                  ':status' => $params['status'], 
                                  ':claim_id' => $ce_claim_id));
        if ($result) { 
            $ret_val = TRUE;
            // claim log
        }
        break;
    }
    
    return isset($ret_val)? $ret_val : FALSE;
}

/**
 * Get Active Claims
 */
function ncn_ceportal_get_active_claims_list($user_id, $params) {
    $result = db_query("SELECT * FROM {ncn_ce_claims} WHERE status='active' AND ce_uid=:ceid ORDER BY claim_id DESC LIMIT ".$params['pager']['from'].", ".$params['pager']['count'], 
                        array(':ceid'   =>$user_id));
    $data = array();
    foreach($result as $ce_claim)
    {
        $ce_claim = (array)$ce_claim;
        $data[] = $ce_claim;
    }
    return $data;
}
function ncn_ceportal_get_active_claims_total_count ($user_id, $params=array()) {
    $result = db_query("SELECT * FROM {ncn_ce_claims} WHERE status='active' AND ce_uid=:uid", array(':uid'=>$user_id)); 
    return $result->rowCount();
}

function ncn_ceportal_get_active_ce_claim_trigger($claim_id, $ce_uid) {
    $result = db_query("SELECT * FROM {ncn_ce_claim_trigger} WHERE status=1 AND claim_id=:clid AND ce_uid=:ceid ORDER BY ts_start DESC", array(':clid'=>$claim_id,':ceid'=>$ce_uid));   
    if ($result->rowCount()>0) {
        $row = $result->fetchAssoc();
        return $row;
    }   
    return array();
}

function ncn_ceportal_check_expired_all_timer_trigger() {
    $result = db_query("SELECT * FROM {ncn_ce_claim_trigger} WHERE status=1");
    while ($row = $result->fetchAssoc()) {
        ncn_ceportal_check_expired_active_claim_timer_trigger($row['claim_id']);
    }
}

function ncn_ceportal_check_expired_active_claim_timer_trigger($claim_id) {
global $user;
    if ($user->uid==0 || $user->uid==1) {
        $result = db_query("SELECT * FROM {ncn_ce_claim_trigger} WHERE claim_id=:d AND status=1", array(':d'=>$claim_id));
    } else {
        $result = db_query("SELECT * FROM {ncn_ce_claim_trigger} WHERE claim_id=:d AND status=1 AND ce_uid=:cd", array(':d'=>$claim_id,':cd'=>$user->uid));
    }
    if ($ce_trigger = $result->fetchAssoc()) {
        if (($ce_trigger['ts_start']+$ce_trigger['period'])<date('U')) {
            $res = ncn_ceportal_ava_claim($claim_id, 'miss', $ce_trigger['ce_uid']);
            if ($res) {
                // mail
                $ce_user = user_load($ce_trigger['ce_uid']);
                $mail_to = $ce_user->mail;
                if ($mail_to) {
                    $params = array(
                        '!claim_id'=>$claim_id, 
                        '!ce_name'=>($ce_user->profile_firstname.' '.$ce_user->profile_lastname), 
                    );
                    drupal_mail('ncn_ceportal', 'miss_ava_claim', $mail_to, language_default(), $params);
                }
                
                $pm_subject = t("You missed claim#!claim_id.", array('!claim_id'=>$claim_id ));
                $pm_msg = t("Timer has been expired.
                You should have completed this claim for 4 hours.
                You can't accept to this claim again.");
                pm_send(0, $ce_trigger['ce_uid'], $pm_subject, $pm_msg, $claim_id, 0, 'no mail');
                
                // mail to admin?
            }
        }
    }
}


/**
 * Get Archive Claims List
 */
function ncn_ceportal_get_archive_claims_list($user_id, $params) {
    $result = db_query("SELECT * FROM {ncn_ce_claims} WHERE status='archive' AND ce_uid=:ce_uid ORDER BY claim_id DESC LIMIT ".$params['pager']['from'].", ".$params['pager']['count'], 
                        array(':ce_uid'=>$user_id));
    $data = array();
//  while ($ce_claim = db_fetch_array($result))

    foreach($result as $ce_claim)
    {
        $ce_claim = (array)$ce_claim;
        $data[] = $ce_claim;
    }
    
    return $data;
}
function ncn_ceportal_get_archive_claims_total_count($user_id, $params) {
    $result = db_query("SELECT * FROM {ncn_ce_claims} WHERE status='archive' AND ce_uid=:ce_uid", array(':ce_uid'=>$user_id));
    
    return $result->rowCount();
}

function ncn_ceportal_get_completed_ce_claim_trigger($ce_claim_id, $ce_uid) {
    $result = db_query("SELECT * FROM {ncn_ce_claim_trigger} WHERE claim_id=$ce_claim_id AND ce_uid=$ce_uid AND status=2 ORDER BY ts_end DESC");
    if ($result && $trigger = $result->fetchAssoc()) {
        return $trigger;
    }
    return array();
}

function ncn_ceportal_get_completed_ce_claim_trigger_from_id($tid) {
    $result = db_query("SELECT * FROM {ncn_ce_claim_trigger} WHERE id=$tid");
    if ($result && $trigger = $result->fetchAssoc()) {
        return $trigger;
    }
    return array();
}
/**
 * Pay for Handling of CE Claim 
 */
function ncn_ceportal_pay_ce_claim($ce_claim_id, $ce_uid) {
    $ce_claim_trigger = ncn_ceportal_get_completed_ce_claim_trigger($ce_claim_id, $ce_uid);
    $working_time = ncn_ceportal_get_working_time($ce_claim_id, $ce_uid);
    if (empty($ce_claim_trigger) || $working_time === FALSE ) {
        return FALSE;
    }
    
    $timestamp = date('U');
    $amount = 10.00;
    
    //* check if this is 2nd attempt    
    if ( ncn_ceportal_get_time_assigned($ce_claim_id, $ce_uid, 'missed')==0 && $working_time < 2*3600 ) {
        $amount = 15.00;    // less than 2 hrs of working.
    }
    $result = db_query("INSERT INTO {ncn_ce_sales_report}(ce_uid, amount, timestamp, ce_claim_trigger_id, status) VALUES(:a, :b, :c, :d, :e)", 
                        array(':a' => $ce_uid, 
                              ':b' => $amount, 
                              ':c' => $timestamp, 
                              ':d' => $ce_claim_trigger['id'], 
                              ':e' => 1));
    if (!$result) {
        drupal_set_message(t("Failed to pay for claim(#!claim_id)", array('!claim_id'=>$ce_claim_id)), 'error');
        return FALSE;
    }
    return TRUE;
}

function ncn_ceportal_get_working_time($ce_claim_id, $ce_uid) {
    $ce_claim_trigger = ncn_ceportal_get_completed_ce_claim_trigger($ce_claim_id, $ce_uid);
    if (empty($ce_claim_trigger)) {
        return FALSE;
    }
    
    return ($ce_claim_trigger['ts_end']-$ce_claim_trigger['ts_start']);
}

function ncn_ceportal_get_sales_report_list($user_id, $params) {
    $str = date("Y/m/01", $params['report_date']);
    $from = strtotime($str);
    $to = strtotime("+1 month", $from);

    $where = " AND status=1 AND timestamp>=$from AND timestamp<$to ";
    $result = db_query("SELECT * FROM {ncn_ce_sales_report} WHERE ce_uid=:ce_uid $where LIMIT ".$params['pager']['from'].", ".$params['pager']['count'],
                        array(':ce_uid' => $user_id));
    $data = array();
//  while ($sale = db_fetch_array($result))
    foreach($result as $sale)
    {
        $sale = (array)$sale;
        $data[] = $sale;
    }
    
    return $data;
}
function ncn_ceportal_get_sales_report_total_count($user_id, $params) {
    $str = date("Y/m/01", $params['report_date']);
    $from = strtotime($str);
    $to = strtotime("+1 month", $from);

    $where = " AND status=1 AND timestamp>=$from AND timestamp<$to ";
    $result = db_query("SELECT * FROM {ncn_ce_sales_report} WHERE ce_uid=$user_id $where");
    
    return $result->rowCount();
}

function ncn_ceportal_get_total_monthly_earnings($user_id, $report_date) {
    $str = date("Y/m/01", $report_date);
    $from = strtotime($str);
    $to = strtotime("+1 month", $from);

    $where = " AND status=1 AND timestamp>=$from AND timestamp<$to ";
    $result = db_query("SELECT SUM(amount) FROM {ncn_ce_sales_report} WHERE ce_uid=$user_id $where")->fetchField();
    
    return floatval($result);
}

function ncn_ceportal_validate_complete_active_claim($claim_id) {
    if ( ncn_admin_claim_file_exist($claim_id, 11) && ncn_admin_claim_file_exist($claim_id, 12) ) {
        return TRUE;
    }
    return FALSE;
}



function ncn_ceportal_get_ce_mail_tokens() {
    return array(
        '!claim_id' => t('Claim ID'), 
        '!ce_name' => t("Claim Examiner Name"), 
    );
}







function ncn_ce_portal_send_mail_when_ce_complete($ce_claim_id) {
    $ce_claim = ncn_ceportal_get_ncn_ce_claim($ce_claim_id);
    $ce_user = user_load($ce_claim['ce_uid']);
    $ce_name = $ce_user->profile_firstname.' '.$ce_user->profile_lastname;
    
    $_admin = user_load(1);
    $admin_mail = $_admin->mail;    //***
    $params = array(
        '!claim_id'=>$ce_claim_id, 
        '!ce_name'=>$ce_name
    );
    drupal_mail('ncn_ceportal', 'ce_complete', $admin_mail, language_default(), $params);
    
    $qa_list = get_qa_list();
    foreach($qa_list as $qa) {
        if ($qa['mail']) {
            drupal_mail('ncn_ceportal', 'ce_complete', $qa['mail'], language_default(), $params);
        }
    }
    
}

function ncn_ce_portal_admin_send_mail_when_checked_assign_to_ce($claim_id) {
    $_admin = user_load(1);
    $admin_mail = $_admin->mail;    //***
    $params = array(
        '!claim_id'=>$claim_id
    );
    drupal_mail('ncn_ceportal', 'checked_assign_to_ce', $admin_mail, language_default(), $params);
    
    $ms_list = get_ms_list();
    foreach($ms_list as $ms) {
        if ($ms['mail']) {
            drupal_mail('ncn_ceportal', 'checked_assign_to_ce', $ms['mail'], language_default(), $params);
        }
    }   
}

function ncn_ce_portal_set_mode($mode) {
    variable_set('ce_auto_assign', $mode);
    return TRUE;
}

function ncn_ce_portal_get_sales_info($id) {
    $result = db_query("SELECT * FROM {ncn_ce_sales_report} WHERE id=:id", array(':id'=>$id));
    if($result)
    {
        foreach($result as $row)
        {
            $row = (array)$row;
            if($row)
            {
                return $row;
            }
        }
    }
    return array();

//  if ($result && $row=db_fetch_array($result)) {
//      return $row;
//  }
//  return array();
}
?>
