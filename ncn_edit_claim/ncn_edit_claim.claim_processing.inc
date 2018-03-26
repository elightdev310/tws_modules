<?php
/**
 * URL: account/edit_claim/%
 *
 * Claim Processing Page
 */
function ncn_edit_claim_processing_page($claim_id) {
    $claim = get_claim_data($claim_id);
    $editable = true;
    $send_to_archive = false;
    $send_to_admin = false;
    $approve_deny = false;
    switch ($claim['claim_status']) {
        case 'incomplete':
            $editable = true;
            $send_to_archive = false;
            $send_to_admin = true;
            break;
        case 'out for review':
        case 'returned':
        case 'approved':
            $editable = true;
            $send_to_archive = false;
            $send_to_admin = false;
            $approve_deny = true;
            break;
        case 'archived': 
            $editable = false;
            break;
    }
    
    ob_start();
    @include get_realpath('ncn_edit_claim', 'ui/claim_processing_page.php');
    $content = ob_get_contents();
    ob_end_clean();

    return $content;
}

/**
 * ACI Panel
 */
function ncn_edit_claim_aci_panel($claim_id) {
    ncn_edit_claim_aci_form_handler($claim_id);

    ob_start();
    @include get_realpath('ncn_edit_claim', 'ui/claim_aci_panel.php');
    $content = ob_get_contents();
    ob_end_clean();

    return $content;
}

function ncn_edit_claim_aci_form_handler($claim_id) {
    $old_a_data = array(
        'poi' =>  unserialize(get_ncn_data_extra($claim_id, 'aci_property_owner_info')), 
        'ci' =>   unserialize(get_ncn_data_extra($claim_id, 'aci_claim_information')), 
        'icp' =>  unserialize(get_ncn_data_extra($claim_id, 'aci_insurance_claim_policy')), 
        'comments' => unserialize(get_ncn_data_extra($claim_id, 'aci_comments')), 
    );

    if (isset($_POST['tfunction']) && $_POST['tfunction']=='additional_claims_data') {
        $b_save_log = FALSE;
        if (isset($_POST['poi'])) {
            set_ncn_data_extra($claim_id, 'aci_property_owner_info', serialize($_POST['poi']));
            $b_save_log = TRUE;
        }
        if (isset($_POST['ci'])) {
            $_POST['ci']['cause_origin_of_loss'] = rtrim($_POST['ci']['cause_origin_of_loss'], " .");
            set_ncn_data_extra($claim_id, 'aci_claim_information', serialize($_POST['ci']));
            $b_save_log = TRUE;
        }
        if (isset($_POST['icp'])) {
            set_ncn_data_extra($claim_id, 'aci_insurance_claim_policy', serialize($_POST['icp']));
            $b_save_log = TRUE;
        }
        if (isset($_POST['comments'])) {
            set_ncn_data_extra($claim_id, 'aci_comments', serialize($_POST['comments']));
            $b_save_log = TRUE;
        }
        
        if ($b_save_log) {
            ncn_admin_sync_claim_data($claim_id);      
            $additional_claim_log = ncn_admin_get_claim_log_additional_data($old_a_data, $_POST);
            $log_message = t("!user_name added additional data.", array('!user_name'=>ncn_amin_get_user_role_and_name()));
            $log_message .="\r\n".$additional_claim_log;
            ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
        }
    }
}
/**
 * ACI - Property Owner Information Panel
 */
function ncn_edit_claim_aci_poi_panel($claim_id) {
    $data = array();
    if (isset($_POST['poi'])) {
        $data = $_POST['poi'];
    } else {
        $data = ncn_admin_get_aci_property_owner_info($claim_id);
    }

    ob_start();
    @include get_realpath('ncn_edit_claim', 'ui/aci/claim_aci_poi_panel.php');
    $content = ob_get_contents();
    ob_end_clean();

    return $content;
}

/**
 * ACI - Claim Information Panel
 */
function ncn_edit_claim_aci_ci_panel($claim_id) {
    $data = array();
    if (isset($_POST['ci'])) {
        $data = $_POST['ci'];
    } else {
        $data = ncn_admin_get_aci_claim_information($claim_id);
    }

    ob_start();
    @include get_realpath('ncn_edit_claim', 'ui/aci/claim_aci_ci_panel.php');
    $content = ob_get_contents();
    ob_end_clean();

    return $content;
}

/**
 * ACI - Daily Claim Log Panel
 */
function ncn_edit_claim_aci_dcl_panel($claim_id) {
    ob_start();
    @include get_realpath('ncn_edit_claim', 'ui/aci/claim_aci_dcl_panel.php');
    $content = ob_get_contents();
    ob_end_clean();

    return $content;
}

/**
 * ACI - Insurance Claim Policy Panel
 */
function ncn_edit_claim_aci_icp_panel($claim_id) {
    $data = array();
    if (isset($_POST['icp'])) {
        $data = $_POST['icp'];
    } else {
        $data = ncn_admin_get_aci_insurance_claim_policy($claim_id);
    }

    ob_start();
    @include get_realpath('ncn_edit_claim', 'ui/aci/claim_aci_icp_panel.php');
    $content = ob_get_contents();
    ob_end_clean();

    return $content;
}

/**
 * ACI - Comments Panel
 */
function ncn_edit_claim_aci_comments_panel($claim_id) {
    $data = array();
    if (isset($_POST['comments'])) {
        $data = $_POST['comments'];
    } else {
        $data = ncn_admin_get_aci_comments($claim_id);
    }

    ob_start();
    @include get_realpath('ncn_edit_claim', 'ui/aci/claim_aci_comments_panel.php');
    $content = ob_get_contents();
    ob_end_clean();

    return $content;
}

/**
 * URL: account/claim/%/add-room
 * Ajax Action: add room
 */
function ncn_edit_claim_add_room($claim_id) {
    $json_result = array(
        'status' => 'success', 
        'msg'    => ''
    );

    if (isset($_POST['room_name'])) {
        $room_name = $_POST['room_name'];
        $result = ncn_action_claim_add_room($claim_id, $room_name);
        $json_result = $result;
    } else {
        $json_result = array(
            'status' => 'error', 
            'msg'    => 'You need to input room name',
        );
    }

    echo json_encode($json_result);
}
