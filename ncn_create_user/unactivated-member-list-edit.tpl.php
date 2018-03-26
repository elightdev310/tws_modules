
<?php
//var_dump($result);
$b = $result;
//$query = db_query("SELECT * FROM member_id_pool WHERE used=0 AND member_id=:a",array(":a"=>$result));
$result = db_select('member_id_pool', 'n')
    ->fields('n')
    ->condition('member_id', $b,'=')
    ->condition('used', 0,'=')
    ->execute()
    ->fetchAssoc();

if (!$result) {
    drupal_goto('admin/config/ncn_create_user/unactivated_member_list');
    return;
}

$member = (array)$result;

$_owner = user_load($member['owner']);

if (is_distributor($_owner)) {
    drupal_add_js("
			$(document).ready(function() {
			$('#dist_asso').val('distributor');
			onchange_dist_asso();
			});", 'inline');
} else {
    drupal_add_js("
			$(document).ready(function() {
			$('#dist_asso').val('associate');
			onchange_dist_asso();
			});", 'inline');
}
drupal_add_js(drupal_get_path('module', 'ncn_admin') . '/ncn_admin.js');
?>
<fieldset>
    <h2>Member Information</h2>
    <div class="member_info_wrapper">
        <form id="una_member_edit" method="POST" >
            <input type="hidden" name="tfunction" value="una_member_edit" />
            <table>
                <tr>
                    <td class="td-label" width="150">Status:</td>
                    <td><?php echo draw_select_una_member_status('status', $member['status']); ?></td>
                </tr>
                <tr>
                    <td class="td-label" width="150">Member ID:</td>
                    <td><input type="text" name="member_id" value="<?php echo $member['member_id'];?>" size="50" readonly /></td>
                </tr>
                <tr>
                    <td class="td-label">First Name:</td>
                    <td><input type="text" name="first_name" value="<?php echo $member['first_name'];?>" size="50" /></td>
                </tr>
                <tr>
                    <td class="td-label">Last Name:</td>
                    <td><input type="text" name="last_name" value="<?php echo $member['last_name'];?>" size="50" /></td>
                </tr>
                <tr>
                    <td class="td-label">Company's Legal Name:</td>
                    <td><input type="text" name="legalname" value="<?php echo $member['legalname'];?>" size="50" /></td>
                </tr>
                <tr>
                    <td class="td-label">Country</td>
                    <td><?php draw_countryform_part_sel(array(
                            'name'=>'country', 
                            'class'=>'', 
                            'disabled'=>false, 
                            'required'=>false, 
                            'sel_val' =>$member['country'])); ?>
                    </td>
                </tr>
                <tr>
                    <td class="td-label">Address</td>
                    <td><input type="text" name="address" value="<?= $member['address']; ?>" size="50" /></td>
                </tr>
                <tr>
                    <td class="td-label">City</td>
                    <td ><input type="text" name="city" value="<?= $member['city']; ?>" size="50" /></td>
                </tr>
                <tr>
                    <td class="td-label">Province/State</td>
                    <td><?php draw_stateform_part_sel(array(
                                    'name'  => 'state', 
                                    'class' => '', 
                                    'disabled' => false, 
                                    'required' => flase, 
                                    'sel_val'  => $member['state'], 
                                    'country'  => $member['country'] )); ?>
                    </td>
                </tr>
                <tr>
                    <td class="td-label">Zip</td>
                    <td><input type="text" name="zip" value="<?= $member['zip']; ?>" size="50" /></td>
                </tr>
                <tr>
                    <td class="td-label">Office Phone</td>
                    <td><input type="text" name="officephone" value="<?= $member['officephone']; ?>" size="50" /> </td>
                </tr>
                <tr>
                    <td class="td-label">Mobile Phone</td>
                    <td><input type="text" name="mobilephone" value="<?= $member['mobilephone']; ?>" size="50" /> </td>
                </tr>
                <tr>
                    <td class="td-label">Contact Email</td>
                    <td><input type="text" name="contactemail" value="<?= $member['contactemail']; ?>" size="50" /> </td>
                </tr>
                <tr>
                    <td class="td-label">Sales Rep</td>
                    <td><input type="text" name="sales_rep" value="<?= $member['sales_rep']; ?>" size="50" />	</td>
                </tr>
                <tr class="distributor-list">
                    <td class="td-label">Type</td>
                    <td>
                        <select id="dist_asso" name="dist_asso">
                            <option value="distributor" <?php if(isset($_owner->roles['5']) && $_owner->roles['5']=='distributor') { echo "selected"; }?>>Distributor</option>
                            <option value="associate" <?php if(isset($_owner->roles['7']) && $_owner->roles['7']=='associate') { echo "selected"; }?>>Associate</option>
                        </select>
                    </td>
                </tr>
                <tr class="distributor-list da-dropdown">
                    <td class="td-label">Distributor</td>
                    <td><?php distributor_dropdown($member['owner']); ?></td>
                </tr>
                <tr class="associate-list da-dropdown">
                    <td class="td-label">Associate</td>
                    <td><?php associate_dropdown($member['owner']); ?></td>
                </tr>
                <tr>
                    <td class="td-label">Member Type</td>
                    <td>
                        <select id="member_type" name="member_type">
                            <option value="0" <?php if($member['member_type'] == 0){ echo "selected"; } ?> >NCN Gold Member</option>
                            <option value="1" <?php if($member['member_type'] == 1){ echo "selected"; } ?> >NCN Silver Member</option>
                            <option value="2" <?php if($member['member_type'] == 2){ echo "selected"; } ?> >NCN Gold Lite Member</option>
                            <option value="3" <?php if($member['member_type'] == 3){ echo "selected"; } ?> >NCN Coach on Call Member</option>
                            <option value="4" <?php if($member['member_type'] == 4){ echo "selected"; } ?> >NCN Gold Coach Member</option>
                            <option value="5" <?php if($member['member_type'] == 5){ echo "selected"; } ?> >CSI Member</option>
                        </select>
                    </td>
                </tr>

                <tr> <td colspan="2"><input type="submit" value="Update" /> </td> </tr>
            </table>
        </form>
    </div>
</fieldset>


<?php
if (module_exists('ncn_capsulecrm')) {
    $cc_map_info = ncn_capsulecrm_get_map_info($b);
    $organization_id = 0; $person_id = 0;
    if ($cc_map_info) {
        $organization_id = $cc_map_info['organization_id'];
        $person_id			 = $cc_map_info['person_id'];
        $_url_cc_org = ncn_capsulecrm_get_url()."/party/".$organization_id;
    }
}
?>

<?php if (module_exists('ncn_capsulecrm')): ?>
<fieldset class="edit_user_group">
    <h2 class="sub-title">CapsuleCRM</h2>
    <form method="POST" id="update_org_party_id">
        <input type="hidden" name="tfunction" value="update_org_party_id">
        <table style="width:100%;">
            <tr>
                <td class="td-label" width="150">Organization Party ID:</td>
                <td><input type="text" value="<?php echo $organization_id; ?>" readonly />&nbsp;&nbsp;<?php if (isset($_url_cc_org)) { print l($_url_cc_org, $_url_cc_org, array('attributes'=>array('target'=>'_blank'))); } ?></td>
            </tr>
        </table>
    </form>
</fieldset>
<?php @endif; ?>

<fieldset>
    <h2>Activation Email</h2>
    <form id="una_member_resend_mail" method="POST" >
        <input type="hidden" name="tfunction" value="una_member_resend_mail" />

        <input type="hidden" name="member_id" value="<?php echo $member['member_id'];?>" />
        <input type="hidden" name="first_name" value="<?php echo $member['first_name'];?>" />
        <input type="hidden" name="last_name" value="<?php echo $member['last_name'];?>" />
        <input type="hidden" name="contactemail" value="<?php echo $member['contactemail'];?>" />

        <input type="submit" value="Resend activation email to member"/>
    </form>
</fieldset>


