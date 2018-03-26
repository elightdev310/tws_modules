<?php
/*****************************************************************************/
/*                          Claim Timer Trigger                              */
/*****************************************************************************/
/*
 * Change claim workflow
 */
function ncn_admin_claim_workflow_change($claim_id) {
    $action = '';
    $workflow = $_POST['claim_workflow'];
    $array_wf = get_claim_workflow_array();
    $error = false;
    
    //if ( $workflow == 'all_doc_recieved' ) {
    if ( $workflow == 'fax_doc_recieved' ) {
        $action = 'start';
    } else if ( $workflow == 'doc_clarification' || $workflow == 'rejected_invoices' ) {
        $action = 'reset';
    } else if ( $workflow == 'rejected_claim_docs' || $workflow == 'completed_status' ) {
        $action = 'stop';
    } else if ( !array_key_exists($workflow, $array_wf) ) {
        $error = true;
    }
    
    if ($error) {
        handle_unknown_error('10711-NCN-ADMIN');
        return FALSE;
    }
    
    // TIMER TRIGGER
    if ($action != '') {
        if (!update_timer_trigger($claim_id, $workflow, $action)) {
            /*$query = "SELECT * FROM claims WHERE claim_id=$claim_id";
            $result = db_query($query);
            if ($claim = (array)$result) {
                if ($claim['workflow']=='fax_doc_recieved') {
                    $query = "UPDATE claims SET workflow='$workflow' WHERE claim_id=$claim_id";
                    $result = db_query($query);
                    if (!$result) {
                        handle_mysql_syntax_error('10719-NCN-ADMIN', $query);
                        return FALSE;
                    }
                    // log message
                    $log_message = "Claim workflow updated to: ".$array_wf[$workflow];
                    drupal_set_message($log_message, 'status');
        
                    ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
                }
            }*/
            return FALSE;
        }
    } else {
        $query = "UPDATE claims SET workflow='$workflow' WHERE claim_id=$claim_id";
        $result = db_query('UPDATE {claims} SET workflow=:a WHERE claim_id=:b',array(':a'=>$workflow,':b'=>$claim_id));
        if (!$result) {
            handle_mysql_syntax_error('10709-NCN-ADMIN', $query);
            return FALSE;
        }
        // log message
        $log_message = ncn_amin_get_user_role_and_name()." updated claim workflow to ".$array_wf[$workflow];
        drupal_set_message($log_message, 'status');
        
        ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
    }
    
    // auto-mail
    if (!$error) {
        
        $_admin = user_load(1);
        $admin_mail = $_admin->mail;
        
        $site_mail =  variable_get('site_mail', ini_get('sendmail_from'));
        
    //  $query = "SELECT * FROM claims WHERE claim_id=$claim_id";
        $result = db_query('SELECT * FROM {claims} WHERE claim_id=:a',array(':a'=>$claim_id));
        if ($claim=$result->fetchAssoc()) {
            $_claim_user = user_load($claim['user_id']);
            $_member = get_member_from_id($_claim_user->profile_memberid);
            if (!empty($_member)) {
                $_am = user_load($_member['am_uid']);
                $am_mail = $_am->mail;      //***
            }
            
            $ms_list = get_ms_list();
            $qa_list = get_qa_list();
        
            if ($workflow == 'rejected_claim_docs') {
                //$subject = variable_get('ncn_rejected_claim_docs_mail_subject', '');
                //$subject = "Rejected Claim(# $claim_id) Documents";
                $owner_name = ncn_cd($claim_id, 'customer_name');
                $subject = "Additional Claim Information Need to Process Claim ID# $claim_id / $owner_name";
                $msg = $_POST['rejected_claim_docs_mail_body'];
                if ($msg != '') {
                    //ncn_admin_set_claim_docs_rejected_message($claim_id, $msg);
                    $log_message = "Sent rejection mail :\r\n".$msg;
                    drupal_set_message($log_message, 'status');
                    
                    pm_send(0, $_claim_user->uid, $subject, $msg, $claim_id, 0, 'no mail', array(), 1, $log_message);   //claim log
                    
                    $_claim_user = user_load($claim['user_id']);
                    $member = get_member_from_id($_claim_user->profile_memberid);
          $from_mail = "fax@netclaimsnow.com";    // $site_mail
                    drupal_mail( 'ncn_admin', 'claim_docs_rejected', $_claim_user->mail, language_default(), array('subject'=>$subject, '!claim_id'=>$claim_id, '!owner_name'=>$owner_name, '!reason'=>$msg), $from_mail );
                    
                    /*drupal_mail( 'ncn_admin', 'claim_docs_rejected', $admin_mail, language_default(), array('subject'=>$subject, 'body'=>$msg), $site_mail );
                    
                    if (isset($am_mail) && $am_mail != '') {
                        drupal_mail( 'ncn_admin', 'claim_docs_rejected', $am_mail, language_default(), array('subject'=>$subject, 'body'=>$msg), $site_mail );
                    }
                    foreach($ms_list as $ms) {
                        drupal_mail( 'ncn_admin', 'claim_docs_rejected', $ms['mail'], language_default(), array('subject'=>$subject, 'body'=>$msg), $site_mail );
                    }
                    foreach($qa_list as $qa) {
                        drupal_mail( 'ncn_admin', 'claim_docs_rejected', $qa['mail'], language_default(), array('subject'=>$subject, 'body'=>$msg), $site_mail );
                    }*/
                }
            }
        }
    }
    //drupal_set_message(t('Workflow changed, successfully.'));
    
    return TRUE;
}

