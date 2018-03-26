<?php

/**
 * Page Callback (admin/config/ncn_poll/list)
 */
function ncn_poll_admin_list() {
	global $base_url;
	$header = array(
		array('data' => t('ID')), 
		array('data' => t('Question')), 
		array('data' => t('Status')), 
		array('data' => t('Created')), 
		array('data' => t('Updated')), 
//		array('data' => t('Choices')),
		array('data' => t('Operations'), 'colspan' => 3) );

	$rows = array();

    //var_dump($header);

//    $result = db_query('ncn_poll', 'n')
//        ->fields('n')
//        ->orderBy('updated', 'DESC')//ORDER BY created
//        ->execute();

	$result = db_query('SELECT * FROM {ncn_poll} ORDER BY updated DESC');


//    foreach($result as $poll)
//    {
//        $poll = ncn_poll_get_poll($poll->id);
//        $rows[] = array(
//            $poll->id, (strlen($poll->question)>50)? substr($poll->question, 0, 50)." ...":$poll->question , ($poll->status==0)? 'Unpublished':'Published', date('d M Y H:i:s', $poll->created), date('d M Y H:i:s', $poll->updated),
////			is_array($poll['choice']['data'])? count($poll['choice']['data']):0,
//            l(t('Edit'), "admin/config/ncn_poll_item/".$poll->id."/edit"), l(t('Track'), "admin/config/ncn_poll_item/".$poll->id."/track"),
//            "<a href='".$base_url."/admin/config/ncn_poll_item/".$poll->id."/delete' onclick='return onclick_delete_poll()'>Delete</a>"
//        );
//    }
//
//
//    return theme('table', $header, $rows);
//    }



//    while ($poll = db_fetch_array($result))
foreach($result as $poll)
      {
        $poll = (array)$poll;
       // print_r($poll['id']);
        $poll = ncn_poll_get_poll($poll['id']);
       // var_dump($poll);
          $rows[] = array(
			$poll['id'], (strlen($poll['question'])>50)? substr($poll['question'], 0, 50)." ...":$poll['question'] , ($poll['status']==0)? 'Unpublished':'Published', date('d M Y H:i:s', $poll['created']), date('d M Y H:i:s', $poll['updated']),
//			is_array($poll['choice']['data'])? count($poll['choice']['data']):0,
			l(t('Edit'), "admin/config/ncn_poll_item/".$poll['id']."/edit"), l(t('Track'), "admin/config/ncn_poll_item/".$poll['id']."/track"),
			"<a href='".$base_url."/admin/config/ncn_poll_item/".$poll['id']."/delete' onclick='return onclick_delete_poll()'>Delete</a>"
		);
          //var_dump($rows);
      }

    //var_dump($rows);
	return theme('table', array('header'=>$header,'rows'=>$rows));
}

/**
 * Page Callback (admin/config/ncn_poll/add)
 */
function ncn_poll_admin_add() {
	$tfunction = 'ncn_poll_admin_add';
	$data = array();
	$data['tfunction'] = $tfunction;

	if ((isset($_REQUEST['tfunction'])) && ($_REQUEST['tfunction'] == $tfunction)) {
		ncn_poll_admin_add_handler($data);
	}
	return ncn_poll_render_admin_poll_page($data);
}

function ncn_poll_admin_add_handler(&$data) {
	$quiz = ncn_poll_get_object_from_admin_poll_page('create');
	if (ncn_poll_admin_add_validate($quiz)) {
		if (ncn_poll_admin_add_submit($quiz)) {
			drupal_goto('admin/config/ncn_poll/list');
		}
	}
	$data['quiz'] = $quiz;
}

function ncn_poll_get_object_from_admin_poll_page($op) {
	$quiz_id = arg(3);
	$question	= $_REQUEST['question'];
	$status		= $_REQUEST['status'];
	$choice 	= array();
	$choice['max_choice_id'] = intval($_REQUEST['max_choice_id']);
	
	$choice['data'] = array();
	$choice_text = isset($_REQUEST['choice_text'])? $_REQUEST['choice_text']: array();
	$choice_id   = isset($_REQUEST['choice_id'])? $_REQUEST['choice_id']: array();
	for($i=0; $i<count($choice_id); $i++) {
		$ci = array(
			'choice_text' => trim($choice_text[$i]), 
			'choice_id' => intval($choice_id[$i]), 
		);
		if (!empty($ci['choice_text'])) {
			if ($ci['choice_id'] == 0) {	
				$choice['max_choice_id'] += 1;
				$ci['choice_id'] = $choice['max_choice_id'];
			}
			$choice['data'][] = $ci;
		}
	}
	
	return array(
		'id'=>$quiz_id, 
		'question'=>$question, 
		'status'=>$status, 
		'choice'=>$choice, 
	);
}

function ncn_poll_admin_add_validate(&$quiz) {
	if (empty($quiz['question'])) {
		drupal_set_message('Please input question.', 'error');
		return FALSE;
	}
	return TRUE;
}

