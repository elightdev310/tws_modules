<?php
/**
 * Form builder; The user registration form.
 *
 * @ingroup forms
 * @see user_register_validate()
 * @see user_register_submit()
 */

function user_register()
{
    global $user, $base_url;

    if (!isset($_GET['id'])) //HERE
    {
        Header("Location:".$base_url."/activate.html");
        exit;
    }
    else
    {
        $saved_member_id = base64_decode($_GET['id']);
    }

    $admin = user_access('administer users');

    // If we aren't admin but already logged on, go to the user page instead.
    if (!$admin && $user->uid)
    {
        drupal_goto('user/' . $user->uid);
    }

    $form = array();
    /*$form['service_question'] = array(
                                        '#type'=>'radios',
                                        '#title' => t('What kind of restoration services do you provide'),
                                        '#options' => array(
                                            'Water Restoration' => 'Water Restoration',
                                            'Fire Restoration' => 'Fire Restoration',
                                            'Reconstruction' => 'Reconstruction',
                                            'Mold Remediation' => 'Mold Remediation'
                                        ),
                                        '#default_value' => '',
                                        '#required'=>TRUE,
                                        '#weight' => -40,
                                        );*/

    // Display the registration form.

    if (!$admin)
    {
        $form['user_registration_help'] = array(
            '#value' => filter_xss_admin(variable_get('user_registration_help', '')) ,

            // Ensure that user registration help appears above profile fields.

            '#weight' => - 20,
        );
    }

    // Merge in the default user edit fields.
    $form = array_merge($form, user_edit_form($form_state, NULL, NULL, TRUE));
    if ($admin)
    {
        $form['account']['notify'] = array(
            '#type' => 'checkbox',
            '#title' => t('Notify user of new account')
        );

        // Redirect back to page which initiated the create request;
        // usually admin/user/user/create.

        $form['destination'] = array(
            '#type' => 'hidden',
            '#value' => $_GET['q']
        );
    }

    /******* for confirm ****/
    $form['confirm_page'] = array(
        '#type' => 'hidden',
        '#value' => 0,
        '#attributes' => array('id'=>'edit-user-confirm-page'),
    );
    /******/

    // Create a dummy variable for pass-by-reference parameters. (Profile Fields)
    $null = NULL;
    $extra = _user_forms($null, NULL, NULL, 'register');

    // insert the default email address
    $form['account']['mail']['#default_value'] = $extra['Company Information']['profile_contactemail']['#default_value'];
    // Remove form_group around default fields if there are no other groups.

    if (!$extra)
    {
        foreach(array(
            'name',
            'mail',
            'pass',
            'status',
            'roles',
            'notify'
        ) as $key)
        {
            if (isset($form['account'][$key]))
            {
                $form[$key] = $form['account'][$key];
            }
        }

        unset($form['account']);
    }
    else
    {
        $form = array_merge($form, $extra);
    }

    if (variable_get('configurable_timezones', 1))
    {

        // Override field ID, so we only change timezone on user registration,
        // and never touch it on user edit pages.

        $form['timezone'] = array(
            '#type' => 'hidden',
            '#default_value' => variable_get('date_default_timezone', NULL) ,
            '#id' => 'edit-user-register-timezone',
        );

        // Add the JavaScript callback to automatically set the timezone.

        drupal_add_js('
                // Global Killswitch
                if (Drupal.jsEnabled) {
                    jQuery(document).ready(function() {
                        Drupal.setDefaultTimezone();
                    });
                }',
        'inline');
    }

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Create new account') ,
        '#weight' => 30
    );

    // $form['#validate'][] = 'user_register_anwser_question';

    $form['#validate'][] = 'user_register_validate';
    $form['#validate'][] = 'user_register_confirm';
    $form['#attributes']['id'] = 'user-register';
    //$form['#submit'][] = 'user_register_form_submit';

    return $form;
}

