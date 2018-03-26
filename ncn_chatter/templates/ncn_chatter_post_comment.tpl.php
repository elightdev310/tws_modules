<?php

/**
 * @file
 * This file is Post Comment template
 *
 * @var $comment
 */
global $user;
if (ncn_chatter_check_user_like_entity($user->uid, $comment['id'], 1)) {
    $comment_like_class = 'unlike';
} else {
    $comment_like_class = 'like';
}

$hidden_style = (isset($comment['hidden'])&&$comment['hidden'])? 'hidden-comment':'';
$comment_like_url = url('chatter/like-post-comment/'.$comment['id'].'/1');
?>

<div class="comment-box <?php echo $hidden_style ?>">
    <div class="author-photo">
        <div style="width:32px; height: 32px;">
            <img src="<?php echo file_create_url('public://default-user.png'); ?>" width="32" height="32" />
        </div>
    </div>
    <div class="mentions-container">
        <div class="comment-meta-info">
            <div>
                <strong><?php echo ncn_chatter_render_user_name($comment['uid']) ?></strong>
            </div>
            <div class="created-at">
                <?php echo date("F j, Y \a\\t g:i A", $comment['created_at']) ?>
            </div>
        </div>
        <div class="comment-content">
            <?php echo ncn_chatter_change_tagging_content($comment['content']) ?>
            <div class="chatter-file-preview">
                <?php echo ncn_chatter_render_attached_file($comment['fid']); ?>
            </div>
        </div>
        <div class="comment-action-links">
            <?php if ($user->uid != $comment['uid']): ?>
            <a href ="<?php echo $comment_like_url ?>" 
               class="comment-like-link comment-action-link <?php echo $comment_like_class ?>">
                <?php echo ucwords($comment_like_class) ?> 
            </a>
            <?php endif; ?>
        </div>
    </div>
</div>
