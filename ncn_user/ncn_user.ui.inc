<?php
// NCN Member Specific Page Permission
function check_role($roles = array(USER_ROLE_MEMBER, USER_ROLE_SUB_USER, USER_ROLE_LEADUSER))
{
    GLOBAL $user;
    global $base_url;

    if ($user->uid == 0) {
        drupal_goto();
    }
    if ($user->uid ==1) {
        return TRUE;
    }
    foreach($roles as $role) {
        if (isset($user->roles[$role])) { return TRUE;}
    }

    if(is_distributor($user))
    {
        Header("Location:".$base_url."/account/distributor.html/create");
        exit;
    }

    if(is_associate($user))
    {
        Header("Location:".$base_url."/account/associate.html/create");
        exit;
    }

    if(is_member_user($user) || is_subuser($user))
    {
        Header("Location:".$base_url."/account");
        exit;
    }

    if(is_claim_examiner($user))
    {
        Header("Location:".$base_url."/ce_portal/home");
        exit;
    }

    Header("Location:".$base_url."/admin/config/ncn_admin");
    exit;
}

/**
 * Member Logo & Legal Name Block
 */
function ncn_member_logo_legalname_block($uid) {
    $user = user_load($uid);
    if ($user && is_member_user($user)) {
        $legal_name = $user->profile_legalname;
        $member_logo_path = ncn_admin_get_member_logo($user->profile_memberid);
        ob_start();
        @include get_realpath('ncn_user', 'ui/member_logo_legalname_block.php');
        $content = ob_get_contents();
        ob_end_clean();
        return $content;
    }
    return '';
}