/*
* User Registration Form Submit
*/
function user_register_form_submit($form, &$form_state)
{
    global $base_url;

    $date = explode("/", isset($_POST['profile_credit_card_expiration'])?$_POST['profile_credit_card_expiration']:'');

    if (sizeof($date) == 2)
    {       $date = "20".$date[1]."-".$date[0];   }
    else
    {       $date = '';     }

    // check for free / admin already added profile
    // ** This needs to be removed **
    /*if ((isset($_POST['profile_credit_card_number'])) && ($_POST['profile_credit_card_number'] != '5424000000000015'))
    {
        // add the user to cim
          $content =
              "<?xml version=\"1.0\" encoding=\"utf-8\"?>" .
              "<createCustomerProfileRequest xmlns=\"AnetApi/xml/v1/schema/AnetApiSchema.xsd\">" .
              MerchantAuthenticationBlock().
              "<profile>".
              "<merchantCustomerId>" . get_unique_auth_profile_id() . "</merchantCustomerId>". // Your own identifier for the customer.
              "<description>".$_POST['profile_firstname']." ".$_POST['profile_lastname']."</description>".
              "<email>" . $_POST["mail"] . "</email>".
              "</profile>".
              "</createCustomerProfileRequest>";    // <?php

          $response = send_xml_request($content);
          $parsedresponse = parse_api_response($response);

          $customerProfileId = $parsedresponse->customerProfileId;


        // add a payment profile to cim
          $content =
          "<?xml version=\"1.0\" encoding=\"utf-8\"?>" .
          "<createCustomerPaymentProfileRequest xmlns=\"AnetApi/xml/v1/schema/AnetApiSchema.xsd\">" .
          MerchantAuthenticationBlock().
          "<customerProfileId>" . $customerProfileId . "</customerProfileId>".
          "<paymentProfile>".
          "<billTo>".
           "<firstName>".$_POST['profile_firstname']."</firstName>".
           "<lastName>".$_POST['profile_lastname']."</lastName>".
  //         "<country>".$_POST['profile_country']."</country>".
           "<address>".$_POST['profile_address']."</address>".
           "<city>".$_POST['profile_city']."</city>".
           "<state>".$_POST['profile_state']."</state>".
           "<zip>".$_POST['profile_zip']."</zip>".
           "<phoneNumber>000-000-0000</phoneNumber>".
          "</billTo>".
          "<payment>".
           "<creditCard>".
            "<cardNumber>".$_POST['profile_credit_card_number']."</cardNumber>".
            "<expirationDate>".$date."</expirationDate>". // required format for API is YYYY-MM
            "<cardCode>".$_POST['profile_credit_card_security']."</cardCode>".
           "</creditCard>".
          "</payment>".
          "</paymentProfile>".
          "<validationMode>liveMode</validationMode>". // or liveMode
          "</createCustomerPaymentProfileRequest>"; // <?php

          $response = send_xml_request($content);
          $parsedresponse = parse_api_response($response);
          $customerPaymentProfileId = $parsedresponse->customerPaymentProfileId;

          // check for cc/details errors
          if ($parsedresponse->messages->resultCode == 'Error')
          {
              $msg = preg_replace("/[^a-zA-Z0-9\s]/", "", $parsedresponse->messages->message->text);
              $msg = "Credit/debit card details are invalid/expired.";

              drupal_set_message($msg, 'error');
              Header('Location:apply.html?id='.$_GET['id']);
              exit;
              // $form_state['redirect'] = 'apply.html?id='.$_GET['id'];
              return;
          }

        // add payment/customer profile to database
        $query = "DELETE FROM member_cim WHERE member_id=\"".AddSlashes($form['account']['name']['#value'])."\";";
        $result = db_query($query);

        $query = "INSERT INTO member_cim VALUES(NULL, \"".AddSlashes($form['account']['name']['#value'])."\", \"".AddSlashes($customerProfileId)."\", \"".AddSlashes($customerPaymentProfileId)."\");";
        $result = db_query($query);
    }       // END not a test credit card number
    */

    $admin = user_access('administer users');

    // Set hidden member_id to name field value
    $form_state['values']['name'] = $form_state['values']['name_h'];

    $mail = $form_state['values']['mail'];
    $name = $form_state['values']['name'];

    ncn_track_nda_log($name);   // Track IP Address

    if (!variable_get('user_email_verification', TRUE) || $admin) {
        $pass = $form_state['values']['pass'];
    }
    else {
        $pass = user_password();
    };
    $notify = isset($form_state['values']['notify']) ? $form_state['values']['notify'] : NULL;
    $from = variable_get('site_mail', ini_get('sendmail_from'));
    if (isset($form_state['values']['roles'])) {
        // Remove unset roles.
        $roles = array_filter($form_state['values']['roles']);
    }
    else {
        $roles = array(USER_ROLE_MEMBER => USER_ROLE_MEMBER, 
                       USER_ROLE_AUTHENTICATE => USER_ROLE_AUTHENTICATE);
    }

    if (!$admin && array_intersect(array_keys($form_state['values']), array('uid', 'roles', 'init', 'session', 'status'))) {
        watchdog('security', 'Detected malicious attempt to alter protected user fields.', array(), WATCHDOG_WARNING);
        $form_state['redirect'] = 'apply.html';
        return;
    }

    // The unset below is needed to prevent these form values from being saved as
    // user data.
    unset($form_state['values']['form_token'], $form_state['values']['submit'], $form_state['values']['op'], $form_state['values']['notify'], $form_state['values']['form_id'], $form_state['values']['affiliates'], $form_state['values']['destination']);
    $merge_data = array('pass' => $pass, 'init' => $mail, 'roles' => $roles, 'create_flag'=>1);

    if (!$admin) {
        // Set the user's status because it was not displayed in the form.
        $merge_data['status'] = variable_get('user_register', 1) == 1;
    }
    $account = register_user_save('', array_merge($form_state['values'], $merge_data));

    // Terminate if an error occured during user_save().
    if (!$account) {
        drupal_set_message(t("Error saving user account."), 'error');
        $form_state['redirect'] = '';
        return;
    }
    $form_state['user'] = $account;

    watchdog('user', 'New user: %name (%email).', array('%name' => $name, '%email' => $mail), WATCHDOG_NOTICE, l(t('edit'), 'user/'. $account->uid .'/edit'));

    // assign user to account manager
    // The first user may login immediately, and receives a customized welcome e-mail.
    if ($account->uid == 1) {
        drupal_set_message(t('Welcome to Drupal. You are now logged in as user #1, which gives you full control over your website.'));
        if (variable_get('user_email_verification', TRUE)) {
            drupal_set_message(t('</p><p> Your password is <strong>%pass</strong>. You may change your password below.</p>', array('%pass' => $pass)));
        }

        user_authenticate(array_merge($form_state['values'], $merge_data));

        $form_state['redirect'] = 'user/1/edit';
        return;
    }
    else {
        // Add plain text password into user account to generate mail tokens.
        $account->password = $pass;
        if ($admin && !$notify) {
            drupal_set_message(t('Created a new user account for <a href="@url">%name</a>. No e-mail has been sent.', array('@url' => url("user/$account->uid"), '%name' => $account->name)));
        }
        else if (!variable_get('user_email_verification', TRUE) && $account->status && !$admin) {
            // No e-mail verification is required, create new user account, and login
            // user immediately.
            _user_mail_notify('register_no_approval_required', $account);
            if (user_authenticate(array_merge($form_state['values'], $merge_data))) {
                drupal_set_message(t('Registration successful. You are now logged in.'));
            }
            $form_state['redirect'] = 'user/review';
            return;
        }
        else if ($account->status || $notify) {
            // Create new user account, no administrator approval required.
            $op = $notify ? 'register_admin_created' : 'register_no_approval_required';
            _user_mail_notify($op, $account);
            if ($notify) {
                drupal_set_message(t('Password and further instructions have been e-mailed to the new user <a href="@url">%name</a>.', array('@url' => url("user/$account->uid"), '%name' => $account->name)));
            }
            else {
                //drupal_set_message(t('Your password and further instructions have been sent to your e-mail address.'));
                $form_state['redirect'] = 'user/review';
                return;
            }
        }
        else {
            // Create new user account, administrator approval required.
            _user_mail_notify('register_pending_approval', $account);
            //drupal_set_message(t('Thank you for applying for an account. Your account is currently pending approval by the site administrator.<br />In the meantime, a welcome message with further instructions has been sent to your e-mail address.'));
            $form_state['redirect'] = 'user/review';
            return;
        }
    }
}


