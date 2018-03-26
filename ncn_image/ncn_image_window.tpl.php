<!-- <PRE>
<?php //print_r($_SESSION['ncn_img_tmp']); ?>
</PRE> -->
<fieldset>
<div id="ncn_image_box" class="clear-block">
	<div class="header"><strong>Image Path:</strong> &nbsp;<?php echo $img_url; ?></div>
	<input type="hidden" value="<?php echo $img_url; ?>" id="ncn_image_file_path_orignal" />
	<input type="hidden" value="<?php echo $_SESSION['ncn_img_tmp']['fullpath']; ?>" id="ncn_image_file_path_crop" />
	<input type="hidden" value="<?php echo $_SESSION['ncn_img_tmp']['info']['width']; ?>" id="ncn_image_file_width_orignal" />
	<input type="hidden" value="<?php echo $_SESSION['ncn_img_tmp']['info']['height']; ?>" id="ncn_image_file_height_orignal" />
	
	<?php if ($img_param['exist'] == TRUE): ?>
	<div class="left">
		<div id="scr_img" class="image_screen"><div id="image_wrapper"><img src="<?php echo base_path().$_SESSION['ncn_img_tmp']['fullpath']; ?>" id="cropbox" alt="No Image"/></div></div>
	</div>
	<div class="right">
		<div class="operation-tab clear-block" >
			<label for="img_operation" class="img_label">Operation:</label> 
			<div class="img_input">
				<select id="img_operation">
					<option value="update_image">Update Image</option>
					<option value="swap_image">Swap Image</option>
					<option value="change_image">Change&Remove Image</option>
				</select>
			</div>
		</div>
		
		<div id="change_image" class="img_operation">
			<form id="image_change_form" method="POST">
			<input type="hidden" name="claim_id" id="claim_id" value="<?php echo $img_param['claim_id']; ?>" />
			<input type="hidden" name="room_name" id="room_name" value="<?php echo $img_param['room_name']; ?>" />
			<input type="hidden" name="position" id="position" value="<?php echo $img_param['position']; ?>" />
			<input type="hidden" name="ncn_image_path" id="ncn_image_path" value="<?php echo $img_param['image_path']; ?>" />
			
			<div>
				<label for="ncn_photo_file" class="img_label">Change Image:</label> 
				<div class="img_input">
					<input type="file" name="files[ncn_photo]" id="ncn_photo_file" accept="image/jpg, image/jpeg, image/gif, image/png"/> <br/>
					<input type="button" value="Upload" style="width:auto; margin-top: 10px;" id="btn_image_change_upload" onclick="on_click_ncn_change_image();"/>
				</div>
				<p><strong>OR</strong></p>
				<div class="img_input">
					<input type="button" value="Remove Image" style="width:auto;" onclick="on_click_ncn_remove_image()"/>
				</div>
			</div>
			<div class="submit-panel">
				<div class="save-panel clear-block">
					<input type="button" value="Save" id="image_change_save" />
					<input type="button" value="Save and Close" id="image_change_save_close" />
				</div>
			</div>
			</form>
		</div>
		
		<div id="swap_image" class="img_operation">
			<form id="image_swap_form">
			<input type="hidden" name="claim_id" id="claim_id" value="<?php echo $img_param['claim_id']; ?>" />
			<input type="hidden" name="room_name" id="room_name" value="<?php echo $img_param['room_name']; ?>" />
			<input type="hidden" name="position" id="position" value="<?php echo $img_param['position']; ?>" />
			
			<label for="img_roomname" class="img_label">Room Name:</label> 
			<div class="img_input">
			<?php echo ncn_image_window_theme_roomname($img_param['claim_id']); ?>
			</div>
			
			<label for="img_position" class="img_label">Position:</label> 
			<div id="img_position_wrapper" class="img_input">
				<!-- placeholder -->
			</div>
			
			<div class="submit-panel">
				<div class="save-panel clear-block">
					<input type="button" value="Save" id="image_swap_save" />
					<input type="button" value="Save and Close" id="image_swap_save_close" />
				</div>
			</div>
			</form>
		</div>
		<div id="update_image" class="img_operation">
			<div class="clear-block image_info" >
				<label for="img_action" class="img_label">Size:</label> 
				<span class="image_info_size"><?php echo $_SESSION['ncn_img_tmp']['info']['width'] . " x " . $_SESSION['ncn_img_tmp']['info']['height']; ?></span>
			</div>
			<div class="action-block clear-block" >
				<label for="img_action" class="img_label">Action:</label> 
				<div class="img_input">
					<select id="img_action">
						<option value="img_crop">Crop</option>
						<option value="img_rotate">Rotate</option>
					</select>
				</div>
			</div>
			
			<form id="image_action_form">
			<div class="params">
				<div id="img_crop" class="param-box clear-block">
					<div class="param clear-block">
						<label for="crop_width" class="img_label">Width:</label>
						<div class="img_input"><input type="text" id="crop_width" name="crop_width" value="" readonly/></div>
					</div>
					<div class="param clear-block">
						<label for="crop_height" class="img_label">Height:</label>
						<div class="img_input"><input "text" id="crop_height" name="crop_height" value="" readonly/></div>
					</div>
					<div class="param clear-block">
						<label for="crop_xoffset" class="img_label">X-Offset:</label>
						<div class="img_input"><input "text" id="crop_xoffset" name="crop_xoffset" value="" readonly/></div>
					</div>
					<div class="param clear-block">
						<label for="crop_yoffset" class="img_label">Y-Offset:</label>
						<div class="img_input"><input "text" id="crop_yoffset" name="crop_yoffset" value="" readonly/></div>
					</div>			
				</div>
				<div id="img_rotate" class="param-box clear-block">
					<div class="param clear-block">
						<label for="rotate_angle" class="img_label">Rotation Angle:</label>
						<div class="img_input">
							<select id="rotate_angle" name="rotate_angle">
								<option value="90">90&deg;</option>
								<option value="180">180&deg;</option>
								<option value="270">270&deg;</option>
							</select>
						</div>
					</div>			
				</div>	
			</div>
			
			<div class="submit-panel">
				<input type="button" value="Submit" id="image_action_submit"/>
				<div class="save-panel clear-block">
					<input type="button" value="Save" id="image_action_save" />
					<input type="button" value="Save and Close" id="image_action_save_close" />
				</div>
			</div>
			</form>
		</div>	<!-- Update Image -->
	</div>
	<?php endif; ?>
</div>
</fieldset>
