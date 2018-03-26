<?php
// Owner ID of Member
function get_owner($memberid) {
//  $query = "SELECT * FROM member_id_pool WHERE member_id='$memberid'";
    $result = db_query('SELECT * FROM {member_id_pool} WHERE member_id=:a',array(':a'=>$memberid));
    if ($result->rowCount()>0) {

        $row = $result->fetchAssoc();
        return $row['owner'];
    }
    return 0;
}

//------------------------------------------------------------------------------
// *** 2011-03-25 ***
function make_seed()
{
  list($usec, $sec) = explode(' ', microtime());
  return (float) $sec + ((float) $usec * 100000);
}

// Auto Member Assignment
function check_member_id($member_id) {
    //  $query = "SELECT * FROM member_id_pool WHERE member_id=$member_id";
        $result = db_query('SELECT * FROM {member_id_pool} WHERE member_id=:a',array(':a'=>$member_id));
        $count = $result->rowCount();
        
        $ret_value = false;
        if ($count == 0) {
            $ret_value = true;
        }
        
        return $ret_value;
}

function generate_member_id(){
        $MAX_MEMBER_ID = 99990;
        
        srand(make_seed());
        $member_id = rand(10, $MAX_MEMBER_ID);
        $repeat = 0;
        
        while( !check_member_id($member_id) ){
            $member_id += 1;
            $repeat += 1;
            if ( $repeat >50 ) {
                $repeat = 0;
                $member_id = rand($member_id, $MAX_MEMBER_ID);
                if (!check_member_id($member_id)) {
                    $member_id += 1;
                    $member_id = rand(10, $member_id);
                }
            }
        }
        
        return $member_id;
}

function get_claim_data($claim_id) {
//  $query = "SELECT *FROM claims WHERE claim_id=$claim_id";
    $result = db_query('SELECT * FROM {claims} WHERE claim_id=:a',array(':a'=>$claim_id));
    if ($result->rowCount() > 0) {
        $row = $result->fetchAssoc();
        return $row;
    }
    return array();
}

/**
 * Get All Members
 */
function get_member_list() {
//  $query = "SELECT * FROM member_id_pool ORDER BY first_name ASC, last_name ASC";
    $result = db_query('SELECT * FROM {member_id_pool} ORDER BY first_name ASC, last_name ASC');
    $row_count = $result->rowCount();
    $member_list = array();
    for ($i=0; $i<$row_count; $i++) {
        $member_list[] = $result->fetchAssoc();
    }
    return $member_list;
}

function ImageCreateFromBMP($filename)
{
    $res='';
    if (! $f1 = fopen ($filename ,"rb")) return FALSE ;
    $FILE = unpack ("vfile_type/Vfile_size/Vreserved/Vbitmap_offset", fread ($f1 ,14 ));
    if ($FILE ['file_type'] != 19778 ) return FALSE ;
    $BMP = unpack ('Vheader_size/Vwidth/Vheight/vplanes/vbits_per_pixel'. '/Vcompression/Vsize_bitmap/Vhoriz_resolution'. '/Vvert_resolution/Vcolors_used/Vcolors_important', fread ($f1 ,40 ));
    $BMP ['colors'] = pow (2 ,$BMP ['bits_per_pixel']);
    if ($BMP ['size_bitmap'] == 0 ) $BMP ['size_bitmap'] = $FILE ['file_size'] - $FILE ['bitmap_offset'];
    $BMP ['bytes_per_pixel'] = $BMP ['bits_per_pixel']/8 ;
    $BMP ['bytes_per_pixel2'] = ceil ($BMP ['bytes_per_pixel']);
    $BMP ['decal'] = ($BMP ['width']*$BMP ['bytes_per_pixel']/4 );
    $BMP ['decal'] -= floor ($BMP ['width']*$BMP ['bytes_per_pixel']/4 );
    $BMP ['decal'] = 4 -(4 *$BMP ['decal']);
    if ($BMP ['decal'] == 4 ) $BMP ['decal'] = 0 ;
    $PALETTE = array ();
    if ($BMP ['colors'] < 16777216 ) 
    {
    $PALETTE = unpack ('V'. $BMP ['colors'], fread ($f1 ,$BMP ['colors']*4 ));
    }
    $IMG = fread ($f1 ,$BMP ['size_bitmap']);
    $VIDE = chr (0 );
    $res=imagecreatetruecolor($BMP['width'],$BMP['height']);
    $P = 0 ;
    $Y = $BMP ['height']-1 ;
    while ($Y >= 0 ) 
    {
    $X =0 ;
    while ($X < $BMP ['width']) 
    {
    if ($BMP ['bits_per_pixel'] == 24 ) $COLOR = unpack ("V",substr ($IMG ,$P ,3 ). $VIDE );
    elseif ($BMP ['bits_per_pixel'] == 16 ) 
    {
    $COLOR = unpack ("n",substr ($IMG ,$P ,2 ));
    $COLOR [1 ] = $PALETTE [$COLOR [1 ]+1 ];
    } elseif ($BMP ['bits_per_pixel'] == 8 ) 
    {
    $COLOR = unpack ("n",$VIDE.substr ($IMG ,$P ,1 ));
    $COLOR [1 ] = $PALETTE [$COLOR [1 ]+1 ];
    } elseif ($BMP ['bits_per_pixel'] == 4 ) 
    {
    $COLOR = unpack ("n",$VIDE.substr ($IMG ,floor ($P ),1 ));
    if(($P *2 )%2 == 0 ) $COLOR [1 ] = ($COLOR [1 ] >> 4 ) ;
    else $COLOR [1 ] = ($COLOR [1 ] & 0x0F );
    $COLOR [1 ] = $PALETTE [$COLOR [1 ]+1 ];
    } elseif ($BMP ['bits_per_pixel'] == 1 ) 
    {
    $COLOR = unpack ("n",$VIDE.substr ($IMG ,floor ($P ),1 ));
    if (($P *8 )%8 == 0 ) $COLOR [1 ] = $COLOR [1 ] >>7 ;
    elseif (($P *8 )%8 == 1 ) $COLOR [1 ] = ($COLOR [1 ] & 0x40 )>>6 ;
    elseif (($P *8 )%8 == 2 ) $COLOR [1 ] = ($COLOR [1 ] & 0x20 )>>5 ;
    elseif (($P *8 )%8 == 3 ) $COLOR [1 ] = ($COLOR [1 ] & 0x10 )>>4 ;
    elseif (($P *8 )%8 == 4 ) $COLOR [1 ] = ($COLOR [1 ] & 0x8 )>>3 ;
    elseif (($P *8 )%8 == 5 ) $COLOR [1 ] = ($COLOR [1 ] & 0x4 )>>2 ;
    elseif (($P *8 )%8 == 6 ) $COLOR [1 ] = ($COLOR [1 ] & 0x2 )>>1 ;
    elseif (($P *8 )%8 == 7 ) $COLOR [1 ] = ($COLOR [1 ] & 0x1 );
    $COLOR [1 ] = $PALETTE [$COLOR [1 ]+1 ];
    } else return FALSE ;
    imagesetpixel ($res ,$X ,$Y ,$COLOR [1 ]);
    $X ++;
    $P += $BMP ['bytes_per_pixel'];
    } $Y --;
    $P +=$BMP ['decal'];
    } fclose ($f1 );
    return $res ;
}

