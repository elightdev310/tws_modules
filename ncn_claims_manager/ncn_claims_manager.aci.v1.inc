<?php
/*****************************************************************************/
/*  Additional Claim Information
/*****************************************************************************/

function ncn_claims_manager_additional_claim_info_view($claim_id) {
  drupal_set_title($claim_id." - View Claims Processing");  
  ob_start();
  
?>
<div id="page_loading"></div>
<div id="page_results">
  <div class="claims-detail additional-claim-info"> 
    <div class="claims-section-wrapper">
      <div class="aci-claim-info"><?php print ncn_claims_manager_aci_render_claim_info($claim_id, 'view'); ?></div>
      <div class="title"><?php echo $section_title; ?></div>
      <div class="aci-section-links"><?php print ncn_claims_manager_aci_render_section_links($claim_id); ?></div>
      <div class="aci-section-content-wrapper"><?php print ncn_claims_manager_view_aci_render_section_content($claim_id); ?></div>
    </div>
  </div>
</div>
<?php 
  $output = ob_get_contents();
  ob_end_clean();
  return $output;
}

/**
 * Page Handler ( account/additional_claim_info/% )
 */
function ncn_claims_manager_additional_claim_info($claim_id) {
    $section_title = '';
    drupal_set_title($claim_id." - Edit Claims Processing");
    ob_start();
?>
<div id="page_loading"></div>
<div id="page_results">
    <div class="claims-detail additional-claim-info"> 
        <div class="claims-section-wrapper">
            <div class="aci-claim-info"><?php print ncn_claims_manager_aci_render_claim_info($claim_id, 'edit'); ?></div>
            <div class="title"><?php echo $section_title; ?></div>
            <div class="aci-section-links"><?php print ncn_claims_manager_aci_render_section_links($claim_id); ?></div>
            <div class="aci-section-content-wrapper"><?php print ncn_claims_manager_aci_render_section_content($claim_id); ?></div>
        </div>
    </div>
</div>
<?php 
    $output = ob_get_contents();
    ob_end_clean();
    
    return $output;
}

/**
 * Get ACI Sections
 */ 
function ncn_claims_manager_get_aci_sections() {
    return array(
        'property_owner_info' => array('title'=>'Property Owner Information', 'short_title'=>'Property Owner Info', 'render_func'=>'ncn_admin_aci_render_property_owner_info'), 
        'claim_information' => array('title'=>'Claim Information', 'render_func'=>'ncn_admin_aci_render_claim_information'), 
        'daily_claim_log' => array('title'=>'Daily Claim Log', 'render_func'=>'ncn_admin_aci_render_daily_claim_log'), 
        'insurance_claim_policy' => array('title'=>'Insurance Policy Information', 'render_func'=>'ncn_admin_aci_render_insurance_claim_policy'), 
        'comments'=> array('title' =>'Comments', 'render_func'=>'ncn_admin_aci_render_comments'), 
    );
}

/**
 * Render Claim Information (ACI)
 */ 
function ncn_claims_manager_aci_render_claim_info($claim_id, $type="edit") {
    GLOBAL $base_url;

    $output = '<div class="aci-claim-info-wrapper">';
    $output.= "<div class='aci-claim-info-item'>Claim ID: $claim_id</div>";
    
    $claim = get_claim_data($claim_id);
    $disabled_string = "";
    
    if ( ($claim['claim_status'] != "receivables") && ($claim['claim_status'] != "paid in full") ) {
        if ($claim['claim_status'] == 'approved') { 
            $disabled_string = 'disabled-photo-album'; 
        } else { 
            $disabled_string = '';  
        } 
    } else {  
        $disabled_string = 'disabled-photo-album';  
    }
    $output.= "<div class='aci-claim-info-item align-right claim-navigation' >";
    
    /*if ($type=='edit') {
        $view_url = base_path()."account/additional_claim_info_view/$claim_id";
        $output.= "<div class='switch-mode-claim-processing view-claim-processing-mode aci-claim-info-item-link'>
        <a href='$view_url'><b>VIEW</b> Claims Processing</a></div>";
    } else {
        $edit_url = base_path()."account/additional_claim_info/$claim_id";
        $output.= "<div class='switch-mode-claim-processing edit-claim-processing-mode aci-claim-info-item-link'>
        <a href='$edit_url'><b>EDIT</b> Claims Processing</a></div>";
    }*/

    $edit_album_url = "{$base_url}/account/edit_claim_inline/{$claim_id}/tab/1?width=950&height=600";
    $edit_album_class = "colorbox-node";

    $output.= "<div class='edit-album-btn aci-claim-info-item-link'>";
    $output.= "<a href='#' class='photo-album-link $disabled_string' onclick='open_edit_box({$claim_id}, true)'>Photo Album</a>";
    //$output.= "<a href='{$edit_album_url}' class='{$edit_album_class} photo-album-link $disabled_string'>Photo Album</a>";
    $output.= "</div>";
    $output.= "</div><!-- END OF .aci-claim-info-item -->";
    $output.= '</div><!-- END OF .aci-claim-info-wrapper -->';
    return $output;
}

