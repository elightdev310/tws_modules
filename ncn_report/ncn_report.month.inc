<?php
/**
* Page Handler (admin/config/ncn_montly_overview_report)
* 
*/
function ncn_report_montly_overview_report() {
global $base_url;

  $url = $base_url."/admin/config/ncn_montly_overview_report/";
  if (isset($_REQUEST['rsd'])) {
    $report_date = strtotime($_REQUEST['rsd']);
    $timezone = variable_get('date_default_timezone', 0);
    $report_date -= $timezone;
    drupal_goto($url.$report_date);
    exit;
  }
  
  if ( arg(3) == "" ) {
    $report_date = time();
  } else {
    $report_date = arg(3);
  }

  $length = 6;
  $data = ncn_report_month_get_multi_month_report_data($report_date, $length);
  
  $version = ncn_get_version_time($report_date);

  ob_start();
?>

<?php if ($version == '1.0'): ?>
<!-- Version 1.0 -->
<div class="sales-report-header clear-block">
  <div class="report-date">
    <span><input type="text" id="datepicker" value="<?php echo date("F Y", $report_date); ?>" size="20" readonly></span>&nbsp;
  </div>
  <input type="hidden" id="report_url" value="<?php echo $url; ?>" />
  <?php echo ncn_report_insert_date_script(); ?>
</div>
<div class="ncn-month-report report-content">
  <table>
    <thead>
      <th>Month</th>
      <?php foreach ($data as $month=>$m_data): ?>
      <th><?php print date("F Y", $month); ?></th>
      <?php endforeach; ?>
    </thead>
    <tbody>
      <tr class="group">
        <td>Start Ups</td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['start_up']['total']; ?></td>
        <?php endforeach; ?>
      </tr>
      <tr>
        <td>Gold</td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['start_up']['gold']; ?></td>
        <?php endforeach; ?>
      </tr>
      <tr>
        <td>Coach on Call</td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['start_up']['coach_on_call']; ?></td>
        <?php endforeach; ?>
      </tr>
      <tr>
        <td>Gold Coach</td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['start_up']['gold_coach']; ?></td>
        <?php endforeach; ?>
      </tr>
      <tr>
        <td>Other</td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['start_up']['other']; ?></td>
        <?php endforeach; ?>
      </tr>
      
      <tr class="group">
        <td>Monthly Renewals</td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['monthly_renewal']['total']; ?></td>
        <?php endforeach; ?>
      </tr>
      <tr>
        <td>Gold</td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['monthly_renewal']['gold']; ?></td>
        <?php endforeach; ?>
      </tr>
      <tr>
        <td>Coach on Call</td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['monthly_renewal']['coach_on_call']; ?></td>
        <?php endforeach; ?>
      </tr>
      <tr>
        <td>Gold Coach</td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['monthly_renewal']['gold_coach']; ?></td>
        <?php endforeach; ?>
      </tr>
      <tr>
        <td>Gold lite</td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['monthly_renewal']['gold_lite']; ?></td>
        <?php endforeach; ?>
      </tr>
      <tr>
        <td>Other</td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['monthly_renewal']['other']; ?></td>
        <?php endforeach; ?>
      </tr>
      
      <tr class="group">
        <td>Declined Monthly Renewal</td>
        <td colspan="<?php print $length; ?>"></td>
      </tr>
      <tr>
        <td></td>
        <?php foreach ($data as $month=>$m_data):
            ?>
         <td><?php print $m_data['declined_monthly_renewal']; // $m_data['declined_monthly_renewal']; ?></td>
        <?php endforeach; ?>
      </tr>
      
      <tr class="group">
        <td>Upgrades</td>
        <td colspan="<?php print $length; ?>"></td>
      </tr>
      <tr>
        <td></td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['upgrades']; ?></td>
        <?php endforeach; ?>
      </tr>
      
      <tr class="group">
        <td>Downgrades</td>
        <td colspan="<?php print $length; ?>"></td>
      </tr>
      <tr>
        <td></td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['downgrades']; ?></td>
        <?php endforeach; ?>
      </tr>
      
      <tr class="group">
        <td>Declined Recovery</td>
        <td colspan="<?php print $length; ?>"></td>
      </tr>
      <tr>
        <td></td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print_r($m_data['declined_recovery']); // print $m_data['declined_recovery']; ?></td>
        <?php endforeach; ?>
      </tr>
      
      <tr class="group">
        <td>Cancel</td>
        <td colspan="<?php print $length; ?>"></td>
      </tr>
      <tr>
        <td></td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['cancel']; ?></td>
        <?php endforeach; ?>
      </tr>
      
      <tr class="group">
        <td>Pause Renewal</td>
        <td colspan="<?php print $length; ?>"></td>
      </tr>
      <tr>
        <td></td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['pause_renewal']; ?></td>
        <?php endforeach; ?>
      </tr>
      
      <tr class="group">
        <td>Refund</td>
        <td colspan="<?php print $length; ?>"></td>
      </tr>
      <tr>
        <td></td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['refund']; ?></td>
        <?php endforeach; ?>
      </tr>
    </tbody>
  </table>
</div>
<?php else: ?>
<!-- Version 2.0 -->
<div class="sales-report-header clear-block">
  <div class="report-date">
    <span><input type="text" id="datepicker" value="<?php echo date("F Y", $report_date); ?>" size="20" readonly></span>&nbsp;
  </div>
  <input type="hidden" id="report_url" value="<?php echo $url; ?>" />
  <?php echo ncn_report_insert_date_script(); ?>
</div>
<div class="ncn-month-report report-content">
  <table>
    <thead>
      <th>Month</th>
      <?php foreach ($data as $month=>$m_data): ?>
      <th><?php print date("F Y", $month); ?></th>
      <?php endforeach; ?>
    </thead>
    <tbody>
      <tr class="group">
        <td>Start Ups</td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['start_up']['total']; ?></td>
        <?php endforeach; ?>
      </tr>
      <tr>
        <td>Gold</td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['start_up']['gold']; ?></td>
        <?php endforeach; ?>
      </tr>
      <tr>
        <td>Silver</td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['start_up']['silver']; ?></td>
        <?php endforeach; ?>
      </tr>
      <tr>
        <td>Platinum</td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['start_up']['platinum']; ?></td>
        <?php endforeach; ?>
      </tr>
      <tr>
        <td>Other</td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['start_up']['other']; ?></td>
        <?php endforeach; ?>
      </tr>
      
      <tr class="group">
        <td>Monthly Renewals</td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['monthly_renewal']['total']; ?></td>
        <?php endforeach; ?>
      </tr>
      <tr>
        <td>Gold</td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['monthly_renewal']['gold']; ?></td>
        <?php endforeach; ?>
      </tr>
      <tr>
        <td>Silver</td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['monthly_renewal']['silver']; ?></td>
        <?php endforeach; ?>
      </tr>
      <tr>
        <td>Platinum</td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['monthly_renewal']['platinum']; ?></td>
        <?php endforeach; ?>
      </tr>
      <tr>
        <td>Other</td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['monthly_renewal']['other']; ?></td>
        <?php endforeach; ?>
      </tr>
      
      <tr class="group">
        <td>Declined Monthly Renewal</td>
        <td colspan="<?php print $length; ?>"></td>
      </tr>
      <tr>
        <td></td>
        <?php foreach ($data as $month=>$m_data):
            ?>
         <td><?php print $m_data['declined_monthly_renewal']; // $m_data['declined_monthly_renewal']; ?></td>
        <?php endforeach; ?>
      </tr>
      
      <tr class="group">
        <td>Upgrades</td>
        <td colspan="<?php print $length; ?>"></td>
      </tr>
      <tr>
        <td></td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['upgrades']; ?></td>
        <?php endforeach; ?>
      </tr>
      
      <tr class="group">
        <td>Downgrades</td>
        <td colspan="<?php print $length; ?>"></td>
      </tr>
      <tr>
        <td></td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['downgrades']; ?></td>
        <?php endforeach; ?>
      </tr>
      
      <tr class="group">
        <td>Declined Recovery</td>
        <td colspan="<?php print $length; ?>"></td>
      </tr>
      <tr>
        <td></td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print_r($m_data['declined_recovery']); // print $m_data['declined_recovery']; ?></td>
        <?php endforeach; ?>
      </tr>
      
      <tr class="group">
        <td>Cancel</td>
        <td colspan="<?php print $length; ?>"></td>
      </tr>
      <tr>
        <td></td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['cancel']; ?></td>
        <?php endforeach; ?>
      </tr>
      
      <tr class="group">
        <td>Pause Renewal</td>
        <td colspan="<?php print $length; ?>"></td>
      </tr>
      <tr>
        <td></td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['pause_renewal']; ?></td>
        <?php endforeach; ?>
      </tr>
      
      <tr class="group">
        <td>Refund</td>
        <td colspan="<?php print $length; ?>"></td>
      </tr>
      <tr>
        <td></td>
        <?php foreach ($data as $month=>$m_data): ?>
        <td><?php print $m_data['refund']; ?></td>
        <?php endforeach; ?>
      </tr>
    </tbody>
  </table>
</div>
<?php endif; ?>
<?php 
  $output = ob_get_contents();
  ob_end_clean();
  return $output;
}