function get_transaction_type($type='') {
    $data = array();
    
    $data[] = array('product_id'=>34,       'name'=>'Start-Up Fees (Full)'                  );
    $data[] = array('product_id'=>40,       'name'=>'Start-Up Fees (Full)'                  );
    $data[] = array('product_id'=>41,       'name'=>'Start-Up Fees (Partial)'               );
    $data[] = array('product_id'=>42,       'name'=>'Start-Up Fees (Partial)'               );
    $data[] = array('product_id'=>33,       'name'=>'Start-Up Fees (Dist. Full)'            );
    $data[] = array('product_id'=>43,       'name'=>'Start-Up Fees (Dist. Partial)'         );
    
    $data[] = array('product_id'=>47,       'name'=>'Start-Up Fees (Gold Member Special)'   );
    $data[] = array('product_id'=>48,       'name'=>'Start-Up Fees (Gold Member Special, Dist.)');
    
    $data[] = array('product_id'=>57,       'name'=>'Start-Up Fees (Gold Member 1st Special)');
    $data[] = array('product_id'=>68,       'name'=>'Start-Up Fees (Gold Member Demo)');
    
    $data[] = array('product_id'=>88,       'name'=>'Start-Up Fees (Gold Member Associate Special)' );
    $data[] = array('product_id'=>154,  'name'=>'Start-Up Fees (Gold Coach Member)' );
    $data[] = array('product_id'=>155,  'name'=>'Start-Up Fees (Coach on Call Member)'  );
  
    $data[] = array('product_id'=>156,  'name'=>'Gold to Gold Coach Setup Fee'  );
  $data[] = array('product_id'=>161,  'name'=>'Cancel/Pause Renewal Fee'  );
  
    
    $data[] = array('product_id'=>32,       'name'=>'Monthly Membership Fees (Gold)'                );
    $data[] = array('product_id'=>102,  'name'=>'Monthly Membership Fees (Gold Lite)'               );
    $data[] = array('product_id'=>119,  'name'=>'Monthly Membership Fees (Coach on Call)'               );
    $data[] = array('product_id'=>137,  'name'=>'Monthly Membership Fees (Gold Coach)'              );
    
    $data[] = array('product_id'=>85,       'name'=>'Free Monthly Membership'               );
    $data[] = array('product_id'=>44,       'name'=>'Monthly Finance Fees'                  );
    
    
    
    $data[] = array('product_id'=>18,       'name'=>'Residential Water Claims (Gold)'               );
    $data[] = array('product_id'=>69,       'name'=>'Residential Water Claims (Gold) With Sketch'   );
    $data[] = array('product_id'=>30,       'name'=>'Commercial Water Claims (Gold)'                );
    $data[] = array('product_id'=>70,       'name'=>'Commercial Water Claims (Gold) With Sketch'    );
    $data[] = array('product_id'=>58,       'name'=>'Residential Fire Claims (Gold)'                );
    $data[] = array('product_id'=>71,       'name'=>'Residential Fire Claims (Gold) With Sketch'    );
    $data[] = array('product_id'=>59,       'name'=>'Commercial Fire Claims (Gold)'                 );
    $data[] = array('product_id'=>72,       'name'=>'Commercial Fire Claims (Gold) With Sketch'     );
    $data[] = array('product_id'=>60,       'name'=>'Residential Mold Claims (Gold)'                );
    $data[] = array('product_id'=>73,       'name'=>'Residential Mold Claims (Gold) With Sketch'    );
    $data[] = array('product_id'=>61,       'name'=>'Commercial Mold Claims (Gold)'                 );
    $data[] = array('product_id'=>74,       'name'=>'Commercial Mold Claims (Gold) With Sketch'     );
    $data[] = array('product_id'=>53,       'name'=>'Residential Reconstruction Claims (Gold)'              );
    $data[] = array('product_id'=>75,       'name'=>'Residential Reconstruction Claims (Gold) With Sketch'  );
    $data[] = array('product_id'=>62,       'name'=>'Commercial Reconstruction Claims (Gold)'               );
    $data[] = array('product_id'=>76,       'name'=>'Commercial Reconstruction Claims (Gold) With Sketch'   );
    
    $data[] = array('product_id'=>45,       'name'=>'Residential Water Claims (Silver)'             );
    $data[] = array('product_id'=>77,       'name'=>'Residential Water Claims (Silver) With Sketch' );
    $data[] = array('product_id'=>46,       'name'=>'Commercial Water Claims (Silver)'              );
    $data[] = array('product_id'=>78,       'name'=>'Commercial Water Claims (Silver) With Sketch'  );
    $data[] = array('product_id'=>63,       'name'=>'Residential Fire Claims (Silver)'              );
    $data[] = array('product_id'=>79,       'name'=>'Residential Fire Claims (Silver) With Sketch'  );
    $data[] = array('product_id'=>64,       'name'=>'Commercial Fire Claims (Silver)'               );
    $data[] = array('product_id'=>80,       'name'=>'Commercial Fire Claims (Silver) With Sketch'   );
    $data[] = array('product_id'=>65,       'name'=>'Residential Mold Claims (Silver)'              );
    $data[] = array('product_id'=>81,       'name'=>'Residential Mold Claims (Silver) With Sketch'  );
    $data[] = array('product_id'=>66,       'name'=>'Commercial Mold Claims (Silver)'               );
    $data[] = array('product_id'=>82,       'name'=>'Commercial Mold Claims (Silver) With Sketch'   );
    $data[] = array('product_id'=>67,       'name'=>'Residential Reconstruction Claims (Silver)'            );
    $data[] = array('product_id'=>83,       'name'=>'Residential Reconstruction Claims (Silver) With Sketch');
    $data[] = array('product_id'=>54,       'name'=>'Commercial Reconstruction Claims (Silver)'             );
    $data[] = array('product_id'=>84,       'name'=>'Commercial Reconstruction Claims (Silver) With Sketch' );
    
    $data[] = array('product_id'=>94,       'name'=>'Residential Fire/Smoke Contents Cleaning Claims (Gold)'    );
    $data[] = array('product_id'=>95,       'name'=>'Residential Fire/Smoke Structure Cleaning Claims (Gold)'   );
    $data[] = array('product_id'=>96,       'name'=>'Residential Fire/Smoke Contents Cleaning Claims (Silver)'  );
    $data[] = array('product_id'=>97,       'name'=>'Residential Fire/Smoke Structure Cleaning Claims (Silver)' );
    
    $data[] = array('product_id'=>98,       'name'=>'Commercial Fire/Smoke Contents Cleaning Claims (Gold)' );
    $data[] = array('product_id'=>99,       'name'=>'Commercial Fire/Smoke Structure Cleaning Claims (Gold)'    );
    $data[] = array('product_id'=>100,  'name'=>'Commercial Fire/Smoke Contents Cleaning Claims (Silver)'   );
    $data[] = array('product_id'=>101,  'name'=>'Commercial Fire/Smoke Structure Cleaning Claims (Silver)'  );
    
    // GOLD LITE
    $data[] = array('product_id'=>103,      'name'=>'Residential Water Claims (Gold Lite)'              );
    $data[] = array('product_id'=>111,      'name'=>'Residential Water Claims (Gold Lite) With Sketch'  );
    $data[] = array('product_id'=>104,      'name'=>'Commercial Water Claims (Gold Lite)'               );
    $data[] = array('product_id'=>112,      'name'=>'Commercial Water Claims (Gold Lite) With Sketch'   );
    $data[] = array('product_id'=>105,      'name'=>'Residential Fire/Smoke Contents Cleaning Claims (Gold Lite)'               );
    $data[] = array('product_id'=>113,      'name'=>'Residential Fire/Smoke Structure Cleaning Claims (Gold Lite)'  );
    $data[] = array('product_id'=>106,      'name'=>'Commercial Fire/Smoke Contents Cleaning Claims (Gold Lite)'                    );
    $data[] = array('product_id'=>114,      'name'=>'Commercial Fire/Smoke Structure Cleaning Claims (Gold Lite)'       );
    $data[] = array('product_id'=>107,      'name'=>'Residential Mold Claims (Gold Lite)'               );
    $data[] = array('product_id'=>115,      'name'=>'Residential Mold Claims (Gold Lite) With Sketch'   );
    $data[] = array('product_id'=>108,      'name'=>'Commercial Mold Claims (Gold Lite)'                    );
    $data[] = array('product_id'=>116,      'name'=>'Commercial Mold Claims (Gold Lite) With Sketch'        );
    $data[] = array('product_id'=>109,      'name'=>'Residential Reconstruction Claims (Gold Lite)'             );
    $data[] = array('product_id'=>117,      'name'=>'Residential Reconstruction Claims (Gold Lite) With Sketch' );
    $data[] = array('product_id'=>110,      'name'=>'Commercial Reconstruction Claims (Gold Lite)'              );
    $data[] = array('product_id'=>118,      'name'=>'Commercial Reconstruction Claims (Gold Lite) With Sketch'  );

    // Coach on Call
    $data[] = array('product_id'=>120,      'name'=>'Residential Water Claims (Coach on Call)'              );
    $data[] = array('product_id'=>128,      'name'=>'Residential Water Claims (Coach on Call) With Sketch'  );
    $data[] = array('product_id'=>121,      'name'=>'Commercial Water Claims (Coach on Call)'               );
    $data[] = array('product_id'=>129,      'name'=>'Commercial Water Claims (Coach on Call) With Sketch'   );
    $data[] = array('product_id'=>122,      'name'=>'Residential Fire/Smoke Contents Cleaning Claims (Coach on Call)'               );
    $data[] = array('product_id'=>130,      'name'=>'Residential Fire/Smoke Structure Cleaning Claims (Coach on Call)'  );
    $data[] = array('product_id'=>123,      'name'=>'Commercial Fire/Smoke Contents Cleaning Claims (Coach on Call)'                    );
    $data[] = array('product_id'=>131,      'name'=>'Commercial Fire/Smoke Structure Cleaning Claims (Coach on Call)'       );
    $data[] = array('product_id'=>124,      'name'=>'Residential Mold Claims (Coach on Call)'               );
    $data[] = array('product_id'=>132,      'name'=>'Residential Mold Claims (Coach on Call) With Sketch'   );
    $data[] = array('product_id'=>125,      'name'=>'Commercial Mold Claims (Coach on Call)'                    );
    $data[] = array('product_id'=>133,      'name'=>'Commercial Mold Claims (Coach on Call) With Sketch'        );
    $data[] = array('product_id'=>126,      'name'=>'Residential Reconstruction Claims (Coach on Call)'             );
    $data[] = array('product_id'=>134,      'name'=>'Residential Reconstruction Claims (Coach on Call) With Sketch' );
    $data[] = array('product_id'=>127,      'name'=>'Commercial Reconstruction Claims (Coach on Call)'              );
    $data[] = array('product_id'=>135,      'name'=>'Commercial Reconstruction Claims (Coach on Call) With Sketch'  );
    
    // Gold Coach
    $data[] = array('product_id'=>138,      'name'=>'Residential Water Claims (Gold Coach)'             );
    $data[] = array('product_id'=>146,      'name'=>'Residential Water Claims (Gold Coach) With Sketch'     );
    $data[] = array('product_id'=>139,      'name'=>'Commercial Water Claims (Gold Coach)'              );
    $data[] = array('product_id'=>147,      'name'=>'Commercial Water Claims (Gold Coach) With Sketch'  );
    $data[] = array('product_id'=>140,      'name'=>'Residential Fire/Smoke Contents Cleaning Claims (Gold Coach)'              );
    $data[] = array('product_id'=>148,      'name'=>'Residential Fire/Smoke Structure Cleaning Claims (Gold Coach)' );
    $data[] = array('product_id'=>141,      'name'=>'Commercial Fire/Smoke Contents Cleaning Claims (Gold Coach)'                   );
    $data[] = array('product_id'=>149,      'name'=>'Commercial Fire/Smoke Structure Cleaning Claims (Gold Coach)'      );
    $data[] = array('product_id'=>142,      'name'=>'Residential Mold Claims (Gold Coach)'              );
    $data[] = array('product_id'=>150,      'name'=>'Residential Mold Claims (Gold Coach) With Sketch'  );
    $data[] = array('product_id'=>143,      'name'=>'Commercial Mold Claims (Gold Coach)'                   );
    $data[] = array('product_id'=>151,      'name'=>'Commercial Mold Claims (Gold Coach) With Sketch'       );
    $data[] = array('product_id'=>144,      'name'=>'Residential Reconstruction Claims (Gold Coach)'                );
    $data[] = array('product_id'=>152,      'name'=>'Residential Reconstruction Claims (Gold Coach) With Sketch'    );
    $data[] = array('product_id'=>145,      'name'=>'Commercial Reconstruction Claims (Gold Coach)'             );
    $data[] = array('product_id'=>153,      'name'=>'Commercial Reconstruction Claims (Gold Coach) With Sketch' );
    
    $data[] = array('product_id'=>136,      'name'=>'Change Order Fee'  );
  
      $data[] = array('product_id'=>157,    'name'=>'AR Services Start-Up Fee'  );
      $data[] = array('product_id'=>158,    'name'=>'AR Services'  );
      $data[] = array('product_id'=>159,    'name'=>'Peer Reviews'  );
    
    
    if ($type == 'all') {
        $data[] = array('ptype'=>'ADDITIONAL',  'product_id'=>0,'name'=>'Additional');
    }
    
    for ($i=0; $i<count($data); $i++) {
        if ($data[$i]['product_id'] != 0) {
            $_node = node_load($data[$i]['product_id']);
            if($_node!==false){
                $data[$i]['price'] = '$'.number_format(floatval($_node->sell_price), 2);
                $data[$i]['ptype'] = _ncn_report_product_type($data[$i]['product_id']);
            } else {
                $data[$i]['price'] = '$0';
                $data[$i]['ptype'] = '';
            }
        } else {
            $data[$i]['price'] = '*';
        }
    }
    
    return $data;
}

