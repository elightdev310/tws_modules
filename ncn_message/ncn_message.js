function pm_toggle_message(msg_id, base_url)
{
	var ele = document.getElementById("body_" + msg_id);
	if (ele.style.display == "block")
	{
		jQuery('#body_' + msg_id).slideUp('fast');
	}
	else
	{
		// show the body
		jQuery('#body_' + msg_id).slideDown('fast');
		// change subject display to "read" style
		jQuery('#subject_append_' + msg_id).hide();
		jQuery('#subject_' + msg_id).removeClass().addClass('subject_read');
		// set "read" to "1" (in db via ajax)
		jQuery.get(base_url + msg_id, function(result) {});
	}
}

function pm_select_page(page_num)
{
	obj_name = "." + "ncn-pager-page-" + page_number;
	jQuery(obj_name).removeClass('active');
	obj_name = "." + "ncn-pager-page-" + page_num;
	jQuery(obj_name).addClass('active');
	page_number = page_num;
	jQuery('.ncn-pager-control a').each(function()
	{
		jQuery(this).removeClass('disabled');
	});
	if (page_num <= 1)
	{
		jQuery('.ncn-pager-control .first').addClass('disabled');
		jQuery('.ncn-pager-control .prev').addClass('disabled');
	}
	if (page_num >= page_count)
	{
		jQuery('.ncn-pager-control .next').addClass('disabled');
		jQuery('.ncn-pager-control .last').addClass('disabled');
	}
	var str_ranger;
	var from_r, to_r;
	if (total_records != 0)
	{
		from_r = (page_number - 1) * page_records + 1;
		to_r = page_number * page_records;
		if (to_r > total_records)
		{
			to_r = total_records;
		}
		str_ranger = from_r + " - " + to_r + " of " + total_records;
		jQuery('.ncn-pager-ranger').html(str_ranger);
	}
}

function pm_page_action(action_name)
{
	if (action_name == "first")
	{
		page_num = 1;
	}
	else if (action_name == "prev")
	{
		page_num = page_number - 1;
	}
	else if (action_name == "next")
	{
		page_num = page_number + 1;
	}
	else if (action_name == "last")
	{
		page_num = page_count;
	}
	if (page_num < 1)
	{
		page_num = 1;
	}
	else if (page_num > page_count)
	{
		page_num = page_count;
	}
	pm_select_page(page_num);
}

function backToHistoryReload() {
	window.location.href = document.referrer;
}
