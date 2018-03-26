<?php
//------------------------------------------------------------------------------
function make_monthly_payment($user, $product_nid, &$pid, $mode="standard")
{
    GLOBAL $auto_payment_messages;    
    
    // check for free user
    if (isset($user->profile_credit_card_number) && $user->profile_credit_card_number == "5424000000000015")
    {
        $message = "WARNING: Ignoring monthly membership fee for '".$user->name."' (".$user->mail.") because user is FREE.";
        watchdog('cron', $message);
        return;
    }

    $member_id = $user->profile_memberid;
    // get payment profile information
    $member_cim = ncn_get_member_cim($member_id);

    if (!$member_cim) {
        $message = "ERROR: Failed to charge monthly membership fee for '".$user->name."' (".$user->mail.") because no practicepay profile found.";
        $auto_payment_messages .= $message."\n\n";
        watchdog('cron', $message);
        return;
    }
    $customerProfileId = $member_cim['customerProfileId'];
    $customerPaymentProfileId = $member_cim['customerPaymentProfileId'];
    if (intval($customerProfileId) == 0 || intval($customerPaymentProfileId) == 0)
    {
        $message = "WARNING: Ignoring monthly membership fee for '".$user->name."' (".$user->mail.") because user is FREE.";
        watchdog('cron', $message);
        return;
    }

    $node = node_load($product_nid);
    if (isset($node->data) && !is_array($node->data)) {
      $node->data = unserialize($node->data);
    }
    
    if ($node->sell_price != 0) {
        $title = (string)$node->title;
        // $title .= " for ".$user->profile_firstname." ".$user->profile_lastname." (".$user->profile_legalname.")";

        $response = sendNcnPaymentRequest($customerProfileId, $customerPaymentProfileId, 
                          array(
                            'price'         => $node->sell_price, 
                            'shipping_name' => $title, 
                            'shipping_desc' => $title, 
                            'item_id'       => $product_nid, 
                            'item_name'     => $title, 
                            'item_desc'     => $title, 
                          ));

        if (!$response) {
            return 0;
        }

        $auth_successful = true;
        if ("Ok" == $response->messages->resultCode)
        {
            $auth_successful = true;
        }
        else
        {
            $auth_successful = false;
            $response_reason_text = preg_replace("/[^a-zA-Z0-9\s]/", "", $response->messages->message->text);
            
            $params = array(
                'member_name' => $user->profile_firstname." ".$user->profile_lastname,
                'activation' => true
            );
            if ( strcasecmp($response_reason_text, 'This transaction has been declined.') == 0 ) {
                if ( $mode=="standard" ) {
                    $message = credit_card_declined_mail_template($params);
                    drupal_mail('ncn_auto_payments', 'credit_card_declined', $user->mail, language_default(), array('subject'=>t('Credit Card Declined'), 'body'=>$message));
                } else {
                    $message = "ERROR: ".$response_reason_text;
                    $auto_payment_messages .= $message."\n\n";
                }
                
            } else if ( strcasecmp($response_reason_text, 'The credit card has expired.') == 0 ) {
                if ( $mode=="standard" ) {
                    $message = credit_card_expired_mail_template($params);
                    drupal_mail('ncn_auto_payments', 'credit_card_expired', $user->mail, language_default(), array('subject'=>t('Credit Card Expired'), 'body'=>$message));
                } else {
                    $message = "ERROR: ".$response_reason_text;
                    $auto_payment_messages .= $message."\n\n";
                }
            } else {
                $message = "ERROR: Attempted to charge monthly membership fee for '".$user->profile_firstname." ".$user->profile_lastname."' (".$user->mail.") failed because ".$response_reason_text;
                $auto_payment_messages .= $message."\n\n";
            }
            //watchdog('cron', $message);
            
            if ( $mode=="standard" ) {
                // Log to Declined Report
                ncn_auto_payments_insert_declined_transaction($product_nid, $user->uid, '0', date('U'));
            }
        }
    } else {
        $auth_successful = true;
    }
    //*** payment log ***//
    $payment_success = ($auth_successful)? 1:0;
    $pid = ncn_report_payment_log($product_nid, $payment_success, $user);
    
    return $payment_success;
}

