<?php

function ncn_ceportal_set_available_claim_handler($claim_id) {
    $val = $_POST['ce_is_available_claim'];

    $result = ncn_ceportal_set_available_claim($claim_id, $val);
    if ($result !== "") {
        if ($result) {
            if ($val) {
                $log_msg = "Claim(#$claim_id) is available for Claim Examiners.";
                ncn_admin_insert_claim_log($claim_id, date('U'), $log_msg);
                drupal_set_message($log_msg);
                $mail_to = variable_get('ce_ava_claim_note_mail_to', 'claimsexaminer@netclaimsnow.com');
                
                // Notification mail
                $params = array(
                    '!claim_id'=>$claim_id
                );
//              drupal_mail('ncn_ceportal', 'ava_claim_notification', $mail_to, language_default(), $params); //Nikhil : need to check
            } else {
                $log_msg = "Claim(#$claim_id) isn't available for Claim Examiners.";
                ncn_admin_insert_claim_log($claim_id, date('U'), $log_msg);
                drupal_set_message($log_msg);               
            }
        } else {
            drupal_set_message("Failed to set the claim, for Claim Examiners", 'error');
        }
    }
}

function ncn_ceportal_set_available_claim($claim_id, $val=1) {
    $ce_claim = ncn_ceportal_get_ncn_ce_claim($claim_id);
    
    if ( empty($ce_claim) ) {
        if ($val) {
            $result = db_query("INSERT INTO {ncn_ce_claims} (claim_id, is_available, assigned, ce_uid, status) VALUES(:a, :b, :c, :d, :e)",
                array(":a"=>$claim_id,":b"=>$val,":c"=>0,":d"=>0,":e"=>''));
        }
    } else {
        if ($ce_claim['is_available'] != $val) {
            $result = db_query("UPDATE {ncn_ce_claims} SET is_available=:a WHERE claim_id=:b",
                array(":a"=>$val,":b"=>$claim_id));
        }
    }
    
    if (isset($result)) {
        return $result;
    }
    
    return "";
}

/**
 * Action of CE Claim
 */
