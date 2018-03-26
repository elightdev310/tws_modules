<?php
function ncn_admin_edit_roomname()
{
    $claim_id = $_POST['claim_id'];
    $old_roomname = $_POST['old_roomname'];
    $new_roomname = $_POST['new_roomname'];
    $j_result = ncn_admin_photo_rename_room($claim_id, $old_roomname, $new_roomname);
    echo json_encode($j_result);
}

function ncn_admin_photo_rename_room($claim_id, $old_roomname, $new_roomname)
{
    $new_roomname = trim(preg_replace("/[^a-zA-Z0-9\s]/", "", $new_roomname));
    $error = false;
    $j_result['msg'] = '';
    $j_result['code'] = "success";
//  $query = "SELECT * FROM claims_data WHERE claim_id=" . $claim_id . " AND `key`='scope'";
    $result = db_query('SELECT * FROM {claims_data} WHERE claim_id=:a AND `key`=:b',array(':a'=>$claim_id,':b'=>'scope'));
    if ($result->rowCount() == 0)
    {
        $error = true;
        $j_result['msg'] = t('Claim(#!claim_id) doesn\'t exist.', array(
            '!claim_id' => $claim_id
        ));
        $j_result['code'] = 'NCN_ADMIN_001_EDIT_ROOMNAME';
    }
    else
    if ($old_roomname != $new_roomname)
    {
        $row = (array)$result->fetchAssoc();
        $row['val'] = unserialize($row['val']);
        if (isset($row['val'][$new_roomname]))
        {
            $error = true;
            $j_result['msg'] = t('Roomname(!roomname) is duplicated.', array(
                '!roomname' => $new_roomname
            ));
            $j_result['code'] = 'NCN_ADMIN_005_EDIT_ROOMNAME';
        }
        else
        {
            $new_scope = array();
            $find = false;
            foreach($row['val'] as $roomname => $roomdata)
            {
                if ($roomname == $old_roomname)
                {
                    $new_scope[$new_roomname] = $roomdata;
                    $find = true;
                }
                else
                {
                    $new_scope[$roomname] = $roomdata;
                }
            }

            if (!$find)
            {
                $error = true;
                $j_result['msg'] = t('Claim(#!claim_id) hasn\'t a room(!roomname).', array(
                    '!claim_id' => $claim_id,
                    '!roomname' => $old_roomname
                ));
                $j_result['code'] = 'NCN_ADMIN_002_EDIT_ROOMNAME';
            }
            else
            {
                $new_scope = serialize($new_scope);
                $scope = $new_scope;
            //  $query = "UPDATE claims_data SET `val`='" . mysql_real_escape_string($new_scope) . "' WHERE claim_id=$claim_id AND `key`='scope'";
                $result = db_query('UPDATE {claims_data} SET val=:a WHERE claim_id=:b AND `key`=:c',array(':a'=>$scope,':b'=>$claim_id,':c'=>'scope'));
                if ($result->rowCount()==0)
                {
                    $error = true;
                    $j_result['msg'] = t('Failed to rename room(!roomname).', array(
                        '!roomname' => $old_roomname
                    ));
                    $j_result['code'] = 'NCN_ADMIN_003_EDIT_ROOMNAME';
                }
                else
                {
                    ncn_admin_update_room_data_by_renaming_room($claim_id, $old_roomname, $new_roomname);
                    $log_message = t('Room(!roomname) renamed to !new_roomname, successfully.', array(
                        '!roomname' => $old_roomname,
                        '!new_roomname' => $new_roomname
                    ));
                    $j_result['msg'] = $log_message;
                    $log_message = t('!user_name renamed room(!roomname) to !new_roomname.', array(
                        '!user_name' => ncn_amin_get_user_role_and_name() ,
                        '!roomname' => $old_roomname,
                        '!new_roomname' => $new_roomname
                    ));
                    ncn_admin_insert_claim_log($claim_id, date('U') , $log_message);
                }
            }
        }
    }

    return $j_result;
}

