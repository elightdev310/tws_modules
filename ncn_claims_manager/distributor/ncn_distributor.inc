<?php
function ncn_distributor_page()
{
    return theme('ncn_dist_page', array('stat'=>1));
}

//------------------------------------------------------------------------------
// preprocess function
function template_preprocess_ncn_dist_page(&$vars)
{
    // check create step
    if (!isset($_POST['create_step']) || $_POST['create_step'] == '') {
        $create_step = 'input';
    } else {
        $create_step = $_POST['create_step'];
    }

    GLOBAL $user;
    if (isset($_POST['tfunction'])) {
        switch ($_POST['tfunction']) {
            case "authorize_report":
                if (ncn_dist_authorize_report() == true) {
                    drupal_goto('account/distributor.html/sales_report');
                } else {
                    drupal_set_message('Authorization is failed. Please try again.', 'error');
                }
                break;
            case "change_report_pass":
                if (ncn_dist_change_report_pass()) {
                    drupal_goto("account/distributor.html/sales_report");
                }
                break;
            // ---- add a new member ----
            case "paste_ids":
                ncn_dist_create_member_action($create_step);
                break;
        }
    }
    // ---- build content ----
    ob_start();

    switch (arg(2)) {
        // ---- create a new member ----
        default:
        case "create":
            $vars['title'] = "Create Member";
            $vars['page_class'] = "create-member";
            render_dist_create_member($create_step, $vars);
            break;

        case "view_member_list":
            drupal_add_css(drupal_get_path('module', 'ncn_message') . '/ncn_message.css', 'module', 'all', FALSE);
            drupal_add_js(drupal_get_path('module', 'ncn_message') . '/ncn_message.js');
            $vars['title'] = "View Member List";
            $vars['page_class'] = "view-member-list";
            render_dist_view_member_list($vars);
            break;

        case "sales_report":
            $vars['title'] = "Sales Report";
            $vars['page_class'] = "sales-report";
            render_sales_report_page($user->uid, $vars);
            break;

        case "authentication":
            $vars['title'] = "Authorize Distributor";
            $vars['page_class'] = "authentication";
            render_authorize_page($user->uid, 'dist');
            break;

        case "change_report_pass":
            $vars['title'] = "Change Distributor Password";
            $vars['page_class'] = "change-pass";
            render_change_report_pass_page($user->uid, 'dist');
            break;

        //edit users account
        case "my-account":
            $vars['title'] = "Edit My Profile";
            $vars['page_class'] = "my-account";
            //user_edit_new(false, $user->uid); // function from ncn_admin.module
            print theme('all_user_list_edit',array('in_admin'=>false, 'uid'=>$user->uid));
            /*print ncn_edit_my_account();*/
            break;

    }

    $vars['content'] = ob_get_contents();
    ob_end_clean();
}

//------------------------------------------------------------------------------
function charge_distributor($owner, $product_nid, $target_mid)
{
    // get cim details (for distributor)
    $user = user_load($owner);

    // check for free
    if ($user->profile_credit_card_number == '5424000000000015') {
        return true;
    }

//    $query = "SELECT * FROM member_distributor_cim WHERE username=\"" . AddSlashes($user->name) . "\";";
    $dist_cim = ncn_get_distributor_cim($user->name);
    if (!$dist_cim) {
        return false;
    }

    $customerProfileId = $member_cim['customerProfileId'];
    $customerPaymentProfileId = $member_cim['customerPaymentProfileId'];
    if (intval($customerProfileId) == 0 || intval($customerPaymentProfileId) == 0)
    {
        return true;
    }

    // get product details
    //$node = node_load(33);
    $node = node_load($product_nid);
    if (isset($node->data) && !is_array($node->data)) {
      $node->data = unserialize($node->data);
    }

    if ($node->sell_price != 0) {
        // attempt payment
        $response = sendNcnPaymentRequest($customerProfileId, $customerPaymentProfileId, 
                      array(
                        'price'         => $node->sell_price, 
                        'shipping_name' => $node->title, 
                        'shipping_desc' => $node->title, 
                        'item_id'       => $product_nid, 
                        'item_name'     => $node->title, 
                        'item_desc'     => $node->title, 
                      ));
        $auth_successful = true;
        if ("Ok" == $response->messages->resultCode) {
            $auth_successful = true;
            $response_code = 1; // success

            ncn_debug_message("Distributor charge successful.");
        } else {
            $auth_successful = false;
            $response_code = 3;

            $response_reason_text = preg_replace("/[^a-zA-Z0-9\s\'\.]/", "", $response->messages->message->text);
            ncn_debug_message("Distributor charge failed:" . $response_reason_text);
        }
    } else {
        $auth_successful = true;
    }
    //*** payment log ***//
    $payment_success = ($auth_successful) ? 1 : 0;
    ncn_report_payment_log($product_nid, $payment_success, $user, 0, $target_mid, 0, $user->uid);

    return $auth_successful;
}

