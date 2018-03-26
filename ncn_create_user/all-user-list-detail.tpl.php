<?php
$detail_id = $result;

global $user;
if (isset($_POST['tfunction']) && $_POST['tfunction'] == 'send_claim_reminder_mail') {
    $result = ncn_admin_send_reminder_mail($detail_id);
}

$_user = user_load($detail_id);
if (!$_user) {
    drupal_goto('admin/config/ncn_create_user/all_user_list');
    exit;
}

unset($_user->roles[2]);
$roles = implode($_user->roles);
drupal_set_title($_user->profile_firstname.' '.$_user->profile_lastname." ( $roles )");

$css = ".clearfix:after{clear:both;content:".";display:block;height:0;line-height:0;visibility:hidden;}.clearfix{display:inline-block;}html[xmlns] .clearfix{display:block;}* html .clearfix{height:1%;}body{margin:0;padding:0;background:#edf5fa;font:12px/170% Verdana,sans-serif;color:#494949;}input{font:12px/100% Verdana,sans-serif;color:#494949;}textarea,select{font:12px/160% Verdana,sans-serif;color:#494949;}.resizable-textarea textarea{width:100%;}h1,h2,h3,h4,h5,h6{margin:0;padding:0;font-weight:normal;font-family:Helvetica,Arial,sans-serif;}h1{font-size:170%;}h2{font-size:160%;line-height:130%;}h3{font-size:140%;}h4{font-size:130%;}h5{font-size:120%;}h6{font-size:110%;}ul,quote,code,fieldset{margin:.5em 0;}p{margin:0.6em 0 1.2em;padding:0;}a:link,a:visited{color:#027AC6;text-decoration:none;}a:hover{color:#0062A0;text-decoration:underline;}a:active,a.active{color:#5895be;}hr{margin:0;padding:0;border:none;height:1px;background:#5294c1;}ul{margin:0.5em 0 1em;padding:0;}ol{margin:0.75em 0 1.25em;padding:0;}ol li,ul li{margin:0.4em 0 0.4em .5em;}ul.menu,.item-list ul{margin:0.35em 0 0 -0.5em;padding:0;}ul.menu ul,.item-list ul ul{margin-left:0em;}ol li,ul li,ul.menu li,.item-list ul li,li.leaf{margin:0.15em 0 0.15em .5em;}ul li,ul.menu li,.item-list ul li,li.leaf{padding:0 0 .2em 1.5em;list-style-type:none;list-style-image:none;background:transparent url(/themes/garland/images/menu-leaf.gif) no-repeat 1px .35em;}ol li{padding:0 0 .3em;margin-left:2em;}ul li.expanded{background:transparent url(/themes/garland/images/menu-expanded.gif) no-repeat 1px .35em;}ul li.collapsed{background:transparent url(/themes/garland/images/menu-collapsed.gif) no-repeat 0px .35em;}ul li.leaf a,ul li.expanded a,ul li.collapsed a{display:block;}ul.inline li{background:none;margin:0;padding:0 1em 0 0;}ol.task-list{margin-left:0;list-style-type:none;list-style-image:none;}ol.task-list li{padding:0.5em 1em 0.5em 2em;}ol.task-list li.active{background:transparent url(/themes/garland/images/task-list.png) no-repeat 3px 50%;}ol.task-list li.done{color:#393;background:transparent url(/misc/watchdog-ok.png) no-repeat 0px 50%;}ol.task-list li.active{margin-right:1em;}fieldset ul.clear-block li{margin:0;padding:0;background-image:none;}dl{margin:0.5em 0 1em 1.5em;}dl dt{}dl dd{margin:0 0 .5em 1.5em;}img,a img{border:none;}table{margin:1em 0;width:100%;}thead th{border-bottom:2px solid #d3e7f4;color:#494949;font-weight:bold;}th a:link,th a:visited{color:#6f9dbd;}td,th{padding:.3em .5em;}tr.even,tr.odd,tbody th{border:solid #d3e7f4;border-width:1px 0;}tr.odd,tr.info{background-color:#edf5fa;}tr.even{background-color:#fff;}tr.drag{background-color:#fffff0;}tr.drag-previous{background-color:#ffd;}tr.odd td.active{background-color:#ddecf5;}tr.even td.active{background-color:#e6f1f7;}td.region,td.module,td.container,td.category{border-top:1.5em solid #fff;border-bottom:1px solid #b4d7f0;background-color:#d4e7f3;color:#455067;font-weight:bold;}tr:first-child td.region,tr:first-child td.module,tr:first-child td.container,tr:first-child td.category{border-top-width:0;}span.form-required{color:#ffae00;}span.submitted,.description{font-size:0.92em;color:#898989;}.description{line-height:150%;margin-bottom:0.75em;color:#898989;}.messages,.preview{margin:.75em 0 .75em;padding:.5em 1em;}.messages ul{margin:0;}.form-checkboxes,.form-radios,.form-checkboxes .form-item,.form-radios .form-item{margin:0.25em 0;}#center form{margin-bottom:2em;}.form-button,.form-submit{margin:2em 0.5em 1em 0;}#dblog-form-overview .form-submit,.confirmation .form-submit,.search-form .form-submit,.poll .form-submit,fieldset .form-button,fieldset .form-submit,.sidebar .form-button,.sidebar .form-submit,table .form-button,table .form-submit{margin:0;}.box{margin-bottom:2.5em;}#header-region{min-height:1em;background:#d2e6f3 url(/themes/garland/images/bg-navigation.png) repeat-x 50% 100%;}#header-region .block{display:block;margin:0 1em;}#header-region .block-region{display:block;margin:0 0.5em 1em;padding:0.5em;position:relative;top:0.5em;}#header-region *{display:inline;line-height:1.5em;margin-top:0;margin-bottom:0;}#header-region script{display:none;}#header-region p,#header-region img{margin-top:0.5em;}#header-region h2{margin:0 1em 0 0;}#header-region h3,#header-region label,#header-region li{margin:0 1em;padding:0;background:none;}#wrapper{background:#edf5fa url(/themes/garland/images/body.png) repeat-x 50% 0;min-width:1024px;}#wrapper #container{margin:0 auto;padding:0 20px;max-width:1270px;}#wrapper #container #header{height:80px;}#wrapper #container #header #logo-floater{position:absolute;}#wrapper #container #header h1,#wrapper #container #header h1 a:link,#wrapper #container #header h1 a:visited{line-height:120px;position:relative;z-index:2;white-space:nowrap;}#wrapper #container #header h1 span{font-weight:bold;}#wrapper #container #header h1 img{padding-top:14px;padding-right:20px;float:left;}body.sidebars{min-width:980px;}body.sidebar-left,body.sidebar-right{min-width:780px;}#wrapper #container #center{float:left;width:100%;}body.sidebar-left #center{margin-left:-210px;}body.sidebar-right #center{margin-right:-210px;}body.sidebars #center{margin:0 -210px;}body.sidebar-left #squeeze{margin-left:210px;}body.sidebar-right #squeeze{margin-right:210px;}body.sidebars #squeeze{margin:0 210px;}#wrapper #container .sidebar{margin:60px 0 5em;width:210px;float:left;z-index:2;position:relative;}#wrapper #container .sidebar .block{margin:0 0 1.5em 0;}#sidebar-left .block{padding:0 15px 0 0px;}#sidebar-right .block{padding:0 0px 0 15px;}.block .content{margin:0.5em 0;}#sidebar-left .block-region{margin:0 15px 0 0px;}#sidebar-right .block-region{margin:0 0px 0 15px;}.block-region{padding:1em;background:transparent;border:2px dashed #b4d7f0;text-align:center;font-size:1.3em;}#wrapper #container #center #squeeze{background:#fff url(/themes/garland/images/bg-content.png) repeat-x 50% 0;position:relative;}#wrapper #container #center .right-corner{background:transparent url(/themes/garland/images/bg-content-right.png) no-repeat 100% 0;position:relative;left:10px;}#wrapper #container #center .right-corner .left-corner{padding:60px 25px 5em 35px;background:transparent url(/themes/garland/images/bg-content-left.png) no-repeat 0 0;margin-left:-10px;position:relative;left:-10px;min-height:400px;}#wrapper #container #footer{float:none;clear:both;text-align:center;margin:4em 0 -3em;color:#898989;}#wrapper #container .breadcrumb{position:absolute;top:15px;left:35px;z-index:3;}body.sidebar-left #footer{margin-left:-210px;}body.sidebar-right #footer{margin-right:-210px;}body.sidebars #footer{margin:0 -210px;}#wrapper #container #header h1,#wrapper #container #header h1 a:link,#wrapper #container #header h1 a:visited{color:#fff;font-weight:normal;text-shadow:#1659ac 0px 1px 3px;font-size:1.5em;}#wrapper #container #header h1 a:hover{text-decoration:none;}#wrapper #container .breadcrumb{font-size:0.92em;}#wrapper #container .breadcrumb,#wrapper #container .breadcrumb a{color:#529ad6;}#mission{padding:1em;background-color:#fff;border:1px solid #e0e5fb;margin-bottom:2em;}ul.primary-links{margin:0;padding:0;float:right;position:relative;z-index:4;}ul.primary-links li{margin:0;padding:0;float:left;background-image:none;}ul.primary-links li a,ul.primary-links li a:link,ul.primary-links li a:visited{display:block;margin:0 1em;padding:.75em 0 0;color:#fff;background:transparent url(/themes/garland/images/bg-navigation-item.png) no-repeat 50% 0;}ul.primary-links li a:hover,ul.primary-links li a.active{color:#fff;background:transparent url(/themes/garland/images/bg-navigation-item-hover.png) no-repeat 50% 0;}ul.secondary-links{margin:0;padding:18px 0 0;float:right;clear:right;position:relative;z-index:4;}ul.secondary-links li{margin:0;padding:0;float:left;background-image:none;}ul.secondary-links li a,ul.secondary-links li a:link,ul.secondary-links li a:visited{display:block;margin:0 1em;padding:.75em 0 0;color:#cde3f1;background:transparent;}ul.secondary-links li a:hover,ul.secondary-links li a.active{color:#cde3f1;background:transparent;}ul.primary,ul.primary li,ul.secondary,ul.secondary li{border:0;background:none;margin:0;padding:0;float:left;}#tabs-wrapper{margin:0 -26px 1em;padding:0 26px;border-bottom:1px solid #e9eff3;position:relative;}ul.primary{padding:0.5em 0 10px;float:left;white-space:normal;}ul.secondary{clear:both;text-align:left;border-bottom:1px solid #e9eff3;margin:-0.2em -26px 1em;padding:0 26px 0.6em;}h2.with-tabs{float:left;margin:0 2em 0 0;padding:0;}ul.primary li a,ul.primary li.active a,ul.primary li a:hover,ul.primary li a:visited,ul.secondary li a,ul.secondary li.active a,ul.secondary li a:hover,ul.secondary li a:visited{border:0;background:transparent;padding:4px 1em;margin:0 0 0 1px;height:auto;text-decoration:none;position:relative;top:-1px;display:inline-block;}ul.primary li.active a,ul.primary li.active a:link,ul.primary li.active a:visited,ul.primary li a:hover,ul.secondary li.active a,ul.secondary li.active a:link,ul.secondary li.active a:visited,ul.secondary li a:hover{background:url(/themes/garland/images/bg-tab.png) repeat-x 0 50%;color:#fff;}ul.primary li.active a,ul.secondary li.active a{font-weight:bold;}.node{border-bottom:1px solid #e9eff3;margin:0 -26px 1.5em;padding:1.5em 26px;}ul.links li,ul.inline li{margin-left:0;margin-right:0;padding-left:0;padding-right:1em;background-image:none;}.node .links,.comment .links{text-align:left;}.node .links ul.links li,.comment .links ul.links li{}.terms ul.links li{margin-left:0;margin-right:0;padding-right:0;padding-left:1em;}.picture,.comment .submitted{float:right;clear:right;padding-left:1em;}.new{color:#ffae00;font-size:0.92em;font-weight:bold;float:right;}.terms{float:right;}.preview .node,.preview .comment,.sticky{margin:0;padding:0.5em 0;border:0;background:0;}.sticky{padding:1em;background-color:#fff;border:1px solid #e0e5fb;margin-bottom:2em;}#comments{position:relative;top:-1px;border-bottom:1px solid #e9eff3;margin:-1.5em -25px 0;padding:0 25px;}#comments h2.comments{margin:0 -25px;padding:.5em 25px;background:#fff url(/themes/garland/images/gradient-inner.png) repeat-x 0 0;}.comment{margin:0 -25px;padding:1.5em 25px 1.5em;border-top:1px solid #e9eff3;}.indented{margin-left:25px;}.comment h3 a.active{color:#494949;}.node .content,.comment .content{margin:0.6em 0;}#aggregator{margin-top:1em;}#aggregator .feed-item-title{font-size:160%;line-height:130%;}#aggregator .feed-item{border-bottom:1px solid #e9eff3;margin:-1.5em -31px 1.75em;padding:1.5em 31px;}#aggregator .feed-item-categories{font-size:0.92em;}#aggregator .feed-item-meta{font-size:0.92em;color:#898989;}#palette .form-item{border:1px solid #fff;}#palette .item-selected{background:#fff url(/themes/garland/images/gradient-inner.png) repeat-x 0 0;border:1px solid #d9eaf5;}tr.menu-disabled{opacity:0.5;}tr.odd td.menu-disabled{background-color:#edf5fa;}tr.even td.menu-disabled{background-color:#fff;}.poll .bar{background:#fff url(/themes/garland/images/bg-bar-white.png) repeat-x 0 0;border:solid #f0f0f0;border-width:0 1px 1px;}.poll .bar .foreground{background:#71a7cc url(/themes/garland/images/bg-bar.png) repeat-x 0 100%;}.poll .percent{font-size:.9em;}#autocomplete li{cursor:default;padding:2px;margin:0;}fieldset{margin:1em 0;padding:1em;border:1px solid #d9eaf5;background:#fff url(/themes/garland/images/gradient-inner.png) repeat-x 0 0;}*:first-child+html fieldset{padding:0 1em 1em;background-position:0 .75em;background-color:transparent;}*:first-child+html fieldset > .description,*:first-child+html fieldset .fieldset-wrapper .description{padding-top:1em;}fieldset legend{display:block;}*:first-child+html fieldset legend,*:first-child+html fieldset.collapsed legend{display:inline;}html.js fieldset.collapsed{background:transparent;padding-top:0;padding-bottom:.6em;}html.js fieldset.collapsible legend a{padding-left:2em;background:url(/themes/garland/images/menu-expanded.gif) no-repeat 0% 50%;}html.js fieldset.collapsed legend a{background:url(/themes/garland/images/menu-collapsed.gif) no-repeat 0% 50%;}#block-node-0 h2{float:left;padding-right:20px;}#block-node-0 img,.feed-icon{float:right;padding-top:4px;}#block-node-0 .content{clear:right;}#user-login-form{text-align:center;}#user-login-form ul{text-align:left;}.profile{margin-top:1.5em;}.profile h3{border-bottom:0;margin-bottom:1em;}.profile dl{margin:0;}.profile dt{font-weight:normal;color:#898989;font-size:0.92em;line-height:1.3em;margin-top:1.4em;margin-bottom:0.45em;}.profile dd{margin-bottom:1.6em;}div.admin-panel,div.admin-panel .description,div.admin-panel .body,div.admin,div.admin .left,div.admin .right,div.admin .expert-link,div.item-list,.menu{margin:0;padding:0;}div.admin .left{float:left;width:48%;}div.admin .right{float:right;width:48%;}div.admin-panel{background:#fff url(/themes/garland/images/gradient-inner.png) repeat-x 0 0;padding:1em 1em 1.5em;}div.admin-panel .description{margin-bottom:1.5em;}div.admin-panel dl{margin:0;}div.admin-panel dd{color:#898989;font-size:0.92em;line-height:1.3em;margin-top:-.2em;margin-bottom:.65em;}table.system-status-report th{border-color:#d3e7f4;}#autocomplete li.selected,tr.selected td,tr.selected td.active{background:#027ac6;color:#fff;}tr.selected td a:link,tr.selected td a:visited,tr.selected td a:active{color:#d3e7f4;}tr.taxonomy-term-preview{opacity:0.5;}tr.taxonomy-term-divider-top{border-bottom:none;}tr.taxonomy-term-divider-bottom{border-top:1px dotted #CCC;}.messages{background-color:#fff;border:1px solid #b8d3e5;}.preview{background-color:#fcfce8;border:1px solid #e5e58f;}div.status{color:#33a333;border-color:#c7f2c8;}div.error,tr.error{color:#a30000;background-color:#FFCCCC;}.form-item input.error,.form-item textarea.error{border:1px solid #c52020;color:#363636;}tr.dblog-user{background-color:#fcf9e5;}tr.dblog-user td.active{background-color:#fbf5cf;}tr.dblog-content{background-color:#fefefe;}tr.dblog-content td.active{background-color:#f5f5f5;}tr.dblog-warning{background-color:#fdf5e6;}tr.dblog-warning td.active{background-color:#fdf2de;}tr.dblog-error{background-color:#fbe4e4;}tr.dblog-error td.active{background-color:#fbdbdb;}tr.dblog-page-not-found,tr.dblog-access-denied{background:#d7ffd7;}tr.dblog-page-not-found td.active,tr.dblog-access-denied td.active{background:#c7eec7;}table.system-status-report tr.error,table.system-status-report tr.error th{background-color:#fcc;border-color:#ebb;color:#200;}table.system-status-report tr.warning,table.system-status-report tr.warning th{background-color:#ffd;border-color:#eeb;}table.system-status-report tr.ok,table.system-status-report tr.ok th{background-color:#dfd;border-color:#beb;}#invoice_extended_data  .data-label{}#invoice_extended_data .data-input{}.claims-roomdata .scope-image{display:inline-block;margin-right:10px;margin-bottom:10px;}.claims-roomdata .scope-image .blank-image{height:10px;}.claims-roomdata .update-caption-action{margin-top:10px;}.transaction-where dl.where dt,.transaction-where dl.where dd{float:left;line-height:1.75em;margin:0 1em 0 0;padding:0;}.transaction-where dl.where a{}.transaction-where dl.where dd.b,.transaction-where dl.where dd.b .form-item,.transaction-where dl.where dd.b select{font-family:inherit;font-size:inherit;width:25em;}input.readonly-text{background-color:#f3f3f3;}#transaction_edit_page  .td-input input{width:300px;}#claim_timer_div{margin-top:10px;font-weight:bold;}.timer_alert{color:#ff0000;}.hidden-body{display:none;}tr.even.hidden-tr tr.odd.hidden-tr{border:0px none;padding:0px;}tr.hidden-tr td{border:0px none;padding:0px;}.refunded-price{color:red;}.transaction-panel{margin-bottom:10px;}.p-required{color:red;}.form-submit-panel{margin:10px 0px;}.td-label{font-weight:bold;}.edit_user_group .td-label{width:300px;}.scope_form form.attach-description{margin:1.5em 0 0.5em 0 !important;}td pre{white-space:pre-wrap;word-wrap:break-word;}.membership-calendar-header{margin:25px 0px;text-align:center;}table.calendar{border-left:1px solid #999;}tr.calendar-row{}td.calendar-day{min-height:80px;font-size:11px;position:relative;}* html div.calendar-day{height:80px;}td.calendar-day:hover{background:#eceff5;}td.calendar-day-np{background:#eee;min-height:80px;}* html div.calendar-day-np{height:80px;}td.calendar-day-head{background:#ccc;font-weight:bold;text-align:center;width:120px;padding:5px;border-bottom:1px solid #999;border-top:1px solid #999;border-right:1px solid #999;}div.day-number{background:#999;padding:5px;color:#fff;font-weight:bold;float:right;margin:-5px -5px 0 0;width:20px;text-align:center;}td.calendar-day,td.calendar-day-np{width:120px;padding:5px;border-bottom:1px solid #999;border-right:1px solid #999;}#ncn_admin_membership_calendar table.calendar{width:100%;}#ncn_admin_membership_calendar table.calendar td{vertical-align:top;}#ncn_admin_membership_calendar table.calendar td.calendar-day{height:80px;}#ncn_admin_membership_calendar table.calendar .calendar_day_mebership{font-size:24px;padding:35px 0 0 25px;color:}.ce-remained-time.half-pass{color:red;}tr.choice-item td{border-bottom:solid 1px #ccc;}tr.choice-item .handle-col{width:30px;}tr.choice-item input{height:20px;}#edit_choice_wrapper #add_another_item{margin-left:40px;}#edit_choice_wrapper tr.choice-item:nth-child(odd) {background-color:#EDF5FA;}#edit_choice_wrapper tr.choice-item:nth-child(even) {background-color:#fff;}#choice_prototype{display:none;}.ncn-poll-choice .choice-item .choice-item-index{margin-right:10px;min-width:20px;text-align:right;}.ncn-poll-choice .choice-item .choice-item-total{margin-left:10px;font-style:italic;color:#5895BE;}.report-content .report-sub-tr.collapse{display:none;}.report-content .report-sub-tr.expand{display:table-row;}.report-content .report-group-tr a,.report-content .report-group-item{padding-left:12px;}.report-content .report-group-tr a.collapse{background:transparent url(/themes/garland/images/collapse.png) no-repeat 0px 50%;}.report-content .report-group-tr a.expand{background:transparent url(/themes/garland/images/expand.png) no-repeat 0px 50%;}.user_claim_list{margin-top:50px;}.user_detail_profile .column{float:left;width:48%;margin-right:10px;}.user_detail_profile{font-size:12px;}.user_detail_profile table tbody{border:none;}.user_detail_profile table td.td-label{width:180px;}.user_detail_profile .user-detail-profile-section{margin-bottom:25px;}.user_detail_profile .user-detail-profile-section table td{vertical-align:top;}.user_detail_profile .blocked-track{font-size:12px;margin:5px 0px 5px 10px;}.user_detail_profile .user-detail-profile-section.profile_user_5 table td{vertical-align:bottom;}.user_detail_profile .profile_user_4{font-size:16px;}.user_detail_profile .profile_user_5{font-size:10px;border:solid 1px #666;}#cor_rlist_table th{font-weight:bold;}#cor_rlist_table tr{border-bottom:solid 1px #ccc;line-height:20px;}#cor_rlist_table tr.tr-room{background-color:#eee;}#cor_rlist_table tr.tr-cor td.action span{margin-left:10px;}#user-admin-filter dl.multiselect dd.a,#user-admin-filter dl.multiselect dd.a .form-item{width:12em;}.claims-detail.additional-claim-info{width:850px;}.claims-detail.additional-claim-info .claims-section-wrapper{margin:15px auto;}.additional-claim-info .claims-section-wrapper .title{float:none;text-transform:none;}.additional-claim-info .aci-claim-info{padding-left:2px;margin-bottom:10px;}.aci-section-links ul{padding-left:2px;}.aci-section-links .aci-section-link-item{list-style-type:none;float:left;}.aci-section-links .aci-section-link-item a{margin-right:15px;font-weight:bold;text-decoration:none;color:#505050;}.aci-section-links .aci-section-link-item.active a{border-bottom:solid 3px #A4C440;}.additional-claim-info .aci-section-content{padding:10px;background-color:white;border-top:solid 2px #eee;}.aci-section-content table tr td{padding:4px;}.aci-section-content .td-label{width:250px;}#aci_ci_content .td-label{width:auto;}#aci_ci_content td{border:solid 1px #AAA;}.aci-section-content .text-field-item input{width:350px;}.aci-section-content .text-field-item.small-size input{width:100px;}.aci-section-content .aci-actions{text-align:center;margin-top:15px;}.inline-field-item{display:inline;}li.inline-field-item{list-style-type:none;background:none;padding:0px;}ul.inline-group-fields{margin:0px;padding:0px;display:inline;}ul.inline-group-fields.clearfix{display:inline;}.aci-section-content .field-item.inline-field{display:inline;}.insurance_claim_policy .aci-section-content .field-item.inline-field.field-phone-number input{width:200px;}.insurance_claim_policy .aci-section-content .field-item.inline-field.field-phone-ext label{margin:0 2px;}.insurance_claim_policy .aci-section-content .field-item.inline-field.field-phone-ext input{width:124px;}.insurance_claim_policy .aci-section-content .field-item.inline-field.field-date-from input,.insurance_claim_policy .aci-section-content .field-item.inline-field.field-date-to input{width:168px;}.aci-section-content #dcl_log_list{}.aci-section-content .dcl-action-panel{margin:20px 0px;}#dcl_log_list table .dcl-day-week{width:100px;}#dcl_log_list table .dcl-date{width:120px;}#dcl_log_list table .dcl-hours-day{width:120px;}#dcl_log_list table .dcl-after-hours{width:100px;}.aci-section-content .td-label{text-align:right;padding-right:10px;}.aci-section-content table tbody{border:0px;}#ss_eit_list table .ss-eit-date{width:80px;}#ss_eit_list table .ss-eit-staff{width:80px;}#ss_eit_list table .ss-eit-exterior-temp{width:140px;}#ss_eit_list table .ss-eit-interior-temp{width:140px;}#ss_eit_list table .ss-eit-gpp{width:80px;}#ss_eit_list table .ss-eit-time-of-readings{width:120px;}#ss_smr_list table .ss-smr-date{width:70px;}#ss_smr_list table .ss-smr-wall{width:40px;}#ss_smr_list table .ss-smr-fd-area{width:40px;}#ss_smr_list table .ss-smr-fd-center{width:40px;}#ss_smr_list table .ss-smr-cd-area{width:40px;}#ss_smr_list table .ss-smr-cd-center{width:40px;}#tabcontent .ncn-room-action-panel{float:right;margin-top:15px;font-weight:bold;}table#ss_services_content th,table#ss_services_content td{padding:4px;border:solid 1px #aaa;}table#ss_services_content th{border-bottom:solid 2px #aaa;padding:8px 4px;}#ss_services_content .after-hours{background-color:yellow;text-align:center;}#ss_services_content .reg-hours{text-align:center;}#ss_services_content .ss-services-sub-section{background-color:#444;color:white;}#ss_rd_room_info{margin:5px 0px 15px;}table.ss-list-table{margin:10px 5px;}table.ss-list-table tbody{border-bottom:solid 3px #ccc;min-height:12px;}#ncn_admin_view_room_scopesheet table.ss-list-table tbody{border-bottom:solid 1px #ccc;}#ss_rd_list table .ss-rd-name{width:150px;}#ss_rd_list table .ss-rd-size{width:250px;}#ss_rd_list table .ss-rd-tfa{width:150px;}#ss_rd_list table .ss-rd-floor-type{width:100px;}#ss_ep_list table .ss-ep-panel-type-td{width:150px;}#ss_ep_list table .ss-ep-data-td{width:400px;}#ss_ep_action_panel_type_content,#ss_ep_action_panel_content{margin-left:100px;}#ss_ep_action_panel_content .ss-ep-actions{margin-left:200px;}.ss-actions input{padding-left:20px;padding-right:20px;}.ncn_admin_room_scopesheet .room-ss-room-links .room-link{display:inline;margin-right:10px;background:none;}.ncn_admin_room_scopesheet .room-ss-room-links .room-link.active a{text-decoration:underline;font-weight:bold;}#ss_services_content .clearfix:after{display:inline;}.claim-view-scopesheet-links{margin:10px 0px;}.claim-view-scopesheet-links .room-ss-link{margin-left:15px;display:inline;}.room-ss-room-edit-view-link,.room-ss-room-view-edit-link{float:right;}.hidden-div{display:none;}.claim-select-actions{background:url(/themes/garland/images/arrow_ltr.png) 5px 0 no-repeat;padding-left:45px;padding-top:5px;}.claim-notes-table tbody{border:none;}.claim-notes-table .operation a{color:#0062a0;}.claim-notes-table .operation a:hover{cursor:pointer;}.claim-note-add-form #ncn_admin_claim_note_cancel_btn{display:none;}.claim-note-update-form #ncn_admin_claim_note_cancel_btn{display:inline-block;}.claim-note-delete-form #ncn_admin_claim_note_cancel_btn{display:none;}";
drupal_add_css($css, 'inline');
//var_dump(base_path().'sites/all/modules/ncn_create_user/ncn_create_user.css');
//drupal_add_css(base_path() . 'sites/all/modules/ncn_create_user/ncn_create_user.css');
//drupal_add_css('http://open_netclaimsnow\trunk\sites\all\modules\ncn_create_user', array('group' => CSS_THEME, 'type' => 'external'));


