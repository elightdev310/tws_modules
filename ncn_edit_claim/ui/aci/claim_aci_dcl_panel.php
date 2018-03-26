<?php
/**
 * ACI - Claim Information Panel
 *
 * @var
 *     $claim_id
 */
?>
<div id="dcl_log_list">
    <?php print ncn_admin_aci_render_dcl_log_list($claim_id); ?>
</div>
<div id="dcl_action_panel_content" class="dcl-action-panel">
    <div id="dcl_action_panel_descrption">
    <p style="color: red; margin: 10px;">
        *ONLY record hours for travel to and from jobsite, equipment setup, inspection, monitoring, taking photos, moisture readings, and take down of equipment
    </p>
    </div>
    <?php print ncn_admin_aci_render_dcl_action_panel($claim_id, 'new'); ?>
</div>
