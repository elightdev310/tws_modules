<?php

// $Id: ncn_api_cm_room.inc
// Description: Managements for Claim Room & Photo

function ncn_api_cm_room_menu(&$items)
{
	$items[NCN_API_PREFIX . '/claim/%/rooms/get'] = array(
		'title' => 'Claim Rooms',
		'page callback' => 'ncn_api_cm_room_claim_rooms_get',
		'page arguments' => array(
			2
		) ,
		'access callback' => TRUE,
		'type' => MENU_CALLBACK,
	);
	$items[NCN_API_PREFIX . '/claim/%/room/default/photos/get'] = array(
		'title' => 'Claim Rooms',
		'page callback' => 'ncn_api_cm_room_claim_room_default_photos_get',
		'page arguments' => array(
			2
		) ,
		'access callback' => TRUE,
		'type' => MENU_CALLBACK,
	);
	$items[NCN_API_PREFIX . '/claim/%/room/%/photos/get'] = array(
		'title' => 'Claim Rooms',
		'page callback' => 'ncn_api_cm_room_claim_room_photos_get',
		'page arguments' => array(
			2,
			4
		) ,
		'access callback' => TRUE,
		'type' => MENU_CALLBACK,
	);
	$items[NCN_API_PREFIX . '/claim/%/room/new'] = array(
		'title' => 'Claim Rooms',
		'page callback' => 'ncn_api_cm_room_claim_room_new',
		'page arguments' => array(
			2
		) ,
		'access callback' => TRUE,
		'type' => MENU_CALLBACK,
	);
	$items[NCN_API_PREFIX . '/claim/%/room/%/rename'] = array(
		'title' => 'Claim Rooms',
		'page callback' => 'ncn_api_cm_room_claim_room_rename',
		'page arguments' => array(
			2,
			4
		) ,
		'access callback' => TRUE,
		'type' => MENU_CALLBACK,
	);
	$items[NCN_API_PREFIX . '/claim/%/room/%/delete'] = array(
		'title' => 'Claim Rooms',
		'page callback' => 'ncn_api_cm_room_claim_room_delete',
		'page arguments' => array(
			2,
			4
		) ,
		'access callback' => TRUE,
		'type' => MENU_CALLBACK,
	);
	$items[NCN_API_PREFIX . '/claim/%/room/%/captions/reset'] = array(
		'title' => 'Claim Room Photo',
		'page callback' => 'ncn_api_cm_room_claim_room_captions_reset',
		'page arguments' => array(
			2,
			4
		) ,
		'access callback' => TRUE,
		'type' => MENU_CALLBACK,
	);
	$items[NCN_API_PREFIX . '/claim/%/room/%/photo/upload'] = array(
		'title' => 'Claim Room Photo',
		'page callback' => 'ncn_api_cm_room_claim_room_photo_upload',
		'page arguments' => array(
			2,
			4
		) ,
		'access callback' => TRUE,
		'type' => MENU_CALLBACK,
	);
}

/**
 * API Hanlder ( claim/%/rooms/get )
 */

function ncn_api_cm_room_claim_rooms_get($claim_id)
{
	$data = $_REQUEST;
	$op_claim_room = array(
		'response' => 0,
	);
	$m_data = array(
		'request_type' => 'get',
		'response_status' => 0,
	);
	$user_id = ncn_api_user_get_userid_from_token($data['username'], $data['token']);
	if ($user_id && ncn_api_claim_manager_check_claim_access($claim_id, $user_id))
	{
		$op_claim_room['response'] = 1;
		$op_claim_room['claim_id'] = $claim_id;

		// $op_claim_room['rooms']['{room}'] = ncn_api_cm_room_get_claim_rooms_data($claim_id, FALSE);

		$op_claim_room['rooms'] = ncn_api_cm_room_get_claim_rooms_data($claim_id, FALSE);
	}
	else
	{
		$op_claim_room['error']['access_denied']['message'] = t("You're not authorized to access claim(#!claim_id)", array(
			'!claim_id' => $claim_id
		));
	}

	$output['claimRooms'] = $op_claim_room;
	$m_data['response_status'] = $op_claim_room['response'];
	ncn_api_response(array(
		'data' => $output,
		'meta_data' => $m_data
	));
}