function handle_mysql_syntax_error($err_code, $query, $handle='message'){
    if ($handle == 'message') {
        drupal_set_message( t("Error(%code): mysql query is invalid with syntax. %query", array('%code'=>$err_code, '%query'=>$query)), 'error') ;
    }
}

function handle_unknown_error($err_code){
    drupal_set_message( t("Unknow error(%code) ocurrs. Please report to web administrator ", array('%code'=>$err_code)), 'error' ) ;
}

function send_mail_attached_file($to, $msg_subject, $msg, $attaches, $attache_names, $headers = ''){
    
    $message_part = '';
    $multipart = '';
    
    $boundary = "--".md5(uniqid(time()));
    $headers .= "MIME-Version: 1.0\n";
    $headers .= "Content-Type: multipart/mixed; boundary=\"$boundary\"\n";
    $code = "utf-8";
    $multipart .= "--$boundary\n";
    $multipart .= "Content-Type: text/plain; charset=$code\n";
    //$multipart .= "Content-Transfer-Encoding: Quot-Printed\n\n";
    $multipart .= "$msg\n\n";
    $i2 = count($attaches);
    foreach($attaches as $key => $attach){
        if($attache_names[$key]!="" && $attach){
            if(fopen($attach,"r")) $fp = fopen($attach,"r");
            else return;
            $file = fread($fp, filesize($attach));
            fclose($fp);
            $message_part .= "--$boundary\n";
            $message_part .= "Content-Type: application/octet-stream\n";
            $message_part .= "Content-Transfer-Encoding: base64\n";
            $message_part .= "Content-Disposition: attachment; filename = \"".$attache_names[$key]."\"\n\n";
            $message_part .= chunk_split(base64_encode($file))."\n";
        }
    }
    $multipart .= $message_part."--$boundary--\n";
    mail($to, $msg_subject, $multipart, $headers);
}

