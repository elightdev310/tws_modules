<?php
// $Id: ncn_api_cm_scopesheet.inc
// Description: Managements for Claim Additional Data

function ncn_api_cm_scopesheet_menu(&$items) {
	$items[NCN_API_PREFIX.'/claim/%/room/%/scopesheet/get'] = array(
    'title' => 'Claim Rooms',
		'page callback' => 'ncn_api_cm_scopesheet_get',
		'page arguments' => array(2, 4), 
		'access callback' => TRUE,
    'type' => MENU_CALLBACK,
  );
	$items[NCN_API_PREFIX.'/claim/%/room/%/scopesheet/set'] = array(
    'title' => 'Claim Rooms',
		'page callback' => 'ncn_api_cm_scopesheet_set',
		'page arguments' => array(2, 4), 
		'access callback' => TRUE,
    'type' => MENU_CALLBACK,
  );
	
	$items[NCN_API_PREFIX.'/claim/%/room/%/scopesheet/rd_list'] = array(
    'title' => 'Claim Rooms',
		'page callback' => 'ncn_api_cm_ss_room_dimensions_list',
		'page arguments' => array(2, 4), 
		'access callback' => TRUE,
    'type' => MENU_CALLBACK,
  );
	$items[NCN_API_PREFIX.'/claim/%/room/%/scopesheet/rd_action'] = array(
    'title' => 'Claim Rooms',
		'page callback' => 'ncn_api_cm_ss_room_dimensions_action',
		'page arguments' => array(2, 4), 
		'access callback' => TRUE,
    'type' => MENU_CALLBACK,
  );
	
	$items[NCN_API_PREFIX.'/claim/%/room/%/scopesheet/eit_list'] = array(
    'title' => 'Claim Rooms',
		'page callback' => 'ncn_api_cm_ss_ext_inter_temperatures_list',
		'page arguments' => array(2, 4), 
		'access callback' => TRUE,
    'type' => MENU_CALLBACK,
  );
	$items[NCN_API_PREFIX.'/claim/%/room/%/scopesheet/eit_action'] = array(
    'title' => 'Claim Rooms',
		'page callback' => 'ncn_api_cm_ss_ext_inter_temperatures_action',
		'page arguments' => array(2, 4), 
		'access callback' => TRUE,
    'type' => MENU_CALLBACK,
  );
	
	$items[NCN_API_PREFIX.'/claim/%/room/%/scopesheet/smr_list'] = array(
    'title' => 'Claim Rooms',
		'page callback' => 'ncn_api_cm_ss_structural_moisture_readings_list',
		'page arguments' => array(2, 4), 
		'access callback' => TRUE,
    'type' => MENU_CALLBACK,
  );
	$items[NCN_API_PREFIX.'/claim/%/room/%/scopesheet/smr_action'] = array(
    'title' => 'Claim Rooms',
		'page callback' => 'ncn_api_cm_ss_structural_moisture_readings_action',
		'page arguments' => array(2, 4), 
		'access callback' => TRUE,
    'type' => MENU_CALLBACK,
  );
	
	$items[NCN_API_PREFIX.'/claim/%/room/%/scopesheet/ep_list'] = array(
    'title' => 'Claim Rooms',
		'page callback' => 'ncn_api_cm_ss_equipment_placement_list',
		'page arguments' => array(2, 4), 
		'access callback' => TRUE,
    'type' => MENU_CALLBACK,
  );
	$items[NCN_API_PREFIX.'/claim/%/room/%/scopesheet/ep_action'] = array(
    'title' => 'Claim Rooms',
		'page callback' => 'ncn_api_cm_ss_equipment_placement_action',
		'page arguments' => array(2, 4), 
		'access callback' => TRUE,
    'type' => MENU_CALLBACK,
  );
}

/**
 * API Hanlder ( claim/%/room/%/scopesheet/get )
 */
