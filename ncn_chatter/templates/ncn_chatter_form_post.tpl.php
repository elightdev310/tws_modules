<?php

/**
 * @file
 * This file is Feed Post Form template
 *
 * @var $feed_id
 */
global $user;
$is_ncn_user = is_ncn_user($user);
$action_url = url('chatter/add/post/'.$feed_id);
$follow_feed_url = url('chatter/follow-feed/'.$feed_id);
if (ncn_chatter_check_user_follow_feed($user->uid, $feed_id)) {
    $follow_class = 'unfollow';
} else {
    $follow_class = 'follow';
}

$user_feed = ncn_chatter_get_feed_by_entity(FEED_TYPE_USER, $user->uid);
?>

<?php if (!is_member_user($user) && $user_feed['fid']!=$feed_id): ?>
<div class="follow-feed-section">
    <a href="<?php echo $follow_feed_url ?>" 
       class="follow-feed-link <?php echo $follow_class ?>">
        <?php echo ucwords($follow_class) ?> this feed
    </a>
</div>
<?php endif; ?>

<form id="ncn_chatter_post_form" action="<?php echo $action_url ?>" class="" enctype="multipart/form-data" method="POST">
    <input type="hidden" name="tfunction" value="chatter_create_post" />
    <div class="panel-box">
    <div class="post-field">
        <div class="post-box">
            <div class="author-photo text-label"><span>POST</span></div>
            <div class="mentions-container">
                <div class="mentions-input">
                    <textarea class="status-input-text" name="content" row="1" placeholder="Share and update..."></textarea>
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
                        <?php if ($is_ncn_user): ?>
                        <select class="post-filter" name="post_filter">
                            <option value="1">Netclaimsnow Only</option>
                            <option value="0">Public</option>
                        </select>
                        <?php endif; ?>
                    </div>
                    <div class="button-container">
                        <button type="button" class="btn btn-primary add-post-btn">Share</button>
                    </div>
                </div>
            </div>
        </div>
    </div>
    </div>
</form>

