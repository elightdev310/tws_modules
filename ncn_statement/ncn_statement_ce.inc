<?php

/**
 * NCN CE Statement
 */
 
// Handler (ce_statement/create)
function ncn_statement_ce_create_handler() {
	$uid = $_REQUEST['ce_uid'];
	$date = $_REQUEST['date'];
	$dest = base64_decode($_REQUEST['dest']);
	
	ncn_statement_ce_create_statement($uid, $date);
	drupal_goto($dest);
}

// Handler (ce_statement/download)
function ncn_statement_ce_download() {
	$filepath = base64_decode(arg(2));
	
	header('Content-Type: application/vnd.openxmlformats-officedocument.spreadsheetml.sheet');
	header('Content-Disposition: attachment;filename="'.basename($filepath).'"');
	header('Cache-Control: max-age=0');
	
	echo file_get_contents($filepath);
	exit;
}

// Handler (ce_statement/delete)
function ncn_statement_ce_delete() {
	$statement_id = arg(2);
	$result = db_query('SELECT * FROM {ncn_ce_statement} WHERE statement_id=:d',array(':d'=>$statement_id));

    foreach($result as $statement)
    {
        $statement = (array)$statement;
        if($statement)
        {

            unlink($statement['filepath']);
            $result = db_query('DELETE FROM {ncn_ce_statement} WHERE id=:d',array(':d'=>$statement['id']));
            if ($result) {
                drupal_set_message(t('Deleted statement(#!id), successfully.', array('!id'=>$statement_id)));
            } else {
                drupal_set_message(t('Failed to delete statement(#!id).', array('!id'=>$statement_id)), 'error');
            }
        }
        $dest = base64_decode($_REQUEST['dest']);
        drupal_goto($dest);

    }
}

//    if ($statement = db_fetch_array($result)) {
//		unlink($statement['filepath']);
//		$result = db_query("DELETE FROM {ncn_ce_statement} WHERE id=%d", $statement['id']);
//		if ($result) {
//			drupal_set_message(t('Deleted statement(#!id), successfully.', array('!id'=>$statement_id)));
//		} else {
//			drupal_set_message(t('Failed to delete statement(#!id).', array('!id'=>$statement_id)), 'error');
//		}
//	}
//
//	$dest = base64_decode($_REQUEST['dest']);
//	drupal_goto($dest);

///////////////////////////////////////////////////////////////////////////////
function ncn_statement_ce_create_statement($uid, $timestamp, $auto=false) {
	$statement = ncn_statement_ce_get_statement($uid, $timestamp);
	$b_recreated = false;
	if (empty($statement)) {
		$statement_id = ncn_statement_ce_get_next_statement_id();
	} else {
		$b_recreated = true;
		$statement_id = $statement['statement_id'];
	}
	
	$dir_url = variable_get('file_directory_path', 'sites/default/files') . "/ce_statement/$uid/";
	
	if (!is_dir($dir_url)) {
		mkdir($dir_url, 0755);
	}
	$filename = 'ce_statement_'.$statement_id.".xlsx";
	$filepath = $dir_url.$filename;
	$ts_statement = ncn_statement_get_this_monday($timestamp);
		
	if (ncn_statement_ce_create_statement_file($statement_id, $uid, $timestamp, $filepath, date('U'), $auto)) {
		if ($b_recreated) {
			$result = db_query('UPDATE {ncn_ce_statement} SET filename=:a, filepath=:b, timestamp=:c WHERE statement_id=:d',
                array(':a'=>$filename,':b'=>$filepath,':c'=>date('U'),':d'=>$statement_id));
			if ($result) {
				drupal_set_message(t('Recreated statement(#!id), successfully.', array('!id'=>$statement_id)));
			} else {
				drupal_set_message(t('Failed to recreate statement(#!id)', array('!id'=>$statement_id)));
			}
		} else {
			$result = db_query('INSERT INTO {ncn_ce_statement}(statement_id, ce_uid, ts_statement, filename, filepath, timestamp)
			VALUES(:a,:b,:c,:d,:e,:f)',array(':a'=>$statement_id,':b'=>$uid,':c'=>$ts_statement,':d'=>$filename,
            ':e'=>$filepath,':f'=>date('U')));
			if ($result) {
				//variable_set('ncn_statement_next_id', $statement_id+1);
			}
			
			if ($result) {
				drupal_set_message(t('Created statement(#!id), successfully.', array('!id'=>$statement_id)));
			} else {
				drupal_set_message(t('Failed to create statement(#!id)', array('!id'=>$statement_id)));
			}
		}	
	}
}

