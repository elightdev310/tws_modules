<?php
function get_claim_workflow_array() {
    return array(
        'none'                          => 'None', 
        'fax_doc_recieved'  => 'Fax Documents Received',
        'all_doc_recieved'  => 'All Documents Received',
        'doc_clarification' => 'Document Clarification',
        'rejected_invoices' => 'Rejected Invoices',
        'rejected_claim_docs'=>'Rejected Claim Documents',
        'completed_status'  => 'Completed Claim Status'
    );
}

//------------------------------------------------------------------------------
// view individual claim
function ncn_admin_view_claim($claim_id)
{
    GLOBAL $base_url;
  
    ncn_admin_view_claim_action($claim_id);

    $invoice_data = get_extended_data($claim_id);
    
    // ---- display stuff start ----
    // get logfile count
//  $query2 = "SELECT * FROM claims_log WHERE claim_id=".$claim_id.";";
    $result2 = db_query('SELECT * FROM {claims_log} WHERE claim_id=:a',array(':a'=>$claim_id));
    $entries_in_log = $result2->rowCount();
    
    // get claim status'
    $query = "SHOW COLUMNS FROM `claims` LIKE 'claim_status'";
    $result = db_query($query);
    $row = $result->fetchAssoc();
    
    $statuses = str_replace("'", "", $row['Type']);
    $statuses = str_replace("enum(", "", $statuses);
    $statuses = str_replace(")", "", $statuses);
    $statuses = explode(',', $statuses);
    
        

    // ---- draw controls -----
//  $query = "SELECT * FROM claims WHERE claim_id=".$claim_id.";";
    $result = db_query('SELECT * FROM {claims} WHERE claim_id=:a',array(':a'=>$claim_id));
    $row_count = $result->rowCount();
    if ($row_count == 0) {
        drupal_set_message(t('Claim(#!claim_id) doesn\'t exist.', array('!claim_id'=>$claim_id)), 'error');
        drupal_goto('admin/config/ncn_admin');
    }
    $row = $result->fetchAssoc();
    
    ?>
    
    <?php 
        /*************** Permission of View Claim ***************/
        //* Claim Overview 
        $pvc_claim_status       = ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'Claim Status');  
        $pvc_claim_type         = ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'Claim Type');    
        
        $pvc_claim_workflow     = ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'Claim Workflow'); 
        $pvc_claim_note         = ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'Claim Note'); 
        $pvc_send_cdr   = ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'Send Claim Documents Reminder'); 

        $pvc_claim_status_disabled = '';
        if (!($pvc_claim_status&2))     {$pvc_claim_status_disabled="disabled";}
        $pvc_claim_type_disabled = '';
        if (!($pvc_claim_type&2))       {$pvc_claim_type_disabled="disabled";}
        $pvc_claim_workflow_disabled = '';
        if (!($pvc_claim_workflow&2))       {$pvc_claim_workflow_disabled="disabled";}
        $pvc_claim_note_disabled = '';
        if (!($pvc_claim_note&2))           {$pvc_claim_note_disabled="disabled";}
        $pvc_send_cdr_disabled = '';
        if (!($pvc_send_cdr&2))             {$pvc_send_cdr_disabled="disabled";}
        
        
        //* Claim View - CE Portal Admin Panel
        $pvc_ce_management  = ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'Claim Exaimners Management'); 
        $pvc_ce_management_disabled = '';
        if (!($pvc_ce_management&2))        {$pvc_ce_management_disabled="disabled";}
        
        //* Upload Claim Documents
        $pvc_upload_claim_doc   = ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'Upload Claim Documents'); 
        $pvc_upload_claim_doc_disabled = '';
        if (!($pvc_upload_claim_doc&2))     {$pvc_upload_claim_doc_disabled="disabled";}
        //* Upload CE Documents
        $pvc_upload_ce_doc  = ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'Upload CE Documents');
        $pvc_upload_ce_doc_disabled = '';
        if (!($pvc_upload_ce_doc&2))        {$pvc_upload_ce_doc_disabled="disabled";}
        //* Upload ESX Files
        $pvc_upload_esx_file    = ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'Upload ESX Files'); 
        $pvc_upload_esx_file_disabled = '';
        if (!($pvc_upload_esx_file&2))      {$pvc_upload_esx_file_disabled="disabled";}
        //* Upload CE Invoice PDF Files
        $pvc_upload_ce_invoice_file     = ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'Upload CE Invoice PDF Files'); 
        $pvc_upload_ce_invoice_file_disabled = '';
        if (!($pvc_upload_ce_invoice_file&2))       {$pvc_upload_ce_invoice_file_disabled="disabled";}
        
        //* Claim Data
        $pvc_claim_data         = ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'Claim Data'); 
        $pvc_claim_invoice_data = ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'Claim Invoice Data'); 
        $pvc_claim_invoice_doc  = ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'Claim Invoice Documents'); 
        $pvc_claim_data_disabled = '';
        if (!($pvc_claim_data&2))           {$pvc_claim_data_disabled="disabled";}
        $pvc_claim_invoice_data_disabled = '';
        if (!($pvc_claim_invoice_data&2))   {$pvc_claim_invoice_data_disabled="disabled";}
        $pvc_claim_invoice_doc_disabled = '';
        if (!($pvc_claim_invoice_doc&2))    {$pvc_claim_invoice_doc_disabled="disabled";}
        
        //* Photo Album
        $pvc_photo_album        = ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'Photo Album'); 
        $pvc_photo_album_disabled = '';
        if (!($pvc_photo_album&2))          {$pvc_photo_album_disabled="disabled";}
        
        //* Create Invoice
        $pvc_create_invoice     = ncn_pvc_get_permission_from_user($GLOBALS['user']->uid, 'Create Invoice');
        $pvc_create_invoice_disabled = ''; 
        if (!($pvc_create_invoice&2))       {$pvc_create_invoice_disabled="disabled";}
        
    ?>
    
    <div>
        <fieldset>
        <legend>Claim Overview</legend>
        <form method="POST">
        <input type="hidden" name="claim_status_change" id="claim_status_change" value="" />
        <input type="hidden" name="current_scroll_position" class="current_scroll_position" value="" />
        <table> 
            <thead></thead>
            <tbody style="border:0;">
            <tr valign="top">
            <td style="padding:0;">
                <div>
                <table>
                    <thead></thead>
                    <tbody style="border:0;">
                        <?php if ($pvc_claim_status&01): ?>
                        <tr valign="top">           
                            <td><strong>Claim Type</strong></td>
                            <td>
                                <?php if ($pvc_claim_type_disabled=='disabled'): ?>
                                    <?php echo ucwords($row['claim_type']); ?> &nbsp;&nbsp;
                                    <?php echo ucwords($row['claim_product']); ?>&nbsp;&nbsp;
                                <?php else: ?>
                                    <input type="hidden" name="tfunction_claim_type" id="tfunction_claim_type" value="" />
                                    <?php echo ncn_admin_draw_select_claim_type('claim_type', $row['claim_type']); ?>   <br />
                                    <?php echo ncn_admin_draw_select_claim_product('claim_product', $row['claim_product']); ?>&nbsp;&nbsp;
                                    <input type="submit" value="Change" onclick="return on_click_change_claim_type();" />
                                    
                                <?php endif; ?>
                            </td>
                        </tr>
                        <tr>            
                            <td><strong>Date Created</strong></td>
                            <td><?= date("m/d/Y", $row['created']); ?></td>
                        </tr>
                        <tr>            
                            <td><strong>Last Modified</strong></td>
                            <td><?= date("m/d/Y", $row['last_modified']); ?></td>
                        </tr>
            
                        <?php if (user_access('ncn_admin edit addtional claim data')): ?>
                        <tr>
                            <td><strong>Claims Processing</strong></td>
                            <td>
                <a href="<?= $base_url; ?>/admin/config/<?= $url_part; ?>/view-additional-claim-data/<?= $claim_id; ?>" target='_blank'>View</a> / 
                <a href="<?= $base_url; ?>/admin/config/<?= $url_part; ?>/edit-additional-claim-data/<?= $claim_id; ?>" target='_blank'>Edit<!--Additional Data--></a>
                Claims Processing
              </td>
                        </tr>
            <?php elseif (user_access('ncn_admin view addtional claim data')): ?>
            <tr>
              <td><strong>Claims Processing</strong></td>
              <td>
                <a href="<?= $base_url; ?>/admin/config/<?= $url_part; ?>/view-additional-claim-data/<?= $claim_id; ?>" target='_blank'>View</a> 
                Claims Processing
              </td>
            </tr>
                        <?php endif; ?>
            
                        <tr>
                            <td><strong>Log file</strong></td>
                            <td><a href="<?= $base_url; ?>/admin/config/<?= $url_part; ?>/viewlog/<?= $claim_id; ?>"><?= $entries_in_log;?> entries in log</a></td>
                        </tr>
                        <tr>
                            <td><strong>Claim Amount</strong></td>
                            <td>$<input type="text" name="claim_amount" value="<?= $row['claim_amount']; ?>" style="width:120px;" /></td>
                        </tr>
                        <tr>
                            <td><strong>Payment Received</strong></td>
                            <td>$<input type="text" name="payment_received" value="<?= $row['payment_received']; ?>" style="width:120px;" /></td>
                        </tr>
                        <tr valign="top">
                            <td><strong>File Notes</strong></td>
                            <td>
                                <input type="hidden" name="tfunction_file_note" id="tfunction_file_note" value="" />
                                <textarea name="claim_file_note" style="height:70px; min-width: 200px;" ></textarea>
                                <?php if ($pvc_claim_status_disabled!='disabled'): ?>
                                <div><input type="submit" value=" Set " onclick="return on_click_change_file_note();" /></div>
                                <?php endif; ?>
                            </td>
                        </tr>
                        <?php endif; ?>
                    </tbody>
                </table>
                </div>
            </td>
            <td style="padding:0;">
                <div>
                <table>
                    <thead></thead>
                    <tbody style="border:0;">
                        <?php if ($pvc_claim_status&01): ?>
                        <tr>
                            <td><strong>Download Excel file</strong></td>
                            <td><a href="<?= $base_url; ?>/admin/config/<?= $url_part; ?>/downloadxls/<?= $claim_id; ?>">claim-data_<?= $claim_id; ?>.xls</a></td>
                        </tr>
                        <tr>
                            <td><strong>Download PDF file</strong></td>
                            <td><a href="<?= $base_url; ?>/admin/config/<?= $url_part; ?>/downloadpdf/<?= $claim_id; ?>">claim-data_<?= $claim_id; ?>.pdf</a></td>
                        </tr>
                        <tr>            
                            <td><strong>Claim Status</strong></td>
                            <td>
                                <select name="claim_status" >
                                    <?php
                                    
                                    foreach ($statuses as $status)
                                    {
                                        if ($status == $row['claim_status'])
                                        {   $checked = " selected ";    }
                                        else
                                        {   $checked = '';  }
                                    
                                        ?>
                                        <option value="<?= $status; ?>" <?= $checked; ?> ><?= ucwords($status); ?></option>
                                        <?php
                                    }
                                    
                                    ?>
                                </select>
                            </td>
                        </tr>
                        <tr>            
                            <td valign="top"><strong>Claim Status Message</strong></td>
                            <td>
                                <textarea name="claim_status_message" style="height:70px;" ></textarea>
                            </td>
                        </tr>
                        <tr>
                            <td><strong>Claim Back End</strong></td>
                            <td><?php echo draw_select_claim_view_claim_backend('claim_backend', get_ncn_data_extra($claim_id, 'claim_backend') ); ?></td>
                        </tr>
                        
                        <tr>
                            <td></td>
                            <td>
                                <?php if ($pvc_claim_status_disabled!='disabled'): ?>
                                <input type="submit" value="Update Claim Status" onclick="return on_click_change_claim_status();" >
                                <?php endif; ?>
                            </td>
                        </tr>
                        <?php endif;    // Claim Status?>
                    </tbody>            
                </table>
                </div>
            </td>
            </tr>
            </tbody>
        </table>    
        </form>
        
        <?php if ($pvc_claim_note&01): ?>
        <form id="ncn_admin_claim_note_form" method="POST" class="claim-note-add-form">
        <input type="hidden" name="tfunction_claim_note_action" id="tfunction_claim_note_action" value="tfunction_claim_note_add" />
    <input type="hidden" name="claim_note_id" id="claim_note_id" value="0" />
        <input type="hidden" name="current_scroll_position" class="current_scroll_position" value="" />
        <table>
            <tr>
                <td valign="top" style="width:50px"><strong>Note</strong></td>
                <td>
                    <?php 
            if (ncn_admin_note_check_mode($claim_id) == 'new') { print ncn_admin_note_render_claim_notes_table($claim_id, $GLOBALS['user']->uid); }
            else { ?> 
            <textarea rows="5" cols="80" readonly><?php $_claim_note = str_replace("<br/>", "\n", base64_decode($row['file_note']));  echo $_claim_note; ?></textarea> 
          <?php } ?>
                </td>
            </tr>
            <?php if ($pvc_claim_note_disabled!='disabled'): ?>
            <tr>
                <td></td>
                <td><input id="ncn_admin_claim_note_content" type="text" name="claim_note" size="80" /></td>
            </tr>
            <tr>
                <td></td>
                <td><input id="ncn_admin_claim_note_submit_btn" type="submit"  onclick="return on_set_scroll_position();" value="Add" />&nbsp;&nbsp;&nbsp;
            <input id="ncn_admin_claim_note_cancel_btn" type="button"  onclick="on_click_claim_note_cancel_btn(); "   value="Cancel" />
        </td>
            </tr>
            <?php endif; ?>
        </table>
        </form>
        <?php endif; ?>
        
        <?php if ($pvc_claim_workflow&01): ?>
        <form id="ncn_admin_claim_workflow_form" method="POST">
        <input type="hidden" name="tfunction_workflow" id="tfunction_workflow" value="tfunction_workflow" />
        <input type="hidden" name="current_scroll_position" class="current_scroll_position" value="" />
        <table>
            <tr>
                <td valign="top" style="width:180px"><strong>Claim Workflow</strong></td>
                <td>                    
                    <select id="claim_workflow" name="claim_workflow">
                    <?php
                    $arr_timer = get_claim_workflow_array();
                    foreach ($arr_timer as $key=>$val)
                    {
                        if ($key == $row['workflow'])
                        {   $checked = " selected ";    }
                        else
                        {   $checked = '';  }
                    
                        ?>
                        <option value="<?= $key; ?>" <?= $checked; ?>><?= $val; ?></option>
                        <?php
                    }
                    
                    ?>
                    </select>
                    <?php if ($pvc_claim_workflow_disabled!='disabled'): ?>
                    <input type="submit" onclick="return on_set_scroll_position();" value="Change"/>
                    <?php endif; ?>
                    
                    <div id="claim_timer_div">
                        <?php echo render_claim_timer($claim_id); ?>
                    </div>
                    
                    <?php if ($pvc_claim_workflow_disabled!='disabled'): ?>
                    <?php 
                        //$claim_docs_rejected_msg = variable_get('ncn_rejected_claim_docs_mail_body', '');
                        //$claim_docs_rejected_msg = ncn_admin_get_claim_docs_rejected_message($claim_id);
                    ?>
                    <div id="ncn_admin_rejected_claim_docs_mail_body" <?php if ($row['workflow']!='rejected_claim_docs') {echo 'style="display: none;"';} ?>>
                        <textarea name="rejected_claim_docs_mail_body" style="height:120px; min-width: 500px;"><?php echo isset($claim_docs_rejected_msg)?$claim_docs_rejected_msg:''; ?></textarea>
                    </div>
                    <script>
                        jQuery(document).ready(function() {
                            jQuery('#claim_workflow').bind('change', function() {
                                if( jQuery('#claim_workflow').val()=='rejected_claim_docs') {
                                    jQuery('#ncn_admin_rejected_claim_docs_mail_body').css('display', 'block');
                                } else {
                                    jQuery('#ncn_admin_rejected_claim_docs_mail_body').css('display', 'none');
                                }
                            });
                        });
                    </script>
                    <?php endif; ?>
                </td>
            </tr>
        </table>
        </form>
        <?php endif; // Claim Workflow?>
        
        
        <!--- Send Claim Documents Reminder -->
        <?php if ($pvc_send_cdr_disabled!='disabled'): ?>
        <form id="ncn_admin_send_claim_documents_reminder" method="POST">
            <input type="hidden" name="tfunction_send_cdr" value="tfunction_send_cdr" />
            <input type="hidden" name="current_scroll_position" class="current_scroll_position" value="" />
            <input type="submit" onclick="return on_set_scroll_position();" value="Send Claim Documents Reminder" />
            <p style="font-style: italic; font-size: 11px;">Send a notification mail to said that we have not received claim documents.</p>
        </form>
        <?php endif; ?>
        <!--- END of Send Claim Documents Reminder -->
        
        <?php echo ncn_admin_render_claim_status($claim_id); ?>
        </fieldset>
    
    </div>
    <script type="text/javascript">
        function on_click_change_file_note() {
            jQuery('#tfunction_file_note').val('tfunction_file_note');
            jQuery('.current_scroll_position').val(get_current_scroll_position());
            return true;
        }
        function on_click_change_claim_status() {
            jQuery('#claim_status_change').val('claim_status_change');
            jQuery('.current_scroll_position').val(get_current_scroll_position());
            return true;
        }
        function on_click_change_claim_type() {
            jQuery('#tfunction_claim_type').val('claim_type_change');
            jQuery('.current_scroll_position').val(get_current_scroll_position());
            return true;
        }
        function on_set_scroll_position() {
            jQuery('.current_scroll_position').val(get_current_scroll_position());
            return true;
        }
    
        
    </script>
    <div id="ce_manager_wrapper">
        <?php print ncn_ceportal_claimview_claim_exaimners($claim_id, $pvc_ce_management, $pvc_ce_management_disabled); ?>
    </div>
    <div>
    <?php
    
    if ($pvc_upload_claim_doc&01) {
        echo ncn_admin_draw_invoice_upload($claim_id, 2, $pvc_upload_claim_doc_disabled);
    }
    if ($pvc_upload_ce_doc&01) {
        echo ncn_admin_draw_invoice_upload($claim_id, 10, $pvc_upload_ce_doc_disabled);
    }
    if ($pvc_upload_esx_file&01) {
        echo ncn_admin_draw_invoice_upload($claim_id, 11, $pvc_upload_esx_file_disabled);
    }
    if ($pvc_upload_ce_invoice_file&01) {
        echo ncn_admin_draw_invoice_upload($claim_id, 12, $pvc_upload_ce_invoice_file_disabled);
    }
    
    // get/setup data