function make_monthly_payment_from_memberid($member_id, $product_nid, &$pid, $mode="standard")
{
GLOBAL $auto_payment_messages;
    
    $member = get_member_from_id($member_id);
    if (empty($member)) {
        return 0;
    }

    //watchdog('cron_log', '!member_id', array('!member_id'=>$member_id));
    // get payment profile information
    $member_cim = ncn_get_member_cim($member_id);

    if (!$member_cim) {
        $message = "ERROR: Failed to charge monthly membership fee for '".$member['first_name'].' '.$member['last_name']."' (".$member['contactemail'].") because no practicepay profile found.";
        $auto_payment_messages .= $message."\n\n";
        watchdog('cron', $message);
        return 0;
    }
    
    $customerProfileId = $member_cim['customerProfileId'];
    $customerPaymentProfileId = $member_cim['customerPaymentProfileId'];
    if (intval($customerProfileId) == 0 || intval($customerPaymentProfileId) == 0) {
        $message = "WARNING: Ignoring monthly membership fee for '".$member['first_name'].' '.$member['last_name']."' (".$member['contactemail'].") because user is FREE.";
        watchdog('cron', $message);
        return 0;
    }

    //watchdog('cron_log', 'customerProfileId: !customerProfileId, customerPaymentProfileId: !customerPaymentProfileId', array('!customerProfileId'=>$customerProfileId, '!customerPaymentProfileId'=>$customerPaymentProfileId));
    // build xml query
    $node = node_load($product_nid);
    if (isset($node->data) && !is_array($node->data)) {
      $node->data = unserialize($node->data);
    }

    if ($node->sell_price != 0) {
        $title = (string)$node->title;
        //  $title .= " for ".$user->profile_firstname." ".$user->profile_lastname." (".$user->profile_legalname.")";

        // attempt payment
        $response = sendNcnPaymentRequest($customerProfileId, $customerPaymentProfileId, 
                      array(
                        'price'         => $node->sell_price, 
                        'shipping_name' => $title, 
                        'shipping_desc' => $title, 
                        'item_id'       => $product_nid, 
                        'item_name'     => $title, 
                        'item_desc'     => $title, 
                      ));

        if (!$response) {
            return 0;
        }

        $auth_successful = true;
        if ("Ok" == $response->messages->resultCode)
        {
            $auth_successful = true;
            // echo "success: ".$user->name."<br>";
        }
        else
        {
            $auth_successful = false;
            $response_reason_text = preg_replace("/[^a-zA-Z0-9\s]/", "", $response->messages->message->text);
            
            $params = array(
                'member_name' => $member['first_name'].' '.$member['last_name'],
                'activation' => false
            );
            if ( strcasecmp($response_reason_text, 'This transaction has been declined.') == 0 ) {
                if ( $mode=="standard" ) {
                    $message = credit_card_declined_mail_template($params);
                    drupal_mail('ncn_auto_payments', 'credit_card_declined', $member['contactemail'], language_default(), array('subject'=>t('Credit Card Declined'), 'body'=>$message));
                } else {
                    $message = "ERROR: ".$response_reason_text;
                    $auto_payment_messages .= $message."\n\n";
                }
                
            } else if ( strcasecmp($response_reason_text, 'The credit card has expired.') == 0 ) {
                if ( $mode=="standard" ) {
                    $message = credit_card_expired_mail_template($params);
                    drupal_mail('ncn_auto_payments', 'credit_card_expired', $member['contactemail'], language_default(), array('subject'=>t('Credit Card Expired'), 'body'=>$message));
                } else {
                    $message = "ERROR: ".$response_reason_text;
                    $auto_payment_messages .= $message."\n\n";
                }
            } else {
                $message = "ERROR: Attempted to charge monthly membership fee for '".$member['first_name'].' '.$member['last_name']."' (".$member['contactemail'].") failed because ".$response_reason_text;
                $auto_payment_messages .= $message."\n\n";
            }
            //watchdog('cron', $message);
            if ( $mode=="standard" ) {
                // Log to Declined Report
                ncn_auto_payments_insert_declined_transaction($product_nid, 0, $member_id, date('U'));
            }
        }
    } else {
        $auth_successful = true;
    }   
    //*** payment log ***//
    $payment_success = ($auth_successful)? 1:0;
    $pid = ncn_report_payment_log($product_nid, $payment_success, null, $member_id);
    
    return $payment_success;
}

