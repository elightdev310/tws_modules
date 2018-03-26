<?php
/*****************************************************************************/
/*  Scope Sheet
/*****************************************************************************/

/**
 * Page Handler ( account/scope_sheet/%/% )
 */
/*
function ncn_claims_manager_scope_sheet($claim_id, $room_name) {
    drupal_set_title($claim_id." - Scope Sheet");
    
    ob_start();
?>
<div id="page_loading"></div>
<div id="page_results">
    <div class="claims-detail additional-claim-info scope-sheet"> 
        <div class="claims-section-wrapper">
            <div class="aci-claim-info ss-claim-info"><?php print ncn_claims_manager_scopesheet_render_claim_info($claim_id, $room_name); ?></div>
            <div class="title"><?php echo $section_title; ?></div>
            <div class="aci-section-links ss-section-links"><?php print ncn_claims_manager_scopesheet_render_section_links($claim_id, $room_name); ?></div>
            <div class="aci-section-content-wrapper ss-section-content-wrapper"><?php print ncn_claims_manager_scopesheet_render_section_content($claim_id, $room_name); ?></div>
        </div>
    </div>
</div>
<?php 
    $output = ob_get_contents();
    ob_end_clean();
    return $output;
}*/
function ncn_claims_manager_get_first_roomname($claim_id) {
  $row = ncn_cd($claim_id, 'scope', true);
  if (!empty($row)) {
      $customer_name = ncn_cd($claim_id, 'customer_name');
      foreach ($row as $room_name => $room) {
        if ($customer_name == $room_name) {
          continue;
        }
        return $room_name;
      }
  } 
  return '';
}
// function ncn_claims_manager_get_room_numbers($claim_id) {
//   $row = ncn_cd($claim_id, 'scope', true);
//   if (!empty($row)) {
//       return count($row)-1;
//   } 
//   return 0;
// }

function ncn_claims_manager_scope_sheet($claim_id, $room_name) {
  if (isset($_REQUEST['from_photo_album']) && $_REQUEST['from_photo_album']==1) {
    ncn_save_claim($claim_id, true);
  }
  
  if ($room_name === 0 || $room_name === '0') {
    $room_name = ncn_claims_manager_get_first_roomname($claim_id);
  }

  $section_title = '';
  
  drupal_set_title($claim_id." - Scope Sheet");
  ob_start();
?>
  <div class="additional-claim-info-inline-page">
    <div class="claims-section-wrapper ">
      <div class="aci-claim-info ss-claim-info"><?php print ncn_claims_manager_scopesheet_render_claim_info($claim_id, $room_name); ?></div>
      <div class="title"><?php echo $section_title; ?></div>
      <div class="panel-box">
        <div class="panel-box-content">
          <?php if(!empty($room_name)): ?>
            <div class="aci-section-links ss-section-links"><?php print ncn_claims_manager_scopesheet_render_section_links($claim_id, $room_name, "edit"); ?></div>
            <div class="aci-section-content-wrapper ss-section-content-wrapper"><?php print ncn_claims_manager_scopesheet_render_section_content($claim_id, $room_name); ?></div>
          <?php else: ?>
            <div class="text-center m20">
              Please Add a room
            </div>
          <?php endif; ?>
        </div>
      </div>
    </div>
  </div>
<?php 
  $output = ob_get_contents();
  ob_end_clean();
  return $output;
}

/**
 * Render Claim & Room Information (Scope Sheet)
 */ 
function ncn_claims_manager_scopesheet_render_claim_info($claim_id, $room_name, $mode="edit") {
  ob_start();
  @include get_realpath('ncn_claims_manager', 'ui/scopesheet/ss_claim_info_panel.php');
  $content = ob_get_contents();
  ob_end_clean();

  return $content;
}

