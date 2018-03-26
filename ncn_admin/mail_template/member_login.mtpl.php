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
					<a style="float: right;	width: 210px; height: 41px;	margin-top: 5px;" href="<?php echo $base_url."/user"; ?>">
						<img src="<?php echo $img_basepath."member_login.jpg";?>" alt="Activate Your Membership!"/>
					</a>
				</div>
				<div style="font-size: 26px; color: #1d5ca8; margin:15px 0px;">
					<b>Congratulations!</b> Your account with Net Claims Now has been activated!
				</div>
				<div style="margin:15px 0px;" >
					<b>You may now log in to</b>&nbsp;
					<a href="<?php echo $base_url."/user"; ?>" style="color: #86b144;">http://www.netclaimsnow.com/user</a>&nbsp;
					using the following
				</div>
				<div style="padding: 10px; color: #1d5ca8; background-color: #e4ebf4; width: 280px;	font-size: 14px; margin:15px 0px;">
					<!-- username: <b><?php echo $params['!username']; ?></b><br/> -->
					email: <b><?php echo $params['!email']; ?></b><br/>
					password: <b><?php echo $params['!password']; ?></b>
				</div>
				<div style="margin:15px 0px;" >
					Thank you!
				</div>
			</div>
		</div>
		
		<div style="width: 100%; background-color: white;margin-top: 10px;">
			<div style="width: 540px; margin: 0px auto; padding: 10px 0px;">
				<table width="100%" style="font-size: 12px;"><tr>
					<td width="220px" style="vertical-align: top;">
						<div style="margin-right: 15px;">
							<b>Congratulations on completing your Net Claims Now member training. </b>
							<br/><br/>
							Today you trained with your<br/>
							<span style="font-weight: bold; font-style: italic; color: #1d5ca8;">Account Manager,</span>&nbsp;
							<span style="font-weight: bold; font-style: italic; color: #86b144;"><?php echo $params['!am']->profile_firstname.' '.$params['!am']->profile_lastname ?></span>
						</div>
					</td>
					<td style="background-color:#e4ebf4; padding: 5px;">
						<div style="float: left; margin-right: 5px;">
							<img src="<?php echo $img_basepath."am_".$params['!am']->uid.".jpg";?>" />
						</div>
						<div style="float: left; color: #1d5ca8; margin-top: 15px;">
							<span style="font-weight: bold; font-size: 14px;"><?php echo $params['!am']->profile_firstname.' '.$params['!am']->profile_lastname ?></span><br/>
							<span style="font-weight: bold;">Account Manager</span><br/>
							<?php echo $params['!am_fax']; ?><br/>
							<a href="mailto:<?php echo $params['!am']->mail; ?>" style="color: #1d5ca8;"><?php echo $params['!am']->mail; ?></a>
						</div>
					</td>
				</tr>
				</table>
				<div style="font-style: italic; padding: 5px 0px; text-align: center; background-color: #e4ebf4; font-size: 11px; margin-top: 20px; margin-bottom: 5px;" style="margin:15px 0px;" >
					Please do not reply to this email. If you have questions about your account in general, email us at <a href="mailto:wecare@netclaimsnow.com" style="color: #1d5ca8" >wecare@netclaimsnow.com</a> or contact member services at <span style="color: #1d5ca8">877.654.8668</span>
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
