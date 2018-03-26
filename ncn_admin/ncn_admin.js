jQuery(document).ready(function() {
	jQuery('#dist_asso').bind('change', onchange_dist_asso);
	onchange_dist_asso();
	check_change_order_requests_choosed();	
	jQuery('#claim-change-order-request-list-cor .tr-cor .paid input').each(function(index) {
		jQuery(this).bind('click', check_change_order_requests_choosed);
	});
});

function onchange_dist_asso() {
	var list_dropdown = "." + jQuery('#dist_asso').val() + "-list.da-dropdown";
	jQuery('.da-dropdown').each(function() {
		jQuery(this).css('display', 'none');
	});
	jQuery(list_dropdown).css('display', 'table-row');
}
//------------------------------------------------------------------------------
// used to change status on main form rather than nasty post+redirection 
function jump_status(base_url, url_part)
{

	var filter = document.getElementById('filter_status_id').value;
	var user = document.getElementById('filter_user_id').value;
	var workflow = document.getElementById('filter_claim_workflow').value;
	var claim_id = document.getElementById('filter_claim_id').value;

	var url = base_url+"/admin/config/"+url_part+"/filter/"+filter+"/user/"+user+"/workflow/"+workflow+"/claim_id/"+claim_id;
	
	window.location = url;

}

function jump_filter_url(suf_url) {
	var filter = document.getElementById('payment_filter').value;
	var date_specific = document.getElementById('specific_date').value;
  var claim_expedite = document.getElementById('claim_expedite').value;
	var url = suf_url+"/filter/"+filter;
  
  if (date_specific != '' || claim_expedite != '') { url += '?'; }
  var first_param = 1;
	if (date_specific != '') {
		url += ("specific_date="+encodeURIComponent(date_specific));
    first_param = 0;
	}
  if (claim_expedite != '') {
    if (first_param == 0) { url += '&'; }
	  url += ("claim_expedite="+claim_expedite);
  }
  
	window.location = url;
}
//------------------------------------------------------------------------------
// for claims list pagination
function change_list_page(new_page)
{
	// show/hide tables
	jQuery('.page_table').hide();
	jQuery('#page_table_'+new_page).show();
	
	// unbold/bold page links
	for (var i=1; i<=total_list_pages; i++)
	{
		if (i != new_page)
		{
			jQuery('#page_link_'+i).html('<u>'+(i.toString())+'</u>');
		}
		else	
			jQuery('#page_link_'+i).html('<strong><u>'+(i.toString())+'</u></strong>');
	}
}

//------------------------------------------------------------------------------

function update_profile_submit(){
	jQuery('#update_profile_form').submit();
}
function upload_member_logo() {
	document.getElementById('upload_member_logo_form').submit();
}
	
function remove_member_logo() {
	jQuery('#upload_member_logo_form #member_logo_tfunction').val('remove_member_logo');
	document.getElementById('upload_member_logo_form').submit();
}
function update_credit_card_submit() {
	jQuery('#update_credit_card_form').submit();
}

function delete_claim(claim_id) {
	if (confirm("Are you sure you want to delete claim("+claim_id+")?" )) {
		return true;
	}
	return false;
}

function on_click_transaction_delete(pid) {
	if (confirm("Are you sure  you want to delete payment transaction(#"+pid+")?" )) {
		jQuery('#param_tfunction').val( 'delete_transaction' );
		jQuery('#param_pid').val( pid );
		jQuery('#transaction_filter').submit();
	}
}

function on_click_transaction_refund(pid) {
	if (confirm("Are you sure you want to refund payment transaction(#"+pid+")?" )) {
		jQuery('#param_tfunction').val( 'refund_transaction' );
		jQuery('#param_pid').val( pid );
		jQuery('#transaction_filter').submit();
	}
}

function toggle_tr(tr_id) {
	var ele = document.getElementById(tr_id);
	
	if (ele.style.display == "block")
	{
		jQuery('#'+tr_id).slideUp('normal');
		
	} 
	else
	{
		jQuery('#'+tr_id).slideDown('fast');
	}
}

