<?php

/**
 * account manager list(page) - assignability
 * @url : admin/user/user/account_manager_list
 */

function ncn_admin_account_manager_list()
{
    GLOBAL $base_url;
    if (arg(4) == 'view')
    {
        ncn_admin_am_view_members_page(intval(arg(5)));
        return;
    }

    ncn_admin_account_manager_list_action();
    ncn_admin_update_am_auto_assign();
?>

<div id="ncn_admin_account_manager_list">

<form id="account_manager_auto_assign_form" method="POST">

<input type="hidden" name="tfunction" value="auto_assign" />

<table class="sticky-enabled tableSelect-processed sticky-table">

    <thead class="tableHeader-processed">

        <th>UID</th>

        <th>Username</th>

        <th>Status</th>

        <th>Email</th>

        <th>Auto-Assignable</th>

        <th># of Assigned Members </th>

        <th></th>

    </thead>

    <tbody>

<?php
    $am_list = get_account_manager_list();
    $index = 0;
    foreach($am_list as $am):
        $_user_am = user_load($am['uid']);
        $auto_assign = 0;
        $n_members = 0;
    //  $query = "SELECT * FROM am_auto_assign WHERE am_uid=" . $am['uid'];
        $result = db_query('SELECT * FROM {am_auto_assign} WHERE am_uid=:a',array(':a'=>$am['uid']));
        if ($result->rowCount() > 0)
        {
            $row = $result->fetchAssoc();
            $auto_assign = $row['auto_assign'];
            $n_members = $row['members'];
        }

        $index++;
        $class = ($index % 2) ? 'odd' : 'even';
?>

    <tr class="<?php
        echo $class; ?>">

        <td><?php
        echo $_user_am->uid
?></td>

        <td><?php
        echo $_user_am->profile_firstname . ' ' . $_user_am->profile_lastname; ?></td>

        <td><?php
        echo ($_user_am->status) ? 'active' : 'blocked'; ?></td>

        <td><?php
        echo $_user_am->mail; ?></td>

        <td><input type="checkbox" name="p_auto_assign[<?php
        echo $_user_am->uid
?>]" <?php
        if ($auto_assign) echo 'checked'; ?> /></td>

        <td><?php
        echo $n_members; ?></td>

        <td>

            <?php
        if ($n_members): ?> <a href="<?php
            echo $base_url; ?>/admin/user/user/account_manager_list/view/<?php
            echo $_user_am->uid
?>">View</a> <?php
        endif; ?>

            </td>

    </tr>

<?php
    endforeach; ?>



    </tbody>

</table>



<div class="form-submit-panel">

    <input type="submit" value=" Save " />

</div>

</form>

</div>

<?php
}

/**
 * update auto-assign of account managers (action)
 */

function ncn_admin_am_view_members_page($am_uid)
{
    GLOBAL $base_url;
    $_user = user_load($am_uid);
    drupal_set_title(t('Members List (AM: !am_name)', array(
        '!am_name' => $_user->profile_firstname . ' ' . $_user->profile_lastname
    )));
?>

<div id="ncn_admin_am_view_members_page">

<table class="sticky-enabled tableSelect-processed sticky-table">

    <thead class="tableHeader-processed">

        <th>Member ID</th>

        <th>Username</th>

        <th>Status</th>

        <th>Member Type</th>

        <th></th>

    </thead>

    <tbody>

<?php
    $query = db_query('SELECT * FROM member_id_pool WHERE am_uid=:a',array(':a'=>$am_uid));
    $result = $query;
    $r_count = $result->rowCount();
    for ($i = 0; $i < $r_count; $i++):
        $member = $result->fetchAssoc();
        $uid = get_uid_from_memberid($member['member_id']);
        $status = '';
        if ($uid == 0)
        {
            $status = 'unactivated';
        }
        else
        {
            $_user = user_load($uid);
            $status = ($_user->status) ? 'active' : 'blocked';
        }

        $class = ($i % 2) ? 'even' : 'odd';
        if ($data[$i]['member_type'] == 0)
        {
            $str_member_type = "Gold";
        }
        else
        if ($data[$i]['member_type'] == 1)
        {
            $str_member_type = "Silver";
        }
        else
        if ($data[$i]['member_type'] == 2)
        {
            $str_member_type = "Gold Lite";
        }
        else
        if ($data[$i]['member_type'] == 3)
        {
            $str_member_type = "Coach on Call";
        }
        else
        if ($data[$i]['member_type'] == 4)
        {
            $str_member_type = "Gold Coach";
        }
        else
        if ($data[$i]['member_type'] == 5)
        {
            $str_member_type = "CSI";
        }

?>

    <tr class="<?php
        echo $class; ?>">

        <td><?php
        echo $member['member_id'] ?></td>

        <td><?php
        echo $member['first_name'] . ' ' . $member['last_name']; ?></td>

        <td><?php
        echo $status; ?></td>

        <td><?php
        echo $str_member_type; ?></td>

        <td><?php
        if ($uid): ?>

                <a href="<?php
            echo $base_url; ?>/admin/user/user/edit_user/<?php
            echo $uid; ?>">Edit</a>

            <?php
        endif; ?>

            </td>

    </tr>

<?php
    endfor; ?>



    </tbody>

</table>

</div>

<?php
    return;
}

/**
 * update auto-assign of account managers (action)
 */

function ncn_admin_account_manager_list_action()
{
   // var_dump($_POST);
    if (isset($_POST['tfunction']) && $_POST['tfunction'] == 'auto_assign')
    {
        $auto_assign = $_POST['p_auto_assign'];
     //   var_dump($auto_assign);
        foreach($auto_assign as $key => $aa)
        {
            $auto = 0;
            if ($aa == 'on')
            {
                $auto = 1;
            }

            $query = db_query('UPDATE am_auto_assign SET auto_assign=:a WHERE am_uid=:b',array(':a'=>$auto,':b'=>$key));
            $result = $query;
        }
    }
}

/**
 * update am_auto_assign table
 */
function ncn_admin_update_am_auto_assign()
{
    $am_list = get_account_manager_list();
    foreach($am_list as $am)
    {
        ncn_admin_check_am_auto_assign_row($am['uid']);
        ncn_admin_update_am_auto_assign_row($am['uid']);
    }
}

function ncn_admin_check_am_auto_assign_row($am_uid)
{
//  $query = db_query('SELECT COUNT(*) AS n_count FROM am_auto_assign WHERE am_uid=:a',array(':a'=>$am_uid));
    $query = db_query('SELECT COUNT(*) FROM am_auto_assign WHERE am_uid=:a',array(':a'=>$am_uid))->fetchField();
    $n_count = $query;
    if ($n_count == 0)
    {
        $query = db_query('INSERT INTO am_auto_assign(am_uid, auto_assign, members) VALUES (:a, 0, 0)',array(':a'=>$am_uid));
        $result = $query;
    }
}

function ncn_admin_update_am_auto_assign_row($am_uid)
{
    $query = db_query('SELECT COUNT(*) FROM member_id_pool WHERE am_uid=:a',array(':a'=>$am_uid))->fetchField();
    $n_count = (int)$query;
    $query = db_query('UPDATE am_auto_assign SET members=:a WHERE am_uid=:b',array(':a'=>$n_count,':b'=>$am_uid));
    $result = $query;
}

/**
 * assign member to account manager
 */

function ncn_admin_assign_member_to_am($member_id, $am_uid)
{
//  $query = "UPDATE member_id_pool SET am_uid=$am_uid WHERE member_id='$member_id'";
    $result = db_query('UPDATE {member_id_pool} SET am_uid=:a WHERE member_id=:b',
        array(':a'=>$am_uid ,':b'=>$member_id));
    if ($result->rowCount()==0)
    {
        return FALSE;
    }

    ncn_admin_check_am_auto_assign_row($am_uid);
    ncn_admin_update_am_auto_assign_row($am_uid);
    return TRUE;
}

/**
 * AUTO - assign member to account manager
 */
function ncn_admin_auto_assign_member_to_am($member_id)
{
    $am_uid = ncn_admin_get_auto_assigned_am();
    if ($am_uid) {
        ncn_admin_assign_member_to_am($member_id, $am_uid);
    }

    return FALSE;
}

/**
 * AUTO - get account manager for auto-assign
 */

function ncn_admin_get_auto_assigned_am()
{
    ncn_admin_update_am_auto_assign();

    // Get Active Account Manager
    $am_list = get_account_manager_list();
    $am_uids = array();
    foreach ($am_list as $am) {
        $am_uids[] = $am['uid'];
    }
    $result = db_query('SELECT * FROM {am_auto_assign} WHERE auto_assign=1 ORDER BY members ASC');
    if ($result && $result->rowCount() > 0 && count($am_uids) > 0)
    {
        while ($am = $result->fetchAssoc()) {
            if (in_array($am['am_uid'], $am_uids)) {
                return $am['am_uid'];
            }
        }
    }

    return 0;
}

/**
 * render - select of account manager
 */

function draw_select_account_manager_list($name, $select_val, $attributes = '')
{
    $data = get_account_manager_list();
    $content = "<select name=\"$name\" $attributes>";
    $content.= '<option value="">Please Select</option>';
    for ($i = 0; $i < count($data); $i++)
    {
        $selected = '';
        if ($data[$i]['uid'] == $select_val)
        {
            $selected = 'selected';
        }

        $_user = user_load($data[$i]['uid']);
        $str_value = sprintf(" %s %s (%s)", $_user->profile_firstname, $_user->profile_lastname, $_user->name);
//        $str_value = sprintf(" :a :b (:c)",
//            array(':a'=>$_user->profile_firstname,':b'=>$_user->profile_lastname,':c'=>$_user->name));
        $content.= '<option value="' . $data[$i]['uid'] . '" ' . $selected . ' >' . $str_value . '</option>';
    }

    $content.= "</select>";
    return $content;
}

function set_ncn_data_extra($claim_id, $data_name, $val)
{
    $count = intval(db_query('SELECT COUNT(*) FROM {claims_data_extra} WHERE claim_id=:a AND name=:b',
        array(':a'=>$claim_id,':b'=>$data_name))->fetchField());
    if ($count == 0)
    {
        $result = db_query('INSERT INTO {claims_data_extra}(claim_id, name, val) VALUES(:a,:b,:c)',
            array(':a'=>$claim_id,':b'=>$data_name,':c'=>$val));
    }
    else
    {
        $result = db_query('UPDATE {claims_data_extra} SET val=:a WHERE claim_id=:b AND name=:c',
            array(':a'=>$val,':b'=>$claim_id,':c'=>$data_name));
    }

    if ($result->rowCount()==0)
    {
        return FALSE;
    }

    return TRUE;
}

function get_ncn_data_extra($claim_id, $data_name)
{
    $result = db_query('SELECT * FROM {claims_data_extra} WHERE claim_id=:a AND name=:b',
        array(':a'=>$claim_id,':b'=>$data_name))->fetchAssoc();

    if ($result)
    {
        return $result['val'];
    }

    return '';
}

function ncn_admin_render_claim_status_checkbox($label, $id, $claim_id, $data_name, $pvc, $attr = '')
{
    if (!($pvc & 01))
    {
        return '';
    }

    $checked = '';
    if (get_ncn_data_extra($claim_id, $data_name) == 'on')
    {
        $checked = 'checked';
    }

    $disabled = 'disabled';
    if ($pvc & 2)
    {
        $disabled = '';
    }

    return "<input type='checkbox' id='$id' name='$id' $checked $disabled/>&nbsp;&nbsp;<label for='$id'><strong>$label</strong></label>";
}

/**
 * Claim Status Form
 */

function ncn_admin_render_claim_status($claim_id)
{
    $pvc_pp = ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'PP');
    $pvc_claim_docs_recieved = ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'Claim Docs Recieved');
    $pvc_fax_in_dropbox = ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'Fax in Dropbox');
    $pvc_scott_conf_receipt = ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'Scott Conf. Receipt');
    $pvc_esx_pdf_uploaded = ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'ESX PDF Uploaded');
    $pvc_assign_to_ce = ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'Assign to CE');
    $pvc_check_claim_logfile = ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'Check Claim Logfile for extra data');
    $pvc_save = ($pvc_pp | $pvc_claim_docs_recieved | $pvc_fax_in_dropbox | $pvc_scott_conf_receipt | $pvc_esx_pdf_uploaded | $pvc_assign_to_ce | $pvc_check_claim_logfile);
    ob_start();
    if ($pvc_save & 01)
    {
?>

<form id="ncn_admin_claim_status_form" method="POST">

<input type="hidden" name="tfunction" value="claim_status_extra" />

<table>

    <tr><td><?php
        echo ncn_admin_render_claim_status_checkbox(t('PP') , 'cs_pp', $claim_id, 'pp', $pvc_pp); ?></td></tr>

    <tr><td><?php
        echo ncn_admin_render_claim_status_checkbox(t('Claim Docs Received') , 'cs_claim_docs_recieved', $claim_id, 'claim_docs_recieved', $pvc_claim_docs_recieved); ?></td></tr>

    <tr><td><?php
        echo ncn_admin_render_claim_status_checkbox(t('Fax in Dropbox') , 'cs_fax_in_dropbox', $claim_id, 'fax_in_dropbox', $pvc_fax_in_dropbox); ?></td></tr>

    <tr><td><?php
        echo ncn_admin_render_claim_status_checkbox(t('Scott Conf. Receipt') , 'cs_scott_conf_receipt', $claim_id, 'scott_conf_receipt', $pvc_scott_conf_receipt); ?></td></tr>

    <tr><td><?php
        echo ncn_admin_render_claim_status_checkbox(t('ESX PDF Uploaded') , 'cs_esx_pdf_uploaded', $claim_id, 'esx_pdf_uploaded', $pvc_esx_pdf_uploaded); ?>

    <tr><td><?php
        echo ncn_admin_render_claim_status_checkbox(t('Check Claim Logfile for extra data') , 'cs_check_claim_logfile', $claim_id, 'check_claim_logfile', $pvc_check_claim_logfile); ?></td></tr>

    

        <?php
        if ($pvc_esx_pdf_uploaded == 3): ?>

        &nbsp;&nbsp;<input type="text" name="price_list" value="" />

        <?php
        endif; ?>

        </td></tr>

    <tr><td><?php
        echo ncn_admin_render_claim_status_checkbox(t('Assign to CE') , 'cs_assign_to_ce', $claim_id, 'assign_to_ce', $pvc_assign_to_ce); ?></td></tr>

    <?php
        if ($pvc_save == 3): ?>

    <tr><td><input type="submit" value=" Save " /></td></tr>

    <?php
        endif; ?>

</table>

</form>

<?php
    }

    $content = ob_get_contents();
    ob_end_clean();
    return $content;
}

function ncn_admin_view_claim_action($claim_id)
{
    $action = 0;
    GLOBAL $language;
    if (isset($_POST['tfunction']) && $_POST['tfunction'] == 'claim_status_extra')
    {
        $action = 1;
        $data = array();
        $data['cs_pp'] = isset($_POST['cs_pp']) ? 'on' : '';
        $data['cs_claim_docs_recieved'] = isset($_POST['cs_claim_docs_recieved']) ? 'on' : '';
        $data['cs_fax_in_dropbox'] = isset($_POST['cs_fax_in_dropbox']) ? 'on' : '';
        $data['cs_scott_conf_receipt'] = isset($_POST['cs_scott_conf_receipt']) ? 'on' : '';
        $data['cs_esx_pdf_uploaded'] = isset($_POST['cs_esx_pdf_uploaded']) ? 'on' : '';
        $data['cs_assign_to_ce'] = isset($_POST['cs_assign_to_ce']) ? 'on' : '';
        $data['cs_check_claim_logfile'] = isset($_POST['cs_check_claim_logfile']) ? 'on' : '';

        // PP

        if (ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'PP') == 3)
        {
            $re_0 = ncn_admin_view_claim_set_data_extra_item($claim_id, 'pp', $data['cs_pp']);
        }

        // Claim Docs Recieved

        if (ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'Claim Docs Recieved') == 3)
        {
            $re_1 = ncn_admin_view_claim_set_data_extra_item($claim_id, 'claim_docs_recieved', $data['cs_claim_docs_recieved']);
        }

        // Fax in dropbox

        if (ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'Fax in Dropbox') == 3)
        {
            $re_2 = ncn_admin_view_claim_set_data_extra_item($claim_id, 'fax_in_dropbox', $data['cs_fax_in_dropbox']);
        }

        // Scott Conf. Receipt

        if (ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'Scott Conf. Receipt') == 3)
        {
            $re_3 = ncn_admin_view_claim_set_data_extra_item($claim_id, 'scott_conf_receipt', $data['cs_scott_conf_receipt']);
        }

        // ESX PDF Uploaded

        if (ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'ESX PDF Uploaded') == 3)
        {
            $re_4 = ncn_admin_view_claim_set_data_extra_item($claim_id, 'esx_pdf_uploaded', $data['cs_esx_pdf_uploaded']);
            $price_list = $_POST['price_list'];
            if ($price_list != '')
            {
                save_extended_data_item($claim_id, 'price_list', $price_list);
            }
        }

        // Assign to CE

        if (ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'Assign to CE') == 3)
        {
            $re_5 = ncn_admin_view_claim_set_data_extra_item($claim_id, 'assign_to_ce', $data['cs_assign_to_ce']);
        }

        // Check Claim Logfile for extra data

        if (ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'Check Claim Logfile for extra data') == 3)
        {
            $re_6 = ncn_admin_view_claim_set_data_extra_item($claim_id, 'check_claim_logfile', $data['cs_check_claim_logfile']);
        }