/*
 * Apply page 
 * URL: apply.html
 */
function user_apply()
{
    ob_start(); ?>
    
    <style>
    .form-item-profile-taxnumber{display:<?php echo (isset($_POST['profile_taxstatus']) && $_POST['profile_taxstatus']=='1' ? 'block':'none'); ?>;}
    .form-item-profile-fblocked,.form-item-profile-taxstatus .form-required{display:none;}
    .form-textarea{width:auto !important;}
    .grippie{width:529px;}
    </style>

    <script type="text/javascript">
        jQuery(document).ready(function(){
            jQuery("#edit-profile-taxstatus").click(function(){jQuery('.form-item-profile-taxnumber').slideToggle('slow');});
        });
    </script>
    <?php
    $content = ob_get_contents();
    ob_end_clean();

    $u_form = drupal_get_form('user_register');
    $form = drupal_render($u_form);
    //$form = str_replace('name="profile_taxstatus"', 'onchange="change_tax_status(this.checked);" name="profile_taxstatus"', $form);
    $content.= $form;
    $form_errors = form_get_errors();
    if ($form_errors)
    {
        if (isset($form_errors['next-confirm']))
        {
            $content = "<div id=\"user_register_form\" style=\"display: none;\">$content</div>";
            /*$javascript =
            "\r\t<script type='text/javascript'>
            jQuery().ready(function() {
            jQuery('#edit-user-confirm-page').val('1');
            });
            </script>";*/

            // $content .=$javascript;

            $confirm_page = get_user_confirm_page();
            $content.= $confirm_page;
        }
    }
    return $content;
}

function get_user_confirm_page()
{
    ob_start();
?>
    <div id="create_user_confirm">
        <form method="POST" id="user_confirm_form">
        <div class="title-confirm">Terms and Conditions</div>
        <div class="term-conditions"><tt><p align="justify"><?php echo variable_get('ncn_user_terms_text', ''); ?></p></tt></div>
        <div class="title-confirm" style>Non-Disclosure</div>
        <div class="term-conditions"><tt><p align="justify"><?php echo variable_get('ncn_user_nda_text', ''); ?></p></tt></div>

        <div class="confirm-agree-div">
            <input type="checkbox" name="chk_nda" id="chk_nda" onclick="on_click_chk_nda()"/>
            <label for="chk_nda">I accept the terms and non-disclosure agreement</label>&nbsp;&nbsp;
            <input type="button" id="create_user_btn" class="create-user-agree-btn" onclick="create_user_agree()" value="OK" style="width: 70px; height: 25px;" disabled>
            <!-- <span>&nbsp; or &nbsp;<a href="<?php echo $GLOBALS['base_url']; ?>/account">cancel</a></span> -->
        </div>
        </form>
    </div>
    <script>
    function on_click_chk_nda() {
        if (jQuery('#chk_nda').is(':checked')) {
            jQuery('#create_user_btn').removeAttr('disabled');
        } else {
            jQuery('#create_user_btn').attr("disabled", "disabled");
        }
    }

    function create_user_agree() {
        if (jQuery('#chk_nda').is(':checked')) {
            jQuery('#edit-user-confirm-page').val('1');
            jQuery('#user_register_form #user-register').submit();
        }
    }
    </script>
<?php
    $confirm_page = ob_get_contents();
    ob_end_clean();
    return $confirm_page;
}

/*
 * activate page
 * URL: activate.html
 */
