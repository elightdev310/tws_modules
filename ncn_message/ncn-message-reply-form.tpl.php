<div id="msg_center_inbox" class="ncn_message_reply_form">
	<div class="mci-content">
		<div class="mci-header">
			<div class="title"><span class="sub_title"><?php echo drupal_get_title(); ?></span></div>
			<?php if (!strstr($_SERVER['REQUEST_URI'], 'admin/config')): ?>
			<div class="help"><a href="#">Help</a></div>
			<?php endif; ?>
		</div>
		<div class="mci-content-wrapper">
			<div class="table-body">
				<table class="pm">
					<tr>
						<td class="td-label"><strong><?php print t('Subject');?>:</strong></td>
						<td class="td-input"><?php print drupal_render($form['subject']); ?></td>
					</tr>
					<tr>
						<td class="td-label"><strong><?php print t('Message');?>:</strong></td>
						<td class="td-input"><?php print drupal_render($form['body']); ?></td>
					</tr>
					<tr>
						<td class="td-label"></td>
						<td class="td-input reply-submit-btn"><?php print drupal_render($form['submit']); ?></td>
					</tr>
				</table>
			</div>
		</div>
	</div>
</div>

<?php print $ncn_message_reply_form_hidden; ?>