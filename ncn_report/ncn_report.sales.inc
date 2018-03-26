<?php
function ncn_report_daily() {
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
    
    $url = $base_url."/admin/config/ncn_report/daily/";
    if (isset($_REQUEST['rsd'])) {
        $report_date = strtotime($_REQUEST['rsd']);
        $timezone = variable_get('date_default_timezone', 0);
        $report_date -= $timezone;
        drupal_goto($url.$report_date);
        exit;
    }
    $download_url = $base_url."/download/ncn_report/daily?date=$report_date";
    _ncn_report_get_data($data, $total, $report_date, "daily");
    $best_total = ncn_report_get_best_total_value($total['1'], $report_date, 'daily');
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
        <a href="<?php echo $download_url; ?>">download report</a>
    </div>
</div>
<?php
    build_table($report_date, $data, $total, $best_total, TRUE);
    $content = ob_get_contents();
    ob_end_clean();
    return $content;
}

function ncn_report_weekly() {
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
    
    $url = $base_url."/admin/config/ncn_report/weekly/";
    if (isset($_REQUEST['rsd'])) {
        $report_date = strtotime($_REQUEST['rsd']);
        $timezone = variable_get('date_default_timezone', 0);
        $report_date -= $timezone;
        drupal_goto($url.$report_date);
        exit;
    }
    
    $download_url = $base_url."/download/ncn_report/weekly?date=$report_date";
    
    $to = strtotime("next monday", $report_date);
    $from = strtotime("last monday", $to);
    
    _ncn_report_get_data($data, $total, $report_date, "weekly");
    $best_total = ncn_report_get_best_total_value($total['1'], $report_date, 'weekly');
    
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
        <a href="<?php echo $download_url; ?>">download report</a>
    </div>
</div>
<?php           
    build_table($report_date, $data, $total, $best_total, TRUE);
    $content = ob_get_contents();
    ob_end_clean();
    return $content;
}

function ncn_report_monthly() {
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
    
    $url = $base_url."/admin/config/ncn_report/monthly/";
    if (isset($_REQUEST['rsd'])) {
        $report_date = strtotime($_REQUEST['rsd']);
        $timezone = variable_get('date_default_timezone', 0);
        $report_date -= $timezone;
        drupal_goto($url.$report_date);
        exit;
    }
    $download_url = $base_url."/download/ncn_report/monthly?date=$report_date";
    
    _ncn_report_get_data($data, $total, $report_date, "monthly");
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
        <a href="<?php echo $download_url; ?>">download report</a>
    </div>
</div>
<?php           
    build_table($report_date, $data, $total, $best_total, TRUE);
    $content = ob_get_contents();
    ob_end_clean();
    return $content;
}

function ncn_report_sales_summary() {
    $dist_asso = arg(4);
    
    if ($dist_asso == "") {
        $dist_asso = 'distributor';
    }
    if ($dist_asso == 'distributor') {
        return ncn_report_distributor();
    } else if ($dist_asso == 'associate') {
        return ncn_report_associate();
    }
}

function ncn_report_distributor() {
    $data = array();
    
    $report_date = 0;
    if ( arg(5) == "" ) {
        $report_date = time();
    } else {
        $report_date = arg(5);
    }
    
    
    $next_month = strtotime("+1 month", $report_date);
    $last_month = strtotime("-1 month", $report_date);
    
    global $base_url;
    
    $url_switch = $base_url."/admin/config/ncn_report/sales_summary";
    $url = $base_url."/admin/config/ncn_report/sales_summary/distributor/";
    if (isset($_REQUEST['rsd'])) {
        $report_date = strtotime($_REQUEST['rsd']);
        $timezone = variable_get('date_default_timezone', 0);
        $report_date -= $timezone;
        drupal_goto($url.$report_date);
        exit;
    }
    $download_url = $base_url."/download/distributor/ncn_report/?date=$report_date";
    
    _ncn_report_get_data_distributor($data, $report_date);
    
    ob_start();
?>
<script type="text/javascript">
jQuery(document).ready(function() {
    jQuery("#report_category").bind('change', function() {
        var url = jQuery("#report_category").val();
        window.location = url;
    });
});
</script>
<div class="sales-report-header clear-block column-3">
    <div class="report-category">
        <label for="report_category">Type: </label>
        <select id="report_category">
            <option value="<?php echo "$url_switch/distributor/$report_date"; ?>" selected>Distributor</option>
            <option value="<?php echo "$url_switch/associate/$report_date"; ?>">Associate</option>
        </select>
    </div>
    <div class="report-date">
        <span><a href="<?php echo $url; ?>">[This Month]</a></span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <span><a href="<?php echo $url.$last_month; ?>" title="<?php echo date("F Y", $last_month); ?>">[Last]</a></span>&nbsp;&nbsp;
        <span><input type="text" id="datepicker" value="<?php echo date("F Y", $report_date); ?>" size="15" readonly></span>&nbsp;
        <span><a href="<?php echo $url.$next_month; ?>" title="<?php echo date("F Y", $next_month); ?>">[Next]</a></span>&nbsp;&nbsp;
    </div>
    <input type="hidden" id="report_url" value="<?php echo $url; ?>" />
    <?php echo ncn_report_insert_date_script(); ?>
    <div class="download-report">
        <a href="<?php echo $download_url; ?>">download report</a>
    </div>
</div>
<?php           
    build_table_distributor($data);
    $content = ob_get_contents();
    ob_end_clean();
    return $content;
}

function ncn_report_associate() {
    $data = array();
    
    $report_date = 0;
    if ( arg(5) == "" ) {
        $report_date = time();
    } else {
        $report_date = arg(5);
    }
    
    
    $next_month = strtotime("+1 month", $report_date);
    $last_month = strtotime("-1 month", $report_date);
    
    global $base_url;
    $url_switch = $base_url."/admin/config/ncn_report/sales_summary";
    $url = $base_url."/admin/config/ncn_report/sales_summary/associate/";
    if (isset($_REQUEST['rsd'])) {
        $report_date = strtotime($_REQUEST['rsd']);
        $timezone = variable_get('date_default_timezone', 0);
        $report_date -= $timezone;
        drupal_goto($url.$report_date);
        exit;
    }
    
    $download_url = $base_url."/download/associate/ncn_report/?date=$report_date";
    
    _ncn_report_get_data_associate($data, $report_date);
    
    ob_start();
?>
<script type="text/javascript">
jQuery(document).ready(function() {
    jQuery("#report_category").bind('change', function() {
        var url = jQuery("#report_category").val();
        window.location = url;
    });
});
</script>
<div class="sales-report-header clear-block column-3">
    <div class="report-category">
        <label for="report_category">Type: </label>
        <select id="report_category">
            <option value="<?php echo "$url_switch/distributor/$report_date"; ?>">Distributor</option>
            <option value="<?php echo "$url_switch/associate/$report_date"; ?>" selected>Associate</option>
        </select>
    </div>
    <div class="report-date">
        <span><a href="<?php echo $url; ?>">[This Month]</a></span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <span><a href="<?php echo $url.$last_month; ?>" title="<?php echo date("F Y", $last_month); ?>">[Last]</a></span>&nbsp;&nbsp;
        <span><input type="text" id="datepicker" value="<?php echo date("F Y", $report_date); ?>" size="15" readonly></span>&nbsp;
        <span><a href="<?php echo $url.$next_month; ?>" title="<?php echo date("F Y", $next_month); ?>">[Next]</a></span>&nbsp;&nbsp;
    </div>
    <input type="hidden" id="report_url" value="<?php echo $url; ?>" />
    <?php echo ncn_report_insert_date_script(); ?>
    <div class="download-report">
        <a href="<?php echo $download_url; ?>">download report</a>
    </div>
</div>
<?php           
    build_table_associate($data);
    $content = ob_get_contents();
    ob_end_clean();
    return $content;
}

function ncn_report_sales_detailed() {
    $dist_asso = arg(4);
    
    if ($dist_asso == "") {
        $dist_asso = 'distributor';
    }
    if ($dist_asso == 'distributor') {
        return ncn_report_distributor_detailed();
    } else if ($dist_asso == 'associate') {
        return ncn_report_associate_detailed();
    }
}

function ncn_report_distributor_detailed() {
    $data = array();
    
    $report_date = 0;
    if ( arg(5) == "" ) {
        $report_date = time();
    } else {
        $report_date = arg(5);
    }
    
    
    $next_month = strtotime("+1 month", $report_date);
    $last_month = strtotime("-1 month", $report_date);
    
    $distributor_uid = intval(arg(6));
    $distributor_list = get_user_list(5);
    if (!empty($distributor_list)) {
        if ($distributor_uid == 0 || !array_key_exists($distributor_uid, $distributor_list)) {
            $keys = array_keys($distributor_list);
            $distributor_uid = $keys[0];
        }
    }
    if (array_key_exists($distributor_uid, $distributor_list)) {
        $distributor_list[$distributor_uid]['selected'] = true;
    }
    
    global $base_url;
    
    $url_switch = $base_url."/admin/config/ncn_report/sales_detailed";
    $url = $base_url."/admin/config/ncn_report/sales_detailed/distributor/";
    if (isset($_REQUEST['rsd'])) {
        $report_date = strtotime($_REQUEST['rsd']);
        $timezone = variable_get('date_default_timezone', 0);
        $report_date -= $timezone;
        drupal_goto($url.$report_date);
        exit;
    }
    $download_url = $base_url."/download/distributor/detailed/ncn_report/?date=$report_date&uid=$distributor_uid";
    
    _ncn_report_get_data_distributor_detailed($data, $report_date, $distributor_uid);
    
    if (user_access('ncn statement management')) {
        $statement = ncn_statement_get_statement($distributor_uid, $report_date);
        $statement_url = $base_url."/statement/create?date=$report_date&uid=$distributor_uid";
        $statement_all = $base_url."/statement/all_create?date=$report_date&uid=$distributor_uid";
    }
    
    ob_start();
?>
<script type="text/javascript">
jQuery(document).ready(function() {
    jQuery("#report_category").bind('change', function() {
        var url = jQuery("#report_category").val();
        window.location = url;
    });
    jQuery("#distributor_name").bind('change', function() {
        var url = jQuery("#distributor_name").val();
        window.location = url;
    });
});
</script>
<div class="sales-report-header clear-block column-3 type-a">
    <div class="report-category">
        <label for="report_category">Type: </label>
        <select id="report_category">
            <option value="<?php echo "$url_switch/distributor/$report_date"; ?>" selected>Distributor</option>
            <option value="<?php echo "$url_switch/associate/$report_date"; ?>">Associate</option>
        </select>
    </div>
    <div class="report-date">
        <span><a href="<?php echo $url.time()."/$distributor_uid"; ?>">[This Month]</a></span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <span><a href="<?php echo $url.$last_month."/$distributor_uid"; ?>" title="<?php echo date("F Y", $last_month); ?>">[Last]</a></span>&nbsp;&nbsp;
        <span><input type="text" id="datepicker" value="<?php echo date("F Y", $report_date); ?>" size="15" readonly></span>&nbsp;
        <span><a href="<?php echo $url.$next_month."/$distributor_uid"; ?>" title="<?php echo date("F Y", $next_month); ?>">[Next]</a></span>&nbsp;&nbsp;
    </div>
    <input type="hidden" id="report_url" value="<?php echo $url; ?>" />
    <?php echo ncn_report_insert_date_script(); ?>
    <div class="download-report">
        <?php if (user_access('ncn statement management')): ?>
            <?php if(empty($statement)): ?>
                <a href="<?php echo $statement_url; ?>">create statement</a>
                &nbsp;&nbsp;&nbsp;
            <?php endif; ?>
            <a href="<?php echo $statement_all; ?>">create all statements</a>
            &nbsp;&nbsp;&nbsp;
        <?php endif; ?>
        <a href="<?php echo $download_url; ?>">download report</a>
    </div>
</div>
<div class="sales-report-header-name">
    <label for="distributor_name">Distributor: </label>
    <select id="distributor_name">
        <?php foreach ($distributor_list as $uid=>$dist_user) :?>
        <option value="<?php echo "$url_switch/distributor/$report_date/$uid"; ?>" <?php if(isset($dist_user['selected'])) { echo "selected"; } ?> >
            <?php echo $dist_user['name']; ?>
        </option>
        <?php endforeach; ?>
    </select>
</div>
<?php
    if (user_access('ncn statement management')) {
        build_statement_block($statement, $statement_url);
    }
    build_table_distributor_detailed($data);
    $content = ob_get_contents();
    ob_end_clean();
    return $content;
}

