<?php

/**
 * Create All Feed of Member
 */
function ncn_chatter_sync_user_feed() {
    $query = "SELECT u.* FROM {users} as u 
                LEFT JOIN {users_roles} AS ur ON (ur.uid = u.uid) 
                WHERE ur.rid!=:rid";
    $result = db_query($query, array(':rid'=>USER_ROLE_MEMBER));
    $users = $result->fetchAll();

    foreach ($users as $user) {
        $user = (array)$user;
        // User Feed
        $user_fid = ncn_chatter_create_feed(array(
            'type'      => FEED_TYPE_USER, 
            'entity_id' => $user['uid']
        ));
        // Company Feed
        // $company_fid = ncn_chatter_create_feed(array(
        //     'type'      => FEED_TYPE_COMPANY, 
        //     'entity_id' => $user['uid']
        // ));
    }
}