function user_activate_page()
{
    $form = drupal_get_form('user_activate');
    $output = drupal_render($form);
    $output.= '<br /><strong>Office</strong> 877.654.8668<br /><strong>Fax</strong> 877.654.8667';
    return $output;
}
function user_activate()
{
    global $user;
    $admin = user_access('administer users');

    // If we aren't admin but already logged on, go to the user page instead.
    if (!$admin && $user->uid)
    {
        drupal_goto('user/' . $user->uid);
    }

    $form = array();
    // Account information:
    $form['account'] = array(
        '#type' => 'fieldset',
        '#title' => t('Activate Your Membership') ,
        '#weight' => - 10,
    );
    $form['account']['member_id'] = array(
        '#type' => 'textfield',
        '#default_value' => '',
        '#id' => 'edit-user-register-timezone',
        '#title' => t('Member ID') ,
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Next Step') ,
        '#weight' => 30
    );
    $form['#validate'][] = 'user_activate_validate';
    return $form;
}
function user_activate_validate($form, &$form_state)
{
    // $member_id = trim($_POST['member_id']);
    $member_id = trim($form_state['values']['member_id']);
    $result = db_query('SELECT * FROM member_id_pool WHERE member_id=:mid AND `used`=0', 
                        array(':mid' => $member_id) );
    $row_count = $result->rowCount();
    if ($row_count == 0)
    {
        form_set_error('category', t('That is not a valid membership id.'));
    }
}
function user_activate_submit($form, &$form_state)
{
    // $member_id = trim($_POST['member_id']);
    $member_id = trim($form_state['values']['member_id']);
    Header("Location:apply.html?id=" . base64_encode($member_id));
    exit;
}

function user_regiseter_question_submit($form, &$form_state)
{
}

function user_register_anwser_question($form, &$form_state)
{
    if (!$form_state[values]['service_question'])
    {
        form_set_error('service_question', t('Please choose answer.'));
    }
}

function user_register_confirm($form, &$form_state)
{
    if (!form_get_errors())
    {
        if ($_REQUEST['confirm_page'] != 1)
        {
            form_set_error('next-confirm', 'terms and condition page');
        }

        /*ob_start();

        // $form_state['values']['confirm_page'] = '1';

        print_r($form_state['values']);
        $msg = ob_get_contents();
        ob_end_clean();
        watchdog('trace', $msg, array());*/
    }
}

function user_edit_form(&$form_state, $uid, $edit, $register = FALSE)
{
    //_user_password_dynamic_validation();
    $admin = user_access('administer users');

    // Account information:
    $form['account'] = array(
        '#type' => 'fieldset',
        '#title' => t('Account information') ,
        '#weight' => - 10,
    );

    // Only show name field when: registration page; or user is editing own account and can change username; or an admin user.

    if ($register || ($GLOBALS['user']->uid == $uid && user_access('change own username')) || $admin)
    {
        $form['account']['name'] = array(
            '#type' => 'textfield',
            '#title' => t('Member ID') ,
            '#maxlength' => USERNAME_MAX_LENGTH,
            '#disabled' => TRUE,
            '#required' => TRUE,
            '#value' => base64_decode($_GET['id']),
            '#default_value' => base64_decode($_GET['id']),
        );
        $form['account']['name_h'] = array(
            '#type' => 'hidden',
            '#value' => base64_decode($_GET['id']),
        );
    }

    $form['account']['mail'] = array(
        '#type' => 'textfield',
        '#title' => t('E-mail address') ,
        '#default_value' => $edit['mail'],
        '#maxlength' => EMAIL_MAX_LENGTH,
        '#description' => t('A valid e-mail address. All e-mails from the system will be sent to this address. The e-mail address is not made public and will only be used if you wish to receive a new password or wish to receive certain news or notifications by e-mail.') ,
        '#required' => TRUE,
    );
    if (!$register)
    {
        $form['account']['pass'] = array(
            '#type' => 'password_confirm',
            '#description' => t('To change the current user password, enter the new password in both fields.') ,
            '#size' => 25,
        );
    }
    elseif (!variable_get('user_email_verification', TRUE) || $admin)
    {
        $form['account']['pass'] = array(
            '#type' => 'password_confirm',
            '#description' => t('Provide a password for the new account in both fields.') ,
            '#required' => TRUE,
            '#size' => 25,
        );
    }

    if ($admin)
    {
        $form['account']['status'] = array(
            '#type' => 'radios',
            '#title' => t('Status') ,
            '#default_value' => isset($edit['status']) ? $edit['status'] : 1,
            '#options' => array(
                t('Blocked') ,
                t('Active')
            )
        );
    }

    if (user_access('administer permissions'))
    {
        $roles = user_roles(TRUE);

        // The disabled checkbox subelement for the 'authenticated user' role
        // must be generated separately and added to the checkboxes element,
        // because of a limitation in D6 FormAPI not supporting a single disabled
        // checkbox within a set of checkboxes.
        // TODO: This should be solved more elegantly. See issue #119038.

        $checkbox_authenticated = array(
            '#type' => 'checkbox',
            '#title' => $roles[DRUPAL_AUTHENTICATED_RID],
            '#default_value' => TRUE,
            '#disabled' => TRUE,
        );
        unset($roles[DRUPAL_AUTHENTICATED_RID]);
        if ($roles)
        {
            $default = empty($edit['roles']) ? array() : array_keys($edit['roles']);
            $form['account']['roles'] = array(
                '#type' => 'checkboxes',
                '#title' => t('Roles') ,
                '#default_value' => $default,
                '#options' => $roles,
                DRUPAL_AUTHENTICATED_RID => $checkbox_authenticated,
            );
        }
    }

    // Signature:
    if (variable_get('user_signatures', 0) && module_exists('comment') && !$register)
    {
        $form['signature_settings'] = array(
            '#type' => 'fieldset',
            '#title' => t('Signature settings') ,
            '#weight' => 1,
        );
        $form['signature_settings']['signature'] = array(
            '#type' => 'textarea',
            '#title' => t('Signature') ,
            '#default_value' => $edit['signature'],
            '#description' => t('Your signature will be publicly displayed at the end of your comments.') ,
        );

        // Prevent a "validation error" message when the user attempts to save with a default value they
        // do not have access to.

        if (!filter_access($edit['signature_format']) && empty($_POST))
        {
            drupal_set_message(t("The signature input format has been set to a format you don't have access to. It will be changed to a format you have access to when you save this page."));
            $edit['signature_format'] = FILTER_FORMAT_DEFAULT;
        }

        $form['signature_settings']['signature_format'] = filter_form($edit['signature_format'], NULL, array(
            'signature_format'
        ));
    }

    // Picture/avatar:
    if (variable_get('user_pictures', 0) && !$register)
    {
        $form['picture'] = array(
            '#type' => 'fieldset',
            '#title' => t('Picture') ,
            '#weight' => 1
        );
        $picture = theme('user_picture', (object)$edit);
        if ($edit['picture'])
        {
            $form['picture']['current_picture'] = array(
                '#value' => $picture
            );
            $form['picture']['picture_delete'] = array(
                '#type' => 'checkbox',
                '#title' => t('Delete picture') ,
                '#description' => t('Check this box to delete your current picture.')
            );
        }
        else
        {
            $form['picture']['picture_delete'] = array(
                '#type' => 'hidden'
            );
        }

        $form['picture']['picture_upload'] = array(
            '#type' => 'file',
            '#title' => t('Upload picture') ,
            '#size' => 48,
            '#description' => t('Your virtual face or picture. Maximum dimensions are %dimensions and the maximum size is %size kB.', array(
                '%dimensions' => variable_get('user_picture_dimensions', '85x85') ,
                '%size' => variable_get('user_picture_file_size', '30')
            )) . ' ' . variable_get('user_picture_guidelines', '')
        );
        $form['#validate'][] = 'user_profile_form_validate';
        $form['#validate'][] = 'user_validate_picture';
    }

    $form['#uid'] = $uid;
    return $form;
}