function ncn_claims_manager_scopesheet_render_select_room_name($claim_id, $id, $value, $mode="edit") {
  $rooms = ncn_api_cm_room_get_claim_rooms_data($claim_id, FALSE);
  $select_field = "<select id=\"$id\" name=\"$id\"> \r\n";
  foreach ($rooms as $room) {
      $room_name = $room['name'];
      $selected = ($room_name==$value)? "selected" : "";
      if ($mode=="edit") {
          $ss_url = url("account/scope_sheet/$claim_id/".($room_name));
      } else {
          $ss_url = url("account/scope_sheet_view/$claim_id/".($room_name));
      }
      $select_field .= "<option value=\"".$ss_url."\" $selected >".$room_name."</option>";
  }
  $select_field .= "</select> \r\n";
  
  return $select_field;
}
/**
 * Render Top Section Links (under Scope Sheet)
 */ 
function ncn_claims_manager_scopesheet_render_section_links($claim_id, $room_name, $mode="edit") {
    $output = '<ul class="clearfix">';
    $sections = ncn_claims_manager_get_scopesheet_sections();
    foreach ($sections as $key=>$section) {
        $class = 'ss-si-'.$key;
        
        $div_wrapper = "scopesheet_".$key."_section";
        $link_id = "scopesheet_link_$key";
        $output.= "<li class='aci-section-link-item ss-section-link-item $class'>";  
        $output.= "<a href='$div_wrapper' id='$link_id' onclick='show_aci_section(\"$link_id\"); return false;'><span>". (isset($section['short_title'])? $section['short_title'] : $section['title']) ."</span></a>";
        $output.= '</li>';
    }
    $output.= '</ul>';
    
    drupal_add_js("
            jQuery(document).ready(function() {
                show_aci_section('scopesheet_link_room_dimensions');
            }); ", 'inline'
    );

    return $output;
}

/**
 * claim - scope sheet sections
 */
function ncn_claims_manager_get_scopesheet_sections() {
    return array(
        'room_dimensions'               => array('title'        =>'Room dimensions',
                                                 'render_func'  =>'ncn_admin_ss_render_room_dimensions' ), 
        'exterior_interior_temperatures'=> array('title'        =>'Exterior & Interior Temperatures', 
                                                 'render_func'  =>'ncn_admin_ss_render_ext_inter_temperatures'), 
        'structural_moisture_reading'   => array('title'        =>'Structural Moisture Readings',
                                                 'render_func'  =>'ncn_admin_ss_render_structural_moisture_readings'), 
        'equipment_placement'           => array('title'        =>'Equipment Placement', 
                                                 'render_func'  =>'ncn_admin_ss_render_equipment_placement'), 
        'services'                      => array('title'        =>'Services', 
                                                 'render_func'  =>'ncn_admin_ss_render_services'), 
        'comments'                      => array('title'        =>'Comments', 
                                                 'render_func'  =>'ncn_admin_ss_render_comments'), 
    );
}

