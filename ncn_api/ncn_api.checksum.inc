<?php

function ncn_api_get_meta_data($data)
{
	if (isset($data['meta_data']) && is_array($data['meta_data']))
	{
		$m_data = $data['meta_data'];
	}
	else
	{
		$m_data = array();
	}

	$params = $_GET;
	$url = $_GET['q'];
	unset($params['q']);
	if (isset($params['checksum']))
	{
		$m_data['checksum'] = $params['checksum'];
		unset($params['checksum']);
	}

	if (!empty($params))
	{
		$url.= "?" . http_build_query($params);
	}

	$m_data['url'] = $url;
	return $m_data;
}

// XML

/*function ncn_api_get_response_with_checksum($str_data, $m_data) {

	$data = '';
	
	if ($m_data['request_type'] == 'get') 
	{
		if ($m_data['response_status']) 
		{
			$checksum = ncn_api_get_data_checksum($str_data);			
			if (isset($m_data['checksum']) && $checksum==$m_data['checksum']) {
				$m_data['app_local_data'] = true;			
			} else {			
				$m_data['app_local_data'] = false;
			}			
		}
	}
	
	unset($m_data['checksum']);
	
	if (isset($checksum)) { $m_data['checksum'] = $checksum; }
	
	if (!empty($m_data)) {
	
	$m_data['url_key'] = md5($m_data['url']);
	
	unset($m_data['url']);
	
	$data = ncn_api_get_xml_response(array('meta_data'=>$m_data));
	
	}
	
	if (!$m_data['app_local_data']) {
	
	$data .= $str_data;
	
	}
	
	return $data;

}*/

// JSON

function ncn_api_get_json_response_with_checksum($data, $m_data)
{
	$str_data = ncn_api_get_json_response($data);
	$r_data = array();
	if ($m_data['request_type'] == 'get')
	{
		if ($m_data['response_status'])
		{
			$checksum = ncn_api_get_data_checksum($str_data);
			if (isset($m_data['checksum']) && $checksum == $m_data['checksum'])
			{
				// KEYiDEAS Revert it to later stage
				$m_data['app_local_data'] = 1;
				//$m_data['app_local_data'] = 0;
			}
			else
			{
				$m_data['app_local_data'] = 0;
			}
		}
	}
	
	unset($m_data['checksum']);
	if (isset($checksum))
	{
		$m_data['checksum'] = $checksum;
	}

	if (!empty($m_data))
	{
		$m_data['url_key'] = md5($m_data['url']);
		unset($m_data['url']);
		$r_data['meta_data'] = $m_data;
	}

	if (!$m_data['app_local_data'])
	{
		$r_data = array_merge($r_data, $data);
	}

	return $r_data;
}

function ncn_api_get_data_checksum($str_data)
{
	return md5($str_data);
}