/*$query = "SELECT * FROM profile_fields,profile_values WHERE profile_fields.fid=profile_values.fid AND profile_values.uid=".$uid;
$result = mysql_query($query);
$row_count = mysql_num_rows($result);
$profile_data = array();
for ($i=0; $i<$row_count; $i++)
{
$profile_data[] = mysql_fetch_assoc($result);
}*/
/** User Profile Section - 1 **/
$profile_user_1 = array(
    'profile_legalname'=> array('title'=>t('Company Name'),'value'=>$_user->profile_legalname),
    'name'=> array('title'=>t('Name'),'value'=>$_user->profile_firstname.' '.$_user->profile_lastname),
    'profile_memberid'=> array('title'=>t('Member ID#'),'value'=>isset($_user->profile_memberid)?$_user->profile_memberid:''),
    'profile_credit_card_security'=> array('title'=>t('Security Code'),'value'=>$_user->profile_credit_card_security),
);

/** User Profile Section - 2 **/
$profile_user_2 = array(
    'profile_address'			    => array('title'=>t('Address'),'value'=>$_user->profile_address),
    'profile_city'					=> array('title'=>t('City'),'value'=>$_user->profile_city),
    'profile_state'					=> array('title'=>t('Province/State'),'value'=>$_user->profile_state),
    'profile_zip'					=> array('title'=>t('Zip'),'value'=>$_user->profile_zip),
    'profile_officephone'			=> array('title'=>t('Phone Number'),'value'=>$_user->profile_officephone),
    'profile_mobilephone'			=> array('title'=>t('Mobile Phone Number'),'value'=>$_user->profile_mobilephone),
    'profile_company_fax'			=> array('title'=>t('Fax Number'),'value'=>$_user->profile_company_fax),
    'profile_contactemail'		    => array('title'=>t('Email Address'),'value'=>$_user->mail),
);

