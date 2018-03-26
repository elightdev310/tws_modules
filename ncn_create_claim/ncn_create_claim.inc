<?php
/**
 * URL: account/create-new-claim.html
 */
function ncn_create_claim_page()
{
    // render the form
    drupal_add_css(drupal_get_path('module', 'ncn_create_claim') . '/ncn_create_claim.css');
    drupal_add_js(drupal_get_path('module', 'ncn_create_claim') . '/ncn_create_claim.js');
    return theme('ncn_create_claim');
}
// preprocess function for "ncn_create_claim.tpl.php"
function template_preprocess_ncn_create_claim(&$vars)
{
    $GLOBALS['ncn_create_claim_titles'] = array(
        'Create New Claim',
        'Terms and Conditions',
        '',
        'Finished'
    );

    // get page num to index into
    $pagenum = (isset($_REQUEST['page']) ? $_REQUEST['page'] : 1) ;

    if (!is_numeric($pagenum)) {
        $pagenum = 1;
    }

    $pres = false;
    // run tfunction (post)
    if (isset($_POST['tfunction'])) {
        // switch the pages
        switch ($pagenum) {
            // page #1
            default:
                $pres = create_claim_page1_submit();
                break;
            // page #2
            case "2":
                $pres = create_claim_page2_submit();
                break;
            // page #3
            case "3":
                $pres = create_claim_page3_submit();
                break;
            // page #4
            case "4":
                $pres = create_claim_page4_submit();
                break;
            // page #5
            case "5":
                $pres = create_claim_page5_submit();
                break;
        }

        // there were no errors - move to next page
        if ($pres == true) {
            Header("Location:" . $GLOBALS['base_url'] . "/account/create-new-claim.html?page=" . ($pagenum + 1));
            exit;
        }
    }


    // --- draw specific pages ---
    ob_start();

    switch ($pagenum) {
        default:
        case "1":
            // Creating Form
            $vars['page_class'] = "create-claim-ccpage";
            drupal_set_title('Create new claim');
            create_claim_page1();
            break;
        case "2":
            $vars['page_class'] = "claim-confirm-ccpage";
            print theme('ncn_claim_confirm');
            break;
        case "3":
            create_claim_page3();
            break;
        case "4":
            if (!isset($_SESSION['claim']['scope'])) {
                $room_name = preg_replace("/[^a-zA-Z0-9\s]/", "", $_SESSION['claim']['customer_name']['val']);

                $_SESSION['claim']['scope'][$room_name] = array();
            }
            print theme('ncn_add_room', 1);
            break;
        case "5":
            print theme('ncn_download_pdfs', 1);
            break;
        case "6":
            print theme('ncn_claim_success', 1);
            break;
    }

    $create_content = ob_get_contents();
    ob_end_clean();

    $vars['title'] = $GLOBALS['ncn_create_claim_titles'][$pagenum - 1];
    //$vars['progress_list'] = ncn_create_claim_get_progress_list($pagenum - 1);
    $vars['module_content'] = $create_content;
}

//------------------------------------------------------------------------------
// preprocess function for 
function template_preprocess_ncn_claim_confirm(&$vars)
{
    global $user;
    $puser = user_load($user->uid);
    $member_type = get_member_type($puser);
    $vars['member_type'] = $member_type;
}

//------------------------------------------------------------------------------
// preprocess function 
function template_preprocess_ncn_claim_success(&$vars)
{
}


//------------------------------------------------------------------------------
// first page in multipart form
function create_claim_page1()
{
    // reset claim session
    $_SESSION['claim'] = array();
    @include get_realpath('ncn_create_claim', 'ui/create_claim_form.php');
}