function ncn_report_associate_detailed() {
    $data = array();
    
    $report_date = 0;
    if ( arg(5) == "" ) {
        $report_date = time();
    } else {
        $report_date = arg(5);
    }
    
    
    $next_month = strtotime("+1 month", $report_date);
    $last_month = strtotime("-1 month", $report_date);
    
    $associate_uid = intval(arg(6));
    $associate_list = get_user_list(7);
    if (!empty($associate_list)) {
        if ($associate_uid == 0 || !array_key_exists($associate_uid, $associate_list)) {
            $keys = array_keys($associate_list);
            $associate_uid = $keys[0];
        }
    }
    if (array_key_exists($associate_uid, $associate_list)) {
        $associate_list[$associate_uid]['selected'] = true;
    }
    
    global $base_url;
    
    $url_switch = $base_url."/admin/config/ncn_report/sales_detailed";
    $url = $base_url."/admin/config/ncn_report/sales_detailed/associate/";
    if (isset($_REQUEST['rsd'])) {
        $report_date = strtotime($_REQUEST['rsd']);
        $timezone = variable_get('date_default_timezone', 0);
        $report_date -= $timezone;
        drupal_goto($url.$report_date);
        exit;
    }
    $download_url = $base_url."/download/associate/detailed/ncn_report/?date=$report_date&uid=$associate_uid";
    
    _ncn_report_get_data_associate_detailed($data, $report_date, $associate_uid);
    
    if (user_access('ncn statement management')) {
        $statement = ncn_statement_get_statement($associate_uid, $report_date);
        $statement_url = $base_url."/statement/create?date=$report_date&uid=$associate_uid";
        $statement_all = $base_url."/statement/all_create?date=$report_date&uid=$associate_uid";
    }
    
    ob_start();
?>
<script type="text/javascript">
jQuery(document).ready(function() {
    jQuery("#report_category").bind('change', function() {
        var url = jQuery("#report_category").val();
        window.location = url;
    });
    jQuery("#associate_name").bind('change', function() {
        var url = jQuery("#associate_name").val();
        window.location = url;
    });
});
</script>
<div class="sales-report-header clear-block column-3 type-a">
    <div class="report-category">
        <label for="report_category">Type: </label>
        <select id="report_category">
            <option value="<?php echo "$url_switch/distributor/$report_date"; ?>">Distributor</option>
            <option value="<?php echo "$url_switch/associate/$report_date"; ?>" selected>Associate</option>
        </select>
    </div>
    <div class="report-date">
        <span><a href="<?php echo $url.time()."/$associate_uid"; ?>">[This Month]</a></span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <span><a href="<?php echo $url.$last_month."/$associate_uid"; ?>" title="<?php echo date("F Y", $last_month); ?>">[Last]</a></span>&nbsp;&nbsp;
        <span><input type="text" id="datepicker" value="<?php echo date("F Y", $report_date); ?>" size="15" readonly></span>&nbsp;
        <span><a href="<?php echo $url.$next_month."/$associate_uid"; ?>" title="<?php echo date("F Y", $next_month); ?>">[Next]</a></span>&nbsp;&nbsp;
    </div>
    <input type="hidden" id="report_url" value="<?php echo $url; ?>" />
    <?php echo ncn_report_insert_date_script(); ?>
    <div class="download-report">
        <?php if (user_access('ncn statement management')): ?>
            <?php if(empty($statement)): ?>
                <a href="<?php echo $statement_url; ?>">create statement</a>
                &nbsp;&nbsp;&nbsp;
            <?php endif; ?>
            <a href="<?php echo $statement_all; ?>">create all statements</a>
            &nbsp;&nbsp;&nbsp;
        <?php endif; ?>
        <a href="<?php echo $download_url; ?>">download report</a>
    </div>
</div>
<div class="sales-report-header-name">
    <label for="associate_name">Associate: </label>
    <select id="associate_name">
        <?php foreach ($associate_list as $uid=>$asso_user) :?>
        <option value="<?php echo "$url_switch/associate/$report_date/$uid"; ?>" <?php if(isset($asso_user['selected'])) { echo "selected"; } ?> >
            <?php echo $asso_user['name']; ?>
        </option>
        <?php endforeach; ?>
    </select>
</div>
<?php           
    if (user_access('ncn statement management')) {
        build_statement_block($statement, $statement_url);
    }
    
    build_table_distributor_detailed($data);
    $content = ob_get_contents();
    ob_end_clean();
    return $content;
}

function get_user_list($role_number) {
    $data = array();
    //$result = db_query('SELECT uid FROM users WHERE status=1');
    $sql = "SELECT u.uid FROM users AS u
            JOIN users_roles AS ur ON (ur.uid = u.uid)
            WHERE ur.rid = ".$role_number." AND u.status=1 ";
    $result = db_query($sql);
    if ($result->rowCount()==0) {
        return $data;
    }
    else{
        foreach($result as $row)
        {
            $row = (array)$row;
            $_user = user_load($row['uid']);
            if (!isset($_user->roles[$role_number])) { continue; }
            $_user_name = $_user->profile_legalname; //$_user->profile_firstname." ".$_user->profile_lastname;
            $data[$row['uid']] = array('name'=>$_user_name);
        }
        return $data;
    }
}

///////////////////////////////////////////////////////////////////////////
function build_table($report_date, $data, $total, $best_total='', $grouping=FALSE) {
    $version = ncn_get_version_time($report_date);
?>

<?php  if ($best_total != ''): ?>
<div class="report-best-total">
    <label>Best Total: </label><span><?php echo $best_total; ?></span>
</div>
<?php endif; ?>

<div class="report-content arp">
<?php
if ($grouping) {
    print build_table_grouping($data, $total, $version);
} else {
    print build_table_mail($data, $total, $version);
}
?>
</div>
<?php 
}

function build_table_distributor($data) {
?>
<div class="report-content">
<table>
    <thead>
        <th>Distributor Name</th>
        <th>Start-up Fees (Full)</th>
        <th>Start-up Fees (Partial)</th>
        <th>Start-up Fees (GMS)</th>
        <th>Monthly Membership Fees</th>
        <th>Total Residual Owed</th>
    </thead>
    <tbody>
    <?php 
        $index = 0;
        foreach($data as $key=>$data_part) { 
            $index++;
            $class_tr = "odd";
            if ($index % 2==0) { $class_tr = "even"; }
    ?>
        <tr class="<?php echo $class_tr; ?>">
            <td><?php echo $data_part[0]; ?></td>
            <td><?php echo $data_part[1]; ?></td>
            <td><?php echo $data_part[2]; ?></td>
            <td><?php echo $data_part[3]; ?></td>
            <td><?php echo $data_part[4]; ?></td>
            <td><?php echo render_payment_cost($data_part[5]); ?></td>
        </tr>
    <?php } ?>
    </tbody>
</table>
</div>
<?php 
}

function build_table_associate($data) {
?>
<div class="report-content">
<table>
    <thead>
        <th>Associate Name</th>
        <th>Start-up Fees (Full)</th>
        <th>Start-up Fees (Partial)</th>
        <th>Start-up Fees (GMS)</th>
        <th>Monthly Membership Fees</th>
        <th>Total Start-Up Fees Owed</th>
        <th>Total Residual Owed</th>
    </thead>
    <tbody>
    <?php 
        $index = 0;
        foreach($data as $key=>$data_part) { 
            $index++;
            $class_tr = "odd";
            if ($index % 2==0) { $class_tr = "even"; }
    ?>
        <tr class="<?php echo $class_tr; ?>">
            <td><?php echo $data_part[0]; ?></td>
            <td><?php echo $data_part[1]; ?></td>
            <td><?php echo $data_part[2]; ?></td>
            <td><?php echo $data_part[3]; ?></td>
            <td><?php echo $data_part[4]; ?></td>
            <td><?php echo render_payment_cost($data_part[5]); ?></td>
            <td><?php echo render_payment_cost($data_part[6]); ?></td>
        </tr>
    <?php } ?>
    </tbody>
</table>
</div>
<?php 
}

function build_table_distributor_detailed($data) {
?>
<div class="report-content">
<table>
    <thead>
        <th>Company Name</th>
        <th>Member Name</th>
        <th>Start-up Fee</th>
        <th>Monthly Membership Fee</th>
        <th>Date</th>
        <th>Total Earned</th>
    </thead>
    <tbody>
    <?php 
        $total = 0;
        $index = 0;
        foreach($data as $key=>$data_part) { 
            $index++;
            $class_tr = "odd";
            if ($index % 2==0) { $class_tr = "even"; }
    ?>
        <tr class="<?php echo $class_tr; ?>">
            <td><?php echo $data_part[0]; ?></td>
            <td><?php echo $data_part[1]; ?></td>
            <td><?php echo render_payment_cost($data_part[2]); ?></td>
            <td><?php echo render_payment_cost($data_part[3]); ?></td>
            <td><?php echo date('d M Y H:i:s', $data_part[4]); ?></td>
            <td><?php echo render_payment_cost($data_part[5]); ?></td>
        </tr>
    <?php 
            $total += $data_part[5];
        }
        $index++;
        $class_tr = "odd";
        if ($index % 2==0) { $class_tr = "even"; }
    ?>
        <tr class="<?php echo $class_tr; ?>">
            <td colspan="4"></td>
            <td colspan="2"><div style="font-weight: bold; margin: 5px;"><?php echo t('Total: ').render_payment_cost($total); ?></div></td>
        </tr>
    </tbody>
</table>
</div>
<?php 
}