function ncn_admin_update_room_data_by_renaming_room($claim_id, $room_name, $new_name)
{

    // Image Captions
    $result = db_query('UPDATE {claim_img_captions} SET roomname=:a WHERE claim_id=:b AND roomname=:c',
        array(':a'=>$new_name,':b'=>$claim_id,':c'=>$room_name));

    // Scope Image
    $result = db_query('UPDATE {claims_scope} SET roomname=:a WHERE claim_id=:b AND roomname=:c',
        array(':a'=>$new_name,':b'=>$claim_id,':c'=>$room_name));

    // Scopesheet Data
    $result = db_query('UPDATE {claims_room_data_extra} SET room_name=:a WHERE claim_id=:b AND room_name=:c',
        array(':a'=>$new_name,':b'=>$claim_id,':c'=>$room_name));
}

function _get_image_captions($claim_id, $roomname, $img_count){
//  $query = "SELECT * FROM claim_img_captions WHERE claim_id=$claim_id AND roomname = '$roomname'";
    $result = db_query('SELECT * FROM {claim_img_captions} WHERE claim_id=:a AND roomname = :b',
        array(':a'=>$claim_id,':b'=>$roomname));
    if ($result->rowCount()>0) {
        $row = $result->fetchAssoc();
        $data = unserialize($row['data']);
        return $data;
    }
    
    // set default values
    $data = array();
    if ($img_count == 3) {
        $data[0] = 'Exterior of Dwelling';
        $data[1] = 'Cause & Origin';
        $data[2] = 'Driver\'s License';
    } else {
        $data[0] = 'Pre Mitigation';
        $data[1] = 'Mitigation';
        $data[2] = 'Post Mitigation';
        
        $data[3] = 'Before Reading';
        $data[4] = 'During Reading';
        $data[5] = 'After Reading';
    }
    
    return $data;
    
    
}

function _save_image_captions($claim_id, $roomname, $data){
//  $query = "SELECT * FROM claim_img_captions WHERE claim_id=$claim_id AND roomname = '$roomname'";
    $result = db_query('SELECT * FROM {claim_img_captions} WHERE claim_id=:a AND roomname = :b',
        array(':a'=>$claim_id,':b'=>$roomname));
    $data = serialize($data);
    if ($result->rowCount()>0) {
        $query1 = db_query('UPDATE {claim_img_captions} SET data=:a WHERE claim_id=:b AND roomname=:c',
            array(':a'=>$data,':b'=>$claim_id ,':c'=>$roomname));
    } else {
        $query1 = db_query('INSERT INTO {claim_img_captions} (claim_id, roomname, data) VALUES (:a,:b,:c)',
            array(':a'=>$claim_id,':b'=>$roomname,':c'=>$data));
    }
    $result1 = $query1;
    if (!$result1) {
        return FALSE;
    }
    return TRUE;
    
}
/**
 * type : 3 or 6
 */
function _get_blank_room_image_url($type, $index){
    global $base_url;
    $image_url = $base_url.'/'.drupal_get_path('module', 'ncn_create_claim').'/images/';
    if ($type == 3) {
        switch ($index) {
            case 0: $image_url .= 'fronhouse.png';  break;
            case 1: $image_url .= 'origin.png';     break;
            case 2: $image_url .= 'license.png';    break;
        }
    } else {
        $image_url .= ('blank'.($index+1).'.png');
    }
    
    return $image_url;
}
    
