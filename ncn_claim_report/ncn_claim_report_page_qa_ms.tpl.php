<style>
#ncn_claim_report_page{
	margin: 0em 0em;
	font-family: Verdana,sans-serif;
	font-size: 12px;
	line-height: 20px;
	position: relative;
}
#report_table th, #report_table td{
	text-align : center;
	color: black;
	padding: 3px;
}

.white-cell{
	background-color: white;
}
.gray-cell{
	background-color: #ddd;
}
.pink-cell{
	background-color: #EAD1DC;
}
.dry-leaf-cell{
	background-color: #B8B78C;
}
.green-cell{
	background-color: #D8EAD2;
}
.black-blue-cell{
	background-color: #C9D9F8;
}
.light-green-cell{
	background-color: #00ff00;
}

table, td, th{
	border-color: #888;
}
table {
	margin: 0px;
}

#ncn_table_header{
	left: 10px;
	padding-top: 10px;
	position: fixed;
	background-color: white;
	z-index: 1000;
}

#ncn_table_header .ncn-report-header {
	width: 100%;
	height: 50px;
}

#ncn_table_header .report-title {
	width: 300px;
	float: left;
}
#ncn_table_header .total-invoiced {
	float: right;
}

#ncn_table_header table th{
	height: 150px;
}

#ncn_table_body{
	position: absolute;
	left: 10px;
	top: 220px;
}

.col-group div{ width: 80px;}
.col-member div{ width: 120px;}
.col-type div{ width: 80px;}
.col-product div{ width: 120px;}
.col-owner div{ width: 120px;}
.col-claim-id div{ width: 60px;}

.col-ccl-ei div{ width: 80px;}
.col-ce-crs div{ width: 60px;}
.col-ce-cae div{ width: 60px;}
.col-ce-start div{ width: 80px;}

.col-esx-pdf div{ width: 60px;}
.col-am div{ width: 120px;}
.col-aip-vos div{ width: 80px;}
.col-due-to div{ width: 100px;}


</style>

<?php $cols = 14; ?>

<div id="ncn_claim_report_page">
<div id="ncn_table_header">
<div class="ncn-report-header clearfix">
	<div class="report-title"><strong>Claim Report for Director of Quality Assurance and Director of Member Services</strong></div>
	<div class="total-invoiced"><strong>Total Invoiced: <?php echo $sum; ?></strong></div>
</div>
<table border="1" id="report_table" >
<thead>
	<th class="col-group"><div><strong>&nbsp;</strong></div></th>
	<th class="white-cell col-member"><div><strong>Member</strong></div></th>
	<th class="white-cell col-type"><div><strong>Type</strong></div></th>
	<th class="white-cell col-product"><div><strong>Product</strong></div></th>
	<th class="white-cell col-owner"><div><strong>Owner</strong></div></th>
	<th class="white-cell col-claim-id"><div><strong>Claim ID#</strong></div></th>
	
	<th class="dry-leaf-cell col-ccl-ei" ><div><strong>Check Claim Logfile for extra info</strong></div></th>
	<th class="pink-cell col-ce-crs" ><div><strong>CE: Claim Ready to send</strong></div></th>
	<th class="pink-cell col-ce-cae"><div><strong>CE: Claim Available Email sent</strong></div></th>
	<th class="pink-cell col-ce-start"><div><strong>CE: Download claim - (Timer Start)</strong></div></th>
	
	<th class="green-cell col-esx-pdf"><div><strong>ESX PDF Uploaded / Email sent to AM</strong></div></th>
	
	<th class="black-blue-cell col-am"><div><strong>Account Manager</strong></div></th>
	<th class="black-blue-cell col-aip-vos"><div><strong>Approve Invoice Posted on VOS</strong></div></th>

	<th class="white-cell col-due-to"><div><strong>Due Date/Time</strong></div></th>
	</thead>
</table>
</div>
<div id="ncn_table_body">
<table border="1" id="report_table" >
<tbody>
<?php
if(isset($t_data))
{
foreach ($t_data as $data): ?>
<?php if ($data['#row_type'] == 'claim') : ?>
<tr>
	<td class="col-group"><div>&nbsp;</div></td>
	<td class="col-member"><div><?php echo isset($data['member'])?$data['member']:''; ?></div></td>
	<td class="col-type"><div><?php echo isset($data['type'])?$data['type']:''; ?></div></td>
	<td class="col-product"><div><?php echo isset($data['product'])?$data['product']:''; ?></div></td>
	<td class="col-owner"><div><?php echo isset($data['owner'])?$data['owner']:''; ?></div></td>
	<td class="col-claim-id"><div><?php echo isset($data['claim_id_url'])?$data['claim_id_url']:''; ?></div></td>
	
	<td class="col-ccl-ei"><div><?php echo isset($data['check_claim_logfile'])?$data['check_claim_logfile']:''; ?></div></td>
	<td class="col-ce-crs"><div><?php echo isset($data['ce_claim_ready_to_send'])?$data['ce_claim_ready_to_send']:''; ?></div></td>
	<td class="col-ce-cae"><div><?php echo isset($data['ce_claim_ava_email_sent'])?$data['ce_claim_ava_email_sent']:''; ?></div></td>
	<td class="col-ce-start"><div><?php echo isset($data['ce_timer_start'])?$data['ce_timer_start']:''; ?></div></td>
	
	<td class="col-esx-pdf"><div><?php echo isset($data['esx_pdf_uploaded'])?$data['esx_pdf_uploaded']:''; ?></div></td>
	
	<td class="col-am"><div><?php echo isset($data['account_manager'])?$data['account_manager']:''; ?></div></td>
	<td class="col-aip-vos"><div><?php echo isset($data['invoice_posted_on_vos'])?$data['invoice_posted_on_vos']:''; ?></div></td>

	<td class="col-due-to"><div><?php echo isset($data['due_to_time_tt'])?$data['due_to_time_tt']:''; ?></div></td>
</tr>
<?php else: ?>
<tr>
	<?php $colspan = $cols; ?>
	<td class="gray-cell col-group"><div><?php echo $data['#group']; ?></div></td>
	<?php if ($data['#total'] > 0): ?>
	<td class="gray-cell col-member"><div>Total: <?php echo $data['#total']; ?></div></td>
	<?php $colspan -= 1; ?>
	<?php endif;  ?>
	<td colspan="<?php echo $colspan; ?>" class="gray-cell">&nbsp;</td>
</tr>
<?php endif; ?>
<?php endforeach; } ?>
</tbody>
</table>
</div>
</div>

<script>
jQuery(document).ready(function() {
	jQuery(window).scroll(function() {
		var _scrollLeft = jQuery(window).scrollLeft();
		_scrollLeft -= 10;
		_scrollLeft *= (-1);
		jQuery('#ncn_table_header').css('left', _scrollLeft+'px');
	});
});
</script>