function _ncn_report_get_data_structure_v1(&$data) {
    $data['SUF-F']  = array(0, 0);
    $data['SUF-P']  = array(0, 0);
    $data['SUF-DF'] = array(0, 0);
    $data['SUF-DP'] = array(0, 0);
    $data['MMF']    = array(0, 0);
    $data['MMF-GL'] = array(0, 0);
    $data['MMF-COC']= array(0, 0);
    $data['MMF-GC'] = array(0, 0);
    $data['MFF']    = array(0, 0);
    
    /* GOLD MEMBER SPECIAL */
    $data['SUF-GMS']    = array(0, 0);
    $data['SUF-GMSD']   = array(0, 0);
    $data['SUF-GMS-1ST']= array(0, 0);
    $data['SUF-G-DEMO'] = array(0, 0);
    $data['SUF-GMAS']   = array(0, 0);
    $data['SUF-GC']     = array(0, 0);
    $data['SUF-COC']    = array(0, 0);
    $data['SUF-GTGCSF'] = array(0, 0);
    $data['SUF-CPR']    = array(0, 0);
    
    /* Claim Gold*/
    $data['RC-G-WATER'] = array(0, 0);
    $data['CC-G-WATER'] = array(0, 0);
    $data['RC-G-FIRE']  = array(0, 0);
    $data['CC-G-FIRE']  = array(0, 0);
    $data['RC-G-MOLD']  = array(0, 0);
    $data['CC-G-MOLD']  = array(0, 0);
    $data['RC-G-RECON'] = array(0, 0);
    $data['CC-G-RECON'] = array(0, 0);
  
    $data['RC-G-WATER-EXPEDITE']   = array(0, 0);
    $data['CC-G-WATER-EXPEDITE']   = array(0, 0);
    $data['RC-G-FIRE-EXPEDITE']    = array(0, 0);
    $data['CC-G-FIRE-EXPEDITE']    = array(0, 0);
    $data['RC-G-MOLD-EXPEDITE']    = array(0, 0);
    $data['CC-G-MOLD-EXPEDITE']    = array(0, 0);
    $data['RC-G-RECON-EXPEDITE']   = array(0, 0);
    $data['CC-G-RECON-EXPEDITE']   = array(0, 0);
    
    $data['RC-G-WATER-WS']  = array(0, 0);
    $data['CC-G-WATER-WS']  = array(0, 0);
    $data['RC-G-FIRE-WS']   = array(0, 0);
    $data['CC-G-FIRE-WS']   = array(0, 0);
    $data['RC-G-MOLD-WS']   = array(0, 0);
    $data['CC-G-MOLD-WS']   = array(0, 0);
    $data['RC-G-RECON-WS']  = array(0, 0);
    $data['CC-G-RECON-WS']  = array(0, 0);

    $data['RC-G-WATER-WS-EXPEDITE']   = array(0, 0);
    $data['CC-G-WATER-WS-EXPEDITE']   = array(0, 0);
    $data['RC-G-FIRE-WS-EXPEDITE']    = array(0, 0);
    $data['CC-G-FIRE-WS-EXPEDITE']    = array(0, 0);
    $data['RC-G-MOLD-WS-EXPEDITE']    = array(0, 0);
    $data['CC-G-MOLD-WS-EXPEDITE']    = array(0, 0);
    $data['RC-G-RECON-WS-EXPEDITE']   = array(0, 0);
    $data['CC-G-RECON-WS-EXPEDITE']   = array(0, 0);
    
    /* Claim Silver*/
    $data['RC-S-WATER']     = array(0, 0);
    $data['CC-S-WATER']     = array(0, 0);
    $data['RC-S-FIRE']      = array(0, 0);
    $data['CC-S-FIRE']      = array(0, 0);
    $data['RC-S-MOLD']      = array(0, 0);
    $data['CC-S-MOLD']      = array(0, 0);
    $data['RC-S-RECON']     = array(0, 0);
    $data['CC-S-RECON']     = array(0, 0);
  
    $data['RC-S-WATER-EXPEDITE']    = array(0, 0);
    $data['CC-S-WATER-EXPEDITE']    = array(0, 0);
    $data['RC-S-FIRE-EXPEDITE']     = array(0, 0);
    $data['CC-S-FIRE-EXPEDITE']     = array(0, 0);
    $data['RC-S-MOLD-EXPEDITE']     = array(0, 0);
    $data['CC-S-MOLD-EXPEDITE']     = array(0, 0);
    $data['RC-S-RECON-EXPEDITE']    = array(0, 0);
    $data['CC-S-RECON-EXPEDITE']    = array(0, 0);
    
    $data['RC-S-WATER-WS']  = array(0, 0);
    $data['CC-S-WATER-WS']  = array(0, 0);
    $data['RC-S-FIRE-WS']   = array(0, 0);
    $data['CC-S-FIRE-WS']   = array(0, 0);
    $data['RC-S-MOLD-WS']    = array(0, 0);
    $data['CC-S-MOLD-WS']   = array(0, 0);
    $data['RC-S-RECON-WS']  = array(0, 0);
    $data['CC-S-RECON-WS']  = array(0, 0);
  
    $data['RC-S-WATER-WS-EXPEDITE']   = array(0, 0);
    $data['CC-S-WATER-WS-EXPEDITE']   = array(0, 0);
    $data['RC-S-FIRE-WS-EXPEDITE']    = array(0, 0);
    $data['CC-S-FIRE-WS-EXPEDITE']    = array(0, 0);
    $data['RC-S-MOLD-WS-EXPEDITE']    = array(0, 0);
    $data['CC-S-MOLD-WS-EXPEDITE']    = array(0, 0);
    $data['RC-S-RECON-WS-EXPEDITE']   = array(0, 0);
    $data['CC-S-RECON-WS-EXPEDITE']   = array(0, 0);
    
    /////////
    $data['RC-G-FIRE-SCC']  = array(0, 0);
    $data['RC-G-FIRE-SSC']  = array(0, 0);
    $data['CC-G-FIRE-SCC']  = array(0, 0);
    $data['CC-G-FIRE-SSC']  = array(0, 0);
  
    $data['RC-G-FIRE-SCC-EXPEDITE']   = array(0, 0);
    $data['RC-G-FIRE-SSC-EXPEDITE']   = array(0, 0);
    $data['CC-G-FIRE-SCC-EXPEDITE']   = array(0, 0);
    $data['CC-G-FIRE-SSC-EXPEDITE']   = array(0, 0);
    
    $data['RC-S-FIRE-SCC']  = array(0, 0);
    $data['RC-S-FIRE-SSC']  = array(0, 0);
    $data['CC-S-FIRE-SCC']  = array(0, 0);
    $data['CC-S-FIRE-SSC']  = array(0, 0);
  
    $data['RC-S-FIRE-SCC-EXPEDITE']   = array(0, 0);
    $data['RC-S-FIRE-SSC-EXPEDITE']   = array(0, 0);
    $data['CC-S-FIRE-SCC-EXPEDITE']   = array(0, 0);
    $data['CC-S-FIRE-SSC-EXPEDITE']   = array(0, 0);
    
    /* Claim Gold Lite*/
    $data['RC-GL-WATER']    = array(0, 0);
    $data['CC-GL-WATER']    = array(0, 0);
    $data['RC-GL-FIRE'] = array(0, 0);
    $data['CC-GL-FIRE'] = array(0, 0);
    $data['RC-GL-FIRE-SCC'] = array(0, 0);
    $data['CC-GL-FIRE-SCC'] = array(0, 0);
    $data['RC-GL-MOLD']  = array(0, 0);
    $data['CC-GL-MOLD']     = array(0, 0);
    $data['RC-GL-RECON']    = array(0, 0);
    $data['CC-GL-RECON']    = array(0, 0);
  
    $data['RC-GL-WATER-EXPEDITE']   = array(0, 0);
    $data['CC-GL-WATER-EXPEDITE']   = array(0, 0);
    $data['RC-GL-FIRE-EXPEDITE']    = array(0, 0);
    $data['CC-GL-FIRE-EXPEDITE']    = array(0, 0);
    $data['RC-GL-FIRE-SCC-EXPEDITE'] = array(0, 0);
    $data['CC-GL-FIRE-SCC-EXPEDITE'] = array(0, 0);
    $data['RC-GL-MOLD-EXPEDITE']    = array(0, 0);
    $data['CC-GL-MOLD-EXPEDITE']    = array(0, 0);
    $data['RC-GL-RECON-EXPEDITE']   = array(0, 0);
    $data['CC-GL-RECON-EXPEDITE']   = array(0, 0);
    
    $data['RC-GL-WATER-WS']     = array(0, 0);
    $data['CC-GL-WATER-WS']     = array(0, 0);
    $data['RC-GL-FIRE-WS']  = array(0, 0);
    $data['CC-GL-FIRE-WS']  = array(0, 0);
    $data['RC-GL-FIRE-SSC']     = array(0, 0);
    $data['CC-GL-FIRE-SSC']     = array(0, 0);
    $data['RC-GL-MOLD-WS']   = array(0, 0);
    $data['CC-GL-MOLD-WS']  = array(0, 0);
    $data['RC-GL-RECON-WS']     = array(0, 0);
    $data['CC-GL-RECON-WS']     = array(0, 0);
  
    $data['RC-GL-WATER-WS-EXPEDITE']    = array(0, 0);
    $data['CC-GL-WATER-WS-EXPEDITE']    = array(0, 0);
    $data['RC-GL-FIRE-WS-EXPEDITE']     = array(0, 0);
    $data['CC-GL-FIRE-WS-EXPEDITE']     = array(0, 0);
    $data['RC-GL-FIRE-SSC-EXPEDITE']    = array(0, 0);
    $data['CC-GL-FIRE-SSC-EXPEDITE']    = array(0, 0);
    $data['RC-GL-MOLD-WS-EXPEDITE']     = array(0, 0);
    $data['CC-GL-MOLD-WS-EXPEDITE']     = array(0, 0);
    $data['RC-GL-RECON-WS-EXPEDITE']    = array(0, 0);
    $data['CC-GL-RECON-WS-EXPEDITE']    = array(0, 0);
    
    /* Claim Coach on Call*/
    $data['RC-COC-WATER']   = array(0, 0);
    $data['CC-COC-WATER']   = array(0, 0);
    $data['RC-COC-FIRE-SCC'] = array(0, 0);
    $data['CC-COC-FIRE-SCC'] = array(0, 0);
    $data['RC-COC-MOLD']     = array(0, 0);
    $data['CC-COC-MOLD']    = array(0, 0);
    $data['RC-COC-RECON']   = array(0, 0);
    $data['CC-COC-RECON']   = array(0, 0);
  
    $data['RC-COC-WATER-EXPEDITE']   = array(0, 0);
    $data['CC-COC-WATER-EXPEDITE']   = array(0, 0);
    $data['RC-COC-FIRE-SCC-EXPEDITE'] = array(0, 0);
    $data['CC-COC-FIRE-SCC-EXPEDITE'] = array(0, 0);
    $data['RC-COC-MOLD-EXPEDITE']   = array(0, 0);
    $data['CC-COC-MOLD-EXPEDITE']   = array(0, 0);
    $data['RC-COC-RECON-EXPEDITE']   = array(0, 0);
    $data['CC-COC-RECON-EXPEDITE']   = array(0, 0);  
    
    $data['RC-COC-WATER-WS']    = array(0, 0);
    $data['CC-COC-WATER-WS']    = array(0, 0);
    $data['RC-COC-FIRE-SSC']    = array(0, 0);
    $data['CC-COC-FIRE-SSC']    = array(0, 0);
    $data['RC-COC-MOLD-WS']  = array(0, 0);
    $data['CC-COC-MOLD-WS']     = array(0, 0);
    $data['RC-COC-RECON-WS']    = array(0, 0);
    $data['CC-COC-RECON-WS']    = array(0, 0);
  
    $data['RC-COC-WATER-WS-EXPEDITE']   = array(0, 0);
    $data['CC-COC-WATER-WS-EXPEDITE']   = array(0, 0);
    $data['RC-COC-FIRE-SSC-EXPEDITE']   = array(0, 0);
    $data['CC-COC-FIRE-SSC-EXPEDITE']   = array(0, 0);
    $data['RC-COC-MOLD-WS-EXPEDITE']   = array(0, 0);
    $data['CC-COC-MOLD-WS-EXPEDITE']   = array(0, 0);
    $data['RC-COC-RECON-WS-EXPEDITE']   = array(0, 0);
    $data['CC-COC-RECON-WS-EXPEDITE']   = array(0, 0);
  
    
    /* Claim Gold Coach*/
    $data['RC-GC-WATER']    = array(0, 0);
    $data['CC-GC-WATER']    = array(0, 0);
    $data['RC-GC-FIRE-SCC'] = array(0, 0);
    $data['CC-GC-FIRE-SCC'] = array(0, 0);
    $data['RC-GC-MOLD']     = array(0, 0);
    $data['CC-GC-MOLD']     = array(0, 0);
    $data['RC-GC-RECON']    = array(0, 0);
    $data['CC-GC-RECON']    = array(0, 0);
  
    $data['RC-GC-WATER-EXPEDITE']   = array(0, 0);
    $data['CC-GC-WATER-EXPEDITE']   = array(0, 0);
    $data['RC-GC-FIRE-SCC-EXPEDITE']= array(0, 0);
    $data['CC-GC-FIRE-SCC-EXPEDITE']= array(0, 0);
    $data['RC-GC-MOLD-EXPEDITE']    = array(0, 0);
    $data['CC-GC-MOLD-EXPEDITE']    = array(0, 0);
    $data['RC-GC-RECON-EXPEDITE']   = array(0, 0);
    $data['CC-GC-RECON-EXPEDITE']   = array(0, 0);
    
    $data['RC-GC-WATER-WS']     = array(0, 0);
    $data['CC-GC-WATER-WS']     = array(0, 0);
    $data['RC-GC-FIRE-SSC']     = array(0, 0);
    $data['CC-GC-FIRE-SSC']     = array(0, 0);
    $data['RC-GC-MOLD-WS']      = array(0, 0);
    $data['CC-GC-MOLD-WS']      = array(0, 0);
    $data['RC-GC-RECON-WS']     = array(0, 0);
    $data['CC-GC-RECON-WS']     = array(0, 0);
  
    $data['RC-GC-WATER-WS-EXPEDITE']    = array(0, 0);
    $data['CC-GC-WATER-WS-EXPEDITE']    = array(0, 0);
    $data['RC-GC-FIRE-SSC-EXPEDITE']    = array(0, 0);
    $data['CC-GC-FIRE-SSC-EXPEDITE']    = array(0, 0);
    $data['RC-GC-MOLD-WS-EXPEDITE']     = array(0, 0);
    $data['CC-GC-MOLD-WS-EXPEDITE']     = array(0, 0);
    $data['RC-GC-RECON-WS-EXPEDITE']    = array(0, 0);
    $data['CC-GC-RECON-WS-EXPEDITE']    = array(0, 0);

    $data['CSI']   = array(0, 0);
  
    //////////////////
    $data['COF']    = array(0, 0);
  
    $data['SUF-AR-SERVICES'] = array(0, 0);
    $data['AR-SERVICES']     = array(0, 0);
    $data['PEER-REVIEWS']    = array(0, 0);
}

