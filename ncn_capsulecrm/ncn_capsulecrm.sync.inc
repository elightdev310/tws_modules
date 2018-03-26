<?php
function ncn_capsulecrm_synchronize() {
    $max_cc_id = variable_get('MAX_CC_ID', 0);
    $result = db_query("SELECT * FROM {member_id_pool} WHERE id>:d ORDER BY id ASC",array(":d"=>$max_cc_id));
    if ($result->rowCount()>0) {
        $row=$result->fetchAssoc();
        ncn_capsulecrm_set_map_info( $row['member_id'] );
        $max_cc_id = $row['id'];
    } else {
        //$max_cc_id +=1;
    }
    variable_set('MAX_CC_ID', $max_cc_id);
    $str_ret = $max_cc_id;
    if (isset($row['member_id'])) { $str_ret.= "-" . $row['member_id']; }
    
    return $str_ret;
}

function ncn_capsulecrm_set_map_data($member_id, $org_id, $person_id) {
    $cc_info = ncn_capsulecrm_get_map_info($member_id);
    if ($cc_info) {
        db_query('UPDATE {ncn_capsulecrm_map} SET organization_id=:a, person_id=:b WHERE id=:c',
            array(':a'=>$org_id,':b'=>$person_id,':c'=>$cc_info['id']));
    } else {
        db_query('INSERT INTO {ncn_capsulecrm_map}(member_id, organization_id, person_id) VALUES(:a, :b, :c)',
            array(':a'=>$member_id,':b'=>$org_id,':c'=>$person_id));
    }
    return TRUE;
}

function ncn_capsulecrm_set_map_info($member_id) {
    return FALSE;

    $org_id = 0;
    $person_id = 0;
    if (ncn_capsulecrm_get_partyid_from_memberid($member_id, $org_id, $person_id)) {
        return ncn_capsulecrm_save_map_info($member_id, $org_id, $person_id);
    }
    return FALSE;
}

function ncn_capsulecrm_save_map_info($member_id, $org_id, $person_id) {
    $cc_info = ncn_capsulecrm_get_map_info($member_id);
    if ($cc_info) {
        if ($org_id!=$cc_info['organization_id'] || $person_id!=$cc_info['person_id']) {
            db_query("UPDATE {ncn_capsulecrm_map} SET organization_id=:a, person_id=:b WHERE id=:c",
                array(":a"=>$org_id,":b"=>$person_id,":c"=>$cc_info['id']));
        }
    } else {
        db_query("INSERT INTO {ncn_capsulecrm_map}(member_id, organization_id, person_id) VALUES(':a', ':b', ':c')",
            array(":a"=>$member_id,":b"=>$org_id,":c"=>$person_id));
    }
  
    return TRUE;
}

function ncn_capsulecrm_get_map_info($member_id) {

    $result = db_query('SELECT * FROM {ncn_capsulecrm_map} WHERE member_id=:s',array(':s'=>$member_id))->fetchAssoc();

    if ($result) {
        return $result;
    }
    return FALSE;
}