function ncn_api_cm_room_get_claim_rooms_data($claim_id, $b_include_default = TRUE)
{
	$rooms = array();
	$result = db_query('SELECT * FROM {claims_data} WHERE claim_id=:a AND `key`=:b', array(
		':a' => $claim_id,
		':b' => 'scope'
	));

	//	if ($result && $row=db_fetch_array($result))

	if ($result)
	{
		foreach($result as $row)
		{
			$row = (array)$row;
			if ($row)
			{
				$data = unserialize($row['val']);
				$default_room_name = ncn_admin_claim_get_default_room_name($claim_id);
				foreach($data as $roomname => $roomdata)
				{
					if ($roomname != "type")
					{
						if ($b_include_default || (!$b_include_default && $roomname != $default_room_name))
						{
							$rooms[] = array(
								'name' => $roomname
							);
						}
					}
				}
			}
		}
	}

	return $rooms;
}

function ncn_api_cm_room_check_exist_room($claim_id, $room_name, $b_include_default = FALSE)
{
	$rooms = ncn_api_cm_room_get_claim_rooms_data($claim_id, $b_include_default);
	foreach($rooms as $room)
	{
		if ($room['name'] == $room_name)
		{
			return TRUE;
		}
	}

	return FALSE;
}

/**
 * API Hanlder ( claim/%/room/default/photos/get )
 */

function ncn_api_cm_room_claim_room_default_photos_get($claim_id)
{
	$default_room_name = ncn_admin_claim_get_default_room_name($claim_id);
	ncn_api_cm_room_claim_room_photos_get($claim_id, $default_room_name);
}

/**
 * API Hanlder ( claim/%/room/%/photos/get )
 */

function ncn_api_cm_room_claim_room_photos_get($claim_id, $room_name)
{
	$data = $_REQUEST;
	$op_claim_room_photo = array(
		'response' => 0,
	);
	$m_data = array(
		'request_type' => 'get',
		'response_status' => 0,
	);
	$user_id = ncn_api_user_get_userid_from_token($data['username'], $data['token']);
	if ($user_id && ncn_api_claim_manager_check_claim_access($claim_id, $user_id))
	{
		$op_claim_room_photo['claim_id'] = $claim_id;
		$op_claim_room_photo['room_name'] = $room_name;
		$p_result = ncn_api_cm_room_get_claim_room_photo_data($claim_id, $room_name);
		if ($p_result['success'])
		{
			$op_claim_room_photo['response'] = 1;

			// $op_claim_room_photo['photos']['{photo}'] = $p_result['photos'];

			$op_claim_room_photo['photos'] = $p_result['photos'];
		}
		else
		{
			$op_claim_room_photo['error'] = $p_result['error'];
		}
	}
	else
	{
		$op_claim_room['error']['access_denied']['message'] = t("You're not authorized to access claim(#!claim_id)", array(
			'!claim_id' => $claim_id
		));
	}

	$output['claimRoomPhotos'] = $op_claim_room_photo;
	$m_data['response_status'] = $op_claim_room_photo['response'];
	ncn_api_response(array(
		'data' => $output,
		'meta_data' => $m_data
	));
}

