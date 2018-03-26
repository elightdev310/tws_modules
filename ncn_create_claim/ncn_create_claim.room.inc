<?php
//------------------------------------------------------------------------------
function ncn_create_claim_upload_roomimage()
{
    // check status
    //    $file = $_FILES['file'];
    $file = array();
    // print_r($_FILES);die;
    //  print "|error|";
    //  echo "<pre>";
    //  print_r($_FILES);
    //  echo "</pre>";
    //  exit;
    $claim_id = intval(arg(2));
    $url = "sites/default/files/user_files/$claim_id";
    if (!is_dir($url)) {
        mkdir($url, 0775);
    }

    $special_chars = array('#', '%', '&');
    $filename = preg_replace("/[^a-zA-Z0-9\s\.]/", "_", basename($_FILES['files']['name']['file']));

    $validTypeArr = array("image/jpeg", "image/jpg", "image/pjpeg", "image/gif", "image/png");

    if (in_array($_FILES['files']['type']['file'], $validTypeArr) || strtolower(substr($filename, -4)) == ".bmp") {
        //$res = file_move($file['tmp_name'], "sites/default/files/user_files/$claim_id/" . $filename, FILE_EXISTS_RENAME);
        $resOfFile = file_save_upload('file', array(), "public://user_files/".$claim_id, FILE_EXISTS_RENAME);

      if($resOfFile){
        $teURI = @explode('/', $resOfFile->uri);
        $filename = $teURI[count($teURI) - 1];
        $input_file = "sites/default/files/user_files/$claim_id/" . $filename;
        $quality = 70;
        $o_file = new stdClass();
        $o_file->filepath = $input_file;
        $o_file->filename = $filename;
        $o_file->filemime = "image/bmp";

        if (strtolower(substr($filename, -4)) == ".bmp") {
          $old_filepath = $o_file->filepath;
          $result_convert = convert_bmp_to_jpg($o_file);
          file_delete($old_filepath);
          if ($result_convert === FALSE) {
            $result['flag'] = 'error4';
            $result['msg'] = "Failed to convert uploaded bmp file to png file. ($roomname)";
          } else {
            $r_file = $result_convert;
            $input_file = "sites/default/files/user_files/$claim_id/" . $r_file->filename;
            drupal_set_message($r_file->filename);
          }
        }


        if (file_exists($input_file)) {
          $file['tmp_name'] = $input_file;

          $img_file = $input_file;
          $info = getimagesize($img_file);

          $img_info = array(
            'width' => $info[0],
            'height' => $info[1],
            'bits' => $info['bits'],
            'mime' => $info['mime']
          );

          $mime = $img_info['mime'];

          if ($mime == 'image/gif') {
            $image = imagecreatefromgif($img_file);
          } elseif ($mime == 'image/png') {
            $image = imagecreatefrompng($img_file);
          } elseif ($mime == 'image/jpeg' || $mime == 'image/jpg') {
            $image = imagecreatefromjpeg($img_file);
          }

          $info = pathinfo($img_file);
          $extension = strtolower($info['extension']);

          if ($extension == 'jpeg' || $extension == 'jpg') {
            imagejpeg($image, $img_file, $quality);
          } elseif ($extension == 'png') {
            imagepng($image, $img_file, 0);
          } elseif ($extension == 'gif') {
            imagegif($image, $img_file);
          }

          imagedestroy($image);
        }

        print "|success|";
        //print $GLOBALS['base_url'] . "/" . str_replace("default/files/", "default/files/imagecache/image_tab_upload/", $file['tmp_name']);
        print image_style_url('image_tab_upload',$resOfFile->uri);
        print "|" . $_POST['image_id'] . "|";
        //print $GLOBALS['base_url'] . "/" . str_replace("default/files/", "default/files/imagecache/photo_album/", $file['tmp_name']);
        print image_style_url('photo_album',$resOfFile->uri);
        print "|";
      }

    } else {
        print '|error|That is not a valid image file type|';
        return;
    }

    // now we save the image in the ['claim'] session
    $item_name = $_POST['image_id'];

    $_SESSION['claim'][$claim_id]["scope"][arg(3)][$item_name] = array(
        'type' => 'image',
        'filename' => basename($file['tmp_name']), /****/
        'path' => "public://user_files/".$claim_id."/".basename($file['tmp_name']), //"sites/default/files/user_files/$claim_id/" . basename($file['tmp_name']), /***/
        'weight' => 100,
        'resize_image_path' => $resOfFile,
        //str_replace('default/files/', "default/files/imagecache/photo_album/", $file['tmp_name']), // added by ***
    );

    global $user;
    if (!isset($_SESSION['claim_log'][$claim_id])) {
        $_SESSION['claim_log'][$claim_id] = '';
    }
    $_SESSION['claim_log'][$claim_id] .= t("!user_name added a photo into the room(!roomname).", array('!user_name' => ncn_amin_get_user_role_and_name($user->uid), '!roomname' => arg(3)));
    $_SESSION['claim_log'][$claim_id] .= "\r\n";
}