function ncn_report_month_get_multi_month_report_data($t_month, $length) {
  $to_month = strtotime(date("m/1/y", $t_month));
  $from_month = strtotime("-".($length-1)." month", $to_month);
  $data = array();
  for ($month=$from_month; $month<=$to_month; $month=strtotime("+1 month", $month)) {
    $data[$month] = ncn_report_month_get_month_report_data($month);
  }
  return $data;
}

/**
* Get Speicific Month Overview Report Data 
* 
* @param mixed $t_month
*/
function ncn_report_month_get_month_report_data($t_month) {
  $start_up = ncn_report_month_get_startup_count($t_month);
  $monthly_renewal = ncn_report_month_get_monthly_renewal_count($t_month);
  $declined_monthly_renewal = ncn_report_month_get_declined_monthly_renewal_count($t_month);
  $declined_recovery = ncn_report_month_get_declined_recovery_count($t_month);
  $refund = ncn_report_month_get_refund_count($t_month);
  $member_type_change = ncn_report_month_get_upgrades_downgrades_count($t_month);
  $cancel = ncn_report_month_get_cancel_member_count($t_month);
  $pause_renewal = ncn_report_month_get_pause_renewal_member_count($t_month);
  
  return array(
    'start_up' => $start_up, 
    'monthly_renewal' => $monthly_renewal,
    
    'declined_monthly_renewal' => $declined_monthly_renewal, 
    'declined_recovery' => $declined_recovery,
    
    'upgrades' => $member_type_change['upgrades'],
    'downgrades' => $member_type_change['downgrades'],
    
    'cancel' => $cancel, 
    'pause_renewal' => $pause_renewal, 
    'refund'=>$refund, 
  );
}