function ncn_api_cm_scopesheet_get($claim_id, $room_name) {
	$data = $_REQUEST;
	
	$op_get_claim_room_scopesheet = array(
		'response' => 0, 
		'claim_id' => $claim_id, 
		'room_name' => $room_name, 
	);
  $m_data = array(
    'request_type'=>'get', 
    'response_status'=>0, 
  );
  
	$user_id = ncn_api_user_get_userid_from_token($data['username'], $data['token']);
	if ($user_id && ncn_api_claim_manager_check_claim_access($claim_id, $user_id)) {
		if (ncn_api_cm_room_check_exist_room($claim_id, $room_name)) { 
			$ss = array();
			$ss['room_dimensions']['list'] = ncn_admin_get_ss_rd_list($claim_id, $room_name); //$ss['room_dimensions']['list']['{item}'] = ncn_admin_get_ss_rd_list($claim_id, $room_name);
			$ss['ext_inter_temperatures']['list'] = ncn_admin_get_ss_eit_list($claim_id, $room_name); //$ss['ext_inter_temperatures']['list']['{item}'] = ncn_admin_get_ss_eit_list($claim_id, $room_name);
			$ss['structural_moisture_readings']['standard'] = ncn_admin_get_ss_structural_moisture_readings($claim_id, $room_name);
			$ss['structural_moisture_readings']['list'] = ncn_admin_get_ss_smr_list($claim_id, $room_name); //$ss['structural_moisture_readings']['list']['{item}'] = ncn_admin_get_ss_smr_list($claim_id, $room_name);
			$ss['equipment_placement']['list'] = ncn_admin_get_ss_ep_list($claim_id, $room_name); //$ss['equipment_placement']['list']['{item}'] = ncn_admin_get_ss_ep_list($claim_id, $room_name);
			$ss['services'] = ncn_admin_get_ss_services($claim_id, $room_name);
			$ss['comments'] = ncn_admin_get_ss_comments($claim_id, $room_name);
			$op_get_claim_room_scopesheet['data'] = $ss;
			$op_get_claim_room_scopesheet['response'] = 1;
		} else {
			$op_get_claim_room_scopesheet['error']['no_room']['message'] = t("Claim(#!claim_id) doesn't have room(!room_name)", array('!claim_id'=>$claim_id, '!room_name'=>$room_name));
		}
	} else {
		$op_get_claim_room_scopesheet['error']['access_denied']['message'] = t("You're not authorized to access claim(#!claim_id)", array('!claim_id'=>$claim_id));
	}
	
	$output['getClaimRoomScopeSheet'] = $op_get_claim_room_scopesheet;
  
  $m_data['response_status'] = $op_get_claim_room_scopesheet['response'];
  ncn_api_response(array('data'=>$output, 'meta_data'=>$m_data));
}

function ncn_api_cm_array_add(&$s_data, $data) {
	foreach ($data as $key=>$sub_data) {
		if (is_array($sub_data)) {
			if (!is_array($s_data[$key])) { $s_data[$key]=array(); }
			ncn_api_cm_array_add($s_data[$key], $sub_data);
		} else {
			$s_data[$key] = $sub_data;
		}
	}
}

/**
 * API Hanlder ( claim/%/room/%/scopesheet/set )
 */