/**
 * Retrieve a list of all form elements for the specified category.
 */
function _user_forms(&$edit, $account, $category, $hook = 'form') {
    module_load_include('pages.inc', 'user', 'user');

    $groups = array();

    if(isset($_POST['op'])){
        if($edit==NULL){
            $edit = new stdClass();
        }
        foreach($_POST as $key=>$val) {
            if (strpos($key, 'profile_') === 0) {
                $edit->$key = $val;
            }
        }
    }

    $groups = array_merge(drupal_get_form('user_profile_form',$edit,'Distributor'), $groups);
    $groups = array_merge(drupal_get_form('user_profile_form',$edit,'Operations Information'), $groups);
    $groups = array_merge(drupal_get_form('user_profile_form',$edit,'Principal Owners'), $groups);
    $groups = array_merge(drupal_get_form('user_profile_form',$edit,'Company Information'), $groups);

    // patch in the firstname/lastname
    if (isset($_GET['id']))
    {
        //echo 'SELECT * FROM member_id_pool WHERE member_id='.base64_decode($_GET['id']);
        $result = db_query('SELECT * FROM member_id_pool WHERE member_id=:mid',array(':mid'=>base64_decode($_GET['id'])));
        $row_count = $result->rowCount();

        if ($row_count > 0)
        {
            $row = $result->fetchAssoc();

            $groups['Company Information']['profile_firstname']['#default_value'] = $row['first_name'];
            $groups['Company Information']['profile_firstname']['#value'] = $row['first_name'];
            $groups['Company Information']['profile_firstname']['#attributes'] = array('readonly' => 'readonly', 'style' => 'background-color:#EBEBE4;');;

            $groups['Company Information']['profile_lastname']['#default_value'] = $row['last_name'];
            $groups['Company Information']['profile_lastname']['#value'] = $row['last_name'];
            $groups['Company Information']['profile_lastname']['#attributes'] = array('readonly' => 'readonly', 'style' => 'background-color:#EBEBE4;');;

            $groups['Company Information']['profile_legalname']['#default_value'] = $row['legalname'];
            $groups['Company Information']['profile_legalname']['#value'] = $row['legalname'];
            $groups['Company Information']['profile_legalname']['#attributes'] = array('readonly' => 'readonly', 'style' => 'background-color:#EBEBE4;');;

            $groups['Company Information']['profile_country']['#default_value'] = $row['country'];
            $groups['Company Information']['profile_country']['#value'] = $row['country'];
            $groups['Company Information']['profile_country']['#attributes'] = array('readonly' => 'readonly', 'style' => 'background-color:#EBEBE4;');;

            $groups['Company Information']['profile_address']['#default_value'] = $row['address'];
            $groups['Company Information']['profile_address']['#value'] = $row['address'];
            $groups['Company Information']['profile_address']['#attributes'] = array('readonly' => 'readonly', 'style' => 'background-color:#EBEBE4;');;

            $groups['Company Information']['profile_city']['#default_value'] = $row['city'];
            $groups['Company Information']['profile_city']['#value'] = $row['city'];
            $groups['Company Information']['profile_city']['#attributes'] = array('readonly' => 'readonly', 'style' => 'background-color:#EBEBE4;');;

            $groups['Company Information']['profile_state']['#default_value'] = $row['state'];
            $groups['Company Information']['profile_state']['#value'] = $row['state'];
            $groups['Company Information']['profile_state']['#attributes'] = array('readonly' => 'readonly', 'style' => 'background-color:#EBEBE4;');;

            $groups['Company Information']['profile_zip']['#default_value'] = $row['zip'];
            $groups['Company Information']['profile_zip']['#value'] = $row['zip'];
            $groups['Company Information']['profile_zip']['#attributes'] = array('readonly' => 'readonly', 'style' => 'background-color:#EBEBE4;');;

            $groups['Company Information']['profile_officephone']['#default_value'] = $row['officephone'];
            $groups['Company Information']['profile_officephone']['#value'] = $row['officephone'];
            $groups['Company Information']['profile_officephone']['#attributes'] = array('readonly' => 'readonly', 'style' => 'background-color:#EBEBE4;');;

            if ($row['mobilephone']) {
                $groups['Company Information']['profile_mobilephone']['#default_value'] = $row['mobilephone'];
                $groups['Company Information']['profile_mobilephone']['#value'] = $row['mobilephone'];
            }
            //$groups['Company Information']['profile_mobilephone']['#attributes'] = array('readonly' => 'readonly', 'style' => 'background-color:#EBEBE4;');;

            $groups['Company Information']['profile_contactemail']['#default_value'] = $row['contactemail'];
            $groups['Company Information']['profile_contactemail']['#value'] = $row['contactemail'];
            $groups['Company Information']['profile_contactemail']['#attributes'] = array('readonly' => 'readonly', 'style' => 'background-color:#EBEBE4;');;

            $groups['Company Information']['profile_fblocked']['#default_value'] = 'first';
            $groups['Company Information']['profile_fblocked']['#value'] = 'first';
            //$groups['Company Information']['profile_fblocked']['#attributes'] = array('style'=>'display: none;');

            // make others readonly
            if (trim($row['sales_rep']) != '')
            {
                $groups['Distributor']['profile_sales_rep']['#default_value'] = $row['sales_rep'];
                $groups['Distributor']['profile_sales_rep']['#value'] = $row['sales_rep'];
                $groups['Distributor']['profile_sales_rep']['#attributes'] = array('readonly' => 'readonly', 'style' => 'background-color:#EBEBE4;');;
            }

            $groups['Distributor']['profile_distributor']['#default_value'] = $row['distributor'];
            $groups['Distributor']['profile_distributor']['#value'] = $row['distributor'];
            $groups['Distributor']['profile_distributor']['#attributes'] = array('readonly' => 'readonly', 'style' => 'background-color:#EBEBE4;');;

            // has the administrator already set the credit card details?
            $result2 = db_query('SELECT * FROM member_cim WHERE member_id=:mid',array(':mid'=>base64_decode($_GET['id'])));
            $row_count2 = $result2->rowCount();

            if ($row_count2 != 0)
            {
                $row2 = $result2->fetchAssoc();

                // check for free
                if ($row2['customerProfileId'] == "FREE")
                {
                    // add hidden credit card details for free account
                    unset($groups['Credit Card Information']['profile_credit_card_type']);
                    unset($groups['Credit Card Information']['profile_credit_card_expiration']);
                    unset($groups['Credit Card Information']['profile_credit_card_security']);
                    unset($groups['Credit Card Information']['profile_credit_card_name']);

                    unset($groups['Credit Card Information']['#type']);
                    //$groups['Credit Card Information']['profile_credit_card_number']['#type'] = "hidden";
                    //$groups['Credit Card Information']['profile_credit_card_number']['#default_value'] = "5424000000000015";
                }
                else
                {
                    //unset($groups['Credit Card Information']);
                    unset($groups['Credit Card Information']['profile_credit_card_type']);
                    unset($groups['Credit Card Information']['profile_credit_card_number']);
                    unset($groups['Credit Card Information']['profile_credit_card_expiration']);

                    if (trim($row['security_code'])=='') {
                        unset($groups['Credit Card Information']['profile_credit_card_security']);
                    } else {
                        $groups['Credit Card Information']['profile_credit_card_security']['#type'] = "hidden";
                        $groups['Credit Card Information']['profile_credit_card_security']['#default_value'] = $row['security_code'];
                    }

                    unset($groups['Credit Card Information']['profile_credit_card_name']);
                    $groups['Credit Card Information']['#attributes'] = array('style'=>'display: none;');
                }

            }   // END cc profile already set
            else {
                unset($groups['Credit Card Information']);
            }
        }
        unset($groups['Company Information']['profile_memberid']);
        unset($groups['Company Information']['profile_blocked_time']);
        unset($groups['Principal Owners']['profile_po_zip']);
        unset($groups['actions']['submit']);
    }
    $groups['#submit'] = array();
    $groups['#submit'][] = 'user_register_form_submit';
    $groups['#validate'] = array();
    $groups['#validate'][] = 'user_extra_profile_form_validate';

    /*echo '<pre>';
    print_r($groups);
    echo '</pre>';*/

    return empty($groups) ? FALSE : $groups;
    return FALSE;
}

