<?php

/*****************************************************************************/
/*  Scope Sheet
/*****************************************************************************/

/**
 * ScopeSheet Render - "Room Dimensions" Section
 */ 
function ncn_admin_ss_render_room_dimensions($claim_id, $room_name, $section) {
	$data = array();
	if (isset($_POST['ss_rd'])) {
		$data = $_POST['ss_rd'];
	} else {
		$data = ncn_admin_get_ss_room_dimensions($claim_id, $room_name);
	}
	
	ob_start();
?>
<div id="ss_rd_list">
	<?php print ncn_admin_ss_render_rd_list($claim_id, $room_name); ?>
</div>
<div id="ss_rd_action_panel_content" class="ss-rd-action-panel">
	<?php print ncn_admin_ss_render_rd_action_panel($claim_id, $room_name, 'new'); ?>
</div>
<?php	
	$output = ob_get_contents();
	ob_end_clean();
	
	return $output;
}

function ncn_admin_ss_render_rd_list($claim_id, $room_name) {
	$data = array();
	$data = ncn_admin_get_ss_rd_list($claim_id, $room_name);
	
	ob_start();
?>
<table class="ss-list-table">
<thead>
	<th class='ss-rd-name'></th>
	<th class='ss-rd-size'>Size</th>
	<th class='ss-rd-floor-type'>Floor Type</th>
	<th class='ss-rd-operation'></th>
</thead>
<tbody>
<?php foreach ($data as $key=>$item): ?>
	<tr>
		<td class='ss-rd-name'><?php echo ($item['type']=='Room')? $room_name:$item['type']; if ($item['type']=='Other') { echo "(".$item['name'].")"; } ?></td>
		<td class='ss-rd-size'><?php echo $item['size']['L'].' x '.$item['size']['W'].' x '.$item['size']['H']; ?></td>
		<td class='ss-rd-floor-type'><?php echo $item['floor_type']; ?></td>
		<td class='ss-rd-operation'>
			<a href="" onclick="render_ss_rd_action_panel(<?php echo $claim_id.", '$room_name'".", 'edit', $key"; ?>); return false;">Edit</a>
			<a href="" onclick="on_click_ss_rd_delete(<?php echo $claim_id.", '$room_name', $key"; ?>); return false;">Delete</a>
		</td>
	</tr>
<?php endforeach; ?>
</tbody>
</table>
<?php
	$output = ob_get_contents();
	ob_end_clean();
	
	return $output;
}



/**
 * ScopeSheet - Get Room Dimension List
 */
function ncn_admin_get_ss_rd_list($claim_id, $room_name) {
	$data = get_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_rd');
	if ($data) {
		$data = unserialize($data);
    if (is_array($data)) {
      $data = array_slice($data, 0);
    }
	} else {
		$data = array();
	}
	// Populate
	
	return $data;
}

function ncn_admin_ss_render_rd_action_panel($claim_id, $room_name, $action, $index=0) {
	$rd = array();
	if ($action == 'edit') {
		$data = ncn_admin_get_ss_rd_list($claim_id, $room_name);
		if (isset($data[$index])) { $rd = $data[$index]; }
	} else if ($action == 'new') {
		
	}
	ob_start();
?>
<table>
	<tr>
		<td class="td-label"><label for="ss_rd_item">
			<span><?php print ncn_admin_ss_render_rd_select_type($room_name, "ss_rd_type", "ss_rd[type]", isset($rd['type'])?$rd['type']:''); ?></span>
			<span id="ss_rd_name_wrapper" class="field-item text-field-item small-size">(<input type="text" name="ss_rd[name]" id="ss_rd_name" value="<?php echo isset($rd['name'])?$rd['name']:''; ?>" />)</span>:</label>
		</td>
		<td>
			<span class="field-item text-field-item small-size"><input type="text" name="ss_rd[size][L]" id="ss_rd_size_L" value="<?php echo isset($rd['size'])?$rd['size']['L']:''; ?>" /><label for="ss_rd_size_L">(L)X</label></span>
			<span class="field-item text-field-item small-size"><input type="text" name="ss_rd[size][W]" id="ss_rd_size_W" value="<?php echo isset($rd['size'])?$rd['size']['W']:''; ?>" /><label for="ss_rd_size_W">(W)X</label></span>
			<span class="field-item text-field-item small-size"><input type="text" name="ss_rd[size][H]" id="ss_rd_size_H" value="<?php echo isset($rd['size'])?$rd['size']['H']:''; ?>" /><label for="ss_rd_size_H">(H)</label></span>
		</td>
	</tr>
	<tr>
		<td class="td-label"></td>
		<!-- <td>
			<div class="field-item text-field-item small-size"><label for="ss_rd_tfa">Total floor affected:</label> 
			<input type="text" name="ss_rd[tfa]" id="ss_rd_tfa" value="<?php echo isset($rd['tfa'])?$rd['tfa']:''; ?>" />SF</div>			
		</td> -->
	</tr>
	<tr>
		<td class="td-label"></td>
		<td>
			<div class="field-item text-field-item small-size"><label for="ss_rd_floor_type">Floor Type:</label> 
			<?php print ncn_admin_ss_render_select_floor_type("ss_rd_floor_type", "ss_rd[floor_type]", isset($rd['floor_type'])?$rd['floor_type']:''); ?></div>			
		</td>
	</tr>	
	<tr>
		<td></td>
		<td>
			<div class='ss-rd-actions ss-actions'>
				<input type="hidden" id="ss_rd_action_type" name="ss_rd_action_type" value="<?php echo $action; ?>" />
				<input type="hidden" id="ss_rd_index" name="ss_rd_index" value="<?php echo $index; ?>" />
				<?php if ($action == 'new'): ?>
					<input type='button' value='  Add  ' onclick="on_click_ss_rd_action(<?php echo $claim_id; ?>, '<?php echo $room_name; ?>')"/>
				<?php else: ?>
					<input type='button' value='  OK  ' onclick="on_click_ss_rd_action(<?php echo $claim_id; ?>, '<?php echo $room_name; ?>')" />&nbsp;&nbsp;&nbsp;
					<input type='button' value='  Cancel  ' onclick="render_ss_rd_action_panel(<?php echo $claim_id; ?>, '<?php echo $room_name; ?>', 'new', 0)" />
				<?php endif; ?>
			</div>
		</td>
	</tr>
</table>
<script>
jQuery(function() {
	jQuery('#ss_rd_type').bind('change', onchange_ss_rd_type);
});
</script>
<?php 
	$output = ob_get_contents();
	ob_end_clean();
	return $output;
}

function ncn_admin_ss_render_rd_select_type($room_name, $id, $name, $selected_val) {
	$types = array(
		'Room'=>$room_name, 
		'Closet'=>'Closet', 
		'Other'=>'Other', 
	);
	
	$output = "<select id='$id' name='$name'>";
		$output.= "<option value=''> --------- </option>";
	foreach ($types as $key=>$type) {
		$selected = '';
		if ($key == $selected_val) { $selected = 'selected'; }
		$output.= "<option value='$key' $selected>$type</option>";
	}
	$output.= "<select>";
	
	return $output;
}

function ncn_admin_ss_render_select_floor_type($id, $name, $selected_val) {
	$floor_types = array(
		'Concrete'=>'Concrete', 
		'Carpet'=>'Carpet', 
		'Vinyl'=>'Vinyl', 
		'Laminate'=>'Laminate', 
		'Wood'=>'Wood', 
		'Tile'=>'Tile', 
	);
	
	$output = "<select id='$id' name='$name'>";
		$output.= "<option value=''>-- Please Select --</option>";
	foreach ($floor_types as $key=>$floor_type) {
		$selected = '';
		if ($key == $selected_val) { $selected = 'selected'; }
		$output.= "<option value='$key' $selected>$floor_type</option>";
	}
	$output.= "<select>";
	
	return $output;
}

/**
 * Menu Handler (ajax/ncn_admin/scopesheet/eit/%/render_rd_list)
 */
function ncn_admin_ajax_ss_rd_render_list($claim_id) {
	$room_name = $_POST['room_name'];
	
	$json = array();
	$json['code'] = 'success';
	$json['html']	= ncn_admin_ss_render_rd_list($claim_id, $room_name);
	
	echo json_encode($json);
	exit;
}

/**
 * Menu Handler (ajax/ncn_admin/scopesheet/rd/%/render_ss_rd_action_panel)
 */
function ncn_admin_ajax_ss_rd_render_action_panel($claim_id) {
	$room_name = $_POST['room_name'];
	
	$action = $_POST['action'];
	$index = $_POST['index'];
	
	$json = array();
	$json['code'] = 'success';
	$json['html']	= ncn_admin_ss_render_rd_action_panel($claim_id, $room_name, $action, $index);
	
	echo json_encode($json);
	exit;
}

/**
 * Menu Handler (ajax/ncn_admin/scopesheet/rd/%/action_submit)
 */
function ncn_admin_ajax_ss_rd_action_submit($claim_id) {
	$room_name = $_POST['room_name'];
	
	$action_type		= $_POST['action_type'];
	$ss_rd_index 		= $_POST['ss_rd_index'];
	
	$ss_rd_type			= $_POST['ss_rd_type'];
	$ss_rd_name			= $_POST['ss_rd_name'];
	if ($ss_rd_type == 'Closet' || $ss_rd_type == 'Room' ) {
		$ss_rd_name = '';
	}
	
	$ss_rd_size_L		= $_POST['ss_rd_size_L'];
	$ss_rd_size_W		= $_POST['ss_rd_size_W'];
	$ss_rd_size_H		= $_POST['ss_rd_size_H'];
	
//	$ss_rd_tfa			= $_POST['ss_rd_tfa'];
	$ss_rd_floor_type	= $_POST['ss_rd_floor_type'];
	
	$ss_rd_data = array(
		'type' => $ss_rd_type,
		'name' => $ss_rd_name,
		'size' => array(
			'L'=> $ss_rd_size_L, 
			'W'=> $ss_rd_size_W, 
			'H'=> $ss_rd_size_H, 
		),
//		'tfa'	 => $ss_rd_tfa,
		'floor_type' => $ss_rd_floor_type,
	);
	
	$json = array();
	$json['code'] = 'success';
	if ($action_type == 'new') {
		$json['msg'] = "Successfully Added Data.";
	} else {	// edit
		$json['msg'] = "Successfully Updated Data.";
	}
	
	if (!ncn_admin_ss_rd_submit($claim_id, $room_name, $action_type, $ss_rd_index, $ss_rd_data)) {
		$json['code'] = 'error-101';
		$json['msg'] = "Failed to save data.";
	}
	
	echo json_encode($json);
	exit;
}

/**
 * Menu Handler (ajax/ncn_admin/scopesheet/rd/%/delete)
 */
function ncn_admin_ajax_ss_rd_delete($claim_id) {
	$room_name = $_POST['room_name'];
	$index 	= $_POST['index'];
	
	$json = array();
	$json['code'] = 'success';
	$json['msg'] = "Successfully Deleted Data.";
	
	$action_type = 'delete';
	if (!ncn_admin_ss_rd_submit($claim_id, $room_name, $action_type, $index)) {
		$json['code'] = 'error-101';
		$json['msg'] = "Failed to delete data.";
	}
	
	echo json_encode($json);
	exit;
}

function ncn_admin_ss_rd_submit($claim_id, $room_name, $action, $index=0, $data=array()) {
	$items = ncn_admin_get_ss_rd_data($claim_id, $room_name);
	// Add & Update
	if ($action == 'new') {
		$items[] = $data;
	} else if ($action == 'edit') {
		$items[$index] = $data;
	} else if ($action == 'delete') {
		unset($items[$index]);
	}
	
	// Save
	if ( !set_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_rd', serialize($items)) ) {
		return FALSE;
	}
	
	return TRUE;
}

function ncn_admin_get_ss_rd_data($claim_id, $room_name) {
	$data = get_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_rd');
	if ($data) {
		$data = unserialize($data);
    if (is_array($data)) {
      $data = array_slice($data, 0);
    }
	} else {
		$data = array();
	}
	
	// Populate
	return $data;
}

function ncn_admin_get_ss_room_dimensions($claim_id, $room_name) {
	$data = get_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_room_dimensions');
	if ($data) {
		$data = unserialize($data);
	} else {
		$data = array();
	}
	
	// Populate
	
	return $data;
}




/**
 * ScopeSheet Render - "Exterior & Interior Temperatures" Section
 */ 

function ncn_admin_ss_render_ext_inter_temperatures($claim_id, $room_name, $section) {
	ob_start();
?>
<div id="ss_eit_list">
	<?php print ncn_admin_ss_render_eit_list($claim_id, $room_name); ?>
</div>
<div id="ss_eit_action_panel_content" class="ss-eit-action-panel">
	<?php print ncn_admin_ss_render_eit_action_panel($claim_id, $room_name, 'new'); ?>
</div>
<?php	
	$output = ob_get_contents();
	ob_end_clean();
	
	return $output;
}

function ncn_admin_ss_render_eit_list($claim_id, $room_name) {
	$data = array();
	$data = ncn_admin_get_ss_eit_list($claim_id, $room_name);
	
	ob_start();
?>
<table class="ss-list-table">
<thead>
	<th class='ss-eit-date'>Date</th>
	<th class='ss-eit-exterior-temp'>Exterior Temp / RH%</th>
	<th class='ss-eit-exterior-temp'>Interior Temp / RH%</th>
	<th class='ss-eit-gpp'>GPP</th>
	<th class='ss-eit-time-of-readings'>Time of Readings</th>
	<th class='ss-eit-operation'></th>
</thead>
<tbody>
<?php foreach ($data as $key=>$item): ?>
	<tr>
		<td class='ss-eit-date'><?php echo $item['date']; ?></td>
		<td class='ss-eit-exterior-temp'><?php echo $item['ext_temp']; ?> / <?php echo $item['ext_rh']; ?></td>
		<td class='ss-eit-exterior-temp'><?php echo $item['int_temp']; ?> / <?php echo $item['int_rh']; ?></td>
		<td class='ss-eit-gpp'><?php echo $item['gpp']; ?></td>
		<td class='ss-eit-time-of-readings'><?php echo ncn_admin_format_american_time($item['time_readings']).' '.$item['time_readings_half']; ?></td>
		<td class='ss-eit-operation'>
			<a href="" onclick="render_ss_eit_action_panel(<?php echo $claim_id.", '$room_name'".", 'edit', $key"; ?>); return false;">Edit</a>
			<a href="" onclick="on_click_ss_eit_delete(<?php echo $claim_id.", '$room_name', $key"; ?>); return false;">Delete</a>
		</td>
	</tr>
<?php endforeach; ?>
</tbody>
</table>
<?php
	$output = ob_get_contents();
	ob_end_clean();
	
	return $output;
}

function ncn_admin_format_american_time($time) {
	if (is_numeric($time)) {
		if (strlen($time)<=2) {
			return $time.":00";
		} else if (strlen($time)==3) {
			return substr($time, 0, 1).":".substr($time, -2);
		} else if (strlen($time)==4) {
			return substr($time, 0, 2).":".substr($time, -2);
		}
	} 
	
	return $time;
}

/**
 * ScopeSheet - Get Exterior & Interior Temperatures List
 */
function ncn_admin_get_ss_eit_list($claim_id, $room_name) {
	$data = get_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_eit');
	if ($data) {
		$data = unserialize($data);
    if (is_array($data)) {
      $data = array_slice($data, 0);
    }
    
	} else {
		$data = array();
	}
	// Populate
	
	return $data;
}
function ncn_admin_ss_render_eit_action_panel($claim_id, $room_name, $action, $index=0) {
    global $base_url;
	$eit = array();
	if ($action == 'edit') {
		$data = ncn_admin_get_ss_eit_list($claim_id, $room_name);
		if (isset($data[$index])) { $eit = $data[$index]; }
	} else if ($action == 'new') {
		$eit['date'] = date("m/d/Y"); 
	}
	ob_start();
?>
<table>
	<tr>
		<td class="td-label"><label for="ss_eit_date">Date (MM/DD/YY):</label></td>
		<td><div class="field-item text-field-item small-size"><input type="text" name="ss_eit[date]" id="ss_eit_date" value="<?php echo $eit['date']; ?>" onkeypress="return isDateKey(event);"/></div></td>
	</tr>
	<!-- <tr>
		<td class="td-label"><label for="ss_eit_staff"># Of Staff:</label></td>
		<td><div class="field-item text-field-item small-size"><input type="text" name="ss_eit[staff]" id="ss_eit_staff" value="<?php echo isset($eit['staff'])?$eit['staff']:''; ?>" /></div></td>
	</tr> -->
	<tr>
		<td class="td-label"><label for="ss_eit_ext_temp">Exterior Temp / RH%:</label></td>
		<td><div class="field-item text-field-item small-size">
			<input type="text" name="ss_eit[ext_temp]" id="ss_eit_ext_temp" value="<?php echo isset($eit['ext_temp'])?$eit['ext_temp']:''; ?>" /> / 
			<input type="text" name="ss_eit[ext_rh]" id="ss_eit_ext_rh" value="<?php echo isset($eit['ext_rh'])?$eit['ext_rh']:''; ?>" />
		</div></td>
	</tr>
	<tr>
		<td class="td-label"><label for="ss_eit_int_temp">Interior Temp / RH%:</label></td>
		<td><div class="field-item text-field-item small-size">
			<input type="text" name="ss_eit[int_temp]" id="ss_eit_int_temp" value="<?php echo isset($eit['int_temp'])?$eit['int_temp']:''; ?>" /> / 
			<input type="text" name="ss_eit[int_rh]" id="ss_eit_int_rh" value="<?php echo isset($eit['int_rh'])?$eit['int_rh']:''; ?>" />
		</div></td>
	</tr>
	<tr>
		<td class="td-label"><label for="ss_eit_gpp">GPP:</label></td>
		<td><div class="field-item text-field-item small-size"><input type="text" name="ss_eit[gpp]" id="ss_eit_gpp" value="<?php echo isset($eit['gpp'])?$eit['gpp']:''; ?>" /></div></td>
	</tr>
	<tr>
		<td class="td-label"><label for="ss_eit_time_readings">Time of Readings:</label></td>
		<td>
			<span class="field-item text-field-item small-size"><input type="text" name="ss_eit[time_readings]" id="ss_eit_time_readings" value="<?php echo isset($eit['time_readings'])?$eit['time_readings']:''; ?>" /></span>
			<ul class="field-check-lists inline-group-fields clearfix">
				<li class="inline-field-item"><input type="radio" name="ss_eit[time_readings_half]" id="ss_eit_time_readings_am" value="AM" <?php if(isset($eit['time_readings_half']) && $eit['time_readings_half']=='AM') { echo "checked='checked'";} ?> /><label for="ss_eit_time_readings_am">AM</label></li>
				<li class="inline-field-item"><input type="radio" name="ss_eit[time_readings_half]" id="ss_eit_time_readings_pm" value="PM" <?php if(isset($eit['time_readings_half']) && $eit['time_readings_half']=='PM') { echo "checked='checked'";} ?> /><label for="ss_eit_time_readings_pm">PM</label></li>
			</ul>
		</td>
	</tr>
	<tr>
		<td></td>
		<td>
			<div class='ss-eit-actions ss-actions'>
				<input type="hidden" id="ss_eit_action_type" name="ss_eit_action_type" value="<?php echo $action; ?>" />
				<input type="hidden" id="ss_eit_index" name="ss_eit_index" value="<?php echo $index; ?>" />
				<?php if ($action == 'new'): ?>
					<input type='button' value='  Add  ' onclick="on_click_ss_eit_action(<?php echo $claim_id; ?>, '<?php echo $room_name; ?>')"/>
				<?php else: ?>
					<input type='button' value='  OK  ' onclick="on_click_ss_eit_action(<?php echo $claim_id; ?>, '<?php echo $room_name; ?>')" />&nbsp;&nbsp;&nbsp;
					<input type='button' value='  Cancel  ' onclick="render_ss_eit_action_panel(<?php echo $claim_id; ?>, '<?php echo $room_name; ?>', 'new', 0)" />
				<?php endif; ?>
			</div>
		</td>
	</tr>
</table>
<script>
jQuery(function() {
	ncn_admin_ss_eit_datepicker();
});
function ncn_admin_ss_eit_datepicker() {
	jQuery( '#ss_eit_date' ).datepicker({
		showOn: "button",
		buttonImage: "<?php echo $base_url."/".drupal_get_path('module', 'ncn_report')."/images/calendar.gif"; ?>",
		buttonImageOnly: true,
		dateFormat: 'mm/dd/yy',
	});
}

</script>
<?php 
	$output = ob_get_contents();
	ob_end_clean();
	return $output;
}



/**
 * Menu Handler (ajax/ncn_admin/scopesheet/eit/%/render_eit_list)
 */
function ncn_admin_ajax_ss_eit_render_list($claim_id) {
	$room_name = $_POST['room_name'];
	
	$json = array();
	$json['code'] = 'success';
	$json['html']	= ncn_admin_ss_render_eit_list($claim_id, $room_name);
	
	echo json_encode($json);
	exit;
}

/**
 * Menu Handler (ajax/ncn_admin/scopesheet/eit/%/render_ss_eit_action_panel)
 */
function ncn_admin_ajax_ss_eit_render_action_panel($claim_id) {
	$room_name = $_POST['room_name'];
	
	$action = $_POST['action'];
	$index = $_POST['index'];
	
	$json = array();
	$json['code'] = 'success';
	$json['html']	= ncn_admin_ss_render_eit_action_panel($claim_id, $room_name, $action, $index);
	
	echo json_encode($json);
	exit;
}

/**
 * Menu Handler (ajax/ncn_admin/scopesheet/eit/%/action_submit)
 */