<script>
jQuery(function($) {
    $(document).ready(function() {
        $('.chatter-section').on('keyup', 'textarea.status-input-text', function() {
            textAreaAdjust(this);
        });
        
        $('.chatter-section').on('focus', 'textarea.status-input-text', function() {
            tribute.attach(document.querySelectorAll('.status-input-text'));
        });

        // Add Post
        $('.chatter-section .add-post-btn').on('click', function() {
            var $_form = $('#ncn_chatter_post_form');
            if (!$_form.find('textarea.status-input-text').val() && $_form.find('.chatter-attached-fid').val()==0) {
                return false;
            }
            var _url  = $_form.attr('action');
            $.ajax({
                url:    _url,
                type:   'POST',
                data:   $_form.serialize(), 
                beforeSend: function(jqXHR, settings) {
                    $_form.loadingOverlay();
                },
                error: function() {},
                success: function(response) {
                    eval("var json=" + response + ";");
                    if (json.status == "success") {
                        $_form.find('textarea.status-input-text').val('');
                        $_form.find('.chatter-attached-fid').val(0);
                        $_form.find('.attached-filename').html('');
                        $_form.find('.ncn-chatter-attach-file-section').removeClass('file-attached');
                        ncn_feed_post_next_page(0, 'start');
                    } else {
                        if (json.msg != "") { alert(json.msg); }
                    }
                }, 
                complete: function(jqXHR, textStatus) {
                    $_form.loadingOverlay('remove');
                }
            }); 
        });
        // Add Comment
        $('.chatter-section').on('click', '.add-comment-btn', function() {
            var $_this = $(this);
            var $_comment_section = $_this.closest('.comment-section');
            var $_form = $_this.closest('form.ncn_chatter_comment_form');
            if (!$_form.find('textarea.status-input-text').val() && $_form.find('.chatter-attached-fid').val()==0) {
                return false;
            }
            var _url  = $_form.attr('action');
            $.ajax({
                url:    _url,
                type:   'POST',
                data:   $_form.serialize(), 
                beforeSend: function(jqXHR, settings) {
                    $_form.loadingOverlay();
                },
                error: function() {},
                success: function(response) {
                    eval("var json=" + response + ";");
                    if (json.status == "success") {
                        $_form.find('textarea.status-input-text').val('');
                        $_form.find('.chatter-attached-fid').val(0);
                        $_form.find('.attached-filename').html('');
                        $_form.find('.ncn-chatter-attach-file-section').removeClass('file-attached');

                        // Render Comments
                        $_comment_section.find('.comment-list').html(json.comments);
                    } else {
                        if (json.msg != "") { alert(json.msg); }
                    }
                }, 
                complete: function(jqXHR, textStatus) {
                    $_form.loadingOverlay('remove');
                }
            }); 
        });

        // Follow Link
        $('.chatter-section .follow-feed-link').on('click', function() {
            var $this = $(this);
            var action = 'follow';
            if ($this.hasClass('unfollow')) {
                action = 'unfollow';
            }

            var _url  = $this.attr('href');
            jQuery.ajax({
                url:    _url,
                type:   'POST',
                data:   {follow_action: action}, 
                beforeSend: function(jqXHR, settings) {},
                error: function() {},
                success: function(response) {
                    eval("var json=" + response + ";");
                    if (json.status == "success") {
                        $this.removeClass('follow').removeClass('unfollow');
                        $this.addClass(json.next_action);
                        if (json.next_action == 'follow') {
                            $this.html('Follow this feed');
                        } else {
                            $this.html('Unfollow this feed');
                        }
                    } else {
                        if (json.msg != "") { alert(json.msg); }
                    }
                }, 
                complete: function(jqXHR, textStatus) {}
            });
            return false;
        });

        // Attach File
        $('.chatter-section').on('click', '.ncn-chatter-attach-file-section .upload-chatter-file-link', function() {
            var $section = $(this).closest('.ncn-chatter-attach-file-section');
            $section.find('.chatter-attached-file').trigger('click');
            return false;
        });

        $('.chatter-section').on('change', '.ncn-chatter-attach-file-section .chatter-attached-file', function() {
            var $section = $(this).closest('.ncn-chatter-attach-file-section');

            var input = $(this);
            var inputLength = input[0].files.length; //No of files selected
            if (inputLength > 0) {
                var file;
                var formData = new FormData();
                file = input[0].files[0];
                formData.append('files[chatter_file]', file);
                //send POST request to upload.php
                var _url = '<?php echo url('chatter/attach-file'); ?>';
                $.ajax({
                    url: _url,
                    type: "POST",
                    data: formData,
                    processData: false,
                    contentType: false,
                    beforeSend: function(jqXHR, settings) {
                        $section.loadingOverlay('init', {loadingText:'Uploading'});
                    },
                    error: function() {},
                    success: function(response) {
                        eval("var json=" + response + ";");
                        if (json.status == "success") {
                            $section.find('.chatter-attached-fid').val(json.file.fid);
                            $section.find('.chatter-attached-file-preview .attached-filename').html(json.file.filename);
                            $section.addClass('file-attached');
                        } else {
                            if (json.msg != "") { alert(json.msg); }
                        }
                    }, 
                    complete: function(jqXHR, textStatus) {
                        $section.loadingOverlay('remove');
                    }
                });
            }
        });

        $('.chatter-section').on('click', '.ncn-chatter-attach-file-section .remove-attached-file', function() {
            var $section = $(this).closest('.ncn-chatter-attach-file-section');
            $section.find('.chatter-attached-fid').val(0);
            $section.find('.chatter-attached-file-preview .attached-filename').html('');
            $section.removeClass('file-attached');
            return false;
        });
    });
    
    // @mention
    var tribute = new Tribute({
        values: [
            <?php $t_data = ncn_chatter_get_mention_user_data(); ?>
            <?php foreach($t_data as $uid=>$td): ?>
                { key:      "<?php echo $td['key']; ?>", 
                  value:    "<?php echo $td['value']; ?>", 
                  avatar:   "<?php echo $td['avatar']; ?>" },
            <?php endforeach; ?>
        ],
        menuItemTemplate: function(item) {
            return '<div class="ti-mention"><img src="'+item.original.avatar + '" width="24" height="24">' + item.string + '</div>';
        }, 
        selectTemplate: function(item) {
            return '@[' + item.original.value +']';
        },
        allowSpaces: false, 
    });
});
</script>
