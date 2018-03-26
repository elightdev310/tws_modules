<?php
/**
 * AR Details Panel
 *
 * @var: $claim_id, $claim, $member_user, $aci_poi, $aci_icp
 */
$ars_file_status_info = ncn_ars_file_status();

$invoice_file = ncn_get_claim_invoice_info($claim_id, 1);   // Final Invoice file

$arr_claimant = explode(' ', $aci_poi['insured_name']);
$claimant_firstname = isset($arr_claimant[0])? $arr_claimant[0]:'';
$claimant_lastname  = isset($arr_claimant[1])? $arr_claimant[1]:'';

$arr_adjuster = explode(' ', $aci_icp['claim_adjuster']);
$adjuster_firstname = isset($arr_adjuster[0])? $arr_adjuster[0]:'';
$adjuster_lastname  = isset($arr_adjuster[1])? $arr_adjuster[1]:'';

$ars_pr = unserialize(get_ncn_data_extra($claim_id, 'ars_peer_review'));
if (empty($ars_pr)) { $ars_pr = array(); }
$ars_hoa= unserialize(get_ncn_data_extra($claim_id, 'ars_hoa'));
if (empty($ars_hoa)) { $ars_hoa = array(); }
?>

<div class="ar-details-panel">
    <div class="sub-section">
        <div class="row">
            <div class="form-group col-sm-6">
                <label>Account Manager<a class="about-icon" href="#">about</a></label>
                <div class="form-value"><?php echo get_account_manager_name($member_user->uid); ?></div>
            </div>
            <div class="form-group col-sm-6">
                <label>ARS Number<a class="about-icon" href="#">about</a></label>
                <div class="form-value"><?php print $claim_id; ?></div>
            </div>
        </div>
        <div class="row">
            <div class="form-group col-sm-6">
                <label>Claim Number<a class="about-icon" href="#">about</a></label>
                <div class="form-value"><?php print $claim_id; ?></div>
            </div>
            <div class="form-group col-sm-6">
                <label>Member Name<a class="about-icon" href="#">about</a></label>
                <div class="form-value"><?php echo $member_user->profile_legalname; ?></div>
            </div>
        </div>
        <div class="row">
            <div class="form-group col-sm-6">
                <label>File Status<a class="about-icon" href="#">about</a></label>
                <div class="form-value">
                    <?php echo $ars_file_status_info[$claim['ars_file_status']]; ?>
                </div>
            </div>
            <div class="form-group col-sm-6">
                <label>Primary Contact<a class="about-icon" href="#">about</a></label>
                <div class="form-value"><?php echo user_fullname($member_user); ?></div>
            </div>
        </div>
        <div class="row">
            <div class="form-group col-sm-12">
                <label>File Last Updated<a class="about-icon" href="#">about</a></label>
                <div class="form-value"><?php echo strTime($invoice_file['timestamp']); ?></div>
            </div>
        </div>
        <div class="row">
            <div class="form-group col-sm-12">
                <label>NCN Comparative<a class="about-icon" href="#">about</a></label>
                <div class="form-value"></div>
            </div>
        </div>
        <div class="row">
            <div class="form-group col-sm-12">
                <label>Undisputed Amount<a class="about-icon" href="#">about</a></label>
                <div class="form-value"></div>
            </div>
        </div>
    </div>

    <div class="sub-section">
        <h4>Negotiation Details</h4>
        <div class="row">
            <div class="form-group col-sm-6">
                <label>Amount Agreed Upon by Member<a class="about-icon" href="#">about</a></label>
                <div class="form-value"><?php echo '$' . number_format($claim['payment_received'], 2); ?></div>
            </div>
            <div class="form-group col-sm-6">
                <label>Difference Original vs Agreed<a class="about-icon" href="#">about</a></label>
                <div class="form-value">
                    <?php echo '$' . number_format($claim['claim_amount']-$claim['payment_received'], 2); ?>
                    &nbsp;
                    <span>( =<?php echo '$' . number_format($claim['claim_amount'],2).' - '.'$' . number_format($claim['payment_received'],2); ?> )</span>
                </div>
            </div>
        </div>
    </div>

    <div class="sub-section">
        <h4>Claimant Information</h4>
        <div class="row">
            <div class="form-group col-sm-6">
                <label>Claimant First Name<a class="about-icon" href="#">about</a></label>
                <div class="form-value">
                    <?php echo $claimant_firstname; ?>
                </div>
            </div>
            <div class="form-group col-sm-6">
                <label>Claimant Last Name<a class="about-icon" href="#">about</a></label>
                <div class="form-value">
                    <?php echo $claimant_lastname; ?>
                </div>
            </div>
        </div>
    </div>

    <div class="sub-section">
        <h4>Property Information</h4>
        <div class="row">
            <div class="form-group col-sm-6">
                <label>Property Address<a class="about-icon" href="#">about</a></label>
                <div class="form-value"><?php echo isset($aci_poi['insured_address'])?$aci_poi['insured_address']:''; ?> </div>
            </div>
            <div class="form-group col-sm-6">
                <label>Property City<a class="about-icon" href="#">about</a></label>
                <div class="form-value"><?php echo isset($aci_poi['insured_city'])?$aci_poi['insured_city']:''; ?></div>
            </div>
        </div>
        <div class="row">
            <div class="form-group col-sm-6">
                <label>Property State<a class="about-icon" href="#">about</a></label>
                <div class="form-value"><?php echo isset($aci_poi['insured_state'])?$aci_poi['insured_state']:''; ?></div>
            </div>
            <div class="form-group col-sm-6">
                <label>Property Zip Code<a class="about-icon" href="#">about</a></label>
                <div class="form-value"><?php echo isset($aci_poi['insured_zip'])?$aci_poi['insured_zip']:''; ?></div>
            </div>
        </div>
    </div>

    <div class="sub-section">
        <h4>Carrier Information</h4>
        <div class="row">
            <div class="form-group col-sm-6">
                <label>Carrier Name<a class="about-icon" href="#">about</a></label>
                <div class="form-value">
                    <?php echo isset($aci_icp['insured_name'])?$aci_icp['insured_name']:''; ?>
                </div>
            </div>
            <div class="form-group col-sm-6">
                <label>Carrier Phone Number<a class="about-icon" href="#">about</a></label>
                <div class="form-value">
                    <?php if (!empty($aci_icp['insurance_company_phone']['number']) && !empty($aci_icp['insurance_company_phone']['ext']) ): ?>
                        <?php echo isset($aci_icp['insurance_company_phone']['number'])?$aci_icp['insurance_company_phone']['number']:''; ?>
                        <?php echo isset($aci_icp['insurance_company_phone']['ext'])?' ext '.$aci_icp['insurance_company_phone']['ext']:''; ?>
                    <?php endif; ?>
                </div>
            </div>
        </div>
        <div class="row">
            <div class="form-group col-sm-6">
                <label>Carrier Policy Number<a class="about-icon" href="#">about</a></label>
                <div class="form-value"><?php echo isset($aci_icp['policy_number'])?$aci_icp['policy_number']:''; ?></div>
            </div>
            <div class="form-group col-sm-6">
                <label>Carrier Fax Number<a class="about-icon" href="#">about</a></label>
                <div class="form-value"><?php echo isset($aci_icp['insurance_fax'])?$aci_icp['insurance_fax']:''; ?></div>
            </div>
        </div>
        <div class="row">
            <div class="form-group col-sm-6">
                <label>Carrier Claim Number<a class="about-icon" href="#">about</a></label>
                <div class="form-value"><?php echo isset($aci_icp['insurance_claim_number'])?$aci_icp['insurance_claim_number']:''; ?></div>
            </div>
            <div class="form-group col-sm-6">
                <label>Carrier Email Address<a class="about-icon" href="#">about</a></label>
                <div class="form-value"><?php echo isset($aci_icp['adjuster_email'])?$aci_icp['adjuster_email']:''; ?></div>
            </div>
        </div>
    </div>

    <div class="sub-section">
        <h4>Adjuster Information</h4>
        <div class="row">
            <div class="form-group col-sm-6">
                <label>Adjuster First Name<a class="about-icon" href="#">about</a></label>
                <div class="form-value"><?php echo $adjuster_firstname; ?></div>
            </div>
            <div class="form-group col-sm-6">
                <label>Adjuster Last Name<a class="about-icon" href="#">about</a></label>
                <div class="form-value"><?php echo $adjuster_lastname; ?></div>
            </div>
        </div>
    </div>

    <div class="sub-section">
        <h4>Secondary Adjuster</h4>
        <div class="row">
            <div class="form-group col-sm-6">
                <label>Secondary Adjuster First Name<a class="about-icon" href="#">about</a></label>
                <div class="form-value"></div>
            </div>
            <div class="form-group col-sm-6">
                <label>Secondary Adjuster Last Name<a class="about-icon" href="#">about</a></label>
                <div class="form-value"></div>
            </div>
        </div>
    </div>

    <div class="sub-section">
        <h4>Peer Review</h4>
        <div class="form-group">
            <label>Peer Review Company<a class="about-icon" href="#">about</a></label>
            <div class="form-value"><?php echo $ars_pr['company']; ?></div>
        </div>
        <div class="form-group">
            <label>Peer Review  Contact First Name<a class="about-icon" href="#">about</a></label>
            <div class="form-value"><?php echo $ars_pr['first_name']; ?></div>
        </div>
        <div class="form-group">
            <label>Peer Review Contact Last Name<a class="about-icon" href="#">about</a></label>
            <div class="form-value"><?php echo $ars_pr['last_name']; ?></div>
        </div>
    </div>

    <div class="sub-section">
        <h4>HOA Information</h4>
        <div class="form-group">
            <label>HOA Name<a class="about-icon" href="#">about</a></label>
            <div class="form-value"><?php echo $ars_hoa['name']; ?></div>
        </div>
        <div class="form-group">
            <label>HOA Contact First Name<a class="about-icon" href="#">about</a></label>
            <div class="form-value"><?php echo $ars_hoa['first_name']; ?></div>
        </div>
        <div class="form-group">
            <label>HOA Contact Last Name<a class="about-icon" href="#">about</a></label>
            <div class="form-value"><?php echo $ars_hoa['last_name']; ?></div>
        </div>
    </div>

    <div class="sub-section">
        <h4>Important Dates</h4>
        <div class="row">
            <div class="form-group col-sm-6">
                <label>Date File Open<a class="about-icon" href="#">about</a></label>
                <div class="form-value"></div>
            </div>
            <div class="form-group col-sm-6">
                <label>Date Submitted to Carrier<a class="about-icon" href="#">about</a></label>
                <div class="form-value"></div>
            </div>
        </div>
        <div class="row">
            <div class="form-group col-sm-6">
                <label>Date Sent to Client for Review<a class="about-icon" href="#">about</a></label>
                <div class="form-value"></div>
            </div>
            <div class="form-group col-sm-6">
                <label>Date Carrier Confirmed Receipt<a class="about-icon" href="#">about</a></label>
                <div class="form-value"></div>
            </div>
        </div>
        <div class="row">
            <div class="form-group col-sm-6">
                <label>Date Approved for Submission<a class="about-icon" href="#">about</a></label>
                <div class="form-value"></div>
            </div>
            <div class="form-group col-sm-6">
                <label>Date Negotiation Completed<a class="about-icon" href="#">about</a></label>
                <div class="form-value"></div>
            </div>
        </div>
        <div class="row">
            <div class="form-group col-sm-12">
                <label>Date File Closed<a class="about-icon" href="#">about</a></label>
                <div class="form-value"></div>
            </div>
        </div>
    </div>

</div>