//------------------------------------------------------------------------------
function ncn_create_claim_deleteroomtab()
{
    $claim_id = arg(2);
    // get room name
    $room_name = preg_replace("/[^a-zA-Z0-9\s]/", "", $_REQUEST['room_name']);

    // clear the page
    unset($_SESSION['claim'][$claim_id]['scope'][$room_name]);
    $_SESSION['claim_log'][$claim_id] .= t("!user_name remove a room(!roomname).", array('!user_name' => ncn_amin_get_user_role_and_name(), '!roomname' => $room_name));
    $_SESSION['claim_log'][$claim_id] .= "\r\n";
    // success
    echo $room_name;
    exit;
}

//------------------------------------------------------------------------------
function ncn_create_claim_renameroomtab()
{
    $claim_id = arg(2);
    $old_room_name = preg_replace("/[^a-zA-Z0-9\s]/", "", $_REQUEST['room_name']);
    $new_room_name = trim(preg_replace("/[^a-zA-Z0-9\s]/", "", $_REQUEST['new_name']));

    $j_result['code'] = 'success';
    $j_result['msg'] = '';
    if ($old_room_name == $new_room_name) {
        $j_result['code'] = 'NCN-CREATE-CLAIM-10011-SAME-ROOMNAME';
        $j_result['msg'] = '';
    } else if ($old_room_name != $new_room_name && isset($_SESSION['claim'][$claim_id]['scope'][$new_room_name])) {
        $j_result['code'] = 'NCN-CREATE-CLAIM-10012-DUPLICATED-ROOMNAME';
        $j_result['msg'] = t('Roomname(!roomname) is duplicated,', array('!roomname' => $new_room_name));
    } else {
        // clear the page
        $j_result['new_roomname'] = $new_room_name;
        $_SESSION['claim'][$claim_id]['scope'][$new_room_name] = $_SESSION['claim'][$claim_id]['scope'][$old_room_name];
        unset($_SESSION['claim'][$claim_id]['scope'][$old_room_name]);
        $_SESSION['claim_log'][$claim_id] .= t("!user_name rename a room(!roomname) to !new_roomname.", array('!user_name' => ncn_amin_get_user_role_and_name(), '!roomname' => $old_room_name, '!new_roomname' => $new_room_name));
        $_SESSION['claim_log'][$claim_id] .= "\r\n";

        //* queue rename actions
        if (!isset($_SESSION['claim_room_rename_actions'][$claim_id])) {
            $_SESSION['claim_room_rename_actions'][$claim_id] = array();
        }
        $_SESSION['claim_room_rename_actions'][$claim_id][] = array('old_name' => $old_room_name, 'new_name' => $new_room_name);
    }

    echo json_encode($j_result);

}

