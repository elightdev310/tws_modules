function on_click_delete_group(url){
	if (confirm(Drupal.t('Are you sure you want to delete group?'))) {
		location.href = Drupal.settings.basePath+url;
	}
}

function on_click_delete_item(url){
	if (confirm(Drupal.t('Are you sure you want to delete item?'))) {
		location.href = Drupal.settings.basePath+url;
	}
}