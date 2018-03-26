<?php
function ncn_report_get_payment_from_pid($pid)
{
    $result = db_query('SELECT * FROM {payment_log} WHERE pid=:d',array(':d'=>$pid));
    foreach($result as $payment)
    {
        if($payment)
        {
            $payment = (array)$payment;
            return $payment;
        }
    }
    return array();
}

function ncn_report_insert_payment_ncn($ref_pid, $owner=0) {
    $result = db_query('SELECT * FROM {payment_log} WHERE pid=:d AND status=1',array(':d'=>$ref_pid ));

    if (!$result) {
        //watchdog( "payment_ncn", t("Failed to insert payment_ncn (!ref_pid)", array('!ref_pid'=>$ref_pid)) );
        return FALSE;
    }
    $payment = null;
    foreach($result as $row)
    {
        $payment = (array)$row;
        if(!$payment)
        {
            return FALSE;
        }
        if (empty($payment)) { return TRUE; }
        if ($payment['refund'] != 0) {
            return TRUE;
        }
    }

    if (!$payment) {
        return FALSE;
    }
    // filter 

    $type = _ncn_report_get_payment_ncn_type($payment['ptype']);
    if ($type == '') {
        return TRUE;
    }
    
    if ($type == 'SUF-FREE-DIST') {
        $uid = $payment['uid'];
    } else {
        if ($owner != 0) {
            $uid = $owner;
        } else {
            $member = get_member_from_id($payment['member_id']);
            $uid = $member['owner'];
        }
    }
    
    if ($payment['ptype'] == 'MMF') {   // previous month       
        $year = date('Y', $payment['timestamp']);
        $month = date('n', $payment['timestamp']);
        $day = date('j', $payment['timestamp']);
        
        $ts_to = mktime(0, 0, 0, $month, 1, $year);
        $ts_from = strtotime("-1 month", $ts_to);
        $typ = 'SUF-GMS-1ST';
        $result = db_query('SELECT * FROM {payment_log} WHERE ptype=:a AND status=1 AND refund=0 AND (uid=:d OR member_id=:s)',
            array(':a'=>$typ,':d'=>$payment['uid'],':s'=>$payment['member_id']));
        $result = $result->rowCount();
        
        if ($result) {
            return TRUE;
            //$type = 'SUF-GMS-1ST';
        }
    }
    
    $cost = _ncn_report_get_payment_ncn_cost($type);
    
    $result = db_query('INSERT INTO {payment_ncn} (uid, ref_pid, type, cost, timestamp, status) VALUES(:a, :b, :c, :d, :e, 1)',
                        array(':a'=>$uid,':b'=>$ref_pid,':c'=>$type,':d'=>$cost,':e'=>$payment['timestamp']));
    
    if ($payment['ptype'] == 'SUF-GMS-1ST') {   // next month
        $type = 'SUF-GMS-1ST';
        $cost = _ncn_report_get_payment_ncn_cost($type);
        
        $year = date('Y', $payment['timestamp']);
        $month = date('n', $payment['timestamp']);
        $day = date('j', $payment['timestamp']);
        
        $timestamp = mktime(0, 0, 0, $month, 1, $year);
        $timestamp = strtotime("+1 month", $timestamp);
        
        $year = date('Y', $timestamp);
        $month = date('n', $timestamp);
        if ($day > date('t', $timestamp)) {
            $day = date('t', $timestamp);
        }
        $timestamp = mktime(0, 0, 0, $month, $day, $year);
        
        $result = db_query('INSERT INTO {payment_ncn} (uid, ref_pid, type, cost, timestamp, status) VALUES(:a, :b, :c, :d, :e, 1)',
                        array(':a'=>$uid,':b'=>$ref_pid,':c'=>$type,':d'=>$cost,':e'=>$timestamp ));    // ignore FOR the next month membership fee
    }
    
    return TRUE;
}

function _ncn_report_get_payment_ncn_type($ptype) {
    $type = '';
    switch ($ptype) {
    case 'SUF-F': case 'SUF-P': case 'SUF-GMS': case 'SUF-GMAS': case 'SUF-GMAS': case 'SUF-GC': case 'SUF-COC':
        $type = 'SUF';
        break;
    case 'SUF-GMS-1ST': case 'SUF-G-DEMO':
        $type = 'SUF-FREE';
        break;
    case 'SUF-DF': case 'SUF-DP': case 'SUF-GMSD':
        $type = 'SUF-FREE-DIST';
        break;
    case 'MMF':
//  case 'MMF-GL':
//  case 'MMF-COC':
    case 'MMF-GC': 
    case 'MMF-SILVER':
    case 'MMF-PLATINUM':
        $type = 'MMF';
        break;
    case 'MMF-FREE':    // Ignore
        //$type = 'SUF-GMS-1ST';
        break;
    }
    
    return $type;
}

function _ncn_report_get_payment_ncn_cost($type) {
    $cost = 0;
    if ($type == 'SUF' || $type == 'SUF-GMS-1ST') {
        $cost = 50;
    } else if ($type == 'MMF') {
        $cost = 10;
    }
    
    return $cost;
}

function ncn_report_change_owner($member_id, $old_uid, $new_uid) {
    $str = date("Y/m/01", date('U'));
    $from = strtotime($str);
    $to = strtotime("+1 month", $from);
    $from = strtotime("-1 month", $from);
    
    $result = db_query('SELECT * FROM {payment_ncn} WHERE status=1 AND uid=:d AND timestamp>=:a',
        array(':d'=>$old_uid,':a'=>$from));
//  while ($pn = db_fetch_array($result))
    foreach($result as $pn)
    {
        $pn = (array)$pn;
        $payment = ncn_report_get_payment_from_pid($pn['ref_pid']);
        
        $key = '0';
        if ($payment['ptype'] == 'SUF-DF' || $payment['ptype'] == 'SUF-DP' || $payment['ptype'] == 'SUF-GMSD') {    // uid means user id of distributor
            $key = $payment['target_mid'];
        } else {
            $key = $payment['member_id'];
        }
        
        if ($key != '0' && $key==$member_id) {
            db_query('UPDATE {payment_ncn} SET uid=:a WHERE id=:d',array(':a'=>$new_uid,':d'=>$pn['id']));
        }
    }
}

function ncn_report_refund_payment_ncn($ref_pid) {
    $str = date("Y/m/01", date('U'));
    $from = strtotime($str);
    $to = strtotime("+1 month", $from);
    $from = strtotime("-1 month", $from);
    
    db_query('UPDATE {payment_ncn} SET status=0 WHERE ref_pid=:a AND status=1 AND timestamp>=:d',
        array(':a'=>$ref_pid,':d'=>$from));
}

function ncn_report_delete_payment_ncn($ref_pid) {
    $str = date("Y/m/01", date('U'));
    $from = strtotime($str);
    $to = strtotime("+1 month", $from);
    $from = strtotime("-1 month", $from);
    
    db_query('DELETE FROM {payment_ncn} WHERE ref_pid=:a AND status=1 AND timestamp>=:b',
        array(':a'=>$ref_pid,':b'=>$from));
}

function ncn_report_is_first_membership_fee_of_gm1st($uid, $payment) {
    $year = date('Y', $payment['timestamp']);
    $month = date('n', $payment['timestamp']);
    $day = date('j', $payment['timestamp']);
    
    $from_ts = mktime(0, 0, 0, $month, 1, $year);
    $to_ts   = strtotime("+1 month", $from_ts);
    
    $result = db_query('SELECT * FROM {payment_ncn} WHERE uid=:a AND type=:b AND timestamp>=:c AND timestamp<:d AND status=1',
        array(':a'=>$uid,':b'=>'SUF-GMS-1ST',':c'=>$from_ts,':d'=>$to_ts));
    
    if ($result) {
        return TRUE;
    }
    return FALSE;
}


////////////////////////////////////////////////////////
function ncn_report_temp_make_payment_ncn() {
    $result = db_query('SELECT * FROM {payment_log} WHERE status=1');
//  while ( $payment = db_fetch_array($result) )
    foreach($result as $payment)
    {
        $payment = (array)$payment;
        ncn_report_insert_payment_ncn($payment['pid']);
    }
}
/**
 * Get Grouping Data
 */
