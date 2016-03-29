<?php

// Composer: "fzaninotto/faker": "v1.3.0"
use Faker\Factory as Faker;

class TagTableSeeder extends Seeder
{

    public function run()
    {
        $faker = Faker::create();

        foreach (range(1, 2) as $index) {
            Tag::create([
                "name" => "tag" . $index,
                "article_ids" => "1,2,3,4,5",
            ]);
        }

        foreach (range(3, 9) as $index) {
            Tag::create([
                "name" => "tag" . $index,
            ]);
        }
    }

}