/**
 * Save changes to a user account or add a new user.
 *
 * @param $account
 *   The $user object for the user to modify or add. If $user->uid is
 *   omitted, a new user will be added.
 *
 * @param $array
 *   (optional) An array of fields and values to save. For example,
 *   array('name' => 'My name'); Setting a field to NULL deletes it from
 *   the data column.
 *
 * @param $category
 *   (optional) The category for storing profile information in.
 *
 * @return
 *   A fully-loaded $user object upon successful save or FALSE if the save failed.
 */

function register_user_save($account, $array = array(), $category = 'account') {

    // check if this is to created a new user.
    $user_pass = '';
    if ( isset($array['create_flag']) && $array['create_flag']==1 ) {

        // check for admin
        //if (($GLOBALS['user']->uid != 1) && ($GLOBALS['user']->uid != 6)  && (is_distributor($GLOBALS['user'])!=true) && (is_associate($GLOBALS['user'])!=true) && (is_account_manager($GLOBALS['user'])!=true) )
        //{
        $profile_memberid = $array['name'];     // save for later

        $array['name'] = get_new_username($array['profile_firstname'].$array['profile_lastname']);
        //$array['pass'] = date("j").preg_replace("/[^a-zA-Z0-9]/", "", $array['profile_firstname']).date("Y");
        $array['pass'] = get_member_password($profile_memberid, $array['profile_firstname']);
        $user_pass = $array['pass'];
        $array['password'] = $array['pass'];
        //}
        //else
        //{
        //$array['password'] = date("j").preg_replace("/[^a-zA-Z0-9]/", "", $account->profile_firstname).date("Y");
        //}

    } // check if this is to update status ( blocked -> active )
    else if (is_object($account) && $account->uid && $array['status'] == 1 && $account->status == 0) {

        $_user1 = user_load($account->uid);
        if (isset($_user1->profile_memberid) && is_member($_user1->profile_memberid)) {
            $_fblocked = '';
            if ( isset($_user1->profile_fblocked) ) {
                $_fblocked = $_user1->profile_fblocked;
            }

            $array['pass'] = get_member_password($_user1->profile_memberid, $account->profile_firstname);
            $user_pass = $array['pass'];
            $array['password'] = $array['pass'];

            //$from_email = variable_get('site_mail', ini_get('sendmail_from'));
            $member = get_member_from_id($_user1->profile_memberid);
            $am_uid = $member['am_uid'];
            $am = user_load($am_uid);
            $params = array(
                '!email'    => $_user1->mail, 
                '!username' => $_user1->name,
                '!password' => $user_pass,
                '!am' => $am,
                '!am_fax' => ncn_admin_get_am_fax($am->uid)
            );
            $login_mail_sent = true;
            drupal_mail('ncn_admin', 'member_login', $_user1->mail, language_default(), $params);
            if ($_fblocked == 'first') {
                //$array['password'] = date("j", $account->created).preg_replace("/[^a-zA-Z0-9]/", "", $account->profile_firstname).date("Y", $account->created);
                $_query = "UPDATE profile_value SET value='no first' WHERE fid=64 AND uid=".$_user1->uid;   // profile_fblocked : 64
                $result = db_query($_query);
            }
        }
    }
    else if (is_object($account) && $account->uid && $array['status'] == 0 && $account->status == 0) {
        // No First
        $_user1 = user_load($account->uid);
        $_query = "UPDATE profile_value SET value='no first' WHERE fid=64 AND uid=".$_user1->uid;   // profile_fblocked : 64
        $result = db_query($_query);

        $array['pass'] = get_member_password($_user1->profile_memberid, $account->profile_firstname);
        $user_pass = $array['pass'];
        $array['password'] = $array['pass'];
    }

    // Dynamically compose a SQL query:
    $user_fields = user_fields();

    if (is_object($account) && $account->uid) {

        user_module_invoke('update', $array, $account, $category);
        $query = '';
        $data = unserialize(db_result(db_query('SELECT data FROM {users} WHERE uid = %d', $account->uid)));

        // Consider users edited by an administrator as logged in, if they haven't
        // already, so anonymous users can view the profile (if allowed).
        if (empty($array['access']) && empty($account->access) && user_access('administer users')) {
            $array['access'] = time();
        }
        foreach ($array as $key => $value) {
            if ($key == 'pass' && !empty($value)) {
                require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
                $query .= "$key = '%s', ";
                $v[] = user_hash_password(trim($value));
            }
            else if ((substr($key, 0, 4) !== 'auth') && ($key != 'pass')) {
                if (in_array($key, $user_fields)) {
                    // Save standard fields.
                    $query .= "$key = '%s', ";
                    $v[] = $value;
                }
                else if ($key != 'roles') {
                    // Roles is a special case: it used below.
                    if ($value === NULL) {
                        unset($data[$key]);
                    }
                    elseif (!empty($key)) {
                        $data[$key] = $value;
                    }
                }
            }
        }
        $query .= "data = '%s' ";

        $v[] = serialize($data);

        $success = db_query("UPDATE {users} SET $query WHERE uid = %d", array_merge($v, array($account->uid)));

        if (!$success) {
            // The query failed - better to abort the save than risk further data loss.
            return FALSE;
        }

        // Reload user roles if provided.
        if (isset($array['roles']) && is_array($array['roles'])) {
            db_query('DELETE FROM {users_roles} WHERE uid = %d', $account->uid);

            foreach (array_keys($array['roles']) as $rid) {
                if (!in_array($rid, array(DRUPAL_ANONYMOUS_RID, DRUPAL_AUTHENTICATED_RID))) {
                    db_query('INSERT INTO {users_roles} (uid, rid) VALUES (%d, %d)', $account->uid, $rid);
                }
            }
        }

        // Delete a blocked user's sessions to kick them if they are online.
        if (isset($array['status']) && $array['status'] == 0) {
            sess_destroy_uid($account->uid);
        }

        // If the password changed, delete all open sessions and recreate
        // the current one.
        if (!empty($array['pass'])) {
            sess_destroy_uid($account->uid);
            if ($account->uid == $GLOBALS['user']->uid) {
                sess_regenerate();
            }
        }

        // Refresh user object.
        $user = user_load(array('uid' => $account->uid));

        // Send emails after we have the new user object.
        if (isset($array['status']) && $array['status'] != $account->status) {
            // The user's status is changing; conditionally send notification email.
            $op = $array['status'] == 1 ? 'status_activated' : 'status_blocked';
            if (empty($user->password)) {
                $user->password = $user_pass;
            }
            if (!$login_mail_sent) {
                _user_mail_notify($op, $user);
            }
        } else if (isset($array['status']) && $array['status'] == $account->status && $account->status==0) {    // training -> blocked
            $op = $array['status'] == 1 ? 'status_activated' : 'status_blocked';
            if (empty($user->password)) {
                $user->password = $user_pass;
            }
            if (!$login_mail_sent) {
                _user_mail_notify($op, $user);
            }
        }

        user_module_invoke('after_update', $array, $user, $category);
    }
    else {

        // Allow 'created' to be set by the caller.
        if (!isset($array['created'])) {
            $array['created'] = time();
        }
        // Consider users created by an administrator as already logged in, so
        // anonymous users can view the profile (if allowed).
        if (empty($array['access']) && user_access('administer users')) {
            $array['access'] = time();
        }

        // Note: we wait to save the data column to prevent module-handled
        // fields from being saved there. We cannot invoke hook_user('insert') here
        // because we don't have a fully initialized user object yet.
        foreach ($array as $key => $value) {
            switch ($key) {
                case 'pass':$fields[] = $key;
                            $values[] = $value;
                            $s[] = "'%s'";
                            break;
                case 'mode':
                case 'sort':
                case 'timezone':
                case 'threshold':
                case 'created':
                case 'access':
                case 'login':
                case 'status': $fields[] = $key;
                               $values[] = $value;
                               $s[] = "%d";
                               break;
                default:    if (substr($key, 0, 4) !== 'auth' && in_array($key, $user_fields)) {
                                $fields[] = $key;
                                $values[] = $value;
                                $s[] = "'%s'";
                            }
                            break;
            }
        }

        watchdog('user-trace', $array['pass'], array());
        //$success = db_query('INSERT INTO {users} ('. implode(', ', $fields) .') VALUES ('. implode(', ', $values) .')');
        $usersaveArray = array_combine($fields,$values);
        $success = user_save('', array_merge($array, $usersaveArray));

        if (!$success) {
            // On a failed INSERT some other existing user's uid may be returned.
            // We must abort to avoid overwriting their account.
            return FALSE;
        }

        // Build the initial user object.
        $array['uid'] = $success->uid;
        $user = user_load($array['uid']);

        user_module_invoke('insert', $array, $user, $category);

        // insert the member id
        $result = db_query("SELECT fid FROM profile_field WHERE name='profile_memberid'");
        $row = $result->fetchAssoc();

        $result = db_query("INSERT INTO profile_value VALUES (:fid,:uid,:mid)",
                            array(':fid'=>$row['fid'],':uid'=>$array['uid'],':mid'=>$profile_memberid));

        $result = db_query("UPDATE member_id_pool SET used=1 WHERE member_id=:mid",
                            array(':mid'=>$profile_memberid));

        // Build and save the serialized data field now.
        $data = array();
        foreach ($array as $key => $value) {
            if ((substr($key, 0, 4) !== 'auth') && ($key != 'roles') && (!in_array($key, $user_fields)) && ($value !== NULL)) {
                $data[$key] = $value;
            }
        }
        db_query("UPDATE {users} SET data = :d WHERE uid = :uid", array(':d'=>serialize($data),':uid'=>$user->uid));

        // Save user roles (delete just to be safe).
        if (isset($array['roles']) && is_array($array['roles'])) {
            db_query('DELETE FROM {users_roles} WHERE uid = :d', array(':d'=>$array['uid']));
            foreach (array_keys($array['roles']) as $rid) {
                if (!in_array($rid, array(DRUPAL_ANONYMOUS_RID, DRUPAL_AUTHENTICATED_RID))) {
                    db_query('INSERT INTO {users_roles} (uid, rid) VALUES (:uid,:rid)',
                              array(':uid'=>$array['uid'],':rid'=> $rid));
                }
            }
        }

        // Build the finished user object.
        // drupal_set_message('User ID: '. $array['uid'] . ' - ' . $user->uid);
        
    }

    // Save distributed authentication mappings.
    $authmaps = array();
    foreach ($array as $key => $value) {
        if (substr($key, 0, 4) == 'auth') {
            $authmaps[$key] = $value;
        }
    }
    if (sizeof($authmaps) > 0) {
        user_set_authmaps($user, $authmaps);
    }

    return $user;
}