function _ncn_report_get_data_structure_v2(&$data) {
    $data['SUF-F']  = array(0, 0);
    $data['SUF-P']  = array(0, 0);
    $data['SUF-DF'] = array(0, 0);
    $data['SUF-DP'] = array(0, 0);
    $data['MMF']    = array(0, 0);
    $data['MMF-SILVER']  = array(0, 0);     //v2
    $data['MMF-PLATINUM']= array(0, 0);     //v2

    $data['SUF-CPR']    = array(0, 0);

    $data['MFF']    = array(0, 0);
    
    /* Claim Gold*/
    $data['RC-G-WATER'] = array(0, 0);
    $data['CC-G-WATER'] = array(0, 0);
    $data['RC-G-FIRE']  = array(0, 0);
    $data['CC-G-FIRE']  = array(0, 0);
    $data['RC-G-MOLD']  = array(0, 0);
    $data['CC-G-MOLD']  = array(0, 0);
    $data['RC-G-RECON'] = array(0, 0);
    $data['CC-G-RECON'] = array(0, 0);
  
    $data['RC-G-WATER-EXPEDITE']   = array(0, 0);
    $data['CC-G-WATER-EXPEDITE']   = array(0, 0);
    $data['RC-G-FIRE-EXPEDITE']    = array(0, 0);
    $data['CC-G-FIRE-EXPEDITE']    = array(0, 0);
    $data['RC-G-MOLD-EXPEDITE']    = array(0, 0);
    $data['CC-G-MOLD-EXPEDITE']    = array(0, 0);
    $data['RC-G-RECON-EXPEDITE']   = array(0, 0);
    $data['CC-G-RECON-EXPEDITE']   = array(0, 0);
    
    $data['RC-G-WATER-WS']  = array(0, 0);
    $data['CC-G-WATER-WS']  = array(0, 0);
    $data['RC-G-FIRE-WS']   = array(0, 0);
    $data['CC-G-FIRE-WS']   = array(0, 0);
    $data['RC-G-MOLD-WS']   = array(0, 0);
    $data['CC-G-MOLD-WS']   = array(0, 0);
    $data['RC-G-RECON-WS']  = array(0, 0);
    $data['CC-G-RECON-WS']  = array(0, 0);

    $data['RC-G-WATER-WS-EXPEDITE']   = array(0, 0);
    $data['CC-G-WATER-WS-EXPEDITE']   = array(0, 0);
    $data['RC-G-FIRE-WS-EXPEDITE']    = array(0, 0);
    $data['CC-G-FIRE-WS-EXPEDITE']    = array(0, 0);
    $data['RC-G-MOLD-WS-EXPEDITE']    = array(0, 0);
    $data['CC-G-MOLD-WS-EXPEDITE']    = array(0, 0);
    $data['RC-G-RECON-WS-EXPEDITE']   = array(0, 0);
    $data['CC-G-RECON-WS-EXPEDITE']   = array(0, 0);
    
    /* Claim Silver*/
    $data['RC-S-WATER']     = array(0, 0);
    $data['CC-S-WATER']     = array(0, 0);
    $data['RC-S-FIRE']      = array(0, 0);
    $data['CC-S-FIRE']      = array(0, 0);
    $data['RC-S-MOLD']      = array(0, 0);
    $data['CC-S-MOLD']      = array(0, 0);
    $data['RC-S-RECON']     = array(0, 0);
    $data['CC-S-RECON']     = array(0, 0);
  
    $data['RC-S-WATER-EXPEDITE']    = array(0, 0);
    $data['CC-S-WATER-EXPEDITE']    = array(0, 0);
    $data['RC-S-FIRE-EXPEDITE']     = array(0, 0);
    $data['CC-S-FIRE-EXPEDITE']     = array(0, 0);
    $data['RC-S-MOLD-EXPEDITE']     = array(0, 0);
    $data['CC-S-MOLD-EXPEDITE']     = array(0, 0);
    $data['RC-S-RECON-EXPEDITE']    = array(0, 0);
    $data['CC-S-RECON-EXPEDITE']    = array(0, 0);
    
    $data['RC-S-WATER-WS']  = array(0, 0);
    $data['CC-S-WATER-WS']  = array(0, 0);
    $data['RC-S-FIRE-WS']   = array(0, 0);
    $data['CC-S-FIRE-WS']   = array(0, 0);
    $data['RC-S-MOLD-WS']    = array(0, 0);
    $data['CC-S-MOLD-WS']   = array(0, 0);
    $data['RC-S-RECON-WS']  = array(0, 0);
    $data['CC-S-RECON-WS']  = array(0, 0);
  
    $data['RC-S-WATER-WS-EXPEDITE']   = array(0, 0);
    $data['CC-S-WATER-WS-EXPEDITE']   = array(0, 0);
    $data['RC-S-FIRE-WS-EXPEDITE']    = array(0, 0);
    $data['CC-S-FIRE-WS-EXPEDITE']    = array(0, 0);
    $data['RC-S-MOLD-WS-EXPEDITE']    = array(0, 0);
    $data['CC-S-MOLD-WS-EXPEDITE']    = array(0, 0);
    $data['RC-S-RECON-WS-EXPEDITE']   = array(0, 0);
    $data['CC-S-RECON-WS-EXPEDITE']   = array(0, 0);

    /* Claim Platinum*/
    $data['RC-P-WATER']     = array(0, 0);
    $data['CC-P-WATER']     = array(0, 0);
    $data['RC-P-FIRE']      = array(0, 0);
    $data['CC-P-FIRE']      = array(0, 0);
    $data['RC-P-MOLD']      = array(0, 0);
    $data['CC-P-MOLD']      = array(0, 0);
    $data['RC-P-RECON']     = array(0, 0);
    $data['CC-P-RECON']     = array(0, 0);
  
    $data['RC-P-WATER-EXPEDITE']    = array(0, 0);
    $data['CC-P-WATER-EXPEDITE']    = array(0, 0);
    $data['RC-P-FIRE-EXPEDITE']     = array(0, 0);
    $data['CC-P-FIRE-EXPEDITE']     = array(0, 0);
    $data['RC-P-MOLD-EXPEDITE']     = array(0, 0);
    $data['CC-P-MOLD-EXPEDITE']     = array(0, 0);
    $data['RC-P-RECON-EXPEDITE']    = array(0, 0);
    $data['CC-P-RECON-EXPEDITE']    = array(0, 0);
    
    $data['RC-P-WATER-WS']  = array(0, 0);
    $data['CC-P-WATER-WS']  = array(0, 0);
    $data['RC-P-FIRE-WS']   = array(0, 0);
    $data['CC-P-FIRE-WS']   = array(0, 0);
    $data['RC-P-MOLD-WS']    = array(0, 0);
    $data['CC-P-MOLD-WS']   = array(0, 0);
    $data['RC-P-RECON-WS']  = array(0, 0);
    $data['CC-P-RECON-WS']  = array(0, 0);
  
    $data['RC-P-WATER-WS-EXPEDITE']   = array(0, 0);
    $data['CC-P-WATER-WS-EXPEDITE']   = array(0, 0);
    $data['RC-P-FIRE-WS-EXPEDITE']    = array(0, 0);
    $data['CC-P-FIRE-WS-EXPEDITE']    = array(0, 0);
    $data['RC-P-MOLD-WS-EXPEDITE']    = array(0, 0);
    $data['CC-P-MOLD-WS-EXPEDITE']    = array(0, 0);
    $data['RC-P-RECON-WS-EXPEDITE']   = array(0, 0);
    $data['CC-P-RECON-WS-EXPEDITE']   = array(0, 0);
    
    /////////
    $data['RC-G-FIRE-SCC']  = array(0, 0);
    $data['RC-G-FIRE-SSC']  = array(0, 0);
    $data['CC-G-FIRE-SCC']  = array(0, 0);
    $data['CC-G-FIRE-SSC']  = array(0, 0);
  
    $data['RC-G-FIRE-SCC-EXPEDITE']   = array(0, 0);
    $data['RC-G-FIRE-SSC-EXPEDITE']   = array(0, 0);
    $data['CC-G-FIRE-SCC-EXPEDITE']   = array(0, 0);
    $data['CC-G-FIRE-SSC-EXPEDITE']   = array(0, 0);
    
    $data['RC-S-FIRE-SCC']  = array(0, 0);
    $data['RC-S-FIRE-SSC']  = array(0, 0);
    $data['CC-S-FIRE-SCC']  = array(0, 0);
    $data['CC-S-FIRE-SSC']  = array(0, 0);
  
    $data['RC-S-FIRE-SCC-EXPEDITE']   = array(0, 0);
    $data['RC-S-FIRE-SSC-EXPEDITE']   = array(0, 0);
    $data['CC-S-FIRE-SCC-EXPEDITE']   = array(0, 0);
    $data['CC-S-FIRE-SSC-EXPEDITE']   = array(0, 0);

    $data['RC-P-FIRE-SCC']  = array(0, 0);
    $data['RC-P-FIRE-SSC']  = array(0, 0);
    $data['CC-P-FIRE-SCC']  = array(0, 0);
    $data['CC-P-FIRE-SSC']  = array(0, 0);
  
    $data['RC-P-FIRE-SCC-EXPEDITE']   = array(0, 0);
    $data['RC-P-FIRE-SSC-EXPEDITE']   = array(0, 0);
    $data['CC-P-FIRE-SCC-EXPEDITE']   = array(0, 0);
    $data['CC-P-FIRE-SSC-EXPEDITE']   = array(0, 0);

    //////////////////
    $data['COF']    = array(0, 0);
  
    $data['SUF-AR-SERVICES'] = array(0, 0);
    $data['AR-SERVICES']     = array(0, 0);
    $data['PEER-REVIEWS']    = array(0, 0);
}