function ncn_report_get_grouping_data($version='2.0') {
    $data = array();
    
    if ($version == '1.0') {
        // Start Up Fee
        $data['GROUP_SUF'] = array(
            'type'      => 'GROUP', 
            'title'     => t('Start-Up Fees'), 
            'child'     => array(
                'SUF-F'     => array( 'type'=>'ITEM', 'title'=>t('Start-Up Fees (Full)') ),
                'SUF-P'     => array( 'type'=>'ITEM', 'title'=>t('Start-Up Fees (Partial)') ), 
                'SUF-DF'    => array( 'type'=>'ITEM', 'title'=>t('Start-Up Fees (Dist. Full)') ), 
                'SUF-DP'    => array( 'type'=>'ITEM', 'title'=>t('Start-Up Fees (Dist. Partial)') ), 
                'SUF-GMS'   => array( 'type'=>'ITEM', 'title'=>t('Start-Up Fees (Gold Member Special)') ), 
                'SUF-GMSD'  => array( 'type'=>'ITEM', 'title'=>t('Start-Up Fees (Dist. Gold Member Special)') ), 
                'SUF-GMS-1ST'   => array( 'type'=>'ITEM', 'title'=>t('Start-Up Fees (Gold Member 1st Special)') ), 
                'SUF-G-DEMO'    => array( 'type'=>'ITEM', 'title'=>t('Start-Up Fees (Gold Member Demo)') ), 
                'SUF-GMAS'      => array( 'type'=>'ITEM', 'title'=>t('Start-Up Fees (Gold Member Associate Special)') ), 
                'SUF-GC'            => array( 'type'=>'ITEM', 'title'=>t('Start-Up Fees (Gold Coach Member)') ), 
                'SUF-COC'           => array( 'type'=>'ITEM', 'title'=>t('Start-Up Fees (Coach on Call Member)') ), 
                'SUF-GTGCSF'    => array( 'type'=>'ITEM', 'title'=>t('Gold to Gold Coach Setup Fee') ), 
          'SUF-CPR'  => array( 'type'=>'ITEM', 'title'=>t('Cancel/Pause Renewal Fee') ), 
            ), 
        );
        
        // Monthly Membership Fee
        $data['GROUP_MMF'] = array(
            'type'      => 'GROUP', 
            'title'     => t('Monthly Membership Fees'), 
            'child'     => array(
                'MMF'       => array( 'type'=>'ITEM', 'title'=>t('Monthly Membership Fees (Gold)') ),
                'MMF-GL'    => array( 'type'=>'ITEM', 'title'=>t('Monthly Membership Fees (Gold Lite)') ), 
                'MMF-COC'   => array( 'type'=>'ITEM', 'title'=>t('Monthly Membership Fees (Coach on Call)') ), 
                'MMF-GC'    => array( 'type'=>'ITEM', 'title'=>t('Monthly Membership Fees (Gold Coach)') ), 
            ), 
        );
        
        $data['MFF'] = array(
            'type' => 'ITEM',   'title' => t('Monthly Finance Fees'), 
        );
        
        // Water Claim
        $data['GROUP-RC-WATER'] = array(
            'type'      => 'GROUP', 
            'title'     => t('Residential Water Claims'), 
            'child'     => array(
                'RC-G-WATER'        => array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims (Gold)') ),
          'RC-G-WATER-EXPEDITE' => array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims (Gold) - EXPEDITE') ),
                'RC-G-WATER-WS' => array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims With Sketch (Gold)') ), 
          'RC-G-WATER-WS-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims With Sketch (Gold) - EXPEDITE') ), 
                'RC-S-WATER'        => array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims (Silver)') ),
          'RC-S-WATER-EXPEDITE'     => array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims (Silver) - EXPEDITE') ),
                'RC-S-WATER-WS' => array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims With Sketch (Silver)') ), 
          'RC-S-WATER-WS-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims With Sketch (Silver) - EXPEDITE') ), 
                'RC-GL-WATER'   => array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims (Gold Lite)') ),
          'RC-GL-WATER-EXPEDITE'   => array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims (Gold Lite) - EXPEDITE') ),
                'RC-GL-WATER-WS'=> array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims With Sketch (Gold Lite)') ), 
          'RC-GL-WATER-WS-EXPEDITE'=> array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims With Sketch (Gold Lite) - EXPEDITE') ), 
                'RC-COC-WATER'  => array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims (Coach on Call)') ),
          'RC-COC-WATER-EXPEDITE'   => array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims (Coach on Call) - EXPEDITE') ),
                'RC-COC-WATER-WS'=>array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims With Sketch (Coach on Call)') ), 
          'RC-COC-WATER-WS-EXPEDITE'=>array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims With Sketch (Coach on Call) - EXPEDITE') ), 
                'RC-GC-WATER'   => array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims (Gold Coach)') ),
          'RC-GC-WATER-EXPEDITE'   => array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims (Gold Coach) - EXPEDITE') ),
                'RC-GC-WATER-WS'=> array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims With Sketch (Gold Coach)') ), 
          'RC-GC-WATER-WS-EXPEDITE'=> array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims With Sketch (Gold Coach) - EXPEDITE') ), 
            ), 
        );
        
        
        $data['GROUP-CC-WATER'] = array(
            'type'      => 'GROUP', 
            'title'     => t('Commercial Water Claims'), 
            'child'     => array(
                'CC-G-WATER'        => array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims (Gold)') ),
          'CC-G-WATER-EXPEDITE'     => array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims (Gold) - EXPEDITE') ),
                'CC-G-WATER-WS' => array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims With Sketch (Gold)') ), 
          'CC-G-WATER-WS-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims With Sketch (Gold) - EXPEDITE') ), 
                'CC-S-WATER'        => array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims (Silver)') ),
          'CC-S-WATER-EXPEDITE'     => array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims (Silver) - EXPEDITE') ),
                'CC-S-WATER-WS' => array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims With Sketch (Silver)') ), 
          'CC-S-WATER-WS-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims With Sketch (Silver) - EXPEDITE') ), 
                'CC-GL-WATER'   => array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims (Gold Lite)') ),
          'CC-GL-WATER-EXPEDITE'   => array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims (Gold Lite) - EXPEDITE') ),
                'CC-GL-WATER-WS'=> array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims With Sketch (Gold Lite)') ), 
          'CC-GL-WATER-WS-EXPEDITE'=> array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims With Sketch (Gold Lite) - EXPEDITE') ), 
                'CC-COC-WATER'  => array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims (Coach on Call)') ),
          'CC-COC-WATER-EXPEDITE'   => array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims (Coach on Call)-EXPEDITE') ),
                'CC-COC-WATER-WS'=>array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims With Sketch (Coach on Call)') ), 
          'CC-COC-WATER-WS-EXPEDITE'=>array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims With Sketch (Coach on Call) - EXPEDITE') ), 
                'CC-GC-WATER'   => array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims (Gold Coach)') ),
          'CC-GC-WATER-EXPEDITE'   => array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims (Gold Coach) - EXPEDITE') ),
                'CC-GC-WATER-WS'=> array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims With Sketch (Gold Coach)') ), 
          'CC-GC-WATER-WS-EXPEDITE'=> array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims With Sketch (Gold Coach) - EXPEDITE') ), 
            ), 
        );
        
        // Fire Claim
        $data['GROUP-RC-FIRE'] = array(
            'type'      => 'GROUP', 
            'title'     => t('Residential Fire Claims'), 
            'child'     => array(
                'RC-G-FIRE'         => array( 'type'=>'ITEM', 'title'=>t('Residential Fire Claims (Gold)') ),
          'RC-G-FIRE-EXPEDITE'     => array( 'type'=>'ITEM', 'title'=>t('Residential Fire Claims (Gold) - EXPEDITE') ),
                'RC-G-FIRE-WS'  => array( 'type'=>'ITEM', 'title'=>t('Residential Fire Claims With Sketch (Gold)') ), 
          'RC-G-FIRE-WS-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Residential Fire Claims With Sketch (Gold) - EXPEDITE') ), 
                'RC-S-FIRE'         => array( 'type'=>'ITEM', 'title'=>t('Residential Fire Claims (Silver)') ),
          'RC-S-FIRE-EXPEDITE'     => array( 'type'=>'ITEM', 'title'=>t('Residential Fire Claims (Silver) - EXPEDITE') ),
                'RC-S-FIRE-WS'  => array( 'type'=>'ITEM', 'title'=>t('Residential Fire Claims With Sketch (Silver)') ), 
          'RC-S-FIRE-WS-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Residential Fire Claims With Sketch (Silver) - EXPEDITE') ), 
                'RC-GL-FIRE'        => array( 'type'=>'ITEM', 'title'=>t('Residential Fire Claims (Gold Lite)') ),
          'RC-GL-FIRE-EXPEDITE'     => array( 'type'=>'ITEM', 'title'=>t('Residential Fire Claims (Gold Lite) - EXPEDITE') ),
                'RC-GL-FIRE-WS' => array( 'type'=>'ITEM', 'title'=>t('Residential Fire Claims With Sketch (Gold Lite)') ), 
          'RC-GL-FIRE-WS-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Residential Fire Claims With Sketch (Gold Lite) - EXPEDITE') ), 
            ), 
        );
        
        $data['GROUP-CC-FIRE'] = array(
            'type'      => 'GROUP', 
            'title'     => t('Commercial Fire Claims'), 
            'child'     => array(
                'CC-G-FIRE'         => array( 'type'=>'ITEM', 'title'=>t('Commercial Fire Claims (Gold)') ),
          'CC-G-FIRE-EXPEDITE'     => array( 'type'=>'ITEM', 'title'=>t('Commercial Fire Claims (Gold) - EXPEDITE') ),
                'CC-G-FIRE-WS'  => array( 'type'=>'ITEM', 'title'=>t('Commercial Fire Claims With Sketch (Gold)') ), 
          'CC-G-FIRE-WS-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Commercial Fire Claims With Sketch (Gold) - EXPEDITE') ), 
                'CC-S-FIRE'         => array( 'type'=>'ITEM', 'title'=>t('Commercial Fire Claims (Silver)') ),
          'CC-S-FIRE-EXPEDITE'     => array( 'type'=>'ITEM', 'title'=>t('Commercial Fire Claims (Silver) - EXPEDITE') ),
                'CC-S-FIRE-WS'  => array( 'type'=>'ITEM', 'title'=>t('Commercial Fire Claims With Sketch (Silver)') ), 
          'CC-S-FIRE-WS-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Commercial Fire Claims With Sketch (Silver) - EXPEDITE') ), 
                'CC-GL-FIRE'        => array( 'type'=>'ITEM', 'title'=>t('Commercial Fire Claims (Gold Lite)') ),
          'CC-GL-FIRE-EXPEDITE'     => array( 'type'=>'ITEM', 'title'=>t('Commercial Fire Claims (Gold Lite) - EXPEDITE') ),
                'CC-GL-FIRE-WS' => array( 'type'=>'ITEM', 'title'=>t('Commercial Fire Claims With Sketch (Gold Lite)') ), 
          'CC-GL-FIRE-WS-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Commercial Fire Claims With Sketch (Gold Lite) - EXPEDITE') ), 
            ), 
        );
        
        // Mold Claim
        $data['GROUP-RC-MOLD'] = array(
            'type'      => 'GROUP', 
            'title'     => t('Residential Mold Claims'), 
            'child'     => array(
                'RC-G-MOLD'         => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims (Gold)') ),
          'RC-G-MOLD-EXPEDITE'     => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims (Gold) - EXPEDITE') ),
                'RC-G-MOLD-WS'  => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims With Sketch (Gold)') ), 
          'RC-G-MOLD-WS-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims With Sketch (Gold) - EXPEDITE') ), 
                'RC-S-MOLD'         => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims (Silver)') ),
          'RC-S-MOLD-EXPEDITE'     => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims (Silver) - EXPEDITE') ),
                'RC-S-MOLD-WS'  => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims With Sketch (Silver)') ), 
          'RC-S-MOLD-WS-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims With Sketch (Silver) - EXPEDITE') ), 
                'RC-GL-MOLD'        => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims (Gold Lite)') ),
          'RC-GL-MOLD-EXPEDITE'     => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims (Gold Lite) - EXPEDITE') ),
                'RC-GL-MOLD-WS' => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims With Sketch (Gold Lite)') ), 
          'RC-GL-MOLD-WS-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims With Sketch (Gold Lite) - EXPEDITE') ), 
                'RC-COC-MOLD'   => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims (Coach on Call)') ),
          'RC-COC-MOLD-EXPEDITE'   => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims (Coach on Call) - EXPEDITE') ),
                'RC-COC-MOLD-WS'=> array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims With Sketch (Coach on Call)') ), 
          'RC-COC-MOLD-WS-EXPEDITE'=> array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims With Sketch (Coach on Call) - EXPEDITE') ), 
                'RC-GC-MOLD'        => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims (Gold Coach)') ),
          'RC-GC-MOLD-EXPEDITE'     => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims (Gold Coach) - EXPEDITE') ),
                'RC-GC-MOLD-WS' => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims With Sketch (Gold Coach)') ), 
          'RC-GC-MOLD-WS-EXPEDITE' => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims With Sketch (Gold Coach) - EXPEDITE') ), 
            ), 
        );
        
        $data['GROUP-CC-MOLD'] = array(
            'type'      => 'GROUP', 
            'title'     => t('Commercial Mold Claims'), 
            'child'     => array(
                'CC-G-MOLD'         => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims (Gold)') ),
          'CC-G-MOLD-EXPEDITE'     => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims (Gold) - EXPEDITE') ),
                'CC-G-MOLD-WS'  => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims With Sketch (Gold)') ), 
          'CC-G-MOLD-WS-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims With Sketch (Gold) - EXPEDITE') ), 
                'CC-S-MOLD'         => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims (Silver)') ),
          'CC-S-MOLD-EXPEDITE'     => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims (Silver) - EXPEDITE') ),
                'CC-S-MOLD-WS'  => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims With Sketch (Silver)') ), 
          'CC-S-MOLD-WS-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims With Sketch (Silver) - EXPEDITE') ), 
                'CC-GL-MOLD'        => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims (Gold Lite)') ),
          'CC-GL-MOLD-EXPEDITE'     => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims (Gold Lite) - EXPEDITE') ),
                'CC-GL-MOLD-WS' => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims With Sketch (Gold Lite)') ), 
          'CC-GL-MOLD-WS-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims With Sketch (Gold Lite) - EXPEDITE') ), 
                'CC-COC-MOLD'   => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims (Coach on Call)') ),
          'CC-COC-MOLD-EXPEDITE'   => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims (Coach on Call) - EXPEDITE') ),
                'CC-COC-MOLD-WS'=> array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims With Sketch (Coach on Call)') ), 
          'CC-COC-MOLD-WS-EXPEDITE'=> array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims With Sketch (Coach on Call) - EXPEDITE') ), 
                'CC-GC-MOLD'        => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims (Gold Coach)') ),
          'CC-GC-MOLD-EXPEDITE'     => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims (Gold Coach) - EXPEDITE') ),
                'CC-GC-MOLD-WS' => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims With Sketch (Gold Coach)') ), 
          'CC-GC-MOLD-WS-EXPEDITE' => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims With Sketch (Gold Coach) - EXPEDITE') ), 
            ), 
        );
        
        // Reconstruction Claim
        $data['GROUP-RC-RECON'] = array(
            'type'      => 'GROUP', 
            'title'     => t('Residential Reconstruction Claims'), 
            'child'     => array(
                'RC-G-RECON'        => array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims (Gold)') ),
          'RC-G-RECON-EXPEDITE'     => array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims (Gold) - EXPEDITE') ),
                'RC-G-RECON-WS' => array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims With Sketch (Gold)') ), 
          'RC-G-RECON-WS-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims With Sketch (Gold) - EXPEDITE') ), 
                'RC-S-RECON'        => array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims (Silver)') ),
          'RC-S-RECON-EXPEDITE'     => array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims (Silver) - EXPEDITE') ),
                'RC-S-RECON-WS' => array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims With Sketch (Silver)') ), 
          'RC-S-RECON-WS-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims With Sketch (Silver) - EXPEDITE') ), 
                'RC-GL-RECON'   => array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims (Gold Lite)') ),
          'RC-GL-RECON-EXPEDITE'   => array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims (Gold Lite) - EXPEDITE') ),
                'RC-GL-RECON-WS'=> array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims With Sketch (Gold Lite)') ), 
          'RC-GL-RECON-WS-EXPEDITE'=> array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims With Sketch (Gold Lite) - EXPEDITE') ), 
                'RC-COC-RECON'  => array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims (Coach on Call)') ),
          'RC-COC-RECON-EXPEDITE'   => array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims (Coach on Call) - EXPEDITE') ),
                'RC-COC-RECON-WS'=>array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims With Sketch (Coach on Call)') ), 
          'RC-COC-RECON-WS-EXPEDITE'=>array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims With Sketch (Coach on Call) - EXPEDITE') ), 
                'RC-GC-RECON'   => array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims (Gold Coach)') ),
          'RC-GC-RECON-EXPEDITE'   => array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims (Gold Coach) - EXPEDITE') ),
                'RC-GC-RECON-WS'=> array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims With Sketch (Gold Coach)') ), 
          'RC-GC-RECON-WS-EXPEDITE'=> array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims With Sketch (Gold Coach) - EXPEDITE') ), 
            ), 
        );
        
        $data['GROUP-CC-RECON'] = array(
            'type'      => 'GROUP', 
            'title'     => t('Commercial Reconstruction Claims'), 
            'child'     => array(
                'CC-G-RECON'        => array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims (Gold)') ),
          'CC-G-RECON-EXPEDITE'     => array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims (Gold) - EXPEDITE') ),
                'CC-G-RECON-WS' => array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims With Sketch (Gold)') ), 
          'CC-G-RECON-WS-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims With Sketch (Gold) - EXPEDITE') ), 
                'CC-S-RECON'        => array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims (Silver)') ),
          'CC-S-RECON-EXPEDITE'     => array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims (Silver) - EXPEDITE') ),
                'CC-S-RECON-WS' => array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims With Sketch (Silver)') ), 
          'CC-S-RECON-WS-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims With Sketch (Silver) - EXPEDITE') ), 
                'CC-GL-RECON'   => array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims (Gold Lite)') ),
          'CC-GL-RECON-EXPEDITE'   => array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims (Gold Lite) - EXPEDITE') ),
                'CC-GL-RECON-WS'=> array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims With Sketch (Gold Lite)') ), 
          'CC-GL-RECON-WS-EXPEDITE'=> array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims With Sketch (Gold Lite) - EXPEDITE') ), 
                'CC-COC-RECON'  => array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims (Coach on Call)') ),
          'CC-COC-RECON-EXPEDITE'   => array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims (Coach on Call) - EXPEDITE') ),
                'CC-COC-RECON-WS'=>array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims With Sketch (Coach on Call)') ), 
          'CC-COC-RECON-WS-EXPEDITE'=>array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims With Sketch (Coach on Call) - EXPEDITE') ), 
                'CC-GC-RECON'   => array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims (Gold Coach)') ),
          'CC-GC-RECON-EXPEDITE'   => array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims (Gold Coach) - EXPEDITE') ),
                'CC-GC-RECON-WS'=> array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims With Sketch (Gold Coach)') ), 
          'CC-GC-RECON-WS-EXPEDITE'=> array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims With Sketch (Gold Coach) - EXPEDITE') ), 
            ), 
        );
        
        // Fire Claim (2nd)
        $data['GROUP-RC-FIRE-SMOKE'] = array(
            'type'      => 'GROUP', 
            'title'     => t('Residential Fire/Smoke Claims'), 
            'child'     => array(
                'RC-G-FIRE-SCC' => array(   'type'=>'ITEM', 'title'=>t('Residential Contents Cleaning Claims (Gold)') ),
          'RC-G-FIRE-SCC-EXPEDITE'  => array(   'type'=>'ITEM', 'title'=>t('Residential Contents Cleaning Claims (Gold) - EXPEDITE') ),
                'RC-G-FIRE-SSC' => array(   'type'=>'ITEM', 'title'=>t('Residential Structure Cleaning Claims (Gold)') ), 
          'RC-G-FIRE-SSC-EXPEDITE'  => array(   'type'=>'ITEM', 'title'=>t('Residential Structure Cleaning Claims (Gold) - EXPEDITE') ), 
                'RC-S-FIRE-SCC' => array(   'type'=>'ITEM', 'title'=>t('Residential Contents Cleaning Claims (Silver)') ),
          'RC-S-FIRE-SCC-EXPEDITE'  => array(   'type'=>'ITEM', 'title'=>t('Residential Contents Cleaning Claims (Silver) - EXPEDITE') ),
                'RC-S-FIRE-SSC' => array(   'type'=>'ITEM', 'title'=>t('Residential Structure Cleaning Claims (Silver)') ), 
          'RC-S-FIRE-SSC-EXPEDITE'  => array(   'type'=>'ITEM', 'title'=>t('Residential Structure Cleaning Claims (Silver) - EXPEDITE') ), 
                'RC-GL-FIRE-SCC'    => array( 'type'=>'ITEM', 'title'=>t('Residential Contents Cleaning Claims (Gold Lite)') ),
          'RC-GL-FIRE-SCC-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Residential Contents Cleaning Claims (Gold Lite) - EXPEDITE') ),
                'RC-GL-FIRE-SSC'    => array( 'type'=>'ITEM', 'title'=>t('Residential Structure Cleaning Claims (Gold Lite)') ), 
          'RC-GL-FIRE-SSC-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Residential Structure Cleaning Claims (Gold Lite) - EXPEDITE') ), 
                'RC-COC-FIRE-SCC'   => array( 'type'=>'ITEM', 'title'=>t('Residential Contents Cleaning Claims (Coach on Call)') ),
          'RC-COC-FIRE-SCC-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Residential Contents Cleaning Claims (Coach on Call) - EXPEDITE') ),
                'RC-COC-FIRE-SSC'   => array( 'type'=>'ITEM', 'title'=>t('Residential Structure Cleaning Claims (Coach on Call)') ), 
          'RC-COC-FIRE-SSC-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Residential Structure Cleaning Claims (Coach on Call) - EXPEDITE') ), 
                'RC-GC-FIRE-SCC'    => array( 'type'=>'ITEM', 'title'=>t('Residential Contents Cleaning Claims (Gold Coach)') ),
          'RC-GC-FIRE-SCC-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Residential Contents Cleaning Claims (Gold Coach) - EXPEDITE') ),
                'RC-GC-FIRE-SSC'    => array( 'type'=>'ITEM', 'title'=>t('Residential Structure Cleaning Claims (Gold Coach)') ), 
          'RC-GC-FIRE-SSC-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Residential Structure Cleaning Claims (Gold Coach) - EXPEDITE') ), 
            ), 
        );

        $data['GROUP-CC-FIRE-SMOKE'] = array(
            'type'      => 'GROUP', 
            'title'     => t('Commercial Fire/Smoke Claims'), 
            'child'     => array(
                'CC-G-FIRE-SCC' => array(   'type'=>'ITEM', 'title'=>t('Commercial Contents Cleaning Claims (Gold)') ),
          'CC-G-FIRE-SCC-EXPEDITE'  => array(   'type'=>'ITEM', 'title'=>t('Commercial Contents Cleaning Claims (Gold) - EXPEDITE') ),
                'CC-G-FIRE-SSC' => array(   'type'=>'ITEM', 'title'=>t('Commercial Structure Cleaning Claims (Gold)') ), 
          'CC-G-FIRE-SSC-EXPEDITE'  => array(   'type'=>'ITEM', 'title'=>t('Commercial Structure Cleaning Claims (Gold) - EXPEDITE') ), 
                'CC-S-FIRE-SCC' => array(   'type'=>'ITEM', 'title'=>t('Commercial Contents Cleaning Claims (Silver)') ),
          'CC-S-FIRE-SCC-EXPEDITE'  => array(   'type'=>'ITEM', 'title'=>t('Commercial Contents Cleaning Claims (Silver) - EXPEDITE') ),
                'CC-S-FIRE-SSC' => array(   'type'=>'ITEM', 'title'=>t('Commercial Structure Cleaning Claims (Silver)') ), 
          'CC-S-FIRE-SSC-EXPEDITE'  => array(   'type'=>'ITEM', 'title'=>t('Commercial Structure Cleaning Claims (Silver) - EXPEDITE') ), 
                'CC-GL-FIRE-SCC'    => array( 'type'=>'ITEM', 'title'=>t('Commercial Contents Cleaning Claims (Gold Lite)') ),
          'CC-GL-FIRE-SCC-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Commercial Contents Cleaning Claims (Gold Lite) - EXPEDITE') ),
                'CC-GL-FIRE-SSC'    => array( 'type'=>'ITEM', 'title'=>t('Commercial Structure Cleaning Claims (Gold Lite)') ), 
          'CC-GL-FIRE-SSC-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Commercial Structure Cleaning Claims (Gold Lite) - EXPEDITE') ), 
                'CC-COC-FIRE-SCC'   => array( 'type'=>'ITEM', 'title'=>t('Commercial Contents Cleaning Claims (Coach on Call)') ),
          'CC-COC-FIRE-SCC-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Commercial Contents Cleaning Claims (Coach on Call) - EXPEDITE') ),
                'CC-COC-FIRE-SSC'   => array( 'type'=>'ITEM', 'title'=>t('Commercial Structure Cleaning Claims (Coach on Call)') ), 
          'CC-COC-FIRE-SSC-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Commercial Structure Cleaning Claims (Coach on Call) - EXPEDITE') ), 
                'CC-GC-FIRE-SCC'    => array( 'type'=>'ITEM', 'title'=>t('Commercial Contents Cleaning Claims (Gold Coach)') ),
          'CC-GC-FIRE-SCC-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Commercial Contents Cleaning Claims (Gold Coach) - EXPEDITE') ),
                'CC-GC-FIRE-SSC'    => array( 'type'=>'ITEM', 'title'=>t('Commercial Structure Cleaning Claims (Gold Coach)') ), 
          'CC-GC-FIRE-SSC-EXPEDITE'  => array( 'type'=>'ITEM', 'title'=>t('Commercial Structure Cleaning Claims (Gold Coach) - EXPEDITE') ), 
            ), 
        );
    } else { // Version 2.0
        // Start Up Fee
        $data['GROUP_SUF'] = array(
            'type'      => 'GROUP', 
            'title'     => t('Start-Up Fees'), 
            'child'     => array(
                'SUF-F'     => array( 'type'=>'ITEM', 'title'=>t('Start-Up Fees (Full)') ),
                'SUF-P'     => array( 'type'=>'ITEM', 'title'=>t('Start-Up Fees (Partial)') ), 
                'SUF-DF'    => array( 'type'=>'ITEM', 'title'=>t('Start-Up Fees (Dist. Full)') ), 
                'SUF-DP'    => array( 'type'=>'ITEM', 'title'=>t('Start-Up Fees (Dist. Partial)') ), 

                'SUF-CPR'   => array( 'type'=>'ITEM', 'title'=>t('Cancel/Pause Renewal Fee') ), 
            ), 
        );
        
        // Monthly Membership Fee
        $data['GROUP_MMF'] = array(
            'type'      => 'GROUP', 
            'title'     => t('Monthly Membership Fees'), 
            'child'     => array(
                'MMF'           => array( 'type'=>'ITEM', 'title'=>t('Monthly Membership Fees (Gold)') ),
                'MMF-SILVER'    => array( 'type'=>'ITEM', 'title'=>t('Monthly Membership Fees (Silver)') ), 
                'MMF-PLATINUM'  => array( 'type'=>'ITEM', 'title'=>t('Monthly Membership Fees (Platinum)') ), 
            ), 
        );
        
        $data['MFF'] = array(
            'type' => 'ITEM',   'title' => t('Monthly Finance Fees'), 
        );
        
        // Water Claim
        $data['GROUP-RC-WATER'] = array(
            'type'      => 'GROUP', 
            'title'     => t('Residential Water Claims'), 
            'child'     => array(
                'RC-G-WATER'            => array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims (Gold)') ),
                'RC-G-WATER-EXPEDITE'   => array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims (Gold) - EXPEDITE') ),
                'RC-G-WATER-WS'         => array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims With Sketch (Gold)') ), 
                'RC-G-WATER-WS-EXPEDITE'=> array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims With Sketch (Gold) - EXPEDITE') ), 
                'RC-S-WATER'            => array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims (Silver)') ),
                'RC-S-WATER-EXPEDITE'   => array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims (Silver) - EXPEDITE') ),
                'RC-S-WATER-WS'         => array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims With Sketch (Silver)') ), 
                'RC-S-WATER-WS-EXPEDITE'=> array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims With Sketch (Silver) - EXPEDITE') ), 
                'RC-P-WATER'            => array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims (Platinum)') ),
                'RC-P-WATER-EXPEDITE'   => array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims (Platinum) - EXPEDITE') ),
                'RC-P-WATER-WS'         => array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims With Sketch (Platinum)') ), 
                'RC-P-WATER-WS-EXPEDITE'=> array( 'type'=>'ITEM', 'title'=>t('Residential Water Claims With Sketch (Platinum) - EXPEDITE') ), 
            ), 
        );
        
        
        $data['GROUP-CC-WATER'] = array(
            'type'      => 'GROUP', 
            'title'     => t('Commercial Water Claims'), 
            'child'     => array(
                'CC-G-WATER'            => array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims (Gold)') ),
                'CC-G-WATER-EXPEDITE'   => array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims (Gold) - EXPEDITE') ),
                'CC-G-WATER-WS'         => array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims With Sketch (Gold)') ), 
                'CC-G-WATER-WS-EXPEDITE'=> array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims With Sketch (Gold) - EXPEDITE') ), 
                'CC-S-WATER'            => array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims (Silver)') ),
                'CC-S-WATER-EXPEDITE'   => array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims (Silver) - EXPEDITE') ),
                'CC-S-WATER-WS'         => array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims With Sketch (Silver)') ), 
                'CC-S-WATER-WS-EXPEDITE'=> array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims With Sketch (Silver) - EXPEDITE') ), 
                'CC-P-WATER'            => array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims (Platinum)') ),
                'CC-P-WATER-EXPEDITE'   => array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims (Platinum) - EXPEDITE') ),
                'CC-P-WATER-WS'         => array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims With Sketch (Platinum)') ), 
                'CC-P-WATER-WS-EXPEDITE'=> array( 'type'=>'ITEM', 'title'=>t('Commercial Water Claims With Sketch (Platinum) - EXPEDITE') ), 
            ), 
        );
        
        // Fire Claim
        $data['GROUP-RC-FIRE'] = array(
            'type'      => 'GROUP', 
            'title'     => t('Residential Fire Claims'), 
            'child'     => array(
                'RC-G-FIRE'             => array( 'type'=>'ITEM', 'title'=>t('Residential Fire Claims (Gold)') ),
                'RC-G-FIRE-EXPEDITE'    => array( 'type'=>'ITEM', 'title'=>t('Residential Fire Claims (Gold) - EXPEDITE') ),
                'RC-G-FIRE-WS'          => array( 'type'=>'ITEM', 'title'=>t('Residential Fire Claims With Sketch (Gold)') ), 
                'RC-G-FIRE-WS-EXPEDITE' => array( 'type'=>'ITEM', 'title'=>t('Residential Fire Claims With Sketch (Gold) - EXPEDITE') ), 
                'RC-S-FIRE'             => array( 'type'=>'ITEM', 'title'=>t('Residential Fire Claims (Silver)') ),
                'RC-S-FIRE-EXPEDITE'    => array( 'type'=>'ITEM', 'title'=>t('Residential Fire Claims (Silver) - EXPEDITE') ),
                'RC-S-FIRE-WS'          => array( 'type'=>'ITEM', 'title'=>t('Residential Fire Claims With Sketch (Silver)') ), 
                'RC-S-FIRE-WS-EXPEDITE' => array( 'type'=>'ITEM', 'title'=>t('Residential Fire Claims With Sketch (Silver) - EXPEDITE') ),
                'RC-P-FIRE'             => array( 'type'=>'ITEM', 'title'=>t('Residential Fire Claims (Platinum)') ),
                'RC-P-FIRE-EXPEDITE'    => array( 'type'=>'ITEM', 'title'=>t('Residential Fire Claims (Platinum) - EXPEDITE') ),
                'RC-P-FIRE-WS'          => array( 'type'=>'ITEM', 'title'=>t('Residential Fire Claims With Sketch (Platinum)') ), 
                'RC-P-FIRE-WS-EXPEDITE' => array( 'type'=>'ITEM', 'title'=>t('Residential Fire Claims With Sketch (Platinum) - EXPEDITE') ),
            ), 
        );
        
        $data['GROUP-CC-FIRE'] = array(
            'type'      => 'GROUP', 
            'title'     => t('Commercial Fire Claims'), 
            'child'     => array(
                'CC-G-FIRE'             => array( 'type'=>'ITEM', 'title'=>t('Commercial Fire Claims (Gold)') ),
                'CC-G-FIRE-EXPEDITE'    => array( 'type'=>'ITEM', 'title'=>t('Commercial Fire Claims (Gold) - EXPEDITE') ),
                'CC-G-FIRE-WS'          => array( 'type'=>'ITEM', 'title'=>t('Commercial Fire Claims With Sketch (Gold)') ), 
                'CC-G-FIRE-WS-EXPEDITE' => array( 'type'=>'ITEM', 'title'=>t('Commercial Fire Claims With Sketch (Gold) - EXPEDITE') ), 
                'CC-S-FIRE'             => array( 'type'=>'ITEM', 'title'=>t('Commercial Fire Claims (Silver)') ),
                'CC-S-FIRE-EXPEDITE'    => array( 'type'=>'ITEM', 'title'=>t('Commercial Fire Claims (Silver) - EXPEDITE') ),
                'CC-S-FIRE-WS'          => array( 'type'=>'ITEM', 'title'=>t('Commercial Fire Claims With Sketch (Silver)') ), 
                'CC-S-FIRE-WS-EXPEDITE' => array( 'type'=>'ITEM', 'title'=>t('Commercial Fire Claims With Sketch (Silver) - EXPEDITE') ), 
                'CC-P-FIRE'             => array( 'type'=>'ITEM', 'title'=>t('Commercial Fire Claims (Platinum)') ),
                'CC-P-FIRE-EXPEDITE'    => array( 'type'=>'ITEM', 'title'=>t('Commercial Fire Claims (Platinum) - EXPEDITE') ),
                'CC-P-FIRE-WS'          => array( 'type'=>'ITEM', 'title'=>t('Commercial Fire Claims With Sketch (Platinum)') ), 
                'CC-P-FIRE-WS-EXPEDITE' => array( 'type'=>'ITEM', 'title'=>t('Commercial Fire Claims With Sketch (Platinum) - EXPEDITE') ), 
            ), 
        );
        
        // Mold Claim
        $data['GROUP-RC-MOLD'] = array(
            'type'      => 'GROUP', 
            'title'     => t('Residential Mold Claims'), 
            'child'     => array(
                'RC-G-MOLD'             => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims (Gold)') ),
                'RC-G-MOLD-EXPEDITE'    => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims (Gold) - EXPEDITE') ),
                'RC-G-MOLD-WS'          => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims With Sketch (Gold)') ), 
                'RC-G-MOLD-WS-EXPEDITE' => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims With Sketch (Gold) - EXPEDITE') ), 
                'RC-S-MOLD'             => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims (Silver)') ),
                'RC-S-MOLD-EXPEDITE'    => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims (Silver) - EXPEDITE') ),
                'RC-S-MOLD-WS'          => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims With Sketch (Silver)') ), 
                'RC-S-MOLD-WS-EXPEDITE' => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims With Sketch (Silver) - EXPEDITE') ), 
                'RC-P-MOLD'             => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims (Platinum)') ),
                'RC-P-MOLD-EXPEDITE'    => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims (Platinum) - EXPEDITE') ),
                'RC-P-MOLD-WS'          => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims With Sketch (Platinum)') ), 
                'RC-P-MOLD-WS-EXPEDITE' => array( 'type'=>'ITEM', 'title'=>t('Residential Mold Claims With Sketch (Platinum) - EXPEDITE') ), 
                
            ), 
        );
        
        $data['GROUP-CC-MOLD'] = array(
            'type'      => 'GROUP', 
            'title'     => t('Commercial Mold Claims'), 
            'child'     => array(
                'CC-G-MOLD'             => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims (Gold)') ),
                'CC-G-MOLD-EXPEDITE'    => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims (Gold) - EXPEDITE') ),
                'CC-G-MOLD-WS'          => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims With Sketch (Gold)') ), 
                'CC-G-MOLD-WS-EXPEDITE' => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims With Sketch (Gold) - EXPEDITE') ), 
                'CC-S-MOLD'             => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims (Silver)') ),
                'CC-S-MOLD-EXPEDITE'    => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims (Silver) - EXPEDITE') ),
                'CC-S-MOLD-WS'          => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims With Sketch (Silver)') ), 
                'CC-S-MOLD-WS-EXPEDITE' => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims With Sketch (Silver) - EXPEDITE') ), 
                'CC-P-MOLD'             => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims (Platinum)') ),
                'CC-P-MOLD-EXPEDITE'    => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims (Platinum) - EXPEDITE') ),
                'CC-P-MOLD-WS'          => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims With Sketch (Platinum)') ), 
                'CC-P-MOLD-WS-EXPEDITE' => array( 'type'=>'ITEM', 'title'=>t('Commercial Mold Claims With Sketch (Platinum) - EXPEDITE') ), 
                
            ), 
        );
        
        // Reconstruction Claim
        $data['GROUP-RC-RECON'] = array(
            'type'      => 'GROUP', 
            'title'     => t('Residential Reconstruction Claims'), 
            'child'     => array(
                'RC-G-RECON'            => array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims (Gold)') ),
                'RC-G-RECON-EXPEDITE'   => array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims (Gold) - EXPEDITE') ),
                'RC-G-RECON-WS'         => array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims With Sketch (Gold)') ), 
                'RC-G-RECON-WS-EXPEDITE'=> array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims With Sketch (Gold) - EXPEDITE') ), 
                'RC-S-RECON'            => array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims (Silver)') ),
                'RC-S-RECON-EXPEDITE'   => array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims (Silver) - EXPEDITE') ),
                'RC-S-RECON-WS'         => array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims With Sketch (Silver)') ), 
                'RC-S-RECON-WS-EXPEDITE'=> array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims With Sketch (Silver) - EXPEDITE') ), 
                'RC-P-RECON'            => array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims (Platinum)') ),
                'RC-P-RECON-EXPEDITE'   => array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims (Platinum) - EXPEDITE') ),
                'RC-P-RECON-WS'         => array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims With Sketch (Platinum)') ), 
                'RC-P-RECON-WS-EXPEDITE'=> array( 'type'=>'ITEM', 'title'=>t('Residential Reconstruction Claims With Sketch (Platinum) - EXPEDITE') ), 
            ), 
        );
        
        $data['GROUP-CC-RECON'] = array(
            'type'      => 'GROUP', 
            'title'     => t('Commercial Reconstruction Claims'), 
            'child'     => array(
                'CC-G-RECON'            => array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims (Gold)') ),
                'CC-G-RECON-EXPEDITE'   => array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims (Gold) - EXPEDITE') ),
                'CC-G-RECON-WS'         => array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims With Sketch (Gold)') ), 
                'CC-G-RECON-WS-EXPEDITE'=> array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims With Sketch (Gold) - EXPEDITE') ), 
                'CC-S-RECON'            => array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims (Silver)') ),
                'CC-S-RECON-EXPEDITE'   => array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims (Silver) - EXPEDITE') ),
                'CC-S-RECON-WS'         => array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims With Sketch (Silver)') ), 
                'CC-S-RECON-WS-EXPEDITE'=> array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims With Sketch (Silver) - EXPEDITE') ), 
                'CC-P-RECON'            => array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims (Platinum)') ),
                'CC-P-RECON-EXPEDITE'   => array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims (Platinum) - EXPEDITE') ),
                'CC-P-RECON-WS'         => array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims With Sketch (Platinum)') ), 
                'CC-P-RECON-WS-EXPEDITE'=> array( 'type'=>'ITEM', 'title'=>t('Commercial Reconstruction Claims With Sketch (Platinum) - EXPEDITE') ), 
                
            ), 
        );
        
        // Fire Claim (2nd)
        $data['GROUP-RC-FIRE-SMOKE'] = array(
            'type'      => 'GROUP', 
            'title'     => t('Residential Fire/Smoke Claims'), 
            'child'     => array(
                'RC-G-FIRE-SCC'         => array(   'type'=>'ITEM', 'title'=>t('Residential Contents Cleaning Claims (Gold)') ),
                'RC-G-FIRE-SCC-EXPEDITE'=> array(   'type'=>'ITEM', 'title'=>t('Residential Contents Cleaning Claims (Gold) - EXPEDITE') ),
                'RC-G-FIRE-SSC'         => array(   'type'=>'ITEM', 'title'=>t('Residential Structure Cleaning Claims (Gold)') ), 
                'RC-G-FIRE-SSC-EXPEDITE'=> array(   'type'=>'ITEM', 'title'=>t('Residential Structure Cleaning Claims (Gold) - EXPEDITE') ), 
                'RC-S-FIRE-SCC'         => array(   'type'=>'ITEM', 'title'=>t('Residential Contents Cleaning Claims (Silver)') ),
                'RC-S-FIRE-SCC-EXPEDITE'=> array(   'type'=>'ITEM', 'title'=>t('Residential Contents Cleaning Claims (Silver) - EXPEDITE') ),
                'RC-S-FIRE-SSC'         => array(   'type'=>'ITEM', 'title'=>t('Residential Structure Cleaning Claims (Silver)') ), 
                'RC-S-FIRE-SSC-EXPEDITE'=> array(   'type'=>'ITEM', 'title'=>t('Residential Structure Cleaning Claims (Silver) - EXPEDITE') ), 
                'RC-P-FIRE-SCC'         => array(   'type'=>'ITEM', 'title'=>t('Residential Contents Cleaning Claims (Platinum)') ),
                'RC-P-FIRE-SCC-EXPEDITE'=> array(   'type'=>'ITEM', 'title'=>t('Residential Contents Cleaning Claims (Platinum) - EXPEDITE') ),
                'RC-P-FIRE-SSC'         => array(   'type'=>'ITEM', 'title'=>t('Residential Structure Cleaning Claims (Platinum)') ), 
                'RC-P-FIRE-SSC-EXPEDITE'=> array(   'type'=>'ITEM', 'title'=>t('Residential Structure Cleaning Claims (Platinum) - EXPEDITE') ), 

            ), 
        );

        $data['GROUP-CC-FIRE-SMOKE'] = array(
            'type'      => 'GROUP', 
            'title'     => t('Commercial Fire/Smoke Claims'), 
            'child'     => array(
                'CC-G-FIRE-SCC'         => array(   'type'=>'ITEM', 'title'=>t('Commercial Contents Cleaning Claims (Gold)') ),
                'CC-G-FIRE-SCC-EXPEDITE'=> array(   'type'=>'ITEM', 'title'=>t('Commercial Contents Cleaning Claims (Gold) - EXPEDITE') ),
                'CC-G-FIRE-SSC'         => array(   'type'=>'ITEM', 'title'=>t('Commercial Structure Cleaning Claims (Gold)') ), 
                'CC-G-FIRE-SSC-EXPEDITE'=> array(   'type'=>'ITEM', 'title'=>t('Commercial Structure Cleaning Claims (Gold) - EXPEDITE') ), 
                'CC-S-FIRE-SCC'         => array(   'type'=>'ITEM', 'title'=>t('Commercial Contents Cleaning Claims (Silver)') ),
                'CC-S-FIRE-SCC-EXPEDITE'=> array(   'type'=>'ITEM', 'title'=>t('Commercial Contents Cleaning Claims (Silver) - EXPEDITE') ),
                'CC-S-FIRE-SSC'         => array(   'type'=>'ITEM', 'title'=>t('Commercial Structure Cleaning Claims (Silver)') ), 
                'CC-S-FIRE-SSC-EXPEDITE'=> array(   'type'=>'ITEM', 'title'=>t('Commercial Structure Cleaning Claims (Silver) - EXPEDITE') ), 
                'CC-P-FIRE-SCC'         => array(   'type'=>'ITEM', 'title'=>t('Commercial Contents Cleaning Claims (Platinum)') ),
                'CC-P-FIRE-SCC-EXPEDITE'=> array(   'type'=>'ITEM', 'title'=>t('Commercial Contents Cleaning Claims (Platinum) - EXPEDITE') ),
                'CC-P-FIRE-SSC'         => array(   'type'=>'ITEM', 'title'=>t('Commercial Structure Cleaning Claims (Platinum)') ), 
                'CC-P-FIRE-SSC-EXPEDITE'=> array(   'type'=>'ITEM', 'title'=>t('Commercial Structure Cleaning Claims (Platinum) - EXPEDITE') ), 
            ), 
        );
    }
    
    $data['COF'] = array(
        'type' => 'ITEM',   'title' => t('Change Order Fee'), 
    );
    
    $data['ADDITIONAL'] = array(
        'type' => 'ITEM',   'title' => t('Additional'), 
    );
  
    $data['SUF-AR-SERVICES'] = array(
    'type' => 'ITEM',   'title' => t('AR Services Start-Up Fee'), 
    );
    $data['AR-SERVICES'] = array(
    'type' => 'ITEM',   'title' => t('AR Services'), 
    );
    $data['PEER-REVIEWS'] = array(
    'type' => 'ITEM',   'title' => t('Peer Reviews'), 
    );
  
    $data['REFUND'] = array(
        'type' => 'ITEM',   'title' => t('Refund'), 
    );

    if ($version == '1.0') {
        $data['CSI'] = array(
            'type' => 'ITEM',   'title' => t('CSI Member'), 
        );
    }
    
    return $data;
}


