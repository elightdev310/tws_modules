<?php

// product/payment processor settings
function ncn_admin_page_product_settings()
{
    // check for access denied
    if (isset($GLOBALS['user']->roles[6]))
    {
        return("Access denied.");
    }


    drupal_set_title("Net Claims Now Email Settings");
    return drupal_get_form('ncn_admin_product_settings_form');
}

//------------------------------------------------------------------------------
// product/ordering settings form definition
function ncn_admin_product_settings_form($form_state)
{
    $form = array();
    $form['#attributes'] = array('enctype' => "multipart/form-data");

/*
        $form['pd'] = array(
            '#type' => 'fieldset', 
            '#title' => t('Product details'), 
            '#weight' => 0, 
        );

        $form['pd']['product_title'] = array(
            '#type' => 'textfield', 
            '#title' => t('Product title (residential)'),
            '#default_value' => variable_get('ncn_product_title', ''),
        );

        $form['pd']['product_price'] = array(
            '#type' => 'textfield', 
            '#title' => t('Product price (residential)'),
            '#default_value' => variable_get('ncn_product_price', ''),
            '#field_prefix' => '$',
            '#size' => 7,
        );

        $form['pd']['product_title_commercial'] = array(
            '#type' => 'textfield', 
            '#title' => t('Product title (commercial)'),
            '#default_value' => variable_get('ncn_product_title_commercial', ''),
        );

        $form['pd']['product_price_commercial'] = array(
            '#type' => 'textfield', 
            '#title' => t('Product price (commercial)'),
            '#default_value' => variable_get('ncn_product_price_commercial', ''),
            '#field_prefix' => '$',
            '#size' => 7,
        );
*/      

        // terms and conditions
        $form['utac'] = array(
            '#type' => 'fieldset', 
            '#title' => t('Terms and Conditions For New User'), 
            '#weight' => 1, 
        );

        $form['utac']['user_terms_text'] = array(
            '#type' => 'textarea', 
            '#title' => t('Terms and conditions (Create a New User)'),
            '#default_value' => variable_get('ncn_user_terms_text', ''),
        );
        
        $form['ndanu'] = array(
            '#type' => 'fieldset', 
            '#title' => t('Non-Disclosure Agreement For New User'), 
            '#weight' => 2, 
        );

        $form['ndanu']['user_nda_text'] = array(
            '#type' => 'textarea', 
            '#title' => t('Non-Disclosure Agreement (Create a New User)'),
            '#default_value' => variable_get('ncn_user_nda_text', ''),
        );
        
        
        // terms and conditions
        $form['tac'] = array(
            '#type' => 'fieldset', 
            '#title' => t('Terms and Conditions for Gold Member'), 
            '#weight' => 3, 
        );

        $form['tac']['terms_text'] = array(
            '#type' => 'textarea', 
            '#title' => t('Terms and conditions (pre-ordering)'),
            '#default_value' => variable_get('ncn_terms_text', ''),
        );
        
        $form['tac_silver'] = array(
            '#type' => 'fieldset', 
            '#title' => t('Terms and Conditions for Silver Member'), 
            '#weight' => 4, 
        );

        $form['tac_silver']['terms_text_silver'] = array(
            '#type' => 'textarea', 
            '#title' => t('Terms and conditions (pre-ordering)'),
            '#default_value' => variable_get('ncn_terms_text_silver', ''),
        );
        
        $form['tac_gold_lite'] = array(
            '#type' => 'fieldset', 
            '#title' => t('Terms and Conditions for Gold Lite Member'), 
            '#weight' => 4, 
        );

        $form['tac_gold_lite']['terms_text_gold_lite'] = array(
            '#type' => 'textarea', 
            '#title' => t('Terms and conditions (pre-ordering)'),
            '#default_value' => variable_get('ncn_terms_text_gold_lite', ''),
        );
        
        $form['tac_coach_on_call'] = array(
            '#type' => 'fieldset', 
            '#title' => t('Terms and Conditions for Coach on Call Member'), 
            '#weight' => 4, 
        );

        $form['tac_coach_on_call']['terms_text_coach_on_call'] = array(
            '#type' => 'textarea', 
            '#title' => t('Terms and conditions (pre-ordering)'),
            '#default_value' => variable_get('ncn_terms_text_coach_on_call', ''),
        );
            
        
        // terms and conditions for creating a new user in distributor page
        $form['tnud'] = array(
            '#type' => 'fieldset', 
            '#title' => t('Terms and conditions (for Distributor)'), 
            '#weight' => 5, 
        );

        $form['tnud']['terms_text_dist'] = array(
            '#type' => 'textarea', 
            '#title' => t('Terms and conditions (creating a new user)'),
            '#default_value' => variable_get('ncn_terms_text_dist', ''),
        );
        
        // terms and conditions for creating a new user in associate page
        $form['tnua'] = array(
            '#type' => 'fieldset', 
            '#title' => t('Terms and Conditions for Associate'), 
            '#weight' => 6, 
        );

        $form['tnua']['terms_text_asso'] = array(
            '#type' => 'textarea', 
            '#title' => t('Terms and conditions (creating a new user)'),
            '#default_value' => variable_get('ncn_terms_text_asso', ''),
        );
        
        // Message and Alerts
        $form['maa'] = array(
            '#type' => 'fieldset', 
            '#title' => t('Message and Alerts'), 
            '#weight' => 9, 
        );
            
        $form['maa']['msg_title'] = array(
            '#type' => 'textfield', 
            '#title' => t('Title'),
            '#default_value' => variable_get('ncn_msg_alert_title', ''),
        );
            
        $form['maa']['body_text'] = array(
            '#type' => 'textarea', 
            '#title' => t('Body Text'),
            '#default_value' => variable_get('ncn_msg_alert_text', ''),
        );
            

    $form['ssubmit'] = array('#type' => 'submit', '#weight' => 101, '#value' => t('Save Settings'));
    
    return($form);
}