function ncn_admin_insert_claim_log($claim_id, $timestamp, $log_message) {
  if (!$timestamp) { $timestamp = date('U'); }
  
  $app_track = '';
  $app_track = ncn_mobile_detect_get_device();
  
  $ip_addr = $_SERVER['REMOTE_ADDR'];
//  $query = "INSERT INTO {claims_log} VALUES(%d, %d, '%s', '%s', '%s')";
    $result = db_query('INSERT INTO {claims_log} VALUES(:a,:b,:c,:d,:e)',array(':a'=>$claim_id,
    ':b'=>$timestamp,':c'=>$ip_addr,':d'=>$log_message,':e'=>$app_track));
    return $result;
}

function ncn_amin_get_user_role_and_name($user_id=0) {
global $user;
  if ($user_id == 0) {
    $user_id = $user->uid;
  }
  
  if ($user_id == 1) { return "Admin"; }
  
  $_user = user_load($user_id);
  if (!$_user) return "";
  
  $name = "";
  foreach ($_user->roles as $role_id=>$role_name) {
    if ($role_id == 2) { $name = "User"; } 
    else { $name = ucwords($role_name); }
  }
  
  $full_name = trim($_user->profile_firstname.' '.$_user->profile_lastname);
  
  if ($name != "") {
    if ($full_name) {
      $name .= "($full_name)";
    }
  } else if ($full_name) {
    $name = $full_name;
  }
  return $name;
}

function ncn_admin_get_mail_body($body, &$message, $params, $html=FALSE) {
    $body = trim($body);
    if (strpos($body, '{') === 0 && strrpos($body, '}')== (strlen($body)-1)) {
        $file_name = trim(substr($body, 1, -1));
        $body = ncn_admin_get_mail_body_from_template($file_name, $params);
        $html = TRUE;
    }
    if ($html == TRUE) {
        $headers = array(
            'MIME-Version' => '1.0',
            'Content-Type' => 'text/html; charset=UTF-8; format=flowed',
            'Content-Transfer-Encoding' => '8Bit',
            'X-Mailer' => 'Drupal'
        );
        foreach ($headers as $key => $value) {
            $message['headers'][$key] = $value;
        }
    }
    return $body;
}

function ncn_admin_get_mail_body_from_template($file_name, $params) {
    //$base_url = "http://www.netclaimsnow.com";
global $base_url;
    $img_basepath = $base_url."/".drupal_get_path('module', 'ncn_admin') . '/mail_template/image/';
    
    $mail_css_filepath = "mail_template/mail.css";
    ob_start();
    include($mail_css_filepath);
    $mail_css = ob_get_contents();
    ob_end_clean();
    
    $file_path = get_realpath('ncn_admin', "mail_template/".$file_name.".mtpl.php");
    ob_start();
    include $file_path;
    $output = ob_get_contents();
    ob_end_clean();
    
    return $output;
}

function get_format_phone_string($str) {
    if (trim($str) == "") {
        return '';
    }
    
    if (strpos($str, '-')>0) {
        return $str;
    }
    
    $str = str_replace('-', '', $str);
    $str1 = substr($str, 0, 3);
    $str2 = substr($str, 3, 3);
    $str3 = substr($str, 6);
    $str = "$str1-$str2-$str3";
    
    return $str;
}

function ncn_admin_get_member_logo_info($member_id) {   
    $result = db_query('SELECT * FROM {ncn_member_logo} WHERE member_id=:a',array(':a'=>$member_id));
    if ($result->rowCount()>0) {
        $row = $result->fetchAssoc();
        return $row;
    }
    return array();
}
function ncn_admin_get_member_logo($member_id) {
    $row = ncn_admin_get_member_logo_info($member_id);
    return isset($row['filepath'])?$row['filepath']:'';
}

function ncn_admin_get_claim_info($claim_id) {
    $result = db_query('SELECT * FROM {claims} WHERE claim_id=:d',array(':d'=>$claim_id));
    if ($result->rowCount()>0){
        foreach($result as $row)
        {
            $row = (array)$row;
            if($row)
            {
                return $row;
            }
        }
    }
    return array();
}

function ncn_admin_is_user_in_training($uid) {
    $account = user_load($uid);
    if (isset($account->profile_memberid) && is_member($account->profile_memberid)) {
        $_fblocked = '';
        if ( isset($account->profile_fblocked) ) {
            $_fblocked = $account->profile_fblocked;
        }
        if ($_fblocked == 'first') {
            return TRUE;
        }
    }
    
    return FALSE;
}

function ncn_admin_download_file($filepath, $filetype='', $params=array()) {
    if ($params['filename'] == '') {
        $params['filename'] = basename($filepath);
    }   
    
    $fullPath = $_SERVER['DOCUMENT_ROOT']."/".$filepath;

    if ($fd = fopen ($fullPath, "r")) {
    $fsize = filesize($fullPath);
    $path_parts = pathinfo($fullPath);
    $ext = strtolower($path_parts["extension"]);
        if ($filetype) {
            header("Content-type: $filetype"); // add here more headers for diff. extensions
            header("Content-Disposition: attachment; filename=\"".$params['filename']."\""); // use 'attachment' to force a download
        } else {
            switch ($ext) {
                    case "pdf":
                    header("Content-type: application/pdf"); // add here more headers for diff. extensions
                    header("Content-Disposition: attachment; filename=\"".$params['filename']."\""); // use 'attachment' to force a download
                    break;
                    default;
                    header("Content-type: application/octet-stream");
                    header("Content-Disposition: filename=\"".$params['filename']."\"");
            }
        }
    header("Content-length: $fsize");
    header("Cache-control: private"); //use this to open files directly
    while(!feof($fd)) {
        $buffer = fread($fd, 2048);
        echo $buffer;
    }
    }
    fclose ($fd);
    exit;
}

function ncn_admin_is_first_claim_for_member($claim_id) {
    $claim = ncn_admin_get_claim_info($claim_id);
    if ($claim_id != ncn_admin_get_first_claim_id($claim['user_id'])) {
        return FALSE;
    }
    return TRUE;
}