//  $query = "SELECT * FROM claims_data WHERE claim_id=".$claim_id." AND field_type!='hidden' AND field_type!='submit' AND field_type!='token' ORDER BY `weight` ASC;";
    $result = db_query('SELECT * FROM {claims_data} WHERE claim_id=:a AND field_type!=:b AND field_type!=:c AND field_type!=:d ORDER BY weight ASC',
    array(':a'=>$claim_id,':b'=>'hidden',':c'=>'submit',':d'=>'token'));
    $row_count = $result->rowCount();

    // page title
    drupal_set_title("Viewing claim: #".$claim_id." (".ncn_cd($claim_id, 'customer_name').")");

    
    // ---- draw data list ----
    ?>
    <?php if($pvc_claim_data&01): ?>
    <fieldset>
    <legend>Claim Data</legend>
    <form id="claims_data" method="post">
    <input type="hidden" name="current_scroll_position" class="current_scroll_position" value="" />
    <input id="claims_data_update" name="claims_data_update" type="hidden" value="claims_data_update" />
    
    <table class="sticky-enabled tableSelect-processed sticky-table">
    <thead class="tableHeader-processed">
    <tr>
        <th>Data Key</th>
        <th>Data Value</th>
    </tr>
    </thead>

    <tbody>
    <?php
    
    drupal_add_js(drupal_get_path('module', 'ncn_image') . '/ncn_image.js');
    $index=0;
    //for ($i=0; $i<$row_count; $i++)
    foreach($result as $row)
    {
        // get the data
        $row = (array)$row;
        $row['val'] = unserialize($row['val']);
        $res = "";

        if (empty($row['val'])||!is_array($row['val'])) {
            
        } else {
            if (!isset($row['val']['type']))
            {   $row['val']['type'] = "scope";  }
            
            // convert the data
            switch ($row['val']['type'])
            {
                case "checkboxes":
                    $res = "";
                    
                    foreach ($row['val'] as $k=> $v)
                    {
                        if (trim($v) != "0")
                        {   $res .= "<li> ".$v; }
                    }
                    
                    if ($res != "")
                    {   $res = "<ul>".$res."</ul>";     }
                break;
            
                case "phone_field_phone":
                    $res = implode(" ", $row['val']);
                break;

                case "time_field_time":
                    $res = render_claim_time($row['val']);
                break;

                case "date":
                    $res = render_claim_date($row['val']);
                break;
            
                case "image_upload_element":
                    $image_url = $base_url."/".$row['val']->filepath; 
                    $img_info = image_get_info($row['val']->filepath);
                    
                    $res = "<a href='".$image_url."?width=".$img_info['width']."&height=".$img_info['height']."&iframe=false' class='colorbox-load'>"; 
                    $res .= "<img src='".str_replace('default/files', 'default/files/imagecache/ncnadmin', $image_url)."' />";
                    $res .= "</a>"; 
                    $res .= "<br /><small style='margin:0;padding:0;'>(click to enlarge)</small>";
                break;
                
                case "scope":
                break;
                default:
                    //echo $row['key']."<br/>";
                    if ($row['key'] == 'date_of_loss') {
                        /* Hack for Mobile App Data */
                        if((int)$row['val']['val'][1]==0){
                            $xchng_ary = array($row['val']['val'][1],$row['val']['val'][0],$row['val']['val'][2]);
                            $row['val']['val'] = $xchng_ary;
                        }
                        /* Hack for Mobile App Data */
                        $res = ncn_admin_render_date_loss($row['key'], $row['val']['val']);
                    } else if ($row['key'] == 'insured_state') {
                        /* Hack for Mobile App Data */
                        preg_match('/\(([^\)]*)\)/', $row['val']['val'], $ins_match);
                        if(isset($ins_match[1]) && trim($ins_match[1])!=''){
                            $row['val']['val'] = $ins_match[1];
                        }
                        /* Hack for Mobile App Data */
                        $res = ncn_loc_state(array(
                                    'name'      => $row['key'], 
                                    'id'        => 'claim_data_field_'.$row['key'], 
                                    'class'     => '', 
                                    'disabled'  => false, 
                                    'required'  => false, 
                                    'state'     => $row['val']['val'], 
                                    'country'   => 'US', 
                                ));
                    } else {
                        if (is_array($row['val']['val']))
                        {
                                $res = '<input name="'.$row['key'].'" value="'.implode(" ", $row['val']['val']).'" />';
                        }
                        else
                        {
                            $res = '<input name="'.$row['key'].'" value="'. $row['val']['val'].'" size="50"  />';
                            
                        }
                    }
                break;
            }
        }
        
        // display the data
        if ($res != "" && $row['key']!='id') {
        ?>
        <tr class="<?php if ($index%2) print "even"; else print "odd"; ?>">
            <td><?= ucwords(str_replace("_", " ", $row['key'])); ?></td>
            <td><?= $res; ?></td>
        </tr>
        <?php
            $index+=1;
        }
    }
    ?>
    <?php if($pvc_claim_data_disabled!='disabled'): ?>
    <tr class="<?php if ($index%2) print "even"; else print "odd"; ?>">
        <td colspan="2">
            <input type="submit" onclick="return on_set_scroll_position();" value="Save Claims Data" />
        </td>
    </tr>
    <?php endif; ?>
    </tbody>
    </table>
    </form>
    </fieldset>
    <?php endif; //Claim Data ?>
    
    <?php
    if ($pvc_claim_invoice_data&01) {
        echo ncn_admin_invoice_data($claim_id, $invoice_data, $pvc_claim_invoice_data_disabled);
    }
    
    if ($pvc_claim_invoice_doc&01) {
        echo ncn_admin_draw_invoice_upload($claim_id, 3, $pvc_claim_invoice_doc_disabled);
        echo ncn_admin_draw_invoice_upload($claim_id, 4, $pvc_claim_invoice_doc_disabled);
        echo ncn_admin_draw_invoice_upload($claim_id, 5, $pvc_claim_invoice_doc_disabled);
        echo ncn_admin_draw_multi_file_upload($claim_id, $pvc_claim_invoice_doc_disabled);
    }
    ?>
    </div>
    <?php if($pvc_photo_album&01): ?>
    
    <form id="ncn_admin_add_room" method="post">
        <input type="hidden" name="photo_add_room" value="photo_add_room" />
        <input type="hidden" id="room_name" name="room_name" />
        <input type="hidden" name="current_scroll_position" class="current_scroll_position" value="" />
    </form>
    <form id="ncn_admin_delete_room" method="post">
        <input type="hidden" name="photo_delete_room" value="photo_delete_room" />
        <input type="hidden" id="room_name" name="room_name" />
        <input type="hidden" name="current_scroll_position" class="current_scroll_position" value="" />
    </form>
    <form id="ncn_admin_delete_first_room" method="post">
        <input type="hidden" name="photo_delete_first_room" value="photo_delete_first_room" />
        <input type="hidden" id="room_name" name="room_name" />
        <input type="hidden" name="current_scroll_position" class="current_scroll_position" value="" />
    </form>
    
    <table class="sticky-enabled tableSelect-processed sticky-table">
    <thead>
        <th>Photo Album<span style="margin-left:50px;"><a href="#ncn_admin_add_room" onclick="on_click_ncn_admin_add_room();">Add Room</a></span>
        </th>
    </thead>
    <tbody>
    <?php 
        // $query = "SELECT * FROM claims_data WHERE claim_id=".$claim_id." AND field_type!='hidden' AND field_type!='submit' AND field_type!='token' ORDER BY `weight` ASC;";
        $result = db_query('SELECT * FROM {claims_data} WHERE claim_id=:a AND field_type!=:b AND field_type!=:c AND field_type!=:d ORDER BY weight ASC',
        array(':a'=>$claim_id,':b'=>'hidden',':c'=>'submit',':d'=>'token'));
        // $result = db_query($query);
        if ($result == false) {
            $row_count = 0;
        } else {
            $row_count = $result->rowCount();
        }
        //mysql_data_seek($result, 0);
        for ($i=0; $i<$row_count; $i++)
        {
            // get the data
            $row = $result->fetchAssoc();
            if ($row['key'] == '#data') { continue; }
            
            $row['val'] = unserialize($row['val']);
            $res = "";
            if (empty($row['val'])||!is_array($row['val'])) {
                continue;
            }

            if (!isset($row['val']['type']))
            {   $row['val']['type'] = "scope";  }

            switch ($row['val']['type'])
            {
                case "scope":
                    $start_num = 0;
                    foreach ($row['val'] as $roomname => $roomdata)
                    {
                        if ($roomname != "type")
                        {
                            $first_room = false;
                            $default_room_name = ncn_admin_claim_get_default_room_name($claim_id);
                                                        
                            if ($roomname == $default_room_name) {
                                $first_room = true;
                            }
                            if ($first_room && ncn_admin_is_photo_first_room_deleted($claim_id)) { continue; }
                            
                            $res .= '<fieldset class="claims-roomdata clearfix">';
                            $res .= '<legend><h3><b>'.$roomname.'</b></h3></legend>';
                            $image_count = 6;
                            if ($first_room) {
                                $image_count = 3;
                            }
                            
                            $res .= '<form class="update-captions" method="post" >';
                            $res .= '<input type="hidden" name="roomname" value="'.$roomname.'" />';
                            $res .= '<input type="hidden" name="update_captions" value="update_captions" />';
                            $res .= '<input type="hidden" name="image_count" value="'.$image_count.'" />';
                            $res .= '<input type="hidden" name="current_scroll_position" class="current_scroll_position" value="" />';
                            $res .= '<div class="claims-room-images clearfix">';
                            
                            
                            $captions = _get_image_captions($claim_id, $roomname, $image_count);
                            for ($i=0; $i<$image_count; $i++)
                            //foreach ($row['val'][$roomname] as $img)
                            {
                                $img_index = 'image'.$i;
                                
                                $res .= '<div class="scope-image">';
                                if (isset($row['val'][$roomname][$img_index])) {
                                    $img = $row['val'][$roomname][$img_index];
                                    $image_url = $GLOBALS['base_url']."/".$img['path']."?rid=". date('U');


                                    $win_width = "800";
                                    $win_height = "620";
                                    $in_url = $GLOBALS['base_url']."/admin/config/ncn_image_open/?";
                                    $in_url .= "img=".urlencode($img['path'])."&";
                                    $in_url .= "claim_id=".$claim_id."&room_name=".$roomname . "&position=".$img_index."&";
                                    $in_url .= "width=".$win_width."&height=".$win_height;

                                    if ($pvc_photo_album_disabled != 'disabled') {
                                        $res .= "<a onclick=\"open_image_box('".$GLOBALS['base_url']."', $claim_id, '$roomname', '$img_index', '".urlencode($img['path'])."')\" >";
                                        //$res .= "<a class='colorbox-node' href='{$in_url}'  >";
                                    }
                                    
                                    if(strpos($image_url,"public://")!==false){
                                        $timage_url = str_replace('public://', 'sites/default/files/styles/image_tab_upload/public/', $image_url);                                      
                                    } else {
                                        $timage_url = str_replace('default/files', 'default/files/imagecache/ncnadmin', $image_url);                                        
                                    }
                                    // Show with D7 way
                                    {
                                        $t_path = str_replace('sites/default/files/', 'public://', $img['path']);
                                        $timage_url = image_style_url('medium',$t_path);
                                    }
                                    $res .= "<img src='".$timage_url."' />";
                                    if ($pvc_photo_album_disabled != 'disabled') {
                                        $res .= "</a>"; 
                                    }
                                    
                                } else {
                                    $image_url = _get_blank_room_image_url($image_count, $i);
                                    //$res .= '<div class="blank-image">&nbsp;</div>';

                                    $win_width = "800";
                                    $win_height = "620";
                                    $in_url = $GLOBALS['base_url']."/admin/config/ncn_image_open/?";
                                    $in_url .= "claim_id=".$claim_id."&room_name=".$roomname . "&position=".$img_index."&";
                                    $in_url .= "width=".$win_width."&height=".$win_height;

                                    if ($pvc_photo_album_disabled != 'disabled') {
                                        $res .= "<a onclick=\"open_image_box('".$GLOBALS['base_url']."', $claim_id, '$roomname', '$img_index', '')\" >";
                                        //$res .= "<a class='colorbox-node' href='{$in_url}'  >";
                                    }
                                    $res .= '<img src="'.$image_url.'" width="200px"/>';
                                    if ($pvc_photo_album_disabled != 'disabled') {
                                        $res .= "</a>"; 
                                    }
                                }
                                $res .= '<div class="image-caption"><input type="text" name="'.$img_index.'" value="'.$captions[$i].'" /></div>';
                                $res .= "</div>";
                                
                                /*$res .= "<a href='".$image_url."?width=500&height=500&iframe=false' class='colorbox-load'>"; 
                                $res .= "<img src='".str_replace('default/files', 'default/files/imagecache/ncnadmin', $image_url)."' />";
                                $res .= "</a>"; */
                            }
                            
                            $res.= "</div>";
                            
                            if ($pvc_photo_album_disabled != 'disabled') {
                                $res.= "<div class='update-caption-action'>";
                                $res.= '<input type="submit" onclick="return on_set_scroll_position();" value="Update Captions" />';
                                if ($image_count==6) {
                                    $res.= ('&nbsp;&nbsp;&nbsp;<input type="button" value="Edit Roomname" onclick="on_click_edit_roomname('.$claim_id.', \''.$roomname.'\')"/>');
                                    $res.= ('&nbsp;&nbsp;&nbsp;<input type="button" value="Delete Room" onclick="on_click_ncn_admin_delete_room(\''.$roomname.'\')"/>');
                                } else {
                                    $res.= ('&nbsp;&nbsp;&nbsp;<input type="button" value="Delete Room" onclick="on_click_ncn_admin_delete_first_room(\''.$roomname.'\')"/>');
                                }
                                $res.= "</div>";
                            }
                            $res .= '</form>';
                            
                            $res.= "<div class=''>";
                            if ($image_count==6) {
                                $res.= ncn_admin_draw_scope_upload($claim_id, "$roomname", $pvc_photo_album_disabled, $start_num);
                            }
                            $res.= "</div>";
                            
                            //* Edit Room ScopeSheet Link
                            
                            if ($image_count==6) {
                                $res.="<div class='claim-view-scopesheet-links clearfix'>";
                                if (user_access('ncn_admin view room scopesheet')) {
                                    $_room_ss_view_url = base_path()."admin/config/ncn_admin/view-room-scopesheet/$claim_id/$roomname";
                                    $res.= "<div class='edit-room-scopesheet-link room-ss-link'><a href='$_room_ss_view_url' target='_blank'>View Room ScopeSheet</a></div>";
                                }
                                if (user_access('ncn_admin edit room scopesheet')) {
                                    $_room_ss_edit_url = base_path()."admin/config/ncn_admin/edit-room-scopesheet/$claim_id/$roomname";
                                    $res.= "<div class='view-room-scopesheet-link room-ss-link'><a href='$_room_ss_edit_url' target='_blank'>Edit Room ScopeSheet</a></div>";
                                }
                                $res.="</div>";
                            }
                            
                            $res.= "</fieldset>";   // claims-roomdata
                            
                            $start_num ++;
                        }
                    }
                break;
            }
            if ($res != "") {
            ?>
            <tr class="<?php if ($i%2) print "even"; else print "odd"; ?>">
                <td><?= $res; ?></td>
            </tr>
            <?php
            }
        }
    ?>
    </tbody>
    </table>
    <?php endif; // Photo Album ?>
    
    
    <?php if (user_access('ncn change order request review')): ?>
    <div>
        <fieldset>
        <legend>Change Order Requests</legend>
        <div class='cor-lists'>
            <?php print ncn_change_order_render_request_list_table_admin($claim_id); ?>
        </div>
        </fieldset>
    </div>
    <?php endif; ?>
    
    <?php if ($pvc_create_invoice&01): ?>
    <?php echo ncn_admin_draw_invoice_upload($claim_id, 1, $pvc_create_invoice_disabled); ?>
    <?php if ($pvc_create_invoice_disabled!='disabled'): ?>
    <div class="create-invoice">
    <form id="create_invoice" method="post">
        <input type="hidden" name="create_invoice_function" value="create_invoice" />
        <input type="hidden" name="current_scroll_position" class="current_scroll_position" value="" />
        <input type="submit" onclick="return on_set_scroll_position();" value="CREATE INVOICE" />
    </form>
    </div>
    <?php endif; ?>
    <?php endif;//Create Invoice; ?>

    <?php

    // ARS Section
    print ncn_admin_claim_view_ars_section($claim_id);

    if (module_exists('ncn_chatter')) {
        echo ncn_chatter_claim_feed_block($claim_id);
    }



    ////////////////////////////////////////////////////
    if (isset($_POST['current_scroll_position']) && $_POST['current_scroll_position'] != "")
    {
        echo "<script language='javascript'> window.onload = function(){window.scrollTo(0, ". $_POST['current_scroll_position'] .");} </script>";
    }
    else if (isset($_GET['current_scroll_position']) &&  $_GET['current_scroll_position'] != "")
    {
        echo "<script language='javascript'> window.onload = function(){window.scrollTo(0, ". $_GET['current_scroll_position'] .");}</script>";
    }
    else if (isset($_SESSION['current_scroll_position']))
    {
        echo "<script language='javascript'> window.onload = function(){window.scrollTo(0, ". $_SESSION['current_scroll_position'] .");}</script>";
        $_SESSION['current_scroll_position'] = 0;
    }
}