function convert_pdf_to_jpg($file, $dir_url) {
    
    $old_file_path = $file->uri;
    
    $base_abs_path = $_SERVER['DOCUMENT_ROOT'];
    $pdf_full_path = drupal_realpath($file->uri);
    $folder_full_path = drupal_realpath($dir_url);
    $jpg_prefix = $file->filename."-";
    
    
    //$command = "java -jar c:/AspriseJavaPDF-free.jar $pdf_full_path $folder_full_path $jpg_prefix";
    // $command = "java -jar ../AspriseJavaPDF-free.jar $pdf_full_path $folder_full_path $jpg_prefix";
    $command = "java -jar ".$_SERVER["DOCUMENT_ROOT"]."/AspriseJavaPDF-free.jar $pdf_full_path $folder_full_path $jpg_prefix";
    //$command = "c:/pdf2image -o $folder_full_path $pdf_full_path --format jpg --prefix $jpg_prefix --verb 3 ";
    //$command = "./../pdf2image -o $folder_full_path $pdf_full_path --format jpg --prefix $jpg_prefix --verb 3 ";
    watchdog('java-cmd', $command);
    $output = shell_exec($command);

    //echo $output;
    //exit;
    file_delete($file);
    $error = false;
    $file->uri .=".jpg";
    $file->filename .=".jpg";
    $file->filemime = "image/jpg";
    $file->filepath = str_replace("public://", "sites/default/files/", $file->uri);

    if (!is_file("$dir_url/$jpg_prefix"."1.jpg")){
    
    if (is_file("$dir_url/$jpg_prefix".".jpg")){
        $res = copy("$dir_url/$jpg_prefix".".jpg" , drupal_realpath($file->uri));
        if (!$res) { $error=true; }
        } else {
            return FALSE;       
        }
    } else {
        $res = copy("$dir_url/$jpg_prefix"."1.jpg" , drupal_realpath($file->uri));
        if (!$res) { $error=true; }
    }
    
    if (is_file("$dir_url/$jpg_prefix".".jpg")){
        @unlink("$dir_url/$jpg_prefix".".jpg");
    }
    
    for ($i=1; $i<300; $i++) {
        if (is_file("$dir_url/$jpg_prefix"."$i.jpg")){
            @unlink("$dir_url/$jpg_prefix"."$i.jpg");
        } else {
            break;
        }
    }   
    if ($error) {
        return FALSE;
    }   
    return $file;
}

function convert_bmp_to_jpg($file) {

    $filepath = drupal_realpath($file->uri);
    $mi = ImageCreateFromBMP($filepath);
    $file->uri .= ".jpg";
    $file->filename .= ".jpg";
    $file->filemime = "image/jpg";
    $result = imagejpeg($mi,$filepath);
    
    if ($result) {
        return $file;
    }
    return FALSE;
}

/**
 * Claim Scope Form
 */