function ncn_admin_ajax_ss_eit_action_submit($claim_id) {
	$room_name = $_POST['room_name'];
	
	$action_type		= $_POST['action_type'];
	$ss_eit_index 	= $_POST['ss_eit_index'];
	
	$ss_eit_date		= $_POST['ss_eit_date'];
	//$ss_eit_staff		= $_POST['ss_eit_staff'];
	
	$ss_eit_ext_temp= $_POST['ss_eit_ext_temp'];
	$ss_eit_ext_rh	= $_POST['ss_eit_ext_rh'];
	$ss_eit_int_temp= $_POST['ss_eit_int_temp'];
	$ss_eit_int_rh	= $_POST['ss_eit_int_rh'];
	
	$ss_eit_gpp			= $_POST['ss_eit_gpp'];
	$time_readings	= $_POST['time_readings'];
	$time_readings_half	= $_POST['time_readings_half'];
	
	$ss_eit_data = array(
		'date' => $ss_eit_date,
		//'staff' => $ss_eit_staff,
		'ext_temp' => $ss_eit_ext_temp,
		'ext_rh' => $ss_eit_ext_rh,
		'int_temp' => $ss_eit_int_temp,
		'int_rh' => $ss_eit_int_rh,
		'gpp' => $ss_eit_gpp,
		'time_readings' => $time_readings,
		'time_readings_half' => $time_readings_half,
	);
	
	$json = array();
	$json['code'] = 'success';
	if ($action_type == 'new') {
		$json['msg'] = "Successfully Added Data.";
	} else {	// edit
		$json['msg'] = "Successfully Updated Data.";
	}
	
	if (!ncn_admin_ss_eit_submit($claim_id, $room_name, $action_type, $ss_eit_index, $ss_eit_data)) {
		$json['code'] = 'error-101';
		$json['msg'] = "Failed to save data.";
	}
	
	echo json_encode($json);
	exit;
}

/**
 * Menu Handler (ajax/ncn_admin/scopesheet/eit/%/delete)
 */
function ncn_admin_ajax_ss_eit_delete($claim_id) {
	$room_name = $_POST['room_name'];
	$index 	= $_POST['index'];
	
	$json = array();
	$json['code'] = 'success';
	$json['msg'] = "Successfully Deleted Data.";
	
	$action_type = 'delete';
	if (!ncn_admin_ss_eit_submit($claim_id, $room_name, $action_type, $index)) {
		$json['code'] = 'error-101';
		$json['msg'] = "Failed to delete data.";
	}
	
	echo json_encode($json);
	exit;
}

function ncn_admin_ss_eit_submit($claim_id, $room_name, $action, $index=0, $data=array()) {
	$items = ncn_admin_get_ss_eit_data($claim_id, $room_name);
	// Add & Update
	if ($action == 'new') {
		$items[] = $data;
	} else if ($action == 'edit') {
		$items[$index] = $data;
	} else if ($action == 'delete') {
		unset($items[$index]);
	}
	// Re-Sorting
	usort($items, 'ncn_admin_aci_dcl_log_list_sort_by_date_cmp');
	// Save
	if ( !set_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_eit', serialize($items)) ) {
		return FALSE;
	}
	
	return TRUE;
}

function ncn_admin_get_ss_eit_data($claim_id, $room_name) {
	$data = get_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_eit');
	if ($data) {
		$data = unserialize($data);
    if (is_array($data)) {
      $data = array_slice($data, 0);
    }
	} else {
		$data = array();
	}
	
	// Populate
	return $data;
}





/**
 * ScopeSheet Render - "Structural Moisture Readings" Section
 */ 
function ncn_admin_ss_render_structural_moisture_readings($claim_id, $room_name, $section) {
	$data = array();
	if (isset($_POST['ss_smr'])) {
		$data = $_POST['ss_smr'];
	} else {
		$data = ncn_admin_get_ss_structural_moisture_readings($claim_id, $room_name);
	}
	
	ob_start();
?>
<table>
	<tr>
		<td class="td-label"><label for="ss_smr_walldry_standard">Wall Dry Standard:</label></td>
		<td>
			<div class="field-item text-field-item"><input type="text" name="ss_smr[walldry]" id="ss_smr_walldry_standa	rd" value="<?php echo isset($data['walldry'])?$data['walldry']:''; ?>" /> </div>
		</td>
	</tr>
	<tr>
		<td class="td-label"><label for="ss_smr_floordry_standard">Floor Dry Standard:</label></td>
		<td>
			<div class="field-item text-field-item"><input type="text" name="ss_smr[floordry]" id="ss_smr_floordry_standard" value="<?php echo isset($data['floordry'])?$data['floordry']:''; ?>" /> </div>
		</td>
	</tr>
	<tr>
		<td class="td-label"><label for="ss_smr_ceilingdry_standard">Ceiling Dry Standard:</label></td>
		<td>
			<div class="field-item text-field-item"><input type="text" name="ss_smr[ceilingdry]" id="ss_smr_ceilingdry_standard" value="<?php echo isset($data['ceilingdry'])?$data['ceilingdry']:''; ?>" /> </div>
		</td>
	</tr>
</table>
<div id="ss_smr_list">
	<?php print ncn_admin_ss_render_smr_list($claim_id, $room_name); ?>
</div>
<div id="ss_smr_action_panel_content" class="ss-smr-action-panel">
	<?php print ncn_admin_ss_render_smr_action_panel($claim_id, $room_name, 'new'); ?>
</div>

<?php	
	$output = ob_get_contents();
	ob_end_clean();
	
	return $output;
}

function ncn_admin_get_ss_structural_moisture_readings($claim_id, $room_name) {
	$data = get_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_structural_moisture_readings');
	if ($data) {
		$data = unserialize($data);
	} else {
		$data = array();
	}
	
	// Populate
	return $data;
}


function ncn_admin_ss_render_smr_list($claim_id, $room_name) {
	$data = array();
	$data = ncn_admin_get_ss_smr_list($claim_id, $room_name);
	
	ob_start();
?>
<table class="ss-list-table">
<thead>
	<th class='ss-smr-date'>Date</th>
	<th class='ss-smr-wall'>Wall 1</th>
	<th class='ss-smr-wall'>Wall 2</th>
	<th class='ss-smr-wall'>Wall 3</th>
	<th class='ss-smr-wall'>Wall 4</th>
	<th class='ss-smr-fd-area'>Area 1</th>
	<th class='ss-smr-fd-area'>Area 2</th>
	<th class='ss-smr-fd-area'>Area 3</th>
	<th class='ss-smr-fd-area'>Area 4</th>
	<th class='ss-smr-fd-center'>Center</th>
	<th class='ss-smr-fd-area'>Area 1</th>
	<th class='ss-smr-fd-area'>Area 2</th>
	<th class='ss-smr-fd-area'>Area 3</th>
	<th class='ss-smr-fd-area'>Area 4</th>
	<th class='ss-smr-fd-center'>Center</th>
	<th class='ss-smr-operation'></th>
</thead>
<tbody>
<?php foreach ($data as $key=>$item): ?>
	<tr>
		<td class='ss-smr-date'><?php echo $item['date']; ?></td>
		<td class='ss-smr-wall'><?php echo $item['wall'][0]; ?></td>
		<td class='ss-smr-wall'><?php echo $item['wall'][1]; ?></td>
		<td class='ss-smr-wall'><?php echo $item['wall'][2]; ?></td>
		<td class='ss-smr-wall'><?php echo $item['wall'][3]; ?></td>
		<td class='ss-smr-fd-area'><?php echo $item['area_fd'][0]; ?></td>
		<td class='ss-smr-fd-area'><?php echo $item['area_fd'][1]; ?></td>
		<td class='ss-smr-fd-area'><?php echo $item['area_fd'][2]; ?></td>
		<td class='ss-smr-fd-area'><?php echo $item['area_fd'][3]; ?></td>
		<td class='ss-smr-fd-center'><?php echo $item['area_fd']['center']; ?></td>
		<td class='ss-smr-cd-area'><?php echo $item['area_cd'][0]; ?></td>
		<td class='ss-smr-cd-area'><?php echo $item['area_cd'][1]; ?></td>
		<td class='ss-smr-cd-area'><?php echo $item['area_cd'][2]; ?></td>
		<td class='ss-smr-cd-area'><?php echo $item['area_cd'][3]; ?></td>
		<td class='ss-smr-cd-center'><?php echo $item['area_cd']['center']; ?></td>
		<td class='ss-smr-operation'>
			<a href="" onclick="render_ss_smr_action_panel(<?php echo $claim_id.", '$room_name'".", 'edit', $key"; ?>); return false;">Edit</a>
			<a href="" onclick="on_click_ss_smr_delete(<?php echo $claim_id.", '$room_name', $key"; ?>); return false;">Delete</a>
		</td>
	</tr>
<?php endforeach; ?>
</tbody>
</table>
<?php
	$output = ob_get_contents();
	ob_end_clean();
	
	return $output;
}

/**
 * ScopeSheet - Get Structural Moisture Readings List
 */
function ncn_admin_get_ss_smr_list($claim_id, $room_name) {
	$data = get_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_smr');
	if ($data) {
		$data = unserialize($data);
    if (is_array($data)) {
      $data = array_slice($data, 0);
    }
	} else {
		$data = array();
	}
	// Populate
	
	return $data;
}
function ncn_admin_ss_render_smr_action_panel($claim_id, $room_name, $action, $index=0) {
    global $base_url;
	$smr = array();
	if ($action == 'edit') {
		$data = ncn_admin_get_ss_smr_list($claim_id, $room_name);
		if (isset($data[$index])) { $smr = $data[$index]; }
	} else if ($action == 'new') {
		$smr['date'] = date("m/d/Y"); 
	}
	ob_start();
?>
<table id="ss_smr_action_panel">
	<tr>
		<td class="td-label"><label for="ss_smr_date">Date (MM/DD/YY):</label></td>
		<td><div class="field-item text-field-item small-size"><input type="text" name="smr[date]" id="ss_smr_date" value="<?php echo $smr['date']; ?>" onkeypress="return isDateKey(event);" /></div></td>
	</tr>
	<tr>
		<td class="td-label"><label for="ss_smr_wall">Wall Areas:</label></td>
		<td>
			<div class="field-item text-field-item small-size">
				<input type="text" name="smr[wall][0]" id="ss_smr_wall_0" class="slash-field" title="Wall 1" value="<?php echo isset($smr['wall'])?$smr['wall'][0]:''; ?>" />
				<input type="text" name="smr[wall][1]" id="ss_smr_wall_1" class="slash-field" title="Wall 2" value="<?php echo isset($smr['wall'])?$smr['wall'][1]:''; ?>" />
				<input type="text" name="smr[wall][2]" id="ss_smr_wall_2" class="slash-field" title="Wall 3" value="<?php echo isset($smr['wall'])?$smr['wall'][2]:''; ?>" />
				<input type="text" name="smr[wall][3]" id="ss_smr_wall_3" class="slash-field" title="Wall 4" value="<?php echo isset($smr['wall'])?$smr['wall'][3]:''; ?>" />
			</div>
		</td>
	</tr>
	<tr>
		<td class="td-label"><label for="ss_smr_area_fd">Floor Areas:</label></td>
		<td>
			<div class="field-item text-field-item small-size">
				<input type="text" name="smr[area_fd][0]" id="ss_smr_area_fd_0" class="slash-field" title="Area 1" value="<?php echo isset($smr['area_fd'])?$smr['area_fd'][0]:''; ?>" />
				<input type="text" name="smr[area_fd][1]" id="ss_smr_area_fd_1" class="slash-field" title="Area 2" value="<?php echo isset($smr['area_fd'])?$smr['area_fd'][1]:''; ?>" />
				<input type="text" name="smr[area_fd][2]" id="ss_smr_area_fd_2" class="slash-field" title="Area 3" value="<?php echo isset($smr['area_fd'])?$smr['area_fd'][2]:''; ?>" />
				<input type="text" name="smr[area_fd][3]" id="ss_smr_area_fd_3" class="slash-field" title="Area 4" value="<?php echo isset($smr['area_fd'])?$smr['area_fd'][3]:''; ?>" />
				<input type="text" name="smr[area_fd][center]" id="ss_smr_area_fd_center" class="slash-field" title="Center" value="<?php echo isset($smr['area_fd'])?$smr['area_fd']['center']:''; ?>" />
			</div>
		</td>
	</tr>
	<tr>
		<td class="td-label"><label for="ss_smr_area_cd">Ceiling Areas:</label></td>
		<td>
			<div class="field-item text-field-item small-size">
				<input type="text" name="smr[area_cd][0]" id="ss_smr_area_cd_0" class="slash-field" title="Area 1" value="<?php echo isset($smr['area_cd'])?$smr['area_cd'][0]:''; ?>" />
				<input type="text" name="smr[area_cd][1]" id="ss_smr_area_cd_1" class="slash-field" title="Area 2" value="<?php echo isset($smr['area_cd'])?$smr['area_cd'][1]:''; ?>" />
				<input type="text" name="smr[area_cd][2]" id="ss_smr_area_cd_2" class="slash-field" title="Area 3" value="<?php echo isset($smr['area_cd'])?$smr['area_cd'][2]:''; ?>" />
				<input type="text" name="smr[area_cd][3]" id="ss_smr_area_cd_3" class="slash-field" title="Area 4" value="<?php echo isset($smr['area_cd'])?$smr['area_cd'][3]:''; ?>" />
				<input type="text" name="smr[area_cd][center]" id="ss_smr_area_cd_center" class="slash-field" title="Center" value="<?php echo isset($smr['area_cd'])?$smr['area_cd']['center']:''; ?>" />
			</div>
		</td>
	</tr>
	<tr>
		<td></td>
		<td>
			<div class='ss-smr-actions ss-actions'>
				<input type="hidden" id="ss_smr_action_type" name="ss_smr_action_type" value="<?php echo $action; ?>" />
				<input type="hidden" id="ss_smr_index" name="ss_smr_index" value="<?php echo $index; ?>" />
				<?php if ($action == 'new'): ?>
					<input type='button' value='  Add  ' onclick="on_click_ss_smr_action(<?php echo $claim_id; ?>, '<?php echo $room_name; ?>')"/>
				<?php else: ?>
					<input type='button' value='  OK  ' onclick="on_click_ss_smr_action(<?php echo $claim_id; ?>, '<?php echo $room_name; ?>')" />&nbsp;&nbsp;&nbsp;
					<input type='button' value='  Cancel  ' onclick="render_ss_smr_action_panel(<?php echo $claim_id; ?>, '<?php echo $room_name; ?>', 'new', 0)" />
				<?php endif; ?>
			</div>
		</td>
	</tr>
</table>
<script>
jQuery(function() {
	ncn_admin_ss_smr_datepicker();
	bindSlashEvent('#ss_smr_action_panel');
});
function ncn_admin_ss_smr_datepicker() {
	jQuery( '#ss_smr_date' ).datepicker({
		showOn: "button",
		buttonImage: "<?php echo $base_url."/".drupal_get_path('module', 'ncn_report')."/images/calendar.gif"; ?>",
		buttonImageOnly: true,
		dateFormat: 'mm/dd/yy',
	});
}

</script>
<?php 
	$output = ob_get_contents();
	ob_end_clean();
	return $output;
}

/**
 * Menu Handler (ajax/ncn_admin/scopesheet/smr/%/render_smr_list)
 */
function ncn_admin_ajax_ss_smr_render_list($claim_id) {
	$room_name = $_POST['room_name'];
	
	$json = array();
	$json['code'] = 'success';
	$json['html']	= ncn_admin_ss_render_smr_list($claim_id, $room_name);
	
	echo json_encode($json);
	exit;
}

/**
 * Menu Handler (ajax/ncn_admin/scopesheet/smr/%/render_ss_smr_action_panel)
 */
function ncn_admin_ajax_ss_smr_render_action_panel($claim_id) {
	$room_name = $_POST['room_name'];
	
	$action = $_POST['action'];
	$index = $_POST['index'];
	
	$json = array();
	$json['code'] = 'success';
	$json['html']	= ncn_admin_ss_render_smr_action_panel($claim_id, $room_name, $action, $index);
	
	echo json_encode($json);
	exit;
}

/**
 * Menu Handler (ajax/ncn_admin/scopesheet/smr/%/action_submit)
 */
function ncn_admin_ajax_ss_smr_action_submit($claim_id) {
	$room_name = $_POST['room_name'];
	
	$action_type		= $_POST['action_type'];
	$ss_smr_index 	= $_POST['ss_smr_index'];
	
	$ss_smr_date		= $_POST['ss_smr_date'];
	$ss_smr_wall[0]		= $_POST['ss_smr_wall_0'];
	$ss_smr_wall[1]		= $_POST['ss_smr_wall_1'];
	$ss_smr_wall[2]		= $_POST['ss_smr_wall_2'];
	$ss_smr_wall[3]		= $_POST['ss_smr_wall_3'];
	
	$ss_smr_area_fd[0]	= $_POST['ss_smr_area_fd_0'];
	$ss_smr_area_fd[1]	= $_POST['ss_smr_area_fd_1'];
	$ss_smr_area_fd[2]	= $_POST['ss_smr_area_fd_2'];
	$ss_smr_area_fd[3]	= $_POST['ss_smr_area_fd_3'];
	$ss_smr_area_fd['center'] = $_POST['ss_smr_area_fd_center'];
	
	$ss_smr_area_cd[0]	= $_POST['ss_smr_area_cd_0'];
	$ss_smr_area_cd[1]	= $_POST['ss_smr_area_cd_1'];
	$ss_smr_area_cd[2]	= $_POST['ss_smr_area_cd_2'];
	$ss_smr_area_cd[3]	= $_POST['ss_smr_area_cd_3'];
	$ss_smr_area_cd['center'] = $_POST['ss_smr_area_cd_center'];
	
	
	$ss_smr_data = array(
		'date' => $ss_smr_date,
		'wall' => $ss_smr_wall,
		'area_fd' => $ss_smr_area_fd,
		'area_cd' => $ss_smr_area_cd,
	);
	
	$json = array();
	$json['code'] = 'success';
	if ($action_type == 'new') {
		$json['msg'] = "Successfully Added Data.";
	} else {	// edit
		$json['msg'] = "Successfully Updated Data.";
	}
	
	if (!ncn_admin_ss_smr_submit($claim_id, $room_name, $action_type, $ss_smr_index, $ss_smr_data)) {
		$json['code'] = 'error-101';
		$json['msg'] = "Failed to save data.";
	}
	
	echo json_encode($json);
	exit;
}

/**
 * Menu Handler (ajax/ncn_admin/scopesheet/smr/%/delete)
 */
function ncn_admin_ajax_ss_smr_delete($claim_id) {
	$room_name = $_POST['room_name'];
	$index 	= $_POST['index'];
	
	$json = array();
	$json['code'] = 'success';
	$json['msg'] = "Successfully Deleted Data.";
	
	$action_type = 'delete';
	if (!ncn_admin_ss_smr_submit($claim_id, $room_name, $action_type, $index)) {
		$json['code'] = 'error-101';
		$json['msg'] = "Failed to delete data.";
	}
	
	echo json_encode($json);
	exit;
}

function ncn_admin_ss_smr_submit($claim_id, $room_name, $action, $index=0, $data=array()) {
	$items = ncn_admin_get_ss_smr_data($claim_id, $room_name);
	// Add & Update
	if ($action == 'new') {
		$items[] = $data;
	} else if ($action == 'edit') {
		$items[$index] = $data;
	} else if ($action == 'delete') {
		unset($items[$index]);
	}
	// Re-Sorting
	usort($items, 'ncn_admin_aci_dcl_log_list_sort_by_date_cmp');
	// Save
	if ( !set_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_smr', serialize($items)) ) {
		return FALSE;
	}
	
	return TRUE;
}

function ncn_admin_get_ss_smr_data($claim_id, $room_name) {
	$data = get_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_smr');
	if ($data) {
		$data = unserialize($data);
	} else {
		$data = array();
	}
	
	// Populate
	return $data;
}





/**
 * ScopeSheet Render - "Equipment Placement " Section
 */ 
function ncn_admin_ss_render_equipment_placement($claim_id, $room_name, $section) {
	$data = array();
	if (isset($_POST['ss_ep'])) {
		$data = $_POST['ss_ep'];
	} else {
		$data = ncn_admin_get_ss_equipment_placement($claim_id, $room_name);
	}
	
	ob_start();
?>
<div id="ss_ep_list">
	<?php print ncn_admin_ss_render_ep_list($claim_id, $room_name); ?>
</div>
<div id="ss_ep_action_panel_type_content">
	<?php print ncn_admin_ss_render_ep_action_panel_type($claim_id, $room_name); ?>
</div>
<div id="ss_ep_action_panel_content" class="ss-ep-action-panel">
	<?php print ncn_admin_ss_render_ep_action_panel($claim_id, $room_name, '', 'new'); ?>
</div>
<input type="hidden" name="ss_ep_claim_id" id="ss_ep_claim_id" value="<?php echo $claim_id; ?>" />
<input type="hidden" name="ss_ep_room_name" id="ss_ep_room_name" value="<?php echo $room_name; ?>" />
<?php	
	$output = ob_get_contents();
	ob_end_clean();
	
	return $output;
}

function ncn_admin_get_ss_equipment_placement($claim_id, $room_name) {
	$data = get_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_equipment_placement');
	if ($data) {
		$data = unserialize($data);
	} else {
		$data = array();
	}
	
	// Populate
	return $data;
}

/**
 * ScopeSheet - Get Equipment Placement List
 */
function ncn_admin_ss_render_ep_list($claim_id, $room_name) {
	$data = array();
	$data = ncn_admin_get_ss_ep_list($claim_id, $room_name);
	
	$action_panel_types = ncn_admin_ss_get_action_panel_type();
	ob_start();
?>
<table class="ss-list-table">
	<thead>
		<th class="ss-ep-panel-type-td"></th>
		<th class="ss-ep-data-td"></th>
		<th class="ss-ep-operation"></th>
	</thead>
<tbody>
<?php foreach ($data as $key=>$item): ?>
	<tr>
		<td class="ss-ep-panel-type-td"><?php echo $action_panel_types[$item['ptype']].": " ?></td>
		<td class="ss-ep-data-td"><?php print ncn_admin_render_ss_ep_format_data($item); ?></td>
		<td class="ss-ep-operation">
			<a href="" onclick="render_ss_ep_action_panel(<?php echo $claim_id.", '$room_name'".", 'edit', $key"; ?>); return false;">Edit</a>
			<a href="" onclick="on_click_ss_ep_delete(<?php echo $claim_id.", '$room_name', $key"; ?>); return false;">Delete</a>
		</td>
	</tr>
<?php endforeach; ?>
</tbody>
</table>
<?php
	$output = ob_get_contents();
	ob_end_clean();
	
	return $output;
}