//------------------------------------------------------------------------------
// view individual claims' logfile
function ncn_admin_view_logfile($claim_id)
{
    GLOBAL $base_url;

    // page title
    drupal_set_title("Viewing logfile: ".$claim_id." (".ncn_cd($claim_id, 'customer_name').")");
    
    // draw the logfile
    ?>
    <table>
    <tbody>
        <thead class="tableHeader-processed">
        <tr>
            <th>Timestamp</th>
      <th>IP Address</th>
            <th>Message</th>
      <th>Mobile App</th>
        </tr>
        </thead>
    
    <?php
    
//  $query = "SELECT * FROM claims_log WHERE claim_id=".$claim_id." ORDER BY `timestamp` DESC;";
    $result = db_query('SELECT * FROM {claims_log} WHERE claim_id=:a ORDER BY timestamp DESC',array(':a'=>$claim_id));
    $row_count = $result->rowCount();
    
    
    for ($i=0; $i<$row_count; $i++)
    {
        $row = $result->fetchAssoc();
        
        ?>
        <tr class="<?php if ($i%2) print "even"; else print "odd"; ?>">
            <td>
                <?= date("D jS F Y h:i:s A", $row['timestamp']) ?>
            </td>
      <td><?= $row['ip_address']; ?></td>
            <td>
                <?= nl2br(StripSlashes($row['message'])); ?>
            </td>
      <td>
        <?= $row['app_track']; ?>
      </td>
        </tr>
        <?php
    }
    
    
    ?>
    </tbody>
    </table>
    <?php

}

