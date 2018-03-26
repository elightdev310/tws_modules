<div class="distributor-topbody">
  <?= isset($topbody)? $topbody : ''; ?>
</div>
<div id="distributor_page" class="clearfix">
  <div class="page-wrapper <?php echo $page_class; ?>">
    <div class="left-panel">
      <p><strong>Distributors Network</strong></p>
      <p><a href="/account/distributor.html/create">Create New Member</a></p>
      <p><a href="/account/distributor.html/view_member_list">View Member List</a></p>
      <p><a href="/account/distributor.html/sales_report">Sales Report</a></p>
      <!-- <p><a href="/account/distributor.html/payments_received">Payments Received</a></p>
	<p><a href="/account/distributor.html/contact_member">Contact Member</a></p> -->
    </div>
    <div class="right-content">
      <div class="dp-header">
        <div class="title"><?php echo $title; ?></div>
        <div class="help"><a href="#">Help</a></div>
      </div>
      <div class="dp-content-wrapper">
        <?= $content; ?>
      </div>
    </div>
  </div>
</div>