//------------------------------------------------------------------------------
// first sybmit in multipart form
function create_claim_page1_submit($P_DATA = array())
{

    if (empty($P_DATA)) {
        $P_DATA = $_POST;
    }

    // check for errors
    /*if ($P_DATA['lead_user'] == '') {
        drupal_set_message("Please select Lead user", 'error');
        return FALSE;
    }*/
    if ($P_DATA['date_of_loss'] == '') {
        drupal_set_message("Please input Date of Loss", 'error');
        return FALSE;
    } else {
        $date_of_loss = strtotime($P_DATA['date_of_loss']);
        $P_DATA['date_of_loss'] = array(
            date('F', $date_of_loss), 
            date('j', $date_of_loss), 
            date('Y', $date_of_loss), 
        );
    }

    $P_DATA['customer_name'] = ncn_admin_remove_special_char_from_customer_name($P_DATA['customer_name']);

    if (strlen($P_DATA['customer_name']) < 5) {
        drupal_set_message("Insured's Name must be at least 5 characters", 'error');
        return FALSE;
    }

    if (strlen($P_DATA['insured_address']) < 8) {
        drupal_set_message("Insured's Address must be at least 8 characters", 'error');
        return FALSE;
    }

    if (strlen($P_DATA['insured_city']) < 3) {
        drupal_set_message("Insured's City must be at least 3 characters", 'error');
        return FALSE;
    }

    if (strlen($P_DATA['insured_state']) < 2) {
        drupal_set_message("Insured's State must be at least 2 characters", 'error');
        return FALSE;
    }

    if (strlen($P_DATA['insured_zip']) < 5) {
        drupal_set_message("Insured's Zip code must be at least 5 characters", 'error');
        return FALSE;
    }

    if (strlen($P_DATA['insured_phone_number']) < 10) {
        drupal_set_message("Insured's Phone Number is not valid.", 'error');
        return FALSE;
    }

    /*if (strlen($P_DATA['technician_service_name']) < 5) {
        drupal_set_message("Technician service name must be at least 5 characters", 'error');
        return FALSE;
    }*/

    if ($P_DATA['claim_type'] == '') {
        drupal_set_message("You did not select residential/commercial", 'error');
        return FALSE;
    }

    if ($P_DATA['claim_product'] == '') {
        drupal_set_message("You did not select product", 'error');
        return FALSE;
    }

    /*if (!is_numeric($P_DATA['product_id']))
    {   drupal_set_message("You did not select residential/commercial", 'error');return FALSE;  }*/


    GLOBAL $user;
    $user = user_load($user->uid);
    $user_id = $user->uid;

    if (is_subuser($user)) {
        $parent_mid = ncn_subuser_get_memberid_from_subuid($user_id);
        $user_id = get_uid_from_memberid($parent_mid);
    }

    $puser = user_load($user_id);
    $member_type = get_member_type($puser);


    $product_id = ncn_create_claim_get_product_id($member_type, $P_DATA['claim_type'], $P_DATA['claim_product']);
    if ($product_id == 0) {
        drupal_set_message("When creating a claim, charging has an error of unknown product", 'error');
        return FALSE;
    }

    $_SESSION['claim_type'] = $P_DATA['claim_type'];
    $_SESSION['claim_product'] = $P_DATA['claim_product'];
    $_SESSION['product_id'] = $product_id;
    $_SESSION['lead_user'] = $P_DATA['lead_user'];

    ncn_save_session_var($P_DATA, 'lead_user', 0);
    ncn_save_session_var($P_DATA, 'date_of_loss', 0);
    ncn_save_session_var($P_DATA, 'customer_name', 0);
    ncn_save_session_var($P_DATA, 'insured_country', 0);
    ncn_save_session_var($P_DATA, 'insured_address', 0);
    ncn_save_session_var($P_DATA, 'insured_city', 0);
    ncn_save_session_var($P_DATA, 'insured_state', 0);
    ncn_save_session_var($P_DATA, 'insured_zip', 0);
    ncn_save_session_var($P_DATA, 'insured_phone_number', 0);
    ncn_save_session_var($P_DATA, 'technician_service_name', 0);
    //ncn_save_session_var($P_DATA, 'product_type', 0);

    ncn_save_session_var($P_DATA, 'expedite', 0);
    return TRUE;
}



/*function get_claim_type($product_id) {
    $claim_type = '';
    if ($product_id==18 || $product_id==45) {
        $claim_type = 'residential';
    } else if ($product_id==30 || $product_id==46) {
        $claim_type = 'commercial';
    } else if ($product_id==53 || $product_id==54) {
        $claim_type = 'reconstruction';
    }
    
    return $claim_type;
}*/

//------------------------------------------------------------------------------
// jump to cart from t&c
function create_claim_page2_submit()
{
    $claim_id = ncn_create_claim_create_new_claim();

    // check for expired session
    if ((!isset($_SESSION['claim'])) || (!isset($_SESSION['product_id']))) {
        drupal_set_message('Sorry, your session has expired.', 'error');
        Header("Location:1");
    }

    $claim_id = $_SESSION['claim']['id'];
    $claim_desc = $_SESSION['claim']['customer_name']['val'] . ", " . $_SESSION['claim']['insured_zip']['val'];

    $url = $GLOBALS['base_url'] . "/cart/add/e-p" . $_SESSION['product_id'] . "_q1_m" . $claim_id . "_a1o" . $claim_id . "_a2o" . $claim_desc . "?destination=cart/checkout";

    Header('Location:' . $url);
    exit;
}

