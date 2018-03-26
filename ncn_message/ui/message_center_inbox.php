<?php
/**
 * Message Center
 */
?>
<div id="msg_center_inbox">
<div class="mci-content">
    <div class="mci-header">
        <?php print ncn_member_logo_legalname_block($GLOBALS['user']->uid); ?>
        <div class="title page-title"><?php echo $title; ?></div>
    </div>
    <div class="mci-content-wrapper panel-box">
        <div class="panel-box-content">
            <div class="ncn-data-table table-responsive">
                <table class="pm table">
                    <thead>
                        <tr class="header"> 
                            <!-- <td>&nbsp;</td> -->
                            <th class="td-date">Date</th>
                            <th class="td-subject">Subject</th>
                        </tr>
                    </thead>
                    <tbody>
                        <?php if (empty($messages)): ?>
                        <tr>
                            <td colspan="2">No message</td>
                        </tr>
                    <?php else: ?>
                        <?php $i = 0; ?>
                        <?php foreach($messages as $row): ?>
                        <?php
                            // get data        
                            $row = (array) $row;
                            if ($row['read'] == 0)
                            {   
                                $subject_class = "subject_unread";
                                $subject_append = ' [unread]';
                            }
                            else
                            {   
                                $subject_class = "subject_read";    
                                $subject_append = '';
                            }
                            
                            // render the preview
                            ?>
                            <tr class="<?php print (($i%2)?"even":"odd"); ?>"> 
                                <!-- <td width="20"><input type="checkbox" name="msg_<?= $row['id']; ?>"></td> -->
                                <td class="td-date"><?= date("m/d/Y H:m", $row['timestamp']); ?></td>
                                <td class="td-subject">
                                    <a href="#" onclick="pm_toggle_message(<?= $row['id']; ?>, '<?= $toggle_base; ?>');return false;">
                                        <span id="subject_<?= $row['id']; ?>" class="<?= $subject_class; ?>"><?= StripSlashes($row['subject']); ?></span>
                                    </a>
                                    <span id="subject_append_<?= $row['id']; ?>"><?= $subject_append; ?></span>
                                </td>
                            </tr>
                            <tr class="hidden-row <?php if ($i%2) print "even_body"; else print "odd_body"; ?>">
                                <td colspan="2" class="internal">
                                <div id="body_<?= $row['id']; ?>" class="pm_body_div">
                                    <table class="pm_internal" cellpadding=0 cellspacing=0>
                                    <tbody class="pm_internal">
                                        <tr>
                                            <td class="td-date">&nbsp;</td>
                                            <td>
                                                <div><?= body_for_html($row['body']); ?></div>
                                                <div style="text-align:right;" class="action-panel">
                                                <?php if (!$is_admin): ?>
                                                    <a href="<?= $base_url; ?>/account/message-center/viewthread/<?= $row['claim_id']; ?>"><strong>[view thread]</strong></a>
                                                    <!-- &nbsp;
                                                    <a href="<?= $base_url; ?>/account/message-center/reply/<?= $row['id']; ?>"><strong>[reply]</strong></a> -->
                                                <?php endif; ?>
                                                </div>
                                            </td>
                                        </tr>
                                    </tbody>
                                    </table>
                                </div>
                                </td>
                            </tr>
                            <?php $i++; ?>
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