/*
 * Update Timer Trigger
 * @param:  action - (start|stop|reset)
 *          is_auto - false: in admin, mannually.
 *                    true : cron, or real-time                         
 */
function update_timer_trigger($claim_id, $workflow, $action, $is_auto=false) {
    GLOBAL $user;
    $user = user_load($user->uid);
    $user_id = $user->uid;
    
    if (is_subuser($user)) {
        $parent_mid = ncn_subuser_get_memberid_from_subuid($user_id);
        $user_id = get_uid_from_memberid($parent_mid);
    }
    
    if ($action == '') {
        return FALSE;
    }
    
    if ($action=='start') {
    //  $query2 = "SELECT * FROM claims_invoices WHERE claim_id=".$claim_id." AND `live`=2;";
        $result2 = db_query('SELECT * FROM {claims_invoices} WHERE claim_id=:a AND live=:b',array(':a'=>$claim_id,':b'=>2));
        $live_invoice_count = $result2->rowCount();
        if ($live_invoice_count==0) {
            drupal_set_message(t('Timer isn\'t started, because of no claim document'), 'error');
            return TRUE;        // ESPECIAL
        }
    }
        
//  $query = "SELECT * FROM claim_timer_trigger WHERE claim_id=$claim_id AND status=1 ORDER BY start_time DESC";
    $query = "SELECT * FROM {claim_timer_trigger} WHERE claim_id=".$claim_id." AND status=1 ORDER BY start_time DESC";
    $result = db_query($query);
    if (!$result) {
        handle_mysql_syntax_error('30701-NCN-ADMIN', $query);
        return FALSE;   ///// END /////
    }
    
    $timerstamp = ncn_admin_get_timer_start_time($claim_id, $action);
    if ($timerstamp == 0) {
        $timerstamp = date('U');
    }
    $error = false;
    $limit = ncn_admin_get_timer_limit_length($claim_id, $action); //24*60*60;  //24 hours
    $timer = true;
    if ( $result->rowCount() == 0 ) {
        if ( $action=='start' || $action=='reset' ) {
            $query = db_query('INSERT INTO {claim_timer_trigger} VALUES(NULL,:a,:b,:c,:d,:e,:f)',
                array(':a'=>$claim_id,':b'=>$timerstamp,':c'=>$limit,':d'=>1,':e'=>0,':f'=>0));
        } else {
            //$error = true;
            $timer = false;
        }
    } else {
        $t_row = $result->fetchAssoc();
        if ( $action=='reset' ) {
            $query = db_query('UPDATE {claim_timer_trigger} SET start_time=:a, limit_time=:b, pause=:c, mail_sent=:d WHERE status=1 AND tid=:e',
                array(':a'=>$timerstamp,':b'=>$limit,':c'=>0,':d'=>0,':e'=>$t_row['tid']));
        } else if ($action=='stop' ) {
            $query = db_query('UPDATE {claim_timer_trigger} SET status=0 WHERE status=1 AND tid=:a',array(':a'=>$t_row['tid']));
        } else {
            $error = true;
        }
    }
    
    // check if action is possible, and output error message
    if ($error) {
        if ($action == 'start') {
            drupal_set_message( t('Timer has been started, already.'), 'error' );
        } else if ($action == 'stop') {
            drupal_set_message( t('Timer isn\'t started, yet.'), 'error' );
        } else if ($action == 'reset') {    
            drupal_set_message( t('Failed to reset timer. Please contact to web administrator.'), 'error' );
        } else {
            handle_unknown_error('10702-NCN-ADMIN');
        }
        return FALSE;   ///// END /////
    }   
    // perform sql.
    
    $result = $query;
    if (!$result) {
        handle_mysql_syntax_error('30702-NCN-ADMIN', $query);
        return FALSE;   ///// END /////
    }
    
    // trigger timer
    
    // log message
    if ($timer) {
        if (!$is_auto) {
            if ($action == 'start') {
                $log_message = "Timer(claim: #$claim_id) has been started by".ncn_amin_get_user_role_and_name();
            } else if ($action == 'stop') {
                $log_message = "Timer(claim: #$claim_id) has been stopped by".ncn_amin_get_user_role_and_name();
            } else if ($action == 'reset') { 
                $log_message = "Timer(claim: #$claim_id) has been reset by".ncn_amin_get_user_role_and_name();
            }
        } else {
            if ($action == 'start') {
                $log_message = "Timer(claim: #$claim_id) has been started by".ncn_amin_get_user_role_and_name();
            } else if ($action == 'stop') {
                $log_message = "Timer(claim: #$claim_id) has been expired.";
            }
        }
        if (isset($log_message)) {
            $claim_workflow = get_claim_workflow_array();
            $log_message .= " (".$claim_workflow[$workflow].")";
        }
    } else {
        $claim_workflow = get_claim_workflow_array();
        $wf_text = $claim_workflow[$workflow];
        $log_message = ncn_amin_get_user_role_and_name()." updated claim workflow to $wf_text.";
    }
    
    // change workflow
//  $query = "UPDATE claims SET workflow='$workflow' WHERE claim_id=$claim_id";
    $result = db_query('UPDATE {claims} SET workflow=:a WHERE claim_id=:b',array(':a'=>$workflow,':b'=>$claim_id));
    if (!$result) {
        handle_mysql_syntax_error('10709-NCN-ADMIN', $query);
        return FALSE;
    }
    
    if (isset($log_message)) {
        drupal_set_message($log_message, 'status');

        ncn_admin_insert_claim_log($claim_id, $timerstamp, $log_message);
    }
    
    // auto mail for All Documents Recieved
    if (!$error) {
//      $query = "SELECT * FROM claims WHERE claim_id=$claim_id";
        $result = db_query('SELECT * FROM {claims} WHERE claim_id=:a',array(':a'=>$claim_id));
        if ($claim=$result->fetchAssoc()) {
            if ($workflow == 'all_doc_recieved') {
                $_admin = user_load(1);
                $admin_mail = $_admin->mail;    //***
                
                $_claim_user = user_load($claim['user_id']);
                $_member = get_member_from_id($_claim_user->profile_memberid);
                if (!empty($_member)) {
                    $_am = user_load($_member['am_uid']);
                    if ($_am) {
                        $am_mail = $_am->mail;      //***
                    }
                }
                
                $ms_list = get_ms_list();
                $qa_list = get_qa_list();
                $am_list = get_account_manager_list();
                $sam_list = get_senior_account_manager_list();
                // automatic mail
                $site_mail =  variable_get('site_mail', ini_get('sendmail_from'));
                
                $part = "All Documents Received";
                $msg = get_mail_body_claim_status_change_notify($claim_id, $part);
                
                $subject = 'All Docs Received ('. ncn_cd($claim_id, 'customer_name'). '/'. $claim['claim_id'].'/'. $_claim_user->profile_firstname .' '. $_claim_user->profile_lastname .'/'.$_claim_user->profile_legalname .')';
                
                if ($user->uid != $user_id) { // if (is_subuser($user)) // mail to parent user (member)
                    $parent_user = user_load($user_id);
                    if (!empty($parent_user->mail)) {
                        drupal_mail( 'ncn_admin', 'claim_status_change_notify', $parent_user->mail, language_default(), array('subject'=>$subject, 'body'=>$msg), $site_mail);
                    }
                }
                
                drupal_mail( 'ncn_admin', 'claim_status_change_notify', $admin_mail, language_default(), array('subject'=>$subject, 'body'=>$msg), $site_mail);
                if (isset($am_mail) && $am_mail) {
                    drupal_mail( 'ncn_admin', 'claim_status_change_notify', $am_mail, language_default(), array('subject'=>$subject, 'body'=>$msg), $site_mail );
                }
                
                foreach($ms_list as $ms) {
                    drupal_mail( 'ncn_admin', 'claim_status_change_notify', $ms['mail'], language_default(), array('subject'=>$subject, 'body'=>$msg), $site_mail );
                }
                
                /*foreach($qa_list as $qa) {
                    drupal_mail( 'ncn_admin', 'claim_status_change_notify', $qa['mail'], language_default(), array('subject'=>$subject, 'body'=>$msg), $site_mail );
                }*/
                
            }
        }
    }
    return TRUE;
}

