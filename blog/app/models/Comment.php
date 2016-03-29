<?php

class Comment extends \Eloquent
{
    protected $guarded = [];

    public function article()
    {
        return $this->belongsTo('Article', 'article_id');
    }
}