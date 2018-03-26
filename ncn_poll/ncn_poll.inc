<?php
function ncn_poll_account_choice($poll_id) {
	if ( $_REQUEST['tfunction'] == 'ncn_poll_account_choice' ) {
		ncn_poll_account_choice_handler($poll_id);
	}
	$poll = ncn_poll_get_poll($poll_id);
	if (empty($poll)) {
		drupal_set_message('There isn\'t a poll.', 'error');
		return '';
	}
	ob_start();
?>
<div class="ncn-poll-account-choice-page">
	<form method="POST">
		<div class="ncn-poll-content-wrapper">
			<div class="ncn-poll-question">
				<p><?php print nl2br($poll['question']); ?></p>
			</div>
			<?php if (is_array($poll['choice']['data'])) :?>
				<div class="ncn-poll-choice">
					<?php $index=1; ?>
					<?php foreach ($poll['choice']['data'] as $choice_item): ?>
						<div class="choice-item">
							<span class="choice-item-index"><input type="radio" name="choice" id="choice_item_<?php echo $index?>" value="<?php echo $choice_item['choice_id']; ?>"/></span>
							<span class="choice-item-text"><label for="choice_item_<?php echo $index; ?>"><?php echo $choice_item['choice_text']; ?></label></span>
						</div>
						<?php $index++; ?>
					<?php endforeach; ?>
				</div>
			<?php endif; ?>
		</div>
		<div class="ncn-poll-comment-wrapper">
			<div><label for="edit_comment">Comment: <i>(optional)</i></label></div>
			<textarea rows="5" cols="80" id="edit_comment" name="comment"><?php print $_REQUEST['comment']; ?></textarea>
		</div>
		<div class="form-action">
			<input type="hidden" name="tfunction" value="ncn_poll_account_choice" />
			<input type="submit" value="  OK  " />
		</div>
	</form>
</div>
<?php
	$output = ob_get_contents();
	ob_end_clean();
	return $output;
}

function ncn_poll_account_choice_handler($poll_id) {
global $user;

	$choice_id = 0;
	if (isset($_REQUEST['choice']) && $_REQUEST['choice']>0) {
		$choice_id = $_REQUEST['choice'];
		if (!ncn_poll_member_polling($poll_id, $user->uid, $choice_id, date('U'), $_REQUEST['comment'])) {
			return FALSE;
		}
	} else {
		drupal_set_message("Please choose answer", 'error');
		return FALSE;
	}
	
	drupal_add_js("
		jQuery(document).ready(function(){ parent.$.fn.colorbox.close(); });
	", 'inline');
	return TRUE;
}

// ** NCN Splash Queue function
function ncn_poll_prompt_question() {
GLOBAL $user;
GLOBAL $base_url;
	$user = user_load($user->uid);
	if (!(isset($user->profile_memberid) && is_member($user->profile_memberid))) {
		return FALSE;
	}
	
	$poll_id = ncn_poll_get_upcoming_poll_id($user->uid);
	if ($poll_id != 0) {
		$url = $base_url.base_path().'account/ncn_poll/'.$poll_id;
		$eval_code = "open_colorbox_window('$url', 750, 500);";
		return $eval_code;
	}
	return FALSE;
}
?>
