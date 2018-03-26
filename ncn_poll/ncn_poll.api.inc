<?php

/*****************************************************************************/
/*		NCN Poll API
/*****************************************************************************/
function ncn_poll_get_poll($poll_id) {
	$result = db_query('SELECT * FROM {ncn_poll} WHERE id=:d', array(':d'=>$poll_id));
  //  print_r($result);
    if($result->rowCount()>0){
        foreach($result as $quiz)
        {
            if($quiz && $result)
            {
                $quiz = (array)$quiz;
                $result = (array)$result;

                $quiz['choice'] = unserialize($quiz['choice']);
            }
            else{
                $quiz = array();
            }
            return $quiz;
        }

    }

    //    if ($result && $quiz=db_fetch_array($result)) {
    //		$quiz['choice'] = unserialize($quiz['choice']);
    //	} else {
    //		$quiz = array();
    //	}
    //	return $quiz;
}


function ncn_poll_get_max_choice_id($poll_id) {
	$poll = ncn_poll_get_poll($poll_id);
	if (!empty($poll)) {
		return intval($poll['choice']['max_choice_id']);
	}
	return -1;
}

function ncn_poll_get_poll_track($poll_id, $uid) {
	$result = db_query('SELECT * FROM {ncn_poll_track} WHERE poll_id=:a AND uid=:b',array(':a'=>$poll_id,':b'=>$uid));

    foreach($result as $pt)
    {
        $pt = (array)$pt;
            if($pt)
            {
                return $pt;
            }
           return array();
    }

//
//    if ($result && $pt = db_fetch_array($result)) {
//		return $pt;
//	}
//	return array();
}

function ncn_poll_get_choice_index($poll, $choice_id) {
	if (!is_array($poll['choice']['data'])) {
		return 0;
	}
	
	for($i=0; $i<count($poll['choice']['data']); $i++) {
		if ($poll['choice']['data'][$i]['choice_id'] == $choice_id) {
			return ($i+1);
		}
	}
	return 0;
}

function ncn_poll_member_polling($poll_id, $uid, $choice_id, $timestamp, $comment='') {
	$poll = ncn_poll_get_poll($poll_id);
	
	$choice_index = ncn_poll_get_choice_index($poll, $choice_id);
	$choice_text = $poll['choice']['data'][($choice_index-1)]['choice_text'];
	
	$pt = ncn_poll_get_poll_track($poll_id, $uid);
	if (empty($pt)) {
        $result = db_insert('ncn_poll_track')
            ->fields(array(
                'poll_id' => $poll_id,
                'uid' => $uid,
                'choice_id' => $choice_id,
                'choice_text' => $choice_text,
                'comment' => $comment,
                'timestamp' => $timestamp,
            ))
            ->execute();

//
//		$result = db_query("INSERT INTO {ncn_poll_track} (poll_id, uid, choice_id, choice_text, comment, timestamp)
//								VALUES(%d, %d, %d, '%s', '%s', %d)",
//								$poll_id, $uid, $choice_id, $choice_text, $comment, $timestamp);
	} else {
        $result =  db_update('ncn_poll_track')
            ->condition('poll_id', $poll_id)
            ->condition('uid', $uid)
            ->fields(array(
                'choice_id' => $choice_id,
                'choice_text' => $choice_text,
                'comment' => $comment,
                'timestamp' => $timestamp,
            ))
            ->execute();

//		$result = db_query("UPDATE {ncn_poll_track} SET choice_id=%d, choice_text='%s', comment='%s', timestamp=%d WHERE poll_id=%d AND uid=%d",
//								$choice_id, $choice_text, $comment, $timestamp, $poll_id, $uid);
	}
	
	if ($result) {
		return TRUE;
	} else {
		drupal_set_message("Failed to process poll. please report this to webmaster.", 'error');
		return FALSE;
	}
}

function ncn_poll_get_total_answers($poll_id, $choice_id) {
	$result = db_query('SELECT COUNT(*) FROM {ncn_poll_track} WHERE poll_id=:a AND choice_id=:b',
        array(':a'=>$poll_id,':b'=>$choice_id))->fetchField();
	return $result;
}

function ncn_poll_get_upcoming_poll_id($uid) {
	$result = db_query('SELECT * FROM {ncn_poll} WHERE status=1 ORDER BY updated DESC');
//	while($poll=db_fetch_array($result))

    foreach($result as $poll){

		$poll = (array)$poll;
        $pt = ncn_poll_get_poll_track($poll['id'], $uid);
		if ( empty($pt) ) {
			return $poll['id'];
		}
	}
	return 0;
}
?>