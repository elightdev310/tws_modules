<div class="ncn-morejobs-collectform morejobs-content-wrapper">
  <div class="title"><span>Still Want <b style="font-weight: bold; color: red;">More Jobs?</b></span></div>
  <div align="center"><b class="tag" style="font-weight: bold; color: white;">We're billing and collecting an average of
    </style>
    <br>
    <b class="dollar" style="font-weight: bold; color: red;">$7,900
    </style>
    </b> <b class="dollar" style="font-weight: bold; color: white;">per emergency water damage invoice.
    </style>
    </b> </div>
  <div class="description">Please provide your information below. We will contact you shortly.</div>
  <div id="form-width" class="collect-form">
    <form id="ncn_more_jobs_collectform_frm" method="POST">
      <input type="hidden" name="tfunction" value="ncn-morejobs" />
      <div class="fields clearfix">
        <div class="" style="width: 271px; float: left;">
          <input type="text" name="business_name" class="placeholder-field text-field" value="<?php print $r_data['business_name']; ?>" placeholder="Business Name*" />
          <input type="text" name="business_street_address" class="placeholder-field text-field" value="<?php print $r_data['business_street_address']; ?>" placeholder="Business Street Address*" />
          <div class="clearfix">
            <input type="text" name="city" class="placeholder-field text-field" value="<?php print $r_data['city']; ?>" placeholder="City*" style="width: 90px; float:left; margin-right:4px; "/>
            <!-- US States -->
            <?php $states = array("AL", "AK", "AZ", "AR", "CA", "CO", "CT", "DE", "DC", "FL", "GA", "HI", "ID", "IL", "IN", "IA", "KS", "KY", "LA", "ME", "MD", "MA", "MI", "MN", "MS", "MO", "MT", "NE", "NV", "NH", "NJ", "NM", "NY", "NC", "ND", "OH", "OK", "OR", "PA", "RI", "SC", "SD", "TN", "TX", "UT", "VT", "VA", "WA", "WV", "WI", "WY"); ?>
            <select name="state" class="placeholder-field select-field" value="<?php print $r_data['state']; ?>" placeholder="State*" style="width: 80px; color: #525252; float:left; margin-right:4px; " >
              <option value=""><span style="font-weight: normal; color: #555;">State*</span></option>
              <?php foreach($states as $state): ?>
              <option value="<?= $state; ?>" <?php if ($state == $r_data['state']) print ' selected="selected "'; ?>>
              <?= $state; ?>
              </option>
              <?php endforeach; ?>
            </select>
            <input type="text" name="zipcode" class="placeholder-field text-field" id="morejobs_zipcode" value="<?php print $r_data['zipcode']; ?>" placeholder="Zipcode*" style="width: 60px; float:left;" />
          </div>
          <input type="text" name="website" class="placeholder-field text-field" value="<?php print $r_data['website']; ?>" placeholder="Website" />
        </div>
        <div style="width: 270px; float: right;">
          <input type="text" name="dm_name" class="placeholder-field text-field" value="<?php print $r_data['dm_name']; ?>" placeholder="Decision Maker Name*" />
          <input type="text" name="direct_number_dm" class="placeholder-field text-field" id="direct_number_dm" value="<?php print $r_data['direct_number_dm']; ?>" placeholder="Direct Number*" />
          <input type="text" name="email_address" class="placeholder-field text-field" value="<?php print $r_data['email_address']; ?>" placeholder="Email for Decision Maker*" />
          <input type="text" name="bt_to_contract" class="placeholder-field text-field" value="<?php print $r_data['bt_to_contract']; ?>" placeholder="Best Time to Contact" />
        </div>
      </div>
      <div align="center" class="actions">
        <div align="center" class="get-more-jobs-btn-wrapper">
          <input type="submit" value="GET MORE JOBS" />
        </div>
      </div>
    </form>
  </div>
</div>
<script>jQuery(document).ready(function() {	jQuery('#direct_number_dm').mask('999-999-9999');	jQuery('#morejobs_zipcode').mask('99999');	});</script>