//      $query = "SELECT * FROM claims WHERE claim_id=$claim_id";
        $result = db_query('SELECT * FROM {claims} WHERE claim_id=:a', array(':a'=>$claim_id));
        if ($claim=$result->fetchAssoc())
        {
            $_admin = user_load(1);
            $admin_mail = $_admin->mail; //***
            $_claim_user = user_load($claim['user_id']);
            if (isset($_claim_user->profile_memberid)) {
                $_member = get_member_from_id($_claim_user->profile_memberid);
                if (!empty($_member))
                {
                    $_am = user_load($_member['am_uid']);
                    if ($_am)
                    {
                        $am_mail = $_am->mail; //***
                    }
                }
            }

            $ms_list = get_ms_list();
            $qa_list = get_qa_list();
            $am_list = get_account_manager_list();
            $sam_list = get_senior_account_manager_list();

            // automatic mail

            $site_mail = variable_get('site_mail', ini_get('sendmail_from'));
            if ($re_1 && $data['cs_claim_docs_recieved'] == 'on')
            {
                $part = "Claim Documents Received";
                $msg = get_mail_body_claim_status_change_notify($claim_id, $part);
                $subject = 'Claim Docs Received (' . ncn_cd($claim_id, 'customer_name') . '/' . $claim['claim_id'] . '/' . $_claim_user->profile_firstname . ' ' . $_claim_user->profile_lastname . '/' . $_claim_user->profile_legalname . ')';
                drupal_mail('ncn_admin', 'claim_status_change_notify', $admin_mail, language_default() , array(
                    'subject' => $subject,
                    'body' => $msg
                ) , $site_mail);

                // drupal_mail( 'ncn_admin', 'claim_status_change_notify', $am_mail, language_default(), array('subject'=>$subject, 'body'=>$msg), $site_mail );

                /*foreach($ms_list as $ms) {
                drupal_mail( 'ncn_admin', 'claim_status_change_notify', $ms['mail'], language_default(), array('subject'=>$subject, 'body'=>$msg), $site_mail );
                }*/
                foreach($qa_list as $qa)
                {
                    drupal_mail('ncn_admin', 'claim_status_change_notify', $qa['mail'], language_default() , array(
                        'subject' => $subject,
                        'body' => $msg
                    ) , $site_mail);
                }

                // Send mail to All Senior Account Managers

                foreach($sam_list as $sam)
                {
                    $to_sam_mail = $sam['mail'];
                    if (isset($to_sam_mail) && $to_sam_mail != '')
                    {
                        drupal_mail('ncn_admin', 'claim_status_change_notify', $to_sam_mail, language_default() , array(
                            'subject' => $subject,
                            'body' => $msg
                        ) , $site_mail);
                    }
                    else
                    {

                        // drupal_set_message("Failed to send a mail to Account Manager about 'ESX PDF Uploaded'. Please report this to web master.", 'error');

                    }
                }
            }

            if ($re_2 && $data['cs_fax_in_dropbox'] == 'on')
            {
                $part = "Fax in Dropbox";
                $msg = get_mail_body_claim_status_change_notify($claim_id, $part);
                $subject = 'Fax in Dropbox (' . ncn_cd($claim_id, 'customer_name') . '/' . $claim['claim_id'] . '/' . $_claim_user->profile_firstname . ' ' . $_claim_user->profile_lastname . '/' . $_claim_user->profile_legalname . ')';
                drupal_mail('ncn_admin', 'claim_status_change_notify', $admin_mail, language_default() , array(
                    'subject' => $subject,
                    'body' => $msg
                ) , $site_mail);

                // drupal_mail( 'ncn_admin', 'claim_status_change_notify', $am_mail, language_default(), array('subject'=>$subject, 'body'=>$msg), $site_mail );

                /*foreach($ms_list as $ms) {
                drupal_mail( 'ncn_admin', 'claim_status_change_notify', $ms['mail'], language_default(), array('subject'=>$subject, 'body'=>$msg), $site_mail );
                }*/
                foreach($qa_list as $qa)
                {
                    drupal_mail('ncn_admin', 'claim_status_change_notify', $qa['mail'], language_default() , array(
                        'subject' => $subject,
                        'body' => $msg
                    ) , $site_mail);
                }

                // Send mail to All Senior Account Managers

                foreach($sam_list as $sam)
                {
                    $to_sam_mail = $sam['mail'];
                    if (isset($to_sam_mail) && $to_sam_mail != '')
                    {
                        drupal_mail('ncn_admin', 'claim_status_change_notify', $to_sam_mail, language_default() , array(
                            'subject' => $subject,
                            'body' => $msg
                        ) , $site_mail);
                    }
                    else
                    {

                        // drupal_set_message("Failed to send a mail to Account Manager about 'ESX PDF Uploaded'. Please report this to web master.", 'error');

                    }
                }
            }

            if ($re_3 && $data['cs_scott_conf_receipt'] == 'on')
            {
                $part = "Confirm Receipt";
                $msg = get_mail_body_claim_status_change_notify($claim_id, $part);
                $subject = 'Comfirm Receipt (' . ncn_cd($claim_id, 'customer_name') . '/' . $claim['claim_id'] . '/' . $_claim_user->profile_firstname . ' ' . $_claim_user->profile_lastname . '/' . $_claim_user->profile_legalname . ')';
                drupal_mail('ncn_admin', 'claim_status_change_notify', $admin_mail, language_default() , array(
                    'subject' => $subject,
                    'body' => $msg
                ) , $site_mail);

                // drupal_mail( 'ncn_admin', 'claim_status_change_notify', $am_mail, language_default(), array('subject'=>$subject, 'body'=>$msg), $site_mail );

                /*foreach($ms_list as $ms) {
                drupal_mail( 'ncn_admin', 'claim_status_change_notify', $ms['mail'], language_default(), array('subject'=>$subject, 'body'=>$msg), $site_mail );
                }*/
                /*foreach($qa_list as $qa) {
                drupal_mail( 'ncn_admin', 'claim_status_change_notify', $qa['mail'], language_default(), array('subject'=>$subject, 'body'=>$msg), $site_mail );
                }*/
            }

            if ($re_4 && $data['cs_esx_pdf_uploaded'] == 'on')
            {

                // $part = "ESX PDF Uploaded";

                $msg = t(variable_get('ncn_claim_status_esx_pdf_uploaded', '') , array(
                    '!price_list' => $_POST['price_list']
                ) , $language->language);
                $subject = 'ESX PDF Uploaded (' . $claim['claim_id'] . ', ' . ucwords($claim['claim_type']) . ' & ' . ucwords($claim['claim_status']) . ')';
                drupal_mail('ncn_admin', 'claim_status_change_esx_pdf_uploaded', $admin_mail, language_default() , array(
                    'subject' => $subject,
                    'body' => $msg
                ) , $site_mail);
                foreach($am_list as $am)
                {
                    $to_am_mail = $am['mail'];
                    if (isset($to_am_mail) && $to_am_mail != '')
                    {
                        drupal_mail('ncn_admin', 'claim_status_change_esx_pdf_uploaded', $to_am_mail, language_default() , array(
                            'subject' => $subject,
                            'body' => $msg
                        ) , $site_mail);
                    }
                    else
                    {

                        // drupal_set_message("Failed to send a mail to Account Manager about 'ESX PDF Uploaded'. Please report this to web master.", 'error');

                    }
                }

                // Including Above
                // Send mail to All Senior Account Managers

                /*foreach ($sam_list as $sam) {
                $to_sam_mail = $sam['mail'];
                if (isset($to_sam_mail) && $to_sam_mail!='') {
                drupal_mail( 'ncn_admin', 'claim_status_change_esx_pdf_uploaded', $to_sam_mail, language_default(), array('subject'=>$subject, 'body'=>$msg), $site_mail );
                } else {

                // drupal_set_message("Failed to send a mail to Account Manager about 'ESX PDF Uploaded'. Please report this to web master.", 'error');

                }
                }*/
                /*foreach($ms_list as $ms) {
                drupal_mail( 'ncn_admin', 'claim_status_change_esx_pdf_uploaded', $ms['mail'], language_default(), array('subject'=>$subject, 'body'=>$msg), $site_mail );
                }*/
                /*foreach($qa_list as $qa) {
                drupal_mail( 'ncn_admin', 'claim_status_change_esx_pdf_uploaded', $qa['mail'], language_default(), array('subject'=>$subject, 'body'=>$msg), $site_mail );
                }*/
            }

            if ($re_5 && $data['cs_assign_to_ce'] == 'on')
            {
                ncn_ce_portal_admin_send_mail_when_checked_assign_to_ce($claim['claim_id']);
            }

            if ($re_6 && $data['cs_check_claim_logfile'] == 'on')
            {

                // To Do ...

            }
        }       
    }


    if (isset($_POST['tfunction_workflow']) && $_POST['tfunction_workflow'] == 'tfunction_workflow') {
        $action = 1;
        ncn_admin_claim_workflow_change($claim_id);
        // mark 1st claim mail reminder
        ncn_admin_mark_to_send_1st_mail($claim_id);     
    } else if (isset($_POST['tfunction_file_note']) && $_POST['tfunction_file_note'] == 'tfunction_file_note') {
        $action = 1;
        ncn_admin_claim_file_note_change($claim_id);
    } else if (isset($_POST['tfunction_claim_type']) && $_POST['tfunction_claim_type'] == 'claim_type_change') {
        $action = 1;
        ncn_admin_claim_type_change($claim_id);
    } else if (isset($_POST['tfunction_claim_note_action']) && $_POST['tfunction_claim_note_action'] == 'tfunction_claim_note_add') {
        $action = 1;
        $_claim_note = trim($_REQUEST['claim_note']);
        if ($_claim_note) {
          ncn_admin_note_add_note_from_original($claim_id);
          if (ncn_admin_note_add_note($claim_id, $GLOBALS['user']->uid, $_claim_note)) {
            drupal_set_message("You added note to claim, successfully.");
            $log_message = t("!user_name added Claim Note. !note", array('!user_name'=>ncn_amin_get_user_role_and_name(), '!note'=>strip_tags($_claim_note)));
            ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
          }
          /*if (ncn_admin_add_claim_filenote($claim_id, $_claim_note)) {
            drupal_set_message("You added note to claim, successfully.");
            $log_message = t("!user_name added Claim Note. !note", array('!user_name'=>ncn_amin_get_user_role_and_name(), '!note'=>strip_tags($_claim_note)));
            ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
          }*/
        } 
    } else if (isset($_POST['tfunction_claim_note_action']) && $_POST['tfunction_claim_note_action'] == 'tfunction_claim_note_update') {
        $action = 1;
        $note_id = $_REQUEST['claim_note_id'];
        $_claim_note = trim($_REQUEST['claim_note']);
        if ($_claim_note) {
            if (ncn_admin_note_update_note($note_id, $_claim_note)) {
                drupal_set_message("You updated note to claim, successfully.");
                $log_message = t("!user_name update Claim Note. !note", array('!user_name'=>ncn_amin_get_user_role_and_name(), '!note'=>strip_tags($_claim_note)));
                ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
            }
        }
    }
  
    if (isset($_POST['tfunction_send_cdr']) && $_POST['tfunction_send_cdr'] == 'tfunction_send_cdr') {
        $action = 1;
        ncn_admin_send_cdr($claim_id);
    }
    
    // --- handle of uploading scope file ---
    if (isset($_POST['scope_upload_function']) && $_POST['scope_upload_function'] == 'scope') {
        $action = 1;
        $roomname = $_POST['roomname'];
        ncn_admin_scopeform_submit($roomname);
    } else if (isset($_POST['scope_upload_function']) && $_POST['scope_upload_function'] == 'scope_auto_full_pdf') {
        $action = 1;
        $roomname = $_POST['roomname'];
        ncn_admin_auto_scopeform_full_pdf_submit(arg(4), $roomname);
    } else if (isset($_POST['scope_upload_function']) && $_POST['scope_upload_function'] == 'scope_ss_data') {
        $action = 1;
        $roomname = $_POST['roomname'];
        ncn_admin_ss_make_scopesheet_image(arg(4), $roomname);
    }
    
    if (isset($_POST['claims_data_update']) && $_POST['claims_data_update'] == 'claims_data_update') {
        $action = 1;
        ncn_admin_claimsdata_update($claim_id);
    }
    
    if (isset($_POST['create_invoice_function']) && $_POST['create_invoice_function'] == 'create_invoice') {
        $action = 1;
        generate_final_invoice($claim_id);
    }
    
    if (isset($_POST['extend_data_function']) && $_POST['extend_data_function'] == 'extend_data') {
        $action = 1;
        save_extended_data($claim_id);
    }
    
    
    if (isset($_POST['photo_add_room']) && $_POST['photo_add_room']  == 'photo_add_room') {
        $action = 1;
        ncn_admin_photo_add_room($claim_id, $_POST['room_name']);
    }
    else if (isset($_POST['update_captions']) && $_POST['update_captions'] == 'update_captions') {
        $action = 1;
        save_image_captions($claim_id);
    }
    else if (isset($_POST['photo_delete_room']) && $_POST['photo_delete_room']  == 'photo_delete_room') {
        $action = 1;
        ncn_admin_photo_delete_room($claim_id, $_POST['room_name']);
    } 
    else if (isset($_POST['photo_delete_first_room']) && $_POST['photo_delete_first_room']  == 'photo_delete_first_room') {
        $action = 1;
        ncn_admin_photo_delete_first_room($claim_id, $_POST['room_name']);
    } 
    
    if (stristr($_SERVER['REQUEST_URI'], 'ncn_admin_pool'))
    {   $url_part = "ncn_admin_pool";       }
    else
    {   $url_part = "ncn_admin";        }

    
    // --- handle status update ----
    if ( isset($_POST['claim_status_change']) && $_POST['claim_status_change']=='claim_status_change' && isset($_POST['claim_status']) )
    {
        $action = 1;
        // update claim amount
        if ((is_numeric(trim($_POST['claim_amount']))) && (trim($_POST['claim_amount'])>= 0))
        {
        //  $query = "UPDATE claims SET claim_amount=".$_POST['claim_amount']." WHERE claim_id=".$claim_id;
            $result = db_query('UPDATE {claims} SET claim_amount=:a  WHERE claim_id=:b',
            array(':a'=>$_POST['claim_amount'],':b'=>$claim_id));
        }
        else    // invalid (non numeric) value entered for "claim amount"
        {
            drupal_set_message('Claim amount must be a positive number.' , 'error');
        }
    
        // update payment received amount
        if ((is_numeric(trim($_POST['payment_received']))) && (trim($_POST['payment_received'])>= 0))
        {
        //  $query = "UPDATE claims SET payment_received=".trim($_POST['payment_received'])." WHERE claim_id=".$claim_id;
            $result = db_query('UPDATE {claims} SET payment_received=:a WHERE claim_id=:b',
            array(':a'=>trim($_POST['payment_received']),':b'=>$claim_id));
        }
        else    // invalid (non numeric) value entered for "payment_received"
        {
            drupal_set_message('Payment received amount must be a positive number.' , 'error');
        }
    
        // carry on
        
        
        $updated_user = $GLOBALS['user']->name;
        $log_message = "Status updated by " . ncn_amin_get_user_role_and_name() . " to '".$_POST['claim_status']."' ";
        
        if (trim($_POST['claim_status_message']) != '') // only add status message to log if it's not blank
        {
            $log_message .= "(with message: ".$_POST['claim_status_message'].")";
        }
        
        $log_message .= " Claim amount updated to: $".trim($_POST['claim_amount']);
        drupal_set_message($log_message, 'status');
        
        ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
        
        // --- email approved ---
        if ((isset($_POST['claim_status']) && $_POST['claim_status'] == 'approved') && (variable_get('ncn_approved_active', '') == 1))
        {
        //  $query = "SELECT user_id FROM claims WHERE claim_id=".$claim_id;
            $result = db_query('SELECT user_id FROM {claims} WHERE claim_id=:a',array(':a'=>$claim_id));
            $row = $result->fetchAssoc();
        
            $subject = variable_get('ncn_approved_subject', '');
            $body = variable_get('ncn_approved_body', '');
        
            $user_details = user_load($row['user_id']); 
            $v['claim_id'] = $claim_id;
            $v['customer_name'] = ncn_cd($claim_id, 'customer_name');
        
            $subject = pm_message_tags($v, $subject);
            $body = pm_message_tags($v, $body);
            $params = array(
                '!owner_name' => ncn_cd($claim_id, 'customer_name'),
                '!claim_id' => $claim_id
            );
            
            pm_send(0, $user_details->uid, $subject, $body, $claim_id, 0, 'approve_claim', $params);
        }
        else
        // --- email rejected ---
        if ((isset($_POST['claim_status']) && $_POST['claim_status'] == 'returned') && (variable_get('ncn_approved_active', '') == 1))
        {
        //  $query = "SELECT user_id FROM claims WHERE claim_id=".$claim_id;
            $result = db_query('SELECT user_id FROM {claims} WHERE claim_id=:a',array(':a'=>$claim_id));
            $row = $result->fetchAssoc();
        
            $subject = variable_get('ncn_rejected_subject', '');
            $body = variable_get('ncn_rejected_body', '');
        
            $user_details = user_load($row['user_id']); 
            $v['claim_id'] = $claim_id;
            $v['customer_name'] = ncn_cd($claim_id, 'customer_name');
            $v['reason'] = $_POST['claim_status_message'];
        
            $subject = pm_message_tags($v, $subject);
            $body = pm_message_tags($v, $body);
            
            pm_send(0, $user_details->uid, $subject, $body, $claim_id);
        }
        // --- claim message changed? ----
        else
        if (isset($_POST['claim_status_message']) && trim($_POST['claim_status_message']) != '')
        {
            // check to see if claims message has changed
        //  $query = "SELECT * FROM claims WHERE claim_id=".$claim_id;
            $result = db_query('SELECT * FROM {claims} WHERE claim_id=:a',array(':a'=>$claim_id));
            $row = $result->fetchAssoc();
            
            if (AddSlashes($_POST['claim_status_message']) != $row['claim_status_message'])
            {
                $subject = variable_get('ncn_generic_subject', '');
                $body = variable_get('ncn_generic_body', '');
            
                $user_details = user_load($row['user_id']); 

                $v['claim_id'] = $claim_id;
                $v['customer_name'] = ncn_cd($claim_id, 'customer_name');
                $v['status'] = $_POST['claim_status_message'];
            
                $subject = pm_message_tags($v, $subject);   
                $body = pm_message_tags($v, $body);
                
                pm_send(0, $user_details->uid, $subject, $body, $claim_id);
                
                
            } 
        }
        
        // update claim status
        // $query = "UPDATE claims SET claim_status='".$_POST['claim_status']."',claim_status_message=\"".AddSlashes($_POST['claim_status_message'])."\",last_modified=".date('U')." WHERE claim_id=".$claim_id;
        $result = db_query('UPDATE {claims} SET claim_status=:a, claim_status_message=:b,
        last_modified=:c  WHERE claim_id=:d',array(':a'=>$_POST['claim_status'],
        ':b'=>$_POST['claim_status_message'],':c'=>date('U'),':d'=>$claim_id));
        
        // Claim Back End
        set_ncn_data_extra($claim_id, 'claim_backend', $_POST['claim_backend']);
    }

    if ($action) {
        //ncn_print_r($_POST['current_scroll_position']);
        $query = array();
        if (isset($_POST['current_scroll_position']) && $_POST['current_scroll_position'] != "") {
            $query['current_scroll_position'] = $_POST['current_scroll_position'];
        }
        drupal_goto(current_path(), array('query'=>$query));
        exit;
    }
}

function ncn_admin_view_claim_set_data_extra_item($claim_id, $data_name, $val)
{
    $old_val = get_ncn_data_extra($claim_id, $data_name);
    if ($old_val != $val)
    {
        if ($val == '')
        {
            watchdog('trace_ncn_data_extra', "$claim_id: $data_name - " . $GLOBALS['user']->uid, array());
        }

        return set_ncn_data_extra($claim_id, $data_name, $val);
    }

    return FALSE;
}

/*****************************************************************************/
/*

/*****************************************************************************/

function get_mail_body_claim_status_change_notify($claim_id, $part)
{
    GLOBAL $base_url;
//  $query = "SELECT * FROM claims WHERE claim_id=$claim_id";
    $result = db_query('SELECT * FROM claims WHERE claim_id=:a',array(':a'=>$claim_id));
    if (!($claim=$result->fetchAssoc()))
    {
        return $part;
    }

    $_user = user_load($claim['user_id']);
    ob_start();
?>  

<style>

    td, th{

        text-align: center;

        padding: 5px 10px 0px 10px;

    }

</style>

    <p><?php
    echo $part; ?></p>

    <table cellpadding="2">

        <thead>

            <th>Claim ID</th>

            <th>Type</th>

            <th>Customer</th>

            <th>Zip Code</th>

            <th>User</th>

            <th>Created</th>

            <th>Modified</th>

            <th>Status</th>

            <th>Workflow</th>

        </thead>

        <tbody>

            <tr>

                <td><a href="<?php
    echo $base_url; ?>/admin/config/ncn_admin/viewclaim/<?php
    echo $claim_id; ?>"><?php
    echo $claim_id; ?></a></td>

                <td><?php
    echo ucwords($claim['claim_type']); ?></td>

                <td><?php
    echo ncn_cd($claim_id, 'customer_name'); ?></td>

                <td><?php
    echo ncn_cd($claim_id, 'insured_zip'); ?></td>

                <td><?php
    echo $_user->profile_firstname . ' ' . $_user->profile_lastname ?> (<?php
    echo $_user->name; ?>)</td>

                <td><?php
    echo date("m/d/Y", $claim['created']); ?></td>

                <td><?php
    echo date("m/d/Y", $claim['last_modified']); ?></td>

                <td><?php
    echo ucwords($claim['claim_status']); ?></td>

                <td><?php

    // $timer_trigger = render_claim_timer($claim_id, 1);

    $workflow = render_claim_workflow($claim_id);
    echo $workflow;
    /*if ($timer_trigger != '') {
    echo "<div>($timer_trigger)<div>";
    }*/
?>

                </td>

            </tr>

        </tbody>

    </table>

<?php
    $content = ob_get_contents();
    ob_end_clean();
    return $content;
}

function get_claim_docs_rejected_bcc()
{
    $bcc = '';
    $_admin = user_load(1);
    $admin_mail = $_admin->mail; //***
    $bcc = $admin_mail;
    $_claim_user = user_load($claim['user_id']);
    if (isset($_claim_user->profile_memberid)) {
        $_member = get_member_from_id($_claim_user->profile_memberid);
        if (!empty($_member))
        {
            $_am = user_load($_member['am_uid']);
            $am_mail = $_am->mail; //***
            if ($am_mail != '')
            {
                $bcc.= ", $am_mail";
            }
        }
    }

    /*$ms_list = get_ms_list();
    $qa_list = get_qa_list();
    foreach($ms_list as $ms) {
    if ($ms['mail']!='') {
    $bcc .= (", ".$ms['mail']);
    }
    }

    foreach($qa_list as $qa) {
    if ($qa['mail']!='') {
    $bcc .= (", ".$qa['mail']);
    }
    }*/
    return '';
}

