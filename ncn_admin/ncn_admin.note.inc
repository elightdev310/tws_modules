<?php

function ncn_admin_menu_note(&$items) {
  $items['ajax/ncn_admin/get_claim_note'] = array(
    'page callback' => 'ncn_admin_ajax_get_claim_note',
    'access callback' => TRUE,
    'type' => MENU_CALLBACK,
  );
}

/**
* Insert Note into {claim_note} table
* 
* @param mixed $claim_id
* @param mixed $uid
* @param mixed $note
* @param mixed $capcrm_history_id
*/
function ncn_admin_note_db_insert_note($claim_id, $uid, $note, $capcrm_history_id) {
//  $query = "INSERT INTO {claim_note} (claim_id, uid, note, capcrm_history_id) VALUES(%d, %d, '%s', %d)";
//  $result = db_query($query, $claim_id, $uid, $note, $capcrm_history_id);
  $result = db_query('INSERT INTO {claim_note} (claim_id, uid, note, capcrm_history_id) VALUES(:a,:b,:c,:d)',
  array(':a'=>$claim_id,':b'=>$uid,':c'=>$note,':d'=>$capcrm_history_id));
  if ($result) { return TRUE; }
  else { return FALSE; }
}

/**
* Get Note Record from {claim_note} table
* 
* @param mixed $id
*/
function ncn_admin_note_db_get_note($id) {
 // $query = "SELECT * FROM {claim_note} WHERE id=%d";
  $result = db_query('SELECT * FROM {claim_note} WHERE id=:a',array(':a'=>$id));
//  if ($result && $note=db_fetch_array($result))
  if($result)
  {
      foreach($result as $note)
      {
          $note = (array)$note;
          if($note)
          {
              return $note;
          }
      }
   }

  return array();
}

/**
* Update Note in {claim_note} table
* 
* @param mixed $id
* @param mixed $note
*/
function ncn_admin_note_db_update_note($id, $note) {
 // $query = "UPDATE {claim_note} SET note='%s' WHERE id=%d";
  $result = db_query('UPDATE {claim_note} SET note=:a WHERE id=:b',array(':a'=>$note,':b'=>$id));
  if ($result) { return TRUE; }
  else { return FALSE; }
}

/**
* Delete Note from {claim_note} table
* 
* @param mixed $id
*/
function ncn_admin_note_db_delete_note($id) {
 // $query = "DELETE FROM {claim_note} WHERE id=%d";
  $result = db_query('DELETE FROM {claim_note} WHERE id=:a',array(':a'=>$id));
  if ($result) { return TRUE; }
  else { return FALSE; }
}

/**
* Get ALL notes from claim_id
* 
* @param mixed $claim_id
*/
function ncn_admin_note_get_all_notes_by_claim_id($claim_id) {
 // $query = "SELECT * FROM {claim_note} WHERE claim_id=".$claim_id." ORDER BY id";
  $result = db_query('SELECT * FROM {claim_note} WHERE claim_id=:a ORDER BY id',array(':a'=>$claim_id));
  $notes = array();
  foreach($result as $note)
  {
  	$note = (array)$note;
    $notes[$note['id']] = $note;
  }
  
  return $notes;
}

/**
* Render Claim Note List 
* 
* @param mixed $claim_id
* @param mixed $user
*/
function ncn_admin_note_render_claim_notes_table($claim_id, $user) {
  if (is_numeric($user)) {
    $user = user_load($user);
  }
  $notes = ncn_admin_note_get_all_notes_by_claim_id($claim_id);
  ob_start();
?>
<table class="claim-notes-table">
<tbody>
  <?php foreach ($notes as $id=>$note): ?>
  <?php 
    if (ncn_admin_note_check_editable_claim_note($user->uid, $id)) {
      $operation_links = "<a onclick='on_click_ncn_admin_update_note($id);'>Edit</a>"; 
    } else {
      $operation_links = '';
    }
  ?>
  <tr class="claim-note-item">
    <td class="note"><?php print $note['note']; ?></td>
    <td class="operation"><?php print $operation_links; ?></td>
  </tr>
  <?php endforeach; ?>
</tbody>
</table>
<?php
  $output = ob_get_contents();
  ob_end_clean();
  return $output;
}