/**
 * create a new claim (action)
 */
function ncn_create_claim_create_new_claim($redirect = TRUE)
{
    global $user;
    $user = user_load($user->uid);
    $user_id = $user->uid;

    if (is_subuser($user)) {
        $parent_mid = ncn_subuser_get_memberid_from_subuid($user_id);
        $user_id = get_uid_from_memberid($parent_mid);
    }

    $puser = user_load($user_id);

    $claim_type = $_SESSION['claim_type'];
    $claim_product = $_SESSION['claim_product'];
    $claim_lead = $_SESSION['lead_user'];


    $claim_id = ncn_save_claim(0, false, $claim_type, $claim_product, $claim_lead);

    // NCN Gold First Special
    if (ncn_admin_is_member_first_free($puser->profile_memberid) && ncn_admin_member_first_free_month_passed($puser->profile_memberid) == FALSE) {
        if (ncn_admin_get_member_first_free_claim_id($puser->profile_memberid) == 0) {
            if (ncn_admin_set_member_first_free_claim($puser->profile_memberid, $claim_id)) {
                ncn_admin_set_member_first_free_claim_product_id($claim_id, $_SESSION['product_id']);

                // same as uc_authorizenet_sim module
                // $query = "UPDATE claims SET claim_status='incomplete' WHERE claim_id=" . $claim_id;
                $result = db_query('UPDATE {claims} SET claim_status=:a WHERE claim_id=:b',array(':a'=>'incomplete',':b'=>$claim_id));
                ncn_create_claim_send_mail($claim_id);
                if ($redirect) {
                    drupal_goto("account/claims.html");
                    exit;
                } else {
                    return TRUE;
                }
            }
        }
    } // NCN Gold Member Demo
    else if (ncn_admin_is_gold_member_demo($puser->profile_memberid)) {
        ncn_admin_check_gold_member_demo_expired($puser->profile_memberid);
        if (!ncn_admin_is_gold_member_demo_expired($puser->profile_memberid)) {
            ncn_admin_insert_free_claim_for_gmd($puser->uid, $claim_id, $_SESSION['product_id']);

            ncn_admin_check_gold_member_demo_expired($puser->profile_memberid);
            // same as uc_authorizenet_sim module
            // $query = "UPDATE claims SET claim_status='incomplete' WHERE claim_id=" . $claim_id;
            $result = db_query('UPDATE {claims} SET claim_status=:a WHERE claim_id=:b',array(':a'=>'incomplete',':b'=>$claim_id));
            ncn_create_claim_send_mail($claim_id);
            if ($redirect) {
                drupal_goto("account/claims.html");
                exit;
            } else {
                return TRUE;
            }
        }
    }

    return $claim_id;
}

