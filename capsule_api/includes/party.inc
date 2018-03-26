<?php
// $Id: party.inc,v 1.1 2010/12/08 14:31:47 redijedi Exp $

/**
 * @file
 * The Capsule API party functions.
 */


/**
 * Get details of a single person or organisation by party id.
 * 
 * @param int $party_id
 */
function capsule_get_party($party_id, $embed='') {
  if ($embed) {
    return _capsule_request("/parties/$party_id?embed=$embed");
  }
  return _capsule_request("/parties/$party_id");
}

/**
 * Return all people & organisations visible to the authenticated user. Optionally the results can be limited or paged 
 * using the parameters &limit and &start.
 * [v2]
 */
function capsule_list_parties($page = 0, $perPage = 10) {
  $query = _capsule_query_params(array(
    'page'    => $page,
    'perPage' => $perPage,
  ));
  return _capsule_request("/parties$query");
}

/**
 * Return all people & organisations which match the search term. The search term will be matched against name, 
 * telephone number and exact match on searchable custom fields. Optionally the results can be limited or paged using 
 * the parameters &limit and &start.
 * [v2]
 */
function capsule_search_parties($q, $page = 0, $perPage = 10) {
  $query = _capsule_query_params(array(
    'q' => urlencode($q),
    'page'    => $page,
    'perPage' => $perPage,
  ));
  return _capsule_request("/parties/search$query");
}

/**
 * Return person & organisation records that have been updated or created since the modified since date supplied. The 
 * modified since date should be in the format YYYYMMDDTHHMMSS, eg. Midnight June 31, 2009 GMT would be 
 * 20090631T000000. Optionally the results can be limited or paged using the parameters &limit and &start.
 * 
 * @param date $since
 * @param int $start
 * @param int $limit
 */
function capsule_list_parties_since($since, $start = 0, $limit = 10) {
  $query = _capsule_query_params(array(
    'lastmodified' => $since,
    'start' => $start,
    'limit' => $limit,
  ));
  return _capsule_request("/parties$query");
}

/**
 * Return all people & organisations with matching email address. Optionally the results can be limited or paged using 
 * the parameters &limit and &start.
 * 
 * @param string $email
 * @param int $start
 * @param int $limit
 */
function capsule_list_parties_with_email($email, $start = 0, $limit = 10) {
  $query = _capsule_query_params(array(
    'email' => urlencode($email),
    'start' => $start,
    'limit' => $limit,
  ));
  return _capsule_request("/parties$query");
}

/**
 * Return all people & organisations with tagged with matching tag. Note that the tag will need to be URL encoded. 
 * Optionally the results can be limited or paged using the parameters &limit and &start.
 * 
 * @param string $tag
 * @param int $start
 * @param int $limit
 */
function capsule_list_parties_with_tag($tag, $start = 0, $limit = 10) {
  $query = _capsule_query_params(array(
    'tag' => urlencode($tag),
    'start' => $start,
    'limit' => $limit,
  ));
  return _capsule_request("/parties$query");
}

/**
 * Returns all the people at the selected organisation.
 * 
 * @param int $party_id
 */
function capsule_list_party_people($party_id) {
  return _capsule_request("/parties/$party_id/people");
}

/**
 * Create a new person and optionally attach to an organisation. You can specify an organisation id or organisation 
 * name. If an organisation name is used an it is not already present on the account a new organisation will be 
 * created. 
 * 
 * When adding addresses make the <country /> element if used should be populated with a country returned from the 
 * resource GET /api/resource/country or a ISO 3166-1 alternative. 
 * 
 * If adding this person will exceed the accounts contact limit a 507 Insufficient Storage response will be returned.
 * 
 * [v2]
 */
function capsule_add_person($person = array()) {
  return _capsule_request("/parties", 'POST', $person);
}

/**
 * If adding this organisation will exceed the accounts contact limit a 507 
 * Insufficient Storage response will be returned.
 * 
 * [v2]
 */
function capsule_add_organization($organization = array()) {
  return _capsule_request("/parties", 'POST', $organization);
}

/**
 * Update an existing person record, only attributes that are to be changed need to be supplied in the XML document. 
 * Where a value is not supplied it will remain with its current value. To null a field include a empty XML element 
 * for the field in the document.
 * 
 * Contact records (email, phone or address) can be updated where an id is provided, where an id for a contact is not 
 * provided it is assumed to be null and will be added to the person.
 * 
 * [v2]
 */
function capsule_update_person($person = array(), $id) {
  return _capsule_request("/parties/$id", 'PUT', $person);
}

/**
 * Update an existing organisation record, only attributes that are to be changed need to be supplied in the XML 
 * document. Where a value is not supplied it will remain with its current value. To null a field include a empty XML 
 * element for the field in the document. 
 * 
 * Contact records (email, phone or address) can be updated where an id is provided, where an id for a contact is not 
 * provided it is assumed to be null and will be added to the person.
 * 
 * [v2]
 */
function capsule_update_organization($organization = array(), $id) {
  return _capsule_request("/parties/$id", 'PUT', $organization);
}

/**
 * Deletes the selected person or organisation from Capsule. Operation cannot be performed on the Person record for a 
 * user.
 * 
 * @param int $party_id
 */
function capsule_delete_party($party_id) {
  _capsule_request("/parties/$party_id", 'DELETE');
}

/**
 * Returns a specific customfield for a party.
 * 
 * @param int $party_id
 * @param int $field_id
 */
