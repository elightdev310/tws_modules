<?php

/**
 * NCN Action: Add Room
 */
function ncn_action_claim_add_room($claim_id, $room_name) {
    if ($room_name == '')
    {
        return array(
            'status' => 'error', 
            'msg'    => t("Room name is empty.")
        );
    }

    $room_name = preg_replace("/[^a-zA-Z0-9\s]/", "", $room_name);

    $result = db_query("SELECT * FROM {claims_data} WHERE claim_id=:a AND `key`=:b",
                        array(':a'=>$claim_id,':b'=>'scope'));
    $row_count = $result->rowCount();
    $b_insert = true;
    if ($row_count > 0)
    {
        // load images
        $row = $result->fetchAssoc();
        $scope = unserialize($row['val']);
        $b_insert = false;
    }

    if (empty($scope))
    {
        $scope = array();
        $default_room_name = ncn_admin_claim_get_default_room_name($claim_id);
        $scope[$default_room_name] = array();
    }

    if (array_key_exists($room_name, $scope))
    {
        return array(
            'status'  => 'error', 
            'msg'     => t("Room(!room_name) exists already.", array('!room_name' => $room_name)) 
        );
    }

    $scope[$room_name] = array(); // Add a room
    $scope = serialize($scope);
    if ($b_insert)
    {
        $query = "INSERT INTO claims_data VALUES(:a, :b, :c, :d, :e)";
        $result = db_query($query, array(
            ':a' => $claim_id,
            ':b' => '',
            ':c' => 'scope',
            ':d' => $scope,
            ':e' => 0,
        ));
    }
    else
    {
        $query = "UPDATE claims_data SET `val`=:a WHERE claim_id=:b AND `key`=:c";
        $result = db_query($query, array(
            ':a' => $scope,
            ':b' => $claim_id,
            ':c' => 'scope',
        ));
    }

    if ($result->rowCount()==0)
    {
        return array(
            'status' => 'error', 
            'msg'    => t('Failed to add a room.')
        );
    }

    //////////////////
    $log_message = t('!user_name added a room(!room_name)', array(
        '!user_name' => ncn_amin_get_user_role_and_name() ,
        '!room_name' => $room_name
    ));
    ncn_admin_insert_claim_log($claim_id, date('U') , $log_message);

    return array(
        'status' => 'success', 
        'msg'    => t('Added a room(!room_name), successfully.', array('!room_name' => $room_name)), 
        'room_name' => $room_name
    );
}





//------------------------------------------------------------------------------
// assigns new account manager
function get_account_manager_id($distributor_id)
{
  //  $query = "SELECT * FROM users_parent WHERE parent=" . $distributor_id;
    $result = db_query('SELECT * FROM {users_parent} WHERE parent=:a',array(':a'=>$distributor_id));
    $row_count = $result->rowCount();

    $lowest_val = 1000000;
    $lowest_key = -1;


    for ($i = 0; $i < $row_count; $i++) 
    foreach($result as $row)
    {
        $row = (array)$row;

//        $query2 = "SELECT * FROM member_id_pool WHERE owner=" . $row['uid'];
        $result2 = db_query('SELECT * FROM {member_id_pool} WHERE owner=:a',array(':a'=>$row['uid']));
        $row_count2 = $result2->rowCount();

        if ($row_count2 < $lowest_val) {
            $lowest_val = $row_count2;
            $lowest_key = $row['uid'];
        }
    }

    // check for errors
    if ($lowest_key == -1) {
        return false;
    }

    return $lowest_key;
}

function get_claim_file($claim_id, $live) {
    //  $query = "SELECT * FROM claims_invoices WHERE claim_id=$claim_id AND live=$live ORDER BY timestamp DESC";
    $result = db_query('SELECT * FROM {claims_invoices} WHERE claim_id=:a AND live=:b ORDER BY revision DESC',
                        array(':a'=>$claim_id,':b'=>$live));

    if ($result && $result->rowCount()>0)
    {
        $row = $result->fetchAssoc();
        return $row;
    }
    return array(); 
}

function get_claim_file_count($claim_id, $live) {
    $count = db_query('SELECT count(*) FROM {claims_invoices} WHERE claim_id=:a AND live=:b',
                        array(':a'=>$claim_id,':b'=>$live))
                ->fetchField();
    return $count;
}