function build_table_grouping($data, $total, $version) {
    $group_data = ncn_report_get_grouping_data($version);

    $output = '
    <table>
    <thead>
        <th>Product</th>
        <th>Total</th>
        <th>Total Revenue</th>
    </thead>
    <tbody>';
    
    $index = 0;
    foreach ($group_data as $key=>$g_item) {
        $tr_class = ($index%2)? 'even':'odd';
        if ($g_item['type'] == 'GROUP') {
            $output.= ncn_report_render_group_item($key, $g_item, $tr_class, $data);
        } else {
            //if($g_item['title']=='CSI Member') {echo "<pre>--";print_r(($data[$key][1]));exit;}
            $output.= '
                <tr class="'.$tr_class.' ">
                    <td><span class="report-group-item">'.$g_item['title'].'</span></td>
                    <td>'.intval($data[$key][0]).'</td>
                    <td>'.render_payment_cost($data[$key][1]).'</td>
                </tr>
            ';              
        }
        $index++;
    }
    
    $output .= '
        <tr class="even">
            <td></td>
            <td colspan="2"><div style="margin:10px 10px 0px 50px">Total:&nbsp;&nbsp; '.render_payment_cost($total[1]).'</div></td>
        </tr>
    </tbody>
    </table>
    ';
    
    return $output;
}

