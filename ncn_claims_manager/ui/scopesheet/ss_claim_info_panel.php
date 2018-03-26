<?php
/**
 * Room ScopeSheet - Claim Info Panel
 *
 * @var
 *     $claim_id, $room_name
 */
$photo_album_url = url("account/edit_claim_inline/$claim_id/room/".$room_name);
?>
<div class="aci-claim-info-wrapper ss-claim-info-wrapper clearfix">
    <div class='aci-claim-id aci-claim-info-item ss-claim-info-item'>
        Claim ID: <?php print $claim_id; ?>
    </div>
    <div class='aci-claim-room aci-claim-info-item ss-claim-info-item'>
        Room Name: <?php print ncn_claims_manager_scopesheet_render_select_room_name($claim_id, "ncn_cm_ss_room_name", $room_name, $mode) ?>
    </div>
    <div class='aci-claim-add-room aci-claim-info-item ss-claim-info-item'>
        <a href="#" class="add-room-link">Add Room</a>
    </div>

    <div class='aci-claim-info-item ss-claim-info-item align-right'>
    <?php if(!empty($room_name)): ?>
        <?php if ($mode=='edit'): ?>
            <?php $view_url = url("account/scope_sheet_view/$claim_id/$room_name"); ?>
            <a href="<?php print $view_url; ?>"><b>VIEW</b> Scopesheet</a>
        <?php else: ?>
            <?php $edit_url = url("account/scope_sheet/$claim_id/$room_name"); ?>
            <a href="<?php print $edit_url; ?>"><b>EDIT</b> Scopesheet</a>
        <?php endif; ?>
        &nbsp;&nbsp;|&nbsp;&nbsp;
    <?php endif; ?>
    <a href="<?php print $photo_album_url; ?>"><b>Edit</b> Photo Album</a>

    </div>
</div>

<script>
jQuery(function($) {
$(document).ready(function() {
    // Switch Room
    $('#ncn_cm_ss_room_name').on('change', function() {
        var url = $('#ncn_cm_ss_room_name').val();
        $('.claims-section-wrapper .panel-box').loadingOverlay();
        window.location = url;
    });

    // Add Room
    $('.aci-claim-info-wrapper .aci-claim-add-room .add-room-link').on('click', function() {
        var room_name = prompt("Please input room name.", "");
        room_name = string_trim(room_name);
        if (room_name == null || room_name == "") {
            return false;
        }

        var claim_id = '<?php print $claim_id; ?>';
        var _url = Drupal.settings.basePath+'account/claim/'+claim_id+'/add-room';
        jQuery.ajax({
            url:    _url,
            type:   'POST',
            data:   {room_name: room_name}, 
            beforeSend: function(jqXHR, settings) {
                $('.claims-section-wrapper .panel-box').loadingOverlay();
            },
            error: function() {},
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.status == "success") {
                    window.location = Drupal.settings.basePath+"account/scope_sheet/"+claim_id+"/"+encodeURIComponent(json.room_name);
                } else {
                    if (json.msg != "") { alert(json.msg); }
                }
            }, 
            complete: function(jqXHR, textStatus) {
                $('.claims-section-wrapper .panel-box').loadingOverlay('remove');
            }
        });
        return false;
    });
}); 
});
</script>

