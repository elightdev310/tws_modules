<?php
GLOBAL $base_url;
?>

<div id="ncn_admin_account_manager_list">

    <form id="account_manager_auto_assign_form" method="POST">

        <input type="hidden" name="tfunction" value="auto_assign" />

        <table class="sticky-enabled tableSelect-processed sticky-table">

            <thead class="tableHeader-processed">

            <th>UID</th>

            <th>Username</th>

            <th>Status</th>

            <th>Email</th>

            <th>Auto-Assignable</th>

            <th># of Assigned Members </th>

            <th></th>

            </thead>

            <tbody>

            <?php
            $am_list = get_account_manager_list();
           // var_dump($am_list);
            $index = 0;
            foreach($am_list as $am):
                $_user_am = user_load($am['uid']);
                $auto_assign = 0;
                $n_members = 0;
                $query = db_query('SELECT * FROM am_auto_assign WHERE am_uid=:a',array(':a'=>$am['uid']));
                $result = $query;

                if ($result->rowCount() > 0)
                {
                    $row = $result->fetchAssoc();
                    $auto_assign = $row['auto_assign'];
                    $n_members = $row['members'];
                }

                $index++;
                $class = ($index % 2) ? 'odd' : 'even';
                ?>

                <tr class="<?php
                echo $class; ?>">

                    <td><?php
                        echo $_user_am->uid
                        ?></td>

                    <td><?php
                        echo $_user_am->profile_firstname . ' ' . $_user_am->profile_lastname; ?></td>

                    <td><?php
                        echo ($_user_am->status) ? 'active' : 'blocked'; ?></td>

                    <td><?php
                        echo $_user_am->mail; ?></td>

                    <td><input type="checkbox" name="p_auto_assign[<?php
                        echo $_user_am->uid
                        ?>]" <?php
                        if ($auto_assign) echo 'checked'; ?> /></td>

                    <td><?php
                        echo $n_members; ?></td>

                    <td>

                        <?php
                        if ($n_members): ?>
                            <a href='<?php echo $base_url; ?>/admin/config/ncn_create_user/account_manager_list/view/<?php echo $_user_am->uid; ?>'>View</a>
<!--                        <a href="--><?php
//                        echo $base_url; ?><!--/admin/config/ncn_create_user/account_manager_list/view/--><?php
//                        echo $_user_am->uid
//                        ?><!--">View</a> --><?php
                        endif; ?>

                    </td>

                </tr>

            <?php
            endforeach; ?>



            </tbody>

        </table>



        <div class="form-submit-panel">

            <input type="submit" value="Save" />

        </div>

    </form>

</div>

