<?php
function ncn_admin_invoiceform_3_submit($form, &$form_state)
{
    $live = $form_state['values']['live'];
    $claim_id = arg(4);

    // we know the file was good
    $invoice_dir = 'public://invoices/'.$claim_id;
    if (is_dir($invoice_dir)===false) {
        @drupal_mkdir($invoice_dir);
    }

    $file = file_load($form_state['values']['fid']);
    $newFilename = @strtr($file->filename,array("( "=>"", " )"=>"", "("=>"", ")"=>"", " "=>"_"));
    
    if(isset($form_state['clicked_button']['#post']['current_scroll_position']))
    {
        $_SESSION['current_scroll_position'] = $form_state['clicked_button']['#post']['current_scroll_position'];
    }
    $file->status = FILE_STATUS_PERMANENT;
    $res = file_move($file, "public://invoices/{$claim_id}/{$newFilename}");
    // add invoice to database
    if ($res)
    {
        if ($live == 3 || $live == 4) {
            $res_file = convert_pdf_to_jpg($res, $invoice_dir);
            if ($res_file === FALSE) {
                drupal_set_message('Failed to convert uploaded PDF file to Image', 'error');
                return;
            }
            $file = $res_file;
        }else{
            $file = $res;
        }
        
        // get last revision
        //echo $query = "SELECT * FROM claims_invoices WHERE claim_id=".$claim_id." AND `live`=".$live." ORDER BY revision DESC";
        $result = db_query('SELECT * FROM {claims_invoices} WHERE claim_id=:a AND live=:b ORDER BY revision DESC',
        array(':a'=>$claim_id,':b'=>$live));
        $row_count = $result->rowCount();

        if($live == 11){
            $row_count = 0;
        }

        if ($row_count == 0)
        {
            $file->filepath = str_replace("public://","sites/default/files/",$file->uri);

            $revision = 0;  
        //  $query = "INSERT INTO claims_invoices VALUES(NULL, ".$claim_id.", ".$revision.", \"".AddSlashes($file->filename)."\", \"".AddSlashes($file->filepath)."\", \"".AddSlashes($file->filemime)."\", ".date('U').", ".$live.");";
            $query = db_query('INSERT INTO {claims_invoices} VALUES(NULL,:a,:b,:c,:d,:e,:f,:g)',
                array(':a'=>$claim_id,':b'=>$revision,':c'=>$file->filename,':d'=>$file->filepath,':e'=>$file->filemime,
                    ':f'=>date('U'),':g'=>$live));
        }
        else
        {
            //$row = (array)$result;
            //$revision = $row['revision']+1;
            $row = $result->fetchAssoc();
            $revision = 0;

            /*Create File Object from File path*/
            $uri = str_replace("sites/default/files/","public://",$row['filepath']);
            $fileObj = createFileObject($uri);

            file_delete($fileObj);
            $file->filepath = str_replace("public://","sites/default/files/",$file->uri);

            $query = db_query('UPDATE {claims_invoices} SET filename=:a, filepath=:b, filemime=:c, timestamp=:d 
                                        WHERE claim_id=:e AND live=:f',
                                array(':a'=>$file->filename,
                                      ':b'=>$file->filepath,
                                      ':c'=>$file->filemime,
                                      ':d'=>date('U'),
                                      ':e'=>$claim_id,
                                      ':f'=>$live));
        }
        
        $result = $query;
        
        // success!
        $claim_file_title = ncn_admin_get_claim_file_title($live);
        drupal_set_message( $claim_file_title.' "'.$file->filename.'" was uploaded.', 'status');    
        $log_message = t("!user_name uploaded !file_title  : !file_name", array('!user_name'=>ncn_amin_get_user_role_and_name(), '!file_title'=>$claim_file_title, '!file_name'=>$file->filename));
        ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
    }
}
function ncn_admin_invoiceform_4_submit($form, &$form_state)
{
    ncn_admin_invoiceform_3_submit($form, $form_state);
}
function ncn_admin_invoiceform_5_submit($form, &$form_state)
{
    ncn_admin_invoiceform_3_submit($form, $form_state);
}


//------------------------------------------------------------------------------
function ncn_admin_invoiceform_2_submit($form, &$form_state)
{
    ncn_admin_invoiceform_1_submit($form, $form_state);
}
function ncn_admin_invoiceform_10_submit($form, &$form_state)
{
    ncn_admin_invoiceform_3_submit($form, $form_state);
}
function ncn_admin_invoiceform_11_submit($form, &$form_state)
{
    ncn_admin_invoiceform_3_submit($form, $form_state,'esx');
}
function ncn_admin_invoiceform_12_submit($form, &$form_state)
{
    ncn_admin_invoiceform_3_submit($form, $form_state);
}

