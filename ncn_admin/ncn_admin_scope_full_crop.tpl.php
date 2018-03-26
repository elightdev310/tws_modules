<fieldset>
<div id="ncn_image_box" class="clear-block">
	<div>
		<div><div style="overflow: scroll; height: 500px;"><img src="<?php echo base_path().$img_path; ?>" id="cropbox" alt="No Image"/></div></div>
	</div>
	<div>
		<div>
			<form id="viewclaim_scopefile_crop_form" method="POST">
			<input type="hidden" name="tfunction" id="scope_crop_tfunction" value="viewclaim_scopefile_crop" />
			<input type="hidden" name="claim_id" id="claim_id" value="<?php echo $claim_id; ?>" />
			<input type="hidden" name="room_name" id="room_name" value="<?php echo $room_name ?>" />
			<input type="hidden" value="<?php echo $img_path; ?>" id="image_file_path_orignal" />
			<div>
				<div id="img_crop" class="param-box clear-block">
					<div class="param clear-block">
						<label for="crop_width" class="img_label">Width:</label>
						<div class="img_input"><input type="text" id="crop_width" name="crop_width" value="" size="20" readonly/></div>
					</div>
					<div class="param clear-block">
						<label for="crop_height" class="img_label">Height:</label>
						<div class="img_input"><input type="text" id="crop_height" name="crop_height" value="" size="20" readonly/></div>
					</div>
					<div class="param clear-block">
						<label for="crop_xoffset" class="img_label">X-Offset:</label>
						<div class="img_input"><input type="text" id="crop_xoffset" name="crop_xoffset" value="" size="20" readonly/></div>
					</div>
					<div class="param clear-block">
						<label for="crop_yoffset" class="img_label">Y-Offset:</label>
						<div class="img_input"><input type="text" id="crop_yoffset" name="crop_yoffset" value="" size="20" readonly/></div>
					</div>			
				</div>
			</div>
			
			<div class="submit-panel">
				<div class="save-panel clear-block">
					<input type="submit" value="Save" />
					<input type="button" value="Cancel" onclick="parent.$.colorbox.close(1);" />
				</div>
			</div>
			</form>
		</div>
	</div>
</form>
</div>
</fieldset>
<script language="Javascript">

	// Remember to invoke within jQuery(window).load(...)
	// If you don't, Jcrop may not initialize properly
	jQuery(window).load(function(){
	//jQuery(document).ready(function(){
	
		jQuery('#cropbox').Jcrop({
			onChange: showCoords,
			onSelect: showCoords
		});

	});

	// Our simple event handler, called from onChange and onSelect
	// event handlers, as per the Jcrop invocation above
	function showCoords(c)
	{
		jQuery('#crop_xoffset').val(c.x);
		jQuery('#crop_yoffset').val(c.y);
		//jQuery('#x2').val(c.x2);
		//jQuery('#y2').val(c.y2);
		jQuery('#crop_width').val(c.w);
		jQuery('#crop_height').val(c.h);
	};

</script>