function ncn_report_render_group_item($key, $g_item, $tr_class, $data) {
    $output = '';
    $sub_tr = array('', '', '');
    $g_data = array(0, 0.0);
    foreach ($g_item['child'] as $i_key=>$item) {
        $g_data[0] += $data[$i_key][0];
        $g_data[1] += $data[$i_key][1];
        
        $sub_tr[0] .= "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;".$item['title'].'<br/>';
        $sub_tr[1] .= intval($data[$i_key][0]).'<br/>';
        $sub_tr[2] .= render_payment_cost($data[$i_key][1]).'<br/>';
    }
    
    $output .= '
        <tr class="report-group-tr '.$tr_class.' ar">
            <td><a href="#" id="'.$key.'-item" class="collapse" onclick="toggle_sub_tr(this, \''.$key.'\'); return false;">'.$g_item['title'].'</a></td>
            <td class="ar">'.intval($g_data[0]).'</td>
            <td>'.render_payment_cost($g_data[1]).'</td>
        </tr>
    ';
    $output .= '
        <tr class="report-sub-tr '.$tr_class.' collapse" id="'.$key.'">
            <td>'.$sub_tr[0].'</td>
            <td>'.$sub_tr[1].'</td>
            <td>'.$sub_tr[2].'</td>
        </tr>
    ';
    return $output;
}

