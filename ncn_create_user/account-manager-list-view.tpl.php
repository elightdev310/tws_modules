<?php
    $a = $result;
//    var_dump($a);
            GLOBAL $base_url;
            $_user = user_load($a);
            drupal_set_title(t('Members List (AM: !am_name)', array(
                '!am_name' => $_user->profile_firstname . ' ' . $_user->profile_lastname
            )));
?>
<div id="ncn_admin_am_view_members_page">

    <table class="sticky-enabled tableSelect-processed sticky-table">

        <thead class="tableHeader-processed">

        <th>Member ID</th>

        <th>Username</th>

        <th>Status</th>

        <th>Member Type</th>

        <th></th>

        </thead>

        <tbody>

        <?php
        $query = db_query('SELECT * FROM member_id_pool WHERE am_uid=:a',array(':a'=>$a));
       // var_dump($query);
     //   $result = $query;
        $r_count = $query->rowCount();
       // var_dump($r_count);
        for ($i = 0; $i < $r_count; $i++):
          //  $member = (array)$result;
           // var_dump($result);
            $member = $query->fetchAssoc();
            $uid = get_uid_from_memberid($member['member_id']);
            $status = '';
            if ($uid == 0)
            {
                $status = 'unactivated';
            }
            else
            {
                $_user = user_load($uid);
                $status = ($_user->status) ? 'active' : 'blocked';
            }

            $class = ($i % 2) ? 'even' : 'odd';
            if ($member['member_type'] == 0)
            {
                $str_member_type = "Gold";
            }
            else
                if ($member['member_type'] == 1)
                {
                    $str_member_type = "Silver";
                }
                else
                    if ($member['member_type'] == 2)
                    {
                        $str_member_type = "Gold Lite";
                    }
                    else
                        if ($member['member_type'] == 3)
                        {
                            $str_member_type = "Coach on Call";
                        }
                        else
                            if ($member['member_type'] == 4)
                            {
                                $str_member_type = "Gold Coach";
                            }
                            else
                                if ($member['member_type'] == 5)
                                {
                                    $str_member_type = "CSI";
                                }

            ?>

            <tr class="<?php
            echo $class; ?>">

                <td><?php
                    echo $member['member_id'] ?></td>

                <td><?php
                    echo $member['first_name'] . ' ' . $member['last_name']; ?></td>

                <td><?php
                    echo $status; ?></td>

                <td><?php
                    echo $str_member_type; ?></td>

                <td><?php
                    if ($uid): ?>

                        <a href="<?php
                        echo $base_url; ?>/admin/config/ncn_create_user/all_user_list/edit_user/<?php
                        echo $uid; ?>">Edit</a>

                    <?php
                    endif; ?>

                </td>

            </tr>

        <?php
        endfor; ?>



        </tbody>

    </table>

</div>