//------------------------------------------------------------------------------
// second page in multipart form
function create_claim_page3()
{

    Header("Location:4");
    exit;

    $_POST['property_owner'] = $_SESSION['claim']['customer_name']['val'];

    // draw the form
    ?>
    <table class="create_claim_table">
    <form method="POST">
    <input type="hidden" name="tfunction" value="process">

    <?php fm_draw_textfield('Property Owner', 'property_owner', true); ?>

    <?php fm_draw_date('Date', 'claim_date', false, true); ?>

    <tr>
        <td class="style11">
            <div align="right">Time:</div>
        </td>
        <td class="style11"><select name="claim_time[]">
                <?php
                for ($i = 1; $i <= 12; $i++) {
                    ?>
                    <option value="<?= $i; ?>"><?= $i; ?></option><?php
                }
                ?>
            </select>
            <select name="claim_time[]">
                <?php
                for ($i = 0; $i < 60; $i++) {
                    ?>
                    <option value="<?= sprintf("%02d", $i) ?>"><?= sprintf("%02d", $i) ?></option><?php
                }
                ?>
            </select>
            <select name="claim_time[]">
                <option>AM</option>
                <option>PM</option>
            </select></td>
    </tr>
    <tr>
        <td class="style11">
            <div align="right"></div>
        </td>
    </tr>
    <tr>
        <td>
            <div align="right"><span class="style11">Pro<span class="style10">perty Address: </span></span></div>
        </td>
        <td><span class="style11"><span class="style10">
                <input name="property_address" type="text" value="<?= StripSlashes($_POST['property_address']) ?>"/>
              </span></span></td>
    </tr>
    <tr>
        <td>
            <div align="right"><span class="style11"><span class="style10">City: </span></span></div>
        </td>
        <td><span class="style11"><span class="style10">
                <input name="property_city" type="text" value="<?= StripSlashes($_POST['property_city']) ?>"/>
              </span></span></td>
    </tr>
    <tr>
        <td>
            <div align="right"><span class="style11">State<span class="style10">: </span></span></div>
        </td>
        <td><span class="style11"><span class="style10">
                <input name="property_state" type="text" value="<?= StripSlashes($_POST['property_state']) ?>"/>
              </span></span></td>
    </tr>
    <tr>
        <td>
            <div align="right"><span class="style11">Zip Code <span class="style10">: </span></span></div>
        </td>
        <td><span class="style11"><span class="style10">
                <input name="property_zip" type="text" value="<?= StripSlashes($_POST['property_zip']) ?>"/>
              </span></span></td>
    </tr>
    <tr>
        <td>
            <div align="right"><span class="style11">Phone: </span></div>
        </td>
        <td><span class="style11">
                <input name="property_phone[]" type="text" size="3"
                       value="<?= StripSlashes($_POST['property_phone'][0]) ?>"/>
                -
                <input name="property_phone[]" type="text" size="3"
                       value="<?= StripSlashes($_POST['property_phone'][1]) ?>"/>
                -
                <input name="property_phone[]" type="text" size="4"
                       value="<?= StripSlashes($_POST['property_phone'][2]) ?>"/>
              </span></td>
    </tr>
    <tr>
        <td>
            <div align="right"><span class="style11">Work: </span></div>
        </td>
        <td><span class="style11">
                <input name="work_phone[]" type="text" size="3" value="<?= StripSlashes($_POST['work_phone'][0]) ?>"/>
                -
                <input name="work_phone[]" type="text" size="3" value="<?= StripSlashes($_POST['work_phone'][1]) ?>"/>
                -
                <input name="work_phone[]" type="text" size="4" value="<?= StripSlashes($_POST['work_phone'][2]) ?>"/>
              </span></td>
    </tr>
    <tr>
        <td>
            <div align="right"><span class="style11">Cell: </span></div>
        </td>
        <td><span class="style11">
                <input name="cell[]" type="text" size="3" value="<?= StripSlashes($_POST['cell'][0]) ?>"/>
                -
                <input name="cell[]" type="text" size="3" value="<?= StripSlashes($_POST['cell'][1]) ?>"/>
                -
                <input name="cell[]" type="text" size="4" value="<?= StripSlashes($_POST['cell'][2]) ?>"/>
              </span></td>
    </tr>
    <tr>
        <td>
            <div align="right"><span class="style11">Fax: </span></div>
        </td>
        <td><span class="style11">
                <input name="fax[]" type="text" size="3" value="<?= StripSlashes($_POST['fax'][0]) ?>"/>
                -
                <input name="fax[]" type="text" size="3" value="<?= StripSlashes($_POST['fax'][1]) ?>"/>
                -
                <input name="fax[]" type="text" size="4" value="<?= StripSlashes($_POST['fax'][2]) ?>"/>
              </span></td>
    </tr>
    <tr>
        <td>
            <div align="right"><span class="style11">Email <span class="style10">: </span></span></div>
        </td>
        <td><span class="style11"><span class="style10">
                <input name="email_address" type="text" value="<?= StripSlashes($_POST['email_address']) ?>"/>
              </span></span></td>
    </tr>
    <tr>
        <td>
            <div align="right"></div>
        </td>
    </tr>
    <tr>
        <td>
            <div align="right"><span class="style12">Insurance Policy Information </span></div>
        </td>
    </tr>
    <tr>
        <td>
            <div align="right"></div>
        </td>
    </tr>
    <tr>
        <td class="style11">
            <div align="right">Named Beneficiary of Policy:</div>
        </td>
        <td class="style11"><input type="text" name="beneficiary" value="<?= StripSlashes($_POST['beneficiary']) ?>"/>
        </td>
    </tr>
    <tr>
        <td class="style12">
            <div align="right"></div>
        </td>
        <td class="style12">&nbsp;</td>
        <td class="style12">&nbsp;</td>
    </tr>
    <tr>
        <td class="style12">
            <div align="right">Select Type of Policy:</div>
        </td>
        <td class="style12">&nbsp;</td>
        <td class="style12">&nbsp;</td>
    </tr>
    <tr>
        <td class="style11">
            <div align="right"></div>
            <label></label>
        </td>
        <td class="style11"><label>
                <input type="checkbox" name="home_owner_policy" value="yes"/>
            </label>
            Home owners policy
            <label> <br/>
                <input type="checkbox" name="rental_property_policy" value="yes"/>
            </label>
            Rental Property policy
            <label> <br/>
                <input type="checkbox" name="forced_policy" value="yes"/>
            </label>
            Forced policy
            <label> <br/>
                <input type="checkbox" name="foreclosure_policy" value="yes"/>
                Foreclosure</label>
            policy
        </td>
    </tr>
    <tr>
        <td class="style11">
            <div align="right"></div>
        </td>
    </tr>
    <tr>
        <td class="style11">
            <div align="right">Insurance Company:</div>
        </td>
        <td class="style11"><input type="text" name="insurance_company"
                                   value="<?= StripSlashes($_POST['insurance_company']) ?>"/></td>
    </tr>
    <tr>
        <td>
            <div align="right"><span class="style11">Phone:</span></div>
        </td>
        <td><span class="style11">
                <input name="insurance_phone[]" type="text" size="3"
                       value="<?= StripSlashes($_POST['insurance_phone'][0]) ?>"/>
                -
                <input name="insurance_phone[]" type="text" size="3"
                       value="<?= StripSlashes($_POST['insurance_phone'][1]) ?>"/>
                -
                <input name="insurance_phone[]" type="text" size="4"
                       value="<?= StripSlashes($_POST['insurance_phone'][2]) ?>"/>
              </span></td>
    </tr>
    <tr>
        <td>
            <div align="right"><span class="style11">Insurance Agent: </span></div>
        </td>
        <td><span class="style11">
                <input name="insurance_agent" type="text" value="<?= StripSlashes($_POST['insurance_agent']) ?>"/>
              </span></td>
    </tr>
    <tr>
        <td>
            <div align="right"><span class="style11">Phone: </span></div>
        </td>
        <td><span class="style11">
                <input name="agent_phone[]" type="text" size="3" value="<?= StripSlashes($_POST['agent_phone'][0]) ?>"/>
                -
                <input name="agent_phone[]" type="text" size="3" value="<?= StripSlashes($_POST['agent_phone'][1]) ?>"/>
                -
                <input name="agent_phone[]" type="text" size="4" value="<?= StripSlashes($_POST['agent_phone'][2]) ?>"/>
              </span></td>
    </tr>
    <tr>
        <td>
            <div align="right"><span class="style11">Policy number : </span></div>
        </td>
        <td><span class="style11">
                <input name="policy_number" type="text" value="<?= StripSlashes($_POST['policy_number']) ?>"/>
              </span></td>
    </tr>
    <tr>
        <td>
            <div align="right"><span class="style11">Claim number : </span></div>
        </td>
        <td><span class="style11">
                <input name="claim_number" type="text" value="<?= StripSlashes($_POST['claim_number']) ?>"/>
              </span></td>
    </tr>
    <tr>
        <td>
            <div align="right"></div>
        </td>
    </tr>
    <?php fm_draw_date('Effective policy date', 'effective_date', false, true); ?>
    <tr>
        <td>
            <div align="right"><span class="style11">Deductible Amount  : </span></div>
        </td>
        <td><span class="style11">
                <input name="deductible_amount" type="text" value="<?= StripSlashes($_POST['deductible_amount']) ?>"/>
              </span></td>
    </tr>
    <tr>
        <td>
            <div align="right"></div>
        </td>
    </tr>
    <tr>
        <td>
            <div align="right"><span class="style11">Type of loss: </span></div>
        </td>
        <td><span class="style11">
                <input name="type_of_loss" type="text" value="<?= StripSlashes($_POST['type_of_loss']) ?>"/>
              </span></td>
    </tr>
    <tr>
        <td>
            <div align="right"><span class="style11">Cause and origin of loss: </span></div>
        </td>
        <td><span class="style11">
                <input name="origin" type="text" value="<?= StripSlashes($_POST['origin']) ?>"/>
              </span></td>
    </tr>
    <tr>
        <td>
            <div align="right"><span class="style11">Category of water: </span></div>
        </td>
        <td><span class="style11">
                <input name="category_of_water" type="text" value="<?= StripSlashes($_POST['category_of_water']) ?>"/>
              </span></td>
    </tr>
    <tr>
        <td>
            <div align="right"><span class="style11">Class of loss: </span></div>
        </td>
        <td><span class="style11">
                <input name="class_of_loss" type="text" value="<?= StripSlashes($_POST['class_of_loss']) ?>"/>
              </span></td>
    </tr>
    <tr>
        <td>
            <div align="right"><span class="style11">Number of stories affected : </span></div>
        </td>
        <td><span class="style11">
                <input name="stories_affected" type="text" value="<?= StripSlashes($_POST['stories_affected']) ?>"/>
              </span></td>
    </tr>
    <tr>
        <td>
            <div align="right"><span class="style11">Comments: </span></div>
        </td>
        <td><span class="style11">
                <input name="comments" type="text" value="<?= StripSlashes($_POST['comments']) ?>"/>
              </span></td>
    </tr>


    <tr>
        <td>&nbsp;</td>
        <td><input type="submit" value="Submit"></td>
    </tr>

    </form>
    </table>
<?php

}

