<?php

function payment_transaction_page() {
GLOBAL $base_url;
  if (arg(4) == 'add') {
    return payment_transaction_add_page();
  }
  
  if (arg(4) == 'edit') {
    return payment_transaction_edit_page(arg(5));
  }
  
  
  if (isset($_REQUEST['tfunction']) && ($_REQUEST['tfunction'] == "refund_transaction")) {
    ncn_admin_refund_payment(intval($_REQUEST['param_pid']));
  } else if(isset($_REQUEST['tfunction']) && ($_REQUEST['tfunction'] == "delete_transaction")) {
    delete_payment_transaction(intval($_REQUEST['param_pid']));
  }
  drupal_add_js(drupal_get_path('module', 'ncn_admin') . '/ncn_admin.js');
  
  $filter = 'all';
  if (arg(4) == 'filter') {
    $filter = arg(5);
  }
  $query = "SELECT payment_log.*, claims.expedite FROM payment_log LEFT JOIN claims ON (payment_log.claim_id=claims.claim_id) WHERE status=1 ";

  if ( $filter=='all') {
    $where='';
  } else {
    $where="AND product_id=".$filter;
  }
  
  $specific_date = (isset($_REQUEST['specific_date']) ? $_REQUEST['specific_date'] : false);
  $is_expedite = (isset($_REQUEST['claim_expedite']) ? $_REQUEST['claim_expedite'] : false);
  if (!$is_expedite) { $is_expedite = 'all'; }
  
  if ($specific_date) {
    //$val_specific_date = date('m/d/Y', $specific_date);
    $from_t = strtotime($specific_date);
    $to_t = strtotime('+1 day', $from_t);
    $where .= " AND timestamp>=$from_t AND timestamp<$to_t";
  }
  
  if ($is_expedite != 'all') {
    $b_expedite = 0;
    if ($is_expedite == 'expedite') {
      $b_expedite = 1;
    }
    $where .= " AND expedite=$b_expedite";
  }

  $page = 0;
  if (isset($_REQUEST['page'])) {
    $page = intval($_REQUEST['page']);
  }  
  $query .= $where;
  
  $tot_result = db_query("SELECT COUNT(*) as tot FROM payment_log LEFT JOIN claims ON (payment_log.claim_id=claims.claim_id) WHERE status=1 ".$where)->fetchAssoc();
  $total_count = intval($tot_result['tot']);  
  $num_per_page = 50;
  if ($total_count!=0 && $page >= ceil($total_count/$num_per_page)) {
    $page = ceil($total_count/$num_per_page)-1;
  }
  $query .= " ORDER BY timestamp DESC";
  $query .= " LIMIT ".($page*$num_per_page).", $num_per_page";
  $result = db_query($query);
  $row_count = 0;
  if ($result) {
    $row_count = $result->rowCount();
  }
  
  $data_type = get_transaction_type('all');
  
  $table_head = '
  <thead class="tableHeader-processed">
    <th>PID</th>
    <th>Date</th>
    <th>Type</th>
    <th>Name</th>
    <th>Cost</th>
    <th colspan="3">Operation</th>
  </thead>
  <tbody>';

  $table_foot = '
  </tbody>
  </table>';
  
  $url = $base_url.'/admin/config/ncn_admin/transaction';
  
  drupal_add_js("
    jQuery(function() {
    jQuery( '#specific_date' ).datepicker({
      showOn: \"button\",
      buttonImage: \"$base_url/".drupal_get_path('module', 'ncn_report')."/images/calendar.gif\",
      buttonImageOnly: true,
      dateFormat: 'mm/dd/yy',
    });
  });", 'inline');
  
  ob_start();
?>
<div id="payment_transaction_page">
  <div class="description">  </div>
  <form id="transaction_filter" method="POST">
  <div class="transaction-where">
    <fieldset>
      <legend>Show only transactions where</legend>
      <div class="clear-block">
      <dl class="where">
        <dd class="a">
          <div>Transaction Type</div>
          <div>Expedite of Claim</div>
          <div>Date</div>
        </dd>
        <dt>is</dt>
        <dd class="b">
          <div>
            <?php echo draw_select_transaction_type('filter', 'payment_filter', $filter, 'class=" "'); ?>
          </div>
          <div>
            <?php echo draw_select_filter_expedite('claim_expedite', $is_expedite, 'class=" "'); ?>
          </div>
          <div>
            <input type="text" id="specific_date" name="specific_date" value="<?php echo $specific_date; ?>" />
          </div>
        </dd>
      </dl>
      </div>
      <div class="filter-panel">
        <input type="hidden" name="tfunction" id="param_tfunction" value="" />
        <input type="hidden" name="param_pid" id="param_pid" value="" />
        <dl><input id="edit-submit" class="form-submit" type="button" value="  Filter  " onclick="jump_filter_url('<?php echo $url; ?>');"></dl>
      </div>
    </fieldset>
  </div>
  </form>
  
  <fieldset>
    <legend>Report</legend>
    <div class="transaction-panel report-panel">
      <a href="<?php echo $base_url; ?>/download/transaction">Download Transaction List</a>&nbsp;&nbsp;&nbsp;
    </div>
  </fieldset>
  
  <fieldset>
    <legend>Add Transaction</legend>
    <div class="transaction-panel">
      <a href="<?php echo $base_url; ?>/admin/config/ncn_admin/transaction/add/standard">Add a standard transaction</a>&nbsp;&nbsp;&nbsp;
      <a href="<?php echo $base_url; ?>/admin/config/ncn_admin/transaction/add/additional">Add an additional transaction</a>
    </div>
  </fieldset>
  
  <div class="transaction-table">
<?php 
  // no results message
  if ($total_count == 0)
  {
    print "<table>";
    print $table_head;
    print $table_foot;
  } else {
    echo '<table class="sticky-enabled tableSelect-processed sticky-table">';
    echo $table_head;
    
    //for($i=0; $i<$row_count; $i++) 
  $i=0; 
	foreach($result as $row)
	{
      $row = (array)$row; 
      $ptype = get_item_from_key($data_type, 'ptype', $row['ptype']);
      if ($row['claim_id']!=0 && ncn_cd($row['claim_id'], 'expedite')) { $ptype['name'].=" - EXPEDITE"; }
      $pname = get_payment_from($row);
      
      $str_cost = render_payment_cost($row['cost']);
        
      // draw row
      ?>
      <tr class="<?php if ($i%2) print "even"; else print "odd"; ?> <?php if($row['refund']==-1) print "refund"; ?>">
        <td><?php echo $row['pid']; ?></td>
        <td><?php echo date('d M, Y H:i', $row['timestamp']); ?></td>
        <td><?php echo $ptype['name']; ?></td>
        <td><?php echo $pname; ?></td>
        <td><?php echo $str_cost; ?></td>
        <td><a href="<?php echo $base_url; ?>/admin/config/ncn_admin/transaction/edit/<?php echo $row['pid']; ?>">Edit</a></td>
        <td><?php if($row['refund']==0): ?>
          <a href="#" onclick="on_click_transaction_refund(<?php echo $row['pid']; ?>)">Refund</a>
          <?php elseif($row['refund']==1): ?>
          <span style="color: #808080;">Refunded</span>
          <?php endif; ?>
        </td>
        <td><a href="#" onclick="on_click_transaction_delete(<?php echo $row['pid']; ?>)">Delete</a></td>
        
      </tr>
      <?php
      $i++;
    }
    
    echo $table_foot;
    $url = $base_url."/".drupal_get_path_alias($_GET["q"]);
    echo ncn_admin_table_pagniation($url, $total_count, $num_per_page, $page, 15);
  }
?>
</div></div>
<?php
  $content = ob_get_contents();
  ob_end_clean();
  
  return $content;
}


function delete_payment_transaction($pid) {
 // $query = "DELETE FROM payment_log WHERE pid=$pid";
  $result = db_query('DELETE FROM {payment_log} WHERE pid=:a',array(':a'=>$pid));
  if (!$result) {
    drupal_set_message(t('Failed to delete payment transaction(#!pid) deleted, successfully.', array('!pid'=>$pid)), 'error');
  } else {
    drupal_set_message(t('Payment Transaction(#!pid) deleted, successfully.', array('!pid'=>$pid)));
    ncn_report_delete_payment_ncn($pid);
  }
}
  
function get_payment_from($tr) {
  $pname ='';
  if ($tr['uid'] != 0) {
    $_user = user_load($tr['uid']);
    if ($_user) {
      $pname = $_user->profile_firstname.' '.$_user->profile_lastname;
    }
  } else {
    $member = get_member_from_id($tr['member_id']);
    if (!empty($member)) {
      $pname = $member['first_name'].' '.$member['last_name'];
    } else {
      $pname .= "removed member:".$tr['member_id']."";
    }
  }
  
  if ($tr['uid'] != 0) {
    $_user = user_load($tr['uid']);
    if ($_user && is_distributor($_user) && $tr['target_mid']!=0) {
      $pname = $_user->profile_legalname;
      $member = get_member_from_id($tr['target_mid']);
      if (!empty($member)) {
        $pname .= ( ' (for '.$member['first_name'].' '.$member['last_name'].')' );
      } else {
        $pname .= "(for removed member:".$tr['target_mid'].")";
      }
    }
  }
  
  return $pname;
}  


function draw_select_transaction_type($type, $name, $select_val, $attributes) {
  $content = "<select id='$name' name=\"$name\" $attributes>";
  if ($type == 'filter') {
    $data = get_transaction_type("all");
    $selected = '';
    if ( $select_val == 'all' ) { $selected = 'selected'; }
    $content .= '<option value="all" '.$selected.' >Show All</option>';
  } else {
    $data = get_transaction_type();
  }
  for ($i=0; $i<count($data); $i++) {
    $selected = '';
    if ( ((string)$data[$i]['product_id']) == $select_val ) { $selected = 'selected'; }
    $content .= '<option value="'.$data[$i]['product_id'].'" '.$selected.' >'.$data[$i]['name'].'&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;('.$data[$i]['price'].')</option>';
  }
  $content .= "</select>";
  
  return $content;
}

function draw_select_filter_expedite($name, $select_val, $attributes) {
  $content = "<select id='$name' name=\"$name\" $attributes>";
  $data = array(
    'all' => 'All', 
    'non-expedite' => 'No Expedite', 
    'expedite' => 'Expedite',     
  );
  
  foreach ($data as $key=>$val) {
    $selected = '';
    if ( $key == $select_val ) { $selected = 'selected'; }
    $content .= '<option value="'.$key.'" '.$selected.' >'.$val.'</option>';
  }
  $content .= "</select>";
  
  return $content;
}

/**
*  Download Handler (download/transaction)
*/
function ncn_admin_payment_download_transaction() {
  $transactions = ncn_admin_payment_get_downloadable_transactions('all');
  
  require_once 'sites/all/libraries/php_xls/Classes/PHPExcel.php';

  $report_name = "transaction.xls";
  $demo_file = "transaction.xls";
  $objReader = PHPExcel_IOFactory::createReader("Excel5");
  $objPHPExcel = $objReader->load($demo_file);
  $count = count($transactions);
  
  if ($count>0) {
    $objPHPExcel->getActiveSheet()->insertNewRowBefore(9, $count);
  }
  for ($i=0; $i<$count; $i++) {
    $source = "A7:G7";
    if ($i%2 == 1) {
      $source = "A8:G8";
    }
    $row_index = $i+9;
    $dest = "A$row_index:G$row_index";
    $objPHPExcel->getActiveSheet()->duplicateStyle( $objPHPExcel->getActiveSheet()->getStyle($source), $dest );  
  }
  $objPHPExcel->getActiveSheet()->removeRow(7+$count, 2);
  
  $i = 0;
  foreach ($transactions as $tr) {
    $row_index = $i+7;
    
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("A$row_index" , $tr['pid']);
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("B$row_index" , $tr['time']);
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("C$row_index" , $tr['type']);
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("D$row_index" , $tr['name']);
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("E$row_index" , $tr['company']);
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("F$row_index" , $tr['member_since']);
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("G$row_index" , $tr['amount']);
    
    $i += 1;
  }
  
  header('Content-Type: application/vnd.ms-excel');
  header('Content-Disposition: attachment;filename="'.$report_name.'"');
  header('Cache-Control: max-age=0');
  $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5');
  $objWriter->save('php://output');
  exit;
}

/**
* Get Transactions in Downloadable Report
* 
* @param mixed $mode
*/
function ncn_admin_payment_get_downloadable_transactions($mode) {
  $data = array();
  
  $query = "SELECT * FROM {payment_log} WHERE status=1 AND timestamp>".strtotime('-1 year');
  $where = '';
  
  $query .= $where;
  $query .= " ORDER BY timestamp DESC";
  
  $result = db_query($query);
  $data_type = get_transaction_type('all');
  foreach($result as $row) 
  {
    $row = (array)$row;
    $ptype = get_item_from_key($data_type, 'ptype', $row['ptype']);
    $pname = get_payment_from($row);
    $_user = user_load($row['uid']);
    $pcompany_name = '';
    if ($row['uid'] && $_user) {
      $pcompany_name = $_user->profile_legalname;
    } else if ($row['member_id']){
      $member = get_member_from_id($row['member_id']);
      if (!empty($member)) {
        $pcompany_name = $member['legalname'];
      }
    }
    
    $str_cost = render_payment_cost($row['cost']);
    
    $member_since = '';
    
    if ( isset($_user->profile_memberid) && is_member($_user->profile_memberid) ) {
      $member = get_member_from_id($_user->profile_memberid);
      if (!empty($member)) {
        $member_since = $member['create'];
      }
    }
    if ($member_since == '') {
      if ($_user && $_user->created) {
        $member_since = date('m/d/Y', $_user->created);
      }
    } else {
      if ($member_since != 0) {
        $member_since = date('m/d/Y', $member_since);
      } else {
        $member_since = '';
      }
    }
  
    $transaction = array(
      'pid' => $row['pid'], 
      'time'=> date('d M, Y H:i', $row['timestamp']), 
      'type'=> $ptype['name'], 
      'name'=> $pname,
      'company' => $pcompany_name, 
      'member_since'=>$member_since, 
      'amount'=> strip_tags($str_cost), 
    );
    $data[] = $transaction;
  }
  
  return $data;
}

//------------------------------------------------------------------------------
function get_cim_profile_data($user)
{
    // distributor?
    if (is_distributor($user))
    {
    //  $query = "SELECT * FROM member_distributor_cim WHERE username=\"".AddSlashes($user->name)."\";";
        $result = db_query('SELECT * FROM {member_distributor_cim} WHERE username=:a',array(':a'=>$user->name));
        $row_count = $result->rowCount();
        
        if ($row_count > 0)
        {
            $row = $result->fetchAssoc();
            return array('customerProfileId' => $row['customerProfileId'], 'customerPaymentProfileId' => $row['customerPaymentProfileId']);
        }
        else
        {
            return array('customerProfileId' => '', 'customerPaymentProfileId' => '');
        }
    }
    // standard user
    else
    {
//      $query = "SELECT * FROM member_cim WHERE member_id=\"".AddSlashes($user->profile_memberid)."\";";
        $result = db_query('SELECT * FROM {member_cim} WHERE member_id=:a',array(':a'=>isset($user->profile_memberid)?$user->profile_memberid:0));
        $row_count = $result->rowCount();
        
        if ($row_count > 0)
        {
            $row = $result->fetchAssoc();
            return array('customerProfileId' => $row['customerProfileId'], 'customerPaymentProfileId' => $row['customerPaymentProfileId']);
        }
        else
        {
            return array('customerProfileId' => '', 'customerPaymentProfileId' => '');
        }
    }
}

function ncn_admin_verify_payment_date(){
    global $base_url;
    ob_start();
    ?>
    <table>
        <thead class="tableHeader-processed">
        <th>UserId</th>
        <th>User</th>
        <th>Signup Date</th>
        <th>Renewal Day </th>
        <th>Next Payment Date</th>
        <th>Edit Payment Date</th>
        </thead>
        <tbody>
        <?php
        $sql = 'SELECT u.uid,u.name,u.status,
             mip.create as signup_date,mip.next_payment_date as nxp_date,
             mrd.day as renewal_day
             FROM {users} as u
             JOIN {profile_value} AS upv ON (upv.uid = u.uid)
             JOIN {member_id_pool} AS mip ON (mip.member_id = upv.value)
             LEFT JOIN ncn_member_renewal_day AS mrd ON (mrd.member_id = upv.value)
             WHERE 1 AND upv.fid = 48
             GROUP BY mip.member_id
             ORDER BY u.status DESC, nxp_date ASC, mrd.timestamp DESC
            ';
        $result = db_query($sql);

        //if($result->rowCount()>0)
        {
            foreach($result as $i => $row){
                $row = (array) $row;
                ?>
                <tr class="<?php if ($i%2) print "even"; else print "odd"; ?>">
                    <td><?php echo $row['uid'].($row['status']==1 ? " (Active) ":" (InActive) "); ?></td>
                    <td><?php echo $row['name']; ?></td>
                    <td><?php echo date("F d, Y",$row['signup_date']); ?></td>
                    <td><?php echo ($row['renewal_day']!='' ? $row['renewal_day'] : date("j",$row['nxp_date'])); ?></td>
                    <td><?php echo date("F d, Y",$row['nxp_date']); ?></td>
                    <td><a href="<?php echo $base_url;?>/admin/config/ncn_create_user/all_user_list/edit_user/<?php echo $row['uid']; ?>" target="_blank">Edit</a></td>
                </tr>
            <?php
            }
        }
        ?>
        </tbody>
    </table>
    <?php
    $content = ob_get_contents();
    ob_end_clean();
    return $content;
}


/**
 * Refund (action)
 */

function ncn_admin_refund_payment($pid)
{
    if (ncn_report_refund_payment($pid))
    {
        drupal_set_message(t('Refunded payment(#!pid), successfully.', array(
            '!pid' => $pid
        )));
    }
}

/**
 * Add a transaction (page - wrapper)
 */

function payment_transaction_add_page()
{
    if (arg(5) == "additional")
    {
        return payment_transaction_add_additional_page();
    }
    else
    if (arg(5) == "standard")
    {
        return payment_transaction_add_standard_page();
    }
}

/**
 * Add an additional transaction (page)
 */

function payment_transaction_add_additional_page()
{
    $r_data['page_id'] = 'payment_transaction_add_additional_page';
    $r_data['form_id'] = 'add_additional_transaction_form';
    $r_data['tfunction'] = 'add_additional';
    if (isset($_POST['tfunction']) && $_POST['tfunction'] == $r_data['tfunction'])
    {
        $r_data['member_id'] = $_POST['p_member_id'];
        $r_data['amount'] = floatval($_POST['p_amount']);
        $r_data['date'] = $_POST['p_date'];
        $r_data['time'] = $_POST['p_time'];
        if (add_additional_transaction_action($r_data))
        {
            drupal_goto('admin/config/ncn_admin/transaction');
        }
    }
    else
    {
        $r_data['member_id'] = '';
        $r_data['amount'] = 0;
        $r_data['date'] = date('m/d/Y');
        $r_data['time'] = date('H:i');
    }

    drupal_set_title(t('Add an additional transaction'));
    return payment_transaction_render_additional($r_data);
}

/**
 * Update an additional transaction (page)
 */

function payment_transaction_edit_additional_page($pid)
{
    $r_data['page_id'] = 'payment_transaction_add_additional_page';
    $r_data['form_id'] = 'add_additional_transaction_form';
    $r_data['tfunction'] = 'edit_additional';
    if (isset($_POST['tfunction']) && $_POST['tfunction'] == $r_data['tfunction'])
    {
        $r_data['member_id'] = $_POST['p_member_id'];
        $r_data['amount'] = floatval($_POST['p_amount']);
        $r_data['date'] = $_POST['p_date'];
        $r_data['time'] = $_POST['p_time'];
        $r_data['pid'] = $pid;
        edit_additional_transaction_action($r_data);
    }
    else
    {
    //  $query = "SELECT * FROM payment_log WHERE pid=$pid";
        $result = db_query('SELECT * FROM {payment_log} WHERE pid=:a',array(':a'=>$pid));
        if ($result->rowCount() == 0)
        {
            drupal_set_message(t('Transaction(#!pid) doesn\'t exist.', array(
                '!pid' => $pid
            )) , 'error');
            drupal_goto('admin/config/ncn_admin/transaction');
            return;
        }

        $t = $result->fetchAssoc();
        $r_data['pid'] = $pid;
        $r_data['member_id'] = $t['member_id'];
        $r_data['amount'] = $t['cost'];
        $r_data['date'] = date('m/d/Y', $t['timestamp']);
        $r_data['time'] = date('H:i', $t['timestamp']);
    }

    drupal_set_title(t('Edit an additional transaction'));
    return payment_transaction_render_additional($r_data);
}

function payment_transaction_render_additional($r_data)
{
    ob_start();
?>

<div  id="<?php
    echo $r_data['page_id']; ?>">

<form id="<?php
    echo $r_data['form_id']; ?>" method="POST">

<input type="hidden" name="tfunction" value="<?php
    echo $r_data['tfunction']; ?>" />

<table>

    <tr>    <td class="td-label" width="150"><?php
    echo t('Member ID'); ?><span class="p-required">*</span>:</td>  <td><?php
    echo draw_select_member_list_transaction('p_member_id', $r_data['member_id']); ?> </td> </tr>

    <tr>    <td class="td-label" width="150"><?php
    echo t('Amount($)'); ?><span class="p-required">*</span>:</td>  <td><input type="text" name="p_amount" value="<?php
    echo $r_data['amount']; ?>" size="20"/> </td>   </tr>

    <tr>    <td class="td-label" width="150"><?php
    echo t('Date'); ?><span class="p-required">*</span>:</td>       

            <td>    <input type="text" size="20" id="p_date" name="p_date" value="<?php
    echo $r_data['date']; ?>" />

                    <input type="text" size="10" id="p_time" name="p_time" value="<?php
    echo $r_data['time']; ?>" />    

            </td>   </tr>

    <tr>    <td colspan="2" style="padding-top: 10px;"><input type="submit" value=" Save " /> </td></tr>

</table>

</form>

</div>

<?php
    $content = ob_get_contents();
    ob_end_clean();
    return $content;
}

/*

* MENU CALLBACK (admin/config/ncn_admin/transaction/add/standard/)

* Add a Standard Transaction

*/

function payment_transaction_add_standard_page()
{
    $r_data['page_id'] = 'payment_transaction_add_standard_page';
    $r_data['form_id'] = 'add_standard_transaction_form';
    $r_data['tfunction'] = 'add_standard';
    if (isset($_POST['tfunction']) && $_POST['tfunction'] == $r_data['tfunction'])
    {
        $r_data['product_id'] = $_POST['product_id'];
        $r_data['amount'] = floatval($_POST['p_amount']);
        $r_data['uid'] = $_POST['p_uid'];
        $r_data['member_id'] = $_POST['p_member_id'];
        $r_data['target_mid'] = $_POST['p_target_mid'];
        $r_data['claim_id'] = $_POST['p_claim_id'];
        $r_data['ref_pid'] = $_POST['p_ref_pid'];
        $r_data['refund'] = $_POST['p_refund'];
        $r_data['date'] = $_POST['p_date'];
        $r_data['time'] = $_POST['p_time'];
        if (add_standard_transaction_action($r_data))
        {
            drupal_goto('admin/config/ncn_admin/transaction');
        }
    }
    else
    {
        $r_data['product_id'] = '';
        $r_data['amount'] = 0;
        $r_data['uid'] = '';
        $r_data['member_id'] = '';
        $r_data['target_mid'] = '';
        $r_data['claim_id'] = 0;
        $r_data['ref_pid'] = 0;
        $r_data['refund'] = 0;
        $r_data['date'] = date('m/d/Y');
        $r_data['time'] = date('H:i');
    }

    drupal_set_title(t('Add a standard transaction'));
    return payment_transaction_render_standard($r_data);
}

/**
 * Updaet an additional transaction (page)
 */

function payment_transaction_edit_standard_page($pid)
{
    $r_data['page_id'] = 'payment_transaction_edit_standard_page';
    $r_data['form_id'] = 'add_standard_transaction_form';
    $r_data['tfunction'] = 'edit_standard';
    if (isset($_POST['tfunction']) && $_POST['tfunction'] == $r_data['tfunction'])
    {
        $r_data['product_id'] = $_POST['product_id'];
        $r_data['amount'] = floatval($_POST['p_amount']);
        $r_data['uid'] = $_POST['p_uid'];
        $r_data['member_id'] = $_POST['p_member_id'];
        $r_data['target_mid'] = $_POST['p_target_mid'];
        $r_data['claim_id'] = $_POST['p_claim_id'];
        $r_data['ref_pid'] = $_POST['p_ref_pid'];
        $r_data['refund'] = $_POST['p_refund'];
        $r_data['date'] = $_POST['p_date'];
        $r_data['time'] = $_POST['p_time'];
        $r_data['pid'] = $pid;
        edit_standard_transaction_action($r_data);
    }
    else
    {
//      $query = "SELECT * FROM payment_log WHERE pid=$pid";
        $result = db_query('SELECT * FROM {payment_log} WHERE pid=:a',array(':a'=>$pid));
        if ($result->rowCount() == 0)
        {
            drupal_set_message(t('Transaction(#!pid) doesn\'t exist.', array('!pid' => $pid)) , 'error');
            drupal_goto('admin/config/ncn_admin/transaction');
            return;
        }

        $t = $result->fetchAssoc();
        $r_data['pid'] = $pid;
        $r_data['product_id'] = $t['product_id'];
        $r_data['amount'] = floatval($t['cost']);
        $r_data['uid'] = $t['uid'];
        $r_data['member_id'] = $t['member_id'];
        $r_data['target_mid'] = $t['target_mid'];
        $r_data['claim_id'] = $t['claim_id'];
        $r_data['ref_pid'] = $t['ref_pid'];
        $r_data['refund'] = $t['refund'];
        $r_data['date'] = date('m/d/Y', $t['timestamp']);
        $r_data['time'] = date('H:i', $t['timestamp']);
    }

    drupal_set_title(t('Edit an standard transaction'));
    return payment_transaction_render_standard($r_data);
}

function payment_transaction_render_standard($r_data)
{
    ob_start();
?>

<div  id="<?php
    echo $r_data['page_id']; ?>">

<form id="<?php
    echo $r_data['form_id']; ?>" method="POST">

<input type="hidden" name="tfunction" value="<?php
    echo $r_data['tfunction']; ?>" />

<table>

    <tr>

        <td class="td-label" width="150"><?php
    echo t('Payment Type'); ?><span class="p-required">*</span>:</td>

        <td><?php
    echo draw_select_transaction_type('field', 'product_id', $r_data['product_id'], ''); ?></td>

    </tr>

    <tr>    <td class="td-label" width="150"><?php
    echo t('Refund'); ?><span class="p-required">*</span>:</td> <td><?php
    echo draw_select_refund('p_refund', $r_data['refund']); ?></td> </tr>

    <tr>    <td class="td-label" width="150"><?php
    echo t('Amount($)'); ?><span class="p-required">*</span>:</td>  <td><input type="text" name="p_amount" value="<?php
    echo $r_data['amount']; ?>" size="20"/> </td>   </tr>

    <tr>    <td class="td-label" width="150"><?php
    echo t('User ID'); ?><span class="p-required">*</span>:</td>    <td><?php
    echo draw_select_user_list_transaction('p_uid', $r_data['uid']); ?> 

            &nbsp;&nbsp;&nbsp;<?php
    echo t('or Member ID'); ?><span class="p-required">*</span>:&nbsp;

            <?php
    echo draw_select_member_list_transaction('p_member_id', $r_data['member_id']); ?>

            </td>   </tr>

    <tr>    <td class="td-label" width="150"><?php
    echo t('Target Member ID'); ?>:</td>    <td><?php
    echo draw_select_member_list_transaction('p_target_mid', $r_data['target_mid']); ?> </td>   </tr>

    <tr>    <td class="td-label" width="150"><?php
    echo t('Claim ID'); ?>:</td>    <td><input type="text" size="20" id="p_claim_id" name="p_claim_id" value="<?php
    echo $r_data['claim_id']; ?>" /></td></tr>

    <tr>    <td class="td-label" width="150"><?php
    echo t('Reference PID'); ?>:</td> <td><input type="text" size="20" id="p_ref_pid" name="p_ref_pid" value="<?php
    echo $r_data['ref_pid']; ?>" /></td></tr>

    <tr>    <td class="td-label" width="150"><?php
    echo t('Date'); ?><span class="p-required">*</span>:</td>       

            <td>    <input type="text" size="20" id="p_date" name="p_date" value="<?php
    echo $r_data['date']; ?>" />

                    <input type="text" size="10" id="p_time" name="p_time" value="<?php
    echo $r_data['time']; ?>" />    

            </td>   </tr>

    <tr>    <td colspan="2" style="padding-top: 10px;"><input type="submit" value=" Save " /> </td></tr>

</table>

</form>

</div>

<?php
    $content = ob_get_contents();
    ob_end_clean();
    return $content;
}

/*

* MENU CALLBACK (admin/config/ncn_admin/transaction/%pid)

* Edit Transaction

*/

function payment_transaction_edit_page($pid)
{
//  $query = "SELECT * FROM payment_log WHERE pid=$pid";
    $result = db_query('SELECT * FROM {payment_log} WHERE pid=:a',array(':a'=>$pid));
    if ($result->rowCount()==0)
    {
        drupal_set_message(t('Transaction(#:pid) is invalid, Please contact web master about this.', array(
            ':pid' => $pid
        )) , 'error');
        return "";
    }
    else
    if ($result->rowCount() == 0)
    {
        drupal_set_message(t('Transaction(#:pid) doesn\'t exist.', array(
            ':pid' => $pid
        )) , 'error');
        return "";
    }

//  $transaction = (array)$result;
    $transaction = $result->fetchAssoc();
    if ($transaction['ptype'] == 'ADDITIONAL')
    {
        return payment_transaction_edit_additional_page($pid);
    }
    else
    {
        return payment_transaction_edit_standard_page($pid);
    }
}

function draw_select_user_list_transaction($name, $select_val, $attributes = '')
{
    $data = array();
    $result = db_query('SELECT * FROM {users} ORDER BY uid');
    foreach($result as $row)
    {
        $row = (array)$row;
        $_user = user_load($row['uid']);
        if (is_distributor($_user) || (isset($_user->profile_memberid) && is_member($_user->profile_memberid)))
        {
            $data[] = $_user;
        }
    }

    // $data = get_user_list();

    $content = "<select name=\"$name\" $attributes>";
    $content.= '<option value="">Please Select</option>';
    foreach($data as $_user)
    {
        $selected = '';
        if ($_user->uid == $select_val)
        {
            $selected = 'selected';
        }

        if (is_distributor($_user))
        {
            $str_type = "Distributor";
        }
        else
        {
            $member_type = get_member_type_num($_user->uid);
            if ($member_type == 0)
            {
                $str_type = "Gold Member";
            }
            else
            if ($member_type == 1)
            {
                $str_type = "Silver Member";
            }
            else
            if ($member_type == 2)
            {
                $str_type = "Gold Lite Member";
            }
            else
            if ($member_type == 3)
            {
                $str_type = "Coach on Call Member";
            }
            else
            if ($member_type == 4)
            {
                $str_type = "Gold Coach Member";
            }
            else
            if ($member_type == 5)
            {
                $str_type = "CSI Member";
            }
        }

        $str_value = sprintf("[#%s] - %s %s (%s)", $_user->uid, $_user->profile_firstname, $_user->profile_lastname, $str_type);
//        $str_value = sprintf("[#:a] - :b :c (:d)",
//            array(':a'=>$_user->uid,':b'=>$_user->profile_firstname,':c'=>$_user->profile_lastname,':d'=>$str_type));
        $content.= '<option value="' . $_user->uid . '" ' . $selected . ' >' . $str_value . '</option>';
    }

    $content.= "</select>";
    return $content;
}

function draw_select_member_list_transaction($name, $select_val, $attributes = '')
{
    $data = get_member_list();
    $content = "<select name=\"$name\" $attributes>";
    $content.= '<option value="">Please Select</option>';
    for ($i = 0; $i < count($data); $i++)
    {
        $selected = '';
        if ($data[$i]['member_id'] == $select_val)
        {
            $selected = 'selected';
        }

        if ($data[$i]['member_type'] == 0)
        {
            $str_member_type = "Gold";
        }
        else
        if ($data[$i]['member_type'] == 1)
        {
            $str_member_type = "Silver";
        }
        else
        if ($data[$i]['member_type'] == 2)
        {
            $str_member_type = "Gold Lite";
        }
        else
        if ($data[$i]['member_type'] == 3)
        {
            $str_member_type = "Coach on Call";
        }
        else
        if ($data[$i]['member_type'] == 4)
        {
            $str_member_type = "Gold Coach";
        }
        else
        if ($data[$i]['member_type'] == 5)
        {
            $str_member_type = "CSI";
        }

        $str_value = sprintf("[#%s] - %s %s (%s)", $data[$i]['member_id'], $data[$i]['first_name'], $data[$i]['last_name'], $str_member_type);
//        $str_value = sprintf("[#:a] - :b :c (:d)",
//            array(':a'=>$data[$i]['member_id'],':b'=>$data[$i]['first_name'],
//            ':c'=>$data[$i]['last_name'],':d'=>$str_member_type));
            $content.= '<option value="' . $data[$i]['member_id'] . '" ' . $selected . ' >' . $str_value . '</option>';
    }

    $content.= "</select>";
    return $content;
}

function draw_select_refund($name, $select_val, $attributes = '')
{
    $data = array(
        '0' => t('Default') ,
        '1' => t('Refunded') ,
        '-1' => t('*Refund')
    );
    $content = "<select name=\"$name\" $attributes>";
    foreach($data as $key => $item)
    {
        $selected = '';
        if ($key == $select_val)
        {
            $selected = 'selected';
        }

        $content.= '<option value="' . $key . '" ' . $selected . ' >' . $item . '</option>';
    }

    $content.= "</select>";
    return $content;
}

/**
 * Validate a standard transaction data
 */

function standard_transaction_validation($data)
{
    $ret_val = TRUE;
    /*if ($data['amount'] < 0) {
    $ret_val = FALSE;
    drupal_set_message(t('Amount should be positive'), 'error');
    } else*/
    /*if ($data['amount'] == 0) {
    $ret_val = FALSE;
    drupal_set_message(t('Amount is zero.'), 'error');
    } else {*/
    if ($data['refund'] < 0)
    {
        if ($data['amount'] > 0)
        {
            $ret_val = FALSE;
            drupal_set_message(t('Amount should be negative for refund transaction') , 'error');
        }
    }
    else
    {
        if ($data['amount'] < 0)
        {
            $ret_val = FALSE;
            drupal_set_message(t('Amount should be positive for transaction') , 'error');
        }
    }

    // }

    if ($data['uid'] == '' && $data['member_id'] == '')
    {
        $ret_val = FALSE;
        drupal_set_message(t('You need to select User ID or Member ID.') , 'error');
    }
    else
    {

        // For Distributor

        if ($data['ptype'] == 'SUF-DF' || $data['ptype'] == 'SUF-DP' || $data['ptype'] == 'SUF-GMSD')
        {
            if ($data['uid'] == '')
            {
                $ret_val = FALSE;
                drupal_set_message(t('You need to select only User ID.') , 'error');
            }
            else
            {
                $_user = user_load($data['uid']);
                if (!is_distributor($_user))
                {
                    $ret_val = FALSE;
                    drupal_set_message(t('You need to select only Distributor User ID for this type.') , 'error');
                }
            }

            if ($data['target_mid'] == '')
            {
                $ret_val = FALSE;
                drupal_set_message(t('You need to select Target Member ID for this type.') , 'error');
            }
        }
        else
        if ($data['uid'] != '')
        {
            $_user = user_load($data['uid']);
            if (!(isset($_user->profile_memberid) && is_member($_user->profile_memberid)))
            {
                $ret_val = FALSE;
                drupal_set_message(t('You need to select only Member User ID for this type.') , 'error');
            }
        }
    }

    list($month, $day, $year) = explode("/", $data['date']);
    list($hour, $min) = explode(":", $data['time']);
    if ((0 < intval($day) && intval($day) <= 31) && (0 < intval($month) && intval($month) <= 12) && (0 < intval($year) && intval($year) <= 2099))
    {
    }
    else
    {
        $ret_val = FALSE;
        drupal_set_message(t('Date is invalid. Date format should be "day/month/year" ') , 'error');
    }

    if ((0 <= intval($hour) && intval($hour) < 24) && (0 <= intval($min) && intval($min) < 60))
    {
    }
    else
    {
        $ret_val = FALSE;
        drupal_set_message(t('Time is invalid. Time format should be "hour:minute"') , 'error');
    }

    return $ret_val;
}

/**
 * Add a standard transaction(action)
 */

function add_standard_transaction_action($data)
{
    
    $pt_type = get_transaction_type();
    $ptype = '';
    foreach($pt_type as $p_type)
    {
        if ($p_type['product_id'] == $data['product_id'])
        {
            $ptype = $p_type['ptype'];
            break;
        }
    }

    $data['ptype'] = $ptype;
    if (!standard_transaction_validation($data))
    {
        return FALSE;
    }

    list($month, $day, $year) = explode("/", $data['date']);
    list($hour, $min) = explode(":", $data['time']);
    $timestamp = mktime(intval($hour) , intval($min) , 0, intval($month) , intval($day) , intval($year));

    // $timestamp = strtotime( sprintf("%s %s", $data['date'], $data['time']) );

    $uid = 0;
    $member_id = 0;
    if ($data['uid'] != '')
    {
        $_user = user_load(intval($data['uid']));
        $uid = $data['uid'];
        $member_id = isset($_user->profile_memberid)? $_user->profile_memberid:0;
    }
    else
    if ($data['member_id'] != '')
    {
        $uid = get_uid_from_memberid($data['member_id']);
        $member_id = $data['member_id'];
    }

    if ($member_id == '')
    {
        $member_id = 0;
    }

    if ($data['target_mid'] == '')
    {
        $data['target_mid'] = 0;
    }
//
//  $query = "INSERT INTO payment_log VALUES(null, '" . $ptype . "', " . $data['product_id'] . ", " . $data['amount'] . ", $uid, '$member_id',
//
//              '" . $data['target_mid'] . "', " . $data['claim_id'] . ", $timestamp, 1, " . $data['ref_pid'] . ", " . $data['refund'] . ") ";
//
//


    $result = db_query('INSERT INTO {payment_log} VALUES(null,:a,:b,:c,:d,:e,:f,:g,:h,:i,:j,:k)',array(':a'=>$ptype ,':b'=>$data['product_id'],':c'=>$data['amount'],':d'=>$uid,':e'=>$member_id,':f'=>$data['target_mid'],':g'=>$data['claim_id'],':h'=>$timestamp,':i'=>1,':j'=>$data['ref_pid'],':k'=>$data['refund']));
    
    if ($result->rowCount()==0)
    {
        drupal_set_message(t('Failed to insert an standard transaction.') , 'error');
    }

    $query = "SELECT * FROM payment_log WHERE timestamp=$timestamp";
    $result = db_query('SELECT * FROM {payment_log} WHERE timestamp=:a',array(':a'=>$timestamp));
    $t = $result->fetchAssoc();
//  $t = (array)$result;
    $pid = $t['pid'];
    drupal_set_message(t('Standard transaction(#!pid) added, successfully.', array(
        '!pid' => $pid
    )));
    if ($data['refund'] == 0)
    {
        ncn_report_insert_payment_ncn($pid);
    }

    return TRUE;
}

/**
 * Edit a standard transaction(action)
 */

function edit_standard_transaction_action($data)
{
    $pt_type = get_transaction_type();
    $ptype = '';
    foreach($pt_type as $p_type)
    {
        if ($p_type['product_id'] == $data['product_id'])
        {
            $ptype = $p_type['ptype'];
            break;
        }
    }

    $data['ptype'] = $ptype;
    if (!standard_transaction_validation($data))
    {
        return FALSE;
    }

    list($month, $day, $year) = explode("/", $data['date']);
    list($hour, $min) = explode(":", $data['time']);
    $timestamp = mktime(intval($hour) , intval($min) , 0, intval($month) , intval($day) , intval($year));

    // $timestamp = strtotime( sprintf("%s %s", $data['date'], $data['time']) );

    $uid = 0;
    $member_id = 0;
    if ($data['uid'] != '')
    {
        $_user = user_load(intval($data['uid']));
        $uid = $data['uid'];
        $member_id = isset($_user->profile_memberid)?$_user->profile_memberid:0;
    }
    else
    if ($data['member_id'] != '')
    {
        $uid = get_uid_from_memberid($data['member_id']);
        $member_id = $data['member_id'];
    }

    if ($member_id == '') {
        $member_id = 0;
    }

    if ($data['target_mid'] == '')
    {
        $data['target_mid'] = 0;
    }

    $pid = $data['pid'];
//  $query = "UPDATE payment_log SET ptype='" . $ptype . "', product_id=" . $data['product_id'] . ", cost=" . $data['amount'] . ", uid=$uid, member_id='$member_id',
//
//                  target_mid='" . $data['target_mid'] . "', claim_id=" . $data['claim_id'] . ", timestamp=$timestamp, ref_pid=" . $data['ref_pid'] . ", refund=" . $data['refund'] . "
//
//                  WHERE pid=$pid";
    $result = db_query("UPDATE {payment_log} 
                        SET ptype=:a, product_id=:b, cost=:c, uid=:d, member_id=:e, 
                            target_mid=:f, claim_id=:g, timestamp=:h, ref_pid=:i, refund=:j 
                        WHERE pid=:k",
                  array(
                    ':a'=>$ptype ,
                    ':b'=>$data['product_id'],
                    ':c'=>$data['amount'],
                    ':d'=>$uid,
                    ':e'=>$member_id,
                    ':f'=>$data['target_mid'],
                    ':g'=>$data['claim_id'],
                    ':h'=>$timestamp,
                    ':i'=>$data['ref_pid'],
                    ':j'=>$data['refund'],
                    ':k'=>$pid));
    if ($result->rowCount()==0)
    {
        drupal_set_message(t('Failed to update an standard transaction.') , 'error');
    }
    else
    {
        drupal_set_message(t('Standard transaction(#!pid) updated, successfully.', array(
            '!pid' => $pid
        )));
    }
}

/**
 * Validate an additional transaction data
 */

function additional_transaction_validation($data)
{
    $ret_val = TRUE;
    /*if ($data['amount'] < 0) {
    $ret_val = FALSE;
    drupal_set_message(t('Amount should be positive'), 'error');
    } else*/
    /*if ($data['amount'] == 0) {
    $ret_val = FALSE;
    drupal_set_message(t('Amount is zero.'), 'error');
    }*/
    list($month, $day, $year) = explode("/", $data['date']);
    list($hour, $min) = explode(":", $data['time']);
    if ((0 < intval($day) && intval($day) <= 31) && (0 < intval($month) && intval($month) <= 12) && (0 < intval($year) && intval($year) <= 2099))
    {
    }
    else
    {
        $ret_val = FALSE;
        drupal_set_message(t('Date is invalid. Date format should be "day/month/year" ') , 'error');
    }

    if ((0 <= intval($hour) && intval($hour) < 24) && (0 <= intval($min) && intval($hour) < 60))
    {
    }
    else
    {
        $ret_val = FALSE;
        drupal_set_message(t('Time is invalid. Time format should be "hour:minute"') , 'error');
    }

    return $ret_val;
}

/**
 * Add an additional transaction(action)
 */

function add_additional_transaction_action($data)
{
    if (!additional_transaction_validation($data))
    {
        return FALSE;
    }

    list($month, $day, $year) = explode("/", $data['date']);
    list($hour, $min) = explode(":", $data['time']);
    $timestamp = mktime(intval($hour) , intval($min) , 0, intval($month) , intval($day) , intval($year));

    // $timestamp = strtotime( sprintf("%s %s", $data['date'], $data['time']) );

//  $query = "INSERT INTO payment_log VALUES(null, 'ADDITIONAL', 0, " . $data['amount'] . ", 0, '" . $data['member_id'] . "', '0', 0, $timestamp, 1, 0, 0) ";
    $result = db_query('INSERT INTO {payment_log} VALUES(null,:a,:b,:c,:d,:e,:f,:g,:h,:i,:j,:k)',
        array(':a'=>'ADDITIONAL',':b'=>0,':c'=>$data['amount'],':d'=>0,':e'=>$data['member_id'],
            ':f'=>0,':g'=>0,':h'=>$timestamp,':i'=>1,':j'=>0,':k'=>0));
    if ($result->rowCount()==0)
    {
        drupal_set_message(t('Failed to insert an additional transaction.') , 'error');
    }

//  $query = "SELECT * FROM payment_log WHERE timestamp=$timestamp";
    $result = db_query('SELECT * FROM {payment_log} WHERE timestamp=:a',array(':a'=>$timestamp));
    $t = $result->fetchAssoc();
  //  $t = (array)$result;
    $pid = $t['pid'];
    drupal_set_message(t('Additional transaction(#!pid) added, successfully.', array(
        '!pid' => $pid
    )));
    return TRUE;
}

/**
 * Edit an additional transaction(action)
 */

function edit_additional_transaction_action($data)
{
    if (!additional_transaction_validation($data))
    {
        return FALSE;
    }

    list($month, $day, $year) = explode("/", $data['date']);
    list($hour, $min) = explode(":", $data['time']);
    $timestamp = mktime(intval($hour) , intval($min) , 0, intval($month) , intval($day) , intval($year));

    // $timestamp = strtotime( sprintf("%s %s", $data['date'], $data['time']) );

//  $query = "UPDATE payment_log SET cost=" . $data['amount'] . ", member_id='" . $data['member_id'] . "', timestamp=$timestamp WHERE pid=" . $data['pid'];
    $result = db_query('UPDATE {payment_log} SET cost=:a, member_id=:b, timestamp=:c WHERE pid=:d',
    array(':a'=>$data['amount'],':b'=>$data['member_id'],':c'=>$timestamp ,':d'=>$data['pid']));
    if ($result->rowCount()==0)
    {
        drupal_set_message(t('Failed to insert an additional transaction.') , 'error');
    }
    else
    {
        $pid = $data['pid'];
        drupal_set_message(t('Additional transaction(#!pid) added, successfully.', array(
            '!pid' => $pid
        )));
    }

    return TRUE;
}

function ncn_do_payment($member_id, $product_id)
{
    $member_cim = ncn_get_member_cim($member_id);
    if (!$member_cim) {
      return false;
    }
    $customerProfileId = $member_cim['customerProfileId'];
    $customerPaymentProfileId = $member_cim['customerPaymentProfileId'];
    if (intval($customerProfileId) == 0 || intval($customerPaymentProfileId) == 0)
    {
        return true;
    }

    $node = node_load($product_id);
    if (isset($node->data) && !is_array($node->data)) {
      $node->data = unserialize($node->data);
    }
    if ($node->sell_price != 0)
    {
        // attempt payment
        $response = sendNcnPaymentRequest($customerProfileId, $customerPaymentProfileId, 
                      array(
                        'price'         => $node->sell_price, 
                        'shipping_name' => $node->title, 
                        'shipping_desc' => $node->title, 
                        'item_id'       => $product_id, 
                        'item_name'     => $node->title, 
                        'item_desc'     => $node->title, 
                      ));
        $auth_successful = true;
        if ("Ok" == $response->messages->resultCode)
        {
            $auth_successful = true;
            $response_code = 1; // success
        }
        else
        {
            $auth_successful = false;
            $response_code = 3;
            $response_reason_text = preg_replace("/[^a-zA-Z0-9\s\'\.]/", "", $parsedresponse->messages->message->text);
            drupal_set_message($response_reason_text, 'error');
        }
    }
    else
    {
        $auth_successful = true;
    }

    // *** payment log ***//
    $payment_success = ($auth_successful) ? 1 : 0;
    ncn_report_payment_log($product_id, $payment_success, null, $member_id);
    return $auth_successful;
}

//------------------------------------------------------------------------------
function attempt_add_distributor_card()
{
  $date = explode("/", $_POST['profile_credit_card_expiration']);
  
  if (sizeof($date) == 2)
  {     $date = "20".$date[1]."-".$date[0];   }
  else
  {     $date = '';     }

  // add the user to cim
    $content =
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>" .
        "<createCustomerProfileRequest xmlns=\"AnetApi/xml/v1/schema/AnetApiSchema.xsd\">" .
        MerchantAuthenticationBlock().
        "<profile>".
        "<merchantCustomerId>" . get_unique_auth_profile_id() . "</merchantCustomerId>". // Your own identifier for the customer.
        "<description>Distributor: ".$_POST['profile_firstname']." ".$_POST['profile_lastname']."</description>".
        "<email>" . $_POST["mail"] . "</email>".
        "</profile>".
        "</createCustomerProfileRequest>";  // <?php
    
    $response = send_xml_request($content);
    $parsedresponse = parse_api_response($response);
    if ($parsedresponse->messages->resultCode == 'Error') {
        drupal_set_message(t('Could not create user profile'), 'error');
        return false;
    }

    $customerProfileId = $parsedresponse->customerProfileId;
    
  // add a payment profile to cim
$content =
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>" .
    "<createCustomerPaymentProfileRequest xmlns=\"AnetApi/xml/v1/schema/AnetApiSchema.xsd\">" .
    MerchantAuthenticationBlock().
    "<customerProfileId>" . $customerProfileId . "</customerProfileId>".
    "<paymentProfile>".
    "<billTo>".
     "<firstName>".$_POST['profile_firstname']."</firstName>".
     "<lastName>".$_POST['profile_lastname']."</lastName>".
//   "<country>".$_POST['profile_country']."</country>".
     "<address>".$_POST['profile_address']."</address>".
     "<city>".$_POST['profile_city']."</city>".
     "<state>".$_POST['profile_state']."</state>".
     "<zip>".$_POST['profile_zip']."</zip>".
     "<phoneNumber>000-000-0000</phoneNumber>".
    "</billTo>".
    "<payment>".
     "<creditCard>".
      "<cardNumber>".$_POST['profile_credit_card_number']."</cardNumber>".
      "<expirationDate>".$date."</expirationDate>". // required format for API is YYYY-MM
      "<cardCode>".$_POST['profile_credit_card_security']."</cardCode>".
     "</creditCard>".
    "</payment>".
    "</paymentProfile>".
    "<validationMode>liveMode</validationMode>". // or liveMode
    "</createCustomerPaymentProfileRequest>";   // <?php

    $response = send_xml_request($content);
    $parsedresponse = parse_api_response($response);

    // check for cc/details errors
    if ($parsedresponse->messages->resultCode == 'Error')
    {
        $msg = preg_replace("/[^a-zA-Z0-9\s\'\.]/", "", $parsedresponse->messages->message->text);
    
        drupal_set_message($msg, 'error');
        //$form_state['redirect'] = 'apply.html?id='.$_GET['id'];
        return false;
    }

    $customerPaymentProfileId = $parsedresponse->customerPaymentProfileId;
    // add payment/customer profile to database
    // $query = "DELETE FROM member_distributor_cim WHERE username='".addslashes($_POST['name'])."'";
    $result = db_query('DELETE FROM {member_distributor_cim} WHERE username=:a',
        array(':a'=>$_POST['name']));
  
   // $query = "INSERT INTO member_distributor_cim VALUES ( NULL,
   //                                                      '".addslashes($_POST['name'])."',
   //                                                      '".addslashes($customerProfileId)."',
   //                                                      '".addslashes($customerPaymentProfileId)."'
   //                                                      );";
    $result = db_query('INSERT INTO member_distributor_cim VALUES(:a,:b,:c)',
    array(':a'=>$_POST['name'],':b'=>$customerProfileId,':c'=>$customerPaymentProfileId));
    return true;
}

//------------------------------------------------------------------------------
function charge_retail_kit($customerProfileId, $customerPaymentProfileId, $product_id, $member_id, $owner_id)
{
    $node = node_load($product_id);
    if (isset($node->data) && !is_array($node->data)) {
      $node->data = unserialize($node->data);
    }

    if ($node->sell_price != 0) {
        // attempt payment
        $response = sendNcnPaymentRequest($customerProfileId, $customerPaymentProfileId, 
                      array(
                        'price'         => $node->sell_price, 
                        'shipping_name' => $node->title, 
                        'shipping_desc' => $node->title, 
                        'item_id'       => $product_id, 
                        'item_name'     => $node->title, 
                        'item_desc'     => $node->title, 
                      ));        
        $auth_successful = true;
        if ("Ok" == $response->messages->resultCode)
        {
            $auth_successful = true;
            $response_code = 1;     // success
        }
        else
        {
            $auth_successful = false;
            $response_code = 3;
        }
    } else {
        $auth_successful = true;
    }
    //*** payment log ***//
    $payment_success = ($auth_successful)? 1:0;
    ncn_report_payment_log($product_id, $payment_success, null, $member_id, 0, 0, $owner_id );

    return $auth_successful;
}
