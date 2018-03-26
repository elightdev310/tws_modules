<?php
// $Id: ncn_api_cm_additional_data.inc
// Description: Managements for Claim Additional Data

function ncn_api_cm_additional_data_menu(&$items) {
	$items[NCN_API_PREFIX.'/claim/%/additional_data/get'] = array(
    'title' => 'Claim Rooms',
		'page callback' => 'ncn_api_cm_additional_data_get',
		'page arguments' => array(2), 
		'access callback' => TRUE,
    'type' => MENU_CALLBACK,
  );
	$items[NCN_API_PREFIX.'/claim/%/additional_data/set'] = array(
    'title' => 'Claim Rooms',
		'page callback' => 'ncn_api_cm_additional_data_set',
		'page arguments' => array(2), 
		'access callback' => TRUE,
    'type' => MENU_CALLBACK,
  );
	
	$items[NCN_API_PREFIX.'/claim/%/additional_data/dcl'] = array(
    'title' => 'Claim Rooms',
		'page callback' => 'ncn_api_cm_additional_data_dcl_list',
		'page arguments' => array(2), 
		'access callback' => TRUE,
    'type' => MENU_CALLBACK,
  );
	$items[NCN_API_PREFIX.'/claim/%/additional_data/dcl/list'] = array(
    'title' => 'Claim Rooms',
		'page callback' => 'ncn_api_cm_additional_data_dcl_list',
		'page arguments' => array(2), 
		'access callback' => TRUE,
    'type' => MENU_CALLBACK,
  );
	$items[NCN_API_PREFIX.'/claim/%/additional_data/dcl/action/%'] = array(
    'title' => 'Claim Rooms',
		'page callback' => 'ncn_api_cm_additional_data_dcl_action',
		'page arguments' => array(2, 6), 
		'access callback' => TRUE,
    'type' => MENU_CALLBACK,
  );
	
}

/**
 * API Hanlder ( claim/%/additional_data/get )
 */
function ncn_api_cm_additional_data_get($claim_id) {
	$data = $_REQUEST;
	
	$op_get_claim_additional_data = array(
		'response' => 0, 
		'claim_id' => $claim_id, 
	);
  $m_data = array(
    'request_type'=>'get', 
    'response_status'=>0, 
  );
  
	$user_id = ncn_api_user_get_userid_from_token($data['username'], $data['token']);
	if ($user_id && ncn_api_claim_manager_check_claim_access($claim_id, $user_id)) {
		$ad = array();
		$ad['poi'] 	= ncn_admin_get_aci_property_owner_info($claim_id);
    $ad['poi']['insured_country'] = ncn_cd($claim_id, 'insured_country'); if (!$ad['poi']['insured_country']) { $ad['poi']['insured_country'] = 'US'; }
    
		$ad['ci'] 	= ncn_admin_get_aci_claim_information($claim_id);
		$ad['dcl']  = ncn_admin_get_aci_daily_claim_log($claim_id); //$ad['dcl']['{log}']	= ncn_admin_get_aci_daily_claim_log($claim_id);
    
		$ad['icp'] 	= ncn_admin_get_aci_insurance_claim_policy($claim_id);
		$ad['comments'] = ncn_admin_get_aci_comments($claim_id);
		$op_get_claim_additional_data['data'] = $ad;
		$op_get_claim_additional_data['response'] = 1;
	} else {
		$op_get_claim_additional_data['error']['access_denied']['message'] = t("You're not authorized to access claim(#!claim_id)", array('!claim_id'=>$claim_id));
	}
	
	$output['getClaimAdditionalData'] = $op_get_claim_additional_data;
	
  $m_data['response_status'] = $op_get_claim_additional_data['response'];
  ncn_api_response(array('data'=>$output, 'meta_data'=>$m_data));
}

/**
 * API Hanlder ( claim/%/additional_data/set )
 */
function ncn_api_cm_additional_data_set($claim_id) {
	$data = $_REQUEST;
	
	$op_save_claim_additional_data = array(
		'response' => 0, 
		'claim_id' => $claim_id, 
	);
	$m_data = array(
    'request_type'=>'set', 
    'response_status'=>0, 
  );
  
	$user_id = ncn_api_user_get_userid_from_token($data['username'], $data['token']);
	if ($user_id && ncn_api_claim_manager_check_claim_access($claim_id, $user_id)) {
		$b_save_log = FALSE;
		if (isset($data['poi'])) {
			$data_poi = ncn_admin_get_aci_property_owner_info($claim_id);
			ncn_api_cm_array_add($data_poi, $data['poi']);
			set_ncn_data_extra($claim_id, 'aci_property_owner_info', serialize($data_poi));
			$b_save_log = TRUE;
		}
		if (isset($data['ci'])) {
			$data['ci']['cause_origin_of_loss'] = rtrim($data['ci']['cause_origin_of_loss'], " .");
			
			$data_ci = ncn_admin_get_aci_claim_information($claim_id);
			ncn_api_cm_array_add($data_ci, $data['ci']);
			set_ncn_data_extra($claim_id, 'aci_claim_information', serialize($data_ci));
			$b_save_log = TRUE;
		}
		if (isset($data['icp'])) {
			$data_icp = ncn_admin_get_aci_insurance_claim_policy($claim_id);
			ncn_api_cm_array_add($data_icp, $data['icp']);
			set_ncn_data_extra($claim_id, 'aci_insurance_claim_policy', serialize($data_icp));
			$b_save_log = TRUE;
		}
		if (isset($data['comments'])) {
			set_ncn_data_extra($claim_id, 'aci_comments', serialize($data['comments']));
			$b_save_log = TRUE;
		}
		
		if ($b_save_log) {
      ncn_admin_sync_claim_data($claim_id);
			$log_message = t("!user_name added additional data.", array('!user_name'=>ncn_amin_get_user_role_and_name($user_id)));
			ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
			$op_save_claim_additional_data['response'] = 1;
		} else {
			$op_save_claim_additional_data['warning']['no_data']['message'] = t("You should submit any additional data of claim(#!claim_id)", array('!claim_id'=>$claim_id));
		}
	} else {
		$op_save_claim_additional_data['error']['access_denied']['message'] = t("You're not authorized to access claim(#!claim_id)", array('!claim_id'=>$claim_id));
	}
	$output['saveClaimAdditionalData'] = $op_save_claim_additional_data;
	
  $m_data['response_status'] = $op_save_claim_additional_data['response'];
  ncn_api_response(array('data'=>$output, 'meta_data'=>$m_data));
	
}