function ncn_admin_scopeform($claim_id, $roomname)
{
    ob_start();
?>

<p class="description" style="font-weight: bold;">
    If you don't upload scopesheet for final invoice, then we would generate this scopesheet using scopesheet data when creating a final invoice.
</p>

<form id="ncn_admin_scopeform_<?php echo $roomname; ?>" class="attach-description" enctype="multipart/form-data" method="post">
    <input type="file" size="60" id="ncn_scope_file_<?php echo $roomname; ?>" name="files[ncn_scope_file_<?php echo $roomname; ?>]" accept="image/jpg, image/jpeg, image/gif, image/png, image/bmp" />
    <input type="hidden" name="roomname" value="<?php echo $roomname; ?>" />
    <input type="hidden" name="scope_upload_function" value="scope" />
    <input type="hidden" name="current_scroll_position" class="current_scroll_position" value="" />
    <input type="submit" onclick="return on_set_scroll_position();" value="Upload Exact Scope Image File" />
</form>
<p class="description">
    Upload cropped scope image file using this form. When creating invoice, uploaded image file is used, directly.
</p>

<form id="ncn_admin_auto_scopeform_pdf_<?php echo $roomname; ?>" class="attach-description" enctype="multipart/form-data" method="post">
    <input type="file" size="60" id="ncn_scope_auto_pdf_file_<?php echo str_replace(' ', '_', $roomname); ?>" name="files[ncn_scope_auto_pdf_file_<?php echo $roomname; ?>]" accept="application/pdf" />
    <input type="hidden" name="roomname" value="<?php echo $roomname; ?>" />
    <input type="hidden" name="scope_upload_function" value="scope_auto_full_pdf" />
    <input type="hidden" name="current_scroll_position" class="current_scroll_position" value="" />
    <input type="submit" onclick="return on_set_scroll_position();" value="Upload Full Scope PDF File And Crop Automatically"  />
</form>
<p class="description">
    Upload full scope pdf file using this form. When uploading, scope image file would be cut from scope pdf, automatically.
</p>

<form id="ncn_admin_scopeform_pdf_<?php echo $roomname; ?>" class="attach-description" enctype="multipart/form-data" method="post">
    <input type="file" size="60" id="ncn_scope_pdf_file_<?php echo str_replace(' ', '_', $roomname); ?>" name="files[ncn_scope_pdf_file_<?php echo $roomname; ?>]" accept="application/pdf" />
    <!--<input type="hidden" name="roomname" value="<?php echo $roomname; ?>" />
    <input type="hidden" name="scope_upload_function" value="scope_full_pdf" /> -->
    <input type="hidden" name="current_scroll_position" class="current_scroll_position" value="" />
    <input type="button" value="Upload Full Scope PDF File And Crop Manually" onclick="return on_click_upload_scope_full_pdf('<?php echo $GLOBALS['base_url']; ?>', 'ncn_scope_pdf_file_<?php echo str_replace(' ', '_', $roomname); ?>', '<?php echo $claim_id; ?>', '<?php echo $roomname; ?>' ); " />
</form>
<p class="description">
    Upload full scope pdf file using this form. After uploading, you need to cut from scope file, manually.
</p>
<?php 
    $content = ob_get_contents();
    ob_end_clean();
    
    return $content;
}

function ncn_admin_scopeform_submit($roomname) {
    //$roomname = $form_state['values']['roomname'];
    $claim_id = arg(4);
    
    // we know the file was good
    $source = 'ncn_scope_file_'.$roomname;
    
    if(isset($form_state['clicked_button']['#post']['current_scroll_position']))
    {
        $_SESSION['current_scroll_position'] = $form_state['clicked_button']['#post']['current_scroll_position'];
    } 
    
    $file = file_save_upload($source);
    
    if (!$file) {
        drupal_set_message("File of missing for upload (room name :$roomname)", "error");
        return;
    } else if ($file->filemime != 'image/jpeg' && $file->filemime != 'image/jpg' && $file->filemime != 'image/gif' && $file->filemime != 'image/png' && strtolower(substr($file->filename, -4)) != ".bmp") {
        drupal_set_message("Scope file should be only Image (room name: $roomname)", "error");
        file_delete($file);
        return;
    }

    $filePath = "sites/default/files/invoices/$claim_id/";
    $claim_dir = DRUPAL_ROOT."/".$filePath;
    
    if (!is_dir($claim_dir)) {
        mkdir($claim_dir, 0755);
    }

    $res = file_copy($file, "public://invoices/{$claim_id}", FILE_EXISTS_RENAME);
    file_delete($file);

    $file = $res;

    // add invoice to database
    if ($file)
    {
        if ( strtolower(substr($file->filename, -4)) == ".bmp" ) {
            
//          $old_filepath = drupal_realpath($file->uri);
            $result_convert = convert_bmp_to_jpg($file);
            file_delete($file);
            if ($result_convert === FALSE) {
                drupal_set_message("Failed to convert uploaded bmp file to png file. ($roomname)", 'error');
                return;
            }
            $file = $result_convert;
        }else{
            $file = $res;
        }
    
        // get last revision
    //  $query = "SELECT * FROM claims_scope WHERE claim_id=".$claim_id." AND roomname=\"".$roomname."\"";
        $result = db_query('SELECT * FROM {claims_scope} WHERE claim_id=:a AND roomname=:b',
            array(':a'=>$claim_id,':b'=>$roomname));
        $row_count = $result->rowCount();
        
        
        if ($row_count == 0)
        {   
            $revision = 0;  
            $query = db_query('INSERT INTO {claims_scope} VALUES(NULL,:a,:b,:c,:d,:e,:f)',
                array(':a'=>$claim_id,':b'=>$roomname,':c'=>$file->filename,':d'=>$filePath.$file->filename,
                ':e'=>$file->filemime,':f'=>date('U')));
        }
        else
        {
            $row = (array)$result->fetchAssoc();
//          $revision = $row['revision']+1;

            $revision = 0;  
            file_delete($row['filepath']);
            // @unlink(drupal_realpath($row['filepath']));

            $query = db_query('UPDATE {claims_scope} SET filename=:a, filepath=:b, filemime=:d,
            `timestamp`=:e WHERE claim_id=:f AND roomname=:g',array(':a'=>$file->filename,':b'=>$filePath.$file->filename,
                ':d'=>$file->filemime,':e'=>date('U'),':f'=>$claim_id,':g'=>$roomname));
        }
        
        $result = $query;
        
        $log_message = t("!user_name uploaded Scope File of Room(!room_name). Filename: !file_name", array('!user_name'=>ncn_amin_get_user_role_and_name(), '!room_name'=>$roomname, '!file_name'=>$file->filename));
        ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
        // success!
        $log_message = t("Uploaded Scope File of Room(!room_name). Filename: !file_name", array('!room_name'=>$roomname, '!file_name'=>$file->filename));
        drupal_set_message('File "'.$file->filename.'" was uploaded (roomname: '.$roomname.').', 'status');
    } else {
        
    }
}

