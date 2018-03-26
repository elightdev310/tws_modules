<?php
// $Id: case.inc,v 1.1 2010/12/08 14:31:47 redijedi Exp $

/**
 * @file
 * The Capsule API case specific functions.
 */


/**
 * Gets a case.
 * 
 * @param int $kase_id
 */
function capsule_get_case($kase_id) {
  return _capsule_request("/kase/$kase_id");
}

/**
 * Lists cases matching the criteria provided.
 * 
 */
function capsule_list_cases($start = 0, $limit = 10) {
  $query = _capsule_query_params(array(
    'start' => $start,
    'limit' => $limit,
  ));
  return _capsule_request("/kase$query");
}

/**
 * Lists cases matching the criteria provided.
 * 
 */
function capsule_list_cases_since($lastmodified, $start = 0, $limit = 10) {
  $query = _capsule_query_params(array(
    'lastmodified' => $lastmodified,
    'start' => $start,
    'limit' => $limit,
  ));
  return _capsule_request("/kase$query");
}

/**
 * Lists cases matching the criteria provided.
 * 
 */
function capsule_list_cases_with_tag($tag, $start = 0, $limit = 10) {
  $query = _capsule_query_params(array(
    'tag' => $tag,
    'start' => $start,
    'limit' => $limit,
  ));
  return _capsule_request("/kase$query");
}

/**
 * Delete a case.
 * 
 * @param int $kase_id
 */
function capsule_delete_case($kase_id) {
  return _capsule_request("/kase/$kase_id", 'DELETE');
}

/**
 * Add history to a case.
 * 
 * @param int $kase_id
 * @param array $data
 */
function capsule_add_case_history($kase_id, $data = array()) {
  return _capsule_request("/kase/$kase_id/history", 'POST', $data);
}

/**
 * Create a new case task attached to the case specified.
 * 
 * @param int $kase_id
 * @param array $data
 */
function capsule_add_case_task($kase_id, $data = array()) {
  return _capsule_request("/kase/$kase_id/task", 'POST', $data);
}
