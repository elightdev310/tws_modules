<?php
/**
 * extended hook_menu
 */
function ncn_claims_manager_menu_search(&$items) {
  $items['account/search-claim'] = array(
    'page callback' => 'ncn_claims_manager_search_claim',
    'type' => MENU_CALLBACK,
    'access arguments' => array('access content'),
  );
}

/**
 * Page Handler: account/search-claim
 */
function ncn_claims_manager_search_claim() {
global $user;

  $term = $_REQUEST['term'];

  $data = array();
  $query = "SELECT DISTINCT cd.claim_id FROM {claims_data} AS cd 
            LEFT JOIN {claims} AS c ON c.claim_id = cd.claim_id 
            WHERE c.user_id = {$user->uid} AND 
                  c.claim_status != 'unpurchased' AND 
                  (cd.key='customer_name' OR cd.key='insured_address' OR cd.key='insured_city') AND 
                  cd.val LIKE '%{$term}%' 
            ORDER BY cd.claim_id DESC ";

  $result = db_query($query)->fetchAll();
  foreach ($result as $row) {
    $insured_name = ncn_cd($row->claim_id, 'customer_name');
    $address = strClaimAddress($row->claim_id);
    $data[] = array($row->claim_id, '#'.$row->claim_id ."($insured_name) - $address");
  }
  echo drupal_json_encode($data);
}
