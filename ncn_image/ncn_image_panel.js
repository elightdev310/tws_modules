var jcrop_api;
var b_jcrop_api = false;
jQuery(window).load(function() {
    onchange_img_operation();
});

jQuery(document).ready(function() {
    jQuery(document).delegate('#img_operation', 'change', onchange_img_operation);

    jQuery(document).delegate('#img_roomname', 'change', onchange_img_roomname);

    jQuery(document).delegate('#img_action', 'change', onchange_img_action);

    jQuery(document).delegate('#image_action_submit', 'click', function() {
        var _img_form = jQuery('#ncn_image_box #image_action_form');
        var _url = Drupal.settings.basePath+"ajax/ncn_image_action/" +  jQuery('#img_action').val();

        jQuery.ajax({
            url:    _url,
            data:   _img_form.serialize(),
            type:   'POST',
            beforeSend: function(jqXHR, settings) {
                jQuery('#ncn_image_box .image_screen img').attr('src', Drupal.settings.basePath+'sites/default/files/loading.gif');
            },
            error: function() {
                alert('Error: It is failed to submit Image Action');
            },
            success: function(response) {
                eval("var json=" + response + ";");
                jQuery('#ncn_image_box .image_screen img').attr('src', Drupal.settings.basePath+json.image_path+'?rid=' + Math.random());
                if (json.success == "success") {
                    jQuery('#ncn_image_box .image_info_size').html(json.image_info.width + " x " + json.image_info.height);
                    field_reset();
                    jQuery('#ncn_image_box .image_screen img').css('width', json.image_info.width);
                    jQuery('#ncn_image_box .image_screen img').css('height', json.image_info.height);
                    jQuery('#ncn_image_box #ncn_image_file_width_orignal').val(json.image_info.width);
                    jQuery('#ncn_image_box #ncn_image_file_height_orignal').val(json.image_info.height);
                }
                else {
                    alert('Please input parameter fields, correctly.');
                }
                
                onchange_img_action();
            }   // END OF SUCESS FUNCTION
        });
        return;
    });

    jQuery(document).delegate('#image_action_save', 'click', function() {
        var _url = Drupal.settings.basePath + "ajax/ncn_image_action_save/";
        jQuery.ajax({
            url:    _url,
            data:   {},
            type:   'POST',
            error: function() {
                alert('Error: It is failed to save Image.');
            },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.success == "success") {
                    alert('Save Image, successfully.');
                }
                else {
                    alert('Please input parameter fields, correctly.');
                }
            }   // END OF SUCESS FUNCTION
        });
        return;
    });

    jQuery(document).delegate('#image_action_save_close', 'click', function() {
        var _url = Drupal.settings.basePath + "ajax/ncn_image_action_save/";
        jQuery.ajax({
            url:    _url,
            data:   {},
            type:   'POST',
            error: function() {
                alert('Error: It is failed to save Image.');
            },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.success == "success") {
                    parent.jQuery.colorbox.close();
                    location.href = location.href;
                }
                else {
                    alert('Please input parameter fields, correctly.');
                }
            }   // END OF SUCESS FUNCTION
        });
        return;
    });

    jQuery(document).delegate('#image_swap_save', 'click', function() {
        var _url = Drupal.settings.basePath + "ajax/ncn_image_swap_save/";
        var _img_form = jQuery('#ncn_image_box #image_swap_form');
        jQuery.ajax({
            url:    _url,
            data:   _img_form.serialize(),
            type:   'POST',
            error: function() {
                alert('Error: It is failed to swap Image.');
            },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.flag == "success") {
                    jQuery('#change_image #room_name').val(json.room_name);
                    jQuery('#change_image #position').val(json.position);
                    jQuery('#swap_image #room_name').val(json.room_name);
                    jQuery('#swap_image #position').val(json.position);
                    jQuery('#update_image #room_name').val(json.room_name);
                    jQuery('#update_image #position').val(json.position);
                    alert('Save Image, successfully.');
                }
                else {
                    alert('Error: It is failed to swap Image.');
                }
            }   // END OF SUCESS FUNCTION
        });
        return;
    });

    jQuery(document).delegate('#image_swap_save_close', 'click', function() {
        var _url = Drupal.settings.basePath + "ajax/ncn_image_swap_save/";
        var _img_form = jQuery('#ncn_image_box #image_swap_form');
        jQuery.ajax({
            url:    _url,
            data:   _img_form.serialize(),
            type:   'POST',
            error: function() {
                alert('Error: It is failed to swap Image.');
            },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.flag == "success") {
                    jQuery('#swap_image #room_name').val(json.room_name);
                    jQuery('#swap_image #position').val(json.position);
                    parent.jQuery.colorbox.close();
                    location.href = location.href;
                }
                else {
                    alert('Error: It is failed to swap Image.');
                }
            }   // END OF SUCESS FUNCTION
        });
        return;
    });
});