function ncn_report_month_get_startup_count($t_month) {
  $version = ncn_get_version_time($report_date);

  $total = 0;
  $data = array(
    'gold'          => 0, //v2
    'coach_on_call' => 0, 
    'gold_coach'    => 0, 
    'other'         => 0, 

    'silver'        => 0, //v2
    'platinum'      => 0, //v2
  );

  
  $between = ncn_report_month_get_between_time($t_month);
  $ts_from = $between['from']; $ts_to = $between['to'];
  $result = db_query('SELECT * FROM {payment_log} WHERE timestamp>=:a AND timestamp<:b AND status=1 AND refund=0 AND ptype LIKE :c',
      array(':a'=>$ts_from,':b'=>$ts_to,':c'=>'%'.db_like('SUF-').'%'));

  foreach($result as $row)
  {
    $row = (array)$row;
    $member_id = $row['member_id'];
    

    // Check if sign up transaction is first for member
    $res_check_first_startup = db_query('SELECT COUNT(*) FROM {payment_log} WHERE status=1 AND refund=0 AND ptype LIKE :a AND
    timestamp<:b AND (member_id=:c OR target_mid=:d)', array(':a'=>'%'.db_like('SUF-').'%',':b'=>$row['timestamp'],
    ':c'=>$member_id,':d'=>$member_id))->fetchField();
    if ($res_check_first_startup > 0) { continue; }
    
    if ($member_id == 0) { $member_id = $row['target_mid']; }
    if ($member_id != 0) {
      $member_type = ncn_report_month_get_member_type_time($member_id, $ts_from);

      if ($version == '1.0') {
        if ($row['ptype'] == 'SUF-GTGCSF') {
          continue; 
        } else {
          switch ($member_type) {
            case 0: 
              $data['gold'] += 1;
              break;
            case 3:
              $data['coach_on_call'] += 1;
              break;
            case 4:
              $data['gold_coach'] += 1;
              break;
            default:
              $data['other'] += 1;
              break;
          }
        }
      } else { // Version 2.0
        switch ($member_type) {
          case MT_GOLD: 
            $data['gold'] += 1;
            break;
          case MT_SILVER:
            $data['silver'] += 1;
            break;
          case MT_PLATINUM:
            $data['platinum'] += 1;
            break;
          default:
            $data['other'] += 1;
            break;
        }
      }
    }
  }
  
  // Calc Total
  foreach ($data as $key=>$count) {
    $total += $count;
  }
  $data['total'] = $total;
  return $data;
}

