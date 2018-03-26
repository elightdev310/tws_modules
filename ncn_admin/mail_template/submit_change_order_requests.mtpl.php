<div style="margin:15px 0px;" >
<p>
	<?php print t( "Member(!member_id - !member_name) submitted change order requests for claim(#!claim_id)", 
			array('!member_id'=>$params['member_id'], '!member_name'=>$params['member_name'], '!claim_id'=>$params['claim_id']) ); ?> 
	<br/>
	Please visit the following url to review change order requests.
</p>
<p>
	<?php print l($params['review_url'], $params['review_url']);  ?>
</p>
</div>
