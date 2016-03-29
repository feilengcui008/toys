<?php

// Composer: "fzaninotto/faker": "v1.3.0"
use Faker\Factory as Faker;

class ArticleTableSeeder extends Seeder
{

    public function run()
    {
        $faker = Faker::create();

        foreach (range(1, 5) as $index) {
            Article::create([
                "title" => $faker->name(),
                "description" => $faker->name(),
                "content" => $faker->text(),
                "click_num" => 0,
                "tags" => "1,2",
            ]);
        }
    }

}
