<?php

/**
 * @file
 * This file is Feed Post template
 *
 * @var $post
 */
global $user;
$post_like_class = '';
if (ncn_chatter_check_user_like_entity($user->uid, $post['id'], 0)) {
    $post_like_class = 'unlike';
} else {
    $post_like_class = 'like';
}
$post_like_url = url('chatter/like-post-comment/'.$post['id'].'/0');
$action_url = url('chatter/post/'.$post['id'].'/add/comment');
?>

<div class="panel-box post-panel-box">
    <div class="post-field">
        <div class="post-box">
            <div class="author-photo">
                <div style="width:32px; height: 32px;">
                    <img src="<?php echo file_create_url('public://default-user.png'); ?>" width="32" height="32" />
                </div>
            </div>
            <div class="mentions-container">
                <div class="post-meta-info">
                    <div>
                        <strong><?php echo ncn_chatter_render_user_name($post['uid']) ?></strong>
                    </div>
                    <div class="created-at">
                        <?php echo date("F j, Y \a\\t g:i A", $post['created_at']) ?>
                    </div>
                </div>
                <div class="post-content">
                    <?php echo ncn_chatter_change_tagging_content($post['content']) ?>
                    <div class="chatter-file-preview">
                        <?php echo ncn_chatter_render_attached_file($post['fid']); ?>
                    </div>
                </div>
                <div class="post-action-links">
                    <?php if ($user->uid != $post['uid']): ?>
                    <a href ="<?php echo $post_like_url ?>" 
                       class="post-like-link post-action-link <?php echo $post_like_class ?>">
                        <?php echo ucwords($post_like_class) ?> 
                    </a>
                    <?php endif; ?>
                    
                    <a href="#" class="post-comment-link post-action-link">Comment</a>
                </div>
                <div class="comment-section">
                    <div class="comment-list">
                        <?php echo ncn_chatter_render_post_comments($post['id']) ?>
                    </div>

                    <form class="ncn_chatter_comment_form <?php echo (isset($post['count_comments'])&&$post['count_comments']==0)? 'hidden-form':'' ?>" 
                          action="<?php echo $action_url ?>" class="" enctype="multipart/form-data" method="POST">
                        <input type="hidden" name="tfunction" value="chatter_create_comment" />
                        <div class="comment-box">
                            <div class="author-photo">
                                <div style="width:32px; height: 32px;">
                                    <img src="<?php echo file_create_url('public://default-user.png'); ?>" width="32" height="32" />
                                </div>
                            </div>
                            <div class="mentions-container">
                                <div class="mentions-input">
                                    <textarea class="status-input-text" name="content" row="1" placeholder="Write a comment..."></textarea>                    
                                </div>
                                <div class="clearfix">
                                    <div class="post-option-container">
                                        <div class="ncn-chatter-attach-file-section">
                                            <a href="#" class="upload-chatter-file-link">Attach file</a>
                                            <input type="file" class="chatter-attached-file hidden">
                                            <input type="hidden" name="chatter_file" class="chatter-attached-fid" value="0" />
                                            <div class="chatter-attached-file-preview">
                                                <span class="attached-filename">

                                                </span>&nbsp;
                                                <a href="#" class="remove-attached-file">Remove</a>
                                            </div>
                                        </div>
                                    </div>
                                    <div class="button-container">
                                        <button type="button" class="btn btn-primary add-comment-btn">Write</button>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </form>

                </div>
            </div>
        </div>
    </div>
</div>