function render_dist_create_member($create_step, &$vars) {
    ?>
    <fieldset class="terms-wrapper" style="<?php if ($create_step == 'input') {
        echo 'display: none;';
    } ?>">
        <div class="term-conditions"><tt><p align="justify">
                    <?php echo nl2br(variable_get('ncn_terms_text_dist', '')); ?>
                </p></tt></div>
        <br/>

        <div class="create-claim-agree-div">
            <input type="submit" id="create_member_btn" class="create-claim-agree-btn"
                   onclick="dist_create_member_submit('create_member_btn')" value="I Agree"/>
            <span>&nbsp; or &nbsp;<a href="<?= $GLOBALS['base_url']; ?>/account">cancel</a></span>
        </div>
    </fieldset>
    <fieldset style="<?php if ($create_step == 'create') {
        echo 'display: none;';
    } ?>">

        <p class="description">Input fields for a new member and click "submit" to add to the pool.</p>

        <form method="POST" id="create_member_submit_form">
            <input type="hidden" name="tfunction" value="paste_ids"/>
            <input type="hidden" name="create_step" value="<?php echo $create_step; ?>"/> <!-- STEP -->
            <table style="width:100%;border:0;">
                <tr>
                    <td class="td-label">Member ID#:</td>
                    <td><input type="text" name="member_id" value="<?= generate_member_id(); ?>" readonly></td>
                </tr>
                <tr>
                    <td class="td-label">First Name:</td>
                    <td><input type="text" name="first_name" value="<?= isset($_POST['first_name'])? $_POST['first_name']:''; ?>"></td>
                </tr>
                <tr>
                    <td class="td-label">Last Name:</td>
                    <td><input type="text" name="last_name" value="<?= isset($_POST['last_name'])? $_POST['last_name']:''; ?>"></td>
                </tr>
                <tr>
                    <td class="td-label">Company's Legal Name:</td>
                    <td><input type="text" name="legalname" value="<?= isset($_POST['legalname'])? $_POST['legalname']:''; ?>"></td>
                </tr>
                <tr>
                    <td class="td-label">Country:</td>
                    <td><?php draw_countryform_part(array(
                            'name'=>'country', 
                            'class'=>'', 
                            'disabled'=>false, 
                            'required'=>false)); ?>
                    </td>
                </tr>
                <tr>
                    <td class="td-label">Address:</td>
                    <td><input type="text" name="address" value="<?= isset($_POST['address'])? $_POST['address']:''; ?>"></td>
                </tr>
                <tr>
                    <td class="td-label">City:</td>
                    <td><input type="text" name="city" value="<?= isset($_POST['city'])? $_POST['city']:''; ?>"></td>
                </tr>
                <tr>
                    <td class="td-label">Province/State:</td>
                    <td><?php draw_stateform_part(array(
                            'name'  => 'state', 
                            'class' => '', 
                            'disabled' => false, 
                            'required' => false )); ?>
                    </td>
                </tr>
                <tr>
                    <td class="td-label">Zip:</td>
                    <td><input type="text" name="zip" value="<?= isset($_POST['zip'])? $_POST['zip']:''; ?>"></td>
                </tr>
                <tr>
                    <td class="td-label">Office Phone:</td>
                    <td><input type="text" name="officephone" value="<?= isset($_POST['officephone'])? $_POST['officephone']:''; ?>"></td>
                </tr>
                <tr>
                    <td class="td-label">Contact Email:</td>
                    <td><input type="text" name="contactemail" value="<?= isset($_POST['contactemail'])? $_POST['contactemail']:''; ?>"></td>
                </tr>
                <tr>
                    <td class="td-label">Sales Rep:</td>
                    <td><input type="text" name="sales_rep" value="<?= isset($_POST['sales_rep'])? $_POST['sales_rep']:''; ?>"></td>
                </tr>
                <?php
                if (is_distributor($GLOBALS['user']) == true) {
                    distributor_dropdown(); // It couldn't flow through this way.
                } else {
                ?>
                    <tr>
                        <td class="td-label">Distributor:</td>
                        <td><?php distributor_dropdown(); ?></td>
                    </tr>
                <?php
                }
                ?>
                <!-- <tr>
            <td class="td-label"></td>
            <td><input type="checkbox" name="overwrite_existing" style="width:15px;"> Overwrite existing unactivated member</td>
        </tr> -->
                <tr>
                    <td class="td-label">Member Type:</td>
                    <td>
                        <?php
                        $member_type = 0;
                        if (isset($_POST['member_type'])) {
                            $member_type = $_POST['member_type'];
                        }
                        ncn_admin_render_member_type($member_type);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="td-label">SignUp Fee Type:</td>
                    <td>
                        <?php
                        $product_nid = SKU_SUF_DF;
                        if (isset($_POST['retail_kit'])) {
                            $product_nid = $_POST['retail_kit'];
                        }
                        echo ncn_claims_manager_draw_signup_fee_select('retail_kit', $product_nid);
                        ?>
                    </td>
                </tr>
            </table>

            <p>&nbsp;</p>
            <!-- <p class="description">Credit/Debit Card Details (optional)</p> -->
            <table style="width:100%; border:0;">
                <tr>
                    <td class="td-label">Card Number:</td>
                    <td><input type="text" name="card_number" value="<?= isset($_POST['card_number'])? $_POST['card_number']:''; ?>"></td>
                <tr>
                <tr>
                    <td class="td-label">Expiration Date (mm/yy):</td>
                    <td><input type="text" name="expiration_date" value="<?= isset($_POST['expiration_date'])? $_POST['expiration_date']:''; ?>">
                    </td>
                <tr>
                <tr>
                    <td class="td-label">Security Code:</td>
                    <td><input type="text" name="security_code" value="<?= isset($_POST['security_code'])? $_POST['security_code']:''; ?>"></td>
                <tr>
                <tr>
                    <td class="td-label"></td>
                    <td><input type="submit" id="create_member_btn" class="create-member-submit-btn"
                               onclick="dist_create_member_submit('create_member_btn')" value="submit"/></td>
                <tr>

            </table>


        </form>
    </fieldset>
    <?php
}

