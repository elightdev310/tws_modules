<?php
//------------------------------------------------------------------------------
// see "ncn_message.module"
function ncn_admin_messages_markread()
{
    return ncn_message_markread();
}

//------------------------------------------------------------------------------
// see "ncn_message.module"
function ncn_admin_messages()
{
    return ncn_message_center();
}

//------------------------------------------------------------------------------
function ncn_admin_member_id_ranges()
{
    // check for access denied
    if (isset($GLOBALS['user']->roles[6]))
    {
        return("Access denied.");
    }


    // handle input
    if (isset($_POST['tfunction']))
    {
        $error = false;
    
        if ((!is_numeric($_POST['start'])) || (!is_numeric($_POST['end'])) )
        {
            drupal_set_message('Values must be numeric', 'error');
            $error = true;
        }
        else
        if ($_POST['start'] >= $_POST['end'])
        {
            drupal_set_message('End must be higher than start', 'error');
            $error = true;
        }
        
        // no errors
        if ($error == false)
        {
            // add range
        //  $query = "DELETE FROM member_id_ranges WHERE distributor = ".$_POST['distributor'];
            $result = db_query('DELETE FROM {member_id_ranges} WHERE distributor = :a',array(':a'=>$_POST['distributor']));

        //  $query = "INSERT INTO member_id_ranges VALUES(".$_POST['distributor'].", ".$_POST['start'].", ".$_POST['end'].");";
            $result = db_query('INSERT INTO {member_id_ranges} VALUES(:a,:b,:c)',
                array(':a'=>$_POST['distributor'],':b'=>$_POST['start'],':c'=>$_POST['end']));
            
            drupal_set_message('Range applied to distributor', 'status');
        }
    }
    

    // draw the page
    ob_start();

    ?>
    <fieldset>
    <legend>Add/Update range</legend>
    
    <form method="POST">
    <input type="hidden" name="tfunction" value="update_range">
        <table>
            <tr>
                <td>Distributor</td>
                <td><?php distributor_dropdown(); ?></td>
            </tr>
            <tr>
                <td>Start</td>
                <td><input type="text" name="start" value="<?=(isset($_POST['start'])? $_POST['start'] : ''); ?>">
            </tr>
            <tr>
                <td>End</td>
                <td><input type="text" name="end" value="<?=(isset($_POST['end'])? $_POST['end'] : ''); ?>">
            </tr>
            <tr>
                <td colspan=2><input type="submit" value="Add/Update Range"></td>
            </tr>
        </table>
    
    </form>
    
    </fieldset>
    <?php
    
    // show existing ranges
    ?>
    <fieldset>
    <legend>Existing Ranges</legend>
    
        <table class="sticky-enabled tableSelect-processed sticky-table">
        <thead class="tableHeader-processed">
            <th>Distributor</th>
            <th>Start Range</th>
            <th>End Range</th>
        </thead>
        <?php
        
    //  $query = "SELECT * FROM member_id_ranges";
        $result = db_query('SELECT * FROM {member_id_ranges}');
        foreach($result as $row)
        {
            $row = (array)$row;
            $user = user_load($row['distributor']);

            if (isset($user->name))
            {           
            
                ?>
                <tr>
                    <td><?= $user->profile_legalname; ?></td>
                    <td><?= $row['start']; ?></td>
                    <td><?= $row['end']; ?></td>
                </tr>
                <?php
            }
            
        }
        
        
        ?>
        </table>
    
    </fieldset>
    <?php

    // return
    $content = ob_get_contents();
    ob_end_clean();
    return $content;
}

//------------------------------------------------------------------------------
function distributor_dropdown($select_id = 0)
{
    if (is_distributor($GLOBALS['user']) == true)
    {
        // draw hidden input
        ?>
        <input type="hidden" name="distributor" value="<?= $GLOBALS['user']->uid; ?>">
        <?php   
    }
    /*else
    if  (is_account_manager($GLOBALS['user']) == true)
    {
        // get owner
        $query = "SELECT * FROM users_parent WHERE uid=".$GLOBALS['user']->uid;
        $result = db_query($query);

        foreach($result as $row)
        {
            $row = array($row);
            ?>
            <input type="hidden" name="distributor"  value="<?= $row['parent']; ?>">
            <?php   
        }
        
        // darw hidden input
    }*/
    else
    {
        $dists = get_distributor_list('active');
        ?>
        <select name="distributor">
        <?php
        if ($select_id) {
            $distributor = user_load($select_id);
            if (!$distributor) {
                print '<option value="'.$select_id.'" SELECTED >'.'User #'.$select_id.' (deleted)</option>';
            } elseif (!$distributor->status && is_distributor($distributor)) {
                print '<option value="'.$select_id.'" SELECTED >'.(isset($distributor->profile_legalname) && trim($distributor->profile_legalname)!='' ? $distributor->profile_legalname : $distributor->name ).' (blocked)</option>';
            }
        }

        foreach($dists as $row)
        {
            $user = user_load($row['uid']);
            //if (is_distributor($user)==true)
            {
                $selected = "";
                if ( $select_id == $row['uid']) { $selected = "selected"; }
                print '<option value="'.$row['uid'].'" '. $selected.' >'.(isset($user->profile_legalname) && trim($user->profile_legalname)!='' ? $user->profile_legalname : $user->name ).'</option>';
            }
        }
        ?>
        </select>
        <?php
    }


}