/** User Profile Section - 3 **/
$profile_user_3 = array(
    'username'					    => array('title'=>t('Username'),'value'=>$_user->name),
    'member_type'				    => array('title'=>t('Member Type'),'value'=>get_member_type_name($_user->uid)),
    'sub_user'				    	=> array('title'=>t('Sub User'),'value'=>''),
    'asso_dist'					    => array('title'=>t('Distributer/Associate'),'value'=>get_distributor_associate_name($_user->uid)),
    'account_manager'		        => array('title'=>t('Account Manager'),'value'=>get_account_manager_name($_user->uid)),
    'profile_sales_rep'	            => array('title'=>t('Sales Rep'),'value'=>$_user->profile_sales_rep),
);

/** User Profile Section - 4 **/
$member_since = '';

if ( isset($_user->profile_memberid) && is_member($_user->profile_memberid) ) {
    $member = get_member_from_id($_user->profile_memberid);
    if (!empty($member)) {
        $member_since = $member['create'];
    }
}
if ($member_since == '') {
    $member_since = date('m/d/Y', $_user->created);
} else {
    $member_since = date('m/d/Y', $member_since);
}

if ($_user->profile_fblocked == 'first') {
    $u_status = 'training';
} else {
    $d_status = array(t('blocked'), t('active'));
    $u_status = $d_status[$_user->status];
    if (!$_user->status) {
        $block_track = ncn_user_get_last_block_track($_user->uid);
        if (!empty($block_track) && $block_track['blocked']) {
            $blocked_time = date("m/d/Y H:i", $block_track['blocked']);
        } else {
            $blocked_time = "time - no track";
        }
        $u_status .= " (".$blocked_time.")";
    }
}