function ncn_admin_render_select_role_OLD($name, $select_val, $attributes)
{
    $content = "<select name=\"$name\" $attributes>";
    $result = db_query('SELECT * FROM {role} WHERE rid>2 AND name!=:a AND name!=:b AND name!=:c AND name!=:d',
    array(':a'=>'distributor',':b'=>'associate',':c'=>'member',':d'=>'sub user'));
    foreach($result as $role)
    {
        $role = (array)$role;
        $selected = '';
        if ($role['rid'] == $select_val)
        {
            $selected = 'selected';
        };
        $content.= '<option value="' . $role['rid'] . '" ' . $selected . ' >' . $role['name'] . '</option>';
    }

    $content.= "</select>";
    return $content;
}

function ncn_admin_render_select_role($name, $select_val, $attributes)
{
    $result = db_query("SELECT * FROM {role} WHERE rid>2  AND name!='distributor' AND name!='associate' AND name!='member' AND name!='sub user'")->fetchAll();
    foreach($result as $row)
    {
        $row = (array)$row;
        $roles[$row['rid']] = $row['name'];
    }

    return $roles;
}
function ncn_admin_render_select_roles($name, $select_val, $attributes) {
    $content = "<select name=\"$name\" $attributes>";
    
    $result = db_query("SELECT * FROM {role} WHERE rid>2 AND name!='distributor' AND name!='associate' AND name!='member' AND name!='sub user'")->fetchAll();
    foreach($result as $row)
    {
        $role = (array)$row;
        $selected = '';
        if ($role['rid'] == $select_val) {$selected = 'selected'; };
        $content .= '<option value="'.$role['rid'].'" '.$selected.' >'.$role['name'].'</option>';
    }
    $content .= "</select>";

    return $content;
}

function ncn_admin_update_user_role($user, $role_id)
{
    $result = db_query('DELETE FROM {users_roles} WHERE uid=:d AND rid>2',array(':d'=>$user->uid));
    $result = db_query('INSERT INTO {users_roles}(uid, rid) VALUES(:a,:b)',array(':a'=>$user->uid,':b'=>$role_id));
    if ($result->rowCount()==0)
    {
        return FALSE;
    }

    return TRUE;
}

// ******************** First Free Member ********************//

function ncn_admin_insert_member_first_free($member_id)
{
    $result = db_query('INSERT INTO {member_first_free} (member_id, first_month_free, first_claim_free, claim_product_id, first_invoice_locked, status) VALUES(:a,:b,:c,:d,:e,:f)',
              array(':a'=>$member_id,':b'=>0,':c'=>0,':d'=>0,':e'=>0,':f'=>1));
    if ($result->rowCount()==0)
    {
        drupal_set_message(t('Failed to make member(#!member_id) first free member', array(
            '!member_id' => $member_id
        )) , 'error');
        return FALSE;
    }

    return TRUE;
}

function ncn_admin_is_member_first_free($member_id)
{
    $count = db_query('SELECT COUNT(*) as tot FROM {member_first_free} WHERE member_id=:a AND status=1',array(':a'=>$member_id))->fetchField();
    if (intval($count) == 0)
    {
        return FALSE;
    }

    return TRUE;
}

