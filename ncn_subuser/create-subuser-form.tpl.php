<form accept-charset="UTF-8" id="create-subuser-form" method="post" class="form-horizontal ncn-form-default">
<div id="page_results">
    <div id="create-subuser" class="claims-detail">
        <div class="claims-section-wrapper">
            <div class="clearfix">
                <div class="title page-title">Create Sub User</div>
            </div>
            <div class="panel-box">
                <div id="create-subuser-wrapper" class="panel-box-content">
                    <input type="hidden" name="tfunction" value="create-subuser">
                    <input type="hidden" name="parent_mid" value="<?php echo $user->profile_memberid; ?>">

                    <div class="form-group">
                        <div for="username" class="col-sm-3 control-label">Username: </div>
                        <div class="col-sm-9">
                            <input type="text" name="name" class="form-control" value="<?= $result['name']; ?>" >
                        </div>
                    </div>
                    <div class="form-group">
                        <div for="" class="col-sm-3 control-label">Email: </div>
                        <div class="col-sm-9">
                            <input type="text" name="mail" class="form-control" value="<?= $result['mail']; ?>" AUTOCOMPLETE="off">
                        </div>
                    </div>

                    <div class="form-group">
                        <div for="" class="col-sm-3 control-label">Password: </div>
                        <div class="col-sm-9">
                            <input type="password" name="pass" class="form-control" value="<?= $result['pass']; ?>" AUTOCOMPLETE="off">
                        </div>
                    </div>
                    <div class="form-group">
                        <div for="" class="col-sm-3 control-label">Confirm Password: </div>
                        <div class="col-sm-9">
                            <input type="password" name="pass2" class="form-control" value="<?= $result['pass2']; ?>" AUTOCOMPLETE="off">
                        </div>
                    </div>
                    <div class="form-group">
                        <div for="" class="col-sm-3 control-label">First Name: </div>
                        <div class="col-sm-9">
                            <input type="text" name="profile_firstname" class="form-control" value="<?= $result['profile_firstname']; ?>">
                        </div>
                    </div>
                    <div class="form-group">
                        <div for="" class="col-sm-3 control-label">Last Name: </div>
                        <div class="col-sm-9">
                            <input type="text" name="profile_lastname" class="form-control" value="<?= $result['profile_lastname']; ?>">
                        </div>
                    </div>
                    <div class="form-group">
                        <div for="" class="col-sm-3 control-label">Company's Name: </div>
                        <div class="col-sm-9">
                            <input type="text" name="profile_legalname" class="form-control" value="<?= $result['profile_legalname']; ?>">
                        </div>
                    </div>
                    <div class="form-group">
                        <div for="" class="col-sm-3 control-label">Country: </div>
                        <div class="col-sm-9">
                            <?php draw_countryform_part_sel(array(
                                    'name'=>'profile_country', 
                                    'class'=>'form-control', 
                                    'disabled'=>false, 
                                    'required'=>false, 
                                    'sel_val' =>$result['profile_country'])); ?>
                        </div>
                    </div>
                    <div class="form-group">
                        <div for="" class="col-sm-3 control-label">Address: </div>
                        <div class="col-sm-9">
                            <input type="text" name="profile_address" class="form-control" value="<?= $result['profile_address']; ?>">
                        </div>
                    </div>
                    <div class="form-group">
                        <div for="" class="col-sm-3 control-label">City: </div>
                        <div class="col-sm-9">
                            <input type="text" name="profile_city" class="form-control" value="<?= $result['profile_city']; ?>">
                        </div>
                    </div>
                    <div class="form-group">
                        <div for="" class="col-sm-3 control-label">Province/State: </div>
                        <div class="col-sm-9">
                            <?php draw_stateform_part_sel(array(
                                    'name'  => 'profile_state', 
                                    'class' => 'form-control', 
                                    'disabled' => false, 
                                    'required' => false, 
                                    'sel_val'  => $result['profile_state'], 
                                    'country'  => $result['profile_country'] )); ?>
                        </div>
                    </div>
                    <div class="form-group">
                        <div for="" class="col-sm-3 control-label">Zip: </div>
                        <div class="col-sm-9">
                            <input type="text" name="profile_zip" class="form-control" value="<?= $result['profile_zip']; ?>">
                        </div>
                    </div>


                    <div class="form-group">
                        <div class="col-sm-9 col-sm-offset-3">
                            <input type="submit" class="btn btn-primary subuser-form-btn" value="Create" name="op" id="edit-submit">
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
</form>