//------------------------------------------------------------------------------
// save product settings
function ncn_admin_product_settings_form_submit($form, &$form_state)
{
/*
    variable_set('ncn_product_title', t($form_state['values']['product_title']));
    variable_set('ncn_product_price', t($form_state['values']['product_price']));  
    variable_set('ncn_product_title_commercial', t($form_state['values']['product_title_commercial']));
    variable_set('ncn_product_price_commercial', t($form_state['values']['product_price_commercial']));
*/
      
    variable_set('ncn_user_terms_text', t($form_state['values']['user_terms_text']));  
    variable_set('ncn_user_nda_text', t($form_state['values']['user_nda_text']));  
    variable_set('ncn_terms_text', t($form_state['values']['terms_text']));  
    variable_set('ncn_terms_text_dist', t($form_state['values']['terms_text_dist']));  
    variable_set('ncn_terms_text_asso', t($form_state['values']['terms_text_asso']));  
    
    variable_set('ncn_terms_text_silver', t($form_state['values']['terms_text_silver']));  
    variable_set('ncn_terms_text_gold_lite', t($form_state['values']['terms_text_gold_lite']));  
    variable_set('ncn_terms_text_coach_on_call', t($form_state['values']['terms_text_coach_on_call']));  
    variable_set('ncn_msg_alert_title', t($form_state['values']['msg_title']));  
    variable_set('ncn_msg_alert_text', t($form_state['values']['body_text']));  
    
    // success!
    drupal_set_message('Product/ordering settings saved', 'status');
}


//------------------------------------------------------------------------------
// return email settings form
function ncn_admin_page_email_settings()
{
    // check for access denied
    if (isset($GLOBALS['user']->roles[6]))
    {
        return("Access denied.");
    }

    drupal_set_title("Net Claims Now Email Settings");
    return drupal_get_form('ncn_admin_email_settings_form');
}