/*
 *
 */
function render_claim_timer($claim_id, $mode=0) {
    // check if mail should be sent.
    check_claim_timer($claim_id);

//  $query = "SELECT * FROM claim_timer_trigger WHERE claim_id=$claim_id AND status=1 ORDER BY start_time DESC";
    $result = db_query('SELECT * FROM {claim_timer_trigger} WHERE claim_id=:a AND status=1 ORDER BY start_time DESC',
    array(':a'=>$claim_id ));
    if (!$result) {
        handle_mysql_syntax_error('30731-NCN-ADMIN', $query);
        return "";
    }
    if ($result->rowCount() == 0) {
        return "";
    }

    // Timer is in running
    $row = $result->fetchAssoc();
    $t_now = date('U');
    
    if ( $row['pause']== 0 ) {
        $to_time = $row['start_time'] + $row['limit_time'];
        $remained_time = $to_time - $t_now;
        $to_time = ncn_admin_get_due_to_time($row['start_time'], $row['limit_time']);
    } else {
        $remained_time = $row['limit_time'];
    }
    
    $content = "";
    $before = 2*3600;   //2hrs
    $red_class = '';
    if ($remained_time > 0) {
        if ($remained_time < $before) {
            $red_class = "timer_alert";
        }
        // Remained Time
        $r_second = $remained_time % 60;    $remained_time = (int)($remained_time / 60);
        $r_minute = $remained_time % 60;    $remained_time = (int)($remained_time / 60);
        $r_hour   = $remained_time % 24;    $remained_time = (int)($remained_time / 24);
        $r_day    = $remained_time;
        
        if ($mode == 0) {
            $content ='<span class="'.$red_class.'">'.(($r_day)? "$r_day day ":"") . "$r_hour:$r_minute:$r_second</span> &nbsp;remained";
            
            // To Time
            $content .= "<div>";
            if ($row['pause'] != 0) {
                $content .= '( Stopped )';
            } else {
                $content .= '( To '.date('d M Y, H:i:s', $to_time).' )';
            } 
            $content .= "</div>";
        } else {
            $content .='<span class="'.$red_class.'">'.(($r_day)? "$r_day day ":"") . "$r_hour:$r_minute:$r_second";
            if ($row['pause'] != 0) {
                $content .= ' -Stopped- ';
            }
            $content .="</span>";
        }
    }
    
    return $content;
}