function ncn_capsulecrm_get_partyid_from_memberid($member_id, &$org_id, &$person_id) {
    $page = 1;
    $member = get_member_from_id($member_id);
    while ($parties = ncn_capsulecrm_get_parties($page, 50, array('since'=>'2017-09-20T00:00:00Z'))) {
        if (empty($parties)) { break; }
        foreach ($parties as $party) {
            if ($party->type == 'organisation') {
                if (ncn_capsulecrm_check_match($member, $party)) {
                    $org_id = $party_id;
                    if ($people_party_id = ncn_capsulecrm_get_people_party_id($party_id)) {
                        $person_id = $people_party_id;
                    }
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

function ncn_capsulecrm_check_match($member, $party) {
    $cc_member_id = ncn_capsulecrm_get_member_id($party);
    
    if (!$cc_member_id || $cc_member_id != $member['member_id']) {
        return FALSE;
    }
    return TRUE;
}

function ncn_capsulecrm_get_member_id($party) {
    if (is_numeric($party)) {
        $party = ncn_capsulecrm_get_party($party);
    }
    if ($party) {
        return ncn_capsulecrm_get_customfield_value($party, CRM_FIELD_MEMBER_ID);
    }
    return false;
}

// function ncn_capsulecrm_get_custom_fields($party_id) {
//     $result = capsule_list_party_customfields($party_id);
//     if ($result && $result->code == 200) {
//         $data = json_decode($result->data);
//         if (isset($data->customFields->customField)) {
//             return $data->customFields->customField;
//         }
//     }
//     return FALSE;
// }

// function ncn_capsulecrm_get_custom_field_meta_data($field_data, $label) {
//     if (is_array($field_data)) {
//         foreach($field_data as $data) {
//             if ($data->label == $label) {
//                 return $data;
//             }
//         }
//     }
//     return FALSE;
// }

// function ncn_capsulecrm_get_custom_field_data($field_data, $label, $type) {
//     if (is_array($field_data)) {
//         foreach($field_data as $data) {
//             if ($data->label == $label) {
//                 return $data->$type;
//             }
//         }
//     }
//     return FALSE;
// }

function ncn_capsulecrm_get_peoples($party_id) {
    $result = capsule_list_party_people($party_id);
    if ($result && $result->code == 200) {
        $peoples = $result->parties;
        return $peoples;
    }
    return FALSE;
}

function ncn_capsulecrm_get_people_party_id($party_id) {
    $person = ncn_capsulecrm_get_peoples($party_id);
    
    if ($person) {
        if (is_array($person)) {
            return $person[0]->id;
        } else {
            return $person->id;
        }
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
function ncn_capsulecrm_daily_update_active_user_data() {
    $result = db_query("SELECT * FROM {users} WHERE uid!=0 ORDER BY uid DESC");
    foreach($result as $u_row)
    {
        $u_row = (array)$u_row;
        $user = user_load($u_row['uid']);
        if ($user) {
            if (isset($user->profile_memberid)&&is_member($user->profile_memberid)) {
                ncn_capsulecrm_daily_update_user_member_data($user);
            }
        } else {
        }
    }
}

function ncn_capsulecrm_daily_update_unactived_member_data() {
    $result = db_query("SELECT * FROM {member_id_pool} WHERE used=0");
//  while ($member = db_fetch_array($result))
    foreach($result as $member)
    {
        $member = (array)$member;
        $member_id = $member['member_id'];
        ncn_capsulecrm_daily_update_una_member_data($member_id);
    }
}

/**
 * Update           # of Claims , Total $$ YTD: , Total $$ All Time: 
 */
function ncn_capsulecrm_daily_update_user_member_data($user) {
    $crm_info = ncn_capsulecrm_check_map_info(isset($user->profile_memberid)?$user->profile_memberid:0);
    if (!$crm_info) {
        return FALSE;
    }

    $party_id = $crm_info['organization_id'];

    $organization = array();

    // # of Claims
    $total_claims = user_detail_get_total_claims($user);
    $organization['fields'][] = ncn_capsulecrm_get_customfield_object(CRM_FIELD_COUNT_CLAIMS, (string)$total_claims);
    
    // Total $$ YTD:
    $ytd = render_payment_cost(ncn_report_get_ytd($user->uid));
    $ytd = strip_tags($ytd);
    $organization['fields'][] = ncn_capsulecrm_get_customfield_object(CRM_FIELD_TOTAL_YTD, (string)$ytd);

    // Total $$ All Time:
    $total_ytd = render_payment_cost(ncn_report_get_total_ytd($user->uid));
    $total_ytd = strip_tags($total_ytd);
    $organization['fields'][] = ncn_capsulecrm_get_customfield_object(CRM_FIELD_TOTAL_ALL_TIME, (string)$total_ytd);

    $organization = (object)$organization;
    $response = capsule_update_organization( array('party'=>$organization), $party_id );

    if ($response->code != 200) {
        watchdog('ncn-capsulecrm', 
                 t('Capsule CRM Error: Failed to add update organization into Capsule CRM. User: !user, Party ID: !party_id', 
                    array('!user' => $user->uid, '!party_id' => $party_id)));
        return FALSE;
    }
    return TRUE;
}

/**
 * Update $YTD and $All Time for Unactivated Members
 */
function ncn_capsulecrm_daily_update_una_member_data($member_id) {
    $crm_info = ncn_capsulecrm_check_map_info($member_id);
    if (!$crm_info) {
        return FALSE;
    }
    $party_id = $crm_info['organization_id'];
    $organization = array();

    // Total $$ YTD:
    $ytd = render_payment_cost(ncn_report_get_ytd(0, $member_id));
    $ytd = strip_tags($ytd);
    $organization['fields'][] = ncn_capsulecrm_get_customfield_object(CRM_FIELD_TOTAL_YTD, (string)$ytd);
    
    // Total $$ All Time:
    $total_ytd = render_payment_cost(ncn_report_get_total_ytd(0, $member_id));
    $total_ytd = strip_tags($total_ytd);
    $organization['fields'][] = ncn_capsulecrm_get_customfield_object(CRM_FIELD_TOTAL_ALL_TIME, (string)$total_ytd);

    $organization = (object)$organization;
    $response = capsule_update_organization( array('party'=>$organization), $party_id );

    if ($response->code != 200) {
        watchdog('ncn-capsulecrm', 
                 t('Capsule CRM Error: Failed to add update organization into Capsule CRM. User: !user, Party ID: !party_id', 
                    array('!user' => $user->uid, '!member_id' => $party_id)));
        return FALSE;
    }
    return TRUE;
}

function ncn_capsulecrm_check_map_info($member_id) {
    $crm_info = ncn_capsulecrm_get_map_info($member_id);
    if (!$crm_info) {
        if (ncn_capsulecrm_set_map_info($member_id)) {
            $crm_info = ncn_capsulecrm_get_map_info($member_id);
        }
    }
    return $crm_info;
}

function ncn_capsulecrm_add_task($member_id, $note) {
}

/**
 * Set VOS Account field ( user detail page )
 */
function ncn_capsulecrm_set_user_detail_link($user_id, $member_id=0) {
global $base_url;

    if ($member_id == 0) {
        $_user = user_load($user_id);
        if ($_user && isset($_user->profile_memberid) && $_user->profile_memberid) {
            $member_id = $_user->profile_memberid;
        } else {
            return FALSE;
        }
    }
    
    $cc_info = ncn_capsulecrm_get_map_info($member_id);
    if (!$cc_info || !$cc_info['organization_id']) {
        if (ncn_capsulecrm_set_map_info($member_id)) {
            $cc_info = ncn_capsulecrm_get_map_info($member_id);
        } else {
            return FALSE;
        }
    }
    
    $party_id = $cc_info['organization_id'];
    if (!$party_id) { return FALSE; }
    
    $organization = array();
    // Custom Field (VOS account)
    $user_url = $base_url."/admin/config/ncn_create_user/all_user_list/detail/".$user_id;
    $organization['fields'][] = ncn_capsulecrm_get_customfield_object(CRM_FIELD_VOS_ACCOUNT, (string)$user_url);

    $organization = (object)$organization;
    $response = capsule_update_organization( array('party'=>$organization), $party_id );

    if ($response->code != 200)  {
        return FALSE;
    }
    return TRUE;
}