function _ncn_report_get_data(&$data, &$total, $report_date, $type) {
    $version = ncn_get_version_time($report_date);

    if ($version == '1.0') {
        _ncn_report_get_data_structure_v1($data);
    } else {
        _ncn_report_get_data_structure_v2($data);
    }

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
    //echo date("Y/m/d H:i:s", $from);
    //echo date("Y/m/d H:i:s", $to);
    
    $where = " WHERE status=1 AND refund=0 AND timestamp>=$from AND timestamp<$to ";

    //echo $where;
    $total[0] = $total[1] = 0;
    foreach ($data as $key=>&$data_part) {

        if (strpos($key, '-EXPEDITE')) {
          $org_key = substr($key, 0, -9);
          $data_part[0] = _ncn_report_get_total_for_claim($where." AND payment_log.ptype='$org_key'", 1);
          $data_part[1] = _ncn_report_get_total_revenue_for_claim($where." AND payment_log.ptype='$org_key'", 1);
        } else if(isset($data[$key.'-EXPEDITE'])) {
          $data_part[0] = _ncn_report_get_total_for_claim($where." AND payment_log.ptype='$key'", 0);
          $data_part[1] = _ncn_report_get_total_revenue_for_claim($where." AND payment_log.ptype='$key'", 0);
        }else if($key=='CSI') {
            /*from here add user id condition here for csi member and remove ptype*/
            /* Check Member Type using member type log table */
            $user_query = "SELECT u.uid FROM {users} as u 
                            LEFT JOIN {users_roles} AS ur ON (ur.uid = u.uid) 
                            WHERE ur.rid=:rid";
            $user_result = db_query($user_query, array(':rid'=>USER_ROLE_MEMBER));

            foreach($user_result as $object)
            {
                $object = (array)$object;
                $member_type1 = get_member_type_num($object['uid']);
                if($member_type1==MT_CSI) $csi_mem[]=$object['uid'];
                $array[] = $object;
            }
            if(isset($csi_mem))
            {
                $csi_mem_str=implode(",",$csi_mem);
                $where = " WHERE timestamp>=$from AND timestamp<$to ";
                $data_part[0] = _ncn_report_get_total_for_csi($where." AND payment_log.uid in ($csi_mem_str)", 0);
                $total_for_csi=$data_part[0];
                $data_part[1] = _ncn_report_get_total_revenue_for_csi($where." AND payment_log.uid in ($csi_mem_str)", 0);
                $total_revenue_for_csi=$data_part[1];
            }

        } else {
          $data_part[0] = _ncn_report_get_total($where." AND ptype='$key'");
          $data_part[1] = _ncn_report_get_total_revenue($where." AND ptype='$key'");
        }

        $data_part[0] = intval($data_part[0]);
        $data_part[1] = doubleval($data_part[1]);
        
        $total[0] += $data_part[0];
        $total[1] += $data_part[1];
    }

    if(isset($total_for_csi)) $total[0] = $total[0]-$total_for_csi;
    if(isset($total_revenue_for_csi)) $total[1] = $total[1]-$total_revenue_for_csi;
    
    $where = " WHERE status=1 AND refund< 0 AND timestamp>=$from AND timestamp<$to ";
    $refund[0] = _ncn_report_get_total($where);
    $refund[1] = _ncn_report_get_total_revenue($where);

    $data['REFUND'] = $refund;
    $total[0] += $refund[0];
    $total[1] += $refund[1];
    
    $where = " WHERE status=1 AND refund>=0 AND timestamp>=$from AND timestamp<$to AND ptype='ADDITIONAL'";
    $additional[0] = _ncn_report_get_total($where);
    $additional[1] = _ncn_report_get_total_revenue($where);

    $data['ADDITIONAL'] = $additional;
    $total[0] += $additional[0];
    $total[1] += $additional[1];
}

function _ncn_report_get_total($where) {
    $query = "SELECT COUNT(*) as total FROM payment_log " . $where;
    $result = db_query($query);
    if ($result!=FALSE && $result->rowCount()>0) {
        $row = $result->fetchAssoc();
        return $row['total'];
    }
    return 0;
}

function _ncn_report_get_total_revenue($where) {
    $query = "SELECT SUM(cost) as total FROM payment_log " . $where;
    $result = db_query($query);
    if ($result!=FALSE && $result->rowCount()>0) {
        $row = $result->fetchAssoc();
        return $row['total'];
    }
    return 0;
}
function _ncn_report_get_total_for_csi($where, $expedite=0) {
    //$where.= " AND claims.expedite=$expedite";
    $query = "SELECT COUNT(*) as total FROM payment_log " . $where;
    //echo $query;
    $result = db_query($query);
    if ($result!=FALSE && $result->rowCount()>0) {
        $row = $result->fetchAssoc();
        return $row['total'];
    }
}
function _ncn_report_get_total_revenue_for_csi($where, $expedite=0) {
    //$where.= " AND claims.expedite=$expedite";
    $query = "SELECT SUM(cost) as total FROM payment_log " . $where;
    //echo $query;
    $result = db_query($query);
    if ($result!=FALSE && $result->rowCount()>0) {
        $row = $result->fetchAssoc();
        return $row['total'];
    }
    return 0;
}

function _ncn_report_get_total_for_claim($where, $expedite=0) {
  $where.= " AND claims.expedite=$expedite";
  $query = "SELECT COUNT(*) as total FROM payment_log LEFT JOIN claims ON (payment_log.claim_id=claims.claim_id) " . $where;
    //echo $query;
  $result = db_query($query);
  if ($result!=FALSE && $result->rowCount()>0) {
    $row = $result->fetchAssoc();
    return $row['total'];
  }
}

function _ncn_report_get_total_revenue_for_claim($where, $expedite=0) {
  $where.= " AND claims.expedite=$expedite";
  $query = "SELECT SUM(cost) as total FROM payment_log LEFT JOIN claims ON (payment_log.claim_id=claims.claim_id) " . $where;
  $result = db_query($query);
  if ($result!=FALSE && $result->rowCount()>0) {
    $row = $result->fetchAssoc();
    return $row['total'];
  }
  return 0;
}

function ncn_report_company_sales_summary() {
    $data = array();
    
    $report_date = 0;
    if ( arg(4) == "" ) {
        $report_date = time();
    } else {
        $report_date = arg(4);
    }
    
    
    $next_month = strtotime("+1 month", $report_date);
    $last_month = strtotime("-1 month", $report_date);
    
    global $base_url;
    
    $url_switch = $base_url."/admin/config/ncn_report/company_sales_summary";
    $url = $base_url."/admin/config/ncn_report/company_sales_summary/";
    if (isset($_REQUEST['rsd'])) {
        $report_date = strtotime($_REQUEST['rsd']);
        $timezone = variable_get('date_default_timezone', 0);
        $report_date -= $timezone;
        drupal_goto($url.$report_date);
        exit;
    }
    $download_url = $base_url."/download/company_sales_summary/ncn_report/?date=$report_date";
    
    _ncn_report_get_data_company_sales($data, $report_date);
    $total = 0;
    foreach ($data as $key=>$row) {
        $total += $row[5];
    }
    ob_start();
?>
<div class="sales-report-header clear-block column-2">
    <div class="report-date">
        <span><a href="<?php echo $url; ?>">[This Month]</a></span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <span><a href="<?php echo $url.$last_month; ?>" title="<?php echo date("F Y", $last_month); ?>">[Last]</a></span>&nbsp;&nbsp;
        <span><input type="text" id="datepicker" value="<?php echo date("F Y", $report_date); ?>" size="15" readonly></span>&nbsp;
        <span><a href="<?php echo $url.$next_month; ?>" title="<?php echo date("F Y", $next_month); ?>">[Next]</a></span>&nbsp;&nbsp;
    </div>
    <input type="hidden" id="report_url" value="<?php echo $url; ?>" />
    <?php echo ncn_report_insert_date_script(); ?>
    <div class="download-report">
        <a href="<?php echo $download_url; ?>">download report</a>
    </div>
</div>
<?php           
    build_table_company_sales($data, $total);
    $content = ob_get_contents();
    ob_end_clean();
    return $content;
}