function ncn_admin_set_member_first_free_month($member_id, $ts)
{
    $result = db_query('UPDATE {member_first_free} SET first_month_free=:a

                         WHERE member_id=:b AND status=1',array(':a'=>$ts,':b'=>$member_id));
    if ($result->rowCount()==0)
    {
        watchdog('first_free_month', 'Failed to make the first monthly membership fee free (member: !member_id)', array(
            '!member_id' => $member_id
        ));
        return FALSE;
    }

    return TRUE;
}

function ncn_admin_member_first_free_month_passed($member_id)
{
    if (!isset($member_id))
    {
        $member_id = 0;
    }

    $result = db_query('SELECT * FROM {member_first_free} WHERE member_id=:a AND status=1',
    array(':a'=>$member_id));
    foreach($result as $row)
    {
        $row = (array)$row;
        if ($row)
        {
            if ($row['first_month_free'] == 0)
            {
                return FALSE;
            }
            else
            {
                return TRUE;
            }
        }
    }

    return TRUE;
}

function ncn_admin_set_member_first_free_claim($member_id, $claim_id)
{
    $result = db_query('UPDATE {member_first_free} SET first_claim_free=:a, first_invoice_locked=1

                         WHERE member_id=:b AND status=1',array(':a'=>$claim_id,':b'=>$member_id));
    if ($result->rowCount()==0)
    {
        watchdog('first_free_month', 'Failed to make the first claim free(member: !member_id, claim: !claim_id)', array(
            '!member_id' => $member_id,
            '!claim_id' => $claim_id
        ));
        return FALSE;
    }

    return TRUE;
}

function ncn_admin_get_member_first_free_claim_id($member_id)
{
    $result = db_query('SELECT * FROM {member_first_free} WHERE member_id=:a AND status=1',array(':a'=>$member_id));
    foreach($result as $row)
    {
        if ($row)
        {
            return $row['first_claim_free'];
        }
    }

    return 0;
}

function ncn_admin_set_member_first_free_claim_product_id($claim_id, $product_id)
{
    $result = db_query('UPDATE {member_first_free} SET claim_product_id=:a WHERE first_claim_free=:b AND status=1',
        array(':a'=>$product_id,':b'=>$claim_id));
    if ($result->rowCount()==0)
    {
        watchdog('first_free_month', 'Failed to make the first claim free(claim: !claim_id, product_id: !product_id)', array(
            '!claim_id' => $claim_id,
            '!product_id' => $product_id
        ));
        return FALSE;
    }

    return TRUE;
}

function ncn_admin_get_member_first_free_claim_product_id($claim_id)
{
    $result = db_query('SELECT * FROM {member_first_free} WHERE first_claim_free=:a AND status=1',array(':a'=>$claim_id));
    foreach($result as $row)
    {
        if ($row)
        {
            return $row['claim_product_id'];
        }
    }

    return 0;
}

function ncn_admin_is_claim_first_free($claim_id)
{
    $count = db_query('SELECT COUNT(*) FROM {member_first_free} WHERE first_claim_free=:a AND status=1',array(':a'=>$claim_id))->fetchField();
    if (intval($count) == 0)
    {
        return FALSE;
    }

    return TRUE;
}

function ncn_admin_get_claim_first_free_locked($claim_id)
{
    $result = db_query('SELECT * FROM {member_first_free} WHERE first_claim_free=:a AND status=1',array(':a'=>$claim_id));
    foreach($result as $row)
    {
        $row = (array)$row;
        if ($row)
        {
            if ($row['first_invoice_locked'] == 0)
            {
                return 'UNLOCKED';
            }
            else
            {
                return 'LOCKED';
            }
        }
    }

    return ''; // claim isn't first free claim
}

function ncn_admin_get_claim_docs_rejected_message($claim_id)
{
    $result = db_query('SELECT * FROM {claims_data_extra} WHERE claim_id=:a AND name=:b',
        array(':a'=>'claim_docs_rejected_msg',':b'=>$claim_id));
    foreach($result as $row)
    {
        $row = (array)$row;
        if ($row)
        {
            return $row['val'];
        }
    }

    return '';
}

function ncn_admin_set_claim_docs_rejected_message($claim_id, $msg)
{
    $count = db_query('SELECT COUNT(*) FROM {claims_data_extra} WHERE claim_id=:a AND name=:b',
        array(':a'=>'claim_docs_rejected_msg',':b'=>$claim_id))->fetchAll();
    if ($count == 0)
    {
        $result = db_query('INSERT INTO {claims_data_extra} (claim_id, name, val) VALUES(:a,:b,:c)',
            array(':a'=>$claim_id,':b'=>'claim_docs_rejected_msg',':c'=>$msg));
    }
    else
    {
        $result = db_query('UPDATE {claims_data_extra} SET val=:a WHERE claim_id=:b AND name=:c',
            array(':a'=>'claim_docs_rejected_msg',':b'=>$msg,':c'=>$claim_id));
    }
}

// ******************** NCN Gold Member Demo ********************//

/**
 * Mark Member as 'Demo', when Creating a new member
 */

function ncn_admin_insert_gold_member_demo($member_id)
{
    $expired_time = strtotime('+30 days');
    $result = db_query('INSERT INTO {member_gold_demo} (member_id, to_expired_time, num_claims, expired, status) VALUES(:a,:b,:c,:d,:e)',
                array(':a'=>$member_id,':b'=>$expired_time,':c'=>0,':d'=>0,':e'=>1));
    if ($result->rowCount()==0)
    {
        drupal_set_message(t('Failed to set member(#!member_id) demo', array(
            '!member_id' => $member_id
        )) , 'error');
        return FALSE;
    }

    return TRUE;
}

/**
 * Is Member 'Gold Member Demo'
 */

function ncn_admin_is_gold_member_demo($member_id)
{
    $result = db_query('SELECT COUNT(*) as tot FROM {member_gold_demo} WHERE member_id=:a AND expired=0 AND status=1',
    array(':a'=>$member_id))->fetchAssoc();
    if ($result['tot'] == 0)
    {
        return FALSE;
    }

    return TRUE;
}

/**
 * Check Exired of 'Gold Member Demo'
 */

function ncn_admin_is_gold_member_demo_expired($member_id)
{
    $result = db_query('SELECT to_expired_time, num_claims FROM {member_gold_demo} WHERE member_id=:a AND expired=0 AND status=1 ORDER BY to_expired_time DESC',
        array(':a'=>$member_id));
    if ($result->rowCount()==0)
    {
        return FALSE;
    }

    $limit_claims = 5; // Free for 5 Claims
    foreach($result as $row)
    {
        $row = (array)$row;
        if ($row)
        {
            if ($row['to_expired_time'] < date('U'))
            {
                return TRUE;
            }
            else
            if ($row['num_claims'] >= $limit_claims)
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

/**
 * Set Gold Member Demo to Exired
 */

function ncn_admin_set_gold_member_demo_expired($member_id)
{
    $result = db_query('UPDATE {member_gold_demo} SET expired=1 WHERE member_id=:a AND status=1',
        array(':a'=>$member_id));
    if ($result->rowCount()==0)
    {
        return FALSE;
    }

    // drupal_set_message

    return TRUE;
}

/**
 * Check of Expiring Gold Member Demo'
 */

function ncn_admin_check_gold_member_demo_expired($member_id)
{
    if (ncn_admin_is_gold_member_demo_expired($member_id))
    {
        return ncn_admin_set_gold_member_demo_expired($member_id);
    }

    return TRUE;
}

function ncn_admin_check_all_gold_member_demo_expired()
{
    $result = db_query('SELECT member_id FROM {member_gold_demo} WHERE expired=0 AND status=1');
    if ($result->rowCount()==0)
    {
        return FALSE;
    }

    foreach($result as $row)
    {
        $row = (array)$row;
        ncn_admin_check_gold_member_demo_expired($row['member_id']);
    }

    return TRUE;
}

/**
 * Get Free Number of Created Claims
 */

function ncn_admin_get_gmd_num_claims($member_id)
{
    $result = db_query('SELECT num_claims FROM {member_gold_demo} WHERE member_id=:a AND expired=0 AND status=1 ORDER BY to_expired_time DESC LIMIT 0, 1',
        array(':a'=>$member_id))->fetchField();
    if ($result->rowCount()==0)
    {
        return 0;
    }

    return $result;
}

/**
 * Insert Free Claim for NCN Gold Member Demo
 */

function ncn_admin_insert_free_claim_for_gmd($uid, $claim_id, $product_id)
{
    $product = node_load($product_id);
    $cost = $product->sell_price;
    $result = db_query('INSERT INTO {free_gold_demo_claims} (uid, claim_id, product_id, cost, status) VALUES(:a,:b,:c,:d,:e)',
        array(':a'=>$uid,':b'=>$claim_id,':c'=>$product_id,':d'=>$cost,':e'=>1));
    if ($result->rowCount()==0)
    {
        drupal_set_message(t('Failed to mark claim(#!claim_id) demo claim', array(
            '!claim_id' => $claim_id
        )) , 'error');
        return FALSE;
    }

    $_user = user_load($uid);
    $n_claims = ncn_admin_get_gmd_num_claims($_user->profile_memberid);
    $n_claims+= 1;
    $result = db_query('UPDATE {member_gold_demo} SET num_claims=:a WHERE member_id=:b AND status=1',
        array(':a'=>$n_claims,':b'=>$_user->profile_memberid));
    if ($result->rowCount()==0)
    {
        return FALSE;
    }

    return TRUE;
}

/*

** Check if Claim is Free Claim of NCN Gold Member Demo

*/

function ncn_admin_is_demo_claim_for_gmd($claim_id)
{
    $result = db_query('SELECT COUNT(id) FROM {free_gold_demo_claims} WHERE claim_id=:a AND status=1',
        array(':a'=>$claim_id))->fetchField();
    if ($result > 0)
    {
        return TRUE;
    }

    return FALSE;
}

// ** Net Claims Now Member Report**//

/**
 * Page Handler (admin/config/ncn_member_report)
 */

function ncn_admin_member_report()
{
    $n_gold_member = 0;
    $n_silver_member = 0;
    $n_gold_lite_member = 0;
    $n_coach_on_call_member = 0;
    $n_gold_coach_member = 0;
    $n_gold_member_1st_special = 0;

    $n_gold_member_pause = 0;
    $n_silver_member_pause = 0;
    $n_gold_lite_member_pause = 0;
    $n_coach_on_call_member_pause = 0;
    $n_gold_coach_member_pause = 0;
    $n_gold_member_demo = 0;

    $n_csi_member = 0;
    $n_csi_member_pause = 0;
    $n_platinum_member = 0;             // v2
    $n_platinum_member_pause = 0;       // v2

    $total_member = 0;

    $result = db_query("SELECT * FROM {users} as u 
                        LEFT JOIN {users_roles} AS ur ON (ur.uid = u.uid) 
                        WHERE ur.rid=:rid", array(':rid'=>USER_ROLE_MEMBER));

    foreach($result as $row)
    {
        $row = (array)$row;

        // if ($row['uid'] == 69 || $row['uid'] == 71 || $row['uid'] == 74 || $row['uid'] == 77 || $row['uid'] == 78) { continue; }

        $_user = user_load($row['uid']);
        if (isset($_user->profile_memberid) && $_user->profile_memberid != "" && $_user->profile_memberid != "0")
        {
            $member = get_member_from_id($_user->profile_memberid);
            if (!empty($member))
            {
                $total_member+= 1;
                if ($member['member_type'] == MT_GOLD)
                { // Gold
                    $n_gold_member+= 1;
                    if ($_user->status == 0 && (!isset($_user->profile_fblocked) || $_user->profile_fblocked != 'first'))
                    {
                        $n_gold_member_pause+= 1;
                    }
                    else
                    {
                        if (ncn_admin_is_gold_member_demo($_user->profile_memberid))
                        {
                            $n_gold_member_demo+= 1;
                        }
                        else
                        if (ncn_admin_is_member_first_free($_user->profile_memberid))
                        {
                            $n_gold_member_1st_special+= 1;
                        }
                    }
                }
                else
                if ($member['member_type'] == MT_SILVER)
                { // Silver
                    $n_silver_member+= 1;
                    if ($_user->status == 0 && (!isset($_user->profile_fblocked) || $_user->profile_fblocked != 'first'))
                    {
                        $n_silver_member_pause+= 1;
                    }
                }
                else
                if ($member['member_type'] == MT_PLATINUM)
                { // Platinum
                    $n_platinum_member+= 1;
                    if ($_user->status == 0 && (!isset($_user->profile_fblocked) || $_user->profile_fblocked != 'first'))
                    {
                        $n_platinum_member_pause+= 1;
                    }
                }
                else
                if ($member['member_type'] == MT_GOLD_LITE)
                { // Gold Lite
                    $n_gold_lite_member+= 1;
                    if ($_user->status == 0 && (!isset($_user->profile_fblocked) || $_user->profile_fblocked != 'first'))
                    {
                        $n_gold_lite_member_pause+= 1;
                    }
                }
                else
                if ($member['member_type'] == MT_COACH_ON_CALL)
                { // Coach on Call
                    $n_coach_on_call_member+= 1;
                    if ($_user->status == 0 && (!isset($_user->profile_fblocked) || $_user->profile_fblocked != 'first'))
                    {
                        $n_coach_on_call_member_pause+= 1;
                    }
                }
                else
                if ($member['member_type'] == MT_GOLD_COACH)
                { // Gold Coach
                    $n_gold_coach_member+= 1;
                    if ($_user->status == 0 && (!isset($_user->profile_fblocked) || $_user->profile_fblocked != 'first'))
                    {
                        $n_gold_coach_member_pause+= 1;
                    }
                }
                if ($member['member_type'] == MT_CSI)
                { // CSI
                    $n_csi_member+= 1;
                    if ($_user->status == 0 && (!isset($_user->profile_fblocked) || $_user->profile_fblocked != 'first'))
                    {
                        $n_csi_member_pause+= 1;
                    }
                }
            }
        }
    }

    $n_gold_member_ua = 0;
    $n_silver_member_ua = 0;
    $n_gold_lite_member_ua = 0;
    $n_coach_on_call_member_ua = 0;
    $n_gold_coach_member_ua = 0;
    $n_gold_member_1st_special_ua = 0;
    $n_gold_member_pause_ua = 0;
    $n_silver_member_pause_ua = 0;
    $n_gold_lite_member_pause_ua = 0;
    $n_coach_on_call_member_pause_ua = 0;
    $n_gold_coach_member_pause_ua = 0;
    $n_gold_member_demo_ua = 0;

    $n_csi_member_ua = 0;
    $n_csi_member_pause_ua = 0;
    $n_platinum_member_ua = 0;             // v2
    $n_platinum_member_pause_ua = 0;       // v2

    $total_member_ua = 0;
    $result = db_query('SELECT * FROM {member_id_pool} WHERE used=0');
    foreach($result as $member)
    {
        $member = (array)$member;
        $total_member_ua+= 1;
        if ($member['member_type'] == MT_GOLD)
        { // Gold
            $n_gold_member_ua+= 1;
            if ($member['status'] == 0)
            {
                $n_gold_member_pause_ua+= 1;
            }
            else
            if (ncn_admin_is_gold_member_demo($member['member_id']))
            {
                $n_gold_member_demo_ua+= 1;
            }
            else
            if (ncn_admin_is_member_first_free($member['member_id']))
            {
                $n_gold_member_1st_special_ua+= 1;
            }
        }
        else
        if ($member['member_type'] == MT_SILVER)
        { // Silver
            $n_silver_member_ua+= 1;
            if ($member['status'] == 0)
            {
                $n_silver_member_pause_ua+= 1;
            }
        }
        else
        if ($member['member_type'] == MT_PLATINUM)
        { // Platinum
            $n_platinum_member_ua+= 1;
            if ($member['status'] == 0)
            {
                $n_platinum_member_pause_ua+= 1;
            }
        }
        else
        if ($member['member_type'] == MT_GOLD_LITE)
        { // Gold Lite
            $n_gold_lite_member_ua+= 1;
            if ($member['status'] == 0)
            {
                $n_gold_lite_member_pause_ua+= 1;
            }
        }
        else
        if ($member['member_type'] == MT_COACH_ON_CALL)
        { // Coach on Call
            $n_coach_on_call_member_ua+= 1;
            if ($member['status'] == 0)
            {
                $n_coach_on_call_member_pause_ua+= 1;
            }
        }
        else
        if ($member['member_type'] == MT_GOLD_COACH)
        { // Gold Coach
            $n_gold_coach_member_ua+= 1;
            if ($member['status'] == 0)
            {
                $n_gold_coach_member_pause_ua+= 1;
            }
        }
        else
        if ($member['member_type'] == MT_CSI)
        { // Platinum
            $n_csi_member_ua+= 1;
            if ($member['status'] == 0)
            {
                $n_csi_member_pause_ua+= 1;
            }
        }
    }

    ob_start();
?>

<div class="member_report_content">

<table>

    <thead>

        <th>Member Type</th>

        <th># of Members</th>

        <th>Activated</th>

        <th>Unactivated</th>

    </thead>

    <tbody>

        <tr class="odd">

            <td>Gold Member</td>

            <td><?php echo ($n_gold_member + $n_gold_member_ua); ?></td>

            <td><?php echo $n_gold_member; ?></td>

            <td><?php echo $n_gold_member_ua; ?></td>

        </tr>

        <!-- <tr class="even">

            <td>&nbsp;&nbsp;&nbsp;Gold Member 1st Special</td>

            <td><?php echo ($n_gold_member_1st_special + $n_gold_member_1st_special_ua); ?></td>

            <td><?php echo $n_gold_member_1st_special; ?></td>

            <td><?php echo $n_gold_member_1st_special_ua; ?></td>

        </tr>

        <tr class="odd">

            <td>&nbsp;&nbsp;&nbsp;Gold Member Demo</td>

            <td><?php echo ($n_gold_member_demo + $n_gold_member_demo_ua); ?></td>

            <td><?php echo $n_gold_member_demo; ?></td>

            <td><?php echo $n_gold_member_demo_ua; ?></td>

        </tr> -->

        <tr class="even">

            <td>&nbsp;&nbsp;&nbsp;Gold Member Pause</td>

            <td><?php echo ($n_gold_member_pause + $n_gold_member_pause_ua); ?></td>

            <td><?php echo $n_gold_member_pause; ?></td>

            <td><?php echo $n_gold_member_pause_ua; ?></td>

        </tr>

        <tr class="odd">

            <td>Silver Member</td>

            <td><?php echo $n_silver_member + $n_silver_member_ua; ?></td>

            <td><?php echo $n_silver_member; ?></td>

            <td><?php echo $n_silver_member_ua; ?></td>

        </tr>

        <tr class="even">

            <td>&nbsp;&nbsp;&nbsp;Silver Member Pause</td>

            <td><?php echo ($n_silver_member_pause + $n_silver_member_pause_ua); ?></td>

            <td><?php echo $n_silver_member_pause; ?></td>

            <td><?php echo $n_silver_member_pause_ua; ?></td>

        </tr>

        
        <tr class="odd">

            <td>Platinum Member</td>

            <td><?php echo $n_platinum_member + $n_platinum_member_ua; ?></td>

            <td><?php echo $n_platinum_member; ?></td>

            <td><?php echo $n_platinum_member_ua; ?></td>

        </tr>

        <tr class="even">

            <td>&nbsp;&nbsp;&nbsp;Platinum Member Pause</td>

            <td><?php echo ($n_platinum_member_pause + $n_platinum_member_pause_ua); ?></td>

            <td><?php echo $n_platinum_member_pause; ?></td>

            <td><?php echo $n_platinum_member_pause_ua; ?></td>

        </tr>

        <tr class="odd">

            <td>Gold Lite Member</td>

            <td><?php echo $n_gold_lite_member + $n_gold_lite_member_ua; ?></td>

            <td><?php echo $n_gold_lite_member; ?></td>

            <td><?php echo $n_gold_lite_member_ua; ?></td>

        </tr>

        <tr class="even">

            <td>&nbsp;&nbsp;&nbsp;Gold Lite Member Pause</td>

            <td><?php echo ($n_gold_lite_member_pause + $n_gold_lite_member_pause_ua); ?></td>

            <td><?php echo $n_gold_lite_member_pause; ?></td>

            <td><?php echo $n_gold_lite_member_pause_ua; ?></td>

        </tr>

        

        <tr class="odd">

            <td>Coach on Call Member</td>

            <td><?php echo $n_coach_on_call_member + $n_coach_on_call_member_ua; ?></td>

            <td><?php echo $n_coach_on_call_member; ?></td>

            <td><?php echo $n_coach_on_call_member_ua; ?></td>

        </tr>

        <tr class="even">

            <td>&nbsp;&nbsp;&nbsp;Coach on Call Member Pause</td>

            <td><?php echo ($n_coach_on_call_member_pause + $n_coach_on_call_member_pause_ua); ?></td>

            <td><?php echo $n_coach_on_call_member_pause; ?></td>

            <td><?php echo $n_coach_on_call_member_pause_ua; ?></td>

        </tr>

        <tr class="odd">

            <td>Gold Coach Member</td>

            <td><?php echo $n_gold_coach_member + $n_gold_coach_member_ua; ?></td>

            <td><?php echo $n_gold_coach_member; ?></td>

            <td><?php echo $n_gold_coach_member_ua; ?></td>

        </tr>

        <tr class="even">

            <td>&nbsp;&nbsp;&nbsp;Gold Coach Member Pause</td>

            <td><?php echo ($n_gold_coach_member_pause + $n_gold_coach_member_pause_ua); ?></td>

            <td><?php echo $n_gold_coach_member_pause; ?></td>

            <td><?php echo $n_gold_coach_member_pause_ua; ?></td>

        </tr>

        <tr class="odd">

            <td>CSI Member</td>

            <td><?php echo $n_csi_member + $n_csi_member_ua; ?></td>

            <td><?php echo $n_csi_member; ?></td>

            <td><?php echo $n_csi_member_ua; ?></td>

        </tr>

        <tr class="even">

            <td>&nbsp;&nbsp;&nbsp;CSI Member Pause</td>

            <td><?php echo ($n_csi_member_pause + $n_csi_member_pause_ua); ?></td>

            <td><?php echo $n_csi_member_pause; ?></td>

            <td><?php echo $n_csi_member_pause_ua; ?></td>

        </tr>


        <tr class="odd">

            <td colspan="4">&nbsp;</td>

        </tr>

        <tr class="even">

            <td>Total</td>

            <td><?php echo ($total_member + $total_member_ua); ?></td>

            <td><?php echo $total_member; ?></td>

            <td><?php echo $total_member_ua; ?></td>

        </tr>

    </tbody>

</table>

<?php
    $content = ob_get_contents();
    ob_end_clean();
    return $content;
}

/**
 * Action URL Handler (download/user_list)
 */

function ncn_admin_download_user_list()
{
    require_once 'sites/all/libraries/php_xls/Classes/PHPExcel.php';

    $report_name = "user_list.xls";
    $demo_file = "user_list.xls";
    $objReader = PHPExcel_IOFactory::createReader("Excel5");
    $objPHPExcel = $objReader->load($demo_file);
    $count = db_query('SELECT COUNT(uid) FROM {users} WHERE uid!=0')->fetchField();
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("F3", "Number of Users: " . $count);
    if ($count > 0)
    {
        $objPHPExcel->getActiveSheet()->insertNewRowBefore(9, $count);
    }

    for ($i = 0; $i < $count; $i++)
    {
        $source = "A7:X7";
        if ($i % 2 == 1)
        {
            $source = "A8:X8";
        }

        $row_index = $i + 9;
        $dest = "A$row_index:X$row_index";
        $objPHPExcel->getActiveSheet()->duplicateStyle($objPHPExcel->getActiveSheet()->getStyle($source) , $dest);
    }

    $objPHPExcel->getActiveSheet()->removeRow(7 + $count, 2);
    $result = db_query('SELECT uid FROM {users} WHERE uid!=0 ORDER BY created DESC');
    $i = 0;
    foreach($result as $row)
    {
        $row = (array)$row;
        $row_index = $i + 7;
        $_user = user_load($row['uid']);
        $username = $_user->profile_firstname . " " . $_user->profile_lastname;

        // is account free?

        /*if ((isset($_user->profile_credit_card_number)) && ($_user->profile_credit_card_number == '5424000000000015'))
        {   $free_extra = ' (free)';    }
        else
        {   $free_extra = '';   }*/
        if (!isset($_user->profile_firstname) || $_user->profile_firstname == '')
        {
            $username = $_user->name;
        }

        /*if (is_distributor($_user)==true || is_associate($_user)==true)
        {
        if ($_user->profile_legalname != '')
        {   $username = $_user->profile_legalname;  }
        }*/
        $company_name = $_user->profile_legalname;
        unset($_user->roles[2]);
        $role = implode($_user->roles);
        $member_type = get_member_type_name($_user->uid);
        if (ncn_admin_is_gold_member_demo(isset($_user->profile_memberid)?$_user->profile_memberid:0))
        {
            $member_type.= " (Demo)";
        }
        else
        if (ncn_admin_is_member_first_free(isset($_user->profile_memberid)?$_user->profile_memberid:0))
        {
            $member_type.= " (First Free)";
        }

        $status = '';
        if ($_user->profile_fblocked == 'first')
        {
            $status = 'training';
        }
        else
        {
            $status = ($_user->status) ? 'active' : 'blocked';
        }

        $cancel_time = '';
        if ($status == 'blocked')
        {
            $cancel_time = ncn_user_get_last_blocked_time($_user->uid);
            if ($cancel_time)
            {
                $cancel_time = date("m/d/Y", $cancel_time);
            }
            else
            {
                $cancel_time = '';
            }
        }

        $associate = get_distributor_associate_name($_user->uid);
        $am = get_account_manager_name($_user->uid);
        $member_since = 0;
        $sales_rep = '';
        if (isset($_user->profile_memberid) && is_member($_user->profile_memberid))
        {
            $member = get_member_from_id($_user->profile_memberid);
            if (!empty($member))
            {
                $member_since = $member['create'];
                $sales_rep = $member['sales_rep'];
            }
        }

        if ($member_since == 0)
        {
            $member_since = $_user->created;
        }

        $num_claims = 0;
        $num_claims_res = db_query("SELECT COUNT(*) as Tot FROM {claims} WHERE user_id=:d AND claim_status!='unpurchased'", array(':d'=>$_user->uid))->fetchAssoc();
        $num_claims = (int)$num_claims_res['Tot'];      
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("A$row_index", $username);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("B$row_index", $company_name);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("C$row_index", $role);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("D$row_index", isset($_user->profile_memberid)?$_user->profile_memberid:'');
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("E$row_index", $member_type);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("F$row_index", $status);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("G$row_index", $cancel_time);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("H$row_index", $associate);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("I$row_index", $am);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("J$row_index", date('m/d/Y', $member_since));
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("K$row_index", $sales_rep);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("L$row_index", $num_claims);

        // *

        $ytd_dollar = render_payment_cost(ncn_report_get_ytd($_user->uid));
        $total_dollar = render_payment_cost(ncn_report_get_total_ytd($_user->uid));
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("M$row_index", $ytd_dollar);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("N$row_index", $total_dollar);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("O$row_index", $_user->profile_numberofairmovers);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("P$row_index", $_user->profile_numberofdehumidifiers);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("Q$row_index", $_user->profile_numberofemployees);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("R$row_index", $_user->profile_servicevehicles);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("S$row_index", $_user->profile_jobspermonth);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("T$row_index", strip_tags(render_payment_cost(ncn_report_get_ytd($_user->uid, 0, 2011))));
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("U$row_index", strip_tags(render_payment_cost(ncn_report_get_ytd($_user->uid, 0, 2012))));
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("V$row_index", strip_tags(render_payment_cost(ncn_report_get_ytd($_user->uid, 0, 2013))));
        /*$objPHPExcel->setActiveSheetIndex(0)->setCellValue("W$row_index" ,
        strip_tags(render_payment_cost(ncn_report_get_ytd($_user->uid, 0, 2014))));
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("X$row_index" ,
        strip_tags(render_payment_cost(ncn_report_get_ytd($_user->uid, 0, 2015))));*/
        $i+= 1;
    }

    header('Content-Type: application/vnd.ms-excel');
    header('Content-Disposition: attachment;filename="' . $report_name . '"');
    header('Cache-Control: max-age=0');
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5');
    $objWriter->save('php://output');
    exit;
}

/**
 * User distributor name from child user id
 *
 * @ingroup themeable
 */

function get_member_type_num($user_id)
{
    $user_details = user_load($user_id);
    if ($user_details && isset($user_details->profile_memberid)) {
        $result = db_query('SELECT * FROM member_id_pool WHERE member_id=:a',array(':a'=>$user_details->profile_memberid));
        $row_count = $result->rowCount();
        if ($row_count == 0)
        {
            return -1;
        }

        $member = $result->fetchAssoc();

        return $member['member_type'];
    }
    return -1;
}

function get_member_type_name($user_id)
{

    $member_type = get_member_type_num($user_id);
    if ($member_type < 0) {
        return '';
    }

    $type_arr = get_member_type_array();
    return isset($type_arr[$member_type])? $type_arr[$member_type]:'';
}

function get_member_type_array()
{
    return array(
        MT_GOLD         => "Gold",
        MT_SILVER       => "Silver",
        MT_GOLD_LITE    => "Gold Lite",
        MT_COACH_ON_CALL=> "Coach on Call",
        MT_GOLD_COACH   => "Gold Coach",
        MT_CSI          => "CSI",
        MT_PLATINUM     => "Platinum",
    );
}

function get_account_manager_name($user_id)
{
    $user_details = user_load($user_id);
    if ($user_details) {
        if (is_distributor($user_details) == true || is_associate($user_details) == true)
        {
            $result = db_query('SELECT * FROM users_parent WHERE uid=:a',array(':a'=>$user_id));
            $row_count = $result->rowCount();
            if ($row_count == 0)
            {
                return ('');
            }

            $row = $result->fetchAssoc();
    //      $row = (array)$result;
            $row['owner'] = $row['parent'];

            // now load the distributor account details
         //   var_dump($row);
            $user_details = user_load($row['owner']);
          //  var_dump($user_details);
    //        var_dump($user_details);
            if ($user_details) {
                return $user_details->profile_firstname . " " . $user_details->profile_lastname . " (" . $user_details->name . ")";
            } else {
                return 'User #'.$row['owner'];
            }
        }

        // normal accounts

        else if (isset($user_details->profile_memberid))
        {

            // now get the distributor user id

            //  $query = "SELECT * FROM member_id_pool WHERE member_id=\"" . AddSlashes($user_details->profile_memberid) . "\";";
            $result = db_query('SELECT * FROM member_id_pool WHERE member_id=:a',array(':a'=>$user_details->profile_memberid));
            $row_count = $result->rowCount();
            if ($row_count == 0) // not found/invalid
            {
                return ('');
            }

            $row = $result->fetchAssoc();

            // now get the account managers id

            /*$query = "SELECT parent FROM users_parent WHERE uid=".$row['owner'];
            $result = db_query($query);
            $row_count = $result->rowCount();
            if ($row_count == 0)    // not found/invalid
            {   return ('');    }

            $row = (array)$result;

            // now load the distributor account details

            $user_details = user_load($row['parent']);
            */
            if ($row['am_uid'])
            {
                $user_details = user_load($row['am_uid']);
                if ($user_details) {
                    return $user_details->profile_firstname . " " . $user_details->profile_lastname . " (" . $user_details->name . ")";
                } else {
                    return 'User #'.$row['am_uid'];
                }
            }

            return '';
        }
    } 
    return '';
}

/**
 * User distributor name from child user id
 *
 * @ingroup themeable
 */

// function get_distributor_name($user_id)  //* update function name *

function get_distributor_associate_name($user_id)
{

    $user_details = user_load($user_id);
    if (!$user_details) {
        return '';
    }

    if (is_distributor($user_details) == true || is_associate($user_details) == true)
    {
        return ('');
    }

    // not found/invalid

    if (isset($user_details->profile_memberid)) {
        if ($user_details->profile_memberid == '')
        {

            $result = db_query('SELECT * FROM {users_parent} WHERE uid=:a',array(':a'=>$user_id));

            $row_count = $result->rowCount();
          //  var_dump($row_count);
           // exit;
            if ($row_count == 0)
            {

                return ('');
            }

            $row = $result->fetchAssoc();

            $row['owner'] = $row['parent'];
        }
        else
        {

            // now get the distributor user id
            $result = db_query('SELECT * FROM member_id_pool WHERE member_id=:a',array(':a'=>$user_details->profile_memberid));
            $row_count = $result->rowCount();
            if ($row_count == 0) // not found/invalid
            {
                return ('');
            }

            $row = $result->fetchAssoc();
        }

        // now load the distributor account details
        //  var_dump($user_details);

        if (isset($row['owner'])) {
            $user_details = user_load($row['owner']);
            if ($user_details) {
                return $user_details->profile_legalname . " (" . $user_details->name . ")";
            }
        }
    }
    return '';
}

/**
 * Add Room Action Handler (in View Claim Page)
 *
 * TO DO: Change using ncn_action_claim_add_room()
 */

function ncn_admin_photo_add_room($claim_id, $room_name)
{
    if ($room_name == '')
    {
        drupal_set_message(t("Room name is empty.") , 'error');
        return FALSE;
    }

    $room_name = preg_replace("/[^a-zA-Z0-9\s]/", "", $room_name);
//  $query = "SELECT * FROM claims_data WHERE claim_id=" . $claim_id . " AND `key`='scope';";
    $result = db_query('SELECT * FROM {claims_data} WHERE claim_id=:a AND `key`=:b',
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
        $default_room_name = ncn_admin_claim_get_default_room_name($claim_id);
        $scope[$default_room_name] = array();
    }

    if (array_key_exists($room_name, $scope))
    {
        drupal_set_message(t("Room(!room_name) exists already.", array(
            '!room_name' => $room_name
        )) , 'error');
        return FALSE;
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
        drupal_set_message(t('Failed to add a room.') , 'error');
        return FALSE;
    }

    $log_message = t('Added a room(!room_name), successfully.', array(
        '!room_name' => $room_name
    ));
    drupal_set_message($log_message);
    $log_message = t('!user_name added a room(!room_name)', array(
        '!user_name' => ncn_amin_get_user_role_and_name() ,
        '!room_name' => $room_name
    ));
    ncn_admin_insert_claim_log($claim_id, date('U') , $log_message);
    return TRUE;
}

function ncn_admin_photo_delete_room($claim_id, $room_name)
{
    if ($room_name == '')
    {
        drupal_set_message(t("Room name is empty.") , 'error');
        return FALSE;
    }

    //$query = "SELECT * FROM claims_data WHERE claim_id=" . $claim_id . " AND `key`='scope';";
    $result = db_query('SELECT * FROM {claims_data} WHERE claim_id=:a AND `key`=:b',
        array(':a'=>$claim_id,':b'=>'scope'));
    $row_count = $result->rowCount();
    if ($row_count > 0)
    {

        // load images

        $row = $result->fetchAssoc();
        $scope = unserialize($row['val']);
    }
    else
    {
        drupal_set_message("This Claim hasn't photo album", 'error');
        return FALSE;
    }

    if (!array_key_exists($room_name, $scope))
    {
        drupal_set_message(t("Room(!room_name) doesn't exist.", array(
            '!room_name' => $room_name
        )) , 'error');
        return FALSE;
    }

    unset($scope[$room_name]); // delete a room
    $scope = serialize($scope);
//  $query = "UPDATE claims_data SET `val`='$scope' WHERE claim_id=$claim_id AND `key`='scope'";
    $result = db_query('UPDATE {claims_data} SET val=:a WHERE claim_id=:b AND `key`=:c',
        array(':a'=>$scope,':b'=>$claim_id,':c'=>'scope'));
    if ($result->rowCount()==0)
    {
        drupal_set_message(t('Failed to delete a room.') , 'error');
        return FALSE;
    }

    $log_message = t('Deleted a room(!room_name), successfully.', array(
        '!room_name' => $room_name
    ));
    drupal_set_message($log_message);
    $log_message = t('!user_name deleted a room(!room_name).', array(
        '!user_name' => ncn_amin_get_user_role_and_name() ,
        '!room_name' => $room_name
    ));
    ncn_admin_insert_claim_log($claim_id, date('U') , $log_message);
    return TRUE;
}

function ncn_admin_photo_delete_first_room($claim_id, $room_name)
{
    if ($room_name == '')
    {
        drupal_set_message(t("Room name is empty.") , 'error');
        return FALSE;
    }

    if (ncn_admin_is_photo_first_room_deleted($claim_id))
    {
        drupal_set_message(t("First Room was deleted, already.") , 'error');
        return FALSE;
    }

    $result = db_query('INSERT INTO {claims_data_extra} VALUES(NULL,:a,:b,:c)',
        array(':a'=>$claim_id,':b'=>'photo_first_room_deleted',':c'=>$room_name));
    if ($result->rowCount()==0)
    {
        drupal_set_message(t('Failed to delete the first room.') , 'error');
        return FALSE;
    }

    $log_message = t('Deleted the first room(!room_name), successfully.', array(
        '!room_name' => $room_name
    ));
    drupal_set_message($log_message);
    $log_message = t('!user_name deleted the first room(!room_name), successfully.', array(
        '!user_name' => ncn_amin_get_user_role_and_name() ,
        '!room_name' => $room_name
    ));
    ncn_admin_insert_claim_log($claim_id, date('U') , $log_message);
    return TRUE;
}

function ncn_admin_is_photo_first_room_deleted($claim_id)
{
    $result = db_query('SELECT COUNT(*) FROM {claims_data_extra} WHERE claim_id=:a AND name=:b',
        array(':a'=>'photo_first_room_deleted',':b'=>$claim_id))->fetchField();
    if ($result > 0)
    {
        return TRUE;
    }

    return FALSE;
}

function ncn_admin_page_settings()
{
    drupal_set_title(t('Settings'));
    $video_form = drupal_get_form('ncn_admin_settings_ncn_video_section');
    $output = drupal_render($video_form);
    $setting_form = drupal_get_form('ncn_admin_settings_form');
    $output.= drupal_render($setting_form);
    return $output;
}

function ncn_admin_settings_form()
{
    $form = array();
    $form['#attributes'] = array(
        'enctype' => "multipart/form-data"
    );

    // Scope File Crop

    $form['scope_file_info'] = array(
        '#type' => 'fieldset',
        '#title' => t('Scope File Crop Coordination') ,
        '#weight' => - 45,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form['scope_file_info']['ncn_scope_file_crop_x'] = array(
        '#type' => 'textfield',
        '#title' => t('X') ,
        '#default_value' => variable_get('ncn_scope_file_crop_x', '0') ,
    );
    $form['scope_file_info']['ncn_scope_file_crop_y'] = array(
        '#type' => 'textfield',
        '#title' => t('Y') ,
        '#default_value' => variable_get('ncn_scope_file_crop_y', '0') ,
    );
    $form['scope_file_info']['ncn_scope_file_crop_width'] = array(
        '#type' => 'textfield',
        '#title' => t('Width') ,
        '#default_value' => variable_get('ncn_scope_file_crop_width', '0') ,
    );
    $form['scope_file_info']['ncn_scope_file_crop_height'] = array(
        '#type' => 'textfield',
        '#title' => t('Height') ,
        '#default_value' => variable_get('ncn_scope_file_crop_height', '0') ,
    );

    // Scope File Crop

    $form['cor_instruction_wrapper'] = array(
        '#type' => 'fieldset',
        '#title' => t('Change Order Requests Instruction') ,
        '#weight' => - 45,
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
    );
    $form['cor_instruction_wrapper']['cor_instruction'] = array(
        '#type' => 'textarea',
        '#title' => t('') ,
        '#rows' => 5,
        '#default_value' => variable_get('cor_instruction', '') ,
    );
    $form['settings_save'] = array(
        '#type' => 'submit',
        '#weight' => 101,
        '#value' => t('Save')
    );
    return $form;
}

function ncn_admin_settings_form_submit($form, &$form_state)
{
    variable_set('ncn_scope_file_crop_x', $form_state['values']['ncn_scope_file_crop_x']);
    variable_set('ncn_scope_file_crop_y', $form_state['values']['ncn_scope_file_crop_y']);
    variable_set('ncn_scope_file_crop_width', $form_state['values']['ncn_scope_file_crop_width']);
    variable_set('ncn_scope_file_crop_height', $form_state['values']['ncn_scope_file_crop_height']);
    variable_set('cor_instruction', $form_state['values']['cor_instruction']);
    drupal_set_message(t('Settings are updated.'));
}

function ncn_admin_viewclaim_scopefile_crop($claim_id, $room_name)
{
    $filepath = base64_decode(arg(7));

    if (isset($_POST['tfunction']) && $_POST['tfunction'] == 'viewclaim_scopefile_crop')
    {
        $ts = date('U');

        $dest_file['filepath'] = substr($filepath, 0, -4) . "_cut_".$ts.".jpg";
        $dest_file['filename'] = basename($dest_file['filepath']);
        $dest_file['filemime'] = "image/jpg";
        $api_params = array(
            'action' => 'crop',
            'width' => $_POST['crop_width'],
            'height' => $_POST['crop_height'],
            'x' => $_POST['crop_xoffset'],
            'y' => $_POST['crop_yoffset'],
            'source' => $filepath,
            'dest' => $dest_file['filepath']
        );
        $bAction = _ncn_image_api('crop', $api_params);
        file_delete($filepath);
        if ($bAction)
        {
            $query = "SELECT * FROM claims_scope WHERE claim_id=" . $claim_id . " AND roomname=\"" . $room_name . "\"";
            $result = db_query('SELECT * FROM {claims_scope} WHERE claim_id=:a AND roomname=:b',
                array(':a'=>$claim_id,':b'=>$room_name ));
            $row_count = $result->rowCount();
            if ($row_count == 0)
            {
                $revision = 0;
//              $query = "INSERT INTO claims_scope VALUES(NULL, " . $claim_id . ", \"" . $room_name . "\", \"" . AddSlashes($dest_file['filename']) . "\", \"" . AddSlashes($dest_file['filepath']) . "\", \"" . AddSlashes($dest_file['filemime']) . "\", " . date('U') . ");";
                $query = db_query('INSERT INTO {claims_scope} VALUES(NULL,:a,:b,:c,:d,:e,:f)',
                 array(':a'=>$claim_id ,':b'=>$room_name ,':c'=>$dest_file['filename'],
                        ':d'=>$dest_file['filepath'],':e'=>$dest_file['filemime'],':f'=>date('U')));

            }
            else
            {
                $row = $result->fetchAssoc();
                // $revision = $row['revision']+1;
                $revision = 0;

                file_delete($row['filepath']);
            //  $query = "UPDATE claims_scope SET filename=\"" . AddSlashes($dest_file['filename']) . "\", filepath=\"" . AddSlashes($dest_file['filepath']) . "\", filemime=\"" . AddSlashes($dest_file['filemime']) . "\", timestamp=" . date('U') . " WHERE claim_id=$claim_id AND roomname=\"" . $room_name . "\"";
                $query = db_query('UPDATE {claims_scope} SET filename=:a, filepath=:b, filemime=:c, `timestamp`=:d WHERE claim_id=:e AND roomname=:f',
                    array(':a'=>$dest_file['filename'],':b'=>$dest_file['filepath'],':c'=>$dest_file['filemime'],':d'=>date('U'),':e'=>$claim_id,':f'=>$room_name));
            }

            $result = $query;
            drupal_set_message(t('Scope file generated, successfully. (!room_name)', array(
                '!room_name' => $room_name
            )));
            $log_message = t("!user_name generated Scope File of Room(!room_name). Filename: !file_name", array(
                '!user_name' => ncn_amin_get_user_role_and_name() ,
                '!room_name' => $room_name,
                '!file_name' => $dest_file['filename']
            ));
            ncn_admin_insert_claim_log($claim_id, date('U') , $log_message);
            return "

            <script>

            jQuery(document).ready(function(){
                parent.location.reload(true);
            });

            </script>";
        }
    }
    else
    {
        return theme('ncn_admin_scope_full_crop', 
                     array( 'claim_id'  => $claim_id, 
                            'room_name' => $room_name, 
                            'img_path'  => $filepath ));
    }
}

function draw_select_una_member_status($name, $select_val, $attributes = '')
{
    $data = array(
        '0' => 'paused',
        '1' => 'unactivated'
    );
    $content = "<select name=\"$name\" $attributes>";
    for ($i = 0; $i < count($data); $i++)
    {
        $selected = '';
        if ($i == $select_val)
        {
            $selected = 'selected';
        }

        $content.= '<option value="' . $i . '" ' . $selected . ' >' . $data[$i] . '</option>';
    }

    $content.= "</select>";
    return $content;
}

function draw_select_claim_invoice_type($name, $select_val, $attributes = '')
{
    /*$data = array(
    'water' => 'Water',
    'water_wo_sc' => 'Water w/o SC',
    'mold_wo_water_wo_sc' => 'Mold w/o water w/o SC',
    'mold_wo_water' => 'Mold w/o water',
    'mold_with_water_wo_sc' => 'Mold w/ water w/o SC',
    'mold_with_water' => 'Mold w/ water',
    'reconstruction_with_mold' => 'Reconstruction w/ mold',
    'reconstruction_with_water' => 'Reconstruction w/ water',
    'reconstruction_with_mold_wo_sc' => 'Reconstruction w/ mold w/o SC',
    'reconstruction_with_water_wo_sc' => 'Reconstruction w/ water w/o SC',
    '*no*-xactimate'    =>  'Xactimate'
    );*/
    $data = array(
        'water_inv_wt_sc' => 'Water Invoice with SC',
        'water_inv_wo_sc' => 'Water Invoice without SC',
        'water_est_wt_sc' => 'Water Estimate with SC',
        'water_est_wo_sc' => 'Water Estimate without SC',
        'mold_wt_water_mit_inv_wt_sc' => 'Mold with Water Mitigation Invoice with SC',
        'mold_wt_water_mit_inv_wo_sc' => 'Mold with Water Mitigation Invoice without SC',
        'mold_wt_water_mit_est_wt_sc' => 'Mold with Water Mitigation Estimate with SC',
        'mold_wt_water_mit_est_wo_sc' => 'Mold with Water Mitigation Estimate without SC',
        'mold_wo_water_mit_inv_wt_sc' => 'Mold without Water Mitigation Invoice with SC',
        'mold_wo_water_mit_inv_wo_sc' => 'Mold without Water Mitigation Invoice without SC',
        'mold_wo_water_mit_est_wt_sc' => 'Mold without Water Mitigation Estimate with SC',
        'mold_wo_water_mit_est_wo_sc' => 'Mold without Water Mitigation Estimate without SC',
        'recon_wt_water_mit_inv_wt_sc' => 'Recon Water Invoice with SC',
        'recon_wt_water_mit_inv_wo_sc' => 'Recon Water Invoice without SC',
        'recon_wt_water_mit_est_wt_sc' => 'Recon Water Estimate with SC',
        'recon_wt_water_mit_est_wo_sc' => 'Recon Water Estimate without SC',
        'recon_wt_mold_mit_inv_wt_sc' => 'Recon Invoice with Mold with SC',
        'recon_wt_mold_mit_inv_wo_sc' => 'Recon Invoice with Mold without SC',
        'recon_wt_mold_mit_est_wt_sc' => 'Recon Estimate with Mold with SC',
        'recon_wt_mold_mit_est_wo_sc' => 'Recon Estimate with Mold without SC',
        'recon_inv_fr_fire_wt_sc' => 'Recon Invoice from Fire with SC',
        'recon_inv_fr_fire_wo_sc' => 'Recon Invoice from Fire without SC',
        'recon_est_fr_fire_wt_sc' => 'Recon Estimate from Fire with SC',
        'recon_est_fr_fire_wo_sc' => 'Recon Estimate from Fire without SC',
        'stru_cln_inv_wt_sc' => 'Structural Cleaning Invoice with SC',
        'stru_cln_inv_wo_sc' => 'Structural Cleaning Invoice without SC',
        'stru_cln_est_wt_sc' => 'Structural Cleaning Estimate with SC',
        'stru_cln_est_wo_sc' => 'Structural Cleaning Estimate without SC',
        'cont_cln_inv_wt_sc' => 'Contents Cleaning Invoice with SC',
        'cont_cln_inv_wo_sc' => 'Contents Cleaning Invoice without SC',
        'cont_cln_est_wt_sc' => 'Contents Cleaning Estimate with SC',
        'cont_cln_est_wo_sc' => 'Contents Cleaning Estimate without SC',
        'general_inv_wt_sc' => 'General Invoice with SC',
        'general_inv_wo_sc' => 'General Invoice without SC',
        'general_est_wt_sc' => 'General Estimate with SC',
        'general_est_wo_sc' => 'General Estimate without SC',
        '*no*-xactimate' => 'Xactimate'
    );
    $content = "<select name=\"$name\" $attributes>";
    $content.= '<option value="">Please Select</option>';
    foreach($data as $tpl => $name)
    {
        $selected = '';
        if ($tpl == $select_val)
        {
            $selected = 'selected';
        }

        $content.= '<option value="' . $tpl . '" ' . $selected . ' >' . $name . '</option>';
    }

    $content.= "</select>";
    return $content;
}

function ncn_admin_get_coversheet_template($claim_invoice_type, $b_logo)
{
    $tpl_dir = drupal_get_path('module', 'ncn_admin') . '/cover_sheet/';
    if ($b_logo)
    {
        $tpl_dir = drupal_get_path('module', 'ncn_admin') . '/cover_sheet_with_logo/';
    }

    $tpl_filename = str_replace(' ', '_', strtolower($claim_invoice_type)) . ".doc";
    $tpl_filename = 'cover_sheet_' . $tpl_filename;
    $tpl_path = $tpl_dir . $tpl_filename;
    if (is_file($tpl_path))
    {
        return $tpl_path;
    }

    drupal_set_message(t("There isn't a cover sheet template file (!filename).  Please report this to webmaster", array(
        '!filename' => $tpl_filename
    )) , 'error');
    return '';
}

/**
 * Make Cover Sheet ( first page )
 */

function ncn_admin_make_cover_sheet($pdf_file_url, $data, $b_logo = false)
{
    $claim_invoice_type = isset($data['claim_invoice_type'])?$data['claim_invoice_type']:'';
    if (empty($claim_invoice_type))
    {
        drupal_set_message(t('Please choose claim invoice type.') , 'error');
        return FALSE;
    }

    if (!($tpl_path = ncn_admin_get_coversheet_template($claim_invoice_type, $b_logo)))
    {
        return FALSE;
    }

    $base_abs_path = $_SERVER['DOCUMENT_ROOT'];
    $tmp_doc_name = file_directory_temp() . "/tmp" . $GLOBALS['user']->uid . date('U') . microtime(true) . ".doc";

    watchdog('ncn_debug', 'tpl_path: '.$tpl_path);
    $cs = file_get_contents($tpl_path);
    if (!$cs) {
        drupal_set_message('Failed to open coversheet template', 'error');
        return FALSE;
    }

    $metadata = array(
        'company_name' => array(
            'label' => 'Company name (user profile: legal name)',
            'len' => 80
        ) ,
        'tax_id_number' => array(
            'label' => 'Tax id (user profile: tax id)',
            'len' => 40
        ) ,
        'company_address1' => array(
            'label' => 'Company address (user profile: address)',
            'len' => 40
        ) ,
        'company_address2' => array(
            'label' => 'Company address 2 (user profile: city state zip)',
            'len' => 40
        ) ,
        'company_office_phone' => array(
            'label' => 'Company office phone (user profile: office phone)',
            'len' => 30
        ) ,
        'company_fax_number' => array(
            'label' => 'Company Fax Number (Claim Invoice Data)',
            'len' => 30
        ) ,
        'data_entered' => array(
            'label' => 'Data Entered (Claim Invoice Data)',
            'len' => 40
        ) ,
        'price_list' => array(
            'label' => 'Price list (Claim Invoice Data)',
            'len' => 35
        ) ,
        'client_name' => array(
            'label' => 'Client name (Claim Data: Customer name)',
            'len' => 70
        ) ,
        'property1' => array(
            'label' => 'Insured address (Claim Data)',
            'len' => 40
        ) ,
        'property2' => array(
            'label' => 'Insured address 2 (Claim Data: city state zip)',
            'len' => 40
        ) ,
        'home_phone' => array(
            'label' => 'Insured phone (Claim Data: Insured Phone Number)',
            'len' => 30
        ) ,
        'iop_number' => array(
            'label' => 'Insured other phone number (Claim Invoice Data)',
            'len' => 40,
            'delta' => 10
        ) ,
        'type_of_loss' => array(
            'label' => 'Type of loss (Claim Invoice Data)',
            'len' => 30
        ) ,
        'date_of_loss' => array(
            'label' => 'Date of loss (Claim Data)',
            'len' => 30
        ) ,
        'insurance_company' => array(
            'label' => 'Insurance Company (Claim Invoice Data)',
            'len' => 80
        ) ,
        'claim_number' => array(
            'label' => 'Claim number (Claim Invoice Data)',
            'len' => 40
        ) ,
        'claim_adjuster' => array(
            'label' => 'Claim adjuster (Claim Invoice Data)',
            'len' => 40
        ) ,
        'claim_adjuster_phone' => array(
            'label' => 'Claim adjuster phone (Claim Invoice Data)',
            'len' => 40,
            'delta' => 10
        ) ,
        'claim_adjuster_fax' => array(
            'label' => 'Claim adjuster fax (Claim Invoice Data)',
            'len' => 30
        ) ,
        'start_date' => array(
            'label' => 'Start date (Claim Invoice Data)',
            'len' => 30
        ) ,
        'category' => array(
            'label' => 'Category (Claim Invoice Data)',
            'len' => 20
        ) ,
        'cause' => array(
            'label' => 'Cause (Claim Invoice Data)',
            'len' => 100
        ) ,
        'completion_date' => array(
            'label' => 'Completion Date (Claim Invoice Data)',
            'len' => 30
        ) ,
    );
    if (!ncn_admin_verify_coversheet_data($data, $metadata))
    {
        return FALSE;
    }

    // ------------------------------------------------------------------------
    // variables

    $cs = str_replace('{=====================company_name=============================================}', 
                        str_pad((isset($data['company_name'])?$data['company_name']:''), $metadata['company_name']['len'], "\n") , $cs);
    $cs = str_replace('{===========tax_id_number==============}', 
                        str_pad(_get_format_tax_id_string((isset($data['tax_id_number'])?$data['tax_id_number']:'')) , $metadata['tax_id_number']['len'], "\n") , $cs);
    $cs = str_replace('{===========company_address1===========}', 
                        str_pad((isset($data['company_address1'])?$data['company_address1']:''), $metadata['company_address1']['len'], "\n") , $cs);
    $cs = str_replace('{===========company_address2===========}', 
                        str_pad((isset($data['company_address2'])?$data['company_address2']:''), $metadata['company_address2']['len'], "\n") , $cs);
    $cs = str_replace('{===company_office_phone=====}', 
                        str_pad(get_format_phone_string((isset($data['company_office_phone'])?$data['company_office_phone']:'')) , $metadata['company_office_phone']['len'], "\n") , $cs);
    $cs = str_replace('{=====company_fax_number=====}', 
                        str_pad(get_format_phone_string((isset($data['company_fax_number'])?$data['company_fax_number']:'')) , $metadata['company_fax_number']['len'], "\n") , $cs);
    $cs = str_replace('{============data_entered==============}', 
                        str_pad((isset($data['data_entered'])?$data['company_fax_number']:''), $metadata['data_entered']['len'], "\n") , $cs);
    $cs = str_replace('{=============price_list==========}', 
                        str_pad((isset($data['price_list'])?$data['price_list']:''), $metadata['price_list']['len'], "\n") , $cs);
    $cs = str_replace('{==============================client_name===========================}', 
                        str_pad((isset($data['client_name'])?$data['client_name']:''), $metadata['client_name']['len'], "\n") , $cs);
    $cs = str_replace('{=============property1================}', 
                        str_pad((isset($data['property1'])?$data['property1']:''), $metadata['property1']['len'], "\n") , $cs);
    $cs = str_replace('{=============property2================}', 
                        str_pad((isset($data['property2'])?$data['property2']:''), $metadata['property2']['len'], "\n") , $cs);
    $cs = str_replace('{===========home_phone=======}', 
                        str_pad(get_format_phone_string((isset($data['home_phone'])?$data['home_phone']:'')) , $metadata['home_phone']['len'], "\n") , $cs);
    $cs = str_replace('{================iop_number============}', 
                        str_pad((isset($data['iop_type'])?$data['iop_type']:'') . ': ' . get_format_phone_string((isset($data['iop_number'])?$data['iop_number']:'')) , $metadata['iop_number']['len'], "\n") , $cs);
    $cs = str_replace('{=======type_of_loss=========}', 
                        str_pad((isset($data['type_of_loss'])?$data['type_of_loss']:''), $metadata['type_of_loss']['len'], "\n") , $cs);
    $cs = str_replace('{=======date_of_loss=========}', 
                        str_pad((isset($data['date_of_loss'])?$data['date_of_loss']:''), $metadata['date_of_loss']['len'], "\n") , $cs);
    $cs = str_replace('{=====================insurance_company========================================}', 
                        str_pad((isset($data['insurance_company'])?$data['insurance_company']:''), $metadata['insurance_company']['len'], "\n") , $cs);
    $cs = str_replace('{============claim_number==============}', 
                        str_pad((isset($data['claim_number'])?$data['claim_number']:''), $metadata['claim_number']['len'], "\n") , $cs);
    $cs = str_replace('{============claim_adjuster============}', 
                        str_pad((isset($data['claim_adjuster'])?$data['claim_adjuster']:''), $metadata['claim_adjuster']['len'], "\n") , $cs);
    $cs = str_replace('{========claim_adjuster_phone==========}', 
                        str_pad((isset($data['cap_type'])?$data['cap_type']:'') . ': ' . get_format_phone_string((isset($data['claim_adjuster_phone'])?$data['claim_adjuster_phone']:'')) , $metadata['claim_adjuster_phone']['len'], "\n") , $cs);
    $cs = str_replace('{===claim_adjuster_fax=======}', 
                        str_pad(get_format_phone_string((isset($data['claim_adjuster_fax'])?$data['claim_adjuster_fax']:'')) , $metadata['claim_adjuster_fax']['len'], "\n") , $cs);
    $cs = str_replace('{===========start_date=======}', 
                        str_pad((isset($data['start_date'])?$data['start_date']:''), $metadata['start_date']['len'], "\n") , $cs);
    $cs = str_replace('{======category====}', 
                        str_pad((isset($data['category'])?$data['category']:''), $metadata['category']['len'], "\n") , $cs);
    $cs = str_replace('{=====================cause========================================================================}', 
                        str_pad((isset($data['cause'])?$data['cause']:''), $metadata['cause']['len'], "\n") , $cs);
    $cs = str_replace('{========completion_date=====}', 
                        str_pad((isset($data['completion_date'])?$data['completion_date']:''), $metadata['completion_date']['len'], "\n") , $cs);

    // ------------------------------------------------------------------------

    $fp = fopen($tmp_doc_name, "wb");
    if (flock($fp, LOCK_EX))
    { // do an exclusive lock
        ftruncate($fp, 0); // truncate file
        fwrite($fp, $cs);
        flock($fp, LOCK_UN); // release the lock
    }
    else
    {
        drupal_set_message("File failed to load.", 'error');
    }

    fclose($fp);

    // drupal_set_message($tmp_doc_name);
    // convert
    // $command = "java -jar c:/jod/lib/jodconverter-core-3.0-beta-3.jar ".$tmp_doc_name." ".($base_abs_path."/".$pdf_file_url);
    // 
    // $command = "java -jar ../jod/lib/jodconverter-core-3.0-beta-3.jar " . $tmp_doc_name . " " . ($base_abs_path . "/" . $pdf_file_url);
    // $command = "java -jar ".$_SERVER["DOCUMENT_ROOT"]."/jod/lib/jodconverter-core-3.0-beta-3.jar " . $tmp_doc_name . " " . ($base_abs_path . "/" . $pdf_file_url);
    $command = "java -jar ".$_SERVER["DOCUMENT_ROOT"].JAR_JODCONVERTER . " $tmp_doc_name " . ($base_abs_path . "/" . $pdf_file_url);
    watchdog('java-cmd', $command);
    $output = shell_exec($command);
    return TRUE;
}

function ncn_admin_verify_coversheet_data($data, $metadata)
{
    $ret_val = TRUE;
    foreach($metadata as $key => $m_data)
    {
        $max_len = $m_data['len'] - ((isset($m_data['delta'])) ? $m_data['delta'] : 0);
        if (isset($data[$key]))
        {
            if (strlen($data[$key]) > $max_len)
            {
                drupal_set_message(t("\"@field\" should be @max_len characters at the maximum.", array(
                    '@field' => $m_data['label'],
                    '@max_len' => $max_len
                )) , 'error');
                $ret_val = FALSE;
            }
        }
    }

    return $ret_val;
}

function ncn_admin_upload_member_logo($member_id)
{

    /* Create Dir if not exists */
    if ($wrapper = file_stream_wrapper_get_instance_by_uri('public://')) {
        $path = $wrapper->realpath(); // output => C:\wamp\www\drupaldemo\sites\default\files
    }
    $logo_dir = $path."/member_logo/$member_id";
    //$logo_dir = "sites/default/files/member_logo/$member_id";
    if (is_dir($logo_dir)===false) {
        @drupal_mkdir($logo_dir);
    }

    $source = 'member_logo';
    //$file = file_save_upload($source);
    $file = file_save_upload($source, array(), 'public://member_logo/'.$member_id, FILE_EXISTS_RENAME);
    if (!$file)
    {
        drupal_set_message("Logo file of missing for upload ", "error");
        return FALSE;
    }
    
    $file->status = FILE_STATUS_PERMANENT; //Set the status of the uploaded file.
    $result = file_save($file);
    //$result = file_copy($file, $logo_dir);
    if ($result)
    {
        $api_params = array(
            'action' => 'scale',
            'height' => 300,
            'upscale' => FALSE,
            'source' => drupal_realpath($file->uri),
            'dest' => drupal_realpath($file->uri)
        );
        $result = _ncn_image_api('scale', $api_params);
    }

    if (!$result) 
    {
        drupal_set_message(t("Failed to upload member logo file.") , 'error');
        return FALSE;
    }

    $b_row = db_query("SELECT * FROM {ncn_member_logo} WHERE member_id='$member_id'")->fetchAssoc();
    $filepath = variable_get('file_public_path', conf_path() . '/files')."/member_logo/".$member_id."/".$file->filename;

    if (!empty($b_row))
    {
        //$result = db_query("UPDATE {ncn_member_logo} SET filename=':fn', filepath=':fp', filemime=':fm', timestamp=':ts' WHERE member_id=':mid'", array(':fn' => $file->filename, ':fp' => $filepath,':fm' => $file->filemime,':ts' => date('U') ,':mid' => $member_id));
        file_delete($b_row['filepath']);
        $result =  db_update('ncn_member_logo')->fields(array(
                                                          'filename' => $file->filename,
                                                          'filepath' => $filepath,
                                                          'filemime' => $file->filemime,
                                                          'timestamp' => date('U')
                        ))->condition('member_id',$member_id,'=')->execute();
    }
    else
    {
        //$result = db_query("INSERT INTO {ncn_member_logo}(member_id, filename, filepath, filemime, timestamp) VALUES(':mid', ':fn', ':fp', ':fm', ':ts')", array(':mid' => $member_id, ':fn' => $file->filename, ':fp' => $filepath,':fm' => $file->filemime,':ts' => date('U')));
        
        $result = db_insert('ncn_member_logo')->fields(array(
                                                          'member_id' => $member_id,
                                                          'filename' => $file->filename,
                                                          'filepath' => $filepath,
                                                          'filemime' => $file->filemime,
                                                          'timestamp' => date('U')
                        ))->execute();
    }
    
    if (!$result) 
    {
        drupal_set_message(t("Problem occurs to set member's logo") , 'error');
        return FALSE;
    }

    drupal_set_message(t("Success to upload member logo file"));
    return TRUE;
}

function ncn_admin_remove_member_logo($member_id)
{
    $logo = ncn_admin_get_member_logo_info($member_id);

    if (empty($logo))
    {
        return TRUE;
    }

    $result = db_query('DELETE FROM {ncn_member_logo} WHERE member_id=:a',array(':a'=>$member_id));
    if ($result)
    {
        $uri = 'public://member_logo/'.$member_id.'/'.$logo['filename'];;
        $file_rec = db_query('SELECT fid FROM {file_managed} WHERE uri =:a',array(':a'=>$uri))->fetchAssoc();

        $file = new stdClass;
        $file->fid = (isset($file_rec['fid']) ? $file_rec['fid'] : 0 );
        $file->uid = $logo['member_id'];
        $file->filename = $logo['filename'];
        $file->uri = $uri;
        $file->filemime = $logo['filemime'];
        file_delete($file);
        drupal_set_message(t('Removed member logo, successfully.'));
    }
    else
    {
        drupal_set_message(t('Failed to remove member logo.') , 'error');
        return FALSE;
    }

    return TRUE;
}

/**
 * Claim View Page - Action Handler
 * Send Claim Documents Reminder
 */

function ncn_admin_send_cdr($claim_id)
{
    $claim_info = ncn_admin_get_claim_info($claim_id);
    if (empty($claim_info))
    {
        drupal_set_message(t('Claim(#!claim_id) doesn\'t exist.', array(
            '!claim_id' => $claim_id
        )) , 'error');
        return FALSE;
    }

    $_user1 = user_load($claim_info['user_id']);
    $member = get_member_from_id($_user1->profile_memberid);
    $am_uid = $member['am_uid'];
    $am = user_load($am_uid);
    $subject = "Important! We have not received your claim documents.";
    $body = "Claim documents include:

      - Fax cover sheet 

        - Claims processing form

        - 1 Scope sheet per room

        - * Service contract and Assignment of benefits

        - * Certificate of satisfaction and completion

        - ** Job site photos submitted through Virtual Office Suite. 

        

        Please fax these documents to Net Claims Now at 877.654.8667. Once these documents are received, your Net Claims Now team can begin work on your invoice. If you have any questions please contact us at anytime.

        * Not required but highly recommended. May use your own company's service contract and/or certificate of completion. 

        ** If you have not yet submitted your job site photos to your Virtual Office Suite please do so.

    ";
    $params = array(
        'subject' => $subject,
        '!owner_name' => ncn_cd($claim_id, 'customer_name') ,
        '!claim_id' => $claim_id,
        '!am' => $am,
        '!am_fax' => ncn_admin_get_am_fax($am->uid)
    );
    $to = $_user1->mail;
    drupal_mail('ncn_admin', 'not_received_docs', $to, language_default() , $params);
    pm_send(0, $_user1->uid, $subject, $body, $claim_id, 0, 'no mail');
    drupal_set_message(t('Send a notification mail to said that we have not received claim documents.'));
    return TRUE;
}

function ncn_admin_draw_select_claim_type($name, $select_val, $attributes = '')
{
    $claim_type_data = array(
        'residential' => 'Residential',
        'commercial' => 'Commercial',
    );
    $output = "<select name='$name' $attributes>";
    foreach($claim_type_data as $key => $claim_type)
    {
        $select = ($key == strtolower($select_val)) ? 'selected' : '';
        $output.= "<option value='$key' $select>$claim_type</option>";
    }

    $output.= "</select>";
    return $output;
}

function ncn_admin_draw_select_claim_product($name, $select_val, $attributes = '')
{
    $claim_product_data = array(
        'Water' => 'Water',
        'Water With Sketch' => 'Water With Sketch',
        'Fire' => 'Fire',
        'Fire With Sketch' => 'Fire With Sketch',
        'Fire/Smoke Contents Cleaning' => 'Contents Cleaning',
        'Fire/Smoke Structure Cleaning' => 'Structure Cleaning',
        'Mold' => 'Mold',
        'Mold With Sketch' => 'Mold With Sketch',
        'Reconstruction' => 'Reconstruction',
        'Reconstruction With Sketch' => 'Reconstruction With Sketch',
    );
    $output = "<select name='$name' $attributes>";
    foreach($claim_product_data as $key => $claim_product)
    {
        $select = ($key == $select_val) ? 'selected' : '';
        $output.= "<option value='$key' $select>$claim_product</option>";
    }

    $output.= "</select>";
    return $output;
}

function ncn_admin_claim_type_change($claim_id)
{
    $claim_type = $_REQUEST['claim_type'];
    $claim_product = $_REQUEST['claim_product'];
    if ($claim_type)
    {
        $result = db_query('UPDATE {claims} SET claim_type=:a, claim_product=:b WHERE claim_id=:c',
            array(':a'=>$claim_type,':b'=>$claim_product,':c'=>$claim_id));
        if ($result)
        {
            drupal_set_message(t("Claim type was changed to !claim_type - !claim_product.", array(
                '!claim_type' => ucwords($claim_type) ,
                '!claim_product' => ucwords($claim_product)
            )));
            return TRUE;
        }
    }

    drupal_set_message(t("Failed to change claim type. Please report to web master."));
    return FALSE;
}

/********************** NCN Video ********************/
/**
 * Menu Handler (download/ncn_video)
 */

function ncn_admin_download_ncn_video()
{
    $video_path = base64_decode(arg(2));
    $params = array();
    ncn_admin_download_file($video_path);
}

function ncn_admin_settings_ncn_video_section()
{
    $form = array();
    $form['gen_video_link'] = array(
        '#type' => 'fieldset',
        '#title' => t('Generate NCN Video Download Link') ,
        '#weight' => - 42,
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
    );
    $form['gen_video_link']['explanation'] = array(
        '#type' => 'item',
        '#markup' => '<div>' . t('You can get a link to download video file using this form') . '</div>',
    );
    $form['gen_video_link']['ncn_video_path'] = array(
        '#type' => 'textfield',
        '#title' => t('Video Path') ,
        '#default_value' => 'sites/default/files/',
        '#size' => 100,
    );
    $form['gen_video_link']['ncn_video_link'] = array(
        '#type' => 'item',
        '#prefix' => '<div id="video_link_url" style="margin: 10px 0px;">',
        '#suffix' => '</div>',
        '#markup' => '<div style="font-weight: bold">Video Download URL (readonly): </div><input type="text" value="" readonly size=100/>',
    );
    $form['gen_video_link']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Generate a link to download video file') ,
        '#ajax' => array(
            'callback' => 'ncn_admin_generate_download_video_link',
            'wrapper' => 'video_link_url',
            'method' => 'replace',
            'effect' => 'fade',
        ) ,
    );
    return $form;
}

function ncn_admin_generate_download_video_link($form, $form_state)
{
    GLOBAL $base_url;
    $link_url = $base_url . "/download/ncn_video/" . base64_encode($_POST['ncn_video_path']);
    $output = '<div class="form-item form-type-item" id="edit-ncn-video-link"><div style="font-weight: bold">Video Download URL (readonly): </div><input type="text" value="' . $link_url . '" readonly size=100/></div>';
    return $output;
}

function ncn_admin_cancel_pause_member($uid)
{
    $returned = 0;
    if ($_POST['returned_datepicker'] != "")
    {
        $returned = strtotime($_POST['returned_datepicker']);
    }

    if ($returned > date('U'))
    {
        $account = user_load(array(
            'uid' => $uid
        ));

        // Skip blocking user if they are already blocked.

        if ($account !== FALSE && $account->status == 1)
        {
            user_save($account, array(
                'status' => 0
            ));
            ncn_user_insert_user_block_track($uid, date('U') , $returned);
            $message = "Member is blocked now";
            if ($returned)
            {
                $message.= " and would be reactive " . date("m/d/Y", $returned);
            }

            $message.= ".";
            drupal_set_message($message);
        }
    }
    else
    {
        drupal_set_message("You should set Returned Date later than now.", "error");
        return FALSE;
    }

    return TRUE;
}

function ncn_admin_unblock_member($uid)
{
    $account = user_load(array(
        'uid' => (int)$uid
    ));

    // Skip unblocking user if they are already unblocked.

    if ($account !== FALSE && $account->status == 0)
    {
        user_save($account, array(
            'status' => 1
        ));
        ncn_user_update_user_unblock_track($uid, date('U'));
        return TRUE;
    }

    return FALSE;
}

function ncn_admin_send_reminder_mail($uid)
{
    $_user = user_load($uid);
    if ($_user) {
        $member = get_member_from_id($_user->profile_memberid);
        $am_uid = $member['am_uid'];
        $am = user_load($am_uid);
        $user_pass = get_member_password($_user->profile_memberid, $_user->profile_firstname);
        $params = array(
            '!email'    => $_user->mail,
            '!username' => $_user->name,
            '!password' => $user_pass,
            '!am' => $am,
            '!am_fax' => ncn_admin_get_am_fax($am_uid)
        );
        if ($_user->mail)
        {
            drupal_mail('ncn_admin', 'congrats_claim_reminder', $_user->mail, language_default() , $params);
        }
    }
}

function ncn_admin_send_reminder_mail_using_claim($claim_id)
{
    if (ncn_admin_check_if_send_reminder_mail($claim_id))
    {
        $claim = ncn_admin_get_claim_info($claim_id);
        if ($claim)
        {
            ncn_admin_send_reminder_mail($claim['user_id']);
            return TRUE;
        }
    }
    return FALSE;
}

function ncn_admin_check_if_send_reminder_mail($claim_id)
{
    if (!ncn_admin_is_first_claim_for_member($claim_id))
    {
        return FALSE;
    }

    $claim = ncn_admin_get_claim_info($claim_id);
    if ($claim)
    {
        if ($claim['workflow'] == 'completed_status')
        {
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * mark 1st claim for which it sent mail after 1 week of completion
 */

function ncn_admin_mark_to_send_1st_mail($claim_id)
{
    if (ncn_admin_check_if_send_reminder_mail($claim_id))
    {
        $cme = ncn_admin_get_claim_mail_expiration($claim_id, '1st_claim');
        if (empty($cme))
        {
            $ts_expired = strtotime("+1 week");
            ncn_admin_insert_claim_mail_expiration(array(
                'claim_id' => $claim_id,
                'ts_expired' => $ts_expired,
                'sent' => '0',
                'type' => '1st_claim',
            ));
        }
    }
}

function ncn_admin_check_claim_mail_expiration($type)
{
    if ($type == '1st_claim')
    {
        ncn_admin_check_1st_claim_mail_expiration();
    }
}

function ncn_admin_check_1st_claim_mail_expiration()
{
    $now = date('U');
    $result = db_query('SELECT * FROM {claim_mail_expiration} WHERE sent=0 AND ts_expired<:a AND type=:b',
        array(':a'=>$now,':b'=>'1st_claim'));
    foreach($result as $row)
    {
        $row = (array)$row;
        $claim_id = $row['claim_id'];
        if (ncn_admin_send_reminder_mail_using_claim($claim_id))
        {
            db_query('UPDATE {claim_mail_expiration} SET sent=1 WHERE id=:a',array(':a'=>$row['id']));
        }
        else
        {
            watchdog('1st_claim_reminder_mail', 'Failed to send mail (id:!id, claim_id: #!claim_id)', array(
                '!id' => $row['id'],
                '!claim_id' => $claim_id
            ));
        }
    }
}

function draw_select_claim_view_claim_backend($name, $select_val, $attributes = '')
{
    $data = array(
        'incomplete' => 'Incomplete',
        'complete' => 'Complete',
    );
    $content = "<select name=\"$name\" $attributes>";
    foreach($data as $k => $v)
    {
        $selected = '';
        if ($k == $select_val)
        {
            $selected = 'selected';
        }

        $content.= '<option value="' . $k . '" ' . $selected . ' >' . $v . '</option>';
    }

    $content.= "</select>";
    return $content;
}

/**
 * Menu Handler(admin/config/ncn_membership_cal)
 */

function ncn_admin_membership_cal()
{
    GLOBAL $base_url;
    $timestamp = intval(arg(3));
    if (!$timestamp)
    {
        $timestamp = date('U');
    }

    $url = $base_url . '/admin/config/ncn_membership_cal/';
    if (isset($_REQUEST['rsd']))
    {
        $timestamp = strtotime($_REQUEST['rsd']);
        $timezone = variable_get('date_default_timezone', 0);
        $timestamp-= $timezone;
        drupal_goto($url . $timestamp);
        exit;
    }

    $next_month = strtotime("+1 month", $timestamp);
    $last_month = strtotime("-1 month", $timestamp);
    drupal_add_js(drupal_get_path('module', 'ncn_admin') . '/ncn_admin.js');
    ob_start();
?>

<div class="membership-calendar-header clear-block">

    <div class="calendar-date">

        <span><a href="<?php
    echo $url; ?>">[This Month]</a></span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;

        <span><a href="<?php
    echo $url . $last_month; ?>" title="<?php
    echo date("F Y", $last_month); ?>">[Last]</a></span>&nbsp;&nbsp;

        <span><input type="text" id="datepicker" value="<?php
    echo date("F Y", $timestamp); ?>" size="15" readonly></span>&nbsp;

        <span><a href="<?php
    echo $url . $next_month; ?>" title="<?php
    echo date("F Y", $next_month); ?>">[Next]</a></span>&nbsp;&nbsp;

    </div>

    <input type="hidden" id="membership_cal_url" value="<?php
    echo $url; ?>" />

    <?php
    echo ncn_report_insert_date_script('on_change_ncn_admin_membership_cal_time'); ?>

</div>

<?php
    ncn_admin_build_membership_calendar($timestamp);
    $content = ob_get_contents();
    ob_end_clean();
    return $content;
}

function ncn_admin_build_membership_calendar($timestamp)
{
    $month = date('n', $timestamp);
    $year = date('Y', $timestamp);
    $data = ncn_admin_get_membership_calendar($timestamp);
    echo "<div id='ncn_admin_membership_calendar'>";
    echo ncn_admin_draw_calendar($month, $year, $data);
    echo "</div>";
}

function ncn_admin_get_membership_calendar($timestamp)
{
    GLOBAL $base_url;
    $month = date('n', $timestamp);
    $year = date('Y', $timestamp);
    $days_in_month = date('t', $timestamp);
    $data = array();
    $upcoming_data = ncn_admin_get_upcoming_membership($timestamp);

    for ($day = 1; $day <= $days_in_month; $day++)
    {

        // $ts = mktime(0,0,0,$month,$day,$year);

        $ts = strtotime("$month/$day/$year");
        if ($ts < date('U'))
        {
            $p_day = ncn_admin_get_paid_membership($ts);
            $num = count($p_day);
        }
        else
        {
            $m_day = (isset($upcoming_data[$day]) ? $upcoming_data[$day] : array());
            $num = count($m_day);
        }

        $url = $base_url . '/admin/config/ncn_membership_cal_day_list/' . $ts;
        if ($num)
        {
            $data[$day] = "<div class='calendar_day_mebership'><a href='$url' target='_blank'>$num</a></div>";
        }
    }

    return $data;
}

function ncn_admin_membership_cal_day_list()
{
    GLOBAL $base_url;
    $timestamp = intval(arg(3));
    if (!$timestamp)
    {
        $timestamp = date('U');
    }

    $upcoming_data = ncn_admin_get_upcoming_membership($timestamp);
    $day = date('j', $timestamp);
    $title = 'Net Claims Now Membership List (' . date('d M Y', $timestamp) . ')';
    $upcoming = false;
    if ($timestamp < date('U'))
    {
        $p_day = ncn_admin_get_paid_membership($timestamp);
        $m_day = array();
        foreach($p_day as $item)
        {
            $member = get_member_from_id($item['member_id']);
            $member['mmf'] = $item;
            $m_day[] = $member;
        }

        $title.= " - Paid";
    }
    else
    {
        $m_day = $upcoming_data[$day];
        if (empty($m_day))
        {
            $m_day = array();
        }

        $title.= " - Upcoming";
        $upcoming = true;
    }

    drupal_set_title($title);
    $header = array(
        array(
            'data' => t('Member ID')
        ) ,
        array(
            'data' => t('Name')
        ) ,
        array(
            'data' => t('Company')
        ) ,
        array(
            'data' => t('Contact Email')
        ) ,
        array(
            'data' => t('SignUp Date')
        ) ,
        array(
            'data' => t('Owner')
        ) ,
    );
    if (!$upcoming)
    {
        $header[] = array(
            'data' => t('Payment ID')
        );
    }

    $rows = array();
    foreach($m_day as $member)
    {
        $_owner = user_load($member['owner']);
        $row_data = array(

            // Cells

            $member['member_id'],
            $member['first_name'] . ' ' . $member['last_name'],
            $member['legalname'],
            $member['contactemail'],
            date('d M Y', $member['create']) ,
            $_owner->profile_firstname . ' ' . $_owner->profile_lastname,
        );
        if (!$upcoming)
        {
            $row_data[] = $member['mmf']['pid'];
        }

        $rows[] = array(
            'data' => $row_data
        );
    }

    $output = '';
    $output.= theme('table',  array(
        'header' => $header,
        'rows' => $rows, 
        'attributes' => array('id' => 'membership_cal_day')
    ));
    return $output;
}

/**
 * Login Program Callback (ncn_admin/user/admin_login/%)
 */

function ncn_admin_user_login_program($uid)
{
    if (ncn_admin_login_programatically($uid))
    {
        $redirect_url = ncn_admin_get_workplace_name($uid);
        drupal_goto($redirect_url);
    }
    else
    {
        $_user = user_load($uid);
        drupal_set_message(t('Failed to log in as !name') , array(
            '!name' => $_user->name
        ) , 'error');
        drupal_goto('admin/config/ncn_create_user/all_user_list');
    }
}

/**
 * Login Program - Back to Admin Callback (user/back_to_admin)
 */

function ncn_admin_user_back_to_admin()
{
    global $user;
    if (isset($_SESSION['admin_sandbox']) && $_SESSION['admin_sandbox'] == $user->uid)
    {
        $user = user_load($_SESSION['sandbox_original_uid']);
        unset($_SESSION['admin_sandbox']);
        unset($_SESSION['sandbox_original_uid']);
        drupal_goto('admin/config/ncn_create_user/all_user_list');
    }

    $redirect_url = ncn_admin_get_workplace_name($user->uid);
    drupal_goto($redirect_url);
}

/**
 * Render Multi File Upload Section
 * refer to ncn_admin_draw_invoice_upload
 */

function ncn_admin_draw_multi_file_upload($claim_id, $type, $pvc_disabled = '')
{
    GLOBAL $base_url;
    $legend_title = "Upload Additional Documents";
    ob_start();
?>

<div class="multi-file-upload-section">

    <fieldset class="">

        <legend class="">

            <?php
    echo $legend_title; ?>

        </legend>

        <div class="section-wrapper">

            <?php
    if ($pvc_disabled != 'disabled'): ?>

        <?php
            $multi_file_upload_form = drupal_get_form('ncn_admin_multi_file_upload_form');
            print drupal_render($multi_file_upload_form); 
        ?>

            <?php
    endif; ?>

            <div id="multi-file-list">

            <?php
                $resOutput = drupal_get_form('ncn_admin_multi_file_list_form', $pvc_disabled, $claim_id);
                print (set_theme_ncn_admin_multi_file_list_form($resOutput));
            ?>

            </div>

        </div>

    </fieldset>

</div>

<?php
    $content = ob_get_contents();
    ob_end_clean();
    return $content;
}

function ncn_admin_multi_file_upload_form($form_state)
{
    $form = array();
    $form['#attributes'] = array(
        'enctype' => "multipart/form-data"
    );
    $form['additional_file'] = array(
        '#type' => 'file',
        '#title' => t('') ,
    );
    $form['file_gid'] = array(
        '#type' => 'hidden',
        '#value' => 100,
    );
    $form['current_scroll_position'] = array(
        '#type' => 'hidden',
        '#attributes' => array(
            'class' => t('current_scroll_position')
        ) ,
        '#value' => t('') ,
    );
    $form['next'] = array(
        '#type' => 'submit',
        '#weight' => 101,
        '#value' => t('Upload Additional Document') ,
        '#attributes' => array(
            'onclick' => 'return on_set_scroll_position()'
        )
    );
    return ($form);
}

function ncn_admin_multi_file_upload_form_validate($form, &$form_state)
{
    // check file was uploaded
    $file = file_save_upload('additional_file');
    if (!$file)
    {
        // If you want to require it, you'll want to do it here... something like this:
        form_set_error('Additional File missing for upload.');
        $form_state['values']['fid'] = 0;
    } else {
        $form_state['values']['fid'] = $file->fid;
    }
}

function ncn_admin_multi_file_upload_form_submit($form, &$form_state)
{
    if (isset($form_state['clicked_button']['#post']['current_scroll_position']))
    {
        $_SESSION['current_scroll_position'] = $form_state['clicked_button']['#post']['current_scroll_position'];
    }

    $claim_id = arg(4);
    $file_gid = intval($form_state['values']['file_gid']);

    // we know the file was good

    //$file = file_save_upload('additional_file');
    $file = file_load($form_state['values']['fid']);
    $newFilename = @strtr($file->filename,array("( "=>"", " )"=>"", "("=>"", ")"=>"", " "=>"_"));
    
    if ($file->filemime != 'application/pdf')
    {
        drupal_set_message('You should upload pdf file as additional document.', 'error');
        return FALSE;
    }

    $filePath = "sites/default/files/invoices/$claim_id/";
    $claim_dir = DRUPAL_ROOT."/".$filePath;

    if (!is_dir($claim_dir))
    {
        mkdir($claim_dir, 0755);
    }

    //$res = file_copy($file, "public://invoices/{$claim_id}", FILE_EXISTS_RENAME);
    $file->status = FILE_STATUS_PERMANENT;
    $res = file_move($file, "public://invoices/{$claim_id}/{$newFilename}", FILE_EXISTS_RENAME);
    
    // add file to database
    if ($res)
    {
        $file = $res;
        // get last weight;

        $result = db_query('SELECT weight FROM {claim_extra_file} WHERE claim_id=:a AND gid=:b ORDER BY weight DESC LIMIT 0,1',
            array(':a'=>$claim_id,':b'=>$file_gid));
        $weight = 0;
        foreach($result as $row)
        {
            $row = (array)$row;
            if ($row)
            {
                $weight = $row['weight'] + 1;
            }
        }

        $result = db_query('INSERT INTO {claim_extra_file}(claim_id, filename, filepath, filemime, `timestamp`, gid, weight)
                      VALUES(:a,:b,:c,:d,:e,:f,:g)',
            array(':a'=>$claim_id,':b'=>$file->filename,':c'=>str_replace("public://","sites/default/files/",$file->uri),':d'=>$file->filemime,':e'=>date('U'),
            ':f'=>$file_gid,':g'=>$weight));

        // add entry to logfile

        $log_message = t("!user_name uploaded new additional document: !file_name", array(
            '!user_name' => ncn_amin_get_user_role_and_name() ,
            '!file_name' => $file->filename
        ));
        ncn_admin_insert_claim_log($claim_id, date('U') , $log_message);

        // success!

        $log_message = "Uploaded new additional document: " . $file->filename;
        drupal_set_message($log_message, 'status');
    }
}

function ncn_admin_multi_file_list_form($form,&$form_state, $pvc_disabled, $claim_id)
{
    $form = array();
    $form['#method'] = 'post';
    $form['#claim_id'] = $claim_id;
    $form['#pvc_disabled'] = $pvc_disabled;
    $file_gid = 100;
    $result = db_query('SELECT * FROM {claim_extra_file} WHERE claim_id=:a AND gid=:b ORDER BY weight ASC, timestamp ASC', array(':a'=>$claim_id,':b'=>$file_gid));
    foreach($result as $row)
    {
        $form['rows']['weight']['id_' . $row->id] = array(
            '#type' => 'weight',
            '#size' => 5,
            '#delta' => 50,
            '#default_value' => $row->weight,
            '#attributes' => array('class' => array('weight')), // add a specific class in here - we need this later
        );
    }

    if (isset($form['rows']) && is_array($form['rows']) && $pvc_disabled != 'disabled')
    {
        $form['current_scroll_position'] = array(
            '#type' => 'hidden',
            '#attributes' => array('class' => array(t('current_scroll_position'))) ,
            '#value' => t('') ,
        );
        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Save changes') ,
            '#attributes' => array('onclick' => 'return on_set_scroll_position()') ,
        );
    }

    return $form;
}

function set_theme_ncn_admin_multi_file_list_form($form)
{
    $claim_id = $form['#claim_id'];
    $pvc_disabled = $form['#pvc_disabled'];
    $table_rows = array();
    if (isset($form['rows']) && is_array($form['rows']))
    {
        foreach($form['rows']['weight'] as $str_id => $row)
        {
            // We are only interested in numeric keys
            $id = intval(substr($str_id, 3));
            $file = ncn_admin_get_claim_extra_file($id);
            if ($file)
            {
                $this_row = array();
                $this_row[] = $file['filename'];
                $this_row[] = date('m/d/Y H:i', $file['timestamp']);
                $this_row[] = drupal_render($form['rows']['weight'][$str_id]);
                $this_row[] = ($pvc_disabled != 'disabled') ? l(t('Delete') , "admin/delete_claim_additional_document/$claim_id/$id") : '';

                // Add the row to the array of rows
                $table_rows[] = array('data' => $this_row,'class' => array('draggable'));
            }
        }

        // Set the table headers.
        $header = array("File Name","Timestamp","Weight","Operation");
        $table = theme('table', array('header' => $header,'rows' => $table_rows,'attributes'=>array('id'=>'ncn_claim_additional_docs')));
        $form['rows'] = array('#markup' => $table);

        // Render the entire form
        $output = drupal_render($form);
        drupal_add_tabledrag('ncn_claim_additional_docs', 'order', 'sibling', 'weight');
        return $output;
    }
    return '';
}

/**
 * Implementation of hook_form_submit().
 */

function ncn_admin_multi_file_list_form_submit($form, &$form_state)
{
    if (isset($form_state['clicked_button']['#post']['current_scroll_position']))
    {
        $_SESSION['current_scroll_position'] = $form_state['clicked_button']['#post']['current_scroll_position'];
    }

    foreach($form_state['values'] as $key => $value)
    {
        if (strpos($key, 'id_') === 0)
        {
            $id = intval(substr($key, 3));
            $result = db_query('UPDATE {claim_extra_file} SET weight=:a WHERE id=:b',
                array(':a'=>$value,':b'=>$id));
        }
    }
}

function _get_additional_docs_pdf($claim_id, $dir_url, &$additional_file_removed)
{
    $base_abs_path = $_SERVER['DOCUMENT_ROOT'];
    $pdf_file_url = $dir_url . time() . "_additional.pdf";
    $file_gid = 100;
    $result = db_query('SELECT * FROM {claim_extra_file} WHERE claim_id=:a AND gid=:b ORDER BY weight ASC, timestamp ASC',
        array(':a'=>$claim_id,':b'=>$file_gid));
    $additional_pdfs = array();
//  while ($row = db_fetch_object($result))
    foreach($result as $row)
    {
        if (file_exists($base_abs_path . "/" . $row->filepath))
        {
            $additional_pdfs[] = $row;
        }
    }

    if (count($additional_pdfs) == 0)
    {
        return '';
    }
    else
    if (count($additional_pdfs) == 1)
    {
        $additional_file_removed = FALSE;
        return $additional_pdfs[0]->filepath;
    }

    $foption = '';
    foreach($additional_pdfs as $file)
    {
        $foption.= " -f $base_abs_path/" . $file->filepath;
    }

    $out_option = " -o $base_abs_path/$pdf_file_url";

    // $command = "java -jar C:/pdfsam/lib/pdfsam-console-2.3.1e.jar $foption $out_option concat";

    // $command = "java -jar ../pdfsam/lib/pdfsam-console-2.3.1e.jar $foption $out_option concat";
    $command = "java -jar ".$_SERVER["DOCUMENT_ROOT"]."/pdfsam/lib/pdfsam-console-2.3.1e.jar $foption $out_option concat";
    $output = shell_exec($command);
    if (file_exists($base_abs_path . "/" . $pdf_file_url))
    {
        return $pdf_file_url;
    }
    else
    {
        drupal_set_message("Failed to merge additional pdf files.", "warning");
    }

    return '';
}

function ncn_admin_delete_claim_additional_document($claim_id, $file_id)
{
    $file = ncn_admin_get_claim_extra_file($file_id);

    if ($file)
    {
        $result = db_query('DELETE FROM {claim_extra_file} WHERE id=:a', array(':a'=>$file_id));
        if ($result)
        {
            /*Create File Object from File path*/
            $uri = str_replace("sites/default/files/","public://",$file['filepath']);
            $fileObj = new stdClass();
            $fileObj->uid = $GLOBALS['user']->uid;
            $fileObj->filename = basename($uri);
            $fileObj->uri = $uri;
            $fileObj->filemime = file_get_mimetype($uri);
            $fileObj->timestamp = REQUEST_TIME;
            $fileObj->status = FILE_STATUS_PERMANENT;

            file_delete($fileObj);
            drupal_set_message("Success to delete additional pdf (" . $file['filename'] . ")");
        }
    }
    drupal_goto('admin/config/ncn_admin/viewclaim/'. $claim_id, array('query'=>NULL, 'fragment'=>'multi-file-list'));
}

function ncn_admin_render_user_claim_section($uid)
{
    global $base_url;
    global $user;
    $user_id = $uid;
    $_user = user_load($uid);

    if (is_subuser($_user))
    {
        $parent_mid = ncn_subuser_get_memberid_from_subuid($uid);
        $user_id = get_uid_from_memberid($parent_mid);
    }

    if (stristr($_SERVER['REQUEST_URI'], 'ncn_admin_pool'))
    {
        $url_part = "ncn_admin_pool";
    }
    else
    {
        $url_part = "ncn_admin";
    }

    $query = "SELECT * FROM claims";
    $where = " WHERE claim_status != 'unpurchased' AND deleted=0 AND user_id=$user_id";
    $query.= $where;
    $query.= " ORDER BY last_modified DESC";
    $query_total = db_query("SELECT COUNT(*) as tot FROM claims" . $where)->fetchField();
    $total_count = intval($query_total);
    $result = db_query($query);
    $row_count = $result->rowCount();
    echo "<div style='float:left;'><a href='$base_url/ncn/ncn_claim_report' target='_blank'>NCN Claim Report</a></div>";
    echo '<div style="text-align: right">Number of Claims:&nbsp;&nbsp;' . $total_count . '</div>';
    $column_count = ($user->uid == 1) ? 12 : 11;
    $table_head = '

    <thead class="tableHeader-processed">

    <tr>

        <th>Claim ID</th>

        <th>Type</th>

        <th>Product</th>

        <th>Customer</th>

        <th>Zip Code</th>

        <th>User</th>

        <th>Created</th>

        <th>Modified</th>

        <th>Status</th>

        <th>Workflow</th>

        <th>Changes Logfile</th>' . (($user->uid == 1) ? '<th></th>' : '') . '</tr>

    </thead>



    <tbody>';
    $table_foot = '

    </tbody>

    </table>';
    if ($total_count == 0)
    {
        print "<table>";
        print $table_head;
?>

        <tr><td colspan="<?php
        echo $column_count; ?>">No results</td></tr>

        <?php
        print $table_foot;
    }
    else
    {
        echo '<table class="sticky-enabled tableSelect-processed sticky-table page_table">';
        echo $table_head;
        for ($i = 0; $i < $row_count; $i++)
        {

            // get data

            $row = $result->fetchAssoc();
            $claim_id = $row['claim_id'];
            $claim_type = $row['claim_type'];
            $claim_product = $row['claim_product'];
            $_user = user_load($row['user_id']);
        //  $query2 = "SELECT * FROM claims_log WHERE claim_id=" . $claim_id . ";";
            $result2 = db_query('SELECT * FROM {claims_log} WHERE claim_id=:a',array(':a'=>$claim_id));
            $entries_in_log = $result2->rowCount();

            // draw row

?>

            <tr class="<?php
            if ($i % 2) print "odd";
            else print "even"; ?>">

                <td><a href="<?php
            echo $base_url; ?>/admin/config/<?php
            echo $url_part; ?>/viewclaim/<?php
            echo $claim_id; ?>"><?php
            echo $claim_id; ?></a>

                    <?php
            if (ncn_admin_get_claim_first_free_locked($claim_id) == 'LOCKED')
            {
                echo '(Locked)';
            } ?></td>

                <td><?php
            echo ucwords($claim_type); ?></td>

                <td><?php
            echo ucwords($claim_product); ?></td>

                <td><?php
            echo ncn_cd($claim_id, 'customer_name'); ?></td>

                <td><?php
            echo ncn_cd($claim_id, 'insured_zip'); ?></td>

                <td><?php
            echo $_user->profile_firstname . ' ' . $_user->profile_lastname ?> (<?php
            echo $_user->name; ?>)</td>

                <td><?php
            echo date("m/d/Y", $row['created']); ?></td>

                <td><?php
            echo date("m/d/Y", $row['last_modified']); ?></td>

                <td><?php
            echo ucwords($row['claim_status']); ?></td>

                <td><?php
            $timer_trigger = render_claim_timer($claim_id, 1);
            $workflow = render_claim_workflow($claim_id);
            echo $workflow;
            if ($timer_trigger != '')
            {
                echo "<div>($timer_trigger)<div>";
            }

?>

                </td>

                <td><a href="<?php
            echo $base_url; ?>/admin/config/<?php
            echo $url_part; ?>/viewlog/<?php
            echo $claim_id; ?>"><?php
            echo $entries_in_log; ?> entries in log</a></td>

                <?php
            if ($user->uid == 1)
            { ?>

                    <td><a href="<?php
                echo $base_url; ?>/admin/config/<?php
                echo $url_part; ?>/deleteclaim/<?php
                echo $claim_id; ?>?dest=<?php
                echo urlencode(request_uri()); ?>" onclick="return delete_claim(<?php
                echo $claim_id; ?>)">Delete</a></td>

                <?php
            } ?>

            </tr>

<?php
        }

        print $table_foot;
    }
}

/**
 * Menu Handler (admin/config/ncn_user/%/all_payments)
 */

function ncn_admin_ncn_user_all_payments($user_id)
{
    if ($user_id)
    {
        $_user = user_load($user_id);
        $member_id = isset($_user->profile_memberid)?$_user->profile_memberid:0;
    }

    $output = '';
    $query = "SELECT payment_log.*, claims.expedite FROM payment_log 
                LEFT JOIN claims ON (payment_log.claim_id=claims.claim_id) 
                WHERE status=1 AND (uid=$user_id OR member_id='$member_id')";
    $total_count = db_query('SELECT COUNT(*) as tot FROM payment_log WHERE status=1 AND (uid=:a OR member_id=:b)',
        array(':a'=>$user_id,':b'=>$member_id))->fetchField();
    $output.= ncn_admin_render_payment_table($query, $total_count);
    return $output;
}

/**
 * Menu Handler (admin/config/ncn_user/%/ytd_payments)
 */

function ncn_admin_ncn_user_ytd_payments($user_id)
{
    if ($user_id)
    {
        $_user = user_load($user_id);
        $member_id = isset($_user->profile_memberid)?$_user->profile_memberid:0;
    }

    $year = date('Y', date('U'));
    $first_day = strtotime("1/1/$year");
    $output = '';
    $query = "SELECT payment_log.*, claims.expedite 
                FROM payment_log LEFT JOIN claims ON (payment_log.claim_id=claims.claim_id) 
                WHERE status=1 AND (uid=$user_id OR member_id='$member_id') AND timestamp>$first_day";
    $total_count = db_query('SELECT COUNT(*) AS tot FROM payment_log WHERE status=1 AND (uid=:a OR member_id=:b) AND timestamp>:c',
    array(':a'=>$user_id,':b'=>$member_id,':c'=>$first_day))->fetchField();
    $output.= ncn_admin_render_payment_table($query, $total_count);
    return $output;
}

/**
 * Render Payment Table using Query Result
 */

function ncn_admin_render_payment_table($query, $total_count)
{
    global $base_url;

    drupal_add_js(drupal_get_path('module', 'ncn_admin') . '/ncn_admin.js');
    $page = 0;
    if (isset($_REQUEST['page']))
    {
        $page = intval($_REQUEST['page']);
    }

    $num_per_page = 50;
    if ($page >= ceil($total_count / $num_per_page))
    {
        $page = ceil($total_count / $num_per_page) - 1;
    }
    if ($page < 0) { $page = 0; }

    $query.= " ORDER BY timestamp DESC";
    $query.= " LIMIT " . ($page * $num_per_page) . ", $num_per_page";
    $result = db_query($query);
    $row_count = 0;
    if ($result)
    {
        $row_count = $result->rowCount();
    }

    $data_type = get_transaction_type('all');
    $table_head = '

    <thead class="tableHeader-processed">

        <th>PID</th>

        <th>Date</th>

        <th>Type</th>

        <th>Name</th>

        <th>Cost</th>

    </thead>

    <tbody>';
    $table_foot = '

    </tbody>

    </table>';
    $url = $base_url . '/admin/config/ncn_admin/transaction';
    drupal_add_js("

        $(function() {

        $( '#specific_date' ).datepicker({

            showOn: \"button\",

            buttonImage: \"$base_url/" . drupal_get_path('module', 'ncn_report') . "/images/calendar.gif\",

            buttonImageOnly: true,

            dateFormat: 'mm/dd/yy',

        });

    });", 'inline');
    ob_start();
?>

<div id="payment_transaction_page">

    <div class="description">   </div>

    <div class="transaction-table">

<?php

    // no results message

    if ($total_count == 0)
    {
        print "<table>";
        print $table_head;
        print $table_foot;
    }
    else
    {
        echo '<table class="sticky-enabled tableSelect-processed sticky-table">';
        echo $table_head;
        for ($i = 0; $i < $row_count; $i++)
        {
            $row = $result->fetchAssoc();;
            $ptype = get_item_from_key($data_type, 'ptype', $row['ptype']);
            if ($row['expedite'] == 1)
            {
                $ptype['name'].= " - EXPEDITE";
            }

            $pname = get_payment_from($row);
            $str_cost = render_payment_cost($row['cost']);

            // draw row

?>

            <tr class="<?php
            if ($i % 2) print "even";
            else print "odd"; ?> <?php
            if ($row['refund'] == - 1) print "refund"; ?>">

                <td><?php
            echo $row['pid']; ?></td>

                <td><?php
            echo date('d M, Y H:i', $row['timestamp']); ?></td>

                <td><?php
            echo $ptype['name']; ?></td>

                <td><?php
            echo $pname; ?></td>

                <td><?php
            echo $str_cost; ?></td>             

            </tr>

            <?php
        }

        echo $table_foot;
        $url = $base_url . "/" . drupal_get_path_alias($_GET["q"]);
        echo ncn_admin_table_pagniation($url, $total_count, $num_per_page, $page, 15);
    }

?>

</div></div>

<?php
    $content = ob_get_contents();
    ob_end_clean();
    return $content;
}

/**
 * Action Handler ( download/claim_list )
 *
 */

function ncn_admin_download_claim_list()
{
    $claim_list = ncn_admin_get_downloadable_claim_list('all');
    require_once 'sites/all/libraries/php_xls/Classes/PHPExcel.php';

    $report_name = "claims.xls";
    $demo_file = "claims.xls";
    $objReader = PHPExcel_IOFactory::createReader("Excel5");
    $objPHPExcel = $objReader->load($demo_file);
    $count = count($claim_list);
    if ($count > 0)
    {
        $objPHPExcel->getActiveSheet()->insertNewRowBefore(9, $count);
    }

    for ($i = 0; $i < $count; $i++)
    {
        $source = "A7:J7";
        if ($i % 2 == 1)
        {
            $source = "A8:J8";
        }

        $row_index = $i + 9;
        $dest = "A$row_index:J$row_index";
        $objPHPExcel->getActiveSheet()->duplicateStyle($objPHPExcel->getActiveSheet()->getStyle($source) , $dest);
    }

    $objPHPExcel->getActiveSheet()->removeRow(7 + $count, 2);
    $i = 0;
    foreach($claim_list as $claim)
    {
        $row_index = $i + 7;
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("A$row_index", $claim['claim_id']);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("B$row_index", $claim['type']);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("C$row_index", $claim['product']);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("D$row_index", $claim['claim_amount']);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("E$row_index", $claim['payment_received']);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("F$row_index", $claim['customer']);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("G$row_index", $claim['user_name']);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("H$row_index", $claim['company']);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("I$row_index", $claim['created']);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("J$row_index", $claim['status']);
        $i+= 1;
    }

    header('Content-Type: application/vnd.ms-excel');
    header('Content-Disposition: attachment;filename="' . $report_name . '"');
    header('Cache-Control: max-age=0');
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5');
    $objWriter->save('php://output');
    exit;
}

function ncn_admin_get_downloadable_claim_list($mode)
{
    $data = array();
    $query = "SELECT * FROM {claims}";
    $where = " WHERE claim_status != 'unpurchased' AND deleted=0 AND created>".strtotime('-1 year');
    $query.= $where;
    $query.= " ORDER BY last_modified DESC";
    $result = db_query($query);
    foreach($result as $row)
    {
        $row = (array)$row;
        $claim_id = $row['claim_id'];
        $claim_type = $row['claim_type'];
        $claim_product = $row['claim_product'];
        if ($claim_product == 'Fire/Smoke Contents Cleaning')
        {
            $claim_product = 'Contents Cleaning';
        }

        if ($claim_product == 'Fire/Smoke Structure Cleaning')
        {
            $claim_product = 'Structure Cleaning';
        }

        if (ncn_cd($claim_id, 'expedite'))
        {
            $claim_product.= " (EXPEDITE)";
        }

        $_user = user_load($row['user_id']);
        $claim_data = array(
            'claim_id' => $claim_id,
            'type' => ucwords($claim_type) ,
            'product' => ucwords($claim_product) ,
            'claim_amount' => "$" . $row['claim_amount'],
            'payment_received' => "$" . $row['payment_received'],
            'customer' => ncn_cd($claim_id, 'customer_name') ,
            'user_name' => $_user->profile_firstname . ' ' . $_user->profile_lastname,
            'company' => $_user->profile_legalname,
            'created' => date("m/d/Y", $row['created']) ,
            'status' => ucwords($row['claim_status']) ,
        );
        $data[] = $claim_data;
    }

    return $data;
}

function render_ncn_admin_member_renewal_select($name, $member_id, $attributes = '')
{
    $charge_day = ncn_user_get_member_renewal_day($member_id);
    
    if($_SERVER['REMOTE_ADDR']=='103.240.34.101'){
        //var_dump($charge_day);
        //var_dump($member_id);
    }
    
    $data = array(
        1 => '1st',
        2 => '2nd',
    );
    for ($day = 3; $day <= 31; $day++)
    {
        $str_day = $day . "th";
        $data[$day] = $str_day;
    }

    $content = "<select name=\"$name\" $attributes>";
    foreach($data as $day => $str_day)
    {
        $selected = '';
        if ($charge_day == $day)
        {
            $selected = 'selected';
        }

        $content.= "<option value='$day' $selected>$str_day</option>";
    }

    $content.= "</select>";
    return $content;
}

//------------------------------------------------------------------------------
// delete claims from users that no longer exist
function ncn_admin_cleanup_claims()
{
    // delete claims from dead users
//  $query = "SELECT DISTINCT user_id FROM claims;";
    $result = db_query('SELECT DISTINCT user_id FROM claims');
    foreach($result as $row)
    {
        $row = (array)$row;
                
        // check to see if this user exists
    //  $query2 = "SELECT * FROM users WHERE uid=".$row['user_id'];
        $result2 = db_query('SELECT * FROM users WHERE uid=:a',array(':a'=>$row['user_id']));
        $row_count2 = $result2->rowCount();
        
        // user no longer exists...
        if ($row_count2 == 0)
        {
            // delete claims from this dead user
        //  $query2 = "DELETE FROM claims WHERE user_id=".$row['user_id'];
            $result2 = db_query('DELETE FROM {claims} WHERE user_id=:a',array(':a'=>$row['user_id']));
            
        }
    }
}

function testmail(){

    $_user = user_load(1013);
    profile_user_load(array($_user->uid => $_user));
    $member_type = get_member_type($_user);

    $node_title='Activation Mail (Gold)';
    $node_id = 55;
    if ($member_type == 0) {    // Gold Member
        $node_title='Activation Mail (Gold)';
        $node_id = 55;
    } else if ($member_type == 1) {
        $node_title='Activation Mail (Silver)';
        $node_id = 56;
    } else if ($member_type == 2) {
        $node_title='Activation Mail (Gold)';
        $node_id = 55;
    } else if ($member_type == 3) {
        $node_title='Activation Mail (Gold)';
        $node_id = 55;
    } else if ($member_type == 4) {
        $node_title='Activation Mail (Gold)';
        $node_id = 55;
    } else if ($member_type == 5) {
        $node_title='Activation Mail (Gold)';
        $node_id = 55;
    }

    $result = db_query('SELECT * FROM {node} WHERE type=:a AND nid=:b',array(':a'=>'attached_documents',':b'=>$node_id));

    if ($result->rowCount()>0)
    {
        foreach($result as $row)
        {
            $row = (array)$row;
            $node = node_load($row['nid']);
        }

        $index = 0;
        $atchfiles = array_pop($node->field_attach_docs);
        
        foreach ($atchfiles as $file)
        {
            $file['filepath'] = str_replace("public://","sites/default/files/",$file['uri']);
            $attaches['doc-'.$index] = $file['filepath'];
            $attache_names['doc-'.$index] = $file['filename'];
            $attach_mail_data[$file['description']] = array('filepath'=>$file['filepath']);
            $index++;
        }
    }

    $_params = array('!name'=>($_user->profile_firstname." ".$_user->profile_lastname) ,'!docs'=>$attach_mail_data);
    $ouput = ncn_admin_get_mail_body_from_template('create_user', $_params);
    print $ouput;
}

//------------------------------------------------------------------------------
function ncn_admin_serve_user_document()
{
    $error = false;
    
    // check numeric
    if ((!is_numeric(arg(2))) || (!is_numeric(arg(3))))
    {   $error = true;      }
    else
    { 

    //  $query = "SELECT * FROM member_documents WHERE id=".arg(2)." AND uid=".arg(3);
        $result = db_query('SELECT * FROM {member_documents} WHERE id=:a  AND uid=:b',
            array(':a'=>arg(2),':b'=>arg(3)));
        $row_count = $result->rowCount();
        
        if ($row_count > 0)
        {
            // server the file
            $row = $result->fetchAssoc();
            
            header('Content-type: '.$row['filemime']);
            header('Content-Disposition: attachment; filename="'.$row['filename'].'"');
            readfile($row['filepath']);
            exit;
            
        }
        // not found
        else
        {   $error = true;  }
        
    }
    
    // check errors
    if ($error == true)
    {
        ?>
        <script type="text/javascript">
        history.go(-1);
        </script>
        <?php
        exit;
    }

}

//------------------------------------------------------------------------------
function ncn_admin_delete_user_document()
{

    if ((!is_numeric(arg(2))) || (!is_numeric(arg(3))))
    {   
        drupal_set_message('There was a problem with the url.', 'error');
        Header("Location:/admin");  
    }
    else
    {
        // delete the doc
//      $query = "DELETE FROM member_documents WHERE id=".arg(2)." AND uid=".arg(3);
        $result = db_query('DELETE FROM {member_documents} WHERE id=:a AND uid=:b',array(':a'=>arg(2),':b'=>arg(3)));

        // success
        drupal_set_message("User document deleted.", 'status');
    
        // bounce back to user edit page (as this si the only place you can delete user documents)
        Header("Location:/admin/user/user/edit_user/".arg(3));
    }
    
    exit;
    

}

/*
 * Track user to accept NDA.
 */
function ncn_track_nda_log($member_id) {
    $timestamp = date('U');
    $ip_addr = $_SERVER['REMOTE_ADDR'];
    $member = get_member_from_id($member_id);
    
    //$query = "INSERT INTO nda_log VALUES(NULL, '$ip_addr', '$member_id', $timestamp, '".$member['first_name']."', '".$member['last_name']."')";
    $result = db_query('INSERT INTO {nda_log} VALUES(NULL,:a,:b,:c,:d,:e)',
        array(':a'=>$ip_addr,':b'=>$member_id,':c'=>$timestamp,':d'=>$member['first_name'],':e'=>$member['last_name']));
    if (!$result) {
        watchdog('nda_log', 'Failed to log nda-track from %ip_addr (member_id: %member_id, date: %date)', array('%ip_addr'=>$ip_addr, '%member_id'=>$member_id, '%date'=>date('d M Y H:i:s', $timestamp)));
    }
}

/*
 * Track IP Address List Page
 */
function ncn_admin_nda_log() {
    drupal_add_js(drupal_get_path('module', 'ncn_admin') . '/ncn_admin.js');
    
//  $query = "SELECT * FROM nda_log ORDER BY timestamp DESC";
    $result = db_query('SELECT * FROM {nda_log} ORDER BY timestamp DESC');
    $row_count = $result->rowCount();
    
    $table_head = '
    <thead class="tableHeader-processed">
        <th>Date</th>
        <th>IP Addresss</th>
        <th>Member ID</th>
        <th>First Name</th>
        <th>Last Name</th>
    </thead>
    <tbody>';

    $table_foot = '
    </tbody>
    </table>';
    
    ob_start();
    
    // no results message
    if ($row_count == 0)
    {
        print "<table>";
        print $table_head;
        ?>
        <tr><td colspan=5>No results</td></tr>
        <?php
        print $table_foot;
        
    }
    
    $count = 0;
    $num_per_page = 50;
    $on_page = 0;
    $current_page = 1;

    for ($i=0; $i<$row_count; $i++)
    { 
        // get data
        $row = $result->fetchAssoc();
        $_first_name = $row['m_first_name'];
        $_last_name = $row['m_last_name'];
        
        $member_id = $row['member_id'];
        $member = get_member_from_id($member_id);
        
        if (empty($member)) {
            $member_id .= "(*removed)";
        } else {
            $_first_name = $member['first_name'];
            $_last_name = $member['last_name'];
        }
        
    // header?
        if ($on_page == 0)
        {
            if ($current_page == 1)
            {   ?><table class="sticky-enabled tableSelect-processed sticky-table page_table" id="page_table_<?= $current_page; ?>"><?php   }
            else
            {   ?><table class="sticky-enabled tableSelect-processed sticky-table page_table"  id="page_table_<?= $current_page; ?>" style="display:none;"><?php    }
                
            print $table_head;  
        }
    
        // draw row
        ?>
        <tr class="<?php if ($count%2) print "even"; else print "odd"; ?>">
            <td><?= date('d M Y H:i:s', $row['timestamp']); ?></td>
            <td><?= $row['ip_addr']; ?></td>
            <td><?= $member_id; ?></td>
            <td><?= $_first_name; ?></td>
            <td><?= $_last_name; ?></td>
        </tr>
        <?php

        // update page count            
        $count++;
        $on_page++;
        
        // check end of page
        if ($on_page == $num_per_page)
        {
            ?>
            <?php               
        
            print $table_foot;
            
            $on_page = 0;
            $current_page += 1;
        }
    }
        
    if ($on_page == 0) {
        $current_page -= 1;
    }
    // end pagination
    ?>
    <table class="sticky-enabled tableSelect-processed sticky-table">
    <tbody>
    <tr>
        <td colspan="7" align="right">
            
            <?php
                if ($current_page > 1) {
                    echo t("Page:");
                
                    for ($i=1; $i<= $current_page; $i++)
                    {
                        if ($i > 1)
                        {   
                            print " | ";
                            $page_text = '<u>'.$i.'</u>';   
                        }
                        else
                        {   $page_text = '<strong><u>'.$i.'</u></strong>';  }   
                        
                        
                        ?>
                        <a href="#" onclick="change_list_page(<?= $i; ?>);" id="page_link_<?= $i; ?>"><?= $page_text; ?></a>
                        <?php
                    }
                }
                //$current_page;
            ?>
        </td>
    </tr>
    </tbody>
    </table>
    <?php
    
    // javascript list of page
    ?>
    <script type="text/javascript">
        var total_list_pages = <?= $current_page; ?>;
    </script>
<?php   
    $content = ob_get_contents();
    ob_end_clean();
    return $content;
}

function ncn_admin_create_user_get_doc_info_by_hint($docs, $hint) {
  foreach( $docs as $name=>$data ) {
    if (strpos(strtolower($name), strtolower($hint)) !== FALSE) {
      return $data;
    }
  }  
  return array();
}
