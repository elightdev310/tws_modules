<?php
//------------------------------------------------------------------------------
// complete user account edit page
function ncn_claims_manager_get_account_header_menu()
{
    global $user;
    $menu_data = menu_navigation_links('menu-account-header');
    if (isset($_SESSION['admin_sandbox']) && $_SESSION['admin_sandbox'] == $user->uid) {
        $menu_data['menu-310']['href'] = 'user/back_to_admin';
        $menu_data['menu-310']['title'] = 'Back to Admin';
    }
    return $menu_data;
}

function ncn_claim_manager_claim_status_style($claim_status)
{
    $str_status = ucwords($claim_status);
    return "<span class=\"claim_status $claim_status\">$str_status</span>";
}

//------------------------------------------------------------------------------
function render_claim_time($time)
{
    $str = sprintf("%02d", $time['hour']) . ':' . sprintf("%02d", $time['minute']) . " " . $time['meridiem'];

    return $str;
}

//------------------------------------------------------------------------------
function render_claim_date($date)
{
    return date("F j Y", strtotime($date)); //return $date;

    if (!is_numeric($date)) {
        return '';
    }
    
    return date("m/d/Y", strtotime($date));
}
function strClaimAddress($claim_id) {
    $address = ncn_cd($claim_id, 'insured_address')." ";
    $address.= ncn_cd($claim_id, 'insured_city')." ";
    $address.= ncn_cd($claim_id, 'insured_state')." ";
    $address.= ncn_cd($claim_id, 'insured_zip')." ";
    $address.= ncn_cd($claim_id, 'insured_country');

    return $address;
}

function strTime($timestamp, $format="n/d/Y g:i A") {

    $str = date($format, $timestamp);

    return $str;
  }