function render_claim_workflow($claim_id) {
//  $query = "SELECT * FROM claims WHERE claim_id=$claim_id";
    $result = db_query('SELECT * FROM {claims} WHERE claim_id=:a',array(':a'=>$claim_id));
    if (!$result) {
        handle_mysql_syntax_error('30735-NCN-ADMIN', $query);
        return "";
    } else if ($result->rowCount() == 0 ) {
        drupal_set_message(t('Claim(#%claim_id) doesn\'t exist.', array('%claim_id'=>$claim_id)), 'error');
        return "";
    }

    $row = $result->fetchAssoc();
    $workflow = $row['workflow'];
    if ($workflow == '') {
        $content = '';
    } else {
        $arr_workflow = get_claim_workflow_array();
        $content = $arr_workflow[$workflow] ;
    }
    
    return $content;
}

function get_account_manager_uid_from_claim_id($claim_id) {

//  $query = "SELECT user_id FROM claims WHERE claim_id=$claim_id";
    $result = db_query('SELECT user_id FROM {claims} WHERE claim_id=:a',array(':a'=>$claim_id));
    if (!$result) {
        handle_mysql_syntax_error('30201-NCN-ADMIN', $query);
        return 0;
    } else if ($result->rowCount() == 0) {
        drupal_set_message(t('No member has claim(#%claim_id).', array('%claim_id'=> $claim_id)), 'error');
        return 0;
    }
    $row1 = $result->fetchAssoc();  // get user id of claims
    $user_details = user_load($row1['user_id']);
    
    // get account manager
//  $query = "SELECT * FROM `member_id_pool` WHERE member_id=\"".AddSlashes($user_details->profile_memberid)."\";";
    $result = db_query('SELECT * FROM {member_id_pool} WHERE member_id=:a',array(':a'=>$user_details->profile_memberid));
    if (!$result) {
        handle_mysql_syntax_error('30202-NCN-ADMIN', $query);
        return 0;
    } else if ($result->rowCount() == 0 ) {
        if ($row1['user_id'] != 1) {
            drupal_set_message(t('User(%uid) isn\'t assigned to anyone.', array('%uid'=> $row1['user_id'])), 'error');
        }
        return 0;
    }

    $row1 = $result->fetchAssoc();  // we are grabbing distributor id from memebr id
    return $row1['am_uid'];
    //***************
    
//  $query = "SELECT * FROM  `users_parent` WHERE uid=".$row1['owner']; // get account manager from distributor
    $result = db_query('SELECT * FROM  {users_parent} WHERE uid=:a',array(':a'=>$row1['owner']));
    if (!$result) {
        handle_mysql_syntax_error('30203-NCN-ADMIN', $query);
        return 0;
    } else if ($result->rowCount() == 0) {
        if ($row1['owner'] != 1) {
            drupal_set_message(t('User(%uid) isn\'t assigned to any account manager.', array('%uid'=> $row1['owner'])), 'error');
        }
        return 0;
    }
    $row1 = $result->fetchAssoc();  // we are grabbing account manager id from distributor id (account managers "own" distributors)
    
    return $row1['parent'];     // load the account manager profile
}

