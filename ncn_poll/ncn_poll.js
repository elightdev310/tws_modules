function open_colorbox_window(in_url, width, height) {
	// open the colorbox
	$.colorbox({width:(width+"px"),height:(height+"px"),href:in_url,open:true,iframe:true, overlayClose:false, onClosed:function(){ render_splash_pages('ncn_poll', 'question'); }});
}

function onclick_delete_poll() {
	if (confirm("Are you sure you want to delete this poll?")) {
		return true;
	}
	return false;
}