// upload invoice form SUBMIT
function ncn_admin_invoiceform_1_submit($form, &$form_state)
{
    $live = $form_state['values']['live'];
    
    if(isset($form_state['clicked_button']['#post']['current_scroll_position']))
    {
        $_SESSION['current_scroll_position'] = $form_state['clicked_button']['#post']['current_scroll_position'];
    } 
    
    $claim_id = arg(4);
    
    // we know the file was good
    //$file = file_save_upload('invoice_upload'.$live);
    $file = file_load($form_state['values']['fid']);
    $newFilename = @strtr($file->filename,array("( "=>"", " )"=>"", "("=>"", ")"=>"", " "=>"_"));
    
    $filePath = "sites/default/files/invoices/$claim_id/";
    $claim_dir = DRUPAL_ROOT."/".$filePath;
    
    if (!is_dir($claim_dir)) {
        mkdir($claim_dir, 0755);
    }
    
    $file->status = FILE_STATUS_PERMANENT;
    $res = file_move($file, "public://invoices/{$claim_id}/{$newFilename}", FILE_EXISTS_RENAME);
    
    // add invoice to database
    if ($res)
    {
        // get last revision
    //  $query = "SELECT revision FROM claims_invoices WHERE claim_id=".$claim_id." AND `live`=".$live." ORDER BY revision DESC LIMIT 0,1";
        $result = db_query('SELECT revision FROM {claims_invoices} WHERE claim_id=:a AND live=:b ORDER BY revision DESC LIMIT 0,1',
        array(':a'=>$claim_id,':b'=>$live));

        $row_count = $result->rowCount();
        
        if ($row_count == 0)
        {   $revision = 0;  }
        else
        {
            $row = $result->fetchAssoc();
            $revision = $row['revision']+1;
        }

        // insert new data
    //  $query = "INSERT INTO claims_invoices VALUES(NULL, ".$claim_id.", ".$revision.", \"".AddSlashes($file->filename)."\", \"".AddSlashes($file->filepath)."\", \"".AddSlashes($file->filemime)."\", ".date('U').", ".$live.");";
        $result = db_query('INSERT INTO {claims_invoices} VALUES(NULL,:a,:b,:c,:d,:e,:f,:g)',
            array(':a'=>$claim_id,':b'=>$revision,':c'=>$res->filename,':d'=>str_replace("public://","sites/default/files/",$res->uri),
                ':e'=>$res->filemime,':f'=>date('U'),':g'=>$live));


        $claim_file_title = ncn_admin_get_claim_file_title($live);
        // add entry to logfile
        
        $log_message = ncn_amin_get_user_role_and_name()." uploaded new file: ".$res->filename." ( $claim_file_title - revision #".$revision. " )";
        ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
        
        // success!
        $log_message = "Uploaded new file: ".$file->filename." ( $claim_file_title - revision #".$revision. " )";
        drupal_set_message($log_message, 'status');
        
        if ($live == 1 && ncn_admin_is_claim_first_free($claim_id) ) {
            $base_abs_path = $_SERVER['DOCUMENT_ROOT'];
            $final_invoice_name = $file->filepath;
            // $command = "java -jar C:/pdfwatermark/ncn_pdf_watermark.jar $base_abs_path/$final_invoice_name";
            // $command = "java -jar ../pdfwatermark/ncn_pdf_watermark.jar $base_abs_path/$final_invoice_name";
            $command = "java -jar ".$_SERVER["DOCUMENT_ROOT"]."/pdfwatermark/ncn_pdf_watermark.jar $base_abs_path/$final_invoice_name";

            $output = shell_exec($command);
            //$final_locked_invoice_name = ncn_admin_get_locked_invoice_filename($final_invoice_name);
        }

        // Final Invoice
        if ($live == 1) {
            // Auto Post in Claim Feed
            $msg = "NCN uploaded new invoice file ($res->filename).";
            ncn_chatter_auto_post_in_claim($claim_id, FEED_TYPE_CLAIM, $msg, $res->fid);
        }
    }
}

//------------------------------------------------------------------------------
function ncn_admin_invoiceform_3_validate($form, &$form_state)
{
    $live = $form_state['values']['live'];

    // check file was uploaded
    $file = file_save_upload('invoice_upload'.$live);

    if (!$file)
    {
        // If you want to require it, you'll want to do it here... something like this:
        form_set_error('invoice_upload'.$live, 'File missing for upload.');
        $form_state['values']['fid'] = 0;
    } else {
        $form_state['values']['fid'] = $file->fid;
    }

    switch ($live)
    {
        case 3:
        case 4:
        case 5: if ($file->filemime != "application/pdf") {
                    form_set_error('invoice_upload'.$live, 'Only PDF file could be uploaded.');
                }
        break;
    }
}

function ncn_admin_invoiceform_4_validate($form, &$form_state)
{
    ncn_admin_invoiceform_3_validate($form, $form_state);
}
function ncn_admin_invoiceform_5_validate($form, &$form_state)
{
    ncn_admin_invoiceform_3_validate($form, $form_state);
}
function ncn_admin_invoiceform_10_validate($form, &$form_state)
{
    ncn_admin_invoiceform_3_validate($form, $form_state);
}
function ncn_admin_invoiceform_11_validate($form, &$form_state)
{
    ncn_admin_invoiceform_3_validate($form, $form_state);
}
function ncn_admin_invoiceform_12_validate($form, &$form_state)
{
    ncn_admin_invoiceform_3_validate($form, $form_state);
}

function ncn_admin_invoiceform_2_validate($form, &$form_state)
{
    ncn_admin_invoiceform_1_validate($form, $form_state);
}

// upload invoice form VALIDATE
function ncn_admin_invoiceform_1_validate($form, &$form_state)
{
    $live = $form_state['values']['live'];

    /*if(isset($form_state['#post']['current_scroll_position']))
    {
        $_SESSION['current_scroll_position'] = $form_state['#post']['current_scroll_position'];
    } */

    // check file was uploaded
    $file = file_save_upload('invoice_upload'.$live);

    if (!$file)
    {
        // If you want to require it, you'll want to do it here... something like this:
        form_set_error('invoice_upload'.$live, 'File missing for upload.');
        $form_state['values']['fid'] = 0;       
    } else {
        $form_state['values']['fid'] = $file->fid;
    }  
}


//------------------------------------------------------------------------------
// upload invoice form display
function ncn_admin_invoiceform_1($form_state)
{

    $form = array();
    $form['#attributes'] = array('enctype' => "multipart/form-data");

    $form['invoice_upload1'] = array(
        '#type' => 'file',
        '#title' => t(''),
    );
    
    $form['live'] = array(
        '#type' => 'hidden',
        '#value' => 1,
    );

    $form['current_scroll_position'] = array(
        '#type' => 'hidden',
        '#attributes' => array('class' => t('current_scroll_position')),
        '#value' => t(''),
    );

    $form['next'] = array('#type' => 'submit', '#weight' => 101, '#value' => t('Upload Final Invoice'), '#attributes' => array('onclick' => 'return on_set_scroll_position()'));

    return($form);

}