//------------------------------------------------------------------------------
function ncn_admin_download_pdf($claim_id)
{
    ncn_admin_download_xls($claim_id, true);
}

//------------------------------------------------------------------------------
// download an xls format document of all the claims data
function ncn_admin_download_xls($claim_id, $as_pdf = false)
{
GLOBAL $base_url;

    ob_end_clean();
    
    // include the php/xls class
    require_once 'sites/all/libraries/php_xls/Classes/PHPExcel.php';
     
    // Create new PHPExcel object
    $objPHPExcel = new PHPExcel();
    
    // Set properties
    $objPHPExcel->getProperties()->setCreator("Net Claims Now")
                                 ->setLastModifiedBy("Net Claims Now")
                                 ->setTitle("Office 2003 Document")
                                 ->setSubject("Office 2003 Document")
                                 ->setDescription("Claim data. Generated by Net Claims Now.")
                                 ->setKeywords("net claims now")
                                 ->setCategory("Claim Data");
    
    $objPHPExcel->getActiveSheet()->getColumnDimension('A')->setWidth(37);
    $objPHPExcel->getActiveSheet()->getColumnDimension('B')->setWidth(55);
    

    // insert standard data
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("A2", "Claim ID #");
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("B2", $claim_id);
    

    // get/setup data
//  $query = "SELECT * FROM claims_data WHERE claim_id=".$claim_id." AND field_type!='hidden' AND field_type!='submit' AND field_type!='token' ORDER BY `weight` ASC;";
    $result = db_query('SELECT * FROM {claims_data} WHERE claim_id=:a AND field_type!=:b AND field_type!=:c AND field_type!=:d ORDER BY
     weight ASC',array(':a'=>$claim_id,':b'=>hidden,':c'=>submit,':d'=>token));
    $row_count = $result->rowCount();

    // page title
    
    // ---- draw data list ----
    $i = 0;

    for ($a=0; $a<$row_count; $a++)
    {

        $row = $result->fetchAssoc();

        // get the key
        $key = ucwords(str_replace("_", " ", $row['key']));
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("A".($i+5), $key);
    
        // get the data
        $row['val'] = unserialize($row['val']);
        $res = "";
        
        
        if (!isset($row['val']['type']))
        {   $row['val']['type'] = "scope";      }
        
        // convert the data
        switch ($row['val']['type'])
        {
            case "checkboxes":
                foreach ($row['val'] as $k=>$v)
                {
                    if ($v == "0")
                    {   unset($row['val'][$k]); }
                }
            
                $res = implode(", ", $row['val']);
                $objPHPExcel->setActiveSheetIndex(0)->setCellValue("B".($i+5), $res);
            break;
        
            case "phone_field_phone":
                $res = implode(" ", $row['val']);
                $objPHPExcel->setActiveSheetIndex(0)->setCellValue("B".($i+5), $res);
            break;

            case "time_field_time":
                $res = render_claim_time($row['val']);
                $objPHPExcel->setActiveSheetIndex(0)->setCellValue("B".($i+5), $res);
            break;

            case "date":
                $res = render_claim_date($row['val']);
                $objPHPExcel->setActiveSheetIndex(0)->setCellValue("B".($i+5), $res);
            break;
        
            case "scope":
            
                $count = 0;
            
                foreach ($row['val'] as $roomname => $roomdata)
                {
                
                    if ($count > 0)
                    {   $i++;   }
                
                    if ($roomname != "type")
                    {
                        $res .= '<h3>'.$roomname.'</h3>';
                        
                        $letters = array("B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M");
                        $k = 0;

                        foreach ($row['val'][$roomname] as $img)
                        {
                            // get path/image info
                            $image_url = $img['path'];
                            $img_path = str_replace('public://', 'sites/default/files/', $image_url);
                            if(!strpos($img_path, '.')){
                                $img_path .= '/'.$img['filename'];
                            }

                            if(file_exists(DRUPAL_ROOT.'/'.$img_path)){
                                $discard = file_get_contents($base_url."/".$img_path);
                                $img_info = image_get_info($img_path);

                                // change column height
                                $objPHPExcel->getActiveSheet()->getRowDimension(($i+5))->setRowHeight(($img_info['height']));
                                $objPHPExcel->getActiveSheet()->getColumnDimension($letters[$k])->setWidth(55);

                                // add image
                                $objDrawing = new PHPExcel_Worksheet_Drawing();
                                $objDrawing->setName($key);
                                $objDrawing->setDescription($key);
                                $objDrawing->setPath($img_path);
                                $objDrawing->setCoordinates($letters[$k].($i+5));
                                $objDrawing->setWorksheet($objPHPExcel->getActiveSheet());
                            }


                            $k++;


                        }
                    }
                    
                    $count++;
                }
                
            break;
        
            default:
            
                if (is_array($row['val']['val']))
                {   $res = implode(" ", $row['val']['val']);    }
                else
                {   $res = $row['val']['val'];      }
            
                $objPHPExcel->setActiveSheetIndex(0)->setCellValueExplicit("B".($i+5), $res, PHPExcel_Cell_DataType::TYPE_STRING);
                
//              $objPHPExcel->setActiveSheetIndex(0)->getCell('A'.($i+5))->setValueExplicit($res, PHPExcel_Cell_DataType::TYPE_TEXT);
            break;
        }
        
        $i++;

    }

    // close up the xls
    $objPHPExcel->getActiveSheet()->setTitle('Claim Data');
    
    // Set active sheet index to the first sheet, so Excel opens this as the first sheet
    $objPHPExcel->setActiveSheetIndex(0);


    // Redirect output to a client\92s web browser (Excel5)
    if ($as_pdf == false)
    {
        header('Content-Type: application/vnd.ms-excel');
        header('Content-Disposition: attachment;filename="claim-data_'.$claim_id.'.xls"');
        header('Cache-Control: max-age=0');
        $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5');
        $objWriter->save('php://output');
    }
    // Redirect output to a client\92s web browser (PDF)
    else
    {

        $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel2007');

        $tmp_xls_name = DRUPAL_ROOT.'/tmp/claim-data_'.$claim_id.'.xlsx';

        $tmp_pdf_name = DRUPAL_ROOT.'/tmp/claim-data_'.$claim_id.'.pdf';

        try{
            $objWriter->save($tmp_xls_name);
        }catch (Exception $e){
            print($e->getMessage());
            exit;
        }

        // $command = "java -jar ../jod/lib/jodconverter-core-3.0-beta-3.jar ".$tmp_xls_name." ".$tmp_pdf_name;
        $command = "java -jar ".$_SERVER["DOCUMENT_ROOT"].JAR_JODCONVERTER." $tmp_xls_name $tmp_pdf_name";
        watchdog('java-cmd', $command);
        $output = shell_exec($command);

        echo $output;

        header('Content-type: application/pdf');
        header('Content-Disposition: attachment;filename="claim-data_'.$claim_id.'.pdf"');

        @ readfile($tmp_pdf_name);
        unlink($tmp_xls_name);
        unlink($tmp_pdf_name);
    }
    
    exit;
}

function ncn_admin_claimsdata_update($claim_id) {
    $error = false;
  $options = array();
    foreach($_POST as $key=>$val) {
        if (!is_array($val)) {
            $val = trim($val);
        }
    //  $query = "SELECT * FROM claims_data WHERE claim_id=$claim_id AND `key` = '$key' ";
        $result = db_query('SELECT * FROM {claims_data} WHERE claim_id=:a AND `key`=:b',
            array(':a'=>$claim_id,':b'=>$key));
        if ($result->rowCount()>0) {
      $b_update = TRUE;
            // change the first room name
            if ($key == 'customer_name' && $val!= ncn_cd($claim_id, 'customer_name')) {
        $val = ncn_admin_remove_special_char_from_customer_name($val);
                if (ncn_admin_change_first_room_name($claim_id, $val)) {
          $options['insured_name'] = 'change';
        } else {
          $b_update = FALSE;
        }
            }
            
      if ($b_update) {
          $row = $result->fetchAssoc();
              $row['val'] = unserialize($row['val']);
              $row['val']['val'] = $val;
              $data = serialize($row['val']);
             // $query1 = "UPDATE claims_data SET `val` = '".mysql_real_escape_string($data)."' WHERE claim_id=$claim_id AND `key`='$key' ";
              $result1 = db_query('UPDATE {claims_data} SET val=:a WHERE claim_id=:b AND `key`=:c',
                  array(':a'=>$data,':b'=>$claim_id,':c'=>$key));
      }
        }
    }
    if (!$error) {
        drupal_set_message('Saved claims data, successfully.');
    }
  
  // Synchronize Additional Data
  ncn_admin_sync_additional_data($claim_id, $options);
}

//------------------------------------------------------------------------------
// list the latest claims with sorting/filtering/etc 
function ncn_admin_list_claims()
{
    GLOBAL $user;
    GLOBAL $base_url;
    
    $tfunction = (isset($_REQUEST['tfunction']) ? $_REQUEST['tfunction'] : '');
    // Auto Assign Mode
    if ($tfunction == 'auto_assign_feature') {
        $aa_mode = ($_REQUEST['auto_assign_mode']==1)? TRUE:FALSE;
        if (ncn_ce_portal_set_mode($aa_mode)) {
            drupal_set_message(t('Auto Assign Mode: @mode', array( '@mode'=>($aa_mode)?'On':'Off' )));
        }
    } else if ($tfunction=='delete_multi_claims') {
    $sel_claims = $_REQUEST['sel_claims'];
    foreach ($sel_claims as $claim_id) {
      ncn_admin_delete_claim($claim_id);
    }
  }
    
    $filter = arg(4);
    $filter_user = arg(6);
    $filter_claim_workflow = arg(8);
    $filter_claim_id = arg(10);

  if (ncn_admin_is_user_DriRite($user->uid)) {
    $filter_user = 598; //Dustin Lawson (DriRite)
  }
  
    if (empty($filter_claim_workflow)) {
        $filter_claim_workflow = 'all';
    }
    
    if (empty($filter_claim_id)) {
        $filter_claim_id = 'all';
    }
    
    if (stristr($_SERVER['REQUEST_URI'], 'ncn_admin_pool'))
    {   $url_part = "ncn_admin_pool";   }
    else    
    {   $url_part = "ncn_admin";    }

    // ---- draw the filter controls ----
    // get claim status'
    $query = "SHOW COLUMNS FROM `claims` LIKE 'claim_status'";
    $result = db_query($query)->fetchAssoc();
    $row = (array)$result;
    
    $statuses = str_replace("'", "", $row['Type']);
    $statuses = str_replace("enum(", "", $statuses);
    $statuses = str_replace(")", "", $statuses);
    $statuses = str_replace("unpurchased,", "", $statuses);
    $statuses = explode(',', $statuses);
    array_unshift($statuses, "show all"); 
    
    // get active users
    // $query = "SELECT DISTINCT user_id FROM claims WHERE claim_status != 'unpurchased' AND deleted=0;";
    $result = db_query('SELECT DISTINCT user_id FROM claims WHERE claim_status != :a AND deleted=0',array(':a'=>'unpurchased'));
    $users = array();
    $result_rec = $result->fetchAll();
    foreach($result_rec as $row)
    {
        $row = (array)$row;
        if ( $row['user_id'] == 0 ) { continue; }
        $_user = user_load($row['user_id']);
        if ($_user) {
            $users[$row['user_id']] = $_user->profile_firstname." ".$_user->profile_lastname; 
            if ($_user->profile_legalname != '') {
                $users[$row['user_id']] .= (" (".$_user->profile_legalname.")");
            }
            if ($_user->profile_firstname == '' && $_user->profile_lastname == '' && $_user->profile_legalname == '') {
                $users[$row['user_id']] = $_user->name;
            }
        }
    }
    
    asort($users);      // sort users by value (username)
    $users = array("show all" => "Show All", "show_members"=>"Show All Real Members")+$users;

    ?>
    <div>
    <?php if (user_access('ncn auto-assign settings', $user)): ?>
    <fieldset>
        <legend>Auto Assign Claim to CE</legend>
        <form method="POST">
        <input type="hidden" name="tfunction" value="auto_assign_feature">
        <table>
        <tbody style="border:0;">
            <tr>
                <td style="width:50px;"><strong>Mode: </strong></td>
                <td style="width:50px;">
                    <?php $ce_auto_assign = variable_get('ce_auto_assign', FALSE); ?>
                    <select name="auto_assign_mode" id="auto_assign_mode">
                        <option value="0" <?php if (!$ce_auto_assign){echo "selected";} ?> >Off</option>
                        <option value="1" <?php if ( $ce_auto_assign){echo "selected";} ?> >On</option>
                    </select>
                </td>
                <td><input type="submit" value="Save"></td>
            </tr>
        </tbody>
        </table>
        </form>
    </fieldset>
    <?php endif; ?>
    
    <fieldset>
    <legend>Filter Claims</legend>
        <table>
        <tbody style="border:0;">
            <tr>
                <td style="width:150px;"><strong>Filter by Status</strong></td>
                <td>
                    <select name="status" id="filter_status_id">
                    <?php
                        foreach ($statuses as $status)
                        {
                            if ($status == $filter)
                            {   $checked = " selected ";    }
                            else
                            {   $checked = '';  }
                            ?>
                            <option value="<?= $status; ?>" <?= $checked; ?>><?= ucwords($status); ?></option>
                            <?php
                        }
                    ?>
                    </select>
                </td>
            </tr>
            <tr <?php if(ncn_admin_is_user_DriRite($user->uid)) { echo "style='display:none;'"; } ?>>
                <td><strong>Filter by User</strong></td>
                <td>
                    <select name="user" id="filter_user_id" >
                    <?php
                        foreach ($users as $user_id => $user_name)
                        {
                            if ($user_id == $filter_user)
                            {   $checked = " selected ";    }
                            else
                            {   $checked = '';  }

                            ?>
                            <option value="<?= $user_id; ?>" <?= $checked; ?>><?= $user_name ?></option>
                            <?php
                        }
                    ?>
                    </select>
                </td>
            </tr>
            <tr>
                <td><strong>Filter by Workflow</strong></td>
                <td>
                    <select id="filter_claim_workflow" name="claim_workflow">
                        <option value="all">Show All</option>
                    <?php
                    $arr_timer = get_claim_workflow_array();
                    foreach ($arr_timer as $key=>$val)
                    {
                        if ($key == $filter_claim_workflow)
                        {   $checked = " selected ";    }
                        else
                        {   $checked = '';  }
                    
                        ?>
                        <option value="<?= $key; ?>" <?= $checked; ?>><?= $val; ?></option>
                        <?php
                    }
                    
                    ?>
                    </select>
                </td>
            </tr>
            
            <tr>
                <td><strong>Filter by Claim ID</strong></td>
                <td><input type="text" class="form-text" value="" size="10" id="filter_claim_id" name="filter_claim_id" maxlength="10"></td>
            </tr>

            <tr>
                <td colspan=2><input type="button" value="Update" onclick="jump_status('<?= $base_url; ?>', '<?= $url_part; ?>');"></td>
            </tr>
        </tbody>
        </table>
    </fieldset>
    </div>
    <?php

    // ---- setup the query ----
    $query = "SELECT * FROM claims";
    $where = " WHERE claim_status != 'unpurchased' AND deleted=0";
    
    if ($filter != "show all")
    {   $where .= " AND claim_status='".$filter."' ";   }
    
    if ($filter_user == "show_members") {
        $where .= " AND user_id!=69";
    }
    else if (($filter_user != "show all") && ($filter == "show all"))
    {   $where .= " AND user_id='".$filter_user."' ";   }
    else
    if (($filter_user != "show all") && ($filter != "show all"))
    {   $where .= "AND user_id='".$filter_user."' ";    }
    
    if ($filter_claim_workflow != "all") {
        $where .= " AND workflow='".$filter_claim_workflow."' ";
    }
    
    if ($filter_claim_id != "all") {
        $where .= " AND claim_id='".$filter_claim_id."' ";
    }

    $query .= $where;
    $query .= " ORDER BY last_modified DESC";
    
    $query_total = db_query("SELECT COUNT(*) as tot FROM claims". $where)->fetchAssoc();
    $total_count = intval($query_total['tot']);

    // pagination
    $num_per_page = 50;
    $page = 0;
    if (isset($_GET['page'])) {
        $page = intval($_GET['page']);
    }
    $query .= " LIMIT ".($page*$num_per_page).", $num_per_page";
    
    $result = db_query($query);
    $row_count = $result->rowCount();
    if (user_access('ncn claim report')) {
        echo "<div style='float:left; margin-right: 20px;'><a href='$base_url/ncn/ncn_claim_report' target='_blank'>NCN Claim Report</a></div>"; 
    }
    if (user_access("ncn claim report for qa & ms")) {
        echo "<div style='float:left; margin-right: 20px;'><a href='$base_url/ncn/ncn_claim_report_qa_ms' target='_blank'>NCN Claim Report For QA&MS</a></div>"; 
    }
    if (user_access("ncn claim report for am")) {
        echo "<div style='float:left; margin-right: 20px;'><a href='$base_url/ncn/ncn_claim_report_am' target='_blank'>NCN Claim Report For Account Managers</a></div>"; 
    }
  
  echo "<div style='float:left; margin-right: 20px;'><a href='$base_url/download/claim_list'>Download Claim List</a></div>"; 
  
    echo '<div style="text-align: right">Number of Claims:&nbsp;&nbsp;'.$total_count.'</div>';
    // ---- list the claims ----
    
    $column_count = ($user->uid == 1)? 12: 11;
    $table_head = '
    <thead class="tableHeader-processed">
    <tr>
    <th></th>
        <th>Claim IDs</th>
        <th>Type</th>
        <th>Product</th>
        <th>Customer</th>
        <th>Zip Code</th>
        <th>User</th>
        <th>Created</th>
        <th>Modified</th>
        <th>Status</th>
        <th>Workflow</th>
        <th>Changes Logfile</th>'. (($user->uid == 1)? '<th></th>': '') .
    '</tr>
    </thead>

    <tbody>';

    $table_foot = '
    </tbody>
    </table>';
    
    // no results message
    if ($total_count == 0)
    {
        print "<table>";
        print $table_head;
        ?>
        <tr><td colspan="<?php echo $column_count; ?>">No results</td></tr>
        <?php
        print $table_foot;
    } else {
        echo "<form id='frm-check-mutli-claims' method='POST'>";
        echo '<table class="sticky-enabled tableSelect-processed sticky-table page_table">';
        echo $table_head;
        //for ($i=0; $i<$row_count; $i++)
        $result_rec = $result->fetchAll();
        $i = 0;
        foreach($result_rec as $row)
        {
            // get data
            $row = (array)$row;
            $claim_id = $row['claim_id'];
            $claim_type = $row['claim_type'];
      
            $claim_product = $row['claim_product']; 
            if ($claim_product=='Fire/Smoke Contents Cleaning') { $claim_product='Contents Cleaning'; }
            if ($claim_product=='Fire/Smoke Structure Cleaning'){ $claim_product='Structure Cleaning'; }
            if (ncn_cd($claim_id, 'expedite')) { $claim_product .= " (EXPEDITE)"; }
      
            $_user = user_load($row['user_id']);
            
            // check to see if this is owned (account manager is viewing)
            if (isset($GLOBALS['user']->roles[6]))
            {
                //* Account Manager 
            //  $query2 = "SELECT * FROM member_id_pool WHERE member_id=\"".$_user->profile_memberid."\";";
                $result2 = db_query('SELECT * FROM {member_id_pool} WHERE member_id=:a',array(':a'=>$_user->profile_memberid));
                $row_count2 = $result2->rowCount();
                
                if ($row_count2 == 0)
                {   $show_this = false; }
                else
                {
                    $row2 = $result2->fetchAssoc();
                    
                    
                //  $query2 = "SELECT parent FROM users_parent WHERE uid=".$row2['owner'];      // From Distributor, Get Account Manager
                    $result2 = db_query('SELECT parent FROM {users_parent} WHERE uid=:a',array(':a'=>$row2['owner']));
                    $row_count2 = $result2->rowCount();
                    $row2 = $result2->fetchAssoc();
                    
                    if ($row2['parent'] == $GLOBALS['user']->uid)
                    {   $show_this = true;      }
                    else    
                    {   $show_this = false;     }
                }
            }
            else
            {   $show_this = true;  }   
            
            // carry on
            if ($show_this == true || $show_this == false) // TEMP ** DISPLAY ALL CLAIMS
            {
            
                // carry on
                // $query2 = "SELECT * FROM claims_log WHERE claim_id=".$claim_id.";";
                $result2 = db_query('SELECT * FROM {claims_log} WHERE claim_id=:a',array(':a'=>$claim_id));
                $entries_in_log = $result2->rowCount();
                
                
                // draw row
                ?>
                <tr class="<?php if ($i%2) print "odd"; else print "even"; ?>">
                    <td><?php if ($user->uid==1): ?><input type="checkbox" name="sel_claims[]" class='check-claim' value="<?php print $claim_id; ?>" /><?php endif; ?></td>
                    <td><a href="<?= $base_url; ?>/admin/config/<?= $url_part; ?>/viewclaim/<?= $claim_id; ?>"><?= $claim_id; ?></a>
                        <?php if (ncn_admin_get_claim_first_free_locked($claim_id) == 'LOCKED') { echo '(Locked)'; } ?></td>
                    <td><?= ucwords($claim_type); ?></td>
                    <td><?= ucwords($claim_product); ?></td>
                    <td><?= ncn_cd($claim_id, 'customer_name'); ?></td>
                    <td><?= ncn_cd($claim_id, 'insured_zip'); ?></td>
                    <td><?= $_user->profile_firstname.' '.$_user->profile_lastname ?> (<?= $_user->name; ?>)</td>
                    <td><?= date("m/d/Y", $row['created']); ?></td>
                    <td><?= date("m/d/Y", $row['last_modified']); ?></td>
                    <td><?= ucwords($row['claim_status']); ?></td>
                    <td><?php 
                        $timer_trigger = render_claim_timer($claim_id, 1);
                        $workflow = render_claim_workflow($claim_id);
                        echo $workflow;
                        if ($timer_trigger != '') {
                            echo "<div>($timer_trigger)<div>";  
                        }
                        ?>
                    </td>
                    <td><a href="<?= $base_url; ?>/admin/config/<?= $url_part; ?>/viewlog/<?= $claim_id; ?>"><?= $entries_in_log; ?> entries in log</a></td>
                    <?php if ($user->uid == 1) { ?>
                        <td><a href="<?= $base_url; ?>/admin/config/<?= $url_part; ?>/deleteclaim/<?= $claim_id; ?>?dest=<?php echo urlencode(request_uri()); ?>" onclick="return delete_claim(<?= $claim_id; ?>)">Delete</a></td>
                    <?php }?>
                </tr>
                <?php
            }           
            $i++;
        }
        echo $table_foot;
    if ($user->uid==1) {
      echo "<div class='claim-select-actions'>
        <input type='hidden' name='tfunction' id='tfunction_mutli_claims' value='' />
        <a href='#' onclick='return mutli_claims_action(\"delete_multi_claims\");'>Delete</a>
      </div>";
      echo "</form>";
      drupal_add_js("jQuery(document).ready(function() {
        jQuery('#frm-check-mutli-claims input.check-claim').each(function(index) {
          jQuery(this).bind('click', check_change_mutli_claim);
        });
        check_change_mutli_claim();
      });", 'inline');
    }
        $url = $base_url."/".drupal_get_path_alias($_GET["q"]);
        echo ncn_admin_table_pagniation($url, $total_count, $num_per_page, $page, 15);
    }
}

