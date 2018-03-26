<div class="mail_wrapper" style="color: #222;">
<div style="width: 600px; background-color: #a3c1de; font-family : Arial; font-size: 12px; margin: 5px auto;">
	<div style="width: 580px; margin: 0px auto; padding-top: 10px; padding-bottom: 20px;">
		<div style="color: #1d5ca8; font-style: italic;	width: 100%; margin: 5px 0px; overflow: hidden;">
			<div style="float: left;"><b>Net Claims Now</b> - Email</div>
			<div style="float: right">Contact us today! <b>wecare@netclaimsnow.com - 877.654.8668</b></div>
		</div>
		<div style="width: 100%; background-color: white;">
			<div style="width: 540px; margin: 0px auto; padding: 10px 0px;">
				<div style="width: 100%; margin:15px 0px; overflow: hidden;">
					<img src="<?php echo $img_basepath."logo.png";?>" alt="logo"/>
					<!-- <a style="float: right;	width: 210px; height: 41px;	margin-top: 5px;" href="<?php echo $base_url."/user"; ?>">
						<img src="<?php echo $img_basepath."member_login.jpg";?>" alt="Activate Your Membership!"/>
					</a> -->
				</div>
				<div style="font-size: 24px; color: #1d5ca8; margin:15px 0px;">
					<b><span style="font-size: 28px; color: #86b536">Thank You</span> for activating your membership.</b>
				</div>
				<div style="margin:15px 0px;" >
					<b><?php echo $params['!name']; ?>,</b>
				</div>
				<div style="margin:15px 0px;" >
          <?php 
            $guidebook = ncn_admin_create_user_get_doc_info_by_hint($params['!docs'], "guidebook");
            $training_doc = ncn_admin_create_user_get_doc_info_by_hint($params['!docs'], "sample");
          ?>
					<b>Below in in this email are the training documents.</b> Please print out and read through your <a href='<?php echo "$base_url/account/serve_pdf/".base64_encode($guidebook['filepath'])."/".base64_encode('Guidebook'); ?>' style="color: #1d5ca8;">Guidebook</a> before your training session. You may also print out the <a href='<?php echo "$base_url/account/serve_pdf/".base64_encode($training_doc['filepath'])."/".base64_encode('Training Claim Document'); ?>' style="color: #1d5ca8;">Training Claim Documents</a> for reference. Please contact your Account Manager within 24 hours of your activation to schedule your training at <span style="color: #1d5ca8;">877.654.8668.</span>
				</div>
				<div style="margin:15px 0px;" >
					Your Account Manager will be training on how to use the Virtual Office and most importantly how to fill out claim documents such as scope sheets. It would be best to have all project managers in training as well so they will have an understanding of how to <b>BEST DOCUMENT</b> each claim.
				</div>
				<div style="margin:15px 0px;" >
					After completion of training, we will send you your username and password so your business can start sending in claims through your company's personalized Virtual Office Suite. You will be able to download and print your company's branded claim documents within your Virtual Office Suite.
				</div>
				
				<div style="margin:15px 0px; font-size: 9px; font-style: italic; color: #1d5ca8;" >
					If you have the need to reschedule your training session, please give us 24 hour notice and contact us to reschedule.
				</div>
				<div style="margin:20px 0px;" >
					Sincerely,</br>
					<b>Your Net Claims Now Team</b>
				</div>
			</div>
		</div>
		
		<div style="width: 100%; background-color: white; margin-top:10px;">
			<div style="width: 540px; margin: 0px auto; padding: 10px 0px;">
				<div style="margin:15px 0px;" >
					<b>Documents.</b> Please click the icon to download the documents.
					<div style="overflow: hidden; margin-top: 10px;">
						<?php foreach( $params['!docs'] as $name=>$data ) {
							echo "<div style='float: left; margin-right: 10px;'>";
								echo "<img style='float: left;' src='".$img_basepath."pdf.jpg' />";
								echo "<div style='float: left; margin-left: 5px;'>";
									echo "<b>".$name."</b><br/>";
									echo "<a href='$base_url/account/serve_pdf/".base64_encode($data['filepath'])."/".base64_encode($name)."' style='font-size: 11px; color: #1d5ca8;'>download</a>";
								echo "</div>";
							echo "</div>";
						}?>
					</div>
					<div style="font-style: italic; padding: 5px 0px; text-align: center; background-color: #e4ebf4; font-size: 11px; margin-top: 50px; margin-bottom: 5px;" style="margin:15px 0px;" >
						Please do not reply to this email. If you have questions about your account in general, email us at <a href="mailto:wecare@netclaimsnow.com" style="color: #1d5ca8" >wecare@netclaimsnow.com</a> or contact member services at <span style="color: #1d5ca8">877.654.8668</span>
					</div>
				</div>
			</div>
		</div>
		
		<div style="width: 520px; margin: 0px auto; padding: 10px 0px;">
			<div style="margin:15px 0px; text-align: center; font-size: 11px;">
				<b>LEGAL DISCLAIMER</b><br/>
				The information transmitted is intended solely for the individual or entity to which it is addressed and may contain confidential and/or privileged material. Any review, retransmission, dissemination or other use of or taking action in reliance upon this information by persons or entities other than the intended recipient is prohibited. If you have received this email in error please contact the sender and delete the material from any computer.
			</div>
			<div style="margin:15px 0px; text-align: center; font-size: 11px;">
				&copy; 2011 Net Claims Now, LLC<br/>
				Net Claims Now, the Net Claims Now logo are trademarks of Net Claims Now,</br>
				LLC. All other brand names may be trademarks of their respective owners
			</div>
		</div>
	</div>
</div>
</div>