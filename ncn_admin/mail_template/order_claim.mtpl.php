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
					<b>Thank You</b> for your order.
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
				<table border=0 style="font-size: 12px; width: 100%;">
					<thead style="font-weight: bold; text-align: left;">
						<th width="15%">Item</th>
						<th width="35%">Description</th>
						<th width="5%">Qty</th>
						<th width="10%">Taxable</th>
						<th width="17%">Unit Price</th>
						<th width="18%">Item Total</th>
					</thead>
					<tbody>
						<?php foreach ($params['products'] as $item) { ?>
						<tr>
							<td><?php echo $item['item']; ?></td>
							<td><?php echo $item['description']; ?></td>
							<td><?php echo $item['quantity']; ?></td>
							<td><?php echo $item['taxable']; ?></td>
							<td>US $<?php echo number_format(floatval($item['unit_price']), 2); ?></td>
							<td>US $<?php echo number_format(floatval($item['total_price']), 2); ?></td>
						</tr>
						<?php } ?>
						<tr style="height: 20px;">
							<td colspan="6"><hr style="border-top: solid 1px #a1a1a1;" /></td>
						<tr/>
						<tr style="font-weight: bold;">
							<td colspan="4">&nbsp;</td>
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