function ncn_claims_manager_scopesheet_render_section_content($claim_id, $room_name) {
    if (isset($_POST['tfunction']) && $_POST['tfunction']=='room_scope_sheet') {
        $b_save_log = FALSE;
        if (isset($_POST['ss_rd'])) {
            //set_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_room_dimensions', serialize($_POST['ss_rd']));
            //$b_save_log = TRUE;
        }
        if (isset($_POST['ss_smr'])) {
            set_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_structural_moisture_readings', serialize($_POST['ss_smr']));
            $b_save_log = TRUE;
        }
        if (isset($_POST['ss_ep'])) {
            //set_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_equipment_placement', serialize($_POST['ss_ep']));
            //$b_save_log = TRUE;
        }
        if (isset($_POST['ss_services'])) {
            set_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_services', serialize($_POST['ss_services']));
            $b_save_log = TRUE;
        }
        if (isset($_POST['ss_comments'])) {
            set_ncn_claim_room_data_extra($claim_id, $room_name, 'ss_comments', serialize($_POST['ss_comments']));
            $b_save_log = TRUE;
        }
        
        if ($b_save_log) {
            $log_message = t("!user_name added scopesheet data for room(!room_name).", array('!user_name'=>ncn_amin_get_user_role_and_name(), '!room_name'=>$room_name));
            ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
        }
    }
    
    ob_start();
?>
<form id='ss_claim_room_scope_sheet_form' method='POST'>
    <input type='hidden' name='tfunction' value='room_scope_sheet' />
    <div class="aci-section-content ss-section-content">
    <?php 
        $sections = ncn_claims_manager_get_scopesheet_sections();
        foreach ($sections as $section=>$s_info) {
            if ($s_info['render_func'] && function_exists($s_info['render_func'])) {
                $div_wrapper = "scopesheet_".$section."_section";
                echo "<div id='$div_wrapper' class='aci-section ss-section tinyscrollbar' >";
          echo '<div class="scrollbar"><div class="track"><div class="thumb"><div class="end"></div></div></div></div>';
          echo '<div class="viewport">';
            echo '<div class="overview">';
                    print call_user_func($s_info['render_func'], $claim_id, $room_name, $section);
                    echo '</div>';
          echo '</div>';
        echo '</div>';
            }
        }
    ?>
    </div>
    <div class='aci-actions'><input type='submit' value='  Save  ' onclick="return check_row_required();" /></div>
</form>

<script>
jQuery(document).ready(function() {
  ncn_scopesheet_sections_tinyscrollbar();
});
</script>

<?php 
    $output = ob_get_contents();
    ob_end_clean();
    return $output;
}

function ncn_claims_manager_scope_sheet_view($claim_id, $room_name) {
  if (isset($_REQUEST['from_photo_album']) && $_REQUEST['from_photo_album']==1) {
    ncn_save_claim($claim_id, true);
  }
  
  drupal_set_title($claim_id." - Scope Sheet");
  $section_title = '';
  ob_start();
?>
  <div class="additional-claim-info-inline-page">
    <div class="claims-section-wrapper ">
      <div class="aci-claim-info ss-claim-info"><?php print ncn_claims_manager_scopesheet_render_claim_info($claim_id, $room_name, "view"); ?></div>
      <div class="title"><?php echo $section_title; ?></div>
      <div class="aci-section-links ss-section-links"><?php print ncn_claims_manager_scopesheet_render_section_links($claim_id, $room_name, "view"); ?></div>
      <div class="aci-section-content-wrapper ss-section-content-wrapper"><?php print ncn_claims_manager_scopesheet_view_render_section_content($claim_id, $room_name); ?></div>
    </div>
  </div>
<?php 
  $output = ob_get_contents();
  ob_end_clean();
  return $output;
}

function ncn_claims_manager_scopesheet_view_render_section_content($claim_id, $room_name) {  
  ob_start();
?>
<div id='ss_claim_room_scope_sheet_form'>
  <div class="aci-section-content ss-section-content">
  <?php 
    $sections = ncn_admin_get_scopesheet_section_views();
    foreach ($sections as $section=>$s_info) {
      if ($s_info['render_func'] && function_exists($s_info['render_func'])) {
        $div_wrapper = "scopesheet_".$section."_section";
        echo "<div id='$div_wrapper' class='aci-section ss-section tinyscrollbar' >";
          echo '<div class="scrollbar"><div class="track"><div class="thumb"><div class="end"></div></div></div></div>';
          echo '<div class="viewport">';
            echo '<div class="overview">';
            print call_user_func($s_info['render_func'], $claim_id, $room_name, $section);
            echo '</div>';
          echo '</div>';
        echo '</div>';
      }
    }
  ?>
  </div>
</div>

<script>
jQuery(document).ready(function() {
  ncn_scopesheet_sections_tinyscrollbar();
});
</script>

<?php 
  $output = ob_get_contents();
  ob_end_clean();
  return $output;
}