//------------------------------------------------------------------------------
// calculated when the next month is
function calculate_next_month($last, $charge_day)
{
    $last_d = getdate($last);
    $y = $last_d['year'];
    $m = $last_d['month'];
    $last_d_first = strtotime("1 $m $y");
    $next_month = strtotime('+1 month', $last_d_first);
    
    $next_d = getdate($next_month);
    $y = $next_d['year'];
    $m = $next_d['month'];
    $d = $charge_day;
    if ( $d > date('t', $next_month)) {
        $d = date('t', $next_month);
    }
    
    $next_payment = strtotime("$d $m $y");
    if ($next_payment<date('U')) {
        return calculate_next_month($next_payment, $charge_day);
    }
    return strtotime("$d $m $y");
}

/**
 * Check Credit Card Expiration of Users & Unactivated Members
 * Cron Job (Monthly) - Call this function at the first day of every month
 */
function ncn_auto_payments_check_expiration() {
    $query = "SELECT * FROM {users} as u 
                LEFT JOIN {users_roles} AS ur ON (ur.uid = u.uid) 
                WHERE ur.rid=:rid AND u.uid!=69";
    $result = db_query($query, array(':rid'=>USER_ROLE_MEMBER));

    foreach($result as $u)
    {
        $u = (array)$u;
        $_user = user_load($u['uid']);
        if (!(isset($_user->profile_memberid)&&is_member($_user->profile_memberid))) { continue; }
        if ($u['status'] == 0 && $_user->profile_fblocked != 'first') {
            continue;   // blocked
        }

        ncn_auto_payments_check_expiration_member_cc($_user->profile_memberid, 
                array('member_name' => $_user->profile_firstname." ".$_user->profile_lastname, 
                      'activation'  => true, 
                      'mail'        => $_user->mail));
    }
    
    $result = db_query("SELECT * FROM {member_id_pool} WHERE used=0 AND status=1");
    foreach($result as $m)
    {
        $m = (array)$m;
        ncn_auto_payments_check_expiration_member_cc($m['member_id'], 
                array('member_name' => $m['first_name'].' '.$m['last_name'], 
                      'activation'  => false, 
                      'mail'        => $m['contactemail']));
    }
}

/**
 * Check Credit Card Expiration of Individual Member
 *
 * @param $data : member_name, activation, mail
 */
function ncn_auto_payments_check_expiration_member_cc($member_id, $data=array()) {
    $customerProfileId = 0; $customerPaymentProfileId = 0;
    ncn_auto_payments_get_member_cim($member_id, $customerProfileId, $customerPaymentProfileId);
    
    if (!$customerProfileId) { return true; }   // FREE CREDIT CARD
    $expired = ncn_auto_payments_get_cc_expiration($customerProfileId, $customerPaymentProfileId);
    if ($expired) {
        $params = array(
            'member_name' => $data['member_name'],
            'activation'  => $data['activation']
        );
        $message = credit_card_expired_mail_template($params);
        drupal_mail('ncn_auto_payments', 'credit_card_expired', $data['mail'], language_default(), array('subject'=>t('Credit Card Expired'), 'body'=>$message));
        return true;
    }
    return false;
}

function ncn_auto_payments_get_member_cim($member_id, &$customerProfileId, &$customerPaymentProfileId) {
    $customerProfileId = 0;
    $customerPaymentProfileId = 0;
    
    $result = db_query("SELECT * FROM {member_cim} WHERE member_id=:d",array(":d"=>$member_id));
    if ($row = $result->fetchAssoc()) {
        $customerProfileId = intval($row['customerProfileId']);
        $customerPaymentProfileId = intval($row['customerPaymentProfileId']);
    }
    return;
}