/**
* Get Member Type in Speicific Time
* 
* @param mixed $member_id
* @param mixed $t_time
*/
function ncn_report_month_get_member_type_time($member_id, $t_time=0) {
  if ($t_time == 0) {
    $t_time = date('U');
  }
  
  $member_type = 0;
  $result = db_query('SELECT * FROM {ncn_member_type_change_log} WHERE member_id=:a AND timestamp>=:b ORDER BY timestamp ASC',
  array(':a'=>$member_id,':b'=>$t_time));

  foreach($result as $row)
  {
    $row = (array)$row;
    if($row)
    {
        $member_type = $row['old_type'];
    }
    else
    {
        $member = get_member_from_id($member_id);
        $member_type = $member['member_type'];
    }
  }
    return $member_type;
}

//
//    if ($row = db_fetch_array($result)) {
//    $member_type = $row['old_type'];
//  } else {  // Current
//    $member = get_member_from_id($member_id);
//    $member_type = $member['member_type'];
//  }
//
//  return $member_type;


function ncn_report_month_get_between_time($t_month) {
  $from = strtotime(date("m/01/Y", $t_month));
  $to   = strtotime("+1 month", $from);
  return array(
    'from'=>$from, 
    'to'  =>$to
  );
}

/**
* Get Monthly Renewal
* 
* @param mixed $t_month
*/
function ncn_report_month_get_monthly_renewal_count($t_month) {
  $version = ncn_get_version_time($report_date);

  $total = 0;
  $data = array(
    'gold'          => 0, //v2
    'coach_on_call' => 0, 
    'gold_coach'    => 0, 
    'gold_lite'     => 0, 
    'other'         => 0, 

    'silver'        => 0, //v2
    'platinum'      => 0, //v2

  );
  
  $between = ncn_report_month_get_between_time($t_month);
  $ts_from = $between['from']; $ts_to = $between['to'];
  $result = db_query('SELECT * FROM {payment_log} WHERE timestamp>=:a AND timestamp<:b AND status=1 AND refund=0 AND ptype LIKE :c',
      array(':a'=>$ts_from,':b'=>$ts_to,':c'=>'%'.db_like('MMF').'%'));
//  while ($row = db_fetch_array($result))
  foreach($result as $row)
  {
       $row = (array)$row;
       $cc = db_query('SELECT * FROM {cc_declined} WHERE charge_pid=:a',array(':a'=>$row['pid']));

        foreach($cc as $recharged_transaction)
        {
            $recharged_transaction = (array)$recharged_transaction;
            if($recharged_transaction && date("m/Y", $recharged_transaction['timestamp']) != date("m/Y", $recharged_transaction['charge_timestamp']))
            {
                continue;
            }
            else
            {
                if ($version == '1.0') {
                  switch ($row['ptype'])
                  {
                      case 'MMF':
                          $data['gold'] += 1;
                          break;
                      case 'MMF-COC':
                          $data['coach_on_call'] += 1;
                          break;
                      case 'MMF-GC':
                          $data['gold_coach'] += 1;
                          break;
                      case 'MMF-GL':
                          $data['gold_lite'] += 1;
                          break;
                      default: 
                          $data['other'] += 1;
                          break;
                  }
                } else {  // Version 2.0
                  switch ($row['ptype'])
                  {
                      case 'MMF':
                          $data['gold'] += 1;
                          break;
                      case 'MMF-SILVER':
                          $data['silver'] += 1;
                          break;
                      case 'MMF-PLATINUM':
                          $data['platinum'] += 1;
                          break;
                      default: 
                          $data['other'] += 1;
                          break;
                  }
                }
            }
        }
  }
    foreach ($data as $key=>$count) {
        $total += $count;
    }
    $data['total'] = $total;
    return $data;
}
//  {
//    $recharged_transaction = db_fetch_array(db_query("SELECT * FROM {cc_declined} WHERE charge_pid=".$row['pid']));
//    if ($recharged_transaction && date("m/Y", $recharged_transaction['timestamp']) != date("m/Y", $recharged_transaction['charge_timestamp'])) {
//      continue;
//    } else {
//      switch ($row['ptype']) {
//        case 'MMF':
//          $data['gold'] += 1;
//          break;
//        case 'MMF-COC':
//          $data['coach_on_call'] += 1;
//          break;
//        case 'MMF-GC':
//          $data['gold_coach'] += 1;
//          break;
//        case 'MMF-GL':
//          $data['gold_lite'] += 1;
//          break;
//      }
//    }
//  }
//
//  // Calc Total
//  foreach ($data as $key=>$count) {
//    $total += $count;
//  }
//  $data['total'] = $total;
//  return $data;
//}