//------------------------------------------------------------------------------
// second submit in multipart form
function create_claim_page3_submit()
{

    if ($_POST['property_owner'] != '') {
        ncn_save_session_var($_POST, 'property_owner', 1);
    }

    foreach ($_POST as $key => $val) {
        ncn_save_session_var($_POST, $key, 1);
    }


    return true;
}

//------------------------------------------------------------------------------
// add room page (#3)
function create_claim_page4()
{
    // draw the form
    ?>
    <table class="create_claim_table">
        <form method="POST">
            <input type="hidden" name="tfunction" value="process">


            <tr>
                <td>&nbsp;</td>
                <td><input type="submit" value="Submit"></td>
            </tr>

        </form>
    </table>
<?php

}

//------------------------------------------------------------------------------
// add room submit (#3)
function create_claim_page4_submit()
{
    // nothing to do (all images are handled by ajax)
    return true;
}


//------------------------------------------------------------------------------
// (#4)
function create_claim_page5()
{
    print ":o == 4";

}

//------------------------------------------------------------------------------
// (#4)
function create_claim_page5_submit()
{

    // save the claim
    if ((isset($_SESSION['claim']['id'])) && (is_numeric($_SESSION['claim']['id']))) {
        echo "<pre>";print_r(2);exit;
        ncn_save_claim($_SESSION['claim']['id'], true);
        unset($_SESSION['claim']);
    } else {
        print 'Your session has expired.';
        exit;
    }

    return true;
}