/**
 * URL: admin/config/ncn_admin/deleteclaim/{claim_id}
 * 
 * Delete Claim (just marked as deleted claim)
 */
function ncn_admin_delete_claim($claim_id) {

    //$query = "DELETE FROM claims  WHERE claim_id =$claim_id";
    $query = "UPDATE claims SET deleted=1 WHERE claim_id =$claim_id AND deleted=0";
    $result = db_query($query);
    if ($result) {
        $msg_type = 'status';
        $msg = t('Claim(!claim_id) deleted, successfully', array('!claim_id'=>$claim_id));
    } else {
        $msg_type = 'error';
        $msg = t('Failed to delete claim(!claim_id).', array('!claim_id'=>$claim_id));
    }
    
    drupal_set_message($msg, $msg_type);
    
    // track delete of claims
    $timestamp = date('U');
    $query = "INSERT INTO track_delete_claim VALUES( NULL, $claim_id, $timestamp )";
    $result = db_query($query);
    
  if (isset($_REQUEST['dest'])) {
      $dest = $_REQUEST['dest'];
      $dest = urldecode($dest);
      drupal_goto($dest);
  }
}

/**
 * Delete Claim Permanently.
 *
 * Remove all records in DB
 */
function ncn_admin_permanent_delete_claim($claim_id, $file=false) {
    if (!$claim_id) {
        return false;
    }

    $query = "DELETE FROM claims WHERE claim_id =$claim_id AND (deleted=1 OR claim_status='unpurchased')";
    $result = db_query($query);
    if ($result->rowCount()) {
        watchdog('ncn-admin-delete-claim', 'Deleted Claim #'.$claim_id.' Permanently.');

        db_query("DELETE FROM claims_data               WHERE claim_id=$claim_id");
        db_query("DELETE FROM claims_data_extra         WHERE claim_id=$claim_id");
        // db_query("DELETE FROM claims_invoices           WHERE claim_id=$claim_id");
        db_query("DELETE FROM claims_log                WHERE claim_id=$claim_id");
        db_query("DELETE FROM claims_rooms_data_extra   WHERE claim_id=$claim_id");
        db_query("DELETE FROM claims_scope              WHERE claim_id=$claim_id");
        // db_query("DELETE FROM claim_extra_file          WHERE claim_id=$claim_id");
        db_query("DELETE FROM claim_img_captions        WHERE claim_id=$claim_id");
        db_query("DELETE FROM claim_mail_expiration     WHERE claim_id=$claim_id");
        db_query("DELETE FROM claim_note                WHERE claim_id=$claim_id");
        db_query("DELETE FROM claim_timer_trigger       WHERE claim_id=$claim_id");

        db_query("DELETE FROM free_gold_demo_claims     WHERE claim_id=$claim_id");
        db_query("DELETE FROM invoice_extended_data     WHERE claim_id=$claim_id");

        db_query("DELETE FROM ncn_ce_claims             WHERE claim_id=$claim_id");
        db_query("DELETE FROM ncn_ce_claim_accept       WHERE claim_id=$claim_id");
        db_query("DELETE FROM ncn_ce_claim_trigger      WHERE claim_id=$claim_id");
        db_query("DELETE FROM ncn_change_order          WHERE claim_id=$claim_id");

        // Remove claim files 
        if ($file) {
            // Styled Image
            $style_path = drupal_realpath("public://styles");
            $claim_folder = "/public/user_files/".$claim_id;
            deleteDir($style_path."/full_size".$claim_folder);
            deleteDir($style_path."/image_tab_upload".$claim_folder);
            deleteDir($style_path."/medium".$claim_folder);
            deleteDir($style_path."/photo_album".$claim_folder);

            // Room Image & Invoice files
            deleteDir(drupal_realpath("public://user_files/$claim_id"));
            deleteDir(drupal_realpath("public://invoices/$claim_id"));
        }
    }
}

