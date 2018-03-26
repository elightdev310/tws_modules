//------------------------------------------------------------------------------
function tab_click(id)
{
	//On Click Event
//	jQuery("ul.tabs li").click(function() {

		jQuery("ul.tabs li").removeClass("active"); //Remove any "active" class
		jQuery("#tab"+id).addClass("active"); //Add "active" class to selected tab
//		jQuery(".tab_content").hide(); //Hide all tab content

		var activeTab = jQuery("#tab"+id).find("a").attr("href"); //Find the href attribute value to identify the active tab + content

		jQuery('#tabcontent').hide();
		jQuery('#tabloading').show();

		jQuery.ajax(
		{ 
			url: activeTab, 
			cache: false, 
			success: function(message) 
			{
				jQuery("#tabcontent").empty().append(message);

				jQuery('#tabloading').hide();
				jQuery('#tabcontent').show();

				// setup image uploads
				var options = { 
					beforeSubmit: function(arr, $form, options) {
					    $form.find('.ar_imgbox').loadingOverlay();
					}, 
				    success: imageResponse
				};				
				jQuery('.ar_form').ajaxForm(options);
			} 
		});
		 

		return false;
//	});

}

//------------------------------------------------------------------------------
function imageResponse(res, statusText, xhr, $form)
{
	var results = res.split("|");
	
	if (results[1] == "success")
	{
		document.getElementById(results[3]).src = results[2];
		document.getElementById(results[3]+"_hidden").src = results[4];
		$form.find('.choose-photo-file').html('Replace Photo');
	}
	else
	{
		alert(results[2]);
	}
	$form.find('.ar_imgbox').loadingOverlay('remove');
}

//------------------------------------------------------------------------------
function delete_room(tab_index, claim_id, room_name)
{
	if (confirm('Are you sure you want to delete this room?'))
	{

		// build url
		var delete_url = Drupal.settings.basePath+'account/deleteroomtab/'+claim_id;
		
		// delet the images in the temporary session 
		jQuery.ajax(
		{ 
			url: delete_url, 
			type:   'POST',
      data:{'room_name': room_name}, 
			success: function(message) 
			{
				// remove the tab
				jQuery('#tab'+tab_index).remove();
			}
		});
	} 
		
	
}

//------------------------------------------------------------------------------
function rename_room(tab_index, claim_id)
{
	// get existing room name
	var room_name = jQuery('#roomname_text_'+tab_index).html();

	// grab the new room name from the user
	var new_name = prompt("Enter new room name", room_name);
	
	if (new_name == null)
	{	return false;		}

	// rename the room (in session)
	var rename_url = Drupal.settings.basePath+'account/renameroomtab/'+claim_id;

	jQuery.ajax(
	{ 
		url: rename_url, 
		type:   'POST',
    data:{'room_name': room_name, 'new_name': new_name}, 
		error: function() {
			alert(Drupal.t('Failed to rename the room.'));
		}, 
		success: function(response) 
		{
			eval("var json=" + response + ";");
			if (json.code == 'success') {
				// remove the tab
				jQuery('#roomname_text_'+tab_index).html(json.new_roomname);
				jQuery('#roomname_text_'+tab_index).attr("href", Drupal.settings.basePath+"account/roomtab/"+claim_id+"/"+json.new_roomname+"/"+tab_index);
				
				// need to reload the room to upload upload form "action" variable
				tab_click(tab_index);				
			} else {
				if (json.msg != '') {
					alert(json.msg);
				}
			}
		}
	});

	return false;
}

//------------------------------------------------------------------------------
function add_room(claim_id)
{
	var room_name = prompt("Enter New Room Name", "Room Name");

	if (room_name == null || str_trim(room_name)== "")
	{	return;		}
	
	room_name = str_trim(room_name);
	var _url = Drupal.settings.basePath+'account/addroomtab/'+claim_id;

	jQuery.ajax(
	{ 
		url: _url, 
		type:   'POST',
    data:{'room_name': room_name}, 
		error: function() {
			alert(Drupal.t('Failed to add a new room.'));
		}, 
		success: function(response) 
		{
			eval("var json=" + response + ";");
			if (json.code == 'success') {
        var new_roomname = json.new_roomname; 
				// add item to tabs
				tabindex += 1;
				
				var onclick_call = "delete_room("+tabindex+", "+claim_id+", '"+new_roomname+"');";
				var onclick_call_rename = "rename_room("+tabindex+", "+claim_id+", '"+new_roomname+"');";
				
				jQuery('#add_room_tab').before('<li id="tab'+tabindex+'"><div class="delete_room_button" onclick="'+onclick_call+'"></div><a href="'+Drupal.settings.basePath+'account/roomtab/'+claim_id+'/'+new_roomname+'/'+tabindex+'" onclick="return tab_click('+tabindex+');" class="roomname" id="roomname_text_'+tabindex+'">'+new_roomname+'</a> <a href="#" onclick="'+onclick_call_rename+'" class="roomedit"  id="rename_roomname_text_'+tabindex+'">[edit]</a></li>');
				tab_click(tabindex);
			} else {
				alert(json.msg);
			}
		}
	});
	
}

// Removes leading whitespaces
function LTrim( value ) {
	
	var re = /\s*((\S+\s*)*)/;
	return value.replace(re, "$1");
	
}

// Removes ending whitespaces
function RTrim( value ) {
	
	var re = /((\s*\S+)*)\s*/;
	return value.replace(re, "$1");
	
}

// Removes leading and ending whitespaces
function str_trim( value ) {
	
	return LTrim(RTrim(value));
	
}

//------------------------------------------------------------------------------
