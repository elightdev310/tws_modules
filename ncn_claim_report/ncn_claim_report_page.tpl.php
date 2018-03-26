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

#ncn_table_header table th{
    height: 150px;
}

#ncn_table_body{
    position: absolute;
    left: 10px;
    top: 170px;
}

.col-group div{ width: 80px;}
.col-member div{ width: 100px;}
.col-type div{ width: 70px;}
.col-product div{ width: 100px;}
.col-owner div{ width: 90px;}
.col-claim-id div{ width: 60px;}
.col-expedite div { width: 60px;}
.col-pp div{ width: 20px;}
.col-cdv div{ width: 80px;}
.col-rcd div{ width: 80px;}
.col-adr div{ width: 80px;}
.col-am-backend div{ width: 70px;}
.col-ce-crs div{ width: 60px;}
.col-ce-cae div{ width: 60px;}
.col-ce-start div{ width: 80px;}
.col-ce-end div{ width: 80px;}
.col-esx-pdf div{ width: 60px;}
.col-am div{ width: 100px;}
.col-aip-vos div{ width: 80px;}
.col-total-amount div{ width: 150px;}
.col-coi div{ width: 100px;}

</style>

<?php $cols = 19; ?>

<div id="ncn_claim_report_page">
<div id="ncn_table_header">
<table border="1" id="report_table" >
<thead>
    <th class="col-group"><div><strong>&nbsp;</strong></div></th>
    <th class="white-cell col-member"><div><strong>Member</strong></div></th>
    <th class="white-cell col-type"><div><strong>Type</strong></div></th>
    <th class="white-cell col-product"><div><strong>Product</strong></div></th>
    <th class="white-cell col-owner"><div><strong>Owner</strong></div></th>
    <th class="white-cell col-claim-id"><div><strong>Claim ID#</strong></div></th>
    
    <th class="pink-cell col-pp"><div><strong>PP</strong></div></th>
    <th class="pink-cell col-cdv"><div><strong>Claim Docs in VOS</strong></div></th>
    
    <th class="green-cell col-rcd"><div><strong>Rejected Claim Docs</strong></div></th>
    <th class="green-cell col-adr"><div><strong>All Docs Recieved</strong></div></th>
    
    <th class="black-blue-cell col-am-backend"><div><strong>AM: Backend Complete</strong></div></th>
    
    <th class="pink-cell col-ce-crs" ><div><strong>CE: Claim Ready to send</strong></div></th>
    <th class="pink-cell col-ce-cae"><div><strong>CE: Claim Available Email sent</strong></div></th>
    <th class="pink-cell col-ce-start"><div><strong>CE: Download claim - (Timer Start)</strong></div></th>
    <th class="pink-cell col-ce-end"><div><strong>CE: Upload ESX on VOS (Timer End)</strong></div></th>
    
    <th class="green-cell col-esx-pdf"><div><strong>ESX PDF Uploaded / Email sent to AM</strong></div></th>
    
    <th class="black-blue-cell col-am"><div><strong>Account Manager</strong></div></th>
    <th class="black-blue-cell col-aip-vos"><div><strong>Approve Invoice Posted on VOS</strong></div></th>
    
    <th class="white-cell col-total-amount"><div><strong>Total Invoiced <?php echo $sum; ?></strong></div></th>
    
    <th class="white-cell col-coi"><div><strong>Change Order Invoice</strong></div></th>
    <!-- <th><strong>Returned Invoice(Yuka)</strong></th> -->
</thead>
</table>
</div>
<div id="ncn_table_body">
<table border="1" id="report_table" >
<tbody>
<?php if(isset($t_data))
{
    foreach($t_data as $data):
   if ($data['#row_type'] == 'claim') :
?>
<?php //foreach($t_data as $data): ?>
<?php //if ($data['#row_type'] == 'claim') :   ?>
<tr>
    <td class="col-group"><div>&nbsp;</div></td>
    <td class="col-member"><div><?php echo isset($data['member'])?$data['member']:''; ?></div></td>
    <td class="col-type"><div><?php echo isset($data['type'])?$data['type']:''; ?></div></td>
    <td class="col-product"><div><?php echo isset($data['product'])?$data['product']:''; ?></div></td>
    <td class="col-owner"><div><?php echo isset($data['owner'])?$data['owner']:''; ?></div></td>
    <td class="col-claim-id"><div><?php echo isset($data['claim_id_url'])?$data['claim_id_url']:''; ?></div></td>
    
    <td class="col-pp"><div><?php echo isset($data['pp'])?$data['pp']:''; ?></div></td>
    <td class="col-cdv"><div><?php echo isset($data['claim_docs_in_vos'])?$data['claim_docs_in_vos']:''; ?></div></td>
    
    <td class="col-rcd"><div><?php echo isset($data['rejected_claim_docs'])?$data['rejected_claim_docs']:''; ?></div></td>
    <td class="col-adr"><div><?php echo isset($data['all_docs_reieved'])?$data['all_docs_reieved']:''; ?></div></td>
    
    <td class="col-am-backend"><div><?php echo isset($data['claim_backend'])?$data['claim_backend']:''; ?></div></td>
    
    <td class="col-ce-crs"><div><?php echo isset($data['ce_claim_ready_to_send'])?$data['ce_claim_ready_to_send']:''; ?></div></td>
    <td class="col-ce-cae"><div><?php echo isset($data['ce_claim_ava_email_sent'])?$data['ce_claim_ava_email_sent']:''; ?></div></td>
    <td class="col-ce-start"><div><?php echo isset($data['ce_timer_start'])?$data['ce_timer_start']:''; ?></div></td>
    <td class="col-ce-end"><div><?php echo isset($data['ce_timer_end'])?$data['ce_timer_end']:''; ?></div></td>
    
    <td class="col-esx-pdf"><div><?php echo isset($data['esx_pdf_uploaded'])?$data['esx_pdf_uploaded']:''; ?></div></td>
    
    <td class="col-am"><div><?php echo isset($data['account_manager'])?$data['account_manager']:''; ?></div></td>
    <td class="col-aip-vos"><div><?php echo isset($data['invoice_posted_on_vos'])?$data['invoice_posted_on_vos']:''; ?></div></td>
    
    <td class="col-total-amount"><div><?php echo isset($data['claim_amount'])?$data['claim_amount']:''; ?></div></td>
    <td class="col-coi"><div><?php echo isset($data['rejected_invoice'])?$data['rejected_invoice']:''; ?></div></td>

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
<?php endforeach; }?>
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