function get_claim_scope_file($claim_id) {
//  $query = "SELECT * FROM claims_scope WHERE claim_id=$claim_id ORDER BY timestamp DESC";
    $result = db_query('SELECT * FROM {claims_scope} WHERE claim_id=:a ORDER BY timestamp DESC',array(':a'=>$claim_id ));
    if ($result)
    {   
        $row_count = $result->rowCount();
        $files = array();
        for ($i=0; $i<$row_count; $i++) {
            $row = $result->fetchAssoc();
            $files[$row['roomname']] = $row;
        }
        return $files;
    }
    return array(); 
}

/**
 * URL-AJAX: account/save_file_note (DON'T USE)
 */
function ncn_claims_manager_save_file_note()
{
    GLOBAL $user;

    $claim_id = arg(2);
    $file_note = $_REQUEST['note'];

    if ($user->uid == 1) {
        $user_id_sql = "";
    } else {
        $user_id_sql = " AND user_id=" . $user->uid . " ";
    }

    if (!is_numeric($claim_id)) {
        exit;
    }

    ncn_admin_add_claim_filenote($claim_id, $file_note);

    // set success message
    $log_message = "Notes on file updated by " . ncn_amin_get_user_role_and_name();
    ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
}

/**
 * URL-AJAX: account/send_to_archive
 */
function ncn_claims_manager_send_to_archive($claim_id=0,$uid=0,$ret=false)
{
    // check numeric claim_id
    if (!is_numeric($claim_id)) {
        if($ret){
            return "error";
        }
        exit;
    }

    if($claim_id==0){
        $claim_id = arg(2);
    }

    if($uid!=0){
        $user = user_load($uid);
    } else {
        GLOBAL $user;
    }

    if ($user->uid == 1) {
        $user_id_sql = "";
    } else {
        $user_id_sql = " AND user_id=" . $user->uid . " ";
    }

    // update the claim status
    $query = "UPDATE `claims` SET claim_status='archived' WHERE claim_id=" . $claim_id . " AND claim_status='receivables' " . $user_id_sql;
    $result = db_query($query);

    // set the "payment received" tot he "claim amount"
    $query = "UPDATE `claims` SET payment_received=claim_amount WHERE claim_id=" . $claim_id . " " . $user_id_sql;
    $result = db_query($query);

    // send some data base
    if($ret){
        return "success";
    } else {
        print "success";
    }
}

/**
 * URL-AJAX: account/set_partial_payment
 */
function ncn_claims_manager_set_partial_payment($claim_id=0,$amount=0,$uid=0,$ret=false)
{
    // check numeric amount & claim_id
    if (!is_numeric($amount) || !is_numeric($claim_id)) {
        if($ret){
            return "error";
        }
        exit;
    }

    if($claim_id==0){
        $claim_id = arg(2);
    }
    if($amount==0){
        $amount = trim(arg(3));
    }
    if($uid!=0){
        $user = user_load($uid);
    } else {
        GLOBAL $user;
    }

    if ($user->uid == 1) {
        $user_id_sql = "";
    } else {
        $user_id_sql = " AND user_id=" . $user->uid . " ";
    }

    // set the "payment received" amount
    //$query = "UPDATE `claims` SET payment_received=" . $amount . " WHERE claim_id = " . $claim_id . " " . $user_id_sql;
    $query = "UPDATE `claims` SET payment_received = ( payment_received + ".$amount." ) WHERE claim_id = " . $claim_id . " " . $user_id_sql;
    $result = db_query($query);

    // check claim amount is not 100%
    $query = "SELECT * FROM claims WHERE deleted=0 AND claim_id=" . $claim_id;
    $result = db_query($query);
    $row_count = $result->rowCount();
    $row = $result->fetchAssoc();

    if ($amount >= $row['claim_amount']) // 100%+, so make it archived
    {
        // update the claim status
        $query = "UPDATE `claims` SET claim_status='archived' WHERE claim_id=" . $claim_id . " AND claim_status='receivables' " . $user_id_sql;
        $result = db_query($query);

        if($ret){
            return "100%";
        } else {
            echo "100%";
        }
        exit;
    }

    // send some data base
    if($ret){
        return "success";
    } else {
        print "success";
    }
}

/**
 * URL: account/approve_invoice/%
 */
