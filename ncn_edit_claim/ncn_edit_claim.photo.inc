<?php 
//------------------------------------------------------------------------------
function ncn_edit_claim_handler()
{
GLOBAL $custom_theme;

    $claim_id = arg(2);
    $room_choose_type = arg(3);
    $room_choose_value = arg(4);
    if ($room_choose_type == 'tab') {
    if (!$room_choose_value) { $room_choose_value = 1; }
    } else {
    if (!$room_choose_value) { $room_choose_value = 1; $room_choose_type = 'tab'; }
    }
  
    //$custom_theme = "ncn_account";
    $GLOBALS['in_edit'] = true;

    if (isset($_POST['tfunction']))
    {
        ncn_edit_submit($claim_id);
        exit;
    }
    
    // add the css file (always required for this)
    drupal_add_css(drupal_get_path('module', 'ncn_create_claim').'/ncn_create_claim.css', 'module', 'all', FALSE);
    drupal_add_css(drupal_get_path('module', 'ncn_create_claim') . '/tabs/tabs.css', 'module', 'all', FALSE);
    drupal_add_js(drupal_get_path('module', 'ncn_create_claim') . '/tabs/tabs.js');

    // load customer name
    $_SESSION['claim'][$claim_id]['#data']['claim_id'] = $claim_id;
    $_SESSION['claim'][$claim_id]['customer_name'] = ncn_cd($claim_id, 'customer_name', true);

    $_SESSION['claim_log'][$claim_id] = "";

    // load data
    // $query = "SELECT * FROM claims_data WHERE claim_id=".$claim_id." AND `key`='scope';";
    // $result = db_query('SELECT * FROM {claims_data} WHERE `claim_id`=:a AND `key`=:b',array(':a'=>$claim_id,':b'=>'scope'));
    // $row_count = $result->rowCount();
    $row = ncn_cd($claim_id, 'scope', true);
    if (!empty($row)) {
        // load images  
        //$row = $result->fetchAssoc();
        $_SESSION['claim'][$claim_id]['scope'] = $row;
    } else {
        $room_name = preg_replace("/[^a-zA-Z0-9\s]/", "", $_SESSION['claim'][$claim_id]['customer_name']['val']);
        
        unset($_SESSION['claim'][$claim_id]['scope']);
        $_SESSION['claim'][$claim_id]['scope'][$room_name] = array();
    }
    
    drupal_set_title('Edit Album');
    return theme('ncn_add_room',array("claim_id"=>$claim_id,"room_choose_type"=>$room_choose_type,"room_choose_value"=>$room_choose_value));                // ncn_crete_claim/ncn_add_room.tpl.php

    
//  return drupal_get_form('ncn_editform');

}

//------------------------------------------------------------------------------
function ncn_edit_submit($claim_id)
{

    ncn_save_claim($claim_id, true);

//  unset($_SESSION['claim']);

    ?>
    <html>
    <body onload="parent.jQuery.colorbox.close();">
    </body>
    </html>
    <?php

}

//------------------------------------------------------------------------------
function ncn_editform($form_state)
{

    // get variables
    $claim_id = arg(2);
    $step = arg(3);
    
    
    // Access log settings:
    $options = array('1' => t('Enabled'), '0' => t('Disabled'));
    
    // setup form
    $form = array();
    $form['#attributes'] = array('enctype' => "multipart/form-data");
    
    $form['claim_id'] = array('#type' => 'hidden', '#value' => $claim_id); 
    $form['step'] = array('#type' => 'hidden', '#value' => $step); 

    switch ($step)
    {
        case "1":
            $form = get_formpart_step_1($form);
        break;

        case "2":
            $form = get_formpart_step_2($form);
        break;

        case "3":
            $form = get_formpart_step_3($form);
        break;

        case "4":
            $form = get_formpart_step_4($form);
        break;
    }
            
    $form['next'] = array('#type' => 'submit', '#weight' => 101, '#value' => t('Submit'));
    
    ncn_edit_form_prepopulate($claim_id, $form);
    
    return $form;
}


//------------------------------------------------------------------------------
function ncn_edit_form_prepopulate($claim_id, &$form)
{

    foreach ($form as $field_name => $f)
    {
        if (isset($f['#type']))
        {
            if ($f['#type'] == "fieldset")
            {
                foreach ($f as $field_name2 => $f2)
                {
                    if (is_array($f2))
                    {
                        // prepopulate
                        $form[$field_name][$field_name2]['#default_value'] = ncn_cd($claim_id, $field_name2); 
                    }
                }
            }
            else
            {
                // prepopulate
                $form[$field_name]['#default_value'] = ncn_cd($claim_id, $field_name); 
            }
        }
    }

//  echo "<hr>";
//  echo "<pre>";
//  print_r($form);
//  echo "</pre>";

}

//------------------------------------------------------------------------------
function ncn_editform_submit($form, &$form_state)
{
    $claim_id = arg(2);
    

    // insert claim data
    foreach ($_SESSION['claim'][$claim_id] as $key=>$val)
    {
        // fix up some of the data
        $res = serialize($val['val']);
    
        // update the data
    //  $query = "DELETE FROM claims_data WHERE `claim_id`=".$claim_id." AND `key`=\"".mysql_real_escape_string($key)."\";";
        $result = db_query('DELETE FROM {claims_data} WHERE claim_id=:a AND `key`=:b',
            array(':a'=>$claim_id,':b'=>mysql_real_escape_string($key)));
        
    //  $query = "INSERT INTO claims_data VALUES(".$claim_id.", \"".mysql_real_escape_string($val['type'])."\", \"".mysql_real_escape_string($key)."\", \"".mysql_real_escape_string($res)."\");";
        $result = db_query('INSERT INTO {claims_data} VALUES(:a,:b,:c,:d)',
            array(':a'=>$claim_id,':b'=>mysql_real_escape_string($val['type']),
                ':c'=>mysql_real_escape_string($key),':d'=>mysql_real_escape_string($res)));
    }
    
    // update the log file
    $log_message = t("Claim #!claim_id updated by !user_name.", array('!claim_id'=>$claim_id, '!user_name'=>ncn_amin_get_user_role_and_name()));
    ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
    
    $query = "UPDATE claims SET last_modified=".date('U').",claim_status='in progress' WHERE claim_id=".$claim_id;
//  $result = db_query('UPDATE {claims} SET last_modified=:a, claim_status=:b WHERE claim_id=:c',
//    array(':a'=>date('U'),':b'=>in progress,':c'=>$claim_id));
    $result = db_query($query);

    // show code to close colorbox window
    ?>
    <html>
    <body onload="parent.$.colorbox.close();">
    </body>
    </html>
    <?php
}

function ncn_editform_validate($form, &$form_state)
{
    ncn_createform_validate($form, $form_state);
}

function ncn_edit_claim_preprocess(&$vars)
{
}

