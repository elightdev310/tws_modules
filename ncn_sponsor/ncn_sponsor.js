function on_click_sponsor_image(obj_name) {
	/*$('#'+obj_name+' .loading')
	.ajaxStart(function(){
		$(this).show();
	})
	.ajaxComplete(function(){
		$(this).hide();
	});*/

	var _url = Drupal.settings.basePath+'ajax/sponsor_file_upload';
    jQuery.ajaxFileUpload
	(
		{
			url: _url,
			secureuri:false,
			fileElementId:'sponsor_image_file',
			dataType: 'json',
			data:{div_id: obj_name},
			success: function (data, status)
			{
				if(typeof(data.error) != 'undefined')
				{
					if(data.error != '')
					{
						alert(data.error);
					}else
					{
						jQuery('#'+obj_name).html(HTMLDecode(data.content));
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

function on_click_sponsor_section(sponsor_id) {
	var _url = Drupal.settings.basePath+'ajax/sponsor_click_sponsor_image';
	jQuery.ajax({
		url: 	_url,
		type: 	'POST',
		data:{sponsor_id: sponsor_id},
		beforeSend: function(jqXHR, settings) {
		
		},
		error: function() {
			
		},
		success: function(response) {
			
		}	// END OF SUCESS FUNCTION
	});
}

function on_click_sponsor_image_remove(obj_name) {
	var _url = Drupal.settings.basePath+'ajax/sponsor_remove_uploaded_file';
	jQuery.ajax({
		url: 	_url,
		type: 	'POST',
		data:{div_id: obj_name},
		beforeSend: function(jqXHR, settings) {
		
		},
		error: function() {
			
		},
		success: function(response) {
			jQuery('#'+obj_name).html(response);
		}	// END OF SUCESS FUNCTION
	});
}

function HTMLEncode(html) 
{ 
var temp = document.createElement ("div"); 
(temp.textContent != null) ? (temp.textContent = html) : (temp.innerText = html); 
var output = temp.innerHTML; 
temp = null; 
return output; 
} 
function HTMLDecode(text) 
{ 
var temp = document.createElement("div"); 
temp.innerHTML = text; 
var output = temp.innerText || temp.textContent; 
temp = null; 
return output; 
} 

function on_click_ncn_sponsor_delete() {
	if (confirm(Drupal.t('Are you sure you want to remove a splash?'))) {
		return true;
	}
	return false;
}