$r_block_track = ncn_user_show_block_track($_user->uid);
if ($r_block_track != "") {
    $u_status.="<div class='blocked-track'>
    <div>[Blocked Time Track]</div>
    $r_block_track
</div>";
}

$ytd_dollar = '';
$total_dollar = '';
if (isset($_user->profile_memberid) && is_member($_user->profile_memberid)) {
    $ytd_dollar		= render_payment_cost(ncn_report_get_ytd($_user->uid));
    $total_dollar = render_payment_cost(ncn_report_get_total_ytd($_user->uid));
    if (user_access('payment_transaction_page')) {
        if ($ytd_dollar != '$0.00') {
            $ytd_dollar = l(strip_tags($ytd_dollar), "admin/config/ncn_user/$_user->uid/ytd", array('attributes'=>array('target'=>'_blank')));
        }

        if ($total_dollar != '$0.00') {
            $total_dollar = l(strip_tags($total_dollar), "admin/config/ncn_user/$_user->uid/all_payments", array('attributes'=>array('target'=>'_blank')));
        }
    }
}

$profile_user_4 = array(
    'total_claims'			        => array('title'=>t('# of Claims'),'value'=>user_detail_get_total_claims($_user)),
    'last_access'				    => array('title'=>t('Last Access'),'value'=> ($_user->access) ? t('@time ago', array('@time' => format_interval(time() - $_user->access))) : t('never')),
    'member_since'		        	=> array('title'=>t('Member Since'),'value'=>$member_since),
    'status'						=> array('title'=>t('Status'),'value'=>$u_status),
    'ytd'					        => array('title'=>t('Total $$ YTD'),'value'=>$ytd_dollar),
    'total_dollar'		        	=> array('title'=>t('Total $$ All Time'),'value'=>$total_dollar),
    );