//------------------------------------------------------------------------------
// upload invoice form display
function ncn_admin_invoiceform_2($form_state)
{

    $form = array(); //'#onclick' => t('return on_set_scroll_position();'), 
    $form['#attributes'] = array('enctype' => "multipart/form-data");

    $form['invoice_upload2'] = array(
        '#type' => 'file',
        '#title' => t(''),
    );
    
    $form['live'] = array(
        '#type' => 'hidden',
        '#value' => 2,
    );

    $form['current_scroll_position'] = array(
        '#type' => 'hidden',
        '#attributes' => array('class' => t('current_scroll_position')),
        '#value' => t(''),
    );

    $form['next'] = array('#type' => 'submit', '#weight' => 101, '#value' => t('Upload Claim Documents'), '#attributes' => array('onclick' => 'return on_set_scroll_position()'));
    
    return($form);

}
//------------------------------------------------------------------------------
// upload invoice form display
function ncn_admin_invoiceform_3($form_state)
{

    $form = array();
    $form['#attributes'] = array('enctype' => "multipart/form-data");

    $form['invoice_upload3'] = array(
        '#type' => 'file',
        '#title' => t(''),
        '#attributes' => array('accept'=> 'application/pdf'),
    );
    
    $form['live'] = array(
        '#type' => 'hidden',
        '#value' => 3,
    );

    $form['current_scroll_position'] = array(
        '#type' => 'hidden',
        '#attributes' => array('class' => t('current_scroll_position')),
        '#value' => t(''),
    );

    $form['next'] = array('#type' => 'submit', '#weight' => 101, '#value' => t('Upload Service Contract'), '#attributes' => array('onclick' => 'return on_set_scroll_position()'));

    return($form);

}
//------------------------------------------------------------------------------
// upload invoice form display
function ncn_admin_invoiceform_4($form_state)
{

    $form = array();
    $form['#attributes'] = array('enctype' => "multipart/form-data");

    $form['invoice_upload4'] = array(
        '#type' => 'file',
        '#title' => t(''),
        '#attributes' => array('accept'=> 'application/pdf'),
    );
    
    $form['live'] = array(
        '#type' => 'hidden',
        '#value' => 4,
    );

    $form['current_scroll_position'] = array(
        '#type' => 'hidden',
        '#attributes' => array('class' => t('current_scroll_position')),
        '#value' => t(''),
    );

    $form['next'] = array('#type' => 'submit', '#weight' => 101, '#value' => t('Upload Certificate of Completion'), '#attributes' => array('onclick' => 'return on_set_scroll_position()'));

    return($form);

}
//------------------------------------------------------------------------------
// upload invoice form display
function ncn_admin_invoiceform_5($form_state)
{

    $form = array();
    $form['#attributes'] = array('enctype' => "multipart/form-data");

    $form['invoice_upload5'] = array(
        '#type' => 'file',
        '#title' => t(''),
        '#attributes' => array('accept'=> 'application/pdf'),
    );
    
    $form['live'] = array(
        '#type' => 'hidden',
        '#value' => 5,
    );

    $form['current_scroll_position'] = array(
        '#type' => 'hidden',
        '#attributes' => array('class' => t('current_scroll_position')),
        '#value' => t(''),
    );

    $form['next'] = array('#type' => 'submit', '#weight' => 101, '#value' => t('Upload Completed Invoice'), '#attributes' => array('onclick' => 'return on_set_scroll_position()'));

    return($form);

}
//------------------------------------------------------------------------------
// upload ce claim file
function ncn_admin_invoiceform_10($form_state)
{

    $form = array();
    $form['#attributes'] = array('enctype' => "multipart/form-data");

    $form['invoice_upload10'] = array(
        '#type' => 'file',
        '#title' => t(''),
    );
    
    $form['live'] = array(
        '#type' => 'hidden',
        '#value' => 10,
    );

    $form['current_scroll_position'] = array(
        '#type' => 'hidden',
        '#attributes' => array('class' => t('current_scroll_position')),
        '#value' => t(''),
    );

    $form['next'] = array('#type' => 'submit', '#weight' => 101, '#value' => t('Upload CE Documents'), '#attributes' => array('onclick' => 'return on_set_scroll_position()'));

    return($form);

}
//------------------------------------------------------------------------------
// upload ce esx file
function ncn_admin_invoiceform_11($form_state)
{

    $form = array();
    $form['#attributes'] = array('enctype' => "multipart/form-data");

    $form['invoice_upload11'] = array(
        '#type' => 'file',
        '#title' => t(''),
    );
    
    $form['live'] = array(
        '#type' => 'hidden',
        '#value' => 11,
    );

    $form['current_scroll_position'] = array(
        '#type' => 'hidden',
        '#attributes' => array('class' => t('current_scroll_position')),
        '#value' => t(''),
    );


    $form['next'] = array('#type' => 'submit', '#weight' => 101, '#value' => t('Upload ESX File'), '#attributes' => array('onclick' => 'return on_set_scroll_position()'));

    return($form);

}
//------------------------------------------------------------------------------
// upload invoice form display
function ncn_admin_invoiceform_12($form_state)
{

    $form = array();
    $form['#attributes'] = array('enctype' => "multipart/form-data");

    $form['invoice_upload12'] = array(
        '#type' => 'file',
        '#title' => t(''),
    );
    
    $form['live'] = array(
        '#type' => 'hidden',
        '#value' => 12,
    );

    $form['current_scroll_position'] = array(
        '#type' => 'hidden',
        '#attributes' => array('class' => t('current_scroll_position')),
        '#value' => t(''),
    );

    $form['next'] = array('#type' => 'submit', '#weight' => 101, '#value' => t('Upload CE Invoice PDF File'), '#attributes' => array('onclick' => 'return on_set_scroll_position()'));

    return($form);

}

