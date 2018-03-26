<?php
// Page Handler (admin/user/user/unactivated_member_list) - *Unused*
// See unactivated_member_list() - ncn_create_user.module
function ncn_admin_unactivated_member_list() {
GLOBAL $base_url;
    if (arg(4)== 'member_delete') {
        ncn_admin_unactivated_member_delete(arg(5));
    }
    drupal_add_js(drupal_get_path('module', 'ncn_admin').'/ncn_ admin.js');
    
    $header = array(
        array('data' => t('Member ID')),
        array('data' => t('Name')),
        array('data' => t('Status')), 
        array('data' => t('Company')),
        array('data' => t('Type')),
        array('data' => t('Contact Email')),
        array('data' => t('SignUp Date')),
        array('data' => t('Owner')),
        array('data' => t('Operation'))
    );
    
//  $query = "SELECT * FROM member_id_pool WHERE used=0 ORDER BY id DESC";
    $result = db_select('member_id_pool', 'n')
        ->fields('n')
        ->condition('used', 0,'=')
        ->orderBy('id', 'DESC')
        ->execute();
//  $result = pager_query($query, 50);
    //while ($row = db_fetch_array($result))
    foreach($result as $row)
    {
        $row = (array)$row;
        $member_type = "Gold";
        if ($row['member_type']==0) {
            $member_type = "Gold";
        }   else if ($row['member_type']==1) {
            $member_type = "Silver";
        } else if ($row['member_type']==2) {
            $member_type = "Gold Lite";
        } else if ($row['member_type']==3) {
            $member_type = "Coach on Call";
        } else if ($row['member_type']==4) {
            $member_type = "Gold Coach";
        } else if ($row['member_type']==5) {
            $member_type = "CSI Member";
        }
        
        if (ncn_admin_is_gold_member_demo($row['member_id'])) {
            $member_type .= " (Demo)";
        } else if (ncn_admin_is_member_first_free($row['member_id'])) {
            $member_type .= " (First Free)";
        }
        // is account free?
        $free_extra = '';
        if (is_member($row['member_id']))
        {   
            $result = db_query('SELECT customerProfileId FROM {member_cim} WHERE member_id=:s',array(':s'=>$row['member_id']))->fetchField();
            if (intval($result) == 0) {
                $free_extra = ' (FREE)';    
            }
        }
        $status = 'unactivated';
        if ($row['status'] == 0) {
            $status = 'paused';
        }
        
        $_owner = user_load($row['owner']);
        $rows[] = array('data' => array(
            // Cells
            $row['member_id'],
            $row['first_name'].' '.$row['last_name'] . $free_extra,
            $status, 
            $row['legalname'], 
            $member_type, 
            $row['contactemail'],
            date('d M Y', $row['create']),
            $_owner->profile_firstname.' '.$_owner->profile_lastname,
            "<a href='$base_url/admin/config/ncn_create_user/unactivated_member_list/edit/".$row['member_id']."'>Edit</a>&nbsp;&nbsp;".
//"<a href='$base_url/admin/user/user/unactivated_member_list/member_delete/".$row['member_id']."' onclick='return on_click_ncn_admin_una_member_delete();'>Delete</a>&nbsp;&nbsp;"
            "<a href='$base_url/admin/config/ncn_create_user/unactivated_member_list/member_delete/".$row['member_id']."' onclick='return on_click_ncn_admin_una_member_delete();'>Delete</a>&nbsp;&nbsp;"

        ),
        );
    }
    if (!$rows) {
        $rows[] = array(array('data' => t("There isn't unactivated member."), 'colspan' => 9));
    }
    $output = '';
    $output .= theme('table',array("header"=>$header,"rows"=>$rows, 'attributes'=>array('id' => 'una_member_list')));
    $output .= theme('pager');
    echo $output;
}

function ncn_admin_unactivated_member_delete($member_id) {
    $result = db_delete('member_id_pool')
        ->condition('member_id', $member_id)
        ->condition('used', 0,'=')
        ->execute();
    //    db_query("DELETE FROM {member_id_pool} WHERE member_id=':s' AND used=0",array(":s"=>$member_id));
    if ($result) {
        db_delete('member_cim')
            ->condition('member_id', $member_id)
            ->execute();
        db_delete('member_first_free')
            ->condition('member_id', $member_id)
            ->execute();
        db_delete('member_gold_demo')
            ->condition('member_id', $member_id)
            ->execute();

    //    db_query( "DELETE FROM {member_cim} WHERE member_id=':s'",array(":s"=>$member_id));
    //  db_query( "DELETE FROM {member_first_free} WHERE member_id=':s'",array(":s"=>$member_id ));
    //  db_query( "DELETE FROM {member_gold_demo} WHERE member_id=':s'",array(":s"=>$member_id));

        drupal_set_message( t('Member(#!member_id) deleted, successfully.', array('!member_id'=>$member_id)) );
    } else {
        drupal_set_message( t('Failed to delete member(#!member_id).', array('!member_id'=>$member_id)) );
    }

    drupal_goto('admin/config/ncn_create_user/unactivated_member_list');
}

// Done in ncn_create_user - function unactivated_member_list_edit()
// Unused this function.
// function ncn_admin_una_member_edit($member_id) {
// }