function ncn_dist_create_member_action(&$create_step) {
    // sanitize input
    $member_id = $new_string = preg_replace("/[^a-zA-Z0-9\s]/", "", $_POST['member_id']);
    //$member_id = generate_member_id();
    $first_name = $new_string = preg_replace("/[^a-zA-Z0-9\s\'\.]/", "", $_POST['first_name']);
    $last_name = $new_string = preg_replace("/[^a-zA-Z0-9\s\'\.]/", "", $_POST['last_name']);
    $legalname = $new_string = preg_replace("/[^a-zA-Z0-9\s\'\.]/", "", $_POST['legalname']);
    $address = $new_string = preg_replace("/[^a-zA-Z0-9\s\'\.]/", "", $_POST['address']);
    $city = $new_string = preg_replace("/[^a-zA-Z0-9\s\'\.]/", "", $_POST['city']);
    $state = $new_string = preg_replace("/[^a-zA-Z0-9\s]/", "", $_POST['state']);
    $country = $new_string = preg_replace("/[^a-zA-Z0-9\s\'\.]/", "", $_POST['country']);
    $zip = $new_string = preg_replace("/[^a-zA-Z0-9\s]/", "", $_POST['zip']);
    $officephone = $new_string = preg_replace("/[^a-zA-Z0-9\s]/", "", $_POST['officephone']);
    $contactemail = $new_string = preg_replace("/[^a-zA-Z0-9\/:@\.\+-s]/", "", $_POST['contactemail']);
    $sales_rep = $new_string = preg_replace("/[^a-zA-Z0-9\s\'\.]/", "", $_POST['sales_rep']);

    $card_number = trim($_POST['card_number']); //if ($card_number == "") {$card_number = '5424000000000015'; }
    $expiration_date = trim($_POST['expiration_date']);
    $security_code = trim($_POST['security_code']);

    //* validation */
    if ((trim($member_id) == "") || (trim($first_name) == "") || (trim($last_name) == "") || (trim($legalname) == "") ||
        (trim($country) == "") || (trim($address) == "") || (trim($city) == "") || (trim($state) == "") ||
        (trim($zip) == "") || (trim($officephone) == "") || (trim($contactemail) == "") || (trim($sales_rep) == "") ||
        (trim($card_number) == "") || (trim($expiration_date) == "") || (trim($security_code) == "")

    ) {
        drupal_set_message("You need to input all fields.", "error");
        return;
    }

    if (!valid_email_address($contactemail)) {
        drupal_set_message('Contact email address is invalid. Please retry to input email address.', 'error');
        return;
    }

    // Check Duplication of Email
    if (check_existing_email($contactemail)) {
        drupal_set_message('Contact email address exists. Please use another email address.', 'error');
        return;
    }

    // get distributor name
    $dist = user_load($_POST['distributor']);
    $distributor_name = $dist->profile_firstname . ' ' . $dist->profile_lastname;

    // calculate the owner
    $owner = $_POST['distributor'];

    // check to see if the id is in range
    /* $query = "SELECT * FROM member_id_ranges WHERE distributor=".$GLOBALS['user']->uid." AND start<=".$member_id." AND end >= ".$member_id;
    $result = db_query($query);
    $row_count = $result->rowCount();*/

//              if ($row_count > 0)
//              {
    // check first existing member id
    // $query = "SELECT * FROM member_id_pool WHERE member_id=\"" . AddSlashes($member_id) . "\" AND used=1;";
    $found_used = check_existing_memberid($member_id, 1);

    // $query = "SELECT * FROM member_id_pool WHERE member_id=\"" . AddSlashes($member_id) . "\" AND used=0;";
    $found_unused = check_existing_memberid($member_id, 0);

    // Progress to Next Step ( input -> create )
    if ($create_step == 'input') {
        $create_step = 'create';
        return;
    } else if ($create_step == 'create') {
        $create_step = 'input';
    } else {
        //error
        return;
    }

    // allow add
    /*if (isset($_POST['overwrite_existing']))
    {
        if (($found_unused > 0) && ($found_used == 0))
        {
            $allow_add = true;
            $ignore_distributor_charge = true;  // do not charge distributor for member id again
        }
        else
        {   $allow_add = false;
        }
    }
    else
    {
        $allow_add = false;
    }*/
    $allow_add = false;
    if ($found_used == 0 && $found_unused == 0) {
        $allow_add = true;
    }

    if ($allow_add == true) {
        $member_type = 0;
        if (isset($_POST['member_type'])) {
            $member_type = $_POST['member_type'];
        }
        // get singup fee type
        $product_nid = SKU_SUF_DF;
        if (isset($_POST['retail_kit'])) {
            $product_nid = $_POST['retail_kit'];
        }
        $signup_fee_type = get_signup_fee_type($product_nid);

        // check credit card
        $add_cim_profile = false;
        $error = false; // ***

        if (($card_number != '') && ($card_number != '5424000000000015')) {
            $cim_vars = array(
                'expiration_date'           => $expiration_date, 
                'merchant_customer_id'      => $member_id.date("U"), 
                'first_name'                => $first_name, 
                'last_name'                 => $last_name, 
                'contactemail'              => $contactemail, 
                'address'                   => $address, 
                'city'                      => $city, 
                'state'                     => $state, 
                'zip'                       => $zip, 
                'card_number'               => $card_number, 
                'security_code'             => $security_code, 
            );
            $cim = getNcnCimCustomerProfileInfo($cim_vars);
            if ($cim == false) {
                $error = true;
                //$form_state['redirect'] = 'apply.html?id='.$_GET['id'];
            } else {
                $add_cim_profile = true;
                ncn_debug_message("User card details were good, flagging to add cim_profile.");
            } // END adding profile failed
        } else if ($card_number == '5424000000000015') {
            // free card number
            drupal_set_message('Adding new FREE member...', 'status');

            $customerProfileId = "FREE";
            $customerPaymentProfileId = "FREE";
            $add_cim_profile = true;
            $ignore_distributor_charge = true;
        }

        if ($error == false) {
            // charge the distributor?
            if (!isset($ignore_distributor_charge)) {
                //ncn_debug_message("Charging distributor signup fee.");
                $res = charge_distributor($owner, $product_nid, $member_id);

                // charging distributor failed.
                if ($res == false) {
                    $error = true;
                }
            } // do not charge distributor again (they are re-adding a existing member id)
            else {
                ncn_debug_message("Ignoring distributor charge for existing member id#.");
            }
        }

        // do we add new cim profile?
        if ($add_cim_profile == true) {
            // $query = "DELETE FROM member_cim WHERE member_id=\"" . AddSlashes($member_id) . "\";";
            $result = db_query('DELETE FROM {member_cim} WHERE member_id=:a',array(':a'=>$member_id));

            // $query = "INSERT INTO member_cim VALUES(NULL, \"" . AddSlashes($member_id) . "\", \"" . AddSlashes($customerProfileId) . "\", \"" . AddSlashes($customerPaymentProfileId) . "\");";
            $result = db_query('INSERT INTO {member_cim} VALUES(NULL,:a,:b,:c)',
                array(':a'=>$member_id,':b'=>$customerProfileId,':c'=>$customerPaymentProfileId));
        }

        // email the user with access details
        if ($error == false) {
            $params = array(
                '!firstname' => $first_name,
                '!lastname' => $last_name,
                '!member_id' => $member_id,
            );

            drupal_mail('ncn_claims_manager', 'id_to_member', $contactemail, language_default(), $params);


            // insert into database
            // $query = "DELETE FROM member_id_pool WHERE `member_id`=\"" . AddSlashes($member_id) . "\";";
            $result = db_query('DELETE FROM {member_id_pool} WHERE member_id=:a',array(':a'=>$member_id));

            // $query = "INSERT INTO member_id_pool VALUES(NULL, \"" . AddSlashes($member_id) . "\", 1, \"" . AddSlashes($first_name) . "\", \"" . AddSlashes($last_name) . "\", \"" . AddSlashes($legalname) . "\", \"" . AddSlashes($country) . "\", \"" . AddSlashes($address) . "\", \"" . AddSlashes($city) . "\", \"" . AddSlashes($state) . "\", \"" . AddSlashes($zip) . "\", \"" . AddSlashes($officephone) . "\", \"" . AddSlashes($contactemail) . "\", \"" . AddSlashes($distributor_name) . "\", \"" . AddSlashes($sales_rep) . "\", \"" . AddSlashes($security_code) . "\", " . $owner . ", 0, 0, 0, 0, $member_type ,$signup_fee_type, " . date('U') . " )";
            $result = db_query('INSERT INTO {member_id_pool} VALUES(NULL,:a,:b,:c,:d,:e,:f,:g,:h,:i,:j,:k,:k1,:l,:m,:n,:o,:p,:q,:r,:s,:t,:u,:v,:w)',
                                array(':a'=>$member_id,
                                      ':b'=>1,                      // status
                                      ':c'=>$first_name,
                                      ':d'=>$last_name,
                                      ':e'=>$legalname,
                                      ':f'=>$country,
                                      ':g'=>$address,
                                      ':h'=>$city,
                                      ':i'=>$state,
                                      ':j'=>$zip,
                                      ':k'=>$officephone,
                                      ':k1'=>'',                    // mobilephone
                                      ':l'=>$contactemail,
                                      ':m'=>$distributor_name,
                                      ':n'=>$sales_rep,
                                      ':o'=>$security_code,
                                      ':p'=>$owner,
                                      ':q'=>0,                      // am_uid
                                      ':r'=>0,                      // used
                                      ':s'=>0,                      // grant_subuser
                                      ':t'=>0,                      // next_payment_date
                                      ':u'=>$member_type ,
                                      ':v'=>$signup_fee_type,
                                      ':w'=>date('U')));

            // Not used db_insert because of 'create' field
            // $result = db_insert('member_id_pool')
            //         ->fields(array(
            //             'member_id'         => $member_id, 
            //             'status'            => 1, 
            //             'first_name'        => $first_name, 
            //             'last_name'         => $last_name, 
            //             'legalname'         => $legalname, 
            //             'country'           => $country, 
            //             'address'           => $address, 
            //             'city'              => $city, 
            //             'state'             => $state, 
            //             'zip'               => $zip, 
            //             'officephone'       => $officephone, 
            //             'mobilephone'       => '', 
            //             'contactemail'      => $contactemail, 
            //             'distributor'       => $distributor_name, 
            //             'sales_rep'         => $sales_rep, 
            //             'security_code'     => $security_code, 
            //             'owner'             => $owner, 
            //             'am_uid'            => 0, 
            //             'used'              => 0, 
            //             'grant_subuser'     => 0, 
            //             'next_payment_date' => 0, 
            //             'member_type'       => $member_type, 
            //             'signup_fee_type'   => $signup_fee_type, 
            //             'create'            => date('U'), 
            //         ))
            //         ->execute();

            drupal_set_message('Added new member to the pool.', 'status');

            //** NCN Gold Member 1st Special // ** IGNORE **
            if ($_POST['retail_kit'] == SKU_SUF_GMS_1ST) {
                ncn_admin_insert_member_first_free($member_id);
            }

            // auto-assign account manager
            ncn_admin_auto_assign_member_to_am($member_id);

            if (module_exists('ncn_capsulecrm')) {
                ncn_capsulecrm_create_member($member_id);
            }
            $_POST = array(); // clear the form
        } // END add member
    } else {
        // duplicate member id
        drupal_set_message('That member id# is already in the database/being used.', 'error');
    }
    /*              }
    else
    {
        drupal_set_message('That member id# has not been assigned to this account.' , 'error');
    }
    */
}