function ncn_create_claim_addroomtab()
{
    $claim_id = arg(2);
    $room_name = preg_replace("/[^a-zA-Z0-9\s]/", "", $_REQUEST['room_name']);
    $room_name = trim($room_name);

    $j_result['code'] = 'success';
    $j_result['msg'] = '';
    if (isset($_SESSION['claim'][$claim_id]['scope'][$room_name])) {
        $j_result['code'] = 'NCN-CREATE-CLAIM-10011-DUPLICATED-ROOMNAME';
        $j_result['msg'] = t('Roomname(!roomname) is duplicated.', array('!roomname' => $room_name));
    } else {
        $j_result['new_roomname'] = $room_name;
        $_SESSION['claim'][$claim_id]['scope'][$room_name] = array();

        if (!isset($_SESSION['claim_log'][$claim_id])) {
            $_SESSION['claim_log'][$claim_id] = '';
        }
        $_SESSION['claim_log'][$claim_id] .= t("!user_name add a room(!roomname).", array('!user_name' => ncn_amin_get_user_role_and_name(), '!roomname' => $room_name));
        $_SESSION['claim_log'][$claim_id] .= "\r\n";
    }

    echo json_encode($j_result);
}

//------------------------------------------------------------------------------
// ajax function for loading certain rooms
function ncn_create_claim_roomtab()
{
    $claim_id = arg(2);
    $room_name = preg_replace("/[^a-zA-Z0-9\s]/", "", arg(3));
    $tab_index = arg(4);
    if (!$tab_index) {
        $tab_index = 1;
    }
    //$claim_id = $_SESSION['claim']['#data']['claim_id'];

    $default_name = preg_replace("/[^a-zA-Z0-9\s]/", "", $_SESSION['claim'][$claim_id]['customer_name']['val']);

    // default
    if ($default_name == $room_name) {
        $pic_count = 3;
        $pic_desc = array(
            0 => "Front of House",
            1 => "Cause of Origin",
            2 => "Driver's License",
        );

        $img_desc = array(
            0 => "fronhouse.png",
            1 => "origin.png",
            2 => "license.png",
        );
    } else {
        $pic_count = 6;
        $pic_desc = array();
        $img_desc = array();
    }

    // get room files data
    $fdata = array();
    $c = 0;

    print '<div class="row">';
    // get the room id from the arg
    for ($i = 0; $i < $pic_count; $i++) {
        // image
        if (isset($img_desc[$i])) {
            $img_fname = $img_desc[$i];
        } else {
            $img_fname = "blank" . ($i + 1) . ".png";
        }

        // define defaults
        $image_id = "image" . $i;
        $img_src = $GLOBALS['base_url'] . "/" . drupal_get_path('module', 'ncn_create_claim') . "/images/" . $img_fname;
        $img_src_hidden = $GLOBALS['base_url'] . "/" . drupal_get_path('module', 'ncn_create_claim') . "/images/" . $img_fname;

        $file_value = '';

        if (isset($_SESSION['claim'][$claim_id]['scope'][$room_name][$image_id])) {
            
            /*$img_src = $GLOBALS['base_url'] . "/" . $_SESSION['claim'][$claim_id]['scope'][$room_name][$image_id]['path'];
            $img_src = str_replace('default/files/', "default/files/imagecache/image_tab_upload/", $img_src);*/
            
            /*$img_src_hidden = $GLOBALS['base_url'] . "/" . $_SESSION['claim'][$claim_id]['scope'][$room_name][$image_id]['path'];
            $img_src_hidden = str_replace('default/files/', "default/files/imagecache/photo_album/", $img_src_hidden);*/

            $img_path = str_replace('sites/default/files/', 'public://',$_SESSION['claim'][$claim_id]['scope'][$room_name][$image_id]['path']);
            $img_src = image_style_url('image_tab_upload',$img_path);
            $img_src_hidden = image_style_url('photo_album',$img_path);

            $file_value = $_SESSION['claim'][$claim_id]['scope'][$room_name][$image_id]['filename'];
        }

        ?>
        <div class="col-sm-4">
        <form method="POST" class="ar_form" enctype="multipart/form-data"
              action="<?= base_path() ?>account/upload_room_image/<?= "$claim_id/$room_name"; ?>" id="newHotnessForm">
            <input type="hidden" name="image_id" value="<?= $image_id; ?>">

            <div class="ar_upload_wrapper ar_imb_box_<?= $image_id; ?>">
                <p><?php if (isset($pic_desc[$i])) print '<strong>' . $pic_desc[$i] . '</strong>'; ?></p>

                <div class="ar_imgbox"><img src="<?= $img_src; ?>" id="<?= $image_id; ?>"></div>
                <div class="ar_imgbox_hidden" style="display: none;"><img src="<?= $img_src_hidden; ?>"
                                                                          id="<?= $image_id; ?>_hidden"></div>

                <div class="button-section clearfix">
                    <input type="file" class="photo-file hidden" name="files[file]" value="<?= $file_value; ?>" >
                    <input type="submit" class="hidden upload-photo-file" value="Upload Photo">
                    <a href="#" class="btn btn-primary pull-right choose-photo-file">
                        <?php if (isset($_SESSION['claim'][$claim_id]['scope'][$room_name][$image_id])): ?>
                            Replace Photo
                        <?php else: ?>
                            Upload Photo
                        <?php endif; ?>
                    </a>
                    <?php if (isset($_SESSION['claim'][$claim_id]['scope'][$room_name][$image_id])): ?>
                        <!-- <input type="button" value="Remove Photo" class="remove-photo-album" onclick="removePhotoInAlbum(<?= $claim_id; ?>, '<?= $room_name; ?>', '<?= $image_id; ?>');" /> -->
                    <?php endif; ?>
                </div>
            </div>
        </form>
        </div>
    <?php
    }
    print '</div>';

    $_url_room_scope_sheet = url("account/scope_sheet/$claim_id/" . $room_name) . "?from_photo_album=1";
    if ($pic_count == 6) {
        echo '<div class="ncn-room-action-panel clearfix"><span>You need to SAVE room before you can access Scope Sheet</span> &nbsp;&nbsp; <a href="' . $_url_room_scope_sheet . '" >Edit Room ScopeSheet</a></div>';
    }
}