//------------------------------------------------------------------------------
// see user.admin.inc
function user_add_distributor()
{
    // user tried to add a distributor
    if (isset($_POST['tfunction']))
    {
        // error checking
        $error = false;
        
        foreach ($_POST as $key=>$val)
        {
            if (($_POST[$key] == '') && ($error == false))
            {
                drupal_set_message("All fields must be completed.", 'error');
                $error = true;
            }
        }
        
        // check valid username
        if ($error == false)
        {
        //  $query = "SELECT * FROM users WHERE `name`=\"".AddSlashes($_POST['name'])."\";";
            $result = db_query('SELECT * FROM {users} WHERE name=:a',
                array(':a'=>$_POST['name']));
            
            foreach($result as $row)
            {
                $row = (array)$row;         
                if(count($row) > 0)
                {
                    drupal_set_message("That username is taken.", 'error');
                    $error = true;
                }           
            }
        }
        if ($error == false) {
            if (!valid_email_address($_POST['mail'])) {
                drupal_set_message('Email address is invalid, Please retry to input email address.', 'error');
                $error = true;
            }
        }
        
        // check passwords match        
        if ($error == false)
        {
            if ($_POST['pass'] != $_POST['pass2'])
            {
                drupal_set_message("Passwords do not match.", 'error');
                $error = true;
            }
        }
        if ($error == false)
        {
            if ($_POST['reportpass'] != $_POST['reportpass2'])
            {
                drupal_set_message("Report Passwords do not match.", 'error');
                $error = true;
            }
        }
        
        // check valid email
        /*if ($error == false)
        {
            $query = "SELECT * FROM users WHERE `mail`=\"".AddSlashes($_POST['mail'])."\";";
            $result = db_query($query);
            foreach($result as $row)
            {
                $row = (array)$row;         
                if(count($row) > 0)
                {
                    drupal_set_message("Warining: That email address is already in use.", 'error');
                    $error = true;
                }   
            }           
        }*/
        
        // no errors, so try the card
        if ($error == false)
        {
            $error = !attempt_add_distributor_card();
        }
        
        // add good, so add the distributor
        if ($error == false)
        {
            $section1 = $_POST;
            
            unset($section1['pass2']);
            unset($section1['tfunction']);
            $section1['status'] = 1;
            $section1['roles'] = array(5 => 5, 2 => 1, 6 => 0, 7 => 0, 3 => 0, 4 => 0); 

            $section2 = array(
                'pass' => $_POST['pass'],
                'init' => $_POST['mail'],
                'roles' => array(5 => 5, 2 => 1)
            );
        
        
            $account = user_save('', array_merge($section1, $section2));
                          
            // Terminate if an error occured during user_save().
            if (!$account) {
                drupal_set_message(t("Error saving user account."), 'error');
                $form_state['redirect'] = '';
                $error = true;
            }
            
            // send welcome email (account manager) HERE
            $params = array(
                '!username' => $_POST['name'],
                '!password' => $_POST['pass'],
                '!firstname' => $_POST['profile_firstname'],
                '!lastname' => $_POST['profile_lastname'],
            );
            add_report_password($_POST['name'], $_POST['reportpass']);
            
            drupal_mail('ncn_admin', 'distributor', $_POST['mail'], language_default(), $params);
            
        }
        
        // everything worked - show success
        if ($error == false)
        {
            // save the ownersship relationship
        //  $query = "INSERT INTO users_parent (uid, parent) VALUES(".$account->uid.", ".$_POST['account_manager'].");";
            $result = db_query('INSERT INTO {users_parent} (uid, parent) VALUES(:a,:b)',
                array(':a'=>$account->uid,':b'=>$_POST['account_manager']));
            
            $_POST = array();
            drupal_set_message("Distributor '".$_POST['name']."' added successfully", 'status');
        }
        
         
    }

    // ---- draw the form ----

    ?>
    <form method="POST" autocomplete="off">
    <input type="hidden" name="tfunction" value="add_distributor">

    <fieldset>
    <legend>Create new distributor</legend>
    
        
        <table style="width: 600px; border:0;">
            <tr>
                <td>Username</td>
                <td><input type="text" name="name" value="<?= $_POST['name']; ?>"></td>
            </tr>
            <tr>
                <td>Assign Account Manager</td>
                <td><?php account_manager_dropdown(); ?></td>
            </tr>
            <tr>
                <td>E-mail</td>
                <td><input type="text" name="mail" value="<?= $_POST['mail']; ?>" AUTOCOMPLETE="off"></td>
            </tr>
            <tr>
                <td>Password</td>
                <td><input type="password" name="pass" value="<?= $_POST['pass']; ?>" AUTOCOMPLETE="off"></td>
            </tr>
            <tr>
                <td>Confirm Password</td>
                <td><input type="password" name="pass2" value="<?= $_POST['pass2']; ?>" AUTOCOMPLETE="off"></td>
            </tr>
            <tr>
                <td>First Name</td>
                <td><input type="text" name="profile_firstname" value="<?= $_POST['profile_firstname']; ?>"></td>
            </tr>
            <tr>
                <td>Last Name</td>
                <td><input type="text" name="profile_lastname" value="<?= $_POST['profile_lastname']; ?>"></td>
            </tr>
            <tr>
                <td>Company's Name</td>
                <td><input type="text" name="profile_legalname" value="<?= $_POST['profile_legalname']; ?>"></td>
            </tr>
            <tr>
                <td>Country</td>
                <td><?php draw_countryform_part(array(
                            'name'=>'profile_country', 
                            'class'=>'', 
                            'disabled'=>false, 
                            'required'=>false)); ?></td>
            </tr>
            <tr>
                <td>Address</td>
                <td><input type="text" name="profile_address" value="<?= $_POST['profile_address']; ?>"></td>
            </tr>
            <tr>
                <td>City</td>
                <td><input type="text" name="profile_city" value="<?= $_POST['profile_city']; ?>"></td>
            </tr>
            <tr>
                <td>Province/State</td>
                <td><?php draw_stateform_part(array(
                            'name'  => 'profile_state', 
                            'class' => '', 
                            'disabled' => false, 
                            'required' => false, 
                    )); ?>
                    </td>
            </tr>
            <tr>
                <td>Zip</td>
                <td><input type="text" name="profile_zip" value="<?= $_POST['profile_zip']; ?>"></td>
            </tr>

            <tr>
                <td>Credit Card Number</td>
                <td><input type="text" name="profile_credit_card_number" value="<?= $_POST['profile_credit_card_number']; ?>"></td>
            </tr>
            <tr>
                <td>Expiration Date (mm/yy)</td>
                <td><input type="text" name="profile_credit_card_expiration" value="<?= $_POST['profile_credit_card_expiration']; ?>"></td>
            </tr>
            <tr>
                <td>Security Code</td>
                <td><input type="text" name="profile_credit_card_security" value="<?= $_POST['profile_credit_card_security']; ?>"></td>
            </tr>
            <tr>
                <td>Name as it appears on card</td>
                <td><input type="text" name="profile_credit_card_name" value="<?= $_POST['profile_credit_card_name']; ?>"></td>
            </tr>
            <tr>
                <td>Report Password</td>
                <td><input type="password" name="reportpass" value="<?= $_POST['reportpass']; ?>" AUTOCOMPLETE="off"></td>
            </tr>
            <tr>
                <td>Confirm Password</td>
                <td><input type="password" name="reportpass2" value="<?= $_POST['reportpass2']; ?>" AUTOCOMPLETE="off"></td>
            </tr>
            <tr>
                <td colspan="2"><input type="submit" value="Add Distributor"></td>
            </tr>
        </table>
    </fieldset>
    
    </form> 
    <?php

}
//------------------------------------------------------------------------------
//----------------lead_generation_user--------------------------
function lead_generation_user()
{

    // user tried to add a lead generation user
    if (isset($_POST['tfunction']))
    {
        // error checking
        $error = false;
        $flage= true;
        
        foreach ($_POST as $key=>$val)
        {
            if (($_POST[$key] == '') && ($error == false))
            {
                drupal_set_message("All fields must be completed.", 'error');
                $error = true;
                $flage= false;
            }
        }
        
/*      if ($error == false) {
        if ($_POST['parent_mid'] == '') {
            drupal_set_message("Please select member.", 'error');
            $error = true;
        }
    }*/
        
        // check valid username
        if ($error == false)
        {
    //      $query = "SELECT * FROM users WHERE `name`=\"".AddSlashes($_POST['name'])."\";";
            $result = db_query('SELECT * FROM {users} WHERE name=:a',array(':a'=>$_POST['name']));
            foreach($result as $row)
            {
                $row = (array)$row;         
                if(count($row) > 0)
                {
                    drupal_set_message("That username is taken.", 'error');
                    $error = true;
                }   
            }           
        }
        if ($error == false) {
            if (!valid_email_address($_POST['mail'])) {
                drupal_set_message('Email address is invalid, Please retry to input email address.', 'error');
                $error = true;
            }
        }
        
        // check passwords match        
        if ($error == false)
        {
            if ($_POST['pass'] != $_POST['pass2'])
            {
                drupal_set_message("Passwords do not match.", 'error');
                $error = true;
            }
        }
        if ($error == false)
        {
            if ($_POST['reportpass'] != $_POST['reportpass2'])
            {
                drupal_set_message("Report Passwords do not match.", 'error');
                $error = true;
            }
        } 
        
        // check valid email
        if ($error == false)
        {
    //      $query = "SELECT * FROM users WHERE `mail`=\"".AddSlashes($_POST['mail'])."\";";
            $result = db_query('SELECT * FROM {users} WHERE mail=:a',array(':a'=>$_POST['mail']));
            foreach($result as $row)
            {
                $row = (array)$row;         
                if(count($row) > 0)
                {
                    drupal_set_message("Warining: That email address is already in use.", 'error');
                    $error = true;
                }   
            }           
        }
        
    if (!$error) {
            
            require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
            
            $name="'".$_POST['name']."'";
            $pass="'". user_hash_password(trim($_POST['pass']))."'";
            $mail="'".$_POST['mail']."'";
            $firstname = "'".$_POST['firstname']."'";
            $lastname =  "'".$_POST['lastname']."'";
            $company_name = "'".$_POST['company_name']."'";
            $dist_id = $_POST['parent_mid'];
            
            $date = strtotime(date('Y-m-d H:i:s'));
            $user_id = db_next_id("{users}");
            $success = db_query("INSERT INTO {users} (`uid`,`name`,`mail`,`pass`,`created`,`status`,`init`) VALUES ($user_id,$name,$mail,$pass,$date,'1',$mail)");
           
            if ($success) {
               // $last_id = db_query("SELECT lead_id FROM users_lead ORDER BY lead_id DESC LIMIT 1");
               $last_id = db_query("SELECT Max( Substr( lead_id, 4 ) ) as max_id FROM users_lead ");
               $last_id = (array)$last_id;
                //print_r($last_id);exit;
            if($last_id['max_id'] != '')
            {
                //$temp_id = substr($last_id,3,strlen($last_id));
                $last_id_str=$last_id['max_id']+1;
                 $lead_id = "'LVC".$last_id_str."'";
            }
            else
            {
                 $lead_id = "'LVC101'";
            }
            
           $success1 = db_query("INSERT INTO {users_lead} (`lead_id`,`user_id`,`distributor_id`,`company_name`,`firstname`,`lastname`) VALUES ($lead_id,$user_id,'NULL',$company_name,$firstname,$lastname)");
           $success2 = db_query("INSERT INTO {users_roles} (`uid`,`rid`) VALUES ($user_id,'23')");
            }
            else
            {
                    drupal_set_message("lead generation user not added");
            }
           
           if ($success1) {
                $_POST = array();
                drupal_set_message("lead generation user added successfully");
            }
            else
            {
                    drupal_set_message("lead generation user not added");
            }
            
        }
}
    ?>
    <form method="POST" autocomplete="off">
    <input type="hidden" name="tfunction" value="add_lead_generation_user">

    <fieldset>
    <legend>Create new lead generation user</legend>
    
        
        <table style="width: 600px; border:0;">
            <tr>
                <td>Username</td>
                <td><input type="text" name="name" value="<?= $_POST['name']; ?>"></td>
            </tr>
            <tr>
                <td>E-mail</td>
                <td><input type="text" name="mail" value="<?= $_POST['mail']; ?>" AUTOCOMPLETE="off"></td>
            </tr>
            <tr>
                <td>Password</td>
                <td><input type="password" name="pass" value="<?= $_POST['pass']; ?>" AUTOCOMPLETE="off"></td>
            </tr>
            <tr>
                <td>Confirm Password</td>
                <td><input type="password" name="pass2" value="<?= $_POST['pass2']; ?>" AUTOCOMPLETE="off"></td>
            </tr>
            <tr>
                <td>First Name</td>
                <td><input type="text" name="firstname" value="<?= $_POST['firstname']; ?>"></td>
            </tr>
            <tr>
                <td>Last Name</td>
                <td><input type="text" name="lastname" value="<?= $_POST['lastname']; ?>"></td>
            </tr>
            <tr>
                <td>Company's Name</td>
                <td><input type="text" name="company_name" value="<?= $_POST['company_name']; ?>"></td>
            </tr>
            <?php /*<tr>
                <td>Assign Member</td>
                <td><?php print ncn_subuser_member_dropdown('parent_mid', $_POST['parent_mid']); ?></td>
            </tr> */ ?>
            <tr>
                <td colspan="2"><input type="submit" value="Add Lead Generator User"></td>
            </tr>
        </table>
    </fieldset>
    
    </form> 
    <?php
}
//------------------------------------------------------------------------------
// see user.admin.inc
function user_add_associate()
{
// user tried to add a associate
    if (isset($_POST['tfunction']))
    {
        // error checking
        $error = false;
        
        foreach ($_POST as $key=>$val)
        {
            if (($_POST[$key] == '') && ($error == false))
            {
                drupal_set_message("All fields must be completed.", 'error');
                $error = true;
            }
        }
        
        // check valid username
        if ($error == false)
        {
            $query = "SELECT * FROM users WHERE `name`=\"".AddSlashes($_POST['name'])."\";";
            $result = db_query($query);
            foreach($result as $row)
            {
                $row = (array)$row;         
                if(count($row) > 0)
                {
                    drupal_set_message("That username is taken.", 'error');
                    $error = true;
                }   
            }           
        }
        
        if ($error == false) {
            if (!valid_email_address($_POST['mail'])) {
                drupal_set_message('Email address is invalid, Please retry to input email address.', 'error');
                $error = true;
            }
        }

        // check passwords match        
        if ($error == false)
        {
            if ($_POST['pass'] != $_POST['pass2'])
            {
                drupal_set_message("Passwords do not match.", 'error');
                $error = true;
            }
        }
        
        // check valid email
        if ($error == false)
        {
            $query = 'SELECT * FROM users WHERE mail = :mail';
            $result = db_query($query,array(':mail'=>addslashes($_POST['mail'])));
            foreach($result as $row)
            {
                $row = (array)$row;         
                if(count($row) > 0)
                {
                    drupal_set_message("Warining: That email address is already in use.", 'error');
                    // $error = true;
                }   
            }           
        }
        
        // no errors, so try the card // associate doesn't need credit card
        /*if ($error == false)
        {
            $error = !attempt_add_distributor_card();
        }*/
        
        // add good, so add the associate
        if ($error == false)
        {
            $section1 = $_POST;
            
            unset($section1['pass2']);
            unset($section1['tfunction']);
            $section1['status'] = 1;
            $section1['roles'] = array(7 => 7, 2 => 1, 5 => 0, 6 => 0, 3 => 0, 4 => 0); 

            $section2 = array(
                'pass' => $_POST['pass'],
                'init' => $_POST['mail'],
                'roles' => array(7 => 7, 2 => 1)
            );
        
        
            $account = user_save('', array_merge($section1, $section2));
                          
            // Terminate if an error occured during user_save().
            if (!$account) {
                drupal_set_message(t("Error saving user account."), 'error');
                $form_state['redirect'] = '';
                $error = true;
            }
            
            // send welcome email (account manager) HERE
            $params = array(
                '!username' => $_POST['name'],
                '!password' => $_POST['pass'],
                '!firstname' => $_POST['profile_firstname'],
                '!lastname' => $_POST['profile_lastname'],
            );
                        
            drupal_mail('ncn_admin', 'associate', $_POST['mail'], language_default(), $params);
            
        }
        
        // everything worked - show success
        if ($error == false)
        {
            // save the ownersship relationship
            $query = "INSERT INTO users_parent (uid, parent) VALUES(".$account->uid.", ".$_POST['account_manager'].");";
            $result = db_query($query);
        
            $_POST = array();
            drupal_set_message("Associate '".$_POST['name']."' added successfully", 'status');
        }
        
         
    }

    // ---- draw the form ----

    ?>
    <form method="POST">
    <input type="hidden" name="tfunction" value="add_associate">

    <fieldset>
    <legend>Create new Assoicate</legend>
    
        
        <table style="width:400px;border:0;">
            <tr>
                <td>Username</td>
                <td><input type="text" name="name" value="<?= $_POST['name']; ?>"></td>
            </tr>
            <tr>
                <td>Assign Account Manager</td>
                <td><?php account_manager_dropdown(); ?></td>
            </tr>
            <tr>
                <td>E-mail</td>
                <td><input type="text" name="mail" value="<?= $_POST['mail']; ?>" AUTOCOMPLETE="off"></td>
            </tr>
            <tr>
                <td>Password</td>
                <td><input type="password" name="pass" value="<?= $_POST['pass']; ?>" AUTOCOMPLETE="off"></td>
            </tr>
            <tr>
                <td>Confirm Password</td>
                <td><input type="password" name="pass2" value="<?= $_POST['pass']; ?>" AUTOCOMPLETE="off"></td>
            </tr>
            <tr>
                <td>First Name</td>
                <td><input type="text" name="profile_firstname" value="<?= $_POST['profile_firstname']; ?>"></td>
            </tr>
            <tr>
                <td>Last Name</td>
                <td><input type="text" name="profile_lastname" value="<?= $_POST['profile_lastname']; ?>"></td>
            </tr>
            <tr>
                <td>Company's Name</td>
                <td><input type="text" name="profile_legalname" value="<?= $_POST['profile_legalname']; ?>"></td>
            </tr>
            <tr>
                <td>Country</td>
                <td><?php draw_countryform_part(array(
                            'name'=>'profile_country', 
                            'class'=>'', 
                            'disabled'=>false, 
                            'required'=>false)); ?></td>
            </tr>
            <tr>
                <td>Address</td>
                <td><input type="text" name="profile_address" value="<?= $_POST['profile_address']; ?>"></td>
            </tr>
            <tr>
                <td>City</td>
                <td><input type="text" name="profile_city" value="<?= $_POST['profile_city']; ?>"></td>
            </tr>
            <tr>
                <td>Province/State</td>
                <td><?php draw_stateform_part(array(
                            'name'  => 'profile_state', 
                            'class' => '', 
                            'disabled' => false, 
                            'required' => false, 
                    )); ?></td>
            </tr>
            <tr>
                <td>Zip</td>
                <td><input type="text" name="profile_zip" value="<?= $_POST['profile_zip']; ?>"></td>
            </tr>

            <!-- <tr>
                <td>Credit Card Number</td>
                <td><input type="text" name="profile_credit_card_number" value="<?= $_POST['profile_credit_card_number']; ?>"></td>
            </tr>
            <tr>
                <td>Expiration Date (mm/yy)</td>
                <td><input type="text" name="profile_credit_card_expiration" value="<?= $_POST['profile_credit_card_expiration']; ?>"></td>
            </tr>
            <tr>
                <td>Security Code</td>
                <td><input type="text" name="profile_credit_card_security" value="<?= $_POST['profile_credit_card_security']; ?>"></td>
            </tr>
            <tr>
                <td>Name as it appears on card</td>
                <td><input type="text" name="profile_credit_card_name" value="<?= $_POST['profile_credit_card_name']; ?>"></td>
            </tr> -->
            
            <tr>
                <td colspan="2"><input type="submit" value="Add Associate"></td>
            </tr>
        </table>
    </fieldset>
    
    </form> 
    <?php
}