function ncn_admin_invoice_data($claim_id, $data, $pvc_disabled='') {
    global $base_url;
    if (!isset($data['data_entered']))              { $data['data_entered'] = ''; }
    if (!isset($data['price_list']))                { $data['price_list'] = ''; }
    if (!isset($data['iop_number']))                { $data['iop_number'] = ''; }
    if (!isset($data['insurance_company']))         { $data['insurance_company'] = ''; }
    if (!isset($data['claim_number']))              { $data['claim_number'] = ''; }
    if (!isset($data['claim_adjuster']))            { $data['claim_adjuster'] = ''; }
    if (!isset($data['claim_adjuster_phone']))      { $data['claim_adjuster_phone'] = ''; }
    if (!isset($data['claim_adjuster_fax']))        { $data['claim_adjuster_fax'] = ''; }
    if (!isset($data['start_date']))                { $data['start_date'] = ''; }
    if (!isset($data['completion_date']))           { $data['completion_date'] = ''; }
?>
<script type="text/javascript">
jQuery(function() {
    ncn_admin_invoice_data_datepicker('#invoice_extended_data #start_date');
    ncn_admin_invoice_data_datepicker('#invoice_extended_data #completion_date');
});
function ncn_admin_invoice_data_datepicker(objStr) {
    jQuery( objStr ).datepicker({
        showOn: "button",
        buttonImage: "<?php echo $base_url."/".drupal_get_path('module', 'ncn_report')."/images/calendar.gif"; ?>",
        buttonImageOnly: true,
        dateFormat: 'mm/dd/yy',
    });
}
</script>
<fieldset class="">
    <legend>Claim Invoice Data</legend>
    <div>
    <form id="invoice_extended_data" method="post" style=" margin:0px; ">
        <input type="hidden" name="extend_data_function" value="extend_data" />
        <input type="hidden" name="current_scroll_position" class="current_scroll_position" value="" />
        <table style="border: solid 0px;">
        <tbody style="border:0px;">
            <tr class="even"><td><label for="claim_invoice_type">Claim Invoice Type: &nbsp;</label></td><td><?php echo draw_select_claim_invoice_type('claim_invoice_type', isset($data['claim_invoice_type'])?$data['claim_invoice_type']:''); ?></td></tr>
            
            <tr class="odd"><td><label for="company_fax_number">Company Fax Number: &nbsp;</label></td><td><input id="company_fax_number" name="company_fax_number" size="20" value="<?php echo isset($data['company_fax_number'])?$data['company_fax_number']:''; ?>" /></td></tr>

            <tr class="even"><td width="30%"><label for="data_entered">Data Entered: &nbsp;</label></td><td><input id="data_entered" name="data_entered" size="20" value="<?php echo isset($data['data_entered'])?$data['data_entered']:''; ?>" /></td></tr>
            <tr class="odd"><td width="30%"><label for="price_list">Price List: &nbsp;</label></td><td><input id="price_list" name="price_list" size="20" value="<?php echo isset($data['price_list'])?$data['price_list']:''; ?>"  /></td></tr>
            <tr class="even"><td width="40%"><label for="iop_type">Insured's Other Phone: &nbsp;</label></td><td>
                <select id="iop_type" name="iop_type" >
                    <option value="Work" <?php if(isset($data['iop_type'])&&$data['iop_type']=="Work") {echo "selected";}?> >-Work-</option>
                    <option value="Mobile" <?php if(isset($data['iop_type'])&&$data['iop_type']=="Mobile") {echo "selected";}?> >-Mobile-</option>
                </select>
                <input id="iop_number" name="iop_number" size="12" value="<?php echo isset($data['iop_number'])?$data['iop_number']:''; ?>"  />
            </td></tr>
            <tr class="odd"><td><label for="type_of_loss">Type of Loss: &nbsp;</label></td><td>
                <select id="type_of_loss" name="type_of_loss" style="width: 100px;"  >
                    <option value="Water" <?php if(isset($data['type_of_loss'])&&$data['type_of_loss']=="Water") {echo "selected";}?>>- Water -</option>
                    <option value="Mold" <?php if(isset($data['type_of_loss'])&&$data['type_of_loss']=="Mold") {echo "selected";}?>>- Mold -</option>
                </select>
            </td></tr>
            <tr class="even"><td><label for="insurance_company">Insurance Company: &nbsp;</label></td><td><input id="insurance_company" name="insurance_company" size="20" value="<?php echo isset($data['insurance_company'])?$data['insurance_company']:''; ?>"  /></td></tr>
            <tr class="odd"><td><label for="claim_number">Claim Number: &nbsp;</label></td><td><input id="claim_number" name="claim_number" size="20" value="<?php echo isset($data['claim_number'])?$data['claim_number']:''; ?>"  /></td></tr>
            
            <tr class="even" ><td><label for="claim_adjuster">Claim Adjuster: &nbsp;</label></td><td><input id="claim_adjuster" name="claim_adjuster" size="20" value="<?php echo isset($data['claim_adjuster'])?$data['claim_adjuster']:''; ?>"  /></td></tr>
            <tr class="odd"><td><label for="claim_adjuster_phone">Claim Adjuster Phone: &nbsp;</label></td><td>
                <select id="cap_type" name="cap_type" >
                    <option value="Work" <?php if(isset($data['cap_type'])&&$data['cap_type']=="Work") {echo "selected";}?> >-Work-</option>
                    <option value="Mobile" <?php if(isset($data['cap_type'])&&$data['cap_type']=="Mobile") {echo "selected";}?> >-Mobile-</option>
                </select>
                <input id="claim_adjuster_phone" name="claim_adjuster_phone" size="20" value="<?php echo isset($data['claim_adjuster_phone'])?$data['claim_adjuster_phone']:''; ?>"  /></td></tr>
            <tr class="even"><td><label for="claim_adjuster_fax">Claim Adjuster Fax: &nbsp;</label></td><td><input id="claim_adjuster_fax" name="claim_djuster_fax" size="20" value="<?php echo isset($data['claim_adjuster_fax'])?$data['claim_adjuster_fax']:''; ?>" /></td></tr>

            <tr class="odd"><td><label for="category">Category: &nbsp;</label></td><td>
                <select id="category" name="category" style="width: 100px;"  >
                    <option value="1" <?php if(isset($data['category'])&&$data['category']=="1") {echo "selected";}?> >-1-</option>
                    <option value="2" <?php if(isset($data['category'])&&$data['category']=="2") {echo "selected";}?> >-2-</option>
                    <option value="3" <?php if(isset($data['category'])&&$data['category']=="3") {echo "selected";}?> >-3-</option>
                </select>
            </td></tr>
            <tr class="even"><td><label for="start_date">Start Date: &nbsp;</label></td><td><input id="start_date" name="start_date" size="20" value="<?php echo isset($data['start_date'])?$data['start_date']:''; ?>" /></td></tr>
            <tr class="odd"><td><label for="completion_date">Completion Date: &nbsp;</label></td><td><input id="completion_date" name="completion_date" size="20" value="<?php echo isset($data['completion_date'])?$data['completion_date']:''; ?>" /></td></tr>
            <tr class="even"><td><label for="cause" style="vertical-align: top;">Cause:&nbsp;</label></td><td><textarea id="cause" name="cause" style="width: 90%; height: 100px;" ><?php echo isset($data['cause'])? $data['cause']:''; ?></textarea></td></tr>
            <?php if ($pvc_disabled!='disabled'): ?>
                <tr class="odd"><td colspan="2"><input type="submit" onclick="return on_set_scroll_position();" value="  Save  "/></td></tr>
            <?php endif; ?>
        </tbody>
        </table>
    </form>
    </div>
</fieldset>
<?php
}

