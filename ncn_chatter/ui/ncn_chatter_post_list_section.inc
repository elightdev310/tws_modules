<?php
/**
 * Render Feed Post List Section
 * @var  $fid, $type
 */

$page_url = url("chatter/feed-loading-post/$fid/$type");
?>

<div class="ncn-feed-post-list" data-page="0" data-url="<?php echo $page_url ?>" >
</div>

<script>
jQuery(function($) {
    $(document).ready(function() {
        // Load first feed page
        ncn_feed_post_next_page(0, 'start');

        // Infinite Scroll
        $(window).scroll(function() {
            // console.log($(window).scrollTop());
            // console.log($(document).height() - $(window).height());
            if (Math.ceil($(window).scrollTop()) >= $(document).height() - $(window).height()) {
                var _page = $('.ncn-feed-post-list').attr('data-page');
                ncn_feed_post_next_page(_page);
            }
        });

        // Comment Link
        $('.ncn-feed-post-list').on('click', '.post-comment-link', function() {
            $post_box = $(this).closest('.post-box');
            var $txtInput = $post_box.find('.ncn_chatter_comment_form')
                                     .removeClass('hidden-form')
                                     .find('.status-input-text').focus();
            return false;
        });
        // Show all comments
        $('.ncn-feed-post-list').on('click', '.show-all-comments', function() {
            var $list = $(this).closest('.comment-list');
            $list.find('.comment-box.hidden-comment').each(function() {
                $(this).removeClass('hidden-comment');
            });

            $(this).closest('.comment-box').remove();
            return false;
        });

        // Like Link
        $('.ncn-feed-post-list').on('click', '.post-like-link, .comment-like-link', function() {
            var $this = $(this);
            var action = 'like';
            if ($this.hasClass('unlike')) {
                action = 'unlike';
            }

            var _url  = $this.attr('href');
            jQuery.ajax({
                url:    _url,
                type:   'POST',
                data:   {like_action: action}, 
                beforeSend: function(jqXHR, settings) {},
                error: function() {},
                success: function(response) {
                    eval("var json=" + response + ";");
                    if (json.status == "success") {
                        $this.removeClass('like').removeClass('unlike');
                        $this.addClass(json.next_action);
                        $this.html(json.next_action);
                    } else {
                        if (json.msg != "") { alert(json.msg); }
                    }
                }, 
                complete: function(jqXHR, textStatus) {}
            });
            return false;
        });
    });
});

// Next Feed Post
function ncn_feed_post_next_page(page, start='next') {
    var $section = jQuery('.ncn-feed-post-list');
    if (start == 'start') {
        $section.attr('data-page', 0);
        $section.removeClass('loading-end');
        $section.html('');
    }
    if ($section.hasClass("loading-posts") || $section.hasClass('loading-end')) {
        return;
    }

    var _url  = $section.data('url');
    jQuery.ajax({
        url:    _url,
        type:   'POST',
        data:   {page: page}, 
        beforeSend: function(jqXHR, settings) {
            $section.addClass("loading-posts");
            $section.append(jQuery('<div class="next-page-loading"></div>'));
            $section.find('.next-page-loading').loadingOverlay();
        },
        error: function() {},
        success: function(response) {
            setTimeout(function(){
                $section.removeClass("loading-posts");
                $section.find('.next-page-loading').loadingOverlay('remove');
                $section.find('.next-page-loading').remove();

                eval("var json=" + response + ";");
                if (json.status == "success") {
                    if (page == 0) {
                        $section.html(jQuery(json.posts).hide().fadeIn(300));
                    } else {
                        $section.append(jQuery(json.posts).hide().fadeIn(300));
                    }
                    var prevPage = $section.attr('data-page');
                    if (prevPage == json.nextPage) {
                        $section.addClass('loading-end');
                    } else {
                        $section.attr('data-page', json.nextPage);
                    }

                    // Next Posts until Scroll
                    var hContent = jQuery(document).height(); // get the height of your content
                    var hWindow = jQuery(window).height();  // get the height of the visitor's browser window

                    if(hContent<=hWindow) { 
                      var _page = $section.attr('data-page');
                      ncn_feed_post_next_page(_page);
                    }
                } else {
                    if (json.msg != "") { alert(json.msg); }
                }
            }, 300);
        }, 
        complete: function(jqXHR, textStatus) {}
    }); 
}
</script>
