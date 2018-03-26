<div id="page_results">
    <div id="subuser-list" class="claims-detail">
    <div class="claims-section-wrapper">
        <div class="clearfix">
            <div class="title page-title">Sub Users</div>
        </div>
        <div class="panel-box">
            <div class="panel-box-content">
                <div class="ncn-data-table table-responsive">
                <table class="table">
                <thead>
                    <tr>
                        <th class="td-first-name">First Name</th>
                        <th class="td-last-name">Last Name</th>
                        <th class="td-company-name">Company Name</th>
                        <th class="td-contact">Email</th>
                        <th class="td-address">Address</th>
                        <th class="td-city">City</th>
                        <th class="td-state">State</th>
                        <th class="td-zip">Zip</th>
                    </tr>
                </thead>
                <tbody>
                <?php if (empty($subusers)): ?>
                    <tr>
                        <td colspan="8">No Sub Users</td>
                    </tr>
                <?php else: ?>
                    <?php foreach($subusers as $row): ?>
                    <?php 
                        $row = (array)$row;
                        $_suser = user_load($row['sub_uid']);
                        if (!$_suser) { continue; }
                    ?>
                        <tr>
                            <td class="td-first-name"><?= $_suser->profile_firstname ?></td>
                            <td class="td-last-name"><?= $_suser->profile_lastname ?></td>
                            <td class="td-company-name"><?= $_suser->profile_legalname ?></td>
                            <td class="td-contact"><?= $_suser->mail ?></td>
                            <td class="td-address"><?= $_suser->profile_address ?></td>
                            <td class="td-city"><?= $_suser->profile_city ?></td>
                            <td class="td-state"><?= $_suser->profile_state ?></td>
                            <td class="td-zip"><?= $_suser->profile_zip ?></td>
                        </tr>
                    <?php endforeach; ?>
                <?php endif; ?>
                </tbody>
                </table>
                </div>

                <div class="ncn-table-pager">
                    <?php print theme('pager'); ?>
                </div>
            </div>
        </div>
    </div>
    </div>
</div>