function on_click_edit_roomname(claim_id, roomname) {
	var new_roomname = prompt("Please input room name.", roomname);
	
	if (new_roomname == null || string_trim(new_roomname) == "" || string_trim(new_roomname) == roomname) {
		return;
	}
	
	var _url = Drupal.settings.basePath+'ajax/ncn_admin/edit_roomname';
	jQuery.ajax({
		url: 	_url,
		type: 	'POST',
		data:{claim_id: claim_id, old_roomname: roomname, new_roomname: string_trim(new_roomname)},
		beforeSend: function(jqXHR, settings) {
		
		},
		error: function() {
			var params = {
				"!roomname": roomname
			};
			alert(Drupal.t('Failed to rename room (!roomname)', params));
		},
		success: function(response) {
			eval("var json=" + response + ";");
			if (json.code == "success") {
				if (json.msg != "") { alert(json.msg); location.href = document.URL; }
			}
			else {
        if (json.msg != "") { alert(json.msg); }
			}
		}	// END OF SUCESS FUNCTION
	});	
}

function on_click_ncn_admin_add_room() {
	var roomname = prompt("Please input room name.", "");
	roomname = string_trim(roomname)
	if (roomname == null || roomname == "") {
		return;
	}
	jQuery('#ncn_admin_add_room #room_name').val(roomname);
	jQuery('.current_scroll_position').val(get_current_scroll_position());
	jQuery('#ncn_admin_add_room').submit();
}

function on_click_ncn_admin_delete_room(roomname) {
	if (!confirm("Are you sure you want to delete a room ("+roomname+")?" )) {
		return;
	}
	jQuery('#ncn_admin_delete_room #room_name').val(roomname);
	jQuery('.current_scroll_position').val(get_current_scroll_position());
	jQuery('#ncn_admin_delete_room').submit();
}

function on_click_ncn_admin_delete_first_room(roomname) {
	if (!confirm("Are you sure you want to delete a room ("+roomname+")?" )) {
		return;
	}
	jQuery('#ncn_admin_delete_first_room #room_name').val(roomname);
	jQuery('.current_scroll_position').val(get_current_scroll_position());
	jQuery('#ncn_admin_delete_first_room').submit();
}

function on_click_ncn_admin_una_member_delete() {
	if (confirm(Drupal.t('Are you sure you want to remove an unactivated member?'))) {
		return true;
	}
	return false;
}

function on_click_upload_scope_full_pdf(base_path, obj_name, claim_id, roomname) {
	var _url = Drupal.settings.basePath+'ajax/upload_scope_full_pdf/'+claim_id+'/'+encodeURIComponent(roomname);
	jQuery.ajaxFileUpload
	(
		{
			url: _url,
			secureuri:false,
			fileElementId: obj_name,
			dataType: 'json',
			data:{},
			success: function (data, status)
			{
				if(typeof(data.error) != 'undefined')
				{
					if(data.error != '')
					{
						alert(data.error);
					}else
					{
						open_scope_full_cropbox(base_path, claim_id, roomname, data.filepath);
					}
				}
			},
			error: function (data, status, e)
			{
				alert(e);
			}
		}
	);
	return false;
}

function open_scope_full_cropbox(base_path, claim_id, room_name, img_url) {
	
	var win_width="800px";
	var win_height="620px";
	var in_url = base_path+"/admin/config/viewclaim/scope_file/crop/"+claim_id+"/"+encodeURIComponent(room_name)+"/"+encodeURIComponent(img_url);
	
	jQuery.colorbox({ width:win_width, height:win_height, 
		href:in_url, open:true, iframe:true, overlayClose:false, escKey:false, 
		onClosed:function(){
			jQuery('.current_scroll_position').val(get_current_scroll_position());
			
			var newURL = document.URL;
			
			if(newURL.search("current_scroll_position") > 0)
			{
				newURL = newURL.substr(0, newURL.search("current_scroll_position") -1);
			}
			
			location.href = newURL + "?current_scroll_position=" + get_current_scroll_position();
		
		} 
	});
	
}

