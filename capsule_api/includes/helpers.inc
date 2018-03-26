<?php 
// $Id: helpers.inc,v 1.1 2010/12/08 14:31:47 redijedi Exp $

/**
 * @file
 * Utility functions.
 */

/**
 * Resolves the URL provided using the HTTP method and data provided.
 * 
 * @return The data provided at the resource specified.
 */
function _capsule_request($url, $op = 'GET', $data = NULL) {
global $user;
  /*$user_token = ncn_capsulecrm_token_get_user_token($user);
  if ($user_token == '') {
    $user_token = NULL;
  }*/
  $user_token = variable_get('capsule_user_token', NULL);
  
  $response = _capsule_http_request($url, $op, $data, $user_token);
  return $response;
  /*if (empty($response->data)) {
    return NULL;
  }
  else {
    return json_decode($response->data, TRUE);
  }*/
}

/**
 * Resolves the URL provided using the HTTP method and data provided.
 * 
 * @return The raw response object. 
 */
function _capsule_http_request($path, $op = 'GET', $data = NULL, $user_token = NULL, $key = NULL) {
  $url = _capsule_api_url($key) . $path;

  $json = json_encode($data);
    if ($user_token == NULL) {
    $user_token = variable_get('capsule_user_token', '');
  }
  
  // $headers = array(
  //   'Accept' => 'application/json',
  //   'Content-Type' => 'application/json',
  //   'Authorization' => 'Bearer ' . $user_token,
  // );
  // $response = drupal_http_request($url, array('headers' => $headers, 'method' => $op, 'data' => $json));
  // 
  // watchdog('capsule', "Attempting to resolve $url.", NULL, WATCHDOG_DEBUG);

  $headers = array(
    'Accept: application/json',
    'Content-Type: application/json',
    'Authorization: Bearer ' . $user_token,
  );

  // Get cURL resource
  $curl = curl_init();
  // Set some options - we are passing in a useragent too here
  curl_setopt_array($curl, array(
      CURLOPT_RETURNTRANSFER => 1,
      CURLOPT_URL => $url,
      //CURLOPT_HEADER => 1, 
      CURLOPT_USERAGENT => 'netclaimsnow'
  ));
  curl_setopt($curl, CURLOPT_HTTPHEADER, $headers);

  if ($op == 'POST') {
    curl_setopt($curl, CURLOPT_POST, 1);
    curl_setopt($curl, CURLOPT_POSTFIELDS, json_encode($data)); 
  } else if ($op == 'PUT') {
    curl_setopt($curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_setopt($curl, CURLOPT_POSTFIELDS, json_encode($data)); 
  }

  // Send the request & save response to $resp
  $response = curl_exec($curl);
  $curl_info = curl_getinfo($curl);

  // Close request to clear up some resources
  curl_close($curl);

  $resp = json_decode($response);
  $resp->code = $curl_info['http_code'];
  // watchdog('capsule', "Resolved URL $url returned status code $response->code.", NULL, WATCHDOG_DEBUG);

  return $resp;
}

/**
 * Takes a variable number of arguments and adds them to a string in query param format:
 * 
 * ?KEY=VALUE&OTHER_KEY=OTHER_VALUE...
 * 
 * @param params
 *   The array of key => value parameters.
 */
function _capsule_query_params($params = array()) {
  $query = '';
  foreach ($params as $key => $value) {
    $query .= "&$key=$value";
  }
  return '?' . drupal_substr($query, 1);
}

function _capsule_api_url($key = NULL) {
  return "https://api.capsulecrm.com/api/v2";

  if (empty($key)) {
    $key = variable_get('capsule_url_key', '');
      if(empty($key))
      {
          $key = "netclaimsnow-com";
      }
      else
      {
          $key;
      }
  }

  return "https://$key.capsulecrm.com/api";
}
