function on_click_splash_image(obj_name) {
	/*$('#'+obj_name+' .loading')
	.ajaxStart(function(){
		$(this).show();
	})
	.ajaxComplete(function(){
		$(this).hide();
	});*/

	try{
		var _url = Drupal.settings.basePath+'ajax/splash_file_upload';
		jQuery.ajaxFileUpload
		(
			{
				url: _url,
				secureuri:false,
				fileElementId:'splash_image_file',
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
	} catch(err) {
		alert(err);	
	}
	
	return false;
}

function on_click_splash_image_remove(obj_name) {
	var _url = Drupal.settings.basePath+'ajax/splash_remove_uploaded_file';
	jQuery.ajax({
		url: 	_url,
		type: 	'POST',
		data:{div_id: obj_name},
		beforeSend: function(jqXHR, config) {

		},
		error: function() {

		},
		success: function(response) {
			jQuery('#'+obj_name).html(response);
		}	// END OF SUCESS FUNCTION
	});
}

function open_prompt_window(sid, width, height) {
	if (sid == 0) return;
	if (width == 0 || width>800) { width = 800; }
	if (height == 0 || height>600) { height = 600; }


	width += 30;
	height += 70;
	// open the colorbox
	var in_url = Drupal.settings.basePath+"ncn/ncn_splash_promptpage/"+sid;

	jQuery.colorbox({width:(width+"px"),height:(height+"px"),href:in_url,open:true,iframe:true, overlayClose:false, onClosed:function(){ render_splash_pages('ncn_splash', 'default');  }});
}

function render_splash_pages(module, type) {
	var _url = Drupal.settings.basePath+'ncn_splash/process/'+'?is_ajax=1';
	if (module != '') {
		_url = (_url+module+'/'+type+'/?is_ajax=1');
	}
	jQuery.ajax({
		url: 	_url,
		type: 	'POST',
		beforeSend: function(jqXHR, config) {
		
		},
		error: function() {
			
		},
		success: function(response) {
			eval("var json=" + response + ";");
			if (json.flag == "success") {
				eval(json.eval_code);
			}
			else {
				
			}
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