/**
* Action to add claim note
* 
* @param mixed $claim_id
* @param mixed $uid
* @param mixed $note
*/
function ncn_admin_note_add_note($claim_id, $uid, $note) {
global $user;
  $note = trim($note);
  if (empty($note)) { return false; }

  $capcrm_history_id = 0;
  if (module_exists('ncn_capsulecrm')) {
    $capcrm_history_id = ncn_capsulecrm_note_add_note($claim_id, $note);
  }

  $msg = "Note: ".$note;

  ncn_chatter_auto_post_in_claim($claim_id, 0, $note);
  return ncn_admin_note_db_insert_note($claim_id, $uid, $note, $capcrm_history_id);
}

/**
* Action to update claim note
* 
* @param mixed $note_id
* @param mixed $note
*/
function ncn_admin_note_update_note($note_id, $note) {
  $note = trim($note);
  if (empty($note)) { return false; }
  $_note = ncn_admin_note_db_get_note($note_id);
  if ($_note['note'] != $note) {
    $note_capsulecrm_hid = $_note['capcrm_history_id'];
    if ($note_capsulecrm_hid != 0 && module_exists('ncn_capsulecrm') ) {
      ncn_capsulecrm_note_update_note($_note['claim_id'], $note_capsulecrm_hid, $note);
    }
    return ncn_admin_note_db_update_note($note_id, $note);
  }
  
  return FALSE;
}

/**
* Action to delete claim note
* 
* @param mixed $note_id
*/
function ncn_admin_note_delete_note($note_id) {
  $capcrm_history_id = 0;
  return ncn_admin_note_db_delete_note($claim_id, $uid, $note, $capcrm_history_id);
}

/**
* Callback (ajax/ncn_admin/get_claim_note)
* 
* @param mixed $note_id
*/
function ncn_admin_ajax_get_claim_note() {
  $note_id = $_REQUEST['note_id'];
  
  $json = array();
  $json['code'] = 'success';
  $json['note_id'] = $note_id;
  $note = ncn_admin_note_db_get_note($note_id);
  if (empty($note)) {
    $json['code'] = 'ERROR-NO-NOTE';
    $json['msg'] = "Note doesn't exist now";
  } else {
    $json['note'] = $note['note'];
  }
  
  echo json_encode($json);
  exit;
}

function ncn_admin_note_check_editable_claim_note($uid, $note_id) {
  $account = user_load($uid);
  if (user_access('ncn_admin edit claim note', $account)) { return TRUE; }
  else {
    $note = ncn_admin_note_db_get_note($note_id);
    if ($note['uid'] == $uid) {
      return TRUE; 
    }
  }
  return FALSE;
}

////////////////////////////////////////////////
function ncn_admin_note_add_note_from_original($claim_id) {
  $claim = get_claim_data($claim_id);
  $file_note = $claim['file_note'];
  if ($file_note != '') {
    $file_note = base64_decode($claim['file_note']);
    if ($file_note != '') {
      ncn_admin_note_add_note($claim_id, $claim['user_id'], $file_note);
    }
   // $query = "UPDATE {claims} SET file_note='' WHERE claim_id=%d";
    $result= db_query('UPDATE {claims} SET file_note=:b WHERE claim_id=:a',array(':a'=>$claim_id,':b'=>''));
  }
}

function ncn_admin_note_check_mode($claim_id) {
  $claim = get_claim_data($claim_id);
  $file_note = base64_decode($claim['file_note']);
  if ($file_note != '') {
    return 'old';
  }
  return 'new';
}

function ncn_admin_claim_file_note_change($claim_id) {
    $file_note = $_POST['claim_file_note'];
    if (module_exists('ncn_capsulecrm')) {
      $hid = ncn_capsulecrm_note_add_note($claim_id, $file_note);
    }
  
    $file_note = base64_encode(strip_tags($file_note));
    $log_message = ncn_amin_get_user_role_and_name()." update notes on file to '".$_POST['claim_file_note']."' ";
    drupal_set_message($log_message, 'status');
    ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
}