/**
* Declined Monthly Renewal
*   Monthly Declined: Not recovered within 30 days
* 
* @param mixed $t_month
*/
function ncn_report_month_get_declined_monthly_renewal_count($t_month) {
  $between = ncn_report_month_get_between_time($t_month);
  $ts_from = $between['from'];
    $ts_to = $between['to'];

    $declined = db_query('SELECT COUNT(*) FROM {cc_declined} WHERE timestamp>=:a AND timestamp<:b AND status=0 AND product_id=32',
    array(':a'=>$ts_from,':b'=>$ts_to))->fetchField();
    //$declined = db_query("SELECT * FROM {cc_declined} WHERE timestamp>=$ts_from AND timestamp<$ts_to AND status=0 AND product_id=32");

    $declined_r = db_query('SELECT COUNT(*) FROM {cc_declined} WHERE status=1 AND timestamp>=:a AND timestamp<:b AND charge_timestamp>=:c AND product_id=32',
    array(':a'=>$ts_from,':b'=>$ts_to,':c'=>$ts_to))->fetchField();

    return ($declined+$declined_r);


//  $declined = db_query("SELECT COUNT(*) FROM {cc_declined} WHERE timestamp>=$ts_from AND timestamp<$ts_to AND status=0 AND product_id=32");
  //$declined = db_query("SELECT * FROM {cc_declined} WHERE timestamp>=$ts_from AND timestamp<$ts_to AND status=0 AND product_id=32");
  
//   $declined_r = db_query("SELECT COUNT(*) FROM {cc_declined} WHERE status=1 AND timestamp>=$ts_from AND timestamp<$ts_to AND charge_timestamp>=$ts_to AND product_id=32");


  //Drupal 7 working
   /* $declined = db_query('SELECT * FROM {cc_declined} WHERE timestamp>=:a AND timestamp<:b AND status=0 AND status=1 AND charge_timestamp>=:c AND product_id=32',
    array(':a'=>$ts_from,':b'=>$ts_to,':c'=>$ts_to));
    $result = $declined->rowCount(); */


//  return ($declined+$declined_r);
// return $result;
//  return $declined_r;
//  return ($declined+$declined_r);
}