// upload scope pdf file and crop automatically.
function ncn_admin_auto_scopeform_full_pdf_submit($claim_id, $roomname) {
    
    // we know the file was good
    $source = 'ncn_scope_auto_pdf_file_'.$roomname;
    
    $file = file_save_upload($source);

    $json_data = array();
    $json_data['error'] = '';
    $json_data['roomname'] = $source;
    
    if (!$file) {
        drupal_set_message("File of missing for upload (room name :$roomname)", "error");
        return;
    } else if ($file->filemime != 'application/pdf') {
        drupal_set_message("Scope file should be only PDF (room name: $roomname)", "error");
        file_delete($file);
        return;
    }
    
    if ($json_data['error'] == '') {

    $filePath = "sites/default/files/invoices/$claim_id/";
    $claim_dir = DRUPAL_ROOT."/".$filePath;
    
    if (!is_dir($claim_dir)) {
        mkdir($claim_dir, 0755);
    }

    $res = file_copy($file, "public://invoices/{$claim_id}", FILE_EXISTS_RENAME);
    file_delete($file);

    $dest_file['uri'] = $res->uri;
    $dest_file['filename'] = $res->filename;
    
    $img_file = convert_pdf_to_jpg($res, $claim_dir);

    if ($img_file == FALSE) {
        drupal_set_message(t("Failed to convert PDF to Image, after uploading scope pdf file(room name :!roomname)", array('!roomname'=>$roomname)), "error");
        return;
    } else {
        // Croping converted image
        $ts = date('U');
        $dest_file['uri'] .= "_cut_".$ts.".jpg";
        $dest_file['filename'] .= "_cut_".$ts.".jpg";
        $dest_file['filemime'] = "image/jpg";
        
        
        $api_params = array(
            'action'    => 'crop',
            'width'     => variable_get('ncn_scope_file_crop_width', '0'),
            'height'    => variable_get('ncn_scope_file_crop_height', '0'),
            'x'         => variable_get('ncn_scope_file_crop_x', '0'),
            'y'         => variable_get('ncn_scope_file_crop_y', '0'),
            
            'source'    => $img_file->uri,
            'dest'      => $dest_file['uri']
        );
            
        $bAction = _ncn_image_api('crop', $api_params);
        //@unlink(drupal_realpath($img_file->uri));
        file_delete($img_file);
        if ($bAction) {
            // get last revision
        //  $query = "SELECT * FROM claims_scope WHERE claim_id=".$claim_id." AND roomname=\"".$roomname."\"";
            $result = db_query('SELECT * FROM {claims_scope} WHERE claim_id=:a AND roomname=:b',
                array(':a'=>$claim_id,':b'=>$roomname));
            $row_count = $result->rowCount();
            
            if ($row_count == 0)
            {   
                $revision = 0;  
                $query = db_query('INSERT INTO {claims_scope} VALUES(NULL,:a,:b,:c,:d,:e,:f)',
                    array(':a'=>$claim_id,':b'=>$roomname,':c'=>$dest_file['filename'],
                    ':d'=>$filePath.$dest_file['filename'],':e'=>$dest_file['filemime'],':f'=>date('U')));
            }
            else
            {
                $row = (array)$result->fetchAssoc();
                //$revision = $row['revision']+1;
                
                $revision = 0;  

                //@unlink(drupal_realpath($row['filepath']));
                file_delete($row['filepath']);

                $query = db_query('UPDATE {claims_scope} SET filename=:a , filepath=:b , filemime=:c , `timestamp`=:d WHERE claim_id=:e AND roomname=:f',
                array(':a'=>$dest_file['filename'],':b'=>$filePath.$dest_file['filename'],':c'=>$dest_file['filemime'],
                    ':d'=>date('U'),':e'=>$claim_id,':f'=>$roomname));
            }
      
            $result = $query;
            drupal_set_message('File "'.$dest_file['filename'].'" was generated (roomname: '.$roomname.').', 'status');
      
      $log_message = t("!user_name generated Scope File of Room !room_name. Filename: !file_name", array('!user_name'=>ncn_amin_get_user_role_and_name(), '!room_name'=>$roomname, '!file_name'=>$dest_file['filename']));
      ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
        } else {
            drupal_set_message(t("Failed to crop image, after uploading scope pdf file and converting to image (room name :!roomname)", array('!roomname'=>$roomname)), "error");
            return;
            
        }
    }
    }
    
}