function render_dist_view_member_list(&$vars) {
    $page_records = 8;
    ?>
    <div class="clear-block">
        <div class="ncn-pager-ranger" style="font-weight: bold; margin-bottom:5px;"></div>
    </div>
    <div class="table-header">
        <table>
            <tr class="header">
                <td class="td-member-id"><strong>Member ID</strong></td>
                <td class="td-first-name"><strong>First Name</strong></td>
                <td class="td-last-name"><strong>Last Name</strong></td>
                <td class="td-company-name"><strong>Company Name</strong></td>
                <td class="td-address"><strong>Address</strong></td>
                <td class="td-city"><strong>City</strong></td>
                <td class="td-state"><strong>State</strong></td>
                <td class="td-zip"><strong>Zip</strong></td>
                <td class="td-phone"><strong>Phone</strong></td>
                <td class="td-contact"><strong>Contact Member</strong></td>
            </tr>
        </table>
    </div>
    <div class="table-body">
        <?php
        // $query = "SELECT * FROM member_id_pool WHERE owner=" . $GLOBALS['user']->uid . " AND used=1;";
        $result = db_query('SELECT * FROM {member_id_pool} WHERE owner=:a AND used=1',array(':a'=>$GLOBALS['user']->uid));
        $row_count = $result->rowCount();
        $page_num = 0;

        $result_1 = db_query('SELECT * FROM {profile_field} WHERE name=:a',array(':a'=>'profile_memberid'));
        $memberid_field = $result_1->fetchAssoc();

        $member_count = 0;
        for ($i = 0; $i < $row_count; $i++) {
            $row = $result->fetchAssoc();

            $result_1 = db_query('SELECT * FROM {profile_value} WHERE fid=:a AND value=:b ',
            array(':a'=>$memberid_field['fid'],':b'=>$row['member_id']))->fetchField();
            if ($result_1 == 0) {
                continue;
            }
            ?>
            <?php if ($member_count % $page_records == 0) {
                $page_num++; ?>
                <table class="ncn-pager ncn-pager-page-<?php echo $page_num; ?>">
            <?php } ?>
            <tr>
                <td class="td-member-id"><?= $row['member_id']; ?></td>
                <td class="td-first-name"><?= $row['first_name']; ?></td>
                <td class="td-last-name"><?= $row['last_name']; ?></td>
                <td class="td-company-name"><?= $row['legalname']; ?></td>
                <td class="td-address"><?= $row['address']; ?></td>
                <td class="td-city"><?= $row['city']; ?></td>
                <td class="td-state"><?= $row['state']; ?></td>
                <td class="td-zip"><?= $row['zip']; ?></td>
                <td class="td-phone"><?= $row['officephone']; ?></td>
                <td class="td-contact"><a
                        href="mailto:<?= $row['contactemail']; ?>"><?= $row['contactemail']; ?></a></td>
            </tr>
            <?php if ($member_count % $page_records == ($page_records - 1)): ?>
                </table>
            <?php endif; ?>
            <?php
            $member_count += 1;
        }
        if ($member_count == 0) {
            ?>
            <table class="">
                <tr>
                    <td colspan="10">No Members</td>
                </tr>
            </table>
        <?php
        } else if (($member_count % $page_records) != 0) {
            for ($i = $member_count; $i < (ceil($member_count / $page_records) * $page_records); $i++) {
                ?>
                <tr>
                    <td colspan="10">&nbsp;</td>
                </tr>
            <?php
            }
            echo "</table>";
        }
        ?>

    </div>
    <div class="ncn-pager-control clearfix">    <!-- pager_control -->
        <div style="float: right">
            <a class="first" href="#" onclick="pm_page_action('first');">first</a>
            <a class="prev" href="#" onclick="pm_page_action('prev');">prev</a>
            <a class="next" href="#" onclick="pm_page_action('next');">next</a>
            <a class="last" href="#" onclick="pm_page_action('last');">last</a>
        </div>
    </div>
    <script type="text/javascript">
        var total_records = <?php echo $member_count ?>;
        var page_records = <?php echo $page_records; ?>;
        var page_number = 1;    //global;
        var page_count = <?php echo $page_num; ?>;
        jQuery(document).ready(function () {
            pm_select_page(page_number);
            if (total_records <= page_records) {
                jQuery('.ncn-pager-ranger').css('display', 'none');
                jQuery('.ncn-pager-control').css('display', 'none');
            }
        });
    </script>
    <?php
}

