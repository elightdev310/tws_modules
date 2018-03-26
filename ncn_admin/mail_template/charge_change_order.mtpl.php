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
					<b>Thank You</b> for your Change Order.
				</div>
				<div style="margin:15px 0px; padding: 3px 10px; font-weight: bold; color: #f2f2f2; background-color: #a1a1a1; text-transform: uppercase;" >
					Order Information
				</div>
				<div style="margin:15px 0px;" >
					<b>Merchant: </b>&nbsp;&nbsp; <?php echo $params['merchant']; ?> <br/>
					<!--<b>Invoice Number: </b>&nbsp;&nbsp; <?php echo $params['invoice_number']; ?> <br/>-->
					<b>Claim ID: </b>&nbsp;&nbsp; <?php echo $params['claim_id']; ?> <br/>
					<b>Customer ID: </b>&nbsp;&nbsp; <?php echo $params['customer_id']; ?> <br/>
					<b>Date/Time: </b>&nbsp;&nbsp; <?php echo $params['timestamp']; ?>
				</div>
				<hr style="width: 100%; color: #a1a1a1;" />
				<div style="margin:15px 0px;" >
					<div style="font-weight: bold;">Billing Information</div>
					<div>
						<?php echo $params['member_name']; ?> <br/>
						<?php echo $params['member_address']; ?> <br/>
						<?php echo $params['member_city'].', '.$params['member_state'].' '.$params['member_zip']; ?> <br/>
						<?php echo $params['member_email']; ?> <br/>
						<?php echo $params['member_phone']; ?> <br/>
					</div>
				</div>
				<hr style="width: 100%; color: #a1a1a1;" />
				<div style="margin: 5px 0px;">
					<span><b>Claim ID: </b>&nbsp;&nbsp; <?php echo $params['claim_id']; ?></span> &nbsp;&nbsp;&nbsp;
					<span><b>Customer Name: </b>&nbsp;&nbsp; <?php echo $params['customer_name']; ?></span>
				</div>
				<table border=0 style="font-size: 12px; width: 100%;">
					<thead style="font-weight: bold; text-align: left;">
						<th width="15%">Action</th>
						<th width="10%">Line#</th>
						<th width="45%">Provided Details</th>
						<th width="15%">On Orignial Scope Sheets</th>
						<th width="15%">Amount</th>
					</thead>
					<tbody>
						<tr style="height: 20px;">
							<td colspan="5"><hr style="border-top: solid 1px #a1a1a1;" /></td>
						<tr/>
						<?php foreach ($params['requests'] as $room_name=>$r_data) { ?>
						<tr class='tr-room'><td colspan='4'><b><?php echo $room_name; ?></b>
							<?php $price = 0; ?>
							<?php foreach ($r_data['actions'] as $action=>$r_a_data) {?>
								<?php if ($action=='add_room'): ?> <span>(new room)</span> <?php endif; ?>
							<?php } ?></td>
							<td><?php if ($price) { echo render_payment_cost($price);} ?></td>
							<?php foreach ($r_data['requests'] as $r_co_data) {
								$ooss_checked = ($r_co_data['ooss'])? "&#x2713;": ''; ?>						
								<tr class='tr-cor'>
									<td><span style="padding-left: 10px;"><?php echo $r_co_data['action']; ?><span></td>
									<td><?php echo $r_co_data['line']; ?></td>
									<td><?php echo $r_co_data['detail']; ?></td>
									<td><?php echo $ooss_checked; ?></td>
									<td><?php echo render_payment_cost($r_co_data['price']); ?></td>
								</tr>
							<?php }?>
						<?php } ?>
						<tr style="height: 20px;">
							<td colspan="5"><hr style="border-top: solid 1px #a1a1a1;" /></td>
						<tr/>
						<tr style="font-weight: bold;">
							<td colspan="3">&nbsp;</td>
							<td>Total: </td>
							<td>US $<?php echo number_format(floatval($params['total']), 2); ?></td>
						</tr>
						
						
					</tbody>
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