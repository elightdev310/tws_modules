<?php
/**
 * URL: account/ar/%
 */
function ncn_edit_claim_ar_page($claim_id) {
    $claim = get_claim_data($claim_id);
    $member_user = user_load($claim['user_id']);
    $aci_icp = ncn_admin_get_aci_insurance_claim_policy($claim_id);
    
    ob_start();
    @include get_realpath('ncn_edit_claim', 'ui/ar_page.php');
    $content = ob_get_contents();
    ob_end_clean();

    return $content;
}

function ncn_edit_claim_ar_details_panel($claim_id) {
    $claim = get_claim_data($claim_id);
    $member_user = user_load($claim['user_id']);

    $aci_poi = ncn_admin_get_aci_property_owner_info($claim_id);
    $aci_icp = ncn_admin_get_aci_insurance_claim_policy($claim_id);
    
    ob_start();
    @include get_realpath('ncn_edit_claim', 'ui/ar_details_panel.php');
    $content = ob_get_contents();
    ob_end_clean();

    return $content;
}

/**
 * URL: account/ar/%/approve_invoice
 */
function ncn_edit_claim_ar_approve_invoice($claim_id) {
    $json_result = array(
        'status' => 'success', 
        'msg'    => ''
    );
    $status = ARS_CARRIER_SUBMISSION;
    ncn_action_change_ars_file_status($claim_id, $status);

    echo json_encode($json_result);
    exit;
}
function ncn_action_change_ars_file_status($claim_id, $status) {
    $num_updated = db_update("claims")
                ->fields(array('ars_file_status'=>$status))
                ->condition('claim_id', $claim_id)
                ->execute();
}

function ncn_edit_claim_ars_approve_invoice_js($container_section) {
    ob_start();
    @include get_realpath('ncn_edit_claim', 'ui/ar_approve_invoice.js.php');
    $content = ob_get_contents();
    ob_end_clean();

    return $content;
}