/**
 * ScopeSheet - Get Equipment Placement List
 */
function ncn_admin_get_ss_ep_list($claim_id, $room_name) {
	$data = get_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_ep');
	if ($data) {
		$data = unserialize($data);
    if (is_array($data)) {
      $data = array_slice($data, 0);
    }
	} else {
		$data = array();
	}
	// Populate
	
	return $data;
}

function ncn_admin_ss_get_action_panel_type() {
	$types = array(
		'Dehumidifier'=>'Dehumidifier', 
		'Wall Drying' =>'Wall Drying', 
		'Airmover'		=>'Airmover', 
		'Air Scrubber'=>'Air Scrubber', 
		'E-Tes Tex Unit'=>'E-Tes Tex Unit', 
		'E-Tes Trailer'=>'E-Tes Trailer', 
		'Other'				=> 'Other', 
	);
	
	return $types;
}

function ncn_admin_ss_render_ep_action_panel_type($claim_id, $room_name) {
	$types = ncn_admin_ss_get_action_panel_type();
	
	$output = "<select id='ss_ep_action_panel_type' name='ep_action_panel_type'>";
	$output.= "<option value=''>----- Please Select -----</option>";
	foreach ($types as $key=>$type) {
		$output.= "<option value='$key'>$type</option>";
	}
	$output.= "</select>";
	
	drupal_add_js("
		jQuery(document).ready(function(){
			jQuery('#ss_ep_action_panel_type').bind('change', onchange_ss_ep_action_panel_type);
		});
	", 'inline'
	)	;
	
	return $output;
}

function ncn_admin_ss_render_ep_action_panel($claim_id, $room_name, $panel_type, $action, $index=0) {
	$ep = array();
	if ($action == 'edit') {
		$data = ncn_admin_get_ss_ep_list($claim_id, $room_name);
		if (isset($data[$index])) { 
			$ep = $data[$index]; 
			$panel_type = $ep['ptype'];
		}
	} else if ($action == 'new') {
		
	}
	
	ob_start();
	print ncn_admin_ss_render_ep_action_panel_by_type($panel_type, $ep);
?>
	<div class='ss-ep-actions ss-actions'>
		<input type="hidden" id="ss_ep_action_type" name="ss_ep_action_type" value="<?php echo $action; ?>" />
		<input type="hidden" id="ss_ep_index" name="ss_ep_index" value="<?php echo $index; ?>" />
		<?php if ($action == 'new'): ?>
			<input type='button' value='  Add  ' 		class="ss-ep-button" style="<?php if (!$panel_type) { echo "display: none;";} ?>" onclick="on_click_ss_ep_action(<?php echo $claim_id; ?>, '<?php echo $room_name; ?>')"/>
		<?php else: ?>
			<input type='button' value='  OK  '  		class="ss-ep-button" onclick="on_click_ss_ep_action(<?php echo $claim_id; ?>, '<?php echo $room_name; ?>')" />&nbsp;&nbsp;&nbsp;
			<input type='button' value='  Cancel  ' class="ss-ep-button" onclick="render_ss_ep_action_panel(<?php echo $claim_id; ?>, '<?php echo $room_name; ?>', 'new', 0)" />
		<?php endif; ?>
	</div>	
<?php
	$output = ob_get_contents();
	ob_end_clean();
	return $output;
}