/**
* Declined Recovery
*   If declined is recovered within the same 30 day period it is no longer declined, it is "Declined Recovery".
* @param mixed $t_month
*/
function ncn_report_month_get_declined_recovery_count($t_month) {
  $between = ncn_report_month_get_between_time($t_month);
  $ts_from = $between['from']; $ts_to = $between['to'];
    
  $declined_r = db_query('SELECT * FROM {cc_declined} WHERE status=1 AND charge_timestamp>=:a AND charge_timestamp<:b AND timestamp<:c AND product_id=32',
      array(':a'=>$ts_from,':b'=>$ts_to,':c'=>$ts_from));
  $declined_r = $declined_r->rowCount();
  return $declined_r;
}

/**
* Member Upgrades 
* 
* @param mixed $t_month
*/
function ncn_report_month_get_upgrades_downgrades_count($t_month) {
  $between = ncn_report_month_get_between_time($t_month);
  $ts_from = $between['from'];
  $ts_to = $between['to'];
  
  $result = db_query('SELECT * FROM {ncn_member_type_change_log} WHERE timestamp>=:a AND timestamp<:b',
      array(':a'=>$ts_from,':b'=>$ts_to));

  $upgrades = $downgrades = array();
//  while ($row = db_fetch_array($result))
  foreach($result as $row)
  {
      $row = (array)$row;
      if (ncn_report_month_check_upgrades_downgrades($row['old_type'], $row['new_type']))
      {
          if (isset($upgrades[$row['old_type']][$row['new_type']])) { $upgrades[$row['old_type']][$row['new_type']] += 1; }
          else { $upgrades[$row['old_type']][$row['new_type']] = 1; }
      }
      else
      {
          if (isset($downgrades[$row['old_type']][$row['new_type']])) { $downgrades[$row['old_type']][$row['new_type']] += 1; }
          else { $downgrades[$row['old_type']][$row['new_type']] = 1; }
      }
  }


//
//
//  {
//    if (ncn_report_month_check_upgrades_downgrades($row['old_type'], $row['new_type'])) {
//      if (isset($upgrades[$row['old_type']][$row['new_type']])) { $upgrades[$row['old_type']][$row['new_type']] += 1; }
//      else { $upgrades[$row['old_type']][$row['new_type']] = 1; }
//    } else {
//      if (isset($downgrades[$row['old_type']][$row['new_type']])) { $downgrades[$row['old_type']][$row['new_type']] += 1; }
//      else { $downgrades[$row['old_type']][$row['new_type']] = 1; }
//    }
//  }
//
  $type_name = array(
    0=>'gold', 
    1=>'silver', 
    2=>'gl', 
    3=>'coc', 
    4=>'gc', 
    5=>'csi'
  );
  
  $str_upgrades = array();
  $str_downgrades = array();
  
  foreach ($upgrades as $old_type=>$n_types) {
    foreach ($n_types as $new_type=>$count) {
      if ($count > 0) {
        $str_upgrades[] = $count." ".$type_name[$old_type]." to ".$type_name[$new_type];
      }
    }
  }
  
  foreach ($downgrades as $old_type=>$n_types) {
    foreach ($n_types as $new_type=>$count) {
      if ($count > 0) {
        $str_downgrades[] = $count." ".$type_name[$old_type]." to ".$type_name[$new_type];
      }
    }
  }
  
  return array(
    'upgrades' => implode("<br/>", $str_upgrades), 
    'downgrades' => implode("<br/>", $str_downgrades),
  );
}

/**
* Check whether changing of type is upgrades or downgrades
* 
* @param mixed $old_type
* @param mixed $new_type
*/
function ncn_report_month_check_upgrades_downgrades($old_type, $new_type) {
  $data = array(
    0 => 3,     // Gold
    1 => 0,     // Silver
    2 => 1,     // Gold Lite
    3 => 2,     // Coach on Call
    4 => 4,     // Gold Coach
    5 => 5,     // CSI
  );
  
  if ($data[$old_type] < $data[$new_type]) {
    return TRUE;
  }
  return FALSE;
}

/**
* Refund
* 
* @param mixed $t_month
*/
function ncn_report_month_get_refund_count($t_month) {
  $between = ncn_report_month_get_between_time($t_month);
  $ts_from = $between['from']; $ts_to = $between['to'];
  
  $refund = db_query('SELECT COUNT(*) FROM {payment_log} WHERE timestamp>=:a AND timestamp<:b AND status=1 AND refund=-1',
  array(':a'=>$ts_from,':b'=>$ts_to))->fetchField();
 // $refund = $refund->rowCount();
  return $refund;
}

