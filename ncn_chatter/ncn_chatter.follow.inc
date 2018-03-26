<?php

/**
 * Insert record to chatter_follow_feed table
 */
function ncn_chatter_insert_follow_feed_record($data=array()) {
    $result = db_insert('chatter_follow_feed')
            ->fields(array(
                'uid'   => $data['uid'], 
                'fid'   => $data['fid']
            ))
            ->execute();
    return $result;
}

/**
 * Insert record to chatter_company_feed_tree table
 */
function ncn_chatter_insert_company_feed_tree_record($parent_fid, $child_fid) {
    $result = db_insert('chatter_company_feed_tree')
            ->fields(array(
                'parent_fid' => $parent_fid, 
                'child_fid'  => $child_fid
            ))
            ->execute();
    return $result;
}

/**
 * Create Claim Feed Tree
 * Company - AR - Claim
 */
function ncn_chatter_create_company_feed_tree($feed) {
    switch ($feed['type']) {
        case FEED_TYPE_COMPANY:
            break;
        case FEED_TYPE_AR:
        case FEED_TYPE_CLAIM:
            // Parent - Company Feed
            $claim_id = $feed['entity_id'];
            $claim = ncn_admin_get_claim_info($claim_id);
            if ($claim && $claim['user_id']) {
                $company_feed = ncn_chatter_get_feed_by_entity(FEED_TYPE_COMPANY, $claim['user_id']);
                if ($company_feed) {
                    $company_fid = $company_feed['fid'];
                } else {
                    // Create Company Feed
                    $company_fid = ncn_chatter_create_feed(array(
                        'type'      => FEED_TYPE_COMPANY, 
                        'entity_id' => $claim['user_id'])
                    );
                }
                ncn_chatter_insert_company_feed_tree_record($company_fid, $feed['fid']);    // Insert
            }

            if ($feed['type'] == FEED_TYPE_AR) {
                // Child - Company Feed
                $claim_feed = ncn_chatter_get_feed_by_entity(FEED_TYPE_CLAIM, $claim_id);

                if ($claim_feed) {
                    $claim_fid = $claim_feed['fid'];
                } else {
                    // Create Claim Feed
                    $claim_fid = ncn_chatter_create_feed(array(
                        'type'      => FEED_TYPE_CLAIM, 
                        'entity_id' => $claim_id)
                    );
                }
                if (isset($claim_fid)) {
                    ncn_chatter_insert_company_feed_tree_record($feed['fid'], $claim_fid);  // Insert
                }
            }
            break;
    }
}

/**
 * Check if user follows feed
 */
function ncn_chatter_check_user_follow_feed($uid, $fid) {
    $count = db_query("SELECT COUNT(*) 
                        FROM {chatter_follow_feed} 
                        WHERE uid=:uid AND fid=:fid", 
                    array(':uid' => $uid, 
                          ':fid' => $fid )
                )->fetchField();

    return ( ($count>0)? true:false );
}

/**
 * Get Child Feeds in Company 
 */
function ncn_chatter_get_follow_fid_from_tree($fid) {
    $result = db_select('chatter_company_feed_tree', 'cft')
        ->fields('cft')
        ->condition('parent_fid', $fid)
        ->execute();

    $data = array($fid=>$fid);        // Include itself
    while ($record = $result->fetchAssoc()) {
        $data[$record['child_fid']] = $record['child_fid'];
    }
    return $data;
}

/**
 * Get Feed of Users (Member + Sub Users) in Company
 */
function ncn_chatter_get_company_users_fid($company_fid) {
    $company_feed = ncn_chatter_get_feed($company_fid);
    $data = array();
    if ($company_feed) {
        $member_user = user_load($company_feed['entity_id']);
        if ($member_user) {
            // Member
            $member_feed = ncn_chatter_get_feed_by_entity(FEED_TYPE_USER, $member_user->uid);
            if ($member_feed) {
                $data[$member_feed['fid']] = $member_feed['fid'];
            }

            // Sub User
            if (isset($member_user->profile_memberid)) {
                $result = db_query("SELECT * FROM {member_subuser} WHERE member_id=:a", 
                                    array(':a'=>$member_user->profile_memberid));
                while ($record = $result->fetchAssoc()) {
                    $sub_user_feed = ncn_chatter_get_feed_by_entity(FEED_TYPE_USER, $record['sub_uid']);
                    if ($sub_user_feed) {
                        $data[$sub_user_feed['fid']] = $sub_user_feed['fid'];
                    }
                }
            }
        }
    }

    return $data;
}

/**
 * Get User's Follow Feeds
 */
function ncn_chatter_get_user_follow_fid($uid) {
    $data = array();
    if ($user_feed = ncn_chatter_get_feed_by_entity(FEED_TYPE_USER, $uid)) {
        // Include User Profile Feed
        $data[$user_feed['fid']] = $user_feed['fid'];
    }

    // Following Feed
    $result = db_select('chatter_follow_feed', 'ff')
        ->fields('ff')
        ->condition('uid', $uid)
        ->execute();
    while ($record = $result->fetchAssoc()) {
        $data[$record['fid']] = $record['fid'];
    }
    return $data;
}


/**
 * Callback: chatter/follow-feed/%fid
 */
function ncn_chatter_follow_feed_action($fid) {
global $user;
    $result = array(
        'status' => 'success', 
        'msg'    => '', 
        'posts'  => ''
    );

    $_action = isset($_REQUEST['follow_action'])? $_REQUEST['follow_action']:'follow';

    if (!is_member_user($user)) {
        if ($_action == "follow") {
            $msg = ncn_chatter_follow_feed($user->uid, $fid);
            if ($msg === true) {
            
            } else {
                $result['status'] = 'error';
                if ($msg) {
                    $result['msg'] = $msg;
                }
            }
        } else if ($_action == "unfollow") {
            ncn_chatter_unfollow_feed($user->uid, $fid);
        }
    }

    if ($result['status'] == 'success') {
        if ($_action == 'follow') {
            $result['next_action'] = 'unfollow';
        } else {
            $result['next_action'] = 'follow';
        }
    }

    echo json_encode($result);
}

/**
 * Follow Feed (Action)
 * Ignore for feed user followed automatically.
 */
function ncn_chatter_follow_feed($uid, $fid) {
    $feed = ncn_chatter_get_feed($fid);
    if ($feed) {
        if (!ncn_chatter_check_user_follow_feed($uid, $fid)) {
            $result = ncn_chatter_insert_follow_feed_record(array(
                'uid' => $uid,
                'fid' => $fid
            ));
        }
    } else {
        return "There isn't feed. Please refresh page.";
    }
    return true;
}
function ncn_chatter_unfollow_feed($uid, $fid) {
    $result = db_delete('chatter_follow_feed')
        ->condition('uid', $uid)
        ->condition('fid', $fid)
        ->execute();
}