function ncn_admin_get_first_claim_id($user_id) {
    $result = db_query('SELECT * FROM {claims} WHERE user_id=:a AND claim_status!=:b ORDER BY claim_id ASC',
    array(':a'=>$user_id,':b'=>'unpurchased'));
//  if ($result && $row = db_fetch_array($result) )

    if($result->rowCount()>0)
        foreach($result as $row)
        {
            $row = (array)$row;
            if($row)
            {
                return $row['claim_id'];
            }
        }
    return 0;
}

/*---------------------------------------------------------------------------*/
function ncn_admin_insert_claim_mail_expiration($data) {
    $result = db_query('INSERT INTO {claim_mail_expiration}(claim_id, ts_expired, sent, type) VALUES(:a,:b,:c,:d)',
        array(':a'=>$data['claim_id'],':b'=>$data['ts_expired'],':c'=>$data['sent'],':d'=>$data['type']));
    if (!$result) {
        return FALSE;
    }
    return TRUE;
}
function ncn_admin_get_claim_mail_expiration($claim_id, $type) {
    $result = db_query('SELECT * FROM {claim_mail_expiration} WHERE claim_id=:a AND type=:b',array(':a'=>$claim_id,':b'=>$type));
//  if ( $result && ($row=db_fetch_array($result)) )

    if($result->rowCount()>0)
    {
        foreach($result as $row)
        {
            $row = (array)$row;
            if($row)
            {
                return $row;
            }
        }
    }
    
    return array();
}

function ncn_admin_draw_calendar($month, $year, $data=array()){

  /* draw table */
  $calendar = '<table cellpadding="0" cellspacing="0" class="calendar">';

  /* table headings */
  $headings = array('Sunday','Monday','Tuesday','Wednesday','Thursday','Friday','Saturday');
  $calendar.= '<tr class="calendar-row"><td class="calendar-day-head" width="14.2%">'.implode('</td><td class="calendar-day-head" width="14.2%">',$headings).'</td></tr>';

  /* days and weeks vars now ... */
  $running_day = date('w',strtotime("$month/01/$year"));
  $days_in_month = date('t',strtotime("$month/01/$year"));
  $days_in_this_week = 1;
  $day_counter = 0;
  $dates_array = array();

  /* row for week one */
  $calendar.= '<tr class="calendar-row">';

  /* print "blank" days until the first of the current week */
  for($x = 0; $x < $running_day; $x++):
    $calendar.= '<td class="calendar-day-np">&nbsp;</td>';
    $days_in_this_week++;
  endfor;

  /* keep going with days.... */
  for($list_day = 1; $list_day <= $days_in_month; $list_day++):
    $calendar.= '<td class="calendar-day">';
      /* add in the day number */
      $calendar.= '<div class="day-number">'.$list_day.'</div>';

      /** QUERY THE DATABASE FOR AN ENTRY FOR THIS DAY !!  IF MATCHES FOUND, PRINT THEM !! **/
      if(count($data)>0){
        $calendar.= isset($data[$list_day])?$data[$list_day]:'';
      }
      
    $calendar.= '</td>';
    if($running_day == 6):
      $calendar.= '</tr>';
      if(($day_counter+1) != $days_in_month):
        $calendar.= '<tr class="calendar-row">';
      endif;
      $running_day = -1;
      $days_in_this_week = 0;
    endif;
    $days_in_this_week++; $running_day++; $day_counter++;
  endfor;

  /* finish the rest of the days in the week */
  if($days_in_this_week < 8):
    for($x = 1; $x <= (8 - $days_in_this_week); $x++):
      $calendar.= '<td class="calendar-day-np">&nbsp;</td>';
    endfor;
  endif;

  /* final row */
  $calendar.= '</tr>';

  /* end the table */
  $calendar.= '</table>';
  
  /* all done, return result */
  return $calendar;
}

/**
 * Get Payment Transactions ID paid at this day
 * Return: 
 *          array of PID
 * Caution:
 *          param_1($timestamp) should be 0:0:0
 */ 
function ncn_admin_get_paid_membership($timestamp) {
    $to_ts = strtotime('+1 day', $timestamp);
    $data = array();
    $result = db_query("SELECT * FROM {payment_log} WHERE (ptype='MMF' OR ptype='MMF-GL' OR ptype='MMF-COC' OR ptype='MMF-GC') AND status=1 AND refund=0 AND timestamp>=:timestamp AND timestamp<:to_ts", array(':timestamp' => $timestamp,':to_ts' => $to_ts));
    
    foreach($result as $row) 
    {
        $data[] = (array)$row;
    }
    return $data;
}


/**
 * Get Member ID paid at this day
 * Return: 
 *          2d-array of PID [1..31][]
 * Caution:
 *          param_1($timestamp) should be 0:0:0
 */
function ncn_admin_get_upcoming_membership($timestamp) {
    $to_ts = strtotime('+1 day', $timestamp);
    $data = array();
    
//  $query = "SELECT * FROM users";
    $result = db_query('SELECT * FROM {users}');
    foreach($result as $row)
    {
        $row = (array)$row;
        //$row = (array)$result;
        if ($row['uid'] == 69 || $row['uid'] == 71 || $row['uid'] == 74 || $row['uid'] == 77 || $row['uid'] == 78) {
            continue;
        }
        
        $user = user_load($row['uid']);
    
        if ($user->status==0 && (isset($user->profile_fblocked) && $user->profile_fblocked != 'first')) {
            continue;
        }
        // check if this member is silver
        $member_type = get_member_type($user);
        
        // only for normal users (not distributors or account managers/admin/etc)
        if ( (isset($user->profile_memberid)) && ($user->profile_memberid != '') )
        {
            // check member id is good and get last date
            $member = get_member_from_id($user->profile_memberid);
            
            if (!empty($member) && ($member_type == 0 || $member_type == 2 || $member_type == 3 || $member_type == 4 ) )    // For Gold Member & Gold Lite Member & Coach on Call & Gold Coach
            {
                    $day = ncn_admin_get_day_of_membership_given_month($user->profile_memberid, date('n', $timestamp), date('Y', $timestamp));
                    $data[$day][] = $member;
            }
        }       
    } 
    
    ///////////////////////////////////////////////////////////////////
    // Unactivated Member
//  $query = "SELECT * FROM member_id_pool WHERE used=0 AND status=1";
    $result = db_query('SELECT * FROM {member_id_pool} WHERE used=0 AND status=1')->fetchAll();
    foreach($result as $member) 
    {
        $member = (array)$member;
        if ($member['create'] == 0) { continue; }
        
        if ($member['member_type'] == 0 || $member['member_type'] == 2 || $member['member_type'] == 3 || $member['member_type'] == 4)   // For Gold Member
        {
            $day = ncn_admin_get_day_of_membership_given_month($member['member_id'], date('n', $timestamp), date('Y', $timestamp));
            $data[$day][] = $member;
        }
        
    }
    
    return $data;
}

function ncn_admin_get_day_of_membership_given_month($member_id, $month, $year) {
  $member = get_member_from_id($member_id);
  if (!empty($member)) {
    if ($member['next_payment_date'] == 0) {
      $member['next_payment_date'] = calculate_next_month(date('U'), ncn_user_get_member_renewal_day($member_id));
    }
    
    $day = 0;
    if ( date('n', $member['next_payment_date'])==$month && date('Y', $member['next_payment_date'])==$year ) {
      $day = date('j', $member['next_payment_date']);
    } else {
      $day = ncn_user_get_member_renewal_day($member_id, strtotime("$month/01/$year"));
    }
    return $day;
  }
  return 0;
}