function ncn_admin_delete_scope_file() {
GLOBAL $user, $base_url;

    // get the user id for this claim
    $claim_id = arg(2);
    
    $user_id_sql = "";
    
    // get the "live"
    $roomname = arg(3);

    // get the revision
    
    
    //get current scroll position
    $scroll_position = $_GET['current_scroll_position'];
    
//  $query = "SELECT * FROM claims_scope WHERE `claim_id`=".$claim_id." AND `roomname`=\"".$roomname."\"";
    $result = db_query('SELECT * FROM {claims_scope} WHERE claim_id=:a AND roomname=:b',array(':a'=>$claim_id,':b'=>$roomname));
    if ($result->rowCount()>0) {
        $row = $result->fetchAssoc();
//      file_delete($row['filepath']);
        @unlink(drupal_realpath($row['filepath']));
        // get live/preview
    //  $query = "DELETE FROM claims_scope WHERE `claim_id`=".$claim_id." AND `roomname`=\"".$roomname."\"";
        $result = db_query('DELETE FROM {claims_scope} WHERE claim_id=:a AND roomname=:b',array(':a'=>$claim_id,':b'=>$roomname));
    }   
    // bounce back to claim
    
    header("Location:".$base_url.base_path()."admin/config/ncn_admin/viewclaim/".$claim_id."?current_scroll_position=".$scroll_position);
    
    exit;
}