function build_table_mail($data, $total, $version) {
    $group_data = ncn_report_get_grouping_data($version);
    $output = '
    <table>
    <thead>
        <th>Product</th>
        <th>Total</th>
        <th>Total Revenue</th>
    </thead>
    <tbody>';
    $index = 0;
    foreach ($group_data as $key=>$g_item) {
        if ($g_item['type'] == 'GROUP') {
            foreach( $g_item['child'] as $i_key=>$item ) {      // Always Item
                if ($data[$i_key][0] != 0) {
                    $tr_class = ($index%2)? 'even':'odd';
                    $output.= '
                        <tr class="'.$tr_class.'">
                            <td>'.$item['title'].'</td>
                            <td>'.$data[$i_key][0].'</td>
                            <td>'.render_payment_cost($data[$i_key][1]).'</td>
                        </tr>
                    ';
                    $index++;
                }
            }
        } else {
            if ($data[$key][0] != 0) {
                $tr_class = ($index%2)? 'even':'odd';
                $output.= '
                    <tr class="'.$tr_class.'">
                        <td>'.$g_item['title'].'</td>
                        <td>'.$data[$key][0].'</td>
                        <td>'.render_payment_cost($data[$key][1]).'</td>
                    </tr>
                ';
                $index++;
            }           
        }
    }
    $tr_class = ($index%2)? 'even':'odd';
    $output .= '
        <tr class="'.$tr_class.'">
            <td></td>
            <td colspan="2"><div style="margin:10px 10px 0px 50px">Total:&nbsp;&nbsp; '.render_payment_cost($total[1]).'</div></td>
        </tr>
    </tbody>
    </table>
    ';
    return $output;
}

