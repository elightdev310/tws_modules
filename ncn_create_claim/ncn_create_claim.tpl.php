<div id="create-claim" style="float:left;">
  <div class="cc-content">
    <div class="cc-header">
      <div class="title page-title"><?php echo $title; ?></div>
      <div class="help hidden"><a href="#">Help</a></div>
    </div>
    <div class="cc-content-wrapper <?php echo $page_class; ?>">
      <?= (isset($module_content) ? $module_content : ''); ?>
    </div>
  </div>
  <div class="cc-extra" style="float:left;margin-left:40px;">
    <?= (isset($extra_module_content) ? $extra_module_content : ''); ?>
  </div>
</div>