function _ncn_report_get_data_company_sales(&$data, $report_date) {
    $query = "SELECT * FROM member_id_pool";
    $result = db_query($query);
    if (!$result) {
        return;
    }
    
    $user_count = $result->rowCount();
    for ($i=0; $i<$user_count; $i++) {
        $row = $result->fetchAssoc();
        //$_user = user_load($row['uid']);
        //if (!isset($_user->roles[8])) { continue; }
        $_user_name = $row['legalname']." (". $row['first_name'].' '.$row['last_name'].")";
        $data[$row['member_id']] = array($_user_name, 0, 0, 0, 0, 0, 0);    //(Company Name, StartUp Fee, Monthly Membership Fee, RC, CC, Total, *remained)
    }
    
    //--------------------------------------------------------
    $str = date("Y/m/01", $report_date);
    $from = strtotime($str);
    $to = strtotime("+1 month", $from);
    
    $query = "SELECT * FROM payment_log WHERE status=1 AND timestamp>=$from AND timestamp<$to ";
    $where_ext = " ";
    $result = db_query($query.$where_ext);
    if (!$result) {
        return;
    }
    $log_count = $result->rowCount();
    
    for ($i=0; $i<$log_count; $i++) {
        $row = $result->fetchAssoc();
        $key = 0;
        if ($row['ptype'] == 'SUF-DF' || $row['ptype'] == 'SUF-DP' || $row['ptype'] == 'SUF-GMSD') {
            //$key = get_uid_from_memberid($row['target_mid']);
            $key = $row['target_mid'];
        } else if ($row['ptype'] == 'SUF-F' || $row['ptype'] == 'SUF-P' || $row['ptype'] == 'SUF-GMS' ) {   // uid means user id of member    //*
            //$key = get_uid_from_memberid($row['member_id']);
            $key = $row['member_id'];
        } else {
            $key = $row['member_id'];
        }
    
        
        if ($key != 0 && isset($data[$key])) {
            $data[$key][6] = 1;
            if ($row['ptype'] == 'SUF-DF' || $row['ptype'] == 'SUF-DP' || $row['ptype'] == 'SUF-GMSD' || 
                    $row['ptype'] == 'SUF-F' || $row['ptype'] == 'SUF-P' || $row['ptype'] == 'SUF-GMS' || $row['ptype'] == 'SUF-GMS-1ST' || $row['ptype'] == 'SUF-GMAS' || $row['ptype'] == 'SUF-GC' || $row['ptype'] == 'SUF-COC' || $row['ptype'] == 'SUF-GTGCSF') {
                $data[$key][1] += $row['cost'];
            } else if ($row['ptype'] == 'MMF' || $row['ptype'] == 'MMF-GL' || $row['ptype'] == 'MMF-COC' || $row['ptype'] == 'MMF-GC') {
                $data[$key][2] += $row['cost'];
            } else if ( strpos($row['ptype'], 'RC-') === 0 ) {
                $data[$key][3] += $row['cost'];
            } else if ( strpos($row['ptype'], 'CC-') === 0 ) {
                $data[$key][4] += $row['cost'];
            }
        }
    }
    
    foreach ($data as $key=>$data_part) {
        
        if ($data[$key][6] == 0) {
            unset( $data[$key] );
        } else {
            $data[$key][5] = $data[$key][1] + $data[$key][2] + $data[$key][3] + $data[$key][4];
        }
    }
}