function make_payment($user, $product_nid, $price, &$pid, $mode="standard")
{
GLOBAL $auto_payment_messages;

    // check for free user
    if ($user->profile_credit_card_number == "5424000000000015")
    {
        $message = "WARNING: Ignoring payment for '".$user->name."' (".$user->mail.") because user is FREE.";
        watchdog('cron', $message);
        return 'FREE';
    }

    // get payment profile information
    $member_cim = ncn_get_member_cim($user->profile_memberid);

    if (!$member_cim) {
        $message = "ERROR: Failed to charge for '".$user->name."' (".$user->mail.") because no practicepay profile found.";
        $auto_payment_messages .= $message."\n\n";
        watchdog('cron', $message);
        return FALSE;
    }


    $customerProfileId = $member_cim['customerProfileId'];
    $customerPaymentProfileId = $member_cim['customerPaymentProfileId'];
    if (intval($customerProfileId) == 0 || intval($customerPaymentProfileId) == 0) {
        $message = "WARNING: Ignoring payment for '".$user->name."' (".$user->mail.") because user is FREE.";
        watchdog('cron', $message);
        return FALSE;
    }

    // build xml query
    //$node = node_load(32);
    $node = node_load($product_nid);
    if ($node) {
        if (isset($node->data) && !is_array($node->data)) {
          $node->data = unserialize($node->data);
        }
        if ($price == 0) {
            $price = $node->sell_price;
        }
    }
    
    if ($price != 0) {
        $title = (string)$node->title;
        
        // attempt payment
        $response = sendNcnPaymentRequest($customerProfileId, $customerPaymentProfileId, 
                      array(
                        'price'         => $price, 
                        'shipping_name' => $title, 
                        'shipping_desc' => $title, 
                        'item_id'       => $product_nid, 
                        'item_name'     => $title, 
                        'item_desc'     => $title, 
                      ));
        $auth_successful = true;
        if ("Ok" == $response->messages->resultCode)
        {
            $auth_successful = true;
        }
        else
        {
            $auth_successful = false;
            
            $response_reason_text = preg_replace("/[^a-zA-Z0-9\s]/", "", $response->messages->message->text);
            
            $params = array(
                'member_name' => $user->profile_firstname." ".$user->profile_lastname,
                'activation' => true
            );
            if ( strcasecmp($response_reason_text, 'This transaction has been declined.') == 0 ) {
                if ( $mode=="standard" ) {
                    $message = credit_card_declined_mail_template($params);
                    drupal_mail('ncn_auto_payments', 'credit_card_declined', $user->mail, language_default(), array('subject'=>t('Credit Card Declined'), 'body'=>$message));
                    
                    // Log to Declined Report
                    if ($product_nid) {
                        ncn_auto_payments_insert_declined_transaction($product_nid, $user->uid, '0', date('U'), $price);
                    }
                } else {
                    $message = "ERROR: ".$response_reason_text;
                    $auto_payment_messages .= $message."\n\n";
                }
                
            } else if ( strcasecmp($response_reason_text, 'The credit card has expired.') == 0 ) {
                if ( $mode=="standard" ) {
                    $message = credit_card_expired_mail_template($params);
                    drupal_mail('ncn_auto_payments', 'credit_card_expired', $user->mail, language_default(), array('subject'=>t('Credit Card Expired'), 'body'=>$message));
                } else {
                    $message = "ERROR: ".$response_reason_text;
                    $auto_payment_messages .= $message."\n\n";
                }
            } else {
                $message = "ERROR: Attempted to charge for '".$user->profile_firstname." ".$user->profile_lastname."' (".$user->mail.") failed because ".$response_reason_text;
                $auto_payment_messages .= $message."\n\n";
            }

        }
    } else {
        $auth_successful = true;
    }
    //*** payment log ***//
    $payment_success = ($auth_successful)? 1:0;
    $pid = ncn_report_payment_log($product_nid, $payment_success, $user, 0, 0, 0, 0, $price);
    
    return ($payment_success)? TRUE:FALSE;
}