// upload scope pdf file and crop manually.
function ncn_admin_scopeform_full_pdf_submit($claim_id, $roomname) {
    
    // we know the file was good
    $source = 'ncn_scope_pdf_file_'.$roomname;
    
    $file = file_save_upload($source);
    
    $json_data = array();
    $json_data['error'] = '';
    
    if (!$file) {
        //drupal_set_message("File of missing for upload (room name :$roomname)", "error");
        $json_data['error'] = "File of missing for upload (room name :$roomname)";
        //return;
    } else if ($file->filemime != 'application/pdf') {
        //drupal_set_message("Scope file should be only PDF (room name: $roomname)", "error");
        $json_data['error'] = "Scope file should be only PDF (room name: $roomname)";
        file_delete($file);
        //return;
    }
    
    if ($json_data['error'] == '') {

        $filePath = "sites/default/files/invoices/$claim_id/";
        $claim_dir = DRUPAL_ROOT."/".$filePath;
        
        if (!is_dir($claim_dir)) {
            mkdir($claim_dir, 0755);
        }

        $res = file_copy($file, "public://invoices/{$claim_id}", FILE_EXISTS_RENAME);
        file_delete($file);
        
        $dest_file['uri'] = $res->uri;
        $dest_file['filename'] = $res->filename;

        $img_file = convert_pdf_to_jpg($res, $claim_dir);
        if ($img_file == FALSE) {
            //drupal_set_message(t("Failed to convert PDF to Image, after uploading scope pdf file(room name :!roomname)", array('!roomname'=>$roomname)), "error");
            $json_data['error'] = "Failed to convert PDF to Image, after uploading scope pdf file(room name :$roomname)";
            //return;
        } else {
        
            // Scaling converted image
            $ts = date('U');
            $dest_file['uri'] .= "_scope_".$ts.".jpg";
            $dest_file['filename'] .= "_scope_".$ts.".jpg";
            $dest_file['filemime'] = "image/jpg";
            
            
            $api_params = array(
                'action'    => 'scale',
                'width'     => 600,
                'height'    => NULL,
                'upscale'   => FALSE,
                
                'source'    => $img_file->filepath,
                'dest'      => $dest_file['uri']
            );
                
            $bAction = _ncn_image_api('scale', $api_params);
            //@unlink(drupal_realpath($img_file->uri));
            file_delete($img_file);
            if ($bAction) {
                $json_data['filepath'] = str_replace('public://', 'sites/default/files/', $dest_file['uri']);
                $json_data['filepath'] = base64_encode($json_data['filepath']);
            } else {
                $json_data['error'] = "Failed to crop image, after uploading scope pdf file and converting to image (room name :$roomname)";
            }
        }
    }
    
    echo json_encode($json_data);
}