function ncn_poll_admin_add_submit(&$quiz) {
	$choice_data = serialize($quiz['choice']);
	$timestamp = date('U');

    $result = db_insert('ncn_poll') // Table name no longer needs {}
        ->fields(array(
            'question' => $quiz['question'],
            'status' => $quiz['status'],
            'created' => $timestamp,
            'updated' => $timestamp,
            'choice' => $choice_data,
        ))
        ->execute();


	//$result = db_query("INSERT INTO {ncn_poll} (question, status, created, updated, choice) VALUES(:s, :d, :d, :d, :s)",
	//										array("s"=>$quiz['question'],"d"=>$quiz['status'],"d"=>$timestamp,"d"=>$timestamp,"s"=>$choice_data));

    if ($result) {
		drupal_set_message('Created the new poll, successfully.');
	} else {
		drupal_set_message('Failed to create the new poll.', 'error');
		return FALSE;
	}
	return TRUE;
}


function ncn_poll_render_admin_poll_page($data) {
	$quiz = isset($data['quiz'])? $data['quiz']:array();
    ob_start();
?>
<div class="ncn-poll-admin-poll-page">
	<form method="POST">
	<div id="edit_question_wrapper" class="form-item">
		<label for="edit_question">Question: <span title="This field is required." class="form-required">*</span></label>
		<textarea id="edit_question" class="form-textarea required" name="question" rows="5" cols="150"><?php print $quiz['question']; ?></textarea>
	</div>
	<div id="edit_choice_wrapper" class="form-item">
		<label for="edit_choice">Choice: </label>
		<div id="edit_choice_table_wrapper">
			<table id="edit_choice_table" class="sticky-enabled sticky-table">
				<tbody>
					<?php if (is_array($quiz['choice']['data'])) :?>
					<?php $index=1; ?>
					<?php foreach ($quiz['choice']['data'] as $choice_item): ?>
						<tr class="choice-item">
							<td class="handle-col"><?php echo $index.')';?></td>
							<td>
								<input type="text" name="choice_text[]" style="width: 95%;" value="<?php echo $choice_item['choice_text']; ?>" />
								<input type="hidden" name="choice_id[]" size="10" value="<?php echo $choice_item['choice_id']; ?>" />
							</td>
						</tr>
						<?php $index++; ?>
					<?php endforeach; ?>
					<?php endif; ?>
				</tbody>
			</table>
			<div class="description">
				To rearrange choice, drag the choice item to a new location in the list. <br/>
				To remove, make a text field empty.
			</div>
			<div class="choice-add">
				<input type="button" value=" Add another item " id="add_another_item" onclick="onclick_add_adnoter_item()"/>
			</div>
		</div>
	</div>
	<div id="edit_status_wrapper" class="form-item">
		<label for="edit_status">Status: <span title="This field is required." class="form-required">*</span></label>
		<select name="status" id="edit_status" class="form-select required">
			<option value="0" <?php echo ($quiz['status']==0)? 'selected':''; ?>>Unpublished</option>
			<option value="1" <?php echo ($quiz['status']==1)? 'selected':''; ?>>Published</option>
		</select>
	</div>
	<div class="form-submit">
		<input type="hidden" name="max_choice_id" value="<?php echo ($quiz['choice']['max_choice_id'])? $quiz['choice']['max_choice_id']: "0"; ?>" />
		<input type="hidden" name="tfunction" value="<?php echo $data['tfunction']; ?>" />
		<input type="submit" value="Save" />
	</div>
	</form>
</div>

<div id="choice_prototype">
	<table>
		<tr class="choice-item">
			<td class="handle-col">&nbsp;</td>
			<td>
				<input type="text" name="choice_text[]" style="width: 95%;"/>
				<input type="hidden" name="choice_id[]" size="10"/>
			</td>
		</tr>
	</table>
</div>

<script type="text/javascript">
function onclick_add_adnoter_item() {
	jQuery('#choice_prototype .choice-item').clone().appendTo('#edit_choice_table tbody');
	re_order_rows();
}

function re_order_rows() {
	jQuery("#edit_choice_table tr .handle-col").each(function(index) {
		jQuery(this).text((index+1)+")");
	});
}
jQuery(document).ready(function() {
	onclick_add_adnoter_item();
	jQuery("#edit_choice_table").tableDnD({
	    onDragClass: "myDragClass",
	    onDrop: function(table, row) {
        re_order_rows();
	    },
			onDragStart: function(table, row) {
			}
	});
});
</script>
<?php
	$output = ob_get_contents();
	ob_end_clean();
	return $output;
}


/**
 * Page Callback (admin/config/ncn_poll_item/%/edit)
 */
function ncn_poll_admin_edit($poll_id)
{
	drupal_set_title(t('Poll #!poll_id', array('!poll_id'=>$poll_id)));
	
	$tfunction = 'ncn_poll_admin_edit';
	$data = array();
	$data['tfunction'] = $tfunction;
	if ((isset($_REQUEST['tfunction'])) && ($_REQUEST['tfunction'] == $tfunction)) {
		ncn_poll_admin_edit_handler($data);
	} else {
		$data['quiz'] = ncn_poll_get_poll($poll_id);
	}
	return ncn_poll_render_admin_poll_page($data);
}