function ncn_report_get_ytd($user_id, $member_id=0, $year=0) {
    if ($user_id) {
        $_user = user_load($user_id);
        $member_id = isset($_user->profile_memberid)? $_user->profile_memberid:0;
    }
    
    if ($year == 0) {
        $year = date('Y', date('U'));
    }

    $first_day = strtotime("1/1/$year");
    $next_year = $year+1;
    $next_year_day = strtotime("1/1/$next_year");
    
    $amount = 0;
    if ($member_id) {
        if ($user_id != 0) {
            $amount = db_query('SELECT SUM(cost)
              FROM {payment_log} 
              WHERE status=1 AND (uid=:a OR member_id=:b) AND timestamp>=:c AND timestamp<:d'
            ,array(':a'=>$user_id,':b'=>$member_id,':c'=>$first_day,':d'=>$next_year_day))->fetchField();
        } else {
            $amount = db_query('SELECT SUM(cost)
              FROM {payment_log} WHERE status=1 AND (member_id=:a) AND timestamp>=:b AND timestamp<:c',
            array(':a'=>$member_id,':b'=>$first_day,':c'=>$next_year_day))->fetchField();
        }
    }
    return $amount;
}

function ncn_report_get_total_ytd($user_id, $member_id=0) {
    if ($user_id) {
        $_user = user_load($user_id);
        $member_id = isset($_user->profile_memberid)? $_user->profile_memberid:0;
    }
    
    $amount = 0;
    if ($member_id) {
        if ($user_id != 0) {
            $amount = db_query('SELECT SUM(cost) FROM {payment_log} WHERE status=1 AND (uid=:a OR member_id=:b)',
                array(':a'=>$user_id,':b'=>$member_id))->fetchField();
        } else {
            $amount = db_query('SELECT SUM(cost) FROM {payment_log} WHERE status=1 AND (member_id=:a)',
                array(':a'=>$member_id))->fetchField();
        }
    }
    return $amount;
}