function check_claim_timer($claim_id) {

//  $query = "SELECT * FROM claim_timer_trigger WHERE claim_id=$claim_id AND status=1 ORDER BY start_time DESC";
    $result = db_query('SELECT * FROM {claim_timer_trigger} WHERE claim_id=:a AND status=1 ORDER BY start_time DESC',
    array(':a'=>$claim_id ));
    if (!$result) {
        handle_mysql_syntax_error('30301-NCN-ADMIN', $query);
        return;
    }
    if ($result->rowCount() == 0) {
        return;
    }

    $row = $result->fetchAssoc();
    
    $t_now = date('U');
    if ($row['pause']==0) {
        $to_time = $row['start_time'] + $row['limit_time'];
        $remained_time = $to_time - $t_now;
    } else {
        $remained_time = $row['limit_time'];
    }
    
    
    $account_manager_uid = get_account_manager_uid_from_claim_id($claim_id);
    $account_manager_email = '';
    if ($account_manager_uid) {
        $account_manager = user_load($account_manager_uid);
        if ($account_manager) {
            $account_manager_email = $account_manager->mail;    // grab the account manager email from their profile 
        }
    }
    
    // expired
    if ($remained_time <= 0) {
        // * if (update_timer_trigger($claim_id, 'completed_status', 'stop', true)) {
//      $query = "SELECT * FROM claim_timer_trigger WHERE claim_id=$claim_id AND status=1 ORDER BY start_time DESC";
        $result = db_query('SELECT * FROM {claim_timer_trigger} WHERE claim_id=:a AND status=1 ORDER BY start_time DESC',
        array(':a'=>$claim_id));
        $t_row = $result->fetchAssoc();
        
    //  $query = "UPDATE claim_timer_trigger SET status=0 WHERE status=1 AND tid=".$t_row['tid'];
        if (db_query('UPDATE {claim_timer_trigger} SET status=0 WHERE status=1 AND tid=:a',array(':a'=>$t_row['tid']))) {
            ncn_admin_insert_claim_log($claim_id, date('U'), "Timer(claim: #$claim_id) has been expired.");
            if ($account_manager_email) {
                $params = array(
                    '!claim_id' => $claim_id
                );
                drupal_mail('ncn_admin', 'timer_expired', $account_manager_email, language_default(), $params);
                return;
            }
        }
    } else if($row['mail_sent'] == 0) {     // mail before 2 hours of expired
        $before = 2*3600;   //2hrs  
        if ( $remained_time < $before ) {
            if ($account_manager_email) {
                $params = array(
                    '!claim_id' => $claim_id
                );
                drupal_mail('ncn_admin', 'timer_alert', $account_manager_email, language_default(), $params);
            }
        //  $query = "UPDATE claim_timer_trigger SET mail_sent=1 WHERE tid=".$row['tid'];
            $result = db_query('UPDATE {claim_timer_trigger} SET mail_sent=1 WHERE tid=:a',array(':a'=>$row['tid']));
        }
    }
    
    if ($row['pause']==1) {
        if ($row['start_time'] <= date('U')) {
        //  $query = "UPDATE claim_timer_trigger SET pause=0 WHERE tid=".$row['tid'];
            $result = db_query('UPDATE {claim_timer_trigger} SET pause=0 WHERE tid=:a',array(':a'=>$row['tid']));
        }
    }
    
    check_timer_work_day($row['tid']);
}