//------------------------------------------------------------------------------
// preprocess function for "ncn_add_room.tpl.php"
function template_preprocess_ncn_add_room(&$vars)
{
    drupal_add_css(drupal_get_path('module', 'ncn_create_claim') . '/tabs/tabs.css');
    drupal_add_library('system', 'jquery.form'); 
    // drupal_add_js(drupal_get_path('module', 'ncn_create_claim') . '/tabs/ajax-upload.js');
    drupal_add_js(drupal_get_path('module', 'ncn_create_claim') . '/tabs/tabs.js');
}

/**
 * Action Handler ( ajax/ncn_create_claim/remove_photo/% )
 * Remove Photo in Photo Album
 * @param mixed $claim_id
 */
function ncn_create_claim_remove_photo($claim_id)
{
    global $base_url;

    $room_name = $_REQUEST['room_name'];
    $image_id = $_REQUEST['image_id'];

    $j_result['code'] = 'success';
    $j_result['msg'] = '';

    $default_room_name = ncn_admin_claim_get_default_room_name($claim_id);
    $image_count = 6;
    if ($room_name == $default_room_name) {
        $image_count = 3;
    }
    $image_index = substr($image_id, -1, 1);
    $j_result['image_blank_path'] = _get_blank_room_image_url($image_count, $image_index);

    if (isset($_SESSION['claim'][$claim_id]["scope"][$room_name][$image_id])) {
        unset($_SESSION['claim'][$claim_id]["scope"][$room_name][$image_id]);

        global $user;
        $_SESSION['claim_log'][$claim_id] .= t("!user_name remove photo(!img_id) into the room(!roomname).", array('!user_name' => ncn_amin_get_user_role_and_name($user->uid), '!roomname' => $room_name, '!img_id' => $image_id));
        $_SESSION['claim_log'][$claim_id] .= "\r\n";
    }

    echo json_encode($j_result);
}