function ncn_admin_draw_invoice_upload($claim_id, $type, $pvc_disabled='')
{
    GLOBAL $base_url;
    ob_start();
    $legend_title = "";
    switch ($type)
    {
        case "1":
            $legend_title = "Upload Final Invoice";
        break;

        case "2":
            $legend_title = "Upload Claim Documents";
        break;
        
        case "3":
            $legend_title = "Upload Service Contract";
        break;
        case "4":
            $legend_title = "Upload Certificate of Completion";
        break;
        case "5":
            $legend_title = "Upload Completed Invoice";
        break;
        
        case "10":
            $legend_title = "Upload CE Documents";
            break;
        case "11":
            $legend_title = "Upload ESX File";
            break;
        case "12":
            $legend_title = "Upload CE Invoice PDF File";
            break;

    }

?>
    <div>
        <fieldset class="">
            <legend class="">
                <?php echo $legend_title; ?>
            </legend>
            <div class="">
            <?php
                if ($pvc_disabled!='disabled') {
                    $ncn_admin_invoiceform = drupal_get_form('ncn_admin_invoiceform_'.$type);
                    print drupal_render($ncn_admin_invoiceform);
                }
            ?>
            
                <div id="invoice_revisions">
                <table>
                <tbody>
                    <thead class="tableHeader-processed">
                    <tr>
                        <th>Revision</th>
                        <th>File</th>
                        <th>Timestamp</th>
                        <th>Action</th>
                    </tr>
                    </thead>
                    
                    <?php
                    // get latest 5 invoice revisions
                    
                    // $query = "SELECT * FROM claims_invoices WHERE claim_id=".$claim_id." AND `live`=".$type." ORDER BY revision DESC LIMIT 0,5";
                    $result = db_query('SELECT * FROM {claims_invoices} WHERE claim_id=:a AND `live`= :b ORDER BY revision DESC,invoice_id DESC LIMIT 0,5',array(':a'=>$claim_id ,':b'=>$type));
                    $row_count = $result->rowCount();
                    
                    // run through invoice revisions
                    //for ($i=0; $i<$row_count; $i++)
                    $result_rec = $result->fetchAll();
                    $i = 0;
                    foreach($result_rec as $row)
                    {
                        $row = (array)$row;
                        $id = uniqid();
                        
                        if ($row['revision'] == 0)  // don't want to show "revision #0")
                        {   $revision = ""; }
                        else
                        {   $revision = " Rev. #".$row['revision']; }

                        ?>              
                        <tr class="<?php if ($i%2) print "even"; else print "odd"; ?>">
                            <td><?= $revision; ?></td>
                            <td><img src="<?= $base_url; ?>/sites/all/modules/ncn_claims_manager/images/pdf_icon_16.gif" width="16" height="16" align="left" />&nbsp;<a href="<?= $base_url; ?>/account/serve_invoice_file/<?= $claim_id; ?>/<?= $row['live']; ?>/<?= $row['revision']; ?>/<?= $row['invoice_id']; ?>"><?= StripSlashes($row['filename']); ?></a></td>
                            <td><?= date("m/d/Y H:i:s", $row['timestamp']); ?></td>
                            <td><?php if ($pvc_disabled!='disabled'): ?>
                                <a class='invoice_file_<?=$id?>' href="<?= $base_url; ?>/account/delete_invoice_file/<?= $claim_id; ?>/<?= $row['live']; ?>/<?= $row['revision']; ?>/<?= $row['invoice_id']; ?>" onclick='set_scroll_position("invoice_file_<?=$id ?>"); return confirm("Are you sure you want to delete this file?");'>[delete]</a>
                                <?php endif; ?>
                            </td>
                        </tr>
                        <?php
                        $i++;
                    }
                    
                    // no results
                    if ($row_count == 0)
                    {
                        ?>
                        <tr>
                            <td coslpan=3>No files uploaded</td>
                        </tr>
                        <?php
                    }
                                
                    ?>
                </tbody>
                </table>
                </div>
            </div>
        </fieldset>
            
        

    </div>
    <?php

    $content = ob_get_contents();
    ob_end_clean();
    
    return $content;
}

//------------------------------------------------------------------------------
// upload invoice form
function ncn_admin_upload_invoice($form_state)
{
    $form = array();
    $form['#attributes'] = array('enctype' => "multipart/form-data");

    $form['invoice'] = array(
        '#type' => 'file',
        '#title' => t("New Claim Invoice"),
        '#tree' => FALSE,
        '#size' => 40,
    );

    $form['next'] = array('#type' => 'submit', '#weight' => 101, '#value' => t('Save Invoice'));
    return $form;
    
}

function _add_pdf_invoice($file, $claim_id) {
    
    $live = 1;
    // get last revision
//  $query = "SELECT revision FROM claims_invoices WHERE claim_id=".$claim_id." AND `live`=".$live." ORDER BY revision DESC LIMIt 0,1";
    $result = db_query('SELECT revision FROM {claims_invoices} WHERE claim_id=:a AND live=:b ORDER BY revision DESC LIMIt 0,1',
    array(':a'=>$claim_id,':b'=>$live));
    $row_count = $result->rowCount();
    
    if ($row_count == 0)
    {   $revision = 0;  }
    else
    {
        $row = $result->fetchAssoc();
        $revision = $row['revision']+1;
    }
    
    // insert new data
    // $query = "INSERT INTO claims_invoices VALUES(NULL, ".$claim_id.", ".$revision.", \"".AddSlashes($file['filename'])."\", \"".AddSlashes($file['filepath'])."\", \"".AddSlashes($file['filemime'])."\", ".date('U').", ".$live.");";
    $result = db_query('INSERT INTO {claims_invoices} VALUES(NULL,:a,:b,:c,:d,:e,:f,:g)',
        array(':a'=>$claim_id,':b'=>$revision,':c'=>$file['filename'],':d'=>$file['filepath'],':e'=>$file['filemime'],
            ':f'=>date('U'),':g'=>$live));
    
    // Inser file_managed entry
    global $user;
    $db_file = new stdClass;
    $db_file->uid       = $user->uid;
    $db_file->filename  = $file['filename'];
    $db_file->uri       = str_replace("sites/default/files/", "public://", $file['filepath']);
    $db_file->status    = 1;
    $db_file->filemime  = $file['filemime'];
    file_save($db_file);

    // add entry to logfile
    if ($live == 1)
    {   $log_message = ncn_amin_get_user_role_and_name()." created new file: ".$file['filename'].", revision #".$revision;    }
    else
    {   $log_message = ncn_amin_get_user_role_and_name()." created new file: ".$file['filename'].", revision #".$revision;    }
    
    ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
    // Auto Post - Create Invoice
    $msg = "NCN created new invoice file ($db_file->filename).";
    ncn_chatter_auto_post_in_claim($claim_id, FEED_TYPE_CLAIM, $msg, $db_file->fid);
}