function ncn_admin_get_claim_file_title($type) {
    $title = '';
    switch ($type)
    {
        case "1":
            $title = "Final Invoice";
        break;

        case "2":
            $title = "Claim Document";
        break;
        
        case "3":
            $title = "Service Contract";
        break;
        case "4":
            $title = "Certificate of Completion";
        break;
        case "5":
            $title = "Completed Invoice";
        break;
        
        case "10":
            $title = "CE Claim File";
            break;
        case "11":
            $title = "ESX File";
            break;
        case "12":
            $title = "Invoice PDF File";
            break;

    }
    return $title;
}


function ncn_admin_claim_file_exist($claim_id, $claim_file_type, $revision=0) {
    $result = db_query('SELECT * FROM {claims_invoices} WHERE claim_id=:a and live=:b AND revision=:c',
        array(':a'=>$claim_id,':b'=>$claim_file_type,':c'=>$revision));
//  if ($result && $file = db_fetch_array($result))
    if($result->rowCount()>0)
    {
        foreach($result as $file)
        {
            $file = (array)$file;
            if($file)
            {
                if (is_file($file['filepath'])) {
                    return TRUE;
                }
            }
        }
    }
//    {
//      if (is_file($file['filepath'])) {
//          return TRUE;
//      }
//  }
    
    return FALSE;
}

function ncn_admin_get_claim_file($claim_id, $claim_file_type, $revision=0) {
    $result = db_query('SELECT * FROM {claims_invoices} WHERE claim_id=:a and live=:b AND revision=:c',
        array(':a'=>$claim_id,':b'=>$claim_file_type,':c'=>$revision));
//  if ($result && $file = db_fetch_array($result))

    if($result->rowCount()>0)
    {
        foreach($result as $file)
        {
            $file = (array)$file;
            if($file)
            {
                return $file;
            }
        }
    }
    return FALSE;
}

function ncn_admin_get_workplace_name($uid) {
    $_user = user_load($uid);
    if ($_user) {
        if ((isset($_user->profile_memberid) && is_member($_user->profile_memberid)) || 
            is_distributor($_user) || 
            is_associate($_user) || 
            is_subuser($_user) || 
            is_leaduser($_user))
        {
            return "account";
        } else if (is_claim_examiner($_user)) {
            return "ce_portal/home";
        } else {
            return "admin";
        }
    }
    return FALSE;
}

function ncn_admin_login_programatically($uid) {
global $user;
    $user = user_load($user->uid);
    if (user_access('login for test', $user)) {
    //  $user1 = db_fetch_object(db_query("SELECT * FROM {users} WHERE uid = %d", $uid));
        $user1 = db_query('SELECT * FROM {users} WHERE uid = :d',array(':d'=>$uid))->fetchObject();

        if ($user1) {
            $_SESSION['sandbox_original_uid'] = $user->uid;
            $user = user_load($uid);
            $user->sandbox = "admin";
            $_SESSION['admin_sandbox'] = $uid;

            return TRUE;
        }
    }
    
    return FALSE;
}

function ncn_admin_get_am_fax($am_uid) {
    $fax = "877.654.8668 ext. 103";
    if ($am_uid == 373) {
        $fax = "877.654.8668 ext. 111";
    }
    return $fax;
}

function ncn_admin_get_user_from_claim_id($claim_id) {
    $user_id = db_query('SELECT user_id FROM {claims} WHERE claim_id=:d',array(':d'=>$claim_id))->fetchField();
    if (!$user_id) {
        return FALSE;
    }
    $_user = user_load($user_id);
    return $_user;
}

function ncn_admin_render_date_loss($name, $val) {
    $output = '';
    if (is_array($val)) {
        $month  = $val[0];
        $day        = $val[1];
        $year   = $val[2];
    } else {
        $date = strtotime($val);
        $month  = date('F', $date);
        $day        = date('d', $date);
        $year   = date('Y', $date);
    }
    $d_month = array('January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December');
    $d_day = array(1,2,3,4,5,6,7,8,9,10, 11,12,13,14,15,16,17,18,19,20, 21,22,23,24,25,26,27,28,29,30, 31);
    //$d_year = array(2011, 2012, 2013, 2014);
    $yd = (strtotime(date("Y")."-01-01") - strtotime("2011-01-01"));
    $ydc = floor(($yd)/(60*60*24*365));
    for($y=0;$y<=$ydc;$y++) {
        $d_year[] = (2011+$y);
    }
    
    $output .= ncn_admin_render_date_loss_item($name.'[]', $month, $d_month);
    $output .= ncn_admin_render_date_loss_item($name.'[]', $day, $d_day);
    $output .= ncn_admin_render_date_loss_item($name.'[]', $year, $d_year);
    
    return $output;
}

function ncn_admin_render_date_loss_item($name, $val, $ref_data) {
    $output = "<select name='$name'>";
    foreach($ref_data as $label) {  
        $select = '';
        if ($label == $val) { $select = 'SELECTED'; }
        $output.= "<option value='$label' $select>$label</option>";
    }
    $output.= "</select>";
    return $output;
}

function ncn_admin_get_total_claim_from_user($uid) {
    $count = db_query('SELECT COUNT(claim_id) FROM {claims} WHERE user_id=:a AND claim_status!=:b AND deleted=0',
        array(':a'=>$uid,':b'=>'unpurchased'))->fetchField();
    return $count;
}

function ncn_admin_get_claim_extra_file($id) {
    $result = db_query('SELECT * FROM {claim_extra_file} WHERE id=:a',array(':a'=>$id));
//  if ($result && $row = db_fetch_array($result))
    if($result->rowCount()>0)
    {
        foreach($result as $row)
        {
            $row = (array)$row;
            if($row)
            {
                return $row;
            }
        }
    }
    return FALSE;
}

function user_detail_get_total_claims($_user) {
    if (isset($_user->profile_memberid) && is_member($_user->profile_memberid)) {
        $total_claims = ncn_admin_get_total_claim_from_user($_user->uid);
        return $total_claims;
    }
    return '';
}

function ncn_admin_get_member_type_data() {
    return array(0 => t('Gold Member'), 1 => t('Silver Member'), 2 => t('Gold Lite Member'), 3 => t('Coach on Call Member'), 4 => t('Gold Coach Member'));
}

/**
 * Get Past Renewal Date
 */
function ncn_admin_get_member_renewed_date($member_id) {
    $member = get_member_from_id($member_id);
    $renewed_time = 0;
    
    if ($member) {
        $create_time = $member['create'];
        if ($create_time == 0) {
            $_user = user_load(get_uid_from_memberid($member_id));
            if ($_user) {
                $create_time = $_user->created;
            }
        }
    }
    
    if ($create_time) {
        $time = date('U');
        $day = ncn_admin_get_day_of_membership_given_month($member_id, date('n', $time), date('Y', $time));
        if ( $day <= date('j', $time) ) {
            $renewed_time = strtotime(date('n', $time)."/$day/".date('Y', $time));
        } else {
            $time = strtotime(date("Y/m/01", $time));
            $time = strtotime("-1 month", $time);
            $day = ncn_admin_get_day_of_membership_given_month($member_id, date('n', $time), date('Y', $time));
            $renewed_time = strtotime(date('n', $time)."/$day/".date('Y', $time));
        }
    } 
    
    return $renewed_time;
}

