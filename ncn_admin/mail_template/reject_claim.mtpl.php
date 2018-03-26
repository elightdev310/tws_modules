<div class="mail_wrapper" style="color: #222;">
<div style="width: 600px; background-color: #a3c1de; font-family : Arial; font-size: 12px; margin: 5px auto;">
	<div style="width: 580px; margin: 0px auto; padding-top: 10px; padding-bottom: 20px;">
		<div style="color: #1d5ca8; font-style: italic;	width: 100%; margin: 5px 0px; overflow: hidden;">
			<div style="float: left;"><b>Net Claims Now</b> - Email</div>
			<div style="float: right">Contact us today! <b>877.654.8668</b></div>
		</div>
		<div style="width: 100%; background-color: white;">
			<div style="width: 540px; margin: 0px auto; padding: 10px 0px;">
				<div style="width: 100%; margin:15px 0px; overflow: hidden;">
					<img src="<?php echo $img_basepath."logo.png";?>" alt="logo"/>
					<a style="float: right;	width: 210px; height: 41px;	margin-top: 5px;" href="<?php echo $base_url."/user"; ?>">
						<img src="<?php echo $img_basepath."member_login.jpg";?>" alt="Activate Your Membership!"/>
					</a>
				</div>
				<div style="font-size: 26px; color: #c9100c; margin:15px 0px; width: 300px;">
					<b>Your claim documents are incomplete.</b>
				</div>
				<div style="margin:15px 0px;" >
					<b>The claim listed below has been returned to you.</b>
				</div>
				<div style="padding: 10px; color: #1d5ca8; background-color: #e4ebf4; font-size: 14px; margin:15px 0px;">
					<b>Homeowner name: </b><?php echo $params['!owner_name']; ?><br/>
					<b>Claim ID#: </b><?php echo $params['!claim_id']; ?>
				</div>
				<div style="margin:15px 0px;" >
					<b>Please address the following concerns: </b>
				</div>
				<div style="margin:15px 0px; padding-left: 15px; color: #c9100c; word-wrap: break-word;" >
					<?php echo nl2br($params['!reason']); ?>
				</div>
				<div style="margin:20px 0px 20px 0px;" >
					<b>Reply to this email with your answers or resend the appropriate revised documents via fax (877-654-8667). Please be sure to submit all changes at once.</b>
				</div>
        <div style="margin:0px 0px 20px 0px; color: #1f497d;" >
          Have you tried our Electronic Claims Processing Form and Scope Sheets? For more information please see directions in your document center!
        </div>
				<div style="margin:20px 0px;" >
					<b>Answers will only be processed during business hours.</b> If your answers are received after 2PM EST they will be processed the next business day. Business Hours are Monday through Friday 8AM to 5PM EST, excluding national holidays. Your invoice will be completed within 1 Business Day once we have confirmed all changes have been submitted. (Typically 24 hours or less, 2 Business Days for Reconstruction billing)
				</div>
				<div style="margin:30px 0px;" >
					Thank you!
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