function ncn_claims_manager_approve_invoice($claim_id=0,$uid=0,$ret=false)
{
    GLOBAL $base_url;

    if($uid!=0){
        $user = user_load($uid);
    } else {
        GLOBAL $user;
    }

    // get the user id for this claim
    if($claim_id==0){
        $claim_id = arg(2);
    }
    //echo $claim_id;exit;
    //echo '<pre>';print_r(ncn_amin_get_user_role_and_name());exit;
    

    // if (!is_numeric($revision)) {
    //     $revision_sql = "";
    // } else {
    //     $revision_sql = " AND claims_invoices.revision=" . $revision . " ";
    // }
    $user_query = "SELECT users.mail FROM users as users inner join users_roles as users_roles  on users.uid = users_roles.uid and users_roles.rid=9";
    $user_result = db_query($user_query);
    foreach($user_result as $object)
    {
        $object = (array)$object;
        $to=$object['mail'];
        send_approve_invoice_mail($to,$claim_id);
    }
    $updated_user = $GLOBALS['user']->name;
    $log_message = "Status updated by " . ncn_amin_get_user_role_and_name() . " to 'approved'.Claim#'.$claim_id.' has been moved from 'Out for Review' to 'Receivables'";
    ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);

    watchdog('claim', 'approve invoice notification(claim moved from "Out for Review" to "Receivables") for %name.', array('%name' => $user->name));
    
    // get live/preview
    $user_id_sql = '';
    $query = "SELECT * FROM claims WHERE claim_id=" . $claim_id . $user_id_sql;
    $result = db_query($query);
    $row_count = $result->rowCount();

    if ($row_count > 0) {
        $claim = $result->fetchAssoc();

     //   $query = "UPDATE claims SET claim_status='receivables' WHERE claim_id=" . $claim_id;
        $result = db_query('UPDATE {claims} SET claim_status=:a WHERE claim_id=:b',array(':a'=>'receivables',':b'=>$claim_id));
        $msg = t('Your Claim(#'.$claim_id.') has been moved to "Receivables".');
        if($ret){
            return "success__".$msg;
        } else {
            drupal_set_message($msg,'status');
        }
    } else {
        if($ret){
            return "error__No Claim Found.";
        }
    }
    if(!$ret & isset($claim) && $claim['ars']){
        drupal_goto('account/ars.html');
        //Header("Location:".$base_url."/account/receivables.html");
    }
    drupal_goto('account/claims.html');
    exit;
}

function send_approve_invoice_mail($to,$claim_id) {
    $subject = 'Member Action Notification';
    $msg = 'Claim#'.$claim_id.' has been moved from "Out for Review" to "Receivables"';
    
    //$to = 'webdesigns@imageproz.com';
    //$to = 'kalpesh.thakkar@imobdevtech.com';
    //$to = 'nikhil.parmar@keyideasglobal.com';
    drupal_mail('ncn_report', 'approve_invoice_notification', $to, language_default(), array('subject'=>$subject, 'body'=>$msg));
}

/**
 * URL: account/purchase_invoice
 */
