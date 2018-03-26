<?php
// $Id: task.inc,v 1.1 2010/12/08 14:31:47 redijedi Exp $

/**
 * @file
 * The Capsule API task functions.
 */


/**
 * Update the task specified.
 * 
 * @param int $task_id
 * @param string $data
 */
function capsule_update_task($task_id, $data = array()) {
  _capsule_request("/task/$task_id", 'POST', $data);
}

/**
 * Complete the task specified.
 * 
 * @param int $task_id
 */
function capsule_complete_task($task_id) {
  _capsule_request("/task/$task_id/complete", 'POST');
}

/**
 * Reopen the task specified.
 * 
 * @param int $task_id
 */
function capsule_reopen_task($task_id) {
  _capsule_request("/task/$task_id/reopen", 'POST');
}