/* Added by sanjay  Lead User Dropdown list  */
/* Updated by kevin at Sep 5, 2017 */

function leaduser_dropdown($select_id = 0)
{
        // run through users
        $query = "SELECT usr_lead.*, usr.* FROM users_lead as usr_lead INNER JOIN users as usr  WHERE usr.uid = usr_lead.user_id AND usr.status != '0'";
        $result = db_query($query);
        ?>
        <select name="leaduser">
        <?php       
        foreach($result as $row)
        {
            $row = (array)$row;
            $selected = "";
            if ( $select_id == $row['uid']) { $selected = "selected"; }
            print '<option value="'.$row['uid'].'" '. $selected.' >'.$row['company_name'].'</a>';
        }
        ?>
        </select>
        <?php
}



function associate_dropdown($select_id = 0)
{
    if (is_associate($GLOBALS['user']) == true)
    {
        // draw hidden input
        ?>
        <input type="hidden" name="associate" value="<?= $GLOBALS['user']->uid; ?>">
        <?php   
    }
    /*else
    if  (is_account_manager($GLOBALS['user']) == true)
    {
        // get owner
        $query = "SELECT * FROM users_parent WHERE uid=".$GLOBALS['user']->uid;
        $result = db_query($query);
        
        foreach($result as $row)
        {
            $row = (array)$row;
            ?>
            <input type="hidden" name="distributor"  value="<?= $row['parent']; ?>">
            <?php   
        }
        
        // darw hidden input
    }*/
    else
    {
        $associates = get_associate_list('active');
        
        ?>
        <select name="associate">
        <?php
        $associate = user_load($select_id);
        if (!$associate) {
            print '<option value="'.$select_id.'" SELECTED >'.'User #'.$select_id.' (*deleted)</option>';
        } elseif (!$associate->status && is_associate($associate)) {
            print '<option value="'.$select_id.'" SELECTED >'.(isset($associate->profile_legalname) && trim($associate->profile_legalname)!='' ? $associate->profile_legalname : $associate->name ).' (blocked)</option>';
        }
        foreach($associates as $row)
        {
            $user = user_load($row['uid']);
            
            //if (is_associate($user)==true)
            {
                $selected = "";
                if ( $select_id == $row['uid']) { $selected ="selected"; }
                print '<option value="'.$row['uid'].'" '. $selected.' >'.(isset($user->profile_legalname) && trim($user->profile_legalname)!='' ? $user->profile_legalname : $user->name).'</option>';
            }
        }
        
        ?>
        </select>
        <?php
    }


}

//------------------------------------------------------------------------------
function account_manager_dropdown()
{
    // run through users
    $am_list = get_account_manager_list('active');
    ?>
    <select name="account_manager">
    <?php
    
    foreach($am_list as $row)
    {
        $user = user_load($row['uid']);
        
        //if (is_account_manager($user)==true)
        {
            print '<option value="'.$row['uid'].'">'.$user->profile_firstname.' '.$user->profile_lastname.'</a>';
        }
    }
    
    ?>
    </select>
    <?php
}

function ncn_admin_table_pagniation($url, $total_count, $num_per_page, $page, $limit=10) {
    $num_page = ceil($total_count/$num_per_page);
    if ($num_page==1) return '';
    
    $content = "<div class='item-list'>";
    $content .= "<ul class='pager'>";

    if ($page>0) {
        $content .= "<li class='pager-first'><a href='$url'>&laquo; first</a></li>";
    }
    if ($limit<$total_count) {
        $first = ($page-(int)($limit/2)); 
        if ($first<0) {$first = 0;}
        $last = $first+$limit;
        if ($last > $num_page ) {
            $last = $num_page;
            $first = $last - $limit;
            if ($first<0) {$first = 0;}
        }
        for ($index = $first; $index<$last; $index++) {
            if ($index == $page) {
                $content .= "<li class='pager-current'>".($index+1)."</li>";
            } else {
                $content .= "<li class='pager-item'><a href='$url?page=$index'>".($index+1)."</a></li>";
            }
        }
    } else {
        for ($index = 0; $index<$num_page; $index++) {
            if ($index == $page) {
                $content .= "<li class='pager-current'>".($index+1)."</li>";
            } else {
                $content .= "<li class='pager-item'><a href='$url?page=$index'>".($index+1)."</a></li>";
            }
        }
    }
    if ($page<($num_page-1)) {
        $content .= "<li class='pager-last'><a href='$url?page=".($num_page-1)."'>last &raquo;</a></li>";
    }
    $content .= "</ul>";
    $content .= "</div>";
    
    return $content;
}