function ncn_poll_admin_edit_handler(&$data) {
	$quiz = ncn_poll_get_object_from_admin_poll_page('update');
	if (ncn_poll_admin_add_validate($quiz)) {
		if (ncn_poll_admin_edit_submit($quiz)) {
			drupal_goto('admin/config/ncn_poll/list');
		}
	}
	$data['quiz'] = $quiz;
}

function ncn_poll_admin_edit_submit(&$quiz) {
	$choice_data = serialize($quiz['choice']);
	$timestamp = date('U');

    $result = db_update('ncn_poll') // Table name no longer needs {}
        ->fields(array(
            'question' => $quiz['question'],
            'status' => $quiz['status'],
            'updated' => $timestamp,
            'choice' => $choice_data,
        ))
        ->condition('id', $quiz['id'], '=')
        ->execute();

//	$result = db_query("UPDATE {ncn_poll} SET question='%s', status=%d, updated=%d, choice='%s' WHERE id=%d",
//											$quiz['question'], $quiz['status'], $timestamp, $choice_data, $quiz['id']);
	if ($result) {
		drupal_set_message('Updated poll, successfully.');
	} else {
		drupal_set_message('Failed to update poll.', 'error');
		return FALSE;
	}
	return TRUE;
}

/**
 * Page Callback (admin/config/ncn_poll_item/%/track)
 */
function ncn_poll_admin_delete($poll_id)
{
	$poll = ncn_poll_get_poll($poll_id);
	if (!empty($poll)) {
		$result  = db_query('DELETE FROM {ncn_poll} WHERE id=:d', array(':d'=>$poll_id));
		$result1 = db_query('DELETE FROM {ncn_poll_track} WHERE poll_id=:d',array(':d'=>$poll_id));
		if ($result && $result1) {
			drupal_set_message(t("Deleted poll(#!poll_id), successfully.", array('!poll_id'=>$poll_id)));
		} else {
			drupal_set_message(t("Failed to delete poll(#!poll_id).", array('!poll_id'=>$poll_id)), 'error');
		}
	} else {
		drupal_set_message(t("Poll(#!poll_id) doesn't exist", array('!poll_id'=>$poll_id)), 'error');
	}
	drupal_goto('admin/config/ncn_poll/list');
}


/**
 * Page Callback (admin/config/ncn_poll_item/%/track)
 */
function ncn_poll_admin_track($poll_id)
{
	drupal_set_title(t('Poll #!poll_id', array('!poll_id'=>$poll_id)));
	$poll = ncn_poll_get_poll($poll_id);
    //var_dump($poll);
	$output = '';
	ob_start();
?>
<div class="ncn-poll-admin-poll-result-page">
	<div class="ncn-poll-content-wrapper">
		<div class="ncn-poll-question">
			<p><?php echo nl2br($poll['question']); ?></p>
		</div>
		<?php if (is_array($poll['choice']['data'])) :?>
			<div class="ncn-poll-choice">
				<?php $index=1; ?>
				<?php foreach ($poll['choice']['data'] as $choice_item): ?>
                	<div class="choice-item">
						<span class="choice-item-index"><?php echo $index.')';?></span>
						<span class="choice-item-text"><?php echo $choice_item['choice_text']; ?></span>
						<span class="choice-item-total">(<?php echo ncn_poll_get_total_answers($poll_id, $choice_item['choice_id']) ?>)</span>
					</div>
					<?php $index++; ?>
				<?php endforeach; ?>
			</div>
		<?php endif; ?>
		
	</div>
	<div class="ncn-poll-result-wrapper">
		<?php print ncn_poll_admin_render_poll_result($poll_id); ?>
	</div>
</div>
<?php
	$output = ob_get_contents();
	ob_end_clean();
	return $output;
}

function ncn_poll_admin_render_poll_result($poll_id) {
	$header = array(
		array('data' => t('Name')), 
		array('data' => t('Member ID')), 
		array('data' => t('Choice')), 
		array('data' => t('Timestamp')), 
		array('data' => t('Comment')), 
	);

	$poll = ncn_poll_get_poll($poll_id);
	$rows = array();
	$result = db_query('SELECT * FROM {ncn_poll_track} WHERE poll_id=:d', array(':d'=>$poll_id));
	
	//while ($pt = db_fetch_array($result))
    foreach($result as $pt)
    {
        $pt = (array)$pt;
		$_user = user_load($pt['uid']);
		$choice_index = ncn_poll_get_choice_index($poll, $pt['choice_id']);
		$choice = '';
		if ($choice_index > 0) {
			$choice = $choice_index.") " .$pt['choice_text'];
		}
		
		$rows[] = array(
			($_user->profile_firstname.' '.$_user->profile_lastname), 
			$_user->profile_memberid, 
			$choice, 
			date('d M Y H:i:s', $pt['timestamp']), 
			nl2br($pt['comment']), 
		);
	}
	$output = theme('table', array('header'=>$header,'rows'=>$rows,'attributes'=>array('id'=>'poll_result_table')));
	
	return $output;
}

?>