function ncn_api_cm_scopesheet_set($claim_id, $room_name) {
	$data = $_REQUEST;
	
	$op_save_claim_room_scopesheet = array(
		'response' => 0, 
		'claim_id' => $claim_id, 
		'room_name' => $room_name, 
	);
  $m_data = array(
    'request_type'=>'set', 
    'response_status'=>0, 
  );
  
	$user_id = ncn_api_user_get_userid_from_token($data['username'], $data['token']);
	if ($user_id && ncn_api_claim_manager_check_claim_access($claim_id, $user_id)) {
		if (ncn_api_cm_room_check_exist_room($claim_id, $room_name)) { 
			$b_save_log = FALSE;
			if (isset($data['ss_smr'])) {
				$data_smr = ncn_admin_get_ss_structural_moisture_readings($claim_id, $room_name);
				ncn_api_cm_array_add($data_smr, $data['ss_smr']);
				set_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_structural_moisture_readings', serialize($data_smr));
				$b_save_log = TRUE;
			}
			if (isset($data['ss_services'])) {
				$data_services = ncn_admin_get_ss_services($claim_id, $room_name);
				ncn_api_cm_array_add($data_services, $data['ss_services']);
        ncn_api_cm_handle_encode_data($data_services);
				set_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_services', serialize($data_services));
				$b_save_log = TRUE;
			}
			if (isset($data['ss_comments'])) {
				set_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_comments', serialize($data['ss_comments']));
				$b_save_log = TRUE;
			}
			
			if ($b_save_log) {
				$log_message = t("!user_name added scopesheet data for room(!room_name).", array('!user_name'=>ncn_amin_get_user_role_and_name($user_id), '!room_name'=>$room_name));
				ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
				$op_save_claim_room_scopesheet['response'] = 1;
			} else {
				$op_save_claim_room_scopesheet['warning']['no_data']['message'] = t("You should submit any scopesheet data");
			}
		} else {
			$op_save_claim_room_scopesheet['error']['no_room']['message'] = t("Claim(#!claim_id) doesn't have room(!room_name)", array('!claim_id'=>$claim_id, '!room_name'=>$room_name));
		}
	} else {
		$op_save_claim_room_scopesheet['error']['access_denied']['message'] = t("You're not authorized to access claim(#!claim_id)", array('!claim_id'=>$claim_id));
	}
	
	$output['saveClaimRoomScopeSheet'] = $op_save_claim_room_scopesheet;
  
	$m_data['response_status'] = $op_save_claim_room_scopesheet['response'];
  ncn_api_response(array('data'=>$output, 'meta_data'=>$m_data));
}

function ncn_api_cm_handle_encode_data(&$data) {
  foreach ($data as $key=>&$item) {
    if (is_array($item)) {
      ncn_api_cm_handle_encode_data($item);
    } else {
      $item = str_replace("%26", "&", $item);
    }
  }
}

/**
 * API Hanlder ( claim/%/room/%/scopesheet/rd/list )
 */
function ncn_api_cm_ss_room_dimensions_list($claim_id, $room_name) {
	$data = $_REQUEST;
	
	$op_get_claim_ss_rd_list = array(
		'response' => 0, 
		'claim_id' => $claim_id, 
		'room_name' => $room_name, 
	);
	$m_data = array(
    'request_type'=>'get', 
    'response_status'=>0, 
  );
  
	$user_id = ncn_api_user_get_userid_from_token($data['username'], $data['token']);
	if ($user_id && ncn_api_claim_manager_check_claim_access($claim_id, $user_id)) {
		if (ncn_api_cm_room_check_exist_room($claim_id, $room_name)) {
			$op_get_claim_ss_rd_list['room_dimensions'] = ncn_admin_get_ss_rd_list($claim_id, $room_name); //$op_get_claim_ss_rd_list['room_dimensions']['{item}']	= ncn_admin_get_ss_rd_list($claim_id, $room_name);
			$op_get_claim_ss_rd_list['response'] = 1;
		} else {
			$op_get_claim_ss_rd_list['error']['no_room']['message'] = t("Claim(#!claim_id) doesn't have room(!room_name)", array('!claim_id'=>$claim_id, '!room_name'=>$room_name));
		}
	} else {
		$op_get_claim_ss_rd_list['error']['access_denied']['message'] = t("You're not authorized to access claim(#!claim_id)", array('!claim_id'=>$claim_id));
	}
	$output['getClaimScopeSheetRDList'] = $op_get_claim_ss_rd_list;
	
  $m_data['response_status'] = $op_get_claim_ss_rd_list['response'];
  ncn_api_response(array('data'=>$output, 'meta_data'=>$m_data));
}