function ncn_admin_serve_scope_file()
{
GLOBAL $user;

    // get the user id for this claim
    $claim_id = arg(2);
    
    // get the "live"
    $roomname = arg(3);
//  $query = "SELECT * FROM claims_scope WHERE `claim_id`=".$claim_id." AND `roomname`=\"".$roomname."\"";
    $result = db_query('SELECT * FROM {claims_scope} WHERE claim_id=:a AND roomname=:b',array(':a'=>$claim_id,':b'=>$roomname));
    if ($result->rowCount() == 0) {
        return("Access denied to scope file, or file not found.");
    }
    
    // get data
    $row = $result->fetchAssoc();
    
    // serve file
    header('Content-Type: '.$row['filemime']);
    header('Content-Disposition: attachment;filename="'.StripSlashes($row['filename']).'"');
    header('Cache-Control: max-age=0');

    readfile($row['filepath']); 

    exit;
}

function ncn_admin_array_trim($data){
    foreach ($data as $key=>$item) {
        if (empty($item)) {
            unset($data[$key]);
        } else if (is_array($item)) {
            ncn_admin_array_trim($data[$key]);
        }
    }
}

function save_extended_data($claim_id) {
    $data = array();
    $data['claim_invoice_type'] = $_POST['claim_invoice_type'];
    $data['company_fax_number'] = $_POST['company_fax_number'];
    $data['data_entered']       = $_POST['data_entered'];
    $data['price_list']         = $_POST['price_list'];
    $data['iop_type']           = $_POST['iop_type'];
    $data['iop_number']         = $_POST['iop_number'];
    $data['type_of_loss']       = $_POST['type_of_loss'];
    $data['insurance_company']  = $_POST['insurance_company'];
    $data['claim_number']       = $_POST['claim_number'];
    $data['claim_adjuster']     = $_POST['claim_adjuster'];
    $data['cap_type']           = $_POST['cap_type'];
    $data['claim_adjuster_phone']= $_POST['claim_adjuster_phone'];
    $data['claim_adjuster_fax'] = isset($_POST['claim_adjuster_fax'])?$_POST['claim_adjuster_fax']:'';
    $data['category']           = $_POST['category'];
    $data['start_date']         = $_POST['start_date'];
    $data['completion_date']    = $_POST['completion_date'];
    $data['cause']              = $_POST['cause'];

    foreach( $data as $fname=>$fval ) {
        if ($fval != '') {
            /*$query = "INSERT INTO invoice_extended_data VALUES(NULL, $claim_id, '$fname', '$fval')";
            $result = db_query($query);*/
            save_extended_data_item($claim_id, $fname, trim((string)$fval));
        } else {
            $query = "DELETE FROM invoice_extended_data 
                      WHERE claim_id = $claim_id AND fname = '$fname'";
            $result = db_query($query); 
        }
    }
    drupal_set_message("Save invoice data, successfully.");
}

