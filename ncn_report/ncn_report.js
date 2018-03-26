function on_change_report_time(dateText, inst) 
{
	var url;
	url = jQuery('#report_url').val();
	window.location = url+"?rsd="+dateText;
}

function toggle_sub_tr(item_obj, sub_tr_id)
{
	/*var ele = document.getElementById(sub_tr_id);*/
	item_id = item_obj.id;
	
	if (jQuery('#'+sub_tr_id).hasClass('collapse'))
	{
		jQuery('#'+sub_tr_id).removeClass('collapse');
		jQuery('#'+sub_tr_id).addClass('expand');
		jQuery('#'+item_id).removeClass('collapse');
		jQuery('#'+item_id).addClass('expand');
	} 
	else
	{
		jQuery('#'+sub_tr_id).removeClass('expand');
		jQuery('#'+sub_tr_id).addClass('collapse');
		jQuery('#'+item_id).removeClass('expand');
		jQuery('#'+item_id).addClass('collapse');
	} 
}