//------------------------------------------------------------------------------
// (#5)
function create_claim_page6()
{
    print ":o == 5";

}

//------------------------------------------------------------------------------
// (#5)
function create_claim_page6_submit()
{

//  unset($_SESSION['claim']);

    return true;
}

//------------------------------------------------------------------------------
// used in validation function (below)
function ncn_process_file_upload($form_state, $fieldName, $file_desc)
{

    $form_id = $form_state['values']['form_build_id'];
    $file_data = $_SESSION['files']['upload_element'][$form_id][$fieldName];

    // check file was uploaded
    if (sizeof($file_data) == 0) {
        return;
    }

    // check file was discarded
    if ($file_data->submit_action != 1) {
        return;
    }

    // file was uploaded
    upload_element_save($file_data, 'sites/default/files/user_files', FILE_EXISTS_RENAME);

    return ($file_data);

}

function remove_unnecessary_file($claim_id)
{
    /* Add by 2011-03-26 */
    if (!isset($_SESSION['claim'][$claim_id]['scope'])) {
        return;
    }
    foreach ($_SESSION['claim'][$claim_id]['scope'] as $key => $room) {
        foreach ($room as $image_id => $image_info) {
            if (isset($image_info['resize_image_path'])) {
                $path = $image_info['resize_image_path'];
                $path->uri  = str_replace("public://","public://styles/photo_album/public/",$path->uri);
                $res = file_move($path, $image_info['path'], FILE_EXISTS_REPLACE);
                unset($_SESSION['claim'][$claim_id]['scope'][$key][$image_id]['resize_image_path']);
            }
        }
    }
}