function ncn_statement_ce_get_next_statement_id() {
	$result = db_query('SELECT MAX(statement_id) FROM {ncn_ce_statement}')->fetchField();
	$result += 1;
	if ($result < 1) {
		$result = 1;
	}
	return $result;
}
function ncn_statement_ce_create_statement_file($statement_id, $uid, $report_date, $filepath, $timestamp, $auto=false) {	
	$data = array();
	$total= 0;
	$_user = user_load($uid);
	ncn_ceportal_get_sales_data($report_date, $uid, $data);
	
	if (count($data) == 0) {
		if (!$auto) {
			drupal_set_message(t("You don't need to create statement."), 'warning');
		}
		return FALSE;
	}
	
	$to = strtotime("next monday", $report_date);
	$from = strtotime("last monday", $to);
	$str_period = date("Y/m/d", $from)." ~ ".date("Y/m/d", strtotime("-1 day", $to));
	$total = 0;
	
	foreach($data as $key=>$data_part) {
		$total += $data_part['amount'];
	}
	
	require_once 'sites/all/libraries/php_xls/Classes/PHPExcel.php';
	$demo_file = "ceportal_statement.tpl.xlsx";
	//$objReader = PHPExcel_IOFactory::createReader("Excel2007");
	$objReader = new PHPExcel_Reader_Excel2007();
	$objPHPExcel = $objReader->load($demo_file);
	
	$objPHPExcel->setActiveSheetIndex(0)->setTitle('#'.$statement_id);
	// Gradient
	$objPHPExcel->getActiveSheet()->getStyle('C2')->getFill()->setFillType(PHPExcel_Style_Fill::FILL_GRADIENT_LINEAR);
	$objPHPExcel->getActiveSheet()->getStyle('B2')->getFill()->setFillType(PHPExcel_Style_Fill::FILL_GRADIENT_LINEAR);
	$objPHPExcel->getActiveSheet()->getStyle('H2')->getFill()->setFillType(PHPExcel_Style_Fill::FILL_GRADIENT_LINEAR);
	
	$objPHPExcel->getActiveSheet()->getStyle('C23')->getFill()->setFillType(PHPExcel_Style_Fill::FILL_GRADIENT_LINEAR);
	$objPHPExcel->getActiveSheet()->getStyle('B23')->getFill()->setFillType(PHPExcel_Style_Fill::FILL_GRADIENT_LINEAR);
	$objPHPExcel->getActiveSheet()->getStyle('H23')->getFill()->setFillType(PHPExcel_Style_Fill::FILL_GRADIENT_LINEAR);
	///////////////////////////////////////////////////////////////////////////
	$objPHPExcel->getActiveSheet()->setCellValue("E3" , 'INVOICE #'.$statement_id);
	$objPHPExcel->getActiveSheet()->setCellValue("E4" , 'PERIOD: '.$str_period);
	$objPHPExcel->getActiveSheet()->setCellValue("E5" , strtoupper('DATE: '.date('F d, Y', $timestamp)));
	
	$objPHPExcel->getActiveSheet()->setCellValue("D10" , $_user->profile_firstname.' '.$_user->profile_lastname);
	$objPHPExcel->getActiveSheet()->setCellValue("D11" , $_user->profile_address);
	$objPHPExcel->getActiveSheet()->setCellValue("D12" , $_user->profile_city.', '.$_user->profile_state.' '.$_user->profile_zip);
	
	
	/*$objPHPExcel->getActiveSheet()->setCellValue("D19" , $statement_id);
	$objPHPExcel->getActiveSheet()->setCellValue("D20" , date('m/d/Y', $timestamp));
	$objPHPExcel->getActiveSheet()->setCellValue("D21" , $total);*/
	$objPHPExcel->getActiveSheet()->setCellValue("G16" , $total);
	
	$objPHPExcel->getActiveSheet()->insertNewRowBefore(16, count($data));

	$index = 15;
	foreach($data as $key=>$data_part) {		
		$objPHPExcel->getActiveSheet()->setCellValue("C$index" , $data_part['id']);
		$objPHPExcel->getActiveSheet()->setCellValue("D$index" , $data_part['ce_name']);
		$objPHPExcel->getActiveSheet()->setCellValue("E$index" , $data_part['claim_id']);
		$objPHPExcel->getActiveSheet()->setCellValue("F$index" , date('n/d/Y', $data_part['timestamp']));
		$objPHPExcel->getActiveSheet()->setCellValue("G$index" , $data_part['amount']);
		
		$index += 1;
		
		/*$source = 'A15:I15';
		$dest = "A$index:I$index";
		//$objPHPExcel->getActiveSheet()->duplicateStyle( $objPHPExcel->getActiveSheet()->getStyle($source), $dest );*/
	}
	
	
	
	///////////////////////////////////////////////////////////////////////////
	//$objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, "Excel2007");
	$objWriter = new PHPExcel_Writer_Excel2007($objPHPExcel);
	/*$objWriterPart = $objWriter->getWriterPart('workbook');
	echo "<pre>";
	print_r ($objWriterPart);
	echo "</pre>";*/
	
	
	$objWriter->save($filepath);
	
	return TRUE;
}