function ncn_api_cm_ss_room_dimensions_action($claim_id, $room_name) {
	$action = arg(7);
	
	$data = $_REQUEST;
	
	$op_claim_ss_rd_action = array(
		'response' => 0, 
		'claim_id' => $claim_id, 
		'room_name' => $room_name, 
		'action' => $action, 
	);
	$m_data = array(
    'request_type'=>'set', 
    'response_status'=>0, 
  );
  
	$ss_rd_data = array();
	$index = 0;
	$b_valid_action_data = TRUE;
	if ( $action == 'new' || $action == 'edit') {
		if ( isset($data['ss_rd']) && is_array($data['ss_rd']) ) {
			$ss_rd_data = $data['ss_rd'];	
		} else {
			$op_claim_ss_rd_action['error']['no_data']['message'] = t("You should submit any data as form \"ss_rd[]\".");
			$b_valid_action_data = FALSE;
		}
	}
	
	if ( $action == 'edit' || $action == 'delete' ) {
		if ( isset($data['ss_rd_index']) ) {
			$index = $data['ss_rd_index'];
		} else {
			$op_claim_ss_rd_action['error']['no_index']['message'] = t("You should submit index of data as \"ss_rd_index\".");
			$b_valid_action_data = FALSE;
		}
	}
	
	if ($b_valid_action_data) {
		$user_id = ncn_api_user_get_userid_from_token($data['username'], $data['token']);
		if ($user_id && ncn_api_claim_manager_check_claim_access($claim_id, $user_id)) {
			if (ncn_api_cm_room_check_exist_room($claim_id, $room_name)) {
				if ($action=='new' || $action=='edit' || $action=='delete') {
					if (ncn_admin_ss_rd_submit($claim_id, $room_name, $action, $index, $ss_rd_data)) {
						$op_claim_ss_rd_action['response'] = 1;
					} else {
						$op_claim_ss_rd_action['error']['fail_action']['message'] = t("Failed to do !action action", array('!action'=>$action));
					}
				} else {
					$op_claim_ss_rd_action['warning']['no_action']['message'] = t("We don't support \"!action\" action", array('!action'=>$action));
				}
			} else {
				$op_claim_ss_rd_action['error']['no_room']['message'] = t("Claim(#!claim_id) doesn't have room(!room_name)", array('!claim_id'=>$claim_id, '!room_name'=>$room_name));
			}
		} else {
			$op_claim_ss_rd_action['error']['access_denied']['message'] = t("You're not authorized to access claim(#!claim_id)", array('!claim_id'=>$claim_id));
		}
	}
	$output['claimScopeSheetRDAction'] = $op_claim_ss_rd_action;
	
  $m_data['response_status'] = $op_claim_ss_rd_action['response'];
  ncn_api_response(array('data'=>$output, 'meta_data'=>$m_data));
}

/**
 * API Hanlder ( claim/%/room/%/scopesheet/eit/list )
 */
function ncn_api_cm_ss_ext_inter_temperatures_list($claim_id, $room_name) {
	$data = $_REQUEST;
	
	$op_get_claim_ss_eit_list = array(
		'response' => 0, 
		'claim_id' => $claim_id, 
		'room_name' => $room_name, 
	);
	$m_data = array(
    'request_type'=>'get', 
    'response_status'=>0, 
  );
  
	$user_id = ncn_api_user_get_userid_from_token($data['username'], $data['token']);
	if ($user_id && ncn_api_claim_manager_check_claim_access($claim_id, $user_id)) {
		if (ncn_api_cm_room_check_exist_room($claim_id, $room_name)) {
			$op_get_claim_ss_eit_list['ext_inter_temperatures'] = ncn_admin_get_ss_eit_list($claim_id, $room_name); //$op_get_claim_ss_eit_list['ext_inter_temperatures']['{item}']	= ncn_admin_get_ss_eit_list($claim_id, $room_name);
			$op_get_claim_ss_eit_list['response'] = 1;
		} else {
			$op_get_claim_ss_eit_list['error']['no_room']['message'] = t("Claim(#!claim_id) doesn't have room(!room_name)", array('!claim_id'=>$claim_id, '!room_name'=>$room_name));
		}
	} else {
		$op_get_claim_ss_eit_list['error']['access_denied']['message'] = t("You're not authorized to access claim(#!claim_id)", array('!claim_id'=>$claim_id));
	}
	$output['getClaimScopeSheetEITList'] = $op_get_claim_ss_eit_list;
	
  $m_data['response_status'] = $op_get_claim_ss_eit_list['response'];
  ncn_api_response(array('data'=>$output, 'meta_data'=>$m_data));
}

