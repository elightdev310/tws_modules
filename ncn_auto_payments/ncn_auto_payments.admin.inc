<?php
	/**
 * Page Handler ( admin/config/ncn_admin/declined_report )
 */

	// -- Function Name : ncn_auto_payments_declined_report
	// -- Params : 
	// -- Purpose : 
	function ncn_auto_payments_declined_report()
	{
		$output = "
		<div class='charged-again-report charge-again-section'>
			<h3 class='title'>Credit Card Declined Transaction</h3>
			".ncn_auto_payments_table_declined()."
		</div>
		<div class='charged-again-history charge-again-section'>
			<h3 class='title'>Charged Again, Successfully</h3>
			".ncn_auto_payments_table_charged_successfully()."
		</div>
	";
		return $output;
	}

	

	// -- Function Name : ncn_auto_payments_table_declined
	// -- Params : 
	// -- Purpose : 
	function ncn_auto_payments_table_declined()
	{
		$output = '';
		$header = array(array('data' => 'ID'), array('data' => 'Member Name'), array('data' => 'Member ID'), array('data' => 'Cost'), array('data' => 'Timestamp'), array('data' => 'Last Access'), array('data' => 'Operation'), );
		$data = array();
		$res = db_query("SELECT * FROM {cc_declined} WHERE status=0 ORDER BY timestamp DESC");
		//	while ($dr_info = db_fetch_array($res))
		foreach($res as $dr_info)
		{
			$dr_info = (array)$dr_info;
			$b_blocked = FALSE;
			$access_time = '';
			
			if ($dr_info['user_id'] != 0)
			{
				$_user = user_load($dr_info['user_id']);
				$member_name = $_user->profile_firstname.' '.$_user->profile_lastname;
				$member_id = $_user->profile_memberid;
				
				if ($_user->status==0 && !ncn_admin_is_user_in_training($_user->uid))
				{
					$b_blocked = TRUE;
				}

				$access_time = ($_user->access) ? t('@time ago', array('@time' => format_interval(time() - $_user->access))) :
				t('never');
			}
			else
			if ($dr_info['member_id'] != '0')
			{
				$member = get_member_from_id($dr_info['member_id']);
				
				if ($member)
				{
					$member_name = $member['first_name'].' '.$member['last_name'];
					
					if ($member['status'] == 0)
					{
						$b_blocked = TRUE;
					}

				}

				$member_id = $dr_info['member_id'];
			}

			
			if (!$b_blocked)
			{
				$operation = "<a href='".base_path()."cc_declined/charge/".$dr_info['id']."'>Charge</a>&nbsp;"."<a href='".base_path()."cc_declined/delete/".$dr_info['id']."' onclick='return confirm(\"Are you sure you want to delete this?\");'>Delete</a>";
			}
			else
			{
				$operation = "<span>User Blocked</span>&nbsp;"."<a href='".base_path()."cc_declined/delete/".$dr_info['id']."' onclick='return confirm(\"Are you sure you want to delete this?\");'>Delete</a>";
			}

			$data[] = array($dr_info['id'], $member_name, $member_id, render_payment_cost($dr_info['amount']), date('m/d/Y H:i', $dr_info['timestamp']), $access_time, $operation, );
		}

		$output = theme('table',array("header"=>$header,"rows"=>$data));
		$output .= theme('pager');
		return $output;
	}

	

	// -- Function Name : ncn_auto_payments_table_charged_successfully
	// -- Params : 
	// -- Purpose : 
	function ncn_auto_payments_table_charged_successfully()
	{
		$output = '';
		$header = array(array('data' => 'ID'), array('data' => 'Member Name'), array('data' => 'Member ID'), array('data' => 'Cost'),         array('data' => 'Declined'),array('data' => 'Charged'), array('data' => 'Transaction ID'), );
		$data = array();
		$res = db_query("SELECT * FROM {cc_declined} WHERE status=1 ORDER BY charge_timestamp DESC");
		//	while ($dr_info = db_fetch_array($res))
		foreach($res as $dr_info)
		{
			$dr_info = (array)$dr_info;
			
			if ($dr_info['user_id'] != 0)
			{
				$_user = user_load($dr_info['user_id']);
				$member_name = $_user->profile_firstname.' '.$_user->profile_lastname;
				$member_id = $_user->profile_memberid;
			}
			else
			if ($dr_info['member_id'] != '0')
			{
				$member = get_member_from_id($dr_info['member_id']);
				
				if ($member)
				{
					$member_name = $member['first_name'].' '.$member['last_name'];
				}

				$member_id = $dr_info['member_id'];
			}

			$data[] = array($dr_info['id'],$member_name,$member_id,render_payment_cost($dr_info['amount']),      date('m/d/Y H:i', $dr_info['timestamp']),date('m/d/Y H:i', $dr_info['charge_timestamp']),$dr_info['charge_pid'],);
		}

		$output = theme('table',array("header"=>$header,"rows"=>$data));
		$output .= theme('pager');
		return $output;
	}

	

	// -- Function Name : ncn_auto_payments_insert_declined_transaction
	// -- Params : $product_id, $user_id=305, $member_id='0', $timestamp=305, $amount=305
	// -- Purpose : 
	function ncn_auto_payments_insert_declined_transaction($product_id, $user_id=0, $member_id='0', $timestamp=0, $amount=0)
	{
		
		if ($user_id)
		{
			$_user = user_load($user_id);
			
			if ($_user->profile_memberid)
			{
				$member_id = $_user->profile_memberid;
			}

		}

		$node = node_load($product_id);
		if (isset($node->data) && !is_array($node->data)) {
	      $node->data = unserialize($node->data);
	    }
		
		if ($amount == 0)
		{
			$amount = $node->sell_price;
		}

		$result = db_query("INSERT INTO {cc_declined} (user_id, member_id, product_id, amount, timestamp, status, charge_pid, charge_timestamp) 
        VALUES(:a,:b,:c,:d,:e,:f,:g,:h)",array(":a"=>$user_id,":b"=>$member_id,":c"=>$product_id,":d"=>$amount,":e"=>$timestamp,":f"=>0,":g"=>0,":h"=>0));
		
		if($result)
		{
			return TRUE;
		}

		return FALSE;
	}

	

	// -- Function Name : ncn_auto_payments_get_declined_info
	// -- Params : $id
	// -- Purpose : 
	function ncn_auto_payments_get_declined_info($id)
	{
		$result = db_query("SELECT * FROM {cc_declined} WHERE id=:d",array(":d"=>$id));
		
		if($result)
		{
			foreach($result as $row)
			{
				$row = (array)$row;
				
				if($row)
				{
					return $row;
				}

				return FALSE;
			}

		}

		//
		//
		//
		//	if ($result && $row=db_fetch_array($result)) {
		//
		//		return $row;
		//
		//	}
		//
		//	return FALSE;
	}

	

	// -- Function Name : ncn_auto_payments_declined_report_charge_again
	// -- Params : $id, $charge_pid, $charge_ts
	// -- Purpose : 
	function ncn_auto_payments_declined_report_charge_again($id, $charge_pid, $charge_ts)
	{
		$result = db_query("UPDATE {cc_declined} SET status=1, charge_pid=:d, charge_timestamp=:e WHERE id=:f AND status=0",            array(":d"=>$charge_pid,":e"=>$charge_ts,":f"=>$id));
		
		if ($result)
		{
			return TRUE;
		}

		return FALSE;
	}

	

	// -- Function Name : ncn_auto_payments_charge_again
	// -- Params : $id
	// -- Purpose : 
	function ncn_auto_payments_charge_again($id)
	{
		global $auto_payment_messages;
		$dr_info = ncn_auto_payments_get_declined_info($id);
		
		if ($dr_info)
		{
			
			if ($dr_info['status'] > 0)
			{
				drupal_set_message("This was charged, already. (#$id)", 'warning');
				return FALSE;
			}
			else
			{
				$result = FALSE;
				$charge_pid = 0;
				//$result = TRUE;
				$auto_payment_messages = '';
				
				if ($dr_info['user_id'])
				{
					$_user = user_load($dr_info['user_id']);
					if ($_user)
					{
						$result = make_payment($_user, $dr_info['product_id'], $dr_info['amount'], $charge_pid, 'charge_again');
					}

				}
				else if($dr_info['member_id'] != '0')
				{
					$result = make_payment_from_memberid($dr_info['member_id'], $dr_info['product_id'], $dr_info['amount'], $charge_pid, 'charge_again');
				}
				else
				{
					drupal_set_message("This declined info is incorrect (#$id). Please report to web master.", 'error');
					return FALSE;
				}

				
				if ($result)
				{
					$res_report = ncn_auto_payments_declined_report_charge_again($id, $charge_pid, date('U'));
					drupal_set_message("Charged successfully (#$id). Payment Transaction ID is $charge_pid");
					$auto_payment_messages = '';
				}
				else
				{
					drupal_set_message("Failed to charge again (#$id).", 'error');
					drupal_set_message($auto_payment_messages, 'error');
					$auto_payment_messages = '';
					return FALSE;
				}

			}

		}
		else
		{
			drupal_set_message("This attempt(#$id) has problem. Please report to web master. ", 'error');
			return FALSE;
		}

	}

	

	// -- Function Name : ncn_auto_payments_charge_again_handler
	// -- Params : $id
	// -- Purpose : 
	function ncn_auto_payments_charge_again_handler($id)
	{
		ncn_auto_payments_charge_again($id);
		drupal_goto('admin/config/ncn_admin/declined_report');
	}

	

	// -- Function Name : ncn_auto_payments_cc_declined_delete_handler
	// -- Params : $id
	// -- Purpose : 
	function ncn_auto_payments_cc_declined_delete_handler($id)
	{
		$result = db_query("DELETE FROM {cc_declined} WHERE id=:d",array(":d"=>$id));
		
		if ($result)
		{
			drupal_set_message("Success to delete (#$id).");
		}
		else
		{
			drupal_set_message("Failed to delete (#$id).");
		}

		drupal_goto('admin/config/ncn_admin/declined_report');
	}
