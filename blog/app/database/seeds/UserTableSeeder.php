<?php

class UserTableSeeder extends Seeder
{

    public function run()
    {
        //DB::table('users')->delete();
        // DB::table('groups')->delete();
        //  DB::table('users_groups')->delete();

        Sentry::getUserProvider()->create(array(
            'email' => 'tan@tan.com',
            'password' => "tan",
            'first_name' => 'tan',
            'last_name' => 'hong',
            'activated' => 1,
        ));

        Sentry::getGroupProvider()->create(array(
            'name' => 'Admin',
            'permissions' => ['admin' => 1],
        ));

        // 将用户加入用户组
        $adminUser = Sentry::getUserProvider()->findByLogin('tan@tan.com');
        $adminGroup = Sentry::getGroupProvider()->findByName('Admin');
        $adminUser->addGroup($adminGroup);
    }
}
