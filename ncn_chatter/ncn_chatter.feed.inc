<?php

/**
 * Insert record to chatter_feed table
 */
function ncn_chatter_insert_feed_record($data=array()) {
    if ( ncn_chatter_get_feed_by_entity($data['type'], $data['entity_id']) )
    {
        return false;
    }

    $result = db_insert('chatter_feed')
        ->fields(array(
            'type'      => $data['type'], 
            'entity_id' => $data['entity_id'], 
            'status'    => (isset($data['status'])? $data['status'] : 1), 
        ))
        ->execute();

    $feed = ncn_chatter_get_feed_by_entity($data['type'], $data['entity_id']);
    return $feed['fid'];
}

/**
 * Insert record to chatter_feed_post table
 */
function ncn_chatter_insert_feed_post_record($data=array()) {
    if (!ncn_chatter_is_post_in_feed($data['pid'], $data['fid'])) {
        $result = db_insert('chatter_feed_post')
            ->fields(array(
                'fid'   => $data['fid'], 
                'pid'   => $data['pid'], 
                'created_at' => date('U'), 
            ))
            ->execute();
        return true;
    } else {
        return 'exist';
    }
    return true;
}



/**
 * Create Feed
 * @param  $feed: array( type, entity_id, status:optional )
 */
function ncn_chatter_create_feed($feed=array()) {
    $fid = ncn_chatter_insert_feed_record($feed);
    if (!$fid) { return false; }

    $feed['fid'] = $fid;
    ncn_chatter_create_company_feed_tree($feed);
    return $fid;
}

/**
 * Create User & Company Feed
 */
function ncn_chatter_create_user_feed($uid) {
    // User Feed
    $user_fid = ncn_chatter_create_feed(array(
        'type'      => FEED_TYPE_USER, 
        'entity_id' => $uid
    ));

    if (is_member_user($account)) {
        // Company Feed
        $company_fid = ncn_chatter_create_feed(array(
            'type'      => FEED_TYPE_COMPANY, 
            'entity_id' => $uid
        ));
    }
}

/**
 * Get Claim Feed
 * If it doesn't exist, it would be created.
 */
function ncn_chatter_get_claim_feed_id($claim_id, $type=FEED_TYPE_CLAIM) {
    $feed = ncn_chatter_get_feed_by_entity($type, $claim_id);
    $fid = 0;
    if (!$feed) {
        // Create Claim Feed
        $fid = ncn_chatter_create_feed(array('type'=>$type, 'entity_id'=>$claim_id));
        if (!$fid) { return 0; }
    } else {
        $fid = $feed['fid'];
    }
    return $fid;
}


/**
 * Get Feed by fid
 */
function ncn_chatter_get_feed($fid) {
    $feed = db_select('chatter_feed', 'f')
        ->fields('f')
        ->condition('fid', $fid)
        ->execute()
        ->fetchAssoc();
    return $feed;
}

/**
 * Get Feed of Entity
 */
function ncn_chatter_get_feed_by_entity($type, $entity_id) {
    $feed = db_select('chatter_feed', 'f')
        ->fields('f')
        ->condition('type', $type)
        ->condition('entity_id', $entity_id)
        ->execute()
        ->fetchAssoc();
    return $feed;
}

/**
 * Check if Post is in Feed
 */
function ncn_chatter_is_post_in_feed($pid, $fid) {
    $count = db_query("SELECT COUNT(*) 
                        FROM {chatter_feed_post} 
                        WHERE pid=:pid AND fid=:fid", 
                    array(':pid' => $pid, 
                          ':fid' => $fid )
                )->fetchField();

    return ( ($count>0)? true:false );
}




////////////////////////////////////////////////////////////////////////////////

/**
 * Get Feed Posts
 */
function ncn_chatter_get_feed_posts($fid, $type="feed", $params=array()) {
    if (!isset($params['page']))    { $params['page'] = 0; }
    if (!isset($params['perPage'])) { $params['perPage'] = 3; }     // Default

    $feed = ncn_chatter_get_feed($fid);
    if ($feed) {
        if ($type == 'feed') {
            switch ($feed['type']) {
                case FEED_TYPE_COMPANY:
                    return ncn_chatter_get_company_feed_posts($fid, $params);
                    break;
                case FEED_TYPE_AR:
                    return ncn_chatter_get_ar_feed_posts($fid, $params);
                    break;
                case FEED_TYPE_CLAIM:
                    return ncn_chatter_get_claim_feed_posts($fid, $params);
                    break;
                case FEED_TYPE_USER:
                    return ncn_chatter_get_user_profile_feed_posts($fid, $params);
                    break;
            }
        } else if ($type == 'user_follow' && $feed['type'] == FEED_TYPE_USER) {
            return ncn_chatter_get_user_follow_feed_posts($fid, $params);
        }
    }
    return array();
}

