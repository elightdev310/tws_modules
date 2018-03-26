function create_claim_page1() {
	//if (jQuery('#reconstruction-invoice').is(':checked')) {
	var claim_type 		= jQuery('#claim_product').val();
	var claim_product 	= jQuery('#claim_product').val();
	if (claim_type == "" || claim_product == "") {	
		alert( Drupal.t('Choose Claim Type.') );
		return false;
	}
	if (claim_product == 'Reconstruction') {
		if (!confirm("You selected Reconstruction Invoice. Do you want to create a claim with this invoice?")) {
			return false;
		}
	}
	jQuery('.create-claim-ccpage #create-claim-form').submit();
}

function create_claim_agree(){
	jQuery('.claim-confirm-ccpage #term-agree-form').submit();
}

function get_claim_invoice_price() {
	
	var claim_type, claim_product, expedite;
	claim_type 		= jQuery('#claim_type').val();
	claim_product 	= jQuery('#claim_product').val();
	
	expedite = '';
	if (jQuery('#fm_expedite').is(':checked')) {
		expedite = 'on';
	}
	
	var _url = Drupal.settings.basePath+'ajax/ncn_create_claim/get_claim_invoice';
	
	jQuery.ajax({
		url: 	_url,
		type: 	'POST',
		data:{claim_type: claim_type, claim_product: claim_product, expedite: expedite },
		beforeSend: function(jqXHR, settings) {
		
		},
		error: function() {
			jQuery('#claim_price').html('');
			jQuery('#expedite_price').html('');
		},
		success: function(response) {
			eval("var json=" + response + ";");
			if (json.code == "success") {
				if (json.price == '') {
					jQuery('#claim_price').html('');
					jQuery('#expedite_price').html('');
				} else {
					jQuery('#claim_price').html("("+json.price+")");
					if (expedite == 'on') {
						jQuery('#expedite_price').html(json.price);
					}
				}
			}
			else {
				jQuery('#claim_price').html('');
				jQuery('#expedite_price').html('');
			}
		}	// END OF SUCCESS FUNCTION
	});	
}

function get_claim_invoice_expedite_price() {
	var claim_type, claim_product, expedite;
	claim_type 		= jQuery('#claim_type').val();
	claim_product 	= jQuery('#claim_product').val();
	
	expedite = '';
	if (jQuery('#fm_expedite').is(':checked')) {
		expedite = 'on';
	} else {
		jQuery('#expedite_price').html('');
		return;
	}
	
	var _url = Drupal.settings.basePath+'ajax/ncn_create_claim/get_claim_invoice';
	
	jQuery.ajax({
		url: 	_url,
		type: 	'POST',
		data:{claim_type: claim_type, claim_product: claim_product, expedite: expedite },
		beforeSend: function(jqXHR, settings) {
		
		},
		error: function() {
			jQuery('#expedite_price').html('');
		},
		success: function(response) {
			eval("var json=" + response + ";");
			if (json.code == "success") {
				if (json.price == '') {
					jQuery('#expedite_price').html('');
				} else {
					jQuery('#expedite_price').html(json.price);
				}
			}
			else {
				jQuery('#expedite_price').html('');
			}
		}	// END OF SUCCESS FUNCTION
	});	
}
