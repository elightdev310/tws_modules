<?php
function capsule_list_users() {
  return _capsule_request("/users", 'GET');
}