//------------------------------------------------------------------------------
// see user.admin.inc
function user_add_sub_user()
{
// user tried to add a associate
    if (isset($_POST['tfunction']))
    {
        $parent_mid = $_POST['parent_mid'];
        if ( module_exists('ncn_subuser') ) {
            if (ncn_subuser_create_subuser($parent_mid, $_POST)) {
                $_POST = array();
            }
        }
    }

    // ---- draw the form ----

    ?>
    <form method="POST">
    <input type="hidden" name="tfunction" value="create-subuser">

    <fieldset>
    <legend>Create new Sub User</legend>
    
        
        <table style="width:400px;border:0;">
            <tr>
                <td>Username</td>
                <td><input type="text" name="name" value="<?= $_POST['name']; ?>"></td>
            </tr>
            <tr>
                <td>Assign Member</td>
                <td><?php print ncn_subuser_member_dropdown('parent_mid', $_POST['parent_mid']); ?></td>
            </tr>
            <tr>
                <td>E-mail</td>
                <td><input type="text" name="mail" value="<?= $_POST['mail']; ?>" AUTOCOMPLETE="off"></td>
            </tr>
            <tr>
                <td>Password</td>
                <td><input type="password" name="pass" value="<?= $_POST['pass']; ?>" AUTOCOMPLETE="off"></td>
            </tr>
            <tr>
                <td>Confirm Password</td>
                <td><input type="password" name="pass2" value="<?= $_POST['pass']; ?>" AUTOCOMPLETE="off"></td>
            </tr>
            <tr>
                <td>First Name</td>
                <td><input type="text" name="profile_firstname" value="<?= $_POST['profile_firstname']; ?>"></td>
            </tr>
            <tr>
                <td>Last Name</td>
                <td><input type="text" name="profile_lastname" value="<?= $_POST['profile_lastname']; ?>"></td>
            </tr>
            <tr>
                <td>Company's Name</td>
                <td><input type="text" name="profile_legalname" value="<?= $_POST['profile_legalname']; ?>"></td>
            </tr>
            <tr>
                <td>Country</td>
                <td><?php draw_countryform_part(array(
                            'name'=>'profile_country', 
                            'class'=>'', 
                            'disabled'=>false, 
                            'required'=>false)); ?></td>
            </tr>
            <tr>
                <td>Address</td>
                <td><input type="text" name="profile_address" value="<?= $_POST['profile_address']; ?>"></td>
            </tr>
            <tr>
                <td>City</td>
                <td><input type="text" name="profile_city" value="<?= $_POST['profile_city']; ?>"></td>
            </tr>
            <tr>
                <td>Province/State</td>
                <td><?php draw_stateform_part(array(
                            'name'  => 'profile_state', 
                            'class' => '', 
                            'disabled' => false, 
                            'required' => false, 
                    )); ?>
                </td>
            </tr>
            <tr>
                <td>Zip</td>
                <td><input type="text" name="profile_zip" value="<?= $_POST['profile_zip']; ?>"></td>
            </tr>
            
            <tr>
                <td colspan="2"><input type="submit" value="Add Sub User"></td>
            </tr>
        </table>
    </fieldset>
    
    </form> 
    <?php
}

