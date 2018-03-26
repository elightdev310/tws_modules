<?php

/**

 * Menu Handler (acount/ncn_change_order_request/%)

 */

function ncn_change_order_request($claim_id) {

    $selected_room = '';

    $type = '';

    if (isset($_POST['tfunction']) && $_POST['tfunction']=='cor_action_add_room') {

        $new_room = trim($_POST['new_room_name']);

        if (ncn_change_order_request_handler_add_room($claim_id)) {

            $selected_room = $new_room;

            $type = 'add-only';

        }

    } else if (isset($_POST['tfunction']) && $_POST['tfunction']=='cor_action_remove_room') {

        $r_id = $_POST['r_id'];

        ncn_change_order_request_handler_remove_room($claim_id, $r_id);

    }

    $total_price = ncn_change_order_get_total_price($claim_id);



    $title = t('Change Order Request');



    ob_start();

    ?>

    <div class="pull-right">
    </div>
    <div id="page_loading" class="panel-box">

        <div class="claims-detail panel-box-content">

            <div class="claims-section-wrapper">

                <div id="ncn_change_order_request_page">

                    <div class="clearfix">

                        <div class="title"><?php print $title; ?></div>

                    </div>

                    <div class="clearfix">

                        <div class="cor-instruction">

                            <?php print variable_get('cor_instruction', ''); ?>

                        </div>

                        <div class="content">

                            <div class="claim-info section clearfix">

                                <div style="float: left;">

                                    <span><b>Claim ID: </b><?php print $claim_id; ?></span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;

                                    <span><b>Customer Name: </b><?php print ncn_cd($claim_id, 'customer_name'); ?></span>

                                </div>

                                <div style="float: right;">

                                    <div id='ci_credit_price'><?php print ncn_change_order_render_credit_remained($claim_id); ?></div>

                                </div>

                            </div>

                            <div id="claim-room-section-cor" class="section">

                                <?php print ncn_change_order_render_room_select($claim_id, 'cor_claim_room', $selected_room); ?>

                            </div>



                            <div id="claim-change-order-request-header" class="section">

                                <?php print ncn_change_order_render_request_header($claim_id); ?>

                            </div>

                            <div id="claim-change-order-action-cor" class="section">

                                <?php print ncn_change_order_render_request_action($claim_id, $selected_room, $type); ?>

                            </div>

                            <div id="claim-change-order-request-list-cor" class="section">

                                <?php print ncn_change_order_render_request_list_table($claim_id); ?>

                            </div>

                            <div id="claim-change-order-request-footer" class="section clearfix">

                                <div style="float: left; margin-left: 100px;">

                                    <span id='cor_rh_total_price'><b>Total Price: </b><?php print render_payment_cost($total_price); ?></span>&nbsp;&nbsp;&nbsp;

                                    <span id='cor_rh_invoice_spent_price'>(<?php echo render_payment_cost(ncn_change_order_get_total_price($claim_id, 'invoice')); ?>)</span>

                                </div>

                                <div style="float: right; margin-right: 50px;"><?php print ncn_change_order_render_request_submit($claim_id); ?></div>

                            </div>

                            <div class="section clearfix">

                                <div style='float: right; margin-right: 20px;'>YOUR CARD WILL NOT BE CHARGED UNTIL REVIEWED BY NET CLAIMS NOW and confirmed with you.</div>

                            </div >

                        </div>  <!-- END OF content -->

                    </div>

                </div>

            </div>

        </div>

    </div>

    <?php

    $output = ob_get_contents();
    ob_end_clean();
    drupal_add_js("
        jQuery(document).ready(function() {
            jQuery('#cor_rf_action').bind('change', on_change_action_select);
        });
    ", 'inline');
    return $output;

}



function ncn_change_order_render_request_submit($claim_id) {

    $b_submit = ncn_change_order_available_submit($claim_id);



    $output = "<form id='cor_request_submit_form' method='POST' style='float: right'>";

    $output.= "<input type='button' value='Submit' onclick='on_click_cor_request_submit($claim_id)' ".((!$b_submit)? "disabled='disabled'": '')." />";

    $output.= "</form>";

    return $output;

}



function ncn_change_order_render_room_select($claim_id, $name, $val='', $attr=array()) {

    $rooms = array();
    $result = db_query("SELECT * FROM {claims_data} WHERE claim_id=:d AND `key`='scope'", array(':d'=>$claim_id));

    if ($result->rowCount()>0) {
        $row = $result->fetchAssoc();
        $data = unserialize($row['val']);
        $default_room_name = ncn_admin_claim_get_default_room_name($claim_id);
        foreach ($data as $roomname => $roomdata){
            if ($roomname != "type" && $roomname != $default_room_name){
                $rooms[] = $roomname;
            }
        }
    }

    $output = "<form id='cor_add_room_form' method='POST'>";
    $output.= "<input type='hidden' name='tfunction' value='cor_action_add_room'>";
    $output.= "<input type='hidden' id='cor_arf_claim_id' name='cor_rf_claim_id' value='$claim_id' />";
    $output.= "<select name='$name' id='cor_arf_room_sel' ".drupal_attributes($attr).">";
    $output.= "<option value=''>-Select Room-</option>";
    $roomname = 'Cover Sheet';
    $select = ($val==$roomname)? 'selected' : '';
    $output.= "<option value='".$roomname."' $select>".$roomname."</option>";
    $roomname = 'Miscellaneous';
    $select = ($val==$roomname)? 'selected' : '';
    $output.= "<option value='".$roomname."' $select>".$roomname."</option>";


    foreach ($rooms as $roomname) {
        $select = ($val==$roomname)? 'selected' : '';
        $output.= "<option value='".$roomname."' $select>".$roomname."</option>";
    }

    $output.= "<option value='add_room'>Add a Room</option>";
    $output.= "</select>";
    $output.= "<span id='add_room_section' style='display:none;'>&nbsp;&nbsp;&nbsp;<input type='text' size='30' name='new_room_name' id='new_room_name' />&nbsp;&nbsp;&nbsp;<input type='submit' value='  OK  ' /></span>";
    $output.= "</form>";
    drupal_add_js("
        jQuery(document).ready(function() {
            jQuery('#cor_arf_room_sel').bind('change', on_change_room_select);
        });
        ", 'inline');

    $output.="<form id='cor_remove_room_form' method='POST'>";
    $output.= "<input type='hidden' name='tfunction' value='cor_action_remove_room'>";
    $output.= "<input type='hidden' id='cor_arrf_claim_id' name='claim_id' value='$claim_id' />";
    $output.= "<input type='hidden' id='cor_arrf_r_id' name='r_id' value='' />";
    $output.="</form>";

    return $output;

}



function ncn_change_order_request_handler_add_room($claim_id) {

    $new_room = trim($_POST['new_room_name']);

    $f_data = array(

        'claim_id'=>$claim_id,

        'room_name'=>$new_room,

        'action' => 'add_room',

    );



    $chargable = 1;

    $msg=ncn_change_order_new_room_price_validate($claim_id, $f_data);

    if ($msg !== TRUE) {

        $chargable = 0;

        drupal_set_message($msg, 'warning');

    }



    if (ncn_admin_photo_add_room($claim_id, $new_room)) {

        $result=ncn_change_order_insert_request($f_data, 'add_room', $chargable);

        return $result;

    }



    return FALSE;

}



function ncn_change_order_render_request_header($claim_id) {

    $output ="<table id='cor_rh_table' class='cor-table-style'>";

    $output.="<thead>";

    $output.="</thead>";

    $output.="<tbody>";

    $output.="<tr>

        <td class='action'>Action</td>

        <td class='line'>Line #</td>

        <td class='detail'>Please provide detail</td>

        <td class='ooss'>On Original Scope Sheets</td>

        <td class='price'>Amount</td>

        <td class='paid'>Chargable</td>

        <td class='operation'>Operation</td>

        </tr>

    ";

    $output.="</tbody>";

    $output.="</table>";



    return $output;

}

function ncn_change_order_render_request_action($claim_id, $roomname, $type='', $r_info=array()) {

    if (empty($roomname)) {

        return false;

    }



    $action_type = 'new';

    $r_id = 0;

    if (!empty($r_info) && $r_info['id']) {

        $action_type = 'edit';

        $r_id = $r_info['id'];

    }

    if ($type=='') {

        if (ncn_change_order_is_request_for_new_room($claim_id, $roomname)) {

            $type = 'add-only';

        }

    }



    /** Request Action Form **/

    $output ="<form id='cor_request_form' method='POST' >";

    $output.= "<input type='hidden' name='tfunction' value='cor_action_request' />";



    $output.= "<input type='hidden' name='action_type' value='$action_type' />";

    $output.= "<input type='hidden' name='r_id' value='$r_id' />";

    $output.= "<input type='hidden' id='cor_rf_claim_id' name='cor_rf_claim_id' value='$claim_id' />";

    $output.= "<input type='hidden' id='cor_rf_roomname' name='cor_rf_roomname' value='$roomname' />";



    if ($type == 'add-only') {

        $action_select = "<select id='cor_rf_action' name='cor_rf_action' >";

        $action_select.= "<option value=''>-Select Change-</option>";

        $action_select.= "<option value='add' ".(($r_info['action']=='add')? 'SELECTED':'')." >Add</option>";

        $action_select.= "<select>";

    } else {

        $action_select = "<select id='cor_rf_action' name='cor_rf_action' >";

        $action_select.= "<option value=''>-Select Change-</option>";

        $action_select.= "<option value='add' ".((isset($r_info['action'])&&$r_info['action']=='add')? 'SELECTED':'')." >Add</option>";

        $action_select.= "<option value='change' ".((isset($r_info['action'])&&$r_info['action']=='change')? 'SELECTED':'')." >Change</option>";

        $action_select.= "<option value='delete' ".((isset($r_info['action'])&&$r_info['action']=='delete')? 'SELECTED':'')." >Delete</option>";

        $action_select.= "<select>";

    }



    $output.="<table id='cor_rf_table' class='cor-table-style'>";

    $output.="<thead>";

    $output.="</thead>";

    $output.="<tbody>";

    $output.="

        <tr>

        <td class='action'>$action_select</td>

        <td class='line'    >".(($roomname!='Cover Sheet')?"<input type='text' name='cor_rf_line' id='cor_rf_line' value='".(isset($r_info['line'])?$r_info['line']:'')."' style='".((isset($r_info['action'])&&$r_info['action']=='add')? "display:none":"")."'/>":'')."</td>";

    $output.="<td class='detail'><input type='text' name='cor_rf_detail' id='cor_rf_detail' value='".(isset($r_info['detail'])?$r_info['detail']:'')."' /></td>

        <td class='ooss'    ><input type='checkbox' name='cor_rf_ooss' id='cor_rf_ooss' ".((isset($r_info['ooss'])&&$r_info['ooss'])? 'CHECKED':'')." /></td>

        <td class='price'   ><span id='cor_rf_price'>".(($action_type=='edit')? render_payment_cost(isset($r_info['price'])?$r_info['price']:''):'')."</span></td>

        <td class='paid'></td>

        <td class='operation'>";

    if ($action_type == 'new') {

        $output.= "<input type='button' value='OK' onclick='on_submit_cor_request()' /></td>";

    } else {

        $output.= "<input type='button' value='Save' onclick='on_submit_cor_request()' /></td>";

    }

    $output.= "</tr>";

    $output.="</tbody>";

    $output.="</table>";

    $output.="</form>";



    drupal_add_js("

        jQuery(document).ready(function() {

            jQuery('#cor_rf_ooss').bind('click', on_change_action_select);

        });

    ", 'inline'

    );

    return $output;

}



function ncn_change_order_get_request_action_edit() {


    $r_id = $_REQUEST['r_id'];



    $json = array();

    $json['code'] = 'success';



    $r_info = ncn_change_order_get_request_info($r_id);

    if ($r_info) {

        $claim_id = $r_info['claim_id'];

        $roomname = $r_info['room_name'];

        $json['roomname'] = $roomname;

        /*$json['code'] = 'test';

        $json['msg']    = "$roomname";*/

        $json['form_html'] = ncn_change_order_render_request_action($claim_id, $roomname, '', $r_info);

    } else {

        $json['code'] = 'error';

        $json['msg']    = "Change Order Request (#$r_id) doesn't exist.";

    }



    echo json_encode($json);

    exit;

}



function ncn_change_order_render_request_list_table($claim_id) {

    $data = ncn_change_order_get_request_table_sorted($claim_id);

    /** Request List Table **/

    $output ="<table id='cor_rlist_table' class='cor-table-style'>";

    $output.="<thead>";

    $output.="</thead>";

    $output.="<tbody>";

    $output.="<tr>

        <td class='action'></td>

        <td class='line'></td>

        <td class='detail'></td>

        <td class='ooss'></td>

        <td class='price'></td>

        <td class='paid'></td>

        <td class='operation'></td>

        </tr>

    ";

    foreach ($data as $room_name=>$r_data) {

        $output.="<tr class='tr-room'><td colspan='4'><b>$room_name</b>";

        $price = 0;

        $operation_links = '';

        $paid='';

        $b_new_room = empty($r_data['actions'])? FALSE:TRUE;

        foreach ($r_data['actions'] as $action=>$r_a_data) {

            if ($action=='add_room') {

                $output.=" <span>(new room)</span>";

            }

            $price += $r_a_data['price'];



            if ($r_a_data['price']) {

                $paid="&#x2713;";

                if ($r_a_data['paid']) {

                    if ($r_a_data['credit']) {

                        $paid = '[credit]'; //$paid = '[C-paid]';

                    } else {

                        $paid = '[paid]';

                    }

                } else {

                    /*$paid = "<input type='checkbox' name='pay[$r_id]' id='pay_$r_id'";

                    $paid.= $r_a_data['chargable']? "CHECKED":"" ;

                    $paid.= " />";*/

                }

            } else {

                if (!$r_a_data['chargable']) {

                    $paid = '';

                }

            }



            if (!$r_a_data['paid']) {

                $operation_links = "<a href='#' onclick='on_click_remove_cor_room(".$r_a_data['id'].")'>Remove room</a>";

            }

        }

        $output.="</td>";



        $output.="<td class='price'>".(($b_new_room)? render_payment_cost($price):'')."</td>";

        $output.="<td class='paid'>".$paid."</td>";



        $output.="<td class='operation'>".$operation_links."</td></tr>";



        foreach ($r_data['requests'] as $r_co_data) {

            $paid="";

            if ($r_co_data['price']) {

                $paid="&#x2713;";

                if ($r_co_data['paid']) {

                    if ($r_co_data['credit']) {

                        $paid = '[credit]'; //$paid = '[C-paid]';

                    } else {

                        $paid = '[paid]';

                    }

                } else {

                    /*$paid = "<input type='checkbox' name='pay[$r_id]' id='pay_$r_id'";

                    $paid.= $r_co_data['chargable']? "CHECKED":"" ;

                    $paid.= " />";*/

                }

            } else {

                if (!isset($r_a_data['chargable']) || !$r_a_data['chargable']) {

                    $paid = '';

                }

            }



            $ooss_checked = ($r_co_data['ooss'])? "&#x2713;": '';

            $operation_links = '';

            if (!$r_co_data['paid']) {

                $operation_links = "<a href='#' onclick='on_click_edit_cor_item(".$r_co_data['id'].")'>Edit</a>"; $operation_links.=" | ";

                $operation_links.= "<a href='#' onclick='on_click_remove_cor_item($claim_id, ".$r_co_data['id'].")'>Remove</a>";

            }

            $output.="<tr class='tr-cor'>";

            $output.="<td class='action'><span>".$r_co_data['action']."<span></td>";

            $output.="<td class='line'  >".$r_co_data['line']."</td>";

            $output.="<td class='detail'>".$r_co_data['detail']."</td>";

            $output.="<td class='ooss'  >".$ooss_checked."</td>";

            $output.="<td class='price' >".render_payment_cost($r_co_data['price'])."</td>";

            $output.="<td class='paid'>".$paid."</td>";

            $output.="<td class='operation'>$operation_links</td>";

            $output.="</tr>";

        }

    }



    $output.="</tbody>";

    $output.="</table>";



    return $output;

}





function ncn_change_order_get_request_action() {

    $claim_id = $_REQUEST['claim_id'];

    $roomname = $_REQUEST['roomname'];



    $json = array();

    $json['code'] = 'success';

    $json['form_html'] = ncn_change_order_render_request_action($claim_id, $roomname);



    echo json_encode($json);

    exit;

}



function ncn_change_order_request_submit() {

    $f_data = array();

    $f_data['claim_id']     = isset($_REQUEST['cor_rf_claim_id'])?$_REQUEST['cor_rf_claim_id']:'';
    $f_data['room_name']    = isset($_REQUEST['cor_rf_roomname'])?$_REQUEST['cor_rf_roomname']:'';
    $f_data['action']       = isset($_REQUEST['cor_rf_action'])?$_REQUEST['cor_rf_action']:'';
    $f_data['line']         = isset($_REQUEST['cor_rf_line'])?$_REQUEST['cor_rf_line']:'';
    $f_data['detail']       = isset($_REQUEST['cor_rf_detail'])?$_REQUEST['cor_rf_detail']:'';
    $f_data['ooss']         = isset($_REQUEST['cor_rf_ooss'])?$_REQUEST['cor_rf_ooss']:'';



    $f_data['action_type'] = $_REQUEST['action_type'];

    $f_data['r_id'] = $_REQUEST['r_id'];



    if ($f_data['ooss'] == null) {

        $f_data['ooss'] = 0;

    }



    $json = array();

    $json['code'] = 'success';

    $json['data'] = $f_data;

    $json['action_type'] = $f_data['action_type'];

    // Validate

    if ( ($msg=ncn_change_order_get_request_field_validate($f_data)) !== TRUE ) {

        $json['code'] = 'error-field';

        $json['msg'] = $msg;

    } else {


        $chargable = 0;

        if (($msg=ncn_change_order_request_price_validate($f_data['claim_id'], $f_data['room_name'], $f_data)) === TRUE ) {

            $chargable = 1;

        }



        $result = FALSE;

        if ($f_data['action_type'] == 'new') {
            $result = ncn_change_order_insert_request($f_data, 'change_room', $chargable);
        } else {
            $result = ncn_change_order_update_request($f_data, 'change_room', $chargable);
        }

        if ($result) {

        } else {

            $json['code'] = 'error-db';

            $err_msg = t("Failed to request change order.");

            $json['msg'] = $err_msg;

        }

    }

    $json['total_price'] = render_payment_cost(ncn_change_order_get_total_price($f_data['claim_id']));

    $json['invoice_spent_price'] = "(".render_payment_cost(ncn_change_order_get_total_price($f_data['claim_id'], 'invoice')).")";

    $json['credit_price'] = ncn_change_order_render_credit_remained($f_data['claim_id']);

    echo json_encode($json);

    exit;

}



function ncn_change_order_get_request_field_validate($f_data) {

    $message = '';

    if ($f_data['claim_id'] == '') { $message .= t("Empty Claim ID. Please report to web master.\r\n"); }

    if ($f_data['room_name'] == '') { $message .= t("Empty Room Name. Please report to web master.\r\n"); }

    if ($f_data['action']   == '') { $message .= t("You didn't select Change Type.\r\n"); }



    if ($f_data['room_name'] != 'Cover Sheet' && $f_data['action'] != 'add' && $f_data['line'] == '') { $message .= t("You didn't input Line #.\r\n"); }

    if ($f_data['detail'] == '') { $message .= t("You didn't input Detail #.\r\n"); }



    if ($message == '') return TRUE;



    return $message;

}



function ncn_change_order_get_request_list() {

    $claim_id = $_REQUEST['claim_id'];



    $json = array();

    $json['code'] = 'success';

    $json['list_html'] = ncn_change_order_render_request_list_table($claim_id);

    if (ncn_change_order_available_submit($claim_id)) {

        $json['submit_available'] = 'true';

    } else {

        $json['submit_available'] = 'false';

    }

    echo json_encode($json);

    exit;

}



function ncn_change_order_request_remove($r_id) {

    $json = array();

    $json['code'] = 'success';



    $r_info = ncn_change_order_get_request_info($r_id);



    if ($r_info && ncn_change_order_remove_request($r_id)) {

        $claim_id = $r_info['claim_id'];

        $roomname = $r_info['room_name'];

        $json['claim_id'] = $claim_id;

        $json['roomname'] = $roomname;

        $json['form_html'] = ncn_change_order_render_request_action($claim_id, $roomname, '', $r_info);



        $json['total_price'] = render_payment_cost(ncn_change_order_get_total_price($claim_id));

        $json['invoice_spent_price'] = "(".render_payment_cost(ncn_change_order_get_total_price($claim_id, 'invoice')).")";

        $json['credit_price'] = ncn_change_order_render_credit_remained($claim_id);

    } else {

        $msg = "Failed to remove request (#$r_id)";

        $json['code'] = 'error';

        $json['msg'] = $msg;

    }







    echo json_encode($json);

    exit;

}



function ncn_change_order_available_submit($claim_id) {

    $result = db_query("SELECT COUNT(*) as tot FROM {ncn_change_order} WHERE claim_id=:d AND status>0 ", array(':d'=>$claim_id))->fetchAssoc();
    if ($result['tot']>0) { return TRUE; }

    return FALSE;

}