/**
* Cancel
* 
* @param mixed $t_month
*/
function ncn_report_month_get_cancel_member_count($t_month) {
  $between = ncn_report_month_get_between_time($t_month);
  $ts_from = $between['from']; $ts_to = $between['to'];
  
  $data = array();
  
  $result = db_query('SELECT * FROM {ncn_user_block_track} WHERE blocked!=0 AND returned=0 AND blocked>=:a AND blocked<:b',
      array(':a'=>$ts_from,':b'=>$ts_to));
//  while ($bt_row=db_fetch_array($result))

  foreach($result as $bt_row)
  {
      $bt_row = (array)$bt_row;
      $_user = user_load($bt_row['uid']);
      if (isset($_user->profile_memberid) && is_member($_user->profile_memberid)) {
          $renewal_day = ncn_user_get_member_renewal_day($_user->profile_memberid, $bt_row['blocked']);
          if ($renewal_day && $renewal_day<=date("d", $bt_row['blocked'])) {
              $member_type = ncn_report_month_get_member_type_time($_user->profile_memberid, $bt_row['blocked']);
              if (isset($data[$member_type])) {
                  $data[$member_type] += 1;
              } else {
                  $data[$member_type] = 1;
              }
          }
      }

  }
//  {
//    $_user = user_load($bt_row['uid']);
//    if ($_user->profile_memberid && is_member($_user->profile_memberid)) {
//      $renewal_day = ncn_user_get_member_renewal_day($_user->profile_memberid, $bt_row['blocked']);
//      if ($renewal_day && $renewal_day<=date("d", $bt_row['blocked'])) {
//        $member_type = ncn_report_month_get_member_type_time($_user->profile_memberid, $bt_row['blocked']);
//        if (isset($data[$member_type])) {
//          $data[$member_type] += 1;
//        } else {
//          $data[$member_type] = 1;
//        }
//      }
//    }
//  }
  
  // check for the next month  
  $ts_from = strtotime("+1 month", $ts_from);
  $ts_to = strtotime("+1 month", $ts_from);
  
  $result = db_query('SELECT * FROM {ncn_user_block_track} WHERE blocked!=0 AND returned=0 AND blocked>=:a AND blocked<:b',
      array(':a'=>$ts_from,':b'=>$ts_to));
//  while ($bt_row=db_fetch_array($result))

  foreach($result as $bt_row)
  {
      $bt_row = (array)$bt_row;
      $_user = user_load($bt_row['uid']);
      if (isset($_user->profile_memberid) && is_member($_user->profile_memberid)) {
          $renewal_day = ncn_user_get_member_renewal_day($_user->profile_memberid, $bt_row['blocked']);
          if ($renewal_day && $renewal_day>date("d", $bt_row['blocked'])) {
              $member_type = ncn_report_month_get_member_type_time($_user->profile_memberid, $bt_row['blocked']);
              if (isset($data[$member_type])) {
                  $data[$member_type] += 1;
              } else {
                  $data[$member_type] = 1;
              }
          }
      }

  }
//
//  {
//    $_user = user_load($bt_row['uid']);
//    if ($_user->profile_memberid && is_member($_user->profile_memberid)) {
//      $renewal_day = ncn_user_get_member_renewal_day($_user->profile_memberid, $bt_row['blocked']);
//      if ($renewal_day && $renewal_day>date("d", $bt_row['blocked'])) {
//        $member_type = ncn_report_month_get_member_type_time($_user->profile_memberid, $bt_row['blocked']);
//        if (isset($data[$member_type])) {
//          $data[$member_type] += 1;
//        } else {
//          $data[$member_type] = 1;
//        }
//      }
//    }
//  }
  
  
  $type_name = array(
    0=>'gold', 
    1=>'silver', 
    2=>'gl', 
    3=>'coc', 
    4=>'gc', 
    5=>'csi', 
    6=>'platinum'
  );
  
  $str_data = array();
  foreach ($data as $key=>$count) {
    $str_data[$key] = $count." ".$type_name[$key];
  }
  
  return implode("<br/>", $str_data);
}

