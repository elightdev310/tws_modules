<form method="POST" autocomplete="off">
  <input type="hidden" name="tfunction" value="add_lead_generation_user">
  <fieldset>
  <legend>Create new lead generation user</legend>
  <table style="width: 600px; border:0;">
    <tr>
      <td>Username</td>
      <td><input type="text" name="name" value="<?= $result['name']; ?>"></td>
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
      <td><input type="text" name="firstname" value="<?= $result['firstname']; ?>"></td>
    </tr>
    <tr>
      <td>Last Name</td>
      <td><input type="text" name="lastname" value="<?= $result['lastname']; ?>"></td>
    </tr>
    <tr>
      <td>Company's Name</td>
      <td><input type="text" name="company_name" value="<?= $result['company_name']; ?>"></td>
    </tr>
    <?php /*<tr>
				<td>Assign Member</td>
				<td><?php print ncn_subuser_member_dropdown('parent_mid', $result['parent_mid']); ?></td>
			</tr> */ ?>
    <tr>
      <td colspan="2"><input type="submit" value="Add Lead Generator User"></td>
    </tr>
  </table>
  </fieldset>
</form>