function save_extended_data_item($claim_id, $key, $value) {
    $result = db_query('SELECT * FROM {invoice_extended_data} WHERE claim_id=:a AND fname=:b',array(':a'=>$claim_id,':b'=>$key));
//  if ($result) && $row = db_fetch_array($result)) {
//      $result = db_query("UPDATE {invoice_extended_data} SET fvalue='%s' WHERE data_id=%d", $value, $row['data_id']);
//  }
    if($result->rowCount()>0)
    {
        foreach($result as $row)
        {
            $row = (array)$row;
            if($row)
            {
                $result = db_query('UPDATE {invoice_extended_data} SET fvalue=:a WHERE data_id=:b',array(':a'=>$value,':b'=>$row['data_id']));
            }
        }

    }
    else
    {
        $result = db_query('INSERT INTO {invoice_extended_data}(claim_id, fname, fvalue) VALUES(:a,:b,:c)', array(':a'=>$claim_id,':b'=>$key,':c'=>$value));
    }
}

function get_extended_data($claim_id) {
    $data = array();
//  $query = "SELECT * FROM invoice_extended_data WHERE claim_id = $claim_id";
    $result = db_query('SELECT * FROM {invoice_extended_data} WHERE claim_id=:a',array(':a'=>$claim_id));
    /*$row_count = $result->rowCount();
    for($i=0; $i<$row_count; $i++) */
    foreach($result as $row)
    {
        $row = (array)$row;
        $data[$row['fname']] = $row['fvalue'];
    }
    
    //* auto-populated
    if (!isset($data['company_fax_number']) || trim($data['company_fax_number']) == '') {
        $_user = ncn_admin_get_user_from_claim_id($claim_id);
        if ($_user) {
            $data['company_fax_number'] = $_user->profile_company_fax;
        }
    }
    return $data;
}

/**
 * URL: admin/config/ncn_admin/track_delete_claims
 * 
 * Deleted Claims
 */
function track_delete_claims_page() {
global $base_url;
    if (arg(4) == 'undelete') {
        undelete_claim_action(intval(arg(5)));
    }
    
    drupal_add_js(drupal_get_path('module', 'ncn_admin') . '/ncn_admin.js');
    
//  $query = "SELECT * FROM track_delete_claim ORDER BY timestamp DESC";
    $result = db_query('SELECT * FROM {track_delete_claim} ORDER BY timestamp DESC');
    $row_count = $result->rowCount();
    
    $table_head = '
    <thead class="tableHeader-processed">
        <th>Claim ID</th>
        <th>User</th>
        <th>Date</th>
        <th></th>
    </thead>
    <tbody>';

    $table_foot = '
    </tbody>
    </table>';
    
    ob_start();
    
    // no results message
    if ($row_count == 0)
    {
        print "<table>";
        print $table_head;
        ?>
        <tr><td colspan=2>No results</td></tr>
        <?php
        print $table_foot;
        
    }
    
    $count = 0;
    $num_per_page = 50;
    $on_page = 0;
    $current_page = 1;

    for ($i=0; $i<$row_count; $i++)
    { 
        // get data
        $row = $result->fetchAssoc();
        $claim_id = $row['claim_id'];
        
    //  $query = "SELECT * FROM claims WHERE claim_id=$claim_id";
        $result1 = db_query('SELECT * FROM {claims} WHERE claim_id=:a',array(':a'=>$claim_id));
        if(($claim_count=$result1->rowCount())>0) 
        {
            $claim = $result1->fetchAssoc();
            $_user = user_load($claim['user_id']);
            if ($claim['deleted']==0) {
                continue;
            }
        }
    // header?
        if ($on_page == 0)
        {
            if ($current_page == 1)
            {   ?><table class="sticky-enabled tableSelect-processed sticky-table page_table" id="page_table_<?= $current_page; ?>"><?php   }
            else
            {   ?><table class="sticky-enabled tableSelect-processed sticky-table page_table"  id="page_table_<?= $current_page; ?>" style="display:none;"><?php    }
                
            print $table_head;  
        }
    
        // draw row
        ?>
        <tr class="<?php if ($count%2) print "even"; else print "odd"; ?>">
            <?php if ($claim_count==0) : ?>
                <td><?= $claim_id; ?></td>
            <?php else: ?>
                <td><a href="#" onclick="toggle_tr('track_deleted_claim_<?php echo $claim_id;?>')"><?= $claim_id; ?></a></td>
            <?php endif; ?>
            <td><?php if($claim_count) { echo $_user->profile_firstname.' '.$_user->profile_lastname.' ('.$_user->name.')'; } ?></td>
            <td><?= date('d M Y H:i:s', $row['timestamp']); ?></td>
            <td>
                <?php if ($claim_count > 0) : ?>
                    <a href="<?= $base_url; ?>/admin/config/ncn_admin/track_delete_claims/undelete/<?php echo $claim_id?>">Undelete</a>
                <?php else: ?>
                    Undelete
                <?php endif; ?>
            </td>
        </tr>
        <?php if ($claim_count>0) : ?>
        <tr class="<?php if ($count%2) print "even"; else print "odd"; ?> hidden-tr">
            <td></td>
            <td colspan="2">
                <div id="track_deleted_claim_<?php echo $claim_id;?>" class="hidden-body" >
                    <?php echo draw_ncn_claim_data($claim_id); ?>
                </div>
            </td>
            <td></td>
        </tr>
        <?php endif; ?>
        
        <?php
        
        // update page count            
        $count++;
        $on_page++;
        
        // check end of page
        if ($on_page == $num_per_page)
        {
            ?>
            <?php               
        
            print $table_foot;
            
            $on_page = 0;
            $current_page += 1;
        }
    }
        
    if ($on_page == 0) {
        $current_page -= 1;
    }
    // end pagination
    ?>
    <table class="sticky-enabled tableSelect-processed sticky-table">
    <tbody>
    <tr>
        <td colspan="7" align="right">
            
            <?php
                if ($current_page > 1) {
                    echo t("Page:");
                
                    for ($i=1; $i<= $current_page; $i++)
                    {
                        if ($i > 1)
                        {   
                            print " | ";
                            $page_text = '<u>'.$i.'</u>';   
                        }
                        else
                        {   $page_text = '<strong><u>'.$i.'</u></strong>';  }   
                        
                        
                        ?>
                        <a href="#" onclick="change_list_page(<?= $i; ?>);" id="page_link_<?= $i; ?>"><?= $page_text; ?></a>
                        <?php
                    }
                }
                //$current_page;
            ?>
        </td>
    </tr>
    </tbody>
    </table>
    <?php
    
    // javascript list of page
    ?>
    <script type="text/javascript">
        var total_list_pages = <?= $current_page; ?>;
    </script>
    <?php   
    $content = ob_get_contents();
    ob_end_clean();
    
    return $content;
}

/*
 * Undelet Claim
 */
function undelete_claim_action($claim_id) {
//  $query = "DELETE FROM track_delete_claim WHERE claim_id=$claim_id";
    $result = db_query('DELETE FROM {track_delete_claim} WHERE claim_id=:a',array(':a'=>$claim_id));
    if (!$result) {
        handle_mysql_syntax_error('30110-NCN-ADMIN', $query);
        return FALSE;
    }
    
//  $query = "UPDATE claims SET deleted=0 WHERE claim_id=$claim_id AND deleted=1";
    $result = db_query('UPDATE {claims} SET deleted=0 WHERE claim_id=:a AND deleted=1',array(':a'=>$claim_id));
    if (!$result) {
        handle_mysql_syntax_error('30110-NCN-ADMIN', $query);
        return FALSE;
    }
    
    drupal_set_message(t('Claim(#%claim_id) undeleted, successfully.', array('%claim_id'=>$claim_id)));
    
    return TRUE;
}


function draw_ncn_claim_data($claim_id) {
//  $query = "SELECT * FROM claims_data WHERE claim_id=".$claim_id." AND field_type!='hidden' AND field_type!='submit' AND field_type!='token' ORDER BY `weight` ASC;";
    $result = db_query('SELECT * FROM {claims_data} WHERE claim_id=:a AND field_type!=:b AND field_type!=:c AND field_type!=:d ORDER BY weight ASC',
    array(':a'=>$claim_id,':b'=>'hidden',':c'=>'submit',':d'=>'token'));
    //$row_count = $result->rowCount();
    //var_dump($row_count);

    ob_start();
    // ---- draw data list ----
    ?>
    
    <table>
    <tbody style="border:solid 0px;">
    <?php
    
    //for ($i=0; $i<$row_count; $i++)
    foreach($result as $row)
    {
        // get the data
        //
        $row = (array)$row;
        $row['val'] = unserialize($row['val']);
        $res = "";

        if (!isset($row['val']['type']))
        {   $row['val']['type'] = "scope";  }       
        
    
        // convert the data
        switch ($row['val']['type'])
        {
            case "checkboxes":
                $res = "";
                
                foreach ($row['val'] as $k=> $v)
                {
                    if (trim($v) != "0")
                    {   $res .= "<li> ".$v; }
                }
                
                if ($res != "")
                {   $res = "<ul>".$res."</ul>";     }
            break;
        
            case "phone_field_phone":
                $res = implode(" ", $row['val']);
            break;

            case "time_field_time":
                $res = render_claim_time($row['val']);
            break;

            case "date":
                $res = render_claim_date($row['val']);
            break;
        
            case "image_upload_element":
                $image_url = $base_url."/".$row['val']->filepath; 
                $img_info = image_get_info($row['val']->filepath);
                
                $res = "<a href='".$image_url."?width=".$img_info['width']."&height=".$img_info['height']."&iframe=false' class='colorbox-load'>"; 
                $res .= "<img src='".str_replace('default/files', 'default/files/imagecache/ncnadmin', $image_url)."' />";
                $res .= "</a>"; 
                $res .= "<br /><small style='margin:0;padding:0;'>(click to enlarge)</small>";
            break;
            
            case "scope": break;
            default:
            
                if (is_array($row['val']['val']))
                {
                    $res = implode(" ", $row['val']['val']);
                }
                else
                {
                    $res = $row['val']['val'];
                    
                }
            break;
        }
        
        // display the data
        if ($res != "") {
        ?>
        <tr>
            <td width="300"><?= ucwords(str_replace("_", " ", $row['key'])); ?></td>
            <td><?= $res; ?></td>
        </tr>
        <?php
        }
    }
    ?>
    </tbody>
    </table>
    
<?php
    $content = ob_get_contents();
    ob_end_clean();
    return $content;
}