function ncn_api_cm_ss_ext_inter_temperatures_action($claim_id, $room_name) {
	$action = arg(7);
	
	$data = $_REQUEST;
	
	$op_claim_ss_eit_action = array(
		'response' => 0, 
		'claim_id' => $claim_id, 
		'room_name' => $room_name, 
		'action' => $action, 
	);
	$m_data = array(
    'request_type'=>'set', 
    'response_status'=>0, 
  );
  
	$ss_eit_data = array();
	$index = 0;
	$b_valid_action_data = TRUE;
	if ( $action == 'new' || $action == 'edit') {
		if ( isset($data['ss_eit']) && is_array($data['ss_eit']) ) {
			$ss_eit_data = $data['ss_eit'];	
		} else {
			$op_claim_ss_eit_action['error']['no_data']['message'] = t("You should submit any data as form \"ss_eit[]\".");
			$b_valid_action_data = FALSE;
		}
	}
	
	if ( $action == 'edit' || $action == 'delete' ) {
		if ( isset($data['ss_eit_index']) ) {
			$index = $data['ss_eit_index'];
		} else {
			$op_claim_ss_eit_action['error']['no_index']['message'] = t("You should submit index of data as \"ss_eit_index\".");
			$b_valid_action_data = FALSE;
		}
	}
	
	if ($b_valid_action_data) {
		$user_id = ncn_api_user_get_userid_from_token($data['username'], $data['token']);
		if ($user_id && ncn_api_claim_manager_check_claim_access($claim_id, $user_id)) {
			if (ncn_api_cm_room_check_exist_room($claim_id, $room_name)) {
				if ($action=='new' || $action=='edit' || $action=='delete') {
					if (ncn_admin_ss_eit_submit($claim_id, $room_name, $action, $index, $ss_eit_data)) {
						$op_claim_ss_eit_action['response'] = 1;
					} else {
						$op_claim_ss_eit_action['error']['fail_action']['message'] = t("Failed to do !action action", array('!action'=>$action));
					}
				} else {
					$op_claim_ss_eit_action['warning']['no_action']['message'] = t("We don't support \"!action\" action", array('!action'=>$action));
				}
			} else {
				$op_claim_ss_eit_action['error']['no_room']['message'] = t("Claim(#!claim_id) doesn't have room(!room_name)", array('!claim_id'=>$claim_id, '!room_name'=>$room_name));
			}
		} else {
			$op_claim_ss_eit_action['error']['access_denied']['message'] = t("You're not authorized to access claim(#!claim_id)", array('!claim_id'=>$claim_id));
		}
	}
	$output['claimScopeSheetEITAction'] = $op_claim_ss_eit_action;
  
	$m_data['response_status'] = $op_claim_ss_eit_action['response'];
  ncn_api_response(array('data'=>$output, 'meta_data'=>$m_data));
}

/**
 * API Hanlder ( claim/%/room/%/scopesheet/smr_list )
 */
function ncn_api_cm_ss_structural_moisture_readings_list($claim_id, $room_name) {
	$data = $_REQUEST;
	
	$op_get_claim_ss_smr_list = array(
		'response' => 0, 
		'claim_id' => $claim_id, 
		'room_name' => $room_name, 
	);
	$m_data = array(
    'request_type'=>'get', 
    'response_status'=>0, 
  );
  
	$user_id = ncn_api_user_get_userid_from_token($data['username'], $data['token']);
	if ($user_id && ncn_api_claim_manager_check_claim_access($claim_id, $user_id)) {
		if (ncn_api_cm_room_check_exist_room($claim_id, $room_name)) {
			$op_get_claim_ss_smr_list['ext_int_temperatures'] = ncn_admin_get_ss_smr_list($claim_id, $room_name); //$op_get_claim_ss_smr_list['ext_int_temperatures']['{item}']	= ncn_admin_get_ss_smr_list($claim_id, $room_name);
			$op_get_claim_ss_smr_list['response'] = 1;
		} else {
			$op_get_claim_ss_smr_list['error']['no_room']['message'] = t("Claim(#!claim_id) doesn't have room(!room_name)", array('!claim_id'=>$claim_id, '!room_name'=>$room_name));
		}
	} else {
		$op_get_claim_ss_smr_list['error']['access_denied']['message'] = t("You're not authorized to access claim(#!claim_id)", array('!claim_id'=>$claim_id));
	}
	$output['getClaimScopeSheetSMRList'] = $op_get_claim_ss_smr_list;
	
  $m_data['response_status'] = $op_get_claim_ss_smr_list['response'];
  ncn_api_response(array('data'=>$output, 'meta_data'=>$m_data));
}