//------------------------------------------------------------------------------
function ncn_save_claim($claim_id = 0, $update = false, $claim_type = '', $claim_product = '', $claim_lead = '')
{
    GLOBAL $user, $base_url;
    $user = user_load($user->uid);
    $user_id = $user->uid;

    if (is_subuser($user)) {
        $parent_mid = ncn_subuser_get_memberid_from_subuid($user_id);
        $user_id = get_uid_from_memberid($parent_mid);
        drupal_set_message($user_id);
    }

    // only do new if not set
    if ($update == false) {
        $expedite = 0;
        if ($_SESSION['claim']['expedite']['val'] == 'on') {
            $expedite = 1;
        }

        // insert claim definition
        if($claim_lead=='') $claim_lead=0;

        $claim_id = db_insert('claims')
                 ->fields(array('user_id' => $user_id,
                                'claim_type' => $claim_type,
                                'claim_product' => $claim_product,
                                'created'=>date("U"),
                                'last_modified'=>date("U"),
                                'claim_amount'=>0,
                                'payment_received'=>0,
                                'claim_status'=>'unpurchased',
                                'claim_status_message'=>'',
                                'file_note'=>'',
                                'workflow'=>'',
                                'expedite'=>$expedite,
                                'deleted'=>0,
                                'leaduser'=>$claim_lead, 
                                'ars'     =>0, 
                                'ars_file_status' => ''))
                 ->execute();
        $_SESSION['claim']['id'] = $claim_id;
    }

    remove_unnecessary_file($claim_id);

    // insert claim data
    $scope_data = array();
    if ($update) {
        $scope_data = $_SESSION['claim'][$claim_id];
    } else {
        $scope_data = $_SESSION['claim'];
    }
    foreach ($scope_data as $key => $val) {
        if ($key == '#data') {
            continue;
        }
        // fix up some of the data
        $res = serialize($val);

        if (isset($val['weight']) && !is_numeric($val['weight'])) {
            @$val['weight'] = 0;
        }

        // remove old data
        $result = db_query("DELETE FROM {claims_data} WHERE claim_id=:a AND `key`=:b",array(':a'=>$claim_id,':b'=>$key));

        // insert new data
        $cfType = isset($val['type']) ? $val['type'] : '';
        $cfWeight = isset($val['weight']) ? $val['weight'] : 0;

        $result = db_query("INSERT INTO {claims_data} (`claim_id`,`field_type`,`key`,`val`,`weight`) VALUES (:a,:b,:c,:d,:e) ",
            array(':a'=>$claim_id,':b'=>$cfType,':c'=>$key,':d'=>$res,':e'=>$cfWeight));
    }

    // update the log file
    if ($update == true) {
        if (isset($_SESSION['claim_log'][$claim_id]) && $_SESSION['claim_log'][$claim_id] != "") {
            ncn_admin_insert_claim_log($claim_id, date('U'), $_SESSION['claim_log'][$claim_id]);
            $_SESSION['claim_log'][$claim_id] = "";
            unset($_SESSION['claim_log'][$claim_id]);
        }
        /*else {
                $log_message = t("Claim #!claim_id updated by !user_name.", array('!claim_id'=>$claim_id, '!user_name'=>ncn_amin_get_user_role_and_name()));
                ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
            }*/

        if (isset($_SESSION['claim_room_rename_actions'][$claim_id])) {
            if (is_array($_SESSION['claim_room_rename_actions'][$claim_id])) {
                foreach ($_SESSION['claim_room_rename_actions'][$claim_id] as $rename_action) {
                    ncn_admin_update_room_data_by_renaming_room($claim_id, $rename_action['old_name'], $rename_action['new_name']);
                }
            }
            unset($_SESSION['claim_room_rename_actions'][$claim_id]);
        }
    }

    // send notification
    if ($update == false) {

    }

    return ($claim_id);
}