/**
 *    Display Sales Report Page for Distributor
 */
function render_sales_report_page($dist_uid, &$vars)
{

// authentication for distributor report
    if (!ncn_dist_authentication($dist_uid)) {
        drupal_goto('account/distributor.html/authentication');
        return;
    }
/////////////////////////////////////////////////////////
    $data = array();
    $report_date = 0;
    if (isset($_GET['report_date'])) {
        $report_date = $_GET['report_date'];
    } else {
        $report_date = time();
    }

    _ncn_report_get_data_distributor_detailed($data, $report_date, $dist_uid);

    /* top body */
    $topbody = '<div class="content clearfix">';
    $topbody .= '<div class="date-links">';

    global $base_root;
    $url = $base_root . "/account/distributor.html/sales_report/";
    $next_month = strtotime("+1 month", $report_date);
    $last_month = strtotime("-1 month", $report_date);

    $topbody .= '<span><a href="' . $url . '?report_date=' . time() . '">[This Month]</a></span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;';
    $topbody .= '<span><a href="' . $url . '?report_date=' . $last_month . '" title="' . date("F Y", $last_month) . '">[Last]</a></span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;';
    $topbody .= '<span>' . date("F Y", $report_date) . '</span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;';
    $topbody .= '<span><a href="' . $url . '?report_date=' . $next_month . '" title="' . date("F Y", $next_month) . '">[Next]</a></span>';
    $topbody .= '</div>';

    $total_monthly_earnings = 0;
    foreach ($data as $key => $data_part) {
        $total_monthly_earnings += $data_part[5];
    }

    $topbody .= '<div class="total-monthly-earnings">Total Monthly Earnings: &nbsp;<strong>' . render_payment_cost($total_monthly_earnings) . '</strong></div>';
    $topbody .= '</div>';
    $vars['topbody'] = $topbody;

    draw_sales_report_table($data);
}

