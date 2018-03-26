<?php
// $Id: opportunity.inc,v 1.1 2010/12/08 14:31:47 redijedi Exp $

/**
 * @file
 * The Capsule API opportunity functions.
 */


/**
 * Returns an opportunity.
 * 
 * @param int $opp_id
 */
function capsule_get_opportunity($opp_id) {
  return _capsule_request("/opportunity/$opp_id");
}

/**
 * List all the tags attached to the opportunity specified.
 * 
 * @param int $opp_id
 */
function capsule_list_opportunity_tags($opp_id) {
  return _capsule_request("/opportunity/$opp_id/tag");
}

/**
 * Create a new tag attached to the opportunity specified.
 * 
 * @param int $opp_id
 * @param string $tag
 */
function capsule_add_opportunity_tag($opp_id, $tag) {
  return _capsule_request("/opportunity/$opp_id/tag/$tag", 'POST');
}

/**
 * Delete the tag specified.
 * 
 * @param int $opp_id
 * @param string $tag
 */
function capsule_delete_opportunity_tag($opp_id, $tag) {
  return _capsule_request("/opportunity/$opp_id/tag/$tag", 'DELETE');
}

/**
 * Update the opportunity specified.
 * 
 * @param int $opp_id
 * @param array $data
 */
function capsule_update_opportunity($opp_id, $data = array()) {  
  return _capsule_request("/opportunity/$opp_id", 'POST', $data);
}

/**
 * Create a new task attached to the opportunity specified.
 * 
 * @param int $opp_id
 * @param string $data
 */
function capsule_add_opportunity_task($opp_id, $data) {
  return _capsule_request("/opportunity/$opp_id/task", 'POST', $data);
}

/**
 * Delete the opportunity specified.
 * 
 * @param int $opp_id
 */
function capsule_delete_opportunity($opp_id) {
  return _capsule_request("/opportunity/$opp_id", 'DELETE');
}

/**
 * Add the Person or Organisation to the opportunity.
 * 
 * View additional people & organisations related to this opportunity. 
 * Example response:
 * array(
 *   'parties' => array(
 *     'organisation' => array...
 *     'person' => array...
 *   )
 * )
 * 
 * @param int $opp_id
 */
function capsule_list_opportunity_parties($opp_id) {
  return _capsule_request("/opportunity/$opp_id/party");
}

/**
 * Add the person or organisation to the opportunity.
 * 
 * @param int $opp_id
 * @param int $party_id
 */
function capsule_add_opportunity_party($opp_id, $party_id) {
  return _capsule_request("/opportunity/$opp_id/party/$party_id", 'POST');
}

/**
 * Remove the person or organisation from the opportunity.
 * 
 * @param int $opp_id
 * @param int $party_id
 */
function capsule_delete_opportunity_party($opp_id, $party_id) {
  return _capsule_request("/opportunity/$opp_id/party/$party_id", 'DELETE');
}

/**
 * List custom fields for a opportunity. Note that boolean custom fields that have been set to false will not be 
 * returned.
 * 
 * @param int $opp_id
 */
function capsule_list_opportunity_customfields($opp_id) {
  return _capsule_request("/opportunity/$opp_id/customfield");
}

/**
 * List of available custom field configurations for opportunities.
 */
function capsule_list_opportunity_customfield_definitions() {
  return _capsule_request("/opportunity/customfield/definitions");
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
 * @param int $opp_id
 * @param array $data
 */
function capsule_add_opportunity_customfield($opp_id, $data) {
  return _capsule_request("/opportunity/$opp_id/customfield", 'POST', $data);
}

/**
 * Updating an existing boolean field to a value of false will delete the custom field from the contact, it will not 
 * be displayed on the next get.
 * 
 * @param int $opp_id
 * @param int $field_id
 * @param array $data
 */
function capsule_update_opportunity_customfield($opp_id, $field_id, $data) {
  return _capsule_request("/opportunity/$opp_id/customfield/$field_id", 'PUT', $data);
}

/**
 * Deletes an opportunity's custom field.
 * 
 * @param int $opp_id
 * @param int $field_id
 */
function capsule_delete_opportunity_customfield($opp_id, $field_id) {
  return _capsule_request("/opportunity/$opp_id/customfield/$field_id", 'DELETE');
}

/**
 * History of notes and emails for opportunity records.
 * 
 * @param int $opp_id
 */
function capsule_list_opportunity_history($opp_id) {
  return _capsule_request("/opportunity/$opp_id/history");
}

/**
 * Entry date and creator can be excluded when adding the note. When excluded the entry date will be defaulted to now 
 * and the creator will be defaulted to the authenticated user.
 * 
 * @param int $opp_id
 * @param array $data
 */
function capsule_add_opportunity_history($opp_id, $data) {
  return _capsule_request("/opportunity/$opp_id/history", 'POST', $data);
}

/**
 * Update an existing history note.
 * 
 * @param int $opp_id
 * @param int $history_id
 * @param array $data
 */
function capsule_update_opportunity_history($opp_id, $history_id, $data) {
  return _capsule_request("/opportunity/$opp_id/history/$history_id", 'PUT', $data);
}

/**
 * Delete an existing history note.
 * 
 * @param int $opp_id
 * @param int $history_id
 */
function capsule_delete_opportunity_history($opp_id, $history_id) {
  return _capsule_request("/opportunity/$opp_id/history/$history_id", 'DELETE');
}

