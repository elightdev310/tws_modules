function open_image_box(base_path, claim_id, room_name, img_position, img_url) {

    var win_width="820px";
    var win_height="620px";
    var in_url = base_path+"/admin/config/ncn_image_open/?";
    if (img_url != "") {
        win_height="620px";
        in_url = in_url + "img="+img_url+"&";
    }
    in_url = in_url+"claim_id="+claim_id+"&room_name="+encodeURIComponent(room_name) + "&position="+img_position;

    jQuery.colorbox({ width:win_width, height:win_height,
        href:in_url, open:true, iframe:true,
        onClosed:function(){
            var _url = base_path + "/ajax/ncn_image_clean_tmp";
            jQuery.get(_url, function(data)	{
                if (data=="reload") {
                    location.href = document.URL;
                }
            })
        }
    });

}

function on_click_ncn_change_image() {
    var _img_form = jQuery('#ncn_image_box #image_change_form');
    var _url = Drupal.settings.basePath+"ajax/ncn_upload_changed_image";
    //jcrop_api.destroy();
    //alert(_url);
    var claim_id = jQuery('#ncn_image_box #image_change_form #claim_id').val();
    jQuery.ajaxFileUpload
    (
        {
            url: _url,
            secureuri:false,
            fileElementId:'ncn_photo_file',
            dataType: 'json',
            data: 	{claim_id: claim_id},
            success: function (json, status)
            {
                if (json.flag == "success") {
                    jQuery('#ncn_image_box .image_screen img').attr('src', Drupal.settings.basePath+json.image_path+'?rid=' + Math.random());
                    jQuery('#ncn_image_box #image_change_form #ncn_image_path').val(json.image_path);
                }
                else {
                    jQuery('#ncn_image_box .image_screen img').attr('src', Drupal.settings.basePath+jQuery('#ncn_image_box #image_change_form #ncn_image_path').val()+'?rid=' + Math.random());
                    alert(json.msg);
                }
                //initJcrop();
            },
            error: function (data, status, e)
            {
                alert(e);
            }
        }
    );

}

function on_click_ncn_remove_image() {
    jQuery('#ncn_image_box .image_screen img').attr('src', '');
    jQuery('#ncn_image_box #image_change_form #ncn_image_path').val('');
}

jQuery(document).ready(function() {
    jQuery(document).delegate('#image_change_save', 'click',function() {
        var _url = Drupal.settings.basePath + "ajax/ncn_image_change_image_save/";
        var _img_form = jQuery('#ncn_image_box #image_change_form');
        jQuery.ajax({
            url: 	_url,
            data: 	_img_form.serialize(),
            type:	'POST',
            error: function() {
                alert('Error: It is failed to swap Image.');
            },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.flag == "success") {
                    alert('Change image, successfully.');
                    var img_url = jQuery('#ncn_image_box #image_change_form #ncn_image_path').val();
                    var claim_id =jQuery('#ncn_image_box #image_change_form #claim_id').val();
                    var room_name =jQuery('#ncn_image_box #image_change_form #room_name').val();
                    var img_position =jQuery('#ncn_image_box #image_change_form #position').val();

                    var in_url = Drupal.settings.basePath+"admin/config/ncn_image_open/?";
                    if (img_url != "") {
                        in_url = in_url + "img="+img_url+"&";
                    }
                    in_url = in_url+"claim_id="+claim_id+"&room_name="+encodeURIComponent(room_name) + "&position="+img_position;

                    location.href = in_url;

                }
                else {
                    alert(json.msg);
                }
            }	// END OF SUCESS FUNCTION
        });
        return;
    });

    jQuery(document).delegate('#image_change_save_close', 'click', function() {
        var _url = Drupal.settings.basePath + "ajax/ncn_image_change_image_save/";
        var _img_form = jQuery('#ncn_image_box #image_change_form');
        jQuery.ajax({
            url: 	_url,
            data: 	_img_form.serialize(),
            type:	'POST',
            error: function() {
                alert('Error: It is failed to swap Image.');
            },
            success: function(response) {
                eval("var json=" + response + ";");
                if (json.flag == "success") {
                    alert('Change image, successfully.');
                    parent.jQuery.colorbox.close();
                    location.href = location.href;

                    /*var img_url = jQuery('#ncn_image_box #image_change_form #ncn_image_path').val();
                     var claim_id =jQuery('#ncn_image_box #image_change_form #claim_id').val();
                     var room_name =jQuery('#ncn_image_box #image_change_form #room_name').val();
                     var img_position =jQuery('#ncn_image_box #image_change_form #position').val();

                     var in_url = Drupal.settings.basePath+"admin/config/ncn_image_open/?";
                     if (img_url != "") {
                     in_url = in_url + "img="+img_url+"&";
                     }
                     in_url = in_url+"claim_id="+claim_id+"&room_name="+encodeURIComponent(room_name) + "&position="+img_position;


                     location.href = in_url;*/

                }
                else {
                    alert(json.msg);
                }
            }	// END OF SUCESS FUNCTION
        });
        return;
    });
});