function ncn_api_cm_room_get_claim_room_photo_data($claim_id, $room_name)
{
	$r_output = array(
		'success' => FALSE,
		'code' => 0,
	);
	$result = db_query('SELECT * FROM {claims_data} WHERE claim_id=:a AND `key`=:b', array(
		':a' => $claim_id,
		':b' => 'scope'
	));
	$room_data = array();

	//	if ($result && $row=db_fetch_array($result))

	if ($result)
	{
		foreach($result as $row)
		{
			$row = (array)$row;
			if ($row)
			{
				$data = unserialize($row['val']);
			}
		}
	}

	//    {
	//		$data = unserialize($row['val']);
	//	}

	else
	{
		$r_output['code'] = 101;

		// $r_output['error']['no_claim']['message'] = t("Claim(#!claim_id) doesn't exist.", array('!claim_id'=>$claim_id));

	}

	$default_room_name = ncn_admin_claim_get_default_room_name($claim_id);
	if (isset($data[$room_name]) || $room_name == $default_room_name)
	{
		$room_data = $data[$room_name];
		$image_count = 6;
		if ($room_name == $default_room_name)
		{
			$image_count = 3;
		}

		$r_output['success'] = TRUE;
		$r_output['code'] = 200;
		$photos = array();
		$captions = _get_image_captions($claim_id, $room_name, $image_count);
		for ($i = 0; $i < $image_count; $i++)
		{
			$img_index = 'image' . $i;
			$photo = array();
			$photo['caption'] = $captions[$i];
			if (isset($room_data[$img_index]))
			{
				$img = $room_data[$img_index];
				$image_url = $GLOBALS['base_url'] . base_path() . $img['path'];
				$photo['image_url'] = $image_url;
			}
			else
			{
				$image_url = _get_blank_room_image_url($image_count, $i);
				$photo['image_url'] = '';
				$photo['blank_image_url'] = $image_url;
			}

			$photos[] = $photo;
		}

		$r_output['photos'] = $photos;
	}
	else
	{
		$r_output['code'] = 102;
		$r_output['error']['no_room']['message'] = t("Claim(#!claim_id) hasn't room(!room_name).", array(
			'!claim_id' => $claim_id,
			'!room_name' => $room_name
		));
	}

	return $r_output;
}

/**
 * API Hanlder ( claim/%/room/new )
 */

function ncn_api_cm_room_claim_room_new($claim_id)
{
	$data = $_REQUEST;
	$room_name = $data['room_name'];
	$room_name = preg_replace("/[^a-zA-Z0-9\s]/", "", $room_name);
	$op_claim_new_room = array(
		'response' => 0,
	);
	$m_data = array(
		'request_type' => 'set',
		'response_status' => 0,
	);
	$user_id = ncn_api_user_get_userid_from_token($data['username'], $data['token']);
	if ($user_id && ncn_api_claim_manager_check_claim_access($claim_id, $user_id))
	{
		$op_claim_new_room['claim_id'] = $claim_id;
		$op_claim_new_room['room_name'] = $room_name;
		$p_result = ncn_admin_photo_add_room($claim_id, $room_name);
		if ($p_result)
		{
			$op_claim_new_room['response'] = 1;
		}
		else
		{
			$dp_message = ncn_api_get_dp_message();
			$op_claim_new_room['error'] = $dp_message;
		}
	}
	else
	{
		$op_claim_new_room['error']['access_denied']['message'] = t("You're not authorized to access claim(#!claim_id)", array(
			'!claim_id' => $claim_id
		));
	}

	$output['claimNewRoom'] = $op_claim_new_room;
	$m_data['response_status'] = $op_claim_new_room['response'];
	ncn_api_response(array(
		'data' => $output,
		'meta_data' => $m_data
	));
}

/**
 * API Hanlder ( claim/%/room/%/rename )
 */

function ncn_api_cm_room_claim_room_rename($claim_id, $room_name)
{
	$data = $_REQUEST;
	$new_room_name = trim($data['new_room_name']);
	$new_room_name = preg_replace("/[^a-zA-Z0-9\s]/", "", $new_room_name);
	$op_claim_rename_room = array(
		'response' => 0,
	);
	$m_data = array(
		'request_type' => 'set',
		'response_status' => 0,
	);
	$user_id = ncn_api_user_get_userid_from_token($data['username'], $data['token']);
	if ($user_id && ncn_api_claim_manager_check_claim_access($claim_id, $user_id))
	{
		$op_claim_rename_room['claim_id'] = $claim_id;
		$op_claim_rename_room['room_name'] = $room_name;
		$op_claim_rename_room['new_room_name'] = $new_room_name;
		if ($new_room_name == '')
		{
			$op_claim_rename_room['error']['empty_roomname']['message'] = t("You should provide new room name.");
		}
		else
		{
			$p_result = ncn_admin_photo_rename_room($claim_id, $room_name, $new_room_name);
			if ($p_result['code'] == 'success')
			{
				$op_claim_rename_room['response'] = 1;
			}
			else
			{
				$op_claim_rename_room['error'][$p_result['code']]['message'] = $p_result['msg'];
			}
		}
	}
	else
	{
		$op_claim_rename_room['error']['access_denied']['message'] = t("You're not authorized to access claim(#!claim_id)", array(
			'!claim_id' => $claim_id
		));
	}

	$output['claimRenameRoom'] = $op_claim_rename_room;
	$m_data['response_status'] = $op_claim_rename_room['response'];
	ncn_api_response(array(
		'data' => $output,
		'meta_data' => $m_data
	));
}