//------------------------------------------------------------------------------
// see user.admin.inc
function user_add_account_manager()
{
    // user tried to add a distributor
    if (isset($_POST['tfunction']))
    {
        // error checking
        $error = false;
        
        foreach ($_POST as $key=>$val)
        {
            if (($_POST[$key] == '') && ($error == false))
            {
                drupal_set_message("All fields must be completed.", 'error');
                $error = true;
            }
        }

        // check valid username
        if ($error == false)
        {
        //  $query = "SELECT * FROM users WHERE `name`=\"".AddSlashes($_POST['name'])."\";";
            $result = db_query('SELECT * FROM {users} WHERE name=:a',array(':a'=>$_POST['name']));
            foreach($result as $row)
            {
                $row = (array)$row;         
                if(count($row) > 0)
                {
                    drupal_set_message("That username is taken.", 'error');
                    $error = true;
                }   
            }           
        }
        
        if ($error == false) {
            if (!valid_email_address($_POST['mail'])) {
                drupal_set_message('Email address is invalid, Please retry to input email address.', 'error');
                $error = true;
            }
        }
        
        // check passwords match        
        if ($error == false)
        {
            if ($_POST['pass'] != $_POST['pass2'])
            {
                drupal_set_message("Passwords do not match.", 'error');
                $error = true;
            }
        }
        
        
        // check valid email
        if ($error == false)
        {
        //  $query = "SELECT * FROM users WHERE `mail`=\"".AddSlashes($_POST['mail'])."\";";
            $result = db_query('SELECT * FROM {users} WHERE mail=:a',array(':a'=>$_POST['mail']));
            foreach($result as $row)
            {
                $row = (array)$row;         
                if(count($row) > 0)
                {
                    drupal_set_message("That email address is already in use.", 'error');
                    $error = true;
                }
            }           
        }
        
        $role_array = user_roles();
        // add good, so add the account manager
        if ($error == false)
        {
            $section1 = $_POST;
            
            unset($section1['pass2']);
            unset($section1['tfunction']);
            unset($section1['distributor']);
            $section1['status'] = 1;
            //$section1['roles'] = array(5 => 0, 7 => 0, 2 => 1, 6 => 6, 3 => 0, 4 => 0);   

            $section2 = array(
                'pass' => $_POST['pass'],
                'init' => $_POST['mail'],
                'roles' => array($_POST['user_role'] => $_POST['user_role'], 2 => 1)
            );
            
            $account = user_save('', array_merge($section1, $section2));
                          
            // Terminate if an error occured during user_save().
            if (!$account) {
                drupal_set_message(t("Error saving user account."), 'error');
                $form_state['redirect'] = '';
                $error = true;
            }
            
            // send welcome email (account manager) HERE
            $params = array(
                '!username' => $_POST['name'],
                '!password' => $_POST['pass'],
                '!firstname' => $_POST['profile_firstname'],
                '!lastname' => $_POST['profile_lastname'],
                '!rolename' => ucwords($role_array[$_POST['user_role']]),
            );
                        
         drupal_mail('ncn_admin', 'accountmanager', $_POST['mail'], language_default(), $params);
            
        }
        
        // everything worked - show success
        if ($error == false)
        {
            drupal_set_message(ucwords($role_array[$_POST['user_role']]). " " . $_POST['name']." added successfully", 'status');
        }
        
         
    }

    // ---- draw the form ----

    ?>
    <form method="POST" autocomplete="off">
    <input type="hidden" name="tfunction" value="add_distributor">

    <fieldset>
    <legend>Create New User</legend>
    
        
        <table style="width:400px;border:0;">
            <tr>
                <td>Username</td>
                <td><input type="text" name="name" value="<?= $_POST['name']; ?>"></td>
            </tr>
            <tr>
                <td>E-mail</td>
                <td><input type="text" name="mail" value="<?= $_POST['mail']; ?>" autocomplete="off"></td>
            </tr>
            <tr>
                <td>Password</td>
                <td><input type="password" name="pass" value="<?= $_POST['pass']; ?>" autocomplete="off"></td>
            </tr>
            <tr>
                <td>Confirm Password</td>
                <td><input type="password" name="pass2" value="<?= $_POST['pass']; ?>" autocomplete="off"></td>
            </tr>
            <tr>
                <td>Role</td>
                <td><?php echo ncn_admin_render_select_role('user_role', '', ''); ?></td>
            </tr>
            <tr>
                <td>First Name</td>
                <td><input type="text" name="profile_firstname" value="<?= $_POST['profile_firstname']; ?>"></td>
            </tr>
            <tr>
                <td>Last Name</td>
                <td><input type="text" name="profile_lastname" value="<?= $_POST['profile_lastname']; ?>"></td>
            </tr>
            <tr>
                <td>Company's Name</td>
                <td><input type="text" name="profile_legalname" value="<?= $_POST['profile_legalname']; ?>"></td>
            </tr>
            <tr>
                <td>Country</td>
                <td><?php draw_countryform_part(array(
                            'name'=>'profile_country', 
                            'class'=>'', 
                            'disabled'=>false, 
                            'required'=>false)); ?></td>
            </tr>
            <tr>
                <td>Address</td>
                <td><input type="text" name="profile_address" value="<?= $_POST['profile_address']; ?>"></td>
            </tr>
            <tr>
                <td>City</td>
                <td><input type="text" name="profile_city" value="<?= $_POST['profile_city']; ?>"></td>
            </tr>
            <tr>
                <td>Province/State</td>
                <td><?php draw_stateform_part(array(
                            'name'  => 'profile_state', 
                            'class' => '', 
                            'disabled' => false, 
                            'required' => false, 
                    )); ?>
                </td>
            </tr>
            <tr>
                <td>Zip</td>
                <td><input type="text" name="profile_zip" value="<?= $_POST['profile_zip']; ?>"></td>
            </tr>

            <tr>
                <td colspan="2"><input type="submit" value="Add"></td>
            </tr>
        </table>
    </fieldset>
    
    </form> 
    <?php

}