/**
 * Menu Handler (admin/config/ncn_member_renewal_report/daily)
 */
function ncn_member_renewal_report_daily() {
GLOBAL $base_url;

    $data = array();
    $total= array();
    
    $report_date = 0;
    if ( arg(4) == "" ) {
        $report_date = time();
    } else {
        $report_date = arg(4);
    }
    
    
    $next_day = strtotime("+1 day", $report_date);
    $last_day = strtotime("-1 day", $report_date);
    
    $url = $base_url."/admin/config/ncn_member_renewal_report/daily/";
    if (isset($_REQUEST['rsd'])) {
        $report_date = strtotime($_REQUEST['rsd']);
        $timezone = variable_get('date_default_timezone', 0);
        $report_date -= $timezone;
        drupal_goto($url.$report_date);
        exit;
    }
    //$download_url = $base_url."/download/ncn_report/daily?date=$report_date";
    _ncn_member_renewal_report_get_data($data, $total, $report_date, "daily");
    
    ob_start();
?>
<div class="sales-report-header clear-block">
    <div class="report-date">
        <span><a href="<?php echo $url; ?>">[Today]</a></span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <span><a href="<?php echo $url.$last_day; ?>" title="<?php echo date("Y/m/d", $last_day); ?>">[Last]</a></span>&nbsp;&nbsp;
        <span><input type="text" id="datepicker" value="<?php echo date("Y/m/d", $report_date); ?>" size="10" readonly></span>&nbsp;
        <span><a href="<?php echo $url.$next_day; ?>" title="<?php echo date("Y/m/d", $next_day); ?>">[Next]</a></span>&nbsp;&nbsp;
    </div>
    <input type="hidden" id="report_url" value="<?php echo $url; ?>" />
    <?php echo ncn_report_insert_date_script(); ?>
    <div class="download-report">
        <?php 
            echo "Count: ". count($data); 
            if ($total) {
                echo "&nbsp;&nbsp;&nbsp;";
                echo "Total: ". render_payment_cost($total);    
            }
        ?>
    </div>
</div>
<?php
    $page = isset($_REQUEST['page'])?$_REQUEST['page']:0;
    build_renewal_report_table($data, $total, array($page, 50));
    $content = ob_get_contents();
    ob_end_clean();
    return $content;
}

/**
 * Menu Handler (admin/config/ncn_member_renewal_report/weekly)
 */
function ncn_member_renewal_report_weekly() {
GLOBAL $base_url;

    $data = array();
    $total= array();
    
    $report_date = 0;
    if ( arg(4) == "" ) {
        $report_date = time();
    } else {
        $report_date = arg(4);
    }
    
    $next_week = strtotime("+1 week", $report_date);
    $last_week = strtotime("-1 week", $report_date);
    
    $url = $base_url."/admin/config/ncn_member_renewal_report/weekly/";
    if (isset($_REQUEST['rsd'])) {
        $report_date = strtotime($_REQUEST['rsd']);
        $timezone = variable_get('date_default_timezone', 0);
        $report_date -= $timezone;
        drupal_goto($url.$report_date);
        exit;
    }
    
    //$download_url = $base_url."/download/ncn_report/weekly?date=$report_date";
    
    $to = strtotime("next monday", $report_date);
    $from = strtotime("last monday", $to);
    
    _ncn_member_renewal_report_get_data($data, $total, $report_date, "weekly");
    
    ob_start();
?>
<div class="sales-report-header clear-block">
    <div class="report-date">
        <span><a href="<?php echo $url; ?>">[This Week]</a></span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <span><a href="<?php echo $url.$last_week; ?>" title="<?php echo date("Y/m/d", strtotime("-1 week", $from)); ?> to <?php echo date("Y/m/d", strtotime("-1 day", $from)); ?>">[Last]</a></span>&nbsp;&nbsp;
        <span><input type="text" id="datepicker" value="<?php echo date("Y/m/d", $from); ?> ~ <?php echo date("Y/m/d", strtotime("-1 day", $to)); ?>" size="25" readonly></span>&nbsp;
        <span><a href="<?php echo $url.$next_week; ?>" title="<?php echo date("Y/m/d", $to); ?> to <?php echo date("Y/m/d", strtotime("+6 day", $to)); ?>">[Next]</a></span>&nbsp;&nbsp;
    </div>
    <input type="hidden" id="report_url" value="<?php echo $url; ?>" />
    <?php echo ncn_report_insert_date_script(); ?>
    <div class="download-report">
        <?php 
            echo "Count: ". count($data); 
            if ($total) {
                echo "&nbsp;&nbsp;&nbsp;";
                echo "Total: ". render_payment_cost($total);    
            }
        ?>
    </div>
</div>
<?php           
    $page = isset($_REQUEST['page'])?$_REQUEST['page']:0;
    build_renewal_report_table($data, $total, array($page, 50));
    $content = ob_get_contents();
    ob_end_clean();
    return $content;
}

/**
 * Menu Handler (admin/config/ncn_member_renewal_report/monthly)
 */
function ncn_member_renewal_report_monthly() {
GLOBAL $base_url;

    $data = array();
    $total= array();
    
    $report_date = 0;
    if ( arg(4) == "" ) {
        $report_date = time();
    } else {
        $report_date = arg(4);
    }
    
    
    $next_month = strtotime("+1 month", $report_date);
    $last_month = strtotime("-1 month", $report_date);
    
    $url = $base_url."/admin/config/ncn_member_renewal_report/monthly/";
    if (isset($_REQUEST['rsd'])) {
        $report_date = strtotime($_REQUEST['rsd']);
        $timezone = variable_get('date_default_timezone', 0);
        $report_date -= $timezone;
        drupal_goto($url.$report_date);
        exit;
    }
    
    _ncn_member_renewal_report_get_data($data, $total, $report_date, "monthly");
    $best_total = ncn_report_get_best_total_value($total['1'], $report_date, 'monthly');
    
    ob_start();
?>
<div class="sales-report-header clear-block">
    <div class="report-date">
        <span><a href="<?php echo $url; ?>">[This Month]</a></span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <span><a href="<?php echo $url.$last_month; ?>" title="<?php echo date("F Y", $last_month); ?>">[Last]</a></span>&nbsp;&nbsp;
        <span><input type="text" id="datepicker" value="<?php echo date("F Y", $report_date); ?>" size="15" readonly></span>&nbsp;
        <span><a href="<?php echo $url.$next_month; ?>" title="<?php echo date("F Y", $next_month); ?>">[Next]</a></span>&nbsp;&nbsp;
    </div>
    <input type="hidden" id="report_url" value="<?php echo $url; ?>" />
    <?php echo ncn_report_insert_date_script(); ?>
    <div class="download-report">
        <?php 
            echo "Count: ". count($data); 
            if ($total) {
                echo "&nbsp;&nbsp;&nbsp;";
                echo "Total: ". render_payment_cost($total);    
            }
        ?>
    </div>
</div>
<?php 
    $page = isset($_REQUEST['page'])?$_REQUEST['page']:0;
    build_renewal_report_table($data,$total,array($page, 50));
    $content = ob_get_contents();
    ob_end_clean();
    return $content;
}