/**
 * API Hanlder ( claim/%/room/%/delete )
 */

function ncn_api_cm_room_claim_room_delete($claim_id, $room_name)
{
	$data = $_REQUEST;
	$op_claim_delete_room = array(
		'response' => 0,
	);
	$m_data = array(
		'request_type' => 'set',
		'response_status' => 0,
	);
	$user_id = ncn_api_user_get_userid_from_token($data['username'], $data['token']);
	if ($user_id && ncn_api_claim_manager_check_claim_access($claim_id, $user_id))
	{
		$op_claim_delete_room['claim_id'] = $claim_id;
		$op_claim_delete_room['room_name'] = $room_name;
		$p_result = ncn_admin_photo_delete_room($claim_id, $room_name);
		if ($p_result)
		{
			$op_claim_delete_room['response'] = 1;
		}
		else
		{
			$dp_message = ncn_api_get_dp_message();
			$op_claim_delete_room['error'] = $dp_message;
		}
	}
	else
	{
		$op_claim_delete_room['error']['access_denied']['message'] = t("You're not authorized to access claim(#!claim_id)", array(
			'!claim_id' => $claim_id
		));
	}

	$output['claimDeleteRoom'] = $op_claim_delete_room;
	$m_data['response_status'] = $op_claim_delete_room['response'];
	ncn_api_response(array(
		'data' => $output,
		'meta_data' => $m_data
	));
}

/**
 * API Hanlder ( claim/%/room/%/photo/upload/% )
 */