function capsule_get_party_customfield($party_id, $field_id) {
  return _capsule_request("/parties/$party_id/customfield/$field_id");
}

/**
 * List custom fields for a person or organisation. Note that boolean custom fields that have been set to false will 
 * not be returned.
 * 
 * @param int $party_id
 */
function capsule_list_party_customfields($party_id) {
  return _capsule_request("/parties/$party_id/customfield");
}

/**
 * List of available custom field configurations for people & organisation.
 */
function capsule_list_party_customfield_definitions() {
  return _capsule_request("/parties/customfield/definitions");
}

/**
 * When adding a custom field set the text of the label element to be the name of the custom field and optionally if 
 * you are adding a custom field for a data tag also set the text of the tag element to be the custom field name. 
 * 
 * Populate the text of the appropriate value element <text /> for text and list custom fields, <date /> for date 
 * custom fields and <boolean /> for boolean custom fields. 
 * 
 * Adding a boolean custom field with a false boolean value will have no effect.
 * 
 * @param id $party_id
 * @param array $data
 * Example:
 * $data = array(
 *   'customField' => array(
 *     'tag' => 'Staff',
 *     'label' => 'Start Date',
 *     'date' => '2009-03-13T00:00:00Z',
 *   )
 * );
 */
function capsule_add_party_customfield($party_id, $data = array()) {
  return _capsule_request("/parties/$party_id/customfield", 'POST', $data);
}

/**
 * Updating an existing boolean field to a value of false will delete the 
 * custom field from the contact, it will not be displayed on the next get.
 * 
 * @param int $party_id
 * @param int $field_id
 * @param array $data
 * Example:
 * $data = array(
 *   'customField' => array(
 *     'date' => '2009-03-13T00:00:00Z',
 *   )
 * );
 */
function capsule_update_party_customfield($party_id, $field_id, $data = array()) {
  return _capsule_request("/parties/$party_id/customfield/$field_id", 'PUT', $data);
}

/**
 * Deletes the selected party's customfield from Capsule.
 * 
 * @param int $party_id
 * @param int $field_id
 */
function capsule_delete_party_customfield($party_id, $field_id) {
  return _capsule_request("/parties/$party_id/customfield/$field_id", 'DELETE');
}

/**
 * Gets an existing history record.
 * 
 * @param int $party_id
 * @param int $history_id
 */
function capsule_get_party_history($party_id, $history_id) {
  return _capsule_request("/parties/$party_id/history/$history_id");
}

/**
 * Lists history of notes and emails for party records.
 * 
 * @param int $party_id
 */
function capsule_list_party_histories($party_id) {
  return _capsule_request("/parties/$party_id/history");
}

/**
 * Entry date and creator can be excluded when adding the note. When excluded the entry date will be defaulted to now 
 * and the creator will be defaulted to the authenticated user.
 * 
 * @param int $party_id
 * @param array $data
 */
function capsule_add_party_history($party_id, $data) {
  return _capsule_request("/parties/$party_id/history", 'POST', $data);
}

/**
 * Update an existing history note.
 * 
 * @param int $party_id
 * @param int $history_id
 * @param array $data
 */
function capsule_update_party_history($party_id, $history_id, $data) {
  return _capsule_request("/parties/$party_id/history/$history_id", 'PUT', $data);
}

/**
 * Delete an existing history note.
 * 
 * @param int $party_id
 * @param int $history_id
 */
function capsule_delete_party_history($party_id, $history_id) {
  return _capsule_request("/parties/$party_id/history/$history_id", 'DELETE');
}

/**
 * List all the tags attached to the party specified.
 * 
 * @param int $party_id
 */
function capsule_list_party_tags($party_id) {
  return _capsule_request("/parties/$party_id/tag");
}

/**
 * Create a new tag attached to the party specified.
 * 
 * @param int $party_id
 * @param string $tag
 */
function capsule_add_party_tag($party_id, $tag) {
  return _capsule_request("/parties/$party_id/tag/$tag", 'POST');
}

/**
 * Deletes a tag from the party specified.
 * 
 * @param int $party_id
 * @param string $tag
 */
function capsule_delete_party_tag($party_id, $tag) {
  return _capsule_request("/parties/$party_id/tag/$tag", 'DELETE');
}

/**
 * Create a new opportunity attached to the party specified.
 * 
 * @param int $party_id
 * @param array $data
 */
function capsule_add_party_opportunity($party_id, $data = array()) {
  _capsule_request("/parties/$party_id/opportunity", 'POST', $data);
}

/**
 * Create a new case attached to this person or organisation.
 * 
 * @param int $party_id
 * @param array $data
 */
function capsule_add_party_case($party_id, $data) {
  return _capsule_request("/parties/$party_id/kase", 'POST', $data);
}

/**
 * Update the case.
 * 
 * @param int $party_id
 * @param int $kase_id
 * @param array $data
 */
function capsule_update_party_case($party_id, $kase_id, $data) {
  return _capsule_request("/parties/$party_id/kase/$kase_id", 'PUT', $data);
}

/**
 * List all the cases attached to the party specified.
 * 
 * @param int $party_id
 */
function capsule_list_party_cases($party_id) {
  return _capsule_request("/parties/$party_id/kase");
}

function capsule_update_history($history_id, $data) {
  return _capsule_request("/history/$history_id", 'PUT', $data);
}