// used in "user_save()" below...
function get_new_username($name, $next = 2)
{
    $name = preg_replace("/[^a-zA-Z0-9]/", "", strtolower($name));

    $result = db_query("SELECT * FROM users WHERE name=:name",array(':name'=>$name));
    $row_count = $result->rowCount();

    if ($row_count != 0)
    {
        while (1)
        {
            $new_name = $name.$next++;
            $result_n = db_query("SELECT * FROM users WHERE name=:name",array(':name'=>$new_name));
            $row_count_n = $result_n->rowCount();

            if ($row_count_n == 0)
            {   return $new_name;   }
        }
    }

    return($name);
}

function array_unshift_assoc(&$arr, $key, $val) {
    $arr = array_reverse($arr, true);
    $arr[$key] = $val;
    $arr = array_reverse($arr, true);
    return count($arr);
}

function user_fields() {
    static $fields;

    if (!$fields) {
        $result = db_query('SELECT * FROM {users} WHERE uid = 1');
        if ($result->rowCount()>0) {
            $field = $result->fetchAssoc();
            $fields = array_keys($field);
        }
        else {
            // Make sure we return the default fields at least.
            $fields = array('uid', 'name', 'pass', 'mail', 'picture', 'mode', 'sort', 'threshold', 'theme', 'signature', 'signature_format', 'created', 'access', 'login', 'status', 'timezone', 'language', 'init', 'data');
        }
    }

    return $fields;
}