function ncn_admin_get_member_next_renewal_date($member_id) {
    $member = get_member_from_id($member_id);
    
    $renewal_time = 0;
    if ($member) {
        $create_time = $member['create'];
        if ($create_time == 0) {
            $_user = user_load(get_uid_from_memberid($member_id));
            if ($_user) {
                $create_time = $_user->created;
            }
        }
    }
    
    if ($create_time) {
        $time = date('U');
        $day = ncn_admin_get_day_of_membership_given_month($member_id, date('n', $time), date('Y', $time));
        if ( $day <= date('j', $time) ) {
            $time = strtotime(date("Y/m/01", $time));
            $time = strtotime("+1 month", $time);
            $day = ncn_admin_get_day_of_membership_given_month($member_id, date('n', $time), date('Y', $time));
            $renewal_time = strtotime(date('n', $time)."/$day/".date('Y', $time));          
        } else {
            $renewal_time = strtotime(date('n', $time)."/$day/".date('Y', $time));          
        }
    }   
    return $renewal_time;
}

function ncn_admin_get_timer_start_time($claim_id, $action) {
    if ($action != 'start' && $action != 'reset') {
        return FALSE;
    }
    
    $now = date('U');
    $s_time = $now;
    $tt = getdate($now);
    
    if ($tt['hours']>14) {      // 2 PM
        $s_time = strtotime("+1 day 8 hours", strtotime(date("d M Y", $s_time)));
    } else if ($tt['hours']<8) {    // 8 AM
        $s_time = strtotime("+8 hours", strtotime(date("d M Y", $s_time)));
        
    }
    
    if (ncn_admin_is_in_working_time($s_time)) {
        
    } else {
        $tt = getdate($s_time);
        if ( $tt['wday'] == 6 || $tt['wday'] == 0 ) {
            $s_time = strtotime("next Monday");
            $s_time = strtotime("+8 hours", $s_time);
        } else if ( $tt['wday'] == 1 && $tt['hours']<8) {
            $s_time = strtotime(date("d M Y", $s_time));
            $s_time = strtotime("+8 hours", $s_time);
        }
    }
    
    
    return $s_time;
}

function ncn_admin_is_in_working_time($time) {
    $tt = getdate($time);
    /*if ( ( $tt['wday'] == 5 && ($tt['hours']>17 || ($tt['hours']==17 && $tt['minutes']>=30))) ||
            $tt['wday'] == 6 || $tt['wday'] == 0 ) {
        return FALSE;
    } else if ( $tt['wday'] == 1 && ($tt['hours']<8 || ($tt['hours']==8 && $tt['minutes']<30)) ) {
        return FALSE;
    }*/
    if ( $tt['wday'] == 6 || $tt['wday'] == 0 ) {
        return FALSE;
    } else if ($tt['wday'] == 1 && $tt['hours']<8) {
        return FALSE;
    }
    return TRUE;
}

// Get Timer Limitation Length ( unit : second )
function ncn_admin_get_timer_limit_length($claim_id, $action) {
    $val = 27 * 60 * 60;        // 27 Hours
    $claim = ncn_admin_get_claim_info($claim_id);
    if (!empty($claim)) {
        if (strpos(strtolower($claim['claim_product']), 'reconstruction') === 0) {
            $val = 51 * 60 * 60;        // 51 Hours
        }
    }
    return $val;
}

// Get Due to Time from Start Time and Limitation
function ncn_admin_get_due_to_time($start_time, $limit) {
    $to_time = $start_time + $limit;
    if (!ncn_admin_is_in_working_time($to_time) || date('N', $start_time) > date('N', $to_time) ) {
        //$to_time += (15+24*2)*3600;   //From Friday 5:30 PM , To Monday 8:30 AM
        $to_time += (8+24*2)*3600;  //From Saturday 12:00 AM , To Monday 8:00 AM
    }
    
    return $to_time;
}

function get_ncn_data($claim_id, $data_name) {
    $result = db_query('SELECT * FROM {claims_data} WHERE claim_id=:a AND `key`=:b',
        array(':a'=>$claim_id,':b'=>$data_name));
//  if ($data=db_fetch_array($result))
    foreach($result as $data)
    {
        $data = (array)$data;
        if($data)
        {
            return unserialize($data['val']);
        }
    }
    return FALSE;
}

function set_ncn_data($claim_id, $data_name, $val) {
  $result = db_query('UPDATE {claims_data} SET val=:a WHERE claim_id=:b AND `key`=:c',
      array(':a'=>serialize($val),':b'=>$claim_id,':c'=>$data_name));
  return $result;
}

function set_ncn_cd($claim_id, $data_name, $val) {
  $data = get_ncn_data($claim_id, $data_name);
  if ($data) {
    $data['val'] = $val;
    return set_ncn_data($claim_id, $data_name, $data);
  }
  return FALSE;
}

function set_ncn_claim_room_data_extra($claim_id, $room_name, $data_name, $val) {
    $count = intval((db_query('SELECT COUNT(*) FROM {claims_room_data_extra} WHERE claim_id=:a AND room_name=:b AND name=:c',array(':a'=>$claim_id,':b'=>$room_name,':c'=>$data_name))->fetchField()));
    if ($count==0) {
        $result = db_query('INSERT INTO {claims_room_data_extra}(claim_id, room_name, name, val) VALUES(:a,:b,:c,:d)',
            array(':a'=>$claim_id,':b'=>$room_name,':c'=>$data_name,':d'=>$val));
    } else {
        $result = db_query('UPDATE {claims_room_data_extra} SET val=:a WHERE claim_id=:b AND room_name=:c AND name=:d',
            array(':a'=>$val,':b'=>$claim_id,':c'=>$room_name,':d'=>$data_name));
    }
    if (!$result) { return FALSE; }
    return TRUE;
}

function get_ncn_claim_room_data_extra($claim_id, $room_name, $data_name) {
    $result = db_query('SELECT * FROM {claims_room_data_extra} WHERE claim_id=:a AND room_name=:b AND name=:c',
        array(':a'=>$claim_id,':b'=>$room_name,':c'=>$data_name));
//  if ($data=db_fetch_array($result))
    foreach($result as $data)
    {
        $data = (array)$data;
        if($data)
        {
            return $data['val'];
        }
    }
    return '';
}

function ncn_admin_check_exist_claim_room($claim_id, $room_name, $default_room=TRUE) {
//  $query = "SELECT * FROM claims_data WHERE claim_id=".$claim_id." AND `key`='scope'";
    $result = db_query('SELECT * FROM {claims_data} WHERE claim_id=:a AND `key`=:b',array(':a'=>$claim_id,':b'=>'scope'));
    
    if ($result->rowCount()==0) {
        return FALSE;
    }
  
    if ($row = $result->fetchAssoc()) {
        $row['val'] = unserialize($row['val']);
        if (!isset($row['val'][$room_name])) {
            return FALSE;
        } else if ($default_room==FALSE) {
            $default_name = ncn_admin_claim_get_default_room_name($claim_id);
            if ($default_name == $room_name) {
                return FALSE;
            }
        }
    } else {
        return FALSE;
    }
    
    return TRUE;
}

