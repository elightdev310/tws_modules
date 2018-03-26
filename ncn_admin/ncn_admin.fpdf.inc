<?php
/**
 * Instead of fpdf/pdf.php.
 */
require_once('sites/all/libraries/PDFMerger/fpdf/image_alpha.php');

class NCNPDF extends PDF_ImageAlpha
{
//Page header
/*function Header()
{
    //Logo
    //$this->Image('logo_pb.png',10,8,33);
    //Arial bold 15
    $this->SetFont('Arial','B',15);
    //Move to the right
    $this->Cell(80);
    //Title
    $this->Cell(30,10,'Title',1,0,'C');
    //Line break
    $this->Ln(20);
}*/

function Header_Invoice($str_header) {
    $this->SetDrawColor(82,123,175);
    $this->SetFont('Arial','B',12);
    $this->Cell(50,10,$str_header);
    $this->Ln(5);
    $this->SetLineWidth(0.5);
    $this->Line(10, 20, 200, 20);
    $this->Ln(20);
}

function get_defraged_array($data, $count) {
    $darray = array();
    for ($i=0; $i<$count; $i++) {
        $img_index = 'image'.$i;
        if (isset($data[$img_index])&&is_file($data[$img_index]['path'])) {
            $darray[] = $i;
        }
    }
    return $darray;
}

function Photo_Table_Invoice($data) {
    //Colors, line width and bold font
    $this->SetFillColor(255,0,0);
    $this->SetTextColor(255);
//    $this->SetDrawColor(128,0,0);
    $this->SetLineWidth(.3);
    $this->SetFont('','B', '10');
    //Header
    $w=array(62,62,62);
    
    //Color and font restoration
    $this->SetFillColor(224,235,255);
    $this->SetTextColor(82,123,175);
    $this->SetFont('');
    //Data
    $fill=false;
    $base_relate_url = "";
    $left_margin = 10;
    
    $darray = $this->get_defraged_array($data, 3);
    
    $top = 25;
    
    for ($i=0; $i<count($darray); $i++) {
        $img_index = 'image'.$darray[$i];
        $this->Image(drupal_realpath($data[$img_index]['path']), $left_margin+62*$i,     $top, 55, 35);
    }
    
    $top += 45;
    
    for ($i=0; $i<count($darray); $i++) {
        $index = $darray[$i];
        $img_index = 'image'.$index;

        $this->Text($left_margin+62*$i, $top, $data['captions'][$index]);
    }

    $this->SetTextColor(0);
}

function RoomPhoto_Table_Invoice($data) {
    //Colors, line width and bold font
    $this->SetFillColor(255,0,0);
    $this->SetTextColor(255);
//    $this->SetDrawColor(128,0,0);
    $this->SetLineWidth(.3);
    $this->SetFont('','B', '10');
    //Header
    $w=array(62,62,62);
    
    //Color and font restoration
    $this->SetFillColor(224,235,255);
    $this->SetTextColor(82,123,175);
    $this->SetFont('');
    //Data
    $fill=false;
    $base_relate_url = "";
    $left_margin = 10;
    $top0 = 25;
    
    $darray = $this->get_defraged_array($data, 6);
    
    /*if ($count_image > 3) {
        $count_image = 3;
    }*/
    $index = 0;
    for ($i=0; $i<3; $i++) {
        $img_index = 'image'.$i;
        if (isset($data[$img_index])&&is_file($data[$img_index]['path'])) {
            $this->Image(drupal_realpath($data[$img_index]['path']), $left_margin+62*$index,     $top0, 55, 35);
            $index++;
        }
        
    }
    $index = 0;
    for ($i=0; $i<3; $i++) {
        $img_index = 'image'.$i;
        if (isset($data[$img_index])&&is_file($data[$img_index]['path'])) {
            $this->Text($left_margin+62*$index, $top0+45, $data['captions'][$i]);
            $index++;
        }
    }
    
    if ($index==0) {
        $top1 = $top0;
    } else {
        $top1 = 95;
    } 
    
    $index = 0;
    for ($i=3; $i<6; $i++) {
        $img_index = 'image'.$i;
        if (isset($data[$img_index])&&is_file($data[$img_index]['path'])) {
            $this->Image(drupal_realpath($data[$img_index]['path']), $left_margin+62*$index,     $top1, 55, 35);
            $index++;
        }
    }
    
    $index = 0;
    for ($i=3; $i<6; $i++) {
        $img_index = 'image'.$i;
        if (isset($data[$img_index])&&is_file($data[$img_index]['path'])) {
            $this->Text($left_margin+62*$index, $top1+45, $data['captions'][$i]);
            $index++;
        }
    }
    ///////////////////////////////////////////
    if ($index>0) {
        $top1 += 45;
    }
    
    $this->SetTextColor(0);
    
    return ($top1+10);
}

function draw_scope_file($data, $top2) {
    $left_margin = 10;
    if(is_file($data['filepath'])) {
        $this->Ln();
        $this->SetXY($left_margin, $top2);
        //$this->Cell(190, 5, "Dry Log - ".$data['roomname'], 0, 0, 'C', false);
        $this->Cell(190, 5, $data['roomname'], 0, 0, 'C', false);
        $this->Ln();
        $this->Image(drupal_realpath($data['filepath']), $left_margin,  $top2+10, 190, 120);
    }
}

function get_format_phone_string($str) {
    if (trim($str) == "") {
        return '';
    }
    $str = str_replace('-', '', $str);
    $str1 = substr($str, 0, 3);
    $str2 = substr($str, 3, 3);
    $str3 = substr($str, 6);
    $str = "$str1-$str2-$str3";
    
    return $str;
}

function Make_CoverSheet($data) {
    // ** Header **
    $this->SetDrawColor(82,123,175);
    $this->SetFont('Arial','B',11);
    $this->Cell(100,10, $data['company_name']);
    $this->Cell(100,10, 'Tax ID Number '.$data['tax_id_number']);
    $this->Ln(5);
    $this->SetLineWidth(0.5);
    $this->Line(10, 20, 200, 20);
    $this->Ln(20);
    
    $this->SetFont('Arial','',11);
    // ** Company Address **
    $left_margin = 10; $left1 = 100;
    $this->Text($left_margin, 30, $data['company_address1']);       $this->Text($left1, 30, 'Office: '.$this->get_format_phone_string($data['company_office_phone']));
    $this->Text($left_margin, 35, $data['company_address2']);       $this->Text($left1, 35, 'Fax: '.$this->get_format_phone_string($data['company_fax_number']));
    
    // **
    $this->Text($left_margin, 45, 'Estimator: Account Manager');
    // ** Data Entered
    $this->Text($left_margin, 55, 'Data Entered: '.$data['data_entered']);
    // ** Price List
    $this->Text($left_margin, 65, 'Price List: '.$data['price_list']);
    // ** Client
    $this->Text($left_margin, 80, 'Client: ');  $this->Text($left_margin+20, 80, $data['client_name']); $this->Text($left1, 80, 'Home phone: '.$this->get_format_phone_string($data['home_phone']));
    $this->Text($left_margin, 85, 'Property: ');$this->Text($left_margin+20, 85, $data['property1']);   $this->Text($left1, 85, $data['iop_type'].': '.$this->get_format_phone_string($data['iop_number']));
                                                $this->Text($left_margin+20, 90, $data['property2']);
    // ** Type of Loss **
    $this->Text($left_margin, 105, 'Type of Loss: '.$data['type_of_loss']);                                             
    // ** Date of Loss **
    $this->Text($left_margin, 115, 'Date of Loss: '.$data['date_of_loss']);
    // ** Insurance Company **
    $this->Text($left_margin, 125, 'Insurance Company: '.$data['insurance_company']);
    // ** Claim Number **
    $this->Text($left_margin, 135, 'Claim Number: '.$data['claim_number']);
    // ** Insurance Company **
    $this->Text($left_margin, 145, 'Claim Adjuster: '.$data['claim_adjuster']); $this->Text($left1, 145, 'Phone: '.$this->get_format_phone_string($data['claim_adjuster_phone']));
                                                                                $this->Text($left1, 150, 'Fax: '.$this->get_format_phone_string($data['claim_adjuster_fax']));                                                                          
    $this->SetXY($left_margin, 170);
    // ** Water Damage Service Invoice:
    $this->SetFont('Arial','B',12);
    $this->Cell(100, 10, 'Water Damage Service Invoice:');
    $this->Ln();
    $this->SetFont('Arial','',9);
    $content1 = 'The following is our invoice for the services provided on '.$data['start_date'].' at the above referenced location due to a Category '.$data['category'].' water loss event. ';
    $content1.= 'The cause and origin of the loss appears to be '.$data['cause'].'.  Due to the documented elevated moisture content, structural drying services were provided. ';
    $content1.= 'Drying equipment was placed on '.$data['start_date'].' and removed '.$data['completion_date'].' once a dry standard was achieved and documented.';
    $this->MultiCell(190, 5, $content1);
    
    $this->Ln(10);
    $this->SetFont('Arial','B',9);
    $this->Cell(100, 10, 'Notice of Assignment of Benefits Clause:');
    $this->Ln();
    $content2 = 'Please note, the attached executed services contract includes an Assignment of Benefits Clause. Any payment for goods and services for this invoice that are issued to or received by any party other than our company will be considered a breach of contract for non-payment and will be treated as such.';
    $this->MultiCell(190, 5, $content2);
    
}

//Page footer
function Footer()
{
    /*
    //Position at 1.5 cm from bottom
    $this->SetY(-15);
    //Arial italic 8
    $this->SetFont('Arial','I',8);
    //Page number
    $this->Cell(0,10,'Page '.$this->PageNo().'/{nb}',0,0,'C');
    */
}
var $widths;
var $aligns;

function SetWidths($w)
{
    //Set the array of column widths
    $this->widths=$w;
}

function SetAligns($a)
{
    //Set the array of column alignments
    $this->aligns=$a;
}

function Row($data)
{
    //Calculate the height of the row
    $nb=0;
    for($i=0;$i<count($data);$i++)
        $nb=max($nb, $this->NbLines($this->widths[$i], $data[$i]));
    $h=5*$nb;
    //Issue a page break first if needed
    $this->CheckPageBreak($h);
    //Draw the cells of the row
    for($i=0;$i<count($data);$i++)
    {
        $w=$this->widths[$i];
        $a=isset($this->aligns[$i]) ? $this->aligns[$i] : 'L';
        //Save the current position
        $x=$this->GetX();
        $y=$this->GetY();
        //Draw the border
        $this->Rect($x, $y, $w, $h);
        //Print the text
        $this->MultiCell($w, 5, $data[$i], 0, $a);
        //Put the position to the right of the cell
        $this->SetXY($x+$w, $y);
    }
    //Go to the next line
    $this->Ln($h);
}

function CheckPageBreak($h)
{
    //If the height h would cause an overflow, add a new page immediately
    if($this->GetY()+$h>$this->PageBreakTrigger)
        $this->AddPage($this->CurOrientation);
}

function NbLines($w, $txt)
{
    //Computes the number of lines a MultiCell of width w will take
    $cw=&$this->CurrentFont['cw'];
    if($w==0)
        $w=$this->w-$this->rMargin-$this->x;
    $wmax=($w-2*$this->cMargin)*1000/$this->FontSize;
    $s=str_replace("\r", '', $txt);
    $nb=strlen($s);
    if($nb>0 and $s[$nb-1]=="\n")
        $nb--;
    $sep=-1;
    $i=0;
    $j=0;
    $l=0;
    $nl=1;
    while($i<$nb)
    {
        $c=$s[$i];
        if($c=="\n")
        {
            $i++;
            $sep=-1;
            $j=$i;
            $l=0;
            $nl++;
            continue;
        }
        if($c==' ')
            $sep=$i;
        $l+=$cw[$c];
        if($l>$wmax)
        {
            if($sep==-1)
            {
                if($i==$j)
                    $i++;
            }
            else
                $i=$sep+1;
            $sep=-1;
            $j=$i;
            $l=0;
            $nl++;
        }
        else
            $i++;
    }
    return $nl;
}

function InsertScopesheetData($data) {
    $this->SetFont('Arial', '', 9); 
    $this->SetDrawColor(0, 0, 0);
    
    /*** Exterior & Interior Temperatures ***/
    if (isset($data['eit']) && $data['eit']) {
        $this->SetWidths(array(25, 40, 40, 35, 35));
        $this->Row( array('Date', 'Exterior Temp / RH%', 'Interior Temp / RH%', 'GPP', 'Time of Readings') );
        foreach ($data['eit'] as $key=>$item) {
            $this->Row( array(
                    $item['date'],
                    $item['ext_temp']."/".$item['ext_rh'], 
                    $item['int_temp']."/".$item['int_rh'], 
                    $item['gpp'], 
                    ncn_admin_format_american_time($item['time_readings']).' '.$item['time_readings_half'], 
            ));
        }
        
        $this->Ln(6);
    }
    
    /*** Structural Moisture Readings ***/
    if (isset($data['smr_standard']) && $data['smr_standard']) {
        $x=$this->GetX();
        $y=$this->GetY();
        $this->SetXY($x+25, $y);
        //Print the text
        $this->Cell(48, 5, "Wall Dry Standard: ".$data['smr_standard']['walldry'], 0);
        $this->Cell(60, 5, "Floor Dry Standard: ".$data['smr_standard']['floordry'], 0);
        $this->Cell(60, 5, "Ceiling Dry Standard: ".$data['smr_standard']['ceilingdry'], 0);
        $this->Ln(6);
    }
    
    if (isset($data['smr']) && $data['smr']) {
        $this->SetWidths(array(25, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12));
        $this->Row( array('Date', 'Wall 1', 'Wall 2', 'Wall 3', 'Wall 4', 'Area 1', 'Area 2', 'Area 3', 'Area 4', 'Center', 'Area 1', 'Area 2', 'Area 3', 'Area 4', 'Center') );
        foreach ($data['smr'] as $key=>$item) {
            $this->Row( array(
                $item['date'], 
                $item['wall'][0],       $item['wall'][1],       $item['wall'][2],       $item['wall'][3], 
                $item['area_fd'][0],    $item['area_fd'][1],    $item['area_fd'][2],    $item['area_fd'][3],    $item['area_fd']['center'], 
                $item['area_cd'][0],    $item['area_cd'][1],    $item['area_cd'][2],    $item['area_cd'][3],    $item['area_cd']['center'], 
            ));
        }
        $this->Ln(6);
    }
    
    if (isset($data['ep']) && $data['ep']) {
        $this->SetWidths(array(40, 120));
        foreach ($data['ep'] as $key=>$item) {
            $this->Row( array(
                    $item['ptype'], 
                    $item['data'] 
            ));
        }
    }
}

}
?>