function ncn_admin_ss_render_ep_action_panel_by_type($panel_type, $data) {
	ob_start();
?>
	<span class='ser-field-item'><input type='hidden' name='ss_ep[ptype]' value='<?php echo $panel_type; ?>' /></span>
	<?php if ($panel_type == 'Dehumidifier'): ?>
	<div id="ss_ep_dehumidifier" class="ss-ep-action-panel">
		<label for="ss_ep_dehumidifier_model_size">Dehumidifier:</label>
		<span class="field-item text-field-item small-size ser-field-item"><input type="text" name="ss_ep[data][model]" id="ss_ep_dehum_model_size_model" class="required" value="<?php echo isset($data['data']['model'])?$data['data']['model']:''; ?>" /> </span>
		<span class="field-item text-field-item small-size ser-field-item"> (<input type="text" name="ss_ep[data][ea]" id="ss_ep_dehum_model_size_ea" class="required" value="<?php echo isset($data['data']['ea'])?$data['data']['ea']:''; ?>" />#) </span>X
		<span class="field-item text-field-item small-size ser-field-item"> (<input type="text" name="ss_ep[data][days]" id="ss_ep_dehum_model_size_days" class="required" value="<?php echo isset($data['data']['days'])?$data['data']['days']:''; ?>" />Days) </span>
	</div>
	<?php elseif($panel_type == 'Wall Drying'): ?>
	<div id="ss_ep_wall_drying" class="ss-ep-action-panel">
		<label for="ss_ep_wall_drying">Wall Drying:</label>
		<ul class="field-check-lists inline-group-fields clearfix">
			<li class="inline-field-item ser-field-item"><input type="radio" id="ss_ep_wall_drying_injected" name="ss_ep[data][type]" value="Injected" <?php if(isset($data['data']['type'])&&$data['data']['type']=='Injected') { echo "checked='checked'";} ?> ><label for="ss_ep_wall_drying_injected">Injected</label></li>
			<li class="inline-field-item ser-field-item"><input type="radio" id="ss_ep_wall_drying_ducted" name="ss_ep[data][type]" value="Ducted" <?php if(isset($data['data']['type'])&&$data['data']['type']=='Ducted') { echo "checked='checked'";} ?> ><label for="ss_ep_wall_drying_ducted">Ducted</label></li>
		</ul>
		<span class="field-item text-field-item small-size ser-field-item"> (<input type="text" name="ss_ep[data][ea]" id="ss_ep_wall_drying_ea" class="required" value="<?php echo isset($data['data']['ea'])?$data['data']['ea']:''; ?>" />#) </span>X
		<span class="field-item text-field-item small-size ser-field-item"> (<input type="text" name="ss_ep[data][days]" id="ss_ep_wall_drying_days" class="required" value="<?php echo isset($data['data']['days'])?$data['data']['days']:''; ?>" />Days) </span>
	</div>
	<?php elseif($panel_type == 'Airmover'): ?>
	<div id="ss_ep_airmover" class="ss-ep-action-panel">
		<label for="ss_ep_airmover">Airmover:</label>
		<ul class="field-check-lists inline-group-fields clearfix">
			<li class="inline-field-item ser-field-item"><input type="radio" id="ss_ep_airmover_standard" name="ss_ep[data][type]" value="Standard" <?php if(isset($data['data']['type'])&&$data['data']['type']=='Standard') { echo "checked='checked'";} ?> ><label for="ss_ep_airmover_standard">Standard</label></li>
			<li class="inline-field-item ser-field-item"><input type="radio" id="ss_ep_airmover_axial" name="ss_ep[data][type]" value="Axial" <?php if(isset($data['data']['type'])&&$data['data']['type']=='Axial') { echo "checked='checked'";} ?> ><label for="ss_ep_airmover_axial">Axial</label></li>
			<li class="inline-field-item ser-field-item"><input type="radio" id="ss_ep_airmover_axial_1hp" name="ss_ep[data][type]" value="Axial 1HP" <?php if(isset($data['data']['type'])&&$data['data']['type']=='Axial 1HP') { echo "checked='checked'";} ?> ><label for="ss_ep_airmover_axial_1hp">Axial 1HP</label></li>
		</ul>
		<span class="field-item text-field-item small-size ser-field-item"> (<input type="text" name="ss_ep[data][ea]" id="ss_ep_airmover_ea" class="required" value="<?php echo isset($data['data']['ea'])?$data['data']['ea']:''; ?>" />#) </span>X
		<span class="field-item text-field-item small-size ser-field-item"> (<input type="text" name="ss_ep[data][days]" id="ss_ep_airmover_days" class="required" value="<?php echo isset($data['data']['days'])?$data['data']['days']:''; ?>" />Days) </span>
	</div>
	<?php elseif($panel_type == 'Air Scrubber'): ?>
	<div id="ss_ep_air_scrubber" class="ss-ep-action-panel">
		<label for="ss_ep_air_scrubber">Air Scrubber:</label>
		<ul class="field-check-lists inline-group-fields clearfix">
			<li class="inline-field-item ser-field-item"><input type="radio" id="ss_ep_air_scrubber_small" name="ss_ep[data][type]" value="Small" <?php if(isset($data['data']['type'])&&$data['data']['type']=='Small') { echo "checked='checked'";} ?> ><label for="ss_ep_air_scrubber_small">Small</label></li>
			<li class="inline-field-item ser-field-item"><input type="radio" id="ss_ep_air_scrubber_large" name="ss_ep[data][type]" value="Large" <?php if(isset($data['data']['type'])&&$data['data']['type']=='Large') { echo "checked='checked'";} ?> ><label for="ss_ep_air_scrubber_large">Large</label></li>
		</ul>
		<span class="field-item text-field-item small-size ser-field-item"> (<input type="text" name="ss_ep[data][ea]" id="ss_ep_air_scrubber_ea" class="required" value="<?php echo isset($data['data']['ea'])?$data['data']['ea']:''; ?>" />#) </span>X
		<span class="field-item text-field-item small-size ser-field-item"> (<input type="text" name="ss_ep[data][days]" id="ss_ep_air_scrubber_days" class="required" value="<?php echo isset($data['data']['days'])?$data['data']['days']:''; ?>" />Days) </span> &nbsp;&nbsp;
		<label for="ss_ep_replace_internal_filter">Replace Internal Filter:</label>
		<ul class="field-check-lists inline-group-fields clearfix">
			<li class="inline-field-item ser-field-item"><input type="radio" id="ss_ep_rif_yes" name="ss_ep[data][rif_type]" value="Yes" <?php if(isset($data['data']['rif_type'])&&$data['data']['rif_type']=='Yes') { echo "checked='checked'";} ?> ><label for="ss_ep_rif_yes">Yes</label></li>
			<li class="inline-field-item ser-field-item"><input type="radio" id="ss_ep_rif_no" name="ss_ep[data][rif_type]" value="No" <?php if(isset($data['data']['rif_type'])&&$data['data']['rif_type']=='No') { echo "checked='checked'";} ?> ><label for="ss_ep_rif_no">No</label></li>
		</ul>
	</div>
	<?php elseif($panel_type == 'E-Tes Tex Unit'): ?>
	<div id="ss_ep_etes_text_unit" class="ss-ep-action-panel">
		<label for="ss_ep_e_tes_tex_unit">E-TES Tex Unit:</label>
		<!-- <ul class="field-check-lists inline-group-fields clearfix">
			<li class="inline-field-item ser-field-item"><input type="radio" id="ss_ep_ettu_50_btu" name="ss_ep[data][unit]" value="50 Btu" <?php if(isset($data['data']['unit'])&&$data['data']['unit']=='50 Btu') { echo "checked='checked'";} ?> ><label for="ss_ep_ettu_50_btu">50 Btu</label></li>
			<li class="inline-field-item ser-field-item"><input type="radio" id="ss_ep_ettu_80_btu" name="ss_ep[data][unit]" value="80 Btu" <?php if(isset($data['data']['unit'])&&$data['data']['unit']=='80 Btu') { echo "checked='checked'";} ?> ><label for="ss_ep_ettu_80_btu">80 Btu</label></li>
		</ul> -->
		<span class="field-item text-field-item small-size ser-field-item"> (<input type="text" name="ss_ep[data][ea]" id="ss_ep_ettu_ea" class="required" value="<?php echo isset($data['data']['ea'])?$data['data']['ea']:''; ?>" />#) </span>X
		<span class="field-item text-field-item small-size ser-field-item"> (<input type="text" name="ss_ep[data][days]" id="ss_ep_ettu_days" class="required" value="<?php echo isset($data['data']['days'])?$data['data']['days']:''; ?>" />Days) </span>
	</div>
	<?php elseif($panel_type == 'E-Tes Trailer'): ?>
	<div id="ss_ep_etes_trailer" class="ss-ep-action-panel">
		<label for="ss_ep_e_tes_trailer">E-TES Trailer:</label>
		<ul class="field-check-lists inline-group-fields clearfix">
			<li class="inline-field-item ser-field-item"><input type="radio" id="ss_ep_ett_200_btu" name="ss_ep[data][unit]" value="200 Btu" <?php if(isset($data['data']['unit'])&&$data['data']['unit']=='200 Btu') { echo "checked='checked'";} ?> ><label for="ss_ep_ett_200_btu">200 Btu</label></li>
			<li class="inline-field-item ser-field-item"><input type="radio" id="ss_ep_ett_250_btu" name="ss_ep[data][unit]" value="250 Btu" <?php if(isset($data['data']['unit'])&&$data['data']['unit']=='250 Btu') { echo "checked='checked'";} ?> ><label for="ss_ep_ett_250_btu">250 Btu</label></li>
			<li class="inline-field-item ser-field-item"><input type="radio" id="ss_ep_ett_300_btu" name="ss_ep[data][unit]" value="300 Btu" <?php if(isset($data['data']['unit'])&&$data['data']['unit']=='300 Btu') { echo "checked='checked'";} ?> ><label for="ss_ep_ett_300_btu">300 Btu</label></li>
		</ul>
		<span class="field-item text-field-item small-size ser-field-item"> (<input type="text" name="ss_ep[data][ea]" id="ss_ep_ett_ea" class="required" value="<?php echo isset($data['data']['ea'])?$data['data']['ea']:''; ?>" />#) </span>X
		<span class="field-item text-field-item small-size ser-field-item"> (<input type="text" name="ss_ep[data][days]" id="ss_ep_ett_days" class="required" value="<?php echo isset($data['data']['days'])?$data['data']['days']:''; ?>" />Days) </span>
	</div>
	<?php elseif($panel_type == 'Other'): ?>
	<div id="ss_ep_other" class="ss-ep-action-panel">
		<label for="ss_ep_other">Other:</label>
		<span class="field-item text-field-item small-size ser-field-item"><input type="text" name="ss_ep[data][name]" id="ss_ep_other_name" class="required" value="<?php echo isset($data['data']['name'])?$data['data']['name']:''; ?>" /> </span>
		<span class="field-item text-field-item small-size ser-field-item"> (<input type="text" name="ss_ep[data][ea]" id="ss_ep_other_ea" class="required" value="<?php echo isset($data['data']['ea'])?$data['data']['ea']:''; ?>" />#) </span>X
		<span class="field-item text-field-item small-size ser-field-item"> (<input type="text" name="ss_ep[data][days]" id="ss_ep_other_days" class="required" value="<?php echo isset($data['data']['days'])?$data['data']['days']:''; ?>" />Days) </span>
		&nbsp;&nbsp;&nbsp;<label for="ss_ep_other_comment">Comment:</label>
		<span class="field-item text-field-item small-size ser-field-item"> <input type="text" name="ss_ep[data][comment]" id="ss_ep_other_comment" class="required" value="<?php echo isset($data['data']['comment'])?$data['data']['comment']:''; ?>" /></span>
	</div>
	<?php endif; ?>
<?php
	$content = ob_get_contents();
	ob_end_clean();
	return $content;
}

function ncn_admin_render_ss_ep_format_data($item, $print_invoice=false) {
	$ptype = $item['ptype'];
	$data = $item['data'];
	$output = '';
	switch ($ptype) {
	case 'Dehumidifier':
		$output.= $data['model']."&nbsp;&nbsp;&nbsp;". $data['ea'] ." x ".$data['days']." Days";
		break;
	case 'Wall Drying':
		$output.= $data['type']."&nbsp;&nbsp;&nbsp;". $data['ea'] ." x ".$data['days']." Days";
		break;
	case 'Airmover';
		$output.= $data['type']."&nbsp;&nbsp;&nbsp;". $data['ea'] ." x ".$data['days']." Days";
		break;
	case 'Air Scrubber';
		$output.= $data['type']."&nbsp;&nbsp;&nbsp;". $data['ea'] ." x ".$data['days']." Days";
		$output.= "&nbsp;&nbsp;Replace Internal Filter: ".$data['rif_type'];
		break;
	case 'E-Tes Tex Unit';
		$output.= /*$data['unit']."&nbsp;&nbsp;&nbsp;".*/ $data['ea'] ." x ".$data['days']." Days";
		break;
	case 'E-Tes Trailer';
		$output.= $data['unit']."&nbsp;&nbsp;&nbsp;". $data['ea'] ." x ".$data['days']." Days";
		break;
	case 'Other';
    if ($print_invoice) {
		  $output.= $data['name']."&nbsp;&nbsp;&nbsp;". $data['ea'] ." x ".$data['days']." Days";
    } else {
      $output.= $data['name']."&nbsp;&nbsp;&nbsp;". $data['ea'] ." x ".$data['days']." Days"."&nbsp;&nbsp;&nbsp;". $data['comment'];
    }
		break;
	}
	
	return $output;
}

/**
 * Menu Handler (ajax/ncn_admin/scopesheet/ep/%/get_action_panel)
 */ 
function ncn_admin_ajax_ss_ep_get_action_panel($claim_id) {
	$type = $_POST['type'];
	$room_name = $_POST['room_name'];
	$json = array();
	$json['code'] = 'success';
	$json['html']	= ncn_admin_ss_render_ep_action_panel($claim_id, $room_name, $type, 'new');
	
	echo json_encode($json);
	exit;
}

/**
 * Menu Handler (ajax/ncn_admin/scopesheet/ep/%/render_ep_list)
 */
function ncn_admin_ajax_ss_ep_render_list($claim_id) {
	$room_name = $_POST['room_name'];
	
	$json = array();
	$json['code'] = 'success';
	$json['html']	= ncn_admin_ss_render_ep_list($claim_id, $room_name);
	
	echo json_encode($json);
	exit;
}

/**
 * Menu Handler (ajax/ncn_admin/scopesheet/ep/%/render_ss_ep_action_panel)
 */
function ncn_admin_ajax_ss_ep_render_action_panel($claim_id) {
	$room_name = $_POST['room_name'];
	
	$action = $_POST['action'];
	$index = $_POST['index'];
	
	$json = array();
	$json['code'] = 'success';
	$json['html']	= ncn_admin_ss_render_ep_action_panel($claim_id, $room_name, '', $action, $index);
	
	echo json_encode($json);
	exit;
}

/**
 * Menu Handler (ajax/ncn_admin/scopesheet/ep/%/action_submit)
 */
function ncn_admin_ajax_ss_ep_action_submit($claim_id) {
	$room_name = $_POST['room_name'];
	
	$action_type		= $_POST['action_type'];
	$ss_ep_index 		= $_POST['ss_ep_index'];
	
	$ss_ep_data 		= $_POST['ss_ep'];
	
	$json = array();
	$json['code'] = 'success';
	if ($action_type == 'new') {
		$json['msg'] = "Successfully Added Data.";
	} else {	// edit
		$json['msg'] = "Successfully Updated Data.";
	}
	
	if (!ncn_admin_ss_ep_submit($claim_id, $room_name, $action_type, $ss_ep_index, $ss_ep_data)) {
		$json['code'] = 'error-101';
		$json['msg'] = "Failed to save data.";
	}
	
	echo json_encode($json);
	exit;
}

/**
 * Menu Handler (ajax/ncn_admin/scopesheet/ep/%/delete)
 */
function ncn_admin_ajax_ss_ep_delete($claim_id) {
	$room_name = $_POST['room_name'];
	$index 	= $_POST['index'];
	
	$json = array();
	$json['code'] = 'success';
	$json['msg'] = "Successfully Deleted Data.";
	
	$action_type = 'delete';
	if (!ncn_admin_ss_ep_submit($claim_id, $room_name, $action_type, $index)) {
		$json['code'] = 'error-101';
		$json['msg'] = "Failed to delete data.";
	}
	
	echo json_encode($json);
	exit;
}

function ncn_admin_ss_ep_submit($claim_id, $room_name, $action, $index=0, $data=array()) {
	$items = ncn_admin_get_ss_ep_data($claim_id, $room_name);
	// Add & Update
	if ($action == 'new') {
		$items[] = $data;
	} else if ($action == 'edit') {
		$items[$index] = $data;
	} else if ($action == 'delete') {
		unset($items[$index]);
	}
  
	// Save
	if ( !set_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_ep', serialize($items)) ) {
		return FALSE;
	}
	
	return TRUE;
}

function ncn_admin_get_ss_ep_data($claim_id, $room_name) {
	$data = get_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_ep');
	if ($data) {
		$data = unserialize($data);
    if (is_array($data)) {
      $data = array_slice($data, 0);
    }
	} else {
		$data = array();
	}
	
	// Populate
	return $data;
}




/**
 * ScopeSheet Render - "Services" Section
 */ 
function ncn_admin_ss_render_services($claim_id, $room_name, $section) {
	$data = array();
	if (isset($_POST['ss_services'])) {
		$data = $_POST['ss_services'];
	} else {
		$data = ncn_admin_get_ss_services($claim_id, $room_name);
	}
	
	ob_start();
?>
<script>
  jQuery(document).ready(function() { bind_ncn_admin_ss_services_radio_reset_buttons(); bind_ncn_admin_ss_services_cboxlabel_buttons(); });
</script>
<table id="ss_services_content" border="0">
	<thead>
		<th class="after-hours">After Hours</th>
		<th class="reg-hours">Reg Hours</th>
		<th class="service-list"></th>
        <th class="service-reset">Reset</th>
	</thead>
	<tbody>
		<tr><td colspan="4" class="ss-services-sub-section">Water Extraction</td></tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_we_carpet_time_ah" name="ss_services[we][carpet][time]" value="after" <?php if (isset($data['we']['carpet']['time'])&&$data['we']['carpet']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_we_carpet_tim_rh" name="ss_services[we][carpet][time]" value="reg" <?php if (isset($data['we']['carpet']['time'])&&$data['we']['carpet']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Carpet: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_we_carpet_type_regular" name="ss_services[we][carpet][type]" value="Regular" <?php if(isset($data['we']['carpet']['type'])&&$data['we']['carpet']['type']=='Regular') { echo "checked='checked'";} ?> ><label for="ss_service_we_carpet_type_regular">Regular</label></li>
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_we_carpet_type_heavy" name="ss_services[we][carpet][type]" value="Heavy" <?php if(isset($data['we']['carpet']['type'])&&$data['we']['carpet']['type']=='Heavy') { echo "checked='checked'";} ?> ><label for="ss_service_we_carpet_type_heavy">Heavy</label></li>
				</ul>
				<span class="field-item text-field-item small-size"> (<input type="text" name="ss_services[we][carpet][heavy][sf]" class="required-text" id="ss_service_we_carpet_heavy_sf" value="<?php echo isset($data['we'])?$data['we']['carpet']['heavy']['sf']:''; ?>" />SF) </span>
			</div>
			<div class="hidden-div"><label class="service-name">Carpet under Water Extraction</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_we_hard_floor_time_ah" name="ss_services[we][hard_floor][time]" value="after" <?php if (isset($data['we']['hard_floor']['time'])&&$data['we']['hard_floor']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_we_hard_floor_time_rh" name="ss_services[we][hard_floor][time]" value="reg" <?php if (isset($data['we']['hard_floor']['time'])&&$data['we']['hard_floor']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Hard Floor: </label>
				<span class="field-item text-field-item small-size"> (<input type="text" name="ss_services[we][hard_floor][sf]" class="required-text" id="ss_service_we_hard_floor_sf" value="<?php echo isset($data['we'])?$data['we']['hard_floor']['sf']:''; ?>" />SF) </span>
			</div>
			<div class="hidden-div"><label class="service-name">Hard Floor under Water Extraction</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_we_weight_assisted_time_ah" name="ss_services[we][weight_assisted][time]" value="after" <?php if (isset($data['we']['weight_assisted']['time'])&&$data['we']['weight_assisted']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_we_weight_assisted_time_rh" name="ss_services[we][weight_assisted][time]" value="reg" <?php if (isset($data['we']['weight_assisted']['time'])&&$data['we']['weight_assisted']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Weight-Assisted: </label>
				<span class="field-item text-field-item small-size"> (<input type="text" name="ss_services[we][weight_assisted][sf]" class="required-text" id="ss_service_we_weight_assisted_sf" value="<?php echo isset($data['we'])?$data['we']['weight_assisted']['sf']:''; ?>" />SF) </span>
			</div>
			<div class="hidden-div"><label class="service-name">Weight-Assisted under Water Extraction</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_we_lift_float_carpet_time_ah" name="ss_services[we][lift_float_carpet][time]" value="after" <?php if (isset($data['we']['lift_float_carpet']['time'])&&$data['we']['lift_float_carpet']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_we_lift_float_carpet_time_rh" name="ss_services[we][lift_float_carpet][time]" value="reg" <?php if (isset($data['we']['lift_float_carpet']['time'])&&$data['we']['lift_float_carpet']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"> <div><label>Lift/Float Carpet: </label>
				<span class="field-item text-field-item small-size"> (<input type="text" name="ss_services[we][lift_float_carpet][sf]" class="required-text" id="ss_service_we_lift_float_carpet_sf" value="<?php echo isset($data['we'])?$data['we']['lift_float_carpet']['sf']:''; ?>" />SF) </span>
			</div>
			<div class="hidden-div"><label class="service-name">Lift/Float Carpet under Water Extraction</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		
		<tr><td colspan="4" class="ss-services-sub-section">Material Removal</td></tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_mr_carpet_time_ah" name="ss_services[mr][carpet][time]" value="after" <?php if (isset($data['mr']['carpet']['time'])&&$data['mr']['carpet']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_mr_carpet_time_rh" name="ss_services[mr][carpet][time]" value="reg" <?php if (isset($data['mr']['carpet']['time'])&&$data['mr']['carpet']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Carpet: </label>
				(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[mr][carpet][sf]" class="required-text" id="ss_service_mr_carpet_sf" value="<?php echo isset($data['mr'])?$data['mr']['carpet']['sf']:''; ?>" /> </span> SF)
			</div>
			<div class="hidden-div"><label class="service-name">Carpet under Material Removal</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_mr_glued_carpet_time_ah" name="ss_services[mr][glued_carpet][time]" value="after" <?php if (isset($data['mr']['glued_carpet']['time'])&&$data['mr']['glued_carpet']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_mr_glued_carpet_time_rh" name="ss_services[mr][glued_carpet][time]" value="reg" <?php if (isset($data['mr']['glued_carpet']['time'])&&$data['mr']['glued_carpet']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Glued Carpet: </label>
				(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[mr][glued_carpet][sf]" class="required-text" id="ss_service_mr_glued_carpet_sf" value="<?php echo isset($data['mr'])?$data['mr']['glued_carpet']['sf']:''; ?>" /> </span> SF)
			</div>
			<div class="hidden-div"><label class="service-name">Glued Carpet under Material Removal</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_mr_padding_time_ah" name="ss_services[mr][padding][time]" value="after" <?php if (isset($data['mr']['padding']['time'])&&$data['mr']['padding']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_mr_padding_time_rh" name="ss_services[mr][padding][time]" value="reg" <?php if (isset($data['mr']['padding']['time'])&&$data['mr']['padding']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Padding: </label>
				(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[mr][padding][sf]" class="required-text" id="ss_service_mr_padding_sf" value="<?php echo isset($data['mr'])?$data['mr']['padding']['sf']:''; ?>" /> </span> SF)
			</div>
			<div class="hidden-div"><label class="service-name">Padding under Material Removal</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_mr_vinyl_time_ah" name="ss_services[mr][vinyl][time]" value="after" <?php if (isset($data['mr']['vinyl']['time'])&&$data['mr']['vinyl']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_mr_vinyl_time_rh" name="ss_services[mr][vinyl][time]" value="reg" <?php if (isset($data['mr']['vinyl']['time'])&&$data['mr']['vinyl']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Vinyl: </label>
				(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[mr][vinyl][sf]" class="required-text" id="ss_service_mr_vinyl_sf" value="<?php echo isset($data['mr'])?$data['mr']['vinyl']['sf']:''; ?>" /> </span> SF)
			</div>
			<div class="hidden-div"><label class="service-name">Vinyl under Material Removal</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_mr_hardwood_time_ah" name="ss_services[mr][hardwood][time]" value="after" <?php if (isset($data['mr']['hardwood']['time'])&&$data['mr']['hardwood']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_mr_hardwood_time_rh" name="ss_services[mr][hardwood][time]" value="reg" <?php if (isset($data['mr']['hardwood']['time'])&&$data['mr']['hardwood']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Hardwood: </label>
				(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[mr][hardwood][sf]" class="required-text" id="ss_service_mr_hardwood_sf" value="<?php echo isset($data['mr'])?$data['mr']['hardwood']['sf']:''; ?>" /> </span> SF)
			</div>
			<div class="hidden-div"><label class="service-name">Hardwood under Material Removal</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>		
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_mr_laminate_time_ah" name="ss_services[mr][laminate][time]" value="after" <?php if (isset($data['mr']['laminate']['time'])&&$data['mr']['laminate']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_mr_laminate_time_rh" name="ss_services[mr][laminate][time]" value="reg" <?php if (isset($data['mr']['laminate']['time'])&&$data['mr']['laminate']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Laminate: </label>
				(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[mr][laminate][sf]" class="required-text" id="ss_service_mr_laminate_sf" value="<?php echo isset($data['mr'])?$data['mr']['laminate']['sf']:''; ?>" /> </span> SF)
			</div>
			<div class="hidden-div"><label class="service-name">Laminate under Material Removal</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_mr_tile_time_ah" name="ss_services[mr][tile][time]" value="after" <?php if (isset($data['mr']['tile']['time'])&&$data['mr']['tile']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_mr_tile_time_rh" name="ss_services[mr][tile][time]" value="reg" <?php if (isset($data['mr']['tile']['time'])&&$data['mr']['tile']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Tile: </label>
				(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[mr][tile][sf]" class="required-text" id="ss_service_mr_tile_sf" value="<?php echo isset($data['mr'])?$data['mr']['tile']['sf']:''; ?>" /> </span> SF)
			</div>
			<div class="hidden-div"><label class="service-name">Tile under Material Removal</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>		
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_mr_baseboard_time_ah" name="ss_services[mr][baseboard][time]" value="after" <?php if (isset($data['mr']['baseboard']['time'])&&$data['mr']['baseboard']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_mr_baseboard_time_rh" name="ss_services[mr][baseboard][time]" value="reg" <?php if (isset($data['mr']['baseboard']['time'])&&$data['mr']['baseboard']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><label>Baseboard: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_mr_baseboard_type_detach" name="ss_services[mr][baseboard][type]" value="Detach" <?php if(isset($data['mr']['baseboard']['type'])&&$data['mr']['baseboard']['type']=='Detach') { echo "checked='checked'";} ?> ><label for="ss_service_mr_baseboard_type_detach">Detach</label></li>
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_mr_baseboard_type_dispose" name="ss_services[mr][baseboard][type]" value="Detach & Dispose" <?php if(isset($data['mr']['baseboard']['type'])&&$data['mr']['baseboard']['type']=='Detach & Dispose') { echo "checked='checked'";} ?> ><label for="ss_service_mr_baseboard_type_dispose">Detach & Dispose</label></li>
				</ul>
				<span class="field-item text-field-item small-size">( <input type="text" name="ss_services[mr][baseboard][lf]" class="required-text" id="ss_service_mr_baseboard_lf" value="<?php echo isset($data['mr'])?$data['mr']['baseboard']['lf']:''; ?>" /> LF)</span> 
				<div class="hidden-div"><label class="service-name">Baseboard under Material Removal</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_mr_track_strip_time_ah" name="ss_services[mr][track_strip][time]" value="after" <?php if (isset($data['mr']['track_strip']['time'])&&$data['mr']['track_strip']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_mr_track_strip_time_rh" name="ss_services[mr][track_strip][time]" value="reg" <?php if (isset($data['mr']['track_strip']['time'])&&$data['mr']['track_strip']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Track Strip: </label>
				(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[mr][track_strip][lf]" class="required-text" id="ss_service_mr_track_strip_lf" value="<?php echo isset($data['mr'])?$data['mr']['track_strip']['lf']:''; ?>" /> </span> LF)
			</div>
			<div class="hidden-div"><label class="service-name">Track Strip under Material Removal</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>		
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_mr_toe_kick_time_ah" name="ss_services[mr][toe_kick][time]" value="after" <?php if (isset($data['mr']['toe_kick']['time'])&&$data['mr']['toe_kick']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_mr_toe_kick_time_rh" name="ss_services[mr][toe_kick][time]" value="reg" <?php if (isset($data['mr']['toe_kick']['time'])&&$data['mr']['toe_kick']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Toe Kick: </label>
				(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[mr][toe_kick][lf]" class="required-text" id="ss_service_mr_toe_kick_lf" value="<?php echo isset($data['mr'])?$data['mr']['toe_kick']['lf']:''; ?>" /> </span> LF)
			</div>
			<div class="hidden-div"><label class="service-name">Toe Kick under Material Removal</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>		
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_mr_door_casing_time_ah" name="ss_services[mr][door_casing][time]" value="after" <?php if (isset($data['mr']['door_casing']['time'])&&$data['mr']['door_casing']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_mr_door_casing_time_rh" name="ss_services[mr][door_casing][time]" value="reg" <?php if (isset($data['mr']['door_casing']['time'])&&$data['mr']['door_casing']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Door casing: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_mr_door_casing_type_detach" name="ss_services[mr][door_casing][type]" value="Detach" <?php if(isset($data['mr']['door_casing']['type'])&&$data['mr']['door_casing']['type']=='Detach') { echo "checked='checked'";} ?> ><label for="ss_service_mr_door_casing_type_detach">Detach</label></li>
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_mr_door_casing_type_dispose" name="ss_services[mr][door_casing][type]" value="Detach & Dispose" <?php if(isset($data['mr']['door_casing']['type'])&&$data['mr']['door_casing']['type']=='Detach & Dispose') { echo "checked='checked'";} ?> ><label for="ss_service_mr_door_casing_type_dispose">Detach & Dispose</label></li>
				</ul>
				(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[mr][door_casing][lf]" class="required-text" id="ss_service_mr_door_casing_lf" value="<?php echo isset($data['mr'])?$data['mr']['door_casing']['lf']:''; ?>" /> </span> LF)
			</div>
			<div class="hidden-div"><label class="service-name">Door casing under Material Removal</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_we_drywall_time_ah" name="ss_services[mr][drywall][time]" value="after" <?php if (isset($data['mr']['drywall']['time'])&&$data['mr']['drywall']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_we_drywall_time_rh" name="ss_services[mr][drywall][time]" value="reg" <?php if (isset($data['mr']['drywall']['time'])&&$data['mr']['drywall']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Drywall: </label>
				<span class="field-item text-field-item small-size"> (<input type="text" name="ss_services[mr][drywall][sf]" class="required-text" id="ss_service_we_drywall_sf" value="<?php echo isset($data['mr'])?$data['mr']['drywall']['sf']:''; ?>" />SF) </span>
			</div>
			<div class="hidden-div"><label class="service-name">Drywall under Material Removal</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_we_plaster_time_ah" name="ss_services[mr][plaster][time]" value="after" <?php if (isset($data['mr']['plaster']['time'])&&$data['mr']['plaster']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_we_plaster_time_rh" name="ss_services[mr][plaster][time]" value="reg" <?php if (isset($data['mr']['plaster']['time'])&&$data['mr']['plaster']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Plaster: </label>
				<span class="field-item text-field-item small-size"> (<input type="text" name="ss_services[mr][plaster][sf]" class="required-text" id="ss_service_we_plaster_sf" value="<?php echo isset($data['mr'])?$data['mr']['plaster']['sf']:''; ?>" />SF) </span>
			</div>
			<div class="hidden-div"><label class="service-name">Plaster under Material Removal</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_mr_insulation_time_ah" name="ss_services[mr][insulation][time]" value="after" <?php if (isset($data['mr']['insulation']['time'])&&$data['mr']['insulation']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_mr_insulation_time_rh" name="ss_services[mr][insulation][time]" value="reg" <?php if (isset($data['mr']['insulation']['time'])&&$data['mr']['insulation']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Insulation: </label>
				(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[mr][insulation][sf]" class="required-text" id="ss_service_mr_insulation_sf" value="<?php echo isset($data['mr'])?$data['mr']['insulation']['sf']:''; ?>" /> </span> SF)
			</div>
			<div class="hidden-div"><label class="service-name">Insulation under Material Removal</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>		
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_mr_cabinets_vanity_time_ah" name="ss_services[mr][cabinets_vanity][time]" value="after" <?php if (isset($data['mr']['cabinets_vanity']['time'])&&$data['mr']['cabinets_vanity']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_mr_cabinets_vanity_time_rh" name="ss_services[mr][cabinets_vanity][time]" value="reg" <?php if (isset($data['mr']['cabinets_vanity']['time'])&&$data['mr']['cabinets_vanity']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Cabinets or Vanity: </label>
				(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[mr][cabinets_vanity][lf]" class="required-text" id="ss_service_mr_cabinets_vanity_lf" value="<?php echo isset($data['mr'])?$data['mr']['cabinets_vanity']['lf']:''; ?>" /> </span> LF)
			</div>
			<div class="hidden-div"><label class="service-name">Cabinets or Vanity under Material Removal</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>	
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_mr_countertop_time_ah" name="ss_services[mr][countertop][time]" value="after" <?php if (isset($data['mr']['countertop']['time'])&&$data['mr']['countertop']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_mr_countertop_time_rh" name="ss_services[mr][countertop][time]" value="reg" <?php if (isset($data['mr']['countertop']['time'])&&$data['mr']['countertop']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Countertop: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><input type="radio" name="ss_services[mr][countertop][type]" id="ss_service_mr_countertop_type_laminate" value="Laminate" <?php if (isset($data['mr']['countertop']['type'])&&$data['mr']['countertop']['type']=='Laminate'){ echo "checked";} ?> class="cboxradio" />
				<label class="cboxlabel" for="ss_service_mr_countertop_laminate_lf">Laminate(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[mr][countertop][laminate][lf]" id="ss_service_mr_countertop_laminate_lf" value="<?php echo isset($data['mr'])?$data['mr']['countertop']['laminate']['lf']:''; ?>" /> </span> LF)</label></li>
					<li class="inline-field-item"><input type="radio" id="ss_service_mr_countertop_type_marble" name="ss_services[mr][countertop][type]" value="Marble" <?php if(isset($data['mr']['countertop']['type'])&&$data['mr']['countertop']['type']=='Marble') { echo "checked='checked'";} ?> ><label for="ss_service_mr_countertop_type_marble">Marble</label></li>
					<li class="inline-field-item"><input type="radio" id="ss_service_mr_countertop_type_granite" name="ss_services[mr][countertop][type]" value="Granite" <?php if(isset($data['mr']['countertop']['type'])&&$data['mr']['countertop']['type']=='Granite') { echo "checked='checked'";} ?> ><label for="ss_service_mr_countertop_type_granite">Granite</label></li>
				</ul>
				(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[mr][countertop][marble_granite][sf]" id="ss_service_mr_countertop_marble_granite_sf" value="<?php echo isset($data['mr'])?$data['mr']['countertop']['marble_granite']['sf']:''; ?>" /> </span> SF)
			</div>
			<div class="hidden-div"><label class="service-name">Countertop under Material Removal</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>	
		
		<tr><td colspan="4" class="ss-services-sub-section">Detach Only / Detach & Reset</td></tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_dodr_singledoor_time_ah" name="ss_services[dodr][singledoor][time]" value="after" <?php if (isset($data['dodr']['singledoor']['time'])&&$data['dodr']['singledoor']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_dodr_singledoor_time_rh" name="ss_services[dodr][singledoor][time]" value="reg" <?php if (isset($data['dodr']['singledoor']['time'])&&$data['dodr']['singledoor']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Single Door: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_dodr_singledoor_type_detach" name="ss_services[dodr][singledoor][type]" value="Detach" <?php if(isset($data['dodr']['singledoor']['type'])&&$data['dodr']['singledoor']['type']=='Detach') { echo "checked='checked'";} ?> ><label for="ss_service_dodr_singledoor_type_detach">Detach</label></li>
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_dodr_singledoor_type_dr" name="ss_services[dodr][singledoor][type]" value="Detach & Reset" <?php if(isset($data['dodr']['singledoor']['type'])&&$data['dodr']['singledoor']['type']=='Detach & Reset') { echo "checked='checked'";} ?> ><label for="ss_service_dodr_singledoor_type_dr">Detach & Reset</label></li>
				</ul>
				(#<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[dodr][singledoor][count]" class="required-text" id="ss_service_dodr_singledoor_count" value="<?php echo isset($data['dodr'])?$data['dodr']['singledoor']['count']:''; ?>" /> </span>)
			</div>
			<div class="hidden-div"><label class="service-name">Single Door under Detach Only / Detach & Reset</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_dodr_dbldoor_time_ah" name="ss_services[dodr][dbldoor][time]" value="after" <?php if (isset($data['dodr']['dbldoor']['time'])&&$data['dodr']['dbldoor']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_dodr_dbldoor_time_rh" name="ss_services[dodr][dbldoor][time]" value="reg" <?php if (isset($data['dodr']['dbldoor']['time'])&&$data['dodr']['dbldoor']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Double Door: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_dodr_dbldoor_type_detach" name="ss_services[dodr][dbldoor][type]" value="Detach" <?php if(isset($data['dodr']['dbldoor']['type'])&&$data['dodr']['dbldoor']['type']=='Detach') { echo "checked='checked'";} ?> ><label for="ss_service_dodr_dbldoor_type_detach">Detach</label></li>
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_dodr_dbldoor_type_dr" name="ss_services[dodr][dbldoor][type]" value="Detach & Reset" <?php if(isset($data['dodr']['dbldoor']['type'])&&$data['dodr']['dbldoor']['type']=='Detach & Reset') { echo "checked='checked'";} ?> ><label for="ss_service_dodr_dbldoor_type_dr">Detach & Reset</label></li>
				</ul>
				(#<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[dodr][dbldoor][count]" class="required-text" id="ss_service_dodr_dbldoor_count" value="<?php echo isset($data['dodr'])?$data['dodr']['dbldoor']['count']:''; ?>" /> </span>)
			</div>
			<div class="hidden-div"><label class="service-name">Double Door under Detach Only / Detach & Reset</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_dodr_bi_folddoor_time_ah" name="ss_services[dodr][bi_folddoor][time]" value="after" <?php if (isset($data['dodr']['bi_folddoor']['time'])&&$data['dodr']['bi_folddoor']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_dodr_bi_folddoor_time_rh" name="ss_services[dodr][bi_folddoor][time]" value="reg" <?php if (isset($data['dodr']['bi_folddoor']['time'])&&$data['dodr']['bi_folddoor']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Bi-fold Door: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_dodr_bi_folddoor_type_detach" name="ss_services[dodr][bi_folddoor][type]" value="Detach" <?php if(isset($data['dodr']['bi_folddoor']['type'])&&$data['dodr']['bi_folddoor']['type']=='Detach') { echo "checked='checked'";} ?> ><label for="ss_service_dodr_bi_folddoor_type_detach">Detach</label></li>
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_dodr_bi_folddoor_type_dr" name="ss_services[dodr][bi_folddoor][type]" value="Detach & Reset" <?php if(isset($data['dodr']['bi_folddoor']['type'])&&$data['dodr']['bi_folddoor']['type']=='Detach & Reset') { echo "checked='checked'";} ?> ><label for="ss_service_dodr_bi_folddoor_type_dr">Detach & Reset</label></li>
				</ul>
				(#<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[dodr][bi_folddoor][count]" class="required-text" id="ss_service_dodr_bi_folddoor_count" value="<?php echo isset($data['dodr'])?$data['dodr']['bi_folddoor']['count']:''; ?>" /> </span>)
			</div>
			<div class="hidden-div"><label class="service-name">Bi-fold Door under Detach Only / Detach & Reset</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_dodr_dbl_bi_folddoor_time_ah" name="ss_services[dodr][dbl_bi_folddoor][time]" value="after" <?php if (isset($data['dodr']['dbl_bi_folddoor']['time'])&&$data['dodr']['dbl_bi_folddoor']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_dodr_dbl_bi_folddoor_time_rh" name="ss_services[dodr][dbl_bi_folddoor][time]" value="reg" <?php if (isset($data['dodr']['dbl_bi_folddoor']['time'])&&$data['dodr']['dbl_bi_folddoor']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Double Bi-fold Door: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_dodr_dbl_bi_folddoor_type_detach" name="ss_services[dodr][dbl_bi_folddoor][type]" value="Detach" <?php if(isset($data['dodr']['dbl_bi_folddoor']['type'])&&$data['dodr']['dbl_bi_folddoor']['type']=='Detach') { echo "checked='checked'";} ?> ><label for="ss_service_dodr_dbl_bi_folddoor_type_detach">Detach</label></li>
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_dodr_dbl_bi_folddoor_type_dr" name="ss_services[dodr][dbl_bi_folddoor][type]" value="Detach & Reset" <?php if(isset($data['dodr']['dbl_bi_folddoor']['type'])&&$data['dodr']['dbl_bi_folddoor']['type']=='Detach & Reset') { echo "checked='checked'";} ?> ><label for="ss_service_dodr_dbl_bi_folddoor_type_dr">Detach & Reset</label></li>
				</ul>
				(#<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[dodr][dbl_bi_folddoor][count]" class="required-text" id="ss_service_dodr_dbl_bi_folddoor_count" value="<?php echo isset($data['dodr'])?$data['dodr']['dbl_bi_folddoor']['count']:''; ?>" /> </span>)
			</div>
			<div class="hidden-div"><label class="service-name">Double Bi-fold Door under Detach Only / Detach & Reset</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_dodr_singlesink_time_ah" name="ss_services[dodr][singlesink][time]" value="after" <?php if (isset($data['dodr']['singlesink']['time'])&&$data['dodr']['singlesink']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_dodr_singlesink_time_rh" name="ss_services[dodr][singlesink][time]" value="reg" <?php if (isset($data['dodr']['singlesink']['time'])&&$data['dodr']['singlesink']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Single Sink: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_dodr_singlesink_type_detach" name="ss_services[dodr][singlesink][type]" value="Detach" <?php if(isset($data['dodr']['singlesink']['type'])&&$data['dodr']['singlesink']['type']=='Detach') { echo "checked='checked'";} ?> ><label for="ss_service_dodr_singlesink_type_detach">Detach</label></li>
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_dodr_singlesink_type_dr" name="ss_services[dodr][singlesink][type]" value="Detach & Reset" <?php if(isset($data['dodr']['singlesink']['type'])&&$data['dodr']['singlesink']['type']=='Detach & Reset') { echo "checked='checked'";} ?> ><label for="ss_service_dodr_singlesink_type_dr">Detach & Reset</label></li>
				</ul>
				(#<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[dodr][singlesink][count]" class="required-text" id="ss_service_dodr_singlesink_count" value="<?php echo isset($data['dodr'])?$data['dodr']['singlesink']['count']:''; ?>" /> </span>)
			</div>
			<div class="hidden-div"><label class="service-name">Single Sink under Detach Only / Detach & Reset</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_dodr_dblsink_time_ah" name="ss_services[dodr][dblsink][time]" value="after" <?php if (isset($data['dodr']['dblsink']['time'])&&$data['dodr']['dblsink']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_dodr_dblsink_time_rh" name="ss_services[dodr][dblsink][time]" value="reg" <?php if (isset($data['dodr']['dblsink']['time'])&&$data['dodr']['dblsink']['time']=='reg'){ echo "checked";} ?> /></td>
			<td><div><label>Double Sink: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_dodr_dblsink_type_detach" name="ss_services[dodr][dblsink][type]" value="Detach" <?php if(isset($data['dodr']['dblsink']['type'])&&$data['dodr']['dblsink']['type']=='Detach') { echo "checked='checked'";} ?> ><label for="ss_service_dodr_dblsink_type_detach">Detach</label></li>
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_dodr_dblsink_type_dr" name="ss_services[dodr][dblsink][type]" value="Detach & Reset" <?php if(isset($data['dodr']['dblsink']['type'])&&$data['dodr']['dblsink']['type']=='Detach & Reset') { echo "checked='checked'";} ?> ><label for="ss_service_dodr_dblsink_type_dr">Detach & Reset</label></li>
				</ul>
				(#<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[dodr][dblsink][count]" class="required-text" id="ss_service_dodr_dblsink_count" value="<?php echo isset($data['dodr']['dblsink']['count'])?$data['dodr']['dblsink']['count']:''; ?>" /> </span>)
			</div>
			<div class="hidden-div"><label class="service-name">Double Sink under Detach Only / Detach & Reset</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_dodr_stove_time_ah" name="ss_services[dodr][stove][time]" value="after" <?php if (isset($data['dodr']['stove']['time'])&&$data['dodr']['stove']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_dodr_stove_time_rh" name="ss_services[dodr][stove][time]" value="reg" <?php if (isset($data['dodr']['stove']['time'])&&$data['dodr']['stove']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div>
				<label>Stove
					<ul class="field-check-lists inline-group-fields clearfix">
						<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_dodr_stove_stype_gas" name="ss_services[dodr][stove][stype]" value="Gas" <?php if(isset($data['dodr']['stove']['stype'])&&$data['dodr']['stove']['stype']=='Gas') { echo "checked='checked'";} ?> ><label for="ss_service_dodr_stove_stype_gas">Gas</label></li>
						<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_dodr_stove_stype_elec" name="ss_services[dodr][stove][stype]" value="Elec" <?php if(isset($data['dodr']['stove']['stype'])&&$data['dodr']['stove']['stype']=='Elec') { echo "checked='checked'";} ?> ><label for="ss_service_dodr_stove_stype_elec">Elec</label></li>
					</ul>: 
				</label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_dodr_stove_type_detach" name="ss_services[dodr][stove][type]" value="Detach" <?php if(isset($data['dodr']['stove']['type'])&&$data['dodr']['stove']['type']=='Detach') { echo "checked='checked'";} ?> ><label for="ss_service_dodr_stove_type_detach">Detach</label></li>
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_dodr_stove_type_dr" name="ss_services[dodr][stove][type]" value="Detach & Reset" <?php if(isset($data['dodr']['stove']['type'])&&$data['dodr']['stove']['type']=='Detach & Reset') { echo "checked='checked'";} ?> ><label for="ss_service_dodr_stove_type_dr">Detach & Reset</label></li>
				</ul>
				(#<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[dodr][stove][count]" class="required-text" id="ss_service_dodr_stove_count" value="<?php echo isset($data['dodr'])?$data['dodr']['stove']['count']:''; ?>" /> </span>)
			</div>
			<div class="hidden-div"><label class="service-name">Stove under Detach Only / Detach & Reset</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_dodr_washer_time_ah" name="ss_services[dodr][washer][time]" value="after" <?php if (isset($data['dodr']['washer']['time'])&&$data['dodr']['washer']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_dodr_washer_time_rh" name="ss_services[dodr][washer][time]" value="reg" <?php if (isset($data['dodr']['washer']['time'])&&$data['dodr']['washer']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div>
				<label>Washer: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_dodr_washer_type_detach" name="ss_services[dodr][washer][type]" value="Detach" <?php if(isset($data['dodr']['washer']['type'])&&$data['dodr']['washer']['type']=='Detach') { echo "checked='checked'";} ?> ><label for="ss_service_dodr_washer_type_detach">Detach</label></li>
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_dodr_washer_type_dr" name="ss_services[dodr][washer][type]" value="Detach & Reset" <?php if(isset($data['dodr']['washer']['type'])&&$data['dodr']['washer']['type']=='Detach & Reset') { echo "checked='checked'";} ?> ><label for="ss_service_dodr_washer_type_dr">Detach & Reset</label></li>
				</ul>
				(#<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[dodr][washer][count]" class="required-text" id="ss_service_dodr_washer_count" value="<?php echo isset($data['dodr'])?$data['dodr']['washer']['count']:''; ?>" /> </span>)
			</div>
			<div class="hidden-div"><label class="service-name">Washer under Detach Only / Detach & Reset</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_dodr_dryer_time_ah" name="ss_services[dodr][dryer][time]" value="after" <?php if (isset($data['dodr']['dryer']['time'])&&$data['dodr']['dryer']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_dodr_dryer_time_rh" name="ss_services[dodr][dryer][time]" value="reg" <?php if (isset($data['dodr']['dryer']['time'])&&$data['dodr']['dryer']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div>
				<label>Dryer: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_dodr_dryer_type_detach" name="ss_services[dodr][dryer][type]" value="Detach" <?php if(isset($data['dodr']['dryer']['type'])&&$data['dodr']['dryer']['type']=='Detach') { echo "checked='checked'";} ?> ><label for="ss_service_dodr_dryer_type_detach">Detach</label></li>
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_dodr_dryer_type_dr" name="ss_services[dodr][dryer][type]" value="Detach & Reset" <?php if(isset($data['dodr']['dryer']['type'])&&$data['dodr']['dryer']['type']=='Detach & Reset') { echo "checked='checked'";} ?> ><label for="ss_service_dodr_dryer_type_dr">Detach & Reset</label></li>
				</ul>
				(#<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[dodr][dryer][count]" class="required-text" id="ss_service_dodr_dryer_count" value="<?php echo isset($data['dodr'])?$data['dodr']['dryer']['count']:''; ?>" /> </span>)
			</div>
			<div class="hidden-div"><label class="service-name">Dryer under Detach Only / Detach & Reset</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_dodr_dishwasher_time_ah" name="ss_services[dodr][dishwasher][time]" value="after" <?php if (isset($data['dodr']['dishwasher']['time'])&&$data['dodr']['dishwasher']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_dodr_dishwasher_time_rh" name="ss_services[dodr][dishwasher][time]" value="reg" <?php if (isset($data['dodr']['dishwasher']['time'])&&$data['dodr']['dishwasher']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div>
				<label>Dishwasher: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_dodr_dishwasher_type_detach" name="ss_services[dodr][dishwasher][type]" value="Detach" <?php if(isset($data['dodr']['dishwasher']['type'])&&$data['dodr']['dishwasher']['type']=='Detach') { echo "checked='checked'";} ?> ><label for="ss_service_dodr_dishwasher_type_detach">Detach</label></li>
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_dodr_dishwasher_type_dr" name="ss_services[dodr][dishwasher][type]" value="Detach & Reset" <?php if(isset($data['dodr']['dishwasher']['type'])&&$data['dodr']['dishwasher']['type']=='Detach & Reset') { echo "checked='checked'";} ?> ><label for="ss_service_dodr_dishwasher_type_dr">Detach & Reset</label></li>
				</ul>
				(#<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[dodr][dishwasher][count]" class="required-text" id="ss_service_dodr_dishwasher_count" value="<?php echo isset($data['dodr']['dishwasher']['count'])?$data['dodr']['dishwasher']['count']:''; ?>" /> </span>)
			</div>
			<div class="hidden-div"><label class="service-name">Dishwasher under Detach Only / Detach & Reset</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_dodr_refrigerator_time_ah" name="ss_services[dodr][refrigerator][time]" value="after" <?php if (isset($data['dodr']['refrigerator']['time'])&&$data['dodr']['refrigerator']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_dodr_refrigerator_time_rh" name="ss_services[dodr][refrigerator][time]" value="reg" <?php if (isset($data['dodr']['refrigerator']['time'])&&$data['dodr']['refrigerator']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div>
				<label>Refrigerator: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_dodr_refrigerator_type_detach" name="ss_services[dodr][refrigerator][type]" value="Detach" <?php if(isset($data['dodr']['refrigerator']['type'])&&$data['dodr']['refrigerator']['type']=='Detach') { echo "checked='checked'";} ?> ><label for="ss_service_dodr_refrigerator_type_detach">Detach</label></li>
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_dodr_refrigerator_type_dr" name="ss_services[dodr][refrigerator][type]" value="Detach & Reset" <?php if(isset($data['dodr']['refrigerator']['type'])&&$data['dodr']['refrigerator']['type']=='Detach & Reset') { echo "checked='checked'";} ?> ><label for="ss_service_dodr_refrigerator_type_dr">Detach & Reset</label></li>
				</ul>
				(#<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[dodr][refrigerator][count]" class="required-text" id="ss_service_dodr_refrigerator_count" value="<?php echo isset($data['dodr'])?$data['dodr']['refrigerator']['count']:''; ?>" /> </span>)
			</div>
			<div class="hidden-div"><label class="service-name">Refrigerator under Detach Only / Detach & Reset</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_dodr_toilet_time_ah" name="ss_services[dodr][toilet][time]" value="after" <?php if (isset($data['dodr']['toilet']['time'])&&$data['dodr']['toilet']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_dodr_toilet_time_rh" name="ss_services[dodr][toilet][time]" value="reg" <?php if (isset($data['dodr']['toilet']['time'])&&$data['dodr']['toilet']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div>
				<label>Toilet: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_dodr_toilet_type_detach" name="ss_services[dodr][toilet][type]" value="Detach" <?php if(isset($data['dodr']['toilet']['type'])&&$data['dodr']['toilet']['type']=='Detach') { echo "checked='checked'";} ?> ><label for="ss_service_dodr_toilet_type_detach">Detach</label></li>
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_dodr_toilet_type_dr" name="ss_services[dodr][toilet][type]" value="Detach & Reset" <?php if(isset($data['dodr']['toilet']['type'])&&$data['dodr']['toilet']['type']=='Detach & Reset') { echo "checked='checked'";} ?> ><label for="ss_service_dodr_toilet_type_dr">Detach & Reset</label></li>
				</ul>
				(#<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[dodr][toilet][count]" class="required-text" id="ss_service_dodr_toilet_count" value="<?php echo isset($data['dodr'])?$data['dodr']['toilet']['count']:''; ?>" /> </span>)
			</div>
			<div class="hidden-div"><label class="service-name">Toilet under Detach Only / Detach & Reset</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		
		<tr><td colspan="4" class="ss-services-sub-section">Cleaning Services</td></tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_cls_clean_carpet_time_ah" name="ss_services[cls][clean_carpet][time]" value="after" <?php if (isset($data['cls']['clean_carpet']['time'])&&$data['cls']['clean_carpet']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_cls_clean_carpet_time_rh" name="ss_services[cls][clean_carpet][time]" value="reg" <?php if (isset($data['cls']['clean_carpet']['time'])&&$data['cls']['clean_carpet']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Clean Carpet: </label>
				(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[cls][clean_carpet][sf]" class="required-text" id="ss_service_cls_clean_carpet_sf" value="<?php echo isset($data['cls'])?$data['cls']['clean_carpet']['sf']:''; ?>" /> </span> SF)
			</div>
			<div class="hidden-div"><label class="service-name">Clean Carpet under Cleaning Services</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_cls_clean_tile_time_ah" name="ss_services[cls][clean_tile][time]" value="after" <?php if (isset($data['cls']['clean_tile']['time'])&&$data['cls']['clean_tile']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_cls_clean_tile_time_rh" name="ss_services[cls][clean_tile][time]" value="reg" <?php if (isset($data['cls']['clean_tile']['time'])&&$data['cls']['clean_tile']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Clean Tile: </label>
				(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[cls][clean_tile][sf]" class="required-text" id="ss_service_cls_clean_tile_sf" value="<?php echo isset($data['cls'])?$data['cls']['clean_tile']['sf']:''; ?>" /> </span> SF)
			</div>
			<div class="hidden-div"><label class="service-name">Clean Tile under Cleaning Services</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_cls_clean_otherfloor_time_ah" name="ss_services[cls][clean_otherfloor][time]" value="after" <?php if (isset($data['cls']['clean_otherfloor']['time'])&&$data['cls']['clean_otherfloor']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_cls_clean_otherfloor_time_rh" name="ss_services[cls][clean_otherfloor][time]" value="reg" <?php if (isset($data['cls']['clean_otherfloor']['time'])&&$data['cls']['clean_otherfloor']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Clean Other Floor: </label>
				(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[cls][clean_otherfloor][sf]" class="required-text" id="ss_service_cls_clean_otherfloor_sf" value="<?php echo isset($data['cls'])?$data['cls']['clean_otherfloor']['sf']:''; ?>" /> </span> SF)
			</div>
			<div class="hidden-div"><label class="service-name">Clean Other Floor under Cleaning Services</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		
		<tr><td colspan="4" class="ss-services-sub-section">Carpet Services</td></tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_cs_detach_relay_carpet_time_ah" name="ss_services[cs][detach_relay_carpet][time]" value="after" <?php if (isset($data['cs']['detach_relay_carpet']['time'])&&$data['cs']['detach_relay_carpet']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_cs_detach_relay_carpet_time_rh" name="ss_services[cs][detach_relay_carpet][time]" value="reg" <?php if (isset($data['cs']['detach_relay_carpet']['time'])&&$data['cs']['detach_relay_carpet']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Detach & Relay Carpet: </label>
				(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[cs][detach_relay_carpet][sf]" class="required-text" id="ss_service_cs_detach_relay_carpet_sf" value="<?php echo isset($data['cs'])?$data['cs']['detach_relay_carpet']['sf']:''; ?>" /> </span> SF)
			</div>
			<div class="hidden-div"><label class="service-name">Detach & Relay Carpet under Carpet Services</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_cs_reseam_carpet_time_ah" name="ss_services[cs][reseam_carpet][time]" value="after" <?php if (isset($data['cs']['reseam_carpet']['time'])&&$data['cs']['reseam_carpet']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_cs_reseam_carpet_time_rh" name="ss_services[cs][reseam_carpet][time]" value="reg" <?php if (isset($data['cs']['reseam_carpet']['time'])&&$data['cs']['reseam_carpet']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Re-Seam Carpet: </label>
				(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[cs][reseam_carpet][lf]" class="required-text" id="ss_service_cs_reseam_carpet_lf" value="<?php echo isset($data['cs'])?$data['cs']['reseam_carpet']['lf']:''; ?>" /> </span> LF)
			</div>
			<div class="hidden-div"><label class="service-name">Re-Seam Carpet under Carpet Services</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_cs_apply_carpet_protector_time_ah" name="ss_services[cs][apply_carpet_protector][time]" value="after" <?php if (isset($data['cs']['apply_carpet_protector']['time'])&&$data['cs']['apply_carpet_protector']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_cs_apply_carpet_protector_time_rh" name="ss_services[cs][apply_carpet_protector][time]" value="reg" <?php if (isset($data['cs']['apply_carpet_protector']['time'])&&$data['cs']['apply_carpet_protector']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Apply Carpet Protector: </label>
				(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[cs][apply_carpet_protector][sf]" class="required-text" id="ss_service_cs_apply_carpet_protector_sf" value="<?php echo isset($data['cs'])?$data['cs']['apply_carpet_protector']['sf']:''; ?>" /> </span> SF)
			</div>
			<div class="hidden-div"><label class="service-name">Apply Carpet Protector under Carpet Services</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_cs_install_pad_time_ah" name="ss_services[cs][install_pad][time]" value="after" <?php if (isset($data['cs']['install_pad']['time'])&&$data['cs']['install_pad']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_cs_install_pad_time_rh" name="ss_services[cs][install_pad][time]" value="reg" <?php if (isset($data['cs']['install_pad']['time'])&&$data['cs']['install_pad']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Install Pad: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_cs_install_pad_grade_standard" name="ss_services[cs][install_pad][grade]" value="Standard" <?php if(isset($data['cs']['install_pad']['grade'])&&$data['cs']['install_pad']['grade']=='Standard') { echo "checked='checked'";} ?> ><label for="ss_service_cs_install_pad_grade_standard">Standard</label></li>
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_cs_install_pad_grade_average" name="ss_services[cs][install_pad][grade]" value="Average" <?php if(isset($data['cs']['install_pad']['grade'])&&$data['cs']['install_pad']['grade']=='Average') { echo "checked='checked'";} ?> ><label for="ss_service_cs_install_pad_grade_average">Average</label></li>
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_cs_install_pad_grade_high" name="ss_services[cs][install_pad][grade]" value="High" <?php if(isset($data['cs']['install_pad']['grade'])&&$data['cs']['install_pad']['grade']=='High') { echo "checked='checked'";} ?> ><label for="ss_service_cs_install_pad_grade_high">High</label></li>
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_cs_install_pad_grade_premium" name="ss_services[cs][install_pad][grade]" value="Premium" <?php if(isset($data['cs']['install_pad']['grade'])&&$data['cs']['install_pad']['grade']=='Premium') { echo "checked='checked'";} ?> ><label for="ss_service_cs_install_pad_grade_premium">Premium</label></li>
				</ul>
				(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[cs][install_pad][sf]" class="required-text" id="ss_service_cs_install_pad_sf" value="<?php echo isset($data['cs'])?$data['cs']['install_pad']['sf']:''; ?>" />SF</span>)
			</div>
			<div class="hidden-div"><label class="service-name">Install Pad under Carpet Services</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		
		<tr><td colspan="4" class="ss-services-sub-section">Anti Microbial</td></tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_am_plant_time_ah" name="ss_services[am][plant][time]" value="after" <?php if (isset($data['am']['plant']['time'])&&$data['am']['plant']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_am_plant_time_rh" name="ss_services[am][plant][time]" value="reg" <?php if (isset($data['am']['plant']['time'])&&$data['am']['plant']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label></label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_am_plant_type_non_based" name="ss_services[am][plant][type]" value="Non-Plant Based" <?php if(isset($data['am']['plant']['type'])&&$data['am']['plant']['type']=='Non-Plant Based') { echo "checked='checked'";} ?> ><label for="ss_service_am_plant_type_non_based">Non-Plant Based</label></li>
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_am_plant_type_based" name="ss_services[am][plant][type]" value="Plant Based" <?php if(isset($data['am']['plant']['type'])&&$data['am']['plant']['type']=='Plant Based') { echo "checked='checked'";} ?> ><label for="ss_service_am_plant_type_based">Plant Based</label></li>
				</ul>
				<label>Floors(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[am][plant][floors_sf]" class="required-text" id="ss_service_am_plant_floors_sf" value="<?php echo isset($data['am'])?$data['am']['plant']['floors_sf']:''; ?>" /> </span> SF)</label>
				<label>Walls(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[am][plant][walls_sf]" class="required-text" id="ss_service_am_plant_walls_sf" value="<?php echo isset($data['am'])?$data['am']['plant']['walls_sf']:''; ?>" /> </span> SF)</label>
				<label>Ceiling(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[am][plant][ceiling_sf]" class="required-text" id="ss_service_am_plant_ceiling_sf" value="<?php echo isset($data['am'])?$data['am']['plant']['ceiling_sf']:''; ?>" /> </span> SF)</label>
			</div>
			<div class="hidden-div"><label class="service-name">Plant Based under Anti Microbial</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_am_thermal_fog_time_ah" name="ss_services[am][thermal_fog][time]" value="after" <?php if (isset($data['am']['thermal_fog']['time'])&&$data['am']['thermal_fog']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_am_thermal_fog_time_rh" name="ss_services[am][thermal_fog][time]" value="reg" <?php if (isset($data['am']['thermal_fog']['time'])&&$data['am']['thermal_fog']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Thermal Fog: </label>
				(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[am][thermal_fog][cubic_ft]" class="required-text" id="ss_service_am_thermal_fog_cubic_ft" value="<?php echo isset($data['am'])?$data['am']['thermal_fog']['cubic_ft']:''; ?>" /> </span> Cubic Ft)
			</div>
			<div class="hidden-div"><label class="service-name">Thermal Fog under Anti Microbial</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_am_ozone_time_ah" name="ss_services[am][ozone][time]" value="after" <?php if (isset($data['am']['ozone']['time'])&&$data['am']['ozone']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_am_ozone_time_rh" name="ss_services[am][ozone][time]" value="reg" <?php if (isset($data['am']['ozone']['time'])&&$data['am']['ozone']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Ozone: </label>
				(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[am][ozone][cubic_ft]" class="required-text" id="ss_service_am_ozone_cubic_ft" value="<?php echo isset($data['am'])?$data['am']['ozone']['cubic_ft']:''; ?>" /> </span> Cubic Ft)
			</div>
			<div class="hidden-div"><label class="service-name">Ozone under Anti Microbial</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		
		<tr><td colspan="4" class="ss-services-sub-section">Content Manipulation</td></tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_cm_acm_time_ah" name="ss_services[cm][acm][time]" value="after" <?php if (isset($data['cm']['acm']['time'])&&$data['cm']['acm']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_cm_acm_time_rh" name="ss_services[cm][acm][time]" value="reg" <?php if (isset($data['cm']['acm']['time'])&&$data['cm']['acm']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Amount of contents moved: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_cm_acm_amount_sml" name="ss_services[cm][acm][amount]" value="SML" <?php if(isset($data['cm']['acm']['amount'])&&$data['cm']['acm']['amount']=='SML') { echo "checked='checked'";} ?> ><label for="ss_service_cm_acm_amount_sml">SML</label></li>
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_cm_acm_amount_avg" name="ss_services[cm][acm][amount]" value="AVG" <?php if(isset($data['cm']['acm']['amount'])&&$data['cm']['acm']['amount']=='AVG') { echo "checked='checked'";} ?> ><label for="ss_service_cm_acm_amount_avg">AVG</label></li>
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_cm_acm_amount_lrg" name="ss_services[cm][acm][amount]" value="LRG" <?php if(isset($data['cm']['acm']['amount'])&&$data['cm']['acm']['amount']=='LRG') { echo "checked='checked'";} ?> ><label for="ss_service_cm_acm_amount_lrg">LRG</label></li>
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_cm_acm_amount_xl"  name="ss_services[cm][acm][amount]" value="XL"  <?php if(isset($data['cm']['acm']['amount'])&&$data['cm']['acm']['amount']=='XL' ) { echo "checked='checked'";} ?> ><label for="ss_service_cm_acm_amount_xl" >XL</label> </li>
					<li class="inline-field-item"><input type="radio" class="required-radio cboxradio" id="ss_service_cm_acm_amount_hours"  name="ss_services[cm][acm][amount]" value="Hours"  <?php if(isset($data['cm']['acm']['amount'])&&$data['cm']['acm']['amount']=='Hours' ) { echo "checked='checked'";} ?> >
						<label class="cboxlabel" for="ss_service_cm_acm_hours_val" >Hours (#<input type="text" name="ss_services[cm][acm][hours][val]" id="ss_service_cm_acm_hours_val" value="<?php echo isset($data['cm'])?$data['cm']['acm']['hours']['val']:''; ?>" />)</label> </li>
				</ul>
			</div>
			<div class="hidden-div"><label class="service-name">Amount of contents moved under Content Manipulation</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_cm_block_pad_time_ah" name="ss_services[cm][block_pad][time]" value="after" <?php if (isset($data['cm']['block_pad']['time'])&&$data['cm']['block_pad']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_cm_block_pad_time_rh" name="ss_services[cm][block_pad][time]" value="reg" <?php if (isset($data['cm']['block_pad']['time'])&&$data['cm']['block_pad']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Block and Pad: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_cm_block_pad_amount_sml" name="ss_services[cm][block_pad][amount]" value="SML" <?php if(isset($data['cm']['block_pad']['amount'])&&$data['cm']['block_pad']['amount']=='SML') { echo "checked='checked'";} ?> ><label for="ss_service_cm_block_pad_amount_sml">SML</label></li>
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_cm_block_pad_amount_avg" name="ss_services[cm][block_pad][amount]" value="AVG" <?php if(isset($data['cm']['block_pad']['amount'])&&$data['cm']['block_pad']['amount']=='AVG') { echo "checked='checked'";} ?> ><label for="ss_service_cm_block_pad_amount_avg">AVG</label></li>
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_cm_block_pad_amount_lrg" name="ss_services[cm][block_pad][amount]" value="LRG" <?php if(isset($data['cm']['block_pad']['amount'])&&$data['cm']['block_pad']['amount']=='LRG') { echo "checked='checked'";} ?> ><label for="ss_service_cm_block_pad_amount_lrg">LRG</label></li>
				</ul>
			</div>
			<div class="hidden-div"><label class="service-name">Block and Pad under Content Manipulation</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		
		<tr><td colspan="4" class="ss-services-sub-section">Miscellaneous</td></tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_misc_build_containment_time_ah" name="ss_services[misc][build_containment][time]" value="after" <?php if (isset($data['misc']['build_containment']['time'])&&$data['misc']['build_containment']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_misc_build_containment_time_rh" name="ss_services[misc][build_containment][time]" value="reg" <?php if (isset($data['misc']['build_containment']['time'])&&$data['misc']['build_containment']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Build Containment: </label>
				<span class="field-item text-field-item small-size"> (SF covered <input type="text" name="ss_services[misc][build_containment][sf]" class="required-text" id="ss_service_misc_build_containment_sf" value="<?php echo isset($data['misc'])?$data['misc']['build_containment']['sf']:''; ?>" />) </span>
			</div>
			<div class="hidden-div"><label class="service-name">Build Containment under Miscellaneous</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_misc_protect_time_ah" name="ss_services[misc][protect][time]" value="after" <?php if (isset($data['misc']['protect']['time'])&&$data['misc']['protect']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_misc_protect_time_rh" name="ss_services[misc][protect][time]" value="reg" <?php if (isset($data['misc']['protect']['time'])&&$data['misc']['protect']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Protect(w/6mil plastic): </label>
				<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[misc][protect][L]" class="required-text" id="ss_service_misc_protect_L" value="<?php echo isset($data['misc'])?$data['misc']['protect']['L']:''; ?>" />(L) </span>X
				<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[misc][protect][W]" class="required-text" id="ss_service_misc_protect_W" value="<?php echo isset($data['misc'])?$data['misc']['protect']['W']:''; ?>" />(W) </span>
			</div>
			<div class="hidden-div"><label class="service-name">Protect(w/6mil plastic) under Miscellaneous</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_misc_hepa_vacuum_time_ah" name="ss_services[misc][hepa_vacuum][time]" value="after" <?php if (isset($data['misc']['hepa_vacuum']['time'])&&$data['misc']['hepa_vacuum']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_misc_hepa_vacuum_time_rh" name="ss_services[misc][hepa_vacuum][time]" value="reg" <?php if (isset($data['misc']['hepa_vacuum']['time'])&&$data['misc']['hepa_vacuum']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>HEPA Vacuum: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><input type="radio" class="required-radio cboxradio" id="ss_service_misc_hepa_vacuum_type_sf" name="ss_services[misc][hepa_vacuum][type]" value="SF" <?php if(isset($data['misc']['hepa_vacuum']['type'])&&$data['misc']['hepa_vacuum']['type']=='SF') { echo "checked='checked'";} ?> >
						<label class="cboxlabel" for="ss_service_misc_hepa_vacuum_sf"><span class="field-item text-field-item small-size"> (<input type="text" name="ss_services[misc][hepa_vacuum][sf]" id="ss_service_misc_hepa_vacuum_sf" value="<?php echo isset($data['misc'])?$data['misc']['hepa_vacuum']['sf']:''; ?>" />SF) </span></label></li>
					<li class="inline-field-item"><input type="radio" class="required-radio cboxradio" id="ss_service_misc_hepa_vacuum_type_hours" name="ss_services[misc][hepa_vacuum][type]" value="Hours" <?php if(isset($data['misc']['hepa_vacuum']['type'])&&$data['misc']['hepa_vacuum']['type']=='Hours') { echo "checked='checked'";} ?> >
						<label class="cboxlabel" for="ss_service_misc_hepa_vacuum_hours">Hours <span class="field-item text-field-item small-size"> (#<input type="text" name="ss_services[misc][hepa_vacuum][hours]" id="ss_service_misc_hepa_vacuum_hours" value="<?php echo isset($data['misc'])?$data['misc']['hepa_vacuum']['hours']:''; ?>" />) </span></label></li>
				</ul>
			</div>
			<div class="hidden-div"><label class="service-name">HEPA Vacuum under Miscellaneous</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_misc_drill_holes_time_ah" name="ss_services[misc][drill_holes][time]" value="after" <?php if (isset($data['misc']['drill_holes']['time'])&&$data['misc']['drill_holes']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_misc_drill_holes_time_rh" name="ss_services[misc][drill_holes][time]" value="reg" <?php if (isset($data['misc']['drill_holes']['time'])&&$data['misc']['drill_holes']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Drill Holes: </label>
				(#<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[misc][drill_holes][count]" class="required-text" id="ss_service_misc_drill_holes_count" value="<?php echo isset($data['misc'])?$data['misc']['drill_holes']['count']:''; ?>" /></span>)
			</div>
			<div class="hidden-div"><label class="service-name">Drill Holes under Miscellaneous</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_misc_pump_water_time_ah" name="ss_services[misc][pump_water][time]" value="after" <?php if (isset($data['misc']['pump_water']['time'])&&$data['misc']['pump_water']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_misc_pump_water_time_rh" name="ss_services[misc][pump_water][time]" value="reg" <?php if (isset($data['misc']['pump_water']['time'])&&$data['misc']['pump_water']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Pump Water: </label>
				(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[misc][pump_water][gals]" class="required-text" id="ss_service_misc_pump_water_gals" value="<?php echo isset($data['misc'])?$data['misc']['pump_water']['gals']:''; ?>" />GALS</span>)
			</div>
			<div class="hidden-div"><label class="service-name">Pump Water under Miscellaneous</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_misc_zippers_time_ah" name="ss_services[misc][zippers][time]" value="after" <?php if (isset($data['misc']['zippers']['time'])&&$data['misc']['zippers']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_misc_zippers_time_rh" name="ss_services[misc][zippers][time]" value="reg" <?php if (isset($data['misc']['zippers']['time'])&&$data['misc']['zippers']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Zippers: </label>
				(<span class="field-item text-field-item small-size"> <input type="text" name="ss_services[misc][zippers][qty]" class="required-text" id="ss_service_misc_zippers_qty" value="<?php echo isset($data['misc'])?$data['misc']['zippers']['qty']:''; ?>" />QTY</span>)
			</div>
			<div class="hidden-div"><label class="service-name">Zippers under Miscellaneous</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_misc_zip_poles_time_ah" name="ss_services[misc][zip_poles][time]" value="after" <?php if (isset($data['misc']['zip_poles']['time'])&&$data['misc']['zip_poles']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_misc_zip_poles_time_rh" name="ss_services[misc][zip_poles][time]" value="reg" <?php if (isset($data['misc']['zip_poles']['time'])&&$data['misc']['zip_poles']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Zip Poles: </label>
				<span class="field-item text-field-item small-size">(# <input type="text" name="ss_services[misc][zip_poles][count]" class="required-text" id="ss_service_misc_zip_poles_count" value="<?php echo isset($data['misc'])?$data['misc']['zip_poles']['count']:''; ?>" />)</span>
			</div>
			<div class="hidden-div"><label class="service-name">Zip Poles under Miscellaneous</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		<tr class="required-row">
			<td class="after-hours"><input type="radio" id="ss_service_misc_decon_equipment_time_ah" name="ss_services[misc][decon_equipment][time]" value="after" <?php if (isset($data['misc']['decon_equipment']['time'])&&$data['misc']['decon_equipment']['time']=='after'){ echo "checked";} ?> /></td>
			<td class="reg-hours"><input type="radio" id="ss_service_misc_decon_equipment_time_rh" name="ss_services[misc][decon_equipment][time]" value="reg" <?php if (isset($data['misc']['decon_equipment']['time'])&&$data['misc']['decon_equipment']['time']=='reg'){ echo "checked";} ?> /></td>
			<td class="ss-service-data"><div><label>Decontaminate Equipment: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_misc_decon_equipment_size_regular" name="ss_services[misc][decon_equipment][size]" value="Regular" <?php if(isset($data['misc']['decon_equipment']['size'])&&$data['misc']['decon_equipment']['size']=='Regular') { echo "checked='checked'";} ?> ><label for="ss_service_misc_decon_equipment_size_regular">Regular</label></li>
					<li class="inline-field-item"><input type="radio" class="required-radio" id="ss_service_misc_decon_equipment_size_heavy" name="ss_services[misc][decon_equipment][size]" value="Heavy" <?php if(isset($data['misc']['decon_equipment']['size'])&&$data['misc']['decon_equipment']['size']=='Heavy') { echo "checked='checked'";} ?> ><label for="ss_service_misc_decon_equipment_size_heavy">Heavy</label></li>						
				</ul>
				<span class="field-item text-field-item small-size">(#<input type="text" name="ss_services[misc][decon_equipment][heavy][amount]" class="required-text" id="ss_service_misc_decon_equipment_heavy_amount" value="<?php echo isset($data['misc'])?$data['misc']['decon_equipment']['heavy']['amount']:''; ?>" />)</span>
			</div>
			<div class="hidden-div"><label class="service-name">Decontaminate Equipment under Miscellaneous</label></div>
			</td>
      <td class="reset-radio"><input type="button" class="radio-reset-button" value="Reset" /></td>
		</tr>
		
	</tbody>
</table>
<?php	
	$output = ob_get_contents();
	ob_end_clean();
	
	return $output;
}

function ncn_admin_get_ss_services($claim_id, $room_name) {
	$data = get_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_services');
	if ($data) {
		$data = unserialize($data);
	} else {
		$data = array();
	}
	
	// Populate
	return $data;
}

/**
 * SS Render - "Comments" Section
 */ 
function ncn_admin_ss_render_comments($claim_id, $room_name, $section) {	
	$data = array();
	if (isset($_POST['ss_comments'])) {
		$data = $_POST['ss_comments'];
	} else {
		$data = ncn_admin_get_ss_comments($claim_id, $room_name);
	}
	ob_start();
?>
<table>
	<tr>
		<td class="td-label"><label for="ss_comments_comments">Comments:</label></td>
		<td><div class="field-item text-field-item"><textarea name="ss_comments[content]" id="ss_comments_content" cols="60" rows="10"><?php echo isset($data['content'])?$data['content']:''; ?></textarea></div></td>
	</tr>
</table>
<?php
	$output = ob_get_contents();
	ob_end_clean();
	
	return $output;
}
function ncn_admin_get_ss_comments($claim_id, $room_name) {
	$data = get_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_comments');
	if ($data) {
		$data = unserialize($data);
		if (isset($data['comments']) && !isset($data['content'])) {
			/** change comments->content **/	
			$data['content'] = $data['comments'];
			unset($data['comments']);
			set_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_comments', serialize($data));
		}
	} else {
		$data = array();
	}
	
	// Populate
	
	return $data;
}

/**
 * admin/config/ncn_admin/edit-room-scopesheet/{claim_id}/{room_name}
 */
function ncn_admin_edit_room_scopesheet($claim_id, $room_name='') {
	if ($room_name == '') {
		$room_name = ncn_admin_get_first_room_name($claim_id);
	}
	
	if ($room_name == '') {
		drupal_goto('admin/config/ncn_admin/viewclaim/'.$claim_id);
		exit;
	}
	drupal_set_title(t('Edit Room ScopeSheet (!claim_id - !room_name)', array('!claim_id'=>$claim_id, '!room_name'=>$room_name)));
?>
<div id="ncn_admin_edit_room_scopesheet" class="ncn_admin_room_scopesheet">
	<div class="room-ss-room-links clearfix">
		<?php print ncn_admin_room_ss_render_room_link($claim_id, $room_name, 'edit'); ?>
		<?php if (user_access('ncn_admin view room scopesheet')): ?>
		<div class="room-ss-room-edit-view-link">
			<a href="<?php print base_path()."admin/config/ncn_admin/view-room-scopesheet/$claim_id/$room_name"; ?>">View Scopesheet</a>
		</div>
		<?php endif; ?>
	</div>
	<div class="room-ss-content">
		<?php print ncn_admin_room_ss_render_room_scopesheet($claim_id, $room_name); ?>
	</div>
</div>
<?php
}

// it's not default room name
function ncn_admin_get_first_room_name($claim_id) {
	$r_data = ncn_admin_get_claim_roomname_data($claim_id, TRUE);
	$first_room_name = '';
	if (!empty($r_data)) {
		$first_room_name = $r_data[0]['room_name'];
	}
	
	return $first_room_name;
}

function ncn_admin_room_ss_render_room_link($claim_id, $room_name, $url_type) {
	$output = '<ul class="ss-room-links">';
	$rooms = ncn_admin_get_claim_roomname_data($claim_id, TRUE);
	foreach ($rooms as $r_data) {
		$active = '';
		if ($r_data['room_name'] == $room_name) {
			$active = 'active';
		}
		if ($url_type == 'read') {
			$_url = base_path()."admin/config/ncn_admin/view-room-scopesheet/$claim_id/".$r_data['room_name'];
		} else {
			$_url = base_path()."admin/config/ncn_admin/edit-room-scopesheet/$claim_id/".$r_data['room_name'];
		}
		$output.= '<li class="room-link '.$active.'"><a href="'.$_url.'" >'.$r_data['room_name'].'</a></li>';
	}
	$output.= '</ul>';
	
	return $output;
}

function ncn_admin_get_scopesheet_sections() {
	return array(
		'room_dimensions' 								=> array('title'=>'Room dimensions', 'render_func'=>'ncn_admin_ss_render_room_dimensions' ), 
		'exterior_interior_temperatures'	=> array('title'=>'Exterior & Interior Temperatures', 'render_func'=>'ncn_admin_ss_render_ext_inter_temperatures'), 
		'structural_moisture_reading' 		=> array('title'=>'Structural Moisture Readings', 'render_func'=>'ncn_admin_ss_render_structural_moisture_readings'), 
		'equipment_placement'							=> array('title'=>'Equipment Placement', 'render_func'=>'ncn_admin_ss_render_equipment_placement'), 
		'services'												=> array('title'=>'Services', 'render_func'=>'ncn_admin_ss_render_services'), 
		'comments' 												=> array('title'=>'Comments', 'render_func'=>'ncn_admin_ss_render_comments'), 
	);
}

function ncn_admin_room_ss_render_room_scopesheet($claim_id, $room_name) {
	if (isset($_POST['tfunction']) && $_POST['tfunction']=='room_scope_sheet') {
		if (isset($_POST['ss_rd'])) {
			//set_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_room_dimensions', serialize($_POST['ss_rd']));
		}
		if (isset($_POST['ss_smr'])) {
			set_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_structural_moisture_readings', serialize($_POST['ss_smr']));
		}
		if (isset($_POST['ss_ep'])) {
			//set_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_equipment_placement', serialize($_POST['ss_ep']));
		}
		if (isset($_POST['ss_services'])) {
			set_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_services', serialize($_POST['ss_services']));
		}
		if (isset($_POST['ss_comments'])) {
			set_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_comments', serialize($_POST['ss_comments']));
		}
	}
	
	ob_start();
?>
<form id='room_scopesheet_form' method='POST'>
	<input type='hidden' name='tfunction' value='room_scope_sheet' />
	<?php 
		$sections = ncn_admin_get_scopesheet_sections();
		foreach ($sections as $section=>$s_info) {
			if ($s_info['render_func'] && function_exists($s_info['render_func'])) {
				echo "<div class='$section'>";
				echo "<fieldset class='aci-section-content ss-section-content'>";
				echo "<legend>".$s_info['title']."</legend>";
				print call_user_func($s_info['render_func'], $claim_id, $room_name, $section);
				echo "</fieldset>";
				echo "</div>";
			}
		}
	?>
	<div class='aci-actions ss-actions'><input type='submit' value='  Save  ' onclick="return check_row_required(); "/></div>
</form>
<?php 
	$output = ob_get_contents();
	ob_end_clean();
	return $output;
}

function ncn_admin_view_room_scopesheet($claim_id, $room_name) {
	drupal_set_title(t('View Room ScopeSheet (!claim_id - !room_name)', array('!claim_id'=>$claim_id, '!room_name'=>$room_name)));
	$sections = ncn_admin_get_scopesheet_section_views();
	echo "<div id='ncn_admin_view_room_scopesheet' class='ncn_admin_room_scopesheet'>";
		echo '<div class="room-ss-room-links clearfix">';
			print ncn_admin_room_ss_render_room_link($claim_id, $room_name, 'read');
			if (user_access('ncn_admin edit room scopesheet')) {
			print '<div class="room-ss-room-view-edit-link">
				<a href="'.base_path()."admin/config/ncn_admin/edit-room-scopesheet/$claim_id/$room_name".'">Edit Scopesheet</a>
			</div>';
			}
		echo '</div>';
		echo '<div class="room-ss-content">';
			foreach ($sections as $section=>$s_info) {	
				if ($s_info['render_func'] && function_exists($s_info['render_func'])) {
					echo "<div class='$section'>";
					echo "<fieldset class='aci-section-content ss-section-content'>";
					echo "<legend>".$s_info['title']."</legend>";
					print call_user_func($s_info['render_func'], $claim_id, $room_name, $section);
					echo "</fieldset>";
					echo "</div>";
				}
			}
		echo "</div>";
	echo "</div>";
}

function ncn_admin_get_scopesheet_section_views() {
	return array(
		'room_dimensions' 								=> array('title'=>'Room dimensions', 'render_func'=>'ncn_admin_ss_render_view_room_dimensions' ), 
		'exterior_interior_temperatures'	=> array('title'=>'Exterior & Interior Temperatures', 'render_func'=>'ncn_admin_ss_render_view_ext_inter_temperatures'), 
		'structural_moisture_reading' 		=> array('title'=>'Structural Moisture Readings', 'render_func'=>'ncn_admin_ss_render_view_structural_moisture_readings'), 
		'equipment_placement'							=> array('title'=>'Equipment Placement', 'render_func'=>'ncn_admin_ss_render_view_equipment_placement'), 
		'services'												=> array('title'=>'Services', 'render_func'=>'ncn_admin_ss_render_view_services'), 
		'comments' 												=> array('title'=>'Comments', 'render_func'=>'ncn_admin_ss_render_view_comments'), 
	);
}

function ncn_admin_ss_render_view_room_dimensions($claim_id, $room_name, $section) {
	$data = array();
	$data = ncn_admin_get_ss_rd_list($claim_id, $room_name);
	if (empty($data)) { return ''; }
	ob_start();
?>
<div id="ss_rd_list">
<table class="ss-list-table">
<thead>
	<th class='ss-rd-name'></th>
	<th class='ss-rd-size'>Size</th>
	<th class='ss-rd-floor-type'>Floor Type</th>
	<th class='ss-rd-operation'></th>
</thead>
<tbody>
<?php foreach ($data as $key=>$item): ?>
	<tr>
		<td class='ss-rd-name'><?php echo ($item['type']=='Room')? $room_name:$item['type']; if ($item['type']=='Other') { echo "(".$item['name'].")"; } ?></td>
		<td class='ss-rd-size'><?php echo $item['size']['L'].' x '.$item['size']['W'].' x '.$item['size']['H']; ?></td>
		<td class='ss-rd-floor-type'><?php echo $item['floor_type']; ?></td>
		<td class='ss-rd-operation'></td>
	</tr>
<?php endforeach; ?>
</tbody>
</table>
</div>
<?php
	$output = ob_get_contents();
	ob_end_clean();
	
	return $output;
}

function ncn_admin_ss_render_view_ext_inter_temperatures($claim_id, $room_name, $section) {
	$data = array();
	$data = ncn_admin_get_ss_eit_list($claim_id, $room_name);
	if (empty($data)) { return ''; }
	
	ob_start();
?>
<div id="ss_eit_list">
<table class="ss-list-table">
<thead>
	<th class='ss-eit-date'>Date</th>
	<th class='ss-eit-exterior-temp'>Exterior Temp / RH%</th>
	<th class='ss-eit-exterior-temp'>Interior Temp / RH%</th>
	<th class='ss-eit-gpp'>GPP</th>
	<th class='ss-eit-time-of-readings'>Time of Readings</th>
	<th class='ss-eit-operation'></th>
</thead>
<tbody>
<?php foreach ($data as $key=>$item): ?>
	<tr>
		<td class='ss-eit-date'><?php echo $item['date']; ?></td>
		<td class='ss-eit-exterior-temp'><?php echo $item['ext_temp']; ?> / <?php echo $item['ext_rh']; ?></td>
		<td class='ss-eit-exterior-temp'><?php echo $item['int_temp']; ?> / <?php echo $item['int_rh']; ?></td>
		<td class='ss-eit-gpp'><?php echo $item['gpp']; ?></td>
		<td class='ss-eit-time-of-readings'><?php echo ncn_admin_format_american_time($item['time_readings']).' '.$item['time_readings_half']; ?></td>
		<td class='ss-eit-operation'></td>
	</tr>
<?php endforeach; ?>
</tbody>
</table>
</div>
<?php
	$output = ob_get_contents();
	ob_end_clean();
	
	return $output;
}

function ncn_admin_ss_render_view_structural_moisture_readings($claim_id, $room_name, $section) {
	ob_start();
	$data = ncn_admin_get_ss_structural_moisture_readings($claim_id, $room_name);
?>
<?php if ($data): ?>
<table>
	<?php if ($data['walldry']): ?>
	<tr>
		<td class="td-label"><label for="ss_smr_walldry_standard">Wall Dry Standard:</label></td>
		<td>
			<div class="field-item text-field-item"><?php echo $data['walldry']; ?></div>
		</td>
	</tr>
	<?php endif; ?>
	<?php if ($data['floordry']): ?>
	<tr>
		<td class="td-label"><label for="ss_smr_floordry_standard">Floor Dry Standard:</label></td>
		<td>
			<div class="field-item text-field-item"><?php echo $data['floordry']; ?></div>
		</td>
	</tr>
	<?php endif; ?>
	<?php if ($data['ceilingdry']): ?>
	<tr>
		<td class="td-label"><label for="ss_smr_ceilingdry_standard">Ceiling Dry Standard:</label></td>
		<td>
			<div class="field-item text-field-item"><?php echo $data['ceilingdry']; ?></div>
		</td>
	</tr>
	<?php endif; ?>
</table>
<?php endif; ?>
<?php	
	$data = array();
	$data = ncn_admin_get_ss_smr_list($claim_id, $room_name);
?>
<?php if ($data): ?>
<div id="ss_smr_list">
<table class="ss-list-table">
<thead>
	<th class='ss-smr-date'>Date</th>
	<th class='ss-smr-wall'>Wall 1</th>
	<th class='ss-smr-wall'>Wall 2</th>
	<th class='ss-smr-wall'>Wall 3</th>
	<th class='ss-smr-wall'>Wall 4</th>
	<th class='ss-smr-fd-area'>Area 1</th>
	<th class='ss-smr-fd-area'>Area 2</th>
	<th class='ss-smr-fd-area'>Area 3</th>
	<th class='ss-smr-fd-area'>Area 4</th>
	<th class='ss-smr-fd-center'>Center</th>
	<th class='ss-smr-fd-area'>Area 1</th>
	<th class='ss-smr-fd-area'>Area 2</th>
	<th class='ss-smr-fd-area'>Area 3</th>
	<th class='ss-smr-fd-area'>Area 4</th>
	<th class='ss-smr-fd-center'>Center</th>
	<th class='ss-smr-operation'></th>
</thead>
<tbody>
<?php foreach ($data as $key=>$item): ?>
	<tr>
		<td class='ss-smr-date'><?php echo $item['date']; ?></td>
		<td class='ss-smr-wall'><?php echo $item['wall'][0]; ?></td>
		<td class='ss-smr-wall'><?php echo $item['wall'][1]; ?></td>
		<td class='ss-smr-wall'><?php echo $item['wall'][2]; ?></td>
		<td class='ss-smr-wall'><?php echo $item['wall'][3]; ?></td>
		<td class='ss-smr-fd-area'><?php echo $item['area_fd'][0]; ?></td>
		<td class='ss-smr-fd-area'><?php echo $item['area_fd'][1]; ?></td>
		<td class='ss-smr-fd-area'><?php echo $item['area_fd'][2]; ?></td>
		<td class='ss-smr-fd-area'><?php echo $item['area_fd'][3]; ?></td>
		<td class='ss-smr-fd-center'><?php echo $item['area_fd']['center']; ?></td>
		<td class='ss-smr-cd-area'><?php echo $item['area_cd'][0]; ?></td>
		<td class='ss-smr-cd-area'><?php echo $item['area_cd'][1]; ?></td>
		<td class='ss-smr-cd-area'><?php echo $item['area_cd'][2]; ?></td>
		<td class='ss-smr-cd-area'><?php echo $item['area_cd'][3]; ?></td>
		<td class='ss-smr-cd-center'><?php echo $item['area_cd']['center']; ?></td>
		<td class='ss-smr-operation'>	</td>
	</tr>
<?php endforeach; ?>
</tbody>
</table>
</div>
<?php endif; ?>
<?php
	$output = ob_get_contents();
	ob_end_clean();
	
	return $output;
}

function ncn_admin_ss_render_view_equipment_placement($claim_id, $room_name, $section) {
	$data = array();
	$data = ncn_admin_get_ss_ep_list($claim_id, $room_name);
	if (empty($data)) { return ''; }
	$action_panel_types = ncn_admin_ss_get_action_panel_type();
	ob_start();
?>
<div id="ss_ep_list">
<table class="ss-list-table">
	<thead>
		<th class="ss-ep-panel-type-td"></th>
		<th class="ss-ep-data-td"></th>
		<th class="ss-ep-operation"></th>
	</thead>
<tbody>
<?php foreach ($data as $key=>$item): ?>
	<tr>
		<td class="ss-ep-panel-type-td"><?php echo $action_panel_types[$item['ptype']].": " ?></td>
		<td class="ss-ep-data-td"><?php print ncn_admin_render_ss_ep_format_data($item); ?></td>
		<td class="ss-ep-operation"></td>
	</tr>
<?php endforeach; ?>
</tbody>
</table>
</div>
<?php
	$output = ob_get_contents();
	ob_end_clean();
	
	return $output;
}

function ncn_admin_ss_render_view_services($claim_id, $room_name, $section) {
	$data = array();
	$data = ncn_admin_get_ss_services($claim_id, $room_name);	
	ob_start();
?>
<?php if ($data): ?>
<div id="scopesheet_services_section">
<table id="ss_services_content" border="1">
	<thead>
		<th class="after-hours">After Hours</th>
		<th class="reg-hours">Reg Hours</th>
		<th>&nbsp;</th>
	</thead>
	<tbody>
		<tr><td colspan="3" class="ss-services-sub-section">Water Extraction</td></tr>
		<?php if (isset($data['we']['carpet']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['we']['carpet']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['we']['carpet']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Carpet: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><?php if($data['we']['carpet']['type']=='Regular') { echo "&#9745;"; } else { echo "&#9744;"; } ?> Regular</li>
					<li class="inline-field-item"><?php if($data['we']['carpet']['type']=='Heavy') { echo "&#9745;"; } else { echo "&#9744;"; } ?> Heavy</li>
				</ul>
				<?php if ($data['we']['carpet']['heavy']['sf']): ?><span class="field-item text-field-item small-size"> <?php echo "(".$data['we']['carpet']['heavy']['sf']."SF)"; ?> </span><?php endif; ?>
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['we']['hard_floor']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['we']['hard_floor']['time']=='after'){ echo "&#10003;"; } ?></td>
			<td class="reg-hours"><?php if ($data['we']['hard_floor']['time']=='reg'){ echo "&#10003;"; } ?></td>
			<td><div><label>Hard Floor: </label>
				<span class="field-item text-field-item small-size"> ( <?php echo $data['we']['hard_floor']['sf']; ?>SF) </span>
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['we']['weight_assisted']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['we']['weight_assisted']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['we']['weight_assisted']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Weight-Assisted: </label>
				<span class="field-item text-field-item small-size"> ( <?php echo $data['we']['weight_assisted']['sf']; ?>SF) </span>
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['we']['lift_float_carpet']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['we']['lift_float_carpet']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['we']['lift_float_carpet']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Lift/Float Carpet: </label>
				<span class="field-item text-field-item small-size"> ( <?php echo $data['we']['lift_float_carpet']['sf']; ?>SF) </span>
			</div></td>
		</tr>
		<?php endif; ?>
		
		<tr><td colspan="3" class="ss-services-sub-section">Material Removal</td></tr>
		<?php if (isset($data['mr']['carpet']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['mr']['carpet']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['mr']['carpet']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Carpet: </label> 
				( <span class="field-item text-field-item small-size"><?php echo $data['mr']['carpet']['sf']; ?></span> SF)
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['mr']['glued_carpet']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['mr']['glued_carpet']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['mr']['glued_carpet']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Glued Carpet: </label>
				( <span class="field-item text-field-item small-size"> <?php echo $data['mr']['glued_carpet']['sf']; ?></span> SF)
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['mr']['padding']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['mr']['padding']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['mr']['padding']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Padding: </label>
				( <span class="field-item text-field-item small-size"><?php echo $data['mr']['padding']['sf']; ?></span> SF)
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['mr']['vinyl']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['mr']['vinyl']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['mr']['vinyl']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Vinyl: </label>
				( <span class="field-item text-field-item small-size"><?php echo $data['mr']['vinyl']['sf']; ?></span> SF)
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['mr']['hardwood']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['mr']['hardwood']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['mr']['hardwood']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Hardwood: </label>
				( <span class="field-item text-field-item small-size"><?php echo $data['mr']['hardwood']['sf']; ?></span> SF)
			</div></td>
		</tr><?php endif; ?>		
		<?php if (isset($data['mr']['laminate']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['mr']['laminate']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['mr']['laminate']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Laminate: </label>
				( <span class="field-item text-field-item small-size"><?php echo $data['mr']['laminate']['sf']; ?></span> SF)
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['mr']['tile']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['mr']['tile']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['mr']['tile']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Tile: </label>
				( <span class="field-item text-field-item small-size"><?php echo $data['mr']['tile']['sf']; ?></span> SF)
			</div></td>
		</tr><?php endif; ?>		
		<?php if (isset($data['mr']['baseboard']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['mr']['baseboard']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['mr']['baseboard']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><label>Baseboard: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><?php if($data['mr']['baseboard']['type']=='Detach') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_mr_baseboard_type_detach">Detach</label></li>
					<li class="inline-field-item"><?php if($data['mr']['baseboard']['type']=='Detach & Dispose') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_mr_baseboard_type_dispose">Detach & Dispose</label></li>
				</ul>
				<span class="field-item text-field-item small-size"><?php if ($data['mr']['baseboard']['lf']) { echo "( ".$data['mr']['baseboard']['lf']." LF)";} ?></span> 
			</td>
		</tr><?php endif; ?>
		<?php if (isset($data['mr']['track_strip']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['mr']['track_strip']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['mr']['track_strip']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Track Strip: </label>
				( <span class="field-item text-field-item small-size"><?php echo $data['mr']['track_strip']['lf']; ?></span> LF)
			</div></td>
		</tr><?php endif; ?>		
		<?php if (isset($data['mr']['toe_kick']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['mr']['toe_kick']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['mr']['toe_kick']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Toe Kick: </label>
				( <span class="field-item text-field-item small-size"><?php echo $data['mr']['toe_kick']['lf']; ?></span> LF)
			</div></td>
		</tr><?php endif; ?>		
		<?php if (isset($data['mr']['door_casing']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['mr']['door_casing']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['mr']['door_casing']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Door casing: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><?php if($data['mr']['door_casing']['type']=='Detach') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_mr_door_casing_type_detach">Detach</label></li>
					<li class="inline-field-item"><?php if($data['mr']['door_casing']['type']=='Detach & Dispose') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_mr_door_casing_type_dispose">Detach & Dispose</label></li>
				</ul>
				<span class="field-item text-field-item small-size"><?php if ($data['mr']['door_casing']['lf']) { echo "( ".$data['mr']['door_casing']['lf']." LF)"; } ?></span>
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['mr']['drywall']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['mr']['drywall']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['mr']['drywall']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Drywall: </label>
				<span class="field-item text-field-item small-size">( <?php echo $data['mr']['drywall']['sf']; ?>SF) </span>
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['mr']['plaster']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['mr']['plaster']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['mr']['plaster']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Plaster: </label>
				<span class="field-item text-field-item small-size"> ( <?php echo $data['mr']['plaster']['sf']; ?>SF) </span>
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['mr']['insulation']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['mr']['insulation']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['mr']['insulation']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Insulation: </label>
				( <span class="field-item text-field-item small-size"><?php echo $data['mr']['insulation']['sf']; ?></span> SF)
			</div></td>
		</tr><?php endif; ?>		
		<?php if (isset($data['mr']['cabinets_vanity']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['mr']['cabinets_vanity']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['mr']['cabinets_vanity']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Cabinets or Vanity: </label>
				( <span class="field-item text-field-item small-size"><?php echo $data['mr']['cabinets_vanity']['lf']; ?></span> LF)
			</div></td>
		</tr><?php endif; ?>	
		<?php if (isset($data['mr']['countertop']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['mr']['countertop']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['mr']['countertop']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Countertop: </label>
				<?php if ($data['mr']['countertop']['type']=='Laminate') { echo "&#9745;"; } else { echo "&#9744;"; } ?>
				<label for="ss_service_mr_countertop_type_laminate">Laminate<span class="field-item text-field-item small-size"> <?php if ($data['mr']['countertop']['laminate']['lf']) { echo "(".$data['mr']['countertop']['laminate']['lf']."LF)"; } ?></span></label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><?php if($data['mr']['countertop']['type']=='Marble') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_mr_countertop_type_marble">Marble</label></li>
					<li class="inline-field-item"><?php if($data['mr']['countertop']['type']=='Granite') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_mr_countertop_type_granite">Granite</label></li>
				</ul>
				<span class="field-item text-field-item small-size"><?php if ($data['mr']['countertop']['marble_granite']['sf']) { echo "(".$data['mr']['countertop']['marble_granite']['sf']." SF)"; } ?></span>
			</div></td>
		</tr><?php endif; ?>	
		
		<tr><td colspan="3" class="ss-services-sub-section">Detach Only / Detach & Reset</td></tr>
		<?php if (isset($data['dodr']['singledoor']['time'])) : ?>
		<tr>
			<td class="after-hours"><?php if ($data['dodr']['singledoor']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['dodr']['singledoor']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Single Door: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><?php if($data['dodr']['singledoor']['type']=='Detach') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_dodr_singledoor_type_detach">Detach</label></li>
					<li class="inline-field-item"><?php if($data['dodr']['singledoor']['type']=='Detach & Reset') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_dodr_singledoor_type_dr">Detach & Reset</label></li>
				</ul>
				<?php if ($data['dodr']['singledoor']['count']) : ?>(#<span class="field-item text-field-item small-size"> <?php echo $data['dodr']['singledoor']['count']; ?></span>)<?php endif; ?>
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['dodr']['dbldoor']['time'])) : ?>
		<tr>
			<td class="after-hours"><?php if ($data['dodr']['dbldoor']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['dodr']['dbldoor']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Double Door: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><?php if($data['dodr']['dbldoor']['type']=='Detach') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_dodr_dbldoor_type_detach">Detach</label></li>
					<li class="inline-field-item"><?php if($data['dodr']['dbldoor']['type']=='Detach & Reset') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_dodr_dbldoor_type_dr">Detach & Reset</label></li>
				</ul>
				<?php if ($data['dodr']['dbldoor']['count']) : ?>(#<span class="field-item text-field-item small-size"><?php echo $data['dodr']['dbldoor']['count']; ?></span>)<?php endif; ?>
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['dodr']['bi_folddoor']['time'])) : ?>
		<tr>
			<td class="after-hours"><?php if ($data['dodr']['bi_folddoor']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['dodr']['bi_folddoor']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Bi-fold Door: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><?php if($data['dodr']['bi_folddoor']['type']=='Detach') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_dodr_bi_folddoor_type_detach">Detach</label></li>
					<li class="inline-field-item"><?php if($data['dodr']['bi_folddoor']['type']=='Detach & Reset') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_dodr_bi_folddoor_type_dr">Detach & Reset</label></li>
				</ul>
				<?php if ($data['dodr']['bi_folddoor']['count']) : ?>(#<span class="field-item text-field-item small-size"><?php echo $data['dodr']['bi_folddoor']['count']; ?> </span>)<?php endif; ?>
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['dodr']['dbl_bi_folddoor']['time'])) : ?>
		<tr>
			<td class="after-hours"><?php if ($data['dodr']['dbl_bi_folddoor']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['dodr']['dbl_bi_folddoor']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Double Bi-fold Door: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><?php if($data['dodr']['dbl_bi_folddoor']['type']=='Detach') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_dodr_dbl_bi_folddoor_type_detach">Detach</label></li>
					<li class="inline-field-item"><?php if($data['dodr']['dbl_bi_folddoor']['type']=='Detach & Reset') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_dodr_dbl_bi_folddoor_type_dr">Detach & Reset</label></li>
				</ul>
				<?php if ($data['dodr']['dbl_bi_folddoor']['count']) : ?>(#<span class="field-item text-field-item small-size"><?php echo $data['dodr']['dbl_bi_folddoor']['count']; ?></span>)<?php endif; ?>
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['dodr']['singlesink']['time'])) : ?>
		<tr>
			<td class="after-hours"><?php if ($data['dodr']['singlesink']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['dodr']['singlesink']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Single Sink: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><?php if($data['dodr']['singlesink']['type']=='Detach') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_dodr_singlesink_type_detach">Detach</label></li>
					<li class="inline-field-item"><?php if($data['dodr']['singlesink']['type']=='Detach & Reset') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_dodr_singlesink_type_dr">Detach & Reset</label></li>
				</ul>
				<?php if ($data['dodr']['singlesink']['count']) : ?>(#<span class="field-item text-field-item small-size"><?php echo $data['dodr']['singlesink']['count']; ?></span>)<?php endif; ?>
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['dodr']['dblsink']['time'])) : ?>
		<tr>
			<td class="after-hours"><?php if ($data['dodr']['dblsink']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['dodr']['dblsink']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Double Sink: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><?php if($data['dodr']['dblsink']['type']=='Detach') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_dodr_dblsink_type_detach">Detach</label></li>
					<li class="inline-field-item"><?php if($data['dodr']['dblsink']['type']=='Detach & Reset') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_dodr_dblsink_type_dr">Detach & Reset</label></li>
				</ul>
				<?php if ($data['dodr']['dblsink']['count']) : ?>(#<span class="field-item text-field-item small-size"> <?php echo $data['dodr']['dblsink']['count']; ?></span>)<?php endif; ?>
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['dodr']['stove']['time'])) : ?>
		<tr>
			<td class="after-hours"><?php if ($data['dodr']['stove']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['dodr']['stove']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div>
				<label>Stove
					<ul class="field-check-lists inline-group-fields clearfix">
						<li class="inline-field-item"><?php if($data['dodr']['stove']['stype']=='Gas') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_dodr_stove_stype_gas">Gas</label></li>
						<li class="inline-field-item"><?php if($data['dodr']['stove']['stype']=='Elec') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_dodr_stove_stype_elec">Elec</label></li>
					</ul>: 
				</label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><?php if($data['dodr']['stove']['type']=='Detach') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_dodr_stove_type_detach">Detach</label></li>
					<li class="inline-field-item"><?php if($data['dodr']['stove']['type']=='Detach & Reset') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_dodr_stove_type_dr">Detach & Reset</label></li>
				</ul>
				<?php if ($data['dodr']['stove']['count']) : ?>(#<span class="field-item text-field-item small-size"><?php echo $data['dodr']['stove']['count']; ?> </span>)<?php endif; ?>
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['dodr']['washer']['time'])) : ?>
		<tr>
			<td class="after-hours"><?php if ($data['dodr']['washer']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['dodr']['washer']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div>
				<label>Washer: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><?php if($data['dodr']['washer']['type']=='Detach') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_dodr_washer_type_detach">Detach</label></li>
					<li class="inline-field-item"><?php if($data['dodr']['washer']['type']=='Detach & Reset') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_dodr_washer_type_dr">Detach & Reset</label></li>
				</ul>
				<?php if ($data['dodr']['washer']['count']) : ?>(#<span class="field-item text-field-item small-size"><?php echo $data['dodr']['washer']['count']; ?><span>)<?php endif; ?>
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['dodr']['dryer']['time'])) : ?>
		<tr>
			<td class="after-hours"><?php if ($data['dodr']['dryer']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['dodr']['dryer']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div>
				<label>Dryer: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><?php if($data['dodr']['dryer']['type']=='Detach') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_dodr_dryer_type_detach">Detach</label></li>
					<li class="inline-field-item"><?php if($data['dodr']['dryer']['type']=='Detach & Reset') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_dodr_dryer_type_dr">Detach & Reset</label></li>
				</ul>
				<?php if ($data['dodr']['dryer']['count']) : ?>(#<span class="field-item text-field-item small-size"> <?php echo $data['dodr']['dryer']['count']; ?> </span>)<?php endif; ?>
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['dodr']['dishwasher']['time'])) : ?>
		<tr>
			<td class="after-hours"><?php if ($data['dodr']['dishwasher']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['dodr']['dishwasher']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div>
				<label>Dishwasher: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><?php if($data['dodr']['dishwasher']['type']=='Detach') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_dodr_dishwasher_type_detach">Detach</label></li>
					<li class="inline-field-item"><?php if($data['dodr']['dishwasher']['type']=='Detach & Reset') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_dodr_dishwasher_type_dr">Detach & Reset</label></li>
				</ul>
				<?php if ($data['dodr']['dishwasher']['count']) : ?>(#<span class="field-item text-field-item small-size"> <?php echo $data['dodr']['dishwasher']['count']; ?> </span>)<?php endif; ?>
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['dodr']['refrigerator']['time'])) : ?>
		<tr>
			<td class="after-hours"><?php if ($data['dodr']['refrigerator']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['dodr']['refrigerator']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div>
				<label>Refrigerator: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><?php if($data['dodr']['refrigerator']['type']=='Detach') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_dodr_refrigerator_type_detach">Detach</label></li>
					<li class="inline-field-item"><?php if($data['dodr']['refrigerator']['type']=='Detach & Reset') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_dodr_refrigerator_type_dr">Detach & Reset</label></li>
				</ul>
				<?php if ($data['dodr']['refrigerator']['count']) : ?>(#<span class="field-item text-field-item small-size"><?php echo $data['dodr']['refrigerator']['count']; ?> </span>)<?php endif; ?>
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['dodr']['toilet']['time'])) : ?>
		<tr>
			<td class="after-hours"><?php if ($data['dodr']['toilet']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['dodr']['toilet']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div>
				<label>Toilet: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><?php if($data['dodr']['toilet']['type']=='Detach') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_dodr_toilet_type_detach">Detach</label></li>
					<li class="inline-field-item"><?php if($data['dodr']['toilet']['type']=='Detach & Reset') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_dodr_toilet_type_dr">Detach & Reset</label></li>
				</ul>
				<?php if ($data['dodr']['toilet']['count']) : ?>(#<span class="field-item text-field-item small-size"> <?php echo $data['dodr']['toilet']['count']; ?> </span>)<?php endif; ?>
			</div></td>
		</tr><?php endif; ?>
		
		<tr><td colspan="3" class="ss-services-sub-section">Cleaning Services</td></tr>
		<?php if (isset($data['cls']['clean_carpet']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['cls']['clean_carpet']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['cls']['clean_carpet']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Clean Carpet: </label>
				(<span class="field-item text-field-item small-size"><?php echo $data['cls']['clean_carpet']['sf']; ?></span> SF)
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['cls']['clean_tile']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['cls']['clean_tile']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['cls']['clean_tile']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Clean Tile: </label>
				(<span class="field-item text-field-item small-size"><?php echo $data['cls']['clean_tile']['sf']; ?></span> SF)
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['cls']['clean_otherfloor']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['cls']['clean_otherfloor']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['cls']['clean_otherfloor']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Clean Other Floor: </label>
				(<span class="field-item text-field-item small-size"><?php echo $data['cls']['clean_otherfloor']['sf']; ?></span> SF)
			</div></td>
		</tr><?php endif; ?>
		
		<tr><td colspan="3" class="ss-services-sub-section">Carpet Services</td></tr>
		<?php if (isset($data['cs']['detach_relay_carpet']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['cs']['detach_relay_carpet']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['cs']['detach_relay_carpet']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Detach & Relay Carpet: </label>
				(<span class="field-item text-field-item small-size"><?php echo $data['cs']['detach_relay_carpet']['sf']; ?></span> SF)
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['cs']['reseam_carpet']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['cs']['reseam_carpet']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['cs']['reseam_carpet']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Re-Seam Carpet: </label>
				(<span class="field-item text-field-item small-size"> <?php echo $data['cs']['reseam_carpet']['lf']; ?> </span> LF)
			</div></td>
		</tr>
		<?php endif; ?>
		<?php if (isset($data['cs']['apply_carpet_protector']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['cs']['apply_carpet_protector']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['cs']['apply_carpet_protector']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Apply Carpet Protector: </label>
				(<span class="field-item text-field-item small-size"><?php echo $data['cs']['apply_carpet_protector']['sf']; ?></span> SF)
			</div></td>
		</tr>
		<?php endif; ?>
		<?php if (isset($data['cs']['install_pad']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['cs']['install_pad']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['cs']['install_pad']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Install Pad: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><?php if($data['cs']['install_pad']['grade']=='Standard') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_cs_install_pad_grade_standard">Standard</label></li>
					<li class="inline-field-item"><?php if($data['cs']['install_pad']['grade']=='Average') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_cs_install_pad_grade_average">Average</label></li>
					<li class="inline-field-item"><?php if($data['cs']['install_pad']['grade']=='High') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_cs_install_pad_grade_high">High</label></li>
					<li class="inline-field-item"><?php if($data['cs']['install_pad']['grade']=='Premium') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_cs_install_pad_grade_premium">Premium</label></li>
				</ul>
				<?php if ($data['cs']['install_pad']['sf']): ?>(<span class="field-item text-field-item small-size"> <?php echo $data['cs']['install_pad']['sf']; ?>SF</span>)<?php endif; ?>
			</div></td>
		</tr>
		<?php endif; ?>
		
		<tr><td colspan="3" class="ss-services-sub-section">Anti Microbial</td></tr>
		<?php if (isset($data['am']['plant']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['am']['plant']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['am']['plant']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label></label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><?php if($data['am']['plant']['type']=='Non-Plant Based') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_am_plant_type_non_based">Non-Plant Based</label></li>
					<li class="inline-field-item"><?php if($data['am']['plant']['type']=='Plant Based') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_am_plant_type_based">Plant Based</label></li>
				</ul>
				<?php if ($data['am']['plant']['floors_sf']): ?><label>Floors(<span class="field-item text-field-item small-size"> <?php echo $data['am']['plant']['floors_sf']; ?> </span> SF)</label><?php endif; ?>
				<?php if ($data['am']['plant']['walls_sf']): ?><label>Walls(<span class="field-item text-field-item small-size"> <?php echo $data['am']['plant']['walls_sf']; ?> </span> SF)</label><?php endif; ?>
				<?php if ($data['am']['plant']['ceiling_sf']): ?><label>Ceiling(<span class="field-item text-field-item small-size"> <?php echo $data['am']['plant']['ceiling_sf']; ?> </span> SF)</label><?php endif; ?>
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['am']['thermal_fog']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['am']['thermal_fog']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['am']['thermal_fog']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Thermal Fog: </label>
				(<span class="field-item text-field-item small-size"> <?php echo $data['am']['thermal_fog']['cubic_ft']; ?> </span> Cubic Ft)
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['am']['ozone']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['am']['ozone']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['am']['ozone']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Ozone: </label>
				(<span class="field-item text-field-item small-size"> <?php echo $data['am']['ozone']['cubic_ft']; ?> </span> Cubic Ft)
			</div></td>
		</tr><?php endif; ?>
		
		<tr><td colspan="3" class="ss-services-sub-section">Content Manipulation</td></tr>
		<?php if (isset($data['cm']['acm']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['cm']['acm']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['cm']['acm']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Amount of contents moved: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><?php if($data['cm']['acm']['amount']=='SML') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_cm_acm_amount_sml">SML</label></li>
					<li class="inline-field-item"><?php if($data['cm']['acm']['amount']=='AVG') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_cm_acm_amount_avg">AVG</label></li>
					<li class="inline-field-item"><?php if($data['cm']['acm']['amount']=='LRG') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_cm_acm_amount_lrg">LRG</label></li>
					<li class="inline-field-item"><?php if($data['cm']['acm']['amount']=='XL' ) { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_cm_acm_amount_xl" >XL</label> </li>
					<li class="inline-field-item"><?php if($data['cm']['acm']['amount']=='Hours' ) { echo "&#9745;"; } else { echo "&#9744;"; } ?> 
						<?php if($data['cm']['acm']['hours']['val']): ?><label for="ss_service_cm_acm_amount_hours" >Hours (#<?php echo $data['cm']['acm']['hours']['val']; ?>)</label><?php endif; ?> </li>
				</ul>
			</div></td>
		</tr><?php endif; ?>
		<?php if (isset($data['cm']['block_pad']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['cm']['block_pad']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['cm']['block_pad']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Block and Pad: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><?php if($data['cm']['block_pad']['amount']=='SML') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_cm_block_pad_amount_sml">SML</label></li>
					<li class="inline-field-item"><?php if($data['cm']['block_pad']['amount']=='AVG') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_cm_block_pad_amount_avg">AVG</label></li>
					<li class="inline-field-item"><?php if($data['cm']['block_pad']['amount']=='LRG') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_cm_block_pad_amount_lrg">LRG</label></li>
				</ul>
			</div></td>
		</tr><?php endif; ?>
		
		<tr><td colspan="3" class="ss-services-sub-section">Miscellaneous</td></tr>
		<?php if(isset($data['misc']['build_containment']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['misc']['build_containment']['time']=='after'){ echo "&#10003;";} ?> </td>
			<td class="reg-hours"><?php if ($data['misc']['build_containment']['time']=='reg'){ echo "&#10003;";} ?> </td>
			<td><div><label>Build Containment: </label>
				<span class="field-item text-field-item small-size"> (SF covered <?php echo $data['misc']['build_containment']['sf']; ?>) </span>
			</div></td>
		</tr><?php endif; ?>
		<?php if(isset($data['misc']['protect']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['misc']['protect']['time']=='after'){ echo "&#10003;";} ?> </td>
			<td class="reg-hours"><?php if ($data['misc']['protect']['time']=='reg'){ echo "&#10003;";} ?> </td>
			<td><div><label>Protect(w/6mil plastic): </label>
				<span class="field-item text-field-item small-size"> <?php echo $data['misc']['protect']['L']; ?>(L) </span>X
				<span class="field-item text-field-item small-size"> <?php echo $data['misc']['protect']['W']; ?>(W) </span>
			</div></td>
		</tr><?php endif; ?>
		<?php if(isset($data['misc']['hepa_vacuum']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['misc']['hepa_vacuum']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['misc']['hepa_vacuum']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>HEPA Vacuum: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><?php if($data['misc']['hepa_vacuum']['type']=='SF') { echo "&#9745;"; } else { echo "&#9744;"; } ?> 
						<label for="ss_service_misc_hepa_vacuum_type_sf"><span class="field-item text-field-item small-size"> (<?php echo $data['misc']['hepa_vacuum']['sf']; ?>SF) </span></label></li>
					<li class="inline-field-item"><?php if($data['misc']['hepa_vacuum']['type']=='Hours') { echo "&#9745;"; } else { echo "&#9744;"; } ?> 
						<label for="ss_service_misc_hepa_vacuum_type_hours">Hours <span class="field-item text-field-item small-size"> (#<?php echo $data['misc']['hepa_vacuum']['hours']; ?>) </span></label></li>
				</ul>
			</div></td>
		</tr><?php endif; ?>
		<?php if(isset($data['misc']['drill_holes']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['misc']['drill_holes']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['misc']['drill_holes']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Drill Holes: </label>
				(#<span class="field-item text-field-item small-size"> <?php echo $data['misc']['drill_holes']['count']; ?></span>)
			</div></td>
		</tr><?php endif; ?>
		<?php if(isset($data['misc']['pump_water']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['misc']['pump_water']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['misc']['pump_water']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Pump Water: </label>
				(<span class="field-item text-field-item small-size"> <?php echo $data['misc']['pump_water']['gals']; ?>GALS</span>)
			</div></td>
		</tr><?php endif; ?>
		<?php if(isset($data['misc']['zippers']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['misc']['zippers']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['misc']['zippers']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Zippers: </label>
				(<span class="field-item text-field-item small-size"> <?php echo $data['misc']['zippers']['qty']; ?>QTY</span>)
			</div></td>
		</tr><?php endif; ?>
		<?php if(isset($data['misc']['zip_poles']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['misc']['zip_poles']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['misc']['zip_poles']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Zip Poles: </label>
				(#<span class="field-item text-field-item small-size"> <?php echo $data['misc']['zip_poles']['count']; ?></span>)
			</div></td>
		</tr><?php endif; ?>
		<?php if(isset($data['misc']['decon_equipment']['time'])): ?>
		<tr>
			<td class="after-hours"><?php if ($data['misc']['decon_equipment']['time']=='after'){ echo "&#10003;";} ?></td>
			<td class="reg-hours"><?php if ($data['misc']['decon_equipment']['time']=='reg'){ echo "&#10003;";} ?></td>
			<td><div><label>Decontaminate Equipment: </label>
				<ul class="field-check-lists inline-group-fields clearfix">
					<li class="inline-field-item"><?php if($data['misc']['decon_equipment']['size']=='Regular') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_misc_decon_equipment_size_regular">Regular</label></li>
					<li class="inline-field-item"><?php if($data['misc']['decon_equipment']['size']=='Heavy') { echo "&#9745;"; } else { echo "&#9744;"; } ?> <label for="ss_service_misc_decon_equipment_size_heavy">Heavy</label></li>
				</ul>
				<?php if($data['misc']['decon_equipment']['heavy']['amount']): ?>(#<?php echo $data['misc']['decon_equipment']['heavy']['amount']; ?>)<?php endif; ?>
			</div></td>
		</tr><?php endif; ?>
		
	</tbody>
</table>
</div>
<?php endif; ?>
<?php
	$output = ob_get_contents();
	ob_end_clean();
	
	return $output;
}

function ncn_admin_ss_render_view_comments($claim_id, $room_name, $section) {
	$data = ncn_admin_get_ss_comments($claim_id, $room_name);
	if (empty($data)) { return ''; }
	ob_start();
?>
<table>
	<tr>
		<td class="td-label"><label for="ss_comments_comments">Comments:</label></td>
		<td><div class="field-item text-field-item"><?php echo $data['content']; ?></div></td>
	</tr>
</table>
<?php
	$output = ob_get_contents();
	ob_end_clean();
	
	return $output;
}


/**
 * Make Scope Image using Scopesheet Data
 */
function ncn_admin_ss_make_scopesheet_image($claim_id, $roomname) {
	$filepath = ncn_admin_ss_make_scopesheet_pdf($claim_id, $roomname);
	
	$claim_dir = "sites/default/files/invoices/$claim_id/";
	
	if (!is_dir($claim_dir)) {
		mkdir($claim_dir, 0755);
	}
}

function ncn_admin_ss_make_scopesheet_pdf($claim_id, $room_name) {
	$data = array();
	$ss_eit = ncn_admin_get_ss_eit_list($claim_id, $room_name);
	$ss_smr = ncn_admin_get_ss_smr_list($claim_id, $room_name); 
	$ss_ep  = ncn_admin_get_ss_ep_list($claim_id, $room_name);
	
	if (!empty($ss_eit)) { $data['eit'] = $ss_eit; }
	if (!empty($ss_smr)) { $data['smr'] = $ss_smr; }
	if (!empty($ss_ep )) { $data['ep']  = $ss_ep;  }
	
	if (empty($data)) {
		$msg = t("There isn't no scopesheet data for this room (!room_name)", array('!room_name'=>$room_name));
		drupal_set_message($msg, 'error');
		return FALSE;
	}
	
	$claim_dir = "sites/default/files/invoices/$claim_id/";
	if (!is_dir($claim_dir)) {
		mkdir($claim_dir, 0755);
	}
	
	$pdf=new NCNPDF();
	$pdf->AddPage();

	$pdf->SetFont('Arial', '', 9);	
	
	/*** Exterior & Interior Temperatures ***/
	if ($data['eit']) {
		$pdf->SetWidths(array(25, 40, 40, 35, 35));
		$pdf->Row( array('Date', 'Exterior Temp / RH%', 'Interior Temp / RH%', 'GPP', 'Time of Readings') );
		foreach ($data['eit'] as $key=>$item) {
			$pdf->Row(	array(
					$item['date'],
					$item['ext_temp']."/".$item['ext_rh'], 
					$item['int_temp']."/".$item['int_rh'], 
					$item['gpp'], 
					ncn_admin_format_american_time($item['time_readings']).' '.$item['time_readings_half'], 
			));
		}
		
		$pdf->Ln(6);
	}
	
	/*** Structural Moisture Readings ***/
	if ($data['smr']) {
		$pdf->SetWidths(array(25, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12));
		$pdf->Row( array('Date', 'Wall 1', 'Wall 2', 'Wall 3', 'Wall 4', 'Area 1', 'Area 2', 'Area 3', 'Area 4', 'Center', 'Area 1', 'Area 2', 'Area 3', 'Area 4', 'Center') );
		foreach ($data['smr'] as $key=>$item) {
			$pdf->Row(	array(
				$item['date'], 
				$item['wall'][0], 		$item['wall'][1], 		$item['wall'][2], 		$item['wall'][3], 
				$item['area_fd'][0], 	$item['area_fd'][1], 	$item['area_fd'][2], 	$item['area_fd'][3], 	$item['area_fd']['center'], 
				$item['area_cd'][0], 	$item['area_cd'][1], 	$item['area_cd'][2], 	$item['area_cd'][3], 	$item['area_cd']['center'], 
			));
		}
		$pdf->Ln(6);
	}
	
	if ($data['ep']) {
		$action_panel_types = ncn_admin_ss_get_action_panel_type();
		
		$pdf->SetWidths(array(40, 120));
		foreach ($data['ep'] as $key=>$item) {
			$pdf->Row(	array(
					$action_panel_types[$item['ptype']], 
					strip_tags( str_replace("&nbsp;", " ", ncn_admin_render_ss_ep_format_data($item)) ), 
			));
		}
	}
	
	$pdfpath = $claim_dir."ss_data".time().".pdf";
			
	$pdf->Output($pdfpath, 'F');
	if(is_file($pdfpath)) {
		drupal_set_message( t("Made PDF File, Successfully. File name : !file_name", array('!file_name'=>basename($pdfpath))) );
		return $pdfpath;
	}
	
}

function ncn_admin_ss_ep_print_format_data($data) {
	$action_panel_types = ncn_admin_ss_get_action_panel_type();
	
	$f_data = array();
	foreach ($data as $key=>$item) {
		$f_data[] = array(
			'ptype' => $action_panel_types[$item['ptype']], 
			'data' =>  strip_tags( str_replace("&nbsp;", " ", ncn_admin_render_ss_ep_format_data($item, true)) ), 
		);
	}
	
	return $f_data;
}

/**
 * ACI Render - "Daily Claim Log" Section
 */ 
function ncn_admin_view_aci_render_daily_claim_log($claim_id, $section) {  
  ob_start();
?>
<div id="dcl_log_list">
  <?php print ncn_admin_view_aci_render_dcl_log_list($claim_id); ?>
</div>
<?php
  $output = ob_get_contents();
  ob_end_clean();
  
  return $output;
}

function ncn_admin_view_aci_render_dcl_log_list($claim_id) {
  $data = ncn_admin_get_aci_daily_claim_log($claim_id);
  
  ob_start();
?>
<table>
<thead>
  <th class='dcl-day-week'>Day of Week</th>
  <th class='dcl-date'>Date</th>
  <th class='dcl-men'># Men</th>
  <th class='dcl-hours-day'>Hours Per Day</th>
  <th class='dcl-total-hours'>Total Hours</th>
  <th class='dcl-after-hours'>After Hours</th>
  <th class='dcl-operation'></th>
</thead>
<tbody>
<?php foreach ($data as $key=>$item): ?>
  <tr>
    <td class='dcl-day-week'><?php echo $item['day_week']; ?></td>
    <td class='dcl-date'><?php echo $item['date']; ?></td>
    <td class='dcl-men'><?php echo $item['men']; ?> </td>
    <td class='dcl-hours-day'><?php echo $item['hours_day']; ?> Hours</td>
    <td class='dcl-total-hours'><?php $t_hours = $item['men'] * $item['hours_day']; if ($t_hours) { echo $t_hours." Hours"; } ?></td>
    <td class='dcl-after-hours'><?php echo $item['after_hours']; ?></td>
  </tr>
<?php endforeach; ?>
</tbody>
</table>
<?php
  $output = ob_get_contents();
  ob_end_clean();
  
  return $output;
}
?>
