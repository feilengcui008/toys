<?php

// Composer: "fzaninotto/faker": "v1.3.0"
use Faker\Factory as Faker;

class CommentTableSeeder extends Seeder
{

    public function run()
    {
        $faker = Faker::create();

        foreach (range(1, 5) as $index) {
            Comment::create([
                'username' => $faker->name(),
                'email' => $faker->email(),
                'content' => $faker->text(),
                'article_id' => 1,

            ]);
        }

        foreach (range(1, 5) as $index) {
            Comment::create([
                'username' => $faker->name(),
                'email' => $faker->email(),
                'content' => $faker->text(),
                'article_id' => 2,

            ]);
        }
    }

}