/**
Generate Final Invoice
*/
function generate_final_invoice($claim_id) {
    $final_invoice_name = "";

    $dir_url = variable_get('file_directory_path', 'sites/default/files') . "/invoices/$claim_id/";
    
    if (!is_dir($dir_url)) {
        mkdir($dir_url, 0755);
    }
    
    $file3_info = get_claim_file($claim_id, 3);     // Service Contract
    $file4_info = get_claim_file($claim_id, 4);     // Certificate of Completion
    $file5_info = get_claim_file($claim_id, 5);     // Completed Invoice
    
    $additional_file_removed = false;
    $scope_pdf_count = 0;
    
    $base_abs_path = $_SERVER['DOCUMENT_ROOT'];
    $foption = "";
    $error = false;
    try{
        // 1st pdf
        $first_pdf_url = _get_first_pdf($claim_id, $dir_url);
        if ($first_pdf_url == '[no-coversheet]') {
            // skip first pdf(cover sheet)
        }   else if ($first_pdf_url) {
            $foption .= " -f $base_abs_path/$first_pdf_url";
        } else {
            $error = true;
        }
        // && $_SERVER['REMOTE_ADDR']!='103.240.34.101'
        if (!$error) {
            // 2th pdf  (Completed Invoice)
            if (!empty($file5_info)) {
                $foption .= " -f $base_abs_path/".$file5_info['filepath'];
            }
            // 3rd pdf
            if (!empty($file3_info) || !empty($file4_info)) {
                $third_pdf_url = _get_third_pdf($file3_info, $file4_info, $dir_url);
                if (!empty($third_pdf_url)) {
                    $foption .= " -f $base_abs_path/$third_pdf_url";
                }
            }
            /*if (!empty($file3_info)) {
                $foption .= " -f $base_abs_path/".$file3_info['filepath'];
            }
            // 4th pdf
            if (!empty($file4_info)) {
                $foption .= " -f $base_abs_path/".$file4_info['filepath'];
            }*/
            
            // Additional Documents
            $additional_file_removed = TRUE;
            $pdf_file_additional_pdfs = _get_additional_docs_pdf($claim_id, $dir_url, $additional_file_removed);
            if ($pdf_file_additional_pdfs) {
                $foption .= " -f $base_abs_path/$pdf_file_additional_pdfs";
            }
            
            // 5th pdf
            $pdf_file_url_pattern = _get_scope_pdf($claim_id, $dir_url, $scope_pdf_count);
            if ($scope_pdf_count) {
                for ($index = 0; $index<$scope_pdf_count; $index+=1) {
                    $pdf_file_url = $pdf_file_url_pattern.$index.".pdf";
                    $foption .= " -f $base_abs_path/$pdf_file_url";
                }
            }
        }
    } catch(Exception $ex) {
        drupal_set_message($ex->getMessage(), 'error');
        $error = true;
    }
    
    if (!$error) {
        
        $final_invoice_filename = 'invoice_'.date('U').'.pdf';
        $final_invoice_name = $dir_url.$final_invoice_filename;
        $out_option = " -o $base_abs_path/$final_invoice_name";
        
        //drupal_set_message($foption);
        
        /*if($_SERVER['REMOTE_ADDR']=="103.240.34.101"){
            echo $foption;
        }*/
        
                
        if ($foption != "") {
            //$command = "java -jar C:/pdfsam/lib/pdfsam-console-2.3.1e.jar $foption $out_option concat";
            //$command = "java -jar ../pdfsam/lib/pdfsam-console-2.3.1e.jar $foption $out_option concat";
            $command = "java -jar ".$_SERVER["DOCUMENT_ROOT"]."/pdfsam/lib/pdfsam-console-2.3.1e.jar $foption $out_option concat";
            
            $output = shell_exec($command);
            
            if (!is_file($final_invoice_name)) {
                drupal_set_message('Failed to create an invoice.', 'error');
                //drupal_set_message('You need to upload essential files to create an invoice.', 'error');
                $error = true;
            } else {
            
                if (ncn_admin_is_claim_first_free($claim_id)) {
                    // generate locked pdf // 2011-08-02
                    //$command = "java -jar C:/pdfwatermark/ncn_pdf_watermark.jar $base_abs_path/$final_invoice_name";
                    //$command = "java -jar ../pdfwatermark/ncn_pdf_watermark.jar $base_abs_path/$final_invoice_name";
                    $command = "java -jar ".$_SERVER["DOCUMENT_ROOT"]."/pdfwatermark/ncn_pdf_watermark.jar $base_abs_path/$final_invoice_name";

                    $output = shell_exec($command);
                    $final_locked_invoice_name = ncn_admin_get_locked_invoice_filename($final_invoice_name);
                    
                    if (!is_file($final_locked_invoice_name)) {
                        drupal_set_message('Failed to create an locked invoice.', 'error');
                    }
                }
            }
        }
        else{
            $error = true;
        }
    }
    
    // clear temporary files
    if (isset($first_pdf_url) && $first_pdf_url && $first_pdf_url!='[no-coversheet]') {  
        $first_pdf_file = new stdClass();
        $first_pdf_file->filepath = $first_pdf_url;
        $first_pdf_file->filename = basename($first_pdf_url);
        $first_pdf_file->uri = str_replace("sites/default/files/","public://",$first_pdf_url);
        //file_delete($first_pdf_url);
        file_delete($first_pdf_file);
    }
    
    if (isset($third_pdf_url) && $third_pdf_url) {   
        $third_pdf_file = new stdClass();
        $third_pdf_file->uri = str_replace("sites/default/files/","public://",$third_pdf_url);
        //file_delete($third_pdf_url);
        file_delete($third_pdf_file);
    }   
    
    if ($additional_file_removed && isset($pdf_file_additional_pdfs) && $pdf_file_additional_pdfs ) {
        $additional_pdf_file = new stdClass();
        $additional_pdf_file->uri = str_replace("sites/default/files/","public://",$pdf_file_additional_pdfs);
        //file_delete($pdf_file_additional_pdfs);
        file_delete($additional_pdf_file);
    }
    
    if ($scope_pdf_count) {
        for ($index = 0; $index<$scope_pdf_count; $index+=1) {
            $pdf_file_url = $pdf_file_url_pattern.$index.".pdf";
            $scope_pdf_file = new stdClass();
            $scope_pdf_file->uri = str_replace("sites/default/files/","public://",$pdf_file_url);
            //file_delete($pdf_file_url);
            file_delete($scope_pdf_file);
        }
    }
    
    if ($error) { return; }
    
    drupal_set_message("Final Invoice Created, Successfully.");
    drupal_set_message("File : $final_invoice_name");
    if (isset($final_locked_invoice_name)&&is_file($final_locked_invoice_name)) {
        drupal_set_message("Locked Invoice File : $final_locked_invoice_name");
    }
    
    $file_info = array(
        'filename' => $final_invoice_filename,
        'filepath' => $final_invoice_name,
        'filemime' => 'application/pdf'
    );
    _add_pdf_invoice($file_info, $claim_id);
}

