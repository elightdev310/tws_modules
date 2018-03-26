<div id="msg_center_inbox">
<div class="mci-content">
    <div class="mci-header">
        <?php print ncn_member_logo_legalname_block($GLOBALS['user']->uid); ?>
        <div style="float: right;"> 
            <a href="javascript:backToHistoryReload()">Back</a>
        </div>
        <div class="title page-title"> <?php echo $title; ?></div>

    </div>
    <div class="mci-content-wrapper panel-box">
        <div class="panel-box-content">
            <div class="ncn-data-table table-responsive">
                <table class="pm table">
                    <thead>
                        <tr class="header">
                            <th class="td-vt-from">From</th> 
                            <th class="td-vt-to">To</th>
                            <th class="td-vt-date">Date</th>
                            <th class="td-vt-msg">Message</th>
                        </tr>
                    </thead>
                    <tbody>
                        <?php $i = 0; ?>
                        <?php foreach($messages as $row): ?>
                        <?php
                            // get data
                            $row = (array)$row;
                            
                            ?>      
                            <tr class="<?php if ($i%2) print "even"; else print "odd"; ?>"> 
                                <td class="td-vt-from"><?= pm_get_username($row['from']); ?></td> 
                                <td class="td-vt-to" ><?= pm_get_username($row['to']); ?></td> 
                                <td class="td-vt-date"><?= date("m/d/Y H:m", $row['timestamp']); ?></td>
                                <td class="td-vt-msg">
                                    <strong><?= StripSlashes($row['subject']); ?></strong><br />
                                    <br />
                                    <div class="msg-body">
                                        <?= body_for_html($row['body']); ?>
                                    </div>
                                    <?php
                                    // onyl show "[reply]" button on message TO this user
                                    if ($row['to'] == $user->uid)
                                    {
                                        ?>
                                        <!-- <div style="text-align:right;" class="action-panel">
                                        <a href="<?= $base_url; ?>/account/message-center/reply/<?= $row['id']; ?>"><strong>[reply]</strong></a>
                                        </div> -->
                                        <?php
                                    }
                                    ?>
                                </td>
                            </tr>
                            <?php $i++; ?>
                        <?php endforeach; ?>
                    </tbody>
                </table>
            </div>
        </div>
    </div>
    <div class="ncn-control-back clearfix">
        <div style="float: right;"> 
            <a href="javascript:backToHistoryReload()">Back</a>
        </div>
    </div>
</div>
