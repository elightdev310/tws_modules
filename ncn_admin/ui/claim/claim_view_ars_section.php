<?php
/**
 * Admin Claim View - ARS Section
 * @var: $claim_id, $is_ars, $claim
 */

$ars_file_status_info = ncn_ars_file_status();

$data = array();
//
if (isset($_POST['pr'])) {
    $data['pr'] = $_POST['pr'];
} else {
    $data['pr'] = get_ncn_data_extra($claim_id, 'ars_peer_review');
    if ($data['pr']) {
        $data['pr'] = unserialize($data['pr']);
    } else {
        $data['pr'] = array();
    }
}
//
if (isset($_POST['hoa'])) {
    $data['hoa'] = $_POST['hoa'];
} else {
    $data['hoa'] = get_ncn_data_extra($claim_id, 'ars_hoa');
    if ($data['hoa']) {
        $data['hoa'] = unserialize($data['hoa']);
    } else {
        $data['hoa'] = array();
    }
}

?>

<div class="na-claim-ars-section ncn-admin-claim-view-section">
<fieldset>
    <legend>ARS</legend>
    <form class="frm_ars" method="POST">
        <input name="ars_function" type="hidden" value="ars">
        <input name="current_scroll_position" class="current_scroll_position" type="hidden" value="">
        <div class="">
            <input name="ars_is_ars" id="ars_is_ars" type="checkbox" value="1" <?php echo $is_ars?"checked":""; ?> />&nbsp;&nbsp;
            <label for="ars_is_ars"><strong>Is this claim in ARS</strong></label>
        </div>
        <div class="ars-fields-section <?php echo ($is_ars?'':'hidden') ?>">
            <div class="">
                <label>ARS File Status: </label>
                <select name="ars_file_status" class="ars-file-status">
                <?php foreach ($ars_file_status_info as $ars_key=>$ars_val): ?>
                    <option value="<?php echo $ars_key; ?>" <?php echo ($claim['ars_file_status']==$ars_key)?'SELECTED':''; ?> >
                        <?php echo $ars_val; ?>
                    </option>
                <?php endforeach; ?>
                </select>
            </div>

            <table style="border: solid 0px;">
                <tbody style="border:0px;">
                    <tr class="odd">
                        <td colspan="2"><h3><strong>- Peer Review -</strong></h3></td>
                    </tr>
                    <tr class="even">
                        <td><label for="pr_company">Peer Review Company:</label></td>
                        <td><input name="pr[company]" size="50" value="<?php echo isset($data['pr']['company'])?$data['pr']['company']:''; ?>"></td>
                    </tr>
                    <tr class="odd">
                        <td><label for="pr_contact_first_name">Peer Review Contact First Name:</label></td>
                        <td><input name="pr[first_name]" size="50" value="<?php echo isset($data['pr']['first_name'])?$data['pr']['first_name']:''; ?>"></td>
                    </tr>
                    <tr class="even">
                        <td><label for="pr_contact_last_name">Peer Review Contact Last Name:</label></td>
                        <td><input name="pr[last_name]" size="50" value="<?php echo isset($data['pr']['last_name'])?$data['pr']['last_name']:''; ?>"></td>
                    </tr>

                    <tr class="odd">
                        <td colspan="2"><h3><strong>- HOA Information -</strong></h3></td>
                    </tr>
                    <tr class="even">
                        <td><label for="hoa_name">HOA Name:</label></td>
                        <td><input name="hoa[name]" size="50" value="<?php echo isset($data['hoa']['name'])?$data['hoa']['name']:''; ?>"></td>
                    </tr>
                    <tr class="odd">
                        <td><label for="hoa_contact_first_name">HOA Contact First Name:</label></td>
                        <td><input name="hoa[first_name]" size="50" value="<?php echo isset($data['hoa']['name'])?$data['hoa']['first_name']:''; ?>"></td>
                    </tr>
                    <tr class="even">
                        <td><label for="hoa_contact_last_name">HOA Contact Last Name:</label></td>
                        <td><input name="hoa[last_name]" size="50" value="<?php echo isset($data['hoa']['name'])?$data['hoa']['last_name']:''; ?>"></td>
                    </tr>
                </tbody>
            </table>
        </div>

        <input onclick="return on_set_scroll_position();" type="submit" value="  Save  ">
    </form>
</fieldset>
</div>

<script>
jQuery(function($) {
    $(document).ready(function() {
        // Hidden fields if claim isn't in ARS.
        $('#ars_is_ars').on('change', function() {
            if($(this).prop("checked") == true){
                $('.frm_ars .ars-fields-section').removeClass('hidden');
            } else {
                $('.frm_ars .ars-fields-section').addClass('hidden');
            }
        });
    });
});
</script>
