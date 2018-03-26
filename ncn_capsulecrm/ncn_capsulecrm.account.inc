<?php

function ncn_capsulecrm_account_get_account($user) {
  if (is_numeric($user)) {
    $user = user_load($user);
  }

  $result = db_query("SELECT * FROM {ncn_capsulecrm_account} WHERE uid=:d",array(":d"=>$user->uid));
  if ($result && $row_cct=$result->fetchAssoc()) {
    if ($row_cct['account']) {
      return $row_cct['account'];
    }
  }
  return "";
}

function ncn_capsulecrm_account_save_account($uid, $account) {
  $result = db_query("SELECT * FROM {ncn_capsulecrm_account} WHERE uid=:d",array(":d"=>$uid));
  if ($result->rowCount() > 0) {
    $result_save = db_query("UPDATE {ncn_capsulecrm_account} SET account=':s' WHERE uid=:d",
        array(":s"=>$account,":d"=>$uid));
  } else {
    $result_save = db_query("INSERT INTO {ncn_capsulecrm_account}(uid, account) VALUES(:d, ':s')",
        array(":d"=>$uid,":s"=>$account));
  }
  return $result_save;
}

function ncn_capsulecrm_account_select_render($name, $val) {

  $accounts = ncn_capsuelcrm_account_get_all_users();

  $output = "<select name='$name'>";

  $output.= "<option value=''>- none -</option>";

  if ($accounts && is_array($accounts)) {
    foreach ($accounts as $account) {
      $selected = '';
      if ($account->username == $val) { $selected = 'selected'; }
      $output.= "<option value='$account->username' $selected>$account->name</option>";
    }
  }

  $output.= "</select>";

  return $output;
}


function ncn_capsuelcrm_account_get_all_users() {
  $response = capsule_list_users();
  if ($response->code == 200)  {
    return $response->users;
  }

  return array();

}
