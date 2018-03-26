<?php

/**
 * Page Handler (ce_portal/review/claim/%/room/%/scopesheet)
 */
function ncn_ceportal_review_claim_room_scopesheet($claim_id, $room_name) {
    $title = t("ScopeSheet (claim #!claim_id - !room_name)", array('!claim_id'=>$claim_id, '!room_name'=>$room_name));
    drupal_set_title($title);
    ob_start();
?>
<div id="page_results">
    <div id="ce_review_scopesheet" class="claims-detail ce-review-page"> 
        <div class="claims-section-wrapper">
            <div class="clearfix">
                <div class="title raw-title"><?php echo $title; ?></div>
                <!-- <div class="help"><a href="#">Help</a></div> -->
                <div class="sub-page-tabs clearfix"><?php print ncn_ceportal_review_render_sub_page_links($claim_id, 'scopesheet', $room_name); ?></div>
                <div class="sub-page-wrapper">
                    <div class="sub-page-content">
                        <div class="aci-section-links ss-section-links"><?php print ncn_claims_manager_scopesheet_render_section_links($claim_id, $room_name); ?></div>
                        <div class="aci-section-content-wrapper ss-section-content-wrapper"><?php print ncn_ceportal_scopesheet_render_section_content($claim_id, $room_name); ?></div>
                    </div>
                </div>
            </div>          
        </div>
    </div>
</div>
<?php
    $output = ob_get_contents();
    ob_end_clean();
    return $output;
}

function ncn_ceportal_scopesheet_render_section_content($claim_id, $room_name) {
    ob_start();
?>
    <div class="aci-section-content ss-section-content">
    <?php 
        $sections = ncn_ceportal_get_scopesheet_section_views();
        foreach ($sections as $section=>$s_info) {
            if ($s_info['render_func'] && function_exists($s_info['render_func'])) {
                $div_wrapper = "scopesheet_".$section."_section";
                echo "<div id='$div_wrapper' class='aci-section ss-section' >";
                print call_user_func($s_info['render_func'], $claim_id, $room_name, $section);
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

function ncn_ceportal_get_scopesheet_section_views() {
    return array(
        'room_dimensions'                               => array('title'=>'Room dimensions', 'render_func'=>'ncn_admin_ss_render_view_room_dimensions' ), 
        'exterior_interior_temperatures'    => array('title'=>'Exterior & Interior Temperatures', 'render_func'=>'ncn_admin_ss_render_view_ext_inter_temperatures'), 
        'structural_moisture_reading'       => array('title'=>'Structural Moisture Readings', 'render_func'=>'ncn_admin_ss_render_view_structural_moisture_readings'), 
        'equipment_placement'                           => array('title'=>'Equipment Placement', 'render_func'=>'ncn_admin_ss_render_view_equipment_placement'), 
        'services'                                              => array('title'=>'Services', 'render_func'=>'ncn_admin_ss_render_view_services'), 
        'comments'                                              => array('title'=>'Comments', 'render_func'=>'ncn_admin_ss_render_view_comments'), 
    );
}
?>