function ncn_dist_authentication($dist_uid)
{

    $_user = user_load($dist_uid);
    if (isset($_SESSION['auth_report']) && $_SESSION['auth_report'] == $_user->name) {
        return true;
    }

    unset($_SESSION['auth_report']);
    return false;
}

function _get_report_pass($username)
{
  //  $query = "SELECT * FROM auth_report WHERE username='$username'";
    $result = db_query('SELECT * FROM {auth_report} WHERE username=:a',array(':a'=>$username));
   // var_dump($result);
    if ($result->rowCount() == 0) {
       // $query = "SELECT * FROM users WHERE name='$username'";
        $result = db_query('SELECT * FROM {users} WHERE name=:a',array(':a'=>$username));
        if ($result->rowCount() > 0) {
            $row = $result->fetchAssoc();
          //  $query = "INSERT INTO auth_report(username, pass) VALUES('$username', '" . $row['pass'] . "')";
            $result = db_query('INSERT INTO {auth_report} (username, pass) VALUES(:a,:b)',array(':a'=>$username,':b'=>$row['pass']));
            return $row['pass'];
        } else {
            return '';
        }
    }

    $row = $result->fetchAssoc();
//    var_dump($row['pass']);
//    exit;
    return $row['pass'];
}

/**
 * Check Password
 * @param $password: plain-text
 * @param $hashed_pwd: in DB
 */
