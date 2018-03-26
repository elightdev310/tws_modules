<?php
/**
 * Create Entry
{
  "entry" : {
    "party" : {
      "id" : 892
    },
    "type" : "note",
    "content" : "The quick brown fox\njumped over the lazy dog"
  }
}
 * [v2] 
 */
function capsule_add_entry($entry = array()) {
  return _capsule_request("/entries", 'POST', $entry);
}


/**
 * Update Entry
{
  "entry" : {
    "content" : "The quick brown fox\njumped over the lazy dog"
  }
}
 * [v2]
 */
function capsule_update_entry($entry = array(), $id) {
  return _capsule_request("/entries/$id", 'PUT', $entry);
}