/**
* Pause Renewal
* 
* @param mixed $t_month
*/
function ncn_report_month_get_pause_renewal_member_count($t_month) {
  $between = ncn_report_month_get_between_time($t_month);
  $ts_from = $between['from']; $ts_to = $between['to'];
  
  $data = array();
  
  $result = db_query('SELECT * FROM {ncn_user_block_track} WHERE blocked!=0 AND blocked>=:a AND blocked<:b AND unblocked!=0',
  array(':a'=>$ts_from,':b'=>$ts_to));
//  while ($bt_row=db_fetch_array($result))

  foreach($result as $bt_row)
  {
    $bt_row = (array)$bt_row;
     $_user = user_load($bt_row['uid']);
      if ($_user && isset($_user->profile_memberid) && is_member($_user->profile_memberid)) {
          $renewal_day = ncn_user_get_member_renewal_day($_user->profile_memberid, $bt_row['blocked']);
          if ($renewal_day && $renewal_day<=date("d", $bt_row['blocked'])) {
              $member_type = ncn_report_month_get_member_type_time($_user->profile_memberid, $bt_row['blocked']);
              if (isset($data[$member_type])) {
                  $data[$member_type] += 1;
              } else {
                  $data[$member_type] = 1;
              }
          }
      }
  }
//
//  {
//    $_user = user_load($bt_row['uid']);
//    if ($_user->profile_memberid && is_member($_user->profile_memberid)) {
//      $renewal_day = ncn_user_get_member_renewal_day($_user->profile_memberid, $bt_row['blocked']);
//      if ($renewal_day && $renewal_day<=date("d", $bt_row['blocked'])) {
//        $member_type = ncn_report_month_get_member_type_time($_user->profile_memberid, $bt_row['blocked']);
//        if (isset($data[$member_type])) {
//          $data[$member_type] += 1;
//        } else {
//          $data[$member_type] = 1;
//        }
//      }
//    }
//  }
//
  // check for the next month  
  $ts_from = strtotime("+1 month", $ts_from);
  $ts_to = strtotime("+1 month", $ts_from);
  
  $result = db_query('SELECT * FROM {ncn_user_block_track} WHERE blocked!=0 AND blocked>=:a AND blocked<:b AND unblocked!=0',
  array(':a'=>$ts_from,':b'=>$ts_to));
//  while ($bt_row=db_fetch_array($result))

  foreach($result as $bt_row)
  {
      $bt_row = (array)$bt_row;
      $_user = user_load($bt_row['uid']);
      if ($_user && isset($_user->profile_memberid) && is_member($_user->profile_memberid)) {
          $renewal_day = ncn_user_get_member_renewal_day($_user->profile_memberid, $bt_row['blocked']);
          if ($renewal_day && $renewal_day>date("d", $bt_row['blocked'])) {
              $member_type = ncn_report_month_get_member_type_time($_user->profile_memberid, $bt_row['blocked']);
              if (isset($data[$member_type])) {
                  $data[$member_type] += 1;
              } else {
                  $data[$member_type] = 1;
              }
          }
      }
  }

//  {
//    $_user = user_load($bt_row['uid']);
//    if ($_user->profile_memberid && is_member($_user->profile_memberid)) {
//      $renewal_day = ncn_user_get_member_renewal_day($_user->profile_memberid, $bt_row['blocked']);
//      if ($renewal_day && $renewal_day>date("d", $bt_row['blocked'])) {
//        $member_type = ncn_report_month_get_member_type_time($_user->profile_memberid, $bt_row['blocked']);
//        if (isset($data[$member_type])) {
//          $data[$member_type] += 1;
//        } else {
//          $data[$member_type] = 1;
//        }
//      }
//    }
//  }
  
  
  $type_name = array(
    0=>'gold', 
    1=>'silver', 
    2=>'gl', 
    3=>'coc', 
    4=>'gc', 
    5=>'csi'
  );
  
  $str_data = array();
  foreach ($data as $key=>$count) {
    $str_data[$key] = $count." ".$type_name[$key];
  }
  
  return implode("<br/>", $str_data);
}



?>