//------------------------------------------------------------------------------
// email settings form definition
function ncn_admin_email_settings_form($form_state)
{
    $form = array();
    $form['#attributes'] = array('enctype' => "multipart/form-data");

    // new claim
    $form['front_end'] = array(
        '#type' => 'fieldset',
        '#title' => t('Front end contact form'),
        '#weight' => -1,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form['front_end']['front_end_subject'] = array(
        '#type' => 'textfield',
        '#title' => t('Subject'),
        '#default_value' => variable_get('ncn_front_end_subject', ''),
    );
    $form['front_end']['front_end_body'] = array(
        '#type' => 'textarea',
        '#title' => t('Body'),
        '#default_value' => variable_get('ncn_front_end_body', ''),
    );

    // new claim customer
    $form['new_claim_customer'] = array(
        '#type' => 'fieldset',
        '#title' => t('New claim user confirmation'),
        '#weight' => 0,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form['new_claim_customer']['new_claim_customer_subject'] = array(
        '#type' => 'textfield',
        '#title' => t('Subject'),
        '#default_value' => variable_get('ncn_new_claim_customer_subject', ''),
    );
    $form['new_claim_customer']['new_claim_customer_body'] = array(
        '#type' => 'textarea',
        '#title' => t('Body'),
        '#default_value' => variable_get('ncn_new_claim_customer_body', ''),
    );

    // new claim
    $form['new_claim'] = array(
        '#type' => 'fieldset',
        '#title' => t('New claim administrator confirmation'),
        '#weight' => 1,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form['new_claim']['new_claim_subject'] = array(
        '#type' => 'textfield',
        '#title' => t('Subject'),
        '#default_value' => variable_get('ncn_new_claim_subject', ''),
    );
    $form['new_claim']['new_claim_body'] = array(
        '#type' => 'textarea',
        '#title' => t('Body'),
        '#default_value' => variable_get('ncn_new_claim_body', ''),
    );
    $form['new_claim']['new_claim_active'] = array(
        '#type' => 'checkbox',
        '#title' => t('Send new claim confirmation to administrator?'),
        '#default_value' => variable_get('ncn_new_claim_active', ''),
    );

    // user clicked "create my invoice"
    $form['create_my_invoice'] = array(
        '#type' => 'fieldset',
        '#title' => t('Create My Invoice Administrator/Account Manager confirmation'),
        '#weight' => 2,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form['create_my_invoice']['create_my_invoice_subject'] = array(
        '#type' => 'textfield',
        '#title' => t('Subject'),
        '#default_value' => variable_get('ncn_create_my_invoice_subject', ''),
    );
    $form['create_my_invoice']['create_my_invoice_body'] = array(
        '#type' => 'textarea',
        '#title' => t('Body'),
        '#default_value' => variable_get('ncn_create_my_invoice_body', ''),
    );

    // approved
    $form['approved'] = array(
        '#type' => 'fieldset',
        '#title' => t('Claim approval notification (user)'),
        '#weight' => 3,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form['approved']['approved_subject'] = array(
        '#type' => 'textfield',
        '#title' => t('Subject'),
        '#default_value' => variable_get('ncn_approved_subject', ''),
    );
    $form['approved']['approved_body'] = array(
        '#type' => 'textarea',
        '#title' => t('Body'),
        '#default_value' => variable_get('ncn_approved_body', ''),
    );
    $form['approved']['approved_active'] = array(
        '#type' => 'checkbox',
        '#title' => t('Send claim approval notice to user'),
        '#default_value' => variable_get('ncn_approved_active', ''),
    );

    // Message
    $form['generic'] = array(
        '#type' => 'fieldset',
        '#title' => t('Claim message notification (user)'),
        '#weight' => 4,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form['generic']['generic_subject'] = array(
        '#type' => 'textfield',
        '#title' => t('Subject'),
        '#default_value' => variable_get('ncn_generic_subject', ''),
    );
    $form['generic']['generic_body'] = array(
        '#type' => 'textarea',
        '#title' => t('Body'),
        '#default_value' => variable_get('ncn_generic_body', ''),
    );

    // rejected
    $form['rejected'] = array(
        '#type' => 'fieldset',
        '#title' => t('Claim rejection notification (user)'),
        '#weight' => 5,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form['rejected']['rejected_subject'] = array(
        '#type' => 'textfield',
        '#title' => t('Subject'),
        '#default_value' => variable_get('ncn_rejected_subject', ''),
    );
    $form['rejected']['rejected_body'] = array(
        '#type' => 'textarea',
        '#title' => t('Body'),
        '#default_value' => variable_get('ncn_rejected_body', ''),

    );

    // activation email
    $form['id_to_member'] = array(
        '#type' => 'fieldset',
        '#title' => t('Member Activation Email'),
        '#weight' => 6,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form['id_to_member']['id_to_member_subject'] = array(
        '#type' => 'textfield',
        '#title' => t('Subject'),
        '#default_value' => variable_get('ncn_id_to_member_subject', ''),
    );
    $form['id_to_member']['id_to_member_body'] = array(
        '#type' => 'textarea',
        '#title' => t('Body'),
        '#default_value' => variable_get('ncn_id_to_member_body', ''),
    );

    // account manager signup
    $form['accountmanager_signup'] = array(
        '#type' => 'fieldset',
        '#title' => t('Account Manager Signup Email'),
        '#weight' => 7,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form['accountmanager_signup']['accountmanager_signup_subject'] = array(
        '#type' => 'textfield',
        '#title' => t('Subject'),
        '#default_value' => variable_get('ncn_accountmanager_signup_subject', ''),
    );
    $form['accountmanager_signup']['accountmanager_signup_body'] = array(
        '#type' => 'textarea',
        '#title' => t('Body'),
        '#default_value' => variable_get('ncn_accountmanager_signup_body', ''),
    );

    // distributor signup
    $form['distributor_signup'] = array(
        '#type' => 'fieldset',
        '#title' => t('Distributor Signup Email'),
        '#weight' => 8,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form['distributor_signup']['distributor_signup_subject'] = array(
        '#type' => 'textfield',
        '#title' => t('Subject'),
        '#default_value' => variable_get('ncn_distributor_signup_subject', ''),
    );
    $form['distributor_signup']['distributor_signup_body'] = array(
        '#type' => 'textarea',
        '#title' => t('Body'),
        '#default_value' => variable_get('ncn_distributor_signup_body', ''),
    );

    // timer_alert
    $form['timer_alert'] = array(
        '#type' => 'fieldset',
        '#title' => t('Claim Timer Alert When 2 Hours Remained'),
        '#weight' => 9,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form['timer_alert']['timer_alert_subject'] = array(
        '#type' => 'textfield',
        '#title' => t('Subject'),
        '#default_value' => variable_get('ncn_timer_alert_subject', ''),
    );
    $form['timer_alert']['timer_alert_body'] = array(
        '#type' => 'textarea',
        '#title' => t('Body'),
        '#default_value' => variable_get('ncn_timer_alert_body', ''),
    );

    // timer_expired
    $form['timer_expired'] = array(
        '#type' => 'fieldset',
        '#title' => t('Message When Claim Timer Expired'),
        '#weight' => 10,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form['timer_expired']['timer_expired_subject'] = array(
        '#type' => 'textfield',
        '#title' => t('Subject'),
        '#default_value' => variable_get('ncn_timer_expired_subject', ''),
    );
    $form['timer_expired']['timer_expired_body'] = array(
        '#type' => 'textarea',
        '#title' => t('Body'),
        '#default_value' => variable_get('ncn_timer_expired_body', ''),
    );

    // associate signup
    $form['associate_signup'] = array(
        '#type' => 'fieldset',
        '#title' => t('Associate Signup Email'),
        '#weight' => 11,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form['associate_signup']['associate_signup_subject'] = array(
        '#type' => 'textfield',
        '#title' => t('Subject'),
        '#default_value' => variable_get('ncn_associate_signup_subject', ''),
    );
    $form['associate_signup']['associate_signup_body'] = array(
        '#type' => 'textarea',
        '#title' => t('Body'),
        '#default_value' => variable_get('ncn_associate_signup_body', ''),
    );

    // Rejected Claim Docs Mail
    $form['rejected_claim_docs_mail'] = array(
        '#type' => 'fieldset',
        '#title' => t('Rejected Claim Documents - Auto Mail'),
        '#weight' => 12,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form['rejected_claim_docs_mail']['rejected_claim_docs_mail_subject'] = array(
        '#type' => 'textfield',
        '#title' => t('Subject'),
        '#default_value' => variable_get('ncn_rejected_claim_docs_mail_subject', ''),
    );
    $form['rejected_claim_docs_mail']['rejected_claim_docs_mail_body'] = array(
        '#type' => 'textarea',
        '#title' => t('Body'),
        '#default_value' => variable_get('ncn_rejected_claim_docs_mail_body', ''),
    );

    // ESX PDF Uploaded Auto Mail
    $form['esx_pdf_uploaded'] = array(
        '#type' => 'fieldset',
        '#title' => t('ESX PDF Uploaded - Auto Mail'),
        '#weight' => 13,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form['esx_pdf_uploaded']['esx_pdf_uploaded_body'] = array(
        '#type' => 'textarea',
        '#title' => t('Body'),
        '#default_value' => variable_get('ncn_claim_status_esx_pdf_uploaded', ''),
    );

    // New Sub-user
    $form['id_to_subuser'] = array(
        '#type' => 'fieldset',
        '#title' => t('New Sub User Added Email'),
        '#weight' => 14,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form['id_to_subuser']['id_to_subuser_subject'] = array(
        '#type' => 'textfield',
        '#title' => t('Subject'),
        '#default_value' => variable_get('ncn_id_to_subuser_subject', ''),
    );
    $form['id_to_subuser']['id_to_subuser_body'] = array(
        '#type' => 'textarea',
        '#title' => t('Body'),
        '#default_value' => variable_get('ncn_id_to_subuser_body', ''),
    );

    // Save Button
    $form['create_an_invoice'] = array(
        '#type' => 'submit',
        '#weight' => 15,
        '#value' => t('Save Email Templates')
    );

    return($form);
}

//------------------------------------------------------------------------------
// save email settings
function ncn_admin_email_settings_form_submit($form, &$form_state)
{
    // email
    variable_set('ncn_front_end_subject', t($form_state['values']['front_end_subject']));  
    variable_set('ncn_front_end_body', t($form_state['values']['front_end_body']));

    // email
    variable_set('ncn_create_my_invoice_subject', t($form_state['values']['create_my_invoice_subject']));  
    variable_set('ncn_create_my_invoice_body', t($form_state['values']['create_my_invoice_body']));

    // email
    variable_set('ncn_new_claim_subject', t($form_state['values']['new_claim_subject']));  
    variable_set('ncn_new_claim_body', t($form_state['values']['new_claim_body']));
    variable_set('ncn_new_claim_active', $form_state['values']['new_claim_active']);

    // email
    variable_set('ncn_new_claim_customer_subject', t($form_state['values']['new_claim_customer_subject']));  
    variable_set('ncn_new_claim_customer_body', t($form_state['values']['new_claim_customer_body']));
    
    // email
    variable_set('ncn_approved_subject', t($form_state['values']['approved_subject']));
    variable_set('ncn_approved_body', t($form_state['values']['approved_body']));
    variable_set('ncn_approved_active', $form_state['values']['approved_active']);

    // email
    variable_set('ncn_rejected_subject', t($form_state['values']['rejected_subject']));
    variable_set('ncn_rejected_body', t($form_state['values']['rejected_body']));
    variable_set('ncn_rejected_active', (isset($form_state['values']['rejected_active']) ? $form_state['values']['rejected_active'] : ''));

    // email
    variable_set('ncn_generic_subject', t($form_state['values']['generic_subject']));
    variable_set('ncn_generic_body', t($form_state['values']['generic_body']));
    variable_set('ncn_generic_active', (isset($form_state['values']['generic_active']) ? $form_state['values']['generic_active']:''));
    
    // email
    variable_set('ncn_id_to_member_subject', t($form_state['values']['id_to_member_subject']));
    variable_set('ncn_id_to_member_body', t($form_state['values']['id_to_member_body']));

    // sub user email
    variable_set('ncn_id_to_subuser_subject', t($form_state['values']['id_to_subuser_subject']));
    variable_set('ncn_id_to_subuser_body', t($form_state['values']['id_to_subuser_body']));
    
    // email
    variable_set('ncn_distributor_signup_subject', t($form_state['values']['distributor_signup_subject']));
    variable_set('ncn_distributor_signup_body', t($form_state['values']['distributor_signup_body']));
    
    variable_set('ncn_associate_signup_subject', t($form_state['values']['associate_signup_subject']));
    variable_set('ncn_associate_signup_body', t($form_state['values']['associate_signup_body']));
    
    variable_set('ncn_accountmanager_signup_subject', t($form_state['values']['accountmanager_signup_subject']));
    variable_set('ncn_accountmanager_signup_body', t($form_state['values']['accountmanager_signup_body']));
    
    variable_set('ncn_timer_alert_subject', t($form_state['values']['timer_alert_subject']));
    variable_set('ncn_timer_alert_body', t($form_state['values']['timer_alert_body']));
    
    variable_set('ncn_timer_expired_subject', t($form_state['values']['timer_expired_subject']));
    variable_set('ncn_timer_expired_body', t($form_state['values']['timer_expired_body']));
    
    variable_set('ncn_rejected_claim_docs_mail_subject', t($form_state['values']['rejected_claim_docs_mail_subject']));
    variable_set('ncn_rejected_claim_docs_mail_body', t($form_state['values']['rejected_claim_docs_mail_body']));
    
    variable_set('ncn_claim_status_esx_pdf_uploaded', t($form_state['values']['esx_pdf_uploaded_body']));
    
    // success!
    drupal_set_message('Email settings saved', 'status');
}

//------------------------------------------------------------------------------
// entry function
function ncn_admin_page()
{
GLOBAL $base_url;

    drupal_add_js(drupal_get_path('module', 'ncn_admin') . '/ncn_admin.js');

    // start buffering
    ob_start();

    // check claim id
    $claim_id = intval(arg(4));
    
    // swicth functions
    switch (arg(3))
    {
        case "downloadxls":
            ncn_admin_download_xls($claim_id);
        break;

        case "downloadpdf":
            ncn_admin_download_pdf($claim_id);
        break;

        case "viewlog":
            ncn_admin_view_logfile($claim_id);
        break;

        case "viewclaim":
            ncn_admin_view_claim($claim_id);
        break;
        
        case "edit-additional-claim-data":
            ncn_admin_edit_additional_claim_data($claim_id);
        break;
        case "view-additional-claim-data":
          ncn_admin_view_additional_claim_data($claim_id);
        break;
    
        case "edit-room-scopesheet":
            $room_name = arg(5);
            ncn_admin_edit_room_scopesheet($claim_id, $room_name);
        break;
        case "view-room-scopesheet":
            $room_name = arg(5);
            ncn_admin_view_room_scopesheet($claim_id, $room_name);
            break;
        
        case "filter":
            ncn_admin_list_claims();
        break;      
        case "deleteclaim":
            ncn_admin_delete_claim($claim_id);
        break;
        default:
            if (stristr($_SERVER['REQUEST_URI'], 'ncn_admin_pool'))
            {
                Header("Location:".$base_url."/admin/config/ncn_admin_pool/filter/show all/user/show all");
            }
            else
            {
                Header("Location:".$base_url."/admin/config/ncn_admin/filter/show all/user/show all");
            }
            exit;
        break;
    }

    
    // return the content
    $content = ob_get_contents();
    ob_end_clean();
    return($content);
//  return system_settings_form($form);
}