function ncn_admin_claim_get_default_room_name($claim_id) {
    $default_room_name = preg_replace("/[^a-zA-Z0-9\s]/", "", ncn_cd($claim_id, 'customer_name'));
    return $default_room_name;
}

function ncn_admin_get_claim_roomname_data($claim_id, $exclude_default=FALSE) {
//  $query = "SELECT * FROM claims_data WHERE claim_id=".$claim_id." AND `key`='scope'";
    $result = db_query('SELECT * FROM {claims_data} WHERE claim_id=:a AND `key`=:b',array(':a'=>$claim_id,':b'=>'scope'));
    
    if ($result->rowCount()==0) {
        return array();
    }
    
    $a_data = array();
    
    if ($row = $result->fetchAssoc()) {
        $row['val'] = unserialize($row['val']);
        $default_name = ncn_admin_claim_get_default_room_name($claim_id);
        foreach ($row['val'] as $room_name=>$room_data) {
            if ($exclude_default && $room_name == $default_name) {
            } else {
                $a_data[] = array('room_name'=>$room_name, 'photo'=>$room_data);
            }
        }
    } else {
        return array();
    }
    
    return $a_data;
}

function ncn_admin_add_claim_filenote($claim_id, $file_note) {
    if ($file_note == '') {
        return FALSE;
    }
    // maintain file note (history)
    $claim_info = ncn_admin_get_claim_info($claim_id);
    if ($claim_info['file_note']) {
        $file_note = base64_decode($claim_info['file_note'])."\r\n".$file_note;
        $file_note = str_replace("<br/>", "\n", $file_note);        //*temp
    }
    $file_note = base64_encode($file_note);
    
    // save it 
//  $query = "UPDATE {claims} SET file_note=\"%s\" WHERE claim_id=%d ";
    $result = db_query('UPDATE {claims} SET file_note=:a WHERE claim_id=:b',array(':a'=>$file_note,':b'=>$claim_id));
    
    return $result;
}

function ncn_admin_change_first_room_name($claim_id, $new_room_name) {
  $r_value = TRUE;
//  $query2 = "SELECT * FROM claims_data WHERE claim_id=".$claim_id." AND `key`='scope'";
    $result2 = db_query('SELECT * FROM {claims_data} WHERE claim_id=:a AND `key`=:b',array(':a'=>$claim_id,':b'=>'scope'));
    if ($result2->rowCount()>0) {
    $row2 = $result2->fetchAssoc();
    $row2['val'] = unserialize($row2['val']);

    if (isset($row2['val'][$new_room_name])) {
      // error : conflicted with other room name
      if (ncn_admin_claim_get_default_room_name($claim_id) == $new_room_name) {
        // Ignore
      } else {
        $r_value = FALSE;
        drupal_set_message(t('Customer name is conflicted with other room name.'), 'error');
      }
    } else {
      $new_scope = array();
      $old_name = preg_replace("/[^a-zA-Z0-9\s]/", "", ncn_cd($claim_id, 'customer_name'));
      foreach ($row2['val'] as $roomname => $roomdata) {
        if ($roomname == $old_name) {
          $default_name = preg_replace("/[^a-zA-Z0-9\s]/", "", $new_room_name);
          $new_scope[$default_name] = $roomdata;
        } else {
          $new_scope[$roomname] = $roomdata;
        }
      }
      $new_scope = serialize($new_scope);
    //  $query2 = "UPDATE claims_data SET `val`='$new_scope' WHERE claim_id=$claim_id AND `key`='scope'";
      $result2 = db_query('UPDATE {claims_data} SET val=:a WHERE claim_id=:b AND `key`=:c',
          array(':a'=>$new_scope,':b'=>$claim_id,':c'=>'scope'));
      // New Room Captions
      if ($result2->rowCount()>0) {
       // $query3 = "UPDATE claim_img_captions SET roomname='$new_room_name' WHERE claim_id=$claim_id AND roomname='$old_name'";
        $result3 = db_query('UPDATE {claim_img_captions} SET roomname=:a WHERE claim_id=:b AND roomname=:c',
            array(':a'=>$new_room_name,':b'=>$claim_id,':c'=>$old_name));
      }
    }
  }
  return $r_value;
}

function ncn_admin_get_claim_log_additional_data($old, $updated) {
  $claim_log = '';
  $section_data = array(
    'poi' => t('Property Owner Information'), 
    'ci' => t('Claim Information'), 
    'icp' => t('Insurance Policy Information'), 
    'comments' => t('Comments'), 
  );
  
  foreach ($section_data as $key=>$title) {
    $extract = ncn_admin_get_claim_log_diff($updated[$key], $old[$key]);
    if (!empty($extract)) {
      $claim_log.="$title section has been updated.\r\n";
      $claim_log.= ncn_admin_get_claim_log_recurrent_var($extract);
      $claim_log.="\r\n";
    }
  }
  
  return $claim_log;
}

function ncn_admin_get_claim_log_scopesheet_data($old, $updated) {
  
}

function ncn_admin_get_claim_log_diff($data, $against) {
  $extract = array();
  foreach ($data as $key=>$sub_data) {
    if (isset($against[$key]) && $sub_data == $against[$key]) {
    } else {
      if (is_array($sub_data)) {
        $extract[$key] = ncn_admin_get_claim_log_diff($sub_data, $against[$key]);
      } else {
        $extract[$key] = $sub_data;
      }
      
    }
  }
  return $extract;
}

function ncn_admin_get_claim_log_recurrent_var($data, $space=4) {
  $log = '';
  if (is_array($data)) {
    foreach ($data as $key=>$val) {
      if (is_array($val)) {
        $log.=str_repeat("&nbsp;", $space)."[$key] => \r\n";
        $log.= ncn_admin_get_claim_log_recurrent_var($val, $space+4);
      } else {
        $log.= str_repeat("&nbsp;", $space)."[$key] => $val \r\n";
      }
    }
  }
  return $log;
}

function ncn_admin_is_user_DriRite($uid) {
  $_user = user_load($uid);
  foreach ($_user->roles as $role)
  {
    if (strtolower($role) == "dririte") {  return true;  }
  }
  
  return false;
}

function ncn_admin_remove_special_char_from_customer_name($customer_name) {
  $special_chars = array("\\", "/", "<", ">", "\"");
  foreach ($special_chars as $schar) {
    $customer_name = str_replace($schar, "", $customer_name);
  }
  return $customer_name;
}

//------------------------------------------------------------------------------
function ncn_debug_message($message)
{
    if (isset($GLOBALS['ncn_debug']) && $GLOBALS['ncn_debug'] == true)
    {
        drupal_set_message($message, 'error');
    }
}

function get_member_from_id($member_id) {
    $query = db_query('SELECT * FROM member_id_pool WHERE member_id=:a',array(':a'=>$member_id));
//    $result = db_select('member_id_pool', 'n')
//        ->fields('n')
//        ->condition('member_id',$member_id,'=')
//        ->execute();
    $result = $query;
    if ($result && $result->rowCount()>0) {
        $member = $result->fetchAssoc();
        //$member = (array)$result;
        return $member;
    }
    
    return array();
}

function get_item_from_key($data, $key, $val) {
    for ($i=0; $i<count($data); $i++){
        $item = $data[$i];
        if ($item[$key] == $val) {
            return $item;
        }
    }
    
    return false;
}