function on_change_ncn_admin_membership_cal_time(dateText, inst) 
{
	var url;
	url = jQuery('#membership_cal_url').val();
	window.location = url+"?rsd="+dateText;
}

function set_scroll_position(classname)
{	
	var delete_link = jQuery('.' + classname).attr('href');
	delete_link = delete_link + "/?current_scroll_position=" + get_current_scroll_position();
	jQuery('.' + classname).attr('href', delete_link);
}

function get_current_scroll_position()
{    
	var scroll;    
	// Netscape compliant  
	if (typeof(window.pageYOffset) == 'number')    
		scroll = window.pageYOffset;  
	// DOM compliant  
	else if (document.body && document.body.scrollTop)    
		scroll = document.body.scrollTop;  
	// IE6 standards compliant mode  
	else if (document.documentElement && document.documentElement.scrollTop)    
		scroll = document.documentElement.scrollTop;  
	// needed for IE6 (when vertical scroll bar is on the top)  
	else scroll = 0;    
	
	return scroll;
}

function check_change_order_requests_choosed() {
	var ck_result = false;
	jQuery('#claim-change-order-request-list-cor tr .paid input').each(function(index) {
		if (jQuery(this).is(':checked')) {
			ck_result = true;
		}
	});
	if (ck_result) {
		jQuery('#cor_btn_charge').removeAttr("disabled");
		
		jQuery('#cor_btn_charge').css('color', '#494949');
	} else {
		jQuery('#cor_btn_charge').attr('disabled', 'disabled');
		
		jQuery('#cor_btn_charge').css('color', '#888');
	}
}

function mutli_claims_action(action) {
  if (confirm("Are you sure you want to delete selected claims?" )) {
    jQuery('#tfunction_mutli_claims').val(action);
    jQuery('#frm-check-mutli-claims').submit();
  }
  
  return false;
}

function check_change_mutli_claim() {
  var ck_result = false;
  jQuery('#frm-check-mutli-claims input.check-claim').each(function(index) {
    if (jQuery(this).is(':checked')) {
      ck_result = true;
    }
  });
  if (ck_result) {
    jQuery('#frm-check-mutli-claims .claim-select-actions').css('display', 'block');
  } else {
    jQuery('#frm-check-mutli-claims .claim-select-actions').css('display', 'none');
  }
}

function on_click_ncn_admin_update_note(note_id) {
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
        jQuery('#ncn_admin_claim_note_form').removeClass('claim-note-add-form');
        jQuery('#ncn_admin_claim_note_form').removeClass('claim-note-delete-form');
        jQuery('#ncn_admin_claim_note_form').addClass('claim-note-update-form');
        
        jQuery("#ncn_admin_claim_note_form #tfunction_claim_note_action").val("tfunction_claim_note_update");
        jQuery("#ncn_admin_claim_note_form #claim_note_id").val(note_id);
        jQuery('#ncn_admin_claim_note_form #ncn_admin_claim_note_content').val(json.note);
        jQuery("#ncn_admin_claim_note_form #ncn_admin_claim_note_submit_btn").val("Update");
        
        jQuery('#ncn_admin_claim_note_form #ncn_admin_claim_note_content').focus();
      }
      else {
        if (json.msg != "") { alert(json.msg); }
      }
    }  // END OF SUCESS FUNCTION
  });  
}

function on_click_claim_note_cancel_btn() {
  jQuery('#ncn_admin_claim_note_form').removeClass('claim-note-update-form');
  jQuery('#ncn_admin_claim_note_form').removeClass('claim-note-delete-form');
  jQuery('#ncn_admin_claim_note_form').addClass('claim-note-add-form');
  
  jQuery("#ncn_admin_claim_note_form #tfunction_claim_note_action").val("tfunction_claim_note_add");
  jQuery("#ncn_admin_claim_note_form #claim_note_id").val(0);
  jQuery('#ncn_admin_claim_note_form #ncn_admin_claim_note_content').val("");
  jQuery("#ncn_admin_claim_note_form #ncn_admin_claim_note_submit_btn").val("Add");
  
  jQuery('#ncn_admin_claim_note_form #ncn_admin_claim_note_content').focus();
}