function ncn_api_cm_ss_structural_moisture_readings_action($claim_id, $room_name) {
	$action = arg(7);
	
	$data = $_REQUEST;
	
	$op_claim_ss_smr_action = array(
		'response' => 0, 
		'claim_id' => $claim_id, 
		'room_name' => $room_name, 
		'action' => $action, 
	);
	$m_data = array(
    'request_type'=>'set', 
    'response_status'=>0, 
  );
  
	$ss_smr_data = array();
	$index = 0;
	$b_valid_action_data = TRUE;
	if ( $action == 'new' || $action == 'edit') {
		if ( isset($data['smr']) && is_array($data['smr']) ) {
			$ss_smr_data = $data['smr'];	
		} else {
			$op_claim_ss_smr_action['error']['no_data']['message'] = t("You should submit any data as form \"smr[]\".");
			$b_valid_action_data = FALSE;
		}
	}
	
	if ( $action == 'edit' || $action == 'delete' ) {
		if ( isset($data['ss_smr_index']) ) {
			$index = $data['ss_smr_index'];
		} else {
			$op_claim_ss_smr_action['error']['no_index']['message'] = t("You should submit index of data as \"ss_smr_index\".");
			$b_valid_action_data = FALSE;
		}
	}
	
	if ($b_valid_action_data) {
		$user_id = ncn_api_user_get_userid_from_token($data['username'], $data['token']);
		if ($user_id && ncn_api_claim_manager_check_claim_access($claim_id, $user_id)) {
			if (ncn_api_cm_room_check_exist_room($claim_id, $room_name)) {
				if ($action=='new' || $action=='edit' || $action=='delete') {
					if (ncn_admin_ss_smr_submit($claim_id, $room_name, $action, $index, $ss_smr_data)) {
						$op_claim_ss_smr_action['response'] = 1;
					} else {
						$op_claim_ss_smr_action['error']['fail_action']['message'] = t("Failed to do !action action", array('!action'=>$action));
					}
				} else {
					$op_claim_ss_smr_action['warning']['no_action']['message'] = t("We don't support \"!action\" action", array('!action'=>$action));
				}
			} else {
				$op_claim_ss_smr_action['error']['no_room']['message'] = t("Claim(#!claim_id) doesn't have room(!room_name)", array('!claim_id'=>$claim_id, '!room_name'=>$room_name));
			}
		} else {
			$op_claim_ss_smr_action['error']['access_denied']['message'] = t("You're not authorized to access claim(#!claim_id)", array('!claim_id'=>$claim_id));
		}
	}
	$output['claimScopeSheetSMRAction'] = $op_claim_ss_smr_action;
	
  $m_data['response_status'] = $op_claim_ss_smr_action['response'];
  ncn_api_response(array('data'=>$output, 'meta_data'=>$m_data));
}

/**
 * API Hanlder ( claim/%/room/%/scopesheet/ep_list )
 */
