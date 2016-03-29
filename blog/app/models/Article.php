<?php

class Article extends \Eloquent
{
    protected $fillable = [];

    public function comments()
    {
        return $this->hasMany('Comment', 'article_id');
    }

    public function users()
    {
        return $this->belongsToMany('User');
    }

}