function ncn_statement_ce_get_statement($uid, $timestamp) {
	$ts_statement = ncn_statement_get_this_monday($timestamp);
	$result = db_query('SELECT * FROM {ncn_ce_statement} WHERE ts_statement=:a AND ce_uid=:b',
        array(':a'=>$ts_statement,':b'=>$uid));

    foreach($result as $statement)
    {
        $statement = (array)$statement;
        if(!$statement)
        {
            return array();
        }
        return $statement;
    }



//	if ( !($statement=db_fetch_array($result)) ) {
//		return array();
//	}
//	return $statement;
}

function ncn_statement_get_this_monday($timestamp) {
	if ( date('w', $timestamp) == 1 ) {
		$ts = strtotime(date("m/d/Y", $timestamp));
	} else {
		$ts = strtotime("last Monday", $timestamp);
	}
	return $ts;
}

function build_ce_statement_block($report_date, $uid) {
GLOBAL $base_url;

	$ts_statement = ncn_statement_get_this_monday($report_date);
	$sql = "SELECT * FROM {ncn_ce_statement} WHERE ts_statement=".$ts_statement;
	if ($uid) {
		$sql .= " AND ce_uid=".$uid;
	}
	$result = db_query($sql);
	
	$ts_statement = ncn_statement_get_this_monday($report_date);
	$index = 0;
?>
	<div id="invoice_revisions">
	<table>
	<thead class="tableHeader-processed">
		<th>File</th>
		<th>Claim Examiner</th>
		<th>Timestamp</th>
		<th>Operation</th>
	</thead>
	<tbody>
		<?php if($result): ?>
			<?php
         //   while($statement = db_fetch_array($result))
            foreach($result as $statement)
            { ?>

                <?php
                $statement = (array)$statement;
				$_user = user_load($statement['ce_uid']);
				$query = "?ce_uid=".$statement['ce_uid']."&date=".$report_date."&dest=".base64_encode("admin/config/ncn_ceportal/weekly_sales_report/$report_date/$uid");
				$create_url 	= $base_url.base_path()."ce_statement/create".$query;
				$download_url = $base_url.base_path()."ce_statement/download/".base64_encode($statement['filepath']);
				$delete_url 	= $base_url.base_path()."ce_statement/delete/".$statement['id'].$query;
			?>
			<tr class="<?php echo ($index%2)? "even":"odd"; ?>" >
				<td><a href="<?= $download_url; ?>"><?= $statement['filename'] ?></a></td>
				<td><?= $_user->profile_firstname.' '.$_user->profile_lastname ?></td>
				<td><?= date("m/d/Y H:i:s", $statement['timestamp']); ?></td>
				<td>
					<a href="<?= $create_url ?>">Recreate</a>&nbsp;&nbsp;&nbsp;
					<a href="<?= $delete_url ?>" onclick="return confirm('Are you sure you want to delete this statement?');">Delete</a>
				</td>
			</tr>
			<?php 
				$index += 1;
				} ?>
		<?php else: ?>
			<tr>
				<td coslpan=4>No Statement file.</td>
			</tr>
		<?php endif; ?>
	</tbody>
	</table>
	</div>
<?php
}

// call in cron, 10th every month
function ncn_statement_ce_weekly_action() {
	$now = date('U');
	$timestamp = strtotime('-2 days', $now);
	ncn_statement_ce_check_all_statement($timestamp, true);
	
}

function ncn_statement_ce_check_all_statement($timestamp, $auto=false) {
	$ce_list = get_user_list(14);
	
	foreach ($ce_list as $uid=>$data) {
		//$statement = ncn_statement_get_statement($uid, $timestamp);
		ncn_statement_ce_create_statement($uid, $timestamp, $auto);
	}
}

function ncn_statement_ce_all_create_handler() {
	$uid = $_REQUEST['uid'];
	$timestamp = $_REQUEST['date'];
	ncn_statement_ce_check_all_statement($timestamp, true);
	
	$dest = base64_decode($_REQUEST['dest']);
	drupal_goto($dest);
}