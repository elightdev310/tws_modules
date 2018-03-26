<?php
/**
 * ACI - Claim Information Panel
 *
 * @var
 *     $claim_id
 *     $data
 */
?>
<div class="aci-comments-section">
<div class="form-group">
    <div for="comments_comments" class="col-sm-3 control-label">Comments: </div>
    <div class="col-sm-9">
        <textarea class="form-control" name="comments[content]" id="comments_content" rows="5"><?php echo isset($data['content'])?$data['content']:''; ?></textarea>
    </div>
</div>
</div>
