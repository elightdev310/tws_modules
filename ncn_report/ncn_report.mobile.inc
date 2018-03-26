<?php
function ncn_report_mobile_monthly_claims_on_mobile_app() {
  global $base_root,$base_path, $base_url;	
  
  $report_date = 0;
  if ( arg(4) == "" ) {
    $report_date = time();
  } else {
    $report_date = arg(4);
  }
  
  
  $next_month = strtotime("+1 month", $report_date);
  $last_month = strtotime("-1 month", $report_date);
  
  $url = $base_url."/admin/config/ncn_admin/monthly_claims_on_mobile_app/";
  if (isset($_REQUEST['rsd'])) {
    $report_date = strtotime($_REQUEST['rsd']);
    $timezone = variable_get('date_default_timezone', 0);
    $report_date -= $timezone;
    drupal_goto($url.$report_date);
    exit;
  }
  
  $download_url = $base_url."/download/claims_on_mobile/ncn_report/?date=$report_date";
  
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
  <div class="download-report">
    <a href="<?php echo $download_url; ?>">download report</a>
  </div>
  <?php echo ncn_report_insert_date_script(); ?>
</div>
<?php       
  $content = ob_get_contents();
  ob_end_clean();
  
  $total_count = 0;
  $header = array('Claim ID', 'Type', 'Product', 'Customer', 'Zip Code', 'User', 'Created', 'Status');
  $rows = array();
  
  $data = ncn_report_mobile_get_monthly_claims_on_mobile_app_data($report_date);
  foreach ($data as $row) {
    $rows[] = array(
      $row['claim_id'], 
      $row['claim_type'], 
      $row['claim_product'], 
      $row['customer'], 
      $row['zip'], 
      $row['user_name'], 
      $row['claim_created'], 
      $row['claim_status']
    );
  }

  $total_count = count($rows);
  ob_start();
?>
  <div class="report-total">
    <label>Total: </label><span><?php echo $total_count; ?></span>
  </div>
  <div class="report-content">
    <?php
		print theme('table',array('header'=>$header,'rows'=>$rows));
		// Nikhil: Need to check here
        //print theme('pager',NULL,50,0);
		print theme('pager');
    ?>
  </div>
<?php
  $content .= ob_get_contents();  
  ob_end_clean();
  
  return $content;
}

function ncn_report_mobile_get_monthly_claims_on_mobile_app_data($report_date) {
  $from = $to = 0;
  $str = date("Y/m/01", $report_date);
  $from = strtotime($str);
  $to = strtotime("+1 month", $from);
  
  $where = "WHERE app_track!='' and timestamp>=$from AND timestamp<$to";
  $query = "SELECT DISTINCT(claim_id) FROM {claims_log} $where ORDER BY claim_id DESC";  
  $result = db_query($query);
  $rows = array();
  
  //while ($row = db_fetch_array($result)) {
  foreach ($result as $row) 
  {
  	//print_r($row);
	$row = (array)$row;
    $_claim = get_claim_data($row['claim_id']);
    if ($_claim['user_id'] == 69) { continue; }
    
    $claim_type = $_claim['claim_type'];
    $claim_product = $_claim['claim_product'];  
    if ($claim_product=='Fire/Smoke Contents Cleaning') { $claim_product='Contents Cleaning'; }
    if ($claim_product=='Fire/Smoke Structure Cleaning'){ $claim_product='Structure Cleaning'; }
    if (ncn_cd($claim_id, 'expedite')) { $claim_product .= " (EXPEDITE)"; }
    
    $_user = user_load($_claim['user_id']);
      
    $row = array(
      'claim_id' => $row['claim_id'], 
      'claim_type' => ucwords($claim_type), 
      'claim_product' => ucwords($claim_product), 
      'customer' => ncn_cd($row['claim_id'], 'customer_name'), 
      'zip' => ncn_cd($row['claim_id'], 'insured_zip'), 
      'user_name' => $_user->profile_firstname.' '.$_user->profile_lastname, 
      'claim_created' => date("m/d/Y", $_claim['created']), 
      'claim_status' => ucwords($_claim['claim_status']), 
    );
    $rows[] = $row;
  }
  
  return $rows;
}

function ncn_report_download_claims_on_mobile() {
  require_once 'sites/all/libraries/php_xls/Classes/PHPExcel.php';
  
  $demo_file = "claims_on_mobile_report.xls";
  $objReader = PHPExcel_IOFactory::createReader("Excel5");
  $objPHPExcel = $objReader->load($demo_file);
    
  $report_date = intval($_REQUEST['date']);
  if ($report_date == 0) {
    $report_date = time();
  }
  
  $str_date = date("F Y", $report_date);
  $objPHPExcel->setActiveSheetIndex(0)->setCellValue("F3" , "Date: ".$str_date);
  $data = ncn_report_mobile_get_monthly_claims_on_mobile_app_data($report_date);
  
  $count_data = count($data);
  if ($count_data>0) {
    $objPHPExcel->getActiveSheet()->insertNewRowBefore(9, $count_data);  
  }
  for ($i=0; $i<$count_data; $i++) {
    $source = "A7:H7";
    if ($i%2 == 1) {
      $source = "A8:H8";
    }
    $row_index = $i+9;
    $dest = "A$row_index:H$row_index";
    $objPHPExcel->getActiveSheet()->duplicateStyle( $objPHPExcel->getActiveSheet()->getStyle($source), $dest );    
  }
  $objPHPExcel->getActiveSheet()->removeRow(7+$count_data, 2);
  
  $i = 0;
  foreach($data as $key=>$data_part) {
    $row_index = $i+7;
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("A$row_index" , $data_part['claim_id']);
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("B$row_index" , $data_part['claim_type']);
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("C$row_index" , $data_part['claim_product']);
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("D$row_index" , $data_part['customer']);
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("E$row_index" , $data_part['zip']);
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("F$row_index" , $data_part['user_name']);
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("G$row_index" , $data_part['claim_created']);
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("H$row_index" , $data_part['claim_status']);
    $i++;
  }
  
  $report_name = "Claims on Mobile Report ($str_date)";
  
  header('Content-Type: application/vnd.ms-excel');
  header('Content-Disposition: attachment;filename="'.$report_name.'.xls"');
  header('Cache-Control: max-age=0');
  $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5');
  $objWriter->save('php://output');
    
  /*$objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, "Excel5");
  $objWriter->save("daily.xls");*/
  exit;
}
