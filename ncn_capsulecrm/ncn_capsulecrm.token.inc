<?php

function ncn_capsulecrm_token_get_user_token($user) {

  if (is_numeric($user)) {

    $user = user_load($user);

  }

  

  $result = db_query("SELECT * FROM {ncn_capsulecrm_token} WHERE uid=:d",array(":d"=>$user->uid));

  if ($result && $row_cct=$result->fetchAssoc()) {

    if ($row_cct['user_token']) {

      return $row_cct['user_token'];

    }

  }

  

  return "";

}



function ncn_capsulecrm_token_save_user_token($uid, $token) {

  $result = db_query("SELECT * FROM {ncn_capsulecrm_token} WHERE uid=:d",array(":d"=>$uid));

   $result = $result->rowCount();
  if ($result > 0) {

    $result_save = db_query("UPDATE {ncn_capsulecrm_token} SET user_token=':s' WHERE uid=:d",array(":s"=>$token,":d"=>$uid));

  } else {

    $result_save = db_query("INSERT INTO {ncn_capsulecrm_token}(uid, user_token) VALUES(:d, ':s')",
        array(":d"=>$uid,":s"=>$token));

  }

  return $result_save;

}