function get_uid_from_memberid($memberid) {
    /*$query = "SELECT uid FROM users";
    $result = db_query($query);
    $count = mysql_num_rows($result);
    for ($i=0; $i<$count; $i++) {
        $row = mysql_fetch_assoc($result);
        $_user = user_load($row['uid']);
        if ($_user->profile_memberid == $memberid) {
            return $row['uid'];
        }
    }*/
    
    $result = db_query('
            SELECT u.uid FROM {users} u 
            LEFT JOIN {profile_value} pf ON (u.uid = pf.uid AND pf.fid=48)
            LEFT JOIN {member_id_pool} mip ON (pf.value = mip.member_id) 
            WHERE mip.member_id =:a',array(':a'=>$memberid));

    if($result)
    {
        foreach($result as $row)
        {
            $row = (array)$row;
            return $row['uid'];
        }
        return 0;
    }

//
//  if ($result && $row = db_fetch_array($result)) {
//      return $row['uid'];
//  }
//
//  return 0;
}

function build_table_company_sales($data, $total) {
?>
<div class="report-content">
<table>
    <thead>
        <th>Company Name</th>
        <th>Start-up Fee</th>
        <th>Monthly Membership Fee</th>
        <th>Residential Claims</th>
        <th>Commercial Claims</th>
        <th>Total</th>
    </thead>
    <tbody>
    <?php 
        $index = 0;
        foreach($data as $key=>$data_part) { 
            $index++;
            $class_tr = "odd";
            if ($index % 2==0) { $class_tr = "even"; }
    ?>
        <tr class="<?php echo $class_tr; ?>">
            <td><?php echo $data_part[0]; ?></td>
            <td><?php echo $data_part[1]; ?></td>
            <td><?php echo $data_part[2]; ?></td>
            <td><?php echo $data_part[3]; ?></td>
            <td><?php echo $data_part[4]; ?></td>
            <td><?php echo render_payment_cost($data_part[5]); ?></td>
        </tr>
    <?php } 
        $class_tr = "odd";  
        
        if (($index+1) % 2==0) { $class_tr = "even"; }
    ?>
        <tr class="<?php echo $class_tr; ?>">
            <td colspan="4"></td>
            <td style="text-align:right;">Total:</td>
            <td>$<?php echo $total; ?></td>
        </tr>
    </tbody>
</table>
</div>
<?php 
}

///////////////////////////////////////////////////////////////////////////////
function ncn_report_download() {
    $type = arg(2);
    require_once 'sites/all/libraries/php_xls/Classes/PHPExcel.php';
    
    $demo_file = "sales_report.xls";
    $objReader = PHPExcel_IOFactory::createReader("Excel5");
    $objPHPExcel = $objReader->load($demo_file);

    //$objPHPExcel = new PHPExcel(); //PHPExcel_IOFactory->load($demo_file);
    
    $data = array();
    $total= array();
    
    $report_date = intval($_REQUEST['date']);
    if ($report_date == 0) {
        $report_date = time();
    }
    
    $str_date = "";
    if ($type == "daily") {
        $str_date = date("M d, Y", $report_date);
    } else if ($type == "weekly") {
        $to = strtotime("next monday", $report_date);
        $from = strtotime("last monday", $to);
        $str_date =  date("M d, Y", $from)." ~ ".date("M d, Y", strtotime("-1 day", $to));
        
    } else if ($type == "monthly") {
        $str_date = date("F Y", $report_date);
    }
    
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("B3" , "Date: ".$str_date);
    
    _ncn_report_get_data($data, $total, $report_date, $type);
    
    foreach ($data as $key=>&$data_part) {
        if ($data_part[0] == 0) {
            $data_part[0] = "";
        }
    }
    
    if ($type == "daily") { $report_title = "Daily Sales Summary Report"; }
    else if ($type == "weekly") { $report_title = "Weekly Sales Summary Report"; }
    else if ($type == "monthly") { $report_title = "Monthly Sales Summary Report"; }
    
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("A5" , $report_title);
    
    $version = ncn_get_version_time($report_date);

    $table_data = ncn_report_get_sale_report_table_data($version);
    $count_data = count($table_data);
    if ($count_data>0) {
    $objPHPExcel->getActiveSheet()->insertNewRowBefore(9, $count_data);  
    }
    for ($i=0; $i<$count_data; $i++) {
        $source = "A7:B7";
        if ($i%2 == 1) {
          $source = "A8:B8";
        }
        $row_index = $i+9;
        $dest = "A$row_index:B$row_index";
        $objPHPExcel->getActiveSheet()->duplicateStyle( $objPHPExcel->getActiveSheet()->getStyle($source), $dest );
        
        $source = "C7";
        if ($i%2 == 1) {
          $source = "C8";
        }
        $dest = "C$row_index";
        
        $objPHPExcel->getActiveSheet()->duplicateStyle( $objPHPExcel->getActiveSheet()->getStyle($source), $dest );
        
    }
    
  $objPHPExcel->getActiveSheet()->removeRow(7+$count_data, 2);
  
  $row = 7;
  foreach ($table_data as $key=>$i_data) {
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("A$row" , $i_data['title']);
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("B$row" , $data[$key][0]);
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("C$row" , $data[$key][1]);
    $row++;
  }
    
  $row_index = $count_data+1+7;
  $objPHPExcel->setActiveSheetIndex(0)->setCellValue("C$row_index" , $total[1]);
  
  
    $report_name = ucfirst($type)." Sales Report ($str_date)";
    
    header('Content-Type: application/vnd.ms-excel');
    header('Content-Disposition: attachment;filename="'.$report_name.'.xls"');
    header('Cache-Control: max-age=0');
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5');
    $objWriter->save('php://output');
        
    /*$objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, "Excel5");
    $objWriter->save("daily.xls");*/
    exit;
}

function ncn_report_get_sale_report_table_data($version) {
  $data = array();
  $group_data = ncn_report_get_grouping_data($version);
  foreach ($group_data as $key=>$g_data) {
    if ($g_data['type'] == 'GROUP') {
      $data = array_merge($data, $g_data['child']);
    } else if ($g_data['type'] == 'ITEM') {
      $data[$key] = $g_data;
    }
  }
  
  return $data;
}

function _ncn_report_get_data_distributor(&$data, $report_date) {   
//  $query = "SELECT uid FROM users WHERE status=1; ";
    $result = db_query('SELECT uid FROM {users} WHERE status=1');
    if (!$result) {
        return;
    }
    
    $user_count = $result->rowCount();
    for ($i=0; $i<$user_count; $i++) {
        $row = $result->fetchAssoc();
        $_user = user_load($row['uid']);
        if (!isset($_user->roles[5])) { continue; }
        $_user_name = $_user->profile_legalname;
        $data[$row['uid']] = array($_user_name, 0, 0, 0, 0, 0); //(Distributor Name, SUF-DF, SUF-DP, SUF-GMSD, MMF, Total Residual Owed)
    }
    
    //--------------------------------------------------------
    $str = date("Y/m/01", $report_date);
    $from = strtotime($str);
    $to = strtotime("+1 month", $from);
    
    $query = "SELECT * FROM payment_log WHERE status=1 AND refund>=0 AND timestamp>=$from AND timestamp<$to ";
    $where_ext = " AND (ptype LIKE 'SUF-%' OR ptype LIKE 'MMF%')";
    $result = db_query($query.$where_ext);
    if (!$result) {
        return;
    }
    $log_count = $result->rowCount();
    for ($i=0; $i<$log_count; $i++) {
        $row = $result->fetchAssoc();
        $key = 0;
        if ($row['ptype'] == 'SUF-DF' || $row['ptype'] == 'SUF-DP' || $row['ptype'] == 'SUF-GMSD' ) {   // uid means user id of distributor
            $key = $row['uid'];
        } else {
            // search for owner of member_id
            $query1 = "SELECT owner FROM member_id_pool WHERE member_id='".$row['member_id']."'";
            $result1 = db_query('SELECT owner FROM {member_id_pool} WHERE member_id=:a',
                array(':a'=>$row['member_id']));
            if ($result1 && $result1->rowCOunt()>0) {
                $row1 = $result1->fetchAssoc();
                $key = $row1['owner'];
            }
        }
        if (isset($data[$key])) {
            $n_count = 1;
            //if ($row['refund'] < 0) { $n_count = -1; } 
            if ($row['ptype'] == 'SUF-F' || $row['ptype'] == 'SUF-DF' || $row['ptype'] == 'SUF-GC' || $row['ptype'] == 'SUF-COC') {
                $data[$key][1] += $n_count;
            } else if ($row['ptype'] == 'SUF-P' || $row['ptype'] == 'SUF-DP') {
                $data[$key][2] += $n_count;
            } else if ($row['ptype'] == 'SUF-GMS' || $row['ptype'] == 'SUF-GMSD' || $row['ptype'] == 'SUF-GMS-1ST' ) {
                $data[$key][3] += $n_count;
            } else if (strpos($row['ptype'], "MMF")===0 && _ncn_report_get_payment_ncn_type($row['ptype'])) {
                $data[$key][4] += $n_count;
            }
        }
    }
    foreach ($data as $key=>$data_part) {
        $_remove = true;
        for ($j=1; $j<5; $j++) {
            if ($data[$key][$j] != 0) {
                $_remove = false; break;
            }
        }
        if ($_remove) {
            unset( $data[$key] );
        } else {
            $data[$key][5] = $data[$key][4] * 10;   // calculate "Total Residual Owed" : $10 Per charging membber monthly fee, 
        }
    }
}

function _ncn_report_get_data_associate(&$data, $report_date) {
//  $query = "SELECT uid FROM users WHERE status=1; ";
    $result = db_query('SELECT uid FROM {users} WHERE status=1');
    if (!$result) {
        return;
    }
    
    $user_count = $result->rowCount();
    for ($i=0; $i<$user_count; $i++) {
        $row = $result->fetchAssoc();
        $_user = user_load($row['uid']);
        if (!isset($_user->roles[7])) { continue; }
        $_user_name = $_user->profile_legalname;
        $data[$row['uid']] = array($_user_name, 0, 0, 0, 0, 0, 0);  //(Associate Name, SUF-F, SUF-P, SUF-GMS, MMF, Total Start-Up Fees Owed, Total Residual Owed)
    }
    
    //--------------------------------------------------------
    $str = date("Y/m/01", $report_date);
    $from = strtotime($str);
    $to = strtotime("+1 month", $from);
    
    $query = "SELECT * FROM payment_log WHERE status=1 AND timestamp>=$from AND timestamp<$to ";
    $where_ext = " AND (ptype LIKE 'SUF-%' OR ptype LIKE 'MMF%')";
    $result = db_query($query.$where_ext);
    if (!$result) {
        return;
    }
    $log_count = $result->rowCount();
    for ($i=0; $i<$log_count; $i++) {
        $row = $result->fetchAssoc();
        
        $key = 0;
        // search for owner of member_id
    //  $query1 = "SELECT owner FROM member_id_pool WHERE member_id='".$row['member_id']."'";
        $result1 = db_query('SELECT owner FROM {member_id_pool} WHERE member_id=:a',array(':a'=>$row['member_id']));
        if ($result1 && $result1->rowCount()>0) {
            $row1 = $result1->fetchAssoc();
            $key = $row1['owner'];
        }

        if (isset($data[$key])) {
            $n_count = 1;
            //if ($row['refund'] < 0) { $n_count = -1; } 
            
            if ($row['ptype'] == 'SUF-F' || $row['ptype'] == 'SUF-GC' || $row['ptype'] == 'SUF-COC') {
                $data[$key][1] += $n_count;
            } else if ($row['ptype'] == 'SUF-P') {
                $data[$key][2] += $n_count;
            } else if ($row['ptype'] == 'SUF-GMS' || $row['ptype'] == 'SUF-GMAS' || $row['ptype'] == 'SUF-GMS-1ST') {
                $data[$key][3] += $n_count;
            } else if (strpos($row['ptype'], "MMF")===0) {
                $data[$key][4] += $n_count;
            }
        }
    }
    foreach ($data as $key=>$data_part) {
        $_remove = true;
        for ($j=1; $j<5; $j++) {
            if ($data[$key][$j] != 0) {
                $_remove = false; break;
            }
        }
        if ($_remove) {
            unset( $data[$key] );
        } else {
            $data[$key][5] = ($data[$key][1]+$data[$key][2]+$data[$key][3]) * 50;   // calculate "Total Start-Up Fees Owed" : $50 Per start-up fee, 
            $data[$key][6] = $data[$key][4] * 10;   // calculate "Total Residual Owed" : $10 Per charging membber monthly fee, 
        }
    }
}

function _ncn_report_get_data_distributor_detailed(&$data, $report_date, $distributor_uid) {

    //--------------------------------------------------------
    $str = date("Y/m/01", $report_date);
    $from = strtotime($str);
    $to = strtotime("+1 month", $from);
    
//  $query = "SELECT * FROM payment_ncn WHERE status=1 AND uid=$distributor_uid AND timestamp>=$from AND timestamp<$to ";
    $result = db_query('SELECT * FROM {payment_ncn} WHERE status=1 AND uid=:a AND timestamp>=:b AND timestamp<:c',
        array(':a'=>$distributor_uid,':b'=>$from,':c'=>$to));
    if (!$result) {
        return;
    }
    $pn_count = $result->rowCount();
    for ($i=0; $i<$pn_count; $i++) {
        $row = $result->fetchAssoc();
        $payment = ncn_report_get_payment_from_pid($row['ref_pid']);
        
        $key = '0';
        if ($payment['ptype'] == 'SUF-DF' || $payment['ptype'] == 'SUF-DP' || $payment['ptype'] == 'SUF-GMSD') {    // uid means user id of distributor
            $key = $payment['target_mid'];
        } else {
            $key = $payment['member_id'];
        }
        
        if ($key != '0') {
            if (!isset($data[$key])) {
                $member = get_member_from_id($key);
                $_user_name = $member['first_name']." ".$member['last_name'];
                $_company_name = $member['legalname'];
                
                $data[$key] = array($_company_name, $_user_name, 0, 0, 0, 0);   //(Company Name, Member Name, Start-Up Fee, Monthly Membership Fee, Date, Total Earned)
            }
            
            $n_count = 1;
            if ($row['type'] == 'SUF' || $row['type'] == 'SUF-GMS-1ST') {
                $data[$key][2] += $row['cost'];//$n_count;
            } else if ($row['type'] == 'MMF') {
                $data[$key][3] += $row['cost'];//$n_count;
            }
            if ($row['cost']) {
                $data[$key][5] += $row['cost'];
            }
            $data[$key][4] = $row['timestamp'];
        }
        
    }
}

function _ncn_report_get_data_associate_detailed(&$data, $report_date, $associate_uid) {
    //--------------------------------------------------------
    $str = date("Y/m/01", $report_date);
    $from = strtotime($str);
    $to = strtotime("+1 month", $from);
    
//  $query = "SELECT * FROM payment_ncn WHERE status=1 AND uid=$associate_uid AND timestamp>=$from AND timestamp<$to ";
    //drupal_set_message($query);
    
    $result = db_query('SELECT * FROM {payment_ncn} WHERE status=1 AND uid=:a AND timestamp>=:b AND timestamp<:c',
        array(':a'=>$associate_uid,':b'=>$from,':c'=>$to));
    if (!$result) {
        return;
    }
    $pn_count = $result->rowCount();
    for ($i=0; $i<$pn_count; $i++) {
        $row = $result->fetchAssoc();
        $payment = ncn_report_get_payment_from_pid($row['ref_pid']);
        
        $key = '0';
        if ($payment['ptype'] == 'SUF-DF' || $payment['ptype'] == 'SUF-DP' || $payment['ptype'] == 'SUF-GMSD') {    // uid means user id of distributor
            $key = $payment['target_mid'];
        } else {
            $key = $payment['member_id'];
        }
        
        if ($key != 0) {
            if (!isset($data[$key])) {
                $member = get_member_from_id($key);
                $_user_name = $member['first_name']." ".$member['last_name'];
                $_company_name = $member['legalname'];
                
                $data[$key] = array($_company_name, $_user_name, 0, 0, 0, 0);   //(Company Name, Member Name, Start-Up Fee, Monthly Membership Fee, Date, Total Earned)
            }
            
            $n_count = 1;
            if ($row['type'] == 'SUF' || $row['type'] == 'SUF-GMS-1ST') {
                $data[$key][2] += $row['cost'];//$n_count;
            } else if ($row['type'] == 'MMF') {
                $data[$key][3] += $row['cost'];//$n_count;
            }
            if ($row['cost']) {
                $data[$key][5] += $row['cost'];
            }
            $data[$key][4] = $row['timestamp'];
        }
        
    }
}

function ncn_report_download_distributor() {
require_once 'sites/all/libraries/php_xls/Classes/PHPExcel.php';
    
    $demo_file = "sales_report_distributor.xls";
    $objReader = PHPExcel_IOFactory::createReader("Excel5");
    $objPHPExcel = $objReader->load($demo_file);
    
    $data = array();
    $total= array();
    
    $report_date = intval($_REQUEST['date']);
    if ($report_date == 0) {
        $report_date = time();
    }
    
    $str_date = date("F Y", $report_date);
    
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("F3" , "Date: ".$str_date);
    _ncn_report_get_data_distributor($data, $report_date);
    
    $count_data = count($data);
    if ($count_data>0) {
        $objPHPExcel->getActiveSheet()->insertNewRowBefore(9, $count_data); 
    }
    for ($i=0; $i<$count_data; $i++) {
        $source = "A7:E7";
        if ($i%2 == 1) {
            $source = "A8:E8";
        }
        $row_index = $i+9;
        $dest = "A$row_index:E$row_index";
        $objPHPExcel->getActiveSheet()->duplicateStyle( $objPHPExcel->getActiveSheet()->getStyle($source), $dest );
        
        $source = "F7";
        if ($i%2 == 1) {
            $source = "F8";
        }
        $dest = "F$row_index";
        
        $objPHPExcel->getActiveSheet()->duplicateStyle( $objPHPExcel->getActiveSheet()->getStyle($source), $dest );
        
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("F$row_index" , "=E$row_index*10");
        
    }
    $objPHPExcel->getActiveSheet()->removeRow(7+$count_data, 2);
    
    $i = 0;
    foreach($data as $key=>$data_part) {
        $row_index = $i+7;
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("A$row_index" , $data_part[0]);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("B$row_index" , $data_part[1]);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("C$row_index" , $data_part[2]);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("D$row_index" , $data_part[3]);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("E$row_index" , $data_part[4]);
        $i++;
    }
    
    $report_name = "Monthly Distributor Sales Summary Report ($str_date)";
    
    header('Content-Type: application/vnd.ms-excel');
    header('Content-Disposition: attachment;filename="'.$report_name.'.xls"');
    header('Cache-Control: max-age=0');
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5');
    $objWriter->save('php://output');
        
    /*$objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, "Excel5");
    $objWriter->save("daily.xls");*/
    exit;
}

function ncn_report_download_associate() {
    require_once 'sites/all/libraries/php_xls/Classes/PHPExcel.php';
    
    $demo_file = "sales_report_associate.xls";
    $objReader = PHPExcel_IOFactory::createReader("Excel5");
    $objPHPExcel = $objReader->load($demo_file);
    
    $data = array();
    $total= array();
    
    $report_date = intval($_REQUEST['date']);
    if ($report_date == 0) {
        $report_date = time();
    }
    
    $str_date = date("F Y", $report_date);
    
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("G3" , "Date: ".$str_date);
    _ncn_report_get_data_associate($data, $report_date);
    
    $count_data = count($data);
    if ($count_data>0) {
        $objPHPExcel->getActiveSheet()->insertNewRowBefore(9, $count_data); 
    }
    for ($i=0; $i<$count_data; $i++) {
        $source = "A7:E7";
        if ($i%2 == 1) {
            $source = "A8:E8";
        }
        $row_index = $i+9;
        $dest = "A$row_index:E$row_index";
        $objPHPExcel->getActiveSheet()->duplicateStyle( $objPHPExcel->getActiveSheet()->getStyle($source), $dest );
        
        $source = "F7";
        if ($i%2 == 1) {
            $source = "F8";
        }
        $dest = "F$row_index";
        $objPHPExcel->getActiveSheet()->duplicateStyle( $objPHPExcel->getActiveSheet()->getStyle($source), $dest );
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("F$row_index" , "=((B$row_index+C$row_index+D$row_index)*50)");
        
        $source = "G7";
        if ($i%2 == 1) {
            $source = "G8";
        }
        $dest = "G$row_index";
        $objPHPExcel->getActiveSheet()->duplicateStyle( $objPHPExcel->getActiveSheet()->getStyle($source), $dest );
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("G$row_index" , "=E$row_index*10");
        
        
    }
    $objPHPExcel->getActiveSheet()->removeRow(7+$count_data, 2);
    
    $i = 0;
    foreach($data as $key=>$data_part) {
        $row_index = $i+7;
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("A$row_index" , $data_part[0]);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("B$row_index" , $data_part[1]);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("C$row_index" , $data_part[2]);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("D$row_index" , $data_part[3]);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("E$row_index" , $data_part[4]);
        $i++;
    }
    
    $report_name = "Monthly Associate Sales Summary Report ($str_date)";
    
    header('Content-Type: application/vnd.ms-excel');
    header('Content-Disposition: attachment;filename="'.$report_name.'.xls"');
    header('Cache-Control: max-age=0');
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5');
    $objWriter->save('php://output');
        
    /*$objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, "Excel5");
    $objWriter->save("daily.xls");*/
    exit;
}

function ncn_report_download_monthly_detailed($type) {
    require_once 'sites/all/libraries/php_xls/Classes/PHPExcel.php';
    
    $demo_file = "sales_report_detailed.xls";
    $objReader = PHPExcel_IOFactory::createReader("Excel5");
    $objPHPExcel = $objReader->load($demo_file);
    
    $data = array();
    $total= array();
    
    $report_date = intval($_REQUEST['date']);
    if ($report_date == 0) {
        $report_date = time();
    }
    $uid = intval($_REQUEST['uid']);
    
    $str_date = date("F Y", $report_date);
    
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("F3" , "Date: ".$str_date);
    $_user = user_load($uid);
    
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("A5" , "[".$_user->profile_firstname." ".$_user->profile_lastname."] Detailed Monthly Sales Report");
    if ($type == 'distributor') {
        _ncn_report_get_data_distributor_detailed($data, $report_date, $uid);
    } else {
        _ncn_report_get_data_associate_detailed($data, $report_date, $uid);
    }
    
    $count_data = count($data);
    if ($count_data>0) {
        $objPHPExcel->getActiveSheet()->insertNewRowBefore(9, $count_data); 
    }
    for ($i=0; $i<$count_data; $i++) {
        $source = "A7:E7";
        if ($i%2 == 1) {
            $source = "A8:E8";
        }
        $row_index = $i+9;
        $dest = "A$row_index:E$row_index";
        $objPHPExcel->getActiveSheet()->duplicateStyle( $objPHPExcel->getActiveSheet()->getStyle($source), $dest );
        
        $source = "F7";
        if ($i%2 == 1) {
            $source = "F8";
        }
        $dest = "F$row_index";
        
        $objPHPExcel->getActiveSheet()->duplicateStyle( $objPHPExcel->getActiveSheet()->getStyle($source), $dest );
        
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("F$row_index" , "=(C$row_index*50)+(D$row_index*10)");
        
    }
    $objPHPExcel->getActiveSheet()->removeRow(7+$count_data, 2);
    
    $i = 0;
    $total = 0;
    foreach($data as $key=>$data_part) {
        $row_index = $i+7;
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("A$row_index" , $data_part[0]);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("B$row_index" , $data_part[1]);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("C$row_index" , $data_part[2]);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("D$row_index" , $data_part[3]);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("E$row_index" , date('d M Y H:i:s', $data_part[4]));
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("F$row_index" , $data_part[5]);
        
        $total += $data_part[5];
        $i++;
    }
    
    $row_index += 2;
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("F$row_index" , $total);
    
    $report_name = "[".$_user->profile_firstname." ".$_user->profile_lastname."]Detailed Monthly Sales Report ($str_date)";
    
    header('Content-Type: application/vnd.ms-excel');
    header('Content-Disposition: attachment;filename="'.$report_name.'.xls"');
    header('Cache-Control: max-age=0');
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5');
    $objWriter->save('php://output');
        
    /*$objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, "Excel5");
    $objWriter->save("daily.xls");*/
    exit;
}

function ncn_report_download_company_sales_summary() {
    require_once 'sites/all/libraries/php_xls/Classes/PHPExcel.php';
    
    $demo_file = "company_sales_report.xls";
    $objReader = PHPExcel_IOFactory::createReader("Excel5");
    $objPHPExcel = $objReader->load($demo_file);
    
    $data = array();
    $total= array();
    
    $report_date = intval($_REQUEST['date']);
    if ($report_date == 0) {
        $report_date = time();
    }
    
    $str_date = date("F Y", $report_date);
    
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("F3" , "Date: ".$str_date);
    _ncn_report_get_data_company_sales($data, $report_date);
    
    $count_data = count($data);
    if ($count_data>0) {
        $objPHPExcel->getActiveSheet()->insertNewRowBefore(9, $count_data); 
    }
    for ($i=0; $i<$count_data; $i++) {
        $source = "A7:E7";
        if ($i%2 == 1) {
            $source = "A8:E8";
        }
        $row_index = $i+9;
        $dest = "A$row_index:E$row_index";
        $objPHPExcel->getActiveSheet()->duplicateStyle( $objPHPExcel->getActiveSheet()->getStyle($source), $dest );
        
        $source = "F7";
        if ($i%2 == 1) {
            $source = "F8";
        }
        $dest = "F$row_index";
        
        $objPHPExcel->getActiveSheet()->duplicateStyle( $objPHPExcel->getActiveSheet()->getStyle($source), $dest );
        
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("F$row_index" , "=SUM(B$row_index:E$row_index)");
        
    }
    $objPHPExcel->getActiveSheet()->removeRow(7+$count_data, 2);
    
    $i = 0;
    foreach($data as $key=>$data_part) {
        $row_index = $i+7;
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("A$row_index" , $data_part[0]);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("B$row_index" , $data_part[1]);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("C$row_index" , $data_part[2]);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("D$row_index" , $data_part[3]);
        $objPHPExcel->setActiveSheetIndex(0)->setCellValue("E$row_index" , $data_part[4]);
        $i++;
    }
    
    $total = 0;
    foreach ($data as $key=>$row) {
        $total += $row[5];
    }
    $row_index = $i+1+7;
    $objPHPExcel->setActiveSheetIndex(0)->setCellValue("E$row_index" , 'Total:  $ '.$total);
    $report_name = "Company Sales Summary Report ($str_date)";
    
    header('Content-Type: application/vnd.ms-excel');
    header('Content-Disposition: attachment;filename="'.$report_name.'.xls"');
    header('Cache-Control: max-age=0');
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5');
    $objWriter->save('php://output');
        
    /*$objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, "Excel5");
    $objWriter->save("daily.xls");*/
    exit;
}

function ncn_report_get_best_total_global($from) {
    $max_month = variable_get('best_total_monthly', 0);
    $max_week = variable_get('best_total_weekly', 0);
    $max_day = variable_get('best_total_daily', 0);
    
    $max_month = ncn_report_get_best_total_from($ts_max_month, $max_month, $from, 'monthly');
    $max_week = ncn_report_get_best_total_from($ts_max_week, $max_week, $from, 'weekly');
    $max_day = ncn_report_get_best_total_from($ts_max_day, $max_day, $from, 'daily');
    
    if ($max_month && $ts_max_month) {
        variable_set('best_total_monthly', $max_month);
        variable_set('best_total_monthly_ts', $ts_max_month);
    }
    if ($max_week && $ts_max_week) {
        variable_set('best_total_weekly', $max_week);
        variable_set('best_total_weekly_ts', $ts_max_week);
    }
    if ($max_day && $ts_max_day) {
        variable_set('best_total_daily', $max_day);
        variable_set('best_total_daily_ts', $ts_max_day);
    }
}

/**
 * ts_max : return value (ref)
 * max_total : current max total
 * from: from when
 * type : monthly/weekly/daily
 */
function ncn_report_get_best_total_from(&$ts_max, $max_total, $from, $type='daily') {
    $max = $max_total;
    $ts_max = 0;
    $unit = 'day';
    if ($type == 'weekly') { $unit = 'week'; }
    else if ($type == 'monthly') { $unit = 'month'; }
    
    $index_ts = $from;
    $today_ts = strtotime("+1 unit", date('U'));
    
    
    while ($index_ts < $today_ts) {
        $data = array();
        $total = array();
        
        _ncn_report_get_data($data, $total, $index_ts, $type);
        if ($total[1] > $max) {
            $max = $total[1];
            $ts_max = $index_ts;
        }
        
        $index_ts = strtotime("+1 $unit", $index_ts);
    }
    
    if ($ts_max == 0) {
        return 0;
    }
    
    return $max;
}

function ncn_report_get_best_total_value($report_total, $report_date, $type) {
    $variable_total = 'best_total_'.$type;
    $variable_ts = 'best_total_'.$type.'_ts';
    
    $max_total = variable_get($variable_total, 0);
    $max_ts = variable_get($variable_ts, 0);
    
    if ($report_total>$max_total) {
        variable_set($variable_total, $report_total);
        variable_set($variable_ts, $report_date);
        $max_total = $report_total;
        $max_ts = $report_date;
    }
    
    if ($type == "monthly") { $max_ts_str = date('F Y', $max_ts);   } 
    else if ($type == "weekly") { 
        $to = strtotime("next monday", $max_ts);
        $from = strtotime("last monday", $to);
        $max_ts_str = date("Y/m/d", $from)." ~ ".date("Y/m/d", strtotime("-1 day", $to));
    }
    else {$max_ts_str = date('Y/m/d', $max_ts); }
    
    return render_payment_cost($max_total). "&nbsp;&nbsp;&nbsp;($max_ts_str)";
}
