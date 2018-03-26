<?php

function ncn_claims_manager_menu_note(&$items) {
  $items['account/claim_note/add'] = array(
    'page callback' => 'ncn_claims_manager_note_add',
    'template' => 'page-inline',
    'type' => MENU_CALLBACK,
    'access arguments' => array('access content'),
  );
  
  $items['account/claim_note/update'] = array(
    'page callback' => 'ncn_claims_manager_note_update',
    'template' => 'page-inline',
    'type' => MENU_CALLBACK,
    'access arguments' => array('access content'),
  );
}

function ncn_claims_manager_note_render_claim_notes_table($claim_id, $user) {
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
      $operation_links = "<a href='' onclick='on_click_ncn_claims_manager_update_note($id); return false; '>Edit</a>"; 
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
* Callback (account/claim_note/add)
* 
*/
function ncn_claims_manager_note_add() {
GLOBAL $user;
  $claim_id = $_REQUEST['claim_id'];
  $note = $_REQUEST['note'];
  
  ncn_admin_note_add_note_from_original($claim_id);
  
  $json = array();
  $json['code'] = 'success';
  $json['msg'] = '';
  if (ncn_admin_note_add_note($claim_id, $user->uid, $note)) {
    $log_message = t("!user_name added Claim Note. !note", array('!user_name'=>ncn_amin_get_user_role_and_name($user->uid), '!note'=>strip_tags($note)));
    ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
  } else {
    $json['code'] = 'ERROR';
    $json['msg'] = t("Failed to add note to claim.");
  }
  
  echo json_encode($json);
  exit;
  
}

/**
* Callback (account/claim_note/update)
* 
*/
function ncn_claims_manager_note_update() {
  $note_id = $_REQUEST['note_id'];
  $note = $_REQUEST['note'];
  
  $json = array();
  $json['code'] = 'success';
  $json['note_id'] = $note_id;
  $json['msg'] = '';

    $claim_id = isset($_REQUEST['claim_id']) ? $_REQUEST['claim_id'] : 0;

  if (ncn_admin_note_update_note($note_id, $note)) {
    $log_message = t("!user_name update Claim Note. !note", array('!user_name'=>ncn_amin_get_user_role_and_name(), '!note'=>strip_tags($note)));
    ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
  } else {
    $json['code'] = 'ERROR';
  }
  
  echo json_encode($json);
  exit;
}