function ncn_api_cm_room_claim_room_photo_upload($claim_id, $room_name)
{
	global $base_url;
	$photo_index = intval(arg(7));
	$data = $_REQUEST;
	if ($room_name == 'default')
	{
		$room_name = ncn_admin_claim_get_default_room_name($claim_id);
	}
	
	$op_cr_photo_upload = array(
		'response' => 0,
		'claim_id' => $claim_id,
		'room_name' => $room_name,
	);
	$m_data = array(
		'request_type' => 'set',
		'response_status' => 0,
	);
	$default_room_name = ncn_admin_claim_get_default_room_name($claim_id);
	$user_id = ncn_api_user_get_userid_from_token($data['username'], $data['token']);
	if ($user_id && ncn_api_claim_manager_check_claim_access($claim_id, $user_id))
	{
		if (ncn_api_cm_room_check_exist_room($claim_id, $room_name, TRUE) || $room_name == $default_room_name)
		{
			$image_count = 6;
			if ($room_name == $default_room_name)
			{
				$image_count = 3;
			}

			if ($photo_index < $image_count)
			{
				/*$image = str_replace(" ","+", $data['image']); // need to replace " " with "+" in image string.
				$image = base64_decode($image);*/
				$image_filename = '';
				if (isset($data['filename']))
				{
					$image_filename = preg_replace("/[^a-zA-Z0-9\s\.]/", "_", $data['filename']);
				}
				else
				{
					$image_filename = "image_$photo_index_" . date('U');
				}
				
				$url = "sites/default/files/user_files/$claim_id";
				if (!is_dir($url))
				{
					mkdir($url, 0775);
				}
				
				$source = 'image';
				$file = file_save_upload($source, array(), "public://user_files/$claim_id", FILE_EXISTS_RENAME);
				if (!$file)
				{
					$op_cr_photo_upload['error']['fail_save_image']['message'] = t("Failed to upload image data to file.");
				}
				else
				{		
					$file->status = FILE_STATUS_PERMANENT; //Set the status of the uploaded file.
					$result = file_save($file);						
					//$result = file_copy($file, $url);
					
					if ($result)
					{
						$file->filepath = str_replace("public://","sites/default/files/",$file->uri);
						$filepath = $file->filepath;

						// scale 500x350 initially.
						$api_params = array(
							'action' => 'scale',
							'width' => 500,
							'height' => 350,
							'upscale' => FALSE,
							'source' => $filepath,
							'dest' => $filepath
						);
						if (TRUE) /*_ncn_image_api('scale', $api_params)*/
						{
							$op_cr_photo_upload['image_url'] = $base_url . base_path() . $filepath;
							$result = db_query('SELECT * FROM {claims_data} WHERE claim_id=:a AND `key`=:b', array(':a' => $claim_id,':b' => 'scope'));
							$room_data = array();
							$b_insert = FALSE;

							if ($result->rowCount()>0)
							{
								$row = $result->fetchAssoc();
								$s_data = unserialize($row['val']);
							}
							else
							{
								$b_insert = TRUE;
							}						
							
							if (isset($s_data[$room_name]))
							{
								$room_data = $s_data[$room_name];
							}

							$img_index = 'image' . $photo_index;
							$room_data[$img_index]['path'] = $filepath;
							$s_data[$room_name] = $room_data;
							$ss_data = serialize($s_data);
							if ($b_insert)
							{
								$result = db_query('INSERT INTO {claims_data} (`claim_id`, `field_type`, `key`, `val`, `weight`) VALUES (:a,:b,:c,:d,:e)', 
													array(':a' => $claim_id, ':b' => '', ':c' => 'scope', ':d' => $ss_data, ':e' => 0));
							}
							else
							{
								$result = db_query('UPDATE {claims_data} SET val=:a WHERE claim_id=:b AND `key`=:c', array(
									':a' => $ss_data,
									':b' => $claim_id,
									':c' => 'scope'
								));
							}

							if ($result)
							{
								$op_cr_photo_upload['response'] = 1;
							}
							else
							{
								$op_cr_photo_upload['error']['fail_storeDB_image']['message'] = t("Failed to save image information to Database.");
							}
						}
						else
						{
							$op_cr_photo_upload['error']['fail_scale_image']['message'] = t("Failed to scale image.");
						}
					}
					else
					{
						$op_cr_photo_upload['error']['fail_save_image']['message'] = t("Failed to save image data to file.");
					}
				}
			}
			else
			{
				$op_cr_photo_upload['error']['no_room']['message'] = t("Room(!room_name) has only !image_count images.", array(
					'!room_name' => $room_name,
					'!image_count' => $image_count
				));
			}
		}
		else
		{
			$op_cr_photo_upload['error']['no_room']['message'] = t("Claim(#!claim_id) doesn't have room(!room_name)", array(
				'!claim_id' => $claim_id,
				'!room_name' => $room_name
			));
		}
	}
	else
	{
		$op_cr_photo_upload['error']['access_denied']['message'] = t("You're not authorized to access claim(#!claim_id)", array(
			'!claim_id' => $claim_id
		));
	}

	$output['claimRoomPhotoUpload'] = $op_cr_photo_upload;
	$m_data['response_status'] = $op_cr_photo_upload['response'];
	ncn_api_response(array(
		'data' => $output,
		'meta_data' => $m_data
	));
}

function ncn_api_cm_room_claim_room_captions_reset($claim_id, $room_name)
{
	$op_cr_caption_reset = array(
		'response' => 0,
		'claim_id' => $claim_id,
		'room_name' => $room_name,
	);
	$m_data = array(
		'request_type' => 'set',
		'response_status' => 0,
	);
	$result = db_query('DELETE FROM {claim_img_captions} WHERE claim_id=:a AND roomname=:b', array(
		':a' => $claim_id,
		':b' => $room_name
	));
	if ($result)
	{
		$op_cr_caption_reset['response'] = 1;
	}

	$output['claimRoomCaptionReset'] = $op_cr_caption_reset;
	$m_data['response_status'] = $op_cr_caption_reset['response'];
	ncn_api_response(array(
		'data' => $output,
		'meta_data' => $m_data
	));
}
