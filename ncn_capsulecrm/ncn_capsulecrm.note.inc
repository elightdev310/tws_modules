<?php

/**
* Add note to party in CapsuleCRM
* 
* @param mixed $claim_id
* @param mixed $note
*/
function ncn_capsulecrm_note_add_note($claim_id, $note) {
global $user;

  $claim = get_claim_data($claim_id);
  if ($claim && $claim['user_id'] && $account=user_load($claim['user_id'])) {
    $member_id = isset($account->profile_memberid)?$account->profile_memberid:0;
    if ($member_id && $cc_info=ncn_capsulecrm_get_map_info($member_id)) {
      if ($cc_info['organization_id']) {
        $entry = array();
        $entry['party']   = (object)(array('id'=>(int)$cc_info['organization_id']));
        $entry['type']    = 'note';
        $entry['content'] = "[claim #$claim_id] - ".$note;
        // $capsulecrm_account = ncn_capsulecrm_account_get_account($user);
        // if ($capsulecrm_account) {
        //   $data['creator'] = $capsulecrm_account;
        // }
        $entry = (object)$entry;
        
        $response = capsule_add_entry( (object)(array('entry'=>$entry)) );
        if ($response->code != 201) { 
          drupal_set_message(t('Capsule CRM Error: Failed to add an NOTE into Capsule CRM'), 'error');
          return 0; 
        }
        $entry_id = $response->entry->id;
        if (is_numeric($entry_id)) {
          watchdog('ncn-capsulecrm', "Add Note: https://pherona.capsulecrm.com/party/".$cc_info['organization_id']."/history/$entry_id");
          return $entry_id;
        }
      }
    }
  }
  return 0;
}

/**
* Update note to party in CapsuleCRM
* 
* @param mixed $history_id
* @param mixed $note
*/
function ncn_capsulecrm_note_update_note($claim_id, $history_id, $note) {
global $user;
  $entry = array();
  $entry['content'] = "[claim #$claim_id] - ".$note;
  // $capsulecrm_account = ncn_capsulecrm_account_get_account($user);
  // if ($capsulecrm_account) {
  //   $data['creator'] = $capsulecrm_account;
  // }
  $entry = (object)$entry;
  $response = capsule_update_entry( (object)(array('entry'=>$entry)), $history_id );
  if ($response->code != 200) { 
    drupal_set_message(t('Capsule CRM Error: Failed to update NOTE into Capsule CRM'), 'error');
    return FALSE; 
  }
  
  return TRUE;
}
