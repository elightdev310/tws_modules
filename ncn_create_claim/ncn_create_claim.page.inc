<?php

//------------------------------------------------------------------------------
function fm_draw_date($title, $name, $required)
{
    if(!isset($_POST[$name])){
        $_POST[$name] = array(array(),array(),array());
    }
    $month_list = array("January","February","March","April","May","June","July","August","September","October","November","December");

    $yd = (strtotime(date("Y")."-01-01") - strtotime("2011-01-01"));
    $ydc = floor(($yd)/(60*60*24*365));
    ?>
    <tr>
        <td class="tlabel"><label><?= $title; ?> : <?php if ($required == true) print '<font color=red>*</font>'; ?></label></td>
        <td class="style11"><select name="<?= $name; ?>[]">
                <option <?php if (!isset($_POST[$name][0]) || $_POST[$name][0] == "") print 'selected' ?>></option>
                <?php foreach($month_list as $month){ ?>
                    <option <?php echo ($_POST[$name][0] == $month) ? 'selected' : '' ;?>><?php echo $month;?></option>
                <?php }?>
            </select>
            <select name="<?= $name; ?>[]">
                <option <?php if (!isset($_POST[$name][1]) || $_POST[$name][1] == "") print 'selected' ?>></option>
                <?php for ($i = 1; $i <= 31; $i++) {?>
                    <option  <?php if ($_POST[$name][1] == $i) print 'selected' ?>><?= $i; ?></option>
                <?php }?>
            </select>
            <select name="<?= $name; ?>[]">
                <option <?php if (!isset($_POST[$name][2]) || $_POST[$name][2] == "") print 'selected' ?>></option>
                <?php for($y=0;$y<=$ydc;$y++) { $yr_val = (2011+$y); ?>
                    <option <?php if ($_POST[$name][2] == $yr_val) print 'selected';?>><?php echo $yr_val;?></option>
                <?php } ?>
            </select></td>
    </tr>

<?php
}

function fm_draw_leaduser($title, $name, $required)
{
    $query = "SELECT usr_lead.*, usr.* FROM users_lead as usr_lead INNER JOIN users as usr  WHERE usr.uid = usr_lead.user_id AND usr.status != '0'";
    $result = db_query($query);
    $row_count = $result->rowCount();
    ?>
    <div class="form-group">
        <div class="col-sm-12 text-right">
            <span class="control-label"><?= $title; ?></span> : 
            <select name="<?= $name; ?>" class="form-control form-inline-control">
                <option <?php if (!isset($_POST[$name][0]) || $_POST[$name][0] == "") print 'selected' ?>></option>
                <?php
                foreach ($result as $row) {
                    $row = (array)$row;
                    $selected = "";
                    $select_id = 0;
                    if ($select_id == $row['uid']) {
                        $selected = "selected";
                    }
                    print '<option value="' . $row['uid'] . '" ' . $selected . ' >' . $row['lead_id'] . '</option>';
                }
                ?>
            </select>
        </div>
    </div>

<?php
}

//------------------------------------------------------------------------------
function draw_countryform_part($params)
{
    $profile_country = (isset($_POST[$name]) && trim($_POST[$name]) != '' ? $_POST[$name] : '');
    $params['sel_val'] = $profile_country;
    draw_countryform_part_sel($params);
}
function draw_countryform_part_sel($params)
{
    echo ncn_loc_country(array(
        'name'      => $params['name'], 
        'id'        => 'ncn-loc-country', 
        'state_id'  => 'ncn-loc-state', 
        'class'     => $params['class'], 
        'disabled'  => $params['disabled'], 
        'required'  => $params['required'], 
        'country'   => $params['sel_val'], 
    ));
}

function draw_stateform_part($params)
{
    $profile_state = (isset($_POST[$name]) && trim($_POST[$name]) != '' ? $_POST[$name] : '');
    $params['sel_val'] = $profile_state;
    $params['country'] = 'US';
    draw_stateform_part_sel($params);
}

function draw_stateform_part_sel($params)
{
    echo ncn_loc_state(array(
        'name'      => $params['name'], 
        'id'        => 'ncn-loc-state', 
        'class'     => $params['class'], 
        'disabled'  => $params['disabled'], 
        'required'  => $params['required'], 
        'state'     => $params['sel_val'], 
        'country'   => $params['country'], 
    ));
}

//------------------------------------------------------------------------------
function fm_draw_countryfield($title, $name, $disabled = false, $required = false)
{

    ?>
    <tr>
        <td class="tlabel"><label><?= $title; ?>
                : <?php if ($required == true) print '<font color=red>*</font>'; ?></label></td>
        <td>
            <?php draw_countryform_part(array(
                            'name'=>$name, 
                            'class'=>'', 
                            'disabled'=>$disabled, 
                            'required'=>$required)); ?>
        </td>
    </tr>

<?php
}

function fm_draw_statefield($title, $name, $disabled = false, $required = false)
{

    ?>
    <tr>
        <td class="tlabel"><label><?= $title; ?>
                : <?php if ($required == true) print '<font color=red>*</font>'; ?></label></td>
        <td>
            <?php draw_stateform_part(array(
                            'name'  => $name, 
                            'class' => '', 
                            'disabled' => $disabled, 
                            'required' => $required )); ?>
        </td>
    </tr>

<?php
}

//------------------------------------------------------------------------------
function fm_draw_textfield($title, $name, $disabled = false, $required = false)
{
    ?>
    <tr>
        <td class="tlabel"><label><?= $title; ?>
                : <?php if ($required == true) print '<font color=red>*</font>'; ?></label></td>
        <td><input type="text" name="<?= $name; ?>" id="fm_<?= $name; ?>" autocomplete="off"
                   value="<?= isset($_POST[$name])?$_POST[$name]:''; ?>" <?php if ($disabled == true) print ' disabled="disabled"'; ?>></td>
    </tr>
<?php
}

function fm_draw_checkbox($title, $name, $disabled = false, $required = false)
{
    ?>
    <tr>
        <td class="tlabel"><input type="checkbox" name="<?= $name; ?>" id="fm_<?= $name; ?>"
                                  autocomplete="off" <?php if ($disabled == true) print ' disabled="disabled"'; ?>></td>
        <td><label
                for="fm_<?= $name; ?>"><?= $title; ?> <?php if ($required == true) print '<font color=red>*</font>'; ?></label>
        </td>
    </tr>
<?php
}

//------------------------------------------------------------------------------
// show the lhs progress tick list
function ncn_create_claim_get_progress_list($active_page)
{
    GLOBAL $ncn_create_claim_titles;

    // start processing
    ob_start();

    print '<ul class="create_claim_lhs_list">';

    // run through options
    $other_class = "previous";

    for ($i = 0; $i < sizeof($ncn_create_claim_titles); $i++) {
        if ($ncn_create_claim_titles[$i] != '') {

            if ($i == $active_page) {
                print "<li class='active'>" . $ncn_create_claim_titles[$i] . "</li>";
                $other_class = "inactive";
            } else {
                print "<li class='" . $other_class . "'>" . $ncn_create_claim_titles[$i] . "</li>";
            }
        }
    }

    print '</ul>';

    // get and return contents
    $content = ob_get_contents();
    ob_end_clean();

    return ($content);
}