function make_payment_from_memberid($member_id, $product_nid, $price, &$pid, $mode="standard")
{
GLOBAL $auto_payment_messages;

    $member = get_member_from_id($member_id);
    if (empty($member)) {
        return FALSE;
    }

    //watchdog('cron_log', '!member_id', array('!member_id'=>$member_id));
    // get payment profile information
    $member_cim = ncn_get_member_cim($member_id);
    if (!$member_cim) {
        $message = "ERROR: Failed to charge monthly membership fee for '".$member['first_name'].' '.$member['last_name']."' (".$member['contactemail'].") because no practicepay profile found.";
        $auto_payment_messages .= $message."\n\n";
        watchdog('cron', $message);
        return FALSE;
    }

    $customerProfileId = $member_cim['customerProfileId'];
    $customerPaymentProfileId = $member_cim['customerPaymentProfileId'];
    if (intval($customerProfileId) == 0 || intval($customerPaymentProfileId) == 0) {
        $message = "WARNING: Ignoring monthly membership fee for '".$member['first_name'].' '.$member['last_name']."' (".$member['contactemail'].") because user is FREE.";
        watchdog('cron', $message);
        return FALSE;
    }
    
    //watchdog('cron_log', 'customerProfileId: !customerProfileId, customerPaymentProfileId: !customerPaymentProfileId', array('!customerProfileId'=>$customerProfileId, '!customerPaymentProfileId'=>$customerPaymentProfileId));
    // build xml query
    //$node = node_load(32);
    $node = node_load($product_nid);
    if ($node) {
        if (isset($node->data) && !is_array($node->data)) {
          $node->data = unserialize($node->data);
        }
        if ($price == 0) {
            $price = $node->sell_price;
        }
    }

    if ($price != 0) {
        $title = (string)$node->title;
        //  $title .= " for ".$user->profile_firstname." ".$user->profile_lastname." (".$user->profile_legalname.")";

        // attempt payment
        $response = sendNcnPaymentRequest($customerProfileId, $customerPaymentProfileId, 
                      array(
                        'price'         => $price, 
                        'shipping_name' => $title, 
                        'shipping_desc' => $title, 
                        'item_id'       => $product_nid, 
                        'item_name'     => $title, 
                        'item_desc'     => $title, 
                      ));

        $auth_successful = true;
        if ("Ok" == $response->messages->resultCode)
        {
            $auth_successful = true;
        }
        else
        {
            
            $auth_successful = false;
            
            $response_reason_text = preg_replace("/[^a-zA-Z0-9\s]/", "", $response->messages->message->text);
            
            $params = array(
                'member_name' => $member['first_name'].' '.$member['last_name'],
                'activation' => false
            );
            if ( strcasecmp($response_reason_text, 'This transaction has been declined.') == 0 ) {
                if ( $mode=="standard" ) {
                    $message = credit_card_declined_mail_template($params);
                    drupal_mail('ncn_auto_payments', 'credit_card_declined', $member['contactemail'], language_default(), array('subject'=>t('Credit Card Declined'), 'body'=>$message));
                    
                    // Log to Declined Report
                    if ($product_nid) {
                        ncn_auto_payments_insert_declined_transaction($product_nid, 0, $member_id, date('U'), $price);
                    }
                } else {
                    $message = "ERROR: ".$response_reason_text;
                    $auto_payment_messages .= $message."\n\n";
                }
                
            } else if ( strcasecmp($response_reason_text, 'The credit card has expired.') == 0 ) {
                if ( $mode=="standard" ) {
                    $message = credit_card_expired_mail_template($params);
                    drupal_mail('ncn_auto_payments', 'credit_card_expired', $member['contactemail'], language_default(), array('subject'=>t('Credit Card Expired'), 'body'=>$message));
                } else {
                    $message = "ERROR: ".$response_reason_text;
                    $auto_payment_messages .= $message."\n\n";
                }
            } else {
                $message = "ERROR: Attempted to charge for '".$member['first_name'].' '.$member['last_name']."' (".$member['contactemail'].") failed because ".$response_reason_text;
                $auto_payment_messages .= $message."\n\n";
            }
        }
    } else {
        $auth_successful = true;
    }
    //*** payment log ***//
    $payment_success = ($auth_successful)? 1:0;
    $pid = ncn_report_payment_log($product_nid, $payment_success, null, $member_id, 0, 0, 0, $price);
    
    return ($payment_success)? TRUE:FALSE;
}