if (isset($_user->profile_memberid) && ncn_change_order_is_available_to_use_credit($_user->profile_memberid)) {
    $remained_credit = ncn_change_order_get_month_credit_remained($_user->uid);
    $remained_credit = render_payment_cost($remained_credit);
    $profile_user_4['remained_credit'] = array('title'=>t('Remained Credits'), 'value'=>$remained_credit);
}



/** Extra Profile Section **/
$profile_user_5 = array(
    'profile_numberofairmovers'		=> array('title'=>t('Number of Airmovers'),'value'=>$_user->profile_numberofairmovers),
    'profile_numberofdehumidifiers'	=> array('title'=>t('Number of Dehumidifiers'),'value'=>$_user->profile_numberofdehumidifiers),
    'profile_numberofemployees'		=> array('title'=>t('Number of employees'),'value'=>$_user->profile_numberofemployees),
    'profile_servicevehicles'		=> array('title'=>t('Number of services vehicles'),'value'=>$_user->profile_servicevehicles),
    'profile_certifications'		=> array('title'=>t('Industry certifications'),'value'=>$_user->profile_certifications),
    'profile_emergencycontact'		=> array('title'=>t('Dedicated 24/7 Emergency Contact Number'),'value'=>$_user->profile_emergencycontact),
    'profile_websiteaddress'		=> array('title'=>t('Website address'),'value'=>$_user->profile_websiteaddress),
    'profile_taxnumber'				=> array('title'=>t('Sales tax number'),'value'=>$_user->profile_taxnumber),
    'profile_taxid'					=> array('title'=>t('Federal tax ID number'),'value'=>$_user->profile_taxid),
    'profile_taxstatus'				=> array('title'=>t('Do you collect state or county sales'),'value'=>$_user->profile_taxstatus),
    'profile_companytype'			=> array('title'=>t('Type of Entity'),'value'=>$_user->profile_companytype),
    'profile_legalname'				=> array('title'=>t('Company\'s legal name'),'value'=>$_user->profile_legalname),
    'profile_jobspermonth'			=> array('title'=>t('Estimated number of water damages jobs you do a month'),'value'=>$_user->profile_jobspermonth),
    'profile_question_service'		=> array('title'=>t('What kind of restoration services do you provide'),'value'=>trim((isset($_user->profile_question_service)?$_user->profile_question_service:''), ",")),
    );
    ?>
    <div class="user_detail_profile clearfix">
        <div class="column">
            <div class="user-detail-profile-section profile_user_1">
                <?php user_detail_profile_section($profile_user_1); ?>
            </div>
            <div class="user-detail-profile-section profile_user_2">
                <?php user_detail_profile_section($profile_user_2); ?>
            </div>
            <div class="user-detail-profile-section profile_user_3">
                <?php user_detail_profile_section($profile_user_3); ?>
            </div>
            <div class="clearfix">
                <form method="POST" class="user-detail-profile-action">
                    <input type="hidden" name="tfunction" value="send_claim_reminder_mail">
                    <table>
                        <tr>
                            <td>
                                <?php if (isset($_user->profile_memberid) && is_member($_user->profile_memberid)): ?>
                                    <input type="submit" value="Send Claim Reminder Mail" />
                                <?php endif; ?>
                            </td>
                        </tr>
                    </table>
                </form>
            </div>
        </div>
        <div class="column">
            <div class="user-detail-profile-section profile_user_4">
                <?php user_detail_profile_section($profile_user_4); ?>
            </div>
            <div class="user-detail-profile-section profile_user_5">
                <?php user_detail_profile_section($profile_user_5); ?>
            </div>
            <div class="clearfix">
                <?php if (user_access('edit user profile', $user)): ?>
                    <?php $url_edit = $GLOBALS['base_url']."/admin/config/ncn_create_user/all_user_list/edit_user/".$_user->uid; ?>
                    <input type="button" value="Edit" onclick="window.location='<?php echo $url_edit; ?>'; " style="float: right; margin-right: 30px;"/>
                <?php endif; ?>

                <?php if ($_user->status && user_access('login for test', $user) && !isset($_SESSION['admin_sandbox']) ): ?>
                    <?php $url_login = $GLOBALS['base_url']."/admin/config/ncn_create_user/all_user_list/admin_login/".$_user->uid; ?>
                    <input type="button" value="Login" onclick="window.location='<?php echo $url_login; ?>'; " style="float: right; margin-right: 30px;"/>
                <?php endif; ?>
            </div>
        </div>
    </div>

    <?php if ( (isset($_user->profile_memberid) && is_member($_user->profile_memberid))|| is_subuser($_user) ): ?>
        <div class="user_claim_list clearfix">
            <?php ncn_admin_render_user_claim_section($detail_id); ?>
        </div>
    <?php endif; ?>

    <?php
    return;