function ncn_claims_manager_purchase_invoice()
{
    GLOBAL $user;
    $user = user_load($user->uid);

    $claim_id = arg(2);
    $member_id = $user->profile_memberid;

    $product_id = ncn_admin_get_member_first_free_claim_product_id($claim_id);
    if ($product_id != 0) {
        if (!ncn_do_payment($member_id, $product_id)) {
            drupal_set_message(t('Failed to purchase invoice. (claim_id: !claim_id)', array('!claim_id' => $claim_id)), 'error');
            //drupal_goto('account/out-for-review.html');
            drupal_goto('account/claims.html');
            exit;
        } else {
            $arr_products[$product_id] = node_load($product_id);
            $pid = db_query('SELECT pid FROM {payment_log} WHERE member_id=:a AND product_id=:b',
                array(':a'=>$user->profile_memberid,':b'=>$product_id))->fetchField();

            if ($pid) {
                $this_user = $user;
                $mail_params = array(
                    'merchant' => 'Net Claims Now',
                    'invoice_number' => $pid,
                    'claim_id' => $claim_id,
                    'customer_id' => $this_user->profile_memberid,
                    'timestamp' => date('d-M-Y h:i:s A T', time()),

                    'member_name' => $this_user->profile_firstname . ' ' . $this_user->profile_lastname,
                    'member_address' => $this_user->profile_address,
                    'member_city' => $this_user->profile_city,
                    'member_state' => $this_user->profile_state,
                    'member_zip' => $this_user->profile_zip,
                    'member_email' => $this_user->mail,
                    'member_phone' => _get_format_phone_string($this_user->profile_officephone),

                    'products' => $arr_products,
                );
            }
            if (isset($mail_params)) {
                drupal_mail('ncn_admin', 'order_claim', $mail_params['member_email'], language_default(), $mail_params);
            }
        }
    }

    // Unlocked
    $result = db_query("UPDATE {member_first_free} SET first_invoice_locked=0
                         WHERE first_claim_free=%d AND status=1", $claim_id);
    if (!$result) {
        drupal_set_message(t('Failed to unlock invoice. (claim_id: !claim_id)', array('!claim_id' => $claim_id)), 'error');
    } else if ($product_id != 0) {
        drupal_set_message(t('Unlocked Invoice. (claim: #!claim_id)', array('!claim_id' => $claim_id)));
    }
    ncn_claims_manager_approve_invoice();
}

/**
 * URL: account/reject_invoice
 */
function ncn_claims_manager_reject_invoice()
{
    GLOBAL $user;

    // get the user id for this claim
    $claim_id = arg(2);


    // if (!is_numeric($revision)) {
    //     $revision_sql = "";
    // } else {
    //     $revision_sql = " AND claims_invoices.revision=" . $revision . " ";
    // }

    // get live/preview
    $query = "SELECT claim_status FROM claims WHERE claim_id=" . $claim_id . $user_id_sql;
    $result = db_query($query);
    $row_count = $result->rowCount();

    if ($row_count > 0) {
        $_SESSION['reject_claim_invoice'] = array(
            'claim_id' => $claim_id,
        );
        /*      // update the claim
        $row = mysql_fetch_assoc($result);

        $query = "UPDATE claims SET claim_status='returned' WHERE claim_id=".$claim_id;
        $result = db_query($query);

        // log Timestamp to reject invoice.
        set_ncn_data_extra($claim_id, 'rejected_invoice_timestamp', date('U'));

        // claim workflow
        $workflow = 'rejected_invoices';
        $action = 'reset';
        if (!update_timer_trigger($claim_id, $workflow, $action)) {

        }
*/
        // get the last message admin sent
        /*$query2 = "SELECT * FROM pm_message WHERE claim_id=".$claim_id." AND in_reply_to=0 ORDER BY id DESC LIMIT 0,1";
        $result2 = db_query($query2);
        $row_count2 = $result2->rowCount();

        if ($row_count2 == 0)   // error - no message available
        {
            Header("Location:/account/out-for-review.html");
        }
        else    // message avilable to reply to
        {
            $row2 = mysql_fetch_assoc($result2);
        */
        $desc = base64_encode("Change Order(Claim#" . $claim_id . ") Invoice");
        $claim_log = 'log';
        Header("Location:/account/message-center/send/c" . $claim_id . "/$desc/$claim_log");
        /*}*/
    }

    exit;
}

function ncn_claims_manager_reject_claim_invoice_action($claim_id)
{
   // $query = "SELECT claim_status FROM claims WHERE claim_id=" . $claim_id;
    $result = db_query('SELECT claim_status FROM {claims} WHERE claim_id=:a',array(':a'=>$claim_id));
    $row_count = $result->rowCount();

    if ($row_count > 0) {
        // update the claim
        $row = $result->fetchAssoc();
        if ($row['claim_status'] == 'out for review' || $row['claim_status'] == 'approved') {
           // $query = "UPDATE claims SET claim_status='returned' WHERE claim_id=" . $claim_id;
            $result = db_query('UPDATE {claims} SET claim_status=:a WHERE claim_id=:b',array(':a'=>'returned',':b'=>$claim_id));

            // log Timestamp to reject invoice.
            set_ncn_data_extra($claim_id, 'rejected_invoice_timestamp', date('U'));

            // claim workflow
            $workflow = 'rejected_invoices';
            $action = 'reset';
            if (!update_timer_trigger($claim_id, $workflow, $action)) {

            }
        }
    }
}


//------------------------------------------------------------------------------
function ncn_claims_manager_submit_claim_outforreview($claim_id=0,$uid=0,$ret=false)
{
    global $base_url;
    // get the user id for this claim
    if($claim_id==0){
        $claim_id = arg(2);
    }
    if($uid!=0){
        $user = user_load($uid);
    } else {
        GLOBAL $user;
    }

    $user = user_load($user->uid);
    $user_id = $user->uid;
    $actvt_user_id = $user_id;
    if (is_subuser($user)) {
        $parent_mid = ncn_subuser_get_memberid_from_subuid($user_id);
        $user_id = get_uid_from_memberid($parent_mid);
    }

    if ($user_id == 1) {
        $user_id_sql = "";
    } else {
        $user_id_sql = " AND user_id=" . $user_id . " ";
    }

    if (is_leaduser($user)) {
        $user_id_sql = " AND leaduser=" . $user_id . " ";
    }

    // get the revision
    // $revision = arg(3);
    // if (!is_numeric($revision)) {
    //     $revision_sql = "";
    // } else {
    //     $revision_sql = " AND claims_invoices.revision=" . $revision . " ";
    // }

    // get live/preview
    $query = "SELECT claim_status FROM claims WHERE claim_id=" . $claim_id . $user_id_sql;
    $result = db_query($query);
    $row_count = $result->rowCount();

    if ($row_count > 0) {
        $row = $result->fetchAssoc();

        // ---- inform the admin and account manager about this claim ----
        $user_details = user_load($user_id);

        // get account manager + admin email
        //$query = "SELECT * FROM `member_id_pool` WHERE member_id=\"" . AddSlashes($user_details->profile_memberid) . "\";";
        $result = db_query('SELECT * FROM {member_id_pool} WHERE member_id=:a',array(':a'=>$user_details->profile_memberid));
        $row_count = $result->rowCount();
        $row = $result->fetchAssoc(); // we are grabbing distributor id from emebr id

        $member = get_member_from_id($user_details->profile_memberid);

        if (!empty($member)) {
            $am = user_load($member['am_uid']);
            // $query = "SELECT * FROM  `users_parent` WHERE uid=" . $row['owner']; // get account manager from distributor
            $result = db_query('SELECT * FROM  {users_parent} WHERE uid=:a',array(':a'=>$member['owner']));
            $row_count = $result->rowCount();
            if ($row = $result->fetchAssoc()) { // we are grabbing account manager id from distributor id (account managers "own" distributors)
                $account_manager = user_load($row['parent']); // load the account manager profile
                $account_manager_email = $account_manager->mail; // grab the account manager email from their profile
            }
        }
        $admin_email = variable_get('site_mail', ini_get('sendmail_from')); // just take it from database for admin

        // send the email
        $params = array(
            '!claim_id' => $claim_id,
            '!firstname' => $user_details->profile_firstname,
            '!lastname' => $user_details->profile_lastname,
            '!legalname' => $user_details->profile_legalname,
        );


        drupal_mail('ncn_claims_manager', 'create_my_invoice', $admin_email, language_default(), $params);
        // if (isset($account_manager_email) && $account_manager_email) {
        //     drupal_mail('ncn_claims_manager', 'create_my_invoice', $account_manager_email, language_default(), $params);
        // }
        if (isset($am) && !empty($am->mail)) {
            drupal_mail('ncn_claims_manager', 'create_my_invoice', $am->mail, language_default(), $params);
        }

        if ($user_id != $user->uid) { // is_subuser($user)
            $parent_user = user_load($user_id);
            if (!empty($parent_user->mail)) {
                drupal_mail('ncn_claims_manager', 'create_my_invoice', $parent_user->mail, language_default(), $params);
            }
        }

        // update the claim status
        // $query = "UPDATE claims SET claim_status='out for review' WHERE claim_id=" . $claim_id;
        $result = db_query('UPDATE {claims} SET claim_status=:a WHERE claim_id=:b',array(':a'=>'out for review',':b'=>$claim_id));

        // start timer
        update_timer_trigger($claim_id, 'all_doc_recieved', 'start', true);
        drupal_get_messages('error');

        // show success to the user
        $msg = t('Your Claim(#'.$claim_id.') has been moved to "Out for Review" and has been submitted to admin for approval.');
        $log_message = t('!user_name moved claim to "Out for Review".', array('!user_name'=>ncn_amin_get_user_role_and_name($actvt_user_id)));
        ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
        if($ret){
            return "success__".$msg;
        } else {
            drupal_set_message($msg);
        }
    } 
    else // check invoice exists and user has permissions
    {
        $msg = t("Access denied to invoice file, or file not found.");
        if($ret){
            return "error__".$msg;
        } else {
            drupal_set_message($msg, 'error');
            return ("");
        }
    }
    if(!$ret){
        Header("Location:{$base_url}/account/claims.html");
        exit;
    }

}

/**
 * NCN Help Function: Get Last Invoice File Information
 */
function ncn_get_claim_invoice_info($claim_id, $live, $revision, $invoice_id) {
    if (!is_numeric($revision)) {
        $revision_sql = "";
    } else {
        $revision_sql = " AND claims_invoices.revision=" . $revision . " ";
    }

    if($live==11){
        $revision_sql .= " AND claims_invoices.invoice_id=" . $invoice_id . " ";
    }

    $query = "SELECT * FROM claims_invoices 
                WHERE claims_invoices.claim_id={$claim_id} AND claims_invoices.`live`= {$live} {$revision_sql}
                ORDER BY claims_invoices.revision DESC LIMIT 0,1";
    $result = db_query($query);
    if ($invoice_file = $result->fetchAssoc()) {
        return $invoice_file;
    }
    return false;
}

/**
 * Download claim file
 * URL: account/serve_invoice_file/{$claim_id}/{$live}/[$revision]/[$invoice_id]
 */
function ncn_claims_manager_serve_invoice_file()
{
    GLOBAL $user;

    $user = user_load($user->uid);
    // get the user id for this claim
    $claim_id = arg(2);

    /*
    if (($user->uid == 1) || ($user->uid == 6) )
    {   $user_id_sql = "";  }
    else
    {   $user_id_sql = " AND user_id=".$user->uid." ";  }
    */

    $user_id_sql = "";

    // get the "live"
    $live = arg(3);

    // get the revision
    $revision = arg(4);
    $invoice_id = arg(5);


    if (!is_numeric($revision)) {
        $revision_sql = "";
    } else {
        $revision_sql = " AND claims_invoices.revision=" . $revision . " ";
    }

    if($live==11){
        $revision_sql .= " AND claims_invoices.invoice_id=" . $invoice_id . " ";
    }

    // get live/preview
    $query = "SELECT claim_status FROM claims WHERE claim_id=" . $claim_id . $user_id_sql;

    $result = db_query($query);
    $row_count = $result->rowCount();
    $row = $result->fetchAssoc();

    /*
    if (($row['claim_status'] == 'receivables') || ($row['claim_status'] == 'paid in full')  || ($row['claim_status'] == 'approved') || ($user->uid == 1) || ($user->uid == 6) )
    {   $live = $live;  }
    else
    {   exit;   }
    */

    // setup data
    $query = "SELECT * FROM claims_invoices,claims 
                WHERE claims_invoices.claim_id=claims.claim_id AND claims.claim_id=" . $claim_id . " 
                AND claims_invoices.`live`=" . $live . " " . $user_id_sql . " " . $revision_sql . " 
                ORDER BY claims_invoices.revision DESC LIMIT 0,1";
    $result = db_query($query);
    $row_count = $result->rowCount();

    // check invoice exists and user has permissions
    if ($row_count == 0) {
        return ("Access denied to invoice file, or file not found.");
    }

    // get data
    $row = $result->fetchAssoc();

    //if (isset($user->profile_memberid) && is_member($user->profile_memberid)) {
    if (ncn_admin_get_claim_first_free_locked($claim_id) == 'LOCKED') {
        if (is_file(ncn_admin_get_locked_invoice_filename($row['filepath']))) {
            $row['filename'] = ncn_admin_get_locked_invoice_filename($row['filename']);
            $row['filepath'] = ncn_admin_get_locked_invoice_filename($row['filepath']);
        }
    }
    //}

    // serve file
    header('Content-Type: ' . $row['filemime']);
    header('Content-Disposition: attachment;filename="' . StripSlashes($row['filename']) . '"');
    header('Cache-Control: max-age=0');

    readfile($row['filepath']);

    exit;
}

//------------------------------------------------------------------------------
function ncn_claims_manager_delete_invoice_file()
{
    GLOBAL $user;

    // get the user id for this claim
    $claim_id = arg(2);

    /*
    if (($user->uid == 1) || ($user->uid == 6) )
    {   $user_id_sql = "";  }
    else
    {   $user_id_sql = " AND user_id=".$user->uid." ";  }
    */

    $user_id_sql = "";

    // get the "live"
    $live = arg(3);
    
    // get the revision
    $revision = arg(4);
    $invoice_id = arg(5);
    // get current scroll position
    $scroll_position = $_GET['current_scroll_position'];

//    $query = "SELECT * FROM claims_invoices WHERE `claim_id`=" . $claim_id . " AND `revision`=" . $revision . " AND `live`=" . $live;
        if($live==11){
            $result = db_query('SELECT * FROM {claims_invoices} WHERE claim_id=:a AND revision=:b AND live=:c AND invoice_id = :d',
                array(':a'=>$claim_id,':b'=>$revision,':c'=>$live,':d'=>$invoice_id));
        } else {
            $result = db_query('SELECT * FROM {claims_invoices} WHERE claim_id=:a AND revision=:b AND live=:c',
                array(':a'=>$claim_id,':b'=>$revision,':c'=>$live));
        }


    if ($result->rowCount() > 0) {
        $row = $result->fetchAssoc();
        //$delImg = DRUPAL_ROOT.'/'.$row['filepath'];

        /*Create File Object from File path*/
        $uri = str_replace("sites/default/files/","public://",$row['filepath']);
        $fileObj = createFileObject($uri);

        file_delete($fileObj);
        if ($live == 1) {
            $locked_file_uri = ncn_admin_get_locked_invoice_filename($row['filepath']);
            if(file_exists($locked_file_uri)){
                $lockedfileObj = createFileObject($locked_file_uri);
                file_delete($lockedfileObj);
            }
        }
    }
    // get live/preview
 //   $query = "DELETE  FROM claims_invoices WHERE `claim_id`=" . $claim_id . " AND `revision`=" . $revision . " AND `live`=" . $live;
    if($live==11){
        $result = db_query('DELETE  FROM {claims_invoices} WHERE claim_id=:a AND revision=:b  AND live=:c AND invoice_id = :d',
                            array(':a'=>$claim_id,':b'=>$revision,':c'=>$live,':d'=>$invoice_id));
    } else {
        $result = db_query('DELETE  FROM {claims_invoices} WHERE claim_id=:a AND revision=:b  AND live=:c',
            array(':a'=>$claim_id,':b'=>$revision,':c'=>$live));
    }

    // bounce back to claim
    header("Location:" . base_path() . "admin/config/ncn_admin/viewclaim/" . $claim_id . "?current_scroll_position=" . $scroll_position);

    exit;
}

//------------------------------------------------------------------------------
function ncn_claims_manager_update_handler()
{
    $page_title = base64_decode(arg(2));
    $class_name = base64_decode(arg(3));
    $order_by = base64_decode(arg(4));
    $additional_sql = base64_decode(arg(8));
    $editable = base64_decode(arg(5));
    $send_to_admin = base64_decode(arg(6));
    $approve_deny = base64_decode(arg(7));

    $claim_id = arg(9);

    /* Add by 2011-03-26  Remove unnecessary file*/
    foreach ($_SESSION['claim'][$claim_id]['scope'] as $key => $room) {
        foreach ($room as $image_id => $image_info) {
            if ($image_info['resize_image_path']) {
                $res = file_delete($image_info['path']);
                if ($res == true) {
                    unset($_SESSION['claim'][$claim_id]['scope'][$key][$image_id]['resize_image_path']);
                }
            }
        }
    }

    if (arg(2) == 'account-home') {
        echo ncn_claim_manager_homepage_innerpage();
    } else {

        if ($editable == 1) {
            $editable = true;
        } else {
            $editable = false;
        }

        if ($send_to_admin == 1) {
            $send_to_admin = true;
        } else {
            $send_to_admin = false;
        }

        if ($approve_deny == 1) {
            $approve_deny = true;
        } else {
            $approve_deny = false;
        }


        if ($editable == 1) {
            $editable = true;
        } else {
            $editable = false;
        }

        echo ncn_claim_manager_page_internal($page_title, $class_name, $order_by, $additional_sql, $editable, false, $send_to_admin, $approve_deny);
    }
}

//------------------------------------------------------------------------------
// see: modules/ubercart/uc_cart/uc_cart.pages.inc
function ncn_process_completed_order($order)
{
    // get data
    $user_id = $order->uid;
    $claim_id = $order->products[0]->data['attributes']['Claim'][0];

    // check errors
    if ((!is_numeric($claim_id)) || (!is_numeric($user_id))) {
        drupal_set_message(t("We're sorry.  An error occurred (#100) while processing your order that prevents us from completing it at this time. Please contact us and we will resolve the issue as soon as possible."), 'error');
        return false;
    }

    // check order belongs to this user and status is "approved"
   // $query = "SELECT * FROM `claims` WHERE claim_id=" . $claim_id . " AND `user_id`=" . $user_id . " AND `claim_status`='approved';";
    $result = db_query('SELECT * FROM {claims} WHERE claim_id=:a AND user_id=:b AND claim_status=:c',
        array(':a'=>$claim_id,':b'=>$user_id,':c'=>'approved'));
    $row_count = $result->rowCount();

    if ($row_count == 0) // user attempted to buya  claim twice, or claim does not belong to user
    {
        drupal_set_message(t("We're sorry.  An error occurred (#200) while processing your order that prevents us from completing it at this time. Please contact us and we will resolve the issue as soon as possible."), 'error');
        return false;
    }

    // all is good - update the claim status
    // $query = "UPDATE claims SET claim_status='receivables' WHERE claim_id=" . $claim_id . " AND `user_id`=" . $user_id . ";";
    $result = db_query('UPDATE {claims} SET claim_status=:a WHERE claim_id=:b AND user_id=:c',
        array(':a'=>'receivables',':b'=>$claim_id ,':c'=>$user_id));

    return true;
}

function invoiced_to_date($uid, $number=false)
{
    $_user = user_load($uid);
    $user_id = $uid;

    if (is_subuser($_user)) {
        $parent_mid = ncn_subuser_get_memberid_from_subuid($user_id);
        $user_id = get_uid_from_memberid($parent_mid);
    }
    // get total of all claims (that are not incomplete)
    $total = 0;

    if (is_leaduser($_user)) {
        $total = db_query('SELECT SUM(claim_amount) AS total FROM {claims} WHERE leaduser=:a and (claim_status=:b OR claim_status=:c)',array(':a'=>$user_id,':b'=>'receivables',':c'=>'archived'))->fetchField;
    } else {
        $total = db_query('SELECT SUM(claim_amount) AS total FROM {claims} WHERE user_id=:a',array(':a'=>$user_id))->fetchField();
    }

    if ($number) {
        return $total;
    }
    
    // fix display
    $total = '$' . number_format($total, 2);
    return ($total);
}

function ncn_claims_manager_check_access_additional_page($claim_id)
{
    global $user;
    $user = user_load($user->uid);
    $user_id = $user->uid;

    if (is_subuser($user)) {
        $parent_mid = ncn_subuser_get_memberid_from_subuid($user_id);
        $user_id = get_uid_from_memberid($parent_mid);
    }

    if (!user_access('access ncn_claims_manager content')) {
        return FALSE;
    }

    $claim = ncn_admin_get_claim_info($claim_id);
    if (empty($claim)) {
        return FALSE;
    }

    if ($user_id != $claim['user_id']) {
        return FALSE;
    }
    if ($claim['claim_status'] != 'incomplete') {
        //return FALSE;
    }

    return TRUE;
}

function ncn_claims_manager_check_access_scopesheet_page($claim_id, $room_name)
{
    global $user;
    $user = user_load($user->uid);
    $user_id = $user->uid;

    if (is_subuser($user)) {
        $parent_mid = ncn_subuser_get_memberid_from_subuid($user_id);
        $user_id = get_uid_from_memberid($parent_mid);
    }

    if (!user_access('access ncn_claims_manager content')) {
        return FALSE;
    }

    $claim = ncn_admin_get_claim_info($claim_id);
    if (empty($claim)) {
        return FALSE;
    }

    if ($user_id != $claim['user_id']) {
        return FALSE;
    }
    if ($claim['claim_status'] != 'incomplete' && $claim['claim_status'] != 'out for review' && $claim['claim_status'] != 'returned') {
        //return FALSE;
    }

    /*if (!ncn_admin_check_exist_claim_room($claim_id, $room_name, FALSE)) {
    drupal_set_message(t("Room(!room_name) doesn't exist.", array('!room_name'=>$room_name)), 'error');
        return FALSE;
    }*/
    return TRUE;
}

function createFileObject($uri){

    $fileObj = new stdClass();
    $fileObj->uid = $GLOBALS['user']->uid;
    $fileObj->filename = basename($uri);
    $fileObj->uri = $uri;
    $fileObj->filemime = file_get_mimetype($uri);
    $fileObj->timestamp = REQUEST_TIME;
    //$fileObj->status = FILE_STATUS_PERMANENT;

    {
        $fid_res = db_query("SELECT fid FROM {file_managed} WHERE uri = :a ",array(':a'=>$uri))->fetchAssoc();
        if($fid_res['fid']){
            $fileObj->fid = $fid_res['fid'];
        }
    }
    return $fileObj;
}

function send_returned_claim_mail_update($mail_key, $params, $admin_email_address){

    $user_query = "SELECT users.mail FROM users as users
                   inner join users_roles as users_roles on (users.uid = users_roles.uid and users_roles.rid=9)
                   WHERE users.status = 1";
    $user_result = db_query($user_query);
    foreach($user_result as $object)
    {
        $object = (array)$object;
        $to=$object['mail'];
        drupal_mail('ncn_message', $mail_key, $to, language_default(), $params, $admin_email_address, TRUE);
    }
}