//------------------------------------------------------------------------------
function ncn_admin_draw_scope_upload($claim_id, $roomname, $pvc_disabled='', $start_num) {
GLOBAL $base_url;
    ob_start();
?>
    <div class="scope_file">
        <fieldset class="">
            <legend class="">
            </legend>
            <div class="scope_form">
            <?php
                if ($pvc_disabled != 'disabled') {
                    print ncn_admin_scopeform($claim_id, $roomname );
                }
            ?>
            
                <div id="invoice_revisions">
                <table>
                <tbody>
                    <thead class="tableHeader-processed">
                    <tr>
                        <th>File</th>
                        <th>Timestamp</th>
                        <th>Action</th>
                    </tr>
                    </thead>
                    <?php
                    // get latest 5 invoice revisions
                    // $query = "SELECT * FROM claims_scope WHERE claim_id=".$claim_id." AND roomname =\"$roomname\"";
                    $result = db_query('SELECT * FROM {claims_scope} WHERE claim_id=:a AND roomname=:b',array(':a'=>$claim_id,':b'=>$roomname));
                    $row_count = $result->rowCount();
                    
                    // run through invoice revisions
                    for ($i=0; $i<$row_count; $i++)
                    {
                        $row = $result->fetchAssoc();
                        ?>              
                        <tr class="<?php if ($i%2) print "even"; else print "odd"; ?>">
                            <td><img src="<?php echo $base_url ?>/sites/all/modules/ncn_claims_manager/images/pdf_icon_16.gif" width="16" height="16" align="left" />&nbsp;<a href="<?= $base_url; ?>/account/serve_scope_file/<?= $claim_id; ?>/<?= $row['roomname']; ?>"><?= StripSlashes($row['filename']); ?></a></td>
                            <td><?= date("m/d/Y H:i:s", $row['timestamp']); ?></td>
                            <td>
                                <?php if ($pvc_disabled != 'disabled'): ?>
                                <a class="scope_file_<?=$i+$start_num*1000?>" href="<?= $base_url; ?>/account/delete_scope_file/<?= $claim_id; ?>/<?= $row['roomname']; ?>" onclick='set_scroll_position("scope_file_<?=$i+$start_num*1000 ?>"); return confirm("Are you sure you want to delete this file?");'>[delete]</a>
                                <?php endif; ?>
                            </td>
                        </tr>
                        <?php
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

function save_image_captions($claim_id) {
    $roomname   = $_POST['roomname'];
    $img_count  = $_POST['image_count'];
    
    for ($i=0; $i<$img_count; $i++) {
        $img_index = 'image'.$i;
        $data[$i] = $_POST[$img_index];
    }
    
    if (_save_image_captions($claim_id, $roomname, $data)) {
        drupal_set_message("Image captions updated, successfully. (room:$roomname)");
    $log_message = ncn_amin_get_user_role_and_name()." updated Image Captions . (room:$roomname)";
    ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
    } else {
        drupal_set_message("Failed to update image captions. (room:$roomname)", 'error');
    }
    
}