/**
 * Get Claim Feed Posts
 *
 * Only Posts in Claim Feed
 */
function ncn_chatter_get_claim_feed_posts($claim_fid, $params) {
    $follow_fids = array($claim_fid);
    $posts = ncn_chatter_get_posts_in_feeds($follow_fids, $params);
    return $posts;
}

/**
 * Get AR Feed Posts
 *
 * AR + Claim
 */
function ncn_chatter_get_ar_feed_posts($ar_fid, $params) {
    $follow_fids = ncn_chatter_get_follow_fid_from_tree($ar_fid);
    $posts = ncn_chatter_get_posts_in_feeds($follow_fids, $params);
    return $posts;
}

/**
 * Get Company Feed Posts
 *
 * AR + Claim + Member + Sub User
 */
function ncn_chatter_get_company_feed_posts($company_fid, $params) {
    // Company + AR + Claim
    $follow_fids = ncn_chatter_get_follow_fid_from_tree($company_fid);
    // Member + Sub User
    $user_fids = ncn_chatter_get_company_users_fid($company_fid);

    ////
    $follow_fids = array_merge($follow_fids, $user_fids);
    $posts = ncn_chatter_get_posts_in_feeds($follow_fids, $params);

    return $posts;
}

/**
 * Get User Own Feed Post
 */
function ncn_chatter_get_user_profile_feed_posts($user_fid, $params) {
    $follow_fids = array($user_fid);
    $posts = ncn_chatter_get_posts_in_feeds($follow_fids, $params);
    return $posts;
}

/**
 * Get User's Follow Feed Post
 */
function ncn_chatter_get_user_follow_feed_posts($user_fid, $params) {
    $posts = array();
    $user_feed = ncn_chatter_get_feed($user_fid);
    if ($user_feed) {
        $u = user_load($user_feed['entity_id']);
        if ($u) {
            if (is_member_user($u)) {
                // Company Feed Posts for Member Role
                $company_feed = ncn_chatter_get_feed_by_entity(FEED_TYPE_COMPANY, $u->uid);
                return ncn_chatter_get_company_feed_posts($company_feed['fid'], $params);
            } else {
                // User Follow Feed Posts
                $follow_fids = ncn_chatter_get_user_follow_fid($u->uid);
                $posts = ncn_chatter_get_posts_in_feeds($follow_fids, $params);
            }
        }
    }
    return $posts;
}

/**
 * Get All Posts in Following Feeds - [Basic Function]
 * @param $params: array(page, perPage)
 */
function ncn_chatter_get_posts_in_feeds($follow_fids, $params) {
global $user;
    $is_ncn_user = is_ncn_user($user);
    $perPage = $params['perPage'];
    $offset = $params['page'] * $perPage;

    $str_follow_fids = implode(', ', $follow_fids);

    $where_query='';
    if (!$is_ncn_user) {
        $where_query = "WHERE p.ncn_only = 0 ";
    }
    $query = "SELECT p.* 
                FROM (  SELECT DISTINCT(fp.pid) AS fpid, fp.created_at AS created_at 
                        FROM {chatter_feed_post} AS fp 
                        WHERE fp.fid IN ( $str_follow_fids ) 
                        ORDER BY fp.created_at DESC ) AS ffp 
                LEFT JOIN {chatter_post} AS p ON (ffp.fpid = p.id) 
                $where_query 
                ORDER BY ffp.created_at DESC, p.updated_at DESC 
                LIMIT $offset, $perPage";

    //ncn_print_r($query);
    $result = db_query($query);
    $posts = array();
    if ($result) {
        while ($post = $result->fetchAssoc()) {
            $post['count_comments'] = ncn_chatter_get_comments_under_post($post['id'], true);
            $posts[$post['id']] = $post;
        }
    }
    return $posts;
}