function ncn_create_claim_send_mail($claim_id)
{
    GLOBAL $user, $base_url;
    $user = user_load($user->uid);
    $user_id = $user->uid;

    if (is_subuser($user)) {
        $parent_mid = ncn_subuser_get_memberid_from_subuid($user_id);
        $user_id = get_uid_from_memberid($parent_mid);
    }

    $user_details = user_load($user_id);

    $log_message = t("Claim #!claim_id created by !user_name.", array('!claim_id' => $claim_id, '!user_name' => ncn_amin_get_user_role_and_name($user_id)));
    // Claim Information
    $log_message .= t("\r\n Insured's name: !insured_name", array('!insured_name' => ncn_cd($claim_id, 'customer_name')));
    $log_message .= t("\r\n Date of Loss: !date_of_loss", array('!date_of_loss' => ncn_cd($claim_id, 'date_of_loss')));
    $log_message .= t("\r\n Country: !insured_country", array('!insured_country' => ncn_cd($claim_id, 'insured_country')));
    $log_message .= t("\r\n Address: !insured_address", array('!insured_address' => ncn_cd($claim_id, 'insured_address')));
    $log_message .= t("\r\n City: !insured_city", array('!insured_city' => ncn_cd($claim_id, 'insured_city')));
    $log_message .= t("\r\n State: !insured_state", array('!insured_state' => ncn_cd($claim_id, 'insured_state')));
    $log_message .= t("\r\n Zip Code: !insured_zip", array('!insured_zip' => ncn_cd($claim_id, 'insured_zip')));
    $log_message .= t("\r\n Insured's Phone Number: !insured_phone_number", array('!insured_phone_number' => ncn_cd($claim_id, 'insured_phone_number')));
    $log_message .= t("\r\n Technician Service Name: !technician_service_name", array('!technician_service_name' => ncn_cd($claim_id, 'technician_service_name')));

    $claim_info = get_claim_data($claim_id);
    $claim_type = ucwords($claim_info['claim_type']) . ' ' . $claim_info['claim_product'];
    if (ncn_cd($claim_id, 'expedite')) {
        $claim_type .= " (EXPEDITE)";
    }
    $log_message .= t("\r\n Claim Type: !claim_type", array('!claim_type' => $claim_type));

    ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);

    // email the user with access details
    $params = array(
        '!claim_id' => $claim_id,
        '!legalname' => $user->profile_legalname,
        '!firstname' => $user->profile_firstname,
        '!lastname' => $user->profile_lastname,
    );
    // send to customer
    //drupal_mail('ncn_create_claim', 'new_claim_customer', $user_details->mail, language_default(), $params);
    if ($user->uid != $user_id) { // mail to sub user
        if (!empty($user->mail)) {
            drupal_mail('ncn_create_claim', 'new_claim_customer', $user->mail, language_default(), $params);
        }
    }

    // HERE
    if (variable_get('ncn_new_claim_active', '') == 1) {
        // send to rep (first get rep from pool table)
    //    $query = "SELECT * FROM member_id_pool WHERE member_id=\"" . AddSlashes($user_details->profile_memberid) . "\";";
        $result = db_query('SELECT * FROM {member_id_pool} WHERE member_id=:a',array(':a'=>$user_details->profile_memberid));
        $row_count = $result->rowCount();
        $row = $result->fetchAssoc();

        $am = user_load($row['am_uid']);
        if (!$am->mail) {
            drupal_mail('ncn_create_claim', 'new_claim', $am->mail, language_default(), $params);
        }

        /*$query = "SELECT parent FROM users_parent WHERE uid=".$row['owner'];
        $result = db_query($query);
        $row_count = $result->rowCount();
        for ($i=0; $i<$row_count; $i++) {
            $row = (array)$result;

            $this_user = user_load($row['parent']);     // load the sales rep
            if (!$this_user) {
                drupal_mail('ncn_create_claim', 'new_claim', $this_user->mail, language_default(), $params);
            }
        }*/

        // send to admin
        $this_user = user_load(1); // load the admin
        if (!$this_user) {
            drupal_mail('ncn_create_claim', 'new_claim', $this_user->mail, language_default(), $params);
        }

    }
}

/**
 * Ajax Handler (ajax/ncn_create_claim/get_claim_invoice)
 */
function ncn_create_claim_display_price()
{
    $j_result['msg'] = '';
    $j_result['code'] = "success";
    $j_result['price'] = '';

    global $user;
    $puser = user_load($user->uid);
    $member_type = get_member_type($puser);

    $product_id = ncn_create_claim_get_product_id($member_type, $_POST['claim_type'], $_POST['claim_product']);
    if ($product_id != 0) {
        $p_node = node_load($product_id);
        $price = $p_node->sell_price;
        $j_result['price'] = '$' . number_format($price, 2, '.', ',');
        /*if ($_POST['expedite']) {
            $j_result['price'] .= ' x 2';
        }*/
    }
    echo json_encode($j_result);
}

//------------------------------------------------------------------------------
function ncn_download_create_pdf()
{
    $filename = drupal_get_path('module', 'ncn_create_claim') . "/pdfs/" . arg(2);

    header('Content-type: application/pdf');
    header('Content-Disposition: attachment; filename="' . arg(2) . '"');

    readfile($filename);
    exit;
}

//------------------------------------------------------------------------------
// used in "create_claim_product_title()" below...
function create_claim_product_title($nid)
{
    $node = node_load($nid);
    return ($node->title . " ($" . number_format($node->cost, 2) . ")");
}
