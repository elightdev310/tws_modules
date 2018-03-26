<div id="create-claim" style="float:left;">
  <div class="cc-progress-list" style="float:left">
    <?= $progress_list; ?>
  </div>
  <div class="cc-content" style="float:left;margin-left:40px;">
    <div class="cc-header">
      <div class="title"><?php echo $title; ?></div>
      <div class="help"><a href="#">Help</a></div>
    </div>
    <div class="cc-content-wrapper <?php echo $page_class; ?>">
      <?= $module_content; ?>
      <form method="POST" id="create-claim-form">
        <input type="hidden" name="tfunction" value="process">
		<input type="hidden" name="page" value="1">
      	<input type="submit" name="sub" value="sub" />
      </form>
    </div>
  </div>
  <div class="cc-extra" style="float:left;margin-left:40px;">
    <?= $extra_module_content; ?>
  </div>
</div>
