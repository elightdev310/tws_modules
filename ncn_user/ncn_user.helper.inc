<?php
// is this account a distributor?
function is_distributor($user)
{
    return isset($user->roles) && isset($user->roles[USER_ROLE_DISTRIBUTOR]);
    // foreach ($user->roles as $role)
    // {
    //     if ($role == "distributor")
    //     {   return true;    }
    // }
    // return false;
}
function is_leaduser($user)
{
    return isset($user->roles) && isset($user->roles[USER_ROLE_LEADUSER]);
}
function is_associate($user)
{
    return isset($user->roles) && isset($user->roles[USER_ROLE_ASSOCIATE]);
}
function is_member_user($user)
{
    return isset($user->roles) && isset($user->roles[USER_ROLE_MEMBER]);
}
function is_subuser($user)
{
    return isset($user->roles) && isset($user->roles[USER_ROLE_SUB_USER]);
}
function is_account_manager($user)
{
    return isset($user->roles) && isset($user->roles[USER_ROLE_ACCOUNT_MANAGER]);
}
function is_senior_account_manager($user)
{
    return isset($user->roles) && isset($user->roles[USER_ROLE_SENIOR_ACCOUNT_MANAGER]);
}
function is_director_member_service($user)
{
    return isset($user->roles) && isset($user->roles[USER_ROLE_DIRECTOR_OF_MEMBER_SERVICE]);
}
function is_claim_examiner($user)
{
    return isset($user->roles) && isset($user->roles[USER_ROLE_CLAIM_EXAMINER]);
}

function is_director_quality_assurance($user)
{
    foreach ($user->roles as $role)
    {
        if ($role == "Director of Quality Assurance")
        {   return true;    }
    }
    return false;
}

// User List
function get_roled_user_list($role_id, $mode) {
    $query = "SELECT * FROM users AS u
                INNER JOIN users_roles AS ur ON (ur.uid = u.uid)
                WHERE ur.rid = :rid AND u.status = :status";
    
    $status = 0;
    if ($mode=="active") {
        $status = 1;
    }

    $result = db_query($query, array(':rid'=>$role_id, ':status'=>$status));

    $n_result = $result->rowCount();
    $data = array();
    for ($i=0; $i<$n_result; $i++) {
        $row = $result->fetchAssoc();
        $data[] = $row;
    }
    return $data;
}
function get_distributor_list($mode="active"){
    return get_roled_user_list(USER_ROLE_DISTRIBUTOR, $mode);
}
function get_associate_list($mode="active"){
    return get_roled_user_list(USER_ROLE_ASSOCIATE, $mode);
}
function get_account_manager_list($mode="active") {
    return get_roled_user_list(USER_ROLE_ACCOUNT_MANAGER, $mode);
}
function get_senior_account_manager_list($mode="active") {
    return get_roled_user_list(USER_ROLE_SENIOR_ACCOUNT_MANAGER, $mode);
}
function get_ms_list($mode="active"){
    return get_roled_user_list(USER_ROLE_DIRECTOR_OF_MEMBER_SERVICE, $mode);
}

function get_qa_list($mode="active"){
    $query = "SELECT * FROM users";
    if ($mode=="active") {
        $query .=" WHERE status=1";
    } else if ($mode=="inactive") {
        $query .=" WHERE status=0";
    }
    
    $result = db_query($query);
    $n_qa = $result->rowCount();
    $qa_list = array();
    for ($i=0; $i<$n_qa; $i++) {
        $qa = $result->fetchAssoc();
        $_user = user_load($qa['uid']);
        if (is_director_quality_assurance($_user)) {
            $qa_list[] = $qa;
        }   
    }
    return $qa_list;
}

function is_ncn_user($user) {
    if (is_member_user($user) || 
        is_subuser($user))
    {
        return false;
    }
    
    return true;
}

// Check Member Role
function is_member($memberid) {
    $query = db_query('SELECT * FROM member_id_pool WHERE member_id=:a AND used=1',array(':a'=>$memberid));
    $result = $query;
    if ($result->rowCount()>0) {
        return true;
    }
    return false;
}

// Distributor Report Password
function add_report_password($username, $pass) {
    module_load_include('inc', 'content', 'includes/password');
    $query = "INSERT INTO auth_report(username, pass) VALUES('$username', '".user_hash_password(trim($pass))."')";
    $result = db_query($query);
}

function check_existing_email($email) {
    // Check in member_id_pool
    $result = db_query("SELECT count(*) FROM {member_id_pool} 
                        WHERE contactemail=:a AND used=:b",
                        array(':a'=>$email,':b'=>0))
            ->fetchField();
    if ($result) { return true; }

    // Check in users
    $result = db_query("SELECT count(*) FROM {users} WHERE mail=:a",
                        array(':a'=>$email))
            ->fetchField();
    if ($result) { return true; }
    return false;
}
function check_existing_memberid($member_id, $used) {
    $result = db_query('SELECT * FROM {member_id_pool} WHERE member_id=:a AND used=:b',array(':a'=>$member_id,':b'=>$used));
    return $result->rowCount();
}

function user_fullname($user) {
    $fullname = '';
    if (!isset($user->profile_firstname) || $user->profile_firstname == '')
    {
        $fullname = $user->name;
    } else {
        $fullname = $user->profile_firstname.' '.$user->profile_lastname;
    }
    return $fullname;
}