// same as in all-user-list-edit.tpl.php
function user_edit_new($in_admin = true, $uid = false)
{
    drupal_add_js(drupal_get_path('module', 'ncn_admin') . '/ncn_admin.js');
    // we are running this frunction from admin 
    if ($in_admin == true)
    {
        $user_id = arg(4);

        // check url supplied user id is valid
        if (!is_numeric($user_id))
        {   Header("Location:admin/user/user/list");exit;       }
                
        // load the user
        $user = user_load($user_id);
        
        if (!$user)     // check user is valid
        {   Header("Location:admin/user/user/list");exit;       }

        // set default page title
        $title = "Edit User: ".$user->profile_firstname." ".$user->profile_lastname;
        
    }
    // we are running this function from users account
    else
    {   
        $user_id = $uid;    
        $user = user_load($user_id);    // this will always work in users account

        // set default page title
        $title = "Edit My Profile";
    }

    // ---- function ----
    $tfunction = (isset($_POST['tfunction']) ? $_POST['tfunction'] : '');
    switch ($tfunction)
    {
        case "update_profile":
            
            $username_changed = false;
            $email_changed = false;
            $pass_changed = false;
            
            // has user updated their email address?
            if ($user->name != $_POST['username'])
            {
                if (update_username($user->uid, $_POST['username'])) {
                    drupal_set_message('username has been updated');
                    $username_changed = true;
                }
            }
            
            if ($user->mail != $_POST['mail'])
            {
                // check new email address is valid
                if ($error = user_validate_mail($_POST['mail']))
                {
                    drupal_set_message($error, 'error');    // email failed verfication 
                }
                // email was valid, so update in the database and flag to send new welcome email 
                else
                {
                    // save new email in database
                    $query = 'UPDATE `users` SET `mail` = :mail WHERE uid = :uid';
                    $result = db_query($query,array(':mail'=>addslashes($_POST['mail']),':uid'=>$user_id));
                    
                    // flag for confirmation email
                    $email_changed = true;
                    
                    // success message
                    drupal_set_message(t("Email address has been updated."), 'status');
                }
            }
            
            // has user updated the password?
            if (($_POST['pass'] != '') || ($_POST['pass2'] != ''))
            {
                // check match
                if ($_POST['pass'] != $_POST['pass2'])
                {
                    drupal_set_message(t("Passwords do not match."), 'error');
                }
                else
                if (strlen($_POST['pass']) < 5)
                {
                    drupal_set_message(t("Password must be at least 5 characters."), 'error');
                }
                // all good, so update it 
                else
                {
                    // update database with new password 
                    require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
                    $encrypted_pass = user_hash_password(trim($_POST['pass']));
                    
                //  $query = "UPDATE `users` SET `pass`=\"".AddSlashes($encrypted_pass)."\" WHERE uid=".$user_id;
                    $result = db_query('UPDATE {users} SET pass=:a WHERE uid=:b',
                        array(':a'=>$encrypted_pass,':b'=>$user_id));
                    
                    // flag for confirmation email
                    $pass_changed = true;

                    // success message
                    drupal_set_message(t("Password has been updated."), 'status');
                }
                
            }
            
            // update all other details
            /*update_profile_var($user->uid, 'profile_firstname', $_POST['profile_firstname']);
            update_profile_var($user->uid, 'profile_lastname', $_POST['profile_lastname']);
            update_profile_var($user->uid, 'profile_legalname', $_POST['profile_legalname']);
            update_profile_var($user->uid, 'profile_country', $_POST['profile_country']);
            update_profile_var($user->uid, 'profile_address', $_POST['profile_address']);
            update_profile_var($user->uid, 'profile_city', $_POST['profile_city']);
            update_profile_var($user->uid, 'profile_state', $_POST['profile_state']);
            update_profile_var($user->uid, 'profile_zip', $_POST['profile_zip']);*/
            
            $user->profile_firstname = $_POST['profile_firstname'];
            $user->profile_lastname = $_POST['profile_lastname'];
            $user->profile_legalname = $_POST['profile_legalname'];
            $user->profile_country = $_POST['profile_country'];
            $user->profile_address = $_POST['profile_address'];
            $user->profile_city = $_POST['profile_city'];
            $user->profile_state = $_POST['profile_state'];
            $user->profile_zip = $_POST['profile_zip'];
            
            $account = new stdClass;
            $account->uid = $user->uid;
            
            $edit = (array)$user;
            /*echo "<pre>";             
            print_r($edit);
            exit;*/
            
            profile_save_profile($edit, $account, 'Company Information');
            
            // check for no changes
            //if ($email_changed || $pass_changed || $username_changed)
            //{
                drupal_set_message(t("Profile has been updated."), 'status');
            //}

            if (isset($user->profile_memberid)&&is_member($user->profile_memberid)) {
                $result = db_query("UPDATE {member_id_pool} SET 
                                   first_name=:fn, last_name=:ln, legalname=:lgn, country=:cn, address=:ad, city=:ct, state=:st, zip=:zp WHERE member_id=:mid", 
                                   array(':fn'=>$_POST['profile_firstname'],':ln'=>$_POST['profile_lastname'],':lgn'=>$_POST['profile_legalname'],
                                         ':cn'=>$_POST['profile_country'],':ad'=>$_POST['profile_address'],':ct'=>$_POST['profile_city'], 
                                         ':st'=>$_POST['profile_state'],':zp'=>$_POST['profile_zip'],':mid'=>$user->profile_memberid)
                                  );
            }
            // we need to reload user
            $user = user_load($user_id);
        break;
        case "upload_member_logo":
            ncn_admin_upload_member_logo($_REQUEST['member_id']);
            break;
        case "remove_member_logo":
            ncn_admin_remove_member_logo($_REQUEST['member_id']);
            break;
        case "update_user_role":
            if (ncn_admin_update_user_role($user, $_POST['user_role'])) {
                drupal_set_message(t("User role has been updated."), 'status');
                $user = user_load($user_id);
            } else {
                drupal_set_message(t("Failed to update user role."), 'error');
            }
            break;
        case "update_dist_asso":
            $owner_id = 0; 
            if (isset($user->profile_memberid)) {
                $owner_id = get_owner($user->profile_memberid);
                $new_type = $_POST['dist_asso'];
                $new_ownerid = $_POST[$new_type];
                if ($owner_id != $new_ownerid) {
                    ///////////////////////////////////////////////////////
                //  $query = "UPDATE member_id_pool SET owner=$new_ownerid WHERE member_id ='".$user->profile_memberid."'";
                    $result = db_query('UPDATE {member_id_pool} SET owner=:a WHERE member_id =:b',
                        array(':a'=>$new_ownerid,':b'=>$user->profile_memberid));
                    if ($result) {
                        drupal_set_message("Updated associated $new_type successfully.");
                        ncn_report_change_owner($user->profile_memberid, $owner_id, $new_ownerid);
                    } else {
                        drupal_set_message("Failed to update associated $new_type", "error");
                        break;
                    }
                }
            }
            break;
        case "update_account_manager":
            $am_uid = $_POST['am_uid'];
            if (!ncn_admin_assign_member_to_am($user->profile_memberid, $am_uid)) {
                drupal_set_message(t('Failed to reassign. Please report to web master.'), 'error');
            } else {
                drupal_set_message(t('Reassigned to account manager, successfully.'));
            }
            
            break;
        case "change_report_pass":
            if ( ncn_dist_change_report_pass() ) {  
            
            }
            break;
      //  $res = db_query('SELECT COUNT(*) FROM {member_cim} WHERE member_id=:a',array(':a'=>$user->profile_memberid))->fetchField();
        case "update_practicepay_profile":
            if (is_distributor($user))  // for distributors
            {
                $query = db_query('UPDATE {member_distributor_cim} SET customerProfileId=:a, customerPaymentProfileId=:b
                WHERE username=:c',array(':a'=>$_POST['customerProfileId'],':b'=>$_POST['customerPaymentProfileId'],
                    ':c'=>$user->name));
            }
            else    // for normal users
            {
                if ((db_query('SELECT COUNT(*) FROM {member_cim} WHERE member_id=:a',array(':a'=>$user->profile_memberid))->fetchField())==0) {
                    $query = db_query('INSERT INTO {member_cim} (member_id, customerProfileId, customerPaymentProfileId)
                    VALUES(:a,:b,:c)',array(':a'=>$user->profile_memberid,':b'=>$_POST['customerProfileId'],
                        ':c'=>$_POST['customerPaymentProfileId']));
                } else {
                    $query = db_query('UPDATE {member_cim} SET customerProfileId=:a, customerPaymentProfileId=:b
                    WHERE member_id=:c',array(':a'=>$_POST['customerProfileId'],':b'=>$_POST['customerPaymentProfileId'],
                        ':c'=>$user->profile_memberid));
                }
            }
            
            $result = $query;   // do the sql update
            drupal_set_message(t("PracticePay Profile ID's have been updated in the database."), 'status');
            
        break;
        case "update_member_type":
      $p_member_type = $_POST['member_type'];
      $member = get_member_from_id($user->profile_memberid);
      if ($member['member_type'] == $p_member_type) { 
        break; 
      } else {
        ncn_user_insert_log_old_member_type($user->profile_memberid, $member['member_type'], $p_member_type, date('U'));
      }
            
            $query = db_query('UPDATE {member_id_pool} SET member_type =:a WHERE member_id=:b',
                array(':a'=>$p_member_type,':b'=>$user->profile_memberid));
            
            if ($p_member_type==5) {
                $member_type_name = 'CSI Member';
            } else if ($p_member_type==4) {
                $member_type_name = 'NCN Gold Coach Member';    
            } else if ($p_member_type==3) {
                $member_type_name = 'NCN Coach on Call Member';
            } else if ($p_member_type==2) {
                $member_type_name = 'NCN Gold Lite Member';
            } else if ($p_member_type==1) {
                $member_type_name = 'NCN Silver Member';
            } else {
                $member_type_name = 'NCN Gold Member';
            }
            
            $result = $query;
            if ($result) {
                drupal_set_message(t('Member Type updated to !member_type_name successfully', array('!member_type_name'=>$member_type_name)) );
            } else {
                drupal_set_message(t('Failed to update member type.'), 'error');
            }
        break;
    case "cancel_pause_member":
      $result = ncn_admin_cancel_pause_member($user->uid);
      $user = user_load($user->uid);
      break;
    case "unblock_member":
      $result = ncn_admin_unblock_member($user->uid);
      $user = user_load($user->uid);
      break;
    case "change_member_renewal_date":
      $result = ncn_user_change_member_renewal_day($user->profile_memberid, (int)($_POST['ncn_admin_member_renewal_date']));
      if ($result) {
        drupal_set_message("Success to change member renewal day");        
      }
      $user = user_load($user->uid);
      break;
        case "send_claim_reminder_mail": 
            $result = ncn_admin_send_reminder_mail($user->uid);
        break;
        case "grant_subuser" :
            $grant_subuser = $_POST['grant_subuser'];
            ncn_subuser_grant_creating_subuser($user->profile_memberid, $grant_subuser);
        break;
        case "convert_demo_to_regular":
            $result = db_query('UPDATE {member_gold_demo} SET status=0 WHERE member_id=:a AND expired=0 AND status=1 ORDER BY to_expired_time DESC',
                array(':a'=>$user->profile_memberid));
            if ($result) {
                drupal_set_message(t('Convert demo to regular, successfully.'));
            } else {
                drupal_set_message(t('Failed to convert demo to regular.'), 'error');
            }
        case "update_cc":
            
            /*
            // check for free
            if ($_POST['credit_card_number'] != '5424000000000015')
            {
            
                // add the user to cim
                $content =
                    "<?xml version=\"1.0\" encoding=\"utf-8\"?>" .
                    "<createCustomerProfileRequest xmlns=\"AnetApi/xml/v1/schema/AnetApiSchema.xsd\">" .
                    MerchantAuthenticationBlock().
                    "<profile>".
                    "<merchantCustomerId>" . get_unique_auth_profile_id() . "</merchantCustomerId>". // Your own identifier for the customer.
                    "<description>".$user->profile_firstname." ".$user->profile_lastname."</description>".
                    "<email>" . $user->mail . "</email>".
                    "</profile>".
                    "</createCustomerProfileRequest>";  // <?php
                
                $response = send_xml_request($content);
                $parsedresponse = parse_api_response($response);
                    
                $customerProfileId = $parsedresponse->customerProfileId;
            
            
                // try and add the payment profile
                $date = explode("/", $_POST['credit_card_expiration']);
                
                if (sizeof($date) == 2)
                {       $date = "20".$date[1]."-".$date[0];   }
                else
                {       $date = '';     }
                
                $content =
                "<?xml version=\"1.0\" encoding=\"utf-8\"?>" .
                "<createCustomerPaymentProfileRequest xmlns=\"AnetApi/xml/v1/schema/AnetApiSchema.xsd\">" .
                MerchantAuthenticationBlock().
                "<customerProfileId>" . $customerProfileId . "</customerProfileId>".
                "<paymentProfile>".
                "<billTo>".
                 "<firstName>".$user->profile_firstname."</firstName>".
                 "<lastName>".$user->profile_lastname."</lastName>".
//               "<country>".$user->profile_country."</country>".
                 "<address>".$user->profile_address."</address>".
                 "<city>".$user->profile_city."</city>".
                 "<state>".$user->profile_state."</state>".
                 "<zip>".$user->profile_zip."</zip>".
                 "<phoneNumber>000-000-0000</phoneNumber>".
                "</billTo>".
                "<payment>".
                 "<creditCard>".
                  "<cardNumber>".$_POST['credit_card_number']."</cardNumber>".
                  "<expirationDate>".$date."</expirationDate>". // required format for API is YYYY-MM
                  "<cardCode>".$_POST['credit_card_security']."</cardCode>".
                 "</creditCard>".
                "</payment>".
                "</paymentProfile>".
                "<validationMode>liveMode</validationMode>". // or liveMode
                "</createCustomerPaymentProfileRequest>";   // <?php
            
                $response = send_xml_request($content);
                $parsedresponse = parse_api_response($response);
                $customerPaymentProfileId = $parsedresponse->customerPaymentProfileId;

                // error check
                if ($parsedresponse->messages->resultCode == 'Error')
                {
                    $error = "Credit/debit card details are invalid/expired.";
                    drupal_set_message($error, 'error');
                }
                else
                {
                    // success 
                    drupal_set_message(t("Payment profile updated with new credit card."), 'status');
                }

            }   // END check free
            else
            {
                // save this profile as free!
                $customerProfileId = "FREE";
                $customerPaymentProfileId = "FREE";
                
                drupal_set_message(t("Credit card updated to FREE."), "status");
            }       
            
        
            // update the payment profile id# in the database
            if (!isset($error))
            {
                // now update the actual card details in the profile
                //update_profile_var($user->uid, 'profile_credit_card_number', $_POST['credit_card_number']);
                //update_profile_var($user->uid, 'profile_credit_card_expiration', $_POST['credit_card_expiration']);
                update_profile_var($user->uid, 'profile_credit_card_security', $_POST['credit_card_security']);

                // switch for distributor/user
                if (is_distributor($user))
                {
                    $query = "UPDATE member_distributor_cim SET customerProfileId=\"".AddSlashes($customerProfileId)."\",customerPaymentProfileId=\"".AddSlashes($customerPaymentProfileId)."\" WHERE username=\"".AddSlashes($user->name)."\";";
                    $result = db_query($query);
                }
                else    // normal users
                {
                    $query = "UPDATE member_cim SET customerProfileId=\"".AddSlashes($customerProfileId)."\",customerPaymentProfileId=\"".AddSlashes($customerPaymentProfileId)."\" WHERE member_id=\"".AddSlashes($user->profile_memberid)."\";";
                    $result = db_query($query);
                }
                
            }   // END there were no errors
            */  
        if ($_POST['credit_card_security']!="") {
            update_profile_var($user->uid, 'profile_credit_card_security', $_POST['credit_card_security']);
        //  $query = "UPDATE member_id_pool SET security_code='".$_POST['credit_card_security']."' WHERE member_id='".$user->profile_memberid."'";
            $result = db_query('UPDATE {member_id_pool} SET security_code=:a  WHERE member_id=:b',
            array(':a'=>$_POST['credit_card_security'],':b'=>$user->profile_memberid));
            $user = user_load($user->uid);
        }
        break;
        case "update_org_party_id":
            $org_id = intval($_POST['org_id']);
            $person_id = intval($_POST['person_id']);
            if (module_exists('ncn_capsulecrm')) {
                if (ncn_capsulecrm_save_map_info($user->profile_memberid, $org_id, $person_id) ) {
                    ncn_capsulecrm_set_user_detail_link($user->uid);
                    drupal_set_message("Success to update capsulecrm party id");
                } else {
                    drupal_set_message("Failed to update capsulecrm party id", 'error');
                }
            }
        break;
    case "update_capsulecrm_user_token":
      $casulecrm_user_token = $_POST['casulecrm_user_token'];
      if (module_exists('ncn_capsulecrm')) {
          if (ncn_capsulecrm_token_save_user_token($user->uid, $casulecrm_user_token)) {
            drupal_set_message("Success to set capsulecrm user token");
          } else {
            drupal_set_message("Failed to set capsulecrm user token", 'error');
          }
        }
    break;
    
    case "update_capsulecrm_account":
      $capsulecrm_account = $_POST['casulecrm_account'];
      if (module_exists('ncn_capsulecrm')) {
          if (ncn_capsulecrm_account_save_account($user->uid, $capsulecrm_account)) {
            drupal_set_message("Success to set capsulecrm account");
          } else {
            drupal_set_message("Failed to set capsulecrm account", 'error');
          }
      }
    break;
    }
    
    // ---- show the form ----
    ?>
    <!-- <h2><?= $title; ?></h2> -->
    
    <fieldset class="edit_user_group">
    <?php if(isset($user->profile_memberid)&&is_member($user->profile_memberid)): ?>
    <?php 
        $extra_field_url = 'account/my-extra-profile.html';
        if (arg(0) == 'admin') {
            $extra_field_url = 'admin/config/ncn_create_user/edit_user_extra_profile/'.$user->uid;
        }
    ?>
        <div style="float: right; margin-top: 5px;"><a href="<?php echo base_path().$extra_field_url; ?>">Edit extra profiles</a></div>
    <?php endif;?>
    <h2 class="sub-title">Profile</h2>

    <p style="font-style: italic; font-size: 11px;">Password is optional.</p>
    <p>For example, if you only wish to update users email address, leave all other fields blank.</p>
    
    <form method="POST" id="update_profile_form">
    <input type="hidden" name="tfunction" value="update_profile">
    <table style="width:100%;">
        <tr>
            <td class="td-label">Username:</td>
            <td><input type="text" name="username" id="username" value="<?= $user->name; ?>" style="width:200px;" <?php if ($GLOBALS['user']->uid!=1 && !user_access('ncn_admin edit username')) {echo "readonly";} ?>></td>
        </tr>
        <!-- <tr>
            <td class="td-label">Name:</td>
            <td><input type="text" disabled="disabled" value="<?= $user->profile_firstname; ?> <?= $user->profile_firstname; ?>"  style="width:200px;"></td>
        </tr>
        <tr>
            <td class="td-label">Company:</td>
            <td><input type="text" disabled="disabled" value="<?= $user->profile_legalname; ?>"  style="width:200px;"></td>
        </tr> -->
        <tr>
            <td class="td-label">Email:</td>
            <td><input type="text" name="mail" value="<?= $user->mail; ?>"  style="width:200px;"></td>
        </tr>
        <tr>
            <td class="td-label">Password:</td>
            <td><input type="password" name="pass" value=""  style="width:200px;"></td>
        </tr>
        <tr>
            <td class="td-label">Confirm Password:</td>
            <td><input type="password" name="pass2" value=""  style="width:200px;"></td>
        </tr>
        
        <tr>
            <td class="td-label">First Name:</td>
            <td><input type="text" name="profile_firstname" value="<?= $user->profile_firstname; ?>">
        </tr>
        <tr>
            <td class="td-label">Last Name:</td>
            <td><input type="text" name="profile_lastname" value="<?= $user->profile_lastname; ?>">
        </tr>
        <tr>
            <td class="td-label">Company's Name:</td>
            <td><input type="text" name="profile_legalname" value="<?= $user->profile_legalname; ?>">
        </tr>
        <tr>
            <td class="td-label">Country:</td>
            <td><?php draw_countryform_part_sel(array(
                            'name'=>'profile_country', 
                            'class'=>'', 
                            'disabled'=>false, 
                            'required'=>false, 
                            'sel_val' =>$user->profile_country)); ?>
            </td>
        </tr>
        <tr>
            <td class="td-label">Address:</td>
            <td><input type="text" name="profile_address" value="<?= $user->profile_address; ?>">
        </tr>
        <tr>
            <td class="td-label">City:</td>
            <td><input type="text" name="profile_city" value="<?= $user->profile_city; ?>">
        </tr>
        <tr>
            <td class="td-label">Province/State:</td>
            <td><?php draw_stateform_part_sel(array(
                            'name'  => 'profile_state', 
                            'class' => '', 
                            'disabled' => false, 
                            'required' => false, 
                            'sel_val'  => $user->profile_state, 
                            'country'  => $user->profile_country, 
                    )); ?>
            </td>
        </tr>
        <tr>
            <td class="td-label">Zip:</td>
            <td><input type="text" name="profile_zip" value="<?= $user->profile_zip; ?>">
        </tr>
        
        <tr>
            <td class="td-label"></td>
            <td><a class="update-profile-btn" href="javascript:void(0)" onclick="update_profile_submit();">Update Profile</a></td>
        </tr>
    </table>
    </form>
    
    <?php if(isset($user->profile_memberid)&&is_member($user->profile_memberid)) : ?>
    <!-- Upload Member Logo-->
    <form id="upload_member_logo_form" method="POST" enctype="multipart/form-data">
        <input type="hidden" id="member_logo_tfunction" name="tfunction" value="upload_member_logo" />
        <input type="hidden" name="member_id" value="<?php echo $user->profile_memberid; ?>" />
        <table style="width:100%; margin-top: 30px;">
        <tr>
            <td class="td-label">Member's logo:</td>
            <td>
                    <?php if ($member_logo_path = ncn_admin_get_member_logo($user->profile_memberid)): ?>
                        <img src="<?php echo base_path().$member_logo_path; ?>" style="width: 300px"/> <br/>
                    <?php endif; ?>
                    <input type="file" size="25" accept="image/jpg, image/jpeg, image/gif, image/png" id="member_logo" name="files[member_logo]" /></td>
        </tr>
        <tr>
            <td class="td-label"></td>
            <td><a class="upload-logo-btn" href="javascript:void(0)" onclick="upload_member_logo();">Upload Member Logo</a>
                <?php if ($member_logo_path = ncn_admin_get_member_logo($user->profile_memberid)): ?>
                    <a class="remove-logo-btn" href="javascript:void(0)" onclick="remove_member_logo();" style="margin-left: 15px;">Remove Member Logo</a>
                <?php endif; ?>
            </td>
        </tr>
        </table>
    </form>
    <?php endif; ?>
    </fieldset>
    
    <?php if(isset($user->profile_memberid) && ncn_admin_is_gold_member_demo($user->profile_memberid)): ?>
    <?php
        $result = db_query('SELECT to_expired_time, num_claims FROM {member_gold_demo} WHERE member_id=:a AND expired=0 AND status=1 ORDER BY to_expired_time DESC',
            array(':a'=>$user->profile_memberid));
        if ($result) {
//          if ($row=db_fetch_array($result))
            foreach($result as $row)
            {
                $row = (array)$row;
                if($row)
                {
                    $expired_time = date('m/d/Y H:i', $row['to_expired_time']);
                    $num_claims = $row['num_claims'];
                }
            }
        }
    ?>
    <fieldset class="edit_user_group">
        <h2 class="sub-title">Gold Member Demo</h2>
        <form method="POST" id="convert_demo_to_regular">
        <input type="hidden" name="tfunction" value="convert_demo_to_regular">
        <table style="width:100%;">
            <tr>
                <td class="td-label">Expired Time:</td>
                <td>
                    <?php echo $expired_time; ?>
                </td>
            </tr>
            <tr>
                <td class="td-label">Created Free Claims:</td>
                <td>
                    <?php echo $num_claims; ?>
                </td>
            </tr>
            <tr><td colspan='2'>&nbsp;</td></tr>
            <tr>
                <td colspan='2'><input type="submit" value="Convert to Regular Member" /></td>
            </tr>
        </table>
        </form>
    </fieldset> 
    <?php endif; ?>
    
    <?php   // Role
        if ($in_admin && !(isset($user->profile_memberid)&&is_member($user->profile_memberid)) && !is_distributor($user) && !is_associate($user) && !is_subuser($user) ): 
    ?>
    <?php 
        $select_role = 1;
        foreach ($user->roles as $rid=>$role_name)
        {
            $select_role = $rid;
        }
    ?>
    <fieldset class="edit_user_group">
        <h2 class="sub-title">User Role</h2>
        <form method="POST" id="update_user_role">
        <input type="hidden" name="tfunction" value="update_user_role">
        <table style="width:100%;">
            <tr>
                <td class="td-label">Role:</td>
                <td>
                    <?php echo ncn_admin_render_select_role('user_role', $select_role, ''); ?>
                </td>
            </tr>
            <tr>
                <td colspan='2'><input type="submit" value="Save" /></td>
            </tr>
        </table>
        </form>
    </fieldset>
    
    <?php endif; // Role?>
    
    <?php if ($GLOBALS['user']->uid==1 || user_access('ncn capsule_crm management')) { ?>
    <fieldset class="edit_user_group">
    <?php 
      if (isset($user->profile_memberid)) {
          $organization_id = 0; $person_id = 0;
          if (module_exists('ncn_capsulecrm')) {
              $cc_map_info = ncn_capsulecrm_get_map_info($user->profile_memberid);
              if ($cc_map_info) {
                $organization_id = $cc_map_info['organization_id'];
                $person_id       = $cc_map_info['person_id'];
                $_url_cc_org = ncn_capsulecrm_get_url()."/party/".$organization_id;
              }
          }
      }
    ?>
    <?php if (module_exists('ncn_capsulecrm')): ?>
    <?php if ( isset($user->profile_memberid) && is_member($user->profile_memberid) && ($GLOBALS['user']->uid==1 || user_access('ncn capsule_crm management')) ) { ?>
        <h2 class="sub-title">CapsuleCRM</h2>
        <form method="POST" id="update_org_party_id">
        <input type="hidden" name="tfunction" value="update_org_party_id">
        <table style="width:100%;">
            <tr>
                <td class="td-label">Organization Party ID:</td>
                <td><input type="text" name="org_id" value="<?php echo $organization_id; ?>" />&nbsp;&nbsp;<?php if (isset($_url_cc_org)) { print l($_url_cc_org, $_url_cc_org, array('attributes'=>array('target'=>'_blank'))); } ?></td>
            </tr>
            <tr>
                <td class="td-label">Person Party ID:</td>
                <td><input type="text" name="person_id" value="<?php echo $person_id; ?>" /></td>
            </tr>
            <tr>
                <td colspan='2'><input type="submit" value="Update" /></td>
            </tr>
        </table>
        </form>
    <?php } ?>
    <?php endif; ?>

    <?php if (module_exists('ncn_capsulecrm')): ?>
    <h2 class="sub-title">CapsuleCRM Account</h2>
    <form method="POST" id="update_capsulecrm_account_form">
    <input type="hidden" name="tfunction" value="update_capsulecrm_account">
    <table style="width:100%;">
      <tr>
        <td class="td-label">Account:</td>
        <td><?php print ncn_capsulecrm_account_select_render("casulecrm_account", ncn_capsulecrm_account_get_account($user->uid)); ?></td>
      </tr>
      <tr>
        <td colspan='2'><input type="submit" value="Set" /></td>
      </tr>
    </table>
    </form>
    <?php endif; ?>
    
    </fieldset>
    <?php } ?>
    
    <?php 
    if ( ($GLOBALS['user']->uid==1 || user_access('ncn_admin edit member type')) && (isset($user->profile_memberid)&&is_member($user->profile_memberid))) {
        $member_type = get_member_type($user);
    ?>
    <fieldset class="edit_user_group">
        <h2 class="sub-title">Member Type</h2>
        <form method="POST" id="update_member_type">
        <input type="hidden" name="tfunction" value="update_member_type">
        <table style="width:100%;">
            <tr>
                <td class="td-label">Member Type:</td>
                <td>
                    <select id="member_type" name="member_type">
                        <option value="0" <?php if($member_type == 0){ echo "selected"; } ?> >NCN Gold Member</option>
                        <option value="1" <?php if($member_type == 1){ echo "selected"; } ?> >NCN Silver Member</option>
                        <option value="2" <?php if($member_type == 2){ echo "selected"; } ?> >NCN Gold Lite Member</option>
                        <option value="3" <?php if($member_type == 3){ echo "selected"; } ?> >NCN Coach on Call Member</option>
                        <option value="4" <?php if($member_type == 4){ echo "selected"; } ?> >NCN Gold Coach Member</option>
                        <option value="5" <?php if($member_type == 5){ echo "selected"; } ?> >CSI Member</option>
                    </select>
                </td>
            </tr>
            <tr>
                <td colspan='2'><input type="submit" value="Update Member Type" /></td>
            </tr>
        </table>
        </form>
    </fieldset>
    
  <fieldset class="edit_user_group">
    <h2 class="sub-title">Cancel/Pause Member</h2>
    <?php print ncn_user_show_block_track_table($user->uid); ?>
    <!--
    <?php if ($user->status): ?>
    <form method="POST" id="cancel_pause_member">
    <input type="hidden" name="tfunction" value="cancel_pause_member">
      <div>
        <label for="returned_datepicker">Returned Date: </label><input type="text" id="returned_datepicker" name="returned_datepicker" size="20" readonly>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <input type="submit" value="Pause Member" onclick="return confirm('Are you sure you want to block member?');" />
        <script>
          $(function() {
            $( '#returned_datepicker' ).datepicker({
              showOn: "button",
              buttonImage: "<?php global $base_url; print $base_url.drupal_get_path('module', 'ncn_report')."/images/calendar.gif"; ?>",
              buttonImageOnly: true,
              dateFormat: 'yy/mm/dd',
            });
          });
        </script>
      </div>
      <div class="description">If you don't specify returned date, it would cancel member.</div>
    </form>
    <?php else: ?>
    <form method="POST" id="unblock_member">
      <input type="hidden" name="tfunction" value="unblock_member">
      <input type="submit" value="Unblock Member" onclick="return confirm('Are you sure you want to unblock member?');" />
    </form>
    <?php endif; ?>
    -->
  </fieldset>

  <fieldset class="edit_user_group">
    <h2 class="sub-title">Member Renewal Date</h2>
    <form method="POST" id="change_member_renewal_date">
    <input type="hidden" name="tfunction" value="change_member_renewal_date">
    <div style="margin-top: 20px;">
      <?php print render_ncn_admin_member_renewal_select("ncn_admin_member_renewal_date", $user->profile_memberid); ?>&nbsp;&nbsp;<label>on Every Month</label>
      &nbsp;&nbsp;&nbsp;&nbsp;
      <input type="submit" value="Change" />
    </div>
    </form>
  </fieldset>
    
    <fieldset class="edit_user_group">
        <h2 class="sub-title">Send Claim Reminder Mail</h2>
        <form method="POST" id="send_claim_reminder_mail">
        <input type="hidden" name="tfunction" value="send_claim_reminder_mail">
        <table style="width:100%;">
            <tr>
                <td>
                    <input type="submit" value="Send Claim Reminder Mail" />
                </td>
            </tr>
        </table>
        </form>
    </fieldset>
    
    <?php $grant_subuser = ncn_subuser_has_grant_to_create_subuser(isset($user->profile_memberid)?$user->profile_memberid:0); ?>
    <fieldset class="edit_user_group">
        <h2 class="sub-title">Grant to create sub user</h2>
        <form method="POST" id="grant_subuser_form">
        <input type="hidden" name="tfunction" value="grant_subuser">
        <table style="width:100%;">
            <tr>
                <td class="td-label">Give a grant to create sub user:</td>
                <td>
                    <select id="grant_subuser" name="grant_subuser">
                        <option value="0" <?php if($grant_subuser == FALSE){ echo "selected"; } ?> >No</option>
                        <option value="1" <?php if($grant_subuser == TRUE){ echo "selected"; } ?> >Yes</option>
                    </select>
                </td>
            </tr>
            <tr>
                <td colspan='2'><input type="submit" value="Save" /></td>
            </tr>
        </table>
        </form>
    </fieldset>
    <?php } ?>
    
    
    <?php 
    if ($in_admin && isset($user->profile_memberid) && is_member($user->profile_memberid)) { 
        $owner_id = get_owner($user->profile_memberid);
        if ($owner_id != 0) {
    ?>
    <fieldset class="edit_user_group">
        <h2 class="sub-title">Distributor/Associate</h2>
        
        <form method="POST" id="update_dist_asso_form">
        <input type="hidden" name="tfunction" value="update_dist_asso">
        <table style="width:100%;">
            <tr class="distributor-list">
                <td class="td-label">Type:</td>
                <td>
                    <select id="dist_asso" name="dist_asso">
                        <option value="distributor" <?php if(is_distributor(user_load($owner_id))){ echo "selected"; } ?> >Distributor</option>
                        <option value="associate"   <?php if(is_associate  (user_load($owner_id))){ echo "selected"; } ?> >Associate</option>
                    </select>
                </td>
            </tr>
            <tr class="distributor-list da-dropdown">
                <td class="td-label">Distributor:</td>
                <td><?php distributor_dropdown($owner_id); ?></td>
            </tr>
            <tr class="associate-list da-dropdown">
                <td class="td-label">Associate</td>
                <td><?php associate_dropdown($owner_id); ?></td>
            </tr>
            <tr>
                <td colspan='2'><input type="submit" value="Update Distributor/Associate" /></td>
            </tr>
        </table>
        </form>
    </fieldset>
        
    <?php
        } else {
            drupal_set_message("Owner of this user isn't setted", "error");
        }
    }
    
    // Update Account Manager
    if ($in_admin && isset($user->profile_memberid) && is_member($user->profile_memberid)) { 
        $am_uid = 0;
    //  $query = "SELECT * FROM member_id_pool WHERE member_id=".$user->profile_memberid;
        $result = db_query('SELECT * FROM {member_id_pool} WHERE member_id=:a',array(':a'=>$user->profile_memberid));
        if ($result->rowCount()>0) {
            $row = $result->fetchAssoc();
        //  $row = (array)$result;
            $am_uid = $row['am_uid'];
        }
    ?>
    <fieldset class="edit_user_group">
        <h2 class="sub-title">Account Manager</h2>
        
        <form method="POST" id="update_account_manager_form">
        <input type="hidden" name="tfunction" value="update_account_manager">
        <table style="width:100%;">
            <tr class="am-list">
                <td class="td-label">Account Manager:</td>
                <td>
                    <?php echo draw_select_account_manager_list('am_uid',  $am_uid); ?>
                </td>
            </tr>
            <tr>
                <td colspan='2'><input type="submit" value="Update" /></td>
            </tr>
        </table>
        </form>
    </fieldset>
        
<?php
    }
        
            
    if ((is_distributor($user) == true) && stristr($_SERVER['REQUEST_URI'], 'admin')) {
                // show credit card form
?>
        <fieldset class="edit_user_group">
        <h2 class="sub-title">Report Password</h2>
        <!-- <legend>Credit Card Details</legend> -->
        <form method="POST" id="change_report_pass">
            <input type="hidden" name="tfunction" value="change_report_pass" />
            <input type="hidden" name="username" value="<?= $user->name; ?>">
            <table style="width:100%;border:0;">
                <tr>
                    <td class="td-label">Old Password:</td>
                    <td><input type="password" name="old_pass" ></td>
                </tr>
                <tr>
                    <td class="td-label">New Password:</td>
                    <td><input type="password" name="new_pass" ></td>
                </tr>
                <tr>
                    <td class="td-label">Confirm Password:</td>
                    <td><input type="password" name="confirm_pass" ></td>
                </tr>
                <tr>
                    <td class="td-label">&nbsp;</td>
                    <td><input type="submit" value="Update" ></td>
                </tr>
            </table>
        </form>
        </fieldset>
<?php 
    }
    
    // only show credit card and document upload for non account managers
    
    if ($in_admin && (is_distributor($user) || (isset($user->profile_memberid) &&is_member($user->profile_memberid))))
    {
        // show credit card form
        ?>
        <fieldset class="edit_user_group">
        <h2 class="sub-title">Credit Card Details</h2>
        <!-- <legend>Credit Card Details</legend> -->
        
        <form method="POST" id="update_credit_card_form">
        <input type="hidden" name="tfunction" value="update_cc">
        <table style="width:100%;">
            <!-- <tr>
                <td class="td-label">Credit Card Number:</td>
                <td><input type="text" name="credit_card_number" style="width:200px;"></td>
            </tr>
            <tr>
                <td class="td-label">Expiration Date (mm/yy):</td>
                <td><input type="text" name="credit_card_expiration" style="width:200px;"></td>
            </tr> -->
            <tr>
                <td class="td-label">Security Code:</td>
                <td><input type="text" name="credit_card_security" value="<?php echo $user->profile_credit_card_security; ?>" style="width:200px;"></td>
            </tr>
            <tr>
                <td class="td-label"></td>
                <td><a class="update-credit-card-btn" href="#" onclick="update_credit_card_submit();">Update</a></td>
            </tr>
        </table>
        </form>
        </fieldset>
        <?php
        
        // show edit practicepay profile id#'s form for these users (but only in admin)
        if (stristr($_SERVER['REQUEST_URI'], 'admin') && (is_distributor($user) || (isset($user->profile_memberid)&&is_member($user->profile_memberid))))
        {
            // get the current data
            $cim_profile = get_cim_profile_data($user); // see function below
            
            // show the form part
            ?>
            <fieldset class="edit_user_group">
            <h2 class="sub-title">Practice Pay Solutions Profile Data</h2>
            <!-- <legend>Practice Pay Solutions Profile Data</legend> -->
            
            <form method="POST">
            <input type="hidden" name="tfunction" value="update_practicepay_profile">
                <table style="width:100%;">
                    <tr>
                        <td class="td-label">customerProfileId</td>
                        <td><input type="text" name="customerProfileId" value="<?= $cim_profile['customerProfileId']; ?>" style="width:200px;" />
                    </tr>
                    <tr>
                        <td class="td-label">customerPaymentProfileId</td>
                        <td><input type="text" name="customerPaymentProfileId" value="<?= $cim_profile['customerPaymentProfileId']; ?>" style="width:200px;" />
                    </tr>
                    <tr>
                        <td class="td-label"></td>
                        <td><input type="submit" value="Update PracticePay Profile"></td>
                    </tr>
                </table>
            </fieldset>
            <?php
        }
                
    }

    // document center additions    
    if (false) {
    if ((is_account_manager($user) == false) && (is_distributor($user) == false) && (stristr($_SERVER['REQUEST_URI'], 'admin')))
    {
        // document upload and list
        ?>
        <fieldset class="edit_user_group">
        <h2 class="sub-title">User Documents</h2>
        <!--<legend>User Documents</legend>-->
        
            <?php
            print drupal_get_form('ncn_admin_upload_document'); 
            
            // see if there are any documents
        //  $query = "SELECT * FROM member_documents WHERE uid=".$user->uid." ORDER BY title ASC;";
            $result = db_query('SELECT * FROM {member_documents} WHERE uid=:a ORDER BY title ASC',
            array(':a'=>$user->uid));
            $row_count = $result->rowCount();
            
            if ($row_count > 0)
            {
                ?>
                <div style="width:500px;">
                <table class="sticky-enabled tableSelect-processed sticky-table page_table" id="page_table_1">
                    <tbody>
                <?php                           
            
                for ($i=0; $i<$row_count; $i++)
                {
                    $row = (array)$result;
                    
                    ?>
                    <tr <?php if ($i % 2) { print 'class="odd"'; } else { print 'class="even"'; }?> >
                        <td><a href="/account/serve_user_document/<?= $row['id']; ?>/<?= $row['uid']; ?>"><?= StripSlashes($row['title']); ?></a></td>
                        <td><a class='delete_link' href="/admin/delete_user_document/<?= $row['id']; ?>/<?= $row['uid']; ?>" onclick="return confirm('Are you sure you want to delete this document?');">[delete]</td>
                    </tr>
                    <?php
                    
                }
                
                ?>
                    </tbody>
                    </table>    
                    </div>          
                <?php
            }
            // no documents found
            else
            {
                ?>No documents have been uploaded for this user.<?php
            }
        
            ?>
        
        </fieldset>
        <?php
    }}
}