function ncn_dist_user_check_password($password, $hashed_pwd)
{
    module_load_include('inc', 'content', 'includes/password');
    $account = array('pass'=>$hashed_pwd);
    $account = (object)$account;
    return user_check_password($password, $account);
}
function ncn_dist_authorize_report()
{
    global $user;
    $user = user_load($user->uid);
    $ret_val = false;
    if (isset($_POST['username']) && $_POST['username'] == $user->name) {
        $reportpass = _get_report_pass($user->name);
        if (isset($_POST['reportpass']) && ncn_dist_user_check_password($_POST['reportpass'], $reportpass)) {
            $_SESSION['auth_report'] = $user->name;
            $ret_val = true;
        }
    }

    return $ret_val;
}

function render_authorize_page($uid, $type)
{
    $_user = user_load($uid);

    if ($type == "dist") {
        $header_desc = 'Please enter your distributor password.';
    } else if ($type == "asso") {
        $header_desc = 'Please enter your associate password.';
    }
    ?>
    <div class="header">
        <p><?php echo $header_desc; ?></p>
    </div>
    <form method="POST" id="authorize_report">
        <input type="hidden" name="tfunction" value="authorize_report"/>

        <table style="width:100%;border:0;">
            <tr>
                <td class="td-label">Username:</td>
                <td><input type="text" name="username" value="<?= $_user->name; ?>" readonly></td>
            </tr>
            <tr>
                <td class="td-label">Password:</td>
                <td><input type="password" name="reportpass"></td>
            </tr>
            <tr>
                <td class="td-label">&nbsp;</td>
                <td><input type="submit" value="Continue"></td>
            </tr>
        </table>
    </form>
<?php
}

function ncn_dist_change_report_pass()
{
    $username = $_POST['username'];
    $old_pass = $_POST['old_pass'];
    $new_pass = $_POST['new_pass'];
    $confirm_pass = $_POST['confirm_pass'];

    if (!ncn_dist_user_check_password($old_pass, _get_report_pass($username))) {
        drupal_set_message('Old report password is invalid', 'error');
        return false;
    }
    if ($new_pass != $confirm_pass) {
        drupal_set_message('The new report passwords you type do not match.', 'error');
        return false;
    }

    module_load_include('inc', 'content', 'includes/password');

    // $query = "UPDATE auth_report SET pass='" . md5($new_pass) . "' WHERE username='$username'";
    $result = db_query('UPDATE {auth_report} SET pass=:a WHERE username=:b',array(':a'=>user_hash_password(trim($new_pass)),':b'=>$username));
    if (!$result) {
        drupal_set_message('It is failed to change password.', 'error');
        return false;
    }
    drupal_set_message('Report password changed, successfully.');
    return true;
}

function render_change_report_pass_page($uid, $type)
{
    if (!ncn_dist_authentication($uid)) {
        drupal_goto("account/distributor.html/authentication");
        return;
    }

    $_user = user_load($uid);

    if ($type == "dist") {
        $header_desc = '';
    } else if ($type == "asso") {
        $header_desc = '';
    }
    ?>
    <div class="header">
        <p><?php echo $header_desc; ?></p>
    </div>
    <form method="POST" id="change_report_pass">
        <input type="hidden" name="tfunction" value="change_report_pass"/>

        <table style="width:100%;border:0;">
            <tr>
                <td class="td-label">Username:</td>
                <td><input type="text" name="username" value="<?= $_user->name; ?>" readonly></td>
            </tr>
            <tr>
                <td class="td-label">Old Password:</td>
                <td><input type="password" name="old_pass"></td>
            </tr>
            <tr>
                <td class="td-label">New Password:</td>
                <td><input type="password" name="new_pass"></td>
            </tr>
            <tr>
                <td class="td-label">Confirm Password:</td>
                <td><input type="password" name="confirm_pass"></td>
            </tr>
            <tr>
                <td class="td-label">&nbsp;</td>
                <td><input type="submit" value="Update"></td>
            </tr>
        </table>
    </form>
<?php
}