/**
 * API Hanlder ( claim/%/additional_data/dcl/list )
 */
function ncn_api_cm_additional_data_dcl_list($claim_id) {
	$data = $_REQUEST;
	
	$op_get_claim_additional_data_dcl_list = array(
		'response' => 0, 
		'claim_id' => $claim_id, 
	);
	$m_data = array(
    'request_type'=>'get', 
    'response_status'=>0, 
  );
  
	$user_id = ncn_api_user_get_userid_from_token($data['username'], $data['token']);
	if ($user_id && ncn_api_claim_manager_check_claim_access($claim_id, $user_id)) {
		$op_get_claim_additional_data_dcl_list['dcl']  = ncn_admin_get_aci_daily_claim_log($claim_id); //$op_get_claim_additional_data_dcl_list['dcl']['{log}']	= ncn_admin_get_aci_daily_claim_log($claim_id);
		$op_get_claim_additional_data_dcl_list['response'] = 1;
	} else {
		$op_get_claim_additional_data_dcl_list['error']['access_denied']['message'] = t("You're not authorized to access claim(#!claim_id)", array('!claim_id'=>$claim_id));
	}
	$output['getClaimAdditionalDataDclList'] = $op_get_claim_additional_data_dcl_list;
	
  $m_data['response_status'] = $op_get_claim_additional_data_dcl_list['response'];
  ncn_api_response(array('data'=>$output, 'meta_data'=>$m_data));
}

function ncn_api_cm_additional_data_dcl_action($claim_id, $action) {
	$data = $_REQUEST;
	
	$op_claim_additional_data_dcl_action = array(
		'response' => 0, 
		'claim_id' => $claim_id, 
		'action' => $action, 
	);
	$m_data = array(
    'request_type'=>'set', 
    'response_status'=>0, 
  );
  
	$dcl_data = array();
	$index = 0;
	$b_valid_action_data = TRUE;
	if ( $action == 'new' || $action == 'edit') {
		if (isset($data['dc_log']) && is_array($data['dc_log'])) {
			$dcl_data = $data['dc_log'];
		} else {
			$op_claim_additional_data_dcl_action['error']['no_data']['message'] = t("You should submit any data as form \"dc_log[]\".");
			$b_valid_action_data = FALSE;
		}
	} 
	
	if ( $action == 'edit' || $action == 'delete') {
		if (isset($data['dcl_log_index'])) {
			$index = $data['dcl_log_index'];
		} else {
			$op_claim_additional_data_dcl_action['error']['no_index']['message'] = t("You should submit index of data as \"dcl_log_index\".");
			$b_valid_action_data = FALSE;
		}
	}
	
	if ($b_valid_action_data) {
		$user_id = ncn_api_user_get_userid_from_token($data['username'], $data['token']);
		if ($user_id && ncn_api_claim_manager_check_claim_access($claim_id, $user_id)) {
			if ($action=='new' || $action=='edit' || $action=='delete') {
				if (ncn_admin_aci_dcl_submit_daily_log($claim_id, $action, $index, $dcl_data)) {
					$op_claim_additional_data_dcl_action['response'] = 1;
				} else {
					$op_claim_additional_data_dcl_action['error']['fail_action']['message'] = t("Failed to do !action action", array('!action'=>$action));
				}
			} else {
				$op_claim_additional_data_dcl_action['warning']['no_action']['message'] = t("We don't support \"!action\" action", array('!action'=>$action));
			}
		} else {
			$op_claim_additional_data_dcl_action['error']['access_denied']['message'] = t("You're not authorized to access claim(#!claim_id)", array('!claim_id'=>$claim_id));
		}
	}
	$output['claimAdditionalDataDclAction'] = $op_claim_additional_data_dcl_action;
	
  $m_data['response_status'] = $op_claim_additional_data_dcl_action['response'];
  ncn_api_response(array('data'=>$output, 'meta_data'=>$m_data));
}