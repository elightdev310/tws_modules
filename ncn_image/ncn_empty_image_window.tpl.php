<fieldset>
<div id="ncn_image_box" class="clear-block">
	<div class="left">
		<div id="scr_img" class="image_screen"><div id="image_wrapper"><img src="" id="cropbox" alt="No Image"/></div></div>
	</div>
	<div class="right">		
		<div id="change_image">
			<form id="image_change_form" method="POST">
			<input type="hidden" name="claim_id" id="claim_id" value="<?php echo $img_param['claim_id']; ?>" />
			<input type="hidden" name="room_name" id="room_name" value="<?php echo $img_param['room_name']; ?>" />
			<input type="hidden" name="position" id="position" value="<?php echo $img_param['position']; ?>" />
			<input type="hidden" name="ncn_image_path" id="ncn_image_path" value="" />
			
			<div>
				<label for="ncn_photo_file" class="img_label">Upload Image:</label> 
				<div class="img_input">
					<input type="file" name="files[ncn_photo]" id="ncn_photo_file" accept="image/jpg, image/jpeg, image/gif, image/png"/> <br/>
					<input type="button" value="Upload" style="width:auto; margin-top: 10px;" id="btn_image_change_upload" onclick="on_click_ncn_change_image();"/>
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
	</div>
</div>
</fieldset>