function initJcrop()
{
    jQuery('#cropbox').Jcrop({
        onChange: showCoords,
        onSelect: showCoords
    }, function() {
        jcrop_api = this;
        b_jcrop_api = true;
    });
};

function showCoords(c)
{
    jQuery('#crop_xoffset').val(c.x);
    jQuery('#crop_yoffset').val(c.y);
    //jQuery('#x2').val(c.x2);
    //jQuery('#y2').val(c.y2);
    jQuery('#crop_width').val(c.w);
    jQuery('#crop_height').val(c.h);
};

function onchange_img_operation() {
    var img_url = jQuery('#ncn_image_box #ncn_image_file_path_orignal').val();
    
    var img_operation;
    img_operation = jQuery('#img_operation').val();
    jQuery('.img_operation').each(function(index) {
        jQuery(this).removeClass('active');
    });
    
    jQuery('#'+img_operation).addClass('active');
    
    if (jcrop_api && b_jcrop_api) { 
        jcrop_api.destroy(); 
        b_jcrop_api = false;
    }
    
    if (img_operation == 'update_image') {
        var img_width = jQuery('#ncn_image_box #ncn_image_file_width_orignal').val();
        var img_height = jQuery('#ncn_image_box #ncn_image_file_height_orignal').val();
        jQuery('#ncn_image_box .image_screen img').css('width', img_width+'px');
        jQuery('#ncn_image_box .image_screen img').css('height', img_height+'px');
        
        img_url = jQuery('#ncn_image_box #ncn_image_file_path_crop').val();
        
        jQuery('#ncn_image_box .image_screen img').attr('src', Drupal.settings.basePath+img_url+'?rid=' + Math.random());
        onchange_img_action();
    } else {
        if (img_operation == 'change_image') {
            img_url = jQuery('#ncn_image_box #image_change_form #ncn_image_path').val();
        } else if (img_operation == 'swap_image') {
            onchange_img_roomname();
        }
        jQuery('#ncn_image_box .image_screen #image_wrapper').html("<img src='"+(Drupal.settings.basePath+img_url+'?rid=' + Math.random())+"' id='cropbox' alt='No Image'/>");
    }
}

function onchange_img_roomname() {
    var img_roomname;
    var claim_id = jQuery('#ncn_image_box #image_swap_form #claim_id').val();
    img_roomname = jQuery('#img_roomname').val();
    var _url = Drupal.settings.basePath + "ajax/ncn_image_position/" + claim_id + "/" + encodeURIComponent(img_roomname);
    
    jQuery.ajax({
        url:    _url,
        type:   'POST',
        beforeSend: function(jqXHR, settings) {
            //jQuery('#ncn_image_box .image_screen img').attr('src', Drupal.settings.basePath+'sites/default/files/loading.gif');
        },
        error: function() {
            alert('Error: It is failed to get image positions');
        },
        success: function(response) {
            eval("var json=" + response + ";");
            if (json.flag == "success") {
                jQuery('#ncn_image_box #img_position_wrapper').html(json.img_position);
            }
        }   // END OF SUCESS FUNCTION
    });
            
}

function onchange_img_action() {
    var img_action;
    if (jcrop_api && b_jcrop_api) {
        jcrop_api.destroy();
        b_jcrop_api = false;
    }
    img_action = jQuery('#img_action').val();
    jQuery('.params .param-box').each(function(index) {
        jQuery(this).removeClass('active');
    });
    
    jQuery('#'+img_action).addClass('active');
    if (img_action == 'img_crop') {
        initJcrop();
    }
}

function field_reset() {
    jQuery('.params .img_input input').each(function(index) {
        jQuery(this).val('');
    });
}
