<?php
/**
 * User Dashboard Feed
 */
function ncn_chatter_user_news_feed_block($uid=0) {
global $user;
    $_user = NULL;
    if ($uid == 0) {
        $_user = $user;
    } else {
        $_user = user_load($uid);
    }
    if (!$_user) {
        return '';
    }

    $user_feed = ncn_chatter_get_feed_by_entity(FEED_TYPE_USER, $_user->uid);
    if (!$user_feed) {
        ncn_chatter_create_user_feed($_user->uid);
        $user_feed = ncn_chatter_get_feed_by_entity(FEED_TYPE_USER, $_user->uid);
        if (!$user_feed) {
            return '';
        }
    }

    $post_form = theme('ncn_chatter_form_post', array('feed_id'=>$user_feed['fid']));   // Post Form
    $post_list = ncn_chatter_render_post_list_section($user_feed['fid'], 'user_follow');
    
    $output = $post_form.$post_list;
    return $output;
}

/**
 * Page Handler: ncn-chatter/ncn-feed
 */
function ncn_chatter_news_feed_page() {
global $user;
    if (!$user->uid) {
        return false;
    }

    $user_feed = ncn_chatter_get_feed_by_entity(FEED_TYPE_USER, $user->uid);
    if (!$user_feed) {
        ncn_chatter_create_user_feed($user->uid);
        $user_feed = ncn_chatter_get_feed_by_entity(FEED_TYPE_USER, $user->uid);
        if (!$user_feed) {
            drupal_set_message(t("User Feed doesn't exist. Please report this to administrator."), 'warning');
            return false;
        }
    }
    
    $post_form = theme('ncn_chatter_form_post', array('feed_id'=>$user_feed['fid']));   // Post Form
    $post_list = ncn_chatter_render_post_list_section($user_feed['fid'], 'user_follow');

    ob_start();
    @include get_realpath('ncn_chatter', 'ui/ncn_feed_page.php');
    $content = ob_get_contents();
    ob_end_clean();

    return $content;
}

/**
 * Page Handler: ncn-chatter/user-profile-feed/%
 */
function ncn_chatter_user_profile_feed_page($uid) {
    $user_feed = ncn_chatter_get_feed_by_entity(FEED_TYPE_USER, $uid);
    if ($user_feed) {

        return ncn_chatter_feed_page($user_feed['fid']);
    }
    return '';
}

/**
 * Page Handler: ncn-chatter/feed/%
 */
function ncn_chatter_feed_page($fid) {
    $feed_block = ncn_chatter_feed_block($fid);

    $output = '<div class="chatter-section">';
    $output.=   '<div class="chatter-section-content">';
    $output.=   $feed_block;
    $output.= '</div></div>';

    return $output;
}
function ncn_chatter_feed_block($fid) {
global $user;
    if (!ncn_chatter_check_feed_available($user, $fid)) {
        return '';
    }

    $feed = ncn_chatter_get_feed($fid);
    if ($feed) {
        $post_form = theme('ncn_chatter_form_post', array('feed_id'=>$fid));   // Post Form
        // $posts = ncn_chatter_get_feed_posts($fid);
        // $post_list = ncn_chatter_render_post_list($posts);      // Render Post List
        $post_list = ncn_chatter_render_post_list_section($fid);

        $output = $post_form.$post_list;
        return $output;
    }
    return '';
}

/**
 * Render Claim Feed Block in Claim View Page
 */
function ncn_chatter_claim_feed_block($claim_id, $type='claim') {
    $claim = get_claim_data($claim_id);
    if ($claim) {
        if ($type == 'ar') {
            $type = FEED_TYPE_AR;
        } else {
            $type = FEED_TYPE_CLAIM;
        }

        $fid = ncn_chatter_get_claim_feed_id($claim_id, $type);
        if (!$fid) {
            return '';
        }

        $feed_block = ncn_chatter_feed_block($fid);
        $output = '<div class="chatter-section">';
        $output.=   '<div class="chatter-section-content">';
        $output.=   $feed_block;
        $output.= '</div></div>';
        return $output;
    }
    return '';
}

/**
 * Check if user could access to this feed
 */