/**
 * Render Top Section Links (under Additional Claim Info)
 */ 
function ncn_claims_manager_aci_render_section_links($claim_id) {
    $output = '<ul class="clearfix">';
    $sections = ncn_claims_manager_get_aci_sections();
    foreach ($sections as $key=>$section) {
        $class = 'aci-si-'.$key;
        
        $div_wrapper = "aci_".$key."_section";
        $link_id = "aci_link_$key";
        $output.= "<li class='aci-section-link-item $class'>";
        $output.= "<a href='$div_wrapper' id='$link_id' onclick='show_aci_section(\"$link_id\"); return false;'><span>". (isset($section['short_title'])? $section['short_title'] : $section['title']) ."</span></a>";
        $output.= '</li>';
    }
    $output.= '</ul>';
    
    drupal_add_js("
            jQuery(document).ready(function() {
                show_aci_section('aci_link_property_owner_info');
            }); ", 'inline'
    );
    
    return $output;
    
}

function ncn_claims_manager_aci_render_section_content($claim_id) {
    $old_a_data = array(
    'poi' =>  unserialize(get_ncn_data_extra($claim_id, 'aci_property_owner_info')), 
    'ci' =>   unserialize(get_ncn_data_extra($claim_id, 'aci_claim_information')), 
    'icp' =>  unserialize(get_ncn_data_extra($claim_id, 'aci_insurance_claim_policy')), 
    'comments' => unserialize(get_ncn_data_extra($claim_id, 'aci_comments')), 
  );

    if (isset($_POST['tfunction']) && $_POST['tfunction']=='additional_claims_data') {
        $b_save_log = FALSE;
        if (isset($_POST['poi'])) {
            set_ncn_data_extra($claim_id, 'aci_property_owner_info', serialize($_POST['poi']));
            $b_save_log = TRUE;
        }
        if (isset($_POST['ci'])) {
            $_POST['ci']['cause_origin_of_loss'] = rtrim($_POST['ci']['cause_origin_of_loss'], " .");
            set_ncn_data_extra($claim_id, 'aci_claim_information', serialize($_POST['ci']));
            $b_save_log = TRUE;
        }
        if (isset($_POST['icp'])) {
            set_ncn_data_extra($claim_id, 'aci_insurance_claim_policy', serialize($_POST['icp']));
            $b_save_log = TRUE;
        }
        if (isset($_POST['comments'])) {
            set_ncn_data_extra($claim_id, 'aci_comments', serialize($_POST['comments']));
            $b_save_log = TRUE;
        }
        
        if ($b_save_log) {
            ncn_admin_sync_claim_data($claim_id);      
            $additional_claim_log = ncn_admin_get_claim_log_additional_data($old_a_data, $_POST);
            $log_message = t("!user_name added additional data.", array('!user_name'=>ncn_amin_get_user_role_and_name()));
            $log_message .="\r\n".$additional_claim_log;
            ncn_admin_insert_claim_log($claim_id, date('U'), $log_message);
        }
    }
    
    ob_start();
?>
<form id='aci_addtional_claims_data_form' method='POST'>
    <input type='hidden' name='tfunction' value='additional_claims_data' />
    <div class="aci-section-content">
    <?php 
        $sections = ncn_admin_get_aci_sections();
        $i=0;
        foreach ($sections as $section => $s_info) {
            
            if ($s_info['render_func'] && function_exists($s_info['render_func'])) {
                $div_wrapper = "aci_".$section."_section";
                echo "<div id='$div_wrapper' class='aci-section'>";
                print call_user_func($s_info['render_func'], $claim_id, $section);
                echo "</div>";
            }
            $i++;
            /*if($i>1){
                break;
            }*/
            
        }
    ?>
    </div>
    <div class='aci-actions'><input type='submit' value='Save'/></div>
</form>

<?php 
    $output = ob_get_contents();
    ob_end_clean();
    return $output;
}


function ncn_claims_manager_view_aci_render_section_content($claim_id) {
  ob_start();
?>
<div id='view_aci_addtional_claims_data'>
  <div class="aci-section-content">
  <?php 
    $sections = ncn_admin_get_view_aci_sections();
    foreach ($sections as $section=>$s_info) {
      if ($s_info['render_func'] && function_exists($s_info['render_func'])) {
        $div_wrapper = "aci_".$section."_section";
        echo "<div id='$div_wrapper' class='aci-section' >";
        print call_user_func($s_info['render_func'], $claim_id, $section);
        echo "</div>";
      }
    }
  ?>
  </div>

<?php 
  $output = ob_get_contents();
  ob_end_clean();
  return $output;
}
