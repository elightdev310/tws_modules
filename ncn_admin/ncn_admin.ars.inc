<?php
/**
 * ARS
 */

/**
 * Admin - Claim View - ARS Section
 */
function ncn_admin_claim_view_ars_section($claim_id) {
    $ars_file_status_info = ncn_ars_file_status();
    $claim = get_claim_data($claim_id);

    $old_a_data = array(
        'pr' =>  unserialize(get_ncn_data_extra($claim_id, 'ars_peer_review')), 
        'hoa' => unserialize(get_ncn_data_extra($claim_id, 'ars_hoa')), 
    );
    $is_ars = ncn_is_claim_ars($claim);

    // Checkbox - ARS
    if (isset($_POST['ars_function']) && $_POST['ars_function']=='ars') {
        $ars_is_ars = 0;
        if (isset($_POST['ars_is_ars']) && $_POST['ars_is_ars']==1) {
            $ars_is_ars = 1;
        }

        if ($is_ars != $ars_is_ars) {
            ncn_set_claim_ars($claim_id, $ars_is_ars);
            if ($ars_is_ars) {
                $log_message = t("!user_name set claim to ARS.", array('!user_name'=>ncn_amin_get_user_role_and_name()));
            } else {
                $log_message = t("!user_name unset claim from ARS.", array('!user_name'=>ncn_amin_get_user_role_and_name()));
            }
            ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
        }
        $is_ars = $ars_is_ars;

        // 
        if ($_POST['ars_file_status'] != $claim['ars_file_status']) {
            ncn_action_change_ars_file_status($claim_id, $_POST['ars_file_status']);
            $log_message = t("!user_name change ARS File Status to :ars_file_status.", 
                                array('!user_name'=>ncn_amin_get_user_role_and_name(), 
                                      ':ars_file_status' => $ars_file_status_info[$_POST['ars_file_status']]
                                ));
            ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
        }
    }

    // Fields
    if (isset($_POST['ars_function']) && $_POST['ars_function']=='ars') {

        if (isset($_POST['pr'])) {
            set_ncn_data_extra($claim_id, 'ars_peer_review', serialize($_POST['pr']));
        }
        if (isset($_POST['hoa'])) {
            set_ncn_data_extra($claim_id, 'ars_hoa', serialize($_POST['hoa']));
        }

        $additional_claim_log = ncn_admin_get_claim_log_ar_data($old_a_data, $_POST);
        if (!empty($additional_claim_log)) {
            $log_message = t("!user_name added ARS additional data.", array('!user_name'=>ncn_amin_get_user_role_and_name()));
            $log_message .="\r\n".$additional_claim_log;
            ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
        }
    }

    if (isset($_POST['ars_function'])) {
        $query = array();
        if (isset($_POST['current_scroll_position']) && $_POST['current_scroll_position'] != "") {
            $query['current_scroll_position'] = $_POST['current_scroll_position'];
        }
        drupal_goto(current_path(), array('query'=>$query));
        exit;
    }

    $claim = get_claim_data($claim_id);

    //ui
    ob_start();
    @include get_realpath('ncn_admin', 'ui/claim/claim_view_ars_section.php');
    $content = ob_get_contents();
    ob_end_clean();
    return $content;
}

function ncn_admin_get_claim_log_ar_data($old, $updated) {
  $claim_log = '';
  $section_data = array(
    'pr' => t('Peer Review'), 
    'hoa' => t('HOA Information'), 
  );
  
  foreach ($section_data as $key=>$title) {
    $extract = ncn_admin_get_claim_log_diff($updated[$key], $old[$key]);
    if (!empty($extract)) {
      $claim_log.="$title section has been updated.\r\n";
      $claim_log.= ncn_admin_get_claim_log_recurrent_var($extract);
      $claim_log.="\r\n";
    }
  }
  
  return $claim_log;
}

function ncn_is_claim_ars($claim) {
    if (is_numeric($claim)) {
        $claim = get_claim_data($claim);
    }
    if (isset($claim['ars']) && !empty($claim['ars'])) {
        return true;
    }
    return false;
}
function ncn_set_claim_ars($claim_id, $yes=1) {
    $num_updated = db_update('claims')
        ->fields(array('ars' => $yes))
        ->condition('claim_id', $claim_id)
        ->execute();
}

function ncn_ars_file_status() {
    return array(
        ARS_FILE_SETUP          => 'File Setup', 
        ARS_OUT_FOR_APPROVAL    => 'Out for Approval', 
        ARS_CARRIER_SUBMISSION  => 'Carrier Submission', 
        ARS_AWAITING_CC         => 'Awaiting Carrier Confirmation', 
        ARS_NEGOTIATIONS        => 'Negotiations', 
        ARS_COLLECTIONS         => 'Collections', 
        ARS_CLOSED              => 'Closed', 
    );
}