function ncn_chatter_check_feed_available($user, $fid) {
    // User profile feed
    $user_feed = ncn_chatter_get_feed_by_entity(FEED_TYPE_USER, $user->uid);
    if ($user_feed && $user_feed['fid'] == $fid) {
        return true;
    }

    if (is_member_user($user)) {
        $company_feed = ncn_chatter_get_feed_by_entity(FEED_TYPE_COMPANY, $user->uid);
        if ($company_feed) {
            $company_fid = $company_feed['fid'];
            // Company + AR + Claim
            $follow_fids = ncn_chatter_get_follow_fid_from_tree($company_fid);
            // Member + Sub User
            $user_fids = ncn_chatter_get_company_users_fid($company_fid);
            if (isset($follow_fids[$fid]) || isset($user_fids[$fid])) {
                return true;
            }
        }
    } else if (is_subuser($user)) {
        $member_id = ncn_subuser_get_memberid_from_subuid($user->uid);
        if (!$member_id) {
            return false;
        }

        $member_uid = ncn_subuser_get_parent_member_info($member_id);
        if ($member_uid) {
            $company_feed = ncn_chatter_get_feed_by_entity(FEED_TYPE_COMPANY, $member_uid);
            if ($company_feed) {
                $company_fid = $company_feed['fid'];
                // AR + Claim
                $follow_fids = ncn_chatter_get_follow_fid_from_tree($company_fid);
                unset($follow_fids[$company_fid]);  // Remove Company Feed
                if (isset($follow_fids[$fid])) {
                    return true;
                }
            }
        }
    } else {
        return true;
    }

    return false;
}

/**
 * Render Post List
 */
function ncn_chatter_render_post_list_section($fid, $type="feed") {
    // Tribute
    drupal_add_js (drupal_get_path('module', 'ncn_admin').'/js_plugins/tribute/tribute.min.js');
    drupal_add_css(drupal_get_path('module', 'ncn_admin').'/js_plugins/tribute/tribute.css', 'module', 'all', FALSE);

    ob_start();
    @include get_realpath('ncn_chatter', 'ui/ncn_chatter_post_list_section.inc');    // Render Post List
    $output = ob_get_contents();
    ob_end_clean();
    return $output;
}

/**
 * Get Next Posts in Infinite Scroll
 * 
 * Callback: chatter/feed-loading-post/%fid/%type
 */
function ncn_chatter_get_feed_next_posts($fid, $type) {
global $user;
    $result = array(
        'status' => 'success', 
        'msg'    => '', 
        'posts'  => ''
    );

    $page = isset($_REQUEST['page'])? $_REQUEST['page'] : 0;
    $result['nextPage'] = $page;

    $posts = ncn_chatter_get_feed_posts($fid, $type, array('page'=>$page));
    if ($posts && !empty($posts)) {
        $posts_html = ncn_chatter_render_post_list($posts);

        $result['nextPage'] = ++$page;
        $result['posts'] = $posts_html;
    }
    
    echo json_encode($result);
}

function ncn_chatter_render_post_list($posts) {
    $post_list = '';
    if(!empty($posts)) {
        foreach ($posts as $post) {
            $post_list.=theme('ncn_chatter_feed_post', array('post'=>$post));
        }
    }
    return $post_list;
}


/**
 * Render Comment lists under Post
 */
function ncn_chatter_render_post_comments($pid) {
    $comments = ncn_chatter_get_comments_under_post($pid);
    $html = '';

    $count = count($comments) - 3;
    if ($count>0) {
        $_str = "Show all ".count($comments)." comments";

        $html.= '<div class="comment-box">';
        $html.=     '<a href="#" class="show-all-comments">'.$_str.'</a>';
        $html.= '</div>';
    }
    foreach($comments as $comment) {
        if ($count > 0) {
            $comment['hidden'] = true;
        }
        $html .= theme('ncn_chatter_post_comment', array('comment'=>$comment));
        $count--;
    }
    return $html;
}

/**
 * Render username in post panel
 */
function ncn_chatter_render_user_name($user_id) {
    $user = user_load($user_id);
    if (!$user) {
        return 'User #'.$user_id;
    }
    $username = ncn_chatter_get_username($user);
    if ($username) {
        return $username;
    }
}
function ncn_chatter_get_username($user) {
    if ($user) {
        if (isset($user->profile_firstname) && isset($user->profile_lastname)) {
            if (!$user->profile_firstname && !$user->profile_lastname) {
                return $user->name;
            }
            return $user->profile_firstname.' '.$user->profile_lastname;
        } else if (isset($user->profile_legalname)) {
            return $user->profile_legalname;
        } else {
            return $user->name;
        }
    }
}