/*
 * Check Timer Pause, out of Work day.
 Friday 5:30 PM ~ Monday 8:30 AM
 */
function check_timer_work_day($tid) {
//  $query = "SELECT * FROM claim_timer_trigger WHERE tid=$tid";
    $result = db_query('SELECT * FROM {claim_timer_trigger} WHERE tid=:a',array(':a'=>$tid));
    if (!$result) {
        //handle_mysql_syntax_error('30305-NCN-ADMIN', $query);
        return;
    }
    if ($result->rowCount() == 0) {
        return;
    }

    $row = $result->fetchAssoc();
    if ($row['pause'] == 0) {
        $today = getdate();
        $resume_time = 0;
        $flag = false;
        /*if ( ( $today['wday'] == 5 && ($today['hours']>17 || ($today['hours']==17 && $today['minutes']>=30))) ||
            $today['wday'] == 6 || $today['wday'] == 0 ) {
            $flag = true;
            $resume_time = strtotime("next Monday");
            $resume_time = strtotime("+8 hours 30 minutes", $resume_time);
        } else if ( $today['wday'] == 1 && ($today['hours']<8 || ($today['hours']==8 && $today['minutes']<30)) ) {
            $flag = true;
            $resume_time = strtotime("Monday this week");
            $resume_time = strtotime("+8 hours 30 minutes", $resume_time);
        }*/
        
        if ( $today['wday'] == 6 || $today['wday'] == 0 ) {
            $flag = true;
            $resume_time = strtotime("next Monday");
            $resume_time = strtotime("+8 hours", $resume_time);
        } else if ( $today['wday'] == 1 && $today['hours']<8 ) {
            $flag = true;
            $resume_time = strtotime("Monday this week");
            $resume_time = strtotime("+8 hours", $resume_time);
        }
        
        if ($flag) {
            $limit_time = $row['limit_time'] - (date('U')-$row['start_time']);
            if ($limit_time < 0) {
                $limit_time = 0;
            }
        //  $query = "UPDATE claim_timer_trigger SET pause=1, start_time=$resume_time, limit_time=$limit_time WHERE tid=$tid";
            $result = db_query('UPDATE {claim_timer_trigger} SET pause=1, start_time=:a, limit_time=:b WHERE tid=:c',
                array(':a'=>$resume_time,':b'=>$limit_time,':c'=>$tid));
        }
    }
}