function _ncn_member_renewal_report_get_data(&$data, &$total, $report_date, $type) {
    //$data = array();
    
    $from = $to = 0;
    if ($type == "daily") {
        $str = date("Y/m/d", $report_date);
        $from = strtotime($str);
        $to = strtotime("+1 day", $from);
    } else if ($type=="weekly") {
        $to = strtotime("next monday", $report_date);
        $from = strtotime("last monday", $to);
    } else if ($type == "monthly") {
        $str = date("Y/m/01", $report_date);
        $from = strtotime($str);
        $to = strtotime("+1 month", $from);
    }
    
    $total = 0;
    
    $all_member_data = array();
    if ($type == 'monthly') {
        $all_member_data = ncn_admin_get_upcoming_membership($report_date);
    }
    
    $members_gbyd = ncn_report_group_member_by_purchase_date($report_date);
    //ncn_print_r($members_gbyd);

    if(!empty($members_gbyd))
    {
        for ($d=$from; $d<$to; $d=strtotime('+1 day', $d)) {
            $total_daily = 0;
            if (date('U') <= $d) {
                _ncn_member_renewal_report_get_data_daily_upcoming($data, $d, $all_member_data);
            } else {
                _ncn_member_renewal_report_get_data_daily_past($data, $total_daily, $d, isset($members_gbyd[date('j', $d)])?$members_gbyd[date('j', $d)]:null) ;
                $total += $total_daily;
            }
        }
    }
}

function _ncn_member_renewal_report_get_data_daily_upcoming(&$data, $report_date, $all_member_data=array()) {

    if(!empty($all_member_data))
    {
        $all_member_data = ncn_admin_get_upcoming_membership($report_date);
        $day = date('j', $report_date);
        $members = isset($all_member_data[$day])?$all_member_data[$day]:null;
        //   var_dump($members);
        if(!empty($members))
        {
            foreach ($members as $_member) {
                $p = array();
                $p['ptype'] = 'UPCOMING';

                $_user = false;
                if ($_member['used']) {
                    $user_id = get_uid_from_memberid($_member['member_id']);
                    if ($user_id) {
                        $_user = user_load($user_id);
                        if ($_user) {
                            $p['name']      = l($_user->profile_firstname.' '.$_user->profile_lastname, 'admin/config/ncn_create_user/all_user_list/detail/'.$_user->uid);
                            $p['company'] = $_user->profile_legalname;
                        }
                    }
                }

                $p['member_id'] = $_member['member_id'];
                if (!$_user) {
                    $p['name']      = $_member['first_name'].' '.$_member['last_name'];
                    $p['company'] = $_member['legalname'];
                }

                $type_arr = get_member_type_array();
                $p['member_type'] = $type_arr[$_member['member_type']];

                if (ncn_admin_is_gold_member_demo($p['member_id'])) {
                    $p['member_type'] .= " (Demo)";
                } else if (ncn_admin_is_member_first_free($p['member_id'])) {
                    $p['member_type'] .= " (First Free)";
                }

                $p['date_purchase'] = $_member['create'];

                $p['amount'] = 0;
                $p['date_processed'] = 0;

                $data[] = $p;
            }
        }

    }

}

function _ncn_member_renewal_report_get_data_daily_past(&$data, &$total, $report_date, $members) {
    $total = 0;
    if (empty($members)) { return; }
    
    foreach ($members as $_member) {
        $p = array();
        
        $transaction = ncn_report_get_member_renewal_transaction($_member['member_id'], $report_date);
        if (!$transaction) { continue; }
        
        $p['ptype'] = $transaction['ptype'];
        
        $_user = false;
        $user_id = get_uid_from_memberid($_member['member_id']);
        if ($user_id) {
            $_user = user_load($user_id);
            if ($_user) {
                $p['name']      = l($_user->profile_firstname.' '.$_user->profile_lastname, 'admin/config/ncn_create_user/all_user_list/detail/'.$_user->uid);
                $p['company'] = $_user->profile_legalname;
            }
        }
        
        $p['member_id'] = $_member['member_id'];
        if (!$_user) {
            $p['name']      = $_member['first_name'].' '.$_member['last_name'];
            $p['company'] = $_member['legalname'];
        }
        
        $type_arr = get_member_type_array();
        $p['member_type'] = $type_arr[$_member['member_type']];
        
        if (ncn_admin_is_gold_member_demo($p['member_id'])) {
            $p['member_type'] .= " (Demo)";
        } else if (ncn_admin_is_member_first_free($p['member_id'])) {
            $p['member_type'] .= " (First Free)";
        }
            
        $p['date_purchase'] = $_member['create'];
        
        $p['amount'] = ($transaction['ptype']=='DECLINED')? 0 : $transaction['cost'];
        $p['date_processed'] = ($transaction['ptype']=='DECLINED')? 0 : $transaction['timestamp'];

        $total += $p['amount'];     
        $data[] = $p;
    }
    
    return;
}

/**
 * Grouping Members by Purchase Date
 * @return: array[0..31]
 */
function ncn_report_group_member_by_purchase_date($timestamp) {
    $data = array();
    $year = date('Y', $timestamp);
    $month= date('n', $timestamp);
    $result = db_query('SELECT * FROM {member_id_pool}')->fetchAll();
//  while ($member = db_fetch_array($result))
    foreach($result as $member)
    {
        $member = (array)$member;
        $day = ncn_admin_get_day_of_membership_given_month($member['member_id'], $month, $year);
        $data[$day][] = $member;
    }
    
    return $data;
}

function ncn_report_get_member_renewal_transaction($member_id, $expected_date) {
    $from = $to = 0;
    $str = date("Y/m/01", $expected_date);
    $from = strtotime($str);
    $to = strtotime("+1 month", $from);
    
//  $result = db_query("SELECT * FROM {payment_log} WHERE status=1 AND member_id='$member_id' AND ptype LIKE 'MMF%' AND timestamp>=$from AND timestamp<$to");

    $result = db_query('SELECT * FROM {payment_log} WHERE status=1 AND member_id=:a AND ptype LIKE :b AND timestamp>=:c AND timestamp<:d',
    array(':a'=>$member_id,':b'=>'MMF%',':c'=>$from,':d'=>$to));

    if($result)
    {
        foreach($result as $transaction)
        {
            $transaction = (array)$transaction;
            return $transaction;
        }
    }




//    if ($result && $transaction=db_fetch_array($result)) {
//      return $transaction;
//  }
    
    // Declined
    $result = db_query('SELECT * FROM {cc_declined} WHERE status=0 AND member_id=:a AND (product_id=32 OR product_id=102 OR product_id=119 OR product_id=137 ) AND timestamp>=:b AND timestamp<:c ORDER BY timestamp',
    array(':a'=>$member_id,':b'=>$from,':c'=>$to));
                // MMF: 32, MMF-GL: 102, MMF-COC: 119, MMF-GC: 137

    if($result)
    {
        foreach($result as $transaction)
        {
            $transaction = (array)$transaction;
            $transaction['ptype'] = 'DECLINED';
            return $transaction;
        }
    }

//    if ($result && $transaction=db_fetch_array($result)) {
//      $transaction['ptype'] = 'DECLINED';
//      return $transaction;
//  }
//
    return FALSE;
}

function ncn_report_cmp_renewal_data($a, $b) {
    if ($a['date_processed'] == $b['date_processed']) {
    return 0;
  }
  return ($a['date_processed'] < $b['date_processed']) ? -1 : 1;
}

function build_renewal_report_table($data, $total, $pager=array(0, 50)) {
    global $base_url;
    $table_head = '
        <thead class="tableHeader-processed">
            <th>Member ID</th>
            <th>Member Name</th>
            <th>Company Name</th>
            <th>Member Type</th>
            <th>Date of Purchase</th>
            <th>Amount</th>
            <th>Date of Processed</th>
        </thead>
        <tbody>';

    $table_foot = '
        </tbody>
        </table>';

    
    if (count($data) <= $pager[0]*$pager[1])
    {
        print "<table>";
        print $table_head;
        print $table_foot;
    } else {
        echo '<table class="sticky-enabled tableSelect-processed sticky-table">';
        echo $table_head;
        
        $from_index = $pager[0]*$pager[1];
        $to_index   = min( ($pager[0]+1)*$pager[1], count($data) );
        for($i=$from_index; $i<$to_index; $i++) {
            $i_data = $data[$i];
            ?>
            <tr class="<?php if ($i%2) print "even"; else print "odd"; ?>">
                <td><?php echo $i_data['member_id']; ?></td>
                <td><?php echo $i_data['name']; ?></td>
                <td><?php echo $i_data['company']; ?></td>
                <td><?php echo $i_data['member_type']; ?></td>
                <td><?php if($i_data['date_purchase']) { echo date('m/d/Y', $i_data['date_purchase']); } ?></td>
                <td>
                    <?php if($i_data['ptype'] != 'UPCOMING') {
                            if ($i_data['ptype'] != 'DECLINED') { echo render_payment_cost($i_data['amount']); } else { echo 'Declined'; } 
                        } ?>
                </td>
                <td>
                    <?php if($i_data['ptype'] != 'UPCOMING') {
                            if ($i_data['ptype'] != 'DECLINED' && $i_data['date_processed'] ) { echo date('m/d/Y H:i', $i_data['date_processed']); } 
                        }   ?>
                </td>
            </tr>
            <?php
        }
        
        echo $table_foot;
        $url = $base_url."/".drupal_get_path_alias($_GET["q"]);
        echo ncn_admin_table_pagniation($url, count($data), $pager[1], $pager[0], 15);
    }
}

?>