function ncn_admin_get_locked_invoice_filename($fname) {
    return substr($fname, 0, -4)."_locked.pdf";
}

function _get_format_phone_string($str) {
    if (trim($str) == "") {
        return '';
    }
    $str = str_replace('-', '', $str);
    $str1 = substr($str, 0, 3);
    $str2 = substr($str, 3, 3);
    $str3 = substr($str, 6);
    $str = "$str1-$str2-$str3";
    
    return $str;
}

function _get_format_tax_id_string($str) {
    if (trim($str) == "") {
        return '';
    }
    $str = str_replace('-', '', $str);
    $str1 = substr($str, 0, 2);
    $str2 = substr($str, 2);
    $str = "$str1-$str2";
    
    return $str;
}

function _get_first_pdf($claim_id, $dir_url) {
    
    $pdf_file_url = $dir_url.time()."_first.pdf";
    //$template_first_page = "invoice_cover_sheet.doc";
    
    //get essential data
    $invoice_data = get_extended_data($claim_id);   
    $claim_invoice_type = isset($invoice_data['claim_invoice_type'])?$invoice_data['claim_invoice_type']:'';

    if ( strpos($claim_invoice_type , "*no*") !== false ) {
        return '[no-coversheet]';
    }
    
    $claim_data = get_claim_data($claim_id);
    $_user = user_load($claim_data['user_id']);
    $data = array();
    
    $data['company_name']           = $_user->profile_legalname;    //'ABC Restoration, Inc';
    $data['tax_id_number']          = $_user->profile_taxid;        //'12-3456789';
    
    $data['company_address1']       = $_user->profile_address;      //'123 Company Lane';
    $data['company_address2']       = $_user->profile_city.', '.$_user->profile_state.' '.$_user->profile_zip;      //  'Dallas, TX. 75201';
    $data['company_office_phone']   = _get_format_phone_string($_user->profile_officephone);    //'555-555-1234';
    
    $data['client_name']            = ncn_cd($claim_id, 'customer_name');   //'Robert Smith';
    $data['property1']              = ncn_cd($claim_id, 'insured_address'); //'123 Company Lane';
    $data['property2']              = ncn_cd($claim_id, 'insured_city').', '.ncn_cd($claim_id, 'insured_state').' '.ncn_cd($claim_id, 'insured_zip');       //'Dallas, TX. 75201';
    $data['home_phone']             = _get_format_phone_string(ncn_cd($claim_id, 'insured_phone_number'));
    
    $data['date_of_loss']           = ncn_cd($claim_id, 'date_of_loss');    //'12/07/10';
    
    $data = array_merge($invoice_data, $data);
    
    $logo = ncn_admin_get_member_logo_info($_user->profile_memberid);
    $b_logo = true;
    if (empty($logo)) { $b_logo = false; }
    $result = ncn_admin_make_cover_sheet($pdf_file_url, $data, $b_logo);

    if(is_file($pdf_file_url)) {
        
        /********* add Member Logo into coversheet page ************/
        if (empty($logo)) {
            return $pdf_file_url;
        }
        $file = new stdClass();
        $file->filepath = $pdf_file_url;
        $file->filename = basename($pdf_file_url);
        $file->filemime = "application/pdf";        
        $file->uri = str_replace("sites/default/files/","public://",$file->filepath);
        
        $logo_params = array(
                'x' => 100,
                'y' => 50,
                'width' => 350,
                'height' => 150,
                'valign' => 'bottom',
            );
            
        $final_cv_pdf = generate_pdf_with_member_logo($_user->profile_memberid, $file, $logo_params);

        //file_delete($pdf_file_url);
        if ($final_cv_pdf) {
            return $final_cv_pdf;
        } else {
            drupal_set_message('Failed to make a cover sheet with logo.', 'error');
        }
    } else if ($result !== FALSE) {
        drupal_set_message('Failed to make a cover sheet.', 'error');
    }
    
    return '';
    
}

function _get_third_pdf($file3, $file4, $dir_url) {
    $both_image = 0;
    if (!empty($file3['filepath']) && is_file($file3['filepath']) ) {
        if ( strtolower(substr($file3['filepath'], -4)) == ".jpg" ) {
            $both_image += 3;
        }
    }
    if (!empty($file4['filepath']) && is_file($file4['filepath']) ) {
        if ( strtolower(substr($file4['filepath'], -4)) == ".jpg" ) {
            $both_image += 4;
        }
    }
    if ( $both_image==0 ) {
        return '';
    }
    
    $pdf_file_url = $dir_url.time()."_third.pdf";
    
    $pdf=new NCNPDF();
    $pdf->AddPage();
    if ($both_image == 7) {
        $pdf->Image($file3['filepath'], 0, 20, 120);
        $pdf->Image($file4['filepath'], 120, 20, 80);
    } else if ($both_image == 3) {
        $pdf->Image($file3['filepath'], 0, 20, 200);
    } else if ($both_image == 4) {
        $pdf->Image($file4['filepath'], 0, 20, 200);
    }
    
    $pdf->Output($pdf_file_url, 'F');
    
    if(is_file($pdf_file_url)) {
        return $pdf_file_url;
    } else {
        drupal_set_message('Failed to make a \"Service Contact & Certificate of Completion" page.', 'error');
    }
    return '';
}

