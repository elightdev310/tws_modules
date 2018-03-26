
    jQuery(document).ready(function(){
        jQuery('input.phone-style-field').each(function() {});
    });

    function toggle_slide(layer_id, action)
    {
        if (document.getElementById(layer_id).style.display == "block")
        {
            jQuery("#"+layer_id).slideUp('slow');
            if (action==1) {
                jQuery("#"+layer_id).parent().parent().addClass('hidden-row');
            }
        }
        else
        {
            if (action==1) {
                jQuery("#"+layer_id).parent().parent().removeClass('hidden-row');
            }
            jQuery("#"+layer_id).slideDown('slow');
        }
    }

    //------------------------------------------------------------------------------
    function archive_claim(base_url, claim_id)
    {
        var archive_url = base_url+'/account/send_to_archive/'+claim_id;

        jQuery.get(archive_url, function(data)  {
            if (data == "success")
            {
                //window.location = base_url+'/account/archived.html';
                window.location = base_url+'/account/claims.html';
            }
        });

    }

    //------------------------------------------------------------------------------
    function partial_payment(base_url, claim_id, default_val)
    {
        //var answer = prompt ("Please enter the total dollar amount received to date.\n(Do not include the jQuery symbol).", default_val);
        var answer = prompt ("Please enter the total dollar amount received to date.", default_val);
        if (IsNumeric(answer) == false)
        {
            alert("You did not enter a valid dollar amount.");
            return false;
        }

        var partial_url = base_url+'/account/set_partial_payment/'+claim_id+'/'+answer;

        jQuery.get(partial_url, function(data)  {
            if (data == "100%")
            {
                //window.location = base_url+'/account/archived.html';
                window.location = base_url+'/account/claims.html';
            }
            else
            {
                //window.location = base_url+'/account/receivables.html';
                window.location = base_url+'/account/ars.html';
            }
        });

    }

    //------------------------------------------------------------------------------
    function set_file_note(base_url, claim_id, mode)
    {
        // show the extra details

        if (mode == true) {
            var note = prompt("Please enter your claim note for this claim.", '');

            if (note == null)
            {
                // and return
                return false;
            }

            var save_url = Drupal.settings.basePath+'account/claim_note/add';

            jQuery.ajax({
                url:    save_url,
                type:   'POST',
                data:{claim_id: claim_id, note: note},
                beforeSend: function(jqXHR, settings) {

                },
                error: function(jqXHR, textStatus, errorThrown) {

                },
                success: function(response) {

                }   // END OF SUCESS FUNCTION
            });
        } else {
            jQuery('#basic_'+claim_id).parent().parent().removeClass('hidden-row');
            jQuery('#basic_'+claim_id).slideDown('slow', function() {

                // get the notes
                var note = prompt("Please enter your claim note for this claim.", '');

                if (note == null)
                {
                    // hide the extra details
                    jQuery('#basic_'+claim_id).slideUp('slow');
                    jQuery('#basic_'+claim_id).parent().parent().addClass('hidden-row');

                    // and return
                    return false;
                }

                var save_url = Drupal.settings.basePath+'account/claim_note/add';

                jQuery.ajax({
                    url:    save_url,
                    type:   'POST',
                    data:{claim_id: claim_id, note: note},
                    beforeSend: function(jqXHR, settings) {

                    },
                    error: function(jqXHR, textStatus, errorThrown) {

                    },
                    success: function(response) {
                        location.reload(true);
                    }   // END OF SUCESS FUNCTION
                });
            });

        }
    }

    //------------------------------------------------------------------------------
    //  check for valid numeric strings
    function IsNumeric(strString)
    {
       var strValidChars = "0123456789.-";
       var strChar;
       var blnResult = true;

       if (strString.length == 0) return false;

       //  test strString consists of valid characters listed above
       for (i = 0; i < strString.length && blnResult == true; i++)
          {
          strChar = strString.charAt(i);
          if (strValidChars.indexOf(strChar) == -1)
             {
             blnResult = false;
             }
          }

       // check for vali < 0
       if (blnResult == true)
       {
            if (parseInt(strString) <= 0)
            {   blnResult = false;  }
       }

       return blnResult;
    }

    //------------------------------------------------------------------------------
    function buy_invoice_submit(base_url, claim_id, claim_desc)
    {

        var buy_url = base_url+"/cart/add/e-p18_q1_m"+claim_id+"_a1o"+claim_id+"_a2o"+claim_desc+"?destination=cart/checkout";

        jQuery.get(buy_url, function(data)  {
            window.location = base_url+'/cart/checkout';
        });
    }

    //------------------------------------------------------------------------------
    // ajax function to reload claim data
    function reload_claim(claim_id)
    {

          jQuery('#page_loading').show();   // show the loading animation

          jQuery('#page_loading').hide();       // hide the loading animation
          return;

        jQuery.get(reload_url+"/"+claim_id, function(data) {
          jQuery('#page_results').html(data);   // copy the html to the layer
          jQuery('#page_loading').hide();       // hide the loading animation

        });

    }

    //------------------------------------------------------------------------------
    function check_options_for_submit()
    {
        var count = 0;

        for (var i=1; i<=3; i++)
        {
            if (document.getElementById('checked_'+i).checked == true)
            {   count++;    }
        }

        // enable/disable?
        if (count == 3)
        {
            document.getElementById('continue_button').disabled = false;
        }
        else
        {
            document.getElementById('continue_button').disabled = true;
        }

    }

    //------------------------------------------------------------------------------
    function open_submit_box(claim_id)
    {
        // open the colorbox
        var in_url = Drupal.settings.basePath+"account/confirm_submit_claim/"+claim_id;

        jQuery.colorbox({width:"700px",height:"540px",href:in_url,open:true,iframe:true,onClosed:function(){  }});

    }

    //------------------------------------------------------------------------------
    function open_edit_box(claim_id, mode)
    {
        try{
            // open the colorbox
            var in_url = Drupal.settings.basePath+"account/edit_claim_inline/"+claim_id+"/tab/1";
            jQuery.colorbox({width:"1024px",height:"650px",href:in_url,open:true,iframe:true,onClosed:function(){ reload_claim(claim_id); }});
        } catch(err) {
            alert(err);
        }
        return false;
    }

    function open_scopesheet_edit_box(claim_id)
    {
        try{
            // open the colorbox
            var in_url = Drupal.settings.basePath+"account/scope_sheet/"+claim_id+"/0";

            jQuery.colorbox({width:"1024px",height:"650px",href:in_url,open:true,iframe:true,onClosed:function(){ }});
        } catch(err) {
            alert(err);
        }
        return false;
    }

    //------------------------------------------------------------------------------
    function save_photos_inline(post_url)
    {
        // set teh button to "Saving..."
        document.getElementById('save_jquery').value = 'Saving';
        document.getElementById('save_jquery').disabled = true;

        // save the photos
        jQuery.post(post_url, jQuery("#save_photos_form").serialize(), function(data) {

            document.getElementById('save_jquery').value = 'Save';
            document.getElementById('save_jquery').disabled = false;

        });
    }

    //------------------------------------------------------------------------------
    function save_photos_inline_and_close(post_url)
    {
        // set teh button to "Saving..."
        document.getElementById('save_jquery_and_close').value = 'Saving';
        document.getElementById('save_jquery_and_close').disabled = true;

        // save the photos
        jQuery.post(post_url, jQuery("#save_photos_form").serialize(), function(data) {
            document.getElementById('save_jquery_and_close').value = 'Save and Close';
            document.getElementById('save_jquery_and_close').disabled = false;
            parent.jQuery('#cboxClose').trigger('click');
        });
    }

    //------------------------------------------------------------------------------
    function dist_create_member_submit(submit_btn_id) {
      if (ncn_change_submit_button_disabled(submit_btn_id, "", "")) {
          jQuery('#distributor_page #create_member_submit_form').submit();
      }
    }


    function open_change_order_request_window(in_url, width, height) {
        // open the colorbox
        jQuery.colorbox({width:(width+"px"),height:(height+"px"),href:in_url,open:true,iframe:true, overlayClose:false, onClosed:function(){ /*refresh_out_of_claim_page();*/ }});
    }

    function refresh_out_of_claim_page() {
        window.location.reload(true);
    }

    function on_change_room_select() {
        if (jQuery('#cor_arf_room_sel').val() == 'add_room') {
            jQuery('#add_room_section').css('display', 'inline-block');
            jQuery('#claim-change-order-action-cor').css('display', 'none');
        } else {
            jQuery('#add_room_section').css('display', 'none');
            jQuery('#claim-change-order-action-cor').css('display', 'block');
            /** Get Render of Action Form **/
            var claim_id  = jQuery('#cor_arf_claim_id').val();
            var roomname = jQuery('#cor_arf_room_sel').val();
            if (roomname == '') { jQuery('#claim-change-order-action-cor').css('display', 'none'); return; }
            render_request_action_new(claim_id, roomname);
        }
    }

    function render_request_action_new(claim_id, roomname) {
        var _url = Drupal.settings.basePath+'ajax/ncn_change_order/get_request_action';
        jQuery.ajax({
            url:    _url,
            type:   'POST',
            data:{claim_id: claim_id, roomname: roomname},
            beforeSend: function(jqXHR, settings) {
            },
            error: function() { },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.code == "success") {
                    jQuery('#claim-change-order-action-cor').html(json.form_html);
                    jQuery('#cor_rf_action').bind('change', on_change_action_select);
                    jQuery('#cor_rf_ooss').bind('click', on_change_action_select);
                }
                else {

                }
            }   // END OF SUCESS FUNCTION
        });
    }

    function on_change_action_select() {
        if (jQuery('#cor_rf_action').val() == 'add') {
            jQuery('#cor_rf_line').css('display', 'none');
        } else {
            jQuery('#cor_rf_line').css('display', 'block');
        }

        var _url = Drupal.settings.basePath+'ajax/ncn_change_order/get_price';
        var _form = jQuery('#cor_request_form');
        jQuery.ajax({
            url:    _url,
            type:   'POST',
            data: _form.serialize(),
            beforeSend: function(jqXHR, settings) {
            },
            error: function() { },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.code == "success") {
                    jQuery('#cor_rf_price').html(json.action_price);
                    if (json.msg !='') {
                        alert(json.msg);
                    }
                }
                else {
                    alert(json.msg);
                }
            }   // END OF SUCCESS FUNCTION
        });

    }

    function on_submit_cor_request() {
        var _url = Drupal.settings.basePath+'ajax/ncn_change_order/request_submit';
        var _form = jQuery('#cor_request_form');

        jQuery.ajax({
            url:    _url,
            type: 'POST',
            data: _form.serialize(),
            beforeSend: function(jqXHR, settings) {
            },
            error: function() { alert("Error: Request hasn't problem."); },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.code == "success") {
                    alert("Success to request change order");
                    /* Form Reset */
                    if (json.action_type == 'new') {
                        jQuery('#cor_request_form #cor_rf_action').val('');
                        jQuery('#cor_request_form #cor_rf_line').val(''); jQuery('#cor_request_form #cor_rf_line').css('display', 'block');
                        jQuery('#cor_request_form #cor_rf_detail').val('');
                        jQuery('#cor_request_form #cor_rf_ooss').attr('checked', false);
                        jQuery('#cor_request_form #cor_rf_price').html('');
                    } else {
                        render_request_action_new(json.data.claim_id, json.data.room_name);
                    }
                    on_render_request_list(json.data.claim_id);
                }
                else {
                    alert(json.msg);
                }

                jQuery('#cor_rh_total_price').html(json.total_price);
                jQuery('#cor_rh_invoice_spent_price').html(json.invoice_spent_price);
                jQuery('#ci_credit_price').html(json.credit_price);
            }   // END OF SUCESS FUNCTION
        });
    }

    function on_render_request_list(claim_id) {
        var _url = Drupal.settings.basePath+'ajax/ncn_change_order/get_request_list';
        jQuery.ajax({
            url:    _url,
            type:   'POST',
            data:{claim_id: claim_id},
            beforeSend: function(jqXHR, settings) {
            },
            error: function() { },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.code == "success") {
                    jQuery('#claim-change-order-request-list-cor').html(json.list_html);
                    if (json.submit_available == 'true') {
                        jQuery('#cor_request_submit_form input').removeAttr("disabled");
                    } else {
                        jQuery('#cor_request_submit_form input').attr('disabled', 'disabled');
                    }
                }
                else {

                }
            }   // END OF SUCCESS FUNCTION
        });
    }

    // Submit Change Order
    function on_click_cor_request_submit(claim_id) {
        var _url = Drupal.settings.basePath+'ajax/ncn_change_order/submit/'+claim_id;
        jQuery.ajax({
            url:    _url,
            type:   'POST',
            beforeSend: function(jqXHR, settings) {
                jQuery("#ncn_change_order_request_page").loadingOverlay();
            },
            error: function() { },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.code == "success") {
                alert("Success to submit change order requests");
                    //parent.jQuery.colorbox.close();
                    //parent.window.location.reload(true);
                    //window.location = Drupal.settings.basePath+'account/out-for-review.html';
                    window.location = Drupal.settings.basePath+'account/claims.html';
                }
                else {
                    alert(json.msg);
                }
            },    // END OF SUCCESS FUNCTION
            complete: function(jqXHR, textStatus) {
                jQuery("#ncn_change_order_request_page").loadingOverlay('remove');
            }
        });
    }

    function on_click_edit_cor_item(r_id) {
        var _url = Drupal.settings.basePath+'ajax/ncn_change_order/get_request_action_edit';
            jQuery.ajax({
                url:    _url,
                type:   'POST',
                data:{r_id: r_id},
                beforeSend: function(jqXHR, settings) {
                },
                error: function() { },
                success: function(response) {
                    eval("var json=" + response + ";");
                    if (json.code == "success") {
                        jQuery('#cor_arf_room_sel').val(json.roomname);
                        jQuery('#claim-change-order-action-cor').html(json.form_html);
                        jQuery('#cor_rf_action').bind('change', on_change_action_select);
                        jQuery('#cor_rf_ooss').bind('click', on_change_action_select);
                    }
                    else {
                        alert(json.msg);
                    }
                }   // END OF SUCESS FUNCTION
            });
    }

    function on_click_remove_cor_item(claim_id, r_id) {
        if (!confirm('Are you sure you want to remove request?')) {
            return;
        }

        var _url = Drupal.settings.basePath+'ajax/ncn_change_order/request_remove/'+r_id;
            jQuery.ajax({
                url:    _url,
                type:   'POST',
                beforeSend: function(jqXHR, settings) {
                },
                error: function() { },
                success: function(response) {
                    eval("var json=" + response + ";");
                    if (json.code == "success") {
                        alert("Success to remove request");
                    }
                    else {
                        alert(json.msg);
                    }

                    on_render_request_list(claim_id);
                    jQuery('#cor_rh_total_price').html(json.total_price);
                    jQuery('#cor_rh_invoice_spent_price').html(json.invoice_spent_price);
                    jQuery('#ci_credit_price').html(json.credit_price);
                }   // END OF SUCESS FUNCTION
            });
    }

    function on_click_remove_cor_room(r_id) {
        if (!confirm('Are you sure you want to remove room?')) {
            return;
        }
        jQuery('#cor_arrf_r_id').val(r_id);
        jQuery('#cor_remove_room_form').submit();
    }

    function on_change_order_charge() {
        jQuery('#tfunction_change_order').val('ncn_cor_charge');
        jQuery('.current_scroll_position').val(get_current_scroll_position());

        return true;
    }

    function on_change_order_review() {
        jQuery('#tfunction_change_order').val('ncn_cor_review');
        jQuery('.current_scroll_position').val(get_current_scroll_position());
        return true;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // Additional Claim Information

    function dcl_calc_total_hours() {
        var dcl_men = jQuery('#dcl_men').val();
        var dcl_hours_day = jQuery('#dcl_hours_day').val();
        jQuery('#dcl_total_hours').val(dcl_men*dcl_hours_day);
    }

    /**
     * Submit Daily Claim Log (ACI-DCL)
     * new / edit
     */
    function on_click_aci_dcl_log_action(claim_id) {
        var action_type     = jQuery('#dcl_action_type').val();
        var log_index       = jQuery('#dcl_log_index').val();
        var dcl_date            = jQuery('#dcl_date').val();
        var dcl_men = jQuery('#dcl_men').val();
        var dcl_hours_day = jQuery('#dcl_hours_day').val();
        var dcl_after_hours = '';

        if (jQuery('#dcl_after_hours_yes').is(':checked')) {
            dcl_after_hours = jQuery('#dcl_after_hours_yes').val();
        } else if (jQuery('#dcl_after_hours_no').is(':checked')) {
            dcl_after_hours = jQuery('#dcl_after_hours_no').val();
        }

        var _url = Drupal.settings.basePath+'ajax/ncn_admin/aci/dcl/'+claim_id+'/daily_log_submit';
        jQuery.ajax({
            url:    _url,
            type:   'POST',
            data:{  action_type         : action_type,
                            log_index               : log_index,
                            dcl_date                : dcl_date,
                            dcl_men                 : dcl_men,
                            dcl_hours_day       : dcl_hours_day,
                            dcl_after_hours : dcl_after_hours },
            beforeSend: function(jqXHR, settings) {

            },
            error: function() { },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.code == "success") {
                    alert(json.msg);
                    render_aci_dcl_log_action_panel(claim_id, 'new',0);
                    jQuery('#dcl_action_panel_descrption').css('display', 'none');
                } else {
                    alert(json.msg);
                }
                refresh_aci_dcl_log_list(claim_id);
            }   // END OF SUCESS FUNCTION
        });
    }

    function refresh_aci_dcl_log_list(claim_id) {
        var _url = Drupal.settings.basePath+'ajax/ncn_admin/aci/dcl/'+claim_id+'/render_daily_log_list';
        jQuery.ajax({
            url:    _url,
            type:   'POST',
            data:{},
            beforeSend: function(jqXHR, settings) {
            },
            error: function() { },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.code == "success") {
                    jQuery('#dcl_log_list').html(json.html);
                } else {
                    alert(json.msg);
                }
            }   // END OF SUCESS FUNCTION
        });
    }

    function render_aci_dcl_log_action_panel(claim_id, action, index) {
        var _url = Drupal.settings.basePath+'ajax/ncn_admin/aci/dcl/'+claim_id+'/render_log_action_panel';
        jQuery.ajax({
            url:    _url,
            type:   'POST',
            data:{action: action, index: index},
            beforeSend: function(jqXHR, settings) {
            },
            error: function() { },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.code == "success") {
                    jQuery('#dcl_action_panel_content').html(json.html);
                } else {
                    alert(json.msg);
                }
            }   // END OF SUCESS FUNCTION
        });
    }

    function on_click_aci_dcl_delete_log(claim_id, index) {
        if (!confirm('Are you sure you want to remove log?')) {
            return;
        }

        var _url = Drupal.settings.basePath+'ajax/ncn_admin/aci/dcl/'+claim_id+'/delete_daily_log';
        jQuery.ajax({
            url:    _url,
            type:   'POST',
            data:{log_index : index },
            beforeSend: function(jqXHR, settings) {

            },
            error: function() { },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.code == "success") {
                    alert(json.msg);
                    render_aci_dcl_log_action_panel(claim_id, 'new',0);
                } else {
                    alert(json.msg);
                }
                refresh_aci_dcl_log_list(claim_id);
            }   // END OF SUCESS FUNCTION
        });
    }

    function show_aci_section(link_obj) {
        jQuery('.aci-section-links .aci-section-link-item a').each(function(index) {
            jQuery(this).removeClass('active');
        });

        jQuery('#'+link_obj).addClass('active');

        jQuery('.aci-section-content .aci-section').each(function(index) {
            jQuery(this).removeClass('active');
        });

        section_obj = jQuery('#'+link_obj).attr('href');
        jQuery('#'+section_obj).addClass('active');

      ncn_scopesheet_sections_tinyscrollbar();
    }

    function isNumberKey(evt, allow_dot)
    {
         var charCode = (evt.which) ? evt.which : event.keyCode

         if (allow_dot && charCode == 46) { return true; } // dot
         if (charCode > 31 && (charCode < 48 || charCode > 57))
                return false;

         return true;
    }

    function isDateKey(evt)
    {
       var charCode = (evt.which) ? evt.which : event.keyCode

       if (charCode == 47) { return true; } // forward slash (/)
       if (charCode > 31 && (charCode < 48 || charCode > 57))
          return false;

       return true;
    }

    /*****************************************************************************/
    /* Scope Sheet - Room Dimensions                                                                                         */
    /*****************************************************************************/
    /**
     * Scope Sheet
     * new / edit
     */
    function on_click_ss_rd_action(claim_id, room_name) {
        var action_type     = jQuery('#ss_rd_action_type').val();
        var ss_rd_index     = jQuery('#ss_rd_index').val();

        var ss_rd_type      = jQuery('#ss_rd_type').val();
        var ss_rd_name      = jQuery('#ss_rd_name').val();

        var ss_rd_size_L    = jQuery('#ss_rd_size_L').val();
        var ss_rd_size_W    = jQuery('#ss_rd_size_W').val();
        var ss_rd_size_H    = jQuery('#ss_rd_size_H').val();
        //var ss_rd_tfa         = jQuery('#ss_rd_tfa').val();
        var ss_rd_floor_type = jQuery('#ss_rd_floor_type').val();

        var _url = Drupal.settings.basePath+'ajax/ncn_admin/scopesheet/rd/'+claim_id+'/action_submit';
        jQuery.ajax({
            url:    _url,
            type:   'POST',
            data:{
                            room_name               : room_name,
                            action_type         : action_type,
                            ss_rd_index         : ss_rd_index,
                            ss_rd_type          : ss_rd_type,
                            ss_rd_name          : ss_rd_name,
                            ss_rd_size_L        : ss_rd_size_L,
                            ss_rd_size_W        : ss_rd_size_W,
                            ss_rd_size_H        : ss_rd_size_H,
    //                  ss_rd_tfa               : ss_rd_tfa,
                            ss_rd_floor_type: ss_rd_floor_type  },
            beforeSend: function(jqXHR, settings) {

            },
            error: function() { },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.code == "success") {
                    alert(json.msg);
                    render_ss_rd_action_panel(claim_id, room_name, 'new',0);
                } else {
                    alert(json.msg);
                }
                refresh_ss_rd_list(claim_id, room_name);
            }   // END OF SUCESS FUNCTION
        });
    }

    function render_ss_rd_action_panel(claim_id, room_name, action, index) {
        var _url = Drupal.settings.basePath+'ajax/ncn_admin/scopesheet/rd/'+claim_id+'/render_ss_rd_action_panel';
        jQuery.ajax({
            url:    _url,
            type:   'POST',
            data:{room_name: room_name, action: action, index: index},
            beforeSend: function(jqXHR, settings) {
            },
            error: function() { },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.code == "success") {
                    jQuery('#ss_rd_action_panel_content').html(json.html);
                    onchange_ss_rd_type();
                } else {
                    alert(json.msg);
                }
            }   // END OF SUCESS FUNCTION
        });
    }

    function onchange_ss_rd_type() {
        var ss_rd_type = jQuery( '#ss_rd_type' ).val();
        if (ss_rd_type == 'Other') {
            jQuery('#ss_rd_name_wrapper').css('display', 'inline');
        } else {
            jQuery('#ss_rd_name_wrapper').css('display', 'none');
        }
    }

    function on_click_ss_rd_delete(claim_id, room_name, index) {
        if (!confirm('Are you sure you want to remove data?')) {
            return;
        }

        var _url = Drupal.settings.basePath+'ajax/ncn_admin/scopesheet/rd/'+claim_id+'/delete';
        jQuery.ajax({
            url:    _url,
            type:   'POST',
            data:{room_name: room_name, index   : index},
            beforeSend: function(jqXHR, settings) {

            },
            error: function() { },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.code == "success") {
                    alert(json.msg);
                    render_ss_rd_action_panel(claim_id, room_name, 'new',0);
                } else {
                    alert(json.msg);
                }
                refresh_ss_rd_list(claim_id, room_name);
            }   // END OF SUCESS FUNCTION
        });
    }

    function refresh_ss_rd_list(claim_id, room_name) {
        var _url = Drupal.settings.basePath+'ajax/ncn_admin/scopesheet/rd/'+claim_id+'/render_rd_list';
        jQuery.ajax({
            url:    _url,
            type:   'POST',
            data:{room_name: room_name},
            beforeSend: function(jqXHR, settings) {
            },
            error: function() { },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.code == "success") {
                    jQuery('#ss_rd_list').html(json.html);
                } else {
                    alert(json.msg);
                }
          ncn_scopesheet_sections_tinyscrollbar();
            }   // END OF SUCESS FUNCTION
        });
    }

    /*****************************************************************************/
    /* Scope Sheet - Exterior & Interior Temperatures                                                        */
    /*****************************************************************************/
    /**
     * Scope Sheet
     * new / edit
     */
    function on_click_ss_eit_action(claim_id, room_name) {
        var action_type     = jQuery('#ss_eit_action_type').val();
        var ss_eit_index    = jQuery('#ss_eit_index').val();
        var ss_eit_date     = jQuery('#ss_eit_date').val();
        //var ss_eit_staff  = jQuery('#ss_eit_staff').val();
        var ss_eit_ext_temp = jQuery('#ss_eit_ext_temp').val(); var ss_eit_ext_rh = jQuery('#ss_eit_ext_rh').val();
        var ss_eit_int_temp = jQuery('#ss_eit_int_temp').val(); var ss_eit_int_rh = jQuery('#ss_eit_int_rh').val();
        var ss_eit_gpp      = jQuery('#ss_eit_gpp').val();
        var time_readings = jQuery('#ss_eit_time_readings').val();

        var time_readings_half = '';
        if (jQuery('#ss_eit_time_readings_am').is(':checked')) {
            time_readings_half = jQuery('#ss_eit_time_readings_am').val();
        } else if (jQuery('#ss_eit_time_readings_pm').is(':checked')) {
            time_readings_half = jQuery('#ss_eit_time_readings_pm').val();
        }

        var _url = Drupal.settings.basePath+'ajax/ncn_admin/scopesheet/eit/'+claim_id+'/action_submit';
        jQuery.ajax({
            url:    _url,
            type:   'POST',
            data:{
                            room_name               : room_name,
                            action_type         : action_type,
                            ss_eit_index        : ss_eit_index,
                            ss_eit_date         : ss_eit_date,
    //                      ss_eit_staff        : ss_eit_staff,
                            ss_eit_ext_temp : ss_eit_ext_temp,
                            ss_eit_ext_rh       : ss_eit_ext_rh,
                            ss_eit_int_temp : ss_eit_int_temp,
                            ss_eit_int_rh   : ss_eit_int_rh,
                            ss_eit_gpp          : ss_eit_gpp,
                            time_readings       : time_readings,
                            time_readings_half : time_readings_half             },
            beforeSend: function(jqXHR, settings) {

            },
            error: function() { },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.code == "success") {
                    alert(json.msg);
                    render_ss_eit_action_panel(claim_id, room_name, 'new',0);
                } else {
                    alert(json.msg);
                }
                refresh_ss_eit_list(claim_id, room_name);
            }   // END OF SUCESS FUNCTION
        });
    }

    function render_ss_eit_action_panel(claim_id, room_name, action, index) {
        var _url = Drupal.settings.basePath+'ajax/ncn_admin/scopesheet/eit/'+claim_id+'/render_ss_eit_action_panel';
        jQuery.ajax({
            url:    _url,
            type:   'POST',
            data:{room_name: room_name, action: action, index: index},
            beforeSend: function(jqXHR, settings) {
            },
            error: function() { },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.code == "success") {
                    jQuery('#ss_eit_action_panel_content').html(json.html);
                } else {
                    alert(json.msg);
                }
            }   // END OF SUCESS FUNCTION
        });
    }

    function on_click_ss_eit_delete(claim_id, room_name, index) {
        if (!confirm('Are you sure you want to remove data?')) {
            return;
        }

        var _url = Drupal.settings.basePath+'ajax/ncn_admin/scopesheet/eit/'+claim_id+'/delete';
        jQuery.ajax({
            url:    _url,
            type:   'POST',
            data:{room_name: room_name, index   : index},
            beforeSend: function(jqXHR, settings) {

            },
            error: function() { },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.code == "success") {
                    alert(json.msg);
                    render_ss_eit_action_panel(claim_id, room_name, 'new',0);
                } else {
                    alert(json.msg);
                }
                refresh_ss_eit_list(claim_id, room_name);
            }   // END OF SUCESS FUNCTION
        });
    }

    function refresh_ss_eit_list(claim_id, room_name) {
        var _url = Drupal.settings.basePath+'ajax/ncn_admin/scopesheet/eit/'+claim_id+'/render_eit_list';
        jQuery.ajax({
            url:    _url,
            type:   'POST',
            data:{room_name: room_name},
            beforeSend: function(jqXHR, settings) {
            },
            error: function() { },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.code == "success") {
                    jQuery('#ss_eit_list').html(json.html);
                } else {
                    alert(json.msg);
                }
          ncn_scopesheet_sections_tinyscrollbar();
            }   // END OF SUCESS FUNCTION
        });
    }




    /*****************************************************************************/
    /* Scope Sheet - Structural Moisture Readings                                                                */
    /*****************************************************************************/
    function get_splash_field_value(objStr) {
        var obj_val = jQuery(objStr).val();
        var obj_title = jQuery(objStr).attr('title');
        if (obj_val == obj_title) {
            return '';
        }
        return obj_val;
    }

    /**
     * Scope Sheet
     * new / edit
     */
    function on_click_ss_smr_action(claim_id, room_name) {
        var action_type     = jQuery('#ss_smr_action_type').val();
        var ss_smr_index    = jQuery('#ss_smr_index').val();
        var ss_smr_date     = jQuery('#ss_smr_date').val();

        var ss_smr_wall_0 = get_splash_field_value('#ss_smr_wall_0');
        var ss_smr_wall_1 = get_splash_field_value('#ss_smr_wall_1');
        var ss_smr_wall_2 = get_splash_field_value('#ss_smr_wall_2');
        var ss_smr_wall_3 = get_splash_field_value('#ss_smr_wall_3');

        var ss_smr_area_fd_0 = get_splash_field_value('#ss_smr_area_fd_0');
        var ss_smr_area_fd_1 = get_splash_field_value('#ss_smr_area_fd_1');
        var ss_smr_area_fd_2 = get_splash_field_value('#ss_smr_area_fd_2');
        var ss_smr_area_fd_3 = get_splash_field_value('#ss_smr_area_fd_3');
        var ss_smr_area_fd_center = get_splash_field_value('#ss_smr_area_fd_center');

        var ss_smr_area_cd_0 = get_splash_field_value('#ss_smr_area_cd_0');
        var ss_smr_area_cd_1 = get_splash_field_value('#ss_smr_area_cd_1');
        var ss_smr_area_cd_2 = get_splash_field_value('#ss_smr_area_cd_2');
        var ss_smr_area_cd_3 = get_splash_field_value('#ss_smr_area_cd_3');
        var ss_smr_area_cd_center = get_splash_field_value('#ss_smr_area_cd_center');

        var _url = Drupal.settings.basePath+'ajax/ncn_admin/scopesheet/smr/'+claim_id+'/action_submit';
        jQuery.ajax({
            url:    _url,
            type:   'POST',
            data:{
                            room_name               : room_name,
                            action_type         : action_type,
                            ss_smr_index        : ss_smr_index,
                            ss_smr_date         : ss_smr_date,
                            ss_smr_wall_0       : ss_smr_wall_0,
                            ss_smr_wall_1       : ss_smr_wall_1,
                            ss_smr_wall_2       : ss_smr_wall_2,
                            ss_smr_wall_3       : ss_smr_wall_3,

                            ss_smr_area_fd_0: ss_smr_area_fd_0,
                            ss_smr_area_fd_1: ss_smr_area_fd_1,
                            ss_smr_area_fd_2: ss_smr_area_fd_2,
                            ss_smr_area_fd_3: ss_smr_area_fd_3,
                            ss_smr_area_fd_center   : ss_smr_area_fd_center,

                            ss_smr_area_cd_0: ss_smr_area_cd_0,
                            ss_smr_area_cd_1: ss_smr_area_cd_1,
                            ss_smr_area_cd_2: ss_smr_area_cd_2,
                            ss_smr_area_cd_3: ss_smr_area_cd_3,
                            ss_smr_area_cd_center : ss_smr_area_cd_center },
            beforeSend: function(jqXHR, settings) {

            },
            error: function() { },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.code == "success") {
                    alert(json.msg);
                    render_ss_smr_action_panel(claim_id, room_name, 'new',0);
                } else {
                    alert(json.msg);
                }
                refresh_ss_smr_list(claim_id, room_name);
            }   // END OF SUCESS FUNCTION
        });
    }

    function render_ss_smr_action_panel(claim_id, room_name, action, index) {
        var _url = Drupal.settings.basePath+'ajax/ncn_admin/scopesheet/smr/'+claim_id+'/render_ss_smr_action_panel';
        jQuery.ajax({
            url:    _url,
            type:   'POST',
            data:{room_name: room_name, action: action, index: index},
            beforeSend: function(jqXHR, settings) {
            },
            error: function() { },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.code == "success") {
                    jQuery('#ss_smr_action_panel_content').html(json.html);
                } else {
                    alert(json.msg);
                }
            }   // END OF SUCESS FUNCTION
        });
    }

    function on_click_ss_smr_delete(claim_id, room_name, index) {
        if (!confirm('Are you sure you want to remove data?')) {
            return;
        }

        var _url = Drupal.settings.basePath+'ajax/ncn_admin/scopesheet/smr/'+claim_id+'/delete';
        jQuery.ajax({
            url:    _url,
            type:   'POST',
            data:{room_name: room_name, index   : index},
            beforeSend: function(jqXHR, settings) {

            },
            error: function() { },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.code == "success") {
                    alert(json.msg);
                    render_ss_smr_action_panel(claim_id, room_name, 'new',0);
                } else {
                    alert(json.msg);
                }
                refresh_ss_smr_list(claim_id, room_name);
            }   // END OF SUCESS FUNCTION
        });
    }

    function refresh_ss_smr_list(claim_id, room_name) {
        var _url = Drupal.settings.basePath+'ajax/ncn_admin/scopesheet/smr/'+claim_id+'/render_smr_list';
        jQuery.ajax({
            url:    _url,
            type:   'POST',
            data:{room_name: room_name},
            beforeSend: function(jqXHR, settings) {
            },
            error: function() { },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.code == "success") {
                    jQuery('#ss_smr_list').html(json.html);
                } else {
                    alert(json.msg);
                }
          ncn_scopesheet_sections_tinyscrollbar();
            }   // END OF SUCESS FUNCTION
        });
    }

    function bindSlashInfo(objStr, displayStr) {
        jQuery(objStr).bind('focus', function() {
            if (jQuery(this).hasClass('slashInfo')) jQuery(this).val('');
            jQuery(this).removeClass('slashInfo');
            jQuery(this).css('color', '#111');
        }).bind('blur', function() {
            if (jQuery(this).val() != '') return;
            jQuery(this).addClass('slashInfo').val(displayStr);
            jQuery(this).css('color', '#888');
        });

        if (jQuery(objStr).val() == '') {
            jQuery(objStr).val(displayStr).addClass('slashInfo').css('color', '#888');
        }
    }

    function bindSlashEvent(wrapper) {
        jQuery(wrapper).find('.slash-field').each(function(index, Element) {
                var obj_id = '#'+jQuery(this).attr('id');
                var obj_title = jQuery(this).attr('title');
                bindSlashInfo(obj_id, obj_title);
        });
    }

    /*****************************************************************************/
    /*  Scope Sheet - Equipment Placement
    /*****************************************************************************/
    function onchange_ss_ep_action_panel_type() {
        var claim_id = jQuery('#ss_ep_claim_id').val();
        var room_name = jQuery('#ss_ep_room_name').val();

        var type = jQuery('#ss_ep_action_panel_type').val();
        var _url = Drupal.settings.basePath+'ajax/ncn_admin/scopesheet/ep/'+claim_id+'/get_action_panel';
        jQuery.ajax({
            url:    _url,
            type:   'POST',
            data:{room_name: room_name, type: type},
            beforeSend: function(jqXHR, settings) {

            },
            error: function() { },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.code == "success") {
                    jQuery('#ss_ep_action_panel_content').html(json.html);
                } else {
                    alert(json.msg);
                }
                if (type == '') {
                    jQuery('#ss_ep_action_panel_content .ss-ep-actions .ss-ep-button').each(function() {
                        jQuery(this).css('display', 'none');
                    });
                } else {
                    jQuery('#ss_ep_action_panel_content .ss-ep-actions .ss-ep-button').each(function() {
                        jQuery(this).css('display', 'inline-block');
                    });
                }
            }   // END OF SUCESS FUNCTION
        });
    }

    function check_required_field(objStr) {
        var wrappper = objStr+" .required";
        var bChecked = true;
        jQuery(wrappper).each(function() {
            if (jQuery(this).val() == "") {
                bChecked = false;
            }
        });
        return bChecked;
    }

    function on_click_ss_ep_action(claim_id, room_name) {
        var action_type     = jQuery('#ss_ep_action_type').val();
        var ss_ep_index     = jQuery('#ss_ep_index').val();

        var _url = Drupal.settings.basePath+'ajax/ncn_admin/scopesheet/ep/'+claim_id+'/action_submit';
        var frm_data = jQuery('#ss_ep_action_panel_content .ser-field-item input').serialize();

        var toReturn    = [];
        toReturn.push( encodeURIComponent('room_name') + "=" + encodeURIComponent( room_name ) );
        toReturn.push( encodeURIComponent('action_type') + "=" + encodeURIComponent( action_type ) );
        toReturn.push( encodeURIComponent('ss_ep_index') + "=" + encodeURIComponent( ss_ep_index ) );

        if (!check_required_field(".ss-ep-action-panel")) {
            alert("You need to input all values.");
            return false;
        }
        var _data = toReturn.join("&").replace(/%20/g, "+");
        if (frm_data != '') {
            _data = _data + "&" + frm_data;
        }

        jQuery.ajax({
            url:    _url,
            type:   'POST',
            data:   _data,
            beforeSend: function(jqXHR, settings) {

            },
            error: function() { },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.code == "success") {
                    alert(json.msg);
                    render_ss_ep_action_panel(claim_id, room_name, 'new',0);
                } else {
                    alert(json.msg);
                }
                refresh_ss_ep_list(claim_id, room_name);
            }   // END OF SUCESS FUNCTION
        });
    }

    function render_ss_ep_action_panel(claim_id, room_name, action, index) {
        var _url = Drupal.settings.basePath+'ajax/ncn_admin/scopesheet/ep/'+claim_id+'/render_ss_ep_action_panel';
        jQuery.ajax({
            url:    _url,
            type:   'POST',
            data:{room_name: room_name, action: action, index: index},
            beforeSend: function(jqXHR, settings) {
            },
            error: function() { },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.code == "success") {
                    jQuery('#ss_ep_action_panel_content').html(json.html);
                    if (action=='new') {
                        jQuery('#ss_ep_action_panel_type').css('display', 'inline-block');
                        jQuery('#ss_ep_action_panel_type').val('');
                    } else {
                        jQuery('#ss_ep_action_panel_type').css('display', 'none');
                    }
                } else {
                    alert(json.msg);
                }
          ncn_scopesheet_sections_tinyscrollbar();
            }   // END OF SUCESS FUNCTION
        });
    }

    function on_click_ss_ep_delete(claim_id, room_name, index) {
        if (!confirm('Are you sure you want to remove data?')) {
            return;
        }

        var _url = Drupal.settings.basePath+'ajax/ncn_admin/scopesheet/ep/'+claim_id+'/delete';
        jQuery.ajax({
            url:    _url,
            type:   'POST',
            data:{room_name: room_name, index   : index},
            beforeSend: function(jqXHR, settings) {

            },
            error: function() { },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.code == "success") {
                    alert(json.msg);
                    render_ss_ep_action_panel(claim_id, room_name, 'new',0);
                } else {
                    alert(json.msg);
                }
                refresh_ss_ep_list(claim_id, room_name);
            }   // END OF SUCESS FUNCTION
        });
    }

    function refresh_ss_ep_list(claim_id, room_name) {
        var _url = Drupal.settings.basePath+'ajax/ncn_admin/scopesheet/ep/'+claim_id+'/render_ep_list';
        jQuery.ajax({
            url:    _url,
            type:   'POST',
            data:{room_name: room_name},
            beforeSend: function(jqXHR, settings) {
            },
            error: function() { },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.code == "success") {
                    jQuery('#ss_ep_list').html(json.html);
                } else {
                    alert(json.msg);
                }
          ncn_scopesheet_sections_tinyscrollbar();
            }   // END OF SUCESS FUNCTION
        });
    }

    function check_row_required() {
        var ret_val = true;
        jQuery('#ss_services_content tr.required-row').each(function() {
            var service = jQuery(this).find('.service-name').text();
            var b_hours = false;
            if (jQuery(this).find('.after-hours input').is(':checked') || jQuery(this).find('.reg-hours input').is(':checked')) {
                b_hours = true;
            }

            var b_data_radio = false;
            var b_exist_radio = false;
            jQuery(this).find('.ss-service-data input.required-radio').each(function() {
                b_exist_radio = true;
                if (jQuery(this).is(':checked')) {
                    b_data_radio = b_data_radio | true;
                }
            });
            if (!b_data_radio && !b_exist_radio) { b_data_radio = true; }

            var b_data_text = true;
            var b_exist_text = false;
            jQuery(this).find('.ss-service-data input.required-text').each(function() {
                b_exist_text = true;
                if (jQuery(this).val() == '') {
                    b_data_text = b_data_text & false ;
                }
            });

        var b_data = b_data_radio && b_data_text;

        //
        var inputted_data = false;
        jQuery(this).find('.ss-service-data input:radio').each(function() {
          if (jQuery(this).is(':checked')) {
            inputted_data = true;
          }
        });
        if (!inputted_data) {
          jQuery(this).find('.ss-service-data input:text').each(function() {
            if (jQuery(this).val() != '') {
              inputted_data = true;
            }
          });
        }

        if (!b_hours && inputted_data) {
          alert("Please select After Hours or Reg Hours for \""+service+"\".");
          ret_val = false;
        } else if (b_hours && !b_data) {
            if(service=="Plant Based under Anti Microbial"){
                alert("Please input All DATA for Anti Microbial.");
            } else {
                alert("Please input All DATA for \""+service+"\".");
            }
            ret_val = false;
        }
        });

        return ret_val;
    }

    function bind_ncn_admin_aci_ci_cboxlabel_buttons(){
        jQuery('#aci_ci_content label.cboxlabel input').bind('click',function() {
            var parentli = jQuery(this).parents('li')[0];
            jQuery(parentli).find('input[type="radio"].cboxradio').attr('checked', 'checked');
        });

        jQuery('#aci_ci_content input.cboxradio').bind('click',function() {
            jQuery(this).parents('li').find('label.cboxlabel')[0].click();
        });
    }

    function bind_ncn_admin_aci_ci_radio_reset_buttons() {
      jQuery('#aci_ci_content input.radio-reset-button').each(function() {
        jQuery(this).bind('click', on_click_ss_service_radio_reset);
      });
    }

    function bind_ncn_admin_ss_services_radio_reset_buttons() {
      jQuery('#ss_services_content input.radio-reset-button').each(function() {
        jQuery(this).bind('click', on_click_ss_service_radio_reset);
      });
    }

    function on_click_ss_service_radio_reset() {
      jQuery(this).parent().parent().find('input:radio').each(function() {
          jQuery(this).removeAttr("checked");
      });

      jQuery(this).parent().parent().find('input:text').each(function() {
          jQuery(this).val('');
      });
    }

    // Remove Photo in Photo Album
    function removePhotoInAlbum(claim_id, room_name, image_id) {
      var _url = Drupal.settings.basePath+'ajax/ncn_create_claim/remove_photo/'+claim_id;

      jQuery.ajax({
        url:   _url,
        type:   'POST',
        data:{room_name: room_name, image_id: image_id },
        beforeSend: function(jqXHR, settings) {

        },
        error: function() {

        },
        success: function(response) {
          eval("var json=" + response + ";");
          if (json.code == "success") {
            var blank_path = json.image_blank_path;
            jQuery("#"+image_id).attr('src', blank_path);
            jQuery(".ar_imb_box_"+image_id+" .remove-photo-album").css('display', 'none');
          }
          else {

          }
        }  // END OF SUCCESS FUNCTION
      });
    }

    function ncn_scopesheet_sections_tinyscrollbar() {
      jQuery("#ss_claim_room_scope_sheet_form .aci-section-content").find('.tinyscrollbar').each(function()
        {
          jQuery(this).tinyscrollbar({wheel: 25});
        });
    }

    function ncn_change_submit_button_disabled(btn_id, change_title, form_id) {
      return true;

      if (change_title != "") {
        jQuery("#"+btn_id).val(change_title);
      }
      jQuery("#"+btn_id).attr('disabled', 'disabled');
      if (form_id != "") {
        jQuery("#"+form_id).submit();
        return false;
      }
      return true;
    }

    function on_click_ncn_claims_manager_update_note(note_id) {
      var _url = Drupal.settings.basePath+'ajax/ncn_admin/get_claim_note';
      jQuery.ajax({
        url:   _url,
        type:   'POST',
        data:{note_id: note_id},
        beforeSend: function(jqXHR, settings) {

        },
        error: function() {

        },
        success: function(response) {
          eval("var json=" + response + ";");
          if (json.code == "success") {
            var new_note = prompt("Please update claim note.", json.note);
            if (new_note != null && new_note != '') {
              ////// Update ///////////////////////////////////
              var _url_update = Drupal.settings.basePath+'account/claim_note/update';
              jQuery.ajax({
                url:   _url_update,
                type:   'POST',
                data:{note_id: note_id, note: new_note},
                beforeSend: function(jqXHR, settings) {

                },
                error: function() {

                },
                success: function(response) {
                  eval("var json=" + response + ";");
                  if (json.code == "success") {
                    location.reload(true);
                  }
                  else {
                    if (json.msg != "") { alert(json.msg); }
                  }
                }  // END OF SUCESS FUNCTION
              });

            }
          }
          else {
            if (json.msg != "") { alert(json.msg); }
          }
        }  // END OF SUCESS FUNCTION
      });
    }
    
    function bind_ncn_admin_ss_services_cboxlabel_buttons(){
        jQuery('#ss_services_content label.cboxlabel input').bind('click',function() {
            var parentli= jQuery(this).parents('li')[0];
            jQuery(parentli).find('input[type="radio"].cboxradio').attr('checked', 'checked');
        });
    
        jQuery('#ss_services_content input.cboxradio').bind('click',function() {
            jQuery(this).parents('li').find('label.cboxlabel')[0].click();
        });
    }
