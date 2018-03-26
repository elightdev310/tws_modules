<?php
function ncn_create_claim_get_product_id_NEW($member_type, $claim_type, $claim_product)
{
    $product_id = 0;
    $claim_type = strtolower($claim_type);
    $claim_product = strtolower($claim_product);
    // Gold Member
    if ($member_type == 0) {
        if ($claim_type == 'residential') {
            if ($claim_product == 'water') {
                $product_id = 52; //$product_id = 18;
            } // RC-G-WATER
            else if ($claim_product == 'water with sketch') {
                $product_id = 80; //$product_id = 69;
            } // RC-G-WATER-WS
            else if ($claim_product == 'fire') {
                $product_id = 69; //$product_id = 58;
            } // RC-G-FIRE
            else if ($claim_product == 'fire with sketch') {
                $product_id = 82; //$product_id = 71;
            } // RC-G-FIRE-WS
            else if ($claim_product == 'fire/smoke contents cleaning') {
                $product_id = 97; //$product_id = 94;
            } // RC-G-FIRE-SCC
            else if ($claim_product == 'fire/smoke structure cleaning') {
                $product_id = 98; //$product_id = 95;
            } // RC-G-FIRE-SSC
            else if ($claim_product == 'contents cleaning') {
                $product_id = 97; //$product_id = 94;
            } // RC-G-FIRE-SCC
            else if ($claim_product == 'structure cleaning') {
                $product_id = 98; //$product_id = 95;
            } // RC-G-FIRE-SSC
            else if ($claim_product == 'mold') {
                $product_id = 71; //$product_id = 60;
            } // RC-G-MOLD
            else if ($claim_product == 'mold with sketch') {
                $product_id = 84; //$product_id = 73;
            } // RC-G-MOLD-WS
            else if ($claim_product == 'reconstruction') {
                $product_id = 66; //$product_id = 53;
            } // RC-G-RECON
            else if ($claim_product == 'reconstruction with sketch') {
                $product_id = 86; //$product_id = 75;
            } // RC-G-RECON-WS
        }
        else if ($claim_type == 'commercial') {
            if ($claim_product == 'water') {
                $product_id = 53; //$product_id = 30;
            } // CC-G-WATER
            else if ($claim_product == 'water with sketch') {
                $product_id = 81; //$product_id = 70;
            } // CC-G-WATER-WS
            else if ($claim_product == 'fire') {
                $product_id = 70; //$product_id = 59;
            } // CC-G-FIRE
            else if ($claim_product == 'fire with sketch') {
                $product_id = 83; //$product_id = 72;
            } // CC-G-FIRE-WS
            else if ($claim_product == 'fire/smoke contents cleaning') {
                $product_id = 101; //$product_id = 98;
            } // CC-G-FIRE-SCC
            else if ($claim_product == 'fire/smoke structure cleaning') {
                $product_id = 102; //$product_id = 99;
            } // CC-G-FIRE-SSC
            else if ($claim_product == 'contents cleaning') {
                $product_id = 101; //$product_id = 98;
            } // CC-G-FIRE-SCC
            else if ($claim_product == 'structure cleaning') {
                $product_id = 102; //$product_id = 99;
            } // CC-G-FIRE-SSC
            else if ($claim_product == 'mold') {
                $product_id = 72; //$product_id = 61;
            } // CC-G-MOLD
            else if ($claim_product == 'mold with sketch') {
                $product_id = 85; //$product_id = 74;
            } // CC-G-MOLD-WS
            else if ($claim_product == 'reconstruction') {
                $product_id = 73; //$product_id = 62;
            } // CC-G-RECON
            else if ($claim_product == 'reconstruction with sketch') {
                $product_id = 87; //$product_id = 76;
            } // CC-G-RECON-WS
        }
    } // CSI Member
    else if ($member_type == 5) {
        if ($claim_type == 'residential') {
            if ($claim_product == 'water') {
                $product_id = 52; //$product_id = 18;
            } // RC-S-WATER
            else if ($claim_product == 'water with sketch') {
                $product_id = 80; //$product_id = 69;
            } // RC-S-WATER-WS
            else if ($claim_product == 'fire') {
                $product_id = 69; //$product_id = 58;
            } // RC-S-FIRE
            else if ($claim_product == 'fire with sketch') {
                $product_id = 82; //$product_id = 71;
            } // RC-S-FIRE-WS
            else if ($claim_product == 'fire/smoke contents cleaning') {
                $product_id = 97; //$product_id = 94;
            } // RC-G-FIRE-SCC
            else if ($claim_product == 'fire/smoke structure cleaning') {
                $product_id = 98; //$product_id = 95;
            } // RC-G-FIRE-SSC
            else if ($claim_product == 'contents cleaning') {
                $product_id = 97; //$product_id = 94;
            } // RC-G-FIRE-SCC
            else if ($claim_product == 'structure cleaning') {
                $product_id = 98; //$product_id = 95;
            } // RC-G-FIRE-SSC
            else if ($claim_product == 'mold') {
                $product_id = 71; //$product_id = 60;
            } // RC-G-MOLD
            else if ($claim_product == 'mold with sketch') {
                $product_id = 84; //$product_id = 73;
            } // RC-G-MOLD-WS
            else if ($claim_product == 'reconstruction') {
                $product_id = 66; //$product_id = 53;
            } // RC-G-RECON
            else if ($claim_product == 'reconstruction with sketch') {
                $product_id = 86; //$product_id = 75;
            } // RC-G-RECON-WS
        }
        else if ($claim_type == 'commercial') {
            if ($claim_product == 'water') {
                $product_id = 53; //$product_id = 30;
            } // CC-G-WATER
            else if ($claim_product == 'water with sketch') {
                $product_id = 81; //$product_id = 70;
            } // CC-G-WATER-WS
            else if ($claim_product == 'fire') {
                $product_id = 70; //$product_id = 59;
            } // CC-G-FIRE
            else if ($claim_product == 'fire with sketch') {
                $product_id = 83; //$product_id = 72;
            } // CC-G-FIRE-WS
            else if ($claim_product == 'fire/smoke contents cleaning') {
                $product_id = 101; //$product_id = 98;
            } // CC-G-FIRE-SCC
            else if ($claim_product == 'fire/smoke structure cleaning') {
                $product_id = 102; //$product_id = 99;
            } // CC-G-FIRE-SSC
            else if ($claim_product == 'contents cleaning') {
                $product_id = 101; //$product_id = 98;
            } // CC-G-FIRE-SCC
            else if ($claim_product == 'structure cleaning') {
                $product_id = 102; //$product_id = 99;
            } // CC-G-FIRE-SSC
            else if ($claim_product == 'mold') {
                $product_id = 72; //$product_id = 61;
            } // CC-S-MOLD
            else if ($claim_product == 'mold with sketch') {
                $product_id = 85; //$product_id = 74;
            } // CC-S-MOLD-WS
            else if ($claim_product == 'reconstruction') {
                $product_id = 73; //$product_id = 62;
            } // CC-S-RECON
            else if ($claim_product == 'reconstruction with sketch') {
                $product_id = 87; //$product_id = 76;
            } // CC-S-RECON-WS
        }
    } // Silver Member
    else if ($member_type == 1) {
        if ($claim_type == 'residential') {
            if ($claim_product == 'water') {
                $product_id = 62; //$product_id = 45;
            } // RC-S-WATER
            else if ($claim_product == 'water with sketch') {
                $product_id = 88; //$product_id = 77;
            } // RC-S-WATER-WS
            else if ($claim_product == 'fire') {
                $product_id = 74; //$product_id = 63;
            } // RC-S-FIRE
            else if ($claim_product == 'fire with sketch') {
                $product_id = 90; //$product_id = 79;
            } // RC-S-FIRE-WS
            else if ($claim_product == 'fire/smoke contents cleaning') {
                $product_id = 99; //$product_id = 96;
            } // RC-S-FIRE-SCC
            else if ($claim_product == 'fire/smoke structure cleaning') {
                $product_id = 100; //$product_id = 97;
            } // RC-S-FIRE-SSC
            else if ($claim_product == 'contents cleaning') {
                $product_id = 99; //$product_id = 96;
            } // RC-S-FIRE-SCC
            else if ($claim_product == 'structure cleaning') {
                $product_id = 100; //$product_id = 97;
            } // RC-S-FIRE-SSC
            else if ($claim_product == 'mold') {
                $product_id = 76; //$product_id = 65;
            } // RC-S-MOLD
            else if ($claim_product == 'mold with sketch') {
                $product_id = 92; //$product_id = 81;
            } // RC-S-MOLD-WS
            else if ($claim_product == 'reconstruction') {
                $product_id = 78; //$product_id = 67;
            } // RC-S-RECON
            else if ($claim_product == 'reconstruction with sketch') {
                $product_id = 94; //$product_id = 83;
            } // RC-S-RECON-WS
        }
        else if ($claim_type == 'commercial') {
            if ($claim_product == 'water') {
                $product_id = 63; //$product_id = 46;
            } // CC-S-WATER
            else if ($claim_product == 'water with sketch') {
                $product_id = 89; //$product_id = 78;
            } // CC-S-WATER-WS
            else if ($claim_product == 'fire') {
                $product_id = 75; //$product_id = 64;
            } // CC-S-FIRE
            else if ($claim_product == 'fire with sketch') {
                $product_id = 91; //$product_id = 80;
            } // CC-S-FIRE-WS
            else if ($claim_product == 'fire/smoke contents cleaning') {
                $product_id = 103; //$product_id = 100;
            } // CC-G-FIRE-SCC
            else if ($claim_product == 'fire/smoke structure cleaning') {
                $product_id = 104; //$product_id = 101;
            } // CC-G-FIRE-SSC
            else if ($claim_product == 'contents cleaning') {
                $product_id = 103; //$product_id = 100;
            } // CC-G-FIRE-SCC
            else if ($claim_product == 'structure cleaning') {
                $product_id = 104; //$product_id = 101;
            } // CC-G-FIRE-SSC
            else if ($claim_product == 'mold') {
                $product_id = 77; //$product_id = 66;
            } // CC-S-MOLD
            else if ($claim_product == 'mold with sketch') {
                $product_id = 93; //$product_id = 82;
            } // CC-S-MOLD-WS
            else if ($claim_product == 'reconstruction') {
                $product_id = 67; //$product_id = 54;
            } // CC-S-RECON
            else if ($claim_product == 'reconstruction with sketch') {
                $product_id = 95; //$product_id = 84;
            } // CC-S-RECON-WS
        }
    }
    else if ($member_type == 2) { // Gold Lite
        if ($claim_type == 'residential') {
            if ($claim_product == 'water') {
                $product_id = 106; //$product_id = 103;
            } // RC-GL-WATER
            else if ($claim_product == 'water with sketch') {
                $product_id = 114; //$product_id = 111;
            } // RC-GL-WATER-WS
            //          else if ($claim_product == 'fire')              { $product_id = 69; //$product_id = 58; }   // RC-GL-FIRE
            //          else if ($claim_product == 'fire with sketch')  { $product_id = 82; //$product_id = 71; }   // RC-GL-FIRE-WS
            else if ($claim_product == 'fire/smoke contents cleaning') {
                $product_id = 108; //$product_id = 105;
            } // RC-GL-FIRE-SCC
            else if ($claim_product == 'fire/smoke structure cleaning') {
                $product_id = 116; //$product_id = 113;
            } // RC-GL-FIRE-SSC
            else if ($claim_product == 'contents cleaning') {
                $product_id = 108; //$product_id = 105;
            } // RC-GL-FIRE-SCC
            else if ($claim_product == 'structure cleaning') {
                $product_id = 116; //$product_id = 113;
            } // RC-GL-FIRE-SSC
            else if ($claim_product == 'mold') {
                $product_id = 110; //$product_id = 107;
            } // RC-GL-MOLD
            else if ($claim_product == 'mold with sketch') {
                $product_id = 118; //$product_id = 115;
            } // RC-GL-MOLD-WS
            else if ($claim_product == 'reconstruction') {
                $product_id = 112; //$product_id = 109;
            } // RC-GL-RECON
            else if ($claim_product == 'reconstruction with sketch') {
                $product_id = 120; //$product_id = 117;
            } // RC-GL-RECON-WS
        }
        else if ($claim_type == 'commercial') {
            if ($claim_product == 'water') {
                $product_id = 107; //$product_id = 104;
            } // CC-GL-WATER
            else if ($claim_product == 'water with sketch') {
                $product_id = 115; //$product_id = 112;
            } // CC-GL-WATER-WS
            //          else if ($claim_product == 'fire')              { $product_id = 70; //$product_id = 59; }   // CC-GL-FIRE
            //          else if ($claim_product == 'fire with sketch')  { $product_id = 83; //$product_id = 72; }   // CC-GL-FIRE-WS
            else if ($claim_product == 'fire/smoke contents cleaning') {
                $product_id = 109; //$product_id = 106;
            } // CC-GL-FIRE-SCC
            else if ($claim_product == 'fire/smoke structure cleaning') {
                $product_id = 117; //$product_id = 114;
            } // CC-GL-FIRE-SSC
            else if ($claim_product == 'contents cleaning') {
                $product_id = 109; //$product_id = 106;
            } // CC-GL-FIRE-SCC
            else if ($claim_product == 'structure cleaning') {
                $product_id = 117; //$product_id = 114;
            } // CC-GL-FIRE-SSC
            else if ($claim_product == 'mold') {
                $product_id = 111; //$product_id = 108;
            } // CC-GL-MOLD
            else if ($claim_product == 'mold with sketch') {
                $product_id = 119; //$product_id = 116;
            } // CC-GL-MOLD-WS
            else if ($claim_product == 'reconstruction') {
                $product_id = 113; //$product_id = 110;
            } // CC-GL-RECON
            else if ($claim_product == 'reconstruction with sketch') {
                $product_id = 121; //$product_id = 118;
            } // CC-GL-RECON-WS
        }
    }
    else if ($member_type == 3) { // Coach on Call
        if ($claim_type == 'residential') {
            if ($claim_product == 'water') {
                $product_id = 123; //$product_id = 120;
            } // RC-COC-WATER
            else if ($claim_product == 'water with sketch') {
                $product_id = 131; //$product_id = 128;
            } // RC-COC-WATER-WS
            //          else if ($claim_product == 'fire')              { $product_id = 69; //$product_id = 58; }   // RC-COC-FIRE
            //          else if ($claim_product == 'fire with sketch')  { $product_id = 82; //$product_id = 71; }   // RC-COC-FIRE-WS
            else if ($claim_product == 'fire/smoke contents cleaning') {
                $product_id = 125; //$product_id = 122;
            } // RC-COC-FIRE-SCC
            else if ($claim_product == 'fire/smoke structure cleaning') {
                $product_id = 133; //$product_id = 130;
            } // RC-COC-FIRE-SSC
            else if ($claim_product == 'contents cleaning') {
                $product_id = 125; //$product_id = 122;
            } // RC-COC-FIRE-SCC
            else if ($claim_product == 'structure cleaning') {
                $product_id = 133; //$product_id = 130;
            } // RC-COC-FIRE-SSC
            else if ($claim_product == 'mold') {
                $product_id = 127; //$product_id = 124;
            } // RC-COC-MOLD
            else if ($claim_product == 'mold with sketch') {
                $product_id = 135; //$product_id = 132;
            } // RC-COC-MOLD-WS
            else if ($claim_product == 'reconstruction') {
                $product_id = 129; //$product_id = 126;
            } // RC-COC-RECON
            else if ($claim_product == 'reconstruction with sketch') {
                $product_id = 137; //$product_id = 134;
            } // RC-COC-RECON-WS
        }
        else if ($claim_type == 'commercial') {
            if ($claim_product == 'water') {
                $product_id = 124; //$product_id = 121;
            } // CC-COC-WATER
            else if ($claim_product == 'water with sketch') {
                $product_id = 132; //$product_id = 129;
            } // CC-COC-WATER-WS
            //          else if ($claim_product == 'fire')              { $product_id = 70; //$product_id = 59;}    // CC-COC-FIRE
            //          else if ($claim_product == 'fire with sketch')  { $product_id = 72; }   // CC-COC-FIRE-WS
            else if ($claim_product == 'fire/smoke contents cleaning') {
                $product_id = 126; //$product_id = 123;
            } // CC-COC-FIRE-SCC
            else if ($claim_product == 'fire/smoke structure cleaning') {
                $product_id = 134; //$product_id = 131;
            } // CC-COC-FIRE-SSC
            else if ($claim_product == 'contents cleaning') {
                $product_id = 126; //$product_id = 123;
            } // CC-COC-FIRE-SCC
            else if ($claim_product == 'structure cleaning') {
                $product_id = 134; //$product_id = 131;
            } // CC-COC-FIRE-SSC
            else if ($claim_product == 'mold') {
                $product_id = 128; //$product_id = 125;
            } // CC-COC-MOLD
            else if ($claim_product == 'mold with sketch') {
                $product_id = 136; //$product_id = 133;
            } // CC-COC-MOLD-WS
            else if ($claim_product == 'reconstruction') {
                $product_id = 130; //$product_id = 127;
            } // CC-COC-RECON
            else if ($claim_product == 'reconstruction with sketch') {
                $product_id = 138; //$product_id = 135;
            } // CC-COC-RECON-WS
        }
    }
    else if ($member_type == 4) { // Gold Coach
        if ($claim_type == 'residential') {
            if ($claim_product == 'water') {
                $product_id = 141; //$product_id = 138;
            } // RC-GC-WATER
            else if ($claim_product == 'water with sketch') {
                $product_id = 149; //$product_id = 146;
            } // RC-GC-WATER-WS
            //          else if ($claim_product == 'fire')              { $product_id = 69; //$product_id = 58; }   // RC-GC-FIRE
            //          else if ($claim_product == 'fire with sketch')  { $product_id = 82; //$product_id = 71; }   // RC-GC-FIRE-WS
            else if ($claim_product == 'fire/smoke contents cleaning') {
                $product_id = 143; //$product_id = 140;
            } // RC-GC-FIRE-SCC
            else if ($claim_product == 'fire/smoke structure cleaning') {
                $product_id = 151; //$product_id = 148;
            } // RC-GC-FIRE-SSC
            else if ($claim_product == 'contents cleaning') {
                $product_id = 143; //$product_id = 140;
            } // RC-GC-FIRE-SCC
            else if ($claim_product == 'structure cleaning') {
                $product_id = 151; //$product_id = 148;
            } // RC-GC-FIRE-SSC
            else if ($claim_product == 'mold') {
                $product_id = 145; //$product_id = 142;
            } // RC-GC-MOLD
            else if ($claim_product == 'mold with sketch') {
                $product_id = 153; //$product_id = 150;
            } // RC-GC-MOLD-WS
            else if ($claim_product == 'reconstruction') {
                $product_id = 147; //$product_id = 144;
            } // RC-GC-RECON
            else if ($claim_product == 'reconstruction with sketch') {
                $product_id = 155; //$product_id = 152;
            } // RC-GC-RECON-WS
        }
        else if ($claim_type == 'commercial') {
            if ($claim_product == 'water') {
                $product_id = 142; //$product_id = 139;
            } // CC-GC-WATER
            else if ($claim_product == 'water with sketch') {
                $product_id = 150; //$product_id = 147;
            } // CC-GC-WATER-WS
            //          else if ($claim_product == 'fire')              { $product_id = 70; //$product_id = 59; }   // CC-GC-FIRE
            //          else if ($claim_product == 'fire with sketch')  { $product_id = 83; //$product_id = 72; }   // CC-GC-FIRE-WS
            else if ($claim_product == 'fire/smoke contents cleaning') {
                $product_id = 144; //$product_id = 141;
            } // CC-GC-FIRE-SCC
            else if ($claim_product == 'fire/smoke structure cleaning') {
                $product_id = 152; //$product_id = 149;
            } // CC-GC-FIRE-SSC
            else if ($claim_product == 'contents cleaning') {
                $product_id = 144; //$product_id = 141;
            } // CC-GC-FIRE-SCC
            else if ($claim_product == 'structure cleaning') {
                $product_id = 152; //$product_id = 149;
            } // CC-GC-FIRE-SSC
            else if ($claim_product == 'mold') {
                $product_id = 146; //$product_id = 143;
            } // CC-GC-MOLD
            else if ($claim_product == 'mold with sketch') {
                $product_id = 154; //$product_id = 151;
            } // CC-GC-MOLD-WS
            else if ($claim_product == 'reconstruction') {
                $product_id = 148; //$product_id = 145;
            } // CC-GC-RECON
            else if ($claim_product == 'reconstruction with sketch') {
                $product_id = 156; //$product_id = 153;
            } // CC-GC-RECON-WS
        }
    }
    return $product_id;
}