function _get_scope_pdf($claim_id, $dir_url, &$pdf_count) {
    $scope_file = get_claim_scope_file($claim_id);
    
//  $query = "SELECT * FROM claims_data WHERE claim_id=".$claim_id." AND `key`='scope'";
    $result = db_query('SELECT * FROM {claims_data} WHERE claim_id=:a AND `key`=:b',array(':a'=>$claim_id,':b'=>'scope'));
    $row_count = $result->rowCount();
    
    if ($row_count == 0) { return ''; }
    
    // get the data
    $row = $result->fetchAssoc();
    $data = unserialize($row['val']);
    
    $customer_name = ncn_cd($claim_id, 'customer_name');
    
    $pdf=new NCNPDF();
  
    $pdf_count = 0;
    $room_count = 0;
    
    foreach ($data as $roomname => $roomdata)
    {
        if ($roomname != "type")
        {
            $room_count += 1;
        }
    }
    
    $scope_per_pdf = 5;
    $pdf_count = 0;
    $page_count = 0;
    $pdf_file_url_pattern = $dir_url.time()."_scope_";
    $i = 0;
    
    // search first room and render
    if (!ncn_admin_is_photo_first_room_deleted($claim_id)) {
    foreach ($data as $roomname => $roomdata)
    {
        if ($roomname != "type")
        {
            $default_room_name = ncn_admin_claim_get_default_room_name($claim_id);
            if ($roomname == $default_room_name) {
                $first_flag = true;
            }
            
            if ($first_flag) {
                if ($i % $scope_per_pdf == 0 && $page_count) {
                    if ($pdf) {
                        $pdf_file_url = $pdf_file_url_pattern.$pdf_count.".pdf";
                        $pdf->Output($pdf_file_url, 'F');
                        
                        $pdf_count += 1;
                    }
                    $pdf=new NCNPDF();
                    $page_count = 0;
                }
            
        if (!ncn_admin_invoice_check_default_room_data_exist($claim_id, $roomdata)) { break; }
                $pdf->AddPage();    
                $page_count++;
        
                $roomdata['captions'] = _get_image_captions($claim_id, $roomname, 3);
                $pdf->Header_Invoice($customer_name);           
                $pdf->Photo_Table_Invoice($roomdata);
                
                $i += 1;
                break;
            }
        }
    }
    }
    
    // other rooms
    foreach ($data as $roomname => $roomdata)
    {
        if ($roomname != "type")
        {
            $first_flag = false;
            $default_room_name = ncn_admin_claim_get_default_room_name($claim_id);
            if ($roomname == $default_room_name) {
                $first_flag = true;
            }
            
            if(!$first_flag) {
                if ($i % $scope_per_pdf == 0 && $page_count) {
                    if ($pdf) {
                        $pdf_file_url = $pdf_file_url_pattern.$pdf_count.".pdf";
                        $pdf->Output($pdf_file_url, 'F');
                        
                        $pdf_count += 1;
                    }
                    $pdf=new NCNPDF();
                    $page_count = 0;
                }
            
                if (!ncn_admin_invoice_check_room_data_exist($claim_id, $roomname, $roomdata)) { continue; }
                $pdf->AddPage();    
                $page_count++;
        
                $roomdata['captions'] = _get_image_captions($claim_id, $roomname, 6);
                $pdf->Header_Invoice($customer_name." - ".$roomname);           
                $top2 = $pdf->RoomPhoto_Table_Invoice($roomdata);
                if (isset($scope_file[$roomname])) {
                    $pdf->draw_scope_file($scope_file[$roomname], $top2);
                } else {
                    /** Dry Log ScopeSheet**/
                    $ss_data = array();
                    $ss_eit = ncn_admin_get_ss_eit_list($claim_id, $roomname);
                    $ss_smr = ncn_admin_get_ss_smr_list($claim_id, $roomname); 
                    $ss_ep  = ncn_admin_get_ss_ep_list($claim_id, $roomname);
                    $ss_smr_standard    = ncn_admin_get_ss_structural_moisture_readings($claim_id, $roomname);
                    ncn_admin_array_trim($ss_smr_standard);
                    
                    if (!empty($ss_eit)) { $ss_data['eit'] = $ss_eit; }
                    if (!empty($ss_smr)) { $ss_data['smr'] = $ss_smr; }
                    if (!empty($ss_ep )) { $ss_data['ep']  = ncn_admin_ss_ep_print_format_data($ss_ep);  }
                    if (!empty($ss_smr_standard)) { $ss_data['smr_standard'] = $ss_smr_standard; }
                    
                    if (!empty($ss_data)) {
                        $pdf->SetY($top2);
                        $pdf->InsertScopesheetData($ss_data);
                    }
                }
                
                $i += 1;
            }
        }
    }
    
    
    if ($pdf && $page_count) {
        $pdf_file_url = $pdf_file_url_pattern.$pdf_count.".pdf";
        $pdf->Output($pdf_file_url, 'F');       
        $pdf_count += 1;
    }
    
    //$scope_link = '<a href="http://www.netclaimsnow.com/'.$pdf_file_url.'" target="_blank">Scopesheet PDF</a>';
    //drupal_set_message(t($scope_link), 'status');
    return $pdf_file_url_pattern;
}

function ncn_admin_invoice_check_default_room_data_exist($claim_id, $roomdata) {
  $ret = FALSE;
  for ($i=0; $i<3; $i++) {
    $img_index = 'image'.$i;
    if (isset($roomdata[$img_index])&&is_file($roomdata[$img_index]['path'])) {
      $ret = TRUE; break;
    }
  }
  return $ret;
}
function ncn_admin_invoice_check_room_data_exist($claim_id, $roomname, $roomdata) {
    //* photo album//
    $ret = FALSE;
    for ($i=0; $i<6; $i++) {
        $img_index = 'image'.$i;
        if (isset($roomdata[$img_index])&&is_file($roomdata[$img_index]['path'])) {
            $ret = TRUE; break;
        }
    }
    
    //ScopeSheet
    if (!$ret) {
        $scope_file = get_claim_scope_file($claim_id);
        if (isset($scope_file[$roomname])) {
            $ret = TRUE;
        } else {
            $ss_data = array();
            $ss_eit = ncn_admin_get_ss_eit_list($claim_id, $roomname);
            $ss_smr = ncn_admin_get_ss_smr_list($claim_id, $roomname); 
            $ss_ep  = ncn_admin_get_ss_ep_list($claim_id, $roomname);
            $ss_smr_standard    = ncn_admin_get_ss_structural_moisture_readings($claim_id, $roomname);
            ncn_admin_array_trim($ss_smr_standard);
            
            if (!empty($ss_eit)) { $ss_data['eit'] = $ss_eit; }
            if (!empty($ss_smr)) { $ss_data['smr'] = $ss_smr; }
            if (!empty($ss_ep )) { $ss_data['ep']  = ncn_admin_ss_ep_print_format_data($ss_ep);  }
            if (!empty($ss_smr_standard)) { $ss_data['smr_standard'] = $ss_smr_standard; }
            
            if (!empty($ss_data)) {
                $ret = TRUE;
            }
        }
    }
    return $ret;
}