function ncn_api_cm_ss_equipment_placement_list($claim_id, $room_name) {
	$data = $_REQUEST;
	
	$op_get_claim_ss_ep_list = array(
		'response' => 0, 
		'claim_id' => $claim_id, 
		'room_name' => $room_name, 
	);
	$m_data = array(
    'request_type'=>'get', 
    'response_status'=>0, 
  );
  
	$user_id = ncn_api_user_get_userid_from_token($data['username'], $data['token']);
	if ($user_id && ncn_api_claim_manager_check_claim_access($claim_id, $user_id)) {
		if (ncn_api_cm_room_check_exist_room($claim_id, $room_name)) {
			$op_get_claim_ss_ep_list['ext_int_temperatures'] = ncn_admin_get_ss_ep_list($claim_id, $room_name); //$op_get_claim_ss_ep_list['ext_int_temperatures']['{item}']	= ncn_admin_get_ss_ep_list($claim_id, $room_name);
			$op_get_claim_ss_ep_list['response'] = 1;
		} else {
			$op_get_claim_ss_ep_list['error']['no_room']['message'] = t("Claim(#!claim_id) doesn't have room(!room_name)", array('!claim_id'=>$claim_id, '!room_name'=>$room_name));
		}
	} else {
		$op_get_claim_ss_ep_list['error']['access_denied']['message'] = t("You're not authorized to access claim(#!claim_id)", array('!claim_id'=>$claim_id));
	}
	$output['getClaimScopeSheetEPList'] = $op_get_claim_ss_ep_list;
	
  $m_data['response_status'] = $op_get_claim_ss_ep_list['response'];
  ncn_api_response(array('data'=>$output, 'meta_data'=>$m_data));
}

function ncn_api_cm_ss_equipment_placement_action($claim_id, $room_name) {
	$action = arg(7);
	
	$data = $_REQUEST;
	
	$op_claim_ss_ep_action = array(
		'response' => 0, 
		'claim_id' => $claim_id, 
		'room_name' => $room_name, 
		'action' => $action, 
	);
	$m_data = array(
    'request_type'=>'set', 
    'response_status'=>0, 
  );
  
	$ss_ep_data = array();
	$index = 0;
	$b_valid_action_data = TRUE;
	if ( $action == 'new' || $action == 'edit') {
		if ( isset($data['ss_ep']) && is_array($data['ss_ep']) ) {
			$ss_ep_data = $data['ss_ep'];	
		} else {
			$op_claim_ss_ep_action['error']['no_data']['message'] = t("You should submit any data as form \"ss_ep[]\".");
			$b_valid_action_data = FALSE;
		}
	}
	
	if ( $action == 'edit' || $action == 'delete' ) {
		if ( isset($data['ss_ep_index']) ) {
			$index = $data['ss_ep_index'];
		} else {
			$op_claim_ss_ep_action['error']['no_index']['message'] = t("You should submit index of data as \"ss_ep_index\".");
			$b_valid_action_data = FALSE;
		}
	}
	
	if ($b_valid_action_data) {
		$user_id = ncn_api_user_get_userid_from_token($data['username'], $data['token']);
		if ($user_id && ncn_api_claim_manager_check_claim_access($claim_id, $user_id)) {
			if (ncn_api_cm_room_check_exist_room($claim_id, $room_name)) {
				if ($action=='new' || $action=='edit' || $action=='delete') {
					if (ncn_admin_ss_ep_submit($claim_id, $room_name, $action, $index, $ss_ep_data)) {
						$op_claim_ss_ep_action['response'] = 1;
					} else {
						$op_claim_ss_ep_action['error']['fail_action']['message'] = t("Failed to do !action action", array('!action'=>$action));
					}
				} else {
					$op_claim_ss_ep_action['warning']['no_action']['message'] = t("We don't support \"!action\" action", array('!action'=>$action));
				}
			} else {
				$op_claim_ss_ep_action['error']['no_room']['message'] = t("Claim(#!claim_id) doesn't have room(!room_name)", array('!claim_id'=>$claim_id, '!room_name'=>$room_name));
			}
		} else {
			$op_claim_ss_ep_action['error']['access_denied']['message'] = t("You're not authorized to access claim(#!claim_id)", array('!claim_id'=>$claim_id));
		}
	}
	$output['claimScopeSheetEPAction'] = $op_claim_ss_ep_action;
	
  $m_data['response_status'] = $op_claim_ss_ep_action['response'];
  ncn_api_response(array('data'=>$output, 'meta_data'=>$m_data));
}