function ncn_ceportal_ava_claim($ce_claim_id, $op, $ce_uid=0) {
global $user;
    $ce_claim = ncn_ceportal_get_ncn_ce_claim($ce_claim_id);
    if (!empty($ce_claim)) {
        if ($ce_uid == 0) {
            $ce_uid = $user->uid;
        }
        
        $timestamp = date('U');
        $ce_user = user_load($ce_uid);
        
        switch ($op) {
        case 'accept':
            if (!ncn_ceportal_check_claim_available($ce_claim, $ce_claim_id)) { $ret_val=FALSE; break; }
            if ( ncn_ceportal_get_active_claims_total_count($ce_uid) >= 2 ) { 
                drupal_set_message(t('You have already worked on more than 2 claims.'), 'error');
                $ret_val=FALSE; 
                break; 
            }
            
            db_query("DELETE FROM {ncn_ce_claim_accept} WHERE claim_id=:a AND ce_uid=:b",
                array(":a"=>$ce_claim_id,":b"=>$ce_uid));
            $result = db_query("INSERT INTO {ncn_ce_claim_accept}(claim_id, ce_uid, `timestamp`) VALUES(:a, :b, :c)",
                array(":a"=>$ce_claim_id,":b"=>$ce_uid,":c"=>$timestamp));
            if ($result) {
                drupal_set_message(t("Accepted to claim#!claim_id, successfully.", array('!claim_id'=>$ce_claim_id)));
                // claim_log
                $log_msg = t("Claim Examiner(!ce_name) accepted to claim#!claim_id.", array('!ce_name'=>($ce_user->profile_firstname.' '.$ce_user->profile_lastname), '!claim_id'=>$ce_claim_id));
                ncn_admin_insert_claim_log($ce_claim_id, $timestamp, $log_msg);
                
                // mail 
                $mail_from = $ce_user->mail;
                $_admin = user_load(1);
                $mail_to = $_admin->mail;
                if ($mail_from) {
                    $params = array(
                        '!claim_id'=>$ce_claim_id, 
                        '!ce_name'=>($ce_user->profile_firstname.' '.$ce_user->profile_lastname), 
                    );
                    drupal_mail('ncn_ceportal', 'accept_ava_claim', $mail_to, language_default(), $params, $mail_from);
                }
            }
            
            // Auto Assign Mode: On
            if (variable_get('ce_auto_assign', FALSE)) {
                ncn_ceportal_ava_claim($ce_claim_id, 'assign', $ce_uid);
            }
            break;
        
        case 'assign':
            if (!ncn_ceportal_check_claim_available($ce_claim, $ce_claim_id)) { $ret_val=FALSE; break; }
            if ( ncn_ceportal_get_active_claims_total_count($ce_uid) >= 2 ) { 
                drupal_set_message(t('You have already worked on more than 2 claims.'), 'error');
                $ret_val=FALSE; 
                break; 
            }
            
            $assigned_status = 1;   // assigned
            if ($ce_claim['status'] != '') { $assigned_status = 2; }    // re-assigned
            $result=db_query("UPDATE {ncn_ce_claims} SET assigned=:a, ce_uid=:b, status=:c WHERE claim_id=:d",
                array(":a"=>$assigned_status,":b"=>$ce_uid,":c"=>'active',":d"=>$ce_claim_id));
            
            if ($result) {
                // claim_log
                $log_msg = t("Assigned claim#!claim_id to Claim Examiner(!ce_name).", array('!claim_id'=>$ce_claim_id, '!ce_name'=>($ce_user->profile_firstname.' '.$ce_user->profile_lastname)));
                ncn_admin_insert_claim_log($ce_claim_id, $timestamp, $log_msg);
                
                $log_msg = t("Assigned claim#!claim_id to Claim Examiner(!ce_name).", array('!claim_id'=>$ce_claim_id, '!ce_name'=>($ce_user->profile_firstname.' '.$ce_user->profile_lastname)));
                drupal_set_message($log_msg);
                // mail
                $mail_to = $ce_user->mail;
                if ($mail_to) {
                    $params = array(
                        '!claim_id'=>$ce_claim_id, 
                        '!ce_name'=>($ce_user->profile_firstname.' '.$ce_user->profile_lastname), 
                    );
                    drupal_mail('ncn_ceportal', 'assign_ava_claim', $mail_to, language_default(), $params);
                }
                
                $pm_subject = t("Assigned claim#!claim_id to Claim Examiner(!ce_name).", array('!claim_id'=>$ce_claim_id, '!ce_name'=>($ce_user->profile_firstname.' '.$ce_user->profile_lastname)));
                $pm_msg = t("Admin has assigned claim to you. 
                You may now log into your account and upload 2 files (ESX and Invoicd PDF files).
                You need to complete this claim for 4 hours.");
                pm_send(0, $ce_uid, $pm_subject, $pm_msg, $ce_claim_id, 0, 'no mail');
                
                $period = 4*3600; // 4hrs
                ncn_ceportal_delete_all_accepted_ce_list($ce_claim_id);
                $result = ncn_ceportal_ce_claim_timer_action($ce_claim_id, 'start', array( 'ts_start'=>$timestamp, 'period'=>$period, 'ce_uid'=>$ce_uid) );
            } else {
                $ret_val = FALSE;
            }
            if ($result) {
            } else {
                $ret_val = FALSE;
            }
            break;
            
        case 'miss':
            $result = ncn_ceportal_ce_claim_timer_action($ce_claim_id, 'stop', array( 'ts_end'=>$timestamp, 'status'=>0 ));
            if ($result) {
                $assigned_status = 0;   // not assigned
                $result=db_query("UPDATE {ncn_ce_claims} SET assigned=:a, ce_uid=:b, status=:c WHERE claim_id=:d",
                                 array(":a"=>$assigned_status,":b"=>0,":c"=>'miss',":d"=>$ce_claim_id));
            } else {
                $ret_val = FALSE;
            }
            if ($result) {
                // claim log
                $log_msg = t("Claim Examiner(!ce_name) missed claim#!claim_id.", array('!ce_name'=>($ce_user->profile_firstname.' '.$ce_user->profile_lastname), '!claim_id'=>$ce_claim_id ));
                ncn_admin_insert_claim_log($ce_claim_id, $timestamp, $log_msg);
                
                drupal_set_message($log_msg, 'warning');
            } else {
                $ret_val = FALSE;
            }
            
            break;
            
        case 'complete':
            if (!ncn_ceportal_validate_complete_active_claim($ce_claim_id)) {
                drupal_set_message(t("You can\'t complete claim(#!claim_id). Please make sure that you have uploaded essential files", array('!claim_id'=>$ce_claim_id)));
                $ret_val = FALSE; break;
            } 
            $result = ncn_ceportal_ce_claim_timer_action($ce_claim_id, 'stop', array( 'ts_end'=>$timestamp, 'status'=>2 ));
            if ($result) {
                db_query("UPDATE {ncn_ce_claims} SET status=:a WHERE claim_id=:b",
                    array(":a"=>'archive',":b"=>$ce_claim_id));
            } else {
                $ret_val = FALSE;
            }
            if ($result) {
                $working_time = ncn_ceportal_get_working_time($ce_claim_id, $ce_uid);
                $w_second = $working_time % 60; $working_time = (int)($working_time/60);
                $w_minute = $working_time % 60; $w_hour = (int)($working_time/60);
                $str_working_time = sprintf("%02d:%02d:%02d", $w_hour, $w_minute, $w_second);
                // claim log
                $log_msg = t("Claim Examiner(!ce_name) completed claim#!claim_id, successfully. (working time - !working_time)", 
                                            array('!ce_name'=>($ce_user->profile_firstname.' '.$ce_user->profile_lastname), '!claim_id'=>$ce_claim_id, '!working_time'=>$str_working_time));
                ncn_admin_insert_claim_log($ce_claim_id, $timestamp, $log_msg); 
                drupal_set_message($log_msg);
            
                // mailing to Admin and Director of Quality Assuarance
                ncn_ce_portal_send_mail_when_ce_complete($ce_claim_id);
                
                // paying
                ncn_ceportal_pay_ce_claim($ce_claim_id, $ce_user->uid);
            } else {
                $ret_val = FALSE;
            }
            break;
        }
    } else {
        $ret_val = FALSE;
    }
    
    if (isset($_REQUEST['destination'])) {
        $_url_dest = $_REQUEST['destination'];
        if ($_url_dest[0] == '/') {
            $_url_dest = substr($_url_dest, 1);
        }
        drupal_goto($_url_dest);
    }
    
    return isset($ret_val)? $ret_val : TRUE;
}

function ncn_ceportal_check_claim_available($ce_claim, $ce_claim_id) {
    $ret_val = TRUE;
    if ($ce_claim['status'] == 'active') {
        drupal_set_message(t("Claim#@claim_id has been assigned.", array('@claim_id'=>$ce_claim_id)),'error');
        $ret_val = FALSE;
    } else if($ce_claim['status'] == 'archive') {
        drupal_set_message(t("Claim#@claim_id has been assigned.", array('@claim_id'=>$ce_claim_id)),'error');
        $ret_val = FALSE;
    }
    
    return $ret_val;
}

/** 
 * Menu Handler ( CALLBACK )
 * Upload CE Claim file
 */
function ncn_ceportal_ajax_upload_claim_file($claim_id, $claim_file_type) {
global $user;

    //$source_name = $_REQUEST['file_name'];
    $source_name = "cefile";
    $r_json = array();
    $r_json['flag'] = 'success';
    
    /***************************************************************************/
    $claim_file_title = ncn_admin_get_claim_file_title($claim_file_type);
    
    $tmp_file = file_save_upload($source_name);

    $claim_dir = "public://invoices/$claim_id";

    if (!is_dir(drupal_realpath($claim_dir))) {
        mkdir(drupal_realpath($claim_dir), 0755);
    }
    $file = file_copy($tmp_file, $claim_dir, FILE_EXISTS_RENAME);
    file_delete($tmp_file);
    // add invoice to database
    $res = true;
    if ($file)
    {
        $file->filepath = str_replace('public://', 'sites/default/files/', $file->uri);
        $result = db_query("SELECT * FROM {claims_invoices} WHERE claim_id=:a AND `live`=:b ORDER BY revision DESC",
            array(":a"=>$claim_id,":b"=>$claim_file_type));
        if (!($row=$result->fetchAssoc()) ) {
            $row = array();
        }
        if (empty($row))
        {   
            $revision = 0;
            $res = db_query('INSERT INTO {claims_invoices} VALUES(NULL,:a,:b,:c,:d,:e,:f,:g)',
                                array(':a'=>$claim_id,
                                      ':b'=>$revision,
                                      ':c'=>$file->filename,
                                      ':d'=>$file->filepath,
                                      ':e'=>$file->filemime,
                                      ':f'=>date('U'),
                                      ':g'=>$claim_file_type));
        }
        else
        {
            $revision = 0;
            file_delete($row['filepath']);
            $res = db_query('UPDATE {claims_invoices} SET filename=:a, filepath=:b, filemime=:c, timestamp=:d 
                                        WHERE claim_id=:e AND live=:f',
                               array(':a'=>$file->filename,
                                     ':b'=>$file->filepath,
                                     ':c'=>$file->filemime,
                                     ':d'=>date('U'),
                                     ':e'=>$claim_id,
                                     ':f'=>$claim_file_type));
        }
    } 
    
    if ($file && $res) {
        $r_json['message'] = t("Upload a file, successfully (#!claim_id - !file_name)", array('!claim_id'=>$claim_id, '!file_name'=>$claim_file_title));
        drupal_set_message($r_json['message']);
        
        $user = user_load($user->uid);
        if (is_claim_examiner($user)) {
            $who = t("Claim Examiner( !ce_name ) ", array('!ce_name'=>$user->profile_firstname.' '.$user->profile_lastname));
        } else {
            $who = t("!_name ", array('!_name'=>$user->name));
        }
        // claim_log
        $log_msg = $who . t("uploaded a file (#!claim_id - !file_name)", array('!claim_id'=>$claim_id, '!file_name'=>$claim_file_title));
        ncn_admin_insert_claim_log($claim_id, date('U'), $log_msg);
        
    } else {
        $r_json['flag'] = 'error';
        $r_json['err_code'] = '001';
        $r_json['message'] = t("Failed to upload a file (#!claim_id - !file_name)", array('!claim_id'=>$claim_id, '!file_name'=>$claim_file_title));
    }
    /***************************************************************************/
    echo json_encode($r_json);
    exit;
}

?>
