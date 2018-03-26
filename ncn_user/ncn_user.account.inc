<?php
/**
 * URL: account/my-account.html
 * version: v2
 */
function ncn_edit_my_account()
{
    GLOBAL $user;

    ob_start();
    ?>
    <div id="my_account_page">
        <div class="map-content">
            <div class="map-header">
                <div class="title page-title text-bold">Edit My Profile</div>
                <!-- <div class="help"><a href="#">Help</a></div> -->
            </div>
            <div class="map-content-wrapper">
                <?php print theme('all_user_list_edit',array('in_admin'=>false, 'uid'=>$user->uid)) ?>
            </div>
        </div>
    </div>
    <?php
    $contents = ob_get_contents();
    ob_end_clean();

    return $contents;

}

/**
 * URL: account/my-extra-profile.html
 */
function ncn_edit_extra_profile()
{
    GLOBAL $user;

    ob_start();
    ?>
    <div id="my_account_page">
        <div class="map-content">
            <div class="map-header">
                <div class="title">Edit My Extra Profile</div>
                <div class="help"><a href="#">Help</a></div>
            </div>
            <div class="map-content-wrapper">
                <?php user_edit_extra_profile($user->uid, false); // function from ncn_admin.module ?>
            </div>
        </div>
    </div>
    <?php
    $contents = ob_get_contents();
    ob_end_clean();

    return $contents;
}