function draw_sales_report_table($data)
{
    ?>
    <div class="clear-block">
        <div class="ncn-pager-ranger" style="font-weight: bold; margin-bottom:5px;"></div>
    </div>
    <div class="table-header">
        <table>
            <tr class="header">
                <td class="td-company-name"><strong>Company Name</strong></td>
                <td class="td-member-name"><strong>Member Name</strong></td>
                <td class="td-start-up-fee"><strong>Start-up Fee</strong></td>
                <td class="td-monthly-membership-fee"><strong>Monthly Membership Fee</strong></td>
                <td class="td-date"><strong>Date</strong></td>
                <td class="td-total-earned"><strong>Total Earned</strong></td>
            </tr>
        </table>
    </div>
    <div class="table-body">
        <?php
        drupal_add_css(drupal_get_path('module', 'ncn_message') . '/ncn_message.css', 'module', 'all', FALSE);
        drupal_add_js(drupal_get_path('module', 'ncn_message') . '/ncn_message.js');

        $page_records = 10;
        $row_count = count($data);
        $page_num = 0;

        $member_count = 0;
        foreach ($data as $key => $data_part) {
            if ($member_count % $page_records == 0) {
                $page_num++; ?>
                <table class="ncn-pager ncn-pager-page-<?php echo $page_num; ?>">
            <?php } ?>
            <tr>
                <td class="td-company-name"><?php echo $data_part[0]; ?></td>
                <td class="td-member-name"><?php echo $data_part[1]; ?></td>
                <td class="td-start-up-fee"><?php echo $data_part[2]; ?></td>
                <td class="td-monthly-membership-fee"><?php echo $data_part[3]; ?></td>
                <td class="td-date"><?php echo date('d M Y H:i:s', $data_part[4]); ?></td>
                <td class="td-total-earned"><?php echo render_payment_cost($data_part[5]); ?></td>
            </tr>
            <?php if ($member_count % $page_records == ($page_records - 1)): ?>
                </table>
            <?php endif; ?>
            <?php
            $member_count += 1;
        }
        if ($member_count == 0) {
            ?>
            <table class="">
                <?php
                for ($i = $member_count; $i < $page_records; $i++) {
                    ?>
                    <tr>
                        <td colspan="6">&nbsp;</td>
                    </tr>
                <?php } ?>
            </table>
        <?php
        } else if (($member_count % $page_records) != 0) {
            for ($i = $member_count; $i < (ceil($member_count / $page_records) * $page_records); $i++) {
                ?>
                <tr>
                    <td colspan="6">&nbsp;</td>
                </tr>
            <?php
            }
            echo "</table>";
        }
        ?>
    </div>
    <div class="ncn-pager-control clearfix">    <!-- pager_control -->
        <div style="float: right">
            <a class="first" href="#" onclick="pm_page_action('first');">first</a>
            <a class="prev" href="#" onclick="pm_page_action('prev');">prev</a>
            <a class="next" href="#" onclick="pm_page_action('next');">next</a>
            <a class="last" href="#" onclick="pm_page_action('last');">last</a>
        </div>
    </div>
    <script type="text/javascript">
        var total_records = <?php echo $member_count ?>;
        var page_records = <?php echo $page_records; ?>;
        var page_number = 1;    //global;
        var page_count = <?php echo $page_num; ?>;
        jQuery(document).ready(function () {
            pm_select_page(page_number);
            if (total_records <= page_records) {
                jQuery('.ncn-pager-ranger').css('display', 'none');
                jQuery('.ncn-pager-control').css('display', 'none');
            }
        });
    </script>
<?php
}

function ncn_claims_manager_draw_signup_fee_select($name, $default)
{
    // #options
    $payments = array();
    $pnode = node_load(SKU_SUF_DF);
    $pnode->data = unserialize($pnode->data);
    $payments[SKU_SUF_DF] = $pnode; // NCN Membership Kit ($279)
    
    // $pnode = node_load(SKU_SUF_DP);
    // $pnode->data = unserialize($pnode->data);
    // $payments[SKU_SUF_DP] = $pnode; // NCN Membership Kit Options ($99)

    /* NCN GOLD MEMBER SPECIAL  */
    // $pnode = node_load(SKU_SUF_GMSD);
    // $pnode->data = unserialize($pnode->data);
    // $payments[SKU_SUF_GMSD] = $pnode; // NCN Membership Kit Options ($49)

    /*$pnode = node_load(57);   $pnode->data = unserialize($pnode->data);
    $payments['57'] = $pnode;   // NCN Gold First Special($0)*/

    $select_field = "<select id=\"$name\" name=\"$name\"> \r\n";
    foreach ($payments as $key => $product) {
        $selected = ($key == $default) ? "selected" : "";
        $select_field .= "<option value=\"$key\" $selected >" . $product->title . " ($" . number_format($product->cost, 2) . ")</option>";
    }
    $select_field .= "</select> \r\n";

    return $select_field;
}
