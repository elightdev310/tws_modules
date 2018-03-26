<form method="POST">
  <input type="hidden" name="tfunction" value="create-subuser">
  <fieldset>
  <legend>Create new Sub User</legend>
  <table style="width:400px;border:0;">
    <tr>
      <td>Username</td>
      <td><input type="text" name="name" value="<?= $result['name']; ?>"></td>
    </tr>
    <tr>
      <td>Assign Member</td>
      <td><?php print ncn_subuser_member_dropdown('parent_mid', $result['parent_mid']); ?></td>
    </tr>
    <tr>
      <td>E-mail</td>
      <td><input type="text" name="mail" value="<?= $result['mail']; ?>" AUTOCOMPLETE="off"></td>
    </tr>
    <tr>
      <td>Password</td>
      <td><input type="password" name="pass" value="<?= $result['pass']; ?>" AUTOCOMPLETE="off"></td>
    </tr>
    <tr>
      <td>Confirm Password</td>
      <td><input type="password" name="pass2" value="<?= $result['pass2']; ?>" AUTOCOMPLETE="off"></td>
    </tr>
    <tr>
      <td>First Name</td>
      <td><input type="text" name="profile_firstname" value="<?= $result['profile_firstname']; ?>"></td>
    </tr>
    <tr>
      <td>Last Name</td>
      <td><input type="text" name="profile_lastname" value="<?= $result['profile_lastname']; ?>"></td>
    </tr>
    <tr>
      <td>Company's Name</td>
      <td><input type="text" name="profile_legalname" value="<?= $result['profile_legalname']; ?>"></td>
    </tr>
    <tr>
      <td>Country</td>
      <td><?php draw_countryform_part_sel(array(
                            'name'=>'profile_country', 
                            'class'=>'', 
                            'disabled'=>false, 
                            'required'=>false)); ?></td>
    </tr>
    <tr>
      <td>Address</td>
      <td><input type="text" name="profile_address" value="<?= $result['profile_address']; ?>"></td>
    </tr>
    <tr>
      <td>City</td>
      <td><input type="text" name="profile_city" value="<?= $result['profile_city']; ?>"></td>
    </tr>
    <tr>
      <td>Province/State</td>
      <td><?php draw_stateform_part(array(
                            'name'  => 'profile_state', 
                            'class' => '', 
                            'disabled' => false, 
                            'required' => flase )); ?>
      </td>
    </tr>
    <tr>
      <td>Zip</td>
      <td><input type="text" name="profile_zip" value="<?= $result['profile_zip']; ?>"></td>
    </tr>
    <tr>
      <td colspan="2"><input type="submit" value="Add Sub User"></td>
    </tr>
  </table>
  </fieldset>
</form>