//------------------------------------------------------------------------------
// upload user specific document
function ncn_admin_upload_document($form_state)
{

    $form = array();
    $form['#attributes'] = array('enctype' => "multipart/form-data");

    $form['op'] = array(
        '#type' => 'user_edit',
        '#value' => 'true',
    );

    $form['document_title'] = array(
        '#type' => 'textfield', 
        '#title' => t('Document Title'),
    );

    $form['document_upload'] = array(
        '#type' => 'file',
        '#title' => t(''),
    );
    
    $form['current_scroll_position'] = array(
        '#type' => 'hidden',
        '#attributes' => array('class' => t('current_scroll_position')),
        '#value' => t(''),
    );
    
    $form['next'] = array('#type' => 'submit', '#weight' => 101, '#value' => t('Upload Document'), '#attributes' => array('onclick' => 'return on_set_scroll_position()'));

    return($form);

}

//------------------------------------------------------------------------------
function ncn_admin_upload_document_validate($form, $form_state)
{

    // check title was long enough
    
    if (strlen($form_state['values']['document_title']) < 6)
    {
        // If you want to require it, you'll want to do it here... something like this:
        form_set_error('document_title', 'Document title must be at least 6 characters.');
        return;
    }

    // check file was uploaded
    $file = file_save_upload('document_upload');

    if (!$file)
    {
        // If you want to require it, you'll want to do it here... something like this:
        form_set_error('document_upload', 'File missing for upload.');
    }
  
}

//------------------------------------------------------------------------------
function ncn_admin_upload_document_submit($form, $form_state)
{
    $user_id = arg(4);

    if(isset($form_state['clicked_button']['#post']['current_scroll_position']))
    {
        $_SESSION['current_scroll_position'] = $form_state['clicked_button']['#post']['current_scroll_position'];
    } 

    // we know the file was good
    $file = file_save_upload('document_upload');
    $res = file_copy($file, 'sites/default/files/user_files/');
    
    // add invoice to database
    if ($res)
    {
    //  $query = "INSERT INTO member_documents VALUES(NULL, ".$user_id.", \"".AddSlashes($form_state['values']['document_title'])."\", \"".AddSlashes($file->filepath)."\", \"".AddSlashes($file->filename)."\",  \"".AddSlashes($file->filemime)."\");";
        $result = db_query('INSERT INTO {member_documents} VALUES(NULL,:a,:b,:c,:d,:e)',
            array(':a'=>$user_id,':b'=>$form_state['values']['document_title'],':c'=>$file->filepath,':d'=>$file->filename,
            ':e'=>$file->filemime));

        drupal_set_message('Uploaded new document.', 'status');
            
    }
    
}
